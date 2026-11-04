#include "stdafx.h"
#include "sync.h"
#include "Defines.h"
#include "natives.h"

int& animation_task_count = *reinterpret_cast<int*>(0x82BF9AA0);
short& weapon_anim_task_count = *reinterpret_cast<short*>(0x82BF9AB0);

Detour<int> CDummyTask_SerializeWeaponThrow_detour;
int CDummyTask_SerializeWeaponThrow(void* _this, SYNC_SERIALIZE type, CMessage* message)
{
	int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset;

	if (type == SERIALIZE_READ)
	{
		//short anim_index = static_cast<short>(message->PeekDWORD(16, seek_bits));
		unsigned short anim_index = static_cast<short>(message->PeekDWORD(16, seek_bits));

		//if (anim_index < 0 || anim_index > weapon_anim_task_count)
		if (anim_index > weapon_anim_task_count)
		{
			printf("[CARTEL::Dummy Task] - Invalid Weapon Throw Animation Index!\n");
			attacker_list.push_back(std::make_tuple(INVALID_PLAYER_INDEX, ATTACK_SYNC_INVALID_WEAPON_THROW, 0));
			return CDummyTask_SerializeWeaponThrow_detour.CallOriginal(_this, SERIALIZE_SKIP, message);
		}
	}

	return CDummyTask_SerializeWeaponThrow_detour.CallOriginal(_this, type, message);
}

Detour<int> CNetObjPhysical_SerializeAttachmentDetour;
int CNetObjPhysical_SerializeAttachment(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message) //Only runs with non peds
{
	int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset;
	CNetworkObject* net_ped = NetworkInterface::GetLocalPlayerPed()->GetNetworkObject();

	if (net_ped && type == SERIALIZE_READ && obj->IsGlobalFlagSet(GLOBAL_FLAG_SCRIPT_OBJECT))
	{
		bool attachment = message->PeekBool(seek_bits); //Read if we got an attachment
		if (attachment)
		{
			WORD net_id = message->PeekShort(seek_bits + 1); //Read network id for what obj is being attached to
			if (net_id == net_ped->GetNetworkID()) //if network id is our network id we are being attached to
			{
				CNetGamePlayer* owner = obj->GetPlayerOwner(); //if player gets kicked this will print wrongly - happened to me /Nath
				if (owner->IsPhysical())
					printf("[CARTEL::Attachment] - %s tried to attach something to you!\n", owner->GetPlayerInfo()->GetPlayerName());

				if (obj->GetObjectType() == NET_OBJ_TYPE_OBJECT)
				{
					int object_index;
					GET_OBJECT_FROM_NETWORK_ID(obj->GetNetworkID(), &object_index);
					SET_OBJECT_COLLISION(object_index, false);
					SET_OBJECT_LIGHTS(object_index, false);
				}

				attacker_list.push_back(std::make_tuple(owner->IsPhysical() ? owner->GetPeerID() : INVALID_PLAYER_INDEX, ATTACK_SYNC_ATTACHMENT, obj->GetNetworkID()));
				return CNetObjPhysical_SerializeAttachmentDetour.CallOriginal(obj, SERIALIZE_SKIP, message);
			}
		}
	}

	return CNetObjPhysical_SerializeAttachmentDetour.CallOriginal(obj, type, message);
}

Detour<int> CNetObjPed_SerializeMovementAnimTaskDetour;
int CNetObjPed_SerializeMovementAnimTask(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message)
{
	int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset;
	if (type == SERIALIZE_READ)
	{
		int anim_id = static_cast<int>(message->PeekDWORD(16, seek_bits)); //read some id
		if ((anim_id > animation_task_count || anim_id < 0) && anim_id != -1) //id is out side of the bounds
		{
			if (obj->GetPlayerOwner()->IsPhysical())
			{
				printf("[CARTEL::Ped Animation Task] - %s tried to set animation task out of bounds! [%i]\n", obj->GetPlayerOwner()->GetPlayerInfo()->GetPlayerName(), anim_id);
				attacker_list.push_back(std::make_tuple(obj->GetPlayerOwner()->GetPeerID(), ATTACK_SYNC_ANIM_TASK, obj->GetNetworkID()));
			}
			
			return CNetObjPed_SerializeMovementAnimTaskDetour.CallOriginal(obj, SERIALIZE_SKIP, message);
		}
	}
	
	return CNetObjPed_SerializeMovementAnimTaskDetour.CallOriginal(obj, type, message);
}

Detour<int> CNetObjPed_SerializeAIDataDetour;
int CNetObjPed_SerializeAIData(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message)
{
	int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset;
	if (type == SERIALIZE_READ && obj->IsGlobalFlagSet(GLOBAL_FLAG_SCRIPT_OBJECT))
	{
		int relationship_id = static_cast<int>(message->PeekDWORD(6, seek_bits));
		if (relationship_id > 55 || relationship_id < 0)
		{
			if (obj->GetPlayerOwner()->IsPhysical())
			{
				printf("[CARTEL::Ped AI Data] - %s tried to set relationship group out of bounds! [%i]\n", obj->GetPlayerOwner()->GetPlayerInfo()->GetPlayerName(), relationship_id);
				attacker_list.push_back(std::make_tuple(obj->GetPlayerOwner()->GetPeerID(), ATTACK_SYNC_RELATIONSHIP_GROUP, obj->GetNetworkID()));
			}

			return CNetObjPed_SerializeAIDataDetour.CallOriginal(obj, SERIALIZE_SKIP, message);
		}
	}

	return CNetObjPed_SerializeAIDataDetour.CallOriginal(obj, type, message);
}

Detour<void*> CTaskQuery_AllocateDetour;
void* CTaskQuery_Allocate(int task_type)
{
	*reinterpret_cast<DWORD*>(0x823E6E60) = 0x60000000; //NOP writing value to task (crashes because of nullptr if pool is full)

	void* task = CTaskQuery_AllocateDetour.CallOriginal(task_type);

	if (task == nullptr)
	{
		printf("[CARTEL::Task Query] - We ran out of pool slots!\n");
		attacker_list.push_back(std::make_tuple(INVALID_PLAYER_INDEX, ATTACK_SYNC_TASK_QUERY, NULL));
		return nullptr;
	}

	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(task) + 4) = task_type; //Write task_type if we got task pointer
	return task;
}

Detour<BOOL> CNetCloneTaskSimplePlayerAimProjectile_CreateLocalTaskDetour;
BOOL CNetCloneTaskSimplePlayerAimProjectile_CreateLocalTask(void* _this, CPed* ped)
{
	CWeaponItem* item = ped->GetInventory()->GetCurrentWeapon();
	if (item && item->m_WeaponIndex == WEAPON_ROCKET)
	{
		if (ped->IsPlayer())
		{
			if (ped->GetPlayerInfo())
			{
				printf("[CARTEL::Aim Projectile Task] - %s tried to throw a rocket!\n", ped->GetPlayerInfo()->GetPlayerName());
				attacker_list.push_back(std::make_tuple(ped->GetPlayerInfo()->m_PeerID, ATTACK_SYNC_FREEZE_GUN, NULL));
			}
		}
		else
		{
			if (ped->GetNetworkObject())
			{
				CNetGamePlayer* player = ped->GetNetworkObject()->GetPlayerOwner();
				if (player->IsPhysical())
				{
					printf("[CARTEL::Aim Projectile Task] - %s's ped tried to throw a rocket!\n", player->GetPlayerInfo()->GetPlayerName());
					attacker_list.push_back(std::make_tuple(player->GetPeerID(), ATTACK_SYNC_FREEZE_GUN, ped->GetNetworkObject()->GetNetworkID()));
				}
			}
		}
	}

	return CNetCloneTaskSimplePlayerAimProjectile_CreateLocalTaskDetour.CallOriginal(_this, ped);
}

Detour<int> CNetObjPed_SerializeGameStateDataDetour;
int CNetObjPed_SerializeGameStateData(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message)
{
	//Lets call other Serialize Gamestate Data
	int ret = ((int(*)(CNetworkObject*, SYNC_SERIALIZE, CMessage*))0x82720298)(obj, type, message);
	*reinterpret_cast<DWORD*>(0x8270DB88) = 0x38600000 | ret; //li %r3, ret

	int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset + 1 + 2 + 7;
	if (type == SERIALIZE_READ)
	{
		WEAPON_TYPE weapon_index = static_cast<WEAPON_TYPE>(message->PeekDWORD(6, seek_bits));
		if (weapon_index >= WEAPON_ANYWEAPON || weapon_index < WEAPON_UNARMED)
		{
			if (obj->GetPlayerOwner()->IsPhysical())
			{
				printf("[CARTEL::Ped Game State Data] - %s tried to set out of bounds weapon index! [%i]\n", obj->GetPlayerOwner()->GetPlayerInfo()->GetPlayerName(), weapon_index);
				attacker_list.push_back(std::make_tuple(obj->GetPlayerOwner()->GetPeerID(), ATTACK_SYNC_WEAPON_INDEX, obj->GetNetworkID()));
			}

			return CNetObjPed_SerializeGameStateDataDetour.CallOriginal(obj, SERIALIZE_SKIP, message);
		}

		if (obj->IsGlobalFlagSet(GLOBAL_FLAG_SCRIPT_OBJECT))
		{
			seek_bits += 6 + 1;
			seek_bits += weapon_index == WEAPON_OBJECT ? 12 : 1;
			seek_bits += message->PeekBool(seek_bits) ? 13 : 1;
			seek_bits += message->PeekBool(seek_bits) ? 16 : 1;
			seek_bits += 2 + 13 + 8;

			int animation_group = static_cast<int>(message->PeekDWORD(16, seek_bits));
			if (animation_group >= animation_task_count || animation_group < 0)
			{
				if (obj->GetPlayerOwner()->IsPhysical())
				{
					printf("[CARTEL::Ped Game State Data] - %s tried to set out of bounds default animation group index! [%i]\n", obj->GetPlayerOwner()->GetPlayerInfo()->GetPlayerName(), animation_group);
					attacker_list.push_back(std::make_tuple(obj->GetPlayerOwner()->GetPeerID(), ATTACK_SYNC_ANIMATION_GROUP, obj->GetNetworkID()));
				}

				return CNetObjPed_SerializeGameStateDataDetour.CallOriginal(obj, SERIALIZE_SKIP, message);
			}
		}
	}

	return CNetObjPed_SerializeGameStateDataDetour.CallOriginal(obj, type, message);
}

Detour<int> CNetObjPed_SerializeAttachmentDetour;
int CNetObjPed_SerializeAttachment(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message) //Only runs with peds
{
	int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset;
	CNetworkObject* net_ped = NetworkInterface::GetLocalPlayerPed()->GetNetworkObject();

	if (net_ped && type == SERIALIZE_READ && obj->IsGlobalFlagSet(GLOBAL_FLAG_SCRIPT_OBJECT))
	{
		bool attachment = message->PeekBool(seek_bits); //Read if we got an attachment
		if (attachment)
		{
			WORD net_id = message->PeekShort(seek_bits + 1); //Read network id for what obj is being attached to
			if (net_id == net_ped->GetNetworkID()) //if network id is our network id we are being attached to
			{
				CNetGamePlayer* owner = obj->GetPlayerOwner();
				if (owner->IsPhysical())
					printf("[CARTEL::Attachment] - %s tried to attach something to you!\n", owner->GetPlayerInfo()->GetPlayerName());

				attacker_list.push_back(std::make_tuple(owner->IsPhysical() ? owner->GetPeerID() : INVALID_PLAYER_INDEX, ATTACK_SYNC_ATTACHMENT, obj->GetNetworkID()));
				return CNetObjPed_SerializeAttachmentDetour.CallOriginal(obj, SERIALIZE_SKIP, message);
			}
		}
	}

	return CNetObjPed_SerializeAttachmentDetour.CallOriginal(obj, type, message);
}

Detour<int> CNetObjPlayer_SerializeAppearanceDataDetour;
int CNetObjPlayer_SerializeAppearanceData(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message, int unused)
{
	float distance = FLT_MAX;
	int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset;

	if (type == SERIALIZE_READ)
	{
		DWORD model_hash = message->PeekDWORD(32, seek_bits);
		if (!IS_THIS_MODEL_A_PED(model_hash) || model_hash == PED_PLAYER || model_hash == PED_SUPERLOD)
		{
			if (obj->GetPlayerOwner()->IsPhysical())
			{
				CEntity* our_ent = NetworkInterface::GetLocalPlayerPed()->m_Vehicle ? NetworkInterface::GetLocalPlayerPed()->m_Vehicle : dynamic_cast<CEntity*>(NetworkInterface::GetLocalPlayerPed());
				if (our_ent && obj->GetEntity())
				{
					GET_DISTANCE_BETWEEN_COORDS_3D(obj->GetEntity()->GetPosition()[0], obj->GetEntity()->GetPosition()[1], obj->GetEntity()->GetPosition()[2], our_ent->GetPosition()[0], our_ent->GetPosition()[1], our_ent->GetPosition()[2], &distance);

					if (distance < 70.0f)
					{
						printf("[CARTEL::Player Apperance Data] - %s tried to set model to crash model! [0x%X]\n", obj->GetPlayerOwner()->GetPlayerInfo()->GetPlayerName(), model_hash);
						attacker_list.push_back(std::make_tuple(obj->GetPlayerOwner()->GetPeerID(), ATTACK_SYNC_MODEL_CHANGE, model_hash));
					}
				}
			}

			return CNetObjPlayer_SerializeAppearanceDataDetour.CallOriginal(obj, SERIALIZE_SKIP, message, unused);
		}
	}

	return CNetObjPlayer_SerializeAppearanceDataDetour.CallOriginal(obj, type, message, unused);
}