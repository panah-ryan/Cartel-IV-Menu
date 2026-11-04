#include "stdafx.h"
#include "Player_Options.h"
#include "MenuExecutionHandling.h"
#include "Functions.h"
#include "Network_Options.h"
#include "World_Options.h"
#include "Utils.h"
#include "Admin_Options.h"
#include "Vehicle_Options.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types" 

void playerActSpeech(char* speachName, int unused1);
void playerActVoice(char* voiceName, int unused1);

#define AddItemPlayerModel(ItemText, Model) AddItemWithParamAlt(ItemText, Model, changePlayerModel, nullptr, nullptr, nullptr)
#define AddItemPlayerVoice(playerVoice) AddItemWithParamAlt(playerVoice, (uint)playerVoice, playerActVoice, "Press ~PAD_A~ to change voice.", nullptr, nullptr) //need to credit Buffeting for this.
#define AddItemPlayerSpeech(playerSpeach) AddItemWithParamAlt(playerSpeach, (uint)playerSpeach, playerActSpeech, "Press ~PAD_A~ to speak.", nullptr, nullptr)
#define AddItemPlayerWeapon(weaponText, weaponModel) AddItemWithParamAlt(weaponText, weaponModel, GivePlayerWeapon, nullptr, nullptr, nullptr);

//int ctlMem->m_bit_sets.m_player = 0; //393224; //131080; // 8;  //spawnedVehsRequireBreakIn and DeleteLastFunnyCarObjs & DeleteLastNeonObjects true by default.
//int ctlMem->m_bit_sets.m_player = 1121; //393224; //131080; // 8;  //spawnedVehsRequireBreakIn and DeleteLastFunnyCarObjs & DeleteLastNeonObjects true by default.
int wantedLvlInt = 1;
int weaponsStringSelect;
int superHeroModelSelect;
int ammoTypeSelect = 0;

float scriptedCamPitch = 0.0f;

void playerOptChangeMultiplayerModel() //I promise all this bullshit is necessary, ACTIVATE_NETWORK_SETTINGS_MENU() shifts the Weather to SUNNY_WINDY_2, if we have bad weather this causes a protection catch, this bullshit negates this.
{
	bool PreventFreezeTime = bitTest(ctlMem->m_bit_sets.m_player, freezeTime); //loop for freeze time is in cartel_helper.c
	if (PreventFreezeTime)
		bitReset(&ctlMem->m_bit_sets.m_player, freezeTime); //Prevent SETTINGS_MENU bailing due to time getting set while it's loading.

	if (bitTest(ctlMem->m_bit_sets.m_player, freezeWeather))
	{
		bitReset(&ctlMem->m_bit_sets.m_player, freezeWeather);
		bitSet(&ctlMem->m_bit_sets.m_debug, DEBUG_ENTERED_PLAYERSETTINGS_MENU_FREEZE_WEATHER_ON_EXIT); //don't fuck with weather while we're in the playersettings menu.
	}

	ACTIVATE_NETWORK_SETTINGS_MENU();
	bitSet(&ctlMem->m_bit_sets.m_debug, DEBUG_ENTERED_PLAYERSETTINGS_MENU_RESTORE_INPUT_ON_EXIT); 

	WAIT(1000); //if we change time while loading SETTINGS_MENU() it will fail. //we have to delay like this due to freeze time being in helper.c (diff thread)
	//the R* player settings menu in question seems to override time while in it so we are fine with this.
	if (PreventFreezeTime)
		bitSet(&ctlMem->m_bit_sets.m_player, freezeTime);
}

void playerOptUpdateMultiplayerModel(int unused1, int unused2)
{
	changePlayerModel(GET_PLAYERSETTINGS_MODEL_CHOICE(), 0);
}

void changePlayerModelObject(uint objModel, float offsetX, float offsetY, float offsetZ, float rX, float rY, float rZ)
{
	INTERIOR_INSTANCE_INDEX myInterior;
	GET_INTERIOR_FROM_CHAR(myPlayerPed, &myInterior);

	if (!IS_MODEL_IN_CDIMAGE(objModel))
	{
		printf("~r~CTL: ~w~Model %i is missing from your image files.", objModel);
		return;
	}

	REQUEST_MODEL(objModel);

	if (DOES_OBJECT_EXIST(attached_object))
	{
		NETWORK_INDEX objNetId;

		GET_NETWORK_ID_FROM_OBJECT(attached_object, &objNetId);
		if (RequestNetworkControl(objNetId, "attached object"))
			DELETE_OBJECT(&attached_object);
	}

	SET_CHAR_VISIBLE(myPlayerPed, false);
	while (!HAS_MODEL_LOADED(objModel)) 
		WAIT(0);

	CreateObject(objModel, 0.0f, 0.0f, 0.0f, 0.0f, &attached_object, true);
	ADD_OBJECT_TO_INTERIOR_ROOM_BY_KEY(attached_object, myInterior);
	SET_OBJECT_HEALTH(attached_object, 165.0f); //make my protection skip
	SET_OBJECT_INVINCIBLE(attached_object, true);
	SET_OBJECT_VISIBLE(attached_object,true);
	ATTACH_OBJECT_TO_PED(attached_object, myPlayerPed, BONE_ROOT, offsetX, offsetY, offsetZ, rX, rY, rZ, false);

	//SET_OBJECT_COLLISION(attachedModelObject, true); //TESTING
	//might need to disable camera collisions or whatever its called
}

void PlayPedAnimations(char* animSet, char* animName)
{
#ifdef AdminOptions
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_ANIMATION))
	{
		strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, animSet, 32);
		SendAdminCommandToPlayer(ADMIN_CMD_READ_STRING_STORE_ANIMATION_SET, 0); //tick will catch this i think if we call the loop..
		int start_time = GetTickCount();
		while (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG))
		{
			AdminSendLoop(); 
			WAIT(0);
		}
		int time_taken_for_first_command_response = (GetTickCount() - start_time);
		WAIT(time_taken_for_first_command_response + 125); //works but misses sometimes, will keep increasing slowly.
		//WAIT(3000); //works with this..
		strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, animName, 32);
		SendAdminCommandToPlayer(ADMIN_CMD_READ_STRING_PLAY_ANIMATION_FROM_SET, 0);

		if(!bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers))
			return;

		bitSet(&ctlMem->m_bit_sets.m_player, loopAnims);
	}
#endif
	REQUEST_ANIMS(animSet);
	while (!HAVE_ANIMS_LOADED(animSet)) 
		WAIT(0);

	if(bitTest(ctlMem->m_bit_sets.m_player, loopAnims))
		TASK_PLAY_ANIM_WITH_ADVANCED_FLAGS(myPlayerPed, animName, animSet, 8.0f, bitTest(ctlMem->m_bit_sets.m_player, loopAnims), false, false, false, false, false, false, -1);
	else
		TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, animName, animSet, 8.0, 0, 0);

	REMOVE_ANIMS(animSet);
}

void playerOptModelsObjectToilet()
{
	changePlayerModelObject(-396920699, 0.0f, 0.0f, -0.9844f, 0.0f, 0.0f, 3.150f);
}

void playerOptModelsObjectSkip()
{
	changePlayerModelObject(-1614959272, 0.0f, 0.0f, -1.0111f, 0.0f, 0.0f, 3.150f);
}

void playerOptModelsObjectLampost()
{
	changePlayerModelObject(1729125334, 0.0f, 0.0f, -1.0144, 0.0f, 0.0f, 0.0f);
}

void playerOptModelsObjectSurveillance()
{
	changePlayerModelObject(-1233684636, 0.0f, -0.7165f, 6.3644f, 0.0f, 0.0f, 0.0f);
}

void playerOptModelsObjectLadder()
{
	changePlayerModelObject(OBJECT_CJ_LADDER_1, 0, 3.0, 1.5, 0, 0, 0);
	SET_OBJECT_COLLISION(attached_object, true);
}

void playerOptModelsObjectSofabed()
{
	changePlayerModelObject(601627119, 0.0f, 0.0f, -0.9933f, 0.0f, 0.0f, 3.10f); //needs adding to loop
}

void playerOptModelsObjectMicrowave()
{
	changePlayerModelObject(1141222731, 0.0f, 0.0f, -0.9950f, 0.0f, 0.0f, 3.10f); //needs adding to loop
}

void playerOptModelsObjectSink()
{
	changePlayerModelObject(2068938297, 0.0f, 0.0f, -0.9972f, 0.0f, 0.0f, 3.10f); //needs adding to loop
}

void playerOptModelsObjectShelves()
{
	changePlayerModelObject(946774332, 0.0f, 0.0f, -1.0022f, 0.0f, 0.0f, 0.0f); //needs adding to loop
}

void playerOptModelsObjectSodaDispenser()
{
	changePlayerModelObject(1367139644, 0.0f, 0.0f, -0.9894, 0.0f, 0.0f, 3.10f); //needs adding to loop
}

void playerOptModelsObjectsLaptop()
{
	changePlayerModelObject(-805425417, 0.0f, 0.0f, -0.9950f, 0.0f, 0.0f, 3.10f); //neeeds adding to loop
}

void playerOptModelsObjectsOfficeChair()
{
	changePlayerModelObject(-96241772, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 3.10f); //needs adding to loop
}

void playerOptModelsObjectsTree()
{
	changePlayerModelObject(-957155803, 0.0f, 0.0f, -1.0022f, 0.0f, 0.0f, 0.0f); //needs adding to loop
}

void playerOptModelsObjectsPlantpot()
{
	changePlayerModelObject(57793320, 0.0f, 0.0f, -1.0033f, 0.0f, 0.0f, 0.0f); //needs adding to loop
}

void playerOptModelsObjectsBurgersign()
{
	changePlayerModelObject(-625491925, 0.0f, 0.0f, 2.1639f, 0.0f, 0.0f, 1.550f); //needs adding to loop
}

void SetPlayerGodMode() //don't loop this.
{
	bool Godmode = bitTest(ctlMem->m_bit_sets.m_player, playerGodMode);

	SET_PLAYER_INVINCIBLE(myPlayerIndex, Godmode); //dop we need anything other than this?
	SET_CHAR_PROOFS(myPlayerPed, Godmode, Godmode, Godmode, Godmode, Godmode);
	SET_CHAR_INVINCIBLE(myPlayerPed, Godmode);
	SET_CHAR_DROWNS_IN_WATER(myPlayerPed, !Godmode);
	SET_CHAR_DROWNS_IN_SINKING_VEHICLE(myPlayerPed, !Godmode);
}

void SetPlayerVisibility()
{
	SET_CHAR_VISIBLE(myPlayerPed, !bitTest(ctlMem->m_bit_sets.m_player, Invisibility));
	if (DOES_OBJECT_EXIST(attached_object))
		SET_OBJECT_VISIBLE(attached_object, true); //SET_CHAR_VISIBLE triggers attached entities visibility aswell.
	//doesn't seem to set object visibility inside inteirors.... hurr durr..
}

void setWantedLvl()
{
	if (wantedLvlInt == 0)
		CLEAR_WANTED_LEVEL(GetMenuPlayerId());
	else
		ALTER_WANTED_LEVEL(GetMenuPlayerId(), wantedLvlInt);

	APPLY_WANTED_LEVEL_CHANGE_NOW(GetMenuPlayerId());

	if (GetMenuPlayerId() != myPlayerIndex)
		printf("~r~CTL: %s's ~w~wanted level set to ~y~%i.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), wantedLvlInt);
}


void playerOptToggleRagdoll()
{
	if (bitTest(ctlMem->m_bit_sets.m_player, playerRagdoll))
	{
		SWITCH_PED_TO_RAGDOLL(myPlayerPed, 20000, 30000, false, false, false, false);
		SWITCH_PED_TO_ANIMATED(myPlayerPed, true);
		GIVE_PLAYER_RAGDOLL_CONTROL(myPlayerIndex, true);
		FREEZE_CHAR_POSITION(myPlayerPed, false); //stops protection fkin us over
	}
	else
	{
		SWITCH_PED_TO_ANIMATED(myPlayerPed, false);
		GIVE_PLAYER_RAGDOLL_CONTROL(myPlayerIndex, false);
	}
}

void disableFlymod()
{
	if (bitTest(ctlMem->m_bit_sets.m_player, flyMod)) //Shouldn't we toggle this before any teleports happen? - No, they will simply arrive in flymod/Nath
	{
		printf("~r~CTL: ~w~Fly mod disabled.");
		bitReset(&ctlMem->m_bit_sets.m_player, flyMod);
		worldOptsToggleFlymod();
	}
}


void worldOptsToggleFlymod()
{
	if (bitTest(ctlMem->m_bit_sets.m_player, flyMod))
	{
		//if (IS_PED_ATTACHED_TO_ANY_CAR(myPed))
		//	DETACH_PED(myPed, true);

		//NEW/TESTING (NOT SURE THIS WONT CAUSE OTHER ISSUES BUT WILL BE OKAY I THINK 
		if (IS_IN_SPECTATOR_MODE())
		{
			netPlayerSpectate(myPlayerIndex, 0);
			/*
			SET_IN_SPECTATOR_MODE(0);
			SET_CHAR_VISIBLE(myPed, true);
			netPlayerSpectating = -1;
			printf("~r~CTL: ~w~Spectate disabled.");
			CAMERA_INDEX playerCamera;
			GET_GAME_CAM(&playerCamera);
			SET_CAM_TARGET_PED(playerCamera, myPed);
			*/
		}
		////////////////////
		safeDetachLocalPlayer(); //ready for flymod loop to catch (Turning ON).
	}
	else
	{
		if (DOES_CAM_EXIST(scriptedCam))
		{
			SET_CAM_ACTIVE(scriptedCam, false);
			SET_CAM_PROPAGATE(scriptedCam, false);
			ACTIVATE_SCRIPTED_CAMS(false, false);
			UNATTACH_CAM(scriptedCam);
			DESTROY_CAM(scriptedCam);

			while (DOES_CAM_EXIST(scriptedCam)) 
				WAIT(0);

			SET_CAM_BEHIND_PED(myPlayerPed);
			scriptedCam = 0;
		}
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
			FREEZE_CAR_POSITION(GetPlayerVeh(), false); //pretty sure this is required

		FREEZE_CHAR_POSITION(myPlayerPed, false);
	}
}

void playerActSpeech(char* speachName, int unused1)
{
	SAY_AMBIENT_SPEECH(myPlayerPed, speachName, 1, 1, 0);

	//TASK_SAY(PED_INDEX ped, int unk);
	//SAY_AMBIENT_SPEECH_WITH_VOICE(myPlayerPed, "RHATEED", "Whatever lil jacob is", 1, 1, 0);
}

void playerActVoice(char* voiceName, int unused1)
{
	SET_AMBIENT_VOICE_NAME(myPlayerPed, voiceName);
}

void changePlayerModel(int playerModel, int unused1)
{
#ifdef AdminOptions
	if(bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_PLAYER_MODEL))
	{ 
		char playerModelBuffer[16];
		strcpy(playerModelBuffer, "", 16); //fill with null terminators.
		straddi(playerModelBuffer, playerModel, 16);
		strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, playerModelBuffer, 16);
		SEND_CLIENT_BROADCAST_VARIABLES_NOW(1); //moved to inside function
		SendAdminCommandToPlayer(ADMIN_CMD_STRING_TO_INT_ADMIN_STRING_CHANGEPLAYER_MODEL, 0);

		if(!bitTest(ctlMem->m_bit_sets.m_vehicle,vehicleSpawnForAllPlayers))
			return;
	}
#endif

	if (!IS_MODEL_IN_CDIMAGE(playerModel))
	{
		printf("~r~CTL: ~w~Model %i is missing from your ISO!", playerModel);
		return;
	}

	REQUEST_MODEL(playerModel);

	float heading;
	WEAPON_TYPE savedWeapons[WEAPON_SLOT_THROWN + 1];
	int savedAmmo[WEAPON_SLOT_THROWN + 1];
	uint key;
	int seatIndex = 0;
	WEAPON_TYPE currentWeapon;
	VEHICLE_INDEX playerVehicle = 0;
	int i, armor, health, currentWeaponAmmo;
	uint ammo1;
#ifdef AdminOptions
	//change all player models? ehhh
#endif

	GET_CHAR_ARMOUR(myPlayerPed, &armor);
	GET_CHAR_HEALTH(myPlayerPed, &health);

	for (i = WEAPON_SLOT_UNARMED; i <= WEAPON_SLOT_THROWN; i++)
		GET_CHAR_WEAPON_IN_SLOT(myPlayerPed, i, &savedWeapons[i], &savedAmmo[i], &ammo1);
	
	GET_CURRENT_CHAR_WEAPON(myPlayerPed, &currentWeapon);
	GET_AMMO_IN_CLIP(myPlayerPed, currentWeapon, &currentWeaponAmmo);

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		PED_INDEX pedInSeat = 0;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &playerVehicle);
		GET_DRIVER_OF_CAR(playerVehicle, &pedInSeat);

		if (pedInSeat == myPlayerPed)
			seatIndex = -1;
		else
		{
			for (i = 0; i < 3; i++)
			{
				GET_CHAR_IN_CAR_PASSENGER_SEAT(playerVehicle, i, &pedInSeat);

				if (myPlayerPed == pedInSeat)
				{
					seatIndex = i;
					break;
				}
			}
		}
	}

	if (DOES_OBJECT_EXIST(attached_object))
	{
		NETWORK_INDEX objNetId;

		GET_NETWORK_ID_FROM_OBJECT(attached_object, &objNetId);
		if (HAS_CONTROL_OF_NETWORK_ID(objNetId))
			DELETE_OBJECT(&attached_object);
	}

	while (!HAS_MODEL_LOADED(playerModel))
		WAIT(0);

	GET_KEY_FOR_CHAR_IN_ROOM(myPlayerPed, &key);
	GET_CHAR_HEADING(myPlayerPed, &heading);
	CHANGE_PLAYER_MODEL(myPlayerIndex, playerModel);
	GET_PLAYER_CHAR(myPlayerIndex, &myPlayerPed); //REQUIRED!! - We just changed ped so our handle has changed, re-grab to avoid nullptr-ing ourselves and crashing :D
	SET_CHAR_INVINCIBLE(myPlayerPed, bitTest(ctlMem->m_bit_sets.m_player, playerGodMode)); //enables keeping of current setting
	SET_CHAR_HEADING(myPlayerPed, heading);
	ADD_ARMOUR_TO_CHAR(myPlayerPed, armor);
	SET_CHAR_HEALTH(myPlayerPed, health);
	if(bitTest(ctlMem->m_bit_sets.m_weapons, forceNeverTargettedPed))
		SET_CHAR_NEVER_TARGETTED(myPlayerPed, true); //don't want to force this if we have it turned off so..

	//Weapons
	for (i = WEAPON_SLOT_UNARMED; i <= WEAPON_SLOT_THROWN; i++)
	{
		if (savedWeapons[i] != currentWeapon)
			GIVE_WEAPON_TO_CHAR(myPlayerPed, savedWeapons[i], savedAmmo[i], false);
	}

	if (currentWeapon)
	{
		int ammo;

		GET_AMMO_IN_CHAR_WEAPON(myPlayerPed, currentWeapon, &ammo);
		REMOVE_WEAPON_FROM_CHAR(myPlayerPed, currentWeapon);
		GIVE_WEAPON_TO_CHAR(myPlayerPed, currentWeapon, ammo, false);
		SET_CURRENT_CHAR_WEAPON(myPlayerPed, currentWeapon, true);
		SET_AMMO_IN_CLIP(myPlayerPed, currentWeapon, currentWeaponAmmo);
		SET_CHAR_AMMO(myPlayerPed, currentWeapon, ammo);
	}

	//Vehicle
	if (DOES_VEHICLE_EXIST(playerVehicle))
	{
		if (seatIndex == -1)
			WARP_CHAR_INTO_CAR(myPlayerPed, playerVehicle);
		else
			WARP_CHAR_INTO_CAR_AS_PASSENGER(myPlayerPed, playerVehicle, seatIndex);
	}

	if (IS_INTERIOR_SCENE()) //can probably do this anyway as it will just set -1 or 0 whatever.
		SET_ROOM_FOR_CHAR_BY_KEY(myPlayerPed, key);

	FORCE_FULL_VOICE(myPlayerPed);

	if (playerModel == GET_PLAYERSETTINGS_MODEL_CHOICE())
		SET_PLAYERSETTINGS_MODEL_VARIATIONS_CHOICE(myPlayerIndex);
	else
		SET_CHAR_RANDOM_COMPONENT_VARIATION(myPlayerPed);

	if (bitTest(ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack))
		DISABLE_PLAYER_AUTO_VEHICLE_EXIT(myPlayerPed, true);
}

void GivePlayerWeapon(WEAPON_TYPE weapon, int unused1)
{
	if (GetMenuPlayerId() == myPlayerIndex)
	{
		GIVE_WEAPON_TO_CHAR(myPlayerPed, weapon, AMMO_MAX, false);
		SET_CURRENT_CHAR_WEAPON(myPlayerPed, weapon, true);
		return;
	}

	if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		PED_INDEX networkPlayerPed;

		GET_PLAYER_CHAR(GetMenuPlayerId(), &networkPlayerPed);
		GIVE_WEAPON_TO_CHAR(networkPlayerPed, weapon, AMMO_MAX, false);
		SET_CURRENT_CHAR_WEAPON(networkPlayerPed, weapon, true);
	}
}

void GivePlayerWeapons()
{
	if (PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		PED_INDEX networkPlayerPed;

		GET_PLAYER_CHAR(GetMenuPlayerId(), &networkPlayerPed);
		GiveWeaponsToPed(networkPlayerPed, weaponsStringSelect, true);
	}
}

void RemovePlayersWeapon()
{
	WEAPON_TYPE currentWeapon;

	if (GetMenuPlayerId() == myPlayerIndex) //this works for singleplayers sake also
	{
		GET_CURRENT_CHAR_WEAPON(myPlayerPed, &currentWeapon);
		REMOVE_WEAPON_FROM_CHAR(myPlayerPed, currentWeapon);
	}
	else
	{
		if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
		{
			PED_INDEX networkPlayerPed;

			GET_PLAYER_CHAR(GetMenuPlayerId(), &networkPlayerPed);
			GET_CURRENT_CHAR_WEAPON(networkPlayerPed, &currentWeapon);
			REMOVE_WEAPON_FROM_CHAR(networkPlayerPed, currentWeapon);
		}
	}
}

void RemoveAllPlayersWeapons()
{
	if (GetMenuPlayerId() == myPlayerIndex)
		REMOVE_ALL_CHAR_WEAPONS(myPlayerPed);
	else
	{
		if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
		{
			PED_INDEX networkPlayerPed;

			GET_PLAYER_CHAR(GetMenuPlayerId(), &networkPlayerPed);
			REMOVE_ALL_CHAR_WEAPONS(networkPlayerPed);
		}
	}
}

void playerOptModelsIG()
{
	SetHeader("Change model -> IG models");
#ifndef Developer
	if (!NetworkGame)
#endif
		AddItemPlayerModel("PLAYER", PED_PLAYER);

	AddItemPlayerModel("M_Y_MULTIPLAYER", PED_M_Y_MULTIPLAYER);
	AddItemPlayerModel("F_Y_MULTIPLAYER", PED_F_Y_MULTIPLAYER);
	AddItemPlayerModel("IG_ANNA", PED_IG_ANNA);
	AddItemPlayerModel("IG_ANTHONY", PED_IG_ANTHONY);
	AddItemPlayerModel("IG_BADMAN", PED_IG_BADMAN);
	AddItemPlayerModel("IG_BERNIE_CRANE", PED_IG_BERNIE_CRANE);
	AddItemPlayerModel("IG_BLEDAR", PED_IG_BLEDAR);
	AddItemPlayerModel("IG_BRIAN", PED_IG_BRIAN);
	AddItemPlayerModel("IG_BRUCIE", PED_IG_BRUCIE);
	AddItemPlayerModel("IG_BULGARIN", PED_IG_BULGARIN);
	AddItemPlayerModel("IG_CHARISE", PED_IG_CHARISE);
	AddItemPlayerModel("IG_CHARLIEUC", PED_IG_CHARLIEUC);
	AddItemPlayerModel("IG_CLARENCE", PED_IG_CLARENCE);
	AddItemPlayerModel("IG_DARDAN", PED_IG_DARDAN);
	AddItemPlayerModel("IG_DARKO", PED_IG_DARKO);
	AddItemPlayerModel("IG_DERRICK_MC", PED_IG_DERRICK_MC);
	AddItemPlayerModel("IG_DMITRI", PED_IG_DMITRI);
	AddItemPlayerModel("IG_DWAYNE", PED_IG_DWAYNE);
	AddItemPlayerModel("IG_EDDIELOW", PED_IG_EDDIELOW);
	AddItemPlayerModel("IG_FAUSTIN", PED_IG_FAUSTIN);
	AddItemPlayerModel("IG_FRANCIS_MC", PED_IG_FRANCIS_MC);
	AddItemPlayerModel("IG_FRENCH_TOM", PED_IG_FRENCH_TOM);
	AddItemPlayerModel("IG_GORDON", PED_IG_GORDON);
	AddItemPlayerModel("IG_GRACIE", PED_IG_GRACIE);
	AddItemPlayerModel("IG_HOSSAN", PED_IG_HOSSAN);
	AddItemPlayerModel("IG_ILYENA", PED_IG_ILYENA);
	AddItemPlayerModel("IG_ISAAC", PED_IG_ISAAC);
	AddItemPlayerModel("IG_IVAN", PED_IG_IVAN);
	AddItemPlayerModel("IG_JASON", PED_IG_JASON);
	AddItemPlayerModel("IG_JEFF", PED_IG_JEFF);
	AddItemPlayerModel("IG_JIMMY", PED_IG_JIMMY);
	AddItemPlayerModel("IG_JOHNNYBIKER", PED_IG_JOHNNYBIKER);
	AddItemPlayerModel("IG_KATEMC", PED_IG_KATEMC);
	AddItemPlayerModel("IG_KENNY", PED_IG_KENNY);
	AddItemPlayerModel("IG_LILJACOB", PED_IG_LILJACOB);
	AddItemPlayerModel("IG_LILJACOBW", PED_IG_LILJACOBW);
	AddItemPlayerModel("IG_LUCA", PED_IG_LUCA);
	AddItemPlayerModel("IG_LUIS", PED_IG_LUIS);
	AddItemPlayerModel("IG_MALLORIE", PED_IG_MALLORIE);
	AddItemPlayerModel("IG_MAMC", PED_IG_MAMC);
	AddItemPlayerModel("IG_MANNY", PED_IG_MANNY);
	AddItemPlayerModel("IG_MARNIE", PED_IG_MARNIE);
	AddItemPlayerModel("IG_MEL", PED_IG_MEL);
	AddItemPlayerModel("IG_MICHAEL", PED_IG_MICHAEL);
	AddItemPlayerModel("IG_MICHELLE", PED_IG_MICHELLE);
	AddItemPlayerModel("IG_MICKEY", PED_IG_MICKEY);
	AddItemPlayerModel("IG_PACKIE_MC", PED_IG_PACKIE_MC);
	AddItemPlayerModel("IG_PATHOS", PED_IG_PATHOS);
	AddItemPlayerModel("IG_PETROVIC", PED_IG_PETROVIC);
	AddItemPlayerModel("IG_PHIL_BELL", PED_IG_PHIL_BELL);
	AddItemPlayerModel("IG_PLAYBOY_X", PED_IG_PLAYBOY_X);
	AddItemPlayerModel("IG_RAY_BOCCINO", PED_IG_RAY_BOCCINO);
	AddItemPlayerModel("IG_RICKY", PED_IG_RICKY);
	AddItemPlayerModel("IG_ROMAN", PED_IG_ROMAN);
	AddItemPlayerModel("IG_ROMANW", PED_IG_ROMANW);
	AddItemPlayerModel("IG_SARAH", PED_IG_SARAH);
	AddItemPlayerModel("IG_TUNA", PED_IG_TUNA);
	AddItemPlayerModel("IG_VINNY_SPAZ", PED_IG_VINNY_SPAZ);
	AddItemPlayerModel("IG_VLAD", PED_IG_VLAD);

	//cutscene
	AddItemPlayerModel("CS_ANDREI", PED_CS_ANDREI);
	AddItemPlayerModel("CS_ANGIE", PED_CS_ANGIE);
	AddItemPlayerModel("CS_BADMAN", PED_CS_BADMAN);
	AddItemPlayerModel("CS_BLEDAR", PED_CS_BLEDAR);
	AddItemPlayerModel("CS_BULGARIN", PED_CS_BULGARIN);
	AddItemPlayerModel("CS_BULGARINHENCH", PED_CS_BULGARINHENCH);
	AddItemPlayerModel("CS_CIA", PED_CS_CIA);
	AddItemPlayerModel("CS_DARDAN", PED_CS_DARDAN);
	AddItemPlayerModel("CS_DAVETHEMATE", PED_CS_DAVETHEMATE);
	AddItemPlayerModel("CS_DMITRI", PED_CS_DMITRI);
	AddItemPlayerModel("CS_EDTHEMATE", PED_CS_EDTHEMATE);
	AddItemPlayerModel("CS_FAUSTIN", PED_CS_FAUSTIN);
	AddItemPlayerModel("CS_FRANCIS", PED_CS_FRANCIS);
	AddItemPlayerModel("CS_HOSSAN", PED_CS_HOSSAN);
	AddItemPlayerModel("CS_ILYENA", PED_CS_ILYENA);
	AddItemPlayerModel("CS_IVAN", PED_CS_IVAN);
	AddItemPlayerModel("CS_JAY", PED_CS_JAY);
	AddItemPlayerModel("CS_JIMMY_PEGORINO", PED_CS_JIMMY_PEGORINO);
	AddItemPlayerModel("CS_MEL", PED_CS_MEL);
	AddItemPlayerModel("CS_MICHELLE", PED_CS_MICHELLE);
	AddItemPlayerModel("CS_MICKEY", PED_CS_MICKEY);
	AddItemPlayerModel("CS_OFFICIAL", PED_CS_OFFICIAL);
	AddItemPlayerModel("CS_RAY_BOCCINO", PED_CS_RAY_BOCCINO);
	AddItemPlayerModel("CS_SERGEI", PED_CS_SERGEI);
	AddItemPlayerModel("CS_VLAD", PED_CS_VLAD);
	AddItemPlayerModel("CS_WHIPPINGGIRL", PED_CS_WHIPPINGGIRL);
	AddItemPlayerModel("CS_MANNY", PED_CS_MANNY);
	AddItemPlayerModel("CS_ANTHONY", PED_CS_ANTHONY);
	AddItemPlayerModel("CS_ASHLEY", PED_CS_ASHLEY);
	AddItemPlayerModel("CS_ASSISTANT", PED_CS_ASSISTANT);
	AddItemPlayerModel("CS_CAPTAIN", PED_CS_CAPTAIN);
	AddItemPlayerModel("CS_CHARLIEUC", PED_CS_CHARLIEUC);
	AddItemPlayerModel("CS_DARKO", PED_CS_DARKO);
	AddItemPlayerModel("CS_DWAYNE", PED_CS_DWAYNE);
	AddItemPlayerModel("CS_ELI_JESTER", PED_CS_ELI_JESTER);
	AddItemPlayerModel("CS_ELIZABETA", PED_CS_ELIZABETA);
	AddItemPlayerModel("CS_GAYTONY", PED_CS_GAYTONY);
	AddItemPlayerModel("CS_GERRYMC", PED_CS_GERRYMC);
	AddItemPlayerModel("CS_GORDON", PED_CS_GORDON);
	AddItemPlayerModel("CS_ISSAC", PED_CS_ISSAC);
	AddItemPlayerModel("CS_JOHNNYTHEBIKER", PED_CS_JOHNNYTHEBIKER);
	AddItemPlayerModel("CS_JONGRAVELLI", PED_CS_JONGRAVELLI);
	AddItemPlayerModel("CS_JORGE", PED_CS_JORGE);
	AddItemPlayerModel("CS_KAT", PED_CS_KAT);
	AddItemPlayerModel("CS_KILLER", PED_CS_KILLER);
	AddItemPlayerModel("CS_LUIS", PED_CS_LUIS);
	AddItemPlayerModel("CS_MAGICIAN", PED_CS_MAGICIAN);
	AddItemPlayerModel("CS_MAMC", PED_CS_MAMC);
	AddItemPlayerModel("CS_MELODY", PED_CS_MELODY);
	AddItemPlayerModel("CS_MITCHCOP", PED_CS_MITCHCOP);
	AddItemPlayerModel("CS_MORI", PED_CS_MORI);
	AddItemPlayerModel("CS_PBXGIRL2", PED_CS_PBXGIRL2);
	AddItemPlayerModel("CS_PHILB", PED_CS_PHILB);
	AddItemPlayerModel("CS_PLAYBOYX", PED_CS_PLAYBOYX);
	AddItemPlayerModel("CS_PRIEST", PED_CS_PRIEST);
	AddItemPlayerModel("CS_RICKY", PED_CS_RICKY);
	AddItemPlayerModel("CS_TOMMY", PED_CS_TOMMY);
	AddItemPlayerModel("CS_TRAMP", PED_CS_TRAMP);
	AddItemPlayerModel("CS_BRIAN", PED_CS_BRIAN);
	AddItemPlayerModel("CS_CHARISE", PED_CS_CHARISE);
	AddItemPlayerModel("CS_CLARENCE", PED_CS_CLARENCE);
	AddItemPlayerModel("CS_EDDIELOW", PED_CS_EDDIELOW);
	AddItemPlayerModel("CS_GRACIE", PED_CS_GRACIE);
	AddItemPlayerModel("CS_JEFF", PED_CS_JEFF);
	AddItemPlayerModel("CS_MARNIE", PED_CS_MARNIE);
	AddItemPlayerModel("CS_MARSHAL", PED_CS_MARSHAL);
	AddItemPlayerModel("CS_PATHOS", PED_CS_PATHOS);
	AddItemPlayerModel("CS_SARAH", PED_CS_SARAH);
	AddItemPlayerModel("CS_ROMAN_D", PED_CS_ROMAN_D);
	AddItemPlayerModel("CS_ROMAN_T", PED_CS_ROMAN_T);
	AddItemPlayerModel("CS_ROMAN_W", PED_CS_ROMAN_W);
	AddItemPlayerModel("CS_BRUCIE_B", PED_CS_BRUCIE_B);
	AddItemPlayerModel("CS_BRUCIE_T", PED_CS_BRUCIE_T);
	AddItemPlayerModel("CS_BRUCIE_W", PED_CS_BRUCIE_W);
	AddItemPlayerModel("CS_BERNIE_CRANEC", PED_CS_BERNIE_CRANEC);
	AddItemPlayerModel("CS_BERNIE_CRANET", PED_CS_BERNIE_CRANET);
	AddItemPlayerModel("CS_BERNIE_CRANEW", PED_CS_BERNIE_CRANEW);
	AddItemPlayerModel("CS_LILJACOB_B", PED_CS_LILJACOB_B);
	AddItemPlayerModel("CS_LILJACOB_J", PED_CS_LILJACOB_J);
	AddItemPlayerModel("CS_MALLORIE_D", PED_CS_MALLORIE_D);
	AddItemPlayerModel("CS_MALLORIE_J", PED_CS_MALLORIE_J);
	AddItemPlayerModel("CS_MALLORIE_W", PED_CS_MALLORIE_W);
	AddItemPlayerModel("CS_DERRICKMC_B", PED_CS_DERRICKMC_B);
	AddItemPlayerModel("CS_DERRICKMC_D", PED_CS_DERRICKMC_D);
	AddItemPlayerModel("CS_MICHAEL_B", PED_CS_MICHAEL_B);
	AddItemPlayerModel("CS_MICHAEL_D", PED_CS_MICHAEL_D);
	AddItemPlayerModel("CS_PACKIEMC_B", PED_CS_PACKIEMC_B);
	AddItemPlayerModel("CS_PACKIEMC_D", PED_CS_PACKIEMC_D);
	AddItemPlayerModel("CS_KATEMC_D", PED_CS_KATEMC_D);
	AddItemPlayerModel("CS_KATEMC_W", PED_CS_KATEMC_W);
}

void playerOptModelsGangMembers()
{
	SetHeader("Change model -> Gang members");
	AddItemPlayerModel("M_Y_GAFR_LO_01", PED_M_Y_GAFR_LO_01);
	AddItemPlayerModel("M_Y_GAFR_LO_02", PED_M_Y_GAFR_LO_02);
	AddItemPlayerModel("M_Y_GAFR_HI_01", PED_M_Y_GAFR_HI_01);
	AddItemPlayerModel("M_Y_GAFR_HI_02", PED_M_Y_GAFR_HI_02);
	AddItemPlayerModel("M_Y_GALB_LO_01", PED_M_Y_GALB_LO_01);
	AddItemPlayerModel("M_Y_GALB_LO_02", PED_M_Y_GALB_LO_02);
	AddItemPlayerModel("M_Y_GALB_LO_03", PED_M_Y_GALB_LO_03);
	AddItemPlayerModel("M_Y_GALB_LO_04", PED_M_Y_GALB_LO_04);
	AddItemPlayerModel("M_M_GBIK_LO_03", PED_M_M_GBIK_LO_03);
	AddItemPlayerModel("M_Y_GBIK_HI_01", PED_M_Y_GBIK_HI_01);
	AddItemPlayerModel("M_Y_GBIK_HI_02", PED_M_Y_GBIK_HI_02);
	AddItemPlayerModel("M_Y_GBIK02_LO_02", PED_M_Y_GBIK02_LO_02);
	AddItemPlayerModel("M_Y_GBIK_LO_01", PED_M_Y_GBIK_LO_01);
	AddItemPlayerModel("M_Y_GBIK_LO_02", PED_M_Y_GBIK_LO_02);
	AddItemPlayerModel("M_Y_GIRI_LO_01", PED_M_Y_GIRI_LO_01);
	AddItemPlayerModel("M_Y_GIRI_LO_02", PED_M_Y_GIRI_LO_02);
	AddItemPlayerModel("M_Y_GIRI_LO_03", PED_M_Y_GIRI_LO_03);
	AddItemPlayerModel("M_M_GJAM_HI_01", PED_M_M_GJAM_HI_01);
	AddItemPlayerModel("M_M_GJAM_HI_02", PED_M_M_GJAM_HI_02);
	AddItemPlayerModel("M_M_GJAM_HI_03", PED_M_M_GJAM_HI_03);
	AddItemPlayerModel("M_Y_GJAM_LO_01", PED_M_Y_GJAM_LO_01);
	AddItemPlayerModel("M_Y_GJAM_LO_02", PED_M_Y_GJAM_LO_02);
	AddItemPlayerModel("M_Y_GKOR_LO_01", PED_M_Y_GKOR_LO_01);
	AddItemPlayerModel("M_Y_GKOR_LO_02", PED_M_Y_GKOR_LO_02);
	AddItemPlayerModel("M_Y_GLAT_LO_01", PED_M_Y_GLAT_LO_01);
	AddItemPlayerModel("M_Y_GLAT_LO_02", PED_M_Y_GLAT_LO_02);
	AddItemPlayerModel("M_Y_GLAT_HI_01", PED_M_Y_GLAT_HI_01);
	AddItemPlayerModel("M_Y_GLAT_HI_02", PED_M_Y_GLAT_HI_02);
	AddItemPlayerModel("M_Y_GMAF_HI_01", PED_M_Y_GMAF_HI_01);
	AddItemPlayerModel("M_Y_GMAF_HI_02", PED_M_Y_GMAF_HI_02);
	AddItemPlayerModel("M_Y_GMAF_LO_01", PED_M_Y_GMAF_LO_01);
	AddItemPlayerModel("M_Y_GMAF_LO_02", PED_M_Y_GMAF_LO_02);
	AddItemPlayerModel("M_O_GRUS_HI_01", PED_M_O_GRUS_HI_01);
	AddItemPlayerModel("M_Y_GRUS_LO_01", PED_M_Y_GRUS_LO_01);
	AddItemPlayerModel("M_Y_GRUS_LO_02", PED_M_Y_GRUS_LO_02);
	AddItemPlayerModel("M_Y_GRUS_HI_02", PED_M_Y_GRUS_HI_02);
	AddItemPlayerModel("M_M_GRU2_HI_01", PED_M_M_GRU2_HI_01);
	AddItemPlayerModel("M_M_GRU2_HI_02", PED_M_M_GRU2_HI_02);
	AddItemPlayerModel("M_M_GRU2_LO_02", PED_M_M_GRU2_LO_02);
	AddItemPlayerModel("M_Y_GRU2_LO_01", PED_M_Y_GRU2_LO_01);
	AddItemPlayerModel("M_M_GTRI_HI_01", PED_M_M_GTRI_HI_01);
	AddItemPlayerModel("M_M_GTRI_HI_02", PED_M_M_GTRI_HI_02);
	AddItemPlayerModel("M_Y_GTRI_LO_01", PED_M_Y_GTRI_LO_01);
	AddItemPlayerModel("M_Y_GTRI_LO_02", PED_M_Y_GTRI_LO_02);
}

void playerOptModelsForegroundFemale()
{
	SetHeader("Change model -> Foreground (Female)");
	AddItemPlayerModel("F_O_MAID_01", PED_F_O_MAID_01);
	AddItemPlayerModel("F_O_BINCO", PED_F_O_BINCO);
	AddItemPlayerModel("F_Y_BANK_01", PED_F_Y_BANK_01);
	AddItemPlayerModel("F_Y_DOCTOR_01", PED_F_Y_DOCTOR_01);
	AddItemPlayerModel("F_Y_GYMGAL_01", PED_F_Y_GYMGAL_01);
	AddItemPlayerModel("F_Y_FF_BURGER_R", PED_F_Y_FF_BURGER_R);
	AddItemPlayerModel("F_Y_FF_CLUCK_R", PED_F_Y_FF_CLUCK_R);
	AddItemPlayerModel("F_Y_FF_RSCAFE", PED_F_Y_FF_RSCAFE);
	AddItemPlayerModel("F_Y_FF_TWCAFE", PED_F_Y_FF_TWCAFE);
	AddItemPlayerModel("F_Y_FF_WSPIZZA_R", PED_F_Y_FF_WSPIZZA_R);
	AddItemPlayerModel("F_Y_HOOKER_01", PED_F_Y_HOOKER_01);
	AddItemPlayerModel("F_Y_HOOKER_03", PED_F_Y_HOOKER_03);
	AddItemPlayerModel("F_Y_NURSE", PED_F_Y_NURSE);
	AddItemPlayerModel("F_Y_STRIPPERC01", PED_F_Y_STRIPPERC01);
	AddItemPlayerModel("F_Y_STRIPPERC02", PED_F_Y_STRIPPERC02);
	AddItemPlayerModel("F_Y_WAITRESS_01", PED_F_Y_WAITRESS_01);
}

void playerOptModelsForegroundMale()
{
	SetHeader("Change model -> Foreground (Male)");
	AddItemPlayerModel("M_M_ALCOHOLIC", PED_M_M_ALCOHOLIC);
	AddItemPlayerModel("M_M_ARMOURED", PED_M_M_ARMOURED);
	AddItemPlayerModel("M_M_BUSDRIVER", PED_M_M_BUSDRIVER);
	AddItemPlayerModel("M_M_CHINATOWN_01", PED_M_M_CHINATOWN_01);
	AddItemPlayerModel("M_M_CRACKHEAD", PED_M_M_CRACKHEAD);
	AddItemPlayerModel("M_M_DOC_SCRUBS_01", PED_M_M_DOC_SCRUBS_01);
	AddItemPlayerModel("M_M_DOCTOR_01", PED_M_M_DOCTOR_01);
	AddItemPlayerModel("M_M_DODGYDOC", PED_M_M_DODGYDOC);
	AddItemPlayerModel("M_M_EECOOK", PED_M_M_EECOOK);
	AddItemPlayerModel("M_M_ENFORCER", PED_M_M_ENFORCER);
	AddItemPlayerModel("M_M_FACTORY_01", PED_M_M_FACTORY_01);
	AddItemPlayerModel("M_M_FATCOP_01", PED_M_M_FATCOP_01);
	AddItemPlayerModel("M_M_FBI", PED_M_M_FBI);
	AddItemPlayerModel("M_M_FEDCO", PED_M_M_FEDCO);
	AddItemPlayerModel("M_M_FIRECHIEF", PED_M_M_FIRECHIEF);
	AddItemPlayerModel("M_M_GUNNUT_01", PED_M_M_GUNNUT_01);
	AddItemPlayerModel("M_M_HELIPILOT_01", PED_M_M_HELIPILOT_01);
	AddItemPlayerModel("M_M_HPORTER_01", PED_M_M_HPORTER_01);
	AddItemPlayerModel("M_M_KOREACOOK_01", PED_M_M_KOREACOOK_01);
	AddItemPlayerModel("M_M_LAWYER_01", PED_M_M_LAWYER_01);
	AddItemPlayerModel("M_M_LAWYER_02", PED_M_M_LAWYER_02);
	AddItemPlayerModel("M_M_LOONYBLACK", PED_M_M_LOONYBLACK);
	AddItemPlayerModel("M_M_PILOT", PED_M_M_PILOT);
	AddItemPlayerModel("M_M_PINDUS_01", PED_M_M_PINDUS_01);
	AddItemPlayerModel("M_M_POSTAL_01", PED_M_M_POSTAL_01);
	AddItemPlayerModel("M_M_SAXPLAYER_01", PED_M_M_SAXPLAYER_01);
	AddItemPlayerModel("M_M_SECURITYMAN", PED_M_M_SECURITYMAN);
	AddItemPlayerModel("M_M_SELLER_01", PED_M_M_SELLER_01);
	AddItemPlayerModel("M_M_SHORTORDER", PED_M_M_SHORTORDER);
	AddItemPlayerModel("M_M_STREETFOOD_01", PED_M_M_STREETFOOD_01);
	AddItemPlayerModel("M_M_SWEEPER", PED_M_M_SWEEPER);
	AddItemPlayerModel("M_M_TAXIDRIVER", PED_M_M_TAXIDRIVER);
	AddItemPlayerModel("M_M_TELEPHONE", PED_M_M_TELEPHONE);
	AddItemPlayerModel("M_M_TENNIS", PED_M_M_TENNIS);
	AddItemPlayerModel("M_M_TRAIN_01", PED_M_M_TRAIN_01);
	AddItemPlayerModel("M_M_TRAMPBLACK", PED_M_M_TRAMPBLACK);
	AddItemPlayerModel("M_M_TRUCKER_01", PED_M_M_TRUCKER_01);
	AddItemPlayerModel("M_O_JANITOR", PED_M_O_JANITOR);
	AddItemPlayerModel("M_O_HOTEL_FOOT", PED_M_O_HOTEL_FOOT);
	AddItemPlayerModel("M_O_MPMOBBOSS", PED_M_O_MPMOBBOSS);
	AddItemPlayerModel("M_Y_AIRWORKER", PED_M_Y_AIRWORKER);
	AddItemPlayerModel("M_Y_BARMAN_01", PED_M_Y_BARMAN_01);
	AddItemPlayerModel("M_Y_BOUNCER_01", PED_M_Y_BOUNCER_01);
	AddItemPlayerModel("M_Y_BOUNCER_02", PED_M_Y_BOUNCER_02);
	AddItemPlayerModel("M_Y_BOWL_01", PED_M_Y_BOWL_01);
	AddItemPlayerModel("M_Y_BOWL_02", PED_M_Y_BOWL_02);
	AddItemPlayerModel("M_Y_CHINVEND_01", PED_M_Y_CHINVEND_01);
	AddItemPlayerModel("M_Y_CLUBFIT", PED_M_Y_CLUBFIT);
	AddItemPlayerModel("M_Y_CONSTRUCT_01", PED_M_Y_CONSTRUCT_01);
	AddItemPlayerModel("M_Y_CONSTRUCT_02", PED_M_Y_CONSTRUCT_02);
	AddItemPlayerModel("M_Y_CONSTRUCT_03", PED_M_Y_CONSTRUCT_03);
	AddItemPlayerModel("M_Y_COP", PED_M_Y_COP);
	AddItemPlayerModel("M_Y_COP_TRAFFIC", PED_M_Y_COP_TRAFFIC);
	AddItemPlayerModel("M_Y_COURIER", PED_M_Y_COURIER);
	AddItemPlayerModel("M_Y_COWBOY_01", PED_M_Y_COWBOY_01);
	AddItemPlayerModel("M_Y_DEALER", PED_M_Y_DEALER);
	AddItemPlayerModel("M_Y_DRUG_01", PED_M_Y_DRUG_01);
	AddItemPlayerModel("M_Y_FF_BURGER_R", PED_M_Y_FF_BURGER_R);
	AddItemPlayerModel("M_Y_FF_CLUCK_R", PED_M_Y_FF_CLUCK_R);
	AddItemPlayerModel("M_Y_FF_RSCAFE", PED_M_Y_FF_RSCAFE);
	AddItemPlayerModel("M_Y_FF_TWCAFE", PED_M_Y_FF_TWCAFE);
	AddItemPlayerModel("M_Y_FF_WSPIZZA_R", PED_M_Y_FF_WSPIZZA_R);
	AddItemPlayerModel("M_Y_FIREMAN", PED_M_Y_FIREMAN);
	AddItemPlayerModel("M_Y_GARBAGE", PED_M_Y_GARBAGE);
	AddItemPlayerModel("M_Y_GOON_01", PED_M_Y_GOON_01);
	AddItemPlayerModel("M_Y_GYMGUY_01", PED_M_Y_GYMGUY_01);
	AddItemPlayerModel("M_Y_MECHANIC_02", PED_M_Y_MECHANIC_02);
	AddItemPlayerModel("M_Y_MODO", PED_M_Y_MODO);
	AddItemPlayerModel("M_Y_NHELIPILOT", PED_M_Y_NHELIPILOT);
	AddItemPlayerModel("M_Y_PERSEUS", PED_M_Y_PERSEUS);
	AddItemPlayerModel("M_Y_PINDUS_01", PED_M_Y_PINDUS_01);
	AddItemPlayerModel("M_Y_PINDUS_02", PED_M_Y_PINDUS_02);
	AddItemPlayerModel("M_Y_PINDUS_03", PED_M_Y_PINDUS_03);
	AddItemPlayerModel("M_Y_PMEDIC", PED_M_Y_PMEDIC);
	AddItemPlayerModel("M_Y_PRISON", PED_M_Y_PRISON);
	AddItemPlayerModel("M_Y_PRISONAOM", PED_M_Y_PRISONAOM);
	AddItemPlayerModel("M_Y_ROMANCAB", PED_M_Y_ROMANCAB);
	AddItemPlayerModel("M_Y_RUNNER", PED_M_Y_RUNNER);
	AddItemPlayerModel("M_Y_SHOPASST_01", PED_M_Y_SHOPASST_01);
	AddItemPlayerModel("M_Y_STROOPER", PED_M_Y_STROOPER);
	AddItemPlayerModel("M_Y_SWAT", PED_M_Y_SWAT);
	AddItemPlayerModel("M_Y_SWORDSWALLOW", PED_M_Y_SWORDSWALLOW);
	AddItemPlayerModel("M_Y_THIEF", PED_M_Y_THIEF);
	AddItemPlayerModel("M_Y_VALET", PED_M_Y_VALET);
	AddItemPlayerModel("M_Y_VENDOR", PED_M_Y_VENDOR);
	AddItemPlayerModel("M_Y_FRENCHTOM", PED_M_Y_FRENCHTOM);
	AddItemPlayerModel("M_Y_JIM_FITZ", PED_M_Y_JIM_FITZ);
}

void playerOptModelsBackgroundFemale()
{
	SetHeader("Change model -> Background (Female)");
	AddItemPlayerModel("F_O_PEASTEURO_01", PED_F_O_PEASTEURO_01);
	AddItemPlayerModel("F_O_PEASTEURO_02", PED_F_O_PEASTEURO_02);
	AddItemPlayerModel("F_O_PHARBRON_01", PED_F_O_PHARBRON_01);
	AddItemPlayerModel("F_O_PJERSEY_01", PED_F_O_PJERSEY_01);
	AddItemPlayerModel("F_O_PORIENT_01", PED_F_O_PORIENT_01);
	AddItemPlayerModel("F_O_RICH_01", PED_F_O_RICH_01);
	AddItemPlayerModel("F_M_BUSINESS_01", PED_F_M_BUSINESS_01);
	AddItemPlayerModel("F_M_BUSINESS_02", PED_F_M_BUSINESS_02);
	AddItemPlayerModel("F_M_CHINATOWN", PED_F_M_CHINATOWN);
	AddItemPlayerModel("F_M_PBUSINESS", PED_F_M_PBUSINESS);
	AddItemPlayerModel("F_M_PEASTEURO_01", PED_F_M_PEASTEURO_01);
	AddItemPlayerModel("F_M_PHARBRON_01", PED_F_M_PHARBRON_01);
	AddItemPlayerModel("F_M_PJERSEY_01", PED_F_M_PJERSEY_01);
	AddItemPlayerModel("F_M_PJERSEY_02", PED_F_M_PJERSEY_02);
	AddItemPlayerModel("F_M_PLATIN_01", PED_F_M_PLATIN_01);
	AddItemPlayerModel("F_M_PLATIN_02", PED_F_M_PLATIN_02);
	AddItemPlayerModel("F_M_PMANHAT_01", PED_F_M_PMANHAT_01);
	AddItemPlayerModel("F_M_PMANHAT_02", PED_F_M_PMANHAT_02);
	AddItemPlayerModel("F_M_PORIENT_01", PED_F_M_PORIENT_01);
	AddItemPlayerModel("F_M_PRICH_01", PED_F_M_PRICH_01);
	AddItemPlayerModel("F_Y_BUSINESS_01", PED_F_Y_BUSINESS_01);
	AddItemPlayerModel("F_Y_CDRESS_01", PED_F_Y_CDRESS_01);
	AddItemPlayerModel("F_Y_PBRONX_01", PED_F_Y_PBRONX_01);
	AddItemPlayerModel("F_Y_PCOOL_01", PED_F_Y_PCOOL_01);
	AddItemPlayerModel("F_Y_PCOOL_02", PED_F_Y_PCOOL_02);
	AddItemPlayerModel("F_Y_PEASTEURO_01", PED_F_Y_PEASTEURO_01);
	AddItemPlayerModel("F_Y_PHARBRON_01", PED_F_Y_PHARBRON_01);
	AddItemPlayerModel("F_Y_PHARLEM_01", PED_F_Y_PHARLEM_01);
	AddItemPlayerModel("F_Y_PJERSEY_02", PED_F_Y_PJERSEY_02);
	AddItemPlayerModel("F_Y_PLATIN_01", PED_F_Y_PLATIN_01);
	AddItemPlayerModel("F_Y_PLATIN_02", PED_F_Y_PLATIN_02);
	AddItemPlayerModel("F_Y_PLATIN_03", PED_F_Y_PLATIN_03);
	AddItemPlayerModel("F_Y_PMANHAT_01", PED_F_Y_PMANHAT_01);
	AddItemPlayerModel("F_Y_PMANHAT_02", PED_F_Y_PMANHAT_02);
	AddItemPlayerModel("F_Y_PMANHAT_03", PED_F_Y_PMANHAT_03);
	AddItemPlayerModel("F_Y_PORIENT_01", PED_F_Y_PORIENT_01);
	AddItemPlayerModel("F_Y_PQUEENS_01", PED_F_Y_PQUEENS_01);
	AddItemPlayerModel("F_Y_PRICH_01", PED_F_Y_PRICH_01);
	AddItemPlayerModel("F_Y_PVILLBO_02", PED_F_Y_PVILLBO_02);
	AddItemPlayerModel("F_Y_SHOP_03", PED_F_Y_SHOP_03);
	AddItemPlayerModel("F_Y_SHOP_04", PED_F_Y_SHOP_04);
	AddItemPlayerModel("F_Y_SHOPPER_05", PED_F_Y_SHOPPER_05);
	AddItemPlayerModel("F_Y_SOCIALITE", PED_F_Y_SOCIALITE);
	AddItemPlayerModel("F_Y_STREET_02", PED_F_Y_STREET_02);
	AddItemPlayerModel("F_Y_STREET_05", PED_F_Y_STREET_05);
	AddItemPlayerModel("F_Y_STREET_09", PED_F_Y_STREET_09);
	AddItemPlayerModel("F_Y_STREET_12", PED_F_Y_STREET_12);
	AddItemPlayerModel("F_Y_STREET_30", PED_F_Y_STREET_30);
	AddItemPlayerModel("F_Y_STREET_34", PED_F_Y_STREET_34);
	AddItemPlayerModel("F_Y_TOURIST_01", PED_F_Y_TOURIST_01);
	AddItemPlayerModel("F_Y_VILLBO_01 ", PED_F_Y_VILLBO_01);
}

void playerOptModelsBackgroundMale()
{
	SetHeader("Change model -> Background (Male)");
	AddItemPlayerModel("M_M_BUSINESS_02", PED_M_M_BUSINESS_02);
	AddItemPlayerModel("M_M_BUSINESS_03", PED_M_M_BUSINESS_03);
	AddItemPlayerModel("M_M_EE_HEAVY_01", PED_M_M_EE_HEAVY_01);
	AddItemPlayerModel("M_M_EE_HEAVY_02", PED_M_M_EE_HEAVY_02);
	AddItemPlayerModel("M_M_FATMOB_01", PED_M_M_FATMOB_01);
	AddItemPlayerModel("M_M_GAYMID", PED_M_M_GAYMID);
	AddItemPlayerModel("M_M_GENBUM_01", PED_M_M_GENBUM_01);
	AddItemPlayerModel("M_M_LOONYWHITE", PED_M_M_LOONYWHITE);
	AddItemPlayerModel("M_M_MIDTOWN_01", PED_M_M_MIDTOWN_01);
	AddItemPlayerModel("M_M_PBUSINESS_01", PED_M_M_PBUSINESS_01);
	AddItemPlayerModel("M_M_PEASTEURO_01", PED_M_M_PEASTEURO_01);
	AddItemPlayerModel("M_M_PHARBRON_01", PED_M_M_PHARBRON_01);
	AddItemPlayerModel("M_M_PINDUS_02", PED_M_M_PINDUS_02);
	AddItemPlayerModel("M_M_PITALIAN_01", PED_M_M_PITALIAN_01);
	AddItemPlayerModel("M_M_PITALIAN_02", PED_M_M_PITALIAN_02);
	AddItemPlayerModel("M_M_PLATIN_01", PED_M_M_PLATIN_01);
	AddItemPlayerModel("M_M_PLATIN_02", PED_M_M_PLATIN_02);
	AddItemPlayerModel("M_M_PLATIN_03", PED_M_M_PLATIN_03);
	AddItemPlayerModel("M_M_PMANHAT_01", PED_M_M_PMANHAT_01);
	AddItemPlayerModel("M_M_PMANHAT_02", PED_M_M_PMANHAT_02);
	AddItemPlayerModel("M_M_PORIENT_01", PED_M_M_PORIENT_01);
	AddItemPlayerModel("M_M_PRICH_01", PED_M_M_PRICH_01);
	AddItemPlayerModel("M_O_EASTEURO_01", PED_M_O_EASTEURO_01);
	AddItemPlayerModel("M_O_HASID_01", PED_M_O_HASID_01);
	AddItemPlayerModel("M_O_MOBSTER", PED_M_O_MOBSTER);
	AddItemPlayerModel("M_O_PEASTEURO_02", PED_M_O_PEASTEURO_02);
	AddItemPlayerModel("M_O_PHARBRON_01", PED_M_O_PHARBRON_01);
	AddItemPlayerModel("M_O_PJERSEY_01", PED_M_O_PJERSEY_01);
	AddItemPlayerModel("M_O_STREET_01", PED_M_O_STREET_01);
	AddItemPlayerModel("M_O_SUITED", PED_M_O_SUITED);
	AddItemPlayerModel("M_Y_BOHO_01", PED_M_Y_BOHO_01);
	AddItemPlayerModel("M_Y_BOHOGUY_01", PED_M_Y_BOHOGUY_01);
	AddItemPlayerModel("M_Y_BRONX_01", PED_M_Y_BRONX_01);
	AddItemPlayerModel("M_Y_BUSINESS_01", PED_M_Y_BUSINESS_01);
	AddItemPlayerModel("M_Y_BUSINESS_02", PED_M_Y_BUSINESS_02);
	AddItemPlayerModel("M_Y_CHINATOWN_03", PED_M_Y_CHINATOWN_03);
	AddItemPlayerModel("M_Y_CHOPSHOP_01", PED_M_Y_CHOPSHOP_01);
	AddItemPlayerModel("M_Y_CHOPSHOP_02", PED_M_Y_CHOPSHOP_02);
	AddItemPlayerModel("M_Y_DODGY_01", PED_M_Y_DODGY_01);
	AddItemPlayerModel("M_Y_DORK_02", PED_M_Y_DORK_02);
	AddItemPlayerModel("M_Y_DOWNTOWN_01", PED_M_Y_DOWNTOWN_01);
	AddItemPlayerModel("M_Y_DOWNTOWN_02", PED_M_Y_DOWNTOWN_02);
	AddItemPlayerModel("M_Y_DOWNTOWN_03", PED_M_Y_DOWNTOWN_03);
	AddItemPlayerModel("M_Y_GAYYOUNG", PED_M_Y_GAYYOUNG);
	AddItemPlayerModel("M_Y_GENSTREET_11", PED_M_Y_GENSTREET_11);
	AddItemPlayerModel("M_Y_GENSTREET_16", PED_M_Y_GENSTREET_16);
	AddItemPlayerModel("M_Y_GENSTREET_20", PED_M_Y_GENSTREET_20);
	AddItemPlayerModel("M_Y_GENSTREET_34", PED_M_Y_GENSTREET_34);
	AddItemPlayerModel("M_Y_HARDMAN_01", PED_M_Y_HARDMAN_01);
	AddItemPlayerModel("M_Y_HARLEM_01", PED_M_Y_HARLEM_01);
	AddItemPlayerModel("M_Y_HARLEM_02", PED_M_Y_HARLEM_02);
	AddItemPlayerModel("M_Y_HARLEM_04", PED_M_Y_HARLEM_04);
	AddItemPlayerModel("M_Y_HASID_01", PED_M_Y_HASID_01);
	AddItemPlayerModel("M_Y_LEASTSIDE_01", PED_M_Y_LEASTSIDE_01);
	AddItemPlayerModel("M_Y_PBRONX_01", PED_M_Y_PBRONX_01);
	AddItemPlayerModel("M_Y_PCOOL_01", PED_M_Y_PCOOL_01);
	AddItemPlayerModel("M_Y_PCOOL_02", PED_M_Y_PCOOL_02);
	AddItemPlayerModel("M_Y_PEASTEURO_01", PED_M_Y_PEASTEURO_01);
	AddItemPlayerModel("M_Y_PHARBRON_01", PED_M_Y_PHARBRON_01);
	AddItemPlayerModel("M_Y_PHARLEM_01", PED_M_Y_PHARLEM_01);
	AddItemPlayerModel("M_Y_PJERSEY_01", PED_M_Y_PJERSEY_01);
	AddItemPlayerModel("M_Y_PLATIN_01", PED_M_Y_PLATIN_01);
	AddItemPlayerModel("M_Y_PLATIN_02", PED_M_Y_PLATIN_02);
	AddItemPlayerModel("M_Y_PLATIN_03", PED_M_Y_PLATIN_03);
	AddItemPlayerModel("M_Y_PMANHAT_01", PED_M_Y_PMANHAT_01);
	AddItemPlayerModel("M_Y_PMANHAT_02", PED_M_Y_PMANHAT_02);
	AddItemPlayerModel("M_Y_PORIENT_01", PED_M_Y_PORIENT_01);
	AddItemPlayerModel("M_Y_PQUEENS_01", PED_M_Y_PQUEENS_01);
	AddItemPlayerModel("M_Y_PRICH_01", PED_M_Y_PRICH_01);
	AddItemPlayerModel("M_Y_PVILLBO_01", PED_M_Y_PVILLBO_01);
	AddItemPlayerModel("M_Y_PVILLBO_02", PED_M_Y_PVILLBO_02);
	AddItemPlayerModel("M_Y_PVILLBO_03", PED_M_Y_PVILLBO_03);
	AddItemPlayerModel("M_Y_QUEENSBRIDGE", PED_M_Y_QUEENSBRIDGE);
	AddItemPlayerModel("M_Y_SHADY_02", PED_M_Y_SHADY_02);
	AddItemPlayerModel("M_Y_SKATEBIKE_01", PED_M_Y_SKATEBIKE_01);
	AddItemPlayerModel("M_Y_SOHO_01", PED_M_Y_SOHO_01);
	AddItemPlayerModel("M_Y_STREET_01", PED_M_Y_STREET_01);
	AddItemPlayerModel("M_Y_STREET_03", PED_M_Y_STREET_03);
	AddItemPlayerModel("M_Y_STREET_04", PED_M_Y_STREET_04);
	AddItemPlayerModel("M_Y_STREETBLK_02", PED_M_Y_STREETBLK_02);
	AddItemPlayerModel("M_Y_STREETBLK_03", PED_M_Y_STREETBLK_03);
	AddItemPlayerModel("M_Y_STREETPUNK_02", PED_M_Y_STREETPUNK_02);
	AddItemPlayerModel("M_Y_STREETPUNK_04", PED_M_Y_STREETPUNK_04);
	AddItemPlayerModel("M_Y_STREETPUNK_05", PED_M_Y_STREETPUNK_05);
	AddItemPlayerModel("M_Y_TOUGH_05 ", PED_M_Y_TOUGH_05);
}

void playerOptModelsIGTlad()
{
	SetHeader("IG models (TLaD)");
	AddItemPlayerModel("IG_ASHLEYA", PED_IG_ASHLEYA);
	AddItemPlayerModel("IG_BILLY", PED_IG_BILLY);
	AddItemPlayerModel("IG_BILLYPRISON", PED_IG_BILLYPRISON);
	AddItemPlayerModel("IG_BRIANJ", PED_IG_BRIANJ);
	AddItemPlayerModel("IG_CLAY", PED_IG_CLAY);
	AddItemPlayerModel("IG_DAVE_GROSSMAN", PED_IG_DAVE_GROSSMAN);
	AddItemPlayerModel("IG_DESEAN", PED_IG_DESEAN);
	AddItemPlayerModel("IG_EVAN", PED_IG_EVAN);
	AddItemPlayerModel("IG_JIM_FITZ", PED_IG_JIM_FITZ);
	AddItemPlayerModel("IG_LOSTGIRL", PED_IG_LOSTGIRL);
	AddItemPlayerModel("IG_MALC", PED_IG_MALC);
	AddItemPlayerModel("IG_MARTA", PED_IG_MARTA);
	AddItemPlayerModel("IG_MATTHEWS", PED_IG_MATTHEWS);
	AddItemPlayerModel("IG_MCCORNISH", PED_IG_MCCORNISH);
	AddItemPlayerModel("IG_PGIRL_01", PED_IG_PGIRL_01);
	AddItemPlayerModel("IG_PGIRL_02", PED_IG_PGIRL_02);
	AddItemPlayerModel("IG_ROMAN_E1", PED_IG_ROMAN_E1);
	AddItemPlayerModel("IG_STROOPER", PED_IG_STROOPER);
	AddItemPlayerModel("IG_TERRY", PED_IG_TERRY);
	AddItemPlayerModel("CS_NIKO", PED_CS_NIKO);
	AddItemPlayerModel("CS_ANDREAS", PED_CS_ANDREAS);
	AddItemPlayerModel("CS_ANGELGUY", PED_CS_ANGELGUY);
	AddItemPlayerModel("CS_ANGUS", PED_CS_ANGUS);
	AddItemPlayerModel("CS_ASHLEYA", PED_CS_ASHLEYA);
	AddItemPlayerModel("CS_BILLY", PED_CS_BILLY);
	AddItemPlayerModel("CS_BIKESTRIPPER", PED_CS_BIKESTRIPPER);
	AddItemPlayerModel("CS_BRIANJ", PED_CS_BRIANJ);
	AddItemPlayerModel("CS_CLAY", PED_CS_CLAY);
	AddItemPlayerModel("CS_DAVE_GROSSMAN", PED_CS_DAVE_GROSSMAN);
	AddItemPlayerModel("CS_DESEAN", PED_CS_DESEAN);
	AddItemPlayerModel("CS_ELIZABETAT", PED_CS_ELIZABETAT);
	AddItemPlayerModel("CS_FRANKIE", PED_CS_FRANKIE);
	AddItemPlayerModel("CS_JASON", PED_CS_JASON);
	AddItemPlayerModel("CS_JIM_FITZ", PED_CS_JIM_FITZ);
	AddItemPlayerModel("CS_MAITRE_D", PED_CS_MAITRE_D);
	AddItemPlayerModel("CS_MALC", PED_CS_MALC);
	AddItemPlayerModel("CS_MITCHB", PED_CS_MITCHB);
	AddItemPlayerModel("CS_MARTA", PED_CS_MARTA);
	AddItemPlayerModel("CS_MASSEUSE", PED_CS_MASSEUSE);
	AddItemPlayerModel("CS_MATTHEWS", PED_CS_MATTHEWS);
	AddItemPlayerModel("CS_MCCORNISH", PED_CS_MCCORNISH);
	AddItemPlayerModel("CS_PAUL_LAMONT", PED_CS_PAUL_LAMONT);
	AddItemPlayerModel("CS_PRISONDLC_01", PED_CS_PRISONDLC_01);
	AddItemPlayerModel("CS_PRISONGUARD", PED_CS_PRISONGUARD);
	AddItemPlayerModel("CS_RAYGOON", PED_CS_RAYGOON);
	AddItemPlayerModel("CS_RAY_LEATHER", PED_CS_RAY_LEATHER);
	AddItemPlayerModel("CS_RONCERO", PED_CS_RONCERO);
	AddItemPlayerModel("CS_RUSSIAN_GOON", PED_CS_RUSSIAN_GOON);
	AddItemPlayerModel("CS_RUSSIANGOON2", PED_CS_RUSSIANGOON2);
	AddItemPlayerModel("CS_TERRY", PED_CS_TERRY);
	AddItemPlayerModel("CS_SKANK", PED_CS_SKANK);
	AddItemPlayerModel("CS_STUBBS", PED_CS_STUBBS);
	AddItemPlayerModel("CS_STUBBS_SUIT", PED_CS_STUBBS_SUIT);
}

void playerOptModelsGangMembersFemaleTlad()
{
	SetHeader("Gangs members (Female)(TLaD)");
	AddItemPlayerModel("F_Y_GANGELS_01", PED_F_Y_GANGELS_01);
	AddItemPlayerModel("F_Y_GANGELS_02", PED_F_Y_GANGELS_02);
	AddItemPlayerModel("F_Y_GANGELS_03", PED_F_Y_GANGELS_03);
	AddItemPlayerModel("F_Y_GLOST_01", PED_F_Y_GLOST_01);
	AddItemPlayerModel("F_Y_GLOST_02", PED_F_Y_GLOST_02);
	AddItemPlayerModel("F_Y_GLOST_03", PED_F_Y_GLOST_03);
	AddItemPlayerModel("F_Y_GLOST_04", PED_F_Y_GLOST_04);
	AddItemPlayerModel("F_Y_GRYDERS_01", PED_F_Y_GRYDERS_01);
}

void playerOptModelsGangMembersMaleTlad()
{
	SetHeader("Gangs members (Male)(TLaD)");
	AddItemPlayerModel("M_Y_GANGELS_01", PED_M_Y_GANGELS_01);
	AddItemPlayerModel("M_Y_GANGELS_02", PED_M_Y_GANGELS_02);
	AddItemPlayerModel("M_Y_GANGELS_03", PED_M_Y_GANGELS_03);
	AddItemPlayerModel("M_Y_GANGELS_04", PED_M_Y_GANGELS_04);
	AddItemPlayerModel("M_Y_GANGELS_05", PED_M_Y_GANGELS_05);
	AddItemPlayerModel("M_Y_GLOST_01", PED_M_Y_GLOST_01);
	AddItemPlayerModel("M_Y_GLOST_02", PED_M_Y_GLOST_02);
	AddItemPlayerModel("M_Y_GLOST_03", PED_M_Y_GLOST_03);
	AddItemPlayerModel("M_Y_GLOST_04", PED_M_Y_GLOST_04);
	AddItemPlayerModel("M_Y_GLOST_05", PED_M_Y_GLOST_05);
	AddItemPlayerModel("M_Y_GLOST_06", PED_M_Y_GLOST_06);
	AddItemPlayerModel("M_Y_GRYDERS_01", PED_M_Y_GRYDERS_01);
	AddItemPlayerModel("M_Y_GRYDERS_02", PED_M_Y_GRYDERS_02);
	AddItemPlayerModel("M_Y_GTRIAD_HI_01", PED_M_Y_GTRIAD_HI_01);
	AddItemPlayerModel("M_Y_GTRI_02", PED_M_Y_GTRI_02);
}

void playerOptModelsAmbientsFemaleTlad()
{
	SetHeader("Ambients (Female)(TLaD)");
	AddItemPlayerModel("F_Y_BUSIASIAN", PED_M_Y_BUSIASIAN);
	AddItemPlayerModel("F_Y_BIKESTRIPPER_01", PED_F_Y_BIKESTRIPPER_01);
	AddItemPlayerModel("F_Y_EMIDTOWN_01", PED_F_Y_EMIDTOWN_01);
	AddItemPlayerModel("F_Y_UPTOWN_01", PED_F_Y_UPTOWN_01);
	AddItemPlayerModel("F_Y_UPTOWN_CS", PED_F_Y_UPTOWN_CS);
}

void playerOptModelsAmbientsMaleTlad()
{
	SetHeader("Ambients (Male)(TLaD)");
	AddItemPlayerModel("M_M_SPRETZER", PED_M_M_SPRETZER);
	AddItemPlayerModel("M_M_SMARTBLACK", PED_M_M_SMARTBLACK);
	AddItemPlayerModel("M_M_UPEAST_01", PED_M_M_UPEAST_01);
	AddItemPlayerModel("M_M_UPTOWN_01", PED_M_M_UPTOWN_01);
	AddItemPlayerModel("M_O_HISPANIC_01", PED_M_O_HISPANIC_01);
	AddItemPlayerModel("M_Y_BIKEMECH", PED_M_Y_BIKEMECH);
	AddItemPlayerModel("M_Y_BUSIASIAN", PED_M_Y_BUSIASIAN);
	AddItemPlayerModel("M_Y_BUSIMIDEAST", PED_M_Y_BUSIMIDEAST);
	AddItemPlayerModel("M_Y_CIADLC_01", PED_M_Y_CIADLC_01);
	AddItemPlayerModel("M_Y_CIADLC_02", PED_M_Y_CIADLC_02);
	AddItemPlayerModel("M_Y_DOORMAN_01", PED_M_Y_DOORMAN_01);
	AddItemPlayerModel("M_Y_PRISONDLC_01", PED_M_Y_PRISONDLC_01);
	AddItemPlayerModel("M_Y_GAYGANG_01", PED_M_Y_GAYGANG_01);
	AddItemPlayerModel("M_Y_HIPMALE_01", PED_M_Y_HIPMALE_01);
	AddItemPlayerModel("M_Y_HIP_02", PED_M_Y_HIP_02);
	AddItemPlayerModel("M_Y_HISPANIC_01", PED_M_Y_HISPANIC_01);
	AddItemPlayerModel("M_Y_PRISONBLACK", PED_M_Y_PRISONBLACK);
	AddItemPlayerModel("M_Y_PRISONGUARD", PED_M_Y_PRISONGUARD);
}

void playerOptModelsBikerBuddiesTlad()
{
	SetHeader("Biker buddies (TLaD)");
	AddItemPlayerModel("LOSTBUDDY_01", PED_LOSTBUDDY_01);
	AddItemPlayerModel("LOSTBUDDY_02", PED_LOSTBUDDY_02);
	AddItemPlayerModel("LOSTBUDDY_03", PED_LOSTBUDDY_03);
	AddItemPlayerModel("LOSTBUDDY_04", PED_LOSTBUDDY_04);
	AddItemPlayerModel("LOSTBUDDY_05", PED_LOSTBUDDY_05);
	AddItemPlayerModel("LOSTBUDDY_06", PED_LOSTBUDDY_06);
	AddItemPlayerModel("LOSTBUDDY_07", PED_LOSTBUDDY_07);
	AddItemPlayerModel("LOSTBUDDY_08", PED_LOSTBUDDY_08);
	AddItemPlayerModel("LOSTBUDDY_09", PED_LOSTBUDDY_09);
	AddItemPlayerModel("LOSTBUDDY_10", PED_LOSTBUDDY_10);
	AddItemPlayerModel("LOSTBUDDY_11", PED_LOSTBUDDY_11);
	AddItemPlayerModel("LOSTBUDDY_12", PED_LOSTBUDDY_12);
	AddItemPlayerModel("LOSTBUDDY_13", PED_LOSTBUDDY_13);
}

void playerOptModelsIGTbogt()
{
	SetHeader("IG models (TBoGT)");
	AddItemPlayerModel("IG_AHMAD", PED_IG_AHMAD);
	AddItemPlayerModel("IG_ARMANDO", PED_IG_ARMANDO);
	AddItemPlayerModel("IG_ARMSDEALER", PED_IG_ARMSDEALER);
	AddItemPlayerModel("IG_ARNAUD", PED_IG_ARNAUD);
	AddItemPlayerModel("IG_BANKER", PED_IG_BANKER);
	AddItemPlayerModel("IG_BLUEBROS", PED_IG_BLUEBROS);
	AddItemPlayerModel("IG_BRUCIE2", PED_IG_BRUCIE2);
	AddItemPlayerModel("IG_BULGARIN2", PED_IG_BULGARIN2);
	AddItemPlayerModel("IG_DAISY", PED_IG_DAISY);
	AddItemPlayerModel("IG_DEEJAY", PED_IG_DEEJAY);
	AddItemPlayerModel("IG_DESSIE", PED_IG_DESSIE);
	AddItemPlayerModel("IG_EVAN", PED_IG_EVAN);
	AddItemPlayerModel("IG_GRACIE2", PED_IG_GRACIE2);
	AddItemPlayerModel("IG_HENRIQUE", PED_IG_HENRIQUE);
	AddItemPlayerModel("IG_ISSAC2", PED_IG_ISSAC2);
	AddItemPlayerModel("IG_JACKSON", PED_IG_JACKSON);
	AddItemPlayerModel("IG_JOHNNY2", PED_IG_JOHNNY2);
	AddItemPlayerModel("IG_LUIS2", PED_IG_LUIS2);
	AddItemPlayerModel("IG_MARGOT", PED_IG_MARGOT);
	AddItemPlayerModel("IG_MORI_K", PED_IG_MORI_K);
	AddItemPlayerModel("IG_MR_SANTOS", PED_IG_MR_SANTOS);
	AddItemPlayerModel("IG_NAPOLI", PED_IG_NAPOLI);
	AddItemPlayerModel("IG_NIKO", PED_IG_NIKO);
	AddItemPlayerModel("IG_OYVEY", PED_IG_OYVEY);
	AddItemPlayerModel("IG_ROCCO", PED_IG_ROCCO);
	AddItemPlayerModel("IG_ROYAL", PED_IG_ROYAL);
	AddItemPlayerModel("IG_SPADE", PED_IG_SPADE);
	AddItemPlayerModel("IG_TAHIR", PED_IG_TAHIR);
	AddItemPlayerModel("IG_TIMUR", PED_IG_TIMUR);
	AddItemPlayerModel("IG_TONY", PED_IG_TONY);
	AddItemPlayerModel("IG_TRAMP2", PED_IG_TRAMP2);
	AddItemPlayerModel("IG_TRIAD", PED_IG_TRIAD);
	AddItemPlayerModel("IG_TROY", PED_IG_TROY);
	AddItemPlayerModel("IG_VIC", PED_IG_VIC);
	AddItemPlayerModel("IG_VICGIRL", PED_IG_VICGIRL);
	AddItemPlayerModel("IG_VINCE", PED_IG_VINCE);
	AddItemPlayerModel("IG_YUSEF", PED_IG_YUSEF);
	AddItemPlayerModel("CS_BAZ", PED_CS_BAZ);
	AddItemPlayerModel("CS_JEREMY", PED_CS_JEREMY);
	AddItemPlayerModel("CS_NIKO", PED_CS_NIKO);
	AddItemPlayerModel("CS_ABDUL", PED_CS_ABDUL);
	AddItemPlayerModel("CS_ABODYGUARD", PED_CS_ABODYGUARD);
	AddItemPlayerModel("CS_ADRIANA", PED_CS_ADRIANA);
	AddItemPlayerModel("CS_AHMAD", PED_CS_AHMAD);
	AddItemPlayerModel("CS_ARMANDO", PED_CS_ARMANDO);
	AddItemPlayerModel("CS_ARNAUD", PED_CS_ARNAUD);
	AddItemPlayerModel("CS_BATHROOM", PED_CS_BATHROOM);
	AddItemPlayerModel("CS_BILLY2", PED_CS_BILLY2);
	AddItemPlayerModel("CS_BULGARIN2", PED_CS_BULGARIN2);
	AddItemPlayerModel("CS_BRUCIE2", PED_CS_BRUCIE2);
	AddItemPlayerModel("CS_DAISY", PED_CS_DAISY);
	AddItemPlayerModel("CS_DERRICK2", PED_CS_DERRICK2);
	AddItemPlayerModel("CS_DESSIE", PED_CS_DESSIE);
	AddItemPlayerModel("CS_DOCKGOON", PED_CS_DOCKGOON);
	AddItemPlayerModel("CS_EUGENE", PED_CS_EUGENE);
	AddItemPlayerModel("CS_EVAN2", PED_CS_EVAN2);
	AddItemPlayerModel("CS_GPARTY_01", PED_CS_GPARTY_01);
	AddItemPlayerModel("CS_GRACIE2", PED_CS_GRACIE2);
	AddItemPlayerModel("CS_HENRIQUE", PED_CS_HENRIQUE);
	AddItemPlayerModel("CS_ISSAC2", PED_CS_ISSAC2);
	AddItemPlayerModel("CS_JOHNNY2", PED_CS_JOHNNY2);
	AddItemPlayerModel("CS_MARGOT", PED_CS_MARGOT);
	AddItemPlayerModel("CS_MAURICE", PED_CS_MAURICE);
	AddItemPlayerModel("CS_MORI_K", PED_CS_MORI_K);
	AddItemPlayerModel("CS_MR_SANTOS", PED_CS_MR_SANTOS);
	AddItemPlayerModel("CS_NIKO2", PED_CS_NIKO2);
	AddItemPlayerModel("CS_OYVEY", PED_CS_OYVEY);
	AddItemPlayerModel("CS_PACKIE2", PED_CS_PACKIE2);
	AddItemPlayerModel("CS_PAPI", PED_CS_PAPI);
	AddItemPlayerModel("CS_ROCCO", PED_CS_ROCCO);
	AddItemPlayerModel("CS_ROMAN2", PED_CS_ROMAN2);
	AddItemPlayerModel("CS_RUS_KN", PED_CS_RUS_KN);
	AddItemPlayerModel("CS_SHARON", PED_CS_SHARON);
	AddItemPlayerModel("CS_TAHIR", PED_CS_TAHIR);
	AddItemPlayerModel("CS_TIMUR", PED_CS_TIMUR);
	AddItemPlayerModel("CS_TONY", PED_CS_TONY);
	AddItemPlayerModel("CS_TONYAQ", PED_CS_TONYAQ);
	AddItemPlayerModel("CS_TRAMP2", PED_CS_TRAMP2);
	AddItemPlayerModel("CS_TRIAD", PED_CS_TRIAD);
	AddItemPlayerModel("CS_TROY", PED_CS_TROY);
	AddItemPlayerModel("CS_VIC", PED_CS_VIC);
	AddItemPlayerModel("CS_VICGIRL", PED_CS_VICGIRL);
	AddItemPlayerModel("CS_VINCE", PED_CS_VINCE);
	AddItemPlayerModel("CS_YUSEF", PED_CS_YUSEF);
}

void playerOptModelsGangMembersTbogt()
{
	SetHeader("Gang members (TBoGT)");
	AddItemPlayerModel("M_Y_GANGELS_02", PED_M_Y_GANGELS_02);
	AddItemPlayerModel("M_Y_GANGELS_04", PED_M_Y_GANGELS_04);
	AddItemPlayerModel("M_Y_GLOST_01", PED_M_Y_GLOST_01);
	AddItemPlayerModel("M_Y_GLOST_02", PED_M_Y_GLOST_02);
	AddItemPlayerModel("M_Y_GTRIAD_HI_01", PED_M_Y_GTRIAD_HI_01);
	AddItemPlayerModel("M_Y_GTRI_02", PED_M_Y_GTRI_02);
}

void playerOptModelsAmbientsFemaleTbogt()
{
	SetHeader("Ambients (Female)(TBoGT)");
	AddItemPlayerModel("F_Y_ASIANCLUB_01", PED_F_Y_ASIANCLUB_01);
	AddItemPlayerModel("F_Y_ASIANCLUB_02", PED_F_Y_ASIANCLUB_02);
	AddItemPlayerModel("F_Y_CLUBEURO_01", PED_F_Y_CLUBEURO_01);
	AddItemPlayerModel("F_Y_DOMGIRL_01", PED_F_Y_DOMGIRL_01);
	AddItemPlayerModel("F_Y_EMIDTOWN_02", PED_F_Y_EMIDTOWN_02);
	AddItemPlayerModel("F_Y_HOSTESS", PED_F_Y_HOSTESS);
	AddItemPlayerModel("F_Y_HOTCHICK_01", PED_F_Y_HOTCHICK_01);
	AddItemPlayerModel("F_Y_HOTCHICK_02", PED_F_Y_HOTCHICK_02);
	AddItemPlayerModel("F_Y_HOTCHICK_03", PED_F_Y_HOTCHICK_03);
	AddItemPlayerModel("F_Y_PGIRL_01", PED_F_Y_PGIRL_01);
	AddItemPlayerModel("F_Y_PGIRL_02", PED_F_Y_PGIRL_02);
	AddItemPlayerModel("F_Y_SMID_01", PED_F_Y_SMID_01);
	AddItemPlayerModel("F_Y_TRENDY_01", PED_F_Y_TRENDY_01);
}

void playerOptModelsAmbientsMaleTbogt()
{
	SetHeader("Ambients (Male)(TBoGT)");
	AddItemPlayerModel("M_M_E2MAF_01", PED_M_M_E2MAF_01);
	AddItemPlayerModel("M_M_E2MAF_02", PED_M_M_E2MAF_02);
	AddItemPlayerModel("M_Y_AMIRGUARD_01", PED_M_Y_AMIRGUARD_01);
	AddItemPlayerModel("M_Y_BATHROOM", PED_M_Y_BATHROOM);
	AddItemPlayerModel("M_Y_BARMAISON", PED_M_Y_BARMAISON);
	AddItemPlayerModel("M_Y_CIADLC_01", PED_M_Y_CIADLC_01);
	AddItemPlayerModel("M_Y_CIADLC_02", PED_M_Y_CIADLC_02);
	AddItemPlayerModel("M_Y_CLUBBLACK_01", PED_M_Y_CLUBBLACK_01);
	AddItemPlayerModel("M_Y_CLUBEURO_01", PED_M_Y_CLUBEURO_01);
	AddItemPlayerModel("M_Y_CLUBEURO_02", PED_M_Y_CLUBEURO_02);
	AddItemPlayerModel("M_Y_CLUBEURO_03", PED_M_Y_CLUBEURO_03);
	AddItemPlayerModel("M_Y_CLUBWHITE_01", PED_M_Y_CLUBWHITE_01);
	AddItemPlayerModel("M_Y_DOMDRUG_01", PED_M_Y_DOMDRUG_01);
	AddItemPlayerModel("M_Y_DOMGUY_01", PED_M_Y_DOMGUY_01);
	AddItemPlayerModel("M_Y_DOMGUY_02", PED_M_Y_DOMGUY_02);
	AddItemPlayerModel("M_Y_DOORMAN_02", PED_M_Y_DOORMAN_02);
	AddItemPlayerModel("M_Y_E2RUSSIAN_01", PED_M_Y_E2RUSSIAN_01);
	AddItemPlayerModel("M_Y_E2RUSSIAN_02", PED_M_Y_E2RUSSIAN_02);
	AddItemPlayerModel("M_Y_E2RUSSIAN_03", PED_M_Y_E2RUSSIAN_03);
	AddItemPlayerModel("M_Y_GAYBLACK_01", PED_M_Y_GAYBLACK_01);
	AddItemPlayerModel("M_Y_GAYDANCER", PED_M_Y_GAYDANCER);
	AddItemPlayerModel("M_Y_GAYGANG_01", PED_M_Y_GAYGANG_01);
	AddItemPlayerModel("M_Y_GAYGENERAL_01", PED_M_Y_GAYGENERAL_01);
	AddItemPlayerModel("M_Y_GAYWHITE_01", PED_M_Y_GAYWHITE_01);
	AddItemPlayerModel("M_Y_GUIDO_01", PED_M_Y_GUIDO_01);
	AddItemPlayerModel("M_Y_GUIDO_02", PED_M_Y_GUIDO_02);
	AddItemPlayerModel("M_Y_MIDEAST_01", PED_M_Y_MIDEAST_01);
	AddItemPlayerModel("M_Y_UPTOWN_01", PED_M_Y_UPTOWN_01);
}

void playerOptModelsFightersTbogt()
{
	SetHeader("Fighters (TBoGT)");
	AddItemPlayerModel("M_Y_FIGHTCLUB_01", PED_M_Y_FIGHTCLUB_01);
	AddItemPlayerModel("M_Y_FIGHTCLUB_02", PED_M_Y_FIGHTCLUB_02);
	AddItemPlayerModel("M_Y_FIGHTCLUB_03", PED_M_Y_FIGHTCLUB_03);
	AddItemPlayerModel("M_Y_FIGHTCLUB_04", PED_M_Y_FIGHTCLUB_04);
	AddItemPlayerModel("M_Y_FIGHTCLUB_05", PED_M_Y_FIGHTCLUB_05);
	AddItemPlayerModel("M_Y_FIGHTCLUB_06", PED_M_Y_FIGHTCLUB_06);
	AddItemPlayerModel("M_Y_FIGHTCLUB_07", PED_M_Y_FIGHTCLUB_07);
	AddItemPlayerModel("M_Y_FIGHTCLUB_08", PED_M_Y_FIGHTCLUB_08);
}

void playerOptModelsMissionSpecificTbogt()
{
	SetHeader("Mission specific (TBoGT)");
	AddItemPlayerModel("F_Y_CLOEPARKER", PED_F_Y_CLOEPARKER);
	AddItemPlayerModel("F_Y_DANCER_01", PED_F_Y_DANCER_01);
	AddItemPlayerModel("F_Y_JONI", PED_F_Y_JONI);
	AddItemPlayerModel("M_M_MAFUNION", PED_M_M_MAFUNION);
	AddItemPlayerModel("M_Y_CELEBBLOG", PED_M_Y_CELEBBLOG);
	AddItemPlayerModel("M_Y_EXSPORTS", PED_M_Y_EXSPORTS);
	AddItemPlayerModel("M_Y_MOBPARTY", PED_M_Y_MOBPARTY);
	AddItemPlayerModel("M_Y_PAPARAZZI_01", PED_M_Y_PAPARAZZI_01);
}

void playerOptModelsObjects()
{
	SetHeader("Player -> Change model -> Objects");
	AddItem("Toilet", playerOptModelsObjectToilet, nullptr);
	AddItem("Fountain lamp", playerOptModelsObjectLampost, nullptr);
	AddItem("Skip", playerOptModelsObjectSkip, nullptr);
	AddItem("Camera tower", playerOptModelsObjectSurveillance, "Big brother is watching YOU!");
	AddItem("Sofa bed", playerOptModelsObjectSofabed, nullptr);
	AddItem("Microwave", playerOptModelsObjectMicrowave, nullptr);
	AddItem("Sink", playerOptModelsObjectSink, nullptr);
	AddItem("Shelves", playerOptModelsObjectShelves, nullptr);
	AddItem("Soda dispenser", playerOptModelsObjectSodaDispenser, nullptr);
	AddItem("Laptop", playerOptModelsObjectsLaptop, nullptr);
	AddItem("Office chair", playerOptModelsObjectsOfficeChair, nullptr);
	AddItem("Tree", playerOptModelsObjectsTree, nullptr);
	AddItem("Plant pot", playerOptModelsObjectsPlantpot, nullptr);
	AddItem("Burger sign", playerOptModelsObjectsBurgersign, nullptr);
	AddItem("Ladder", playerOptModelsObjectLadder, nullptr);
	//attach objects to player and set invisi :D
}

void playerOptVoiceSpeeches()
{
	SetHeader("Player -> Voice -> Speeches");
	AddItemPlayerSpeech("Arrested");
	AddItemPlayerSpeech("Bump");
	AddItemPlayerSpeech("Car_On_Fire");
	AddItemPlayerSpeech("Chased");
	AddItemPlayerSpeech("Crash_Car");
	AddItemPlayerSpeech("Crying");
	AddItemPlayerSpeech("Drink");
	AddItemPlayerSpeech("Dodge");
	AddItemPlayerSpeech("Duck");
	AddItemPlayerSpeech("Fight");
	AddItemPlayerSpeech("Followed");
	AddItemPlayerSpeech("Get_Out");
	AddItemPlayerSpeech("Hurry_Up");
	AddItemPlayerSpeech("Intimidate");
	AddItemPlayerSpeech("Late");
	AddItemPlayerSpeech("Mugging");
	AddItemPlayerSpeech("Need_A_Vehicle");
	AddItemPlayerSpeech("Photo");
	AddItemPlayerSpeech("Saved");
	AddItemPlayerSpeech("sh*t");
	AddItemPlayerSpeech("Shocked");
	AddItemPlayerSpeech("Shoot");
	AddItemPlayerSpeech("Surprised");
	AddItemPlayerSpeech("Surrounded");
	AddItemPlayerSpeech("Take_Cover");
	AddItemPlayerSpeech("Thanks");
	AddItemPlayerSpeech("Target");
	AddItemPlayerSpeech("Trapped");
	AddItemPlayerSpeech("Van");
	AddItemPlayerSpeech("You_Drive");
	AddItemPlayerSpeech("Generic_f*ck_off");
	AddItemPlayerSpeech("Generic_Hi");
	AddItemPlayerSpeech("Generic_Bye");
	AddItemPlayerSpeech("Generic_Yes");
	AddItemPlayerSpeech("Generic_No");
	AddItemPlayerSpeech("Blocked_Vehicle"); //18
	AddItemPlayerSpeech("Blocked_Ped");
	AddItemPlayerSpeech("Conv_Gang_State");
	AddItemPlayerSpeech("Cover_Me");
	AddItemPlayerSpeech("Gang_Ask_Player_Leave");
	AddItemPlayerSpeech("Gang_Attack_Warning");
	AddItemPlayerSpeech("Gang_Bump");
	AddItemPlayerSpeech("Gang_Chase");
	AddItemPlayerSpeech("Gang_Dodge_Warning");
	AddItemPlayerSpeech("Gang_Fight_Cheer");
	AddItemPlayerSpeech("Gang_Intervene");
	AddItemPlayerSpeech("Gang_Watch_This_Guy");
	AddItemPlayerSpeech("Gang_Watch_This_Guy_Resp");
	AddItemPlayerSpeech("Gang_Watch_This_Guy_Solo");
	AddItemPlayerSpeech("Gang_Weapon_Warning");
	AddItemPlayerSpeech("Gang_You_Drop_Weapon");
	AddItemPlayerSpeech("Jacked_In_Car");
	AddItemPlayerSpeech("Jack_Car_Back");
	AddItemPlayerSpeech("Jacking_Car_Back");
	AddItemPlayerSpeech("Jacking_Generic_Back");
	AddItemPlayerSpeech("Mobile_Chat");
	AddItemPlayerSpeech("Player_Over_There");
	AddItemPlayerSpeech("Player_Up_There");
	AddItemPlayerSpeech("Shot_In_Leg");
	AddItemPlayerSpeech("Shut_Up_Horn");
	AddItemPlayerSpeech("Two_Way_Phone_Chat");
	AddItemPlayerSpeech("Vehicle_Attacked");
	AddItemPlayerSpeech("Lets_Play_Pool");
	AddItemPlayerSpeech("Lets_Play_Darts");
	AddItemPlayerSpeech("Darts_Bullseye");
	AddItemPlayerSpeech("Bowling_Gutter");
	AddItemPlayerSpeech("Bowling_Spare");
	AddItemPlayerSpeech("Bowling_Strike");
	AddItemPlayerSpeech("Bowling_Turkey");
	AddItemPlayerSpeech("Pool_Foul");
	AddItemPlayerSpeech("Cough");
	AddItemPlayerSpeech("Drowning");
	AddItemPlayerSpeech("High_Fall");
	AddItemPlayerSpeech("Inhale");
	AddItemPlayerSpeech("Cheer");
	AddItemPlayerSpeech("Giggle");
	AddItemPlayerSpeech("Laugh");
	AddItemPlayerSpeech("Panic");
	AddItemPlayerSpeech("Whoop");
}

void playerOptVoicesStory()
{
	SetHeader("Player -> Story");
	AddItemPlayerVoice("ALEX");
	AddItemPlayerVoice("BELL");
	AddItemPlayerVoice("BERNIE");
	AddItemPlayerVoice("BLEEDAR");
	AddItemPlayerVoice("BOBBY");
	AddItemPlayerVoice("BRIAN_CLEAN");
	AddItemPlayerVoice("BRIAN_DRUG");
	AddItemPlayerVoice("BRUCIE");
	AddItemPlayerVoice("CABBY");
	AddItemPlayerVoice("CARMEN");
	AddItemPlayerVoice("CLARENCE");
	AddItemPlayerVoice("DERRICK_MCREARY");
	AddItemPlayerVoice("DIMITRI");
	AddItemPlayerVoice("DWAYNE");
	AddItemPlayerVoice("FRANKIE");
	AddItemPlayerVoice("GORDON");
	AddItemPlayerVoice("GRACIE");
	AddItemPlayerVoice("HOSSAN");
	AddItemPlayerVoice("IVAN");
	AddItemPlayerVoice("JACOB");
	AddItemPlayerVoice("JERMAINE");
	AddItemPlayerVoice("JIMMY_PEGORINO");
	AddItemPlayerVoice("KATE_MCREARY");
	AddItemPlayerVoice("KIKI");
	AddItemPlayerVoice("LUCA");
	AddItemPlayerVoice("M_ZOMBIE");
	AddItemPlayerVoice("F_Y_MPASIAN_01");
	AddItemPlayerVoice("F_Y_MPASIAN_02");
	AddItemPlayerVoice("F_Y_MPBLACK_01");
	AddItemPlayerVoice("F_Y_MPBLACK_02");
	AddItemPlayerVoice("F_Y_MPLATIN_01");
	AddItemPlayerVoice("F_Y_MPLATIN_02");
	AddItemPlayerVoice("F_Y_MPWHITE_01");
	AddItemPlayerVoice("F_Y_MPWHITE_02");
	AddItemPlayerVoice("MALLORIE");
	AddItemPlayerVoice("MANNY");
	AddItemPlayerVoice("MICHELLE");
	AddItemPlayerVoice("NIKO");
	AddItemPlayerVoice("NIKO_ANGRY");
	AddItemPlayerVoice("NIKO_EXTRAS");
	AddItemPlayerVoice("NIKO_NORMAL");
	AddItemPlayerVoice("PACKIE_MCREARY");
	AddItemPlayerVoice("PAIN_FEMALE_EXTRAS");
	AddItemPlayerVoice("PAIN_MALE_EXTRAS");
	AddItemPlayerVoice("PATHOS");
	AddItemPlayerVoice("PETROVIC");
	AddItemPlayerVoice("PI_CHERISE");
	AddItemPlayerVoice("PI_ED_LOW");
	AddItemPlayerVoice("PI_MARLENE");
	AddItemPlayerVoice("PI_MEL");
	AddItemPlayerVoice("PLAYBOY");
	AddItemPlayerVoice("RAY_BOCCINO");
	AddItemPlayerVoice("REAL_BADMAN");
	AddItemPlayerVoice("ROMAN_DRUNK");
	AddItemPlayerVoice("ROMAN_NORMAL");
	AddItemPlayerVoice("ROMAN_SAD");
	AddItemPlayerVoice("ROMAN_SHAKEN_UP");
	AddItemPlayerVoice("SARA");
	AddItemPlayerVoice("TALBOT");
	AddItemPlayerVoice("TEST_SHOUT");
	AddItemPlayerVoice("TUNA");
	AddItemPlayerVoice("TWAT_WORKER");
}

void playerOptVoicesGang()
{
	SetHeader("Player -> Voice -> Gang");
	AddItemPlayerVoice("M_Y_GAFR_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GAFR_LO_01_FULL_02");
	AddItemPlayerVoice("M_Y_GAFR_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GAFR_HI_01_FULL_01");
	AddItemPlayerVoice("M_Y_GAFR_HI_02_FULL_01");
	AddItemPlayerVoice("M_Y_GALB_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GALB_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GALB_LO_03_FULL_01");
	AddItemPlayerVoice("M_Y_GALB_LO_04_FULL_01");
	AddItemPlayerVoice("M_M_GBIK_LO_03_FULL_01");
	AddItemPlayerVoice("M_Y_GBIK_HI_01_FULL_01");
	AddItemPlayerVoice("M_Y_GBIK_HI_02_FULL_01");
	AddItemPlayerVoice("M_Y_GBIK02_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GBIK_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GBIK_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GIRI_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GIRI_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GIRI_LO_03_FULL_01");
	AddItemPlayerVoice("M_M_GJAM_HI_01_FULL_01");
	AddItemPlayerVoice("M_M_GJAM_HI_02_FULL_01");
	AddItemPlayerVoice("M_Y_GJAM_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GJAM_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GKOR_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GKOR_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GLAT_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GLAT_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GLAT_HI_01_FULL_01");
	AddItemPlayerVoice("M_Y_GLAT_HI_02_FULL_01");
	AddItemPlayerVoice("M_Y_GMAF_HI_01_FULL_01");
	AddItemPlayerVoice("M_Y_GMAF_HI_02_FULL_01");
	AddItemPlayerVoice("M_Y_GMAF_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GMAF_LO_02_FULL_01");
	AddItemPlayerVoice("M_O_GRUS_HI_01_FULL_01");
	AddItemPlayerVoice("M_Y_GRUS_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GRUS_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GRUS_HI_02_FULL_01");
	AddItemPlayerVoice("M_M_GRU2_HI_01_FULL_01");
	AddItemPlayerVoice("M_M_GRU2_HI_02_FULL_01");
	AddItemPlayerVoice("M_M_GRU2_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GRU2_LO_01_FULL_01");
	AddItemPlayerVoice("M_M_GTRI_HI_01_FULL_01");
	AddItemPlayerVoice("M_M_GTRI_HI_02_FULL_01");
	AddItemPlayerVoice("M_Y_GTRI_LO_01_FULL_01");
	AddItemPlayerVoice("M_Y_GTRI_LO_02_FULL_01");
	AddItemPlayerVoice("M_Y_GAFR_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GAFR_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GAFR_LO_02_GANG_02");
	AddItemPlayerVoice("M_Y_GAFR_HI_02_GANG_01");
	AddItemPlayerVoice("M_Y_GALB_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GALB_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GALB_LO_03_GANG_01");
	AddItemPlayerVoice("M_M_GBIK_LO_03_GANG_01");
	AddItemPlayerVoice("M_Y_GBIK_HI_01_GANG_01");
	AddItemPlayerVoice("M_Y_GBIK_HI_02_GANG_01");
	AddItemPlayerVoice("M_Y_GBIK02_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GBIK_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GBIK_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GIRI_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GIRI_LO_03_GANG_01");
	AddItemPlayerVoice("M_M_GJAM_HI_01_GANG_01");
	AddItemPlayerVoice("M_M_GJAM_HI_02_GANG_01");
	AddItemPlayerVoice("M_Y_GJAM_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GJAM_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GKOR_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GKOR_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GLAT_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GLAT_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GLAT_HI_01_GANG_01");
	AddItemPlayerVoice("M_Y_GLAT_HI_02_GANG_01");
	AddItemPlayerVoice("M_Y_GMAF_HI_01_GANG_01");
	AddItemPlayerVoice("M_Y_GMAF_HI_02_GANG_01");
	AddItemPlayerVoice("M_Y_GMAF_HI_02_GANG_02");
	AddItemPlayerVoice("M_Y_GMAF_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GMAF_LO_01_GANG_02");
	AddItemPlayerVoice("M_Y_GMAF_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GMAF_LO_02_GANG_02");
	AddItemPlayerVoice("M_O_GRUS_HI_01_GANG_01");
	AddItemPlayerVoice("M_Y_GRUS_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GRUS_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GRUS_HI_02_GANG_01");
	AddItemPlayerVoice("M_M_GRU2_HI_01_GANG_01");
	AddItemPlayerVoice("M_M_GRU2_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GRU2_LO_01_GANG_01");
	AddItemPlayerVoice("M_M_GTRI_HI_01_GANG_01");
	AddItemPlayerVoice("M_M_GTRI_HI_02_GANG_01");
	AddItemPlayerVoice("M_Y_GTRI_LO_01_GANG_01");
	AddItemPlayerVoice("M_Y_GTRI_LO_02_GANG_01");
	AddItemPlayerVoice("M_Y_GAFR_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GAFR_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GAFR_LO_02_MINI_02");
	AddItemPlayerVoice("M_Y_GAFR_HI_01_MINI_01");
	AddItemPlayerVoice("M_Y_GAFR_HI_02_MINI_01");
	AddItemPlayerVoice("M_Y_GALB_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GALB_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GALB_LO_03_MINI_01");
	AddItemPlayerVoice("M_Y_GALB_LO_04_MINI_01");
	AddItemPlayerVoice("M_M_GBIK_LO_03_MINI_01");
	AddItemPlayerVoice("M_Y_GBIK_HI_01_MINI_01");
	AddItemPlayerVoice("M_Y_GBIK_HI_02_MINI_01");
	AddItemPlayerVoice("M_Y_GBIK02_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GBIK_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GBIK_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GIRI_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GIRI_LO_02_MINI_01");
	AddItemPlayerVoice("M_M_GJAM_HI_01_MINI_01");
	AddItemPlayerVoice("M_M_GJAM_HI_02_MINI_01");
	AddItemPlayerVoice("M_Y_GJAM_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GJAM_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GKOR_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GKOR_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GLAT_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GLAT_HI_01_MINI_01");
	AddItemPlayerVoice("M_Y_GLAT_HI_02_MINI_01");
	AddItemPlayerVoice("M_Y_GMAF_HI_01_MINI_01");
	AddItemPlayerVoice("M_Y_GMAF_HI_02_MINI_01");
	AddItemPlayerVoice("M_Y_GMAF_HI_02_MINI_02");
	AddItemPlayerVoice("M_Y_GMAF_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GMAF_LO_01_MINI_02");
	AddItemPlayerVoice("M_Y_GMAF_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GMAF_LO_02_MINI_02");
	AddItemPlayerVoice("M_O_GRUS_HI_01_MINI_01");
	AddItemPlayerVoice("M_Y_GRUS_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GRUS_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GRUS_HI_02_MINI_01");
	AddItemPlayerVoice("M_M_GRU2_HI_01_MINI_01");
	AddItemPlayerVoice("M_M_GRU2_HI_02_MINI_01");
	AddItemPlayerVoice("M_M_GRU2_LO_02_MINI_01");
	AddItemPlayerVoice("M_Y_GRU2_LO_01_MINI_01");
	AddItemPlayerVoice("M_M_GTRI_HI_01_MINI_01");
	AddItemPlayerVoice("M_M_GTRI_HI_02_MINI_01");
	AddItemPlayerVoice("M_Y_GTRI_LO_01_MINI_01");
	AddItemPlayerVoice("M_Y_GTRI_LO_02_MINI_01");
}

void playerOptVoicesForegroundFemale()
{
	SetHeader("Player -> Voice -> Foreground (Female)");
	AddItemPlayerVoice("F_O_MAID_01");
	AddItemPlayerVoice("F_Y_BANK");
	AddItemPlayerVoice("F_Y_DOCTOR_01");
	AddItemPlayerVoice("F_Y_GYMGAL_01");
	AddItemPlayerVoice("F_Y_FASTFOODHISPANIC");
	AddItemPlayerVoice("F_Y_FASTFOODWHITES");
	AddItemPlayerVoice("F_Y_HOOKER_01");
	AddItemPlayerVoice("F_Y_HOOKER_03");
	AddItemPlayerVoice("F_Y_NURSE");
	AddItemPlayerVoice("F_Y_STRIPPER_01");
	AddItemPlayerVoice("F_Y_STRIPPER_02");
	AddItemPlayerVoice("F_Y_WAITRESS");
}

void playerOptVoicesForegroundMale()
{
	SetHeader("Player -> Voice -> Foreground (Male)");
	AddItemPlayerVoice("M_M_ALCOHOLIC");
	AddItemPlayerVoice("M_M_ARMOURED");
	AddItemPlayerVoice("M_M_BUSDRIVER");
	AddItemPlayerVoice("M_M_CHINATOWN");
	AddItemPlayerVoice("M_M_CRACKHEAD");
	AddItemPlayerVoice("M_M_DOC_SCRUBS_01");
	AddItemPlayerVoice("M_M_DOCTOR_01");
	AddItemPlayerVoice("M_M_FACTORY_01");
	AddItemPlayerVoice("M_M_FATCOP_01_BLACK");
	AddItemPlayerVoice("M_M_FATCOP_01_WHITE");
	AddItemPlayerVoice("M_M_FEDCO");
	AddItemPlayerVoice("M_M_FIRECHIEF");
	AddItemPlayerVoice("M_M_GUNNUT_01");
	AddItemPlayerVoice("HELICOPTER_PILOT_1");
	AddItemPlayerVoice("HELICOPTER_PILOT_2");
	AddItemPlayerVoice("HELICOPTER_PILOT_3");
	AddItemPlayerVoice("M_M_HPORTER_01");
	AddItemPlayerVoice("M_M_KOREACOOK_01");
	AddItemPlayerVoice("M_M_LOONYBLACK");
	AddItemPlayerVoice("M_M_PILOT");
	AddItemPlayerVoice("M_M_PINDUS_01");
	AddItemPlayerVoice("M_M_POSTAL_01");
	AddItemPlayerVoice("M_M_SECURITYMAN");
	AddItemPlayerVoice("M_M_SELLER_01");
	AddItemPlayerVoice("M_M_SHORTORDER");
	AddItemPlayerVoice("M_M_STREETFOOD_01");
	AddItemPlayerVoice("M_M_SWEEPER");
	AddItemPlayerVoice("M_M_TAXIDRIVER");
	AddItemPlayerVoice("M_M_TELEPHONE");
	AddItemPlayerVoice("M_M_TENNIS");
	AddItemPlayerVoice("M_M_TRAIN_01");
	AddItemPlayerVoice("M_M_TRAMPBLACK");
	AddItemPlayerVoice("M_M_TRUCKER_01");
	AddItemPlayerVoice("M_O_HOTEL_FOOT");
	AddItemPlayerVoice("M_O_JANITOR");
	AddItemPlayerVoice("M_Y_AIRWORKER");
	AddItemPlayerVoice("M_Y_BARMAN_01");
	AddItemPlayerVoice("M_Y_BARMAN_02");
	AddItemPlayerVoice("M_Y_BARRIER_COP_02");
	AddItemPlayerVoice("M_Y_BARRIER_COP_03");
	AddItemPlayerVoice("M_Y_BARRIER_COP_04");
	AddItemPlayerVoice("M_Y_BARRIER_COP_05");
	AddItemPlayerVoice("M_Y_BOUNCER_01");
	AddItemPlayerVoice("M_Y_CHINVEND_01");
	AddItemPlayerVoice("M_Y_CLUBFIT");
	AddItemPlayerVoice("M_Y_CONSTRUCT_01");
	AddItemPlayerVoice("M_Y_CONSTRUCT_02");
	AddItemPlayerVoice("M_Y_CONSTRUCT_03");
	AddItemPlayerVoice("M_Y_COP_BLACK");
	AddItemPlayerVoice("M_Y_COP_BLACK_02");
	AddItemPlayerVoice("M_Y_COP_HISPANIC");
	AddItemPlayerVoice("M_Y_COP_TRAFFIC_BLACK");
	AddItemPlayerVoice("M_Y_COP_TRAFFIC_HISPANIC");
	AddItemPlayerVoice("M_Y_COP_TRAFFIC_WHITE");
	AddItemPlayerVoice("M_Y_COP_WHITE");
	AddItemPlayerVoice("M_Y_BARRIER_COP_01");
	AddItemPlayerVoice("M_Y_COP_WHITE_02");
	AddItemPlayerVoice("M_Y_COURIER");
	AddItemPlayerVoice("M_Y_DEALER");
	AddItemPlayerVoice("M_Y_DRUG_01");
	AddItemPlayerVoice("M_Y_EASTEURO_01");
	AddItemPlayerVoice("M_Y_FASTFOODBLACK");
	AddItemPlayerVoice("M_Y_FASTFOODWHITES");
	AddItemPlayerVoice("M_Y_FIB");
	AddItemPlayerVoice("M_Y_FIREMAN_BLACK");
	AddItemPlayerVoice("M_Y_FIREMAN_WHITE");
	AddItemPlayerVoice("M_Y_GARBAGE");
	AddItemPlayerVoice("M_Y_GOON_01");
	AddItemPlayerVoice("M_Y_GYMGUY_01");
	AddItemPlayerVoice("M_Y_HELI_COP");
	AddItemPlayerVoice("M_Y_MECHANIC_02");
	AddItemPlayerVoice("M_Y_PINDUS_01");
	AddItemPlayerVoice("M_Y_PINDUS_02");
	AddItemPlayerVoice("M_Y_PINDUS_03");
	AddItemPlayerVoice("M_Y_PMEDIC_BLACK");
	AddItemPlayerVoice("M_Y_PMEDIC_CHINESE");
	AddItemPlayerVoice("M_Y_RUNNER");
	AddItemPlayerVoice("M_Y_SHOPASST");
	AddItemPlayerVoice("M_Y_STROOPER_WHITE_01");
	AddItemPlayerVoice("M_Y_SWAT_BLACK");
	AddItemPlayerVoice("M_Y_SWAT_HISPANIC");
	AddItemPlayerVoice("M_Y_SWAT_WHITE");
	AddItemPlayerVoice("M_Y_VALET");
	AddItemPlayerVoice("M_Y_VENDOR");
	AddItemPlayerVoice("M_Y_WAITER_01");
}

void playerOptVoicesBackgroundFemale()
{
	SetHeader("Player -> Voice -> Background (Female)");
	AddItemPlayerVoice("F_O_PEASTEURO_01");
	AddItemPlayerVoice("F_O_PEASTEURO_02");
	AddItemPlayerVoice("F_O_PHARBRON_01");
	AddItemPlayerVoice("F_O_PJERSEY_01");
	AddItemPlayerVoice("F_O_PORIENT_01");
	AddItemPlayerVoice("F_O_RICH_01");
	AddItemPlayerVoice("F_M_BUSINESS_01");
	AddItemPlayerVoice("F_M_BUSINESS_02");
	AddItemPlayerVoice("F_M_CHINATOWN");
	AddItemPlayerVoice("F_M_PBUSINESS_01");
	AddItemPlayerVoice("F_M_PEASTEURO_01");
	AddItemPlayerVoice("F_M_PHARBRON_01");
	AddItemPlayerVoice("F_M_PJERSEY_01");
	AddItemPlayerVoice("F_M_PJERSEY_02");
	AddItemPlayerVoice("F_M_PLATIN_01");
	AddItemPlayerVoice("F_M_PMANHAT_01");
	AddItemPlayerVoice("F_M_PMANHAT_02");
	AddItemPlayerVoice("F_M_PORIENT_01");
	AddItemPlayerVoice("F_M_PRICH_01");
	AddItemPlayerVoice("F_Y_BUSINESS_01");
	AddItemPlayerVoice("F_Y_CDRESS_01");
	AddItemPlayerVoice("F_Y_PBRONX_01");
	AddItemPlayerVoice("F_Y_PCOOL_02");
	AddItemPlayerVoice("F_Y_PEASTEURO_01");
	AddItemPlayerVoice("F_Y_PHARBRON_01");
	AddItemPlayerVoice("F_Y_PHARLEM_01");
	AddItemPlayerVoice("F_Y_PJERSEY_02");
	AddItemPlayerVoice("F_Y_PLATIN_01");
	AddItemPlayerVoice("F_Y_PMANHAT_01");
	AddItemPlayerVoice("F_Y_PMANHAT_02");
	AddItemPlayerVoice("F_Y_PORIENT_01");
	AddItemPlayerVoice("F_Y_PQUEENS_01");
	AddItemPlayerVoice("F_Y_PRICH_01");
	AddItemPlayerVoice("F_Y_PVILLBO_02");
	AddItemPlayerVoice("F_Y_SHOP_03");
	AddItemPlayerVoice("F_Y_SHOP_04");
	AddItemPlayerVoice("F_Y_SHOPPER_05");
	AddItemPlayerVoice("F_Y_SOCIALITE");
	AddItemPlayerVoice("F_Y_STREET_02");
	AddItemPlayerVoice("F_Y_STREET_05");
	AddItemPlayerVoice("F_Y_STREET_09");
	AddItemPlayerVoice("F_Y_STREET_12");
	AddItemPlayerVoice("F_Y_STREET_30");
	AddItemPlayerVoice("F_Y_STREET_34");
	AddItemPlayerVoice("F_Y_TOURIST_01");
	AddItemPlayerVoice("F_Y_VILBO_01");
}

void playerOptVoicesBackgroundMale()
{
	SetHeader("Player -> Voice -> Background (Male)");
	AddItemPlayerVoice("M_M_BUSINESS_02");
	AddItemPlayerVoice("M_M_EE_HEAVY_01");
	AddItemPlayerVoice("M_M_EE_HEAVY_02");
	AddItemPlayerVoice("M_M_FATMOB_01");
	AddItemPlayerVoice("M_M_GAYMID");
	AddItemPlayerVoice("M_M_GENBUM_01");
	AddItemPlayerVoice("M_M_LOONYWHITE");
	AddItemPlayerVoice("M_M_PHARBRON_01");
	AddItemPlayerVoice("M_M_PINDUS_02");
	AddItemPlayerVoice("M_M_PITALIAN_01");
	AddItemPlayerVoice("M_M_PITALIAN_02");
	AddItemPlayerVoice("M_M_PLATIN_01");
	AddItemPlayerVoice("M_M_PLATIN_02");
	AddItemPlayerVoice("M_M_PLATIN_03");
	AddItemPlayerVoice("M_M_PMANHAT_01");
	AddItemPlayerVoice("M_M_PORIENT_01");
	AddItemPlayerVoice("M_M_PRICH_01");
	AddItemPlayerVoice("M_O_BUSKER");
	AddItemPlayerVoice("M_O_EASTEURO_01");
	AddItemPlayerVoice("M_O_HASID_01");
	AddItemPlayerVoice("M_O_PEASTEURO_02");
	AddItemPlayerVoice("M_O_PHARBRON_01");
	AddItemPlayerVoice("M_O_PJERSEY_01");
	AddItemPlayerVoice("M_O_STREET_01");
	AddItemPlayerVoice("M_O_SUITED");
	AddItemPlayerVoice("M_Y_BOHO_01");
	AddItemPlayerVoice("M_Y_BOHOGUY_01");
	AddItemPlayerVoice("M_Y_BRONX_01");
	AddItemPlayerVoice("M_Y_BUSINESS_01");
	AddItemPlayerVoice("M_Y_BUSINESS_02");
	AddItemPlayerVoice("M_Y_CHINATOWN_03");
	AddItemPlayerVoice("M_Y_CHOPSHOP_01");
	AddItemPlayerVoice("M_Y_CHOPSHOP_02");
	AddItemPlayerVoice("M_Y_DODGY_01");
	AddItemPlayerVoice("M_Y_DORK_02");
	AddItemPlayerVoice("M_Y_DOWNTOWN_01");
	AddItemPlayerVoice("M_Y_DOWNTOWN_02");
	AddItemPlayerVoice("M_Y_DOWNTOWN_03");
	AddItemPlayerVoice("M_Y_GAYYOUNG");
	AddItemPlayerVoice("M_Y_GENSTREET_11");
	AddItemPlayerVoice("M_Y_GENSTREET_16");
	AddItemPlayerVoice("M_Y_GENSTREET_20");
	AddItemPlayerVoice("M_Y_HARDMAN_01");
	AddItemPlayerVoice("M_Y_HARLEM_01");
	AddItemPlayerVoice("M_Y_HARLEM_02");
	AddItemPlayerVoice("M_Y_HARLEM_04");
	AddItemPlayerVoice("M_Y_HASID_01");
	AddItemPlayerVoice("M_Y_LEASTSIDE_01");
	AddItemPlayerVoice("M_Y_PBRONX_01");
	AddItemPlayerVoice("M_Y_PCOOL_01");
	AddItemPlayerVoice("M_Y_PCOOL_02");
	AddItemPlayerVoice("M_Y_PHARBRON_01");
	AddItemPlayerVoice("M_Y_PHARLEM_01");
	AddItemPlayerVoice("M_Y_PJERSEY_01");
	AddItemPlayerVoice("M_Y_PLATIN_01");
	AddItemPlayerVoice("M_Y_PLATIN_02");
	AddItemPlayerVoice("M_Y_PLATIN_03");
	AddItemPlayerVoice("M_Y_PMANHAT_01");
	AddItemPlayerVoice("M_Y_PMANHAT_02");
	AddItemPlayerVoice("M_Y_PORIENT_01");
	AddItemPlayerVoice("M_Y_PQUEENS_01");
	AddItemPlayerVoice("M_Y_PRICH_01");
	AddItemPlayerVoice("M_Y_PVILBO_01");
	AddItemPlayerVoice("M_Y_PVILLBO_02");
	AddItemPlayerVoice("M_Y_PVILLBO_03");
	AddItemPlayerVoice("M_Y_QUEENSBRIDGE");
	AddItemPlayerVoice("M_Y_SHADY_02");
	AddItemPlayerVoice("M_Y_SKATEBIKE_01");
	AddItemPlayerVoice("M_Y_SOHO_01");
	AddItemPlayerVoice("M_Y_STREET_01");
	AddItemPlayerVoice("M_Y_STREET_03");
	AddItemPlayerVoice("M_Y_STREET_04");
	AddItemPlayerVoice("M_Y_STREETBLK_02");
	AddItemPlayerVoice("M_Y_STREETBLK_03");
	AddItemPlayerVoice("M_Y_STREETPUNK_02");
	AddItemPlayerVoice("M_Y_STREETPUNK_04");
	AddItemPlayerVoice("M_Y_STREETPUNK_05");
	AddItemPlayerVoice("M_Y_TOUGH_05");
	AddItemPlayerVoice("M_Y_TOURIST_02");
}

void playerOptChangeVoice()
{
	SetHeader("Player -> Voice");
	AddItemMenu("Speeches", playerOptVoiceSpeeches, "Wanna chat some shit?");	//lol
	AddItemMenu("Story", playerOptVoicesStory, nullptr);
	AddItemMenu("Gang", playerOptVoicesGang, nullptr);
	AddItemMenu("Foreground (Female)", playerOptVoicesForegroundFemale, nullptr);
	AddItemMenu("Foreground (Male)", playerOptVoicesForegroundMale, nullptr);
	AddItemMenu("Background (Female)", playerOptVoicesBackgroundFemale, nullptr);
	AddItemMenu("Background (Male)", playerOptVoicesBackgroundMale, nullptr);
}

void playerOptChangeModelSuperhero()
{
	changePlayerModel(superHeroModelSelect ? PED_F_Y_MULTIPLAYER : PED_M_Y_MULTIPLAYER, 0);
	SET_CHAR_RANDOM_COMPONENT_VARIATION(myPlayerPed); //just incase its our mp model, func above will set model choice not a random bariation.
	SET_CHAR_COMPONENT_VARIATION(myPlayerPed, PED_COMPONENT_UPPER, 4, 2);			//SET_CHAR_COMPONENT_VARIATION(myPlayerPed, 1, 4, 2);
	SET_CHAR_COMPONENT_VARIATION(myPlayerPed, PED_COMPONENT_LOWER, 3, 0);			//SET_CHAR_COMPONENT_VARIATION(myPlayerPed, 2, 3, 0);
}

void playerOptAddPlayerHelmet(int un1, int un2){		GIVE_PLAYER_HELMET(myPlayerIndex);}
void playerOptRemovePlayerHelmet(int un1, int un2) { REMOVE_PLAYER_HELMET(myPlayerIndex, true); REMOVE_PED_HELMET(myPlayerPed, true); }

int propIndexHead, propIndexHeadTexture, propIndexFace, propIndexFaceTexture;
void playerOptPropIndexHead(int un1, int un2)
{
	propIndexHeadTexture = 0;
	SET_CHAR_PROP_INDEX(myPlayerPed, 0, propIndexHead);
	SET_CHAR_PROP_INDEX_TEXTURE(myPlayerPed, 0, propIndexHead, 0);
}

void playerOptPropIndexFace(int un1, int un2)
{
	propIndexFaceTexture = 0;
	SET_CHAR_PROP_INDEX(myPlayerPed, 1, propIndexFace);
	SET_CHAR_PROP_INDEX_TEXTURE(myPlayerPed, 1, propIndexFace, 0);
}

void playerOptPropIndexSetHeadTexture(int un1, int un2){	SET_CHAR_PROP_INDEX_TEXTURE(myPlayerPed, 0, propIndexHead, propIndexHeadTexture); }
void playerOptPropIndexSetFaceTexture(int un1, int un2){	SET_CHAR_PROP_INDEX_TEXTURE(myPlayerPed, 1, propIndexFace, propIndexFaceTexture); }

void playerOptPropIndexHeadRemove(int un1, int un2) { CLEAR_CHAR_PROP(myPlayerPed, 0); }
void playerOptPropIndexFaceRemove(int un1, int un2) { CLEAR_CHAR_PROP(myPlayerPed, 1); }

void playerOptModelProps(int un1, int un2)
{
	GET_CHAR_PROP_INDEX(myPlayerPed, 0, &propIndexHead); 
	GET_CHAR_PROP_INDEX(myPlayerPed, 1, &propIndexFace);

	SetHeader("Model props");
	AddItemIntAlt("Head", true, 0, 8, &propIndexHead, 1, playerOptPropIndexHead, "~PAD_X~ = Remove.", playerOptPropIndexHeadRemove, nullptr);
	AddItemInt("Head Texture", true, 0, 5, &propIndexHeadTexture, 1, playerOptPropIndexSetHeadTexture, nullptr);
	AddItemIntAlt("Face", true, 0, 8, &propIndexFace, 1, playerOptPropIndexFace, "~PAD_X~ = Remove.", playerOptPropIndexFaceRemove, nullptr);
	AddItemInt("Face Texture", true, 0, 5, &propIndexFaceTexture, 1, playerOptPropIndexSetFaceTexture, nullptr);
}

void playerOptSetRandomComponentVariation(int un1, int un2) { SET_CHAR_RANDOM_COMPONENT_VARIATION(myPlayerPed); }
void playerOptSetDefaultComponentVariation(int un1, int un2) { SET_CHAR_DEFAULT_COMPONENT_VARIATION(myPlayerPed); }

int selectedComponent, selectedDrawable, selectedTexture;

void playerOptApplyDrawableComponentChoice(int un1, int un2) { SET_CHAR_COMPONENT_VARIATION(myPlayerPed, selectedComponent, selectedDrawable, 0); selectedTexture = 0; } //set textures to 0 by default.
void playerOptApplyTextureComponentChoice(int un1, int un2) { SET_CHAR_COMPONENT_VARIATION(myPlayerPed, selectedComponent, selectedDrawable, selectedTexture); }

void playerOptModifyComponent(int component, int unused)
{
	
	selectedComponent = component;
	selectedDrawable = GET_CHAR_DRAWABLE_VARIATION(myPlayerPed, selectedComponent);
	selectedTexture = GET_CHAR_TEXTURE_VARIATION(myPlayerPed, selectedComponent);
	int maxDrawables = GET_NUMBER_OF_CHAR_DRAWABLE_VARIATIONS(myPlayerPed, selectedComponent) - 1;
	int maxTextures = GET_NUMBER_OF_CHAR_TEXTURE_VARIATIONS(myPlayerPed, selectedComponent, selectedDrawable) - 1;
	
	//SetHeader(Container.Item[Container.CursorIndex].Ui.ItemText); //string should be stored in codespace (hopefully)... this should grab the pointer to that before it gets over-written below. - 
	
	switch (selectedComponent)
	{
	case PED_COMPONENT_HEAD: SetHeader("Head"); break;
	case PED_COMPONENT_FACE: SetHeader("Face"); break;
	case PED_COMPONENT_TEEF: SetHeader("Teeth"); break;
	case PED_COMPONENT_UPPER: SetHeader("Torso / Upper"); break;
	case PED_COMPONENT_LOWER: SetHeader("Legs / Lower"); break;
	case PED_COMPONENT_HAND: SetHeader("Hands"); break;
	case PED_COMPONENT_FEET: SetHeader("Feet"); break;
	case PED_COMPONENT_HAIR: SetHeader("Hair"); break;
	case PED_COMPONENT_JACKET: SetHeader("Jacket"); break;
	case PED_COMPONENT_SUSE: SetHeader("Specials"); break;
	case PED_COMPONENT_SUS2: SetHeader("Specials 2"); break;
	default: SetHeader("U BROKE IT!"); break;
	}
	
	if(maxDrawables > 0)
		AddItemInt("Drawable variation", true, 0, maxDrawables, &selectedDrawable, 1, playerOptApplyDrawableComponentChoice, nullptr);

	if(maxTextures > 0)
		AddItemInt("Texture variation", true, 0, maxTextures, &selectedTexture, 1, playerOptApplyTextureComponentChoice, nullptr);

	//AddItemInt("Drawable variation", true, 0, (maxDrawables < 0 ? 0 : maxDrawables), &selectedDrawable, 1, playerOptApplyDrawableComponentChoice, nullptr);
	//AddItemInt("Texture variation", true, 0, (maxTextures < 0 ? 0 : maxTextures), &selectedTexture, 1, playerOptApplyTextureComponentChoice, nullptr);
}

bool VariationsAvailable(int component)
{
	return (GET_NUMBER_OF_CHAR_DRAWABLE_VARIATIONS(myPlayerPed, component) + GET_NUMBER_OF_CHAR_TEXTURE_VARIATIONS(myPlayerPed, component, GET_CHAR_DRAWABLE_VARIATION(myPlayerPed, component)) > 2);
}

void playerOptComponentOptions(int un1, int un2)
{
	//AddItemMenusWithParamAlt("Head", PED_COMPONENT_HEAD, playerOptModifyComponents, )
	SetHeader("Ped Components");
	AddItemAlt("Set default variation", playerOptSetDefaultComponentVariation, "~PAD_X~ = Random", playerOptSetRandomComponentVariation, nullptr);

	if(VariationsAvailable(PED_COMPONENT_HEAD))
		AddItemMenuWithParam("Head", PED_COMPONENT_HEAD, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_FACE))
		AddItemMenuWithParam("Face", PED_COMPONENT_FACE, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_TEEF))
		AddItemMenuWithParam("Teeth", PED_COMPONENT_TEEF, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_UPPER))
		AddItemMenuWithParam("Torso / Upper", PED_COMPONENT_UPPER, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_LOWER))
		AddItemMenuWithParam("Legs / Lower", PED_COMPONENT_LOWER, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_HAND))
		AddItemMenuWithParam("Hands", PED_COMPONENT_HAND, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_FEET))
		AddItemMenuWithParam("Feet", PED_COMPONENT_FEET, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_HAIR))
		AddItemMenuWithParam("Hair", PED_COMPONENT_HAIR, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_JACKET))
		AddItemMenuWithParam("Jacket", PED_COMPONENT_JACKET, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_SUSE))
		AddItemMenuWithParam("Specials 1", PED_COMPONENT_SUSE, playerOptModifyComponent, nullptr);

	if (VariationsAvailable(PED_COMPONENT_SUS2))
		AddItemMenuWithParam("Specials 2", PED_COMPONENT_SUS2, playerOptModifyComponent, nullptr);

	AddItem("Nothing listed here?", nullptr, "Then this ped model has no options.");
}

void playerOptsModelVariations(int un1, int un2)
{
	SetHeader("Customise");
	AddItemMenuAlt("Components", playerOptComponentOptions, "~PAD_X~ = Random variation.", playerOptSetRandomComponentVariation);

	if (IS_CHAR_MODEL(myPlayerPed, PED_PLAYER)) //changing prop from -1 on niko = Fatal crash.
		printf("~r~CTL: ~w~Customising ~y~Niko/Luis ~w~is ~r~risky, ~w~avoid changing textures, expect crashes.");
	else
		AddItemMenu("Props", playerOptModelProps, nullptr);

	AddItemAlt("Helmet", playerOptAddPlayerHelmet, "~PAD_A~ = Add, ~PAD_X~ = Remove.", playerOptRemovePlayerHelmet, nullptr);

	if (NetworkGame)
		AddItemAlt("Edit multiplayer model", playerOptChangeMultiplayerModel, "Press ~PAD_A~ to change, ~PAD_X~ to apply.", playerOptUpdateMultiplayerModel, nullptr);
}

void playerOptChangeModel()
{
#ifdef Developer
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_PLAYER_MODEL))
	{
		SetHeader("Change their player");
	}
	else
#endif
	{
		SetHeader(ctlMem->m_commonStrings[GXT_STRING_PLAYER_MODEL]); //"Player -> Change model" , "Player Model"
	}
	AddItemMenu("Customise", playerOptsModelVariations, "Want a hat?");
	AddItemMenu("IG models", playerOptModelsIG, "In-game models.");
	AddItemMenu("Gang members", playerOptModelsGangMembers, nullptr);
	AddItemMenu("Foreground (Female)", playerOptModelsForegroundFemale, nullptr);
	AddItemMenu("Foreground (Male)", playerOptModelsForegroundMale, nullptr);
	AddItemMenu("Background (Female)", playerOptModelsBackgroundFemale, nullptr);
	AddItemMenu("Foreground (Male)", playerOptModelsBackgroundMale, nullptr);
	if (!bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_PLAYER_MODEL))
	{
		AddItemMenu("Objects", playerOptModelsObjects, "Sometimes being a toilet goes without question.");
		//AddItemChar("Superhero", false, superHeroModelStrings, 2, &superHeroModelSelect, playerOptChangeModelSuperhero, nullptr);
		AddItemChar("Superhero", false, &ctlMem->m_commonStrings[GXT_STRING_MALE], 2, &superHeroModelSelect, playerOptChangeModelSuperhero, nullptr);
	}

	if (myCurrentEpisode == 1) //TLAD
	{
		AddItemMenu("IG models (TLaD)", playerOptModelsIGTlad, "In-game TLaD models.");
		AddItemMenu("Gang members (Female)(TLaD)", playerOptModelsGangMembersFemaleTlad, nullptr);
		AddItemMenu("Gang members (Male)(TLaD)", playerOptModelsGangMembersMaleTlad, nullptr);
		AddItemMenu("Ambients (Female)(TLaD)", playerOptModelsAmbientsFemaleTlad, nullptr);
		AddItemMenu("Ambients (Male)(TLaD)", playerOptModelsAmbientsMaleTlad, nullptr);
		AddItemMenu("Biker buddies", playerOptModelsBikerBuddiesTlad, nullptr);
	}

	if (myCurrentEpisode == 2) //TBoGT
	{
		AddItemMenu("IG models (TBoGT)", playerOptModelsIGTbogt, "In-game TBoGT models.");
		AddItemMenu("Gang members (TBoGT)", playerOptModelsGangMembersTbogt, nullptr);
		AddItemMenu("Ambients (Female)(TBoGT)", playerOptModelsAmbientsFemaleTbogt, nullptr);
		AddItemMenu("Ambients (Male)(TBoGT)", playerOptModelsAmbientsMaleTbogt, nullptr);
		AddItemMenu("Fighters (TBoGT)", playerOptModelsFightersTbogt, nullptr);
		AddItemMenu("Mission Specific (TBoGT)", playerOptModelsMissionSpecificTbogt, nullptr);
	}
}

void toggleGravityGun()
{
	if (!bitTest(ctlMem->m_bit_sets.m_weapons, gravityGun))
		bitReset(&ctlMem->m_bit_sets.m_weapons, gravityGunExplodeMode);
}

void playerWeaponMods()
{
	SetHeader("Weapon modifications");

	if (GetMenuPlayerId() == myPlayerIndex)
	{
		AddItemBool("Gravity gun", &ctlMem->m_bit_sets.m_weapons, gravityGun, toggleGravityGun, "Switch to deagle to use.~n~By: Buffeting");
		AddItemBool("Gravity gun - Explode mode", &ctlMem->m_bit_sets.m_weapons, gravityGunExplodeMode, toggleGravityGun, "Explode vehicles instead of launching them.");
	}
	
	//set to grenade/rocket weapons etc.
	//gravity gun
	//
}

const char* WeaponsName(WEAPON_TYPE wep) 
{
	if (!NetworkGame) //niko and his phone bullshit breaks this.
		return "Weapon";

	switch (wep) 
	{
		case WEAPON_AK47: return "AK47";
		case WEAPON_DEAGLE: return "Deagle";
		case WEAPON_MP5: return "MP5";
		case WEAPON_M40A1: return "M40A1";
		case WEAPON_PISTOL: return "Pistol";
		case WEAPON_SHOTGUN: return "Pump Shotgun";
		case WEAPON_BARETTA: return "Combat Shotgun";
		case WEAPON_MICRO_UZI: return "Micro-SMG";
		case WEAPON_M4: return "Carbine Rifle";
		case WEAPON_SNIPERRIFLE: return "Combat Sniper";
		case WEAPON_RLAUNCHER: return "RPG";
		case WEAPON_EPISODIC_7: return "Automatic 9mm";
		case WEAPON_EPISODIC_6: return "Sawn-off Shotgun";
		case WEAPON_EPISODIC_2: return "Assault Shotgun";
		case WEAPON_EPISODIC_1: return "Grenade Launcher";
		case WEAPON_EPISODIC_9: return "Pistol.44";
		case WEAPON_EPISODIC_13: return "Gold SMG";
		case WEAPON_EPISODIC_12: return "Assault SMG";
		case WEAPON_EPISODIC_11: return "Automatic Shotgun";
		case WEAPON_EPISODIC_10: return "Explosive Shotgun";
		case WEAPON_EPISODIC_14: return "Advanced MG";
		case WEAPON_EPISODIC_15: return "Advanced Sniper";
		default:
			return "Weapon";
	}

	return "Weapon";
}

void setAmmoType()
{
	WEAPON_TYPE weap;

	GET_CURRENT_CHAR_WEAPON(myPlayerPed, &weap);
	char* weapDef = CWeaponInfoManager_GetInfo(weap);
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);

	if (weapDef)
	{
		if (ammoTypeSelect == 0) //Bullets (Normal)
		{
			*(int*)(weapDef + 8) = 1;
			*(int*)(weapDef + 0x0C) = 2;
			*(int*)(weapDef + 0xA0) = 5;
			*(int*)(weapDef + 0xA8) = 0;
			printf("~r~CTL: ~w~%s set to shoot bullets.", WeaponsName(weap));
		}

		if (ammoTypeSelect == 1) //rockets
		{
			*(int*)(weapDef + 8) = 3;
			*(int*)(weapDef + 0x0C) = 3;
			*(int*)(weapDef + 0xA0) = 5;
			*(int*)(weapDef + 0xA8) = 6;
			printf("~r~CTL: ~w~%s set to shoot rockets.", WeaponsName(weap));
		}

		if (ammoTypeSelect == 2) //Grenades
		{
			*(int*)(weapDef + 8) = 3;
			*(int*)(weapDef + 0x0C) = 3;
			*(int*)(weapDef + 0xA0) = 2;
			*(int*)(weapDef + 0xA8) = 0;
			*(int*)(weapDef + 0xAC) = 0;
			printf("~r~CTL: ~w~%s set to shoot grenades.", WeaponsName(weap));
		}
	}
}

/*void RAPIDFIREM4(int param)
{
    *(QWORD*)0xC2649ACC = 0x4299999A4299999A;
    print("M4 Rapid fire enabled!");
}

void HeliRapidFire(int param)
{
    XMemSet((void*)0x82CBA084, 0, 12);
    *(int*)0x82CBA07C = 0;
    print("Heli Rapid fire enabled!");
}*/
void playerOptAutoAimPlayers(int un1, int un2)
{
	if (NetworkGame) //assuming GetAutoAimPreference (READ_LOBBY_PREFERENCE) will return the current lobbies which will either be 0 or 1, in singleplayer this could be "Don't care" aka -1
	{
		bool force_auto_aim_players = bitTest(ctlMem->m_bit_sets.m_weapons, forceAutoAimPlayers);

		//DISABLE_PLAYER_LOCKON(our_index, !force_auto_aim_players);
		DISABLE_PLAYER_LOCKON(myPlayerIndex, force_auto_aim_players ? false : !GetAutoAimPreference());
		ALLOW_LOCKON_TO_FRIENDLY_PLAYERS(myPlayerIndex, force_auto_aim_players ? true : GetAutoAimPreference());
	}
	/*
	PLAYER_INDEX myPlayerID = myPlayerIndex;
	bool AutoAim = IS_BIT_SET(ctlMem->m_bit_sets.m_weapons, forceAutoAimPlayers);
	DISABLE_PLAYER_LOCKON(myPlayerID, !AutoAim);
	//GetAutoAimPreference()
	ALLOW_LOCKON_TO_FRIENDLY_PLAYERS(myPlayerID, AutoAim);
	ALLOW_LOCKON_TO_RANDOM_PEDS(myPlayerID, AutoAim);
	*/
}

void playerOptAutoAimPeds(int un1, int un2)
{
	ALLOW_LOCKON_TO_RANDOM_PEDS(myPlayerIndex, bitTest(ctlMem->m_bit_sets.m_weapons, forceAutoAimPeds));
}

void playerOptSetNeverTargetted(int un1, int un2)
{
	SET_CHAR_NEVER_TARGETTED(myPlayerPed, bitTest(ctlMem->m_bit_sets.m_weapons, forceNeverTargettedPed));
}

void playerWeaponOptions()
{
	static char* allWeaponsString[3] = { "Advanced", "Poor", "TBoGT" };
	static char* ammoTypeStrings[3] = { "Bullets", "Rockets", "Grenades" };

	if (GetMenuPlayerId() == myPlayerIndex)
	{
		SetHeader(ctlMem->m_commonStrings[GXT_STRING_WEAPONS]); //"Player -> Weapons", "Weapons"
		AddItemBool("Unlimited ammo", &ctlMem->m_bit_sets.m_weapons, unlimitedAmmo, nullptr, nullptr); //could probably give this to other players.
		AddItemBool("Unlimited clip", &ctlMem->m_bit_sets.m_weapons, unlimitedClip, nullptr, nullptr); //could probably give this to other players.
		AddItemBool(ctlMem->m_assembled_strings[ASSEMBLED_STR_AUTO_AIM_PLAYERS], &ctlMem->m_bit_sets.m_weapons, forceAutoAimPlayers, playerOptAutoAimPlayers, "Force auto-aim on players."); //"Auto-aim", "Auto-Aim"
		AddItemBool(ctlMem->m_assembled_strings[ASSEMBLED_STR_AUTO_AIM_PEDS], &ctlMem->m_bit_sets.m_weapons, forceAutoAimPeds, playerOptAutoAimPeds, "Force auto-aim on random peds.");
		AddItemBool("Never targetted (Ped)", &ctlMem->m_bit_sets.m_weapons, forceNeverTargettedPed, playerOptSetNeverTargetted, "Disable auto-aim for others on your ped.");
		AddItemBool("Never targetted (Vehicle)", &ctlMem->m_bit_sets.m_weapons, forceNeverTargettedVehicle, nullptr, "Disable auto-aim for others on your vehicle."); //TODO NATH: Add loop for forceNeverTargettedVehicle
		//AddItemBool("Never targetted (Vehicle)", ctlMem->m_bit_sets.m_weapons, forceNeverTargettedVehicle , playerOptSetNeverTargetted, "Disable auto-aim for others on your vehicles.");
		//TODO NATH, Add "Infinite clip" and remove infinite clip from infinite ammo mode.
		//TODO add force auto aim, also disable lockon of player/vehicle :D
		AddItemChar("Set ammo type", false, ammoTypeStrings, 3, &ammoTypeSelect, setAmmoType, nullptr);
		AddItemMenu("Weapon modifications", playerWeaponMods, nullptr);
	}
	else
	{
		SetHeaderForwarded();
		AddItemBool("Unlimited ammo", &ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, GetMenuPlayerId(), nullptr, "Keep replenishing players ammo.");
	}

	AddItemBool("Infinite armour", &ctlMem->m_bit_sets.m_infiniteArmour_instantRevive, GetMenuPlayerId(), nullptr, "Constantly give max armour.");
	AddItemChar("Give all weapons", false, allWeaponsString, myCurrentEpisode == 2 ? 3 : 2, &weaponsStringSelect, GivePlayerWeapons, nullptr);
	AddItem("Remove all weapons", RemoveAllPlayersWeapons, nullptr);
	AddItem("Remove current Weapon", RemovePlayersWeapon, nullptr);
	AddItemPlayerWeapon("Baseball Bat", WEAPON_BASEBALLBAT);
	AddItemPlayerWeapon("Pool Cue", WEAPON_POOLCUE);
	AddItemPlayerWeapon("Knife", WEAPON_KNIFE);
	AddItemPlayerWeapon("Pistol", WEAPON_PISTOL);
	AddItemPlayerWeapon("Combat Pistol", WEAPON_DEAGLE);
	AddItemPlayerWeapon("Pump Shotgun", WEAPON_SHOTGUN);
	AddItemPlayerWeapon("Combat Shotgun", WEAPON_BARETTA);
	AddItemPlayerWeapon("Micro-SMG", WEAPON_MICRO_UZI);
	AddItemPlayerWeapon("SMG", WEAPON_MP5);
	AddItemPlayerWeapon("Assault Rifle", WEAPON_AK47);
	AddItemPlayerWeapon("Carbine Rifle", WEAPON_M4);
	AddItemPlayerWeapon("Combat Sniper", WEAPON_SNIPERRIFLE);
	AddItemPlayerWeapon("Sniper Rifle", WEAPON_M40A1);
	AddItemPlayerWeapon("RPG", WEAPON_RLAUNCHER);
	AddItemPlayerWeapon("Grenade", WEAPON_GRENADE);
	AddItemPlayerWeapon("Molotov", WEAPON_MOLOTOV);

	if (myCurrentEpisode == EPISODE_TLAD) //TLaD - 1
	{
		AddItemPlayerWeapon("Automatic 9mm", WEAPON_EPISODIC_7);
		AddItemPlayerWeapon("Sawn-off Shotgun", WEAPON_EPISODIC_6);
		AddItemPlayerWeapon("Assault Shotgun", WEAPON_EPISODIC_2);
		AddItemPlayerWeapon("Grenade Launcher", WEAPON_EPISODIC_1);
		AddItemPlayerWeapon("Pipe Bomb", WEAPON_EPISODIC_8);
		AddItemPlayerWeapon("Pool Cue (TLAD)", WEAPON_EPISODIC_4);
	}

	if (myCurrentEpisode == EPISODE_TBOGT) //TBoGT - 2
	{
		AddItemPlayerWeapon("Pistol.44", WEAPON_EPISODIC_9);
		AddItemPlayerWeapon("Gold SMG", WEAPON_EPISODIC_13);
		AddItemPlayerWeapon("Assault SMG", WEAPON_EPISODIC_12);
		AddItemPlayerWeapon("Automatic Shotgun", WEAPON_EPISODIC_11);
		AddItemPlayerWeapon("Explosive Shotgun", WEAPON_EPISODIC_10);
		AddItemPlayerWeapon("Advanced MG", WEAPON_EPISODIC_14);
		AddItemPlayerWeapon("Advanced Sniper", WEAPON_EPISODIC_15);
		AddItemPlayerWeapon("Grenade Launcher", WEAPON_EPISODIC_1);
		AddItemPlayerWeapon("Sticky Bomb", WEAPON_EPISODIC_16);
		AddItemPlayerWeapon("Parachute", WEAPON_EPISODIC_21);
	}
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-conversion" 

void pedAnimOptsStripper()
{
	SetHeader("Stripper Animations");
	AddItemWithTwoParams("Pole dance", "missstripclublo", "pole_dance_a", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Pole dance 2", "missstripclub", "Dance_Ragga_2", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Pole dance 3", "missstripclub", "Dance_Wisper_2", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Pole dance 4", "missstripclub", "Dance_Rick", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Lap dance 1", "missstripclubhi", "lap_triangle_b3_woman", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Lap dance 2", "missstripclubhi", "lap_triangle_a3_woman", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Standing proposition", "missstripclub", "standing_proposition", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Watch lap dance", "missstripclub", "watch_lap_dance_loop", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Wipe counter", "missstripclublo", "wipe_counter", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Clean glass", "missstripclublo", "clean_glass", PlayPedAnimations, nullptr);
}

void pedAnimOptsTaiChi()
{
	SetHeader("Tai Chi Animations");
	AddItemWithTwoParams("Taichi 1", "amb@park_taichi_a", "taichi01", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Taichi 2", "amb@park_taichi_b", "taichi02", PlayPedAnimations, nullptr);
}

void pedAnimOptsDances()
{
	SetHeader("Dance Animations");
	AddItemWithTwoParams("Male Dance 1", "amb@dance_maleidl_a", "loop_a", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Male Dance 2", "amb@dance_maleidl_b", "loop_b", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Male Dance 3", "amb@dance_maleidl_c", "loop_c", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Male Dance 4", "amb@dance_maleidl_d", "loop_d", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Female Dance 1", "amb@dance_femidl_a", "loop_a", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Female Dance 2", "amb@dance_femidl_b", "loop_b", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Female Dance 3", "amb@dance_femidl_c", "loop_c", PlayPedAnimations, nullptr);
}

void pedAnimOptsGestures()
{
	SetHeader("Gesture Animations");
	AddItemWithTwoParams("Busted", "busted", "idle_2_hands_up", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Busted (On bike)", "busted", "busted_on_bike", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Finger (Male)", "gestures@mp_male", "finger", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Finger (Female)", "gestures@mp_female", "finger", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Salute (Male)", "gestures@mp_male", "salute", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Salute (Female)", "gestures@mp_female", "salute", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Wave (Male)", "gestures@mp_male", "wave", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Wave (Female)", "gestures@mp_female", "wave", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Rock (Male)", "gestures@mp_male", "rock", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Rock (Female)", "gestures@mp_female", "rock", PlayPedAnimations, nullptr);
}

void pedAnimOptsSwat()
{
	SetHeader("SWAT Animations");
	AddItemWithTwoParams("Attention", "swat", "signal_attention", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Say again", "swat", "signal_sayagain", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Negative", "swat", "signal_negative", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Advance", "swat", "signal_advance", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Assault", "swat", "signal_assault", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Go right", "swat", "signal_goright", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Go left", "swat", "signal_goleft", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Go back", "swat", "signal_goback", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Go forward", "swat", "signal_gofwd", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Roger", "swat", "signal_roger", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Stop", "swat", "signal_stop", PlayPedAnimations, nullptr);
	AddItemWithTwoParams("Look", "swat", "signal_look", PlayPedAnimations, nullptr);
}

void SetLocalPlayerAnimGroup(char* animGroup, int unused0)
{
	REQUEST_ANIMS(animGroup);
	while (!HAVE_ANIMS_LOADED(animGroup)) 
		WAIT(0);

	SET_ANIM_GROUP_FOR_CHAR(myPlayerPed, animGroup);
	REMOVE_ANIMS(animGroup);
}

void stopPlayerAnim(bool loopAnims, int unused0)
{
	if (!loopAnims)
	{
		CLEAR_CHAR_TASKS_IMMEDIATELY(myPlayerPed);

		//SET_CHAR_ANIM_CURRENT_TIME(myPlayerPed, playingAnimSet, playingAnimName, 1.0f); //immediately finish anim.
		//playingAnimName = "Nada"; //should stop looping starting when re-toggled. //can kill this..

		SetLocalPlayerAnimGroup("move_player", 0);
	}
}

void pedAnimOptsGroup()
{
	SetHeader("lah");
	AddItemWithParam("Normal", "move_player", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("SWAT", "move_m@swat", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Fat", "move_m@fat", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Sexy", "move_f@sexy", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Cower", "move_m@cower", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Cuffed", "move_m@h_cuffed", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Casual", "move_m@casual", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Gay", "move_m@bernie", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("CJ (San Andreas)", "move_gng@afro_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Cop", "move_cop", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Fat Cop", "move_cop_fat", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Bum", "move_m@bum", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Old", "move_m@old_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Injured", "move_injured_generic", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Roman Injured", "move_m@roman_inj", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Zombie", "move_injured_lower", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Melee", "move_melee", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Crouch", "move_crouch", SetLocalPlayerAnimGroup, nullptr);

	//FROM STU
	//AddItemWithParam("Cop", "move_cop", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Bernie", "move_m@bernie", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Playboy", "move_m@playboy", SetLocalPlayerAnimGroup, nullptr);

	//female
	AddItemWithParam("move_f@bness_a", "move_f@bness_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@bness_b", "move_f@bness_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@bness_c", "move_f@bness_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@bness_d", "move_f@bness_d", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@bness_e", "move_f@bness_e", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Female Casual", "move_f@casual", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@casual_b", "move_f@casual_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@casual_c", "move_f@casual_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Female Sexy", "move_f@sexy", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Female Fat", "move_f@fat", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Female Old", "move_f@old_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@old_b", "move_f@old_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@old_c", "move_f@old_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@old_d", "move_f@old_d", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@puffer", "move_f@puffer", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Michelle", "move_f@michelle", SetLocalPlayerAnimGroup, nullptr);

	//male
	//AddItemWithParam("SWAT", "move_m@swat", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_m@cs_swat", "move_m@cs_swat", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_m@bness_a", "move_m@bness_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_m@bness_b", "move_m@bness_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_m@bness_c", "move_m@bness_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Male Casual", "move_m@casual", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@casual_b", "move_m@casual_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_f@casual_c", "move_m@casual_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Male Bum", "move_m@bum", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Male Fat", "move_m@fat", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_m@old_a", "move_m@old_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_m@old_b", "move_m@old_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_m@old_c", "move_m@old_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Tourist", "move_m@tourist", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_m@case", "move_m@case", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("Eddie", "move_m@eddie", SetLocalPlayerAnimGroup, nullptr);

	//gang
	AddItemWithParam("move_gng@afro_a", "move_gng@afro_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@afro_b", "move_gng@afro_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@afro_c", "move_gng@afro_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@generic_a", "move_gng@generic_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@generic_b", "move_gng@generic_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@generic_c", "move_gng@generic_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@latino_a", "move_gng@latino_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@latino_b", "move_gng@latino_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@latino_c", "move_gng@latino_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@jam_a", "move_gng@jam_a", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@jam_b", "move_gng@jam_b", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@jam_c", "move_gng@jam_c", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("move_gng@jam_c", "move_gng@jam_c", SetLocalPlayerAnimGroup, nullptr); //duplicate? wtf rockstar

	/* weapon groups - these dont work freeze your ped*/
	AddItemWithParam("gun@ak47", "gun@ak47", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("gun@baretta", "gun@baretta", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("gun@deagle", "gun@deagle", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("gun@handgun", "gun@handgun", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("gun@mp5k", "gun@mp5k", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("gun@rocket", "gun@rocket", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("gun@shotgun", "gun@shotgun", SetLocalPlayerAnimGroup, nullptr);
	AddItemWithParam("gun@uzi", "gun@uzi", SetLocalPlayerAnimGroup, nullptr);

	/* gesture groups - these dont work freeze your ped*/
	AddItemWithParam("gestures@m_seated", "gestures@m_seated", SetLocalPlayerAnimGroup, nullptr);
	//AddItem("SWAT", pedAnimSetSWATTest, "test");
}

#pragma clang diagnostic pop

void pedAnimationOptions()
{
	SetHeader(ctlMem->m_commonStrings[GXT_STRING_ANIMATION]); //"Ped Animations", "Animation"
#ifdef AdminOptions 
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_ANIMATION)) //take off first item if in admin selection
		AddItemWithParam("Stop their current animation", ADMIN_CMD_STOP_ANIMS, SendAdminCommandToPlayer, nullptr);
	else
#endif
	{
		AddItemBool("Loop anims", &ctlMem->m_bit_sets.m_player, loopAnims, stopPlayerAnim, "Play anims indefinitely, toggle to reset.");
		AddItemMenu("Change anim group", pedAnimOptsGroup, "Walk like a SWAT!");
	}
	AddItemMenu("Dances", pedAnimOptsDances, nullptr);
	AddItemMenu("SWAT", pedAnimOptsSwat, nullptr);
	AddItemMenu("Gestures", pedAnimOptsGestures, nullptr);
	AddItemMenu("Stripper", pedAnimOptsStripper, nullptr);
	AddItemMenu("Tai Chi", pedAnimOptsTaiChi, nullptr); //Release your inner Chink? nah idk lol.
}

void toggleDrunkMode()
{
	if (bitTest(ctlMem->m_bit_sets.m_player, drunkMode))
	{
		SET_PED_IS_DRUNK(myPlayerPed, 1);		// possibly no effect
		SET_DRUNK_CAM(myPlayerPed, 0.70000000, 60000);
		SWITCH_PED_TO_RAGDOLL(myPlayerPed, 0, 60000, 1, 1, 1, 0);
		CREATE_NM_MESSAGE(1, 79);			// ?? I have no idea if any of these comments are right
		SET_NM_MESSAGE_FLOAT(89, 8.70000000);		// HEAD_LOOK_ALWAYS_TIMEOUT
		SET_NM_MESSAGE_FLOAT(98, 0.60000000);		// BODY_TURN_TIMEOUT
		SET_NM_MESSAGE_FLOAT(81, 8.39999900);		// BODY_STIFFNESS
		SET_NM_MESSAGE_FLOAT(82, 0.70000000);		// DAMPING
		SET_NM_MESSAGE_INT(85, 65535);			// ?? MAX_STEPS ??
		SET_NM_MESSAGE_BOOL(95, 1);			// ?? ARMS_OUT_ON_PUSH ??
		SET_NM_MESSAGE_FLOAT(101, 0.80000000);		// ARMS_OUT_ON_PUSH_MULTIPLIER
		SET_NM_MESSAGE_FLOAT(102, 999.00000000);	// ARMS_OUT_ON_PUSH_TIMEOUT
		SET_NM_MESSAGE_FLOAT(84, 1.40000000);		// SHOULDER
		SET_NM_MESSAGE_FLOAT(83, 1.95000000);		// ELBOW
		SET_NM_MESSAGE_FLOAT(94, 1.00000000);		// RETURNING_TO_BALANCE_ARMS_OUT
		SET_NM_MESSAGE_FLOAT(110, 0.00000000);		// TURN_AWAY_PROB
		SET_NM_MESSAGE_FLOAT(111, 0.10000000);		// TURN_LEFT_PROB
		SET_NM_MESSAGE_FLOAT(112, 0.10000000);		// TURN_RIGHT_PROB
		SET_NM_MESSAGE_FLOAT(108, 0.00000000);		// TURN_OFF_PROB
		SET_NM_MESSAGE_FLOAT(113, 0.60000000);		// TURN2_TARGET_PROB
		SET_NM_MESSAGE_FLOAT(109, 0.20000000);		// TURN2_VEL_PROB
		SET_NM_MESSAGE_FLOAT(91, 0.10000000);		// SOMERSAULT_ANGLE_THRESHOLD
		SET_NM_MESSAGE_FLOAT(93, 0.10000000);		// SIDE_SOMERSAULT_ANGLE_THRESHOLD
		SET_NM_MESSAGE_FLOAT(106, -0.30000000);		// BEND_ELBOWS_GAIT
		SEND_NM_MESSAGE(myPlayerPed);		// ??
		GIVE_PLAYER_RAGDOLL_CONTROL(myPlayerIndex, 1);

		//SWITCH_PED_TO_ANIMATED(myPlayerPed, true); //testing network sync
	}
	else
	{
		SET_PED_IS_DRUNK(myPlayerPed, 0);		// possibly no effect
		SET_DRUNK_CAM(myPlayerPed, 0.00010000, 1000);
		SWITCH_PED_TO_ANIMATED(myPlayerPed, FALSE);
		GIVE_PLAYER_RAGDOLL_CONTROL(myPlayerIndex, 0);
	}
}

/*HostVars._fu128[5_POLICE]
						SET_MAX_WANTED_LEVEL( 6 );
                        SET_WANTED_MULTIPLIER( 1.00000000 );
                        SET_CREATE_RANDOM_COPS( 1 );
                        SET_DITCH_POLICE_MODELS( 0 );*/


void rigNeverWantedGlobal(int un1, int un2)
{
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL: setGlobalAtIndex(8633, bitTest(ctlMem->m_bit_sets.m_player, neverWanted)); return; //Disables police near aipoort control tower (and probably some other stuff).
	case EPISODE_TLAD: setGlobalAtIndex(9186, bitTest(ctlMem->m_bit_sets.m_player, neverWanted)); return; //Pulled from TLaD "ambairpotarea.sco"
	case EPISODE_TBOGT: setGlobalAtIndex(9235, bitTest(ctlMem->m_bit_sets.m_player, neverWanted)); return; //Pulled from TBoGT "ambairpotarea.sco"
	}
	//TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("ambairpotarea"); //doesn't seem to do shit.
}

void PlayerOptions()
{
	SetBit(&ctlMem->m_bit_sets.m_player, Invisibility, !IS_CHAR_VISIBLE(myPlayerPed));
	SetBit(&ctlMem->m_bit_sets.m_player, playerRagdoll, IS_PED_RAGDOLL(myPlayerPed));
	STORE_WANTED_LEVEL(myPlayerIndex, &wantedLvlInt);

	if (NetworkGame)
		SetBit(&ctlMem->m_bit_sets.m_player, playerGodMode, IsPlayerInvincible(myPlayerIndex));

	SetHeader(ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_PLAYER]); //"Player Options", "Player"

	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_PLAYER_MODEL], playerOptChangeModel, "Change your character."); //"Model", "Player Model"
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_ANIMATION], pedAnimationOptions, "Ped animations, wanna dance?"); //"Anims", "Animation"
	AddItemMenu("Voice", playerOptChangeVoice, "Credit: Buffeting; Say some crazy shit.");
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_WEAPONS], playerWeaponOptions, nullptr); // "Weapons"
	AddItemBool("God mode", &ctlMem->m_bit_sets.m_player, playerGodMode, SetPlayerGodMode, "Make your character invincible.");
	AddItemBool("Invisibility", &ctlMem->m_bit_sets.m_player, Invisibility, SetPlayerVisibility, "Make your character invisible.");
	AddItemBool("Never wanted", &ctlMem->m_bit_sets.m_player, neverWanted, rigNeverWantedGlobal, "Constantly clear wanted level.");
	AddItemInt("Set wanted level", false, 0, 6, &wantedLvlInt, 1, setWantedLvl, "Only works if game has police enabled.");
	AddItemBool("Super run", &ctlMem->m_bit_sets.m_player, superRun, nullptr, "Hold ~PAD_LB~ while running.");
	AddItemBool("Super jump", &ctlMem->m_bit_sets.m_player, superJump, nullptr, nullptr);
	AddItemBool("Ragdoll", &ctlMem->m_bit_sets.m_player, playerRagdoll, playerOptToggleRagdoll, "Floppidy flop.");
	AddItemBool("Drunk Mode", &ctlMem->m_bit_sets.m_player, drunkMode, toggleDrunkMode, "Doesn't show for other players.");
	AddItemMenu("Questionable", netPlayerQuestionableActions, "Ever wanted to burn your life to the ground?");
	AddItemBool("Smoke spliff 24/7", &ctlMem->m_bit_sets.m_player, smoke_spliff_mode, nullptr, "Wah-gwan me bredrin.");
	AddItemBool("Fly mod", &ctlMem->m_bit_sets.m_player, flyMod, worldOptsToggleFlymod, "Also known as no-clip, ~PAD_RB~ = faster, ~PAD_LB~ = slower.");
}

void GetAimOffset(float distance, float *x, float *y, float *z) 
{
	vector3 p;

	GET_PED_BONE_POSITION(myPlayerPed, BONE_RIGHT_HAND, distance, distance * 0.042, distance * -0.113, &p);
	*x = p.x;
	*y = p.y;
	*z = p.z;
}

bool GetClosestNetworkPlayerVehicleToCoordinate(float mX, float mY, float mZ, bool checkOnScreen, VEHICLE_INDEX *closest)
{
	float nX, nY, nZ, dist, lowestDist = 45.0f;
	PED_INDEX netPed;
	VEHICLE_INDEX netVeh = 0;
	bool found = false;

	for (int i = 0; i < 16; i++)
	{
		if (IsPlayerValid(i) && PLAYER_HAS_CHAR(i))
		{
			GET_PLAYER_CHAR(i, &netPed);

			if (IS_CHAR_IN_ANY_CAR(netPed)) //this only executes when we're on foot so meh @ skipping i == myPlayerIndex
			{
				GET_CHAR_COORDINATES(netPed, &nX, &nY, &nZ);
				GET_DISTANCE_BETWEEN_COORDS_3D(nX, nY, nZ, mX, mY, mZ, &dist);

				if (dist < lowestDist)
				{
					GET_CAR_CHAR_IS_USING(netPed, &netVeh);

					if (!checkOnScreen || IS_CAR_ON_SCREEN(netVeh))
					{
						lowestDist = dist;
						*closest = netVeh;
						found = true;
					}
				}
			}
		}
	}

	return found;
}


bool GetClosestNetworkPlayerVehicle(VEHICLE_INDEX *closest)
{
	float mX, mY, mZ;

	GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
	if (GetClosestNetworkPlayerVehicleToCoordinate(mX, mY, mZ, true, &*closest))
		return true;

	return false;
}

/* //BACKUP!
bool GetClosestNetworkPlayerVehicle(Vehicle *closest)
{
	float mX, mY, mZ, nX, nY, nZ, dist, lowestDist = 45.0f;
	GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
	int netPed, netVeh = 0;
	bool found = false;
	for (int i = 0; i < 16; i++)
	{
		if (IsPlayerValid(i) && PLAYER_HAS_CHAR(i))
		{
			GET_PLAYER_CHAR(i, &netPed);
			if (IS_CHAR_IN_ANY_CAR(netPed)) //this only executes when we're on foot so meh @ skipping i == myPlayerIndex
			{
				GET_CHAR_COORDINATES(netPed, &nX, &nY, &nZ);
				GET_DISTANCE_BETWEEN_COORDS_3D(nX, nY, nZ, mX, mY, mZ, &dist);
				if (dist < lowestDist)
				{
					GET_CAR_CHAR_IS_USING(netPed, &netVeh);
					if (IS_CAR_ON_SCREEN(netVeh))
					{
						lowestDist = dist;
						*closest = netVeh;
						found = true;
					}
				}
			}
		}
	}
	return found;
}
*/

VEHICLE_INDEX GetClosestVehicle(float x, float y, float z, float radius)
{
	CPool* VehPool = *(CPool**)(0x82FB0700); //Vehicle Pool 
	float sX, sY, sZ, dist, lowestDist = 70.0f;
	VEHICLE_INDEX closestVeh = INVALID_HANDLE;

	for (int i = 0; i < VehPool->m_maxHandle; i++)
	{
		uint NetPtr = (uint)(VehPool->m_firstPointer + (i * VehPool->m_structSize));
		uint EntityPtr = *(uint*)(NetPtr + 0x4);

		if (EntityPtr && EntityPtr != 0xCDCDCDCD) //prety sure this means it exists..
		{
			VEHICLE_INDEX scanVeh = GetHandleFromPool(*(CPool**)(0x82FB0700), EntityPtr); //Vehicle  Pool

			if (DOES_VEHICLE_EXIST(scanVeh))
			{
				GET_CAR_COORDINATES(scanVeh, &sX, &sY, &sZ);
				GET_DISTANCE_BETWEEN_COORDS_3D(sX, sY, sZ, x, y, z, &dist);

				if (dist > radius) 
					continue;

				if (dist < lowestDist)
				{
					lowestDist = dist;
					closestVeh = scanVeh;
				}
			}
		}
	}

	return closestVeh;
}
/*void SpliffSmokeLoop()
{
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_player, smoke_spliff_mode))
	{
		if (!HAVE_ANIMS_LOADED("amb@smoking_spliff"))
		{
			REQUEST_ANIMS("amb@smoking_spliff");
			return;
		}
		if (DOES_OBJECT_EXIST(freeMem->player_spliff.spliff_obj))
		{
			if (!IS_BIT_SET(ctlMem->m_bit_sets.m_player, createdSpliffObjPTFX))
			{
				if (IS_CHAR_PLAYING_ANIM(myPlayerPed, "amb@smoking_spliff", "create_spliff"))
				{
					float anim_time;
					GET_CHAR_ANIM_CURRENT_TIME(myPlayerPed, "amb@smoking_spliff", "create_spliff", &anim_time);
					if (anim_time > 0.650f)
					{
						//freeMem->player_spliff.obj_smoke = START_PTFX_ON_OBJ("ambient_cig_smoke", freeMem->player_spliff.spliff_obj, 0.0f, 0.0f, 0.1360f, 0.0f, 0.0f, 0.0f, 1065353216); //1.0 as a float.
						freeMem->player_spliff.obj_smoke = START_PTFX_ON_OBJ("ambient_cig_smoke", freeMem->player_spliff.spliff_obj, 0.0f, 0.0f, 0.1360f, 0.0f, 0.0f, 0.0f, 1.0f);
						SET_BIT(&ctlMem->m_bit_sets.m_player, createdSpliffObjPTFX);
					}
				}
				else
				{
					freeMem->player_spliff.obj_smoke = START_PTFX_ON_OBJ("ambient_cig_smoke", freeMem->player_spliff.spliff_obj, 0.0f, 0.0f, 0.1360f, 0.0f, 0.0f, 0.0f, 1.0f);
					SET_BIT(&ctlMem->m_bit_sets.m_player, createdSpliffObjPTFX);
				}
			}

			int rando;
			GENERATE_RANDOM_INT_IN_RANGE(4, 7, &rando);
			//vehicle_cigurette_smoke = START_PTFX_ON_VEH("smoke_filled_car", GetPlayerVeh(), 0, 0, 0, 0.0f, 0.0f, 90.0f, 1.0f);
			if (IS_CHAR_SITTING_IN_ANY_CAR(myPlayerPed))
			{
				//start ptfx on ped bone, bone head, smoke_filled_car 
				if (!IS_CHAR_PLAYING_ANIM(myPlayerPed, "amb@smoking_spliff", "partial_smoke_car"))
				{
					//timer stuff.
					BLOCK_CHAR_HEAD_IK(myPlayerPed, false);
					BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, false);
					if (freeMem->player_spliff.spliff_tick < GetTickCount())
					{
						if (!IS_CHAR_SHOOTING(myPlayerPed) && !IS_CHAR_GESTURING(myPlayerPed))
						{
							TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "partial_smoke_car", "amb@smoking_spliff", 8.00000000, -1, 3328);
							freeMem->player_spliff.spliff_tick = (GetTickCount() + 8000 + (rando * 1000));
							EVOLVE_PTFX(freeMem->player_spliff.obj_smoke, "fade", 0.0f);
						}
					}
				}
				else
				{
					BLOCK_CHAR_HEAD_IK(myPlayerPed, true);
					BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, true);
				}
			}
			else
			{
				if (!IS_CHAR_PLAYING_ANIM(myPlayerPed, "amb@smoking_spliff", "partial_smoke"))
				{
					//timer stuff
					BLOCK_CHAR_HEAD_IK(myPlayerPed, false);
					BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, false);
					if (freeMem->player_spliff.spliff_tick < GetTickCount())
					{
						if (!IS_CHAR_SHOOTING(myPlayerPed) && !IS_CHAR_GESTURING(myPlayerPed) && !IS_CHAR_IN_MELEE_COMBAT(myPlayerPed) && !IS_CHAR_IN_WATER(myPlayerPed))
						{
							TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "partial_smoke", "amb@smoking_spliff", 8.00000000, -1, 3328);
							freeMem->player_spliff.spliff_tick = (GetTickCount() + 8000 + (rando * 1000));
							EVOLVE_PTFX(freeMem->player_spliff.obj_smoke, "fade", 0.0f);
						}
					}
				}
				else
				{
					BLOCK_CHAR_HEAD_IK(myPlayerPed, true);
					BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, true);
				}
			}

		}
		else
		{
			if (CreateObject(OBJECT_EC_SPLIFF, 0.0f, 0.0f, 0.0f, 0.0f, &freeMem->player_spliff.spliff_obj, true))
			{
				SET_OBJECT_COLLISION(freeMem->player_spliff.spliff_obj, false);
				ATTACH_OBJECT_TO_PED(freeMem->player_spliff.spliff_obj, myPlayerPed, BONE_RIGHT_HAND, 0.1370f, 0.0310f, 0.0350f, 2.2140f, -0.1210f, 0.0f, false);
				freeMem->player_spliff.spliff_tick = (GetTickCount() + 10000); //10 secs then do more.
				BLOCK_CHAR_HEAD_IK(myPlayerPed, true);
				BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, true);
				TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "create_spliff", "amb@smoking_spliff", 8.00000000, -1, 3328);
			}
		}
	}
	else
	{
		if (DOES_OBJECT_EXIST(freeMem->player_spliff.spliff_obj))
		{
			STOP_PTFX(freeMem->player_spliff.obj_smoke);
			DeleteObject(&freeMem->player_spliff.spliff_obj);
			bitReset(&ctlMem->m_bit_sets.m_player, createdSpliffObjPTFX);
			BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, false);
			BLOCK_CHAR_HEAD_IK(myPlayerPed, false);
		}
	}
}*/
void SpliffSmokeLoop()
{
	if (bitTest(ctlMem->m_bit_sets.m_player, smoke_spliff_mode))
	{
		if (!HAVE_ANIMS_LOADED("amb@smoking_spliff"))
		{
			REQUEST_ANIMS("amb@smoking_spliff");
			return;
		}
		if (DOES_OBJECT_EXIST(freeMem->player_spliff.spliff_obj))
		{
			if (!bitTest(ctlMem->m_bit_sets.m_player, createdSpliffObjPTFX))
			{
				if (IS_CHAR_PLAYING_ANIM(myPlayerPed, "amb@smoking_spliff", "create_spliff"))
				{
					float anim_time;
					GET_CHAR_ANIM_CURRENT_TIME(myPlayerPed, "amb@smoking_spliff", "create_spliff", &anim_time);
					if (anim_time > 0.650f)
					{
						//freeMem->player_spliff.obj_smoke = START_PTFX_ON_OBJ("ambient_cig_smoke", freeMem->player_spliff.spliff_obj, 0.0f, 0.0f, 0.1360f, 0.0f, 0.0f, 0.0f, 1065353216); //1.0 as a float.
						freeMem->player_spliff.obj_smoke = START_PTFX_ON_OBJ("ambient_cig_smoke", freeMem->player_spliff.spliff_obj, 0.0f, 0.0f, 0.1360f, 0.0f, 0.0f, 0.0f, 1.0f);
						bitSet(&ctlMem->m_bit_sets.m_player, createdSpliffObjPTFX);
					}
				}
				else
				{
					freeMem->player_spliff.obj_smoke = START_PTFX_ON_OBJ("ambient_cig_smoke", freeMem->player_spliff.spliff_obj, 0.0f, 0.0f, 0.1360f, 0.0f, 0.0f, 0.0f, 1.0f);
					bitSet(&ctlMem->m_bit_sets.m_player, createdSpliffObjPTFX);
				}
			}

			int rando;
			GENERATE_RANDOM_INT_IN_RANGE(4, 7, &rando);
			//vehicle_cigurette_smoke = START_PTFX_ON_VEH("smoke_filled_car", GetPlayerVeh(), 0, 0, 0, 0.0f, 0.0f, 90.0f, 1.0f); //not tried with 1.0f.
			////start ptfx on ped bone, bone head, smoke_filled_car 
			bool sitting_in_car = IS_CHAR_SITTING_IN_ANY_CAR(myPlayerPed);
			char* anim_name = (sitting_in_car ? "partial_smoke_car" : "partial_smoke");
				
			if (!IS_CHAR_PLAYING_ANIM(myPlayerPed, "amb@smoking_spliff", anim_name))
			{
				//timer stuff
				BLOCK_CHAR_HEAD_IK(myPlayerPed, false);
				BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, false);
				if (freeMem->player_spliff.spliff_tick < GetTickCount())
				{
					if (!IS_CHAR_SHOOTING(myPlayerPed) && !IS_CHAR_GESTURING(myPlayerPed) && !IS_CHAR_IN_MELEE_COMBAT(myPlayerPed) && !IS_CHAR_IN_WATER(myPlayerPed))
					{
						//TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, anim_name, "amb@smoking_spliff", 8.00000000, -1, 3328);
						//TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, anim_name, "amb@smoking_spliff", 8.00000000, 0, 3328); //still doesn't see anim
						TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, anim_name, "amb@smoking_spliff", 8.00000000, -1, 0);
						freeMem->player_spliff.spliff_tick = (GetTickCount() + 8000 + (rando * 1000));
						if (sitting_in_car)
						{
							if (bitTest(ctlMem->m_bit_sets.m_vehicle, Created_smoke_spliff_ptfx))
								EVOLVE_PTFX(freeMem->player_spliff.veh_smoke, "fade", 0.0f);
							else
							{
								freeMem->player_spliff.veh_smoke = START_PTFX_ON_VEH("smoke_filled_car", GetPlayerVeh(), 0, 0, 0, 0.0f, 0.0f, 90.0f, 1.0f);
								bitSet(&ctlMem->m_bit_sets.m_vehicle, Created_smoke_spliff_ptfx);
							}
						}
						else
						{
							EVOLVE_PTFX(freeMem->player_spliff.obj_smoke, "fade", 0.0f);
							if (bitTest(ctlMem->m_bit_sets.m_vehicle, Created_smoke_spliff_ptfx))
							{
								STOP_PTFX(freeMem->player_spliff.veh_smoke);
								bitReset(&ctlMem->m_bit_sets.m_vehicle, Created_smoke_spliff_ptfx);
							}
						}
					}
				}
			}
			else
			{
				BLOCK_CHAR_HEAD_IK(myPlayerPed, true);
				BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, true);
			}

		}
		else
		{
			if (CreateObject(OBJECT_EC_SPLIFF, 0.0f, 0.0f, 0.0f, 0.0f, &freeMem->player_spliff.spliff_obj, true))
			{
				SET_OBJECT_COLLISION(freeMem->player_spliff.spliff_obj, false);
				ATTACH_OBJECT_TO_PED(freeMem->player_spliff.spliff_obj, myPlayerPed, BONE_RIGHT_HAND, 0.1370f, 0.0310f, 0.0350f, 2.2140f, -0.1210f, 0.0f, false);
				freeMem->player_spliff.spliff_tick = (GetTickCount() + 10000); //10 secs then do more.
				BLOCK_CHAR_HEAD_IK(myPlayerPed, true);
				BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, true);
				//TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "create_spliff", "amb@smoking_spliff", 8.00000000, -1, 3328); //can walk but doesn't sync (people say it doesn't sync)
				//TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "create_spliff", "amb@smoking_spliff", 8.00000000, 0, 3328);
				TASK_PLAY_ANIM_WITH_FLAGS(myPlayerPed, "create_spliff", "amb@smoking_spliff", 8.00000000, -1, 0);
			}
		}
	}
	else
	{
		if (DOES_OBJECT_EXIST(freeMem->player_spliff.spliff_obj))
		{
			STOP_PTFX(freeMem->player_spliff.obj_smoke);
			DeleteObject(&freeMem->player_spliff.spliff_obj);
			bitReset(&ctlMem->m_bit_sets.m_player, createdSpliffObjPTFX);
			BLOCK_CHAR_AMBIENT_ANIMS(myPlayerPed, false);
			BLOCK_CHAR_HEAD_IK(myPlayerPed, false);
		}
	}
}


void PlayerLoopedOptions()
{
	if (!IS_IN_PLAYER_SETTINGS_MENU())
	{
		if (bitTest(ctlMem->m_bit_sets.m_debug, DEBUG_ENTERED_PLAYERSETTINGS_MENU_RESTORE_INPUT_ON_EXIT))
		{
			bitSet(&Container.BitSet, PB_IsMenuOpen);
			bitReset(&ctlMem->m_bit_sets.m_debug, DEBUG_ENTERED_PLAYERSETTINGS_MENU_RESTORE_INPUT_ON_EXIT);
		}
		if (bitTest(ctlMem->m_bit_sets.m_debug, DEBUG_ENTERED_PLAYERSETTINGS_MENU_FREEZE_WEATHER_ON_EXIT))
		{
			bitReset(&ctlMem->m_bit_sets.m_debug, DEBUG_ENTERED_PLAYERSETTINGS_MENU_FREEZE_WEATHER_ON_EXIT);
			bitSet(&ctlMem->m_bit_sets.m_player, freezeWeather);
		}
	}

	static VEHICLE_INDEX gravityVehicle;
	static uint gravityShotIgnoreTick;
	uint networkPlayerVehicleId; 
	VEHICLE_INDEX networkPlayerVehicle;
	float pX, pY, pZ, pH, distance;

	SpliffSmokeLoop();
	//BEGIN WEAPON LOOPS (move into own function later perhaps)
	if (bitTest(ctlMem->m_bit_sets.m_weapons, gravityGun) && !IS_PAUSE_MENU_ACTIVE() && IS_CHAR_ON_FOOT(myPlayerPed))
	{
		WEAPON_TYPE currentWep;

		GET_CURRENT_CHAR_WEAPON(myPlayerPed, &currentWep);

		if (currentWep == WEAPON_DEAGLE)
		{
			if (IS_BUTTON_PRESSED(0, BUTTON_LT))
			{
				if (DOES_VEHICLE_EXIST(gravityVehicle))
				{
					NETWORK_INDEX netId;

					GET_NETWORK_ID_FROM_VEHICLE(gravityVehicle, &netId);

					if (!NetworkGame || HAS_CONTROL_OF_NETWORK_ID(netId))
					{
						vector3 aim, aim2;

						if(NetworkGame)
							SET_NETWORK_ID_CAN_MIGRATE(netId, false);

						if (IS_CHAR_SHOOTING(myPlayerPed))
						{
							if (bitTest(ctlMem->m_bit_sets.m_weapons, gravityGunExplodeMode))
								EXPLODE_CAR(gravityVehicle, true, false);
							else
							{
								//Shoot car..
								float gravDist, speedZ;

								GET_PED_BONE_POSITION(myPlayerPed, BONE_RIGHT_HAND, 2.0f, 0.0f, 0.0f, &aim);
								GET_PED_BONE_POSITION(myPlayerPed, BONE_RIGHT_HAND, 100.0f, 0.0f, 0.0f, &aim2);
								GET_DISTANCE_BETWEEN_COORDS_3D(aim2.x, aim2.y, aim2.z, aim.x, aim.y, aim.z, &gravDist);
								speedZ = 135 * (aim2.z - aim.z) / gravDist;
								APPLY_FORCE_TO_CAR(gravityVehicle, 1, 100.0f, 0.0f, speedZ, 0, 0, 0, 1, 1, 1, 1);
							}

							if(NetworkGame)
								SET_NETWORK_ID_CAN_MIGRATE(netId, true);

							gravityVehicle = 0;
							gravityShotIgnoreTick = (GetTickCount() + 1000);
						}
						else
						{
							float mX, mY, mZ, mH;

							GET_CHAR_HEADING(myPlayerPed, &mH);
							GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
							GET_PED_BONE_POSITION(myPlayerPed, BONE_RIGHT_HAND, 8.0f, 0.0f, 0.0f, &aim);

							if (mH > 270)
								mH -= 360;

							mH += 90;
							SET_CAR_HEADING(gravityVehicle, mH);
							mZ = mZ - 0.5;

							if (aim.z < mZ)
								SET_CAR_COORDINATES_NO_OFFSET(gravityVehicle, aim.x, aim.y, mZ);
							else
								SET_CAR_COORDINATES_NO_OFFSET(gravityVehicle, aim.x, aim.y, aim.z);
						}
					}
					else
						REQUEST_CONTROL_OF_NETWORK_ID(netId);
				}
				else
				{
					if (gravityShotIgnoreTick < GetTickCount()) //avoids WAIT(1000).
					{
						gravityVehicle = 0;

						for (int i = 1; i < 100; i++)
						{
							float aX, aY, aZ;

							GetAimOffset(TO_FLOAT(i), &aX, &aY, &aZ);
							gravityVehicle = GET_CLOSEST_CAR(aX, aY, aZ, 1.5f, 0, 4);
							
							if (DOES_VEHICLE_EXIST(gravityVehicle))
								break;
							/*
							else
							{
								gravityVehicle = GET_CLOSEST_CAR(aX, aY, aZ, 1.5f, 0, VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_WITH_PEDS_ENTERING_OR_EXITING);
								if (DOES_VEHICLE_EXIST(gravityVehicle))
									break;
								else
								{
									gravityVehicle = GET_CLOSEST_CAR(aX, aY, aZ, 1.5f, 0, VEHICLE_SEARCH_FLAG_ALLOW_VEHICLE_OCCUPANTS_TO_BE_PERFORMING_A_NON_DEFAULT_TASK);
									if (DOES_VEHICLE_EXIST(gravityVehicle))
										break;
								}
							}
							*/
						}
						
						if (!DOES_VEHICLE_EXIST(gravityVehicle) && NetworkGame)
						{
							VEHICLE_INDEX tmp;

							//NOTE Nath this won't get the one you're necessarily aiming at.. should just use GetClosestNetworkPlayerVehicleToCoordinate which is actually called by the below function.
							if (GetClosestNetworkPlayerVehicle(&tmp))
								gravityVehicle = tmp;
						}
					}
				}
			}
			else
			{
				if (NetworkGame && DOES_VEHICLE_EXIST(gravityVehicle))
				{
					NETWORK_INDEX netId;

					GET_NETWORK_ID_FROM_VEHICLE(gravityVehicle, &netId);
					if (HAS_CONTROL_OF_NETWORK_ID(netId))
						SET_NETWORK_ID_CAN_MIGRATE(netId, true);
				}

				gravityVehicle = 0;
			}
		}
	}
	//END WEAPON LOOPS
	/*
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_player,drunkMode) && !IS_PED_RAGDOLL(myPlayerPed)) //NOT TESTED
		SET_DRUNK_CAM(myPlayerPed, 0.00010000, 1000);
	
	
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_player, neverWanted)) //can remove this to use the net loop //ctlMem->m_bit_sets.m_neverWanted_vehAutoFix
	{
		if (IS_WANTED_LEVEL_GREATER(myPlayerIndex, 0)) //stop event spam R07 has reported MD doing, cause is looping clear wanted level constantly.
			CLEAR_WANTED_LEVEL(myPlayerIndex);
	}

	if (IS_BIT_SET(ctlMem->m_bit_sets.m_weapons, unlimitedAmmo))
	{
		if (IS_CHAR_SHOOTING(myPlayerPed))
		{
			WEAPON_TYPE theirCurrent;
			GET_CURRENT_CHAR_WEAPON(myPlayerPed, (WEAPON_TYPE*)&theirCurrent);

			if (theirCurrent == WEAPON_GRENADE || theirCurrent == WEAPON_MOLOTOV)
				GIVE_WEAPON_TO_CHAR(myPlayerPed, theirCurrent, AMMO_MAX, false);
			else
			{
				int maxAmmo;
				GET_MAX_AMMO_IN_CLIP(myPlayerPed, theirCurrent, &maxAmmo);
				SET_AMMO_IN_CLIP(myPlayerPed, theirCurrent, maxAmmo);
			}
		}
	}
	*/

	if (!IS_PAUSE_MENU_ACTIVE())
	{
		if (bitTest(ctlMem->m_bit_sets.m_player, flyMod)) //Flymod
		{
			PED_INDEX driver;
			int leftX, leftY, rightX, rightY;
			float moveTrig, moveSpeed;
			float flymodCamDistance, flymodCamOffsetX, flymodCamOffsetY, flymodCamOffsetZ, flymodCamOffsetTrig;

			//create the camera for it
			if (!DOES_CAM_EXIST(scriptedCam))
			{
				CREATE_CAM(14, &scriptedCam);
				SET_CAM_ACTIVE(scriptedCam, true);
				SET_CAM_PROPAGATE(scriptedCam, true);
				ACTIVATE_SCRIPTED_CAMS(true, true);
				scriptedCamPitch = 0.0;
			}

			FREEZE_CHAR_POSITION(myPlayerPed, true);
			ATTACH_CAM_TO_PED(scriptedCam, myPlayerPed);
			POINT_CAM_AT_PED(scriptedCam, myPlayerPed);

			if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
			{
				vector3 pVehModelMinDims, pVehModelMaxDims;

				//networkPlayerVehicle = GetPlayerVeh(); 
				GET_CAR_CHAR_IS_USING(myPlayerPed, &networkPlayerVehicle); //reusing variables
				GET_CAR_MODEL(networkPlayerVehicle, &networkPlayerVehicleId); //reusing variables
				GET_MODEL_DIMENSIONS(networkPlayerVehicleId, &pVehModelMinDims, &pVehModelMaxDims);
				flymodCamDistance = 1.5 * (pVehModelMaxDims.y - pVehModelMinDims.y);
				GET_CAR_COORDINATES(networkPlayerVehicle, &pX, &pY, &pZ);
				GET_CAR_HEADING(networkPlayerVehicle, &pH);
			}
			else
			{
				GET_CHAR_COORDINATES(myPlayerPed, &pX, &pY, &pZ);
				GET_CHAR_HEADING(myPlayerPed, &pH);
				flymodCamDistance = 4.0;
			}

			//if(!IS_IN_SPECTATOR_MODE()) //?
			GET_POSITION_OF_ANALOGUE_STICKS(0, &leftX, &leftY, &rightX, &rightY);

			// heading control
			pH = pH - (rightX * 0.025f);
			if (pH > 360.0f)
				pH -= 360.0f;
			if (pH < 0.0f)
				pH += 360.0f;

			// pitch control
			if (IS_LOOK_INVERTED())
				scriptedCamPitch = scriptedCamPitch + (rightY * 0.025f);
			else
				scriptedCamPitch = scriptedCamPitch - (rightY * 0.025f);

			if (scriptedCamPitch < -70.0)
				scriptedCamPitch = -70.0;

			if (scriptedCamPitch > 70.0)
				scriptedCamPitch = 70.0;

			if (IS_BUTTON_JUST_PRESSED(0, STICK_RIGHT))
				scriptedCamPitch = 0.0f;

			moveSpeed = 1.0f;

			if (IS_BUTTON_PRESSED(0, BUTTON_RB))
				moveSpeed = 4.0f;
			else if (IS_BUTTON_PRESSED(0, BUTTON_LB))
				moveSpeed = 0.25f;

			moveTrig = (leftY * 0.0133f) * COS(scriptedCamPitch);
			pX = pX + (moveSpeed * ((moveTrig * SIN(pH)) + (leftX * 0.0133f) * COS(pH)));
			pY = pY - (moveSpeed * ((moveTrig * COS(pH)) - (leftX * 0.0133f) * SIN(pH)));
			pZ = pZ - (moveSpeed * (((leftY * 0.0133f) * SIN(scriptedCamPitch)) + (GET_CONTROL_VALUE(0, 6) * 0.0025f) - (GET_CONTROL_VALUE(0, 5) * 0.0025f)));

			if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
			{
				GET_DRIVER_OF_CAR(networkPlayerVehicle, &driver);

				if (driver == myPlayerPed || !NetworkGame)
				{
					SET_CAR_COORDINATES_NO_OFFSET(networkPlayerVehicle, pX, pY, pZ);
					SET_CAR_HEADING(networkPlayerVehicle, pH);
					FREEZE_CAR_POSITION(networkPlayerVehicle, true);
				}
			}
			else
			{
				SET_CHAR_COORDINATES_NO_OFFSET(myPlayerPed, pX, pY, pZ);
				SET_CHAR_HEADING(myPlayerPed, pH);
			}

			flymodCamOffsetTrig = (flymodCamDistance * COS(scriptedCamPitch - 18.0f));
			flymodCamOffsetX = flymodCamOffsetX + (flymodCamOffsetTrig * SIN(pH));
			flymodCamOffsetY = flymodCamOffsetY - (flymodCamOffsetTrig * COS(pH));
			flymodCamOffsetZ = flymodCamOffsetZ - (flymodCamDistance * SIN(scriptedCamPitch - 18.0f));
			SET_CAM_ATTACH_OFFSET(scriptedCam, flymodCamOffsetX, flymodCamOffsetY, flymodCamOffsetZ);
		}

		if (bitTest(ctlMem->m_bit_sets.m_player, superRun))
		{
			if (IS_BUTTON_PRESSED(0, BUTTON_A) && IS_BUTTON_PRESSED(0, BUTTON_LB))
			{
				if (!IS_CHAR_IN_AIR(myPlayerPed) && !IS_CHAR_IN_WATER(myPlayerPed))
				{
					GET_CHAR_SPEED(myPlayerPed, &distance); //reusing variables again
					APPLY_FORCE_TO_PED(myPlayerPed, true, 0.0f, (distance > 40.0f ? 100.0f : 50.0f), 0.0f, 0.0f, 0.0f, 0.0f, true, true, true, true); //makes it less jet-plane style take off.
				}
			}
		}

		if (bitTest(ctlMem->m_bit_sets.m_player, superJump))
		{
			if (IS_BUTTON_PRESSED(0, BUTTON_X))
			{
				APPLY_FORCE_TO_PED(myPlayerPed, true, 0.0f, 1.2f, 2.5f, 0.0f, 0.0f, 0.0f, true, true, true, true);
			}
		}
	}
}