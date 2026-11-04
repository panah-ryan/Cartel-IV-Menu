#include "stdafx.h"
#include "Utils.h"
#include "MenuUiHandling.h"
#include "MenuExecutionHandling.h"
#include "threading.h"
#include "Network_Options.h"
#include "Security.h"
#include "Admin_Options.h"
#include "Vehicle_Options.h"
#include "World_Options.h"
#include "ObjectVehicles_Options.h"
#include "Script_Options.h"
#include "Keyboard_Options.h"
#include "Functions.h"
#include "Player_Options.h"
#include "Maintenance_Loops.h"

extern TextureDict webborders;
extern TextureDict browser;

//SECURITY/AUTH IDS MOVED TO TOP OF FUNCTIONS.C
void main()
{
	THIS_SCRIPT_IS_SAFE_FOR_NETWORK_GAME();
	webborders = LOAD_TXD("webborders");
	browser = LOAD_TXD("browser"); //can't stream these.

	ADD_STRING_TO_NEWS_SCROLLBAR("Cartel Menu was here, FuCKaDoODdlE-DoOOoOoOO!");
	myPlayerIndex = GET_PLAYER_ID();
	myCurrentEpisode = GET_CURRENT_EPISODE();
	NetworkGame = IS_NETWORK_SESSION();
	CartelXEXPresent = bitTest(*(int*)stickyBitSet, STICK_XEX_PRESENT);
	freeMem = (stickyFreeMemory*)freeMemStartAddress;
	freeMem->m_Cartel_ThreadId = GET_ID_OF_THIS_THREAD();

	//SECURITY //you can print a players by going to their name, cartel admin, then print obfuscated ints.
	//MOVED TO TOP OF FUNCTIONS.C

#pragma region cartel_memory
	if (!setup_memory()) 
	{
		SET_TEXT_USE_UNDERSCORE(true);
		printf("~r~CTL: ~w~cartel_memory.sco not found, terminating.");
		TERMINATE_THIS_SCRIPT();
	}
	ctlMem = (ctl_memory_sco*)get_free_memory(); //returns ctl_memory_buffer
	memset(ctlMem, 0, 8188); 
	DickheadScannerLoop();
	
	char* compile_hashes_buff = BUFFER16;
	sprintf(compile_hashes_buff, "%s%s", __DATE__, GET_STRING_FROM_STRING(__TIME__, 0, 2));
	ctlMem->m_CartelMenuCompileDateHourHash = GET_HASH_KEY(compile_hashes_buff);
	ctlMem->m_commonStructsCombinedSize = (sizeof(ctl_memory_sco) + sizeof(stickyFreeMemory));

	init_ctl_mem(); 
	if (!start_cartel_helper())
	{
		printf("~r~CTL: ~y~cartel_helper.sco ~w~is missing! Functionality will be limited and you might even crash!");
		WAIT(5000);
	}
	StoreXUIDAndGamertagInFreeMem(); //used for restoring not just security.
#ifdef Developer
		PRINTSTRING("helper.sco started\n");
#endif

#ifdef Security
	sprintf(ctlMem->m_char_buffers.keyboard_input_buffer, "%s%s%s%s", ctlMem->m_keyboard.key_mappings[KEY_M][0], ctlMem->m_keyboard.key_mappings[KEY_A][0], ctlMem->m_keyboard.key_mappings[KEY_I][0], ctlMem->m_keyboard.key_mappings[KEY_N][0]); //printed/tested as-is prints "main"
	scrThread* main = grab_scrthread(ctlMem->m_char_buffers.keyboard_input_buffer); //works string is "main" granted init_ctl_mem has ran.
	memset(ctlMem->m_char_buffers.keyboard_input_buffer, hidden_number0(), hidden_number57()); //MAX_KEYBOARD_INPUT_SIZE == 96
	__catch();
#endif
	//main->m_Stack++; //crashes
#pragma endregion

#ifdef Developer
	printf("~r~CTL: ~w~sizeof(stickyFreeMemory) == %i/3212, sizeof(ctl_memory_sco) == %i/8188.", sizeof(stickyFreeMemory), sizeof(ctl_memory_sco));
	printf("~r~CTL: ~w~sizeof(saveData_storage) == %i/100", sizeof(saveData_storage));
	printf("~r~CTL: ~w~sizeof(all_broadcast_vars) == %i", sizeof(ctl_attack_network_notifier_array) + sizeof(ctl_host_broadcast_vars_array) + sizeof(ctl_chat_broadcast_vars_array) + sizeof(ctl_broadcast_vars_array));
	//printf("~r~CTL: ~w~ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu adderss == 0x%x", &ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu);
	//sizeof(ctl_attack_network_notifier_array);
#endif
	//time_stamp_buffer[64]
	char* time_stamp_buffer = BUFFER64;
	sprintf(time_stamp_buffer, "Cartel (Early access) Compiled %s at %s.", __DATE__, __TIME__);
	ADD_TO_PREVIOUS_BRIEF(time_stamp_buffer);

	bitSet((int*)stickyBitSet, STICK_CARTEL_SCO_WAS_HERE_MEMES_COMPAT);

#ifdef Security
	if (bitTest(ctlMem->m_bit_sets.m_player, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED)) //triggered by HasXuidBeen.., called by StoreXUIDGamrtagInFreeMem
	{
		init_thread_data();
		//freeMem = (stickyFreeMemory*)freeMemStartAddress;
		init_free_mem(); //will only overwrite what we tell it to.
		searchResults = (search_results_array*)0x830FB110;
	}
	else
	{
	//	main->m_Stack++;  //cause main.sco to crash them.
		__throw();
		memcpy(ctlMem, &freeMem->m_map_mod, (hidden_number1355301() * hidden_number97())); //this should really cause a fuck up
	}
#else
	init_thread_data();
	init_free_mem(); //will only overwrite what we tell it to.
	searchResults = (search_results_array*)0x830FB110;
#endif

	//WAIT(20000);
	InitMenuDraw();	

	if (CartelXEXPresent)
	{
		CTL_ADD_XEX_COMPILE_TIME_TO_BRIEF();
		CTL_XNOTIFY_GREETING();
		spawnerMem = (SpawnerObject*)CTL_GET_SPAWNER_MEMORY();
		max_spawner_objs = 300; //#define MAX_SPAWNER_OBJECTS max_spawner_objs //70 by default
		//memset(spawnerMem, 0, 3600); //maybe only do this if session changes. ..might be okay without actually as we'll do DOES_OBJECT_EXIST etc.
#ifdef Moderator
		CTL_INIT_ALIAS_DATA(); //XEX will only allow this once.
#endif
	}
	else
		spawnerMem = ctlMem->m_RETAIL_spawned_objects;

	init_object_spawner_name_buffers();
	update_menu_open_controls_string(); //lazy fix attempt
	while (IS_SCREEN_FADED_OUT())
	{
		WAIT(0);
		myPlayerIndex = GET_PLAYER_ID();
		GET_PLAYER_CHAR(myPlayerIndex, &myPlayerPed);

		if (NetworkGame)
		{
			DickheadScannerLoop(); //calls logPlayer

			if (GET_HOST_ID() != GET_PLAYER_ID() && GET_HOST_ID() != -1)
			{ //host index has been identified.
				if (NETWORK_IS_BEING_KICKED())	
					bitSet((int*)stickyBitSet, STICK_KICKED_TRYING_TO_JOIN);
				else
					strcpy(freeMem->m_last_games_host_gamertag, (char*)GET_PLAYER_NAME(GET_HOST_ID()), 16); //should be false first ideally.
					//strcpy((char*)LastHostNameMemAddress, GET_PLAYER_NAME(GET_HOST_ID()), 16);
				/*
				if (freeMem->m_cartel_join_pin != 0)
				{ //A join pin has been specified.
					RegisterCartelBroadcastVariables(false); //check in function to only allow regiser once per script load.
					if (freeMem->m_cartelBroadcastVars.m_Data[GET_PLAYER_ID()].m_ints[7] != freeMem->m_cartel_join_pin)
					{//only send pin once at first opportunity to avoid spamming the network.
						freeMem->m_cartelBroadcastVars.m_Data[GET_PLAYER_ID()].m_ints[7] = freeMem->m_cartel_join_pin;
						SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_ADMIN);
					}
				}
				*/
			}
			if (NETWORK_GET_GAME_MODE() == GAME_MODE_FREE_MODE || NETWORK_GET_GAME_MODE() == GAME_MODE_PARTY_MODE)
			{ //fucks up in races without this lol
				vector4 customSpawn;
				if (getCustomSpawnPointCoords(&customSpawn)) //checks coordinates are saved and preference is turned on.
				{
					SET_CHAR_COORDINATES_NO_OFFSET(myPlayerPed, customSpawn.x, customSpawn.y, customSpawn.z);
					SET_CHAR_HEADING(myPlayerPed, customSpawn.h);
				}
				else
				{
					if (bitTest(*(int*)stickyBitSet, STICK_USE_CARTEL_SPAWN))
					{
						//CarryOverSaveSettings(); //TESTING //won't run SP carryover if screen is faded out.
						float x, y, z, h;
						GetCartelSpawnCoordinatesForPlayer(GET_PLAYER_ID(), &x, &y, &z, &h);
						SET_CHAR_COORDINATES_NO_OFFSET(myPlayerPed, x, y, z);
						SET_CHAR_HEADING(myPlayerPed, h);
					}
				}
			}
		}
	}


#ifdef Developer
	if(IS_BUTTON_PRESSED(0,BUTTON_B))
		WAIT(10000); //nath ghost compatibility. //broadcast vars getting in too early probs.
#endif
	myPlayerIndex = GET_PLAYER_ID(); //loading screen has faded so we should be good.
	GET_PLAYER_CHAR(myPlayerIndex, &myPlayerPed);

	if (NetworkGame)
	{
		RegisterCartelBroadcastVariables(true); //check in function to only allow register once per script load.
#ifdef Moderator
		DISPLAY_PLAYER_NAMES(true);
		DISPLAY_PLAYER_ICONS(true);
		//SET_DISPLAY_PLAYER_NAME_AND_ICON(i, true);
#endif
	}

#ifdef Moderator
#ifndef Developer
	bool foundDank = false;
	for (int i = 0; i < NETWORK_GET_FRIEND_COUNT(); i++)
	{
		if (DeObfuscateValue(GET_HASH_KEY(NETWORK_GET_FRIEND_NAME(i))) == 625579819)
		{
			foundDank = true;
			//Dank Functions found.
		}
	}
	while (!foundDank) 
	{
		
		__throw();
	}
#endif
#endif
	printf("Hola ~r~%s~w~, Welcome to the ~r~Cartel.", getName(myPlayerIndex)); //leave the comma where it is.

#ifndef Security
	fix_freeze_guns_in_memory();
	loadCartelAutoScos(); //should add a seperate call to load cartel_helper.sco (W.I.P)
	if (IS_PLAYER_ONLINE())
		DownloadScores();

	CarryOverSaveSettings();

	while (true)
	{
		WAIT(0);
		LoopedExecutionEntry();
		HandleMenuUi();
		process_threads();
	}
#else
	uint VerifiedArrayIndex = hiddenSeventySix(); //Crash if below loop is fucked with due to out of bounds array
#ifdef OUTPUT_AUTH_DEBUG
	PRINTSTRING("VerifiedArrayIndex = hiddenSeventySix() (");
	PRINTINT(VerifiedArrayIndex);
	PRINTSTRING(")");
	PRINTNL();
#endif
	//while (VerifiedArrayIndex > hidden_number32()) //be better to point to machine ID or something weird to beter throw them off in the asm.. //never have more than 32 ObfuscatedIdentifiers at once!
	while (VerifiedArrayIndex > hidden_number57()) //be better to point to machine ID or something weird to beter throw them off in the asm.. 
	{
#ifdef OUTPUT_AUTH_DEBUG
		PRINTSTRING("while(VerifiedArrayIndex > hidden_number32()) (");
		PRINTINT(VerifiedArrayIndex);
		PRINTSTRING(")");
		PRINTNL();
		PRINTSTRING("hidden_number32() == ");
		PRINTINT(hidden_number32());
		WAIT(100);
#endif
		//it might lag but don't wait cuz it'll give us away..
		for (int i = 0; i < MAX_AUTH_USERS; i++)
		{
#ifdef OUTPUT_AUTH_DEBUG
			PRINTSTRING("for (int i = 0; i < MAX_AUTH_USERS; i++)\n");
#endif
			//could make an int in here count the loops and if less than 2 we know it's been fucked with as 0 is empty.
			//if Obfuscated Machine ID mathches (MP COMPATABILITY ADDED BY WAITING FOR LOADING TO FADE ABOVE). //double negative logic to confuse (make sure compiler doesn't optimise & remove)
			//IMPROVEMENT: Should read the machine id pointer into a variable then memcpy it back later to break online if it's not authed / spoofed. //might make it more obvious though..
			if (!(ctlMem->ObfuscatedIdentifiers[i].m_midObfuscatedLower != ObfuscateMachineIDUniqueHalf(*(int*)(((int)getName(myPlayerIndex) - hiddenSeventySix()) + hidden24()))))
			{
				fix_freeze_guns_in_memory(); //put in here so it's authed //will fix in singleplayer before joining/at risk if they autoload it.
				VerifiedArrayIndex = (ctlMem->ObfuscatedIdentifiers[i].m_xuidObfuscatedUpper != ObfuscateXUIDUniqueHalf(GetPlayerXuidFromFreeMemoryAddressObfuscated())) ? (uint)freeMem : i; //breaks the while loop
				bitSet(&ctlMem->m_bit_sets.m_admin, ADMIN_SECURITY_BIT_MACHINE_ID_MATCHED);
#ifdef OUTPUT_AUTH_DEBUG
				PRINTSTRING("i == ");
				PRINTINT(i);
				PRINTSTRING(", VerifiedArrayIndex == ");
				PRINTINT(VerifiedArrayIndex);
				PRINTNL();
				//break;
#endif
			}
		}
	}
#ifdef OUTPUT_AUTH_DEBUG
	PRINTSTRING("Outside auth catch/loop VerifiedArrayIndex == ");
	PRINTINT(VerifiedArrayIndex);
	PRINTNL();
#endif
	loadCartelAutoScos(); //should add a seperate call to load cartel_helper.sco (W.I.P)
	if (IS_PLAYER_ONLINE())
		DownloadScores();

	SET_PLAYER_INVINCIBLE(GET_PLAYER_ID(), bitTest(ctlMem->m_bit_sets.m_player, playerGodMode)); //

	if (bitTest(*(int*)stickyBitSet, STICK_KICKED_TRYING_TO_JOIN))
	{
		if (IsStrValid(freeMem->m_last_games_host_gamertag))
		{
			WAIT(1000);
			printf("~r~CTL: ~w~Hosts gamertag of last game: %s", freeMem->m_last_games_host_gamertag);
		}
		else
			printf("~r~CTL: ~w~We grabbed the hosts gamertag but it would crash you if we displayed it.");

		bitReset((int*)stickyBitSet, STICK_KICKED_TRYING_TO_JOIN);
	}
	int bullshitTickCount = (GetTickCount() + 2000);
	CarryOverSaveSettings(); //TESTING //won't run SP carryover if screen is faded out.
	int bullshit_memcpy_address = (bullshitTickCount * hidden_number114());
	//NOTE Nath: Could the below be causing the autoflip bug people are reporting? If our PlayerID is != myPlayerIndex at this time due to init?

	if(VerifiedArrayIndex == 35 || VerifiedArrayIndex == 23 || VerifiedArrayIndex == 39) //TootanKhamun / CaliCa1iCali - RGH Winner & E2RY / Izzy (Cartel Menu Giveaway Winner) & f2z (second menu winner)
		bitSet(&ctlMem->m_bit_sets.m_debug, DEBUG_LOCAL_USER_IS_GIVEAWAY_WINNER);

	//SET_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (myPlayerIndex + 16)); //will our playerindex be set in Network when this loads?
	//bitSet(&ctlMem->m_bit_sets.m_vehicle, local_autoflip); //should get overwritten if have saved settings
	while (!(ctlMem->ObfuscatedIdentifiers[VerifiedArrayIndex].m_midObfuscatedLower != ObfuscateMachineIDUniqueHalf(*(int*)(((int)getName(myPlayerIndex) - hiddenSeventySix()) + hidden24())))) //works if we wait for loading to fade (time for MID to get memcpy'd)
	{
		//Make sure our array index is valid and our XUID identifier is not 0
		!ctlMem->ObfuscatedIdentifiers[VerifiedArrayIndex].m_xuidObfuscatedUpper ? freeMem++ : (VerifiedArrayIndex ? WAIT(0) : __throw());
	//	!ObfuscatedIdentifiers[VerifiedArrayIndex].m_xuidObfuscatedUpper ?  hidden_number1355301() : (VerifiedArrayIndex ? WAITUNPAUSED(0) : __throw()); //seems the same lol
	//	!ObfuscatedIdentifiers[VerifiedArrayIndex].m_xuidObfuscatedUpper ?  hidden_number1355301() : (VerifiedArrayIndex ? WAITUNWARPED(0) : __throw()); //not tried idk

		//need to check ObfuscateMachineIDUniqueHalf and ObfuscateXUIDUniqueHalf aren't returning 0 somehow.
		//
		//
		if (bitTest(ctlMem->m_bit_sets.m_vehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED))
		{
			//Make sure our array index is valid and our MID identifier is not 0
			!ctlMem->ObfuscatedIdentifiers[VerifiedArrayIndex].m_midObfuscatedLower ? __throw() : LoopedExecutionEntry(); //our myPlayerPed var gets updated here soooooo

			//make sure the ObfuscateXUID function has been called.
			!bitTest(ctlMem->m_bit_sets.m_player, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED) ? main->m_Stack++ : HandleMenuUi();

			
			//!ObfuscatedIdentifiers[VerifiedArrayIndex].m_midObfuscatedLower ? ctlMem++ : LoopedExecutionEntry();
			//if(ObfuscateMachineIDUniqueHalf(hiddenNumberOfChoosing) == HiddenOutcomeNumber) //check if this functions return values have been changed.
			process_threads();
		}
		else //bullshit logic should never execute.
		{
			memset(main->m_Stack, hidden_number0(), (hidden_number114() * hidden24())); //clear main.sco stack and crash woop
			if (bullshitTickCount < GetTickCount()) //make sure it contains a value. / crash
			{
				RefreshCurrentPage(0, 0);
				bullshitTickCount = (GetTickCount() + 2000); //bullshit logic.
			}
			if (!bullshitTickCount)
				__throw();
		}
	}
	//main->m_Stack++;
	//more bullshit logic that wont ever run under normal circumstances
	memcpy(main->m_Stack, &bullshit_memcpy_address, (hidden_number1355301() * hidden_number97())); //this should really cause a fuck up
	while (IS_BIT_SET(ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, myPlayerIndex)) 
	{
		WAIT(0);
		process_threads();
		RefreshCurrentPage(2, 0);
	}
#endif
}
