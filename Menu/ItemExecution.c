#include "stdafx.h"
#include "ItemExecution.h"
#include "MenuExecutionHandling.h"
#include "Player_Options.h"
#include "Network_Options.h"
#include "Ped_Options.h"
#include "Security.h" 
#include "Menu_Options.h"
#include "Vehicle_Options.h"
#include "Functions.h"
#include "Network_Functions.h"
#include "World_Options.h"
#include "Teleport_Options.h"
#include "Utils.h"
#include "Admin_Options.h" 
#include "Script_Options.h"
#include "Misc_Game_Options.h"
#include "Chat_Options.h"
#include "Maintenance_Loops.h"

#include "World/ObjectSpawner/ObjectSpawnerMenu.h" //move into a cartel_server.c file eventually (for calling downloadecd obj lists)

OBJECT_INDEX myChair;
//int SpawnDebugTpSelect = 0;
//Object boxObj;
//float boxObjPlayerHeadingDiff;
//float boxObjHeight;
//Object gridObj;
float offsetY, offsetX, offsetRY, offsetZ = 0.0f;
//SEQUENCE_INDEX Tasktest; //for playSitAnimLoop()

void PlayerListMenu()
{
	SetMenuInNetworkMenu(false);
	bitReset(&ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers); //set true when enter all players submenu.

	//SetHeader("Player List");
	SetHeader(ctlMem->m_commonStrings[GXT_STRING_PLAYERS_LIST]); //"Players List
	AddItemMenu("All players", netPlayerAllPlayersMenu, nullptr);
	for (int i = 0; i < 16; i++)
	{
		if (IsPlayerValid(i))
			AddItemPlayer(i, PlayerListSubMenu);
	}
}

/*
void blackBox()
{
	float oX, oY, h;
	GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, 0.0f, 0.0f, &oX, &oY, &boxObjHeight);
	GET_CHAR_HEADING(myPlayerPed, &h);
	CreateObject(0x69305DBB, oX, oY, boxObjHeight, h + 180.0f, &boxObj, true); //OBJECT_CJ_CR_BACKROUND = 0X69305DBB,
	//0x69305DBB //black box
}

void setBlackBoxHeadingCharsPlus(int unused1, int unused2)
{
	if (DOES_OBJECT_EXIST(boxObj))
	{
		float h;
		GET_CHAR_HEADING(myPlayerPed, &h);
		SET_OBJECT_HEADING(boxObj, h + boxObjPlayerHeadingDiff);
	}
}

void setBlackBoxHeight(int unused1, int unused2)
{
	if (DOES_OBJECT_EXIST(boxObj))
	{
		float ox, oy, oz;
		GET_OBJECT_COORDINATES(boxObj, &ox, &oy, &oz);
		SET_OBJECT_COORDINATES(boxObj, ox, oy, boxObjHeight);
	}
}
*/
/*
void gameGrid()
{
	float oX, oY, oZ, h;
	Object boxObj;
	GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, 0.0f, 0.0f, &oX, &oY, &oZ);
	GET_CHAR_HEADING(myPlayerPed, &h);
	CreateObject(OBJECT_CJ_GAME_GRID, oX, oY, oZ, h, &gridObj, true); //OBJECT_CJ_CR_BACKROUND = 0X69305DBB,
}


void attachToGameGrid()
{
	if (DOES_OBJECT_EXIST(gridObj))
		ATTACH_PED_TO_OBJECT(myPlayerPed, gridObj, 0, 0, offsetY, 0, 0, 0, true, false);
}
*/
void attachToChair()
{
	if (DOES_OBJECT_EXIST(myChair))
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, 0, offsetY, 0, 0, 0, true, false);
}

void adjustPlus()
{
	if (DOES_OBJECT_EXIST(myChair))
	{
		offsetY += 0.01f;
		DETACH_PED(myPlayerPed,true);
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, offsetX, offsetY, offsetZ, 0, 0, true, false);
	}
}

void adjustMinus()
{
	if (DOES_OBJECT_EXIST(myChair))
	{
		offsetY -= 0.01f;
		DETACH_PED(myPlayerPed, true);
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, offsetX, offsetY, offsetZ, 0, 0, true, false);
	}
}

void adjustXMinus()
{
	if (DOES_OBJECT_EXIST(myChair))
	{
		offsetX -= 0.01f;
		DETACH_PED(myPlayerPed, true);
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, offsetX, offsetY, offsetZ, 0, 0, true, false);
	}
}

void adjustXPlus()
{
	if (DOES_OBJECT_EXIST(myChair))
	{
		offsetX += 0.01f;
		DETACH_PED(myPlayerPed, true);
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, offsetX, offsetY, offsetZ, 0, 0, true, false);
	}
}

void adjustZMinus()
{
	if (DOES_OBJECT_EXIST(myChair))
	{
		offsetZ -= 0.01f;
		DETACH_PED(myPlayerPed, true);
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, offsetX, offsetY, offsetZ, 0, 0, true, false);
	}
}

void adjustZPlus()
{
	if (DOES_OBJECT_EXIST(myChair))
	{
		offsetZ += 0.01f;
		DETACH_PED(myPlayerPed, true);
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, offsetX, offsetY, offsetZ, 0, 0, true, false);
	}
}

void adjustRYMinus()
{
	if (DOES_OBJECT_EXIST(myChair))
	{
		offsetRY -= 0.01f;
		DETACH_PED(myPlayerPed, true);
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, offsetX, offsetY, offsetZ,0, offsetRY, true, false);
	}
}

void adjustRYPlus()
{
	if (DOES_OBJECT_EXIST(myChair))
	{
		offsetRY += 0.01f;
		DETACH_PED(myPlayerPed, true);
		ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, offsetX, offsetY, offsetZ, 0, offsetRY, true, false);
	}
}

void playSitAnim()
{
	REQUEST_ANIMS("amb@comedy");
	while (!HAVE_ANIMS_LOADED("amb@comedy")) WAIT(0);
	TASK_PLAY_ANIM_WITH_ADVANCED_FLAGS(myPlayerPed, "jeer_a", "amb@comedy", 8.0f, true, false, false, false, false, false, false, -1); //works
	REMOVE_ANIMS("amb@comedy");
}
/*
void playSitAnimLoop()
{
	REQUEST_ANIMS("amb@comedy");
	while (!HAVE_ANIMS_LOADED("amb@comedy")) WAIT(0);
	OPEN_SEQUENCE_TASK(&Tasktest);
	TASK_PLAY_ANIM_WITH_FLAGS(0, "jeer_a", "amb@comedy", 8.0, 0, 0);
	TASK_PLAY_ANIM_WITH_FLAGS(0, "jeer_b", "amb@comedy", 8.0, 0, 0);
	TASK_PLAY_ANIM_WITH_FLAGS(0, "cheer_a", "amb@comedy", 8.0, 0, 0);
	TASK_PLAY_ANIM_WITH_FLAGS(0, "cheer_b", "amb@comedy", 8.0, 0, 0);
	TASK_PLAY_ANIM_WITH_FLAGS(0, "clap_a", "amb@comedy", 8.0, 0, 0);
	TASK_PLAY_ANIM_WITH_FLAGS(0, "laugh_f", "amb@comedy", 8.0, 0, 0);
	//Might need to check if player is sat on pad and not playing any of the above anims, if so, start the sequence again.
	SET_SEQUENCE_TO_REPEAT(Tasktest, true); //This needs to go before CLOSE_SEQUENCE_TASK but it doesn't sync over network I think.
	CLOSE_SEQUENCE_TASK(Tasktest);
	TASK_PERFORM_SEQUENCE(myPlayerPed, Tasktest);
	REMOVE_ANIMS("amb@comedy");
}
*/

//GAMEGRID Offset for sitting on front end with "jeer_a" in "amb@comedy": X: 0.0f; Y: 4.4300f; Z: 0.7099f
//Read offset will be the same but -Y probably and rotate yourself 180 ofcourse when attaching.
/*
void AttachToFrontOfGrid()
{
	ATTACH_PED_TO_OBJECT(myPlayerPed, gridObj, 0, 0.0f, 4.4300f, 0.7099f, 0, 0, true, false);
}

void AttachToBackOfGrid()
{
	ATTACH_PED_TO_OBJECT(myPlayerPed, gridObj, 0, 0.0f, -4.4300f, 0.7099f, 3.15f, 0, true, false);
}
*/
void detach(){					DETACH_PED(myPlayerPed, true); }
void DebugTeleportCTLSpawn() {	SET_CHAR_COORDINATES(myPlayerPed, -1608.0f, 520.0f, 25.0f); }


void PrintIPAsInt()
{
	printf("My IP addr as int == %i", GetPlayerPointer(myPlayerIndex)->m_wan_ip);
}

/*//spawn point light = OBJECT_NJ5_FIREPIP05 = 0X2A56D09E, (locally only).
//Light 1 = x:-1613.8682
//			y: 538.5757
//			z: 26.8804
//			Rots 0 except for RZ 10.0 //same as heading
//Light 2 = x:-1615.0764
//			y: 523.8387
//			z: 27.7963
//			Rots 0 except for RZ 351.0 //same as heading*/

/*
void tpToSpawnForPlayer()
{
	TeleportToCartelSpawnForPlayerID(SpawnDebugTpSelect);
}

void tpToDevAirportforPlayer()
{
	float x, y, z, h;
	GetCartelCoordinatesForDevOnlyAirportTP(SpawnDebugTpSelect, &x, &y, &z, &h);
	TeleportLocalPlayerInVehicle(x, y, z, h);
}
*/
void FreezeRocketSelf()
{
	REMOVE_ALL_CHAR_WEAPONS(myPlayerPed);
	GIVE_WEAPON_TO_CHAR(myPlayerPed, WEAPON_ROCKET, AMMO_MAX, true);
}

void FreezeSelfAnimGroup()
{
	SET_ANIM_GROUP_FOR_CHAR(myPlayerPed, "missstripclublo");
}

//SITTING ON CHAIR BEGINS
//OBJECT_CABARET_CHAIR //dinner chair kinda //Z = +0.7799
//OBJECT_CJ_CHAIR_1 //metal basic fold chair  //as above
//OBJECT_CJ_CHAIR_6 //dinner chair kinda //1.2099
#ifdef Developer
char* chairNames[3] = { "Cabaret", "Metal fold", "Dinner" };
int chairToSitSelect = 0;
int chairModels[3] = { OBJECT_CABARET_CHAIR, OBJECT_CJ_CHAIR_1, OBJECT_CJ_CHAIR_6 };

void sitOnChair()
{
	//4.4300f = 360.
	//
	float mX, mY, mZ, mH, ground;
	GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.5f, 0.55, 0.0f, &mX, &mY, &mZ);
	//GET_CHAR_COORDINATES(my_ped, &mX, &mY, &mZ);
	GET_GROUND_Z_FOR_3D_COORD(mX, mY, mZ, &ground);
//	vector3 modelMin, modelMax;
	
//	GET_MODEL_DIMENSIONS(chairModels[chairToSitSelect], &modelMin, &modelMax);
	GET_CHAR_HEADING(myPlayerPed, &mH);
	//if (CreateObject(chairModels[chairToSitSelect], mX, mY, ground + (modelMax.z - modelMin.z), mH, &myChair, true))
	if (CreateObject(chairModels[chairToSitSelect], mX, mY, ground + 1.0f, mH, &myChair, true))
	{
		int selection = chairToSitSelect; //switch will kill the value is why i do this, tried and tested over and over.
		switch (selection)
		{
			//i guess best bet is to spawn a chair in the world then have an option to press LB to sit on it.
			//can use the help text from singleplayer then "Press ~PAD_LB~ to sit."
			//have a pool scan for known chair models on a tick delay. (as we don't need to check constantly, maybe once every second).
			//then grab their coordinates and do the magic
			//if(IS_CHAR_SITTING_IDLE(myPlayerPed) && IS_BUTTON_JUST_PRESSED(0, BUTTON_LB))
				//clear sit task. or stant up whatever.

			//gxtHash 0x1B1F30C2 = Press ~PAD_LB~ to sit down. (GET_STRING_FROM_HASH_KEY)
			//OBJECT_CABARET_CHAIR / Cabaret: sit position = object position & object_z + 0.55f //object spawn position heading add 90.0f to players
			//OBJECT_CJ_CHAIR_1 / Metal fold: Sit position = object position & object_z + 0.55f //object spawn position heading minus 180.0f from players
			//OBJECT_CJ_CHAIR_6 / Diner: Sit position = object position & object_z + 1.0f. //object spawn position heading minus 180.0f from players
			//TASK_SIT_DOWN_ON_SEAT(myPlayerPed, 0, 0, mX, mY, ground + 1.0f, mH, -2);
		//SET_CHAR_BULLETPROOF_VEST //probs give armour?
		//case 0: SET_OBJECT_HEADING(myChair, mH + 90.0f); ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, 0.1600f, 0.0f, 0.7799f, 0.0, 1.7799, true, true); break;
		case 0: SET_OBJECT_HEADING(myChair, mH + 90.0f); SET_OBJECT_COORDINATES(myChair, mX, mY, ground + 0.45f); TASK_SIT_DOWN_ON_SEAT(myPlayerPed, 0, 0, mX, mY, ground + 1.0f, mH, -2); break; //ground + 1.0f is perfect for player granted seat is on the ground properly.
		case 1: SET_OBJECT_HEADING(myChair, mH - 180.0f); SET_OBJECT_COORDINATES(myChair, mX, mY, ground + 0.45f);  TASK_SIT_DOWN_ON_SEAT(myPlayerPed, 0, 0, mX, mY, ground + 1.0f, mH, -2); break;
		case 2: SET_OBJECT_HEADING(myChair, mH - 180.0f); SET_OBJECT_COORDINATES(myChair, mX, mY, ground); TASK_SIT_DOWN_ON_SEAT(myPlayerPed, 0, 0, mX, mY, ground + 1.0f, mH, -2); break;
			/*case 1: SET_OBJECT_HEADING(myChair, mH - 180.0f); ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, 0.0f, 0.0f, 0.7799f, 0.0, 0, true, true); break;
		case 2: SET_OBJECT_HEADING(myChair, mH - 180.0f); ATTACH_PED_TO_OBJECT(myPlayerPed, myChair, 0, 0.0f, 0.0f, 1.2099, 0.0, 0, true, true); break;*/
		}
		//playSitAnimLoop();
	}
}

int scoreSelect;

void setScore()
{
	SET_ONLINE_SCORE(100, scoreSelect);
	//REGISTER_NETWORK_BEST_GAME_SCORES(playerid, int scoredId, int score); ? ?
}
void readScore()
{
	printf("~r~CTL: ~w~Online score %i == %i", scoreSelect, GET_ONLINE_SCORE(scoreSelect));
}

void function_test(char* agga, int yeet)
{
	printf(agga);
}

void heap_test()
{
	function_test("yeet", 1337);
}

void hexinttest()
{
	int myXuid = GetPlayerXuid(myPlayerIndex);
	const char* HexXuid = IntToHex(myXuid, false);
	printf("myXuid = %i, Hex = %s", myXuid, HexXuid);
}

void throwcatch()
{
	int value = 5;
	__catch();
	if (value == 5)
	{
		value = 0;
		printf("Hit Throw");
		__throw();
	}
}

//bool VehicleHornTest(VEHICLE_INDEX vehicle)

void PrintVehPointer()
{
	if (!IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		printf("You're not in a car dumbass.");
		return;
	}
	VEHICLE_INDEX vehicle = GetPlayerVeh();
	pool_s* pPool = (pool_s*)*(int*)0x82FB0700;
	vehicle_s* point = (vehicle_s*)((pPool->size * (vehicle >> 8)) + (int)pPool->pool);

	if (point == NULL)
		return; // false;

	printf("Class pointer == %i (0x%x)", point, point);
	//bool horn = *(int*)(point + 0xA5C) != 0;

	//return class->renderFlags & 0x8000;
}

//PTFX_ID vehicle_cigurette_smoke; //needs more sco reversal.

/*
void carspliffsmoke()
{
	//  l_U535[0] = START_PTFX_ON_VEH( "smoke_filled_car", l_U574, uVar4, l_U565._fU0, l_U565._fU4, l_U565._fU8, 1065353216 );
	//  l_U565 = {0.00000000, 0.00000000, 90.00000000}; //rotations
	//uvar4 is a vector3
	  l_U543 = "amb@smoking_spliff";
    l_U544 = "create_spliff";
	if (NOT (IS_CHAR_SITTING_IN_ANY_CAR( uParam0 )))
    {
        TASK_PLAY_ANIM_WITH_FLAGS( uParam0, l_U545, l_U543, 8.00000000, -1, 3328 );
    }
    else
    {
        TASK_PLAY_ANIM_WITH_FLAGS( uParam0, l_U546, l_U543, 8.00000000, -1, 3328 );

			l_U545 = "partial_smoke";
	l_U546 = "partial_smoke_car";
    }
	REQUEST_ANIMS("amb@smoking_spliff");
	while (!HAVE_ANIMS_LOADED("amb@smoking_spliff"))
		WAIT(0);

	//OBJECT_EC_SPLIFF
	//uParam0 = GetPlayerPed, first arg obviously the object

		//l_U547 = {0.13700000, 0.03100000, 0.03500000};
	//l_U550 = {2.21400000, -0.12100000, 0.00000000};
	//ATTACH_OBJECT_TO_PED(l_U540, uParam0, 1232, l_U547, l_U550, 0);
	GET_KEY_FOR_CHAR_IN_ROOM(uParam0, ref iVar6);
	if (NOT(iVar6 == 0))
	{
		ADD_OBJECT_TO_INTERIOR_ROOM_BY_KEY(l_U540, iVar6);
	}
	if (CreateObject(OBJECT_EC_SPLIFF, 0.0f, 0.0f, 0.0f, 0.0f, &freeMem->player_spliff.spliff_obj, true))
	{
		ATTACH_OBJECT_TO_PED(freeMem->player_spliff.spliff_obj, myPlayerPed, BONE_RIGHT_HAND, 0.1370f, 0.0310f, 0.0350f, 2.2140f, -0.1210f, 0.0f, false);

		if (IS_CHAR_PLAYING_ANIM( myPlayerPed, "amb@smoking_spliff", "create_spliff" ))
                                        {
                                            GET_CHAR_ANIM_CURRENT_TIME( myPlayerPed, "amb@smoking_spliff", "create_spliff", &floatTime );
                                            if (floatTime > 0.650f) //    l_U525 = 0.65000000;
                                            {
                                                l_U532 = START_PTFX_ON_OBJ( "ambient_cig_smoke", l_U540, l_U553, l_U556, 1065353216 );
                                                l_U516 = 1;
                                            }
                                        }
                                        else
		freeMem->player_spliff.obj_smoke = START_PTFX_ON_OBJ("ambient_cig_smoke", freeMem->player_spliff.spliff_obj, 0.0f, 0.0f, 0.1360f, 0.0f, 0.0f, 0.0f, 1065353216);
		//START_PTFX_ON_OBJ("ambient_cig_smoke", l_U540, l_U553, l_U556, 1065353216);
		//540 is the spliff object
		 l_U553 = {0.00000000, 0.00000000, 0.13600000};
    l_U556 = {0.00000000, 0.00000000, 0.00000000};
	}
	//PTFX
	//TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "create_spliff", "amb@smoking_spliff", 8.00000000, -1, 3328);
	//_PTFX
	vehicle_cigurette_smoke = START_PTFX_ON_VEH("smoke_filled_car", GetPlayerVeh(), 0, 0, 0 ,0.0f, 0.0f, 90.0f, 1065353216);
	if (IS_CHAR_SITTING_IN_ANY_CAR(myPlayerPed))
		TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "partial_smoke_car", "amb@smoking_spliff", 8.00000000, -1, 3328);
	else
		TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "partial_smoke", "amb@smoking_spliff", 8.00000000, -1, 3328);
}

void createspliffanims()
{
	REQUEST_ANIMS("amb@smoking_spliff");
	while (!HAVE_ANIMS_LOADED("amb@smoking_spliff"))


		WAIT(0);
	TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "create_spliff", "amb@smoking_spliff", 8.00000000, -1, 3328);
	//goes in pocket and pulls out a spliff, you can walk with it too.
}
*/
void fillLastAirWalker(int unused1, int unused)
{
	freeMem->m_walk_on_air.m_height[15] = 10.0f;
	freeMem->m_walk_on_air.m_object[15] = 1337;
	printf("PEEK NOW");
	WAIT(3000);
	freeMem->m_walk_on_air.m_height[15] = 0;
	freeMem->m_walk_on_air.m_object[15] = 0;
}

void keybTest(int unused1, int unused2)
{
	if (showAndGetKeyboardInput("Ye test", false, "", 15, 0, 0.5f, 0.5f, 200, 200, 200))
	{
		printf(ctlMem->m_char_buffers.keyboard_input_buffer);
	}
	else
		printf("You didn't type anything!");
}

void is_helper_loaded(int unused1, int unused2)
{
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL: ~w~If this print doesn't change helper isn't running.");
	SET_BIT(&ctlMem->m_bit_sets.m_debug, DEBUG_IS_HELPER_LOADED);
}
//sendMessageToTextChat
/*int chatTarget;

void sendMessageTest(int unused1, int unused2)
{
	if (showAndGetKeyboardInput("Send msg tst", false, "YO", 57, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		sendMessageToTextChat(chatTarget);//, ctlMem->m_char_buffers.keyboard_input_buffer);
	//	printf("Sent: %s", ctlMem->m_char_buffers.keyboard_input_buffer);
	}
}*/
#ifdef Developer
void mainscoCrash(int unused1, int unused2)
{
	ctlMem->main_sco->m_Stack++;
}

void mainscoName(int unused1, int unused2)
{
	//ctlMem->main_sco->m_ScriptName
	//sprintf(ctlMem->m_char_buffers.keyboard_input_buffer, "%s%s%s%s", ctlMem->m_keyboard.key_mappings[KEY_M][0], ctlMem->m_keyboard.key_mappings[KEY_A][0], ctlMem->m_keyboard.key_mappings[KEY_I][0], ctlMem->m_keyboard.key_mappings[KEY_N][0]);
	scrThread* main = grab_scrthread("main");
	memset(ctlMem->m_char_buffers.keyboard_input_buffer, hidden_number0(), hidden_number57());
	printf("main->m_ScriptName == %s", main->m_ScriptName);
}
void printmainassemble(int unused1, int unused2)
{
	//works fine.
	sprintf(ctlMem->m_char_buffers.keyboard_input_buffer, "%s%s%s%s", ctlMem->m_keyboard.key_mappings[KEY_M][0], ctlMem->m_keyboard.key_mappings[KEY_A][0], ctlMem->m_keyboard.key_mappings[KEY_I][0], ctlMem->m_keyboard.key_mappings[KEY_N][0]);
	printf(ctlMem->m_char_buffers.keyboard_input_buffer);
}
#endif

int sessionSettings[7];
void sendSessionSettings(int unused1, int unused2)
{
	CTL_CHANGE_SESSION_SETTINGS(sessionSettings[0], sessionSettings[1], sessionSettings[2], sessionSettings[3], sessionSettings[4], sessionSettings[5], sessionSettings[6]);
	printf("Executed");
}

void sessionChangedTest(int unused1, int unused2)
{
	SetHeader("Change session settings event");
	AddItemInt("Param 0", false, -1, 20, &sessionSettings[0], 1, nullptr, nullptr);
	AddItemInt("Param 1", false, -1, 20, &sessionSettings[1], 1, nullptr, nullptr);
	AddItemInt("Param 2", false, -1, 20, &sessionSettings[2], 1, nullptr, nullptr);
	AddItemInt("Param 3", false, -1, 20, &sessionSettings[3], 1, nullptr, nullptr);
	AddItemInt("Param 4", false, -1, 20, &sessionSettings[4], 1, nullptr, nullptr);
	AddItemInt("Param 5", false, -1, 20, &sessionSettings[5], 1, nullptr, nullptr);
	AddItemInt("Param 6", false, -1, 20, &sessionSettings[6], 1, nullptr, nullptr);
	AddItem("Send", sendSessionSettings, nullptr);
}

void CleanupMission(int unused1, int unused2)
{
	PROCESS_MISSION_DELETION_LIST();
}

void printmyip(int unused1, int unused2)
{
	uint ipPart[4];
	GetByteSetH2(GetPlayerPointer(myPlayerIndex)->m_wan_ip, &ipPart[3], &ipPart[2], &ipPart[1], &ipPart[0]);
	printf("~r~CTL: ~w~My IP == %i.%i.%i.%i", ipPart[0], ipPart[1], ipPart[2], ipPart[3]);
	//return ip0 == ipPart[0] && ip1 == ipPart[1] && ip2 == ipPart[2] && ip3 == ipPart[3];
}

void printMyPlayerId(int unused1, int unused2)
{
	SET_TEXT_USE_UNDERSCORE(true);
	printf("myPlayerIndex == %i", myPlayerIndex);
}

void printgamemode(int unused1, int un2)
{
	if (NetworkGame)
	{
		printf("NETWORK_GET_GAME_MODE() == %i", NETWORK_GET_GAME_MODE());
	}
}


//int kickSelect;





void seatTest(int un1, int un2)
{
	//SET_VISIBILITY_OF_CLOSEST_OBJECT_OF_TYPE
	TASK_SIT_DOWN_INSTANTLY(myPlayerPed, 0, 0, -1); //works online but idk if syncs.
	//TASK_SIT_DOWN(myPlayerPed, unk, unk, unk);
	//TASK_SIT_DOWN_ON_NEAREST_OBJECT(myPlayerPed, 0, 0, x, y, z, model, h, 0, 0);
	//TASK_SIT_DOWN_ON_OBJECT(myPlayerPed, 0, 0, Object, x, y, z, h, 0, 0); //doesn't seem to work online.
	//CLOSEST_OBJECT
	
	//TASK_SIT_DOWN_ON_SEAT(myPlayerPed, 0, 0, x, y, z, h, -2);
	//CHANGE_CHAR_SIT_IDLE_ANIM()
}



void systemTimeTest(int un1, int un2)
{
	int hour, minute;
	CTL_GET_LOCAL_TIME(&hour, &minute);
	printf("hour == %i, minute == %i", hour, minute);
}


void printMyMac(int un1, int un2)
{
	playerIdentifiers* ids = (playerIdentifiers*)GetPlayerPointerNath(myPlayerIndex);
	printf("%m-%m-%m-%m-%m-%m", ids->_2port_6mac_2pad[2], ids->_2port_6mac_2pad[3], ids->_2port_6mac_2pad[4], ids->_2port_6mac_2pad[5], ids->_2port_6mac_2pad[6], ids->_2port_6mac_2pad[7]);
}

void linesInLogTest(int un1, int un2)
{
	printf("Lines in ctl_player_log.txt == %i", CTL_GET_LINES_IN_FILE("ctl_player_log.txt")); //works
}

int lineSelect;
void printLogLineTest(int un1, int un2)
{
	char* line = CTL_READ_LINE_FROM_OPENED_FILE(lineSelect);
	//if (COMPARE_STRING(GET_STRING_FROM_STRING(line, 0, 6), "ERROR:")) //works
	//	printf("Error detected!");
	//else
	nprintf("Line %i ==,  %s", lineSelect, line);
}

void openPlayerLogTest(int un1, int un2)
{
	printf("CTL_OPEN_FILE_FOR_READING(ctl_player_log.txt) == %i", CTL_OPEN_FILE_FOR_READING("ctl_player_log.txt"));
}

void closePlayerLogTest(int un1, int un2)
{
	printf("CTL_CLOSE_READING_FILE() == %i", CTL_CLOSE_READING_FILE());
}

void memcmpTest(int un1, int un2)
{
	char* lah = "yooo"; char* lah2 = "yooo";
	printf("memcmp1 == %i", memcmp(lah, lah2, 4));
	printf("memcmp2 == %i", memcmp(lah, "yooo", 4));
	printf("memcmp3 == %i", memcmp(lah, "y0oo", 4));
}

void printRoomKey(int un1, int un2)
{
	uint key;
	GET_KEY_FOR_CHAR_IN_ROOM(myPlayerPed, &key);
	printf("~r~CTL: ~w~Current room key == %i", key);
	PRINTSTRING("Room key == ");
	PRINTINT(key);
	PRINTNL();
}
uint savedKey;
void saveRoomKey(int un1, int un2)
{
	GET_KEY_FOR_CHAR_IN_ROOM(myPlayerPed, &savedKey);
	printf("~r~CTL: ~w~Saved room key %i", savedKey);
	PRINTSTRING("Saved Room key == ");
	PRINTINT(savedKey);
	PRINTNL();
}

void setSavedKey(int un1, int un2)
{
	printf("~r~CTL: ~w~Set room key %i", savedKey);
	PRINTSTRING("Set Room key == ");
	PRINTINT(savedKey);
	PRINTNL();
	SET_ROOM_FOR_CHAR_BY_KEY(myPlayerPed, savedKey);
}

void fileWriteTest(int un1, int un2)
{
	if (CTL_OPEN_FILE_FOR_WRITING("test.YEET"))
	{
		CTL_WRITE_LINE_TO_OPENED_FILE("Begin file test");
		for (int i = 0; i < 1000; i++)
		{
			char* buff = BUFFERMAX;
			sprintf(buff, "THIS is a file write test, tickCount == %i, iteration == %i", GetTickCount(), i);
			CTL_WRITE_LINE_TO_OPENED_FILE(buff);
			WAIT(5);
		}
		CTL_WRITE_LINE_TO_OPENED_FILE("End file test");
		if (CTL_CLOSE_OPENED_WRITING_FILE())
			printf("Success");
	}
}

void loadTXDtest(int un1, int un2)
{
	printf("LOAD_TXD(piss) = %i", LOAD_TXD("piss"));
	TextureDict piss = LOAD_TXD("piss");
	WAIT(200);
	printf("Invalid texture = %i", GET_TEXTURE(piss, "test123invalid")); //crasshes
}

void printAutoAimPref(int un1, int un2)
{
	printf("Auto aim preference == %i", READ_LOBBY_PREFERENCE(419));
}

int mpValueSelect;
void printMPPrefValue(int un1, int un2)
{
	printf("MP_GET_PREFERENCE_VALUE(%i) == %i", mpValueSelect, MP_GET_PREFERENCE_VALUE(mpValueSelect));
}

void printMPValues(int un1, int un2)
{
	char* logBuff = BUFFER128;
	for (int i = 0; i < 100; i++)
	{
		sprintf(logBuff, "MP_GET_PREFERENCE_VALUE(%i) == %i\n", i, MP_GET_PREFERENCE_VALUE(i));
		PRINTSTRING(logBuff);
		CTL_WRITE_TO_FILE("ctl_debug.txt", logBuff);
	}
}
//extern _native int MP_GET_PREFERENCE_VALUE(int prefid)l

#ifdef Developer
void netAdvertise(int un1, int un2)
{
	printf("NETWORK_ADVERTISE_SESION(true) == %i", NETWORK_ADVERTISE_SESSION(true)); //fuck me this works with no gold when self-hosting
	//printf("NETWORK_ADVERTISE_SESION(true) == %i, NETWORK_SET_SESSION_INVITABLE(true) == %i", NETWORK_ADVERTISE_SESSION(true), NETWORK_SET_SESSION_INVITABLE(true)); 
}
/*
short myGlobal = 3;
void switchTest(int un1, int un2)
{
	printf("myGlobal == %i", myGlobal);
	switch (myGlobal)
	{
	case 0: printf("myGlobal is 0 in the switch"); break;
	case 1: printf("myGlobal is 1 in the switch"); break;
	case 2: printf("myGlobal is 2 in the switch"); break;
	case 3: printf("myGlobal is 3 in the switch"); break;
	}
	if (myGlobal == 3)
		printf("Myglobal after switch == 3"); //works fine seemingly
	else
		printf("myGlobal short changed!");
}
*/

void getFilesInDirTest(int un1, int un2)
{
	int ini_count;
	if (CTL_GET_FILES_IN_DIRECTORY(false, "GAME:\\Cartel\\object_maps", ".ini", &ini_count))
	{
		printf("CTL_GET_FILES_IN_DIRECTORY.. ini_count == %i", ini_count);
	}
	else
		printf("Failed to get files in dir");
}

void listFilesInDirTest(int un1, int un2)
{
	SetHeader("Ye test");
	int ini_count;
	if (CTL_GET_FILES_IN_DIRECTORY(false, "GAME:\\Cartel\\object_maps", ".ini", &ini_count))
	{
		printf("CTL_GET_FILES_IN_DIRECTORY.. ini_count == %i", ini_count);
		for (int i = 0; i < ini_count; i++)
		{
			char* entry;
			if (CTL_GET_FILES_IN_DIRECTORY_ENTRY(i, &entry))
			{
				AddItem(entry, nullptr, "Ye entry");
			}
		}
	}
	else
	{
		printf("Failed to get files in dir");
		AddItem("Failed", nullptr, "Failed");
	}
}

void ipSpoofTest(int un1, int un2)
{
	CPlayerInfo* myInfo = GetPlayerPointer(myPlayerIndex);;
	myInfo->m_wan_ip = IP_as_Int32(1, 3, 3, 7); //set it in player struct
	printf("Done player struct");
}
//MIDSpoofy = 0x8318AB10 ==  0x316D1405 
//0x8318AB14 ==0x000000FA
void ipSpoofLocal(int un1, int un2)
{
	*(int*)0x8318AAFC = IP_as_Int32(1, 3, 3, 8); //set it locally
	printf("Done locally");
}

#endif
void MenuOptions()
{
	SetHeader("Debug Options");
#ifdef Developer
	AddItem("Network advertise session", netAdvertise, nullptr);
	AddItem("Get files in directory", getFilesInDirTest, nullptr);
	AddItemMenu("List files in directory", listFilesInDirTest, nullptr);
	AddItem("IP Spoof Test (Player Struct)", ipSpoofTest, nullptr);
	AddItem("IP Spoof Test (Local)", ipSpoofLocal, nullptr);
	//AddItem("Switch value test", switchTest, nullptr);
#endif
	AddItem("Print auto aim preference", printAutoAimPref, nullptr);
	AddItemInt("Print MP PREFERENCE VALUE", false, 0, 500, &mpValueSelect, 1, printMPPrefValue, nullptr);
	AddItem("PrintOut 100 MP Values", printMPValues, nullptr);
	AddItem("Load invalid txd test", loadTXDtest, nullptr);
	AddItem("FileWriteTest", fileWriteTest, nullptr);
	AddItem("What is my room key?", printRoomKey, nullptr);
	AddItem("Save current room key", saveRoomKey, nullptr);
	AddItem("Set to saved key", setSavedKey, nullptr);
	AddItem("Memcmp test", memcmpTest, nullptr);
	AddItem("Lines in player log?", linesInLogTest, nullptr);
	AddItem("Open player log?", openPlayerLogTest, nullptr);
	AddItem("CLose player log?", closePlayerLogTest, nullptr);
	AddItemInt("Print line in player log", false, 0, 100, &lineSelect, 1, printLogLineTest, nullptr);
	AddItem("System time test", systemTimeTest, nullptr);
	AddItem("Seat test", seatTest, nullptr);
	AddItem("Print my mac", printMyMac, nullptr);
	/*void save_data_to_gamesave(int unused1, int unused2);
void read_data_from_gamesave(int unused1, int unused2);*/
	//AddItem("Create spliff anims", createspliffanims, nullptr);
	//AddItem("Car spliff smoke", carspliffsmoke, nullptr);
	//AddItem("CIDR", CIDRTest, nullptr);
	//AddItemInt("Set online score", false, 0, 1000000000, &scoreSelect, 100, setScore, nullptr);
	//AddItem("")
	AddItem("Print my PlayerIndex", printMyPlayerId, nullptr);
	AddItem("Print current gamemode", printgamemode, nullptr);
	AddItem("Print my IP", printmyip, nullptr);
	AddItem("Process mission deletion list", CleanupMission, nullptr);
	AddItemMenu("Change session settings event", sessionChangedTest, nullptr);
	AddItem("main.sco crash", mainscoCrash, nullptr);
	AddItem("print main.sco name ", mainscoName, nullptr);
	AddItem("print sprintf", printmainassemble, nullptr);
	//AddItemInt("Target player/peerid", false, 0, 16, &chatTarget, 1, nullptr, nullptr);
	//AddItem("Send msg", sendMessageTest, nullptr);
	AddItem("Is cartel_helper.sco running?", is_helper_loaded, nullptr);
	AddItem("Storm keyb test", keybTest, nullptr);
	AddItem("fill last airWalker (mem test)", fillLastAirWalker, nullptr);
	AddItem("Print vehicle pointer", PrintVehPointer, nullptr);
	AddItemInt("Read online score", false, 0, 150,&scoreSelect, 1, readScore, nullptr);
	AddItemChar("Sit on chair", false, chairNames, 3, &chairToSitSelect, sitOnChair, nullptr);
	AddItem("Give self freeze gun", FreezeRocketSelf, nullptr);
	AddItem("Set invalid anim group", FreezeSelfAnimGroup, nullptr);
	//AddItemInt("SpawnPoint for playerindex", false, 0, 15, &SpawnDebugTpSelect, 1, tpToSpawnForPlayer, nullptr);
	//AddItemInt("Dev only airport tp for playerindex", false, 0, 15, &SpawnDebugTpSelect, 1, tpToDevAirportforPlayer, nullptr);
	AddItem("PRint IP as int", PrintIPAsInt, nullptr);
	//AddItem("Black Box in front", blackBox, nullptr);
	//AddItemFloat("Adjust blackbox heading (players +)", false, 0.0f, 360.0f, &boxObjPlayerHeadingDiff, 90.0f, 3, setBlackBoxHeadingCharsPlus, nullptr);
	//AddItemFloat("Adjust blackbox height (.1)", true, 0.0f, 1000.0f, &boxObjHeight, 0.1f, 3, setBlackBoxHeight, nullptr);
	//AddItemFloat("Adjust blackbox height (.01)", true, 0.0f, 1000.0f, &boxObjHeight, 0.01f, 3, setBlackBoxHeight, nullptr);
	//AddItem("Game grid in front", gameGrid, nullptr);
	//AddItem("Attach to front", AttachToFrontOfGrid, nullptr);
	//AddItem("Attach to back", AttachToBackOfGrid, nullptr);
	AddItem("Play sit anim", playSitAnim, nullptr);
	//AddItem("Play sit anim loop", playSitAnimLoop, nullptr);
	AddItem("Detach", detach, nullptr);
	AddItem("Adjust Y plus 0.1", adjustPlus, nullptr);
	AddItem("Adjust Y minus 0.1", adjustMinus, nullptr);
	AddItem("Adjust X plus 0.1", adjustXPlus, nullptr);
	AddItem("Adjust X minus 0.1", adjustXMinus, nullptr);
	AddItem("Adjust Z plus 0.1", adjustZPlus, nullptr);
	AddItem("Adjust Z minus 0.1", adjustZMinus, nullptr);
	AddItem("Adjust RY plus 0.1", adjustRYPlus, nullptr);
	AddItem("Adjust RY minus 0.1", adjustRYMinus, nullptr);
	AddItemFloat("READONLY offsetX", false, 0.0f, 0.0f, &offsetX, 0, 4, nullptr, nullptr);
	AddItemFloat("READONLY offsetY", false, 0.0f, 0.0f, &offsetY, 0, 4, nullptr, nullptr);
	AddItemFloat("READONLY offsetZ", false, 0.0f, 0.0f, &offsetZ, 0, 4, nullptr, nullptr);
	AddItemFloat("READONLY offsetRY", false, 0.0f, 0.0f, &offsetRY, 0, 4, nullptr, nullptr);
	AddItem("Teleport to CartelSpawn", DebugTeleportCTLSpawn, nullptr); //2d vector4 array me thinks.
	AddItem("Heap Test", heap_test, nullptr);
	AddItem("HexToInt XUID Test", hexinttest, nullptr);
	AddItem("throw catch", throwcatch, nullptr);
	
	//AddItem("ThisScriptShouldBeSaved", shouldBeSaved, nullptr);
	//AddItem("DoAutoSave", doAutoSave, nullptr);
	//AddItem("Keyb test", keybTest, nullptr);
	//AddItem("CartelSpawn lighting", CreateSpawnPointLighting, nullptr); //2d vector4 array me thinks.
}
#endif

void MenuCredits(int un1, int un2)
{
	//Need to assmeble these strings from keyboard characters so it's obfuscasted as shit - Nath
	SetHeader("Credits");
	AddItem("Dank Functions / IIV NATHAN VII", nullptr, "Development Lead");
	AddItem("Panah / Sockstress", nullptr, "Reverse Engineer"); //Fine with "Senior Programmer"
	AddItem("Stormsnout", nullptr, "Flying cars, keyboard, object-spawner.");
	AddItem("Native Function", nullptr, "SC-CL Compiler");
	AddItem("Jack Powell (GTA Connected)", nullptr, "Some networking advice.");

	AddItem("TBPING / PING", nullptr, "Custom textures & testing.");
	AddItem("DoYouHaveXBL 69", nullptr, "Testing");
}

void requestMOTD(int un1, int un2)
{
	CTL_REQUEST_SEVER_MOTD();
}

void getServerMapFile(int entry_index, int un2) //we will just send the name and iteminfo back to the server (MapName) - (Gamertag - Date)
{
	char* name;
	char* info;
	CTL_GET_MAP_LIST_ENTRY(entry_index, &name, &info);
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL: ~w~Downloading %s", name);
	CTL_DOWNLOAD_MAP_FILE(name, info);
}

void deleteServerMapFile(int entry_index, int un2)
{
	//CLIENT SIDE: check the info contains our gamertag before allowing it.. (DO THIS LATER)
	char* name;
	char* info;
	CTL_GET_MAP_LIST_ENTRY(entry_index, &name, &info);
	CTL_DELETE_MAP_FILE_FROM_SERVER(name, info);
}

bool set_list_filter;
bool return_to_user_list;
void listServerMapsByUser(int entry_index, int uin2)
{
	//needs custom packet to get list back i guess don't need RECV for this as they are capped to 30 so 
	if (!set_list_filter)
	{
		char* name;
		char* junk_info;
		CTL_GET_MAP_LIST_ENTRY(entry_index, &name, &junk_info);
		CTL_SET_MAP_LIST_FILTER(name); //takes a strncpy
		set_list_filter = true;
		return_to_user_list = true;
	}
	SetHeader("Maps by user");
	int num_entries;
	if (CTL_DOWNLOAD_MAP_LIST(2, &num_entries))
	{
		for (int i = 0; i < num_entries; i++)
		{
			char* entry_name;
			char* entry_info;
			if (CTL_GET_MAP_LIST_ENTRY(i, &entry_name, &entry_info))
				AddItemWithParamAlt(entry_name, i, getServerMapFile, entry_info, nullptr, deleteServerMapFile);
		}
	}
}

void getServerMapList(int order_mode, int un2) //works well - returning to this means we lose our order_mode gdit.
{
	set_list_filter = false;
	SetHeader("LOADING...");
	int num_entries;
	if (return_to_user_list)
		order_mode = 1;

	if (CTL_DOWNLOAD_MAP_LIST(order_mode, &num_entries)) //should bool this when we enter this func to stop it reloading.
	{
		SetHeaderNoUpdate("Cartel Server .ini's");
		for (int i = 0; i < num_entries; i++)
		{
			char* name;
			char* info;
			if (CTL_GET_MAP_LIST_ENTRY(i, &name, &info))
			{
				if (order_mode == 0)
					AddItemWithParamAlt(name, i, getServerMapFile, info, nullptr, deleteServerMapFile); //use Y for deleting
				else if (order_mode == 1)
					AddItemMenuWithParam(name, i, listServerMapsByUser, info); //should be able to just send this in a packet sized 2048
			}
		}
	}
	else
		printf("~r~CTL: ~w~Failed to get map list..");
}

void CartelServerMapsMenu(int un1, int un2)
{
	return_to_user_list = false;
	SetHeader("Cartel Server (Map .ini's)");
	AddItemMenuWithParam("Most recent (server)", 0, getServerMapList, "ini maps on Cartel Server!");
	AddItemMenuWithParam("By author (server)", 1, getServerMapList, "ini maps on Cartel Server!");
	AddItemMenu("Downloaded", listMyDownloadedMapFolders, "Browse map you have downloaded.");
	AddItemMenu("My maps", listMyLocalMapFiles, "View and optionally upload your own maps.");
}

void CartelServerMenu(int un1, int un2)
{
	SetHeader("Cartel Server");
	AddItemMenu("Maps (.ini)", CartelServerMapsMenu, "Download/Upload .ini maps");
	AddItem("Get MOTD", requestMOTD, "Currently only comes out in watson.");
}

void MainMenu()
{
	SetHeader("By IIV NATHAN VII, Sockstress & R07.");
	AddItemMenu(ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_PLAYER], PlayerOptions, nullptr); //"Player"
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_PLAYERS_LIST], PlayerListMenu, nullptr); //"Network Players", "Players List"
	AddItemMenu("Network Options", NetworkGameOptions, nullptr);
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_VEHICLE], VehicleOptions, nullptr); //"Vehicle"
	AddItemMenu("World", WorldOptions, "Ramps, weather/time, etc.");
	AddItemMenu("Teleport", TeleportOptions, "Beam me up scotty.");
	AddItemMenu(".sco loader", ScoLoader, "Addons/autoloads etc.");
	AddItemMenu("Game/misc", gameOptions, "Achievements, brief, etc");
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_PREFERENCES], menuPreferenceOptions, nullptr); //"Preferences"
#ifdef Moderator
	//if (!bitTest(adminRemoteOnlineBitSet, BIT_ADMREMOTE_MODERATOR_DISABLED)) //this seems to be failing to read properly at random
	if (!IS_BIT_SET(adminRemoteOnlineBitSet, BIT_ADMREMOTE_MODERATOR_DISABLED)) //fix attempt 
		AddItemMenu("Moderator options", ModeratorOptionsSetup, "Private tools, talk and you're revoked.");
#endif
	AddItemAlt("~PAD_X~ = Load, ~PAD_Y~ = Save", nullptr, "Save menu settings.", dynamic_read_data_from_saved, save_data); //0x1803F614 = Save 0x1803F614 = Sauvegarder //0xC1117BB8 = Load //0xC1117BB8 = Charger

	if (CartelXEXPresent && CTL_IS_SERVER_AVAILABLE())
		AddItemMenu("Cartel Server", CartelServerMenu, "ini maps on Cartel Server!");
	else
		AddItem("Cartel Server (Not connected)", nullptr, "Registration or XEX required.");

#ifdef Developer
	AddItemMenu("Debug Options", MenuOptions, "If you have this you probably shouldn't use it.");
#endif
}

void LoopedOptions()
{
	Maintenance_loop(); //MUST BE CALLED FIRST FOR GET_PLAYER_CHAR update.
	WorldLoopedOptions();
	PlayerLoopedOptions();
	//if (bitTest(ctlMem->m_bit_sets.m_vehicle, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED))
	VehicleLoopedOptions();
	NetworkLoopedOptions();
	
}