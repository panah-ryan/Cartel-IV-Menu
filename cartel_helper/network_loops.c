#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"
#include "cartel_chat.h"
#include "Functions.h"
#include "protections.h"
#include "../Menu/Functions.h"
#include "../Menu/Admin_Options.h"

//INFO: Tries to detect if they are modding.. (Player must be valid before calling, only called if not currently detected).

void do_modding_checks(PLAYER_INDEX playerIndex, char* playerName)
{
	/*
#ifdef Moderator //testing
	if (GET_PLAYER_COLOUR(playerIndex) == PLAYER_COLOR_BLACK) //makes ye fail to join games..
	{
		printf("~r~CTL MOD: ~w~%s detected with black colour, is this viable modder detect?", playerName);
		char* buffer = BUFFER128;
		sprintf(buffer, "%s Detected modding but not logged in helper,c (valid & colour is black)\n", playerName);
		//SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
	}
#endif
*/
	if(!CartelXEXPresent) //no point checking tag characters on RGH as XUID check will get them.
	{
		if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, (playerIndex + 16))) //set true if CTL_MACHINE_HAS_ALIAS.
		{
			SET_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, (playerIndex + 16)); //set now so we can bail early.
			for (int i = 0; playerName[i] != '\0'; i++)
			{
				//const char letterAtIndex = playerName[i];
				switch (playerName[i])
				{
				case '~':
					//case '¬': //R* symbol
				case '^':
				case '*':
				case '@':
				case '#':
				case '`': //pretty sure this is also not allowed.
				case '$':
					//case '£':
				case '%':
				case '&':
				case '!':
				case '?':
				case '+':
				case '-':
				case '=':
				case '|':
				case '\\':
				case '/':
				case ':':
				case ';':
				case '"':
				case '\'':
				case '(':
				case ')':
				case '{':
				case '}':
				case '[':
				case ']':
				case '<':
				case '>':
				case ',':
				case '.':
				case '_':
				{
					SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, playerIndex);
#ifdef Developer
					PRINTSTRING("Set ");
					PRINTSTRING(playerName);
					PRINTSTRING(" detected modding (prohibited character in gamertag)\n");
#endif
					return; //no point checking other shit, we found them modding so lets bail.
				}
				}
			}
		}
	}
	if (PLAYER_HAS_CHAR(playerIndex))
	{
		PED_INDEX playerPed;
		GET_PLAYER_CHAR(playerIndex, &playerPed);

		if (IsPlayerInvincible_ctlHelper(playerIndex, playerPed))
		{
#ifdef Moderator
			char* buffer = BUFFER128;
			sprintf(buffer, "Set %s detected modding in helper.c (valid & invincible)\n", playerName);
			PRINTSTRING(buffer);
#endif
			SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, playerIndex);
			return;
		}

		if ((player_valid_tick[playerIndex] + 4000) < GetTickCount() && player_valid_tick[playerIndex] != 0)
		{   //dont run for x + 5 seconds from when they valid (where x is 10 seconds)

			/* //I highly doubt we'll ever see this tbh, INCREASE_
			int netPedHealth, netPedArmour;
			GET_CHAR_HEALTH(netPed, &netPedHealth);
			GET_CHAR_ARMOUR(netPed, &netPedArmour);
			if (netPedHealth > 200 || netPedArmour > 100)
			{
#ifdef Developer
				//PRINTSTRING("Set ");
				PRINTSTRING(GET_PLAYER_NAME(i));
				PRINTSTRING(" detected modding in helper network_loops.c (health or armour above normal ranges)\n");
				//SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
#endif
			}
			*/
			uint playerModel;
			GET_CHAR_MODEL(playerPed, &playerModel);
			if (playerModel != PED_M_Y_MULTIPLAYER && playerModel != PED_F_Y_MULTIPLAYER)
			{
#ifdef Moderator
				char* buffer = BUFFER128;
				sprintf(buffer, "Set %s detected modding in helper.c (valid for 5 seconds and not a MP model)\n", playerName);
				PRINTSTRING(buffer);
#endif
				SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, playerIndex);
			}
		}
	}
}

void modder_detect_loop(void)
{
	if (CartelXEXPresent)
	{
		for (int i = 0; i < 16; i++) //XEX Only loop
		{
			if (IsPlayerValid(i)) //should save is from bad pointer hopefully
			{
				CPlayerInfo* player = GetPlayerPointer(i);
				if (player == NULL)
					continue;
#ifdef Moderator //aliaising only for mods.
				//ALIASING
				if (!IS_BIT_SET(ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, i)) //lets check for an alias.
				{
					if (CTL_MACHINE_HAS_ALIAS(&player->m_machineId[0]))
					{
						char* alias = BUFFER16;
						alias = CTL_GET_QUERIED_ALIAS();
#ifdef Developer
						PRINTSTRING("Found alias for ");
						PRINTSTRING(player->m_gamertag);
						PRINTSTRING(" (");
						PRINTSTRING(alias);
						PRINTSTRING(")\n");
#endif
						strcpy(ctlMem->m_char_buffers.player_name_backup[i], player->m_gamertag, 16); //backup the gamertag so we can use it in friends list checks etc.
						//SET_BIT(&ctlMem->m_bit_sets.m_loggedPlayer_UINUSED, (i + 16)); //set backed up bit.

						printf("~r~CTL: ~w~Alias for %s ~w~found (~y~%s~w~)", NetworkPlayerGetNameWithColour(i), alias);
						int len = GET_LENGTH_OF_LITERAL_STRING(alias);
						if (len < 15)
							alias[len] = '^'; //append a star to the end of their alias, //buffer is \0\0\0 so don't need to worry about null terminator.

						strcpy(player->m_gamertag, alias, 16); //overwrite their gamertag with the alias.

#ifdef Moderator //just incase my ass ever decides to make this public - Nath
						if (alias[0] == 'K' && alias[1] == '\\' && GET_HOST_ID() == myPlayerIndex) //check if the alias starts with "K\" and boot them out if so.
						{
							NETWORK_KICK_PLAYER(i);
							continue; //we just kicked them so duh.
						}
#endif 

						//SET_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, (i + 16)); //Don't bother checking for modded characters cuz we add one. //No longer executed on RGH
					}
					SET_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, i);
				}
#endif //ifdef Moderator

				//modder_db.dat query (lets see if we already checked and not process if so) /
				/*
				if (!IS_BIT_SET(ctlMem->m_bit_sets.m_CrossReferencedXUID_StartedCheck, i)) //we haven't checked their XUID yet.
				{
					if (!IS_BIT_SET(ctlMem->m_bit_sets.m_CrossReferencedXUID_StartedCheck, i + 16)) //we haven't started the check yet.
					{
						CTL_INITIATE_SPOOFER_CHECK(i, ctlMem->m_char_buffers.player_name_backup[i][0] != '\0' ? ctlMem->m_char_buffers.player_name_backup[i] : (char*)GET_PLAYER_NAME(i)); //need to ensure it checks the backup if we overwrote the name in memory (this function gets passed the correct one already)
						SET_BIT(&ctlMem->m_bit_sets.m_CrossReferencedXUID_StartedCheck, i + 16); //we have initiated the check against them.
					}
					int spoofing_status = CTL_IS_PLAYER_SPOOFING(i);
					if (spoofing_status != -1) //check is done processing.
					{
						SET_BIT(&ctlMem->m_bit_sets.m_CrossReferencedXUID_StartedCheck, i); //we have checked their XUID.
						if (spoofing_status) //it returned 1 AKA they are spoofing.
						{
							//SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, playerIndex);
							SET_BIT(&ctlMem->m_bit_sets.m_detectedXUIDSpoof_unused, i); //they are spoofing

#ifdef Moderator
							char* buffer = BUFFER128;
							sprintf(buffer, "Set %s detected modding in network_loops.c (XUID doesn't match that resolved from gamertag)\n");
							PRINTSTRING(buffer);
#endif
							char* notify_buffer = BUFFER128;
							sprintf(notify_buffer, "~r~CTL: ~w~Spoofer found: %s", NetworkPlayerGetNameWithColour(i));
							PRINT_STRING_WITH_TWO_LITERAL_STRINGS_NOW("STRTNM3", notify_buffer, "~w~their XUID does not match the one resolved from their gamertag.", 7500, true);
							//continue; //no point doing more modder checks.

							if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_KICK_360))
								NETWORK_KICK_PLAYER(i);
						}
					}
				}
				else
				{
					if ((player_valid_tick[i] + 4000) < GetTickCount() && player_valid_tick[i] != 0)
					{  //dont run for x + 4 seconds from when they valid (where x is 10 seconds) //if they're still here after this time, their MachineID etc probably isn't spoofed, so we'll log them.
						SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
					}
				}
				*/

				if (IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i)) //XEX will set this true if XUID mis-match and log to modder file.
				{
					//modder_db.dat LOGGING (logged bit is set above if they are found to avoid executing pointlessly).
					if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i)) //we haven't logged them to the modder file..
					{
						//make sure mods/admins don't get yeeted but meh.
						SET_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i);
#ifdef Developer				
						if (CTL_LOG_MACHINE_TO_MODDER_FILE(&player->m_machineId[0]))
						{ //drop them in the ini)
							PRINTSTRING("Logged ");
							PRINTSTRING(player->m_gamertag);
							PRINTSTRING(" to modder_db.dat (modding bit set in helper network_loops.c)\n");
						}
						else
						{
							PRINTSTRING("Skipped adding ");
							PRINTSTRING(player->m_gamertag);
							PRINTSTRING(" to modder_db.dat (already present)\n");
						}
#else
						//if(!IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i) && !IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (i + 16)))
						CTL_LOG_MACHINE_TO_MODDER_FILE(&player->m_machineId[0]);
#endif
					}
				}
				else
				{
					if (CTL_IS_MACHINE_IN_MODDER_FILE(&player->m_machineId[0]))
					{
						SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
						SET_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i); //no point trying to log them, they're already there..
#ifdef Moderator
						PRINTSTRING("Set ");
						PRINTSTRING(player->m_gamertag);
						PRINTSTRING(" detected modding in helper.sco (found in modder_db.dat)\n");
#endif
					}
					else
						do_modding_checks(i, ctlMem->m_char_buffers.player_name_backup[i][0] != '\0' ? ctlMem->m_char_buffers.player_name_backup[i] : player->m_gamertag); //if the name has been overwritten for whatever reasonn, use the backup instead.
						//do_modding_checks(i, player->m_gamertag);
				}
			} //isPlayerValid(i)
			else
			{
#ifdef Developer
				CLEAR_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, (i + 16)); //only devs can use this at present.
#endif
#ifdef PlayerDB
				CLEAR_BIT(&ctlMem->m_bit_sets.m_UploadedData_UNUSED, i);
#endif
#ifdef Moderator
				CLEAR_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, i); 
#endif
				CLEAR_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i); 
				CLEAR_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i); //no point doing this on RGH as XUID check will get it.
				CLEAR_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, (i + 16));

				CLEAR_BIT(&ctlMem->m_bit_sets.m_CrossReferencedXUID_StartedCheck, i);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_CrossReferencedXUID_StartedCheck, i + 16);

				CLEAR_BIT(&ctlMem->m_bit_sets.m_detectedXUIDSpoof_unused, i);
				CTL_CLEAR_PLAYER_SPOOFING_STATUS(i); //hopefully junk this eventually by hooking player left or whatever.
			}
		} //for(i = 0; i < 16);
	} //bitTest(XEX_PRESENT)
	else //Retail only mode.
	{
		for (int i = 0; i < 16; i++)
		{
			if (IsPlayerValid(i))
			{
				if (!IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i))
					do_modding_checks(i, ctlMem->m_char_buffers.player_name_backup[i][0] != '\0' ? ctlMem->m_char_buffers.player_name_backup[i] : (char*)GET_PLAYER_NAME(i));
			}
			else
			{
				CLEAR_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
				//CLEAR_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, (i + 16)); //RGH / Developer only.
				//CLEAR_BIT(&ctlMem->m_bit_sets.m_UploadedData_UNUSED, i);  //can't upload data to the cloud on a retail.
				//CLEAR_BIT(&ctlMem->m_bit_sets.m_UploadedData_UNUSED, (i + 16)); //can't check a text file on retail
				CLEAR_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, (i + 16));
				//CLEAR_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, i); //can't do aliasing from a file on the hard drive on retail ah..
				CLEAR_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, (i + 16));
			}
		}
	}
}

//BACKUP 26-Jan-2026
/*
void modder_detect_loop(void)
{
	if (bitTest(*(int*)stickyBitSet, STICK_XEX_PRESENT))
	{
#ifdef Moderator //Alias stuff only for mods/admins.
		for (int i = 0; i < 16; i++)
		{
#ifdef PlayerDB
			//don't overwrite the data until we uploaded / logged clean
			if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UINUSED, i) || !IS_BIT_SET(ctlMem->m_bit_sets.m_UploadedData_UNUSED, i)) //TODO: Make that shit take into account player name backups so we don't have to continue here;
				continue;
#else
			if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UINUSED, i)) //mandatory for all mods
				continue;
#endif
			if (IsPlayerValid(i) && !IS_BIT_SET(ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, i))
			{
				CPlayerInfo* player = GetPlayerPointer(i);
				if (CTL_MACHINE_HAS_ALIAS(&player->m_machineId[0]))
				{
					char* alias = BUFFER16;
					alias = CTL_GET_QUERIED_ALIAS();
#ifdef Developer
					PRINTSTRING("Found alias for ");
					PRINTSTRING(player->m_gamertag);
					PRINTSTRING(" (");
					PRINTSTRING(alias);
					PRINTSTRING(")\n");
#endif
					strcpy(ctlMem->m_char_buffers.player_name_backup[i], player->m_gamertag, 16);
					SET_BIT(&ctlMem->m_bit_sets.m_loggedPlayer_UINUSED, (i + 16));

					printf("~r~CTL: ~w~Alias for %s ~w~found (~y~%s~w~)", NetworkPlayerGetNameWithColour(i), alias);
					int len = GET_LENGTH_OF_LITERAL_STRING(alias);
					if (len < 15)
					{
						alias[len] = '^'; //buffer is \0\0\0 so don't need to worry about null terminator.
					}
					strcpy(player->m_gamertag, alias, 16);

#ifdef Moderator //just incase i ever make this public
					if (alias[0] == 'K' && alias[1] == '\\' && GET_HOST_ID() == myPlayerIndex)
					{
						NETWORK_KICK_PLAYER(i);
					}
#endif
				}
				SET_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, i);
			}
		}
#endif
		for (int i = 0; i < 16; i++)
		{
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_UploadedData_UNUSED, (i + 16)) || !IsPlayerValid(i) || i == myPlayerIndex)
				continue;

			CPlayerInfo* player = GetPlayerPointer(i);
			if (CTL_IS_MACHINE_IN_MODDER_FILE(&player->m_machineId[0]))
			{
				SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
				SET_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i); //no point trying to log them, they're already there..
#ifdef Developer
				PRINTSTRING("Set ");
				PRINTSTRING(player->m_gamertag);
				PRINTSTRING(" detected modding in helper protections.c (found in modder_db.dat)\n");
#endif
			}

			SET_BIT(&ctlMem->m_bit_sets.m_UploadedData_UNUSED, (i + 16));
		}


		for (int i = 0; i < 16; i++)
		{
			//if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i) && IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i) && IsPlayerValid(i) && i != myPlayerIndex)
#ifdef Developer
			if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i) && IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i) && IsPlayerValid(i))
#else
			if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i) && IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i) && IsPlayerValid(i) && !IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i) && !IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (i + 16)))
#endif
			{
				SET_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, i);
#ifdef Developer
				CPlayerInfo* player = GetPlayerPointer(i); //Use this in normal build once security is laxed
				if (CTL_LOG_MACHINE_TO_MODDER_FILE(&player->m_machineId[0]))
				{ //drop them in the ini)
					PRINTSTRING("Logged ");
					PRINTSTRING(player->m_gamertag);
					PRINTSTRING(" to modder_db.dat (modding bit set in helper network_loops.c)\n");
				}
				else
				{
					PRINTSTRING("Skipped adding ");
					PRINTSTRING(player->m_gamertag);
					PRINTSTRING(" to modder_db.dat (already present)\n");
				}
#else
				CTL_LOG_MACHINE_TO_MODDER_FILE(&GetPlayerPointer(i)->m_machineId[0]);
#endif
			}
		}
	}
	for (int i = 0; i < 16; i++)
	{
		if (IsPlayerValid(i))
		{
			if (!IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i))
			{
#ifdef Moderator
				if (GET_PLAYER_COLOUR(i) == PLAYER_COLOR_BLACK) //think only host can do this? Or do players send it on joining?
				{
					printf("~r~CTL MOD: ~w~%s detected with black colour, is this viable modder detect?", GET_PLAYER_NAME(i));
					//char* buffer = BUFFER128;
					//sprintf(buffer, "Set %s detected modding in helper,c (valid & colour is black)\n", GET_PLAYER_NAME(i));
					//SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
				}
#endif
				if (PLAYER_HAS_CHAR(i))
				{
					PED_INDEX netPed;
					GET_PLAYER_CHAR(i, &netPed);
					if (IsPlayerInvincible_ctlHelper(i, netPed)) 
					{
	#ifdef Moderator
						char* buffer = BUFFER128;
						sprintf(buffer, "Set %s detected modding in helper.c (valid & invincible)\n", GET_PLAYER_NAME(i));
						PRINTSTRING(buffer);
	#endif
						SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
						//continue;
					}
					if ((player_valid_tick[i] + 4000) < GetTickCount() && player_valid_tick[i] != 0) //dont run for x + 4 seconds from when they valid
					{
						uint pedModel;
						GET_CHAR_MODEL(netPed, &pedModel);
						if (pedModel != PED_M_Y_MULTIPLAYER && pedModel != PED_F_Y_MULTIPLAYER)
						{
#ifdef Developer
							char* buffer = BUFFER128;
							sprintf(buffer, "Set %s detected modding in helper.c (valid for 8 seconds and not a MP model)\n", GET_PLAYER_NAME(i));
							PRINTSTRING(buffer);
#endif
							SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
						}
					}
				}
#ifdef Moderator
				//Don't run if we've already checked this gamertag, or they have an alias AKA they will have a '^' appended by us.
				if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, (i + 16)) && !CTL_MACHINE_HAS_ALIAS(&GetPlayerPointer(i)->m_machineId[0]))  //alias shit
#else
				//Don't run if we've already checked this gamertag
				if(!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, (i + 16)))
#endif
				{ 
					SET_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, (i + 16));
					const char* name = GET_PLAYER_NAME(i);
					//COMMENT START size_t nameLen = GET_LENGTH_OF_LITERAL_STRING(name); //some tags exist as one character, not worth false flag potential.
					if (nameLen < 3)
					{
						SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
#ifdef Developer
						PRINTSTRING("Set ");
						PRINTSTRING(name);
						PRINTSTRING(" detected modding (gamertag too short)\n");
#endif
						continue;
					}
					//COMMENT END
					for (int j = 0; name[j] != '\0'; j++)  //iterate through players gamertag characters until null terminator
					{
						const char letterAtIndex = name[j]; //Never trust a switch to not overwrite your fucking variable
						switch (letterAtIndex)
						{
						case '~':
						//case '¬': //R* symbol
						case '^':
						case '*':
						case '@':
						case '#':
						case '`': //pretty sure this is also not allowed.
						case '$':
						//case '£':
						case '%':
						case '&':
						case '!':
						case '?':
						case '+':
						case '-':
						case '=':
						case '|':
						case '\\':
						case '/':
						case ':':
						case ';':
						case '"':
						case '\'':
						case '(':
						case ')':
						case '{':
						case '}':
						case '[':
						case ']':
						case '<':
						case '>':
						case ',':
						case '.':
						case '_': 
						{
							SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
#ifdef Developer
							PRINTSTRING("Set ");
							PRINTSTRING(name);
							PRINTSTRING(" detected modding (prohibited character in gamertag)\n");
#endif
							goto exit_modder_gamertag_char_check;
						}
						break;
						}
					} //for loop (gamertag chars check)
				
				exit_modder_gamertag_char_check:
					continue; //player for loop
				}
			}
		}
		else
		{
			CLEAR_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
			CLEAR_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, (i + 16));
			CLEAR_BIT(&ctlMem->m_bit_sets.m_UploadedData_UNUSED, i);
			CLEAR_BIT(&ctlMem->m_bit_sets.m_UploadedData_UNUSED, (i + 16));
			CLEAR_BIT(&ctlMem->m_bit_sets.m_loggedToModderFile_CheckedTagForModdedChars, (i + 16));
			CLEAR_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, i);
			CLEAR_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, (i + 16));
		}
	}
}
*/
void cartel_extra_bus_checks()
{
	for (int i = 0; i < MAX_RIGGED_BUSSES; i++)
	{
		if (DOES_VEHICLE_EXIST(ctlMem->m_CartelBusses[i].m_attachedBus) && !DOES_VEHICLE_EXIST(ctlMem->m_CartelBusses[i].m_hostBus))
		{ //host bus we attached too is gone, clear up our shit.
			if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(ctlMem->m_CartelBusses[i].m_attachedBus_NetId)) //we'll always have control as we locked it on creation.
				SET_NETWORK_ID_CAN_MIGRATE(ctlMem->m_CartelBusses[i].m_attachedBus_NetId, true);

			MARK_CAR_AS_NO_LONGER_NEEDED(&ctlMem->m_CartelBusses[i].m_attachedBus);
		}

		if (DOES_VEHICLE_EXIST(ctlMem->m_CartelBusses[i].m_attachedBus) && DOES_VEHICLE_EXIST(ctlMem->m_CartelBusses[i].m_hostBus))
		{
			if (IS_CAR_DEAD(ctlMem->m_CartelBusses[i].m_hostBus))
			{
				if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(ctlMem->m_CartelBusses[i].m_attachedBus_NetId)) //we'll always have control as we locked it on creation.
					SET_NETWORK_ID_CAN_MIGRATE(ctlMem->m_CartelBusses[i].m_attachedBus_NetId, true);

				MARK_CAR_AS_NO_LONGER_NEEDED(&ctlMem->m_CartelBusses[i].m_attachedBus);
				//ctlMem->m_CartelBusses[i].m_attachedBus_NetId = 0;
			}
		}
	}
	/*ctlMem->m_CartelBusses[freeSpace].m_hostBus = GetPlayerVeh();*/
}

int unlimited_ammo_re_give_weapons_tick = 0;
#ifdef Moderator
int player_detected_tick[16];
bool printed_player_active[16];
#endif
void cartel_network_loops()
{
	//stuff that mainly affects other players that would interrupt a mod if the main menu pauses goes here.
	//equally this script can't pause due to protections, so spawning and deleting of entities is still handled in cartel.
	if (ctlMem->m_keyboard.start_pressed_tick > GetTickCount())
	{
		DISABLE_PAUSE_MENU(true);
		DEACTIVATE_FRONTEND();
	}

	if (NetworkGame)
	{
		modder_detect_loop();
		if (ctlMem->m_hostFreemodeCrasherTick != 0) //timer has been set / we have suggested NULL to fuck the host.
		{
			if (ctlMem->m_hostFreemodeCrasherTick < GetTickCount() || GET_HOST_ID() == -1) //timer expired or host left/changing.
			{
				SuggestGamemode(-1); //nothing == -1; / default //restore invalid suggestion to prevent further host termination.
				ctlMem->m_hostFreemodeCrasherTick = 0; //prevent loop blocking of suggestions.
			}
		}
	}

	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_LOOP_CALLED) || !NetworkGame)
	{
		cartel_extra_bus_checks();
		PED_INDEX networkPlayerPed, networkPlayerVehicleDriver;
		VEHICLE_INDEX networkPlayerVehicle;
		NETWORK_INDEX networkPlayerVehicleId;
		float mX, mY, mZ, pX, pY, pZ, pH, networkPlayerSpeed, distance;
		bool reset_walk_air_tick = (freeMem->m_walk_on_air.m_tick < GetTickCount());
		uint IndexesToCheck = NetworkGame ? 16 : 1; //playerindex is always 0 in singleplayer.
		bool reset_unlim_ammo_tick = (unlimited_ammo_re_give_weapons_tick < GetTickCount());
		/*
#ifdef Moderator
		for (int i = 0; i < IndexesToCheck; i++)
		{
			if (i == myPlayerIndex)
				continue;

			if (IsPlayerValid(i))
			{
				if (player_detected_tick[i] == 0)
					GET_GAME_TIMER(&player_detected_tick[i]);

				if (!printed_player_active[i] && IS_NETWORK_PLAYER_ACTIVE(i))
				{
					printed_player_active[i] = true;
					char* buff = BUFFER128;
					int tmp;
					GET_GAME_TIMER(&tmp);
					sprintf(buff, "[%s] It took %s %ims to become active. WeAreHost=%i", IsPlayerUsingXboxOne(i) ? "ONE" : "360", SafeNetworkPlayerGetName(i), (tmp - player_detected_tick[i]), GET_HOST_ID() == GET_PLAYER_ID());
					CTL_WRITE_TO_FILE("join_times_log.txt", buff);
				}
			}
			else
			{
				player_detected_tick[i] = 0;
				printed_player_active[i] = false;
			}
		}
#endif
*/
		for (int i = 0; i < IndexesToCheck; i++)
		{
			if (!IsPlayerValid(i) || !PLAYER_HAS_CHAR(i)) //this also runs on local player.
			{
				//handle bit clearing and object deleting in main .sco, just skip here.
				//object deleting takes place inside cartel.sco to keep things clean.
				continue;
			}
			GET_PLAYER_CHAR(i, &networkPlayerPed);
			GET_CHAR_SPEED(networkPlayerPed, &networkPlayerSpeed);
			if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
			{
				GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
				GET_DRIVER_OF_CAR(networkPlayerVehicle, &networkPlayerVehicleDriver);
				GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);
			}

			GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
			GET_CHAR_COORDINATES(networkPlayerPed, &pX, &pY, &pZ);
			float ourDistanceFromNetworkPed;
			GET_DISTANCE_BETWEEN_COORDS_3D(mX, mY, mZ, pX, pY, pZ, &ourDistanceFromNetworkPed);

#pragma region actionpad_warping
			for (int aPad = 0; aPad < MAX_ACTIONPADS; aPad++) //actionPad slingshot warp players back to shore loop.
			{
				if (freeMem->m_action_pad[aPad].m_currentMode == PAD_MODE_SLINGSHOT && freeMem->m_action_pad[aPad].m_playerTick[i] > GetTickCount()) //player has been slingshotted within the last 20 seconds and pad is slingshot
				{
					if (DOES_OBJECT_EXIST(freeMem->m_action_pad[aPad].m_padObject))
					{
						if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
						{
							if (IS_CAR_IN_WATER(networkPlayerVehicle)) //they hae been slingshotted recently and they are now in the water cuz of it.
							{
								if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
								{
									//warp to shore
									float destX, destY, destZ;
									if (GET_CLOSEST_CAR_NODE(pX, pY, pZ, &destX, &destY, &destZ))
									{
										SET_CAR_COORDINATES(networkPlayerVehicle, destX, destY, destZ);
										FIX_CAR(networkPlayerVehicle); //they just got thrown into the air..
										SET_CAR_FORWARD_SPEED(networkPlayerVehicle, 0.0f);
										freeMem->m_action_pad[aPad].m_playerTick[i] = 0; //stop it running twice.
										if (bitTest(ctlMem->m_bit_sets.m_player, aPadNotifications))
											printf("~r~CTL PAD %i: %s ~w~landed in water, warped to land.", (aPad + 1), NetworkPlayerGetNameWithColour(i));
									}
								}
								else 
									REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
							}
						}
						else
						{
							if (IS_CHAR_IN_WATER(networkPlayerPed))
							{
								float destX, destY, destZ;
								if (GET_CLOSEST_CAR_NODE(pX, pY, pZ, &destX, &destY, &destZ))
								{
									GET_CHAR_HEADING(networkPlayerPed, &pH);
									TeleportNetworkPlayerOnFoot(i, destX, destY, destZ, pH);
									freeMem->m_action_pad[aPad].m_playerTick[i] = 0; //stop it running twice.
									if (bitTest(ctlMem->m_bit_sets.m_player, aPadNotifications))
										printf("~r~CTL PAD %i: %s ~w~landed in water, warped to land.", (aPad + 1), NetworkPlayerGetNameWithColour(i));
								}
							}
						}
						//we've slingshotted this player and need to check if they land in the water for warping back to shore..

					}
				}
			}
#pragma endregion

#pragma region vehicle_autoflip
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (i + 16))) //vehicle auto-flip
			{
				//CAN_BE_DESCRIBED_AS_A_CAR(networkPlayerVehicle);
				if (DOES_VEHICLE_EXIST(networkPlayerVehicle) && !IS_CHAR_IN_ANY_HELI(networkPlayerPed) && !IS_CHAR_IN_ANY_BOAT(networkPlayerPed))
				{
					float height_above_ground;
					GET_CHAR_HEIGHT_ABOVE_GROUND(networkPlayerPed, &height_above_ground); 
					if (height_above_ground < 4.0f && networkPlayerSpeed < 0.175f && IS_CAR_UPSIDEDOWN(networkPlayerVehicle))
					{
						if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId) || (!NetworkGame && i == myPlayerIndex)) //will this fuck up ghost?
						{
							SET_CAR_FORWARD_SPEED(networkPlayerVehicle, 0.0f);
							GET_CAR_HEADING(networkPlayerVehicle, &pH);
							SET_VEHICLE_QUATERNION(networkPlayerVehicle, 0.0f, 0.0f, 0.0f, 0.0f);
							SET_CAR_HEADING(networkPlayerVehicle, pH);
						}
						else
							REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
					}
				}
			}
#pragma endregion

#pragma region vehicle_horn_speedboost
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, i))
			{
				//Green or Red traffic light would work well.. can choose to only show an object when boosting is unavailable then.
				if (networkPlayerVehicleDriver == networkPlayerPed) //they are driving the car..
				{
					if (is_vehicle_horn_sounding(networkPlayerVehicle))
					{
						if (!IS_BIT_SET(ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, (i + 16))) //cap it))
						{
							if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId) || (!NetworkGame && i == myPlayerIndex))
							{
								SET_BIT(&ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, (i + 16)); //cap it
								//if(networkPlayerSpeed < 10.0f)
							//		SET_CAR_FORWARD_SPEED(networkPlayerVehicle, 15.0f);
							//	else
								SET_CAR_FORWARD_SPEED(networkPlayerVehicle, (networkPlayerSpeed * 2.25));
							}
							else
								REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
						}
					}
					else
						CLEAR_BIT(&ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, (i + 16)); //remove cap.
				}
			}
#pragma endregion

#pragma region Walking_on_air
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i))
			{
				if (DOES_OBJECT_EXIST(freeMem->m_walk_on_air.m_object[i]))
				{
					if (IS_CHAR_IN_ANY_HELI(networkPlayerPed) || IS_CHAR_IN_ANY_BOAT(networkPlayerPed))
					{
						CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i);
						//maybe remove this print?
						printf("~r~CTL: ~w~Air walk disabled for %s, ~w~they're in a boat or heli.", NetworkPlayerGetNameWithColour(i));
						continue;
					}
					//Coordinates work fine regardless of distance, they just don't update as frequently.
					//if (walkOnAirHeight[i] > (pZ + 20.0f)) //if it's way high above the player.
					if (freeMem->m_walk_on_air.m_height[i] > (pZ + 15.0f)) //if it's way high above the player.
						freeMem->m_walk_on_air.m_height[i] = (pZ - 1.0f);

					if (IS_PLAYER_DEAD(i)) //seems to sort it out for the most part I think?  //maybe disable if they die hmph?
					{ 
						if (IS_PED_ATTACHED_TO_OBJECT(myPlayerPed, freeMem->m_walk_on_air.m_object[i])) //no need to wait so don't call detach function
						{
							DETACH_PED(myPlayerPed, true);
							WAIT(10);
							CLEAR_CHAR_TASKS_IMMEDIATELY(myPlayerPed);
							bitReset(&ctlMem->m_bit_sets.m_player, attachedToWalker); //NEW
							WAIT(90);
						}
						DeleteObject(&freeMem->m_walk_on_air.m_object[i]);
						ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
						printf("~r~CTL: ~w~Walk on air disabled for %s ~w~(R.I.P)", NetworkPlayerGetNameWithColour(i));
						CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i);
						continue;
					}

					float oX, oY, oZ; //tempDist and use distance float for distance from us?
					GET_OBJECT_COORDINATES(freeMem->m_walk_on_air.m_object[i], &oX, &oY, &oZ);
					GET_DISTANCE_BETWEEN_COORDS_2D(oX, oY, pX, pY, &distance);

					if (freeMem->m_walk_on_air.m_height[i] < 0.0f && freeMem->m_walk_on_air.m_height[i] > (pZ + 10.0f)) //under the map and object is 10 higher than player (falling probably)..
						freeMem->m_walk_on_air.m_height[i] = 0.0f;

					if (freeMem->m_walk_on_air.m_height[i] > (pZ - 1.5f))  //if object is not too far below player (Stop it drifting off)
					{
						if (IS_CHAR_DUCKING(networkPlayerPed) || ((networkPlayerPed == networkPlayerVehicleDriver) && is_vehicle_horn_sounding(networkPlayerVehicle))) //if object is too far below dont run //prevents it "running off" below the player. 
							freeMem->m_walk_on_air.m_height[i] -= 0.045f; //might need to be lower, yes small as this will happen FAST!
					}
					if (IS_BIT_SET(ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, (i + 16))) //i + 16 = Ascend when shooting.
					{
						if (IS_CHAR_SHOOTING(networkPlayerPed))// || IS_PLAYER_TARGETTING_OBJECT(i,walkOnAirObj[i])) //distance limit for it to work is 150.0f, i measured 160, but 150 seems to be good.
						{
							//might be able to un-intentionally add jumping support by decreasing the below player distance check, i think it's just an animation and you stay at the same co-ordinates though. 
							//if (walkOnAirHeight[i] > (pZ + 3.0f) || walkOnAirHeight[i] < (pZ - 10.0f)) //if it's more than 3.0f above them or more than 10.0f below them.
							if (freeMem->m_walk_on_air.m_height[i] > (pZ + 3.0f) || freeMem->m_walk_on_air.m_height[i] < (pZ - 3.0f)) //if it's more than 3.0f above them or more than 3.0f below them.
							//if (walkOnAirHeight[i] > (pZ + 3.0f)) //if it's more than 3.0f above them or more than 10.0f below them.
								freeMem->m_walk_on_air.m_height[i] = pZ; //put back to their feet..
							else
								freeMem->m_walk_on_air.m_height[i] += 0.25f; //Perfect amount it seems.
						}
					}

					//is char stopped might be fuckin things up at dist
					SET_OBJECT_COORDINATES(freeMem->m_walk_on_air.m_object[i], oX, oY, freeMem->m_walk_on_air.m_height[i]); //update obj Z Posn only.
					if (IS_CHAR_IN_ANY_CAR(networkPlayerPed) || (reset_walk_air_tick && !IS_CHAR_STOPPED(networkPlayerPed))) //this is for position updates..
					{
						GET_CHAR_HEADING(networkPlayerPed, &pH);
						float world_x, world_y, disp_y, trash;

						//works absolutely fine now when nearby but off screen.. it's the distance that could do with further testing.
						uint objModel;
						GET_OBJECT_MODEL(freeMem->m_walk_on_air.m_object[i], &objModel);
						if (ourDistanceFromNetworkPed > 68.0f) //70 is the dist but we wanna do it just before shit goes south.
						{

							if (objModel == OBJECT_CJ_CR_BACKROUND)
							{
								GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(networkPlayerPed, 0.0f, 10.0f, 0.0f, &world_x, &world_y, &trash);
								pH -= 180.0f; //- heading is the way.. confirmed
								SET_OBJECT_VISIBLE(freeMem->m_walk_on_air.m_object[i], false);
							}
							else
							{
								if (HAS_MODEL_LOADED(OBJECT_CJ_CR_BACKROUND))
								{
									//freeMem->m_walk_on_air.m_height[i] + / - = ? //height is the same :D
									OBJECT_INDEX tmp;
									if (CreateObject(OBJECT_CJ_CR_BACKROUND, oX, oY, freeMem->m_walk_on_air.m_height[i], pH - 180.0f, &tmp, true))
									{
										//if (!IS_OBJECT_ON_PLAYER_MACHINE(freeMem->m_walk_on_air.m_object[i], i))
										//	WAIT(0); 

										DeleteObject(&freeMem->m_walk_on_air.m_object[i]);
										freeMem->m_walk_on_air.m_object[i] = tmp;
										//could maybe do with setting height -1.0f to "catch"the player when there's a moment of no object present.
										//see if changing modelid in memory works but i doubt it.
										//SET_OBJECT_VISIBLE(tmp, false);
									}
									else
									{
										printf("~r~CTL: ~w~Walk on air disabled for %s, ~w~bigger object for use at distance is missing.", NetworkPlayerGetNameWithColour(i));
										CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i);
									}
									SET_OBJECT_HEALTH(freeMem->m_walk_on_air.m_object[i], (TO_FLOAT(i) + 250.0f));
								}
								else
									REQUEST_MODEL(OBJECT_CJ_CR_BACKROUND);
							}
							//GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(networkPlayerPed, 0.0f, 2.50f + disp_y, 0.0f, &world_x, &world_y, &trash);
						}
						else
						{
							if (objModel == OBJECT_CJ_GAME_GRID)
							{
								GET_CHAR_EXTRACTED_DISPLACEMENT(networkPlayerPed, false, &trash, &disp_y, &trash);
								GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(networkPlayerPed, 0.0f, 1.25f + disp_y, 0.0f, &world_x, &world_y, &trash);
								SET_OBJECT_VISIBLE(freeMem->m_walk_on_air.m_object[i], !IS_BIT_SET(ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i + 16));
							}
							else
							{
								if (HAS_MODEL_LOADED(OBJECT_CJ_GAME_GRID))
								{
									OBJECT_INDEX tmp;
									if (CreateObject(OBJECT_CJ_GAME_GRID, oX, oY, freeMem->m_walk_on_air.m_height[i], pH - 180.0f, &tmp, true))
									{
										DeleteObject(&freeMem->m_walk_on_air.m_object[i]);
										freeMem->m_walk_on_air.m_object[i] = tmp;
										SET_OBJECT_VISIBLE(tmp, true);
									}
									else
									{
										printf("~r~CTL: ~w~Walk on air disabled for %s, ~w~bigger object for use at distance is missing.", NetworkPlayerGetNameWithColour(i));
										CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i);
									}
									/*
									DeleteObject(&freeMem->m_walk_on_air.m_object[i]);
									if (!CreateObject(OBJECT_CJ_GAME_GRID, oX, oY, freeMem->m_walk_on_air.m_height[i], pH, &freeMem->m_walk_on_air.m_object[i], true))
									{
										printf("~r~CTL: ~w~Unable to swap walk-air object for %s, ~w~could be no space available, disabled.", NetworkPlayerGetNameWithColour(i));
										CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i);
									}
									*/
									SET_OBJECT_HEALTH(freeMem->m_walk_on_air.m_object[i], (TO_FLOAT(i) + 250.0f));
								}
								else
									REQUEST_MODEL(OBJECT_CJ_GAME_GRID);
							}
						}
						if (IS_CHAR_ON_FOOT(networkPlayerPed))
						{ //in limited desting; displacement caused weird alignment with vehicle
							//if (networkPlayerSpeed > 0.25f) //not tested at distance.
							//{ //seems to help not widely tested yet 06/01/2025 01:19am
								
								SET_OBJECT_COORDINATES(freeMem->m_walk_on_air.m_object[i], world_x, world_y, freeMem->m_walk_on_air.m_height[i]);
							//}
						}
						else
							SET_OBJECT_COORDINATES(freeMem->m_walk_on_air.m_object[i], pX, pY, freeMem->m_walk_on_air.m_height[i]);


						//SET_OBJECT_COORDINATES(freeMem->m_walk_on_air.m_object[i], pX, pY, freeMem->m_walk_on_air.m_height[i]);
						//GET_CHAR_HEADING(networkPlayerPed, &pH);
						SET_OBJECT_HEADING(freeMem->m_walk_on_air.m_object[i], pH);
					}
				}
			}
#pragma endregion
			//don't need an else / warp back as taken care of in DeleteAllEntities and user toggle.
#pragma region infinite_ammo
			if (reset_unlim_ammo_tick) //probably need to delay this to only run every 500ms or something.
			{
				if (IS_BIT_SET(ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, i))
				{
					int currentWeapon;
					GET_CURRENT_CHAR_WEAPON(networkPlayerPed, &currentWeapon);
					//GET_MAX_AMMO(networkPlayerPed, currentWeapon, currentWeaponMaxAmmo);
					int playerWeapons[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
					//int playerMaxAmmos[8];
					for (int y = 0; y < 36; y++)
					{
						if (HAS_CHAR_GOT_WEAPON(networkPlayerPed, y) && y != currentWeapon)
						{
							for (int z = 0; z < 10; z++)
							{
								if (playerWeapons[z] == -1) // room here
								{
									playerWeapons[z] = y;
									//GET_MAX_AMMO(networkPlayerPed, playerWeapons[z], &playerMaxAmmos[z]);
									break;
								}
							}
						}
					}
					REMOVE_ALL_CHAR_WEAPONS(networkPlayerPed);
					for (int z = 0; z < 10; z++)
					{
						if (playerWeapons[z] != -1)
							GIVE_WEAPON_TO_CHAR(networkPlayerPed, playerWeapons[z], AMMO_MAX, false);
					}
					GIVE_WEAPON_TO_CHAR(networkPlayerPed, currentWeapon, AMMO_MAX, false);

					//end testing

					//GiveWeaponsToPed(networkPlayerPed, 1, true); //this will work.
				}
			}
#pragma endregion

#pragma region instant_revive
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, (i + 16))) //instant revive.
			{
				if (IS_PLAYER_DEAD(i))
				{
					if (!IS_BIT_SET(ctlMem->m_bit_sets.m_capDickheadNotify_instantReviveCap, (i + 16))) //cap resurrects while network catches up (don't resurrect them 10 times due to latency)
					{
						if (IS_CHAR_ON_FOOT(networkPlayerPed) && !AreCoordinatesNearKillzone(pX, pY, pZ)) //prevent an infinite death in the killzone or breaking due to being pulled out of a vehicle.
						{
							if (ourDistanceFromNetworkPed < 70.0f) //getting groundz doesn't work at distance.
							{
								float groundZ;
								GET_GROUND_Z_FOR_3D_COORD(pX, pY, (pZ + 4.0f), &groundZ); //this native works your Z and goes down, if it's too low you're fked.
								pZ = groundZ;
							}
							GET_CHAR_HEADING(networkPlayerPed, &pH);
							TeleportNetworkPlayerOnFoot(i, pX, pY, pZ, pH); //instantly resurrect //function handles host byte jiggin
							SET_BIT(&ctlMem->m_bit_sets.m_capDickheadNotify_instantReviveCap, (i + 16));
						}
					}
				}
				else
					CLEAR_BIT(&ctlMem->m_bit_sets.m_capDickheadNotify_instantReviveCap, (i + 16)); //stop resurrect spam while network catches up (we send them resurrect event and will keep doing it until it comes back over the net without this).
			}
#pragma endregion

#pragma region Walking_on_water
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_removeWeapons_driveWater, (i + 16)))
			{
				if (DOES_OBJECT_EXIST(freeMem->m_walk_water_object[i]))
				{
					float ox = 8.0f; float oy;
					if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
					{
						//IS_CAR_UPRIGHT check might not be working at distance, one of them isn't, i suspect it's that..
						if (networkPlayerSpeed > 18.0f && IS_CAR_UPRIGHT(networkPlayerVehicle) && !IS_BIG_VEHICLE(networkPlayerVehicle))
							ox = 18.0f; //else 8.0f //now declared as 8.0f by default, no need for else

						GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(networkPlayerPed, 0.0f, ox, 0.0f, &pX, &pY, &pZ); //char heading = exact same as vehicles
						SET_OBJECT_COORDINATES(freeMem->m_walk_water_object[i], pX, pY, IS_CHAR_IN_ANY_BOAT(networkPlayerPed) ? -15.0f : -9.2f);
					}
					else
					{
						GET_OBJECT_COORDINATES(freeMem->m_walk_water_object[i], &ox, &oy, &pZ);
						GET_DISTANCE_BETWEEN_COORDS_2D(pX, pY, ox, oy, &distance); //distance from center of object
						//if (distance > 16.20f)
						GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(networkPlayerPed, 0.0f, 7.0f, 0.0f, &pX, &pY, &pZ); //moved here as was fucking up exclusion areas for some reason.
						if (distance > 17.00f)
							SET_OBJECT_COORDINATES(freeMem->m_walk_water_object[i], pX, pY, -9.20f);
					}

					bool exclusionZone = IsDriveWaterExclusionArea(pX, pY, pZ, ourDistanceFromNetworkPed);
					//TUNNEL FIX for nearby players
					//if (i == myPlayerIndex)
						//printf("EXCLUSION AREA == %s", exclusionZone ? "TRUE" : "FALSE");

					SET_OBJECT_COLLISION(freeMem->m_walk_water_object[i], !exclusionZone);
					SET_OBJECT_VISIBLE(freeMem->m_walk_water_object[i], (!exclusionZone && bitTest(ctlMem->m_bit_sets.m_player, driveWaterVisible)));
					FREEZE_OBJECT_POSITION(freeMem->m_walk_water_object[i], true); //required
				}
			}
		}
#pragma endregion


		if (reset_walk_air_tick)
			freeMem->m_walk_on_air.m_tick = (GetTickCount() + 250);

		if (reset_unlim_ammo_tick) //so they all execute together. //2500 too little.
			unlimited_ammo_re_give_weapons_tick = (GetTickCount() + 2750); //only run once every 2.5 seconds.. //was 3 but that's a little bit too slow for unlimited clip on some weapons it seems.
	}
}