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

char* attachToVehStrings[5] = { "In front", "On top", "Behind", "Left", "Right" };

typedef enum eLobbyPrefSettings
{
	PREF_DONT_CARE = -1,
	PREF_DISALLOW = 0,
	PREF_ALLOW = 1,
} eLobbyPrefSettings;

//CHECK ALL INCLUDES / HEADER FILES ARE GOOD!
//NETWORK_GET_FRIEND_NAME -1 for your tag? optimistic.


search_results_array *searchResults; //this is seriously gonna need caretl_memory.sco 3640bytes //910 ints worth.
//point to 0x830FB110 //10 in total.

extern Page Container;

int netPlayerVehicleProtectionBitSet;

OBJECT_INDEX myNeonCarObjects[3]; //can probably make this just use freeMem->m_object_car_entities...
VEHICLE_INDEX bladesVehicle;
VEHICLE_INDEX bladesHeli;
int CopyPlayerSelect = 0;

int onlineScoreBitSet;

void NetworkGameSessions()
{
	printf("~r~CTL: ~w~Don't wait too long, indexes change, you'll end up in the wrong session.");
	SetHeader("Sessions");
	NetworkRunSearch(0); //does nothing with the param prolly left over from xex.
	if (*(int*)0x82AB0314 == 0)
	{
		AddItem("There are no online sessions currently!", nullptr, nullptr);
		return;
	}
	char* session_titles[10]; 
	session_titles[0] = BUFFER64;
	session_titles[1] = BUFFER64;
	session_titles[2] = BUFFER64;
	session_titles[3] = BUFFER64;
	session_titles[4] = BUFFER64;
	session_titles[5] = BUFFER64;
	session_titles[6] = BUFFER64;
	session_titles[7] = BUFFER64;
	session_titles[8] = BUFFER64;
	session_titles[9] = BUFFER64;

	for (int i = 0; i < *(int*)0x82AB0314; i++)
	{
		sprintf(session_titles[i], "%s (%i/%i)", searchResults->results[i].hostName, searchResults->results[i].players, searchResults->results[i].maxPlayers);
		AddItemWithParam(session_titles[i], i, NetworkJoinGame, "Attempt to join, we can't change network conditions.");
	}
}

//try NETWORK_INVITE_FRIEND(NETWORK_GET_MET_PLAYER_NAME(metPlayerIndex)); //luls

void NetworkOptsSpoofRecentPlayers(int unused1, int unused2)
{
	//~PAD_Y~ could be Spoof XUID.. //rgh only for that I believe. //unless XUID is also sored in there somewhere? I suspect not but I dunno - Nath
	SetHeader(ctlMem->m_commonStrings[GXT_STRING_RECENT_PLAYERS]); //"Recent players", "Recent Players"
	for (int i = 0; i < NETWORK_GET_NUM_PLAYERS_MET(); i++)
		AddItemWithParamAlt(NETWORK_GET_MET_PLAYER_NAME(i), i, NetworkOptSpoofRecentPlayer, "~PAD_A~ = Spoof gamertag, ~PAD_X~ = Open profile, ~PAD_Y~ = Feedback.", NetworkOptOpenRecentPlayerProfile, NetworkOptOpenRecentPlayerFeedback);
}

void NetworkGameOptsSpoofing()
{
	SetHeader("Network Options -> Spoofing");
	if (CartelXEXPresent)
	{
		AddItem("Spoof gamertag + XUID", NetworkOptsSpoofGTandXUIDTextInput, "Opens on-screen keyboard.");
		AddItem("Spoof XUID (Tag input)", NetworkOptSpoofXuidTextInput, "Opens on-screen keyboard.");
	}
	AddItem("Spoof gamertag", NetworkOptsSpoofGamertagTextInput, "Opens on-screen keyboard.");
	AddItem("Spoof XUID (Hex)", NetworkOptSpoofXUIDTextInput, "Recommend https://cxkes.me");
	//AddItem("Spoof XUID & GT (Gamertag)", NetworkOptSpoofXUID_GTGivenGamertag, "Enter Gamertag and also spoof it's XUID.");

	if (NETWORK_GET_NUM_PLAYERS_MET() > 0)
		AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_RECENT_PLAYERS], NetworkOptsSpoofRecentPlayers, nullptr); //"Recent players", "Recent Players"
	else
		AddItem("Recent players (None detected)", nullptr, nullptr);

	AddItem("Restore Gamertag and XUID", RestoreXUIDAndGamertagInFreeMem, "Cartel saved your Gamertag & XUID on first load.");
}

void NetworkOptKickedConsoles(int unusd1, int unused2) 
{
	SetHeader("Kicked consoles");
	if (GetNumberOfKickedConsoles() > 0)
	{
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
		{
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID != 0) 
				AddItemWithParamAlt(freeMem->m_cartelHostVariables.m_kicked_storage[i].gamertag, i, NetworkOptUnkickConsole, "~PAD_A~ = Un-kick, ~PAD_X~ = Print reason, ~PAD_Y~ = Give reason.", NetworkOptKickedConsolePrintReason, NetworkOptKickedConsoleGiveReason);
		}
	}
	else
		AddItem("None kicked", nullptr, nullptr);
}

void NetworkOptRecentlyKicked(int un1, int un2)
{
	SetHeader("Recently kicked players");
	if (GetNumberOfRecentlyKickedPlayers() > 0)
	{
		for (int i = 0; i < MAX_RECENTLY_KICKED_CONSOLES; i++)
		{
			if (ctlMem->m_recent_kicked_storage[i].MachineID != 0) //someting stored here.
				////use this as whitelist flag. (failsCounter -1337)
				AddItemWithParamAlt(ctlMem->m_recent_kicked_storage[i].gamertag, i, NetworkOptWhitelistRecentJoin, ctlMem->m_recent_kicked_storage[i].failsCounter == -1337 ? "Whitelisted, ~PAD_X~ = Clear and remove whitelist." : "~PAD_A~ = Whitelist, ~PAD_X~ = Clear.", NetworkOptClearRecentJoin, nullptr);
		}
		AddItemWithParam("Clear list & pending whitelists", 0, NetworkOptClearAllRecentJoins, "Make room for new-joins to be whitelisted.");
	}
}

void setPlayerColourBlack(int un1, int un2)
{
	SET_PLAYER_COLOUR(myPlayerIndex, 0);
}

void NetworkGameHostOptions()
{
	static char* gameModeStrings[17] = { "Deathmatch", "Team Deathmatch", "Mafiya Work", "Team Mafiya Work", "Team Car Jack City", "Car Jack City", "Race", "GTA Race", "Party Mode", "Unknown", "Cops and Crooks", "Unknown", "Turf War", "Deal Breaker", "Hangmans NOOSE", "Bomb Da Base", "Free Mode" };
	SetHeader("Network Options -> Host");
	//AddItemBool("Kick Xbox 360's", &ctlMem->m_bit_sets.m_network, NETWORK_KICK_360, toggle360kicker, "As described."); //PAD_X = except currently in game. TODO (maybe).
	AddItemBoolAlt("Kick Xbox 360's", &ctlMem->m_bit_sets.m_network, NETWORK_KICK_360, toggle360kicker, "As described, ~PAD_X~ = On + Allow current 360's.", toggle360KickerAllowCurrent, nullptr);
#ifdef AdminOptions
	AddItemBool("Store kicked 360 identifiers", &ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_STORE_KICKED_IDENTIFIERS, nullptr, "Kick their console, shouldnt be necessary.");
#endif
	AddItemBool("Allow friends", &ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_ALLOW_FRIENDS, nullptr, "Don't kick people on my friends list.");
	AddItemBool("Verify friends online status", &ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_CHECK_FRIENDS_ONLINE_STATUS, nullptr, "Also checks they're on GTA IV."); //PAD_X = except currently in game. TODO (maybe).

	if (GET_HOST_ID() == myPlayerIndex && GetNumberOfRecentlyKickedPlayers() > 0)
		AddItemMenu("Recently kicked players", NetworkOptRecentlyKicked, "Whitelist players, proceed with caution."); //prep for going public when activated on server.
	else
		AddItem("Recently kicked players", nullptr, "Won't open if no recently kickeds, or not host.");
	AddItemInt("Armour pickup regen (seconds)", false, 1, 180, &networkHostArmourRegenSeconds, 1, netHostOptSetArmourRegen, nullptr);
	AddItemInt("Health pickup regen (seconds)", false, 1, 180, &networkHostHealthRegenSeconds, 1, netHostOptSetHealthRegen, nullptr);
	AddItemInt("Weapon pickup regen (seconds)", false, 1, 180, &networkHostWeaponRegenSeconds, 1, netHostOptSetWeaponRegen, nullptr);
	AddItem("Set your player colour to black", setPlayerColourBlack, "Players that join after will see.");
	AddItem("End lobby", NetworkGameHostKickAll, "Kick all players, then find a new game.");

#ifdef AdminOptions 
	AddItemChar("Change game mode", false, gameModeStrings, 17, &gameModeSelect, NetworkChangeGameMode, "Yeet the lobby with you."); //maybe private could be annoying

	//AddItemBoolAlt("Allow join with PIN code.", &ctlMem->m_bit_sets.m_network, NETWORK_KICK_360_ALLOW_PIN_SET, generateLobby360JoinPin, "~PAD_X~ = Print PIN, ~PAD_Y~ = Input custom PIN.", printLobby360JoinPin, setLobby360JoinPin);
	AddItemBool("Disable Off-host kicking", &ctlMem->m_bit_sets.m_network, NETWORK_IGNORE_COMPLAINTS, nullptr, "Simply won't work in this lobby while you're host.");
#endif
	//extern _native void GET_NETWORK_TIMER(int* timer)l
	//Game timer?
	//Switch gamemode / lobby screen etc
}



void NetGameFriendOptions(int friendIndex, int unused0)
{
	//for custom input, mark your name with #1 and friends name with #2
	static char* inviteStrings[2] = { "Normal", "Cartel" };

	SetHeader(NETWORK_GET_FRIEND_NAME(friendIndex));
	AddItemCharAlt(ctlMem->m_commonStrings[GXT_STRING_INVITE], false, inviteStrings, 2, &inviteStringSelect, NetGameInviteFriend, "~PAD_A~ = Selection, ~PAD_X~ = Custom (Keyboard/Chatpad)", NetworkInviteFriendCustom, nullptr); //"Invite"
	AddItemWithParam("Open profile", friendIndex, NetGameFriendOpenProfile, nullptr);
}

void NetGameFriendsList()
{
	SetHeader(ctlMem->m_commonStrings[GXT_STRING_FRIENDS]); //"Friends list", "Friends"

	int friendCount = NETWORK_GET_FRIEND_COUNT();
	for (int i = 0; i < friendCount; i++) //List friends in same title first
	{
		if (NETWORK_IS_FRIEND_IN_SAME_TITLE(NETWORK_GET_FRIEND_NAME(i)))
			AddItemMenuWithParamAlt(NETWORK_GET_FRIEND_NAME(i), i, NetGameFriendOptions, "~PAD_X~ = Open profile.", NetGameFriendOpenProfile);
	}
	for (int i = 0; i < friendCount; i++)
	{
		if (!NETWORK_IS_FRIEND_IN_SAME_TITLE(NETWORK_GET_FRIEND_NAME(i)))
			AddItemMenuWithParamAlt(NETWORK_GET_FRIEND_NAME(i), i, NetGameFriendOptions, "~PAD_X~ = Open profile.", NetGameFriendOpenProfile);
	}
}

void updateOnlineBitSet(int unused0, int unused1)
{
	if (IS_PLAYER_ONLINE()) //could cause problems if a download isn't triggered on startup, think sign in changes cause a reload though so hopefully be aight.
	{
		SET_ONLINE_SCORE(cartel_online_bitset_score_id, onlineScoreBitSet);
		printf("~r~CTL: ~w~Auto-load settings preference saved.");
		UploadScores();
	}
	else
	{
		printf("~r~CTL: ~w~This only works online.");
		bitFlip(&onlineScoreBitSet, ONLINEBIT_AUTO_PULL);
	}
}
/*
void NetworkSaveLoadBitOpts()
{
	SetHeader("Network (Save/load)");
	AddItem("Save settings", save_bits_to_online_scores, "Save menu settings to GTA IV servers.");
	AddItem("Load settings", read_bits_from_online_scores, "Load menu settings from GTA IV servers.");
	AddItemBool("Auto-load when online", &onlineScoreBitSet, ONLINEBIT_AUTO_PULL, updateOnlineBitSet, "Auto pull settings when online.");
}
*/
void NetworkCartelRemoteAllowedCommands()
{
	SetHeader("Cartel Remote (Allowed commands)");
	AddItemBool("Teleport me (on foot)", &ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, SHARED_MENUCMD_TELEPORT_TO_SENDER_ON_FOOT, UpdateSharedCMDAllowList, "Let allowed players teleport you on foot.");
	AddItemBool("Teleport me (vehicle)", &ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, SHARED_MENUCMD_TELEPORT_TO_SENDER_IN_VEHICLE, UpdateSharedCMDAllowList, "Let allowed players teleport your vehicle.");
	AddItemBool("Teleport into their vehicle", &ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, SHARED_MENUCMD_TELEPORT_INTO_MY_VEHICLE, UpdateSharedCMDAllowList, "Let allowed players teleport you into their vehicle.");
}

void NetworkCartelRemoteAllowedPlayers()
{
	SetHeader("Cartel Remote (Allowed players)");
	for (int i = 0; i < 16; i++)
	{
		//if cartel user.
		if (IsPlayerValid(i) && i != myPlayerIndex && IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, i + 16)) //only show cartel users.
			AddItemBool(GET_PLAYER_NAME(i), &ctlMem->m_bit_sets.m_sharedMenuControlCMDS_playerAllowLists, i, UpdateSharedCMDAllowList, "Accept allowed commands from this player.");
	}
}

void NetworkCartelRemoteOptions()
{
	SetHeader("Cartel Remote (Allow lists)");
	if (bitTest(*(int*)stickyBitSet, STICK_ADVERTISE_MENU))
	{
		if (NetworkGame)
		{
			AddItemMenu("Allowed players", NetworkCartelRemoteAllowedPlayers, "Allow Cartel users to send you commands.");
			AddItemMenu("Allowed commands", NetworkCartelRemoteAllowedCommands, "Choose which commands to accept.");
		}
		else
			printf("~r~CTL: ~w~You must be online to use Cartel Remote.");
	}
	else
		AddItem("Advertise must be switched on at both ends.", nullptr, "In order for Cartel remote to work");
	//SendSharedMenuCommandToPlayer
}

void restorePlayerNames(int unused1, int unused2)
{
	if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES))
	{
		for (int i = 0; i < 16; i++)
		{
			if (IsPlayerValid(i))
			{
				//if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, (i + 16)))
				if (ctlMem->m_char_buffers.player_name_backup[i][0] == '\0') //nothing stored here.

				{	//back up the names regardless
					strcpy(ctlMem->m_char_buffers.player_name_backup[i], (char*)GET_PLAYER_NAME(i), 16);
					//SET_BIT(&ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, (i + 16));
				}
				if (i == myPlayerIndex)
#ifdef Developer
					strcpy((char*)GET_PLAYER_NAME(i), "CARTEL TEST", 16);
#else
					strcpy((char*)GET_PLAYER_NAME(i), "Me", 16);
#endif
				else
				{
					strcpy((char*)GET_PLAYER_NAME(i), "playerIndex ", 16);
					straddi((char*)GET_PLAYER_NAME(i), i, 16);
				}
			}
		}
	}
	else
	{	//just turned it off need to restore.
		for (int i = 0; i < 16; i++)
		{
			//if (IsPlayerValid(i) && IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, (i + 16)))
			if (IsPlayerValid(i) && ctlMem->m_char_buffers.player_name_backup[i][0] != '\0') //something is stored here.
				strcpy((char*)GET_PLAYER_NAME(i), ctlMem->m_char_buffers.player_name_backup[i], 16);
		}
	}
}

void cartelNetworkDisplayOptions(int unused1, int unused2)
{
	SetHeader("Network Options -> Display");
	AddItemBool("Draw Host, FPS, player/360 count etc", &ctlMem->m_bit_sets.m_menuPreferences, PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS, nullptr, "Optimised version from my old protection (IIV).");
	AddItemBool("Draw talking players", &ctlMem->m_bit_sets.m_menuPreferences, PREF_MINIMAP_DRAW_TALKING_PLAYERS, nullptr, "Optimised version from my old protection (IIV).");
	AddItemBool("- Draw shadow behind", &ctlMem->m_bit_sets.m_menuPreferences, PREF_DROPSHADOW_LOBBY_INFO, nullptr, nullptr);
	AddItemBool("Draw ghosts/joining players", &ctlMem->m_bit_sets.m_menuPreferences, PREF_TOP_DRAW_JOINING_GHOSTS_PLAYERS, nullptr, "Optimised version from my old protection (IIV).");
	AddItemBool("Hide player names (list)", &ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES, restorePlayerNames, "Handy for grabbing videos.");
#ifdef Developer
	AddItemBool("Display player names (ped)", (int*)0x8315E8BC, 0, nullptr, nullptr); //doesn't work as of yet.
#endif
	AddItemBoolAlt("ESP", &ctlMem->m_bit_sets.m_player, playerESP, nullptr, "~PAD_X~ = Toggle lines, ~PAD_Y~ = Toggle boxes.", ToggleEspLines, ToggleEspBoxes);
}

int autoAimPref, policePref, friendlyFirePref; //could stick this in a set of 4 bytes.. //meh it will fuck the AddItemChar scrolling/reading 
void writeAutoAimPref(int selection, int un1)
{
	WRITE_LOBBY_PREFERENCE(LOBBY_PREF_AUTO_AIM, autoAimPref - 1);
}

void writePolicePref(int selection, int un1)
{
	WRITE_LOBBY_PREFERENCE(LOBBY_PREF_POLICE, policePref - 1);
}

void writeFriendlyFirePref(int selection, int un1)
{
	WRITE_LOBBY_PREFERENCE(LOBBY_PREF_FRIENDLY_FIRE, friendlyFirePref - 1);
}

void NetworkGameSessionPreferences(int un1, int un2)
{
	SetHeader("Session Preferences");
	autoAimPref = READ_LOBBY_PREFERENCE(LOBBY_PREF_AUTO_AIM) + 1;
	policePref = READ_LOBBY_PREFERENCE(LOBBY_PREF_POLICE) + 1;
	friendlyFirePref = READ_LOBBY_PREFERENCE(LOBBY_PREF_FRIENDLY_FIRE) + 1;

	AddItemChar(ctlMem->m_commonStrings[GXT_STRING_AUTO_AIM], true, &ctlMem->m_commonStrings[GXT_STRING_DONT_CARE], 3, &autoAimPref, writeAutoAimPref, nullptr); //middle string so -1 should hit "Don't Care"
	AddItemChar(ctlMem->m_commonStrings[GXT_STRING_POLICE], true, &ctlMem->m_commonStrings[GXT_STRING_DONT_CARE], 3, &policePref, writePolicePref, nullptr);
	AddItemChar(ctlMem->m_commonStrings[GXT_STRING_FRIENDLY_FIRE], true, &ctlMem->m_commonStrings[GXT_STRING_DONT_CARE], 3, &friendlyFirePref, writeFriendlyFirePref, nullptr);
}

void NetworkSetSysLinkMode(int on, int un2)
{
	if (CartelXEXPresent)
		CTL_FORCE_ONLINE_STATE(on);

	if (on)
	{
		if (IS_PLAYER_ONLINE())
			printf("~r~CTL: ~w~OK: Create/Find a game normally (System-LINK).");
		else
		{
			char* status_string = BUFFER128;
			strcpy(status_string, "~r~CTL: ~w~Spoofing LIVE Status: ", 128);

			CPlayerInfo* local_player = GetPlayerPointer(myPlayerIndex);
			*(byte*)(local_player + 0x5C) |= 0x80;

			if (IS_PLAYER_ONLINE())
				stradd(status_string, "OK, Create/Find a game normally (System-LINK)", 128);
			else
			{
				stradd(status_string, "FAILED, Unable to initiate System-LINK (Cartel.xex required)", 128);
				*(bool*)0x8315F7A0 = false;
			}

			printf(status_string);
		}
	}
}


void NetworkGameOptions()
{
	static char* episodeNames[3] = { "Original", "TLaD", "TBoGT" };

	SetHeader("Network Options");
	episodeSelect = myCurrentEpisode;
	//AddItemAlt("Input Lobby/Join PIN", setJoinPin, "Use to join, ~PAD_X~ = Information", printJoinPinInfo, nullptr);
	AddItemBool("System-LINK Mode", (int*)0x8315F7A0, 0, NetworkSetSysLinkMode, "Connect over System-LINK instead of Xbox Live.");
	AddItemMenu("Protections", cartelProtectionOptons, "Work in progress.");
	AddItemMenuAlt("Sessions", NetworkGameSessions, "Search for games, ~PAD_X~ = Re-join current.", NetworkGameRejoinSession);
	AddItemMenu("Session Preferences", NetworkGameSessionPreferences, "Preferences for finding games.");
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_DISPLAY], cartelNetworkDisplayOptions, "Text chat, num 360 users, etc."); //"Display"
	AddItemBool("Auto-load settings online", &onlineScoreBitSet, ONLINEBIT_AUTO_PULL, updateOnlineBitSet, "Auto pull saved menu settings when online.");
	AddItemMenu("Text chat", Chat_Options, "Chat Engine by xStormsnoutx.");
	AddItemBool("Advertise menu", (int*)stickyBitSet, STICK_ADVERTISE_MENU, setMenuAdvertise, "Detect Cartel users and be scannable.");
	AddItemMenu("Cartel Remote", NetworkCartelRemoteOptions, "Allowed commands & players list.");
	AddItemMenu("Spoofing", NetworkGameOptsSpoofing, "Spoof your gamertag & XUID.");
	//AddItemMenuAlt("Sessions", NetworkGameSessions, "Check for sessions, ~PAD_X~ = Re-join current, ~PAD_Y~ = Singeplayer.", NetworkGameRejoinSession, NetworkGameOptSingleplayer);

	if (NetworkGame)
	{
		AddItem("Singleplayer", NetworkGameOptSingleplayer, "Return to Singleplayer.");
		//AddItem("Re-join", NetworkGameRejoinSession, "Leave and join this session."); //moved to sessions alt execute.
#ifdef Moderator
		if (GetNumberOfKickedConsoles() > 0)
			AddItemMenu("Kicked consoles", NetworkOptKickedConsoles, "MODERATOR: Remotely view and manage.");
#else
		if (GetNumberOfKickedConsoles() > 0 && GET_HOST_ID() == myPlayerIndex)
			AddItemMenu("Kicked consoles", NetworkOptKickedConsoles, "Won't open if none kicked, or not host.");
#endif
		else
			AddItem("Kicked consoles", NetworkPrintNoConsolesKicked, "No consoles kicked, or not host.");

		if (GET_HOST_ID() == myPlayerIndex) //I'm aware IS_THIS_MACHINE_THE_SERVER exists but bytes fuck with that and people are dumb so.
			AddItemMenu("Host settings", NetworkGameHostOptions, "Things only host should change.");
		else
			AddItem("Host settings", NetworkGamePrintNotHost, "Things only host should change.");

		AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_FRIENDS], NetGameFriendsList, nullptr); //"Friends", "Friends list" ctlMem->m_commonStrings[GXT_STRING_FRIENDS]
		AddItemBoolAlt("Rank 10 all players", &ctlMem->m_bit_sets.m_player, networkRank10All, toggleRank10All, "Give everyone Rank 10, ~PAD_X~ = Toggle exclude self.", toggleNetworkAllPlayersExcludeSelf, nullptr);
	}

	AddItemChar(NetworkGame ? "Find new game" : "Find game", false, episodeNames, 3, &episodeSelect, NetworkGameOptFindNewGame, nullptr);
	AddItemBool("Use Cartels Spawn-point", (int*)stickyBitSet, STICK_USE_CARTEL_SPAWN, nullptr, "Spawn inside Cartel HQ.");
	AddItemBoolAlt("Use Custom Spawn-point", &ctlMem->m_bit_sets.m_menuPreferences, PREF_CUSTOM_SPAWN, setCustomSpawnPoint, "Spawn at custom location, ~PAD_X~ = Teleport.", teleportCustomSpawnpoint, nullptr);
}


/* //BACKUP!! BEFORE BUS BULLSHIT //please keep this for reference as i intend to modify the bus shit further and may return to this original function - nath
void TeleportIntoNetworkPlayersVehicle(int playerIndex) //needs optimising but works for now.
{
	netPlayerVehDetach();
	Ped target_ped; Ped our_ped = myPlayerPed;
	Vehicle target_veh, our_veh;
	if (PLAYER_HAS_CHAR(playerIndex))
	{
		GET_PLAYER_CHAR(playerIndex, &target_ped);
		if (IS_CHAR_IN_ANY_CAR(target_ped))
		{
			GET_CAR_CHAR_IS_USING(target_ped, &target_veh);
			//if (IS_CHAR_IN_CAR(our_ped, target_veh)) return; //fucks up without this //not triggering seemingly..
			if (target_veh == GetPlayerVeh()) return;
			int freeSeat = GetFreeVehicleSeat(target_veh);
			if (freeSeat == SEAT_INVALID)
			{
				printf("~r~CTL: %s's ~w~%s has no free seats", NetworkPlayerGetNameWithColour(playerIndex), VehiclesModelName(target_veh));
				if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y) && IS_CHAR_IN_ANY_CAR(our_ped))
				{
					GET_CAR_CHAR_IS_USING(our_ped, &our_veh);
					WARP_CHAR_FROM_CAR_TO_CAR(our_ped, our_veh, GetPedSeatIndex(our_ped));
				}
			}
			else
			{
				RigLocalPlayerHealthArmourForProtectionWarpIntoDriversVehSkip();
				//if (freeSeat == SEAT_DRIVER)
					//WARP_CHAR_INTO_CAR(our_ped, target_veh);
				//else
				WARP_CHAR_FROM_CAR_TO_CAR(our_ped, target_veh, freeSeat); //pretty sure it works on foot too //gettin warped out after few secs, wut?
			}
		}
		else
		{
			if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y)) //should be true but whatever
			{
				if (IS_CHAR_IN_ANY_CAR(our_ped))
				{
					GET_CAR_CHAR_IS_USING(our_ped, &our_veh);
					RigLocalPlayerHealthArmourForProtectionWarpIntoDriversVehSkip();
					WARP_CHAR_FROM_CAR_TO_CAR(our_ped, our_veh, GetPedSeatIndex(our_ped)); //cancel out any exits.
				}
				else
				{
					//need to clear Y input
				}
			}
			printf("~r~CTL: %s ~w~is on foot.", NetworkPlayerGetNameWithColour(playerIndex));
			//cancel out exit vehicle etc.
		}
	}
}
*/


void netPlayerVehAttachTo(int* attach_index, int playerIdIfNot0Minus1)
{

	if (playerIdIfNot0Minus1 == 0)
		playerIdIfNot0Minus1 = GetMenuPlayerId();
	else
		playerIdIfNot0Minus1 -= 1;

	PLAYER_INDEX player = playerIdIfNot0Minus1;
	PED_INDEX target_ped;
	VEHICLE_INDEX our_veh;
	vector3 min, targetModelMax;
	uint target_model;
	CAMERA_INDEX playerCamera;

	//POINT_CAM_AT_VEHICLE?

	//PLAYER_INDEX player = GetMenuPlayerId();
	if (!SafeToActOnPlayerPed(player, &target_ped))
		return;

	if (playerIdIfNot0Minus1 == myPlayerIndex)
	{
		printf("~r~CTL: ~w~Why would you want to attach to your own vehicle?");
		return;
	}

	if (IS_CHAR_IN_ANY_CAR(target_ped))
	{
		disableFlymod();

		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			GET_CAR_CHAR_IS_USING(myPlayerPed, &our_veh);
			REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION(myPlayerPed, our_veh);
			WAIT(100);
		}

		if(!safeDetachLocalPlayer())
			printf("~r~CTL: ~w~Press ~PAD_Y~ to detach."); //only tell user they can hit Y on if they aren't already attached.

		GET_CAR_CHAR_IS_USING(target_ped, &attachVeh); //we must grab this as a global.
		GET_CAR_MODEL(attachVeh, &target_model);
		GET_MODEL_DIMENSIONS(target_model, &min, &targetModelMax);

		switch (*attach_index)
		{
		case 0: //in front
			ATTACH_PED_TO_CAR(myPlayerPed, attachVeh, 0, 0.0f, (targetModelMax.y + 0.3f), (targetModelMax.z / 2.0f), 0.0f, 0.0f, true, false);
			break;
		case 1: //On top
			ATTACH_PED_TO_CAR(myPlayerPed, attachVeh, 0, 0.0f, -0.4f, (targetModelMax.z + 0.95f), 0.0f, 0.0f, true, false); //.6 not enough trying .75
			break;
		case 2: //Behind
			ATTACH_PED_TO_CAR(myPlayerPed, attachVeh, 0, 0.0f, -(targetModelMax.y + 0.4f), (targetModelMax.z / 2.0f), 0.0f, 0.0f, true, false);
			break;
		case 3: //Left
			ATTACH_PED_TO_CAR(myPlayerPed, attachVeh, 0, -(targetModelMax.x + 0.25f), 0.0f, (targetModelMax.z / 2.0f), 0.0f, 0.0f, true, false);
			break;
		case 4: //Right
			ATTACH_PED_TO_CAR(myPlayerPed, attachVeh, 0, (targetModelMax.x + 0.25f), 0.0f, (targetModelMax.z / 2.0f), 0.0f, 0.0f, true, false);
			break;
		}

		netPlayerSpectating = -1; //stop spectate code executing and fucking us over
		GET_GAME_CAM(&playerCamera);
		//ATTACH_CAM_TO_VEHICLE(playerCamera, attachVeh);
		POINT_CAM_AT_VEHICLE(playerCamera, attachVeh);
		//SET_CAMERA_AUTO_SCRIPT_ACTIVATION(false);

		//SET_CAM_TARGET_PED(playerCamera, target_ped);
		//SET_IN_SPECTATOR_MODE(true);
	}
}

void VehOptAttachUniversalNeonsBlue(VEHICLE_INDEX vehicle) //this took me lots of trial and error you mf's - Nathan //CLEAN
{
	vector3 modelMin, modelMax, vehPos;
	OBJECT_INDEX tmpObject[3];
	uint model;

	ClearNeonCarObjects();

	if (GetMenuPlayerId() == myPlayerIndex)
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myNeonCar);

	GET_CAR_COORDINATES(vehicle, &vehPos.x, &vehPos.y, &vehPos.z);
	GET_CAR_MODEL(vehicle, &model);
	GET_MODEL_DIMENSIONS(model, &modelMin, &modelMax);

	for (int i = 0; i < 3; i++)
	{
		if(CreateObject(OBJECT_BM_AIR_LANDLITES03, vehPos.x, vehPos.y, (vehPos.z + 10.0f), 0.0f, (GetMenuPlayerId() == myPlayerIndex) ? &myNeonCarObjects[i] : &tmpObject[i], (GetMenuPlayerId() == myPlayerIndex)))
			SET_OBJECT_LIGHTS((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[i] : tmpObject[i]), true);
	}

	float zOffset = -(modelMax.z / 5.0f); //4.80 has been tested as okay, could do with them a lil higher for more of the light. //4.65 almost perfect..
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex) ? myNeonCarObjects[0] : tmpObject[0], vehicle, 0, 0.0f, (modelMax.y / 1.90f), zOffset, 4.530, 0.0f, 0.0f);
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex) ? myNeonCarObjects[1] : tmpObject[1], vehicle, 0, 0.0f, -(modelMax.y / 1.90f), zOffset, 4.530, 0.0f, 3.150f);
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex) ? myNeonCarObjects[2] : tmpObject[2], vehicle, 0, 0.0f, 0.4250f, zOffset, 4.530, 0.0f, 3.150f);
}

void attachBlueNeon() //CLEAN
{
	PED_INDEX targetPed;
	VEHICLE_INDEX targetVehicle;
	if(SafeToActOnPlayerPed(GetMenuPlayerId(),&targetPed))
	{
		GET_CAR_CHAR_IS_USING(targetPed, &targetVehicle);
		if(DOES_VEHICLE_EXIST(targetVehicle))
			VehOptAttachUniversalNeonsBlue(targetVehicle);
	}
}

void VehOptAttachUniversalNeonsRed(VEHICLE_INDEX vehicle) //this took me lots of trial and error you mf's - Nathan //CLEAN
{
	vector3 modelMin, modelMax, vehPos;
	OBJECT_INDEX tmpObject[3];
	uint model;

	ClearNeonCarObjects();

	if (GetMenuPlayerId() == myPlayerIndex)
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myNeonCar);

	GET_CAR_COORDINATES(vehicle, &vehPos.x, &vehPos.y, &vehPos.z);
	GET_CAR_MODEL(vehicle, &model);
	GET_MODEL_DIMENSIONS(model, &modelMin, &modelMax);

	for (int i = 0; i < 3; i++)
	{
		if(CreateObject(OBJECT_NJ5_INDLTR, vehPos.x, vehPos.y, (vehPos.z + 10.0f), 0.0f, (GetMenuPlayerId() == myPlayerIndex ? &myNeonCarObjects[i] : &tmpObject[i]), (GetMenuPlayerId() == myPlayerIndex)))
			SET_OBJECT_LIGHTS((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[i] : tmpObject[i]), true);
	}

	float zOffset = -(modelMax.z / 5.0f); //4.80 has been tested as okay, could do with them a lil higher for more of the light. //4.65 almost perfect..
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[0] : tmpObject[0]), vehicle, 0, 0.0f, (modelMax.y / 1.90f), zOffset, 4.530, 0.0f, 0.0f);
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[1] : tmpObject[1]), vehicle, 0, 0.0f, -(modelMax.y / 1.90f), zOffset, 4.530, 0.0f, 3.150f);
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[2] : tmpObject[2]), vehicle, 0, 0.0f, 0.4250f, zOffset, 4.530, 0.0f, 3.150f);
}
 
void attachRedNeon() //CLEAN
{
	PED_INDEX targetPed;
	VEHICLE_INDEX targetVehicle;

	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &targetPed))
	{
		GET_CAR_CHAR_IS_USING(targetPed, &targetVehicle);
		if(DOES_VEHICLE_EXIST(targetVehicle))
			VehOptAttachUniversalNeonsRed(targetVehicle);
	}
}

void VehOptAttachUniversalNeonsOrange(VEHICLE_INDEX vehicle) //this took me lots of trial and error you mf's - Nathan //CLEAN
{
	vector3 modelMin, modelMax, vehPos;
	OBJECT_INDEX tmpObject[3];
	uint model;

	ClearNeonCarObjects();

	if (GetMenuPlayerId() == myPlayerIndex)
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myNeonCar);

	GET_CAR_COORDINATES(vehicle, &vehPos.x, &vehPos.y, &vehPos.z);
	GET_CAR_MODEL(vehicle, &model);
	GET_MODEL_DIMENSIONS(model, &modelMin, &modelMax);

	for (int i = 0; i < 3; i++)
	{
		if(CreateObject(OBJECT_SEXSHOPNEON2, vehPos.x, vehPos.y, (vehPos.z + 10.0f), 0.0f, (GetMenuPlayerId() == myPlayerIndex ? &myNeonCarObjects[i] : &tmpObject[i]), (GetMenuPlayerId() == myPlayerIndex)))
			SET_OBJECT_LIGHTS((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[i] : tmpObject[i]), true);
	}

	float zOffset = -(modelMax.z / 4.0f); //4.80 has been tested as okay, could do with them a lil higher for more of the light. //4.65 almost perfect..
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[0] : tmpObject[0]), vehicle, 0, 0.0f, (modelMax.y / 1.90f), zOffset, 4.530, 0.0f, 0.0f);
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[1] : tmpObject[1]), vehicle, 0, 0.0f, -(modelMax.y / 1.90f), zOffset, 4.530, 0.0f, 3.150f);
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[2] : tmpObject[2]), vehicle, 0, 0.0f, 0.4250f, zOffset, 4.530, 0.0f, 3.150f);
}

void attachOrangeNeon() //CLEAN
{
	PED_INDEX targetPed;
	VEHICLE_INDEX targetVehicle;

	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &targetPed))
	{
		GET_CAR_CHAR_IS_USING(targetPed, &targetVehicle);
		if(DOES_VEHICLE_EXIST(targetVehicle))
			VehOptAttachUniversalNeonsOrange(targetVehicle);
	}
}

void VehOptAttachUniversalNeonsWhite(VEHICLE_INDEX vehicle) //this took me lots of trial and error you mf's - Nathan //CLEAN
{
	vector3 modelMin, modelMax, vehPos;
	OBJECT_INDEX tmpObject[3];
	uint model;

	ClearNeonCarObjects();

	if (GetMenuPlayerId() == myPlayerIndex)
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myNeonCar);

	GET_CAR_COORDINATES(vehicle, &vehPos.x, &vehPos.y, &vehPos.z);
	GET_CAR_MODEL(vehicle, &model);
	GET_MODEL_DIMENSIONS(model, &modelMin, &modelMax);

	for (int i = 0; i < 3; i++)
	{
		if(CreateObject(OBJECT_CJ_WALL_LIGHT_2, vehPos.x, vehPos.y, (vehPos.z + 10.0f), 0.0f, (GetMenuPlayerId() == myPlayerIndex) ? &myNeonCarObjects[i] : &tmpObject[i], (GetMenuPlayerId() == myPlayerIndex)))
			SET_OBJECT_LIGHTS((GetMenuPlayerId() == myPlayerIndex ? myNeonCarObjects[i] : tmpObject[i]), true);
	}

	float zOffset = -(modelMax.z / 5.0f); //4.80 has been tested as okay, could do with them a lil higher for more of the light. //4.65 almost perfect..
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex) ? myNeonCarObjects[0] : tmpObject[0], vehicle, 0, 0.0f, (modelMax.y / 1.90f), zOffset, 4.530, 0.0f, 0.0f);
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex) ? myNeonCarObjects[1] : tmpObject[1], vehicle, 0, 0.0f, -(modelMax.y / 1.90f), zOffset, 4.530, 0.0f, 3.150f);
	ATTACH_OBJECT_TO_CAR((GetMenuPlayerId() == myPlayerIndex) ? myNeonCarObjects[2] : tmpObject[2], vehicle, 0, 0.0f, 0.4250f, zOffset, 4.530, 0.0f, 3.150f);
}

void attachWhiteNeon() //CLEAN
{
	PED_INDEX targetPed;
	VEHICLE_INDEX targetVehicle;

	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &targetPed))
	{
		GET_CAR_CHAR_IS_USING(targetPed, &targetVehicle);
		if(DOES_VEHICLE_EXIST(targetVehicle))
			VehOptAttachUniversalNeonsWhite(targetVehicle);
	}
}

void VehOptRemoveHeliBlades() //CLEAN
{
	if (DOES_VEHICLE_EXIST(bladesHeli))
	{
		PED_INDEX driver;

		GET_DRIVER_OF_CAR(bladesHeli, &driver);
		if (DOES_CHAR_EXIST(driver))
			DeleteChar(&driver); //should be locked control to us but if this ends up having issues i will add a network ID control check. - Nathan.

		DeleteCar(&bladesHeli);
	}
}

void VehOptAttachHeliBlades() //CLEAN
{
	PED_INDEX bladesDriver;
	NETWORK_INDEX networkId;
	PED_INDEX playerPed;

	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &playerPed)) //only intended for use on myPlayerIndex.
	{
		VehOptRemoveHeliBlades();

		if (!SpawnVehicle(VEHICLE_MAVERICK, 0.0f, 0.0f, -10.0f, 0.0f, &bladesHeli, false, true))
			return;

		SET_CAR_CAN_BE_DAMAGED(bladesHeli, false);
		SET_CAR_CAN_BE_VISIBLY_DAMAGED(bladesHeli, false);
		GET_CAR_CHAR_IS_USING(playerPed, &bladesVehicle);
		ATTACH_CAR_TO_CAR(bladesHeli, bladesVehicle, false, 0.0f, IS_BIG_VEHICLE(bladesVehicle) ? 12.750f : 9.5398, IS_BIG_VEHICLE(bladesVehicle) ? -3.5f : -2.75f, 0.0f, 0.0f, 0.0f);
		LOCK_CAR_DOORS(bladesHeli, VEHICLE_DOOR_LOCKED_BOTH_SIDES);
		WAIT(100);
		CREATE_RANDOM_CHAR_AS_DRIVER(bladesHeli, &bladesDriver);
		GET_NETWORK_ID_FROM_PED(bladesDriver, &networkId);
		SET_NETWORK_ID_CAN_MIGRATE(networkId, false);
		SET_CHAR_INVINCIBLE(bladesDriver, true);
		SET_CAR_ENGINE_ON(bladesHeli, true, true);
		SET_CAR_COLLISION(bladesHeli, true);
		printf("~r~CTL: ~w~Heli blades attached. Credit: ~y~StuartB.");
	}
}

Vehicle GetPlayerVehicleDimensions(Player playerIndex, vector3 *modelMin, vector3 *modelMax) //CLEAN
{
	Ped playersPed;
	GET_PLAYER_CHAR(playerIndex, &playersPed);
	if (!DOES_CHAR_EXIST(playersPed))
	{
		printf("~r~CTL: ~w~Player has no character, can't proceed.");
		return 0;
	}
	if (!IS_CHAR_IN_ANY_CAR(playersPed))
	{
		printf("~r~CTL: ~w~Player isn't in a vehicle, can't proceed");
		return 0;
	}

	Vehicle playerPedVehicle;
	uint playerPedVehicleModel;
	GET_CAR_CHAR_IS_USING(playersPed, &playerPedVehicle);
	GET_CAR_MODEL(playerPedVehicle, &playerPedVehicleModel);
	GET_MODEL_DIMENSIONS(playerPedVehicleModel, modelMin, modelMax);

	return playerPedVehicle;
}

void CreateObjectAndAttachToVehicle(int model, Vehicle veh, float pX, float pY, float pZ, float rX, float rY, float rZ) //CLEAN
{
	Object attachmentObj;
	CreateObject(model, 0.0f, 0.0f, 0.0f, 0.0f, &attachmentObj, (GetMenuPlayerId() == myPlayerIndex)); //only lock control if our car :)
	ATTACH_OBJECT_TO_CAR(attachmentObj, veh, 0, pX, pY, pZ, rX, rY, rZ);
}

void netPlayerAttachToVehHippo() //CLEAN
{
	vector3 vehMin, vehMax;
	Vehicle netVeh = GetPlayerVehicleDimensions(GetMenuPlayerId(), &vehMin, &vehMax);
	if (!DOES_VEHICLE_EXIST(netVeh)) return;
	CreateObjectAndAttachToVehicle(OBJECT_CJ_HIPPO_BIN, netVeh, 0.0f, -0.10f, vehMax.z, 0.0f, 0.0f, 3.150f);
}

void netPlayerAttachToVehBSCaution() //CLEAN
{
	vector3 vehMin, vehMax;
	Vehicle netVeh = GetPlayerVehicleDimensions(GetMenuPlayerId(), &vehMin, &vehMax);
	if (!DOES_VEHICLE_EXIST(netVeh)) return;
	CreateObjectAndAttachToVehicle(OBJECT_CAUTION_SIGN, netVeh, 0.0f, -0.375f, (vehMax.z + 0.50f), 0.0f, 0.0f, 0.0f);
}

void netPlayerAttachToVehDick() //CLEAN
{
	vector3 vehMin, vehMax;
	Vehicle netVeh = GetPlayerVehicleDimensions(GetMenuPlayerId(), &vehMin, &vehMax);
	if (!DOES_VEHICLE_EXIST(netVeh)) return;
	CreateObjectAndAttachToVehicle(OBJECT_BM_KNOBELISK, netVeh, 0.0f, (vehMax.y - 0.1f), 0.0f, 1.575f, 0.0f, 3.150f);
}


void netPlayerAttachToVehAirportLamp() //CLEAN
{
	vector3 modelMin, modelMax;
	Vehicle netVeh = GetPlayerVehicleDimensions(GetMenuPlayerId(), &modelMin, &modelMax);
	if (!DOES_VEHICLE_EXIST(netVeh)) return;
	CreateObjectAndAttachToVehicle(OBJECT_BM_AIR_LAMP03, netVeh, 0.0f, -0.4857f, (modelMax.z - 0.15f), 0.0000f, 0.0000f, 0.0f);
}

void netPlayerAttachToVehUmbrella() 
{
	vector3 vehMin, vehMax;
	Vehicle netVeh = GetPlayerVehicleDimensions(GetMenuPlayerId(), &vehMin, &vehMax);
	if (!DOES_VEHICLE_EXIST(netVeh)) return;
	CreateObjectAndAttachToVehicle(485614386, netVeh, 0.0f, -0.4763f, (vehMax.z - 0.090f), 0.0000f, 0.0000f, 3.150f);
}

void toggleSelectedPlayerMute() 
{
	NETWORK_SET_PLAYER_MUTED(GetMenuPlayerId(), !NETWORK_IS_PLAYER_MUTED_BY_ME(GetMenuPlayerId()));
}

bool IsPlayerOnFriendsList(PLAYER_INDEX PlayerID, bool checkOnlineStatus) //if checkOnlineStatus and not in same title we'll assume spoof / not the actual friend.
{
	//const char* playerName = IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, (PlayerID + 16)) ? ctlMem->m_char_buffers.player_name_backup[PlayerID] : GET_PLAYER_NAME(PlayerID); //aliases back up name and players can hide them
	const char* playerName = ctlMem->m_char_buffers.player_name_backup[PlayerID][0] != '\0' ? ctlMem->m_char_buffers.player_name_backup[PlayerID] : GET_PLAYER_NAME(PlayerID); //aliases back up name and players can hide them
	int friendCount = NETWORK_GET_FRIEND_COUNT();
	for (int i = 0; i < friendCount; i++)
	{
		if (COMPARE_STRING(NETWORK_GET_FRIEND_NAME(i), playerName)) 
			return (checkOnlineStatus ? (NETWORK_IS_FRIEND_ONLINE(playerName) && NETWORK_IS_FRIEND_IN_SAME_TITLE(playerName)) : true);
			//return (checkOnlineStatus ? (NETWORK_IS_FRIEND_ONLINE(NETWORK_GET_FRIEND_NAME(i)) && NETWORK_IS_FRIEND_IN_SAME_TITLE(NETWORK_GET_FRIEND_NAME(i))) : true);
	}
	return false;
}

bool VoteKickPlayer(PLAYER_INDEX PlayerID)
{
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL:
		{
			if (getGlobalAtIndex(185) == PlayerID)
			{
				setGlobalAtIndex(185, -1);
				return false;
			}
			else
			{
				setGlobalAtIndex(185, PlayerID);
				return true;
			}
			//return getGlobalAtIndex(185) == PlayerID;
		}
	case EPISODE_TBOGT:
		{
			if (getGlobalAtIndex(813) == PlayerID)
			{
				setGlobalAtIndex(813, -1);
				return false;
			}
			else
			{
				setGlobalAtIndex(813, PlayerID);
				return true;
			}
			//return getGlobalAtIndex(813) == PlayerID;
		}
	}
	/*else if (myCurrentEpisode == EPISODE_TLAD)
	{

	}*/
	return false;
}

void NetworkPlayerKickNathan(int unused1, int unused2)
{
	//ifdef mod if cartel user is host send admin cmd console kick or whatever etc.
	if (GET_HOST_ID() == myPlayerIndex)
	{
#ifndef Developer
		if (IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, GetMenuPlayerId()) || IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, GetMenuPlayerId() + 16))
			return;
#endif
		bool kicked_console;
		uint kicked_array_index;
		if (!IsPlayerUsingXboxOne(GetMenuPlayerId()))
		{
			if (AddPlayerToKickStorage(GetMenuPlayerId(), &kicked_array_index))
			{
				printf("~r~CTL: ~w~Kicked %s ~w~and scanning for their identifiers.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
				kicked_console = true;
			}
		}
		NETWORK_KICK_PLAYER(GetMenuPlayerId()); //make sure we kick them before asking for keyb input and fucking everything lol.

		if (kicked_console)
		{
			if (showAndGetKeyboardInput("Provide reason (Console kick)", false, "", 25, 0, 0.5f, 0.5f, 200, 0, 0))
				strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[kicked_array_index].kickedReason, ctlMem->m_char_buffers.keyboard_input_buffer, 26);
		}
	}
	else
	{
#ifdef Moderator
		if (GetCartelUserType(GET_HOST_ID()) != CARTEL_NON_USER)
		{
			SendAdminCommandToPlayer( GetMenuPlayerId() + (IsPlayerUsingXboxOne(GetMenuPlayerId()) ? ADMIN_CMD_KICK_PLAYER_BASE_ID_0 : ADMIN_CMD_KICK_CONSOLE_BASE_ID_0), GET_HOST_ID() + 1);
		}
		else
#endif
		if (!VoteKickPlayer(GetMenuPlayerId()))
			printf("~r~CTL: ~w~Cancelled vote to kick %s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
	}
}


void netPlayerBurn(int unused1, int unused2)
{
	PED_INDEX playerPed;
	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &playerPed))
	{
		if (GetMenuPlayerId() == myPlayerIndex)
		{
			bool Godmode = bitTest(ctlMem->m_bit_sets.m_player, playerGodMode);
			SET_CHAR_PROOFS(playerPed, Godmode, false, Godmode, Godmode, Godmode); //allow setting self on fire.
		}
		START_CHAR_FIRE(playerPed);
		printf("~r~CTL: ~w~Buurn %sy ~w~burn!", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
	}
}

void playerExtinguish() 
{
	EXTINGUISH_CHAR_FIRE(myPlayerPed);
	bool Godmode = bitTest(ctlMem->m_bit_sets.m_player, playerGodMode);
	SET_CHAR_PROOFS(myPlayerPed, Godmode, Godmode, Godmode, Godmode, Godmode);
}

void netPlayerExplode(int unused1, int unused2)
{
	PED_INDEX playerPed;
	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &playerPed))
	{
		vector3 ped;
		GET_CHAR_COORDINATES(playerPed, &ped.x, &ped.y, &ped.z);
		ADD_EXPLOSION(ped.x, ped.y, ped.z, EXPLOSION_SHIP_DESTROY, 10.0, true, false, 1.0f);

		if (GetMenuPlayerId() != myPlayerIndex)
			printf("~r~CTL: ~w~Aaand boom goes the %s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
	}
}

void netPlayerRamWithVehicle(uint version, bool usePlayerVeh)
{
	if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		if (!usePlayerVeh)
			REQUEST_MODEL(VEHICLE_BUS);

		Vehicle myVehicle;

		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			GET_CAR_CHAR_IS_USING(myPlayerPed, &myVehicle);
		}
		else
		{
			if (usePlayerVeh)
			{
				printf("~r~CTL: ~w~Where's your vehicle..?");
				return;
			}
		}

		Ped netPed;
		GET_PLAYER_CHAR(GetMenuPlayerId(), &netPed);
		float rX, rY, rZ, rH;
		Vehicle ramVehicle;

		GET_CHAR_HEADING(netPed, &rH);

		if (version == 1)
			GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(netPed, 0.0f, 30.0f, 0.0f, &rX, &rY, &rZ);
		else
		{
			//GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(netPed, 0.0f, 0.0f, 30.0f, &rX, &rY, &rZ);
			GET_CHAR_COORDINATES(netPed, &rX, &rY, &rZ);
			rZ += 30.0f;
		}

		if (usePlayerVeh)
		{
			ramVehicle = myVehicle;
			SET_CAR_COORDINATES(ramVehicle, rX, rY, rZ);
			SET_CAR_HEADING(ramVehicle, (rH - 180.0f));
		}
		else
		{
			if (!SpawnVehicle(VEHICLE_BUS, rX, rY, rZ, (rH - 180.0f), &ramVehicle, true, true))
				return;

			SET_PETROL_TANK_HEALTH(ramVehicle, -1.0f);
		}
		
		
		switch (version)
		{
		case 1:
		{
			GET_CHAR_HEADING(netPed, &rH);
			SET_CAR_HEADING(ramVehicle, (rH - 180.0f));
			SET_CAR_ON_GROUND_PROPERLY(ramVehicle);
			SET_CAR_FORWARD_SPEED(ramVehicle, 260.0f); //was 280
		}
		break;
		case 2:
		{
			SET_VEHICLE_QUATERNION(ramVehicle, 0.7071, 0, 0, 0.7071);
			SET_CAR_FORWARD_SPEED(ramVehicle, 260.0f); //was 280
		}
		break;
		case 3:
		{
			APPLY_FORCE_TO_CAR(ramVehicle, 0, 0.0, 0.0, -1250.0f, 0.0, 0.0, 0.0, 0, true, true, true); 
		}
		break;
		}

		if(!usePlayerVeh)
			MARK_CAR_AS_NO_LONGER_NEEDED(&ramVehicle);
	}
	else
		printf("~r~CTL: ~w~Player left or doesn't have a ped, can't proceed.");
}

void netPlayerTaser() 
{
	PED_INDEX playerPed;
	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &playerPed))
	{
		vector3 ped;
		GET_CHAR_COORDINATES(playerPed, &ped.x, &ped.y, &ped.z);
		if (GET_HOST_ID() != myPlayerIndex)
		{
			ADD_EXPLOSION(ped.x, ped.y, ped.z, EXPLOSION_DIR_WATER_HYDRANT, 5.0, false, true, 0.0f);
			ADD_EXPLOSION(ped.x + 2.0, ped.y, ped.z, EXPLOSION_DIR_WATER_HYDRANT, 5.0, false, true, 0.0f);
			ADD_EXPLOSION(ped.x, ped.y + 2.0, ped.z, EXPLOSION_DIR_WATER_HYDRANT, 5.0, false, true, 0.0f);
			ADD_EXPLOSION(ped.x - 2.0, ped.y, ped.z, EXPLOSION_DIR_WATER_HYDRANT, 5.0, false, true, 0.0f);
			ADD_EXPLOSION(ped.x, ped.y - 2.0, ped.z, EXPLOSION_DIR_WATER_HYDRANT, 5.0, false, true, 0.0f);
			if (GetMenuPlayerId() != myPlayerIndex)
				printf("~r~CTL: %s ~w~just received some extra Giga-Whatz.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
		}
		else printf("~r~CTL: ~w~You cannot taze players as host.");
	}
}

void SpawnPlayerInCloneOfVehicle(Vehicle vehicle)
{
	bool driving = false;
	PED_INDEX driver;
	VEHICLE_INDEX clonedVeh, myCar;
	vector3 myPos, modelMin, modelMax;
	uint model;
	float mH;
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myCar);
		GET_DRIVER_OF_CAR(myCar, &driver);
		driving = (driver == myPlayerPed);
		if (driving)
		{
			GET_CHAR_COORDINATES(myPlayerPed, &myPos.x, &myPos.y, &myPos.z);
		}
		else
		{
			GET_CAR_MODEL(myCar, &model);
			GET_MODEL_DIMENSIONS(model, &modelMin, &modelMax);
			GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(myCar, 0.0f, -(modelMax.y + 2.0f), 0.0f, &myPos.x, &myPos.y, &myPos.z);
		}
	}
	else
		GET_CHAR_COORDINATES(myPlayerPed, &myPos.x, &myPos.y, &myPos.z);

	GET_CHAR_HEADING(myPlayerPed, &mH);

	if (CloneVehicle(vehicle, myPos.x, myPos.y, myPos.z, mH, &clonedVeh))
	{
		if (driving)
			DELETE_CAR(&myCar);

		WARP_CHAR_INTO_CAR(myPlayerPed, clonedVeh);
	}
}

void SetPedsAppearanceToPlayers(Player playerIndex) //Only called on other players and if they leave then menu backs out
{
	if (playerIndex == myPlayerIndex) //you will crash
		return;

	if (PLAYER_HAS_CHAR(playerIndex))
	{
		PED_INDEX playerPed;
		GET_PLAYER_CHAR(playerIndex, &playerPed);

		uint tmpModel;
		GET_CHAR_MODEL(playerPed, &tmpModel);

		changePlayerModel(tmpModel, 0); //swap our model to theirs
		if(DOES_CHAR_EXIST(playerPed)) //small chance they leave while we are WAITING for model to load/change and we get fucked..
		{
			for (int i = 0; i < 11; i++)
				SET_CHAR_COMPONENT_VARIATION(myPlayerPed, i, GET_CHAR_DRAWABLE_VARIATION(playerPed, i), GET_CHAR_TEXTURE_VARIATION(playerPed, i)); //swap all of our clothes + texture choices to theirs

			int targets_prop_index;
			for (int i = 0; i < 2; i++) //swap all our props to theirs 
			{
				GET_CHAR_PROP_INDEX(playerPed, i, &targets_prop_index);
				SET_CHAR_PROP_INDEX(myPlayerPed, i, targets_prop_index);
				//if get issues only run below if targets_prop_index != -1
				SET_CHAR_PROP_INDEX_TEXTURE(myPlayerPed, i, targets_prop_index, 1); //not accurate but sets them to the correct colour now sometimes instead of always wrong (There is no GET_PROP_INDEX_TEXTURE);
			}
			SET_ANIM_GROUP_FOR_CHAR(myPlayerPed, GET_ANIM_GROUP_FROM_CHAR(playerPed)); //set our anim group to theirs incase they changed it with a menu.
		}
	}
}

/*
void SetPedsAppearanceToPlayers(Player playerIndex) //OLD/BACKUP 19-Jan-2026
{
	if (playerIndex == myPlayerIndex) //you will crash
		return;

	
	
	PED_INDEX playerPed;
	if (SafeToActOnPlayerPed(playerIndex, &playerPed))
	{
		uint tmpModel;
		const char* animGroup = GET_ANIM_GROUP_FROM_CHAR(playerPed);

		REQUEST_ANIMS(animGroup);
		GET_CHAR_MODEL(playerPed, &tmpModel);
		changePlayerModel(tmpModel, 0);
		uint DrawableVariations[11], TextureVariations[11];
		int PropIndexs[2];
		for (int i = 0; i < 11; i++)
		{
			DrawableVariations[i] = GET_CHAR_DRAWABLE_VARIATION(playerPed, i);
			TextureVariations[i] = GET_CHAR_TEXTURE_VARIATION(playerPed, i); //I'm aware this isn't required, but it's less likely to freeze this way.
		}

		//for (int i = 0; i < 11; i++)
			//SET_CHAR_COMPONENT_VARIATION(myPlayerPed, i, DrawableVariations[i], TextureVariations[i]);

		for (int i = 0; i < 2; i++)
		{
			GET_CHAR_PROP_INDEX(playerPed, i, &PropIndexs[i]);
		//	SET_CHAR_PROP_INDEX_TEXTURE(playerPed,i,PropIndexs[i], texture???)
		}

		CLEAR_ALL_CHAR_PROPS(myPlayerPed);


		for (int i = 0; i < 2; i++)
		{
			SET_CHAR_PROP_INDEX(myPlayerPed, i, PropIndexs[i]);
		//	SET_CHAR_PROP_INDEX_TEXTURE(myPlayerPed)
			//SET_PED_HELMET_TEXTURE_INDEX
			//GET_CHAR_TEXTURE_VARIATION
		}

		for (int i = 0; i < 11; i++)
			SET_CHAR_COMPONENT_VARIATION(myPlayerPed, i, DrawableVariations[i], TextureVariations[i]);

		while (!HAVE_ANIMS_LOADED(animGroup)) WAIT(0);
		SET_ANIM_GROUP_FOR_CHAR(myPlayerPed, animGroup); 

		FORCE_FULL_VOICE(myPlayerPed);
	}
}
*/

void CopyNetPlayer() //CLEAN
{
	if (PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		switch (CopyPlayerSelect)
		{
		case 0: SetPedsAppearanceToPlayers(GetMenuPlayerId()); return;
		case 1:
		{
			PED_INDEX playerPed;
			VEHICLE_INDEX playerVeh;
			GET_PLAYER_CHAR(GetMenuPlayerId(), &playerPed);
			GET_CAR_CHAR_IS_USING(playerPed, &playerVeh);
			SpawnPlayerInCloneOfVehicle(playerVeh); //safety checks inside this function.
			return;
		}
		/*
		case 2:
		{
			PED_INDEX playerPed;
			GET_PLAYER_CHAR(GetMenuPlayerId(), &playerPed);
			
			COPY_ANIMATIONS(myPlayerPed, playerPed, 0.0f); //needs looping + doesn't sync
			return;
		}
		*/
		}
	}
	else
		printf("~r~CTL: %s ~w~doesn't have a character, can't proceed.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
	/*
	if (CopyPlayerSelect == 0)
	{
		SetPedsAppearanceToPlayers(GetMenuPlayerId());
	}
	else
	{
		PED_INDEX playerPed;
		if (SafeToActOnPlayerPed(GetMenuPlayerId(), &playerPed))
		{
			Vehicle netVeh;
			GET_CAR_CHAR_IS_USING(playerPed, &netVeh);
			SpawnPlayerInCloneOfVehicle(netVeh);
		}
	}
	*/
}

void PrintClientId() //CLEAN
{
	PRINT_WITH_NUMBER_NOW("NUMBER", GetMenuPlayerId(), 2500, true);
}

void netPlayerVehDetach() 
{
	if (IS_IN_SPECTATOR_MODE())
	{
		Camera playerCamera;
		GET_GAME_CAM(&playerCamera);
		SET_CAM_TARGET_PED(playerCamera, myPlayerPed);
		SET_IN_SPECTATOR_MODE(false);
	}
	if (DOES_VEHICLE_EXIST(attachVeh))
	{
		DETACH_PED(myPlayerPed, true);
		vector3 vehMin, vehMax;
		uint model;
		int rand;
		GENERATE_RANDOM_INT(&rand);
		GET_CAR_MODEL(attachVeh, &model);
		GET_MODEL_DIMENSIONS(model, &vehMin, &vehMax);
		float dX, dY, dZ;
		GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(attachVeh, (rand % 2 == 1) ? -(vehMax.x + 1.5f) : (vehMax.x + 1.5f), 0.0f, 0.0f, &dX, &dY, &dZ);
		SET_CHAR_COORDINATES_NO_OFFSET(myPlayerPed, dX, dY, dZ);
		GET_CAR_HEADING(attachVeh, &dZ);
		SET_CHAR_HEADING(myPlayerPed, dZ); //heading might already be the same, have to test this might not be needed
	}
	attachVeh = 0;
}

void SpawnMoneyOnPed(Ped playerPed, uint amount) //NO SAFETY CHECKS HERE (DO THEM EXTERNALLY) 
{
	if (GET_HOST_ID() == myPlayerIndex && NETWORK_IS_GAME_RANKED())
	{
		if (GET_CHAR_MONEY(playerPed) > 1000000000) //already high enough for rank 10  //always returns 0 if not host.
			return;

		SET_CHAR_MONEY(playerPed, (GET_CHAR_MONEY(playerPed) + amount)); //could try the SetHostByte on this to make life easier..
		return;
	}

	if (IS_CHAR_ON_FOOT(playerPed))
	{
		Pickup money;
		vector3 ped, spawn;
		float speed;

		GET_CHAR_SPEED(playerPed, &speed);
		if (speed < 1.0f)
			GET_CHAR_COORDINATES(playerPed, &ped.x, &ped.y, &ped.z);
		else
			GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(playerPed, 0.0f, 2.0f, 2.0f, &ped.x, &ped.y, &ped.z); //in front and up high..

		//I will try without the safe pickup coords at some point, probably defeats the purpose of the above.
		GET_SAFE_PICKUP_COORDS(ped.x, ped.y, ped.z, &spawn.x, &spawn.y, &spawn.z);
		CREATE_MONEY_PICKUP(spawn.x, spawn.y, spawn.z, amount, true, &money);
	}
}

bool AreCoordinatesNearKillzone(float x, float y, float z)
{
	return (x > 3560.0f || x < -3240.0f || y > 3400.0f || y < -3400.0f || z > 4050.0f || z < -130.0f); //-130 z not killzone but useless coords so
}

void ToggleDriveWaterVisibility(int unused, int unused2) { bitFlip(&ctlMem->m_bit_sets.m_player, driveWaterVisible); }

void WalkWaterToggleScan(PLAYER_INDEX PlayerID) //make this take playerindex so we can also call it when enabling on all players.
{
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_removeWeapons_driveWater, (PlayerID + 16))) //player has just switched this on.
	{
		PLAYER_INDEX Controller;
		OBJECT_INDEX scan_obj;
		//below command skips our own driveWalkWaterObjects and walkOnAirObjects (incase player is on anothers we spawned)
		if (ScanNetworkObjectPoolForClosestModelToPlayer(PlayerID, OBJECT_NJ05_OILTNKBIG, true, true, 23.0f, true, &Controller, &scan_obj, 0.0f))
		{
			CLEAR_BIT(&ctlMem->m_bit_sets.m_removeWeapons_driveWater, (PlayerID + 16)); //think this is okay as helper.sco won't run unless object exists.
			if (IsPlayerValid(Controller) && Controller != PlayerID)
			{
				//char* ControllerName = NetworkPlayerGetNameWithColour(Controller);
				char* buffer = BUFFER128;
				strcpy(buffer, "~r~CTL: ", 128);
				stradd(buffer, NetworkPlayerGetNameWithColour(PlayerID), 128);
				stradd(buffer, " ~w~has water object from ", 128);
				stradd(buffer, NetworkPlayerGetNameWithColour(Controller), 128);
				stradd(buffer, ".", 128);
				printf(buffer);
				//printf("~r~CTL: %s ~w~has water object from %s.", NetworkPlayerGetNameWithColour(PlayerID), ControllerName);
			}
			else
				printf("~r~CTL: %s ~w~already has a water object.", NetworkPlayerGetNameWithColour(PlayerID));

			//SET_OBJECT_ALPHA ? So as you see it temporarily but immediately fades to invisible again....
		}
		else
		{
			CreateObject(OBJECT_NJ05_OILTNKBIG, 0.0f, 0.0f, -80.0f, 0.0f, &freeMem->m_walk_water_object[PlayerID], true); //prevent it pausing cartel helper to spawn this that will pick up this loop.
			SET_OBJECT_HEALTH(freeMem->m_walk_water_object[GetMenuPlayerId()], (TO_FLOAT(GetMenuPlayerId()) + 250.0f));
			//SET_OBJECT_
		}
	}
	else
	{
		if (DOES_OBJECT_EXIST(freeMem->m_walk_water_object[PlayerID]))
		{
			if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
			{
				PED_INDEX targetPed;
				float pX, pY, pZ, pH;
				GET_PLAYER_CHAR(GetMenuPlayerId(), &targetPed);
				GET_CHAR_COORDINATES(targetPed, &pX, &pY, &pZ);
				if (COMPARE_STRING(GET_NAME_OF_ZONE(pX, pY, pZ), "LIBERTY")) //out at sea
				{
					float destX, destY, destZ;
					if (GET_CLOSEST_CAR_NODE(pX, pY, pZ, &destX, &destY, &destZ))
					{
						//GET_SAFE_POSITION_FOR_CHAR
						GET_CHAR_HEADING(targetPed, &pH);
						if (IS_CHAR_ON_FOOT(targetPed))
						{
							//this does get called by TeleportNetworkPlayerToCoordinate however this way we don't wait for teleport on foot success.. who cares, if they got mods they can fix themselves.
							TeleportNetworkPlayerOnFoot(GetMenuPlayerId(), destX, destY, destZ + 1.0f, pH, false);
						}
						else
						{ //already auto-disables if they get into a boat or heli.
							ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
							printf("~r~CTL: ~w~Warping %s ~w~back to shore.", NetworkPlayerGetNameWithColour(GetMenuPlayerId())); //only notify if there's likely to be a wait.
							TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), destX, destY, (destZ + 0.5f), pH);
						}
					}
				}
			}
			DeleteObject(&freeMem->m_walk_water_object[PlayerID]);
		}
	}
}

void WalkWaterToggleScanSelectedPlayerId(int unused1, int unused2)
{
	WalkWaterToggleScan(GetMenuPlayerId());
}


void netPlayerToggles() 
{
	
	SetHeader("Give mods");
	AddItemBool("Vehicle auto-flip", &ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (GetMenuPlayerId() + 16), nullptr, "Set vehicle upright when upside-down.");
	AddItemBool("Infinite ammo", &ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, GetMenuPlayerId(), nullptr, nullptr);
	AddItemBoolAlt("Drive/Walk on water", &ctlMem->m_bit_sets.m_removeWeapons_driveWater, (GetMenuPlayerId() + 16), WalkWaterToggleScanSelectedPlayerId, "~PAD_X~ = Toggle cartel water objects visibility.", ToggleDriveWaterVisibility, nullptr);
	AddItemMenu("Walk on air", WorldAirOptions, "Also works as an elevator!");
	AddItemBool("Vehicle auto-fix", &ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, (GetMenuPlayerId() + 16), nullptr, "Fix vehicle when it catches fire or dies.");
	AddItemBool("Vehicle horn speed-boost", &ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, GetMenuPlayerId(), nullptr, "Speed-boost player when they sound their horn.");
	AddItemBool("Infinite armour", &ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, GetMenuPlayerId(), nullptr, nullptr);
	AddItemBool("Instant revive", &ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, (GetMenuPlayerId() + 16), nullptr, "Instantly respawn player when on-foot & dead.");
	AddItemBool("Control monitor & auto-unlocker", &ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, GetMenuPlayerId(), nullptr, "Know when a modder messes with their vehicle.");
	//if police enabled.
	AddItemBool("Never Wanted", &ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, GetMenuPlayerId(), nullptr, "Clear players wanted level when greater than 0");
	AddItemBool(NETWORK_IS_GAME_RANKED() ? "Rank 10" : "Drop money", &ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, GetMenuPlayerId(), nullptr, "Constantly drop money, handy in ranked matches.");
	AddItemBool("Vehicle protection", &netPlayerVehicleProtectionBitSet, GetMenuPlayerId(), nullptr, "Restricts network control to driver (Will break actionpads).");
	AddItemBool("Remove weapons", &ctlMem->m_bit_sets.m_removeWeapons_driveWater, GetMenuPlayerId(), nullptr, "Constantly remove weapons.");
}

void nathNeonz() 
{
	
	SetHeader("Universal neons");
	AddItem("Blue neons", attachBlueNeon, nullptr);
	AddItem("Red neons", attachRedNeon, nullptr);
	AddItem("Orange neons", attachOrangeNeon, nullptr);
	AddItem("White neons", attachWhiteNeon, nullptr);
	//Delete objects on vehicle.
	AddItemBool((GetMenuPlayerId() == myPlayerIndex) ? "Delete last" : "Delete current attachments", &ctlMem->m_bit_sets.m_vehicle, deleteLastNeonVehObjects, nullptr, (GetMenuPlayerId() == myPlayerIndex) ? "Automatically delete neons attached to previous vehicle." : "Delete current objects before attaching neons.");
}

void netPlayerAttachVehObjs() 
{
	
	SetHeader("Vehicle attachments");
	AddItemMenu("Universal neons", nathNeonz, "Attach neon objects based on vehicle size.");

	if (GetMenuPlayerId() == myPlayerIndex)
		AddItemAlt("Attach ground blades", VehOptAttachHeliBlades, "~PAD_A~ = Attach, ~PAD_X~ = Remove.", VehOptRemoveHeliBlades, nullptr);

	AddItem("Hippo on roof", netPlayerAttachToVehHippo, "Why not");
	AddItem("Caution sign on roof", netPlayerAttachToVehBSCaution, "Bullshit caution sign facing drivers screen.");
	AddItem("Dick on bumper", netPlayerAttachToVehDick, "Need I explain?");
	AddItem("Umbrella", netPlayerAttachToVehUmbrella, "For those sunny days.");
	AddItem("Airport lamp post", netPlayerAttachToVehAirportLamp, "Light for days.");
	//delete objects desired
}

void netPlayerVehOpts() 
{
	static int attachToVehSelect;

	SetHeader("Network vehicle");
	
	ctlMem->m_my_veh_data.currentLock = 1; //will return unlocked car if on foot.
	if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		Ped networkPlayerPed;
		GET_PLAYER_CHAR(GetMenuPlayerId(), &networkPlayerPed);
		AddItemMenu("Spawn vehicle", SpawnVehicleSetup, "Spawn the selected player a vehicle.");
		if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
		{
			AddItem("Clone vehicle", netPlayerCloneVeh, "Clone the selected players vehicle.");
			Vehicle networkPlayerVehicle;
			GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
			GET_CAR_DOOR_LOCK_STATUS(networkPlayerVehicle, (uint*)&ctlMem->m_my_veh_data.currentLock);
			ctlMem->m_my_veh_data.itemSelect = DoorLockToItemSelect(ctlMem->m_my_veh_data.currentLock);
			AddItemChar("Attach self to vehicle", false, attachToVehStrings, 5, &attachToVehSelect, netPlayerVehAttachTo, "~PAD_A~ = Attach, ~PAD_Y~ = Detach.");
			AddItemMenu("Attach objects to vehicle", netPlayerAttachVehObjs, "Universal neons, & random objects.");
			AddItemWithParam("Flip", VEH_ACT_FLIP, ProcessVehicleAction, "Set vehicle on ground properly.");
			AddItemAlt("Fix", VehOptFixNormal, "~PAD_A~ = Fix, ~PAD_X~ = Fix + Try setting invincible.", VehOptFixInvincibility, nullptr);
			AddItemWithParam("Clean", VEH_ACT_CLEAN, ProcessVehicleAction, "BANG; and the dirt is gone!");
			//AddItem("Clean", VehOptClean, "BANG; and the dirt is gone!");
			AddItemAlt(ctlMem->m_commonStrings[GXT_STRING_DELETE], VehOptDelete, "~PAD_A~ = Vehicle, ~PAD_X~ = Objects attached.", VehOptDeleteObjects, nullptr); //"Delete"
			AddItemChar("Set door lock", false, DoorLockString, 4, (int*)&ctlMem->m_my_veh_data.itemSelect, DoorLocker, "Lock vehicle doors accordingly.");
			AddItemMenu("Questionable actions", VehOptsQuestionable, "Everybody loves crushing bastards!");
		}
	}
	else
		SetCurrentMenuInvalid(true, "Player has no character/isn't present.");
}

void netPlayerSnipeHead()
{
	//spawn ped a little offset from char, aim at char head / shoot..
	//could probably even get away with spawning the peds locally only to do the job.
	if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		//SetHostByte(true);
		Ped netPed;
		GET_PLAYER_CHAR(GetMenuPlayerId(), &netPed);
		SetHostByte(true);
		EXPLODE_CHAR_HEAD(netPed);
		REGISTER_KILL_IN_MULTIPLAYER_GAME(myPlayerIndex, GetMenuPlayerId(), WEAPON_SNIPERRIFLE);
		SetHostByte(GET_HOST_ID() == myPlayerIndex);
	}
}

void netPlayerQuestionableActions() //to be callable by player options too.. //CLEAN
{
	
	SetHeader("Player -> Questionable");
	if (GetMenuPlayerId() == myPlayerIndex)
	{
		AddItemAlt("Burn", netPlayerBurn, "~PAD_A~ = Light, ~PAD_X~ = Extinguish.", playerExtinguish, nullptr);
	}
	else
	{
		AddItem("Burn", netPlayerBurn, "Set this mf alight."); //could maybe extinguish with the fire hydrant water effect
	}
	AddItem("Explode", netPlayerExplode, "Create a yuge explosion on player.");
	AddItem("Snipe head", netPlayerSnipeHead, nullptr);

	AddItemWithTwoParams("RamBus", 1, false, netPlayerRamWithVehicle, "Send a speeding bus in this players direction!");
	AddItemWithTwoParams("RamBus v2", 2, false, netPlayerRamWithVehicle, "Make a bus fall out of the sky aiming at the player!");
	AddItemWithTwoParams("RamBus v3", 3, false, netPlayerRamWithVehicle, "Squash the player like a bug using a bus in the sky!");
	AddItemWithTwoParams("Ram", 1, true, netPlayerRamWithVehicle, "Teleport in front of and drive at high speed at the player!");
	AddItemWithTwoParams("Ram v3", 3, true, netPlayerRamWithVehicle, "Squash the player like a bug with your vehicle.");

	//AddItem("Attach knobelisk to player");
	if (GET_HOST_ID() != myPlayerIndex)
		AddItem("Taser", netPlayerTaser, "Take 'em to the floor.");

}

#pragma clang diagnostic pop

void SetAllPlayerBitsInRange(int *bitSet, bool set, bool plusSixteen)
{
	//SET_BITS_IN_RANGE doesn't seem to do what  i think it does.. //attempt 2 below works nicely.
	uint offAmount = (plusSixteen ? 16 : 0);
	bool excludeSelf = bitTest(ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf);
	for (int i = offAmount; i < (offAmount + 16); i++)
	{
		if ((i - offAmount) == myPlayerIndex && excludeSelf) 
			continue;
		if (set)
			SET_BIT(&*bitSet, i);
		else
			CLEAR_BIT(&*bitSet, i);
	}

	if(excludeSelf)
		printf("~r~CTL: ~w~Turned %s ~y~%s ~w~for all players, except me.", Container.Item[Container.CursorIndex].Ui.ItemText, ctlMem->m_commonStrings[GXT_STRING_OFF + set]);
	else
		printf("~r~CTL: ~w~Turned %s ~y~%s ~w~for all players.", Container.Item[Container.CursorIndex].Ui.ItemText, ctlMem->m_commonStrings[GXT_STRING_OFF + set]);

}

void netPlayerAllPlayersInfiniteAmmoTrue() {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend,  1, false); }
void netPlayerAllPlayersInfiniteAmmoFalse() {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, 0, false); }

void netPlayerAllPlayersSetWalkOnWaterFalse() {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_removeWeapons_driveWater, 0, true); }
void netPlayerAllPlayersSetWalkOnWaterTrue()
{
	bool excludeSelf = bitTest(ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf);
	for (int i = 0; i < 16; i++)
	{
		if (excludeSelf && i == myPlayerIndex) continue;
		SET_BIT(&ctlMem->m_bit_sets.m_removeWeapons_driveWater, (i + 16));
		WalkWaterToggleScan(i); //will do nothing if unsafe. //will clear bit/disable before loop spawns object if player already has a walkOnWater object.
	}
	if (excludeSelf)
		printf("~r~CTL: ~w~Turned %s ~y~On ~w~for all players, except me.", Container.Item[Container.CursorIndex].Ui.ItemText);
	else
		printf("~r~CTL: ~w~Turned %s ~y~On ~w~for all players.", Container.Item[Container.CursorIndex].Ui.ItemText);
}
//walk on air.
void netPlayerAllPlayersSetNeverWantedTrue(int fuck, int me) {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, 1, false); }
void netPlayerAllPlayersSetNeverWantedFalse(int fuck, int me) {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, 0, false); }
void netPlayerAllPlayersSetVehicleAutoFixTrue(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, 1, true); }
void netPlayerAllPlayersSetVehicleAutoFixFalse(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, 0, true); }
void netPlayerAllPlayersSetInfiniteArmourTrue(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_infiniteArmour_instantRevive,  1, false); }
void netPlayerAllPlayersSetInfiniteArmourFalse(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_infiniteArmour_instantRevive,  0, false); }
void netPlayerAllPlayersSetInstantReviveTrue(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, 1, true); ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false); printf("~r~CTL: ~w~Be wary with this, some people don't want mods."); }
void netPlayerAllPlayersSetInstantReviveFalse(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, 0, true); }
void netPlayerAllPlayersSetVehicleMonitorTrue(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser,  1, false); }
void netPlayerAllPlayersSetVehicleMonitorFalse(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, 0, false); }
void netPlayerAllPlayersSetDropMoneyTrue(int fuck, int me) {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, 1, false); }
void netPlayerAllPlayersSetDropMoneyFalse(int fuck, int me) {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip,  0, false); }
void netPlayerAllPlayersVehicleAutoFlipTrue(int fuck, int me) {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, 1, true); }
void netPlayerAllPlayersVehicleAutoFlipFalse(int fuck, int me) {	SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, 0, true); }
void netPlayerAllPlayersHornSpeedBoostTrue(int fuck, int me) {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, 1, false); }
void netPlayerAllPlayersHornSpeedBoostFalse(int fuck, int me) {		SetAllPlayerBitsInRange(&ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, 0, false); }


void netPlayerAllPlayerToggles()
{
	//CONSIDER RESTRICTING SOME OF THESE TO NON RANKED MATCHES ONLY! - Nath
	//maybe add a bit , excludeSelfFromAllPlayerToggles
	SetHeader("All players -> Toggles");
	AddItemBool("Exclude self", &ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf, nullptr, "Exclude self from all player toggles.");
	AddItemAlt("Vehicle auto-flip", netPlayerAllPlayersVehicleAutoFlipTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersVehicleAutoFlipFalse, nullptr);
	AddItemAlt("Infinite ammo", netPlayerAllPlayersInfiniteAmmoTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersInfiniteAmmoFalse, nullptr);
	AddItemAlt("Drive/Walk on water", netPlayerAllPlayersSetWalkOnWaterTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off, ~PAD_Y~ = Toggle object visibility.", netPlayerAllPlayersSetWalkOnWaterFalse, ToggleDriveWaterVisibility);
	AddItemAlt("Vehicle auto-fix", netPlayerAllPlayersSetVehicleAutoFixTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersSetVehicleAutoFixFalse, nullptr);
	AddItemAlt("Vehicle horn speed-boost", netPlayerAllPlayersHornSpeedBoostTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersHornSpeedBoostFalse, nullptr);
	AddItemAlt("Infinite armour", netPlayerAllPlayersSetInfiniteArmourTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersSetInfiniteArmourFalse, nullptr);
	//instant revive might be a bit annoying to give to public.....
	AddItemAlt("Instant revive", netPlayerAllPlayersSetInstantReviveTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersSetInstantReviveFalse, nullptr);
	AddItemAlt("Control monitor & auto-unlocker", netPlayerAllPlayersSetVehicleMonitorTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersSetVehicleMonitorFalse, nullptr);
	AddItemAlt("Never Wanted", netPlayerAllPlayersSetNeverWantedTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersSetNeverWantedFalse, nullptr);
	AddItemAlt("Drop money", netPlayerAllPlayersSetDropMoneyTrue, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", netPlayerAllPlayersSetDropMoneyFalse, nullptr);
#ifdef AdminOptions
	//Remove reapons.. meh
#endif
	//walk on air.
	//if(CtlHelper_TrainerPrints)
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL INFO: ~w~These won't automatically enable on players that join after.");
}

void netPlayerAllPlayersDeleteObjectsVehicle(int unused, int unused2)
{
	int deleted_count = 0;
	int vehicle_count = 0;
	for (int i = 0; i < 16; i++)
	{
		if (i == myPlayerIndex && bitTest(ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf))
			continue;

		if (IsPlayerValid(i) && PLAYER_HAS_CHAR(i))
		{
			PED_INDEX player_ped;
			GET_PLAYER_CHAR(i, &player_ped);
			if (IS_CHAR_IN_ANY_CAR(player_ped))
			{
				VEHICLE_INDEX player_vehicle;
				GET_CAR_CHAR_IS_USING(player_ped, &player_vehicle);
				
				deleted_count += DeleteObjectsOnVehicle(player_vehicle);

				PED_INDEX player_vehicle_driver;
				GET_DRIVER_OF_CAR(player_vehicle, &player_vehicle_driver);
				if (player_vehicle_driver == player_ped)
					vehicle_count++;
			}
		}
	}
	printf("~r~CTL: ~w~Deleted ~y~%i ~w~objects on ~y~%i ~w~vehicle%s", deleted_count, vehicle_count, (vehicle_count != 1 ? "s." : "."));
}

void netPlayerAllPlayersDeleteObjectsPed(int unused, int unused0)
{
	int deleted_count = 0;
	int player_count = 0;
	for (int i = 0; i < 16; i++)
	{
		if (i == myPlayerIndex && bitTest(ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf))
			continue;

		if (IsPlayerValid(i) && PLAYER_HAS_CHAR(i))
		{
			PED_INDEX player_ped;
			GET_PLAYER_CHAR(i, &player_ped);
			deleted_count += DeleteObjectsOnPed(player_ped);
			player_count++;
		}
	}
	printf("~r~CTL: ~w~Deleted ~y~%i ~w~objects on ~y~%i ~w~player%s", deleted_count, player_count, (player_count == 1 ? "s." : "."));
}

void netPlayerAllPlayersMenu()
{
	bitSet(&ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers);
	SetHeader("All players");
	AddItemBool("Exclude self", &ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf, nullptr, "Exclude self from all player options.");
	AddItemMenu("Toggles", netPlayerAllPlayerToggles, "Toggle loops on all players.");
	AddItemMenu("Spawn vehicle", SpawnVehicleSetup, "Spawn chosen vehicle in front of every player.");
	AddItemAlt("Delete objects", netPlayerAllPlayersDeleteObjectsPed, "~PAD_A~ = Players, ~PAD_X~ = Their vehicles.", netPlayerAllPlayersDeleteObjectsVehicle, nullptr);
#ifdef AdminOptions
	AddItemMenu("Cartel Admin", NetPlayerAdminOptions, "Behaaave...");
#endif
#ifdef Moderator
	if(!bitTest(adminRemoteOnlineBitSet, BIT_ADMREMOTE_MODERATOR_DISABLED))
		AddItemMenu("Cartel Moderator", NetPlayerModeratorOptions, "Please be responsible, this IS revokable.");
#endif
}

void NetPlayerSendCartelRemoteCmd()
{
	
	SetHeader("Cartel Remote");
	AddItemWithParam("Teleport to me (on foot)", SHARED_MENUCMD_TELEPORT_TO_SENDER_ON_FOOT, SendSharedMenuCommandToPlayer, "Teleport them to me on foot.");
	AddItemWithParam("Teleport to me (their vehicle)", SHARED_MENUCMD_TELEPORT_TO_SENDER_IN_VEHICLE, SendSharedMenuCommandToPlayer, "Teleport them to me on foot.");
	AddItemWithParam("Teleport into my vehicle", SHARED_MENUCMD_TELEPORT_INTO_MY_VEHICLE, SendSharedMenuCommandToPlayer, "Teleport them to me on foot.");
}

#ifdef Crashes


void networkCrashWeaponFreeze() //can remove im told dont need a native for this.
{
	if (PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		PED_INDEX SelectedPlayerPed;
		GET_PLAYER_CHAR(GetMenuPlayerId(), &SelectedPlayerPed);
		//WEAPON_FREEZE(SelectedPlayerPed);
		printf("Function not present.");
	}
}


void networkBasicCrashes()
{
	bitReset(&ctlMem->m_bit_sets.m_network, NETWORK_GROUPHACK_BOOL); 
	
	SetHeader("Crashes");
	AddItem("Freeze gun", networkBasicCrashFreezeGun, nullptr);
	AddItemBool("Group hack", &ctlMem->m_bit_sets.m_network, NETWORK_GROUPHACK_BOOL, networkBasicCrashGroupHack, "Works best in vehicle with free seat near player, toggle to use.");
	AddItemAlt("Ped freeze (SuperLOD)", networkBasicCrashSuperlodTryTeleport, "~PAD_A~ = Try teleport them, ~PAD_X~ = On them.", networkBasicCrashSuperlodNormal, nullptr);
	if (CartelXEXPresent)
		AddItem("Weapon freeze", networkCrashWeaponFreeze, nullptr);
	//AddItem("Ped freeze (SuperLOD)", networkBasicCrashSuperlod, nullptr);

}

VEHICLE_INDEX testVehicle;

void netCrashTest(int unused1, int unused2)
{
	if (PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		PED_INDEX playerPed;
		GET_PLAYER_CHAR(GetMenuPlayerId(), &playerPed);
		vector3 pos;
		GET_CHAR_COORDINATES(playerPed, &pos.x, &pos.y, &pos.z);
		if (SpawnVehicle(VEHICLE_COMET, pos.x, pos.y, pos.z, 0.0f, &testVehicle, false, true))
		{
			pool_s* pPool = (pool_s*)*(int*)0x82FB0700;
			vehicle_s* point = (vehicle_s*)((pPool->size * (testVehicle >> 8)) + (int)pPool->pool);
			if (point == NULL)
				return; // false;

			printf("Class pointer == %x (%i)", point, point);
		}
	}
}

PED_INDEX testDriverPed;
void nathCrashTrigger(int unused1, int unused2)
{
	if (DOES_VEHICLE_EXIST(testVehicle))
	{
		pool_s* pPool = (pool_s*)*(int*)0x82FB0700;
		vehicle_s* point = (vehicle_s*)((pPool->size * (testVehicle >> 8)) + (int)pPool->pool);

		if (point == NULL)
			return; // false;
		
		//////
		
		REQUEST_MODEL(PED_M_Y_MULTIPLAYER);
		while (!HAS_MODEL_LOADED(PED_M_Y_MULTIPLAYER))
			WAIT(0);
		CREATE_CHAR_INSIDE_CAR(testVehicle, 0, PED_M_Y_MULTIPLAYER, &testDriverPed);
		NETWORK_INDEX pedNet;
		GET_NETWORK_ID_FROM_PED(testDriverPed, &pedNet);
		SET_NETWORK_ID_CAN_MIGRATE(pedNet, false);
		WAIT(100);
		SOUND_CAR_HORN(testVehicle, 1000);
		///
		
		point->nonZero_ifHornSounding = 0xDEAD1337;
		printf("Done, pointer = 0x%x", point);
	}
	else
		printf("Vehicle Doesn't exist");
}

void nathCrashDelete(int unused1, int unused2)
{
	if (DOES_VEHICLE_EXIST(testVehicle))
		DeleteCar(&testVehicle);
	else
		printf("Vehicle doesn't exist");
	
	if (DOES_CHAR_EXIST(testDriverPed))
		DeleteChar(&testDriverPed);
}

void netPlayerCrashOptions()
{
	
	bitReset(&ctlMem->m_bit_sets.m_network, NETWORK_GROUPHACK_BOOL);
	SetHeader("Crashes");
	AddItem("Nath's freeze test (spawn)", netCrashTest, nullptr);
	AddItemAlt("Nath freeze test (trigger)", nathCrashTrigger, nullptr, nathCrashDelete, nullptr);
	AddItem("Freeze gun", networkBasicCrashFreezeGun, nullptr);
	AddItemBool("Group hack", &ctlMem->m_bit_sets.m_network, NETWORK_GROUPHACK_BOOL, networkBasicCrashGroupHack, "Works best in vehicle with free seat near player, toggle to use.");
	AddItemAlt("Ped freeze (SuperLOD)", networkBasicCrashSuperlodTryTeleport, "~PAD_A~ = Try teleport them, ~PAD_X~ = On them.", networkBasicCrashSuperlodNormal, nullptr);
	if (CartelXEXPresent)
		AddItem("Weapon freeze", networkCrashWeaponFreeze, nullptr);
}
#endif

void printModderDATLogControl(int un1, int un2)
{
	printf("~r~CTL: ~w~Hold ~PAD_A~ and press ~PAD_X~ to ADD player to modder_db.dat");
}

void log_player_to_modder_db_dat(int un1, int un2)
{
	if (IS_BUTTON_PRESSED(0, BUTTON_A))
	{
		if (CartelXEXPresent)
		{
			//playerIdentifiers* ids = (playerIdentifiers*)GetPlayerPointerNath(GetMenuPlayerId());
			//if (CTL_LOG_MACHINE_TO_MODDER_FILE(&ids->machineId[0]))
			if (CTL_LOG_MACHINE_TO_MODDER_FILE(&GetPlayerPointer(GetMenuPlayerId())->m_machineId[0]))
			{
				printf("~r~CTL: ~w~Added %s ~w~to modder_db.dat", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
#ifdef Developer
				PRINTSTRING("Added ");
				PRINTSTRING(GET_PLAYER_NAME(GetMenuPlayerId()));
				PRINTSTRING(" to modder_db.dat (user input)\n");
#endif
				SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, GetMenuPlayerId());
				RefreshCurrentPage(0, 0);
			}
			else
				printf("~r~CTL: %s ~w~is already present in modder_db.dat", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
		}
		else
			printf("~r~CTL: ~w~Cartel.xex required for this function.");
	}
	else
		printModderDATLogControl(0, 0);
}


void printModderDATRemoveControl(int un1, int un2)
{
	printf("~r~CTL: ~w~Hold ~PAD_A~ and press ~PAD_Y~ to REMOVE player from modder_db.dat");
}

void remove_player_from_modder_db_dat(int un1, int un2)
{
	if (IS_BUTTON_PRESSED(0, BUTTON_A))
	{
		if(CartelXEXPresent)
		{
			//playerIdentifiers* ids = (playerIdentifiers*)GetPlayerPointerNath(GetMenuPlayerId());
			//if (CTL_REMOVE_MACHINE_FROM_MODDER_FILE(&ids->machineId[0]))
			if (CTL_REMOVE_MACHINE_FROM_MODDER_FILE(&GetPlayerPointer(GetMenuPlayerId())->m_machineId[0]))
			{
				printf("~r~CTL: ~w~Removed %s ~w~from modder_db.dat", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
#ifdef Developer
				PRINTSTRING("Removed ");
				PRINTSTRING(GET_PLAYER_NAME(GetMenuPlayerId()));
				PRINTSTRING(" from modder_db.dat (user input)\n");
#endif
				CLEAR_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, GetMenuPlayerId());
				RefreshCurrentPage(0, 0);
			}
			else
				printf("~r~CTL: ~w~Failed to remove %s ~w~from modder_db.dat", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
		}
		else
			printf("~r~CTL: ~w~Cartel.xex required for this function.");
	}
	else
		printModderDATRemoveControl(0, 0);
}

void assignMachineAlias(int un1, int un2)
{
#ifdef Developer
	CPlayerInfo* player = GetPlayerPointer(GetMenuPlayerId()); //Use this in normal build once security is laxed
#else
	CPlayerInfo* player = (CPlayerInfo*)GetPlayerPointerNath(GetMenuPlayerId());
#endif
	if (showAndGetKeyboardInput("Provide alias (MachineID)", false, player->m_gamertag, 15, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		if (CTL_GIVE_MACHINE_ALIAS(&player->m_machineId[0], ctlMem->m_char_buffers.keyboard_input_buffer))
		{
			printf("~r~CTL: %s's ~w~alias set to %s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), ctlMem->m_char_buffers.keyboard_input_buffer);
#ifdef PlayerDB
			//don't overwrite the data until we uploaded / logged clean
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, GetMenuPlayerId()) && IS_BIT_SET(ctlMem->m_bit_sets.m_UploadedData_UNUSED, GetMenuPlayerId()))
#else
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, GetMenuPlayerId())) //mandatory for all mods
#endif
			{
				//strcpy(ids->gamertag, ctlMem->m_char_buffers.keyboard_input_buffer, 16);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, GetMenuPlayerId()); //make cartel_helper.sco to re-check.
				RefreshCurrentPage(0, 0);
			}
			else
				printf("~r~CTL: ~w~I/O Busy, try again later.");
		}
	}
}

void removeMachineAlias(int un1, int un2)
{
#ifdef Developer
	CPlayerInfo* player = GetPlayerPointer(GetMenuPlayerId());
#else
	CPlayerInfo* player = (CPlayerInfo*)GetPlayerPointerNath(GetMenuPlayerId());
#endif
	if (CTL_REMOVE_MACHINE_ALIAS(&player->m_machineId[0]))
	{
		printf("~r~CTL: ~COL_NET_%i~%s ~w~ (~y~%s~w~) removed from alias file.", (GetMenuPlayerId() + 1), ctlMem->m_char_buffers.player_name_backup[GetMenuPlayerId()], player->m_gamertag);
		strcpy(player->m_gamertag, ctlMem->m_char_buffers.player_name_backup[GetMenuPlayerId()], 16);
		RefreshCurrentPage(0, 0);
	}
}

/*
void assignMachineAlias(int un1, int un2)
{
	playerIdentifiers* ids = (playerIdentifiers*)GetPlayerPointerNath(GetMenuPlayerId());
	if (showAndGetKeyboardInput("Provide alias (MachineID)", false, GET_PLAYER_NAME(GetMenuPlayerId()), 15, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		if (CTL_GIVE_MACHINE_ALIAS(&ids->machineId[0], ctlMem->m_char_buffers.keyboard_input_buffer))
		{
			printf("~r~CTL: %s's ~w~alias set to %s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), ctlMem->m_char_buffers.keyboard_input_buffer);
#ifdef PlayerDB
			//don't overwrite the data until we uploaded / logged clean
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, GetMenuPlayerId()) && IS_BIT_SET(ctlMem->m_bit_sets.m_UploadedData_UNUSED, GetMenuPlayerId()))
#else
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, GetMenuPlayerId())) //mandatory for all mods
#endif
			{
				//strcpy(ids->gamertag, ctlMem->m_char_buffers.keyboard_input_buffer, 16);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_CheckedForAlias_NotADickhead, GetMenuPlayerId()); //make cartel_helper.sco to re-check.
				RefreshCurrentPage(0, 0);
			}
			else
				printf("~r~CTL: ~w~I/O Busy, try again later.");
		}
	}
}

void removeMachineAlias(int un1, int un2)
{
	playerIdentifiers* ids = (playerIdentifiers*)GetPlayerPointerNath(GetMenuPlayerId());
	if (CTL_REMOVE_MACHINE_ALIAS(&ids->machineId[0]))
	{
		printf("~r~CTL: ~COL_NET_%i~%s ~w~ (~y~%s~w~) removed from alias file.", (GetMenuPlayerId() + 1), ctlMem->m_char_buffers.player_name_backup[GetMenuPlayerId()], GET_PLAYER_NAME(GetMenuPlayerId()));
		strcpy(ids->gamertag, ctlMem->m_char_buffers.player_name_backup[GetMenuPlayerId()], 16);
		RefreshCurrentPage(0, 0);
	}
}
*/
void invincibleTest(int un1, int un2)
{
	printf("%s ~w~Invincible == %i", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), IsPlayerInvincible(GetMenuPlayerId()));
}

void friendsListTest(int un1, int un2)
{
	printf("IsPlayerOnFriendsList(player, false) == %i", IsPlayerOnFriendsList(GetMenuPlayerId(), false));
	printf("IsPlayerOnFriendsList(player, true) == %i", IsPlayerOnFriendsList(GetMenuPlayerId(), true));
}

void PlayerListSubMenu() 
{
	static char* CopyNetPlayerStrings[3] = { "Ped", "Vehicle", "Anims"};
	CopyNetPlayerStrings[1] = ctlMem->m_commonStrings[GXT_STRING_VEHICLE]; //"Vehicle"

	SetMenuInNetworkMenu(true); //IMPORTANT THIS IS BEFORE GetMenuPlayerId()
	
	if (!IsPlayerValid(GetMenuPlayerId())) //seems to fix crashing when they leave and you back out / hit this menu. (testing not confirmed)
	{
		SetCurrentMenuInvalid(true, "Player left.");
		return;
	}
	SetHeader("Main Options");

#ifdef Developer
	//AddItem("IsInvincible??", invincibleTest, nullptr);
	//AddItem("IsOnFriendsList??", friendsListTest, nullptr);
#endif

	if(PLAYER_HAS_CHAR(GetMenuPlayerId()))
		STORE_WANTED_LEVEL(GetMenuPlayerId(), &wantedLvlInt); //maybe this points to ped??

	SetBit(&ctlMem->m_bit_sets.m_player, sharedIsSelectedPlayerMuted, NETWORK_IS_PLAYER_MUTED_BY_ME(GetMenuPlayerId()));
	AddItemWithParamAlt("Teleport", GetMenuPlayerId(), TeleportToNetworkPlayer, "~PAD_A~ = To them, ~PAD_X~ = To you, ~PAD_Y~ = Their vehicle.", TeleportNetworkPlayerToMe, TeleportIntoNetworkPlayersVehicle);
	AddItemMenu("Specific teleports", TeleportOptions, "Teleport player to fixed locations/other players.");
	if(GetMenuPlayerId() != myPlayerIndex)
		AddItemWithParam("Spectate", GetMenuPlayerId(), netPlayerSpectate, nullptr);

	AddItemMenu("Give mods", netPlayerToggles, "Infinite ammo, armour, walk on water/air etc.");
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_VEHICLE], netPlayerVehOpts, "Spawn, delete, fix etc."); //"Vehicle"
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_WEAPONS], playerWeaponOptions, "Give or remove weapons."); //"Weapons"
	AddItemBool(ctlMem->m_commonStrings[GXT_STRING_MUTE], &ctlMem->m_bit_sets.m_player, sharedIsSelectedPlayerMuted, toggleSelectedPlayerMute, nullptr); //Mute

	//Might need to console kick all at some point but we'll see. (MID Spoofing is out there but it's broke asf I think for public games.)
#ifdef Moderator
	if (GetCartelUserType(GET_HOST_ID()) != CARTEL_NON_USER || GET_HOST_ID() == myPlayerIndex)
#else
	if (GET_HOST_ID() == myPlayerIndex)
#endif
	{
		//KICK_PLAYER = "Kick player"
		char* kickString = BUFFER32; 				//CP_KICKPLAYER = "Kick"
		strcpy(kickString, GET_STRING_FROM_TEXT_FILE("CP_KICKPLAYER"), 32); char* kickDescription = "Kick player out of the game.";
		if (!IsPlayerUsingXboxOne(GetMenuPlayerId()))
			stradd(kickString, " console", 32);

		/*
		//char* kickString = "Kick"; char* kickDescription = "Kick player out of the game.";
		if (!IsPlayerUsingXboxOne(GetMenuPlayerId()))
			kickString = "Kick console"; kickDescription = "Kick player and stop them re-joining."; //never pair this with offhost kick or the game is completely fucked, no exageration. - Nath
			*/
		AddItem(kickString, NetworkPlayerKickNathan, kickDescription);
	}
	else
		AddItem("Vote kick", NetworkPlayerKickNathan, "Vote to kick player from the game.");
	//true if the kick byte/int is set to GetMenuPlayerId()
	//	AddItemBool("Vote kick", PlayersVoteKickStatusAddress, 0, NetworkPlayerKickNathan, nullptr);

	AddItemInt("Set wanted level", false, 0, 6, &wantedLvlInt, 1, setWantedLvl, "Only works if game has police enabled.");
	AddItemMenu("Questionable", netPlayerQuestionableActions, "Want one to spontaneously combust?");
	AddItemCharAlt("Copy players", false, CopyNetPlayerStrings, 2, &CopyPlayerSelect, CopyNetPlayer, "~PAD_A~ = Copy, ~PAD_X~ = Reset player.", playerOptUpdateMultiplayerModel, nullptr);
	AddItemCharAlt("Delete near player", false, delete_strings, 12, &delete_selection, callUniversalDelete, "~PAD_A~ = Universal (Scan), ~PAD_X~ = Clear area.", callLocalUniversalDelete, nullptr);
	AddItemRadius("Delete distance", 0, 500, &delete_distance, 10, nullptr, "Set distance limit from netPlayer for deletions, 0 for no limit."); //probably gnna draw locally innit.. //is char on screen do them, if not do us?

	if (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, GetMenuPlayerId() + 16)) //detected user.
		AddItemMenu("Cartel Remote", NetPlayerSendCartelRemoteCmd, "Send commands to their cartel");
	else
		AddItem("Cartel Remote", nullptr, "They must have Advertise menu turned on.");

#ifdef Crashes
	AddItemMenu("Crash (freeze) options", netPlayerCrashOptions, nullptr);
#endif
#ifdef Moderator
	if (!bitTest(adminRemoteOnlineBitSet, BIT_ADMREMOTE_MODERATOR_DISABLED))
	{
		AddItemMenu("Cartel Moderator", NetPlayerModeratorOptions, "Please be responsible.");
		if(CartelXEXPresent)
		{
			playerIdentifiers* ids = (playerIdentifiers*)GetPlayerPointerNath(GetMenuPlayerId());
			if (CTL_IS_MACHINE_IN_MODDER_FILE(&ids->machineId[0]))
				AddItemAlt("~PAD_A~ + ~PAD_Y~ Remove from modder_db.dat", printModderDATRemoveControl, nullptr, nullptr, remove_player_from_modder_db_dat); //only allow mods to do this manually bc users are dumb.
			else
				AddItemAlt("~PAD_A~ + ~PAD_X~ Log to modder_db.dat", printModderDATLogControl, nullptr, log_player_to_modder_db_dat, nullptr); //only allow mods to do this manually bc users are dumb.

			if (CTL_MACHINE_HAS_ALIAS(&ids->machineId[0]))
				AddItem("Remove machine alias", removeMachineAlias, nullptr);
			else
				AddItem("Assign machine alias", assignMachineAlias, "Type in with keyboard.");
		}
	}
#endif
#ifdef AdminOptions
	AddItemMenu("Cartel Admin", NetPlayerAdminOptions, "Behave now child..");
#endif
}

//bool IsDriveWaterExclusionArea(x, y, z, distance)


char* cartel_user_type_string(int usertype) //how they are displayed.
{
	//lets just force Developer/Moderator to return patron so the crackers can't fuck with it.
	switch (usertype)
	{
	//case CARTEL_NON_USER: return "Invalid";
	case CARTEL_PATRON: return "Cartel Patron";
	//case CARTEL_MODERATOR: return "Cartel Moderator";
	case CARTEL_MODERATOR: return "Cartel Patron";
	case CARTEL_GIVEAWAY_WINNER: return "Cartel Winner";
	//case CARTEL_DEVELOPER: return "Cartel Developer"; //never more than 16! //null terminator is 17, interior generation marker character is 18
	case CARTEL_DEVELOPER: return "Cartel Patron"; //never more than 16! //null terminator is 17, interior generation marker character is 18
	case CARTEL_USER: return "Cartel User";
	case CARTEL_TESTER: return "Cartel Tester";
#ifdef Developer
	case CARTEL_USER_DISABLED_ADVERTISE: return "CTL No-Advertise";
	case CARTEL_USER_HIDDEN: return "CTL Invalid";
#endif
	}
	return "Invalid";
}

#ifdef __INTELLISENSE__
#pragma diag_suppress 1578 //"hashof" is not actually providing 0, the ide / VS cannot see that the compiler actually pre-computes these hashes fine, so we are disabling this here.
#endif

eCartelUserType GetCartelUserType(PLAYER_INDEX playerIndex)
{
	uint stringHash = GET_HASH_KEY(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string);
	switch (stringHash)
	{ 
	case hashof("Cartel Patron"): return CARTEL_PATRON; //most common at the moment
	case hashof("Cartel Moderator"): return CARTEL_MODERATOR; //prob second most common
	case hashof("Cartel Winner"): return CARTEL_GIVEAWAY_WINNER;
	case hashof("Cartel Developer"): return CARTEL_DEVELOPER;
	case hashof("Cartel User"): return CARTEL_USER;
	case hashof("Cartel Tester"): return CARTEL_TESTER;
#ifdef Developer
	case hashof("."): return CARTEL_USER_DISABLED_ADVERTISE;
	case hashof("Invalid"): return CARTEL_USER_HIDDEN;
#endif
	}

	if (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (playerIndex + 16))) //can remove this once updated logic to use this bitset over GetCartelUserType
		return CARTEL_USER; //we've printed they joined befoe / already detected them.

#ifdef Moderator
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_admin, (playerIndex + 16))) //detected by admin/user scan.
		return CARTEL_USER;
#endif

	return CARTEL_NON_USER;
}


/*
eCartelUserType GetCartelUserType(PLAYER_INDEX playerIndex) //-1 if not true //how they are detected.
{
	if (COMPARE_STRING(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string, "Cartel User"))
		return CARTEL_USER;

	if (COMPARE_STRING(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string, "Cartel Winner"))
		return CARTEL_GIVEAWAY_WINNER;

	if (COMPARE_STRING(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string, "Cartel Patron"))
		return CARTEL_PATRON;

	if (COMPARE_STRING(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string, "Cartel Tester"))
		return CARTEL_TESTER;

	if (COMPARE_STRING(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string, "Cartel Moderator"))
		return CARTEL_MODERATOR;

	if (COMPARE_STRING(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string, "Cartel Developer"))
		return CARTEL_DEVELOPER;

#ifdef Developer
	if (COMPARE_STRING(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string, "."))
		return CARTEL_USER_DISABLED_ADVERTISE;
	if (COMPARE_STRING(freeMem->m_cartelBroadcastVars.m_Data[playerIndex].m_string, "Invalid"))
		return CARTEL_USER_HIDDEN;
#endif

#ifdef Moderator
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_admin, (playerIndex + 16))) //detected by admin/user scan.
		return CARTEL_USER;
#endif
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (playerIndex + 16))) //printed their join msg.
		return CARTEL_USER; //we've printed they joined befoe / already detected them.
	
	//return playerIndex == myPlayerIndex ? CARTEL_USER : CARTEL_NON_USER;
	return CARTEL_NON_USER;
}*/

PICKUP_INDEX CashDrops[16];

void cartel_joinmsg_loop()
{
#ifndef Moderator
	if (!IS_BIT_SET(*(int*)stickyBitSet, STICK_ADVERTISE_MENU)) //don't let them print people if they aren't advertising.
		return;
#endif
	for (int i = 0; i < 16; i++)
	{
		if (!IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (i + 16)))
		{
			int user_type = GetCartelUserType(i);
			if (user_type != CARTEL_NON_USER)
			{
				if (!HAS_NETWORK_PLAYER_LEFT_GAME(i))
				{
					if (i != myPlayerIndex)
						printf("~r~CTL: ~w~%s %s ~w~%s.", cartel_user_type_string(user_type), NetworkPlayerGetNameWithColour(i), ctlMem->m_commonStrings[GXT_STRING_JOINED]); //"Joined"

					SET_BIT(&ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (i + 16));
					SET_BIT(&ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i);
#ifdef Developer
					//char* buffer = BUFFER128;
					char* buffer = BUFFER64;
					sprintf(buffer, "Set %s detected modding. (ctl user)\n", GET_PLAYER_NAME(i));
					PRINTSTRING(buffer);
#endif
				}
			}
		}
	}
}

void NetworkLoopedOptions() //CLEAN
{
	Ped networkPlayerPed, networkPlayerVehicleDriver;
	int networkPlayerVehicleId;
	float pX, pY, pZ, pH, distance, mX, mY, mZ, networkPlayerSpeed;
	Vehicle networkPlayerVehicle;
	draw_menu_open_inputs();

	if (DOES_VEHICLE_EXIST(bladesHeli))
	{
		if (DOES_VEHICLE_EXIST(bladesVehicle))
		{
			GET_DRIVER_OF_CAR(bladesVehicle, &networkPlayerPed);
			if (!DOES_CHAR_EXIST(networkPlayerPed))
			{
				printf("~r~CTL: ~w~Heli blades vehicle is no longer occupied, deleting.");
				VehOptRemoveHeliBlades();
			}
		}
		else
			VehOptRemoveHeliBlades();
	}

	if (DOES_VEHICLE_EXIST(myNeonCar))
	{
		if (!IS_CHAR_SITTING_IN_CAR(myPlayerPed, myNeonCar))
		{
			for (int i = 0; i < 3; i++)
			{
				if (DOES_OBJECT_EXIST(myNeonCarObjects[i]))
				{
					GET_NETWORK_ID_FROM_OBJECT(myNeonCarObjects[i], &networkPlayerVehicleId); //reusing vars

					if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
						SET_NETWORK_ID_CAN_MIGRATE(networkPlayerVehicleId, true);
				}
			}
		}
	}

	if (NetworkGame)
	{
		if (GET_HOST_ID() == myPlayerIndex)
		{
			if (!bitTest(ctlMem->m_bit_sets.m_network, NETWORK_HAS_BECAME_HOST))
			{
				bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_HAS_BECAME_HOST);
				if (GetNumberOfKickedConsoles() > 0)
					printf("~r~CTL: ~w~You are now host, there are ~y~%i ~w~consoles on the kicked list.", GetNumberOfKickedConsoles());
			}
		}

		CartelSpawnPointLoop();
		cartel_joinmsg_loop(); //before adminclientloop bc of player lookout prints.
		AdminClientLoop();
#ifdef Moderator
		AdminSendLoop();
#endif
		//SECURITY CHECK (Admin)
		if (!bitTest(ctlMem->m_bit_sets.m_network, NETWORK_REGISTERED_BROADCAST_VARS_CALLED)) //should always be true here
		{
			DisableUnusedInputs(freeMem = (stickyFreeMemory*)myPlayerIndex); //break free mem pointer and cause a fuck uppp.
		}
		else
		{
			SharedMenuControlReceiveLoop();
			//KickedConsoleScanner(); //moved to cartel_helper.sco
			LivenMPInteriorsLoop();
			//SECURITY CHECK (Admin)
			if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_LOOP_CALLED))
			{
				if (!CartelXEXPresent)
				{
					fix_tell_net_loop();
					if (NETWORK_GET_GAME_MODE() == GAME_MODE_FREE_MODE)
					{
						if (*(int*)0x83081030 != 0)
						{
							if (!bitTest(ctlMem->m_bit_sets.m_debug, DEBUG_SWAPPED_FREEMODE_THREAD)) //allows helper to check invalid suggest/kick variables as host and correct them before freemode reads them and terminates (fixes singleplayer host attacks).
							{
								bitSet(&ctlMem->m_bit_sets.m_debug, DEBUG_SWAPPED_FREEMODE_THREAD);
								//ryans added a check for me so we can't lower our priority / the shit below will check so we can't accidentally swap it back, nice!
								char* freemode_script_name = GetGamemodeScriptName(GAME_MODE_FREE_MODE);
								swap_script_threads("cartel_helper", freemode_script_name);
#ifdef Developer
								PRINTSTRING("cartel_helper thread priority swapped with freemodes, helper will now execute before freemode.\n");
#endif
							}
						}
					}
				}

				if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_OBJ_PROT_WANTS_VEHICLE_RESPAWN))
					//if(IS_BIT_SET(ctlMem->m_bit_sets.m_network, NETWORK_OBJ_PROT_WANTS_VEHICLE_RESPAWN))
				{
					//spam respawn their vehicle if they're not auth'd
#ifdef Developer
					PRINTSTRING("\n[Cartel] Helper wants our vehicle respawned, calling HandlingRespawn()");
#endif
					HandlingRespawn();
					bitReset(&ctlMem->m_bit_sets.m_network, NETWORK_OBJ_PROT_WANTS_VEHICLE_RESPAWN);
				}

				if (bitTest(ctlMem->m_bit_sets.m_player, attachedToWalker))
				{
					//saves us having to store object or actively check we're attached to it.
					if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y))
					{
						DETACH_PED(myPlayerPed, true);
						CLEAR_CHAR_TASKS_IMMEDIATELY(myPlayerPed);
						bitReset(&ctlMem->m_bit_sets.m_player, attachedToWalker);
					}
				}

				if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_CARTEL_CHAT))
				{
					if (!bitTest(ctlMem->m_bit_sets.m_network, NETWORK_REGISTERED_CHAT_VARIABLES))
					{
						bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_REGISTERED_CHAT_VARIABLES);
						ctlMem->m_cartelChatVariables.m_Count = sizeof(ctl_chat_broadcast_vars_array) / 4; //left from debug doesn't work without it/has to be at top of struct.
						REGISTER_CLIENT_BROADCAST_VARIABLES((uint*)&ctlMem->m_cartelChatVariables, ctlMem->m_cartelChatVariables.m_Count, BROADCAST_ID_CHAT);
					}
					if (!bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_SETTING_ACCEPTED_CHAT_RULES))
					{
						int rules_decision = get_chat_rules_acceptance(); //this is gonna pause the script..
						if (rules_decision == 1)
						{
							bitSet(&ctlMem->m_bit_sets.m_menuPreferences, PREF_SETTING_ACCEPTED_CHAT_RULES);
							print("~g~You accepted the disclaimer", 2000);
							strcpy(ctlMem->m_char_buffers.keyboard_input_buffer, CHAT_JOINING_MESSAGE, CHAT_MSG_SIZE);
							sendMessageToTextChat(16);//, CHAT_JOINING_MESSAGE); //think 16 is all players target.
						}
						else if (rules_decision == 2)
						{
							printf("~r~CTL: ~w~You decided not to accept the disclaimer.");
							bitReset(&ctlMem->m_bit_sets.m_menuPreferences, PREF_CARTEL_CHAT);
							//TERMINATE_THIS_SCRIPT();
							//return 0;
						}
					}
					else
					{
						if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_T_TO_TYPE_CHAT))
						{
							if (IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_T))
							{ //need an option to not show the whole keyboard..
								DisableUnusedInputs();
								if (showAndGetKeyboardInput("Send message (Chat)", false, "", (CHAT_MSG_SIZE - 1), 0, 0.5f, 0.5f, 200, 0, 0))
									sendMessageToTextChat(16);//, ctlMem->m_char_buffers.keyboard_input_buffer);

								if (!bitTest(Container.BitSet, PB_IsMenuOpen))
									ShutDownMenu(); //we're done/need to re-enable inputs so they don't stay blocked.
							}
						}
						if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_CONTROLLER_CHAT_SHORTCUT))
						{
							if (IS_BUTTON_PRESSED(0, STICK_RIGHT) && IS_BUTTON_JUST_PRESSED(0, DPAD_LEFT))
							{
								DisableUnusedInputs();
								if (showAndGetKeyboardInput("Send message (Chat)", false, "", (CHAT_MSG_SIZE - 1), 0, 0.5f, 0.5f, 200, 0, 0))
									sendMessageToTextChat(16);//, ctlMem->m_char_buffers.keyboard_input_buffer);

								if (!bitTest(Container.BitSet, PB_IsMenuOpen))
									ShutDownMenu(); 	//we're done/need to re-enable inputs so they don't stay blocked.
							}
						}
					}
				}
				if (bitTest(ctlMem->m_bit_sets.m_player, networkRank10All))
				{
					if (NETWORK_IS_GAME_RANKED())
					{
						bool rankingAPlayer = false;
						for (int i = 0; i < 16; i++)
						{
							if (bitTest(ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf) && i == myPlayerIndex)
								continue;
							if (!SafeToActOnPlayerPed(i, &networkPlayerPed))
								continue;
							if (GET_PLAYER_RANK_LEVEL_DURING_MP(i) == 10)
								continue; //testing
							//if (GET_CHAR_MONEY(networkPlayerPed) < GetMoneyAmountForRank(10))
							//extern _native int GET_LEFT_PLAYER_CASH_TO_REACH_LEVEL(int playerRank)l
							if ((GetMoneyAmountForRank(GET_PLAYER_RANK_LEVEL_DURING_MP(i)) + GET_CHAR_MONEY(networkPlayerPed)) < GetMoneyAmountForRank(10))
							{
								//need to store cashPickups[16] so the pool doesn't flood..
								//if player is so far away from pickup, delete and respawn another..?
								if (GET_HOST_ID() == myPlayerIndex)
									SET_CHAR_MONEY(networkPlayerPed, (GET_CHAR_MONEY(networkPlayerPed) + 4200)); //1000 is known to work if this fails..
								else
								{
									if (!IsPlayerInvincible(i)) //make sure dey not a modderr.
									{
										GET_CHAR_COORDINATES(networkPlayerPed, &mX, &mY, &mZ);
										if (DOES_PICKUP_EXIST(CashDrops[i])) //i think the money gets spawned as multiple pickups so this doesn't really work
										{
											//GET_NETWORK_ID_FROM_OBJECT //?
											GET_PICKUP_COORDINATES(CashDrops[i], &pX, &pY, &pZ);
											GET_DISTANCE_BETWEEN_COORDS_3D(mX, mY, mZ, pX, pY, pZ, &distance);
											if (distance > 5.0f)
												REMOVE_PICKUP(CashDrops[i]); //spawn it on them again / don't flood pickup pool.
										}
										else
										{
											//GET_SAFE_PICKUP_COORDS(ped.x, ped.y, ped.z, &spawn.x, &spawn.y, &spawn.z);
											CREATE_MONEY_PICKUP(mX, mY, mZ, 10000, true, &CashDrops[i]);
											TeleportNetworkPlayerOnFoot(i, mX, mY, mZ, 0.0f, false);
										}
									}
								}
								rankingAPlayer = true;
							}
						}
						if (!rankingAPlayer)
						{
							printf("~r~CTL: ~w~All players now at Rank 10 cash level, once game finishes or host dashboards it will save.");
							bitReset(&ctlMem->m_bit_sets.m_player, networkRank10All);
						}
					}
					else
					{
						printf("~r~CTL: ~w~This only works in ranked matches.");
						bitReset(&ctlMem->m_bit_sets.m_player, networkRank10All);
					}
				} //else check dropMoney on player bit set?

				if (attachVeh != 0)
				{
					if (IS_PED_ATTACHED_TO_ANY_CAR(myPlayerPed))
					{
						if (DOES_VEHICLE_EXIST(attachVeh))
						{
							
							if (!IS_CAR_ON_SCREEN(attachVeh))
							{
								CAMERA_INDEX playerCamera;
								GET_GAME_CAM(&playerCamera);
								POINT_CAM_AT_VEHICLE(playerCamera, attachVeh);
#ifdef Developer
								PRINTSTRING("attachVeh not on screen & we're attached, ran POINT_CAM_AT_VEHICLE(playerCam + cinematicCam, attachVeh)\n");
#endif
							}
							
							GET_DRIVER_OF_CAR(attachVeh, &networkPlayerPed); //reusing vars again
							if (!DOES_CHAR_EXIST(networkPlayerPed)) //if driver doesn't exist
							{
								printf("~r~CTL: ~w~The driver of this ~y~%s ~w~departed, detaching.", VehiclesModelName(attachVeh));
								netPlayerVehDetach();
							}

							if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y))
							{
								printf("~r~CTL: ~w~Detached player from %s.", VehiclesModelName(attachVeh));
								netPlayerVehDetach();
							}
						}
					}
					else
					{
						if (IS_IN_SPECTATOR_MODE())
						{
							Camera playerCamera;
							GET_GAME_CAM(&playerCamera);
							SET_CAM_TARGET_PED(playerCamera, myPlayerPed);
							SET_IN_SPECTATOR_MODE(false);
							attachVeh = 0;
						}
					}
				}
			} //admin loop called.
			else //admin loop hasn't been called
				memcpy(ctlMem, (int*)GET_FRAME_COUNT(), hidden_number1355301()); //crashes, woop.
		}
	}
	DickheadScannerLoop();
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_LOOP_CALLED) || !NetworkGame)
	{
		NETWORK_INDEX NetworkId;
		uint IndexesToCheck = NetworkGame ? 16 : 1; //playerindex is always 0 in singleplayer.
		for (int i = 0; i < IndexesToCheck; i++)
		{
			if (IsPlayerValid(i)) //yes we don't need to check they have a char for this.
			{
				
				//if (!IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, (i + 16)))
				if(ctlMem->m_char_buffers.player_name_backup[i][0] == '\0') //nothing stored here.
				{	//back up the names regardless
					strcpy(ctlMem->m_char_buffers.player_name_backup[i], GET_PLAYER_NAME(i), 16);
					//SET_BIT(&ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, (i + 16));

					if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES))
					{
						if (i == myPlayerIndex)
#ifdef Developer
							strcpy((char*)GET_PLAYER_NAME(i), "CARTEL TEST", 16);
#else
							strcpy((char*)GET_PLAYER_NAME(i), "Me", 16);
#endif
						else
						{
							//sprintf((char*)GET_PLAYER_NAME(i), "playerIndex %i", i); //auto over-writing now taken care of if player just joined.
							//sprintf doesn't work.
							strcpy((char*)GET_PLAYER_NAME(i), "playerIndex ", 16);
							straddi((char*)GET_PLAYER_NAME(i), i, 16);
						}
					}
				}
			}
			else
			{
#ifdef Moderator
				CLEAR_BIT(&ctlMem->m_bit_sets.m_loggedPlayer_UNUSED, i); //character/ped is not required for this. //only moderators will log people locally.
#endif
				ctlMem->m_char_buffers.player_name_backup[i][0] = '\0'; //ideally this should be moved to helper for better execution tightness.
				CLEAR_BIT(&ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (i + 16));
				CLEAR_BIT(&ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (i + 16));
			}

			/*
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES))
			{
				for (int i = 0; i < 16; i++)
				{
					if (!IsPlayerValid(i) || i == myPlayerIndex)
					{
						strcpy(ctlMem->m_char_buffers.player_name_backup[i], "Not grabbed.", 16);
						continue;
					}

					strcpy(ctlMem->m_char_buffers.player_name_backup[i], GET_PLAYER_NAME(i), 16);
					sprintf(GET_PLAYER_NAME(i), "playerIndex %i", i); //clears buffer nice.
				}
			}
			*/
			if (!IsPlayerValid(i) || !PLAYER_HAS_CHAR(i))
			{
				//some bitsets here might not have any logic, see cartel_helper network_loops.
				CLEAR_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, i);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (i + 16)); //loop in helper.sco
				CLEAR_BIT(&ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, i);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, (i + 16)); //loop in helper.sco
				CLEAR_BIT(&ctlMem->m_bit_sets.m_removeWeapons_driveWater, i);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_removeWeapons_driveWater, (i + 16)); //loop in helper.sco
				CLEAR_BIT(&ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, i); //loop in helper.sco
				CLEAR_BIT(&ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, (i + 16)); //loop in helper.sc
				CLEAR_BIT(&ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, i);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, (i + 16));
				CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i); //loop in helper.sco
				CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, (i + 16)); //loop in helper.sco
				CLEAR_BIT(&ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, i);
				//CLEAR_BIT(&ctlMem->m_bit_sets.m_capDickheadNotify_instantReviveCap, i); //NOT needed here / dealt with elsewhere.
				CLEAR_BIT(&ctlMem->m_bit_sets.m_capDickheadNotify_instantReviveCap, (i + 16));
				CLEAR_BIT(&ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, i);
				CLEAR_BIT(&ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, (i + 16));
				if (DOES_OBJECT_EXIST(freeMem->m_walk_water_object[i]))
					DeleteObject(&freeMem->m_walk_water_object[i]);

				if (DOES_OBJECT_EXIST(freeMem->m_walk_on_air.m_object[i])) //could use ground height check to see if there's a floor beneath us when enabling.
					DeleteObject(&freeMem->m_walk_on_air.m_object[i]);

				//	if (DOES_PICKUP_EXIST(CashDrops[i]))
					//	REMOVE_PICKUP(CashDrops[i]);

				continue;
			}

			//SECURITY CHECK
#ifdef Security
			if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SECURITY_BIT_GET_XUID_FROM_FREE_MEM_CALLED))
#endif
				GET_PLAYER_CHAR(i, &networkPlayerPed);

			if (bitTest(ctlMem->m_bit_sets.m_player, playerESP))
				DrawPlayerBoxes(i); //could skip self but eh

			if (IS_BIT_SET(ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, i)) //jugg
			{
				if (!HAS_CHAR_GOT_WEAPON(networkPlayerPed, WEAPON_ARMOUR))
					GIVE_WEAPON_TO_CHAR(networkPlayerPed, WEAPON_ARMOUR, 1, false); //pretty sure this works good!

				ADD_ARMOUR_TO_CHAR(networkPlayerPed, 999);
			}

			if (IS_BIT_SET(ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, i))
			{
				if (IS_WANTED_LEVEL_GREATER(i, 0))
				{
					CLEAR_WANTED_LEVEL(i); //test this
					//ALTER_WANTED_LEVEL(i, 0);
					//APPLY_WANTED_LEVEL_CHANGE_NOW(i);
				}
			}

			if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
			{
				GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
				GET_DRIVER_OF_CAR(networkPlayerVehicle, &networkPlayerVehicleDriver);
				GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);
			}

			GET_CHAR_SPEED(networkPlayerPed, &networkPlayerSpeed);

			if (DOES_VEHICLE_EXIST(networkPlayerVehicle))
			{
				GET_CAR_SPEED(networkPlayerVehicle, &networkPlayerSpeed);
				if (IS_BIT_SET(ctlMem->m_bit_sets.m_neverWanted_vehAutoFix, (i + 16)))
				{
					if (IS_CAR_DEAD(networkPlayerVehicle) || IS_CAR_ON_FIRE(networkPlayerVehicle))
					{
						if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
						{
							SET_CAR_FORWARD_SPEED(networkPlayerVehicle, 0.0f); //stop net control fucking them up
							FIX_CAR(networkPlayerVehicle);
							SET_CAR_HEALTH(networkPlayerVehicle, 1000);
							SET_CAR_ENGINE_ON(networkPlayerVehicle, true, true);
						}
						else
							REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
					}
				}
				if (IS_BIT_SET(netPlayerVehicleProtectionBitSet, i))
				{
					if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
					{
						SET_NETWORK_ID_CAN_MIGRATE(networkPlayerVehicleId, false);
						ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
						printf("~r~CTL: ~w~Please wait...");
						WAIT(1750);
						printf("~r~CTL: ~w~Control of %s's ~w~%s locked.", NetworkPlayerGetNameWithColour(i), VehiclesModelName(networkPlayerVehicle));
						SET_NETWORK_ID_CAN_MIGRATE(networkPlayerVehicleId, true);
						//maybe it's just sending anything after this that helps the process...
						//could try FIX_CAR or something generic.. not massively noticable, it has to sync though.
						//MARK_CAR_AS_NO_LONGER_NEEDED(&networkPlayerVehicle); //try without this...
						//have to press a few times until you can't get control.
						//works without no longer needed

						//can probs imptove this
						CLEAR_BIT(&netPlayerVehicleProtectionBitSet, i);
						continue;
					}
					else
						REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId); //should make a func to do this that stores the last id so we never request multiple times / spam the network.
				}
				/* //moved to helper 07/July/2025
				if (IS_BIT_SET(ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, i))
				{
					//Green or Red traffic light would work well.. can choose to only show an object when boosting is unavailable then.
					if (networkPlayerVehicleDriver == networkPlayerPed) //they are driving the car..
					{
						if (is_vehicle_horn_sounding(networkPlayerVehicle))
						{
							if (!IS_BIT_SET(ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, (i + 16))) //cap it))
							{
								if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
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
							bitReset(&ctlMem->m_bit_sets.m_hornSpeedBoost_hasBoosted, (i + 16)); //remove cap.
					}
				}
				*/
				/* //moved to helper.sco /04/01/2025
				if (IS_BIT_SET(ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (i + 16))) //vehicle auto-flip
				{
					if (networkPlayerSpeed < 0.175f && IS_CAR_UPSIDEDOWN(networkPlayerVehicle))
					{
						//HEIGHT_ABOVE_GROUND check needed for localplayer..
						if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
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
				*/
				if (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, i))
				{
					if (networkPlayerVehicleDriver == networkPlayerPed) //they are driving the car..
					{
						if (!IS_CHAR_GETTING_IN_TO_A_CAR(networkPlayerPed))
						{
							uint networkPlayerVehicleLock;
							GET_CAR_DOOR_LOCK_STATUS(networkPlayerVehicle, &networkPlayerVehicleLock);
							if (networkPlayerVehicleLock == VEHICLE_DOOR_LOCKED_BOTH_SIDES || networkPlayerVehicleLock == VEHICLE_DOOR_LOCKED)
							{
								if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
								{
									LOCK_CAR_DOORS(networkPlayerVehicle, DOOR_LOCK_NONE);
									printf("~r~CTL: %s's ~w~%s, ~w~is locked, fixed/un-locked.", NetworkPlayerGetNameWithColour(i), VehiclesModelName(networkPlayerVehicle));
								}
								else
									REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
							}
							if (!DOES_PLAYER_HAVE_CONTROL_OF_NETWORK_ID(i, networkPlayerVehicleId))
							{ //someone else has control of their vehicle!
								for (int z = 0; z < 16; z++)
								{
									if (DOES_PLAYER_HAVE_CONTROL_OF_NETWORK_ID(z, networkPlayerVehicleId))
									{
										//should have 16 player ints, vehicle_last_controlled_by[i] == playerIndex? then clear it once they don't have control no more..
										if (z != myPlayerIndex)
										{
											char* buffer = BUFFER128;

											strcpy(buffer, "~r~CTL: ", 128);
											stradd(buffer, NetworkPlayerGetNameWithColour(z), 128);
											stradd(buffer, " ~w~has control of ", 128);
											stradd(buffer, NetworkPlayerGetNameWithColour(i), 128);
											stradd(buffer, "'s ~w~", 128);
											stradd(buffer, VehiclesModelName(networkPlayerVehicle), 128);
											stradd(buffer, ".", 128);

											if (IS_MESSAGE_BEING_DISPLAYED()) //only add to brief once.
												ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);

											printf(buffer);
											break;
										}
									}
								}
							}
						}
					}
				}
			}

			GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
			GET_CHAR_COORDINATES(networkPlayerPed, &pX, &pY, &pZ);
			float ourDistanceFromNetworkPed;
			GET_DISTANCE_BETWEEN_COORDS_3D(mX, mY, mZ, pX, pY, pZ, &ourDistanceFromNetworkPed);
			
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, i)) //Money Drop..
			{
				if (DOES_PICKUP_EXIST(CashDrops[i]))
				{
					//GET_NETWORK_ID_FROM_OBJECT //?
					vector3 Pick;
					GET_PICKUP_COORDINATES(CashDrops[i], &Pick.x, &Pick.y, &Pick.z);
					GET_DISTANCE_BETWEEN_COORDS_3D(pX, pY, pZ, Pick.x, Pick.y, Pick.z, &distance);
					if (distance > 5.0f)
						REMOVE_PICKUP(CashDrops[i]); //spawn it on them again / don't flood pickup pool.
				}
				else
				{
					if (NETWORK_IS_GAME_RANKED())
					{
						//if (GET_PLAYER_RANK_LEVEL_DURING_MP(i) != 10) //think this will return 0 if not ranked..
						if(GET_CHAR_MONEY(networkPlayerPed) < GetMoneyAmountForRank(10))
							CREATE_MONEY_PICKUP(pX, pY, pZ, 10000, true, &CashDrops[i]);
						else
						{
							printf("~r~CTL: %s ~w~is already Rank 10.", NetworkPlayerGetNameWithColour(i));
							CLEAR_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, i);
						}
						//TeleportNetworkPlayerOnFoot(i,pX,pY,pZ,0.0f);
					}
					else           //GET_SAFE_PICKUP_COORDS(ped.x, ped.y, ped.z, &spawn.x, &spawn.y, &spawn.z);
						CREATE_MONEY_PICKUP(pX, pY, pZ, 10, true, &CashDrops[i]);
				}
				//SpawnMoneyOnPed(networkPlayerPed, 10000); //will modify this to take coordinates we already grab below.
			}

			if (IS_BIT_SET(ctlMem->m_bit_sets.m_removeWeapons_driveWater, (i + 16)))
			{
				if (!DOES_OBJECT_EXIST(freeMem->m_walk_water_object[i]))
					CreateObject(OBJECT_NJ05_OILTNKBIG, 0.0f, 0.0f, -80.0f, 0.0f, &freeMem->m_walk_water_object[i], true); //out the way.
			}
		
			if (IS_BIT_SET(ctlMem->m_bit_sets.m_removeWeapons_driveWater, i))
				REMOVE_ALL_CHAR_WEAPONS(networkPlayerPed);
		}


		if (netPlayerSpectating != INVALID_PLAYER_INDEX && IS_IN_SPECTATOR_MODE()) //allows use of spectate for vehicle-player attachments.
		{
			//HIDE_HUD_AND_RADAR_THIS_FRAME();
			if (IsPlayerValid(netPlayerSpectating) && PLAYER_HAS_CHAR(netPlayerSpectating))
			{
				Ped spectatePed;
				GET_PLAYER_CHAR(netPlayerSpectating, &spectatePed); //we get their char over and over as if they flip model, spectating breaks
				if (DOES_CHAR_EXIST(spectatePed)) //can probably remove this as we check PLAYER_HAS_CHAR..
				{
					GET_CHAR_COORDINATES(spectatePed, &pX, &pY, &pZ);
					CAMERA_INDEX playerCamera;
					GET_GAME_CAM(&playerCamera);
					SET_CAM_TARGET_PED(playerCamera, spectatePed);

					GET_DISTANCE_BETWEEN_COORDS_3D(mX, mY, mZ, pX, pY, pZ, &distance);
					if (distance > 70.0f)
						WARP_CHAR_FROM_CAR_TO_COORD(myPlayerPed, pX + 20.0f, pY + 20.0f, pZ); //pretty sure this works on foot without issues
				}
			}
			else
				netPlayerSpectate(netPlayerSpectating, 0); //will detect non presence and bail spectate.
		}
	} //if admin loop called
	else
	{
#ifdef Developer
	//PRINTSTRING("CRASH AT BOTTOM OT NETWORK_OPTIONS.C - admin loop not called\n");
	//WAIT(100);
#endif
		ctlMem++; //mis-align memory and crash because motherfuckers killed our admin loop
	}
}