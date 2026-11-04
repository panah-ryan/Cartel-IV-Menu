#include <algorithm>
#include "stdafx.h"
#include "clone.h"
#include "natives.h"
#include "Detour.h"

enum eEntityOwnedBy : int
{
	ENTITY_OWNEDBY_UNKNOWN,
	ENTITY_OWNEDBY_1,
	ENTITY_OWNEDBY_2,
	ENTITY_OWNEDBY_SCRIPT,
	ENTITY_OWNEDBY_4,
	ENTITY_OWNEDBY_5,
	ENTITY_OWNEDBY_DEBUG
};

struct clone_timer
{
	DWORD tick[NET_OBJ_TYPE_COUNT];
	int count[NET_OBJ_TYPE_COUNT];
	BOOL throttled[NET_OBJ_TYPE_COUNT];
	DWORD throttle_tick[NET_OBJ_TYPE_COUNT];
};

DWORD should_process_clone_bitset = 0x3BF; //All clones enabled
PED_MODELS ped_blacklist_models[] = { PED_PLAYER, PED_SUPERLOD, PED_CS_ANDREI, PED_CS_ANGIE, PED_CS_BADMAN, PED_CS_BLEDAR, PED_CS_BULGARIN,
	PED_CS_BULGARINHENCH, PED_CS_CIA, PED_CS_DARDAN, PED_CS_DAVETHEMATE, PED_CS_DMITRI,
	PED_CS_EDTHEMATE, PED_CS_FAUSTIN, PED_CS_FRANCIS, PED_CS_HOSSAN, PED_CS_ILYENA,
	PED_CS_IVAN, PED_CS_JAY, PED_CS_JIMMY_PEGORINO, PED_CS_MEL, PED_CS_MICHELLE,
	PED_CS_MICKEY, PED_CS_OFFICIAL, PED_CS_RAY_BOCCINO, PED_CS_SERGEI, PED_CS_VLAD,
	PED_CS_WHIPPINGGIRL, PED_CS_MANNY, PED_CS_ANTHONY, PED_CS_ASHLEY, PED_CS_ASSISTANT,
	PED_CS_CAPTAIN, PED_CS_CHARLIEUC, PED_CS_DARKO, PED_CS_DWAYNE, PED_CS_ELI_JESTER,
	PED_CS_ELIZABETA, PED_CS_GAYTONY, PED_CS_GERRYMC, PED_CS_GORDON, PED_CS_ISSAC,
	PED_CS_JOHNNYTHEBIKER, PED_CS_JONGRAVELLI, PED_CS_JORGE, PED_CS_KAT, PED_CS_KILLER,
	PED_CS_LUIS, PED_CS_MAGICIAN, PED_CS_MAMC, PED_CS_MELODY, PED_CS_MITCHCOP,
	PED_CS_MORI, PED_CS_PBXGIRL2, PED_CS_PHILB, PED_CS_PLAYBOYX, PED_CS_PRIEST,
	PED_CS_RICKY, PED_CS_TOMMY, PED_CS_TRAMP, PED_CS_BRIAN, PED_CS_CHARISE,
	PED_CS_CLARENCE, PED_CS_EDDIELOW, PED_CS_GRACIE, PED_CS_JEFF, PED_CS_MARNIE,
	PED_CS_MARSHAL, PED_CS_PATHOS, PED_CS_SARAH, PED_CS_ROMAN_D, PED_CS_ROMAN_T,
	PED_CS_ROMAN_W, PED_CS_BRUCIE_B, PED_CS_BRUCIE_T, PED_CS_BRUCIE_W, PED_CS_BERNIE_CRANEC,
	PED_CS_BERNIE_CRANET, PED_CS_BERNIE_CRANEW, PED_CS_LILJACOB_B, PED_CS_LILJACOB_J, PED_CS_MALLORIE_D,
	PED_CS_MALLORIE_J, PED_CS_MALLORIE_W, PED_CS_DERRICKMC_B, PED_CS_DERRICKMC_D, PED_CS_MICHAEL_B,
	PED_CS_MICHAEL_D, PED_CS_PACKIEMC_B, PED_CS_PACKIEMC_D, PED_CS_KATEMC_D, PED_CS_KATEMC_W,
	PED_CS_NIKO, PED_CS_ANDREAS, PED_CS_ANGELGUY, PED_CS_ANGUS, PED_CS_ASHLEYA,
	PED_CS_BILLY, PED_CS_BIKESTRIPPER, PED_CS_BRIANJ, PED_CS_CLAY, PED_CS_DAVE_GROSSMAN,
	PED_CS_DESEAN, PED_CS_ELIZABETAT, PED_CS_FRANKIE, PED_CS_JASON, PED_CS_JIM_FITZ,
	PED_CS_MAITRE_D, PED_CS_MALC, PED_CS_MITCHB, PED_CS_MARTA, PED_CS_MASSEUSE,
	PED_CS_MATTHEWS, PED_CS_MCCORNISH, PED_CS_PAUL_LAMONT, PED_CS_PRISONDLC_01, PED_CS_PRISONGUARD,
	PED_CS_RAYGOON, PED_CS_RAY_LEATHER, PED_CS_RONCERO, PED_CS_RUSSIAN_GOON, PED_CS_RUSSIANGOON2,
	PED_CS_TERRY, PED_CS_SKANK, PED_CS_STUBBS, PED_CS_STUBBS_SUIT, PED_CS_BAZ, PED_CS_JEREMY, PED_CS_ABDUL, PED_CS_ABODYGUARD, PED_CS_ADRIANA,
	PED_CS_AHMAD, PED_CS_ARMANDO, PED_CS_ARNAUD, PED_CS_BATHROOM, PED_CS_BILLY2,
	PED_CS_BULGARIN2, PED_CS_BRUCIE2, PED_CS_DAISY, PED_CS_DERRICK2, PED_CS_DESSIE,
	PED_CS_DOCKGOON, PED_CS_EUGENE, PED_CS_EVAN2, PED_CS_GPARTY_01, PED_CS_GRACIE2,
	PED_CS_HENRIQUE, PED_CS_ISSAC2, PED_CS_JOHNNY2, PED_CS_MARGOT, PED_CS_MAURICE,
	PED_CS_MORI_K, PED_CS_MR_SANTOS, PED_CS_NIKO2, PED_CS_OYVEY, PED_CS_PACKIE2,
	PED_CS_PAPI, PED_CS_ROCCO, PED_CS_ROMAN2, PED_CS_RUS_KN, PED_CS_SHARON,
	PED_CS_TAHIR, PED_CS_TIMUR, PED_CS_TONY, PED_CS_TONYAQ, PED_CS_TRAMP2,
	PED_CS_TRIAD, PED_CS_TROY, PED_CS_VIC, PED_CS_VICGIRL, PED_CS_VINCE, PED_CS_YUSEF };
std::vector<PED_MODELS> ped_blacklist;
std::vector<DWORD> object_blacklist;
clone_timer timer[16];
DWORD clone_spam_reset_time = 60000;
int clone_spam_lockout_count = 30;
int clone_spam_throttle_count = 9;

void setup_blacklists()
{
	if (ped_blacklist.size() < ARRAYSIZE(ped_blacklist_models)) //small chance menu may add more.. 
	{
		printf("[CARTEL::Debug] Setting up Ped Blacklists\n");
		for (int i = 0; i < ARRAYSIZE(ped_blacklist_models); i++)
			ped_blacklist.push_back(ped_blacklist_models[i]);
	}
	else
		printf("[CARTEL::Debug] Skipping Ped Blacklists, already done.\n");
}

void CTL_GET_CLONE_BITSET(scrThread_Info* cxt)
{
	cxt->ResultPtr[0].Any = reinterpret_cast<DWORD>(&should_process_clone_bitset);
}

void CTL_ADD_BLACKLIST_PED(scrThread_Info* cxt)
{
	PED_MODELS model_hash = static_cast<PED_MODELS>(cxt->Params[0].Uns);
	ped_blacklist.push_back(model_hash);
}

void CTL_REMOVE_BLACKLIST_PED(scrThread_Info* cxt)
{
	PED_MODELS model_hash = static_cast<PED_MODELS>(cxt->Params[0].Uns);
	
	std::vector<PED_MODELS>::iterator it = std::find(ped_blacklist.begin(), ped_blacklist.end(), model_hash);
	ped_blacklist.erase(it);
}

void CTL_ADD_BLACKLIST_OBJECT(scrThread_Info* cxt)
{
	DWORD model_hash = cxt->Params[0].Uns;
	object_blacklist.push_back(model_hash);
}

void CTL_REMOVE_BLACKLIST_OBJECT(scrThread_Info* cxt)
{
	DWORD model_hash = cxt->Params[0].Uns;

	std::vector<DWORD>::iterator it = std::find(object_blacklist.begin(), object_blacklist.end(), model_hash);
	object_blacklist.erase(it);
}

void CTL_SET_CLONE_SPAM_RESET_TIME(scrThread_Info* cxt)
{
	clone_spam_reset_time = cxt->Params[0].Int;
}

void CTL_SET_CLONE_SPAM_VARIABLES(scrThread_Info* cxt)
{
	clone_spam_lockout_count = cxt->Params[0].Int;
	clone_spam_throttle_count = cxt->Params[1].Int;
}

inline bool IsVehicleObjectType(NetworkObjectType type)
{
	return type > NET_OBJ_TYPE_DUMMY_PED && type != NET_OBJ_TYPE_OBJECT;
}

Detour<void> CNetworkObjectMgr_ProcessCloneCreateDataDetour;
void CNetworkObjectMgr_ProcessCloneCreateData(CNetworkObjectMgr* netObjMgr, byte peer, NetworkObjectType objectType, short objectID, byte objectFlags, CMessage* message)
{
	bool should_we_create = false;
	BOOL is_mission_object = (objectFlags & GLOBAL_FLAG_SCRIPT_OBJECT) >> 3;

	if (objectType >= NET_OBJ_TYPE_PLAYER && objectType < NET_OBJ_TYPE_COUNT && ((1 << objectType) & should_process_clone_bitset) != 0)
	{
		
		int seek_bits = message->m_buffer.m_CursorPos + message->m_buffer.m_BaseBitOffset;

		DWORD time = GetTickCount() - timer[peer].tick[objectType];
		if (time > clone_spam_reset_time)
		{
			timer[peer].tick[objectType] = 0;
			timer[peer].count[objectType] = 0;
			timer[peer].throttled[objectType] = FALSE;
			timer[peer].throttle_tick[objectType] = NULL;
		}

		/*
		if (IsVehicleObjectType(objectType) && (ePopType)message->PeekDWORD(3, seek_bits + 32) == POPTYPE_MISSION)
		{
			timer[peer].tick[objectType] = GetTickCount();

			if (timer[peer].count[objectType] > clone_spam_lockout_count)
			{
				printf("[Clone Create] - Prevented %s from spawning by player %s due to spam!\n", CNetworkObjectMgr::GetObjectTypeName(objectType, false), NetworkInterface::GetPlayerInfo(peer)->GetPlayerName());
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_SPAM, objectType));
				return;
			}

			if (timer[peer].count[objectType] > clone_spam_throttle_count)
			{
				if (timer[peer].throttled[objectType])
				{
					DWORD let_up_tick = timer[peer].throttle_tick[objectType];
					if (let_up_tick > GetTickCount())
					{
						printf("[Clone Create] - Continue throttling %s's %s [%i] time left %i MS!\n", NetworkInterface::GetPlayerInfo(peer)->GetPlayerName(), CNetworkObjectMgr::GetObjectTypeName(objectType, false), objectID, let_up_tick - GetTickCount());
						netObjMgr->AddCreateAck(peer, objectID, ACKCODE_TOO_MANY_OBJECTS);
						return;
					}
					else
						timer[peer].throttled[objectType] = FALSE;
				}
				else
				{
					printf("[Clone Create] - Starting throttle for %s's %s [%i]!\n", NetworkInterface::GetPlayerInfo(peer)->GetPlayerName(), CNetworkObjectMgr::GetObjectTypeName(objectType, false), objectID);
					timer[peer].throttle_tick[objectType] = GetTickCount() + 3000;
					timer[peer].throttled[objectType] = TRUE;
					netObjMgr->AddCreateAck(peer, objectID, ACKCODE_TOO_MANY_OBJECTS);
					return;
				}
			}

			timer[peer].count[objectType]++;
		}
		*/
		switch (objectType)
		{
		case NET_OBJ_TYPE_PLAYER: //Need to peek if player_index is invalid or ours, if so change it to a valid one and let them spawn. Prevents weird bugs from happening later down the line.
		{
			int player_index = static_cast<int>(message->PeekDWORD(5, seek_bits));
			DWORD model_hash = message->PeekDWORD(32, seek_bits + 5);


			if (!NetworkInterface::GetPlayerInfo(peer)->GetPlayerPed() && player_index < 16 && player_index >= 0 && player_index != NetworkInterface::GetLocalPlayer()->GetPeerID()) //Verify its not a dupe and isn't taking our player index or an invalid one
				should_we_create = true;
			else
			{
				//change their player index to one that isn't taken.
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_PLAYER, player_index));
				for (int i = 0; i < 16; i++) 
				{
					if (!NetworkInterface::GetNetPlayer(i)->IsPhysical()) //We found an open slot in the game lets set their index to that
					{
						message->PokeDWORD(i, 5, seek_bits);
						player_index = i;
						should_we_create = true;
						break;
					}
				}
			}

			if (model_hash == NULL || (model_hash != PED_M_Y_MULTIPLAYER && model_hash != PED_F_Y_MULTIPLAYER)) //Not spawning as a normal multiplayer model since we want them to spawn lets change it (Fixes holy ghost)
				message->PokeDWORD(PED_M_Y_MULTIPLAYER, 32, seek_bits + 5);

			break;
		}
		case NET_OBJ_TYPE_PED:
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits + 2);
			DWORD prop_hash = NULL;
			bool skip_due_to_prop = false;

			seek_bits += 2 + 32 + 16;
			if (is_mission_object)
			{
				seek_bits += 8 + 1;
				seek_bits += message->PeekBool(seek_bits) ? 10 + 1 : 1;
				seek_bits += 1 + 1 + 1;
			}

			if (message->PeekBool(seek_bits))
				prop_hash = message->PeekDWORD(32, seek_bits + 1);

			if (prop_hash != NULL && (IS_THIS_MODEL_A_PED(prop_hash) || IS_THIS_MODEL_A_VEHICLE(prop_hash))) //Its not an object
				skip_due_to_prop = true;

			if (!skip_due_to_prop && model_hash != NULL && IS_THIS_MODEL_A_PED(model_hash) && std::find(ped_blacklist.begin(), ped_blacklist.end(), model_hash) == ped_blacklist.end()) //Model is valid, is a ped, and doesn't exist in our blacklist
				should_we_create = true;
			else
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_PED, model_hash));

			break;
		}
		case NET_OBJ_TYPE_DUMMY_PED:
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits);
			DWORD prop_hash = NULL;
			bool skip_due_to_prop = false;

			if (message->PeekBool(seek_bits + 32))
				prop_hash = message->PeekDWORD(32, seek_bits + 32 + 1);

			if (prop_hash != NULL && (IS_THIS_MODEL_A_PED(prop_hash) || IS_THIS_MODEL_A_VEHICLE(prop_hash))) //Its not an object
				skip_due_to_prop = true;

			if (!skip_due_to_prop && model_hash != NULL && IS_THIS_MODEL_A_PED(model_hash) && std::find(ped_blacklist.begin(), ped_blacklist.end(), model_hash) == ped_blacklist.end()) //Model is valid, is a ped, and doesn't exist in our blacklist
				should_we_create = true;
			else
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_DUMMYPED, model_hash));

			break;
		}
		case NET_OBJ_TYPE_AUTOMOBILE:
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_CAR(model_hash)) //Valid model for a car
				should_we_create = true;
			else
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_CAR, model_hash));

			break;
		}
		case NET_OBJ_TYPE_OBJECT:
		{
			eEntityOwnedBy owned_by = static_cast<eEntityOwnedBy>(message->PeekDWORD(3, seek_bits) + 1);

			if (is_mission_object || owned_by == ENTITY_OWNEDBY_DEBUG || owned_by == ENTITY_OWNEDBY_SCRIPT)
			{
				DWORD model_hash = message->PeekDWORD(32, seek_bits + 3);

				//check trains in object pool! Cars are OK..
				if (model_hash != NULL && !IS_THIS_MODEL_A_PED(model_hash) // && !IS_THIS_MODEL_A_VEHICLE(model_hash)
					&& std::find(object_blacklist.begin(), object_blacklist.end(), model_hash) == object_blacklist.end()) //Can't check if model is an object so lets check its not ped or vehicle and isn't in our blacklist
					should_we_create = true;
				else
					attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_OBJECT, model_hash));
			}
			else
				should_we_create = true; //Its not reading a model so we can assume its safe

			break;
		}
		case NET_OBJ_TYPE_BIKE:
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_BIKE(model_hash)) //Model is a bike
				should_we_create = true;
			else
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_BIKE, model_hash));

			break;
		}
		case NET_OBJ_TYPE_TRAIN:
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits);
			
			if (model_hash != NULL && IS_THIS_MODEL_A_TRAIN(model_hash)) //Model is a train
				should_we_create = true;
			else
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_TRAIN, model_hash));

			break;
		}
		case NET_OBJ_TYPE_HELI:
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_HELI(model_hash)) //Model is a heli
				should_we_create = true;
			else
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_HELI, model_hash));

			break;
		}
		case NET_OBJ_TYPE_BOAT:
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_BOAT(model_hash)) //Model is a boat
				should_we_create = true;
			else
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_BOAT, model_hash));

			break;
		}
		case NET_OBJ_TYPE_PLANE:
		{
			DWORD model_hash = message->PeekDWORD(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_VEHICLE(model_hash)) //Can't check if model is a plane (always returns false) so haven't found any issues if its any vehicle
				should_we_create = true;
			else
				attacker_list.push_back(std::make_tuple(peer, ATTACK_CLONE_PLANE, model_hash));

			break;
		}
		default:
			break;
		}
	}

	if (!should_we_create)
	{
		if (objectType < NET_OBJ_TYPE_COUNT && objectType >= NET_OBJ_TYPE_PLAYER)
			printf("[CARTEL::Clone Create] - Prevented %s from spawning by player %s!\n", CNetworkObjectMgr::GetObjectTypeName(objectType, false), NetworkInterface::GetPlayerInfo(peer)->GetPlayerName());

		return;
	}

	CNetworkObjectMgr_ProcessCloneCreateDataDetour.CallOriginal(netObjMgr, peer, objectType, objectID, objectFlags, message); //Continue original code since its safe to spawn
}

Detour<bool> CNetObjHeli_SerializeCloneDataDetour;
bool CNetObjHeli_SerializeCloneData(CNetworkObject* net_heli, CMessage* message)
{
	bool ret = CNetObjHeli_SerializeCloneDataDetour.CallOriginal(net_heli, message);

	if (ret)
	{
		CHeli* heli = net_heli->GetBaseHeli();
		if (heli && heli->m_PlayerIndexSpotlight < -1 || heli->m_PlayerIndexSpotlight > 15)
		{
			printf("[CARTEL::Network Heli] - Corrected invalid player spot light crash!\n");
			heli->m_PlayerIndexSpotlight = -1;
		}
	}

	return ret;
}