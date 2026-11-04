#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"
#include "Functions.h"
//#include <Network_Options.h>//for stickyBitSet enum
#include "../Menu/Network_Functions.h" //for stickyBitSet enum
#include "../Menu/Functions.h"
#include "protections.h"
#include "../Menu/stdafx.h" //developer defines etc (MAKE SURE TO RELOAD THIS SOLUTION/VS ONCE MADE CHANGES)
#include "../Menu/Protection_Options.h"
int money_amount_ped_prot_marker;
int cap_playermodel_alert_bitset;

ctl_attack_network_notifier last_notified_attack[16];


void Alert(const char* message)
{
	//TODO ADD VIBRATE CONTROLLER OPTION TO THIS.
	//TODO Nath: Add alert toggle/option to protections and if this code on it
	//TODO NATH: Add a prot_alerts_tick, set it to +1000 each time this runs, and suppress network attack notifys for that duration / only add them to brief instead.
	if (!IS_THIS_PRINT_BEING_DISPLAYED("STRING", SCRIPT_PRINT_ONE_LITERAL_STRING, nullptr, message, nullptr, 0, 0, 0, 0, 0, 0))
	{
		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(true);
		PRINT_STRING_WITH_LITERAL_STRING_NOW("STRING", message, 5000, 1);
		PLAY_AUDIO_EVENT("FRONTEND_OTHER_RACE_GO");
	}
}

bool Is_Vehicles_Type_Modified(uint vehicleModel)
{
	uint compareModel = vehicleModel;
	switch (compareModel)
	{
	case VEHICLE_AIRTUG:
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
	return false; 
}

int GetNoCartelUsers(bool skip_other_admins)
{
	int count = 0;
	for (int i = 0; i < 16; i++)
	{
		if (IsPlayerValid(i))
		{
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i) && i != myPlayerIndex) //don't count other admins as they won't listen to us.
				continue; //this user won't help/vote for us or whatever.
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_admin, (i + 16)) || IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (i + 16)))
				count++;
		}
	}
	return count;
}

void freemode_invalid_suggest_fixer() // no point broadcasting this as everyone will see it.
{
	if (NETWORK_GET_GAME_MODE() == GAME_MODE_FREE_MODE)
	{
		if (*(int*)0x83081030 == 0) //freemode pointer to broadcast vars.
			return;

		//int* freemode_client_vars = (int*)0x83081030;
		bool weShouldKick = bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_AUTO_KICK);
		if (GET_HOST_ID() != myPlayerIndex)
			weShouldKick = false;
		//0x83081030 pointer to gamemode client vars.
		switch (myCurrentEpisode)
		{
		case EPISODE_ORIGINAL:
		{
			freemode_client_vars_original* freemode_client_vars = *(freemode_client_vars_original**)0x83081030;
			for (int i = 0; i < 16; i++)
			{
				if (!IsPlayerValid(i) || i == myPlayerIndex) //prob detectable when not active but eh.
					continue;

				bool attacked = false;
				if (freemode_client_vars[i].m_gameModeSuggestion > GAME_MODE_FREE_MODE || freemode_client_vars[i].m_gameModeSuggestion < -1)
				{
					freemode_client_vars[i].m_gameModeSuggestion = -1;
					attacked = true;
				}

				if (freemode_client_vars[i].m_voteToKickIndex > 15 || freemode_client_vars[i].m_voteToKickIndex < -1)
				{
					freemode_client_vars[i].m_voteToKickIndex = -1;
					attacked = true;
				}

				if (attacked)
				{
					char* buffer = BUFFER128;

					if (GET_HOST_ID() == myPlayerIndex)
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~kick ~w~you!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(i));
					else
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~kick ~w~the host!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(i));

					Alert(buffer);

					if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i) || IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (i + 16)))
						continue; //Allow admins & trusted moderators to attempt an attack without auto-kick.

					if (weShouldKick) //means we're host basically. and host autokick bit is set.
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(i, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Host singleplayer attempt", 26);

						NETWORK_KICK_PLAYER(i);
						continue;
					}
					else if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_VOTE_KICK_ATTACKERS) && !IsPlayerOnFriendsList(i, true))
						VoteKick(i);
				}
			}
		}
		break;
		case EPISODE_TLAD: //if suggests more than 24
		{
			freemode_client_vars_tlad* freemode_client_vars = *(freemode_client_vars_tlad**)0x83081030;
			for (int i = 0; i < 16; i++)
			{
				if (!IsPlayerValid(i) || i == myPlayerIndex) //prob detectable when not active but eh.
					continue;

				bool attacked = false;
				if (freemode_client_vars[i].m_gameModeSuggestion > 24 || freemode_client_vars[i].m_gameModeSuggestion < -1)
				{
					freemode_client_vars[i].m_gameModeSuggestion = -1;
					attacked = true;
				}

				if (freemode_client_vars[i].m_voteToKickIndex > 15 || freemode_client_vars[i].m_voteToKickIndex < -1)
				{
					freemode_client_vars[i].m_voteToKickIndex = -1;
					attacked = true;
				}

				if (attacked)
				{
					char* buffer = BUFFER128;

					if (GET_HOST_ID() == myPlayerIndex)
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~kick ~w~you!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(i));
					else
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~kick ~w~the host!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(i));

					Alert(buffer);

					if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i) || IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (i + 16)))
						continue; //Allow admins & trusted moderators to attempt an attack without auto-kick.

					if (weShouldKick) //means we're host basically. and host autokick bit is set.
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(i, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Host singleplayer attempt", 26);

						NETWORK_KICK_PLAYER(i);
						continue;
					}
					else if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_VOTE_KICK_ATTACKERS) && !IsPlayerOnFriendsList(i, true))
						VoteKick(i);
				}
			}
		}
		break; 
		case EPISODE_TBOGT: //if suggests more than 30
		{
			freemode_client_vars_tbogt* freemode_client_vars = *(freemode_client_vars_tbogt**)0x83081030;
			for (int i = 0; i < 16; i++)
			{
				if (!IsPlayerValid(i) || i == myPlayerIndex) //prob detectable when not active but eh.
					continue;

				bool attacked = false;
				if (freemode_client_vars[i].m_gameModeSuggestion > 30 || freemode_client_vars[i].m_gameModeSuggestion < -1)
				{
					freemode_client_vars[i].m_gameModeSuggestion = -1;
					attacked = true;
				}

				if (freemode_client_vars[i].m_voteToKickIndex > 15 || freemode_client_vars[i].m_voteToKickIndex < -1)
				{
					freemode_client_vars[i].m_voteToKickIndex = -1;
					attacked = true;
				}

				if (attacked)
				{
					char* buffer = BUFFER128;

					if (GET_HOST_ID() == myPlayerIndex)
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~kick ~w~you!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(i));
					else
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~kick ~w~the host!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(i));

					Alert(buffer);

					if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i) || IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (i + 16)))
						continue; //Allow admins & trusted moderators to attempt an attack without auto-kick.

					if (weShouldKick) //means we're host basically. and host autokick bit is set.
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(i, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Host singleplayer attempt", 26);

						NETWORK_KICK_PLAYER(i);
						continue;
					}
					else if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_VOTE_KICK_ATTACKERS) && !IsPlayerOnFriendsList(i, true))
						VoteKick(i);
				}
			}
		}
		break; 
		}

	}
}

void setOffhostKickMSGid()
{ //Packet only reads max 65535 but we can set more than that :D, this will never hit.
	*(int*)0x831CE58C = (GET_HOST_ID() == myPlayerIndex && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_IGNORE_COMPLAINTS)) ? 65536 : 0x98;
}

#ifdef Moderator
void SendAdminCommandToPlayer(int CommandID, PLAYER_INDEX playerId) 
{
	if (!bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG)) //prevent breaking health/armour / sending when CommandID is already on its way.
	{
		freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_targetPlayerID = playerId;
		freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_CommandID = CommandID;
		SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ADMIN);

		bitSet(&ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG);
		if (playerId == ADMIN_PLAYERID_ALL_PLAYERS) 
			freeMem->m_AdminCommandsTick = (GetTickCount() + 1750);
		else
			freeMem->m_AdminCommandsTick = (GetTickCount() + 2000);
	}
}
#endif

int notifiedFreezeGunPresentBitSet;

void player_has_freze_gun_notifier()
{
	PED_INDEX tmp_ped;
	for (int i = 0; i < 16; i++)
	{
		if (IsPlayerValid(i) && i != myPlayerIndex && PLAYER_HAS_CHAR(i))
		{
			GET_PLAYER_CHAR(i, &tmp_ped);
			if (DOES_CHAR_EXIST(tmp_ped))
			{
				bool hasWep;
				if (myCurrentEpisode == EPISODE_TBOGT)
					hasWep = HAS_CHAR_GOT_WEAPON(tmp_ped, WEAPON_EPISODIC_18);

				if (HAS_CHAR_GOT_WEAPON(tmp_ped, WEAPON_ROCKET) || hasWep)
				{
					if (!IS_BIT_SET(notifiedFreezeGunPresentBitSet, i))
					{
						SET_BIT(&notifiedFreezeGunPresentBitSet, i);
						printf("~r~CTL: %s ~w~is holding a ~r~freeze ~w~gun!", NetworkPlayerGetNameWithColour(i));
					}
				}
				else
					CLEAR_BIT(&notifiedFreezeGunPresentBitSet, i);
			}
		}
		else
			CLEAR_BIT(&notifiedFreezeGunPresentBitSet, i);
	}
}

void retail_freeze_gun_thrown_notifier()
{
	PED_INDEX tmp_ped;
	int tmp_wep;
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i) || !PLAYER_HAS_CHAR(i))
			continue;

		if (i == myPlayerIndex) 
			continue;
		//PRINTSTRING("Getting player char etc for freeze gun check\n");
		GET_PLAYER_CHAR(i, &tmp_ped);
		if (!IS_CHAR_IN_ANY_CAR(tmp_ped)) //doesn't crash if thrown from a vehicle
		{
			if (GET_CURRENT_CHAR_WEAPON(tmp_ped, &tmp_wep))
			{
				NETWORK_INDEX pedNetId;
				GET_NETWORK_ID_FROM_PED(tmp_ped, &pedNetId);
				if (tmp_wep == WEAPON_ROCKET || tmp_wep == WEAPON_EPISODIC_18)// || HAS_CHAR_GOT_WEAPON(freezeGunPed, WEAPON_ROCKET))
				{
					if (IS_CHAR_SHOOTING(tmp_ped)) //pretty sure it wont crash you if thrown from a vehicle
					{
						char* buffer = BUFFER128;
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Freeze gun)", NetworkPlayerGetNameWithColour(i));
						Alert(buffer);
						if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS))
						{
							ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = i;
							ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_SYNC_FREEZE_GUN + 1;
							ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = pedNetId;
							SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
							reset_attack_broadcast_tick = (GetTickCount() + 1000);
						}
					}
				}
			}
		}
	}
}
void VoteKick(PLAYER_INDEX kickTarget)
{
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL: setGlobalAtIndex(185, -1); setGlobalAtIndex(185, kickTarget); break;
	case EPISODE_TLAD: break;
	case EPISODE_TBOGT: setGlobalAtIndex(813, -1); setGlobalAtIndex(813, kickTarget); break;
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

void ctl_attack_crash_hosts_freemode(int unused1, int unused2)
{
	if (GET_HOST_ID() == myPlayerIndex)
		return;

	if (NETWORK_GET_GAME_MODE() == GAME_MODE_FREE_MODE) //could do others in future i guess / need tbogt compat.
	{
		if (ctlMem->m_hostFreemodeCrasherTick == 0) //not currently running.
		{
			int rand;
			GENERATE_RANDOM_INT_IN_RANGE(35, 0x1337, &rand);
			SuggestGamemode(rand); //out of bounds / crash freemode script.
			//VoteKick(25);
			ctlMem->m_hostFreemodeCrasherTick = (GetTickCount() + 500); //suggest -1 in network_loops.c after 500ms
			//VoteKick(-1);
			printf("~r~CTL: ~w~Executed, give it a moment.");
		}
	}
}

void network_attack_notifier_loop()
{
	for (int i = 0; i < 16; i++)
	{
		if (IsPlayerValid(i))
		{
			if (i == myPlayerIndex)
				continue;

			if (ctlMem->m_CartelNetAttackVars.m_Data[i].offender_attackType[0] == 255 || ctlMem->m_CartelNetAttackVars.m_Data[i].offender_attackType[1] == 255)
				continue;

			CTL_ATTACK_TYPES net_attack_type = ctlMem->m_CartelNetAttackVars.m_Data[i].offender_attackType[1];

			if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_LISTEN_FOR_NETUSER_ATTACKS_FRIENDS_ONLY))
			{
				if (!IsPlayerOnFriendsList(i, bitTest(ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_CHECK_FRIENDS_ONLINE_STATUS)))
					continue;
			}

			if (net_attack_type > 0) //avoid dud notifications (we send + 1)
				net_attack_type -= 1;
			else
				continue;

			PLAYER_INDEX net_attacker = ctlMem->m_CartelNetAttackVars.m_Data[i].offender_attackType[0];
			uint net_extraData = ctlMem->m_CartelNetAttackVars.m_Data[i].extraData;

			if (net_attacker == last_notified_attack[i].offender_attackType[0] && last_notified_attack[i].extraData == net_extraData && last_notified_attack[i].offender_attackType[1] == net_attack_type)
				continue;
			else
			{ //we've not displayed/processed this already.
				last_notified_attack[i].extraData = net_extraData;
				last_notified_attack[i].offender_attackType[0] = net_attacker;
				last_notified_attack[i].offender_attackType[1] = net_attack_type;


				/*			//ATTACKER CAN BE -1 SOMETIMES!!!!!! //dealt with.
										applies to
							ATTACK_PEER_DATA
							ATTACK_REASSIGN
							ATTACK_SYNC_ATTACHMENT (can apply if we can't find owner)
							ATTACK_SYNC_TASK_QUERY*/
				bool weShouldKick = false;

				bool victimsOnFriendsList = IsPlayerOnFriendsList(i, true);
				bool attackersOnFriendsList = IsPlayerOnFriendsList(net_attacker, true);

				bool attacker_unknown;
				if ((net_attack_type == ATTACK_EVENT_EXPLOSION && i != GET_HOST_ID()) || (net_attacker == i || net_attacker == INVALID_PLAYER_INDEX)) //if explosion lag and they're not host.
					attacker_unknown = true;

				if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i) || IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (i + 16)))
				{ //reporter is an admin/moderator
					attackersOnFriendsList = false; //don't skip the attacker.
					victimsOnFriendsList = true; //treat the admin/moderator as a friend
				}

				if (net_attacker != INVALID_PLAYER_INDEX) //-1 in sco, even tho XEX's is diff.
				{
					if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, net_attacker) || IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (net_attacker + 16)))
					{  //attacker is an admin/moderator
						attackersOnFriendsList = true; //don't kick de admins/mods
					}
				}

				if (net_attacker != myPlayerIndex && victimsOnFriendsList && !attackersOnFriendsList) 
				{
					if (net_attack_type != ATTACK_SYNC_ATTACHMENT && net_attack_type != ATTACK_CLONE_SPAM && !attacker_unknown) ////don't auto-fuck for object attachments, we don't kick for it ourselves.
					{
#ifdef Developer
						if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, myPlayerIndex)) //check we're an admin locally just for extra security.
						{
							if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_ADMIN_ONLY_OFFHOST_ATTACKS_AGAINST_FRIENDS))
							{

								if (GET_HOST_ID() != net_attacker && (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (GET_HOST_ID() + 16)) || IS_BIT_SET(ctlMem->m_bit_sets.m_admin, (GET_HOST_ID() + 16))))
								{ //host is running cartel, we can send an admin command to them to off-host the attacker.
									SendAdminCommandToPlayer(net_attacker + ADMIN_CMD_KICK_CONSOLE_BASE_ID_0, GET_HOST_ID());
								}
								else
								{
									if (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (net_attacker + 16)) || IS_BIT_SET(ctlMem->m_bit_sets.m_admin, (net_attacker + 16)))
									{ //attacker is dumb enough to attack a friend while running cartel, send em to singleplayer.
										SendAdminCommandToPlayer(ADMIN_CMD_SINGLEPLAYER, net_attacker);
									}
								}
								if (!bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG))
								{ //we're not sending a command right now / didn't do that above.
									if (GetNoCartelUsers(true) > (GET_NUMBER_OF_PLAYERS() / 2))
									{ //leverage our user count to get them kicked via votes.
										SendAdminCommandToPlayer(net_attacker + ADMIN_CMD_VOTE_KICK_TRUE_PLAYERID_BASE_0, ADMIN_PLAYERID_ALL_PLAYERS);
										VoteKick(net_attacker);
									}
								}

								if (net_attacker == GET_HOST_ID())
									ctl_attack_crash_hosts_freemode(0, 0);
								else
								{
									if (myPlayerIndex != GET_HOST_ID()) //only run if we're not host.
										CTL_OFFHOST_KICK(net_attacker);
								}
							}
						}
#endif
						if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_VOTE_KICK_FRIENDS_ATTACKER))
							VoteKick(net_attacker);
					}
				}

				if (GET_HOST_ID() == myPlayerIndex && victimsOnFriendsList)
					weShouldKick = (!attackersOnFriendsList && net_attacker != myPlayerIndex && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_AUTO_KICK) && bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_HOST_AUTO_KICK_ATTACKERS_AGAINST_FRIENDS));

				//if ((net_attack_type != ATTACK_SYNC_ATTACHMENT && net_attack_type != ATTACK_CLONE_SPAM) || (attack_type == ATTACK_EVENT_EXPLOSION && GET_HOST_ID() == myPlayerIndex)) //don't auto-fuck for object attachments, we don't k
				if (net_attack_type == ATTACK_CLONE_SPAM || net_attack_type == ATTACK_SYNC_ATTACHMENT || attacker_unknown) //not reliable at this moment in time.
					weShouldKick = false;

				switch ((int)net_attack_type)
				{
				case ATTACK_OUT_OF_BOUNDS_ARRAY:
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~~w~(O.O.B Array)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Out of bounds freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_GROUP_HACK:
				{
					if(attacker_unknown) //they're on retail / can't accurately determine an attacker.
						printf("~r~CTL: ~w~Someone tried to ~r~freeze ~w~%s!~n~(Group hack)", GET_PLAYER_NAME(i)); //could check if there's only 1 other 360 in the game aside from them / friends but meh.
					else
					{
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Group hack)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						if (weShouldKick)
						{
							uint kicked_array_index;
							if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, KICK_REASON_GROUP_HACK, 26);

							NETWORK_KICK_PLAYER(net_attacker);
						}
					}
				}
				break;
				case ATTACK_CLONE_SPAM: // extra data = //object type
				{
					if(net_extraData >= 0 && net_extraData < 10)
						printf("~r~CTL: %s ~w~could be trying to ~y~lag ~w~%s!~n~(%s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[net_extraData]);
					else
						printf("~r~CTL: %s ~w~coule be trying to ~y~lag ~w~%s!~n~(type index O.O.B)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
				}
				break; 
				case ATTACK_CLONE_PLAYER: //extra data = player index they sent
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(PlayerID %i)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), net_extraData);
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, KICK_REASON_CLONE_PLAYER, 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_PED: //extra data = model hash
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Ped %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Ped 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Ped %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid ped freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_DUMMYPED: //extra data = model hash
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Dummy ped %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Dummy ped 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Dummy ped %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid dummy ped freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_CAR: //extra data = model hash
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Car %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Car 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Car %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid car freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_OBJECT: //extra data = model hash
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid object freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_BIKE: //extra data = model hash
				{
					
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid bike freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_TRAIN: //extra data = model hash
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Train %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Train 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Train %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid train freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_HELI: //extra data = model hash
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Heli %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Heli 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Heli %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid heli freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_BOAT: //extra data = model hash
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(%s %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid boat freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_CLONE_PLANE: //extra data = model hash
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Plane %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Plane 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Plane %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid plane freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_EVENT_REQUEST_PICKUP:
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Request Pickup Event)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Request Pickup freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_EVENT_RESURRECT:
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Resurrect Event)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Resurrect event freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_EVENT_REMOVE_WEAPON: //extra data = model hash
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Weapon Event)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Weapon event freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_EVENT_EXPLOSION:
				{
					//if (i != GET_HOST_ID() || net_attacker == i) //they won't know who the attacker is / always comes from host if you're a client
					if (attacker_unknown) //they won't know who the attacker is / always comes from host if you're a client
						printf("~r~CTL: ~w~Someone tried to ~y~lag ~w~%s!~n~(Explosions)", GET_PLAYER_NAME(i)); //could check if there's only 1 other 360 in the game aside from them / friends but meh.
					else
					{
						printf("~r~CTL: %s ~w~tried to ~y~lag ~w~%s!~n~(Explosions)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						/*if (weShouldKick)
						{
							uint kicked_array_index;
							if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, KICK_REASON_GROUP_HACK, 26);

							NETWORK_KICK_PLAYER(net_attacker);
						}*/
					}
				}
				break; 
				case ATTACK_EVENT_WANTED_LEVEL: //extra data = model hash
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(WantedLevel Event)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "WantedLevel event freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_EVENT_CREATE_PICKUP: //extra data = model hash
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Pickup Event)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "CreatePickup event freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_EVENT_RADIO_STATION: //extra data = model hash
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Radio Event)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Radio event freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_TELL_NET: //extra data = model hash
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Blackscreen)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Blackscreen freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_PEER_DATA: //extra data = model hash
				{
					if(attacker_unknown)
						printf("~r~CTL: ~w~Someone tried to ~r~freeze ~w~%s!~n~(Peer data)", GET_PLAYER_NAME(i));
					else
					{
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Peer data)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						if (weShouldKick)
						{
							uint kicked_array_index;
							if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid peer data freeze", 26);

							NETWORK_KICK_PLAYER(net_attacker);
						}
					}
				}
				break;
				case ATTACK_REASSIGN:  //extra data = model hash
				{
					if(attacker_unknown)
						printf("~r~CTL: ~w~Someone tried to ~r~freeze ~w~%s!~n~(Re-assign)", GET_PLAYER_NAME(i));
					else
					{
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Re-assign)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						if (weShouldKick)
						{
							uint kicked_array_index;
							if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid re-assign freeze", 26);

							NETWORK_KICK_PLAYER(net_attacker);
						}
					}
				}
				break;
				case ATTACK_PLAYER_NAME: //extra data = model hash
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Player name)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Player name freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_SYNC_ATTACHMENT:
				{
					char* entityType = "Unknown";
					if (DOES_OBJECT_EXIST_WITH_NETWORK_ID(net_extraData))
						entityType = ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT]; //"Object"

					if (DOES_PED_EXIST_WITH_NETWORK_ID(net_extraData))
					{
						entityType = ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_PED]; //"Ped";
						PED_INDEX netPed;
						GET_PED_FROM_NETWORK_ID(net_extraData, &netPed);
						if (IsThisPedAPlayer(netPed) != INVALID_PLAYER_INDEX)
							entityType = ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_PLAYER]; //"Player"
					}
					if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(net_extraData))
					{
						VEHICLE_INDEX netVeh;
						GET_VEHICLE_FROM_NETWORK_ID(net_extraData, &netVeh);
						entityType = VehiclesModelName(netVeh);
					}
					if(attacker_unknown)
						printf("~r~CTL: ~w~Someone attached an ~y~entity ~w~to %s.~n~(%s)", GET_PLAYER_NAME(i), entityType);
					else
						printf("~r~CTL: %s ~w~attached an ~y~entity ~w~to %s.~n~(%s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), entityType);
				}
				break;
				case ATTACK_SYNC_ANIM_TASK: 
				{
					if (DOES_PED_EXIST_WITH_NETWORK_ID(net_extraData))
					{
						PED_INDEX attackPed;
						GET_PED_FROM_NETWORK_ID(net_extraData, &attackPed);
						if (IsThisPedAPlayer(attackPed) == -1)
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Animation ped)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Animation)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					}
					else
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Animation)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					//printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Animation)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Animation freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_SYNC_RELATIONSHIP_GROUP: //extra data = model hash
				{
					printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Relationship group)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Relationship group freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_SYNC_TASK_QUERY: //extra data = model hash
				{
					if(attacker_unknown)
						printf("~r~CTL: ~w~Someone tried to ~r~freeze ~w~%s!~n~(Task)", GET_PLAYER_NAME(i));
					else
					{
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Task)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						if (weShouldKick)
						{
							uint kicked_array_index;
							if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Task freeze", 26);

							NETWORK_KICK_PLAYER(net_attacker);
						}
					}
				}
				break;
				case ATTACK_SYNC_FREEZE_GUN: //extra data = model hash
				{
					if (DOES_PED_EXIST_WITH_NETWORK_ID(net_extraData))
					{
						PED_INDEX attackPed;
						GET_PED_FROM_NETWORK_ID(net_extraData, &attackPed);
						if (IsThisPedAPlayer(attackPed) == -1)
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Freeze gun ped)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Freeze gun)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					}
					else
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Freeze gun)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					//printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Freeze gun)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Freeze gun", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_SYNC_WEAPON_INDEX: 
				{
					if (DOES_PED_EXIST_WITH_NETWORK_ID(net_extraData))
					{
						PED_INDEX attackPed;
						GET_PED_FROM_NETWORK_ID(net_extraData, &attackPed);
						if (IsThisPedAPlayer(attackPed) == -1)
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Peds weapon)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Weapon)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					}
					else
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Weapon)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					//printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Weapon)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid Weapon freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_SYNC_ANIMATION_GROUP: //extra data = model hash
				{
					if (DOES_PED_EXIST_WITH_NETWORK_ID(net_extraData))
					{
						PED_INDEX attackPed;
						GET_PED_FROM_NETWORK_ID(net_extraData, &attackPed);
						if (IsThisPedAPlayer(attackPed) == -1)
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Peds anim group)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Animation group)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));
					}
					else
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Animation group)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i));

					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Animation group freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_SYNC_MODEL_CHANGE: //extra data = model
				{
					if (IS_THIS_MODEL_A_VEHICLE(net_extraData))
						printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Model %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(net_extraData)));
					else
					{
						bool cutScene; char* badModelName = bad_player_model_name(net_extraData, &cutScene);
						if (COMPARE_STRING(badModelName, "Unkn"))
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Model 0x%x)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), net_extraData);
						else
							printf("~r~CTL: %s ~w~tried to ~r~freeze ~w~%s!~n~(Model %s)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(i), badModelName);
					}
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(net_attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Model change freeze", 26);

						NETWORK_KICK_PLAYER(net_attacker);
					}
				}
				break;
				case ATTACK_SYNC_INVALID_WEAPON_THROW:
				{
					printf("~r~CTL: ~w~Someone tried to ~r~freeze %s!~n~(Invalid Weapon Throw)", NetworkPlayerGetNameWithColour(i));
				}
				break;
				/* //retails will get notified too now so no point checking for this.
				case ATTACK_SINGLEPLAYER_KICK:
				{
					if(!bitTest(*(int*)stickyBitSet, STICK_XEX_PRESENT)) //we will be notified ourselves.
						printf("~r~CTL: %s ~w~tried to ~r~kick ~w~%s!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(net_attacker), GET_PLAYER_NAME(GET_HOST_ID()));
				}
				break;
				*/
				} //end switch
			}
		}
		else
		{
			last_notified_attack[i].offender_attackType[0] = 255; //clear so a normal player doesn't potentially cause a fuck-up when joining.
			last_notified_attack[i].offender_attackType[1] = 255;
			last_notified_attack[i].extraData = -1337;
		}
	}
}

#pragma region kicked_consoles

char* kickedMatchTypeStrings[3] = { "MachineID" , "IP Address", "MAC Address" };
void KickedConsoleScanner()
{
	for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
	{
		if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID == 0)
			continue;//nothing stored here

		for (int j = 0; j < 16; j++)
		{
			if (!IsPlayerValid(j))
				continue;

			CPlayerInfo* player = GetPlayerPointer(j);
			if (player == NULL)
				continue;

			int matchType = 0;

			if (player->m_machineId[0] == freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID)
				matchType = 1;
			else if (player->m_wan_ip == freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress)
				matchType = 2;
			else if (memcmp(&player->_2port_6mac_2pad[2], freeMem->m_cartelHostVariables.m_kicked_storage[i].MAC, 6) == 0)
				matchType = 3;

			if (matchType)
			{
				freeMem->m_cartelHostVariables.m_kicked_storage[i].failsCounter++;
				printf("~r~CTL: ~w~Kicked %s %s detected as %s, re-join fails: %i", kickedMatchTypeStrings[matchType - 1], freeMem->m_cartelHostVariables.m_kicked_storage[i].gamertag, SafeNetworkPlayerGetName(j), freeMem->m_cartelHostVariables.m_kicked_storage[i].failsCounter);
				uint arr_index;
				AddPlayerToKickStorage(j, &arr_index); //won't add unless IP Address or MachineId has changed. (deals with XUID clearing regardless)
				NETWORK_KICK_PLAYER(j);
			}
		}
	}
}

/* //BACKUP
void KickedConsoleScanner()
{
	for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
	{
		if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID == 0)
			continue;//nothing stored here

		for (int j = 0; j < 16; j++)
		{
			if (!IsPlayerValid(j))
				continue;

			int matchType = 0;
			//if (IsPlayerUsingXboxOne(j)) continue; //spoofable now.. //chances of double match in one session are insanely small i'd imagine.

			if (GetPlayerMachineId(j) == freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID)
				matchType = 1;
			else if (GetPlayerIpAddress(j) == freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress)
				matchType = 2;

			if (matchType)
			{
				freeMem->m_cartelHostVariables.m_kicked_storage[i].failsCounter++;
				printf("~r~CTL: ~w~Kicked %s %s detected as %s, re-join fails: %i", kickedMatchTypeStrings[matchType - 1], freeMem->m_cartelHostVariables.m_kicked_storage[i].gamertag, SafeNetworkPlayerGetName(j), freeMem->m_cartelHostVariables.m_kicked_storage[i].failsCounter);
				*(int*)(GetPlayerPointerNath(j) + 0x40) = 0;
				*(int*)(GetPlayerPointerNath(j) + 0x44) = 0; //Set players XUID to 0 so whatever they spoof to doesn't get blacklisted.
				NETWORK_KICK_PLAYER(j);
			}
		}
	}
}*/
#pragma endregion 

#pragma region player_model_functions
uint last_player_model;
uint change_player_tick;
//uint flip_model; //TODO dynamically change this if we're GMAF_HI_02 maybe.
void change_player_model_temporarily(const int model, const int ms)
{
	uint myModel;
	GET_CHAR_MODEL(myPlayerPed, &myModel);
	if (myModel != PED_M_Y_GMAF_HI_02) //stops spamming forcing you to stick to PED_M_Y_GMAF_HI_02
		last_player_model = myModel;
	//if (last_player_model == 0) GET_CHAR_MODEL(myPlayerPed, &last_player_model); //stops spamming forcing you to stick to PED_M_Y_GMAF_HI_02
	REQUEST_MODEL(last_player_model); //make it fastah
	change_player_model(model);
	change_player_tick = (GetTickCount() + ms);
}


int GetNearestSlodOrNikoPlayerPed(float mx, float my, float mz)
{
	PED_INDEX tmp_ped, Closest_niko_ped = 1337;
	float nx, ny, nz, distance, lowest_distance = 70.0f;
	for (int i = 0; i < 16; i++)
	{
		if (!IS_BIT_SET(cap_playermodel_alert_bitset, i) || !IsPlayerValid(i) || !PLAYER_HAS_CHAR(i))
			continue;
		GET_PLAYER_CHAR(i, &tmp_ped);
		GET_CHAR_COORDINATES(tmp_ped, &nx, &ny, &nz);
		GET_DISTANCE_BETWEEN_COORDS_3D(nx, ny, nz, mx, my, mz, &distance);
		if (distance < lowest_distance)
		{
			lowest_distance = distance;
			Closest_niko_ped = tmp_ped;
		}
	}
	if (Closest_niko_ped != 1337)
	{
		return Closest_niko_ped;
	}
	return -1;
}
#pragma endregion

#pragma region ped_protect_functions
eDodgyPedType is_wModelID_a_Dodgy_Ped(short wModelID) //make int type, 1 can be Niko, 2 can be slod, 3 can be cutscene, 4 can be valid.
{
	switch (wModelID)
	{
	case 211: return DODGY_PED_PLAYER; //PED_PLAYER //wModelID 211
	case 214: return DODGY_PED_SUPERLOD; //PED_SUPERLOD //wModelID 214
		break;
	}
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL:
		if (wModelID > 272 && wModelID < 363) return DODGY_PED_CUTSCENE;//Original CutScene Peds Range
		break;
	case EPISODE_TBOGT:
		if ((wModelID > 272 && wModelID < 363) || (wModelID > 691 && wModelID < 739)) return DODGY_PED_CUTSCENE;
		break;
		//range for CS OG 273 - 362
		////692 - 738 TBoGT CutScene
	}
	//if (!is_wModelID_a_Ped(wModelID)) return DODGY_PED_INVALID;
	return DODGY_PED_NONE;
}

char* dodgyPedModelName(int model, bool *cutScene) //could make multiplayer models return UNKNOWN to make it faster... meh
{
	*cutScene = false;

	if(model != PED_PLAYER && model != PED_SUPERLOD)
		*cutScene = true;

	bool trash;

	switch (model) //OG MP Ped Freezes..
	{
	case PED_PLAYER: return bad_player_model_name(model, &trash);
	case PED_SUPERLOD: return "SuperLOD"; break;

	//OG Cutscene Peds;
	case PED_CS_ANDREI: return "CS_ANDREI";
	case PED_CS_ANGIE: return "CS_ANGIE";
	case PED_CS_BADMAN: return "CS_BADMAN";
	case PED_CS_BLEDAR: return "CS_BLEDAR";
	case PED_CS_BULGARIN: return "CS_BULGARIN";
	case PED_CS_BULGARINHENCH: return "CS_BULGARINHENCH";
	case PED_CS_CIA: return "CS_CIA";
	case PED_CS_DARDAN: return "CS_DARDAN";
	case PED_CS_DAVETHEMATE: return "CS_DAVETHEMATE";
	case PED_CS_DMITRI: return "CS_DMITRI";
	case PED_CS_EDTHEMATE: return "CS_EDTHEMATE";
	case PED_CS_FAUSTIN: return "CS_FAUSTIN";
	case PED_CS_FRANCIS: return "CS_FRANCIS";
	case PED_CS_HOSSAN: return "CS_HOSSAN";
	case PED_CS_ILYENA: return "CS_ILYENA";
	case PED_CS_IVAN: return "CS_IVAN";
	case PED_CS_JAY: return "CS_JAY";
	case PED_CS_JIMMY_PEGORINO: return "CS_JIMMY_PEGORINO";
	case PED_CS_MEL: return "CS_MEL";
	case PED_CS_MICHELLE: return "CS_MICHELLE";
	case PED_CS_MICKEY: return "CS_MICKEY";
	case PED_CS_OFFICIAL: return "CS_OFFICIAL";
	case PED_CS_RAY_BOCCINO: return "CS_RAY_BOCCINO";
	case PED_CS_SERGEI: return "CS_SERGEI";
	case PED_CS_VLAD: return "CS_VLAD";
	case PED_CS_WHIPPINGGIRL: return "CS_WHIPPINGGIRL";
	case PED_CS_MANNY: return "CS_MANNY";
	case PED_CS_ANTHONY: return "CS_ANTHONY";
	case PED_CS_ASHLEY: return "CS_ASHLEY";
	case PED_CS_ASSISTANT: return "CS_ASSISTANT";
	case PED_CS_CAPTAIN: return "CS_CAPTAIN";
	case PED_CS_CHARLIEUC: return "CS_CHARLIEUC";
	case PED_CS_DARKO: return "CS_DARKO";
	case PED_CS_DWAYNE: return "CS_DWAYNE";
	case PED_CS_ELI_JESTER: return "CS_ELI_JESTER";
	case PED_CS_ELIZABETA: return "CS_ELIZABETA";
	case PED_CS_GAYTONY: return "CS_GAYTONY";
	case PED_CS_GERRYMC: return "CS_GERRYMC";
	case PED_CS_GORDON: return "CS_GORDON";
	case PED_CS_ISSAC: return "CS_ISSAC";
	case PED_CS_JOHNNYTHEBIKER: return "CS_JOHNNYTHEBIKER";
	case PED_CS_JONGRAVELLI: return "CS_JONGRAVELLI";
	case PED_CS_JORGE: return "CS_JORGE";
	case PED_CS_KAT: return "CS_KAT";
	case PED_CS_KILLER: return "CS_KILLER";
	case PED_CS_LUIS: return "CS_LUIS";
	case PED_CS_MAGICIAN: return "CS_MAGICIAN";
	case PED_CS_MAMC: return "CS_MAMC";
	case PED_CS_MELODY: return "CS_MELODY";
	case PED_CS_MITCHCOP: return "CS_MITCHCOP";
	case PED_CS_MORI: return "CS_MORI";
	case PED_CS_PBXGIRL2: return "CS_PBXGIRL2";
	case PED_CS_PHILB: return "CS_PHILB";
	case PED_CS_PLAYBOYX: return "CS_PLAYBOYX";
	case PED_CS_PRIEST: return "CS_PRIEST";
	case PED_CS_RICKY: return "CS_RICKY";
	case PED_CS_TOMMY: return "CS_TOMMY";
	case PED_CS_TRAMP: return "CS_TRAMP";
	case PED_CS_BRIAN: return "CS_BRIAN";
	case PED_CS_CHARISE: return "CS_CHARISE";
	case PED_CS_CLARENCE: return "CS_CLARENCE";
	case PED_CS_EDDIELOW: return "CS_EDDIELOW";
	case PED_CS_GRACIE: return "CS_GRACIE";
	case PED_CS_JEFF: return "CS_JEFF";
	case PED_CS_MARNIE: return "CS_MARNIE";
	case PED_CS_MARSHAL: return "CS_MARSHAL";
	case PED_CS_PATHOS: return "CS_PATHOS";
	case PED_CS_SARAH: return "CS_SARAH";
	case PED_CS_ROMAN_D: return "CS_ROMAN_D";
	case PED_CS_ROMAN_T: return "CS_ROMAN_T";
	case PED_CS_ROMAN_W: return "CS_ROMAN_W";
	case PED_CS_BRUCIE_B: return "CS_BRUCIE_B";
	case PED_CS_BRUCIE_T: return "CS_BRUCIE_T";
	case PED_CS_BRUCIE_W: return "CS_BRUCIE_W";
	case PED_CS_BERNIE_CRANEC: return "CS_BERNIE_CRANEC";
	case PED_CS_BERNIE_CRANET: return "CS_BERNIE_CRANET";
	case PED_CS_BERNIE_CRANEW: return "CS_BERNIE_CRANEW";
	case PED_CS_LILJACOB_B: return "CS_LILJACOB_B";
	case PED_CS_LILJACOB_J: return "CS_LILJACOB_J";
	case PED_CS_MALLORIE_D: return "CS_MALLORIE_D";
	case PED_CS_MALLORIE_J: return "CS_MALLORIE_J";
	case PED_CS_MALLORIE_W: return "CS_MALLORIE_W";
	case PED_CS_DERRICKMC_B: return "CS_DERRICKMC_B";
	case PED_CS_DERRICKMC_D: return "CS_DERRICKMC_D";
	case PED_CS_MICHAEL_B: return "CS_MICHAEL_B";
	case PED_CS_MICHAEL_D: return "CS_MICHAEL_D";
	case PED_CS_PACKIEMC_B: return "CS_PACKIEMC_B";
	case PED_CS_PACKIEMC_D: return "CS_PACKIEMC_D";
	case PED_CS_KATEMC_D: return "CS_KATEMC_D";
	case PED_CS_KATEMC_W: return "CS_KATEMC_W";

	 //TBoGT Cutscene Peds
	case PED_CS_BAZ: return "CS_BAZ";
	case PED_CS_JEREMY: return "CS_JEREMY";
	case PED_CS_ABDUL: return "CS_ABDUL";
	case PED_CS_ABODYGUARD: return "CS_ABODYGUARD";
	case PED_CS_ADRIANA: return "CS_ADRIANA";
	case PED_CS_AHMAD: return "CS_AHMAD";
	case PED_CS_ARMANDO: return "CS_ARMANDO";
	case PED_CS_ARNAUD: return "CS_ARNAUD";
	case PED_CS_BATHROOM: return "CS_BATHROOM";
	case PED_CS_BILLY2: return "CS_BILLY2";
	case PED_CS_BULGARIN2: return "CS_BULGARIN2";
	case PED_CS_BRUCIE2: return "CS_BRUCIE2";
	case PED_CS_DAISY: return "CS_DAISY";
	case PED_CS_DERRICK2: return "CS_DERRICK2";
	case PED_CS_DESSIE: return "CS_DESSIE";
	case PED_CS_DOCKGOON: return "CS_DOCKGOON";
	case PED_CS_EUGENE: return "CS_EUGENE";
	case PED_CS_EVAN2: return "CS_EVAN2";
	case PED_CS_GPARTY_01: return "CS_GPARTY_01";
	case PED_CS_GRACIE2: return "CS_GRACIE2";
	case PED_CS_HENRIQUE: return "CS_HENRIQUE";
	case PED_CS_ISSAC2: return "CS_ISSAC2";
	case PED_CS_JOHNNY2: return "CS_JOHNNY2";
	case PED_CS_MARGOT: return "CS_MARGOT";
	case PED_CS_MAURICE: return "CS_MAURICE";
	case PED_CS_MORI_K: return "CS_MORI_K";
	case PED_CS_MR_SANTOS: return "CS_MR_SANTOS";
	case PED_CS_NIKO2: return "CS_NIKO2";
	case PED_CS_OYVEY: return "CS_OYVEY";
	case PED_CS_PACKIE2: return "CS_PACKIE2";
	case PED_CS_PAPI: return "CS_PAPI";
	case PED_CS_ROCCO: return "CS_ROCCO";
	case PED_CS_ROMAN2: return "CS_ROMAN2";
	case PED_CS_RUS_KN: return "CS_RUS_KN";
	case PED_CS_SHARON: return "CS_SHARON";
	case PED_CS_TAHIR: return "CS_TAHIR";
	case PED_CS_TIMUR: return "CS_TIMUR";
	case PED_CS_TONY: return "CS_TONY";
	case PED_CS_TONYAQ: return "CS_TONYAQ";
	case PED_CS_TRAMP2: return "CS_TRAMP2";
	case PED_CS_TRIAD: return "CS_TRIAD";
	case PED_CS_TROY: return "CS_TROY";
	case PED_CS_VIC: return "CS_VIC";
	case PED_CS_VICGIRL: return "CS_VICGIRL";
	case PED_CS_VINCE: return "CS_VINCE";
	case PED_CS_YUSEF: return "CS_YUSEF";

	//TLaD Cutscene Peds
	case PED_CS_NIKO: return "CS_NIKO";
	case PED_CS_ANDREAS: return "CS_ANDREAS";
	case PED_CS_ANGELGUY: return "CS_ANGELGUY";
	case PED_CS_ANGUS: return "CS_ANGUS";
	case PED_CS_ASHLEYA: return "CS_ASHLEYA";
	case PED_CS_BILLY: return "CS_BILLY";
	case PED_CS_BIKESTRIPPER: return "CS_BIKESTRIPPER";
	case PED_CS_BRIANJ: return "CS_BRIANJ";
	case PED_CS_CLAY: return "CS_CLAY";
	case PED_CS_DAVE_GROSSMAN: return "CS_DAVE_GROSSMAN";
	case PED_CS_DESEAN: return "CS_DESEAN";
	case PED_CS_ELIZABETAT: return "CS_ELIZABETAT";
	case PED_CS_FRANKIE: return "CS_FRANKIE";
	case PED_CS_JASON: return "CS_JASON";
	case PED_CS_JIM_FITZ: return "CS_JIM_FITZ";
	case PED_CS_MAITRE_D: return "CS_MAITRE_D";
	case PED_CS_MALC: return "CS_MALC";
	case PED_CS_MITCHB: return "CS_MITCHB";
	case PED_CS_MARTA: return "CS_MARTA";
	case PED_CS_MASSEUSE: return "CS_MASSEUSE";
	case PED_CS_MATTHEWS: return "CS_MATTHEWS";
	case PED_CS_MCCORNISH: return "CS_MCCORNISH";
	case PED_CS_PAUL_LAMONT: return "CS_PAUL_LAMONT";
	case PED_CS_PRISONDLC_01: return "CS_PRISONDLC_01";
	case PED_CS_PRISONGUARD: return "CS_PRISONGUARD";
	case PED_CS_RAYGOON: return "CS_RAYGOON";
	case PED_CS_RAY_LEATHER: return "CS_RAY_LEATHER";
	case PED_CS_RONCERO: return "CS_RONCERO";
	case PED_CS_RUSSIAN_GOON: return "CS_RUSSIAN_GOON";
	case PED_CS_RUSSIANGOON2: return "CS_RUSSIANGOON2";
	case PED_CS_TERRY: return "CS_TERRY";
	case PED_CS_SKANK: return "CS_SKANK";
	case PED_CS_STUBBS: return "CS_STUBBS";
	case PED_CS_STUBBS_SUIT: return "CS_STUBBS_SUIT";
	}
	return "UNKNOWN";
}


char* bad_player_model_name(uint playerModel, bool *cutScene)
{
	*cutScene = false;
	const uint storeModel = playerModel;
	if (playerModel == PED_SUPERLOD)
		return "SuperLOD";

	if (playerModel == PED_PLAYER)
	{
		switch (myCurrentEpisode)
		{
		case EPISODE_ORIGINAL: return "Niko";
		case EPISODE_TLAD: return "Johnny";
		case EPISODE_TBOGT: return "Luis";
		}
	}

	//we didn't resolve so lets try the others.
	char* otherString = BUFFER32;
	strcpy(otherString, dodgyPedModelName((int)storeModel, cutScene), 32);
	if (!COMPARE_STRING(otherString, "UNKNOWN"))
		return otherString;

	return "Unkn";
}

#pragma endregion

#pragma region object_prot_functions
bool IsKnownCollisionLagModel(const int model)
{
	return (model == OBJECT_GB_PARK_OLD_SWING); //storms menu (F&Dv2); //OBJECT_GB_PARK_OLD_SWING
	//return false;
}

bool IsKnownLagLightModel(const int model)
{
	switch (model)
	{
	case OBJECT_WS_SUBLIGHT: //Used in MD
	case OBJECT_NJ5_FIREPIP02:
	case OBJECT_CJ_WS_T_BASE_DAM:
	case OBJECT_SUBLIGHT_MS:
	case OBJECT_CJ_WALL_LIGHT_2:
		return true;
	}
	return false;
	//OBJECT_GB_PARK_OLD_SWING = 0X4D294E4E, collision lag..
	//OBJECT_WS_SUBLIGHTNOS = 0X4585E2BA,
	//OBJECT_WS_SUBLIGHTSML = 0X28CE67D3,
}


eObjectProcess GetObjectAction(const int object, const int object_NetID, float player_x, float player_y, float player_z, float height, const int myPed, const int myVehicle, const int myVehicleDriver)
{

	//OBJECT_PROCESS_PREVIOUS_LAGLIGHT
	uint obj_model;
	float ox, oy, oz, distance;
	GET_OBJECT_MODEL(object, &obj_model);
	/*
	if (!IS_THIS_MODEL_A_PED(tmp_model)) //if this fails try just set char visible.. as they might act slightly differently.
	{
		char buffer[96];
		strcpy_s(buffer, "~r~IIV: ~s~");
		if (IS_THIS_MODEL_A_VEHICLE(tmp_model))
		{
			SET_CAR_VISIBLE(handle, false);
			stradd_s(buffer, GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(tmp_model)));
		}
		else //model must be an object
		{
			SET_OBJECT_VISIBLE(handle, false);
			stradd_s(buffer, "Object 0x");
			stradd_s(buffer, IntToHex(tmp_model, false));
		}
		stradd_s(buffer, " detected in ~y~Ped ~s~pool, blocked.");
		Alert(buffer);
	}
	*/
	GET_OBJECT_COORDINATES(object, &ox, &oy, &oz);
	GET_DISTANCE_BETWEEN_COORDS_3D(ox, oy, oz, player_x, player_y, player_z, &distance);
	/*
	#ifndef PRIVATE //private is 360 only (as of now) and this isn't an issue on 360..
	#if PLATFORM == PLATFORM_PC
		if (true)
	#else
		if (!IS_XBOX360_VERSION()) //basically if PS3
	#endif
		{
			if (IS_THIS_MODEL_A_VEHICLE(obj_model) && distance < 80.0f) //yeah cuz tick delay, maybe even 85 or 90
			{
				return OBJECT_PROCESS_OBJ_FREZ_VEHICLE; //PC & PS3 only
			}
		}
	#endif
	*/
	if (IS_OBJECT_ATTACHED(object))
	{
		if (GET_PED_OBJECT_IS_ATTACHED_TO(object) == myPed)
		{
			//																	Parachute model
			if ((ctlMem->m_retail_prot.m_obj_prot_mode == OBJ_PROT_MODE_VEHICLE_ONLY) || (obj_model == 1490460832 && height > 1.8f) || GET_OBJECT_PED_IS_HOLDING(myPed) == object)
				return OBJECT_PROCESS_SKIP; ////if attached to player and mode is vehicle only. / parachute or holding object..
			
			if (HAS_CONTROL_OF_NETWORK_ID(object_NetID)) //objects attached to self with MD or another menu..
			{
				float object_health;
				GET_OBJECT_HEALTH(object, &object_health);
				if (object_health > 162.0f && object_health < 168.0f)
					return OBJECT_PROCESS_SKIP; //object attached to self and health is set telling protection to skip.., might have to remove this whitelist in the future, at the moment no need though.
			}
			
			if (IsKnownLagLightModel(obj_model)) //needs get this on rgh somehow..
			{
				/*if (ARE_OBJECT_COORDS_NEAR_LAST_LAG_OBJECT_COORDS(ox, oy, oz)) //this is attached to us wtf? looks like I made a fatal logic mistake with this lol
				{
					return OBJECT_PROCESS_PREVIOUS_LAGLIGHT;
				}
				LastLagObjectCoordx = ox; LastLagObjectCoordy = oy; LastLagObjectCoordz = oz;*/
				return OBJECT_PROCESS_ATTACHED_LAGLIGHT;
			}
			
#ifdef DEBUG
			char* buffer = BUFFER128;
			sprintf(buffer, "~r~IIV DEBUG: ~s~Object model ~y~%i ~s~was last attached to you.", obj_model);
			ADD_TO_PREVIOUS_BRIEF(buffer);
#endif
			return OBJECT_PROCESS_ATTACHED;
		}
		else if (ctlMem->m_retail_prot.m_obj_prot_mode != OBJ_PROT_MODE_PLAYER_ONLY && DOES_VEHICLE_EXIST(myVehicle) && GET_CAR_OBJECT_IS_ATTACHED_TO(object) == myVehicle && myPed == myVehicleDriver)
		{
			//if (obj_prot_mode == OBJ_PROT_MODE_PLAYER_ONLY) return OBJECT_PROCESS_SKIP; //if attached to veh and obj prot mode = player only
			if (HAS_CONTROL_OF_NETWORK_ID(object_NetID))
			{
				float object_health;
				GET_OBJECT_HEALTH(object, &object_health);
				if (object_health > 162.0f && object_health < 168.0f)
					return OBJECT_PROCESS_SKIP; //attached to self with another menu //works for now
			}
			return OBJECT_PROCESS_ATTACHED_TO_MY_VEH;

		}
	}
	else
	{
		//if distance from myPlayerPed xyz < 3 do code below
		if (ctlMem->m_retail_prot.m_obj_prot_mode == OBJ_PROT_MODE_VEHICLE_ONLY)
			return OBJECT_PROCESS_SKIP; //if not attached, and mode is vehicle only return..
		//check suppress laglight coords and check for deletion in the scan, can be ezily done.. 
		//need check for suppress,, will get to it eventually.
		if (distance < 10.0f)
		{
			/*
			if (IsKnownCollisionLagModel(obj_model))
				OBJECT_PROCESS_COLLISION_LAG_OBJ; //need a count ugh. /nah fuck it 

			if (IsKnownLagLightModel(obj_model))
				return OBJECT_PROCESS_NEARBY_LAGLIGHT; //can't currently do shit about this unless on rgh so meh.
				*/
#ifdef DEBUG
			char* buffer = BUFFER128;
			strcpy_s(buffer, "~r~IIV DEBUG: ~s~Object model ~y~");
			straddi_s(buffer, obj_model);
			stradd_s(buffer, " ~s~was last placed near you.");
			ADD_TO_PREVIOUS_BRIEF(buffer);
#endif
		}
		/*
		if (IS_THIS_MODEL_A_LAG_OBJECT(obj_model))
		{
		//maybe we can set outselves as owner then set visibile that way..
		//MAYBE we can mark them with set object health locally? generate a random float on script startup to use, and put in freemem
			if (ARE_OBJECT_COORDS_NEAR_LAST_LAG_OBJECT_COORDS(ox, oy, oz))
			{
				return OBJECT_PROCESS_PREVIOUS_LAGLIGHT;
			}
			if (distance < 10.0f)
			{
				LastLagObjectCoordx = ox; LastLagObjectCoordy = oy; LastLagObjectCoordz = oz;
				return OBJECT_PROCESS_LAGLIGHT;
			}
		}
#ifdef Nathan
		if (IsKnownCollisionLagModel(obj_model))
		{
			if (distance < 10.0f)
			{
				return OBJECT_PROCESS_COLLISION_LAG_OBJ;
			}
		}
#endif
*/


	}

	return OBJECT_PROCESS_SKIP;
}

#pragma endregion