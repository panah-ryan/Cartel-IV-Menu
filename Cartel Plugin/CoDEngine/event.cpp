#include "stdafx.h"
#include "Defines.h"
#include "natives.h"
#include "caller.h"
#include "Detour.h"

struct explosion_timer
{
	DWORD tick[2];
	int count[2];
};

DWORD should_process_event_bitset;
explosion_timer timer[16];

void CTL_GET_EVENT_BITSET(scrThread_Info* cxt)
{
	cxt->ResultPtr[0].Any = reinterpret_cast<DWORD>(&should_process_event_bitset);
}

bool ReturnEventSuccess() //Nullsub to replace our Handle and Decide virtual calls
{
	return true;
}

//CWeaponDamageEvent_HandleDetour.SetupDetour(0x826D2180, CWeaponDamageEvent_Handle);
/*
Detour<void> CWeaponDamageEvent_HandleDetour;
void CWeaponDamageEvent_Handle(void* classPointer, CMessage* message, int peer)
{
	CWeaponDamageEvent_HandleDetour.CallOriginal(classPointer, message, peer);
}
*/
/* CWeaponDamage
* Handle
*	- Type 2 bits
*	- Weapon Type 6 bits
*	- Has Damage Override 1 bit
*	if HAS DAMAGE OVERRIDE
*		- Damage Override 8 or 10 bits (Depends on WEAPON TYPE)
*	if TYPE EQUALS 3
*		- Has Damage Time 1 bit
*		if DAMAGE TIME
*			- Damage Time 32 bits
*	else
*		- Parent Object 12 bits
*		- Hit Object 12 bits
*	if TYPE EQUALS 2
*		- Component 5 bits
*	else
*		- Local Impact X 13 bits
*		- Local Impact Y 13 bits
*		- Local Impact Z 13 bits
*		if TYPE EQUALS 1
*			- Has Damaged Vehicle 1 bit
*			if DAMAGED VEHICLE
*				- Tires Damaged 3 bits
*				- Suspension Damaged 3 bits
* Reply
*	- Player Health 13 bits*/
//You can fix this bug by checking in the CWeaponDamageEvent is 0x1E (parent object) is NULL and if so check if 0x1C (type) is not 3 

/* two skip conditions..
CWeaponDamageEvent is 0x1E (parent object) is NULL and if so check if 0x1C (type) is not 3
and
 if (type) is 3 check if 0x54 (Damage Time) is 0.


 //you can just hook CWeaponDamageEvent::Handle and then read the values from the offsets I provided
*/
#ifdef Moderator
Detour<int> CWeaponDamageEvent_DecideDetour;
int CWeaponDamageEvent_Decide(char* classPointer, byte peerid)
{
	short parent_object = *reinterpret_cast<short*>(classPointer + 0x1E);
	if (parent_object == 0)
	{
		//Crow returns 0 here LUL. //Wonder what it breaks.
		byte type = *reinterpret_cast<byte*>(classPointer + 0x1C);
		DWORD damage_time = *reinterpret_cast<DWORD*>(classPointer + 0x54);
		if (type != 3 || damage_time == 0)
		{
			printf("[CARTEL::Event - WeaponDamage] %s tried to send invalid event data!\n", NetworkInterface::GetPlayerInfo(peerid)->GetPlayerName());
			return 0;
		}
	}
	return CWeaponDamageEvent_DecideDetour.CallOriginal(classPointer, peerid);
}


/*CROWS PATCH

	Detour<bool> CWeaponDamageEvent_DecideDetour;
	bool CWeaponDamageEvent_DecideHook(DWORD r3, byte bPeerId) {

		_CNetworkPeer* pPlayer = Utils::GetPlayerInformation(bPeerId);
		if (*(short*)(r3 + 0x1E) == 0 && pPlayer) {
			ProtectionNotificationWithName("Invalid Damage Event", pPlayer->szGamertag);

			return true;
		}

		return CWeaponDamageEvent_DecideDetour.CallOriginal(r3, bPeerId);
	}*/
#endif
Detour<void> netEventMgr_HandleEventDetour;
void netEventMgr_HandleEvent(netEventMgr* manager, CNetworkEvent* pEvent, CMessage* message, int peer, short messageSeq, int eventId)
{
	NetGameEventTypes type = pEvent->m_EventType;
	DWORD* vftable = *reinterpret_cast<DWORD**>(pEvent);
	int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset;
	bool skip_processing = false;
	static DWORD modified_vftable[24];

	if (((1 << type) & should_process_event_bitset) != 0) //Event should be processed so lets do our checks
	{
		switch (type)
		{
		case REQUEST_PICKUP_EVENT: 
		{
			int pickup_index = static_cast<int>(message->PeekDWORD(11, seek_bits));
			int player_index = static_cast<int>(message->PeekDWORD(5, seek_bits + 11));

			if (pickup_index > 1514 || pickup_index < 0 || player_index > 15 || player_index < 0) //Pickup index or player index is out of bounds
			{
				attacker_list.push_back(std::make_tuple(peer, ATTACK_EVENT_REQUEST_PICKUP, NULL));
				skip_processing = true;
			}

			break;
		}
		case RESURRECT_PLAYER_EVENT: 
		{
			int player_index = static_cast<int>(message->PeekDWORD(8, seek_bits));

			if (player_index > 15 || player_index < 0) //Player index is out of bounds
			{
				attacker_list.push_back(std::make_tuple(peer, ATTACK_EVENT_RESURRECT, NULL));
				skip_processing = true;
			}

			break;
		}
		case REMOVE_WEAPON_EVENT: 
		case REMOVE_ALL_WEAPONS_EVENT: 
		{
			short net_id = static_cast<short>(message->PeekShort(seek_bits));

			CNetworkObject* net_obj = NetworkInterface::GetObjectManager()->GetNetworkObjectFromID(net_id, false);
			if (net_obj == nullptr) //Invalid network id
			{
				attacker_list.push_back(std::make_tuple(peer, ATTACK_EVENT_REMOVE_WEAPON, NULL));
				skip_processing = true;
			}

			break;
		}
		case REQUEST_EXPLOSION_EVENT:
		case START_EXPLOSION_EVENT: 
		{
			float pos[3];
			int our_char = NULL;
			float dist = FLT_MAX;
			CPlayerPed* our_ped = NetworkInterface::GetLocalPlayerPed();

			EXPLOSION_TAG explosion_type = static_cast<EXPLOSION_TAG>(message->PeekSignedInt(6, seek_bits + 12 + 12));
			message->PeekVector3(19, seek_bits + 12 + 12 + 6 + 32, pos);
			bool ship_destroy = explosion_type == EXPLOSION_SHIP_DESTROY;

			DWORD time = GetTickCount() - timer[peer].tick[ship_destroy];
			if (time > 60000)
			{
				timer[peer].tick[ship_destroy] = 0;
				timer[peer].count[ship_destroy] = 0;
			}
			//printf("our_ped == %i, explosion_type == %i, ship_destroy == %i\n", our_ped, explosion_type, ship_destroy);
			if (our_ped)
			{
				CEntity* our_ent = our_ped->m_Vehicle ? our_ped->m_Vehicle : dynamic_cast<CEntity*>(our_ped); 
				float mx, my, mz;
				int myPed;
				GET_PLAYER_CHAR(GET_PLAYER_ID(), &myPed);
				GET_CHAR_COORDINATES(myPed, &mx, &my, &mz);
				//printf("myPed == %i, our_ent == %i", myPed, our_ent); //these are never the same
				GET_DISTANCE_BETWEEN_COORDS_3D(pos[0], pos[1], pos[2], mx, my, mz, &dist);
				//GET_DISTANCE_BETWEEN_COORDS_3D(pos[0], pos[1], pos[2], our_ent->GetPosition()[0], our_ent->GetPosition()[1], our_ent->GetPosition()[2], &dist);

				//printf("distance == %f, ship_destroy == %i\n", dist, ship_destroy);
				if (ship_destroy && dist < 60.0f || !ship_destroy && dist < 45.0f)
				{
					timer[peer].count[ship_destroy]++;
					timer[peer].tick[ship_destroy] = GetTickCount();
				}
			}

			if (timer[peer].count[0] > 40 || timer[peer].count[1] > 2) //Skip after more than 2 explosions in range of 60.0f all within a minute
			{
				attacker_list.push_back(std::make_tuple(peer, ATTACK_EVENT_EXPLOSION, NULL));
				skip_processing = true;
			}

			break;
		}
		case ALTER_WANTED_LEVEL_EVENT: 
		{
			int player_index = static_cast<int>(message->PeekDWORD(5, seek_bits));

			if (player_index > 15 || player_index < 0) //Player index is out of bounds
			{
				attacker_list.push_back(std::make_tuple(peer, ATTACK_EVENT_WANTED_LEVEL, NULL));
				skip_processing = true;
			}

			break;
		}
		case CREATE_PICKUP_EVENT: 
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits + 57);

			if (IS_THIS_MODEL_A_PED(model_hash) || IS_THIS_MODEL_A_VEHICLE(model_hash)) //Pickup model is a ped or vehicle
			{
				attacker_list.push_back(std::make_tuple(peer, ATTACK_EVENT_CREATE_PICKUP, NULL));
				skip_processing = true;
			}

			break;
		}
		case CHANGE_RADIO_STATION_EVENT:
		{
			int index_1 = static_cast<int>(message->PeekDWORD(8, seek_bits + 8 + 8 + 24));

			if (index_1 > 30 || index_1 < 0) //Some index is out of bounds
			{
				attacker_list.push_back(std::make_tuple(peer, ATTACK_EVENT_RADIO_STATION, NULL));
				skip_processing = true;
			}

			break;
		}
		case GAME_CLOCK_AND_WEATHER_EVENT:
		{
			BOOL check_weather_2 = FALSE;
			WEATHER weather = static_cast<WEATHER>(message->PeekDWORD(5, seek_bits + 3 + 6 + 7 + 7));
			int hour = static_cast<int>(message->PeekDWORD(6, seek_bits + 3));
			int minute = static_cast<int>(message->PeekDWORD(7, seek_bits + 3 + 6));
			
			if (message->PeekBool(seek_bits + 3 + 6 + 7 + 7 + 5))
				check_weather_2 = static_cast<WEATHER>(message->PeekDWORD(7, seek_bits + 3 + 6 + 7 + 7 + 5 + 1)) == WEATHER_SUNNY_WINDY_2;

			if (weather == WEATHER_SUNNY_WINDY_2 || check_weather_2)
			{
				if ((hour == 21 && minute > 24) || hour == 22 || (hour == 23 && minute < 36))
					skip_processing = true;
			}

			break;
		}
		default: //Event doesn't have any issues so lets process like normal
			break;
		}
	}
	
	if((((1 << type) & should_process_event_bitset) == 0) || skip_processing)
	{
		if(skip_processing)
			printf("[CARTEL::Event - %s] - %s tried to send invalid event data!\n", pEvent->GetEventName(), NetworkInterface::GetPlayerInfo(peer)->GetPlayerName());

		XMemCpy(modified_vftable, vftable, sizeof(modified_vftable)); //copy over vftable of current class

		//HandleExtraData doesn't actually do anything here
		modified_vftable[7] = reinterpret_cast<DWORD>(ReturnEventSuccess); //Overwrite Handle to not do anything
		modified_vftable[8] = reinterpret_cast<DWORD>(ReturnEventSuccess); //Overwrite Decide to not do anything
		*reinterpret_cast<DWORD**>(pEvent) = modified_vftable; //Set our modified vftable to the class
	}

	netEventMgr_HandleEventDetour.CallOriginal(manager, pEvent, message, peer, messageSeq, eventId); //We are good to process the event now
}