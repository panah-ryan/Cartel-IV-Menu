#include "stdafx.h"
#include "Utils.h"
#include "Network_Options.h"
#include "Network_Functions.h"
#include "MenuExecutionHandling.h"
#include "MenuUiHandling.h"
#include "Player_Options.h"
#include "Ped_Options.h"
#include "Functions.h"
#include "ItemExecution.h"
#include "Menu_Options.h"
#include "Vehicle_Options.h"
#include "Teleport_Options.h"
#include "World_Options.h"
#include "Protection_Options.h"
#include "Admin_Options.h"
#include "ESP.h"
#include "Security.h"
#include "Keyboard_Options.h"
#include "threading.h"
#include "Chat_Options.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types"

int gameModeSelect;
int episodeSelect;
int inviteStringSelect;

VEHICLE_INDEX attachVeh = 0;

int networkHostArmourRegenSeconds = 1; //could easily be in a byte set / a byte itself
int networkHostHealthRegenSeconds = 1; //could easily be in a byte set / a byte itself
int networkHostWeaponRegenSeconds = 1; //could easily be in a byte set / a byte itself

int netPlayerSpectating = -1;

const char* GetGamemodeString(int Gamemode)
{
	char* Gamemode_GXT_Entry = BUFFER8;
	strcpy(Gamemode_GXT_Entry, "NTGT_", 8);
	straddi(Gamemode_GXT_Entry, Gamemode, 8);
	return GET_STRING_FROM_TEXT_FILE(Gamemode_GXT_Entry);
	//MAIN, NTGT_16 = Free Mode //0 is included / deathmatch.
}

int GetNumberOfRecentlyKickedPlayers()
{
	int count = 0;
	for (int i = 0; i < MAX_RECENTLY_KICKED_CONSOLES; i++)
		if (ctlMem->m_recent_kicked_storage[i].MachineID != 0)
			count++;

	return count;
}

bool AddPlayerToRecentlyKickedStorage(PLAYER_INDEX PlayerID)
{
#ifdef Developer
	CPlayerInfo* player = GetPlayerPointer(PlayerID); //Use this in normal build once security is laxed
#else
	CPlayerInfo* player = (CPlayerInfo*)GetPlayerPointerNath(PlayerID);
#endif
	//Worldwide420o
	player->m_xuid[0] = 25354505;
	player->m_xuid[1] = 34625914;
	//player->m_xuid[0] = 0; //They will always be kicked when this function is called so we mayaswell do this here when the ids are already to hand.
	//player->m_xuid[1] = 0; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted. (Spoofing exploit prevention)

	for (int i = 0; i < MAX_KICKED_CONSOLES; i++) //sometimes shit catches twice, we will just check the IP and MID is not already present.
	{
		//Should we bother to check mac address aswell here? Or perhaps we should only check MachineID to grab new ids if they change them..
		if (ctlMem->m_recent_kicked_storage[i].MachineID == player->m_machineId[0] && ctlMem->m_recent_kicked_storage[i].IPAddress == player->m_wan_ip)
		{ //this machineID and IP address combo is already stored. 
			return false;
		}
	}
	for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
	{
		if (ctlMem->m_recent_kicked_storage[i].MachineID != 0)
			continue; //somethings stored here already.

		memcpy(ctlMem->m_recent_kicked_storage[i].gamertag, player->m_gamertag, 16);
		strcpy(ctlMem->m_recent_kicked_storage[i].kickedReason, "Not provided", 26); //nuke kicked reason
		ctlMem->m_recent_kicked_storage[i].MachineID = player->m_machineId[0]; //upper half is just nonsense for 360 0x000000FA always
		ctlMem->m_recent_kicked_storage[i].IPAddress = player->m_wan_ip;
		//ctlMem->m_recent_kicked_storage[i].failsCounter = 0; //not needed could use for something else down the line.
		for (int j = 0; j < 6; j++)
			ctlMem->m_recent_kicked_storage[i].MAC[j] = player->_2port_6mac_2pad[j + 2];

		//*kick_array_index = i;
		return true;
	}
	return false; //revert back to normal XUID kicking (usually called right after), kicked data array is full.
}

void NetworkOptWhitelistRecentJoin(int index, int unused1)
{
	ctlMem->m_recent_kicked_storage[index].failsCounter = -1337; //use this as whitelist flag.
#ifdef Moderator
	printf("~r~CTL: ~w~Whitelisted ~y~%s (recent_kicked_storage index %i), ~w~they will clear from this list automatically when they join.", ctlMem->m_recent_kicked_storage[index].gamertag, index);
#else
	printf("~r~CTL: ~w~Whitelisted ~y~%s, ~w~they will clear from this list automatically when they join.", ctlMem->m_recent_kicked_storage[index].gamertag);
#endif
	RefreshCurrentPage(0, 0); //refresh description to show whitelisted.
}

void NetworkOptClearRecentJoin(int index, int unused1)
{
	memset(ctlMem->m_recent_kicked_storage[index].gamertag, 0, 16);
	memset(ctlMem->m_recent_kicked_storage[index].kickedReason, 0, 26);
	ctlMem->m_recent_kicked_storage[index].MachineID = 0;
	ctlMem->m_recent_kicked_storage[index].IPAddress = 0;
	ctlMem->m_recent_kicked_storage[index].failsCounter = 0;
	for (int j = 0; j < 6; j++)
		ctlMem->m_recent_kicked_storage[index].MAC[j] = 0;
}

void NetworkOptClearAllRecentJoins(int un1, int un2)
{
	for (int index = 0; index < MAX_RECENTLY_KICKED_CONSOLES; index++)
	{
		memset(ctlMem->m_recent_kicked_storage[index].gamertag, 0, 16);
		memset(ctlMem->m_recent_kicked_storage[index].kickedReason, 0, 26);
		ctlMem->m_recent_kicked_storage[index].MachineID = 0;
		ctlMem->m_recent_kicked_storage[index].IPAddress = 0;
		ctlMem->m_recent_kicked_storage[index].failsCounter = 0;
		for (int j = 0; j < 6; j++)
			ctlMem->m_recent_kicked_storage[index].MAC[j] = 0;
	}
	RefreshCurrentPage(0, 0);
}

bool IsPlayerInRecentListWhitelisted(PLAYER_INDEX playerId, int* recent_kicked_arr_index)
{
#ifdef Developer
	CPlayerInfo* player = GetPlayerPointer(playerId); //Use this in normal build once security is laxed
#else
	CPlayerInfo* player = (CPlayerInfo*)GetPlayerPointerNath(playerId);
#endif
	for (int i = 0; i < MAX_RECENTLY_KICKED_CONSOLES; i++)
	{
		if (ctlMem->m_recent_kicked_storage[i].MachineID == player->m_machineId[0] && ctlMem->m_recent_kicked_storage[i].IPAddress == player->m_wan_ip)
		{
			//optinally compare mac address before returning
			*recent_kicked_arr_index = i;
			return ctlMem->m_recent_kicked_storage[i].failsCounter == -1337;
		}
	}
	return false;
}

bool Is_Vehicles_Type_Modified(uint vehicleModel)
{
	uint compareModel = vehicleModel;
	switch (compareModel)
	{
	case VEHICLE_ADMIRAL:
	case VEHICLE_AMBULANCE:
	case VEHICLE_BANSHEE:
	case VEHICLE_BENSON:
	case VEHICLE_BIFF:
	case VEHICLE_BLISTA:
	case VEHICLE_BOBCAT:
	case VEHICLE_BOXVILLE:
	case VEHICLE_BUCCANEER:
	case VEHICLE_BURRITO:
	case VEHICLE_BURRITO2:
	case VEHICLE_BUS:
	case VEHICLE_CABBY:
	case VEHICLE_CAVALCADE:
	case VEHICLE_CHAVOS:
	case VEHICLE_COGNOSCENTI:
	case VEHICLE_COMET:
	case VEHICLE_COQUETTE:
	case VEHICLE_DF8:
	case VEHICLE_DILETTANTE:
	case VEHICLE_DUKES:
	case VEHICLE_E109:
	case VEHICLE_EMPEROR:
	case VEHICLE_EMPEROR2:
	case VEHICLE_ESPERANTO:
	case VEHICLE_FACTION:
	case VEHICLE_FBI:
	case VEHICLE_FELTZER:
	case VEHICLE_FEROCI:
	case VEHICLE_FEROCI2:
	case VEHICLE_FIRETRUK:
	case VEHICLE_FLATBED:
	case VEHICLE_FORTUNE:
	case VEHICLE_FORKLIFT:
	case VEHICLE_FUTO:
	case VEHICLE_FXT:
	case VEHICLE_HABANERO:
	case VEHICLE_HAKUMAI:
	case VEHICLE_HUNTLEY:
	case VEHICLE_INFERNUS:
	case VEHICLE_INGOT:
	case VEHICLE_INTRUDER:
	case VEHICLE_LANDSTALKER:
	case VEHICLE_LOKUS:
	case VEHICLE_MANANA:
	case VEHICLE_MARBELLA:
	case VEHICLE_MERIT:
	case VEHICLE_MINIVAN:
	case VEHICLE_MOONBEAM:
	case VEHICLE_MRTASTY:
	case VEHICLE_MULE:
	case VEHICLE_NOOSE:
	case VEHICLE_NSTOCKADE:
	case VEHICLE_ORACLE:
	case VEHICLE_PACKER:
	case VEHICLE_PATRIOT:
	case VEHICLE_PERENNIAL:
	case VEHICLE_PERENNIAL2:
	case VEHICLE_PEYOTE:
	case VEHICLE_PHANTOM:
	case VEHICLE_PINNACLE:
	case VEHICLE_PMP600:
	case VEHICLE_POLICE:
	case VEHICLE_POLICE2:
	case VEHICLE_POLPATRIOT:
	case VEHICLE_PONY:
	case VEHICLE_PREMIER:
	case VEHICLE_PRES:
	case VEHICLE_PRIMO:
	case VEHICLE_PSTOCKADE:
	case VEHICLE_RANCHER:
	case VEHICLE_REBLA:
	case VEHICLE_RIPLEY:
	case VEHICLE_ROMERO:
	case VEHICLE_ROM:
	case VEHICLE_RUINER:
	case VEHICLE_SABRE:
	case VEHICLE_SABRE2:
	case VEHICLE_SABREGT:
	case VEHICLE_SCHAFTER:
	case VEHICLE_SENTINEL:
	case VEHICLE_SOLAIR:
	case VEHICLE_SPEEDO:
	case VEHICLE_STALION:
	case VEHICLE_STEED:
	case VEHICLE_STOCKADE:
	case VEHICLE_STRATUM:
	case VEHICLE_STRETCH:
	case VEHICLE_SULTAN:
	case VEHICLE_SULTANRS:
	case VEHICLE_SUPERGT:
	case VEHICLE_TAXI:
	case VEHICLE_TAXI2:
	case VEHICLE_TRASH:
	case VEHICLE_TURISMO:
	case VEHICLE_URANUS:
	case VEHICLE_VIGERO:
	case VEHICLE_VIGERO2:
	case VEHICLE_VINCENT:
	case VEHICLE_VIRGO:
	case VEHICLE_VOODOO:
	case VEHICLE_WASHINGTON:
	case VEHICLE_WILLARD:
	case VEHICLE_YANKEE:
		if (!IS_THIS_MODEL_A_CAR(compareModel)) return true;
		break;

	case VEHICLE_DINGHY:
	case VEHICLE_JETMAX:
	case VEHICLE_MARQUIS:
	case VEHICLE_PREDATOR:
	case VEHICLE_REEFER:
	case VEHICLE_SQUALO:
	case VEHICLE_TUGA:
	case VEHICLE_TROPIC:
		if (!IS_THIS_MODEL_A_BOAT(compareModel)) return true;
		break;

	case VEHICLE_ANNIHILATOR:
	case VEHICLE_MAVERICK:
	case VEHICLE_POLMAV:
	case VEHICLE_TOURMAV:
		if (!IS_THIS_MODEL_A_HELI(compareModel)) return true;
		break;

	case VEHICLE_BOBBER:
	case VEHICLE_FAGGIO:
	case VEHICLE_HELLFURY:
	case VEHICLE_NRG900:
	case VEHICLE_PCJ:
	case VEHICLE_SANCHEZ:
	case VEHICLE_ZOMBIEB:
		if (!IS_THIS_MODEL_A_BIKE(compareModel)) return true;
		break;

	case VEHICLE_CABLECAR:
	case VEHICLE_SUBWAY_LO:
	case VEHICLE_SUBWAY_HI:
		if (!IS_THIS_MODEL_A_TRAIN(compareModel)) return true;
		break;

	default:
		break;
	}
	if (myCurrentEpisode == EPISODE_TLAD)
	{
		compareModel = vehicleModel; //switches nuke variables.
		switch (compareModel)
		{
		case VEHICLE_GBURRITO:
		case VEHICLE_TOWTRUCK:
		case VEHICLE_PACKER2:
		case VEHICLE_PBUS:
		case VEHICLE_YANKEE2:
		case VEHICLE_RHAPSODY:
		case VEHICLE_REGINA:
			if (!IS_THIS_MODEL_A_CAR(compareModel)) return true;
			break;

		case VEHICLE_ANGEL:
		case VEHICLE_BATI:
		case VEHICLE_DAEMON:
		case VEHICLE_DIABOLUS:
		case VEHICLE_DOUBLE2:
		case VEHICLE_HAKUCHOU2:
		case VEHICLE_INNOVATION:
		case VEHICLE_LYCAN:
		case VEHICLE_NIGHTBLADE:
		case VEHICLE_REVENANT:
		case VEHICLE_WAYFARER:
		case VEHICLE_WOLFSBANE:
			if (!IS_THIS_MODEL_A_BIKE(compareModel)) return true;

		default:
			break;
		}
	}

	if (myCurrentEpisode == EPISODE_TBOGT)
	{
		compareModel = vehicleModel; //switches nuke variables.
		switch (compareModel)
		{
		case VEHICLE_SLAMVAN:
		case VEHICLE_CADDY:
		case VEHICLE_APC:
		case VEHICLE_SUPERD:
		case VEHICLE_SUPERD2:
		case VEHICLE_SERRANO:
		case VEHICLE_SERRANO2:
		case VEHICLE_BUFFALO:
		case VEHICLE_AVAN:
		case VEHICLE_SCHAFTER2:
		case VEHICLE_SCHAFTER3:
		case VEHICLE_BULLET:
		case VEHICLE_TAMPA:
		case VEHICLE_CAVALCADE2:
		case VEHICLE_F620:
		case VEHICLE_LIMO2:
		case VEHICLE_POLICE3:
		case VEHICLE_POLICEW:
		case VEHICLE_POLICE4:
			if (!IS_THIS_MODEL_A_CAR(compareModel)) return true;
			break;

		case VEHICLE_SMUGGLER:
		case VEHICLE_FLOATER:
		case VEHICLE_BLADE:
			if (!IS_THIS_MODEL_A_BOAT(compareModel)) return true;

		case VEHICLE_POLICEB:
		case VEHICLE_HEXER:
		case VEHICLE_FAGGIO2:
		case VEHICLE_BATI2:
		case VEHICLE_VADER:
		case VEHICLE_AKUMA:
		case VEHICLE_HAKUCHOU:
		case VEHICLE_DOUBLE:
			if (!IS_THIS_MODEL_A_BIKE(compareModel)) return true;
			break;

		case VEHICLE_BUZZARD:
		case VEHICLE_SWIFT:
		case VEHICLE_SKYLIFT:
			if (!IS_THIS_MODEL_A_HELI(compareModel)) return true;
			break;

		default:
			break;

		}
	}
	return false; // If the vehicle model passed all checks, return true
}


void UploadScores()
{
	*(int*)0x831D53D8 = 1; // 00 00 00 01 == Upload scores, 02 = download scores (works in singleplayer).
}

void DownloadScores()
{
	*(int*)0x831D53D8 = 2; // 00 00 00 01 == Upload scores, 02 = download scores (works in singleplayer).
}

void VoteKick(PLAYER_INDEX kickTarget)
{
	/*
	if(myCurrentEpisode == EPISODE_ORIGINAL)
		setGlobalAtIndex(185, kickTarget);
	else if (myCurrentEpisode == EPISODE_TBOGT)
		setGlobalAtIndex(813, kickTarget);
	*/
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL: setGlobalAtIndex(185, kickTarget); break;
	case EPISODE_TLAD: break;
	case EPISODE_TBOGT: setGlobalAtIndex(813, kickTarget); break;
	}
}

void SuggestGamemode(int gamemode)
{
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL: setGlobalAtIndex(183, -1); setGlobalAtIndex(183, gamemode); break;
	case EPISODE_TLAD: break;
	case EPISODE_TBOGT: setGlobalAtIndex(811, -1); setGlobalAtIndex(811, gamemode); break;
	}
}

void DickheadScannerLoop()
{
	if (!NetworkGame)
		return;

	char* dick_gamertag;
	for (int i = 0; i < 16; i++)
	{
		//if (IS_BIT_SET(ctlMem->m_bit_sets.m_checkedDickheadStatus_unused, i)) //needs to actively check host. //could set all 0 maybe when host id changes?
			//continue; //don't keep running checks on players we've already checked.
		if(IsPlayerValid(i))
		{
			if(i != myPlayerIndex)
				logPlayer(i); //ALWAYS CALL THIS OR ALL THE SHIT WILL BREAK! BITS GET SET, NEEDS TO PROCESS

			if (!IS_BIT_SET(ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, (i + 16)))
			{
				//CPlayerInfo* player = GetPlayerPointer(i); //temporary fix
				//if (player == nullptr)
					//continue;

				int dick_status = PlayerIsADickhead(i, &dick_gamertag);
				//SET_BIT(&ctlMem->m_bit_sets.m_checkedDickheadStatus_unused, i); //don't keep checking all this shit on every player
				if (dick_status) //return 2 if new xb1 found.
				{
					if (i == myPlayerIndex)
					{
						SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME();
					}
#ifndef STEALTH
					if (dick_status == DICKLIST_KICK_IF_HOST_DISPLAY_IDENTIFIERS && !bitTest(ctlMem->m_bit_sets.m_player, printedNewDickheadIdentifiers))
					{
						if (!IS_SCREEN_FADED_OUT())
						{
							printf("~r~CTL: ~w~NEW MID DETECTED FOR DICKLIST (%s) (CHECK BRIEF!)", dick_gamertag);
							bitSet(&ctlMem->m_bit_sets.m_player, printedNewDickheadIdentifiers);
						}
					}
#endif
					if (GET_HOST_ID() == myPlayerIndex)
					{
						uint kicked_array_index;
#ifndef STEALTH
						if (AddPlayerToKickStorage(i, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Cartel dick-list", 26);
						//should watch out for their IP for normal peeps.. hmph
						if (dick_status != DICKLIST_KICK_IF_HOST_DISPLAY_IDENTIFIERS) //NEW XB1 FOUND GET IT BACK TO ME!
							printf("~r~CTL: ~w~Dickhead %s ~w~kicked (%s).", SafeNetworkPlayerGetNameWithColour(i), dick_gamertag);
#endif
						NETWORK_KICK_PLAYER(i);
						continue;
					}
					else
					{ //we are not host.

						if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_AUTO_VOTE_KICK_DICKHEADS)) // || numCartelUsers > (numPlayers / 2) 
						{
							if (IS_NETWORK_PLAYER_ACTIVE(myPlayerIndex)) //&& IS_NETWORK_PLAYER_ACTIVE(i))
								VoteKick(i);
						}

						if (GET_HOST_ID() == i)
						{
							if (IS_NETWORK_PLAYER_ACTIVE(myPlayerIndex) && !IS_BIT_SET(ctlMem->m_bit_sets.m_offhostKicked_triedCrashingHostsFreemode, (i + 16))) //won't work if we're inactive.
							{
								ctl_attack_crash_hosts_freemode(0, 0);
								SET_BIT(&ctlMem->m_bit_sets.m_offhostKicked_triedCrashingHostsFreemode, (i + 16));
							}
						}
#ifdef Developer
						else
						{
							if (!IS_BIT_SET(ctlMem->m_bit_sets.m_offhostKicked_triedCrashingHostsFreemode, i) && CartelXEXPresent)
							{
								CTL_OFFHOST_KICK(i);
								SET_BIT(&ctlMem->m_bit_sets.m_offhostKicked_triedCrashingHostsFreemode, i);
							}
						}
#endif
						if (!IS_BIT_SET(ctlMem->m_bit_sets.m_capDickheadNotify_instantReviveCap, i))
						{
							if (!IS_SCREEN_FADED_OUT())
							{
#ifndef STEALTH
								printf("~r~CTL: ~w~Dickhead (%s) detected as %s", dick_gamertag, SafeNetworkPlayerGetNameWithColour(i));
								SET_BIT(&ctlMem->m_bit_sets.m_capDickheadNotify_instantReviveCap, i);
#endif
#ifdef Moderator
								if (dick_status == DICKLIST_OLD_MODS_LAG_AND_VOTE_KICK)
									printf("~r~CTL MOD: ~w~Lagging dickhead %s (%s).", SafeNetworkPlayerGetNameWithColour(i), dick_gamertag);
#endif		
							}
						}
						if (dick_status == DICKLIST_OLD_MODS_LAG_AND_VOTE_KICK) //ehh maybe on the vote kick later.
						{
							float rand_x, rand_y, rand_z, rand_h;
							GENERATE_RANDOM_FLOAT_IN_RANGE(-2400.0f, 3000.0f, &rand_x);
							GENERATE_RANDOM_FLOAT_IN_RANGE(-1350.0f, 2250.0f, &rand_y);
							GENERATE_RANDOM_FLOAT_IN_RANGE(30.0f, 100.0f, &rand_z);
							GENERATE_RANDOM_FLOAT_IN_RANGE(0.0f, 359.0f, &rand_h); //360 same as 0 so meh
							SetHostByte(true);
							RESURRECT_NETWORK_PLAYER(i, rand_x, rand_y, rand_z, rand_h); //z - 1.0 as it sets +1.0
							SetHostByte(GET_HOST_ID() == myPlayerIndex);
							//i think resurrect them into slod......
							//if nobody is near the randomly generated area..
						}
						else
						{
							/*
							if (!IS_NETWORK_PLAYER_ACTIVE(i) && !IS_SCREEN_FADED_OUT()) //should fuck with their status and prevent them joining.
							{
	#ifdef Developer
								PRINTSTRING("Dicklist player is not active so we're gonna try fuck them up using events that cause status changes..\n");
	#endif
								SetHostByte(true);
								RESURRECT_NETWORK_PLAYER(i, 0.0f, 0.0f, -98.0f, 0.0f); //should fuck their status and deny them joining.
								SetHostByte(GET_HOST_ID() == myPlayerIndex);
							}
							*/
						}
					}
				}
				else if(dick_status != DICKLIST_UNAVAILABLE)
					SET_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, (i + 16)); //cleared in helper.c if not valid
			}
		}
		else
		{
			CLEAR_BIT(&ctlMem->m_bit_sets.m_capDickheadNotify_instantReviveCap, i);
			CLEAR_BIT(&ctlMem->m_bit_sets.m_offhostKicked_triedCrashingHostsFreemode, i + 16);
#ifdef Developer
			CLEAR_BIT(&ctlMem->m_bit_sets.m_offhostKicked_triedCrashingHostsFreemode, i); //offhost kick only available in dev-build
#endif
		}
	}
}

bool getCustomSpawnPointCoords(vector4 *spawn)
{
	if (GET_ONLINE_SCORE(141) != 1) //we aint saved coords
		return false;

	for (int i = 0; i < 4; i++)
		if (GET_ONLINE_SCORE(i + 142) == -99)
			return false;

	bitSet(&ctlMem->m_bit_sets.m_menuPreferences, PREF_CUSTOM_SPAWN); //update the setting incase user doesn't have autoload when online switched on.

	unsigned int coord_scores[4];
	for (int i = 0; i < 4; i++)
		coord_scores[i] = GET_ONLINE_SCORE(i + 142);

	memcpy(spawn, coord_scores, sizeof(vector4));

	return true;
}

void setCustomSpawnPoint(int unused1, int unused2)
{
	//int updateMenuPrefs = GET_ONLINE_SCORE(cartel_online_score_id_mMenuPrefsBitSet);
	if (!NetworkGame)
	{
		printf("~r~CTL: ~w~Set this when online, we don't store it in the gamesave for carryover to MP.");
		return;
	}
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_CUSTOM_SPAWN))
	{
		float savePoint[4];
		GET_CHAR_COORDINATES(myPlayerPed, &savePoint[0], &savePoint[1], &savePoint[2]);
		GET_CHAR_HEADING(myPlayerPed, &savePoint[3]);
		unsigned int coord_scores[4];
		//use score 142 fuck it (2 3 4 5) R* uses 146-9 i think
		memcpy(coord_scores, savePoint, sizeof(vector4));
		for (int i = 0; i < 4; i++)
		{
			SET_ONLINE_SCORE((i + 142), coord_scores[i]);
		}
		SET_ONLINE_SCORE(141, 1); //signify stored
		bitSet(&ctlMem->m_bit_sets.m_menuPreferences, PREF_CUSTOM_SPAWN);
		printf("~r~CTL: ~w~Spawn point location saved.");
	}
	else
	{
		bitReset(&ctlMem->m_bit_sets.m_menuPreferences, PREF_CUSTOM_SPAWN);
		SET_ONLINE_SCORE(141, -99);
		printf("~r~CTL: ~w~Custom spawn point cleared.");
	}
}

void teleportCustomSpawnpoint(int unused1, int unused2)
{
	if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_CUSTOM_SPAWN))
	{
		vector4 loc;
		if (getCustomSpawnPointCoords(&loc))
			TeleportLocalPlayerOnFoot(loc.x, loc.y, loc.z, loc.h);
		else
			printf("~r~CTL: ~w~It helps if you give us a location first %s, ~w~we lost our crystal ball..", NetworkPlayerGetNameWithColour(myPlayerIndex));
	}
}

//BACKUP
/*void save_bits_to_online_scores(int unused1, int unused2)
{
	if (NetworkGame)
	{
		int uploadbitSetPlayer = ctlMem->m_bit_sets.m_player;
		int uploadbitSetVehicle = ctlMem->m_bit_sets.m_vehicle;
		bitReset(&uploadbitSetPlayer, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED);
		bitReset(&uploadbitSetVehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);
		bitReset(&uploadbitSetPlayer, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);
		SET_ONLINE_SCORE(cartel_online_score_id_bitSet_storage_player, uploadbitSetPlayer);
		SET_ONLINE_SCORE(cartel_online_score_id_bitSet_storage_vehicle, uploadbitSetVehicle);
		SET_ONLINE_SCORE(cartel_online_bitset_score_id, onlineScoreBitSet);
		SET_ONLINE_SCORE(cartel_online_score_id_mMenuPrefsBitSet, ctlMem->m_bit_sets.m_menuPreferences);
		int uploadStickyBitSet = *(int*)stickyBitSet;
		bitReset(&uploadStickyBitSet, STICK_XUID_STORED);
		bitReset(&uploadStickyBitSet, STICK_XEX_PRESENT); //we will clear this but won't pull it.
		SET_ONLINE_SCORE(cartel_online_score_id_stickyBitSet, uploadStickyBitSet);

		SET_ONLINE_SCORE(cartel_online_score_id_weather, time_weather_data.m_weather); //could throw these 3 into a byteset no doubt.
		SET_ONLINE_SCORE(cartel_online_score_id_time_hour, time_weather_data.m_timeHour);
		SET_ONLINE_SCORE(cartel_online_score_id_time_minute, time_weather_data.m_timeMinute);

		SET_ONLINE_SCORE(cartel_online_score_id_delete_distance, delete_distance);

		freeMem->m_carryover_mode = SAVE_CARRY_TO_SP;

		printf("~r~CTL: ~w~Settings saved to GTA IV servers.");
	}
	else
		printf("~r~CTL: ~w~This only works online.");
}*/

void save_bits_to_online_scores(int unused1, int unused2)
{
	if (IS_PLAYER_ONLINE()) //was IS_NETWORK_SESSION
	{
#ifdef Developer
		PRINTSTRING("saving bits to online scores\n");
		PRINTSTRING("Start: save_bits_to_online_scores(), ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES == ");
		PRINTINT(IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES));
		PRINTNL();
#endif
		saveData_storage storeData;
		int uploadbitSetPlayer = ctlMem->m_bit_sets.m_player;
		int uploadbitSetVehicle = ctlMem->m_bit_sets.m_vehicle;
		bitReset(&uploadbitSetPlayer, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED);
		bitReset(&uploadbitSetVehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);
		bitReset(&uploadbitSetPlayer, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);
		storeData.m_playerBits = uploadbitSetPlayer;
		storeData.m_vehicleBits = uploadbitSetVehicle;
		SET_ONLINE_SCORE(cartel_online_bitset_score_id, onlineScoreBitSet);
		storeData.m_menuPreferencesBits = ctlMem->m_bit_sets.m_menuPreferences;
		storeData.m_weaponBits = ctlMem->m_bit_sets.m_weapons;
		//SET_ONLINE_SCORE(cartel_online_score_id_bitSet_storage_player, uploadbitSetPlayer);
		//SET_ONLINE_SCORE(cartel_online_score_id_bitSet_storage_vehicle, uploadbitSetVehicle); //reset these to default.
		//SET_ONLINE_SCORE(cartel_online_bitset_score_id, onlineScoreBitSet);
		//SET_ONLINE_SCORE(cartel_online_score_id_mMenuPrefsBitSet, ctlMem->m_bit_sets.m_menuPreferences);
		int uploadStickyBitSet = *(int*)stickyBitSet;
		bitReset(&uploadStickyBitSet, STICK_XUID_STORED);
		bitReset(&uploadStickyBitSet, STICK_XEX_PRESENT); //we will clear this but won't pull it.
		storeData.m_stickyBits = uploadStickyBitSet;
		//SET_ONLINE_SCORE(cartel_online_score_id_stickyBitSet, uploadStickyBitSet);
		storeData.SBYT[SBYT_TIME_HOUR] = (byte)time_weather_data.m_timeHour;
		storeData.SBYT[SBYT_TIME_MINUTE] = (byte)time_weather_data.m_timeMinute;
		storeData.SBYT[SBYT_WEATHER] = (byte)time_weather_data.m_weather;
		storeData.SBYT[SBYT_SAVE_DATA_VERSION] = SAVE_DATA_VERSION_BYTE;
		storeData.SBYT[SBYT_MENU_BUTTON_HOLD] = (byte)Container.Ui.OpenButton1;
		storeData.SBYT[SBYT_MENU_BUTTON_PRESS] = (byte)Container.Ui.OpenButton2;
		storeData.SBYT[SBYT_HEADER_COL_1] = (byte)Container.Ui.HeaderColor.r;
		storeData.SBYT[SBYT_HEADER_COL_2] = (byte)Container.Ui.HeaderColor.g;
		storeData.SBYT[SBYT_HEADER_COL_3] = (byte)Container.Ui.HeaderColor.b;
		storeData.SBYT[SBYT_HEADER_ALPHA] = (byte)Container.Ui.HeaderColor.a;
		storeData.SBYT[SBYT_HEADERTXT_COL_1] = (byte)Container.Ui.HeaderTextColor.r;
		storeData.SBYT[SBYT_HEADERTXT_COL_2] = (byte)Container.Ui.HeaderTextColor.g;
		storeData.SBYT[SBYT_HEADERTXT_COL_3] = (byte)Container.Ui.HeaderTextColor.b;
		storeData.SBYT[SBYT_HEADERTXT_ALPHA] = (byte)Container.Ui.HeaderTextColor.a;
		storeData.SBYT[SBYT_SUBTITLE_COL_1] = (byte)Container.Ui.SubtitleColor.r;
		storeData.SBYT[SBYT_SUBTITLE_COL_2] = (byte)Container.Ui.SubtitleColor.g;
		storeData.SBYT[SBYT_SUBTITLE_COL_3] = (byte)Container.Ui.SubtitleColor.b;
		storeData.SBYT[SBYT_SUBTITLE_ALPHA] = (byte)Container.Ui.SubtitleColor.a;
		storeData.SBYT[SBYT_TEXT_COL_1] = (byte)Container.Ui.TextColor.r;
		storeData.SBYT[SBYT_TEXT_COL_2] = (byte)Container.Ui.TextColor.g;
		storeData.SBYT[SBYT_TEXT_COL_3] = (byte)Container.Ui.TextColor.b;
		storeData.SBYT[SBYT_TEXT_ALPHA] = (byte)Container.Ui.TextColor.a;
		storeData.SBYT[SBYT_BACKGROUND_COL_1] = (byte)Container.Ui.BackgroundColor.r;
		storeData.SBYT[SBYT_BACKGROUND_COL_2] = (byte)Container.Ui.BackgroundColor.g;
		storeData.SBYT[SBYT_BACKGROUND_COL_3] = (byte)Container.Ui.BackgroundColor.b;
		storeData.SBYT[SBYT_BACKGROUND_ALPHA] = (byte)Container.Ui.BackgroundColor.a;
		storeData.SBYT[SBYT_SEL_TEXT_COL_1] = (byte)Container.Ui.SelectedTextColor.r;
		storeData.SBYT[SBYT_SEL_TEXT_COL_2] = (byte)Container.Ui.SelectedTextColor.g;
		storeData.SBYT[SBYT_SEL_TEXT_COL_3] = (byte)Container.Ui.SelectedTextColor.b;
		storeData.SBYT[SBYT_SEL_TEXT_ALPHA] = (byte)Container.Ui.SelectedTextColor.a;
		storeData.SBYT[SBYT_SCROLLBAR_COL_1] = (byte)Container.Ui.ScrollBarColor.r;
		storeData.SBYT[SBYT_SCROLLBAR_COL_2] = (byte)Container.Ui.ScrollBarColor.g;
		storeData.SBYT[SBYT_SCROLLBAR_COL_3] = (byte)Container.Ui.ScrollBarColor.b;
		storeData.SBYT[SBYT_SCROLLBAR_ALPHA] = (byte)Container.Ui.ScrollBarColor.a;

		storeData.SBYT[SBYT_CHAT_MSG_DISPLAY_SECONDS] = (byte)ctlMem->m_chat_msg_display_time_seconds;

		byte* HealthColor = (byte*)GetHealthHUDColourAddress();
		storeData.SBYT[SBYT_HEALTH_COL_1] = HealthColor[0];
		storeData.SBYT[SBYT_HEALTH_COL_2] = HealthColor[1];
		storeData.SBYT[SBYT_HEALTH_COL_3] = HealthColor[2];

		byte* ArmorColor = (byte*)GetArmourHUDColourAddress();
		storeData.SBYT[SBYT_ARMOUR_COL_1] = ArmorColor[0];
		storeData.SBYT[SBYT_ARMOUR_COL_2] = ArmorColor[1];
		storeData.SBYT[SBYT_ARMOUR_COL_3] = ArmorColor[2];

		byte* PauseColor = (byte*)0x82FB30BD; //this is the one we set / gets copied over when user sets
		storeData.SBYT[SBYT_PAUSE_TXT_COL_1] = PauseColor[0];
		storeData.SBYT[SBYT_PAUSE_TXT_COL_2] = PauseColor[1];
		storeData.SBYT[SBYT_PAUSE_TXT_COL_3] = PauseColor[2];

		storeData.menuPos = Container.Ui.DrawPos;

		//SET_ONLINE_SCORE(cartel_online_score_id_weather, time_weather_data.m_weather); //could throw these 3 into a byteset no doubt.
		//SET_ONLINE_SCORE(cartel_online_score_id_time_hour, time_weather_data.m_timeHour);
		//SET_ONLINE_SCORE(cartel_online_score_id_time_minute, time_weather_data.m_timeMinute);

		SET_ONLINE_SCORE(cartel_online_score_id_delete_distance, delete_distance);

		freeMem->m_carryover_mode = SAVE_CARRY_TO_SP;

		int* data_array = (int*)&storeData;
		const uint base_score = (myCurrentEpisode == 0 ? 115 : 90);
		for (int i = 0; i < 25; i++)
		{
			SET_ONLINE_SCORE(base_score + i, data_array[i]);
		}
		UploadScores();
		printf("~r~CTL: ~w~Settings saved to GTA IV servers.");
	}
	else
		printf("~r~CTL: ~w~This only works when signed in online.");

#ifdef Developer
	PRINTSTRING("End: save_bits_to_online_scores(), ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES == ");
	PRINTINT(IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES));
	PRINTNL();
#endif
}

/*
void read_bits_from_online_scores(int unused1, int unused2) //for some reason this runs absolutely fine automatically (behind loading screen), but doesn't when called through the menu wtf
{
#ifdef Developer
	PRINTSTRING("Reading bits from online scores\n");
	PRINTSTRING("Start: read_bits_from_online_scores(), ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES == ");
	PRINTINT(IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES));
	PRINTNL();
#endif
	if (NetworkGame)
	{ //Make sure CarryOverSaveSettings has been running (runs in main while loading screen faded so should be fine).
		unsigned int score_data[25];
		const uint base_score = (myCurrentEpisode == 0 ? 115 : 90);
		for (int i = 0; i < 25; i++) 
			score_data[i] = GET_ONLINE_SCORE(base_score + i);

		saveData_storage* saveData = (saveData_storage*)score_data; //works fine with automatic loading.

		if (saveData->SBYT[SBYT_SAVE_DATA_VERSION] == SAVE_DATA_VERSION_BYTE)
		{
			//bool MIDMatchedAdminSet = bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SECURITY_BIT_MACHINE_ID_MATCHED); //not touched by this func.
			bool StoreXUIDFuncCalled = bitTest(ctlMem->m_bit_sets.m_player, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);
			bool MIDsecurityBit = bitTest(ctlMem->m_bit_sets.m_vehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);
			bool XUIDsecurityBit = bitTest(ctlMem->m_bit_sets.m_player, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED);
			ctlMem->m_bit_sets.m_player = saveData->m_playerBits;
			ctlMem->m_bit_sets.m_vehicle = saveData->m_vehicleBits;
			ctlMem->m_bit_sets.m_weapons = saveData->m_weaponBits;

			SET_PLAYER_INVINCIBLE(myPlayerIndex, IS_BIT_SET(ctlMem->m_bit_sets.m_player, playerGodMode)); 
			SET_CHAR_NEVER_TARGETTED(myPlayerPed, bitTest(ctlMem->m_bit_sets.m_weapons, forceNeverTargettedPed));

			playerOptAutoAimPlayers(0, 0); //will rig the shit.
			playerOptAutoAimPeds(0, 0); //TODO: Optimise later when merge loading functions with better pointer work.

			if(bitTest(ctlMem->m_bit_sets.m_vehicle, local_autoflip))
				SET_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (myPlayerIndex + 16));

			if (MIDsecurityBit)
				bitSet(&ctlMem->m_bit_sets.m_vehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);

			onlineScoreBitSet = GET_ONLINE_SCORE(cartel_online_bitset_score_id); //LEAVE AS IS
			delete_distance = GET_ONLINE_SCORE(cartel_online_score_id_delete_distance); //LEAVE AS IS

			bool xuidStored = bitTest(*(int*)stickyBitSet, STICK_XUID_STORED);

			if (XUIDsecurityBit)
				bitSet(&ctlMem->m_bit_sets.m_player, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED); //intentionally ordered annoyingly.

			bool xexPresenceBit = bitTest(*(int*)stickyBitSet, STICK_XEX_PRESENT);
			*(int*)stickyBitSet = saveData->m_stickyBits;

			if (xuidStored)
				bitSet((int*)stickyBitSet, STICK_XUID_STORED);
			if (xexPresenceBit)
				bitSet((int*)stickyBitSet, STICK_XEX_PRESENT); //keep original values.

			time_weather_data.m_timeHour = saveData->SBYT[SBYT_TIME_HOUR];
			time_weather_data.m_timeMinute = saveData->SBYT[SBYT_TIME_MINUTE];
			time_weather_data.m_weather = saveData->SBYT[SBYT_WEATHER];
			Container.Ui.OpenButton1 = saveData->SBYT[SBYT_MENU_BUTTON_HOLD];
			Container.Ui.OpenButton2 = saveData->SBYT[SBYT_MENU_BUTTON_PRESS];
			Container.Ui.HeaderColor.r = saveData->SBYT[SBYT_HEADER_COL_1];
			Container.Ui.HeaderColor.g = saveData->SBYT[SBYT_HEADER_COL_2];
			Container.Ui.HeaderColor.b = saveData->SBYT[SBYT_HEADER_COL_3];
			Container.Ui.HeaderColor.a = saveData->SBYT[SBYT_HEADER_ALPHA];
			Container.Ui.HeaderTextColor.r = saveData->SBYT[SBYT_HEADERTXT_COL_1];
			Container.Ui.HeaderTextColor.g = saveData->SBYT[SBYT_HEADERTXT_COL_2];
			Container.Ui.HeaderTextColor.b = saveData->SBYT[SBYT_HEADERTXT_COL_3];
			Container.Ui.HeaderTextColor.a = saveData->SBYT[SBYT_HEADERTXT_ALPHA];
			Container.Ui.SubtitleColor.r = saveData->SBYT[SBYT_SUBTITLE_COL_1];
			Container.Ui.SubtitleColor.g = saveData->SBYT[SBYT_SUBTITLE_COL_2];
			Container.Ui.SubtitleColor.b = saveData->SBYT[SBYT_SUBTITLE_COL_3];
			Container.Ui.SubtitleColor.a = saveData->SBYT[SBYT_SUBTITLE_ALPHA];
			Container.Ui.TextColor.r = saveData->SBYT[SBYT_TEXT_COL_1];
			Container.Ui.TextColor.g = saveData->SBYT[SBYT_TEXT_COL_2];
			Container.Ui.TextColor.b = saveData->SBYT[SBYT_TEXT_COL_3];
			Container.Ui.TextColor.a = saveData->SBYT[SBYT_TEXT_ALPHA];
			Container.Ui.BackgroundColor.r = saveData->SBYT[SBYT_BACKGROUND_COL_1];
			Container.Ui.BackgroundColor.g = saveData->SBYT[SBYT_BACKGROUND_COL_2];
			Container.Ui.BackgroundColor.b = saveData->SBYT[SBYT_BACKGROUND_COL_3];
			Container.Ui.BackgroundColor.a = saveData->SBYT[SBYT_BACKGROUND_ALPHA];
			Container.Ui.SelectedTextColor.r = saveData->SBYT[SBYT_SEL_TEXT_COL_1];
			Container.Ui.SelectedTextColor.g = saveData->SBYT[SBYT_SEL_TEXT_COL_2];
			Container.Ui.SelectedTextColor.b = saveData->SBYT[SBYT_SEL_TEXT_COL_3];
			Container.Ui.SelectedTextColor.a = saveData->SBYT[SBYT_SEL_TEXT_ALPHA];
			Container.Ui.ScrollBarColor.r = saveData->SBYT[SBYT_SCROLLBAR_COL_1];
			Container.Ui.ScrollBarColor.g = saveData->SBYT[SBYT_SCROLLBAR_COL_2];
			Container.Ui.ScrollBarColor.b = saveData->SBYT[SBYT_SCROLLBAR_COL_3];
			Container.Ui.ScrollBarColor.a = saveData->SBYT[SBYT_SCROLLBAR_ALPHA];

			ctlMem->m_chat_msg_display_time_seconds = saveData->SBYT[SBYT_CHAT_MSG_DISPLAY_SECONDS];

			byte* HealthColor = (byte*)GetHealthHUDColourAddress();
			HealthColor[0] = saveData->SBYT[SBYT_HEALTH_COL_1];
			HealthColor[1] = saveData->SBYT[SBYT_HEALTH_COL_2];
			HealthColor[2] = saveData->SBYT[SBYT_HEALTH_COL_3];

			byte* ArmorColor = (byte*)GetArmourHUDColourAddress();
			ArmorColor[0] = saveData->SBYT[SBYT_ARMOUR_COL_1];
			ArmorColor[1] = saveData->SBYT[SBYT_ARMOUR_COL_2];
			ArmorColor[2] = saveData->SBYT[SBYT_ARMOUR_COL_3];

			byte* PauseColor = (byte*)0x82FB30BD; //this is the one we set
			PauseColor[0] = saveData->SBYT[SBYT_PAUSE_TXT_COL_1];
			PauseColor[1] = saveData->SBYT[SBYT_PAUSE_TXT_COL_2];
			PauseColor[2] = saveData->SBYT[SBYT_PAUSE_TXT_COL_3];
			setPauseColours(0, 0); //carries these over to the other bytes for pause txt colour

			Container.Ui.DrawPos = saveData->menuPos;

			freeMem->m_savedVehicleModel = saveData->m_savedVehicleModel;
			freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits = saveData->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits; //first 10 = upgrades, windows = 4, doors = 4, tyres = 6

			for (int i = 0; i < 4; i++)
				freeMem->m_savedVehicleColours[i] = saveData->m_savedVehicleColours[i];

			ctlMem->m_bit_sets.m_menuPreferences = saveData->m_menuPreferencesBits;

			SWITCH_AMBIENT_PLANES(!bitTest(ctlMem->m_bit_sets.m_vehicle, disabledRunwayPlanes)); //should remain compatible.
			if (StoreXUIDFuncCalled)
				bitSet(&ctlMem->m_bit_sets.m_player, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);

			//DISABLE_PLAYER_AUTO_VEHICLE_EXIT(myPlayerPed, IS_BIT_SET(ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack)); //kudos stuart
			update_menu_open_controls_string();
			load_cartel_chat();

			printf("~r~CTL: ~w~Settings loaded from GTA IV servers.");

			if (!bitTest(*(int*)stickyBitSet, STICK_XEX_PRESENT))
				bitReset(&ctlMem->m_bit_sets.m_player, syncTimeToRealWorld);
		}
	}
	else
		printf("~r~CTL: ~w~This only works online.");

#ifdef Developer
		PRINTSTRING("End: read_bits_from_online_scores(), ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES == ");
		PRINTINT(IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES));
		PRINTNL();
#endif
}
*/

bool DoesNetworkIdExist(NETWORK_INDEX netId)
{
	if (!NetworkGame)
		return false;

	return DOES_VEHICLE_EXIST_WITH_NETWORK_ID(netId) || DOES_OBJECT_EXIST_WITH_NETWORK_ID(netId) || DOES_PED_EXIST_WITH_NETWORK_ID(netId);
}

bool ReleaseControlOfNetworkId(NETWORK_INDEX netId)
{
	if (!DoesNetworkIdExist(netId))
		return false;

	bool has_control = HAS_CONTROL_OF_NETWORK_ID(netId);
	if (has_control)
		SET_NETWORK_ID_CAN_MIGRATE(netId, true);

	return has_control;
}

PLAYER_INDEX FindControllerOfNetworkId(const NETWORK_INDEX Network_ID)
{
	if (DoesNetworkIdExist(Network_ID))
	{
		for (int i = 0; i < 16; i++)
		{
			if (IsPlayerValid(i)) //i keep crashing on DOES_PLAYER_HAVE.... for some reason. so why not add this..
			{
				if (DOES_PLAYER_HAVE_CONTROL_OF_NETWORK_ID(i, Network_ID))
					return i;
			}
		}
	}
	return INVALID_PLAYER_INDEX;
}

bool RequestNetworkControl(const NETWORK_INDEX netId, char* entity_name)
{
	if (!NetworkGame)
		return true;

	if (!DoesNetworkIdExist(netId))
		return true; //checks if the entity in question is only localy.

	if (HAS_CONTROL_OF_NETWORK_ID(netId))
		return true;
	//CanNetworkIdMigrate? 


	if (NETWORK_LEAVE_GAME_PENDING() || DOES_GAME_CODE_WANT_TO_LEAVE_NETWORK_SESSION())
	{
		if (HAS_CONTROL_OF_NETWORK_ID(netId))
		{
			//if crashes occur it could be this, but i imagine we will act on it so quickly it wont matter (if we do at all, one place we do).
			SET_NETWORK_ID_CAN_MIGRATE(netId, true); //attempt to release control when leaving game..
			return true;
		}

		return HAS_CONTROL_OF_NETWORK_ID(netId); //if we'r leaving and deleting we don't have time to request control.
	}

	const uint Timer = GetTickCount() + 1100;
	while (Timer > GetTickCount())
	{
		//maybe try with just if(REQUEST_CONTROL_OF_NETWORK_ID ?
		if (HAS_CONTROL_OF_NETWORK_ID(netId))
			return true;
		else
			REQUEST_CONTROL_OF_NETWORK_ID(netId); //think this might also return true if we obtain control

		WAIT(100);
	}

	const PLAYER_INDEX Controller = FindControllerOfNetworkId(netId);
	if (Controller != INVALID_PLAYER_INDEX)
		printf("~r~CTL: %s ~w~has control of that %s.", NetworkPlayerGetNameWithColour(Controller), entity_name);

	return false;
}

const char* SafeNetworkPlayerGetName(PLAYER_INDEX playerIndex)
{
	const char* playerName = GET_PLAYER_NAME(playerIndex);
	if (IsStrValid(playerName))
		return playerName;

	return "Invalid name";
}

char* SafeNetworkPlayerGetNameWithColour(PLAYER_INDEX playerindex) //could map 16 strings to these and return based on switch case, that way every player has it's own string storage space...
{
	char* str = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	sprintf(str, "~COL_NET_%i~%s\0", (playerindex + 1), SafeNetworkPlayerGetName(playerindex));
	return (char*)str;
}

char* NetworkPlayerGetNameWithColour(PLAYER_INDEX playerindex) //could map 16 strings to these and return based on switch case, that way every player has it's own string storage space...
{
	char* str = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	sprintf(str, "~COL_NET_%i~%s\0", (playerindex + 1), GET_PLAYER_NAME(playerindex));
	return (char*)str;
}

bool SafeToActOnPlayerPed(PLAYER_INDEX player, PED_INDEX* ped)
{
	if (IsPlayerValid(player))
	{
		GET_PLAYER_CHAR(player, ped);
		return DOES_CHAR_EXIST(*ped);
	}

	return false;
}

const char* getName(PLAYER_INDEX playerIndex)
{
	return GET_PLAYER_NAME(playerIndex);
}

uint GetPlayerPointerNath(PLAYER_INDEX playerIndex) //this is called by security functions..
{
	return (uint)getName(playerIndex) - hiddenSeventySix(); //(just one extra jump to fuck them over).
	//WHEN WE REMOVE SECURITY: return (uint)(GET_PLAYER_NAME(playerIndex) - 0x4C); //gets us to the start of the player struct.
}

//used to see if an player has already got a WalkOnWater or WalkOnAir object.
//INFO: Distance staticly can't be lower than 35.0f
bool ScanNetworkObjectPoolForClosestModelToPlayer(PLAYER_INDEX PlayerID, uint searchModel, bool birdsEye, bool searchNonMigratableOnly, float maxDistanceFromPlayer, bool onlyObjectsWithinWalkWaterObjectHeightRange, PLAYER_INDEX *Controller, OBJECT_INDEX *scanobj, float searchObjHealth)
{
	PED_INDEX targetPlayerPed;
	OBJECT_INDEX closestObjectToPed;

	if (!NetworkGame || PlayerID == myPlayerIndex)
		return false;

	if (SafeToActOnPlayerPed(PlayerID, &targetPlayerPed))
	{
		float tX, tY, tZ, distanceFromPlayer, sX, sY, sZ, lowestDistanceFromPlayer = 35.0f;
		uint scanModel;
		CPool* NetObjPool = *(CPool**)0x8318E950; //Network object pool
		byte entityCreatorID = 17; //invalid

		GET_CHAR_COORDINATES(targetPlayerPed, &tX, &tY, &tZ); //targetX, targetY, targetZ

		for (int i = 0; i < NetObjPool->m_maxHandle; i++)
		{
			uint NetPtr = (uint)(NetObjPool->m_firstPointer + (i * NetObjPool->m_structSize));
			uint EntityPtr = *(uint*)(NetPtr + 0x4);

			if (EntityPtr && EntityPtr != 0xCDCDCDCD)
			{
				bool isOneOfOurs = false;

				//int entityType = *(int*)(EntityPtr + 8); //entity type will always be onject in the object pool...
				//short entityModelID = *(short*)(EntityPtr + 0x2E);
				//short entityNetworkID = *(short*)(NetPtr + 12);

				entityCreatorID = *(byte*)(NetPtr + 15); //prob better off using FindControllerOfNetworkId
				OBJECT_INDEX handle = GetHandleFromPool(*(CPool**)0x82DE5A50, EntityPtr); //object pool

				if (!DOES_OBJECT_EXIST(handle))
					continue;

				if (CanObjectMigrate(handle))
					continue; //every single drive walk on water mod i've ever seen locks net control so..

				GET_OBJECT_MODEL(handle, &scanModel);
			//	if (scanModel != searchModel)
				if (scanModel != searchModel && (searchModel != OBJECT_CJ_GAME_GRID && scanModel != OBJECT_CJ_CR_BACKROUND)) //test this
					continue;

				if (searchObjHealth != 0.0f)
				{
					float scan_health;
					GET_OBJECT_HEALTH(handle, &scan_health);
					if (scan_health != searchObjHealth)
						continue;
				}

				GET_OBJECT_COORDINATES(handle, &sX, &sY, &sZ);
				if (onlyObjectsWithinWalkWaterObjectHeightRange && !(sZ > -16.0f && sZ < -8.0f))
					continue; //check Height is within typical Walk on Water object height range.. // SET_OBJECT_COORDINATES(driveWalkWaterObj[i], pX, pY, IS_CHAR_IN_ANY_BOAT(networkPlayerPed) ? -15.0f : -9.2f);

				for (int z = 0; z < 16; z++)
				{
					if (handle == freeMem->m_walk_water_object[z] || handle == freeMem->m_walk_on_air.m_object[z])
					{
						isOneOfOurs = true;
						break; //skips own objects incase on another players waterWalk object.
					}
				}

				if (isOneOfOurs)
					continue;

				if (birdsEye)
					GET_DISTANCE_BETWEEN_COORDS_2D(sX, sY, tX, tY, &distanceFromPlayer);
				else
					GET_DISTANCE_BETWEEN_COORDS_3D(sX, sY, sZ, tX, tY, tZ, &distanceFromPlayer);

				if (distanceFromPlayer < lowestDistanceFromPlayer)
				{
					lowestDistanceFromPlayer = distanceFromPlayer;
					closestObjectToPed = handle;
				}
			}
		}

		if (DOES_OBJECT_EXIST(closestObjectToPed))
		{
			if (lowestDistanceFromPlayer < maxDistanceFromPlayer)
			{
				if (entityCreatorID != 17) //valid.
					*Controller = entityCreatorID;
				else
					*Controller = -1; //isplayer active check will check the range and return false.

				*scanobj = closestObjectToPed;
				return true;
				//object can't migrate, is correct model, and is within the distance limit we specified in the scan.
				//return true;
			}
		}
	}

	return false;
}

int maxSlotsForGameMode(eGameMode gameMode)
{
	switch (gameMode)
	{
	case GAME_MODE_DEATHMATCH: return 16;
	case GAME_MODE_TEAM_DEATHMATCH: return 16;
	case GAME_MODE_MAFIYA_WORK: return 8;
	case GAME_MODE_TEAM_MAFIYA_WORK: return 16;
	case GAME_MODE_TEAM_CAR_JACK_CITY: return 8;
	case GAME_MODE_CAR_JACK_CITY: return 16;
	case GAME_MODE_RACE: return 16;
	case GAME_MODE_GTA_RACE: return 16;
	case GAME_MODE_PARTY_MODE: return 8;
	case GAME_MODE_COPS_AND_CROOKS: return 16;
	case GAME_MODE_TURF_WAR: return 16; //unknown.
	case GAME_MODE_DEAL_BREAKER: return 4;
	case GAME_MODE_HANGMANS_NOOSE: return 4;
	case GAME_MODE_BOMB_DA_BASE_II: return 4;
	case GAME_MODE_FREE_MODE: return 16;
	default:
		return 16;
	}
	return 16;
}

int GetAutoAimPreference()
{
	return READ_LOBBY_PREFERENCE(LOBBY_PREF_AUTO_AIM); //419
}

int GetPolicePreference()
{
	return READ_LOBBY_PREFERENCE(LOBBY_PREF_POLICE); //420
}

int GetFriendlyFirePreference()
{
	return READ_LOBBY_PREFERENCE(LOBBY_PREF_FRIENDLY_FIRE); //422
}

void DoLeaveGameCleanup()
{
	DeleteAllSpawnedEntities();

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		PED_INDEX driver;
		VEHICLE_INDEX myVehicle;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVehicle);
		GET_DRIVER_OF_CAR(myVehicle, &driver);

		if (driver == myPlayerPed) //optimistically we won't need to check net control. 
			DeleteCar(&myVehicle);
	}

	//STOP_STREAM();
	for (int i = 0; i < MAX_RIGGED_BUSSES; i++)
	{
		if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(ctlMem->m_CartelBusses[i].m_attachedBus_NetId) && HAS_CONTROL_OF_NETWORK_ID(ctlMem->m_CartelBusses[i].m_attachedBus_NetId))
		{
			MARK_CAR_AS_NO_LONGER_NEEDED(&ctlMem->m_CartelBusses[i].m_attachedBus);
			SET_NETWORK_ID_CAN_MIGRATE(ctlMem->m_CartelBusses[i].m_attachedBus_NetId, true);
		}
	}
	WAIT(150);
	//SWITCH_STREAMING(false); //try help rgh loading screen woes with all the writing to text files
	//WAIT(200); //give the deletion time to sync.
	SetTellNetResponseEnabled(true);
}

void NetworkGameRejoinSession()
{
	if (!NetworkGame)
		return;

	if (GET_HOST_ID() == myPlayerIndex)
	{
		printf("~r~CTL: ~w~Nope, if you run this as host it'll fuck you up.");
		return;
	}

	DoLeaveGameCleanup();
	SetHostByte(true); //this is needed, takes care of itself as you rejoin etc.
	NETWORK_CHANGE_GAME_MODE_E1(NETWORK_GET_GAME_MODE(), NETWORK_GET_MAX_SLOTS(), NETWORK_GET_MAX_PRIVATE_SLOTS(), 0, GetAutoAimPreference(), GetPolicePreference(), GetFriendlyFirePreference(), 0);
}

void NetworkChangeGameMode()
{
	//from Stu  NETWORK_CHANGE_GAME_MODE_E1(gamemode, 16, myCurrentEpisode, 0, READ_LOBBY_PREFERENCE(419), READ_LOBBY_PREFERENCE(420), READ_LOBBY_PREFERENCE(422), 0);
	NETWORK_CHANGE_GAME_MODE_E1(gameModeSelect, NETWORK_GET_MAX_SLOTS(), NETWORK_GET_MAX_PRIVATE_SLOTS(), 0, GetAutoAimPreference(), GetPolicePreference(), GetFriendlyFirePreference(), 0);
}

bool IsPlayerUsingXboxOne(PLAYER_INDEX playerIndex) 
{
	//return *(uint*)(GetPlayerPointerNath(playerIndex) + 0x1C) != 250;
	return GetPlayerPointer(playerIndex)->m_machineId[1] != 250;
	//return *(uint*)(GET_PLAYER_NAME(playerIndex) - 0x30) != 250; //from ryan but dont need it.
	//return *(uint*)(GET_PLAYER_NAME(playerIndex) - 0xD0); //lol I got this wrong - Nath
}

char* GetGamemodeScriptName(int gameMode) //used for swapping gamemodes script thread/execution order with helper / OUT OF BOUNDS Suggest/Kick protection for retail.
{
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL: return "freemode_cr";
	case EPISODE_TLAD: return "e1_freemode";
	case EPISODE_TBOGT: return "e2_freemode";
	}
	return "Unknown";
}

void NetworkRunSearch(int param)
{
	//TODO Nath add support for diff gamemodes.
	NETWORK_FIND_GAME_E1(GAME_MODE_FREE_MODE, FALSE, myCurrentEpisode, 0, GetAutoAimPreference(), GetPolicePreference(), GetFriendlyFirePreference(), 0);
	while (NETWORK_FIND_GAME_PENDING())
		WAIT(0);
}

void NetworkJoinGame(int desired_session, int unused1)
{
	CTL_SET_SESSION(desired_session); 
	SHUTDOWN_AND_LAUNCH_NETWORK_GAME(myCurrentEpisode);
}


void SpoofGamertag(const char* tag)
{
	strcpy((char*)0x8318AB44, tag, 16);
	printf("~r~CTL: ~w~Gamertag ~y~%s ~w~written.", (char*)0x8318AB44);
}


bool HasStoredXUIDAndGamertagInFreeMem()
{
	bitSet(&ctlMem->m_bit_sets.m_player, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);
	return bitTest(*(int*)stickyBitSet, STICK_XUID_STORED);
}

void StoreXUIDAndGamertagInFreeMem() //called at script start, and will only run if nothing stored there already.
{
	if (HasStoredXUIDAndGamertagInFreeMem()) //data already copied in.
		return;

	bitSet((int*)stickyBitSet, STICK_ADVERTISE_MENU);
	bitSet((int*)stickyBitSet, STICK_XUID_STORED); //if we ever save this shit make sure this doesn't get saved!
	bitSet((int*)stickyBitSet, STICK_USE_CARTEL_SPAWN);

	memcpy((unsigned char*)ByteAndXUIDGTFreeMemoryAddress, (unsigned char*)0x8318AB38, 32); //copy XUID and GT in one go.
	//*(byte*)(ByteAndXUIDGTFreeMemoryAddress + 32) = 1; 	//Mark as stored
}


void RestoreXUIDAndGamertagInFreeMem()
{
	if (HasStoredXUIDAndGamertagInFreeMem())
	{
		memcpy((unsigned char*)0x8318AB38, (unsigned char*)ByteAndXUIDGTFreeMemoryAddress, 32); // restore XUID and GT in one go
		printf("~r~CTL: ~w~Gamertag & XUID restored (%s).", (char*)(ByteAndXUIDGTFreeMemoryAddress + 12));
	}
#ifdef Developer
	else
		printf("~r~CTL ERROR: ~w~The byte we set to signify data presence is not 1, have you been poking? You naughty creature you..");
#endif
}

void SpoofXUID(char* xuidString)  //should make a Read XUID func
{
	char XuidFirstHalf[9] = { 0 }; // One extra for null terminator '\0'

	for (int i = 0; i < 8; i++)
		XuidFirstHalf[i] = xuidString[i];

	char* XuidSecondHalf = xuidString + 8;

	unsigned char* address = (unsigned char*)0x8318AB38;

	int firstHalf = HexToIntNath(XuidFirstHalf);
	int secondHalf = HexToIntNath(XuidSecondHalf);

	// Manually set the bytes for XuidFirstHalf
	address[0] = (firstHalf >> 24) & 0xFF;
	address[1] = (firstHalf >> 16) & 0xFF;
	address[2] = (firstHalf >> 8) & 0xFF;
	address[3] = firstHalf & 0xFF;

	// Manually set the bytes for XuidSecondHalf
	address[4] = (secondHalf >> 24) & 0xFF;
	address[5] = (secondHalf >> 16) & 0xFF;
	address[6] = (secondHalf >> 8) & 0xFF;
	address[7] = secondHalf & 0xFF;

	printf("~r~CTL: ~w~XUID ~y~0x%s ~w~written.", xuidString);
}

void NetworkOptSpoofXUIDTextInput()
{
	if (showAndGetKeyboardInput("Spoof (XUID)", true, "", 16, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		if (GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer) == 16)
			SpoofXUID(ctlMem->m_char_buffers.keyboard_input_buffer);
		else
		{
			ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
			printf("~r~CTL: ~s~Length must be 16!");
		}
	}
}


void NetworkOptsSpoofGamertagTextInput()
{
	if (showAndGetKeyboardInput("Spoof (Gamertag)", false, GET_PLAYER_NAME(myPlayerIndex), 15, 0, 0.5f, 0.5f, 200, 200, 200))
		SpoofGamertag(ctlMem->m_char_buffers.keyboard_input_buffer);
}

void NetworkOptsSpoofGTandXUIDTextInput()
{
	if (showAndGetKeyboardInput("Spoof (Gamertag + XUID)", false, GET_PLAYER_NAME(myPlayerIndex), 15, 0, 0.5f, 0.5f, 200, 200, 200))
	{
		if (CTL_SPOOF_XUID_GIVEN_GAMERTAG(ctlMem->m_char_buffers.keyboard_input_buffer))
			SpoofGamertag(ctlMem->m_char_buffers.keyboard_input_buffer);
		else
			printf("~r~CTL: ~w~Unable to resolve XUID (%s).", ctlMem->m_char_buffers.keyboard_input_buffer);
	}
}

void NetworkOptSpoofXuidTextInput()
{
	if (showAndGetKeyboardInput("Spoof (Gamertag)", false, GET_PLAYER_NAME(myPlayerIndex), 15, 0, 0.5f, 0.5f, 200, 200, 200))
	{
		if (CTL_SPOOF_XUID_GIVEN_GAMERTAG(ctlMem->m_char_buffers.keyboard_input_buffer))
			printf("~r~CTL: ~w~XUID of %s retreived and set.", ctlMem->m_char_buffers.keyboard_input_buffer);
		else
			printf("~r~CTL: ~w~Unable to resolve XUID (%s).", ctlMem->m_char_buffers.keyboard_input_buffer);
	}
}


void NetworkOptSpoofRecentPlayer(int metPlayerIndex, int unused1)
{
	SpoofGamertag(NETWORK_GET_MET_PLAYER_NAME(metPlayerIndex));
}

void NetworkOptOpenRecentPlayerProfile(int metPlayerIndex, int unused1)
{
	NETWORK_SHOW_MET_PLAYER_PROFILE_UI(metPlayerIndex);
}

void NetworkOptOpenRecentPlayerFeedback(int metPlayerIndex, int unused1)
{
	NETWORK_SHOW_MET_PLAYER_FEEDBACK_UI(metPlayerIndex);
}

void NetworkGameOptFindNewGame(int unused1, int unused2)
{
	DoLeaveGameCleanup();
	SHUTDOWN_AND_LAUNCH_NETWORK_GAME(episodeSelect);
}

void NetworkGameOptSingleplayer(int unused1, int unused2)
{
	if (!NetworkGame)
		return;

	DoLeaveGameCleanup();
	SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME();
}

int GetNumberOfKickedConsoles()
{
	int count = 0;

	for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
		if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID != 0)
			count++;

	return count;
}

uint GetByteSetH1(uint ByteSet, int byteIndex)
{
	return (ByteSet >> (8 * byteIndex)) & 0xFF;
}

/*typedef struct {
    IN_ADDR     ina;                        // IP address (zero if not static/DHCP)
    IN_ADDR     inaOnline;                  // Online IP address (zero if not online)
    WORD        wPortOnline;                // Online port
    BYTE        abEnet[6];                  // Ethernet MAC address
    BYTE        abOnline[20];               // Online identification
} XNADDR;*/

int GetPlayerLocalIpAddress(PLAYER_INDEX playerIndex)
{
	return *(int*)(GetPlayerPointerNath(playerIndex));
}

/*
int GetPlayerIpAddress(PLAYER_INDEX playerIndex) //works fine, returns IP address as an integer.
{
#ifdef Developer
	return GetPlayerPointer(playerIndex)->m_wan_ip; //for use once security nuked
#else
	return *(int*)(GetPlayerPointerNath(playerIndex) + 4);
#endif
}
*/
int GetPlayerPort(PLAYER_INDEX playerIndex)
{
	return *(short*)(GetPlayerPointerNath(playerIndex) + 8);
}

int GetPlayerMachineId(PLAYER_INDEX playerIndex) //works fine for comparing lower half (Only unique half for 360).
{
#ifdef Developer
	return GetPlayerPointer(playerIndex)->m_machineId[0]; //for uswe once security nuked
#else
	return *(int*)(GetPlayerPointerNath(playerIndex) + 0x18);
#endif
}

int GetPlayerXuid(PLAYER_INDEX playerIndex)
{
	//return (*(int*)(GetPlayerPointerNath(playerIndex) + 0x40));  //this is the first half that's the same for most XUID's
	return *(int*)(GetPlayerPointerNath(playerIndex) + 0x44); //second half.
}

/*
void GetPlayerXuidFromPlayerStruct(PLAYER_INDEX player, int *XUID1, int *XUID2)
{
	*XUID1 = *(int*)(GetPlayerPointerNath(player) + 0x40);
	*XUID2 = *(int*)(GetPlayerPointerNath(player) + 0x44); //second half.
}
//use IP_as_Int32(a,b,c,d);
bool PlayerIpAddressMatch(PLAYER_INDEX playerId, uint ip0, uint ip1, uint ip2, uint ip3) //checks if IP octets match what is specified
{
	uint IP[4];
	int ip = GetPlayerIpAddress(playerId);
	GetByteSetH2(ip, &IP[3], &IP[2], &IP[1], &IP[0]);
	return ip0 == IP[0] && ip1 == IP[1] && ip2 == IP[2] && ip3 == IP[3];
}
*/

//GT: JMT X Monsters - XUID: 901FBC1D95154 - IP: 189.217.194.98 - MID: FA00EFF3D523F20E (Xbox One) //okay im told..
//GT: o STORMHAWK957 - XUID: 901F2825DD730 - IP: 189.217.194.98 - MID: FA00EFF3D523F20E (Xbox One) //okay im told..
//MID1 == 0x0EF223D5 && MID2 == 0xF3EF00FA

//INFO: Only call this is the player is valid!
void logPlayer(PLAYER_INDEX playerIndex)
{
	if (!CartelXEXPresent) //ideally we already check this externally.
		return;

	//if(playerIndex == myPlayerIndex) //ideally this won't be called unless this is the case.
		//return;  //checked externally (for now)

#ifdef PlayerDB
	if (!CTL_IS_BUSY() && !IS_BIT_SET(ctlMem->m_bit_sets.m_UploadedData_UNUSED, playerIndex))
	{
		CPlayerInfo* player = GetPlayerPointer(playerIndex);
		if (player == NULL)
			return;

		//char* playerName = IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, (playerIndex + 16)) ? ctlMem->m_char_buffers.player_name_backup[playerIndex] : player->m_gamertag; //If we've fucked with the name we want to log the real one.
		char* playerName = ctlMem->m_char_buffers.player_name_backup[playerIndex][0] != '\0' ? ctlMem->m_char_buffers.player_name_backup[playerIndex] : player->m_gamertag; //If we've fucked with the name we want to log the real one.
		char* WAN_IP_Buff = BUFFER16;
		sprintf(WAN_IP_Buff, "%b.%b.%b.%b", player->wan_ip_octets[0], player->wan_ip_octets[1], player->wan_ip_octets[2], player->wan_ip_octets[3]);
		char* LAN_IP_Buff = BUFFER16;
		sprintf(LAN_IP_Buff, "%b.%b.%b.%b", player->lan_ip_octets[0], player->lan_ip_octets[1], player->lan_ip_octets[2], player->lan_ip_octets[3]);
		char* XUID_Buff = BUFFER20;
		sprintf(XUID_Buff, "0x%x%x", player->m_xuid[0], player->m_xuid[1]);

		char* console_string_PPRX = (player->m_machineId[1] != 250) ? "Xbox One (or Later)" : "Xbox 360";
		char* MID_Buff = BUFFER20;
		sprintf(MID_Buff, "0x%x%x", player->m_machineId[0], player->m_machineId[1]);
		if (CTL_PROVIDE_PLAYER(playerName, XUID_Buff, console_string_PPRX, MID_Buff, WAN_IP_Buff, LAN_IP_Buff, &player->_2port_6mac_2pad[2]))
			SET_BIT(&ctlMem->m_bit_sets.m_UploadedData_UNUSED, playerIndex);
#endif
	}
}

//BACKUP BELOW! - 26-Jan-2026
//const char* playerName = IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, (PlayerID + 16)) ? ctlMem->m_char_buffers.player_name_backup[PlayerID] : GET_PLAYER_NAME(PlayerID);
/*void logPlayer(PLAYER_INDEX playerId)
{
	if(CartelXEXPresent)
	{
#ifdef PlayerDB
		if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, playerId) || (!CTL_IS_BUSY() && !IS_BIT_SET(ctlMem->m_bit_sets.m_UploadedData_UNUSED, playerId)))
#else
		if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, playerId))
#endif
		{ //Do we still need to write the player to the text file, or upload them to PlayerDB?
			if (playerId != myPlayerIndex)
			{
#ifdef Developer
				CPlayerInfo* player = GetPlayerPointer(playerId); //Use this in normal build once security is laxed
#else
				CPlayerInfo* player = (CPlayerInfo*)GetPlayerPointerNath(playerId);
#endif
				if (player == NULL)
					return;


				char* playerDataString = BUFFERMAX; //off to the log file this goes.
				char* WAN_IP_Buff = BUFFER16;
				sprintf(WAN_IP_Buff, "%b.%b.%b.%b", player->wan_ip_octets[0], player->wan_ip_octets[1], player->wan_ip_octets[2], player->wan_ip_octets[3]);
				char* LAN_IP_Buff = BUFFER16;
				sprintf(LAN_IP_Buff, "%b.%b.%b.%b", player->lan_ip_octets[0], player->lan_ip_octets[1], player->lan_ip_octets[2], player->lan_ip_octets[3]);
				char* XUID_Buff = BUFFER20;
				sprintf(XUID_Buff, "0x%x%x", player->m_xuid[0], player->m_xuid[1]);
				char* MID_Buff = BUFFER20;
				sprintf(MID_Buff, "0x%x%x", player->m_machineId[0], player->m_machineId[1]);
				char* console_string = BUFFER8;
				char* console_string_PPRX = BUFFER20;
				if (player->m_machineId[1] != 250) //they are on Xbox One
				{
					console_string = "[One]";
					console_string_PPRX = "Xbox One (or Later)";
				}
				else
				{
					console_string = "[360]";
					console_string_PPRX = "Xbox 360";
				}
#ifndef Developer
				if (!bitTest(adminRemoteOnlineBitSet, BIT_ADMREMOTE_MODERATOR_DISABLED))
				{
#endif
#ifdef Moderator
					if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, playerId))
					{
						sprintf(playerDataString, "%s - %s - XUID %s - IP %s - MID 0x%x 0x%x - MAC %m-%m-%m-%m-%m-%m - LAN %s", console_string, player->m_gamertag, XUID_Buff, WAN_IP_Buff, player->m_machineId[0], player->m_machineId[1], player->_2port_6mac_2pad[2], player->_2port_6mac_2pad[3], player->_2port_6mac_2pad[4], player->_2port_6mac_2pad[5], player->_2port_6mac_2pad[6], player->_2port_6mac_2pad[7], LAN_IP_Buff);
						CTL_WRITE_TO_FILE("ctl_player_log.txt", playerDataString);
						SET_BIT(&ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, playerId);
					}
#else
					SET_BIT(&ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, playerId);
#endif
#ifdef PlayerDB
					if (!IS_BIT_SET(ctlMem->m_bit_sets.m_UploadedData_UNUSED, playerId))
					{
						if (CTL_PROVIDE_PLAYER(player->m_gamertag, XUID_Buff, console_string_PPRX, MID_Buff, WAN_IP_Buff, LAN_IP_Buff, &player->_2port_6mac_2pad[2]))
							SET_BIT(&ctlMem->m_bit_sets.m_UploadedData_UNUSED, playerId);
					}
#endif
#ifndef Developer
				}
#endif
			}
		}
	}
}*/

//from PPRX
/*0xD8903997) return "BnidbyanFs (Xbox One)";            //BnidbyanFs (Xbox One)
0x1DC56E04) return "BnidbyanFs (0x1DC56E04)";        //BnidbyanFs (RGH)
0x3B23C304) return "BnidbyanFs (0x3B23C304)";        //BnidbyanFs (RGH)
0x70A89802) return "BnidbyanFs (0x70A89802)";        //BnidbyanFs (RGH)
0x907B7A03) return "BnidbyanFs (0x907B7A03)";        //BnidbyanFs (RGH)
0xCC851000) return "BnidbyanFs (0xCC851000)";        //BnidbyanFs (RGH)
0xC9521405) return "BnidbyanFs (0xC9521405)";        //BnidbyanFs (RGH)
0xE9BF5C04) return "BnidbyanFs (0xE9BF5C04)";            //BnidbyanFs (RGH)
0xDCA7BA04) return "BnidbyanFs (0xDCA7BA04)";        //BnidbyanFs (RGH)
0x129757E0) return "BnidbyanFs (0x129757E0)";           //BnidbyanFs (RGH)

//0xC9F8C08A) return "JMT Jafer (Xbox One)";        //JMT Jafer (Xbox One) //done
//0x0C76A431) return "JMT Viper (Xbox One)";        //JMT Viper (Xbox One) //done
//0x040C0F05) return "JMT Viper (0x040C0F05)";        //JMT Viper (RGH) //done
//0xA9521405) return "JMT Viper (0xA9521405)";        //JMT Viper (RGH) //done
//0x3E3E3A03) return "JMT Viper (0x3E3E3A03)";        //JMT Viper (RGH) //done

0xA180A304) return "h5ra";                //h5ra (RGH or Hotswap)*/

int PlayerIsADickhead(PLAYER_INDEX playerId, char* *name)
{
#ifdef Developer
	CPlayerInfo* player = GetPlayerPointer(playerId); //Use this in normal build once security is laxed
#else
	CPlayerInfo* player = (CPlayerInfo*)GetPlayerPointerNath(playerId);
#endif

	if (player == NULL)
		return DICKLIST_UNAVAILABLE;
	//if(player != nullptr) //we should check this externally as we set bit checked in the func that calls this (DickheadScannerLoop)
	uint nameHash = GET_HASH_KEY(player->m_gamertag);
	bool xboxOne = (player->m_machineId[1] != 250);

	int mac[6];
	for (int i = 0; i < 6; i++)
		mac[i] = player->_2port_6mac_2pad[i + 2];


		//IP Saudi arabia, JMT x VIPER is a saudi? really? i doubt it //Ibrahim Jafer IRL name
		//[MEMES] 13/11/24 11:29 PM - GT: JMT X VIPER - XUID: 000901F78D6E47B6 - IP: 51.36.222.159 - MID: FA00000051452A9 (Xbox 360) 
	//[CARTEL 20/03/2025 18:30] [One] - NixKay - XUID 000901FE6B0DF1BB - IP 85.201.113.163 - MID 0xB3204B28 0xB6EF00FA - MAC D8-E2-DF-7B-57-2F - LAN 192.168.1.21
	//[CARTEL 20/03/2025 18:19] [360] - SomonDeNorvege - XUID 000901F97233DC84 - IP 85.201.113.163 - MID 0x81A83804 0x000000FA - MAC 7C-ED-8D-0B-F7-53 - LAN 192.168.1.26
	//JMT X VIPER XBOX ONE MACHINE ID	                     	//JMT x Viper alt (Guiliano3195) //TarsaILime61 (fake impersonator acc JMT is using). / SevenOctoper
	//if (	//nameHash == 0xDDA66B49 //NixKay //000901FE6B0DF1BB
		//|| (ids->xuid[0] == 0x000901FE && ids->xuid[1] == 0x6B0DF1BB) //NixKay XUID //000901FE6B0DF1BB
		//|| nameHash == 0xCE0966A1 //SomonDeNorvege 
		//|| (ids->xuid[0] == 0x000901F9 && ids->xuid[1] == 0x7233DC84) //SomonDeNorvage XUID //000901F97233DC84
	/*
	bnid 28:EA:0B 9B:50:4B //yup in db
               C4:9D:ED:E3:D9:C1 //not in db
	viper 
04:27:28:D5:18:E6 //yup in db
 E4:2A:AC:4 A:5E:CE //not in db
  20:16:42-22:C1:0E //yup in db*/
	if( nameHash == 0x81D9DC37 //SevenOctoper
	|| (player->m_xuid[0] == 0x000901F7 && player->m_xuid[1] == 0x5DEB5896) //SevenOctoper XUID  //000901F75DEB5896
	|| nameHash == 0xE391DE7E //JMT X VIPER
	|| (player->m_xuid[0] == 0x000901F7 && player->m_xuid[1] == 0x8D6E47B6) //JMT X VIPER - XUID  //000901F78D6E47B6
	|| nameHash == 0x86B99F7C //JimmyRS126
	|| (mac[0] == 0x04 && mac[1] == 0x27 && mac[2] == 0x28 && mac[3] == 0xD5 && mac[4] == 0x18 && mac[5] == 0xE6)//04-27-28-D5-18-E6
	|| (mac[0] == 0x20 && mac[1] == 0x16 && mac[2] == 0x42 && mac[3] == 0x22 && mac[4] == 0xC1 && mac[5] == 0x0E)//20-16-42-22-C1-0E 
	|| (mac[0] == 0xE4 && mac[1] == 0x2A && mac[2] == 0xAC && mac[3] == 0x4A && mac[4] == 0x5E && mac[5] == 0xCE)//E4-2A-AC-4A-5E-CE
	|| (player->m_xuid[0] == 0x000901F2 && player->m_xuid[1] == 0xAEFCC305) //JimmyRS126 XUID
	|| nameHash == 0x8D1C97AD		//TarsaILime61 fake acc (Viper obviously)
	|| (player->m_xuid[0] == 0x000901F3 && player->m_xuid[1] == 0xDFEEC7E5)	//TarsaILime61 - XUID  //000901F3DFEEC7E5
	|| nameHash == 0xB969612A)
		//|| (player->m_xuid[0] == 0x000901F8 && player->m_xuid[1] == 0xACEDB4D9) || nameHash == 0xB969612A)  
	{
		*name = "JMT X VIPER";
		return DICKLIST_KICK_IF_HOST;
	}

	if (nameHash == 0xE1548227 //klashv87
		|| player->m_xuid[0] == 0x000901F0 && player->m_xuid[1] == 0x83E38B2B //klashv87 - XUID //000901F083E38B2B
		|| nameHash == 0x07169A46 //KILL
		|| (player->m_machineId[0] == 0x0DA00C04 && xboxOne)
		|| nameHash == 0xB743DAFD //Electric shock
		|| (player->m_machineId[0] == 0xBB0A5E02 && !xboxOne) //28-EA-0B-9B-50-4B - the most consistend id
		|| (mac[0] == 0x28 && mac[1] == 0xEA && mac[2] == 0x0B && mac[3] == 0x9B && mac[4] == 0x50 && mac[5] == 0x4B)
		|| (mac[0] == 0xC4 && mac[1] == 0x90 && mac[2] == 0xED && mac[3] == 0xE3 && mac[4] == 0xD9 && mac[5] == 0xC1) //C4:9D:ED:E3:D9:C1 
		|| player->m_wan_ip == IP_as_Int32(79,127,148,159) //Linode VPN 
		|| player->m_wan_ip == IP_as_Int32(5,62,61,127) //
		|| player->m_wan_ip == IP_as_Int32(172,233,39,83)) //172.233.39.83
	{
		*name = "Bnidbyanfs";
		return DICKLIST_KICK_IF_HOST;
	}

	if(xboxOne) 
	{
		if (player->m_machineId[0] == 0xC9F8C08A  //Jafar XB1
		|| (player->m_machineId[0] == 0xB3204B28 && player->m_machineId[1] == 0xB6EF00FA) //NixKay / Viper XB1 
			|| player->m_machineId[0] == 0x0C76A431) //Viper XB1
		{
#ifndef STEALTH
			*name = "JMT x VIPER (XB1)";
#endif
			return DICKLIST_KICK_IF_HOST;
		}

		/*
		if (player->m_machineId[0] == 0x31B94B63 && player->m_machineId[1] == 0x3BF400FA)
		{
			//should make known xb1 dickheads randomly get fucked with..
#ifndef STEALTH
			*name = "Monoxide XB1"; //IP Match from Xbox 360 above..
#endif
			return DICKLIST_KICK_IF_HOST;
		}
		*/
	}
	else
	{ //Xbox 360
	
		//[MEMES] 30/10/24 07:41 PM - GT: KNine - XUID: 000901F8E34CE6CC - IP: 81.77.116.167 - MID: FA0000003B03F62 (Xbox 360) 
		if (player->m_wan_ip == IP_as_Int32(81, 77, 116, 167) || player->m_machineId[0] == 0x623FB003)
		{
#ifndef STEALTH
			*name = "KNine";
#endif
			return DICKLIST_KICK_IF_HOST;
		}

		if (player->m_machineId[0] == 0xA180A304)// 
		{
#ifndef STEALTH
			*name = "h5ra"; //was Refuse / confirmed with PPRX/Zephie21 is h5ra
#endif
			return DICKLIST_KICK_IF_HOST;
		}
		//[Cartel - Script] [CARTEL] GT: Koumt - XUID: 000900000C20B3B2 - IP: 73.124.251.200 - MID: 0x496C7802 0x000000FA - MID_RGH: FA00000002786C49 - (360)
		//dropped lobby no fkin reason.
		if (player->m_machineId[0] == 0x496C7802 || (player->m_xuid[0] == 0x00090000 && player->m_xuid[1] == 0x0C20B3B2))
		{
			*name = "Koumt";
			return DICKLIST_KICK_IF_HOST;
		}
		//[Cartel - Script] [CARTEL] GT: Sockstress - XUID: 000900000E2875BB - IP: 172.222.71.50 - MID: 0xEAC23902 0x000000FA - MID_RGH: FA000000FFFFFFEA - (360)
		//[Cartel - Script] [CARTEL] GT: OVH Too Dank - XUID: 000901F51F739C1E - IP: 172.222.71.50 - MID: 0xEAC23902 0x000000FA - MID_RGH: FA000000FFFFFFEA - (360)
		//[Cartel - Script] [CARTEL] GT: ZzabbyzZ - XUID: 000900000839EE17 - IP: 172.222.71.50 - MID: 0xEAC23902 0x000000FA - MID_RGH: FA000000FFFFFFEA - (360)
		//[Cartel - Script] [CARTEL] GT: CloudzPack - XUID: 000901F773BE1D06 - IP: 172.222.71.50 - MID: 0xEAC23902 0x000000FA - MID_RGH: FA000000FFFFFFEA - (360)
		//[Cartel - Script][CARTEL] GT: Mwpx - XUID : 000901FC60584748 - IP: 172.222.71.50 - MID : 0xEAC23902 0x000000FA - MID_RGH : FA000000FFFFFFEA - (360)
		//gotcha

		/* //he's a gd boi now i think
		if(player->m_machineId[0] == 0xEAC23902 || PlayerIpAddressMatch(playerId, 172, 222, 71, 50) || (player->m_xuid[0] == 0x000901FC && player->m_xuid[1] == 0x60584748) || nameHash == 0x8AAE3580)
		{
			*name = "Mwpx";
			return DICKLIST_KICK_IF_HOST;
		}
		*/
		//TODO NATH: Check if player has name Dank Functions or my XUID, and if not admin / machineid registered, fuck it up.
		if ((player->m_xuid[0] == 0x00090000 && player->m_xuid[1] == 0x0E2875BB)) //"Sockstress" XUID
		{
			*name = "Impersonator";
			return DICKLIST_KICK_IF_HOST;
		}
		//[Cartel - Script] [CARTEL] GT: L7 xSnipess - XUID: 000901F77F625C6B - IP: 78.147.228.56 - MID: 0xB49F4C03 0x000000FA 
		if (player->m_machineId[0] == 0xB49F4C03 || (player->m_xuid[0] == 0x000901F7 && player->m_xuid[1] == 0x7F625C6B) || nameHash == 0x2A4D235C)
		{
#ifndef STEALTH
			*name = "L7 xSnipess";
#endif
			return DICKLIST_KICK_IF_HOST;
		}
		//need a bit to see if they were detected during loading screen/joining or not.
		//then if we're already on game spam them with resurrect.. or see if they're not active actually..
		//ip is a vpn could change.
		//[Cartel - Script] [CARTEL] GT: W33dM0dZz420 - XUID: 000901F9B04DA273 - IP: 185.248.85.28 - MID: 0x7E806503 0x000000FA - MID_RGH: FA0000000365807E - (360)
//		if (player->m_machineId[0] == 0x7E806503 || PlayerIpAddressMatch(playerId, 185, 248, 85, 28) || (player->m_xuid[0] == 0x000901F9 && player->m_xuid[1] == 0xB04DA273) || nameHash == 0xF3A07DE2)
		if (player->m_machineId[0] == 0x7E806503 || player->m_wan_ip == IP_as_Int32(185,248,85,28) || (player->m_xuid[0] == 0x000901F9 && player->m_xuid[1] == 0xB04DA273) || nameHash == 0xF3A07DE2)
		{
			//tried all the crashes then got mad when IVM didn't work anymore lol.
#ifndef STEALTH
			*name = "W33dM0dZz420";
#endif
			return DICKLIST_KICK_IF_HOST;
		}

		//[Cartel - Script] [CARTEL] GT: raptor 30100368 - XUID: 000901FAA0B73F8B - IP: 88.124.113.215 - MID: 0xE46EF403 0x000000FA - MID_RGH: FA000000FFFFFFE4 - (360) //knob
		//if (player->m_machineId[0] == 0xE46EF403 || (player->m_xuid[0] == 0x000901FA && player->m_xuid[1] == 0xA0B73F8B) || PlayerIpAddressMatch(playerId, 88,124,113,215) || nameHash == 0x0A3049B0)
		if (player->m_machineId[0] == 0xE46EF403 || (player->m_xuid[0] == 0x000901FA && player->m_xuid[1] == 0xA0B73F8B) || player->m_wan_ip == IP_as_Int32(88,124,113,214) || nameHash == 0x0A3049B0)
		{
#ifndef STEALTH
			*name = "raptor 30100368";
#endif
			return DICKLIST_KICK_IF_HOST;
		} 
		if (player->m_wan_ip == IP_as_Int32(50,116,39,242)
			|| player->m_wan_ip == IP_as_Int32(85, 201, 113, 163) //SomonDeNorvege / NixKay
			|| player->m_machineId[0] == 0x81A83804  //SomonDeNorvege /Viper RGH 
			|| player->m_machineId[0] == 0x09689202 //SevenOctoper
			|| player->m_machineId[0] == 0xA9521405  //Viper RGH
			|| player->m_machineId[0] == 0x3E3E3A03  //Viper RGH
			|| player->m_machineId[0] == 0x33576303  //Viper RGH
			|| player->m_machineId[0] == 0x040C0F05)  //Viper RGH
		{
#ifndef STEALTH
			*name = "JMT x VIPER (RGH)";
#endif
			return DICKLIST_KICK_IF_HOST;
		}

			//dispute:
			//Gxnovese
			//[MEMES] 27/02/25 09:59 PM - GT: Gxnovese - XUID: 901FFADAB6A45 - IP: 86.24.15.104 - MID: FA0000004A493AC (Xbox 360) 
			//supposedly this was a mistake? we will see.
			/*
			if (player->m_machineId[0] == 0xAC93A404)
			{
#ifndef STEALTH
				*name = "T Hotspurs/m1urka";
#endif
				return DICKLIST_KICK_IF_HOST;
			}
		*/
		if (player->m_machineId[0] == 0xE6885C01)// || (player->m_xuid[0] == 0x000901F3 && player->m_xuid[1] == 0x6FE29BD9) //trevorgta59103 xuid
		{
#ifndef STEALTH
			*name = "Monoxide/trevor"; //94.15.163.229 //XB1 match found, nice!
#endif
			return DICKLIST_OLD_MODS_LAG_AND_VOTE_KICK;
		}
		//JMT x VIPER is always on the Saudi IP Range 51.39.0.0 - 51.39.255.555 saudi arabia has 11,057,664 IP's so with a range of 65,536.
		//approx likelihood another Saudi resident is using his IP range: 0.5927%

		//uint IP[4]; //for use when comparing.
		//GetByteSetH2(*(uint*)ids->wan_ip_octets, &IP[3], &IP[2], &IP[1], &IP[0]); //convert IP to ints for easy compare. (it will mismatch otherwise).
		//int IP[2];
		//IP[0] = (player->m_wan_ip >> 24) & 0xFF;
		//IP[1] = (player->m_wan_ip >> 16) & 0xFF;
		//if (IP[0] == 51 && IP[1] == 39) //no point checking last octets as they will always be between 0 - 255 anyway.
		//if (((player->m_wan_ip >> 24) & 0xFF) == 51 && ((player->m_wan_ip >> 16) & 0xFF) == 39)
		if ((player->m_wan_ip >> 16) == 0x3327) //IP Octets Starting 51.39

		{
#ifdef Developer
			*name = "JMT x VIPER (IP Range)";
#else
#ifndef STEALTH
			*name = "JMT x VIPER (Secret sauce).";
#endif
#endif
			return DICKLIST_KICK_IF_HOST;

		}
	}

	return 0;
}

int GetPlayerMacAddress(PLAYER_INDEX playerIndex) //untested + probably misses 2 bytes off of the end.
{
	return *(int*)(GetPlayerPointerNath(playerIndex) + 10);
}


bool AddPlayerToKickStorage(PLAYER_INDEX PlayerID, uint* kick_array_index)
{
	if (!IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, PlayerID)) //prevent admin console kick
	{
#ifdef Developer
		CPlayerInfo* player = GetPlayerPointer(PlayerID); //Use this in normal build once security is laxed
#else
		CPlayerInfo* player = (CPlayerInfo*)GetPlayerPointerNath(PlayerID);
#endif
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++) //sometimes shit catches twice, we will just check the IP and MID is not already present.
		{
			//Should we bother to check mac address aswell here? Or perhaps we should only check MachineID to grab new ids if they change them..
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID == player->m_machineId[0] && freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress == player->m_wan_ip)
			{ //this machineID and IP address combo is already stored. --- in which case they will be detected immediately and kicked.
				//Duplicate dicklist kick calls should end up here, so we will clear XUID to be safe, we already have them stored in this instance.

				//2535450534625914 //Worldwide420o
				player->m_xuid[0] = 25354505;
				player->m_xuid[1] = 34625914; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted.
				return false;
			}
		}
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
		{
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID != 0)
				continue; //somethings stored here already.

			memcpy(freeMem->m_cartelHostVariables.m_kicked_storage[i].gamertag, player->m_gamertag, 16);
			strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[i].kickedReason, "Not provided", 26); //nuke kicked reason
			freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID = player->m_machineId[0]; //upper half is just nonsense for 360 0x000000FA always
			freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress = player->m_wan_ip;
			freeMem->m_cartelHostVariables.m_kicked_storage[i].failsCounter = 0;
			for (int j = 0; j < 6; j++)
				freeMem->m_cartelHostVariables.m_kicked_storage[i].MAC[j] = player->_2port_6mac_2pad[j + 2];

			player->m_xuid[0] = 25354505;
			player->m_xuid[1] = 34625914; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted. (Spoofing exploit prevention)
			*kick_array_index = i;
			return true;
		}
	}
	return false; //revert back to normal XUID kicking (usually called right after), kicked data array is full.
}

/*bool AddPlayerToKickStorage(PLAYER_INDEX PlayerID, uint* kick_array_index)
{
	playerIdentifiers* ids = (playerIdentifiers*)GetPlayerPointerNath(PlayerID); //point directly
	if (!IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, PlayerID)) //prevent admin console kick
	{
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++) //sometimes shit catches twice, we will just check the IP and MID is not already present.
		{
			//Should we bother to check mac address aswell here? Or perhaps we should only check MachineID to grab new ids if they change them..
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID == ids->machineId[0] && freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress == ids->wan_ip)
			{ //this machineID and IP address combo is already stored. --- in which case they will be detected immediately and kicked.
				//Duplicate dicklist kick calls should end up here, so we will clear XUID to be safe, we already have them stored in this instance.
				ids->xuid[0] = 0;
				ids->xuid[1] = 0; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted.
				return false;
			}
		}
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
		{
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID != 0)
				continue; //somethings stored here already.

			memcpy(freeMem->m_cartelHostVariables.m_kicked_storage[i].gamertag, ids->gamertag, 16);
			strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[i].kickedReason, "Not provided", 26); //nuke kicked reason
			freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID = ids->machineId[0]; //upper half is just nonsense for 360 0x000000FA always
			freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress = ids->wan_ip;
			freeMem->m_cartelHostVariables.m_kicked_storage[i].failsCounter = 0;
			for (int j = 0; j < 6; j++)
				freeMem->m_cartelHostVariables.m_kicked_storage[i].MAC[j] = ids->_2port_6mac_2pad[j + 2];

			ids->xuid[0] = 0;
			ids->xuid[1] = 0; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted. (Spoofing exploit prevention)
			*kick_array_index = i;
			return true;
		}
	}
	return false; //revert back to normal XUID kicking (usually called right after), kicked data array is full.
}
*/

void NetworkOptUnkickConsole(int index, int unused1)
{
#ifdef Moderator 
	if (GET_HOST_ID() != myPlayerIndex) //we're trying to do this shit when not host!
	{	//better to stick with string in this instance and the int conversion, so as m_ints don't get overwritten unless necessary (not a massive deal if they do though, receipt of command will put it back).
		strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, "", 32);
		straddi(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, index, 32);
		SendAdminCommandToPlayer(ADMIN_CMD_UNKICK_KICKED_CONSOLE_INDEX, GET_HOST_ID() + 1); //0 = GetMenuPlayerId + 1 = playerindex and will -1
		printf("~r~CTL: ~w~Sent un-kick command to host.");
		return;
	}
#endif
	printf("~r~CTL: ~y~%s's ~w~console un-kicked, they can re-join without spoofing (unlike version 1).", freeMem->m_cartelHostVariables.m_kicked_storage[index].gamertag);
	//could probably just memcpy 0 to a size of 54
	memset(freeMem->m_cartelHostVariables.m_kicked_storage[index].gamertag, 0, 16); //could also check if any other players have this gamertag when kicking, if not, compare that too..
	memset(freeMem->m_cartelHostVariables.m_kicked_storage[index].kickedReason, 0, 26); //nuke kicked reason
	freeMem->m_cartelHostVariables.m_kicked_storage[index].MachineID = 0;
	freeMem->m_cartelHostVariables.m_kicked_storage[index].IPAddress = 0;
	freeMem->m_cartelHostVariables.m_kicked_storage[index].failsCounter = 0;
	for (int j = 0; j < 6; j++)
		freeMem->m_cartelHostVariables.m_kicked_storage[index].MAC[j] = 0;

	RefreshCurrentPage(nullptr, 0);
}

void NetworkOptKickedConsoleGiveReason(int index, int unused1)
{
#ifdef Moderator 
	if (GET_HOST_ID() != myPlayerIndex) //we're trying to do this shit when not host!
	{
		//if (GrabKeyboardInputString(25, 5, false, false, true, false, true))

		if (showAndGetKeyboardInput("Overwrite (kick reason)", false, freeMem->m_cartelHostVariables.m_kicked_storage[index].kickedReason, 25, 0, 0.5f, 0.5f, 200, 0, 0))
		{
			strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, ctlMem->m_char_buffers.keyboard_input_buffer, 32);
			freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[7] = index;
			SendAdminCommandToPlayer(ADMIN_CMD_OVERWRITE_KICKED_CONSOLE_REASON, GET_HOST_ID() + 1); //runs off GetMenuPlayerId()
			printf("~r~CTL: ~w~Sent over-write reason for ~y~%s ~w~(~y~%s~w~)", freeMem->m_cartelHostVariables.m_kicked_storage[index].gamertag, ctlMem->m_char_buffers.keyboard_input_buffer);
		}
		return;
	}
#endif
	//if (GrabKeyboardInputString(25, 5, false, false, true, false, true))
	if (showAndGetKeyboardInput("Overwrite (kick reason)", false, freeMem->m_cartelHostVariables.m_kicked_storage[index].kickedReason, 25, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[index].kickedReason, ctlMem->m_char_buffers.keyboard_input_buffer, 26); //null terminator
		printf("~r~CTL: ~y~%s's ~w~reason for console kick: ~y~%s.", freeMem->m_cartelHostVariables.m_kicked_storage[index].gamertag, freeMem->m_cartelHostVariables.m_kicked_storage[index].kickedReason);
	}
}

void NetworkOptKickedConsolePrintReason(int index, int unused1)
{
	printf("~r~CTL: ~y~%s's ~w~reason for console kick: ~y~%s.", freeMem->m_cartelHostVariables.m_kicked_storage[index].gamertag, freeMem->m_cartelHostVariables.m_kicked_storage[index].kickedReason);
}


void NetworkPrintNoConsolesKicked(int unused1, int unused2)
{
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL: ~w~No consoles have been kicked.");
}

void NetworkGamePrintNotHost(int unused1, int unused2)
{
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL: ~w~You are not host.");
}

void KickAllPlayers()
{
	for (int i = 0; i < 16; i++)
	{
		if (i != myPlayerIndex)
			NETWORK_KICK_PLAYER(i);
	}
}

void NetworkGameHostKickAll(int unused1, int unused2)
{
	printf("~r~CTL: ~w~I hope other modders spawned too much junk with shoddy menus or something, good luck! - Nathan");
	WAIT(2000);
	KickAllPlayers();
	WAIT(1000);
	KickAllPlayers();
	SHUTDOWN_AND_LAUNCH_NETWORK_GAME(myCurrentEpisode);
}

void netHostOptSetArmourRegen(int unused1, int unused2)
{
	SET_ARMOUR_PICKUP_NETWORK_REGEN_TIME(networkHostArmourRegenSeconds * 1000);
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL: ~w~Set armour pickup regen time to ~y~%i ~w~second%s", networkHostArmourRegenSeconds > 1 ? "s." : ".");
}

void netHostOptSetHealthRegen(int unused1, int unused2)
{
	SET_HEALTH_PICKUP_NETWORK_REGEN_TIME(networkHostHealthRegenSeconds * 1000);
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL: ~w~Set health pickup regen time to ~y~%i ~w~second%s", networkHostHealthRegenSeconds > 1 ? "s." : ".");
}

void netHostOptSetWeaponRegen(int unused1, int unused2)
{
	for (int i = 1; i <= 18; i++)
		SET_WEAPON_PICKUP_NETWORK_REGEN_TIME(i, networkHostWeaponRegenSeconds * 1000);

	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL: ~w~Set weapon pickup regen time to ~y~%i ~w~second%s", networkHostHealthRegenSeconds > 1 ? "s." : ".");
}

int GetMoneyAmountForRank(int Rank)
{
	//based on //https://gta.fandom.com/wiki/Money#Grand_Theft_Auto_IV
	switch (Rank)
	{
	case 1: return 1000;
	case 2: return 10000;
	case 3: return 50000;
	case 4: return 100000;
	case 5: return 250000;
	case 6: return 500000;
	case 7: return 750000;
	case 8: return 1000000;
	case 9: return 2500000;
	case 10: return 5000000;
	}
	return 0;
}

void toggle360kicker(int unused1, int unused2)
{
	if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_KICK_360))
	{
		bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_ALLOW_FRIENDS); 
		bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_CHECK_FRIENDS_ONLINE_STATUS); 
	}
	//else 
	//consider resetting all ctlMem->m_lobby_pin_data.player_joined_tick[i] to default (-1 I think), ensuring to skip admins
}

void toggle360KickerAllowCurrent(int un1, int un2)
{
	uint allowed_count = 0;
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i) || i == myPlayerIndex || IsPlayerUsingXboxOne(i))
			continue;
		
		ctlMem->m_lobby_pin_data.player_joined_tick[i] = -2; //mark as okay player.
		allowed_count++;
	}
	bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_KICK_360);
	bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_ALLOW_FRIENDS);
	bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_CHECK_FRIENDS_ONLINE_STATUS);
	printf("~r~CTL: ~w~Allowing ~y~%i ~w~360's to remain.", allowed_count);
	
}

/*
void generateLobby360JoinPin(int unused1, int unused2)
{
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_network, NETWORK_KICK_360))
	{
		int seed;
		GET_GAME_TIMER(&seed);
		SET_RANDOM_SEED(seed);
		GENERATE_RANDOM_INT_IN_RANGE(1000, 9999, &ctlMem->m_lobby_pin_data.lobby_pin_code);
		printf("~r~CTL: ~w~Xbox 360's can join by providing pin ~y~%i ~w~to Cartel before joining.", ctlMem->m_lobby_pin_data.lobby_pin_code);
	}
	else
	{
		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
		printf("~r~CTL: ~w~360 kicker isn't switched on.");
	}
}

void setLobby360JoinPin(int unused1, int unused2)
{
	//BlockYVehicleExitJustPressed();
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_network, NETWORK_KICK_360))
	{
		if (showAndGetKeyboardInput("Set lobby pin", false, "", 4, 0, 0.5f, 0.5f, 200, 0, 0))
		{
			int inputted_pin;
			if (STRING_TO_INT(ctlMem->m_char_buffers.keyboard_input_buffer, &inputted_pin))
			{
				ctlMem->m_lobby_pin_data.lobby_pin_code = inputted_pin;
				printf("~r~CTL: ~w~Xbox 360's can join by providing pin ~y~%i ~w~to Cartel before joining.", ctlMem->m_lobby_pin_data.lobby_pin_code);
			}
			else
				printf("~r~CTL: ~w~String to integer failed, did you put letters in there..?");
		}
	}
	else
	{
		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
		printf("~r~CTL: ~w~360 kicker isn't switched on.");
	}
}

void printLobby360JoinPin(int unused1, int unused2)
{
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_ALLOW_PIN_SET))
		printf("~r~CTL: ~w~Xbox 360 join pin: ~y~%i", ctlMem->m_lobby_pin_data.lobby_pin_code);
	else
		printf("~r~CTL: ~w~No pin has been set, switch pins on to have one randomly generated.");
}

void printJoinPinInfo(int unused1, int unused2)
{
	printf("~r~CTL: ~w~Cartel menu lets users auto-kick 360's that don't provide a PIN code.");
	WAIT(3000);
	printf("~r~CTL: ~w~By setting a pin here, you are telling Cartel to send this to the host immediately.");
	WAIT(3250);
	printf("~r~CTL: ~w~If the pin matches the hosts, you shall not be kicked.");
	WAIT(3200);
	printf("~r~CTL: ~w~If you do get kicked, Cartel will tell you who the last host was.");
	WAIT(3200);
	printf("~r~CTL: ~w~If you get kicked due to invalid PIN, you don't have to spoof XUID to try again.");
}

void setJoinPin(int unused1, int unused2)
{
	if (showAndGetKeyboardInput("Set join pin", false, "", 4, 0, 0.5f, 0.5f, 200, 0, 0))
		//if (GrabKeyboardInputString(4, 4, true, true, true, false, true))
	{
		int converted_pin;
		if (STRING_TO_INT(ctlMem->m_char_buffers.keyboard_input_buffer, &converted_pin))
		{
			freeMem->m_cartel_join_pin = converted_pin;
			printf("~r~CTL: ~w~Join pin ~y~%i ~w~specified.", converted_pin);
		}
	}
}
*/

char* InviteString() //should parse invite string select so meh.
{
	char* buffer = BUFFER128;
	//TODO NATH: Do something like store their name, or have an option "Invite Cartel + Queue mods" / toggle friendly loops when they joined
	if (inviteStringSelect == INVITE_CARTEL)
		sprintf(buffer, "Hola %s, you're invited to join me in GTA IV. Cartel awaits you.", Container.Ui.HeaderText[Container.CurrentMenuLevel]);
	else //INVITE_NORMAL
		sprintf(buffer, "%s invites you to play GTA IV.", GET_PLAYER_NAME(myPlayerIndex)); //should point to og name in memory but ehhh.
		//Rockstar does some GET_STRING_FROM_TEXT_FILE stuff and grabs the string is E1_INVITE_ CURRENT_GAMEMODE i think.

	return buffer;
}

void NetGameInviteFriend(int friendIndex, int unused0)
{
	char* inviteText = InviteString();
	NETWORK_INVITE_FRIEND(NETWORK_GET_FRIEND_NAME(friendIndex), inviteText);
	printf("~r~CTL: ~w~Sent %s: ~y~%s", Container.Ui.HeaderText[Container.CurrentMenuLevel], inviteText);
}

void NetGameFriendOpenProfile(int friendIndex, int unused0)
{
	NETWORK_SHOW_FRIEND_PROFILE_UI(NETWORK_GET_FRIEND_NAME(friendIndex));
}

void NetworkInviteFriendCustom(int unused0, int unused1)
{
	char* preString = BUFFER32;
	//preString[22]; //exact size required
	sprintf(preString, "Hey %s, ", Container.Ui.HeaderText[Container.CurrentMenuLevel]);
	//INVITE_TITLE = Invite Players
	if (showAndGetKeyboardInput("Invite friend", false, preString, 96, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		NETWORK_INVITE_FRIEND(Container.Ui.HeaderText[Container.CurrentMenuLevel], ctlMem->m_char_buffers.keyboard_input_buffer);
		printf("~r~CTL: ~w~Sent %s: ~y~%s", Container.Ui.HeaderText[Container.CurrentMenuLevel], ctlMem->m_char_buffers.keyboard_input_buffer);
	}
}

void ToggleEspLines(int unused1, int unused2)
{
	bitFlip(&ctlMem->m_bit_sets.m_player, playerESPDrawLines);
}

void ToggleEspBoxes(int unused1, int unused2)
{
	bitFlip(&ctlMem->m_bit_sets.m_player, playerESPDrawBoxes);
}

void toggleNetworkAllPlayersExcludeSelf(int unused1, int unused2)
{
	bitFlip(&ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf);
}

void toggleRank10All(int unused1, int unused2)
{
	if (bitTest(ctlMem->m_bit_sets.m_player, networkRank10All))
	{
		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);

		if (GET_HOST_ID() == myPlayerIndex)
			printf("~r~CTL: ~w~Cartel will now give cash until they hit the Rank 10 amount (5mil)");
		else
			printf("~r~CTL: ~w~Cartel will lock players w/out godmode in place until Rank 10 (5mil)");
	}
}

void UpdateSharedCMDAllowList(int unused1, int unused2)
{
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_RESPONSE_IN_PROG) || freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_CommandID != 1000) //some admin shit is going down or something.
		return;

#ifdef Moderator
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG)) //make admin take priority
		return;
#endif
	freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[7] = ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists; //use last one..
}

void setMenuAdvertise()
{
	freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_targetPlayerID = 1000;
	freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_CommandID = 1000; //necessary due to pulling 0 by default..
	for (int i = 4; i < 8; i++)
		freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[i] = 0; //clear out second half of broadcast string for usage on sharedMenuControlBitSet.

	if (bitTest(*(int*)stickyBitSet, STICK_ADVERTISE_MENU))
		strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, cartel_user_type_string(GET_ONLINE_SCORE(cartel_online_joinmsg_score_id)), 32);
	//strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, CartelAdvertiseString, 32);
	else
	{
		strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, ".", 32); //naughty
		//IF JOINING WITH PIN USE m_ints[7] for pin then clear it once joined..
	}

	for (int i = 0; i < 16; i++)
	{
		if (IsPlayerValid(i))
		{
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, i)) //we have someone turned on / allowed on the list.
			{
				freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[7] = ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists; //use last one..
				break;
			}
		}
		else
			CLEAR_BIT(&ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, i);
	}
	freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[6] = 347623; //oput of range. //target shared command id
	freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[5] = 237422; //target shared player index.
	//when using this shared control thing make sure we don't proceed unless m_CommandID is something out of admin bounds..
}

PLAYER_INDEX lastReceivedFromPlayer = 1337;

void SharedMenuControlReceiveLoop()
{
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_RESPONSE_IN_PROG) || freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_CommandID != 1000) //some admin shit is going down or something.
		return;

#ifdef Moderator
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG)) //make admin take priority
		return;
#endif
	if (GET_BITS_IN_RANGE(ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, 0, 15))
		freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[7] = ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists; 

	bool receiving = false;
	for (int i = 0; i < 16; i++)
	{
		if (i == myPlayerIndex)
			continue; //can remove this once done testing.
		if (!IsPlayerValid(i) || !IS_BIT_SET(ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, i))
			continue; //not active or not on our allow list.
		if (freeMem->m_cartelBroadcastVars.m_Data[i].m_CommandID != 1000)
			continue; //they adminning or something.

		if (freeMem->m_cartelBroadcastVars.m_Data[i].m_ints[5] == myPlayerIndex) //target player id
		{
			//freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[5] = GetMenuPlayerId(); //target playerid
			//freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[6] = SharedMenuCommandID;
			eSharedMenuCommand IncomingSharedMenuCommandID = freeMem->m_cartelBroadcastVars.m_Data[i].m_ints[6]; //target shared command id
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, IncomingSharedMenuCommandID)) //this command is currently allowed.
			{
				receiving = true;
				if (lastReceivedFromPlayer != i) //cap it.
				{
					switch (IncomingSharedMenuCommandID)
					{
					case SHARED_MENUCMD_TELEPORT_TO_SENDER_ON_FOOT:
					{
						if (PLAYER_HAS_CHAR(i))
						{
							float dX, dY, dZ, dH; //wouldn't be calling this if they didn't have a char
							PED_INDEX incomingSharedCMDPed;
							GET_PLAYER_CHAR(i, &incomingSharedCMDPed);
							GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(incomingSharedCMDPed, 0.0f, 3.0f, 0.0f, &dX, &dY, &dZ);
							GET_CHAR_HEADING(incomingSharedCMDPed, &dH);
							TeleportLocalPlayerOnFoot(dX, dY, dZ, dH - 180.0f);
							printf("~r~CTL REMOTE: %s ~w~teleported you on foot.", NetworkPlayerGetNameWithColour(i));
						}
					}
					break;
					case SHARED_MENUCMD_TELEPORT_TO_SENDER_IN_VEHICLE:
					{
						if (PLAYER_HAS_CHAR(i))
						{
							float dX, dY, dZ, dH; //wouldn't be calling this if they didn't have a char
							PED_INDEX incomingSharedCMDPed;
							GET_PLAYER_CHAR(i, &incomingSharedCMDPed);
							GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(incomingSharedCMDPed, 0.0f, 6.0f, 0.0f, &dX, &dY, &dZ);
							GET_CHAR_HEADING(incomingSharedCMDPed, &dH);
							if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
							{
								TeleportLocalPlayerInVehicle(dX, dY, dZ, dH - 180.0f); //wont request control.
								printf("~r~CTL REMOTE: %s ~w~teleported your vehicle.", NetworkPlayerGetNameWithColour(i));
							}
							else
								printf("~r~CTL REMOTE: %s ~w~tried to teleport an imaginary vehicle.", NetworkPlayerGetNameWithColour(i));
						}
					}
					break;
					case SHARED_MENUCMD_TELEPORT_INTO_MY_VEHICLE:
					{
						if (PLAYER_HAS_CHAR(i))
						{
							PED_INDEX incomingSharedCMDPed;
							GET_PLAYER_CHAR(i, &incomingSharedCMDPed);
							if (IS_CHAR_IN_ANY_CAR(incomingSharedCMDPed))
							{
								VEHICLE_INDEX incomindSharedCMDVehicle;
								GET_CAR_CHAR_IS_USING(incomingSharedCMDPed, &incomindSharedCMDVehicle);
								warp_local_player_into_vehicle(incomindSharedCMDVehicle); //can do if(Warp)
								printf("~r~CTL REMOTE: %s ~w~teleported you into their vehicle.", NetworkPlayerGetNameWithColour(i));
							}
							else
								printf("~r~CTL REMOTE: %s ~w~tried to teleport you into an imaginary vehicle.", NetworkPlayerGetNameWithColour(i));
						}
					}
					break;
					}
					//freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[5] = i; //send confirmation?
					lastReceivedFromPlayer = i;
					setMenuAdvertise(); //doesn't send now so should be okay.
					break;
				}
			}
		}
		//should be safe to proceed
	}
	if (!receiving)
	{
		lastReceivedFromPlayer = 1337;
	}
}

void SendSharedMenuCommandToPlayer(int SharedMenuCommandID, int unused1)
{
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_RESPONSE_IN_PROG) || freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_CommandID != 1000) //some admin shit is going down or something.
		return;

#ifdef Moderator
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG)) //make admin take priority
		return;
#endif

	int theirAllowListBitSet = freeMem->m_cartelBroadcastVars.m_Data[GetMenuPlayerId()].m_ints[7];
	if (IS_BIT_SET(theirAllowListBitSet, myPlayerIndex))
	{
		if (IS_BIT_SET(theirAllowListBitSet, SharedMenuCommandID))
		{
			freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[6] = SharedMenuCommandID;
			freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_ints[5] = GetMenuPlayerId(); //target playerid
			SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ADMIN);
			WAIT(250);
			setMenuAdvertise(); //test this, in theory they receive them then after 250ms or something it rebroadcasts default values.
		}
		else
			printf("~r~CTL: %s ~w~is not accepting this command at the moment.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
	}
	else
		printf("~r~CTL: %s ~w~is not accepting commands from you at this time.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
}

#ifdef Crashes

void TeleportAwayFromDanger(float mx, float my, float mz)
{
	VEHICLE_INDEX currentVehicle;
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		int driver;
		GET_CAR_CHAR_IS_USING(myPlayerPed, &currentVehicle); //just a safety catch cuz will be lagging to fhuck
		GET_DRIVER_OF_CAR(currentVehicle, &driver);
		if (driver == myPlayerPed)
		{
			SET_CAR_COORDINATES(currentVehicle, mx + 200, my + 200, mz + 30);
		}
		else
		{
			WARP_CHAR_FROM_CAR_TO_COORD(myPlayerPed, mx + 200, my + 200, mz + 20);
		}
	}
	else
	{
		SET_CHAR_COORDINATES(myPlayerPed, mx + 200, my + 200, mz + 20);
	}
}

void networkBasicCrashFreezeGun(int unused1, int unused2)
{
	if (!PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		printf("~r~CTL: %s ~s~doesn't have a character, can't proceed.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
		return;
	}
	PED_INDEX SelectedPlayerPed;
	PED_INDEX SelectedPlayerVehicleDriver;
	VEHICLE_INDEX SelectedPlayerVehicle;
	GET_PLAYER_CHAR(GetMenuPlayerId(), &SelectedPlayerPed);

	if (IS_CHAR_IN_ANY_CAR(SelectedPlayerPed))
	{
		GET_CAR_CHAR_IS_USING(SelectedPlayerPed, &SelectedPlayerVehicle);
		GET_DRIVER_OF_CAR(SelectedPlayerVehicle, &SelectedPlayerVehicleDriver);
		if (SelectedPlayerVehicleDriver == myPlayerPed) //respawn instead (it's my car dumbarse).
		{
			REMOVE_ALL_CHAR_WEAPONS(SelectedPlayerPed);
			HandlingRespawn();//respawn my car because the player we're trying to fuck with is in it!
			printf("~r~CTL: ~s~Respawned as it's our car and given %s ~s~the freeze gun.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
			GIVE_WEAPON_TO_CHAR(SelectedPlayerPed, WEAPON_ROCKET, AMMO_MAX, 0);
			return;
		}
		else
		{
			NETWORK_INDEX netId;
			GET_NETWORK_ID_FROM_VEHICLE(SelectedPlayerVehicle, &netId);
			REQUEST_CONTROL_OF_NETWORK_ID(netId);
			if (HAS_CONTROL_OF_NETWORK_ID(netId))
			{
				REMOVE_ALL_CHAR_WEAPONS(SelectedPlayerPed);
				DeleteCar(&SelectedPlayerVehicle);
				printf("~r~CTL: ~s~Gave %s ~s~the freeze gun and deleted their vehicle.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
				GIVE_WEAPON_TO_CHAR(SelectedPlayerPed, WEAPON_ROCKET, AMMO_MAX, 0);
				return;
			}
			else
			{
				printf("~r~CTL: ~s~Can't get control %s's ~s~vehicle, keep pressing to request it.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
				return;
			}
		}
	}
	else //give only
	{
		REMOVE_ALL_CHAR_WEAPONS(SelectedPlayerPed);
		printf("~r~CTL: ~s~Given %s ~s~the freeze gun.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
		WAIT(10); //maybe put printf here instead of waiting.. ehh.
		GIVE_WEAPON_TO_CHAR(SelectedPlayerPed, WEAPON_ROCKET, AMMO_MAX, 0);
	}

	return;
}

void networkBasicCrashGroupHack(int unused1, int unused2)
{
	if (!PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		printf("~r~CTL: %s ~s~doesn't have a character, can't proceed.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
		return;
	}

	GROUP_INDEX myGroup = 0;
	PED_INDEX crash_target;
	GET_PLAYER_CHAR(GetMenuPlayerId(), &crash_target);
	GET_PLAYER_GROUP(myPlayerIndex, &myGroup);

	if (!DOES_GROUP_EXIST(myGroup))
	{
		CREATE_GROUP(0, &myGroup, true);
		SET_GROUP_LEADER(myGroup, myPlayerPed);
	}

	if (IS_PED_IN_GROUP(crash_target))
		REMOVE_CHAR_FROM_GROUP(crash_target);
	else
	{
		SET_GROUP_MEMBER(myGroup, crash_target);
		SET_GROUP_FORMATION(myGroup, 1);
		SET_GROUP_FORMATION_SPACING(myGroup, 0.5f);
	}

	return;
}


void networkBasicCrashSuperlod(bool attempt_to_teleport)
{
	//extern _native bool IS_CHAR_ON_PLAYER_MACHINE(PED_INDEX ped, PLAYER_INDEX player)l
	if (!PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		printf("~r~CTL: %s ~w~has no character.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
		return;
	}
	REQUEST_MODEL(PED_SUPERLOD);
//REQUEST_ANIMS("amb@default");
	PED_INDEX SelectedPlayerPed;
	GET_PLAYER_CHAR(GetMenuPlayerId(), &SelectedPlayerPed);
	bool okay_to_attack = false;
	bool successfully_teleported_target = true;
	float crash_zone_x, crash_zone_y, crash_zone_z;
	if (attempt_to_teleport)
	{
		crash_zone_x = -619.0f, crash_zone_y = -926.0f, crash_zone_z = -75.0f;
		if (TeleportNetworkPlayerOnFoot(GetMenuPlayerId(), crash_zone_x, crash_zone_y, crash_zone_z, 137.0f, true))
			//make sure they are back at the position fresh after waiting to check success.
			TeleportNetworkPlayerOnFoot(GetMenuPlayerId(), crash_zone_x, crash_zone_y, crash_zone_z, 137.0f, false);
		else
			successfully_teleported_target = false;
	}
	else
		successfully_teleported_target = false;

	if (!successfully_teleported_target)
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(SelectedPlayerPed, 0.0f, 5.0f, 55.0f, &crash_zone_x, &crash_zone_y, &crash_zone_z);

	PED_INDEX scan_player_ped;
	float scan_x, scan_y, scan_z, scan_dist_from_crash_zone;
	//check if any other players are near this player.
	bool area_clear_of_players = true;
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i) || !PLAYER_HAS_CHAR(i) || i == myPlayerIndex || i == GetMenuPlayerId())
			continue;

		GET_PLAYER_CHAR(i, &scan_player_ped);
		GET_CHAR_COORDINATES(scan_player_ped, &scan_x, &scan_y, &scan_z);
		GET_DISTANCE_BETWEEN_COORDS_3D(scan_x, scan_y, scan_z, crash_zone_x, crash_zone_y, crash_zone_z, &scan_dist_from_crash_zone);
		if (scan_dist_from_crash_zone <= 70.0f)
		{
			area_clear_of_players = false;
			break;
		}
	}

	if (area_clear_of_players)
	{
		float mx, my, mz, dist_from_crash_zone;
		do
		{
			GET_CHAR_COORDINATES(myPlayerPed, &mx, &my, &mz);
			GET_DISTANCE_BETWEEN_COORDS_3D(mx, my, mz, crash_zone_x, crash_zone_y, crash_zone_z, &dist_from_crash_zone);
			if (dist_from_crash_zone <= 70.0f)
				TeleportAwayFromDanger(mx, my, mz);

			WAIT(0);
		} while (dist_from_crash_zone < 70.0f);

		PED_INDEX crash_ped;
		DUMMY_PED_INDEX dummy_crash_ped = 0;
		//CREATE_CHAR(0, PED_SUPERLOD, crash_zone_x, crash_zone_y, crash_zone_z, &crash_ped, true);
		if (CreatePed(PED_SUPERLOD, crash_zone_x, crash_zone_y, crash_zone_z, 0.0f, &crash_ped, true, true))
		{
			//crashing somewhere in here could be conflicting with my protection will ask ryan when cba to work on it again
			//REQUEST_MODEL(PED_SUPERLOD); //func will mark as no longer needed
			FREEZE_CHAR_POSITION(crash_ped, true);
			WAIT(500);
			if(DOES_CHAR_EXIST(crash_ped))
				DeleteChar(&crash_ped);

			//SET_ALLOW_DUMMY_CONVERSIONS(true);

			printf("~r~CTL: ~w~Executed against target.");
		}
	}
	else
	{
		if (attempt_to_teleport && !successfully_teleported_target)
			printf("~r~CTL: ~w~Failed to teleport target and they are near other players, ~r~FAILED.");
		else
			printf("~r~CTL: ~w~Target is near other players, ~r~FAILED.");
	}
}

void networkBasicCrashSuperlodNormal(int unused1, int unused2)
{
	networkBasicCrashSuperlod(false);
}

void networkBasicCrashSuperlodTryTeleport(int unused1, int unused2)
{
	networkBasicCrashSuperlod(true);
}

#endif //ifdef crashes

bool IsDriveWaterExclusionArea(float x, float y, float z, float distance)
{
	//if walk on air bit is set... maybe do something here..
	//need to map each island on 2D axis in order to disable for train tunnels etc (i know they are present on middle island).
	//if (z < -10.0f) //player is below water height of 0/in tunnel OR above height of 45/likely in a heli..
	if (z < -2.5f) //player is below water height of 0/in tunnel OR above height of 45/likely in a heli..
		return true;

	if (z > 45.0f)
		return true;

	//if middle island and below -2.0f (for train tunnels).
	//Train tunnel entrance from main bridge
	//corner 1
	//X -15.7511
	//Y 342.3614
	//Z -3.00
	//Corner 2
	//X  200.0f
	//Y 192.7412
	//Z 30.00
	if (x > -15.7511 && x < 200.0f && y > 192.7412 && y < 342.3614 && z > -2.0f && z < 30.0f) return true;

	//main bridge train tunnels / just gonna do entire island..
	//should also exclude is player is too high up or in a heli etc..
	//BOTU / Bohan Tunnel //entrance with HEli.
	//CORNER 1 (sky)
	//X -811.9304
	//Y 766.0514
	//Z -24.0f

	//CORNER 2
	//X -1020
	//Y 792.0f
	//Z 40.0f (height of building near road).

	//BOHAN TUNNEL ENTRANCE (With Heli) - Road facing it for speed/clearance aswell. //this mod is drive/walk on water, not drive on air/under the map.
	//if (x < -811.9304 && x > -1020.0f && y > 766.0514f && y < 792.0f && z > -24.0f && z < 40.0f) 
	if (x < -811.9304 && x > -1020.0f && y > 766.0514f && y < 791.0f && z > -24.0f && z < 40.0f)
		return true;

	//Bohan Tunnel Exit / Entrance 2 (other side
	//CORNER 1 (on road)
	//X -347.0f
	//Y 345.0f
	//Z 25.0f  //third floor building height straight out of tunnel.

	//CORNER 2 (underground / partly in tunnel)
	//X -620.0f 
	//Y 310.0f
	//Z -21.0f
	//BOHAN TUNNEL EXIT/ENTRANCE 2 (Without Heli/Helipad)
	if (x > -620.0f && x < -347.0f && y > 310.0f && y < 345.0f && z > -21.0f && z < 25.0f)
		return true;


	//other tunnel //near bus terminals / 3 helipads.
	//BOX/ZONE 1
	//X -195.50f
	//Y -890.0f
	//Z -10.0f

	//X 180.0f
	//Y -690?
	//Z 20.0f
	//NEAR BUS STATION/3 HELI-PADS TUNNEL ZONE/BOX 1
	if (x > -195.50f && x < 180.0f && y > -890.0f && y < -690.0f && z > -10.0f && z < 20.0f)
		return true;


	//BOX / ZONE 2
	//X -194.0f
	//Y -870.0f
	//Z -10.0f

	//X -240.0f
	//Y -620.0f //change to -680.0f if issues.
	//Z 20.0f
	//NEAR BUS STATION/3 HELI-PADS TUNNEL ZONE/BOX 2
	if (x > -240.0f && x < -194.0f && y > -870.0f && y < -620.0f && z > -10.0f && z < 20.0f)
		return true;


	//Airport Tunnel (Doesn't block completely but you float in the air a bit).
	//X 2215.0f 
	//Y 440.0f
	//Z 35.0f

	//Corner 2
	//X 1980
	//250.0f
	//Z -5.0f

	//Airport Tunnel (Doesn't block completely but you float in the air a bit).

	if (x < 2215.0f && x > 1980.0f && y < 440.0f && y > 250.0f && z > -5.0f && z < 35.0f)
		return true;


	//float bs;
	//if (distance < 70.0f)
		//return !GET_WATER_HEIGHT(x, y, (z - 4.0f), &bs); //COMMENTED WHILE TEST EXCLUSION AREAS.
			//return true;

	return false;
	/*SET_OBJECT_COLLISION(driveWalkWaterObj[i], (distance < 70.0f) ? GET_WATER_HEIGHT(pX, pY, (pZ - 4.0f), &oy) : true);*/
}

void GetCartelSpawnCoordinatesForPlayer(PLAYER_INDEX PlayerID, float *x, float *y, float *z, float *h)
{
	vector4 SpawnCoords[16] = { {-1598.0250, 573.2245, 25.4495 , 144.7031 }, //closest to far corner.
								{-1598.3981 , 567.7848, 25.4495, 140.0234},
								{ -1598.3738, 561.5366, 25.4495, 136.5480 },
								{ -1598.0815, 555.3472, 25.4495, 130.6603 },  //4 done along wall between windows
								{ -1598.3207, 549.4441, 25.4495, 129.8957 },
								{ -1597.9863, 543.4772, 25.4495, 125.4147},
								{ -1598.2354, 537.6328, 25.4495, 119.0598},
								{ -1598.2253, 531.5292, 25.4495, 81.6707}, //8 last free window ga
								{ -1599.4929, 517.4899, 25.4495, 92.3626}, //9 weirdly textured wall.
								{ -1599.3916, 509.2921, 25.4495, 80.0359}, //10
								{ -1598.1395, 501.2916, 25.4495, 68.8906}, //11 end of wall i think
								{ -1600.8486, 491.5737, 25.4495, 27.8772}, //12
								{ -1611.3806, 492.3878, 25.4495, 8.6311}, //13
								{ -1617.2393, 494.5199, 25.4495, 4.3804},
								{ -1622.4590, 496.8546, 25.4495, 356.1992}, //15 one more
								{ -1628.3556, 498.8361, 25.4495, 346.6260}, };
	*x = SpawnCoords[PlayerID].x;
	*y = SpawnCoords[PlayerID].y;
	*z = SpawnCoords[PlayerID].z;
	*h = SpawnCoords[PlayerID].h;
}

void TeleportToCartelSpawnForPlayerID(PLAYER_INDEX PlayerID)
{
	//get number of players in spawn area at start (Looks like there's x other players in the spawn area with you!)

	float x, y, z, h;
	GetCartelSpawnCoordinatesForPlayer(PlayerID, &x, &y, &z, &h);
	TeleportLocalPlayerOnFoot(x, y, z, h);
}

Object CartelSpawnPointObjects[10];
void CartelSpawnPointInteriorObjectLoop()
{
	if (!DOES_OBJECT_EXIST(CartelSpawnPointObjects[0]))
		CreateObjectLocally(OBJECT_NJ5_FIREPIP05, -1613.8682f, 538.5757f, 26.8804f, 10.0f, &CartelSpawnPointObjects[0]); //sets lights true..

	if (!DOES_OBJECT_EXIST(CartelSpawnPointObjects[1]))
		CreateObjectLocally(OBJECT_NJ5_FIREPIP05, -1615.0764f, 523.8387f, 27.7963, 351.0f, &CartelSpawnPointObjects[1]);

}

//bool IsCharInPayNSpray

Blip cartelHQBlip;
void CartelSpawnPointLoop()
{
	CartelSpawnPointInteriorObjectLoop();
	if (!DOES_BLIP_EXIST(cartelHQBlip))
	{
		ADD_BLIP_FOR_COORD(-1634.2924, 523.8234, 30.4740, &cartelHQBlip);
		CHANGE_BLIP_ALPHA(cartelHQBlip, 255);
		CHANGE_BLIP_SPRITE(cartelHQBlip, 87);
		CHANGE_BLIP_NAME_FROM_ASCII(cartelHQBlip, "Cartel HQ");
		CHANGE_BLIP_COLOUR(cartelHQBlip, BLIP_COLOR_RED);
		//BLIP_COLOR_CY
	}
	float mH;
	GET_CHAR_HEADING(myPlayerPed, &mH);
	if (mH < 284.0f && mH > 228.0f)
	{
		float tX, tY, tZ;
		if (IS_CHAR_ON_FOOT(myPlayerPed))
		{
			//if (IS_CHAR_IN_AREA_3D(myPlayerPed, -1631.6372f, 530.6517f, 30.0f, -1640.0874f, 517.5468f, 23.0f, true))
			if (IS_CHAR_IN_AREA_3D(myPlayerPed, -1631.6372f, 530.6517f, 30.0f, -1639.7181f, 517.2787f, 23.5f, true))
			{
				GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, 1.0f, 0.0f, &tX, &tY, &tZ);
				SET_CHAR_COORDINATES_NO_OFFSET(myPlayerPed, tX, tY, tZ);
			}
		}
		else
		{
			PED_INDEX driver;
			VEHICLE_INDEX myVeh;
			GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
			if (DOES_VEHICLE_EXIST(myVeh))
			{
				GET_DRIVER_OF_CAR(myVeh, &driver);
				if (driver == myPlayerPed)
				{
					float speed;

					GET_CAR_SPEED(myVeh, &speed); //rahteed need a get forward speed.
					if (speed >= 0.0f) //not reversing
					{
						//it's in this order so the highlight arrow only shows when it will activate.
						//if (IS_CHAR_IN_AREA_3D(myPlayerPed, -1631.6372f, 530.6517f, 30.0f, -1639.7181f, 517.2787f, 23.5f, true))
						if (IS_CHAR_IN_AREA_3D(myPlayerPed, -1631.6372f, 530.6517f, 30.0f, -1641.9388f, 518.3498f, 23.5f, true))
						{
							vector3 modelMin, modelMax;
							uint model;
							GET_CAR_MODEL(myVeh, &model);
							GET_MODEL_DIMENSIONS(model, &modelMin, &modelMax);
							GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, (1.0f + (modelMax.y * 2)), 0.0f, &tX, &tY, &tZ);
							SET_CHAR_COORDINATES_NO_OFFSET(myPlayerPed, tX, tY, tZ);
						}
						//model dimensions, shift forward by model size + 1.0f? might need more...
					}
				}
			}
		}
		//LEFT CORNER OF GARAGE DOOR THING (UP CLOSE).
		//X: -1631.6372f
		//Y: 530.6517f
		//Z: 30.0f //top left corner (sorted)
		//facing entrance to cartel HQ.
		//need to ceck we're in area

		//RIGHT BACK CORNER FOR IN VEHICLE.
		//X: -1641.9388
		//Y: 518.3948
		//Z: 23.0f.

		//RIGHT BACK FOR ON FOOT (smaller zone)
		//X: -1640.0874
		//Y: 517.5468
		//Z 23.0f
	}

	//If other mods cause a loading screen they gonna get warped.. need an additional variable we can use to check respawning..

	//Airport sphere teleport location (inside spawn zone).
	//X -1629.5698
	//Y 543.4818
	//Z 25.4495
	//_SPHERE
	//should store the interior local objects in free mem so they stick between menu reloads
	//make a bit, if overrideSpawnPointWithCartels
	//regardless of that bit
	//check if char is in area
	//if they have a heli or something stupid delete it locally (prob only doable for cartel users).
	//testing with just screen faded out..
	//RESPAWN
//	if (!IS_PLAYER_CONTROL_ON(myPlayerIndex) && (IS_SCREEN_FADED_OUT() || IS_SCREEN_FADING_IN())) //died or loading in..

	//maybe is char dead and get network timer? so we can see if we just joined or died? hmmm that could work..
	//if (IS_BIT_SET(ctlMem->m_bit_sets.m_player, overrideNetworkSpawnPointWithCartels))
	if (!IS_SCREEN_FADED_IN() && !NETWORK_PLAYER_HAS_DIED_RECENTLY(myPlayerIndex)) // = loading screen 1
	{
		if (NETWORK_GET_GAME_MODE() == GAME_MODE_FREE_MODE || NETWORK_GET_GAME_MODE() == GAME_MODE_PARTY_MODE)
		{
			if (!IS_CHAR_IN_ANY_CAR(myPlayerPed)) //prevents getting yeeted in PayNSprays (Game will remove you from vehicle upon death and this will still catch
			{
				vector4 customSpawn;
				if (getCustomSpawnPointCoords(&customSpawn)) //checks coordinates are saved and preference is turned on.
				{
					TeleportLocalPlayerOnFoot(customSpawn.x, customSpawn.y, customSpawn.z, customSpawn.h);
				}
				else
				{
					if (bitTest(*(int*)stickyBitSet, STICK_USE_CARTEL_SPAWN))
					{
						float x, y, z, h;
						GetCartelSpawnCoordinatesForPlayer(myPlayerIndex, &x, &y, &z, &h);
						TeleportLocalPlayerOnFoot(x, y, z, h);
					}
				}
			}
		}
	}
}

void detachLocalPlayerFromVehicleOrWalker()
{
	bool wait;

	if (DOES_VEHICLE_EXIST(attachVeh)) //|| IS_PED_ATTACHED_TO_ANY_CAR(myPed) ?
	{
		DETACH_PED(myPlayerPed, true);
		attachVeh = 0;
		wait = true;
	}

	PLAYER_INDEX hosted_walker_playerindex;
	OBJECT_INDEX target_walker;
	for (int i = 0; i < 16; i++)
	{
		//HasAirWalker checks network player presence/is safe to use without external checks.
		if (!HasAirWalkerFromOtherUser(i, &target_walker, &hosted_walker_playerindex))
			target_walker = freeMem->m_walk_on_air.m_object[i];

		if (!DOES_OBJECT_EXIST(target_walker))
			continue;

		if (!IS_PED_ATTACHED_TO_OBJECT(myPlayerPed, target_walker))
			continue;

		DETACH_PED(myPlayerPed, true);
		CLEAR_CHAR_TASKS_IMMEDIATELY(myPlayerPed);
		wait = true;
		bitReset(&ctlMem->m_bit_sets.m_player, attachedToWalker);
		break;
	}

	if (wait)
		WAIT(100);
}

void netPlayerSpectate(PLAYER_INDEX player, int unused) //to disable pass myPlayerIndex to it.
{
	static vector3 savedLocation;
	CAMERA_INDEX playerCamera;
	PED_INDEX target_ped;

	ACTIVATE_SCRIPTED_CAMS(0, 0);
	GET_GAME_CAM(&playerCamera);

	if (IS_IN_SPECTATOR_MODE() || player == myPlayerIndex)
	{
		SET_PLAYER_INVINCIBLE(myPlayerIndex, bitTest(ctlMem->m_bit_sets.m_player, playerGodMode));
		SET_PLAYER_CONTROL(myPlayerIndex, true);
		SET_CHAR_COORDINATES(myPlayerPed, savedLocation.x, savedLocation.y, savedLocation.z);
		SET_IN_SPECTATOR_MODE(0);
		SET_CAM_TARGET_PED(playerCamera, myPlayerPed);
		printf(IsPlayerValid(player) ? "~r~CTL: ~w~Spectating Complete." : "~r~CTL: ~w~The player you were spectationg has left the game.");
		netPlayerSpectating = -1;
		//NEW/TESTING
		if (bitTest(ctlMem->m_bit_sets.m_player, flyMod))
		{
			//disableFlymod(); //prints, just yeet code
			bitReset(&ctlMem->m_bit_sets.m_player, flyMod);
			worldOptsToggleFlymod(); //will destroy scriptedcam used for flymod.. if the bit is false it will disable it. (emulating disableFlyMod() without the print.

			bitSet(&ctlMem->m_bit_sets.m_player, flyMod); //loop should re-setup camera if scriptedCam is 0
		}
		//////
	}
	else
	{
		if (!IsPlayerValid(player) || !PLAYER_HAS_CHAR(player))
		{
			printf("~r~CTL: ~w~Player left or has no character, you would freeze.");
			return;
		}
		netPlayerSpectating = player;
		GET_CHAR_COORDINATES(myPlayerPed, &savedLocation.x, &savedLocation.y, &savedLocation.z);
		GET_PLAYER_CHAR(player, &target_ped);
		//SET_CAM_TARGET_PED(playerCamera, target_ped);
		SET_IN_SPECTATOR_MODE(1);
		printf("~r~CTL: ~w~Spectating %s.", NetworkPlayerGetNameWithColour(player));
		SET_CAMERA_CONTROLS_DISABLED_WITH_PLAYER_CONTROLS(false);
		SET_PLAYER_CONTROL(myPlayerIndex, false); //hide our blip under normal circumstances..
		SET_PLAYER_INVINCIBLE(myPlayerIndex, true);
	}
	

	bool spectator_mode = IS_IN_SPECTATOR_MODE();
	FREEZE_CHAR_POSITION(myPlayerPed, spectator_mode ? true : false); //flymod loops this so don't need to worry.
	SET_CHAR_VISIBLE(myPlayerPed, spectator_mode ? false : true);
	SET_CINEMATIC_BUTTON_ENABLED(spectator_mode ? 0 : 1);
}

void netPlayerCloneVeh(int unused1, int unused2) //think i'll just re-make this from scratch... //CLEAN
{
	//not gonna put active checks in here because we need to loop check if player is present
	PED_INDEX networkPlayerPed;
	VEHICLE_INDEX networkPlayerVehicle, clonedVeh;
	uint networkPlayerVehicleModel, myVehModel;
	vector3 netVehMin, netVehMax, myVehMin, myVehMax;
	float sX, sY, sZ, sH;

	if (PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		GET_PLAYER_CHAR(GetMenuPlayerId(), &networkPlayerPed);

		if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
		{
			GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
			GET_CAR_MODEL(networkPlayerVehicle, &networkPlayerVehicleModel);
			GET_MODEL_DIMENSIONS(networkPlayerVehicleModel, &netVehMin, &netVehMax);
			GET_CHAR_HEADING(myPlayerPed, &sH);

			if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
			{
				VEHICLE_INDEX myVehicle;
				GET_CAR_CHAR_IS_USING(myPlayerPed, &myVehicle);

				PED_INDEX driver;
				GET_DRIVER_OF_CAR(myVehicle, &driver);

				if (driver == myPlayerPed)
				{
					GET_CHAR_COORDINATES(myPlayerPed, &sX, &sY, &sZ);

					if (CloneVehicle(networkPlayerVehicle, sX, sY, sZ, sH, &clonedVeh))
					{
						DeleteCar(&myVehicle);
						WARP_CHAR_INTO_CAR(myPlayerPed, clonedVeh);
						clearFollowDriverData();
					}
				}
				else
				{
					GET_CAR_MODEL(myVehicle, &myVehModel);
					GET_MODEL_DIMENSIONS(myVehModel, &myVehMin, &myVehMax);
					GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(myVehicle, 0.0f, -(myVehMax.y + netVehMax.y + 2.0f), 0.0f, &sX, &sY, &sZ);

					if (CloneVehicle(networkPlayerVehicle, sX, sY, sZ, sH, &clonedVeh)) //behind current car
					{
						WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, clonedVeh, -1); //might be -1 for driver but we see.
						clearFollowDriverData();
					}
				}
			}
			else
			{
				GET_CHAR_COORDINATES(myPlayerPed, &sX, &sY, &sZ);
				if (CloneVehicle(networkPlayerVehicle, sX, sY, sZ, sH, &clonedVeh))
				{
					WARP_CHAR_INTO_CAR(myPlayerPed, clonedVeh);
					clearFollowDriverData();
				}
			}
		}
		else
			printf("~r~CTL: %s ~w~isn't in a vehicle.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
	}
}