#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"
#include "Functions.h"
//#include "Network_Options.h"//for stickyBitSet enum
#include "../Menu/Network_Functions.h" //for stickyBitSet enum
#include "../Menu/Functions.h"
#include "protections.h"
#include "protection_functions.h"
#include "../Menu/stdafx.h" //developer defines etc (MAKE SURE TO RELOAD THIS SOLUTION/VS ONCE MADE CHANGES)
#include "display.h" //used to update player counts strings and stuff efficiently.

//TODO Nath: add retail constant boom protection, use IS_EXPLOSION_IN_AREA as IN_SPHERE isn't working
//also try CLEAR_AREA instead of having to telport away. //doesn't work

#define stickyBitSet 0x82A9EC52 //used to make settings persist between sessions.
char previous_model_name[16][9] = { 0 }; //longest name == SuperLOD

int reset_attack_broadcast_tick;
int player_valid_tick[16]; //can easily move to ctlMem should we need too.. plenty of space for now.

void cartel_protections() //only called in network session
{
	//PRINTSTRING("\nProt loop running");
	player_has_freze_gun_notifier();

	if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_LISTEN_FOR_NETUSER_ATTACKS))
		network_attack_notifier_loop(); //wont always pick up attacks on admins if they have this turned off... probs wont tho will they.


	//Broadcast Variable stuff, reset if time expired in order to not leave bad data present.
	if (ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] != 255 && ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData != -1337)
	{
		if (reset_attack_broadcast_tick < GetTickCount() && reset_attack_broadcast_tick != 0)
		{
			reset_attack_broadcast_tick = 0;
			ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = 255;
			ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = 255;
			ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = -1337;
#ifdef Developer
			PRINTSTRING("Reset attack bytes to 255 as 1 second has passed.\n");
#endif
		}
	}
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i) || i == myPlayerIndex)
		{
			if (player_valid_tick[i] != 0)
			{
				player_valid_tick[i] = 0;
				Update_playercount_related_strings();
			}
			continue;
		}
		if(player_valid_tick[i] == 0)
		{
			player_valid_tick[i] = (GetTickCount() + 10000);
			Update_playercount_related_strings();
		}
	}
	if (CartelXEXPresent)
	{
		if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_SET_GHOSTS_ACTIVE))
		{
			for (int i = 0; i < 16; i++)
			{
				if (IS_NETWORK_PLAYER_ACTIVE(i) || i == myPlayerIndex)
					continue; //skip ourselves and non-ghosts.

				if (player_valid_tick[i] != 0 && player_valid_tick[i] < GetTickCount()) //time expired //will skip non-valids as IsPlayerValid check above.
				{
					if (PLAYER_HAS_CHAR(i))
					{
						GetPlayerPointer(i)->m_state = 2; //STATUS_ACTIVE //should probably check these on a regular basis and if they change mid-game yaknow modder. (would need to observe normal behaviour)
						printf("~r~CTL: ~w~Removed %s's ~w~ghost status.", NetworkPlayerGetNameWithColour(i));
						char* buff = BUFFER64;
						sprintf(buff, "Removed ghost status from %s [Preference]", GET_PLAYER_NAME(i));
						CTL_WRITE_TO_FILE("ctl_protection_log.txt", buff);
					}
					//add more checks for the fake leave status thing..

				}
			}
		}
		PLAYER_INDEX attacker;
		CTL_ATTACK_TYPES attack_type;
		uint extraData;
		if (CTL_GRAB_LATEST_ATTACKER(&attacker, &attack_type, &extraData))
		{
			if (attack_type == ATTACK_PEER_DATA) //protecting us but not reliable.
			{
				SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME();
				return;
			}
			else if (attack_type == ATTACK_PLAIN_KICK_RESPONSE) //extraData = PLAYER_CHECK_RESULT 
			{
				NETWORK_KICK_PLAYER(attacker);
				return;
			}
			else if (attack_type == ATTACK_CLONE_CAR || attack_type == ATTACK_CLONE_BIKE || attack_type == ATTACK_CLONE_BOAT || attack_type == ATTACK_CLONE_HELI)
			{
				if (Is_Vehicles_Type_Modified(extraData) || !IS_MODEL_IN_CDIMAGE(extraData)) //wouldn't crash us if model doesn't exist anyway so meh, fixes PINGs custom vehicles.
					return; //if they runnin freeze cars then it will alert wrong and cause a shitstorm..
			}

			char* buffer = BUFFER128; //could do the string then \0\0\0\0\0\0 etc for 128
			char* textFileBuffer = BUFFER128;
			sprintf(buffer, "~r~CTL: Someone attacked but we didn't generate a message, attack_type == %i.", attack_type);
			sprintf(textFileBuffer, "[DEBUG] Someone attacked but we didn't generate a message, attack_type == %i.", attack_type);

			bool weShouldKick = bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_AUTO_KICK);

			if (attacker != INVALID_PLAYER_INDEX)
			{
				if (GET_HOST_ID() != myPlayerIndex || (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, attacker) || IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (attacker + 16))))
					weShouldKick = false; //don't kick if not host or they are an admin/moderator

				//PUT FALSE FLAGS IN HERE TO STOP THEM PRINTING FOR 10 SECONDS
				//used to be 3 seconds if needed for ivm fuckery (no idea)
				//pretty sure checking model change withint his timeframe will only detect IVM users as they switch to niko weirdly on joining/we pick it up and don't with normals seemingly.

				if (player_valid_tick[attacker] > GetTickCount()) //dont run for 4500 milliseconds from when they valid
				{
					if ((attack_type == ATTACK_SYNC_MODEL_CHANGE && extraData == PED_PLAYER)) //TODO NATH figute out what this means when happens! if modder? think it means modder. few instances confirmed.
						return; //pretty sure this signifies modder if not XB1 but not sure why yet.

					if (attack_type == ATTACK_CLONE_PLAYER)
					{
						if (!IsPlayerUsingXboxOne(attacker))
						{
							char* buffer = BUFFER64;
							SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, attacker);
							//char* lah = "~r~CTL: ~w~IVM user detected ~COL_NET_14~PlayersNameHere."; //58 chars
#ifdef Developer
							sprintf(buffer, "Set %s as IVM user (invalid clone within 10 seconds of valid).\n", GET_PLAYER_NAME(attacker)); //53 chars?
							PRINTSTRING(buffer);
							sprintf(buffer, "Set %s detected modding (IVM user)\n", GET_PLAYER_NAME(attacker)); //62 chars?
							PRINTSTRING(buffer);
							//sent this within the first 4 seconds.

							if (!IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, (attacker + 16)))
							{
								strcpy(buffer, "~r~CTL: ~w~", 64); //help stop them searching and figuring out how we doing it lol
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_I][1], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_V][1], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_M][1], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_SPACE][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_U][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_S][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_E][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_R][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_SPACE][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_D][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_E][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_T][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_E][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_C][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_T][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_E][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_D][0], 64);
								stradd(buffer, ctlMem->m_keyboard.key_mappings[KEY_SPACE][0], 64);
								stradd(buffer, NetworkPlayerGetNameWithColour(attacker), 64);
								stradd(buffer, ".", 128);
								printf(buffer);
							}
							SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, (attacker + 16));
#endif
						}
						return;
					}
				}
			}


			bool attacker_unknown;
			if (attacker == -1 || (attack_type == ATTACK_EVENT_EXPLOSION && GET_HOST_ID() != myPlayerIndex)) //if explosion lag and we're not host.
			{
				attacker_unknown = true;
				weShouldKick = false;
			}

			if (!weShouldKick && attacker != myPlayerIndex && !IsPlayerOnFriendsList(attacker, true) && !attacker_unknown)
			{
#ifdef Moderator

				if ((attack_type != ATTACK_SYNC_ATTACHMENT && attack_type != ATTACK_CLONE_SPAM))
				{
#ifdef Developer
					if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, myPlayerIndex)) //check we're an admin locally just for extra security.
#else
					if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (myPlayerIndex + 16))) //check we're a moderator locally just for extra security.
#endif
					{
						if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_ADMIN_ONLY_OFFHOST_ATTACKERS))
						{
							if (GET_HOST_ID() != attacker && (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (GET_HOST_ID() + 16)) || IS_BIT_SET(ctlMem->m_bit_sets.m_admin, (GET_HOST_ID() + 16))))
							{ //host is running cartel, we can send an admin command to them to off-host the attacker.
#ifdef Developer
								SendAdminCommandToPlayer(attacker + ADMIN_CMD_KICK_CONSOLE_BASE_ID_0, GET_HOST_ID());
#else
								SendAdminCommandToPlayer(attacker + ADMIN_CMD_KICK_PLAYER_BASE_ID_0, GET_HOST_ID());
#endif
							}
							else
							{ //attacker is dumb enough to attack us while running cartel, send em to singleplayer.
								if (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (attacker + 16)) || IS_BIT_SET(ctlMem->m_bit_sets.m_admin, (attacker + 16)))
								{
									SendAdminCommandToPlayer(ADMIN_CMD_SINGLEPLAYER, attacker);
								}
							}
#ifdef Developer
							if (!bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG))
							{ //we're not sending a command right now / didn't do that above.
								if (GetNoCartelUsers(true) > (GET_NUMBER_OF_PLAYERS() / 2))
								{ //leverage our user count to get them kicked via votes.
									SendAdminCommandToPlayer(attacker + ADMIN_CMD_VOTE_KICK_TRUE_PLAYERID_BASE_0, ADMIN_PLAYERID_ALL_PLAYERS);
									VoteKick(attacker);
								}
							}

							if (attacker == GET_HOST_ID())
							{
								ctl_attack_crash_hosts_freemode(0, 0); //hopefully this doesn't fuck a new host if we admin them to singleplayer will have to see.
							}
							else
							{
								if (myPlayerIndex != GET_HOST_ID()) //only run if we're not host.
									CTL_OFFHOST_KICK(attacker);
							}
#endif
						}
					}
				}
#endif
				if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_VOTE_KICK_ATTACKERS) && !IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, attacker) && !IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (attacker + 16)))
				{
					if (attack_type != ATTACK_SYNC_ATTACHMENT && attack_type != ATTACK_CLONE_SPAM) //don't auto-fuck for object attachments, we don't kick for it ourselves..
						VoteKick(attacker); //only vote-kick if they are not a moderator/admin.
				}
			}
			if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS))
			{
				if (attacker_unknown) //can't obtain explosion creators off-host.
					ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = myPlayerIndex; //we just report we did it as not certain.
				else
					ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = attacker;

				ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = attack_type + 1;
				ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = extraData;
				SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
				reset_attack_broadcast_tick = (GetTickCount() + 1000);
			}
			//RYAN: All attaks are crashes except attachment
			//it will only notify if its going to crash you
			//meaning they tell you to resurrect a playerindex that isn't valid //aka resurrect
			switch (attack_type)
			{
			case ATTACK_SINGLEPLAYER_KICK:
			{
				if (GET_HOST_ID() == myPlayerIndex)
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~kick ~w~you!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried kicking you to singleplayer.", GET_PLAYER_NAME(attacker));
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~kick ~w~the host!~n~(Singleplayer)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried kicking the host to singleplayer.", GET_PLAYER_NAME(attacker)); //dev build - [Invalid Suggest] ?
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Host singleplayer attempt", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_OUT_OF_BOUNDS_ARRAY:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Out of bounds array)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [Out of bounds array]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Out of bounds freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_GROUP_HACK:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Group hack)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [Group hack]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, KICK_REASON_GROUP_HACK, 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_SPAM://extra data = //object type
			{
				sprintf(buffer, "~r~CTL: %s ~w~could be trying to ~y~lag ~w~you!~n~(%s)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[extraData]);
				sprintf(textFileBuffer, "%s could be trying to lag you with %ss - [Clone Spam]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[extraData]);
			}
			break;
			case ATTACK_CLONE_PLAYER: //extra data = player index they sent
			{
				//NOTE: Removed the kicking for now as it seems to migrate to every other player or something and kick half the lobby, griefers are taking advantage of this against our users.
				//sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(PlayerID %i)", NetworkPlayerGetNameWithColour(attacker), extraData);
				sprintf(buffer, "~r~CTL: %s ~w~maybe tried to ~r~freeze ~w~you!~n~(PlayerID %i)", NetworkPlayerGetNameWithColour(attacker), extraData);
				sprintf(textFileBuffer, "%s maybe tried to freeze you - [Clone of playerIndex %i (%s)]", GET_PLAYER_NAME(attacker), extraData, (extraData > -1 && extraData < 16 && IsPlayerValid(extraData)) ? GET_PLAYER_NAME(extraData) : "Invalid");
				//sprintf(textFileBuffer, "%s tried to freeze you - [Clone of playerIndex %i (%s)]", GET_PLAYER_NAME(attacker), extraData, (extraData > -1 && extraData < 16 && IsPlayerValid(extraData)) ? GET_PLAYER_NAME(extraData) : "Invalid");
				/*if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, KICK_REASON_CLONE_PLAYER, 26);

					NETWORK_KICK_PLAYER(attacker);
				}
				*/
			}
			break;
			case ATTACK_CLONE_PED: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Ped %s)", NetworkPlayerGetNameWithColour(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [Ped %s]", GET_PLAYER_NAME(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						//CLEAR_CUTSCENE();
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Ped 0x%x)", NetworkPlayerGetNameWithColour(attacker), extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [Ped 0x%x]", GET_PLAYER_NAME(attacker), extraData);
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Ped %s)", NetworkPlayerGetNameWithColour(attacker), badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [Ped %s]", GET_PLAYER_NAME(attacker), badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid ped freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_DUMMYPED: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Dummy ped %s)", NetworkPlayerGetNameWithColour(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [Dummy ped %s]", GET_PLAYER_NAME(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Dummy ped 0x%x)", NetworkPlayerGetNameWithColour(attacker), extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [Dummy ped 0x%x]", GET_PLAYER_NAME(attacker), extraData);
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Dummy ped %s)", NetworkPlayerGetNameWithColour(attacker), badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [Dummy ped %s]", GET_PLAYER_NAME(attacker), badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid dummy ped freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_CAR: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Car %s)", NetworkPlayerGetNameWithColour(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [Car %s]", GET_PLAYER_NAME(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Car 0x%x)", NetworkPlayerGetNameWithColour(attacker), extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [Car 0x%x]", GET_PLAYER_NAME(attacker), extraData);
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Car %s)", NetworkPlayerGetNameWithColour(attacker), badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [Car %s]", GET_PLAYER_NAME(attacker), badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid car freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_OBJECT: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData)) //shouldn't get these as warnings anymore tbf but fuck it
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s %s)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [%s %s]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s 0x%x)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [%s 0x%x]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], extraData);
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s %s)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [%s %s]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT], badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid object freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_BIKE: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s %s)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [%s %s]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s 0x%x)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [%s 0x%x]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], extraData);
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s %s)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [%s %s]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BIKE], badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid bike freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_TRAIN: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Train %s)", NetworkPlayerGetNameWithColour(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [Train %s]", GET_PLAYER_NAME(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Train 0x%x)", NetworkPlayerGetNameWithColour(attacker), extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [Train 0x&x]", GET_PLAYER_NAME(attacker), extraData);

					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Train %s)", NetworkPlayerGetNameWithColour(attacker), badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [Train %s]", GET_PLAYER_NAME(attacker), badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid train freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_HELI: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Heli %s)", NetworkPlayerGetNameWithColour(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [Heli %s]", GET_PLAYER_NAME(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Heli 0x%x)", NetworkPlayerGetNameWithColour(attacker), extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [Heli 0x%x]", GET_PLAYER_NAME(attacker), extraData);
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Heli %s)", NetworkPlayerGetNameWithColour(attacker), badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [Heli %s]", GET_PLAYER_NAME(attacker), badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid heli freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_BOAT: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s %s)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [%s %s]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s 0x%x)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [%s 0x%x]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], extraData);
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(%s %s)", NetworkPlayerGetNameWithColour(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [%s %s]", GET_PLAYER_NAME(attacker), ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_BOAT], badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid boat freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_CLONE_PLANE: //extra data = model hash
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Plane %s)", NetworkPlayerGetNameWithColour(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [Plane %s]", GET_PLAYER_NAME(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Plane 0x%x)", NetworkPlayerGetNameWithColour(attacker), extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [Plane 0x%x]", GET_PLAYER_NAME(attacker), extraData);
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Plane %s)", NetworkPlayerGetNameWithColour(attacker), badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [Plane %s]", GET_PLAYER_NAME(attacker), badModelName);
					}
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid plane freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_EVENT_REQUEST_PICKUP:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Request Pickup Event)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [REQUEST_PICKUP_EVENT]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Request Pickup freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_EVENT_RESURRECT:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Resurrect Event)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [RESURRECT_PLAYER_EVENT]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Resurrect event freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_EVENT_REMOVE_WEAPON:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Weapon event)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [*_WEAPON_EVENT]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Weapon event freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_EVENT_EXPLOSION:
			{
				if (attacker_unknown)
				{
					strcpy(buffer, "~r~CTL: ~w~Someone tried to ~y~lag ~w~you!~n~(Explosions)", 128);
					strcpy(textFileBuffer, "Someone tried lag you - [Explosion Event]", 128);
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~y~lag ~w~you!~n~(Explosions)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried lag you - [Explosion Event]", GET_PLAYER_NAME(attacker));
					/*if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Explosion lag", 26);

						NETWORK_KICK_PLAYER(attacker);
					}*/
				}
			}
			break;
			case ATTACK_EVENT_WANTED_LEVEL:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(WantedLeveL event)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [ALTER_WANTED_LEVEL_EVENT]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "WantedLevel event freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_EVENT_CREATE_PICKUP:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Pickup event)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [CREATE_PICKUP_EVENT]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "CreatePickup event freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_EVENT_RADIO_STATION:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Radio Event)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [CHANGE_RADIO_STATION_EVENT]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Radio event freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_TELL_NET:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Blackscreen)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [Blackscreen]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Blackscreen freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			/*
			case ATTACK_PEER_DATA: //always going to be host i'm told, fuck it xex now gives us GET_HOST_ID() if we're not host.
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Peer data)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [Peer data]", GET_PLAYER_NAME(attacker)); //currently xex will always give us host as it can only be them.
				SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME();
			}
			break;
			*/
			case ATTACK_REASSIGN:
			{
				if (attacker_unknown)
				{
					strcpy(buffer, "~r~CTL: ~w~Someone tried to ~r~freeze ~w~you!~n~(Re-assign)", 128);
					strcpy(textFileBuffer, "Someone tried to freeze you - [Re-assign]", 128);
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Re-assign)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried to freeze you - [Re-assign]", GET_PLAYER_NAME(attacker));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid re-assign freeze", 26);

						NETWORK_KICK_PLAYER(attacker);
					}
				}
			}
			break;
			case ATTACK_PLAYER_NAME:
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Gamertag)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [Incomplete text formatting codes in gamertag]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
					{
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Joined w/freeze gamertag", 26);
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].gamertag, "Freeze Gamertag", 16);
					}

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_SYNC_ATTACHMENT: //extra data = //network id of attachment
			{
				bool attachment_deleted = false;
				char* entityType = "Unknown\0\0\0\0\0\0";
				if (DOES_OBJECT_EXIST_WITH_NETWORK_ID(extraData))
				{
					entityType = ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_OBJECT]; //"Object"
					if (HAS_CONTROL_OF_NETWORK_ID(extraData)) //some people don't lock control of their objects when attaching them so this will work sometimes.
					{
						OBJECT_INDEX junkObject;
						GET_OBJECT_FROM_NETWORK_ID(extraData, &junkObject);
						DELETE_OBJECT(&junkObject);
						attachment_deleted = true;
					}
				}
				else if (DOES_PED_EXIST_WITH_NETWORK_ID(extraData))
				{
					entityType = ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_PED]; //"Ped";
					PED_INDEX netPed;
					GET_PED_FROM_NETWORK_ID(extraData, &netPed);
					if (IsThisPedAPlayer(netPed) != INVALID_PLAYER_INDEX)
						entityType = ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_PLAYER]; //"Player"
					else
					{
						if (HAS_CONTROL_OF_NETWORK_ID(extraData))
						{
							DELETE_CHAR(&netPed);
							attachment_deleted = true;
						}
					}
				}
				else if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(extraData))
				{
					VEHICLE_INDEX netVeh;
					GET_VEHICLE_FROM_NETWORK_ID(extraData, &netVeh);
					entityType = VehiclesModelName(netVeh);
					if (HAS_CONTROL_OF_NETWORK_ID(extraData))
					{
						DELETE_CAR(&netVeh);
						attachment_deleted = true;
					}
				}
#ifdef Moderator
				if (attachment_deleted)
					PRINTSTRING("Successfully deleted an entity attached to us, not flipping model, WOOO! Please tell Nath about this. \n");
#endif
				if (attacker_unknown)
				{
					sprintf(buffer, "~r~CTL: ~w~Someone attached an ~y~entity ~w~to you.~n~(%s)", entityType);
					sprintf(textFileBuffer, "Someone attached a %s to you - [Attachment Sync]", entityType);
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~attached an ~y~entity ~w~to you.~n~(%s)", NetworkPlayerGetNameWithColour(attacker), entityType);
					sprintf(textFileBuffer, "%s attached a %s to you - [Attachment Sync]", GET_PLAYER_NAME(attacker), entityType);
				}
				if (!attachment_deleted && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_FLIP_MODEL_OBJECT_ATTACHED))
					change_player_model_temporarily(PED_M_Y_GMAF_HI_02, 1200);
			}
			break;
			case ATTACK_SYNC_ANIM_TASK: //extra data = //network id 
			{
				if (DOES_PED_EXIST_WITH_NETWORK_ID(extraData))
				{
					PED_INDEX attackPed;
					GET_PED_FROM_NETWORK_ID(extraData, &attackPed);
					if (IsThisPedAPlayer(attackPed) == -1)
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Animation ped)", NetworkPlayerGetNameWithColour(attacker));
						sprintf(textFileBuffer, "%s tried to freeze you - [Invalid animation via a ped]", GET_PLAYER_NAME(attacker));
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Animation)", NetworkPlayerGetNameWithColour(attacker));
						sprintf(textFileBuffer, "%s tried to freeze you - [Invalid animation]", GET_PLAYER_NAME(attacker));
					}
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Animation)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried to freeze you - [Invalid animation]", GET_PLAYER_NAME(attacker));
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Animation freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_SYNC_RELATIONSHIP_GROUP: //extra data = //network id 
			{
				sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Relationship group)", NetworkPlayerGetNameWithColour(attacker));
				sprintf(textFileBuffer, "%s tried to freeze you - [Relationship group]", GET_PLAYER_NAME(attacker));
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Relationship group freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_SYNC_TASK_QUERY:   //fills the pool afaik
			{
				if (attacker_unknown)
				{
					strcpy(buffer, "~r~CTL: ~w~Someone tried to ~r~freeze ~w~you!~n~(Task)", 128);
					strcpy(textFileBuffer, "Someone tried to freeze you - [Task]", 128);
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Task)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried to freeze you - [Task]", GET_PLAYER_NAME(attacker));
					if (weShouldKick)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(attacker, &kicked_array_index))
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Task freeze", 26);

						NETWORK_KICK_PLAYER(attacker);
					}
				}
			}
			break;
			case ATTACK_SYNC_FREEZE_GUN:  //means someone or someones ped threw a freeze gun
			{
				if (DOES_PED_EXIST_WITH_NETWORK_ID(extraData))
				{
					PED_INDEX attackPed;
					GET_PED_FROM_NETWORK_ID(extraData, &attackPed);
					if (IsThisPedAPlayer(attackPed) == -1)
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Freeze gun ped)", NetworkPlayerGetNameWithColour(attacker));
						sprintf(textFileBuffer, "%s tried to freeze you - [Freeze gun thrown via a ped]", GET_PLAYER_NAME(attacker));
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Freeze gun)", NetworkPlayerGetNameWithColour(attacker));
						sprintf(textFileBuffer, "%s tried to freeze you - [Freeze gun thrown]", GET_PLAYER_NAME(attacker));
					}
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Freeze gun)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried to freeze you - [Freeze gun thrown]", GET_PLAYER_NAME(attacker));
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Freeze gun", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_SYNC_WEAPON_INDEX: //extra data = //network id
			{
				if (DOES_PED_EXIST_WITH_NETWORK_ID(extraData))
				{
					PED_INDEX attackPed;
					GET_PED_FROM_NETWORK_ID(extraData, &attackPed);
					if (IsThisPedAPlayer(attackPed) == -1)
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Peds weapon)", NetworkPlayerGetNameWithColour(attacker));
						sprintf(textFileBuffer, "%s tried to freeze you - [Invalid weapon via a ped]", GET_PLAYER_NAME(attacker));
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Weapon)", NetworkPlayerGetNameWithColour(attacker));
						sprintf(textFileBuffer, "%s tried to freeze you - [Invalid weapon]", GET_PLAYER_NAME(attacker));
					}
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Weapon)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried to freeze you - [Invalid weapon]", GET_PLAYER_NAME(attacker));
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Invalid Weapon freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_SYNC_ANIMATION_GROUP: //network id
			{
				if (DOES_PED_EXIST_WITH_NETWORK_ID(extraData))
				{
					PED_INDEX attackPed;
					GET_PED_FROM_NETWORK_ID(extraData, &attackPed);
					if (IsThisPedAPlayer(attackPed) == -1)
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Peds anim group)", NetworkPlayerGetNameWithColour(attacker));
						sprintf(textFileBuffer, "%s tried to freeze you - [Invalid animation group via a ped]", GET_PLAYER_NAME(attacker));
					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Animation group)", NetworkPlayerGetNameWithColour(attacker));
						sprintf(textFileBuffer, "%s tried to freeze you - [Invalid animation group]", GET_PLAYER_NAME(attacker));
					}
				}
				else
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Animation group)", NetworkPlayerGetNameWithColour(attacker));
					sprintf(textFileBuffer, "%s tried to freeze you - [Invalid animation group]", GET_PLAYER_NAME(attacker));
				}
				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Animation group freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_SYNC_MODEL_CHANGE: //extraData = model
			{
				if (IS_THIS_MODEL_A_VEHICLE(extraData))
				{
					sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Model %s)", NetworkPlayerGetNameWithColour(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
					sprintf(textFileBuffer, "%s tried to freeze you - [Invalid model change %s]", GET_PLAYER_NAME(attacker), GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(extraData)));
				}
				else
				{
					bool cutScene; char* badModelName = bad_player_model_name(extraData, &cutScene);
					if (COMPARE_STRING(badModelName, "Unkn"))
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Model 0x%x)", NetworkPlayerGetNameWithColour(attacker), extraData);
						sprintf(textFileBuffer, "%s tried to freeze you - [Invalid model change 0x%x]", GET_PLAYER_NAME(attacker), extraData);

					}
					else
					{
						sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Model %s)", NetworkPlayerGetNameWithColour(attacker), badModelName);
						sprintf(textFileBuffer, "%s tried to freeze you - [Invalid model change %s]", GET_PLAYER_NAME(attacker), badModelName);
					}
				}

				if (weShouldKick)
				{
					uint kicked_array_index;
					if (AddPlayerToKickStorage(attacker, &kicked_array_index))
						strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Model change freeze", 26);

					NETWORK_KICK_PLAYER(attacker);
				}
			}
			break;
			case ATTACK_SYNC_INVALID_WEAPON_THROW:
			{
				sprintf(buffer, "~r~CTL: Someone tried throwing an invalid weapon!", attack_type);
				sprintf(textFileBuffer, "[DEBUG] Someone tried throwing an invalid weapon!", attack_type);
			}
			break;
			}
			if (!IS_THIS_PRINT_BEING_DISPLAYED("STRING", SCRIPT_PRINT_ONE_LITERAL_STRING, nullptr, buffer, nullptr, 0, 0, 0, 0, 0, 0))
			{
				ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(true);
				PRINT_STRING_WITH_LITERAL_STRING_NOW("STRING", buffer, IS_SCREEN_FADED_IN() ? 5000 : 15000, 1);
				PLAY_AUDIO_EVENT("FRONTEND_OTHER_RACE_GO");
				CTL_WRITE_TO_FILE("ctl_protection_log.txt", textFileBuffer);
			}
		}
	} 
	else 
	{ //Retail only stuff
		setOffhostKickMSGid(); //detoured in xex and we just return/do nothing.
		retail_freeze_gun_thrown_notifier();

		freemode_invalid_suggest_fixer(); //blocked in xex

#pragma region group_hack
		if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_GROUP_HACK))
		{
			GROUP_INDEX my_group;
			GET_PLAYER_GROUP(myPlayerIndex, &my_group);
			bool hacked = false;
			if (DOES_GROUP_EXIST(my_group))
			{
				if (!IS_GROUP_LEADER(myPlayerPed, my_group)) //pretty sure passing 0 to this would always return local player.
				{ //get group leader doesn't work bc they already took themselves out of it in 99% of cases by the time it syncs to you.
					SET_GROUP_LEADER(my_group, myPlayerPed);
					hacked = true;
					Alert("~r~CTL: ~w~Someone altered your ~y~group, ~w~likely in an attempt to ~r~freeze ~w~you.");
				//	PRINTSTRING("[Cartel Helper] Group hacked and group existed, setting leader.\n");
				}
			}
			else
			{
				hacked = true;
				CREATE_GROUP(0, &my_group, true);
				SET_GROUP_LEADER(my_group, myPlayerPed);
				Alert("~r~CTL: ~w~Someone removed your ~y~group, ~w~likely in an attempt to ~r~freeze ~w~you.");
			//	PRINTSTRING("[Cartel Helper] Group hacked and group didn't exist, creating group.\n");
			}
			if (hacked)
			{
				if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS))
				{
					ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = myPlayerIndex; // will check if offender == reporter and display "Someone"
					ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_GROUP_HACK + 1;
					ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = 0;
					SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
					reset_attack_broadcast_tick = (GetTickCount() + 1000);
				}
			}
		}
#pragma endregion

#pragma region tidle_crash_gamertag
		if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_TIDLE_CRASH)) //xex should replace all "~" with "-" in CNetworkPlayerMgr_AddTemporaryPlayer
		{
			for (int i = 0; i < 16; i++)
			{
				//if (i == myPlayerIndex) continue;
				if (!IsStrValid((char*)GET_PLAYER_NAME(i))) //small chance there's left-over/stagnant names so we will check all slots even if empty.
				{
					strcpy((char*)GET_PLAYER_NAME(i), "##REPLACED## ", 16); //could put their ip in lol
					straddi((char*)GET_PLAYER_NAME(i), i, 16);

					if (IsPlayerValid(i)) //should realistically come across with the name at same time and be true.
					{
						if (GET_HOST_ID() == myPlayerIndex)
						{
							uint kicked_array_index;
							if (AddPlayerToKickStorage(i, &kicked_array_index))
							{
								printf("~r~CTL: ~w~Console kicked spoofed player due to ~r~freeze ~w~gamertag.");
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Joined w/freeze gamertag", 26);
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].gamertag, "Freeze Gamertag", 16);
							}
							else
								printf("~r~CTL: ~w~Kicked spoofed player due to ~r~freeze ~w~gamertag.");

							NETWORK_KICK_PLAYER(i);

							if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS))
							{
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = i;
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_PLAYER_NAME + 1;
								SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
								reset_attack_broadcast_tick = (GetTickCount() + 1000);
							}
						}
						else
							printf("~r~CTL: ~COL_NET_%i~playerIndex %i's ~r~gamertag ~w~replaced for ~r~freeze ~w~avoidance.", (i + 1), i);
					}
				}
			}
		}
#pragma endregion

		float mX, mY, mZ, mGround, mHeight;
		GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
		GET_GROUND_Z_FOR_3D_COORD(mX, mY, mZ, &mGround);
		mHeight = (mZ - mGround);
#pragma region player_model
		if (ctlMem->m_retail_prot.m_model_prot_mode > MODEL_PROT_OFF)
		{
			uint tmp_model;
			PED_INDEX tmp_ped;
			//MAY NEED TO CHANGE THIS TO SCAN FOR PEDS NEAR YOU AND SET THEM INVISIBLE IF NIKO, i think spawning a niko ped on someone could fuck them?
			for (int i = 0; i < 16; i++)
			{
				if (i == myPlayerIndex || !IsPlayerValid(i) || !PLAYER_HAS_CHAR(i))
				{
					CLEAR_BIT(&cap_playermodel_alert_bitset, i);
					continue;
				}
				GET_PLAYER_CHAR(i, &tmp_ped);
				GET_CHAR_MODEL(tmp_ped, &tmp_model);
				if (tmp_model != PED_PLAYER && tmp_model != PED_SUPERLOD)
				{
					//changing from niko to something else also freezes players!
					if (IS_BIT_SET(cap_playermodel_alert_bitset, i)) //changed from niko just now.. //won't freeze us as they were invisible!
					{
						float tmp_x, tmp_y, tmp_z, distance;
						GET_CHAR_COORDINATES(tmp_ped, &tmp_x, &tmp_y, &tmp_z);
						PED_INDEX id_ped, lowest_distance_id = 1337;
						float lowest_distance = 35.0f;
						float idx, idy, idz;
						for (int id = 0; id < 16; id++)
						{
							if (id == myPlayerIndex || id == i || !IsPlayerValid(id) || !PLAYER_HAS_CHAR(id))
								continue;
							GET_PLAYER_CHAR(id, &id_ped);
							//if (id_ped == tmp_ped) continue;
							GET_CHAR_COORDINATES(id_ped, &idx, &idy, &idz);
							GET_DISTANCE_BETWEEN_COORDS_3D(idx, idy, idz, tmp_x, tmp_y, tmp_z, &distance);
							if (distance < lowest_distance)
							{
								lowest_distance = distance;
								lowest_distance_id = id;
							}
						}
						if (lowest_distance_id != 1337)
						{
							char* buffer = BUFFER128;
							strcpy(buffer, "~r~CTL: ", 128);
							stradd(buffer, NetworkPlayerGetNameWithColour(i), 128);
							if (GET_HOST_ID() == myPlayerIndex && ctlMem->m_retail_prot.m_model_prot_mode == MODEL_PROT_LOBBY)
							{
								uint kicked_array_index;
								if (AddPlayerToKickStorage(i, &kicked_array_index))
								{
									strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Changed to freeze ped", 26);
								}
								NETWORK_KICK_PLAYER(i);
							}
							stradd(buffer, " ~s~changed from ~y~", 128);
							stradd(buffer, previous_model_name[i], 128);
							//stradd(buffer, bad_player_model_name(tmp_model), 128);
							stradd(buffer, " ~s~near ", 128);
							stradd(buffer, NetworkPlayerGetNameWithColour(lowest_distance_id), 128);
							stradd(buffer, ".", 128); //works...
							//sprintf(buffer, "~r~IIV: %s ~s~changed from ~y~%s ~s~near %s.", NetworkPlayerGetNameWithColour(i), SP_MODEL_NAME(), NetworkPlayerGetNameWithColour(lowest_distance_id)); //prints lowest distance id as first name too, makes no sense.. doesn't work. (possible buffer size but meh)
							Alert(buffer);
						}
					}
					CLEAR_BIT(&cap_playermodel_alert_bitset, i);
					continue;
				}
				SET_CHAR_VISIBLE(tmp_ped, false); //keep this above the suppress messgae skip.
				if (IS_BIT_SET(cap_playermodel_alert_bitset, i))
					continue; //we've already printed the message

				float tmp_x, tmp_y, tmp_z, distance;
				GET_CHAR_COORDINATES(tmp_ped, &tmp_x, &tmp_y, &tmp_z);
				GET_DISTANCE_BETWEEN_COORDS_3D(mX, mY, mZ, tmp_x, tmp_y, tmp_z, &distance);
				//v - If the distance is more than 70-> skip
				//v - If the distance is more than more than 5 (automatically true if previous statement is true) and they're not on screen continue

				//bool trying_to_freeze_you = !((distance > 70.0f) || (distance > 5.0f && !IS_CHAR_ON_SCREEN(tmp_ped)));
				bool trying_to_freeze_you = (distance < 10.0f || IS_CHAR_ON_SCREEN(tmp_ped));
				SET_BIT(&cap_playermodel_alert_bitset, i);

				char* buffer = BUFFER128;
				strcpy(buffer, "~r~CTL: ", 128); //8 characters //
				stradd(buffer, NetworkPlayerGetNameWithColour(i), 128); //26 + 8 = 34
				stradd(buffer, trying_to_freeze_you ? " ~w~tried to use ~y~" : " ~w~changed to ~y~", 128); //39 + 34 //73
				bool cutScene;
				char* badmodelname = bad_player_model_name(tmp_model, &cutScene);
				strcpy(previous_model_name[i], badmodelname, 16);
				//previous_model_name[i] = bad_player_model_name(tmp_model); //thought we'd need to use strcpy but this works apparently..
				stradd(buffer, badmodelname, 128); //Niko/Johnny/Luis
				stradd(buffer, trying_to_freeze_you ? " ~s~against you" : "~s~", 128);
				if (trying_to_freeze_you)
				{
					if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS))
					{
						ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = i;
						ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_SYNC_MODEL_CHANGE + 1;
						ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = tmp_model;
						SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
						reset_attack_broadcast_tick = (GetTickCount() + 1000); //maybe
					}
					if (GET_HOST_ID() == myPlayerIndex && ctlMem->m_retail_prot.m_model_prot_mode == MODEL_PROT_LOBBY)
					{
						uint kicked_array_index;
						if (AddPlayerToKickStorage(i, &kicked_array_index))
						{
							strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Player model freeze", 26);
							stradd(buffer, ", console kicked.", 128);
						}
						else
							stradd(buffer, ", player kicked.", 128);

						NETWORK_KICK_PLAYER(i);
					}
					else
						stradd(buffer, ".", 128);
				}
				else
				{
					PED_INDEX id_ped, lowest_distance_id = 1337;
					float lowest_distance = 35.0f; //maybe increase with time but meh..
					float idx, idy, idz;
					for (int id = 0; id < 16; id++)
					{
						if (id == myPlayerIndex || id == i || !IsPlayerValid(id) || !PLAYER_HAS_CHAR(id)) continue;
						GET_PLAYER_CHAR(id, &id_ped);
						GET_CHAR_COORDINATES(id_ped, &idx, &idy, &idz);
						GET_DISTANCE_BETWEEN_COORDS_3D(idx, idy, idz, tmp_x, tmp_y, tmp_z, &distance);
						if (distance < lowest_distance)
						{
							lowest_distance = distance;
							lowest_distance_id = id;
						}
					}
					if (lowest_distance_id != 1337)
					{ //they did this close to a player
						if (GET_HOST_ID() == myPlayerIndex && ctlMem->m_retail_prot.m_model_prot_mode == MODEL_PROT_LOBBY)
						{
							uint kicked_array_index;
							if (AddPlayerToKickStorage(i, &kicked_array_index))
							{
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Changed to freeze model", 26);
							}
							NETWORK_KICK_PLAYER(i);
						}
						stradd(buffer, " near ", 128);
						stradd(buffer, NetworkPlayerGetNameWithColour(lowest_distance_id), 128);
					}
					stradd(buffer, ".", 128);
				}
				Alert(buffer);
			}

			//{ //only run if a player is currently niko or slod
			const int ClosestNiko = GetNearestSlodOrNikoPlayerPed(mX, mY, mZ);
			for (int i = 0; i < 16; i++)
			{
				if (!IS_BIT_SET(cap_playermodel_alert_bitset, i) || !IsPlayerValid(i) || !PLAYER_HAS_CHAR(i))
					continue;

				//should just try giving fake network name (char*)GET_PLAYER_NAME, and see if it works on multiple peds..
				PED_INDEX netPed;
				GET_PLAYER_CHAR(i, &netPed);
				REMOVE_FAKE_NETWORK_NAME_FROM_PED(netPed);
				if (IS_CHAR_ON_SCREEN(netPed)) //works
				{
					float px, py, pz;
					GET_CHAR_COORDINATES(netPed, &px, &py, &pz);
					int pr, pg, pb;
					GET_PLAYER_RGB_COLOUR(i, &pr, &pg, &pb);

					if (IS_CHAR_IN_ANY_CAR(netPed))
					{
						DRAW_CHECKPOINT_WITH_ALPHA(px, py, (pz - 0.5f), 1.02f, pr, pg, pb, 255);
					}
					else
					{
						DRAW_CHECKPOINT_WITH_ALPHA(px, py, (pz - 1.0f), 1.02f, pr, pg, pb, 255);
						GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(netPed, 0.0f, 0.75f, 0.0f, &px, &py, &pz);
						DRAW_CHECKPOINT_WITH_ALPHA(px, py, (pz - 1.0f), 0.35f, pr, pg, pb, 255);
					}
					if (ClosestNiko == netPed)
					{
						GIVE_PED_FAKE_NETWORK_NAME(netPed, SafeNetworkPlayerGetName(i), pr, pg, pb, 255);
					}
					//^^ PROBABLY THE BEST BET FOR NOW
					//COPY_ANIMATIONS(Ped ped, Ped pednext, float speed); //make a ped in its place?
					//GIVE_PED_FAKE_NETWORK_NAME(netPed, SAFE_GET_PLAYER_NAME(i), pr, pg, pb, 255); //only works on one ped at a time, unless we need to give it a pointer?
				}
			}
		}
#pragma endregion

#pragma region object_attach_protection
		//	*(int*)0x8258E938 = 0x60000000; //SET_OBJECT_VISIBLE bypass //WORKS //RGH ONLY
		//*(int*)0x8258AA5C = 0x60000000; //SET_OBJECT_COLLISION bypass //WORKS
		if (ctlMem->m_retail_prot.m_obj_prot_mode > OBJ_PROT_MODE_DISABLED)
		{
			PED_INDEX myVehDriver;
			VEHICLE_INDEX myVeh;
			GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
			if (DOES_VEHICLE_EXIST(myVeh))
				GET_DRIVER_OF_CAR(myVeh, &myVehDriver); //passed to get object process.

			CPool* NetObjPool = *(CPool**)0x8318E950; //Network object pool
			int process;
			for (int i = 0; i < NetObjPool->m_maxHandle; i++)
			{
				int NetPtr = (int)(NetObjPool->m_firstPointer + (i * NetObjPool->m_structSize));
				int EntityPtr = *(int*)(NetPtr + 0x4);
				if (EntityPtr && EntityPtr != 0xCDCDCDCD)
				{
					//int entityType = *(int*)(EntityPtr + 8); //entity type will always be onject in the object pool...
					//short entityModelID = *(short*)(EntityPtr + 0x2E);
					short entityNetworkID = *(short*)(NetPtr + 12);
					byte entityCreatorID = *(byte*)(NetPtr + 15);
					//think we're better doing find controller of network id, if they're not in game, print player that has control of this left or whatever.
					//prob easiest to check if(is_wModelID_a_Vehicle(entityModelID) || is_wModelID_a_Ped(entityModelID))
					OBJECT_INDEX handle = GetHandleFromPool(*(CPool**)0x82DE5A50, EntityPtr); //object pool
					if (!DOES_OBJECT_EXIST(handle)) continue;
					process = GetObjectAction(handle, entityNetworkID, mX, mY, mZ, mHeight, myPlayerPed, myVeh, myVehDriver);
					switch (process)
					{
					case OBJECT_PROCESS_SKIP:
					{
						continue;
					}
					break;
					case OBJECT_PROCESS_ATTACHED:
					case OBJECT_PROCESS_ATTACHED_LAGLIGHT:
					{
						SET_OBJECT_VISIBLE(handle, false); //locally = gone //needs a bypass
						//TODO Nath, See what *(int*)0x8258E938 = 0x60000000; //SET_OBJECT_VISIBLE bypass and *(int*)0x8258AA5C = 0x60000000; //SET_OBJECT_COLLISION bypass 
						//does, I suspect we can do it on retail / it may just be comparing the ownership which we can eaisly overwrite. (it should sync back / not cause problems).

#ifdef DEVKIT
						SET_OBJECT_COLLISION(handle, false); //should work locally with moi bypass i did.. 
#endif
							//SET_OBJECT_LIGHTS(handle, false);
						REQUEST_CONTROL_OF_NETWORK_ID(entityNetworkID);
						if (HAS_CONTROL_OF_NETWORK_ID(entityNetworkID))
						{
							DeleteObject(&handle);
							Alert((process == OBJECT_PROCESS_ATTACHED_LAGLIGHT) ? "~r~CTL : ~s~A ~y~lag-object ~s~attached to you was successfully deleted." : "~r~CTL: ~s~An ~y~object ~s~attached to you was successfully deleted.");
							continue;
						}
						//SET_OBJECT_VISIBLE(handle,false); //works flawlessly for lag lights
						if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_FLIP_MODEL_OBJECT_ATTACHED))
						{
							change_player_model_temporarily(PED_M_Y_GMAF_HI_02, 1000);
						}
						DETACH_OBJECT(handle, true); //if detaching for others too mayaswell make it faster locally..
						if (entityCreatorID != myPlayerIndex) // Controller then = playerindex so do code
						{
							char* buffer = BUFFER128;
							strcpy(buffer, "~r~CTL: ", 128);
							stradd(buffer, NetworkPlayerGetNameWithColour(entityCreatorID), 128); //should do IS_PLAYER_PRESENT(Controller) ? NameWithColour : "Somebody"
							stradd(buffer, (process == OBJECT_PROCESS_ATTACHED_LAGLIGHT) ? " ~s~attached a ~y~lag-object ~s~to you, detached" : " ~s~attached an ~y~object ~s~to you, detached", 128);
							if (GET_HOST_ID() == myPlayerIndex && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_AUTO_KICK))
							{
								NETWORK_KICK_PLAYER(entityCreatorID);
								stradd(buffer, ", player kicked", 128);
							}
							stradd(buffer, ".", 128); 
							Alert(buffer);
							if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS))
							{
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = entityCreatorID;
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_SYNC_ATTACHMENT + 1;
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = entityNetworkID;
								SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
								reset_attack_broadcast_tick = (GetTickCount() + 1000); 
							}
						}
					}
					break;
					case OBJECT_PROCESS_LAGLIGHT:
					{
						SET_OBJECT_VISIBLE(handle, false); //locally = gone //needa a bypass
						REQUEST_CONTROL_OF_NETWORK_ID(entityNetworkID);
						if (HAS_CONTROL_OF_NETWORK_ID(entityNetworkID))
						{
							DeleteObject(&handle);
							Alert("~r~CTL: ~s~A ~y~lag-object ~s~was successfully deleted.");
							continue;
						}

						//bool ADD_LAG_LIGHT_AREA_TO_SUPPRESSED(const int object, const char* offender_name) //maybe have an xyz or last and create a radius of 6 to ignore? hmmh? idk... what if you return and it still there..
						if (entityCreatorID != myPlayerIndex)
						{
							char* buffer = BUFFER128;
							sprintf(buffer, "~r~CTL: %s ~s~spawned a ~y~lag-object ~s~near your player.", NetworkPlayerGetNameWithColour(entityCreatorID));
							Alert(buffer);
						}
					}
					break;
					case OBJECT_PROCESS_ATTACHED_TO_MY_VEH:
					{
						REQUEST_CONTROL_OF_NETWORK_ID(entityNetworkID);
						if (HAS_CONTROL_OF_NETWORK_ID(entityNetworkID))
						{
							DeleteObject(&handle);
							Alert("~r~CTL: ~s~An ~y~object ~s~attached to your vehicle was successfully deleted.");
							continue;
						}
						if (entityCreatorID != myPlayerIndex) //don't have control...
						{
							char* buffer = BUFFER128;
							sprintf(buffer, "~r~CTL: %s ~s~tried attaching an ~y~object ~s~to your %s.", NetworkPlayerGetNameWithColour(entityCreatorID), VehiclesModelName(myVeh));
							Alert(buffer);
							/*
							if (IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS))
							{
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = i;
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_SYNC_MODEL_CHANGE + 1;
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = entityNetworkID;
								SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
								reset_attack_broadcast_tick = (GetTickCount() + 1000); //maybe
							}
							*/
						}
						if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_FLIP_MODEL_OBJECT_ATTACHED))
							bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_OBJ_PROT_WANTS_VEHICLE_RESPAWN);
						//think this will be okay
						//get cartel menu to handle this so we don't get fucked.
						//spawn_vehicle(-1, 0.0f, 0.0f, 0.0f, 0.0f);
					}
					break;
					case OBJECT_PROCESS_PREVIOUS_LAGLIGHT:
					{
						//would have to try overwriting owner in memory fuckin meh. (Retail)
						SET_OBJECT_VISIBLE(handle, false);
						//SET_OBJECT_LIGHTS(handle, false); //don't think it works but meh
						REQUEST_CONTROL_OF_NETWORK_ID(entityNetworkID);
						if (HAS_CONTROL_OF_NETWORK_ID(entityNetworkID))
							DeleteObject(&handle);
					}
					break;
					case OBJECT_PROCESS_COLLISION_LAG_OBJ:
					{
						SET_OBJECT_COLLISION(handle, false); //only works because control compare is nulled in IV's xex on script startup (RGH/JTag only)
						//can probably also overwrite with pointers.. just set we own this then do collision etc and put back..
						REQUEST_CONTROL_OF_NETWORK_ID(entityNetworkID);
						if (HAS_CONTROL_OF_NETWORK_ID(entityNetworkID))
							DeleteObject(&handle);
					}
					break;
					}
				}
			}
		}
		
#pragma endregion

#pragma region ped_freeze_protection
		int tmp_model, tmp_wep;
		CPool* NetPedPool = *(CPool**)0x8318E6E0; //Network Ped Pool 
		for (int i = 0; i < NetPedPool->m_maxHandle; i++)
		{
			int NetPtr = (int)(NetPedPool->m_firstPointer + (i * NetPedPool->m_structSize));
			int EntityPtr = *(int*)(NetPtr + 0x4);
			if (EntityPtr && EntityPtr != 0xCDCDCDCD)
			{
				short PedModelID = *(short*)(EntityPtr + 0x2E);
				//int entityType = *(int*)(NetPtr + 8); //will ped in ped pool.
				short entityNetworkID = *(short*)(NetPtr + 12);
				byte entityCreatorID = *(byte*)(NetPtr + 15); //also owner i think..
				//if (entityCreatorID == iPlayer) continue; //leave this for now for testing

				PED_INDEX netPed = GetHandleFromPool(*(CPool**)0x831D5388, EntityPtr); //Ped Pool
				if (DOES_CHAR_EXIST(netPed)) //can prolly remove this. //meh //should be fine I'd imagine it'll still return even if wrong model in pool.
				{
					if (IsThisPedAPlayer(netPed) != INVALID_PLAYER_INDEX) //player model protection will get it
						continue;

					if (IS_CHAR_ON_FOOT(netPed))
					{
						if (GET_CURRENT_CHAR_WEAPON(netPed, &tmp_wep))
						{
							if ((tmp_wep == WEAPON_ROCKET || tmp_wep == WEAPON_EPISODIC_18) && IS_CHAR_SHOOTING(netPed))
							{
								char* buffer = BUFFER128;
								sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Freeze gun)", NetworkPlayerGetNameWithColour(i));
								Alert(buffer);
								if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS))
								{
									ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = entityCreatorID;
									ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_SYNC_FREEZE_GUN + 1;
									ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = entityNetworkID;
									SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
									reset_attack_broadcast_tick = (GetTickCount() + 1000); //maybe
								}
							}
						}
					}
					const int dodgyType = is_wModelID_a_Dodgy_Ped(PedModelID);
					if ((dodgyType == DODGY_PED_SUPERLOD || dodgyType == DODGY_PED_PLAYER) && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_SUPERLOD))
					{
						SET_CHAR_VISIBLE(netPed, false);
						vector3 posn;
						float distanceFromBadPed;
						GET_CHAR_COORDINATES(netPed, &posn.x, &posn.y, &posn.z);
						GET_DISTANCE_BETWEEN_COORDS_3D(posn.x, posn.y, posn.z, mX, mY, mZ, &distanceFromBadPed);
						if (GET_CHAR_MONEY(netPed) != money_amount_ped_prot_marker) //generated random on startup
						{
							/**
							if (dodgyType == DODGY_PED_PLAYER) //probs already fucked tbh
							{
								//TODO NATH teleport away.
								printf("Teleport away!");
							}
							*/
							//might move this back to FIndControllerOfNetworkId as this method doesn't seem to be entirely accurate.
							char* buffer = BUFFER128;
							SET_CHAR_MONEY(netPed, money_amount_ped_prot_marker); //locally works / caps
							bool cutScene;
							if (distanceFromBadPed > 70.0f)
							{
								if (dodgyType == DODGY_PED_SUPERLOD)
									sprintf(buffer, "~r~CTL: %s ~w~spawned a ~r~freeze ~w~ped. (SuperLOD)", NetworkPlayerGetNameWithColour(entityCreatorID));
								else
									sprintf(buffer, "~r~CTL: %s ~w~spawned a ~r~freeze ~w~ped. (%s)", NetworkPlayerGetNameWithColour(entityCreatorID), bad_player_model_name(PED_PLAYER, &cutScene));
							}
							else
							{
								if (dodgyType == DODGY_PED_SUPERLOD)
									sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Ped SuperLOD)", NetworkPlayerGetNameWithColour(entityCreatorID));
								else
									sprintf(buffer, "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Ped %s)", NetworkPlayerGetNameWithColour(entityCreatorID), bad_player_model_name(PED_PLAYER, &cutScene));
							}
							if (GET_HOST_ID() == myPlayerIndex && entityCreatorID != myPlayerIndex && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_AUTO_KICK))
							{
								uint kicked_array_index;
								if (AddPlayerToKickStorage(entityCreatorID, &kicked_array_index))
								{
									strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Spawned freeze ped", 26);
									stradd(buffer, ", console kicked.", 128); //probs remove
								}
								else
									stradd(buffer, ", player kicked.", 128); //probs removes

								NETWORK_KICK_PLAYER(entityCreatorID);
							}
							Alert(buffer);
							//maybe if distance less than 80.0f
							if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS) && distanceFromBadPed < 70.0f)
							{
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = entityCreatorID;
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_CLONE_PED + 1;
								ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = (dodgyType == DODGY_PED_SUPERLOD ? PED_SUPERLOD : PED_PLAYER);
								SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
								reset_attack_broadcast_tick = (GetTickCount() + 1000); //maybe
							}
						}
					}
					if (dodgyType == DODGY_PED_CUTSCENE && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_CUTSCENE))
					{
						//TODO add to player model protection.
						//if (IsThisPedAPlayer(netPed)) //player model protection will get it
						//	continue;
						//should really add a count for this to be more precise but meh, you're protected who gaf.
						SET_CHAR_VISIBLE(netPed, false);
						if (GET_CHAR_MONEY(netPed) != money_amount_ped_prot_marker) //generated random on startup
						{
							//might move this back to FIndControllerOfNetworkId as this method doesn't seem to be entirely accurate.
							uint cs_model;
							GET_CHAR_MODEL(netPed, &cs_model);
							bool cutScene;
							printf("~r~CTL: %s ~w~spawned a ~y~glitchy cutscene ~w~ped (%s).", NetworkPlayerGetNameWithColour(entityCreatorID), dodgyPedModelName(cs_model, &cutScene));
							SET_CHAR_MONEY(netPed, money_amount_ped_prot_marker);
						}
					}
				}
			}
		}

		CPool* NetDummyPedPool = *(CPool**)0x8318EBB4;
		for (int i = 0; i < NetDummyPedPool->m_maxHandle; i++)
		{
			int NetPtr = (int)(NetDummyPedPool->m_firstPointer + (i * NetDummyPedPool->m_structSize));
			int EntityPtr = *(int*)(NetPtr + 0x4);
			if (EntityPtr && EntityPtr != 0xCDCDCDCD)
			{
				short dummyPedModelID = *(short*)(EntityPtr + 0x2E);
				short entityNetworkID = *(short*)(NetPtr + 12);
				byte entityCreatorID = *(byte*)(NetPtr + 15);

				const int dodgyType = is_wModelID_a_Dodgy_Ped(dummyPedModelID);
				if (dodgyType != DODGY_PED_NONE)
				{
					vector4 dummyCoords = *(vector4*)(*(int*)(EntityPtr + 0x20) + 0x30);
					float dodgyDistance, originalDistance;
					GET_DISTANCE_BETWEEN_COORDS_3D(dummyCoords.x, dummyCoords.y, dummyCoords.z, mX, mY, mZ, &dodgyDistance);
					originalDistance = dodgyDistance;
					float randX, randY, randZ;
					if ((dodgyType == DODGY_PED_SUPERLOD || dodgyType == DODGY_PED_PLAYER) && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_SUPERLOD))
					{
						if (dodgyDistance < 80.0f)
						{
							do
							{
								//tz > 4050.0f || tz < -130.0f) ); //-130 z not killzone but useless coords so
								GENERATE_RANDOM_FLOAT_IN_RANGE(-2800.0f, 2800.0f, &randX);
								GENERATE_RANDOM_FLOAT_IN_RANGE(-3000.0f, 3000.0f, &randY);
								GENERATE_RANDOM_FLOAT_IN_RANGE(-75.0f, 3500.0f, &randZ); //-100Z will put you on the map, careful now!
								GET_DISTANCE_BETWEEN_COORDS_3D(dummyCoords.x, dummyCoords.y, dummyCoords.z, randX, randY, randZ, &dodgyDistance);
							} while (dodgyDistance < 80.0f);

							//TODO NATH: Sort moving away properly!
							//teleportlocalplayeronfoot i reckon.

							SET_CHAR_COORDINATES(myPlayerPed, randX, randY, randZ);
							mX = randX; mY = randY; mZ = randZ; //used further down in script by lots of other things.
						}
						//might move this back to FIndControllerOfNetworkId as this method doesn't seem to be entirely accurate.
						char* buffer = BUFFER128;
						uint netPedModel;
						bool cutScene;
						if (dodgyType == DODGY_PED_SUPERLOD)
						{
							sprintf(buffer, (originalDistance > 70.0f) ? "~r~CTL: %s ~w~spawned a ~r~freeze ~w~dummy ped. (SuperLOD)" : "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Dummy ped SuperLOD)", NetworkPlayerGetNameWithColour(entityCreatorID));
							netPedModel = PED_SUPERLOD;
						}
						else
						{
							sprintf(buffer, (originalDistance > 70.0f) ? "~r~CTL: %s ~w~spawned a ~r~freeze ~w~dummy ped. (%s)" : "~r~CTL: %s ~w~tried to ~r~freeze ~w~you!~n~(Dummy ped %s)", NetworkPlayerGetNameWithColour(entityCreatorID), bad_player_model_name(PED_PLAYER, &cutScene));

							//sprintf(buffer, "~r~CTL: %s ~w~spawned a ~r~freeze ~w~dummy ped (%s)", NetworkPlayerGetNameWithColour(entityCreatorID), bad_player_model_name(PED_PLAYER));
							netPedModel = PED_PLAYER;
						}

						if (GET_HOST_ID() == myPlayerIndex && entityCreatorID != myPlayerIndex && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_PROT_AUTO_KICK))
						{
							uint kicked_array_index;
							if (AddPlayerToKickStorage(entityCreatorID, &kicked_array_index))
							{
								strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, "Spawned freeze dummy ped.", 26);
								stradd(buffer, ", console kicked.", 128);
							}
							else
								stradd(buffer, ", player kicked.", 128);

							NETWORK_KICK_PLAYER(entityCreatorID);
						}
						Alert(buffer);
						if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS) && originalDistance < 70.0f) //not trying to freeze us if it's out of render..
						{
							ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[0] = entityCreatorID;
							ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].offender_attackType[1] = ATTACK_CLONE_DUMMYPED + 1;
							ctlMem->m_CartelNetAttackVars.m_Data[myPlayerIndex].extraData = netPedModel;
							reset_attack_broadcast_tick = (GetTickCount() + 1000); //maybe
							SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ATTACKS);
						}

					}
					if (dodgyType == DODGY_PED_CUTSCENE && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_CUTSCENE))
					{
						if (dodgyDistance < 80.0f)
						{
							do
							{
								//tz > 4050.0f || tz < -130.0f) ); //-130 z not killzone but useless coords so
								GENERATE_RANDOM_FLOAT_IN_RANGE(-2800.0f, 2800.0f, &randX);
								GENERATE_RANDOM_FLOAT_IN_RANGE(-3000.0f, 3000.0f, &randY);
								GENERATE_RANDOM_FLOAT_IN_RANGE(-75.0f, 3500.0f, &randZ); //-100Z will put you on the map, careful now!
								GET_DISTANCE_BETWEEN_COORDS_3D(dummyCoords.x, dummyCoords.y, dummyCoords.z, randX, randY, randZ, &dodgyDistance);
							} while (dodgyDistance < 80.0f);

							//TODO NATH: Sort moving away properly!
							//teleportlocalplayertocoordinate i reckon.
							SET_CHAR_COORDINATES(myPlayerPed, randX, randY, randZ);
							mX = randX; mY = randY; mZ = randZ; //used further down in script by lots of other things.
						}
						uint cs_model;
						GET_PED_MODEL_FROM_INDEX(dummyPedModelID, &cs_model);
						bool cutScene;
						printf("~r~CTL: %s ~w~spawned a ~y~glitchy cutscene ~w~dummy ped (%s).", NetworkPlayerGetNameWithColour(entityCreatorID), dodgyPedModelName(cs_model, &cutScene));
						//might move this back to FIndControllerOfNetworkId as this method doesn't seem to be entirely accurate.
					}
				}
			}
		}
#pragma endregion

#pragma region explosion_lag
		if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_EXPLOSION_LAG))
		{
			int explosion_count;
			const int number_explosions_available = (myCurrentEpisode == EPISODE_TBOGT ? 25 : 15);
			for (int i = 0; i < number_explosions_available; i++)
			{	//don't think we need an FPS check now ideally?
				if (!IS_EXPLOSION_IN_AREA(i, mX - 7.0f, mY - 7.0f, mZ - 7.0f, mX + 7.0f, mY + 7.0f, mZ + 7.0f))
					continue;

				explosion_count++;
				if (i == EXPLOSION_SHIP_DESTROY && explosion_count > 1 || explosion_count >= 6)
				{
					//overwrite explosion temporarily with non-laggy one?
					//SHIP_DESTROY        500.0        200.0        2.0        1.0        8.0         100.0        -1.2        0.0        0.0            0.0        0.0        -    -    1.0        0    0    0.0    0.0    0.0    EXPLOSION    0        0.741    0.0    0.0    250.0    24.0    0.5    2
					//3706.1404,-2496.5984,10.9329, 90 //prot safe zone / pawns invincible heli here.
					//if driving set invincible car + player + tp to safezone / killzone where explosions don't render.
					CLEAR_AREA(mX, mY, mZ, 60.0f, true);
					SET_PLAYER_INVINCIBLE(myPlayerIndex, true);
					PRINTSTRING("\n[Cartel Helper], detected explosion lag and ran CLEAR_AREA(mX, mY, mZ, 60.0f, true);");
					break;
				}
			}
		}
#pragma endregion

	} //end else from if(Xex_Present)


	if (!IS_IN_PLAYER_SETTINGS_MENU()) //Rockstar uses this weather in the playersettings menu and the below catches on exit without this..
	{
		uint current_weather;
		GET_CURRENT_WEATHER(&current_weather);
		if (current_weather == WEATHER_SUNNY_WINDY_2)
		{
			int cur_hour, cur_minute;
			GET_TIME_OF_DAY(&cur_hour, &cur_minute);
			if ((cur_hour == 21 && cur_minute > 24) || cur_hour == 22 || (cur_hour == 23 && cur_minute < 36)) //dodgy rangee causes lag/deadlock
			{
				SET_TIME_OF_DAY(23, 36); //avoids Deadlock in rendering/weather/time
				printf("~r~CTL: ~w~Bad weather/time combo detected, skipping.");
			}
		}
	}

#pragma region host_only
	if (GET_HOST_ID() == myPlayerIndex)
	{
		KickedConsoleScanner();
	}
#pragma endregion

	if (last_player_model != 0) //catch delayer player model flipping.
	{
		if (change_player_tick < GetTickCount())
		{
			change_player_model(last_player_model);
			last_player_model = 0;
		}
	}
}