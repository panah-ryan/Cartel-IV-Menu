#include "stdafx.h"
#include "World_Options.h"
#include "MenuExecutionHandling.h"
#include "Player_Options.h"
#include "Ped_Options.h"
#include "Functions.h"
#include "ObjectMaps.h"
#include "Utils.h"
#include "Vehicle_Options.h"
#include "Admin_Options.h"
#include "Network_Options.h"
#include "Teleport_Options.h"
#include "ObjectVehicles_Options.h"
#include "World/ObjectSpawner/ObjectSpawnerMenu.h"
#include "World/ObjectSpawner/ObjectSpawnerCore.h"
#include "MilitaryMissiles.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types" 

VEHICLE_INDEX myNeonCar;

char* weather_strings[10] = { "Extra Sunny", "Sunny", "Sunny Windy", "Cloudy", "Raining", "Drizzle", "Foggy", "Lightning", "Extra Sunny 2", "Sunny Windy 2" };
ctl_weather_time time_weather_data;
char* delete_strings[12] = { "peds", "objects", "vehicles.all", "vehicles.abandoned", "cars.all", "cars.abandoned", "bikes.all", "bikes.abandoned", "boats.all", "boats.abandoned", "helis.all", "helis.abandoned" };
int delete_selection = 1; //default highlighted is objects 
int delete_distance = 100;
OBJECT_INDEX attached_object;



float airIncAmount = 0.25f;
int attachToWalkerSelect = 0;
char* attachToWalkerStrings[2] = { "Front", "Back" };
char* drivingStyles[3] = { "Normal", "Race", "Reverse" };

void worldOptsSetTime()
{
	SET_TIME_OF_DAY(time_weather_data.m_timeHour, time_weather_data.m_timeMinute);
}

bool SpaceInNetObjectPool(int amount)
{
	if (!NetworkGame)
		return true;

	CPool* NetObjPool = *(CPool**)0x8318E950; //Network object pool
	return ((NetObjPool->m_maxHandle - NetObjPool->m_handleCount) >= amount);
}

void SpawnActionPadInFront(ACTION_PAD_MODE pad_mode, int unused) //spawns in front fo local player only
{
	float mX, mY, mZ, mH, mGround, mPiss;
	float dX, dY, dZ;

	if (!SpaceInNetObjectPool(5)) //checks net session. / will always return true in singleplayer.
	{
		printf("~r~CTL: ~w~Network object pool is full, other players wouldn't see it if we spawned it.");
		return;
	}

	mGround = 0.0f;
	GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, 8.0f, 2.5f, &mX, &mY, &mZ); //+2.5f on Z axis because GetGroundZ begins up high and works downwards until a result is found.
	GET_CHAR_HEADING(myPlayerPed, &mH);
	GET_GROUND_Z_FOR_3D_COORD(mX, mY, mZ, &mGround);

	if (pad_mode == PAD_MODE_PAYNSPRAY)
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 14.3665f, 38.2951f, -7.6722f, &dX, &dY, &dZ);

	if (pad_mode == PAD_MODE_SLINGSHOT)
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, -2.6738f, 11.4741f, 0.9778f, &dX, &dY, &dZ);

	if (pad_mode == PAD_MODE_TELEPORT_TO_SAVED_DESTINATION)
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, 13.6458f, -1.0817f, &dX, &dY, &dZ);

	for (int i = 0; i < MAX_ACTIONPADS; i++)
	{
		char* padModeStrings[7] = { "Health", "Teleport", "ArmorWeps", "SpawnerID", "PayNSpray", "Slingshot", "Spawner"};
		uint padSprites[7] = { 0, 68, 0, 79, 75, 72, 79 };

		if (DOES_OBJECT_EXIST(freeMem->m_action_pad[i].m_padObject)) 
			continue;

		CreateObject(0x4F3E5681, mX, mY, mGround + 0.125f, mH, &freeMem->m_action_pad[i].m_padObject, true);

		//ADD_BLIP_FOR_OBJECT(myActionPads[i].PadObj, &myActionPads[i].padBlip); //will get rid of the blip should the object get deleted by another script/menu or something..
		
		//Do Blip Stuff
		ADD_BLIP_FOR_COORD(mX, mY, mZ, &freeMem->m_action_pad[i].m_blip);
		CHANGE_BLIP_ALPHA(freeMem->m_action_pad[i].m_blip, 255);
		CHANGE_BLIP_SPRITE(freeMem->m_action_pad[i].m_blip, padSprites[pad_mode]);
		CHANGE_BLIP_COLOUR(freeMem->m_action_pad[i].m_blip, 1); //Red (our colour is Red after all).

		//char buffer[16];
		//sprintf(buffer, "Pad %i %s", (i + 1), padModeStrings[pad_mode]);
		sprintf(freeMem->m_action_pad[i].m_padName, "Pad %i %s", (i + 1), padModeStrings[pad_mode]);

		CHANGE_BLIP_NAME_FROM_ASCII(freeMem->m_action_pad[i].m_blip, freeMem->m_action_pad[i].m_padName);

		//End Blip Stuff
		GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(freeMem->m_action_pad[i].m_padObject, -3.1464f, -4.7379f, 0.0f, &freeMem->m_action_pad[i].m_corner1.x, &freeMem->m_action_pad[i].m_corner1.y, &mPiss);
		GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(freeMem->m_action_pad[i].m_padObject, 3.1464f, 4.7379f, 0.0f, &freeMem->m_action_pad[i].m_corner2.x, &freeMem->m_action_pad[i].m_corner2.y, &mPiss);

		freeMem->m_action_pad[i].m_coords.x = mX;
		freeMem->m_action_pad[i].m_coords.y = mY;
		freeMem->m_action_pad[i].m_coords.z = (mGround + 0.16f);
		freeMem->m_action_pad[i].m_currentMode = pad_mode;

		for (int j = 0; j < 3; j++) //3 Attached grids per ActionPad
			CreateObject(0x4F3E5681, mX, mY, mZ, mH, &freeMem->m_action_pad[i].m_grids[j], true);
		
		ATTACH_OBJECT_TO_OBJECT(freeMem->m_action_pad[i].m_grids[0], freeMem->m_action_pad[i].m_padObject, 0, -3.1206f, 0.0f, -2.270f, 0.0f, 1.575f, 0.0f);
		ATTACH_OBJECT_TO_OBJECT(freeMem->m_action_pad[i].m_grids[1], freeMem->m_action_pad[i].m_padObject, 0, 3.1206f, 0.0f, -2.246f, 0.0f, 4.725f, 0.0f); //-2.246 //-0.454 is the adjust value
		ATTACH_OBJECT_TO_OBJECT(freeMem->m_action_pad[i].m_grids[2], freeMem->m_action_pad[i].m_padObject, 0, 0.0f, 4.50f, -3.7767f, 1.575f, 0.0f, 0.0f);

		for (int j = 0; j < 3; j++) //3 Attached grids per ActionPad
			SET_OBJECT_COLLISION(freeMem->m_action_pad[i].m_grids[j], true);

		if (pad_mode == PAD_MODE_TELEPORT_TO_SAVED_DESTINATION)
		{
			if (CreateObject(OBJECT_BM_SATDISH05, dX, dY, mGround, mH, &freeMem->m_action_pad[i].m_displayObject, true))
			{
				SET_OBJECT_ROTATION(freeMem->m_action_pad[i].m_displayObject, 0.0f, 0.0f, (mH - 180.0f)); //x57.1428571 times attachment rotation values //flawless
				printf("~r~CTL PAD %i: ~w~Don't forget to set the destination the pads settings.", (i + 1));
			}
		}

		if (pad_mode == PAD_MODE_SLINGSHOT)
		{
			if (CreateObject(OBJECT_BM_DIVERSION, dX, dY, (mGround + 2.0f), mH, &freeMem->m_action_pad[i].m_displayObject, true))
			{
				SET_OBJECT_ROTATION(freeMem->m_action_pad[i].m_displayObject, 0.0f, 90.0f, mH); //x57.1428571 times attachment rotation values //flawless
				SET_OBJECT_COLLISION(freeMem->m_action_pad[i].m_displayObject, false); //true by default for some reason, players get stuck in an apply force loop but stuck under the sign.
				printf("~r~CTL PAD %i: ~w~Spawned slingshot pad successfully.", (i + 1));
			}
		}

		if (pad_mode == PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED || pad_mode == PAD_MODE_SPAWN_VEHICLE_NORMAL)
			printf("~r~CTL PAD %i: ~w~Spawned successfully, set vehicle model in pads settings.", (i + 1));

		if (pad_mode == PAD_MODE_PAYNSPRAY)
		{
			if (CreateObject(-41876173, dX, dY, (mGround - 6.620f), mH, &freeMem->m_action_pad[i].m_displayObject, true))
			{
				SET_OBJECT_ROTATION(freeMem->m_action_pad[i].m_displayObject, 2.2857f, 2.2857f, (mH + 92.5f)); //x57.1428571 times attachment rotation values //flawless
				printf("~r~CTL PAD %i: ~w~Spawned pay-n-spray successfully.", (i + 1));
			}
		}

		RefreshCurrentPage(NULL, nullptr);
		return; //only wanna spawn one right? xD
	}

	printf("~r~CTL: ~w~Maximum number of pads already spawned.");
}

void SetActionPadTeleportCoordinates(int pad_index)
{
	GET_CHAR_COORDINATES(myPlayerPed, &freeMem->m_action_pad[pad_index].m_teleportDest.x, &freeMem->m_action_pad[pad_index].m_teleportDest.y, &freeMem->m_action_pad[pad_index].m_teleportDest.z);
	freeMem->m_action_pad[pad_index].m_modeData = 1; //teleport dest has been set
	printf("~r~CTL PAD %i: ~w~Teleport destination set.", pad_index + 1);
}

void setPadGenModel(int pad_index, uint model)
{
	PED_INDEX driver;

	freeMem->m_action_pad[pad_index].m_modeData = model;

	if (DOES_VEHICLE_EXIST(freeMem->m_action_pad[pad_index].m_generatedVeh))
	{
		GET_DRIVER_OF_CAR(freeMem->m_action_pad[pad_index].m_generatedVeh, &driver);
		if (!DOES_CHAR_EXIST(driver) && HAS_CONTROL_OF_NETWORK_ID(freeMem->m_action_pad[pad_index].m_generatedVehNetIndex))
		{
			ReleaseControlOfNetworkId(freeMem->m_action_pad[pad_index].m_generatedVehNetIndex);
			DELETE_CAR(&freeMem->m_action_pad[pad_index].m_generatedVeh);
		}
	}
}

void setPadVehicleGenModelSultan(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_SULTAN);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~Sultan.", pad_index + 1);
	}
}

void setPadVehicleGenModelSultanRS(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_SULTANRS);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~Sultan RS.", pad_index + 1);
	}
}

void setPadVehicleGenModelInfernus(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_INFERNUS);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~Infernus.", pad_index + 1);
	}
}

void setPadVehicleGenModelComet(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_COMET);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~Comet.", pad_index + 1);
	}
}

void setPadVehicleGenModelTurismo(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_TURISMO);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~Turismo.", pad_index + 1);
	}
}

void setPadVehicleGenModelFuto(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_FUTO);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~Futo.", pad_index + 1);
	}
}

void setPadVehicleGenModelCavalcade(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_CAVALCADE);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~Cavalcade.", pad_index + 1);
	}
}

void setPadVehicleGenModelNrg900(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_NRG900);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~NRG 900.", pad_index + 1);
	}
}

void setPadVehicleGenModelMaverick(int pad_index, int unused0)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		setPadGenModel(pad_index, VEHICLE_MAVERICK);
		printf("~r~CTL PAD %i: ~w~Vehicle generation model set: ~y~Maverick.", pad_index + 1);
	}
}

void DeleteAndClearActionPadVars(int padIndex, int unused)
{
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[padIndex].m_padObject))
	{
		NETWORK_INDEX NetworkId;
		freeMem->m_action_pad[padIndex].m_corner1.x = 0.0f;
		freeMem->m_action_pad[padIndex].m_corner2.x = 0.0f;
		freeMem->m_action_pad[padIndex].m_corner1.y = 0.0f;
		freeMem->m_action_pad[padIndex].m_corner2.y = 0.0f;
		freeMem->m_action_pad[padIndex].m_currentMode = -1; //disabled
		freeMem->m_action_pad[padIndex].m_modeData = 0;
		freeMem->m_action_pad[padIndex].m_coords.x = 0.0f;
		freeMem->m_action_pad[padIndex].m_coords.y = 0.0f;
		freeMem->m_action_pad[padIndex].m_coords.z = 0.0f;
		freeMem->m_action_pad[padIndex].m_teleportDest.x = 0.0f;
		freeMem->m_action_pad[padIndex].m_teleportDest.y = 0.0f;
		freeMem->m_action_pad[padIndex].m_teleportDest.z = 0.0f;
		memset(freeMem->m_action_pad[padIndex].m_padName, 0, 16);

		for (int i = 0; i < MAX_ACTIONPADS; i++)
		{
			if (DOES_OBJECT_EXIST(freeMem->m_action_pad[padIndex].m_grids[i]))
				DeleteObject(&freeMem->m_action_pad[padIndex].m_grids[i]);

			for (int z = 0; z < 16; z++)
				freeMem->m_action_pad[i].m_playerTick[z] = 0;
		}

		if (DOES_OBJECT_EXIST(freeMem->m_action_pad[padIndex].m_displayObject))
			DeleteObject(&freeMem->m_action_pad[padIndex].m_displayObject);
		

		if (DOES_BLIP_EXIST(freeMem->m_action_pad[padIndex].m_blip))
			REMOVE_BLIP(freeMem->m_action_pad[padIndex].m_blip); //deleting the object will kill the blip but just for good measure.

		DeleteObject(&freeMem->m_action_pad[padIndex].m_padObject); //could ideally scan for objects attached to this object first before deleting it..

		if (DOES_VEHICLE_EXIST(freeMem->m_action_pad[padIndex].m_generatedVeh))
		{
			GET_NETWORK_ID_FROM_VEHICLE(freeMem->m_action_pad[padIndex].m_generatedVeh, &NetworkId); //network control is not locked 
			if (RequestNetworkControl(NetworkId, "pad vehicle"))
			{
				ReleaseControlOfNetworkId(NetworkId);
				DELETE_CAR(&freeMem->m_action_pad[padIndex].m_generatedVeh);
			}
		}

		printf("~r~CTL PAD %i: ~w~Deleted.", padIndex + 1);
	}
	else
		printf("~r~CTL PAD %i: ~w~Doesn't exist.", padIndex + 1);
}

int FreeTimedRampSlot()
{
	for (int z = 0; z < MAX_TIMED_RAMPS; z++)
	{
		if (!DOES_OBJECT_EXIST(freeMem->m_timed_ramps[z].m_ramps[0]))
			return z;
	}

	return -1; //no free slots
}

void SpawnBoatRampInFront(int *amount, int unused1)
{
	vector3 Offsets[3] = { {0.0f, 40.0f, 0.630f}, {0.0f, 48.8654f, 4.9572f}, {0.0f, 56.7420f, 11.2411f} };
	vector3 Rotations[3] = { {7.4285f, 0.0f, 0.0f}, {16.5714f, 0.0f, 0.0f}, {25.7142f, 0.0f, 0.0f} }; //x57.1428571 times attachment rotation values //flawless
	vector3 RealWorld[3], modelMin, modelMax;
	float pH, groundZ;

	const int rampIndex = FreeTimedRampSlot();

	if (rampIndex == -1)
	{
		printf("~r~CTL: ~w~Maximum number of timed ramps spawned!");
		return;
	}

	/*GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS  y + 10 will not be the same distance in front of you with a pitch of 0 as it will with a pitch of 90*/
	//BACKUP! KEEP! vector3 Offsets[3] = { {0.0f, 40.0f, 0.630f}, {0.0f, 48.8654f, 5.5872f}, {0.0f, 56.7420f, 11.8711f} }; //from char in world coords (set heading on object but shouldnt need to
	
	GET_CHAR_HEADING(myPlayerPed, &pH); //used in set object rotation
	GET_MODEL_DIMENSIONS(0x40433D8B, &modelMin, &modelMax); //hopefully model doesn't need to be loaded for this? //nah

	for (int i = 0; i < *amount; i++)
	{
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, Offsets[i].x, Offsets[i].y, Offsets[i].z, &RealWorld[i].x, &RealWorld[i].y, &RealWorld[i].z);

		if (i == 0)
		{
			GET_GROUND_Z_FOR_3D_COORD(RealWorld[i].x, RealWorld[i].y, RealWorld[i].z + 5.0f, &groundZ);
			RealWorld[i].z = (groundZ + modelMax.z) - 1.0f;
		}
		else
			RealWorld[i].z = ((groundZ + modelMax.z) - 1.0f) + Offsets[i].z;
	}

	//to prevent fucked coordinates when moving.. (yes two for loops)
	for (int i = 0; i < *amount; i++)
	{
		if(CreateObject(0x40433D8B, RealWorld[i].x, RealWorld[i].y, RealWorld[i].z, pH, &freeMem->m_timed_ramps[rampIndex].m_ramps[i], (freeMem->m_timed_ramps->m_deletionDelaySeconds > 0)))
			SET_OBJECT_ROTATION(freeMem->m_timed_ramps[rampIndex].m_ramps[i], Rotations[i].x, Rotations[i].y, pH); //pH / 57.1428571 ?? maybe
	}

	freeMem->m_timed_ramps[rampIndex].m_rampTick = freeMem->m_timed_ramps->m_deletionDelaySeconds > 0 ? GetTickCount() + (freeMem->m_timed_ramps->m_deletionDelaySeconds * 1000) : -1;
}

void SpawnHalfPipeInFront(int *amount, int unused1)
{
	//amount = freeMem->m_timed_ramps->m_rampSpawnAmount; 
	vector3 Offsets[3] = { {0.0f, 40.0f, -1.0f}, {0.0f, 46.9453f, 2.6356f}, {0.0f, 53.3806f, 7.5222f} }; //from char in world coords (set heading on object but shouldnt need to
	vector3 Rotations[3] = { {21.1428f, 0.0f, 0.0f}, {32.5714f, 0.0f, 0.0f}, {41.7142f, 0.0f, 0.0f} }; //x57.1428571 times attachment rotation values //flawless
	vector3 RealWorld[3];
	float pH;

	const int rampIndex = FreeTimedRampSlot();

	if (rampIndex == -1)
	{
		printf("~r~CTL: ~w~Maximum number of timed ramps spawned!");
		return;
	}
	
	GET_CHAR_HEADING(myPlayerPed, &pH);

	for (int i = 0; i < *amount; i++)
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, Offsets[i].x, Offsets[i].y, Offsets[i].z, &RealWorld[i].x, &RealWorld[i].y, &RealWorld[i].z);

	//to prevent fucked coordinates when moving.. (yes two for loops)
	for (int i = 0; i < *amount; i++)
	{
		if(CreateObject(1072695029, RealWorld[i].x, RealWorld[i].y, RealWorld[i].z, pH, &freeMem->m_timed_ramps[rampIndex].m_ramps[i], (freeMem->m_timed_ramps->m_deletionDelaySeconds > 0)))
			SET_OBJECT_ROTATION(freeMem->m_timed_ramps[rampIndex].m_ramps[i], Rotations[i].x, Rotations[i].y, pH); //pH / 57.1428571 ?? maybe
	}

	freeMem->m_timed_ramps[rampIndex].m_rampTick = freeMem->m_timed_ramps->m_deletionDelaySeconds > 0 ? GetTickCount() + (freeMem->m_timed_ramps->m_deletionDelaySeconds * 1000) : -1;
}


void SpawnBargeRampInFront(int *amount, int unused1)
{
	vector3 Offsets[3] = { {0.0f, 40.8163f, -0.4056f}, {0.0f, 63.1753f, 7.2222f}, {0.0f, 80.6465f, 17.3955f} }; //from char in world coords (set heading on object but shouldnt need to
	vector3 Rotations[3] = { {12.0f, 0.0f, 0.0f}, {25.7142f, 0.0f, 0.0f}, {34.2857f, 0.0f, 0.0f} }; //x57.1428571 times attachment rotation values //flawless
	vector3 RealWorld[3];
	float pH;

	const int rampIndex = FreeTimedRampSlot();

	if (rampIndex == -1)
	{
		printf("~r~CTL: ~w~Maximum number of timed ramps spawned!");
		return;
	}

	GET_CHAR_HEADING(myPlayerPed, &pH);

	for (int i = 0; i < *amount; i++)
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, Offsets[i].x, Offsets[i].y, Offsets[i].z, &RealWorld[i].x, &RealWorld[i].y, &RealWorld[i].z);

	//to prevent fucked coordinates when moving.. (yes two for loops)
	for (int i = 0; i < *amount; i++)
	{
		if(CreateObject(-2036689985, RealWorld[i].x, RealWorld[i].y, RealWorld[i].z, pH, &freeMem->m_timed_ramps[rampIndex].m_ramps[i], (freeMem->m_timed_ramps->m_deletionDelaySeconds > 0)))
			SET_OBJECT_ROTATION(freeMem->m_timed_ramps[rampIndex].m_ramps[i], Rotations[i].x, Rotations[i].y, pH); //pH / 57.1428571 ?? maybe
	}

	freeMem->m_timed_ramps[rampIndex].m_rampTick = (freeMem->m_timed_ramps->m_deletionDelaySeconds > 0) ? (GetTickCount() + (freeMem->m_timed_ramps->m_deletionDelaySeconds * 1000)) : -1;
}

void SpawnOilTankRampInFront(int *amount, int unused1)
{
	vector3 Offsets[3] = { {0.0f, 45.4172f, -6.5267f}, {0.0f, 65.1795f, -0.8656f}, {0.0f, 85.1699f, 8.5361f} }; //from char in world coords (set heading on object but shouldnt need to
	vector3 Rotations[3] = { {11.4285f, 0.0f, 0.0f}, {20.0f, 0.0f, 0.0f}, {29.7142f, 0.0f, 0.0f} }; //x57.1428571 times attachment rotation values //flawless
	vector3 RealWorld[3];
	float pH;

	const int rampIndex = FreeTimedRampSlot();

	if (rampIndex == -1)
	{
		printf("~r~CTL: ~w~Maximum number of timed ramps spawned!");
		return;
	}

	GET_CHAR_HEADING(myPlayerPed, &pH);

	for (int i = 0; i < *amount; i++)
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, Offsets[i].x, Offsets[i].y, Offsets[i].z, &RealWorld[i].x, &RealWorld[i].y, &RealWorld[i].z);

	//to prevent fucked coordinates when moving.. (yes two for loops)
	for (int i = 0; i < *amount; i++)
	{
		if(CreateObject(0x4F9981BE, RealWorld[i].x, RealWorld[i].y, RealWorld[i].z, pH, &freeMem->m_timed_ramps[rampIndex].m_ramps[i], (freeMem->m_timed_ramps->m_deletionDelaySeconds > 0)))
			SET_OBJECT_ROTATION(freeMem->m_timed_ramps[rampIndex].m_ramps[i], Rotations[i].x, Rotations[i].y, pH); //pH / 57.1428571 ?? maybe
	}

	freeMem->m_timed_ramps[rampIndex].m_rampTick = freeMem->m_timed_ramps->m_deletionDelaySeconds > 0 ? GetTickCount() + (freeMem->m_timed_ramps->m_deletionDelaySeconds * 1000) : -1;
}


bool CreateObjectLocally(uint model, float x, float y, float z, float h, OBJECT_INDEX* object)
{
	if (IS_MODEL_IN_CDIMAGE(model))
	{
		REQUEST_MODEL(model);
		while (!HAS_MODEL_LOADED(model)) 
			WAIT(0);

		CREATE_OBJECT_NO_OFFSET(model, x, y, z, &*object, false);
		MARK_MODEL_AS_NO_LONGER_NEEDED(model);

		while (!DOES_OBJECT_EXIST(*object)) 
			WAIT(0);

		FREEZE_OBJECT_POSITION(*object, true);
		SET_OBJECT_HEADING(*object, h);
		SET_OBJECT_INVINCIBLE(*object, true);
		SET_OBJECT_LIGHTS(*object, true);
		return true;
	}
	
	printf("~r~CTL: ~w~Model is missing from your image files.");
	return false;
}

bool CreateObject(uint model, float x, float y, float z, float h, OBJECT_INDEX* object, bool lockNetControl)
{
	if (IS_MODEL_IN_CDIMAGE(model) && !bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_BLOCKED_OBJECT_SPAWNS))
	{
		REQUEST_MODEL(model);
		while (!HAS_MODEL_LOADED(model)) 
			WAIT(0);

		bool poolSpace = SpaceInNetObjectPool(1);
		CREATE_OBJECT_NO_OFFSET(model, x, y, z, &*object, poolSpace);
		MARK_MODEL_AS_NO_LONGER_NEEDED(model);

		if (!poolSpace && lockNetControl)
		{
			printf("NO POOL SPACE");
			SET_OBJECT_RENDER_SCORCHED(*object, true);
		}

		while (!DOES_OBJECT_EXIST(*object))
			WAIT(0);

		FREEZE_OBJECT_POSITION(*object, true);
		SET_OBJECT_HEADING(*object, h);
		SET_OBJECT_INVINCIBLE(*object, true);

		//SET_OBJECT_PROOFS(*object, true, true, true, true, true); //see if stops glass breaking..

		SET_OBJECT_HEALTH(*object, 165.0f); //stops detaching by my protection if local player has control.

		if (poolSpace && NetworkGame && lockNetControl)
		{
			NETWORK_INDEX netId;

			GET_NETWORK_ID_FROM_OBJECT(*object, &netId);
			SET_NETWORK_ID_CAN_MIGRATE(netId, false);
		}

		return true;
	}

	printf("~r~CTL: ~w~Model is missing from your image files.");
	return false;
}

/* //BACKUP (KEEP)
void CreateObject(int model, float x, float y, float z, float h, Object* object, bool lockNetControl)
{
	if (IS_MODEL_IN_CDIMAGE(model))
	{
		REQUEST_MODEL(model);
		while (!HAS_MODEL_LOADED(model)) WAIT(0);
		bool poolSpace = SpaceInNetObjectPool(1);
		CREATE_OBJECT_NO_OFFSET(model, x, y, z, &*object, poolSpace);
		if (!poolSpace)
		{
			printf("NO POOL SPACE");
			//SET_OBJECT_ALPHA(*object, 150);
			SET_OBJECT_RENDER_SCORCHED(*object, true);
		}
		//SET_OBJECT_ALPHA
		while (!DOES_OBJECT_EXIST(*object)) WAIT(0);
		FREEZE_OBJECT_POSITION(*object, true);
		SET_OBJECT_HEADING(*object, h);
		SET_OBJECT_INVINCIBLE(*object, true);
		SET_OBJECT_HEALTH(*object, 165.0f); //stops detaching by my protection if local player has control.
		MARK_MODEL_AS_NO_LONGER_NEEDED(model);
		if (poolSpace && NetworkGame && lockNetControl)
		{
			int netId;
			GET_NETWORK_ID_FROM_OBJECT(*object, &netId);
			SET_NETWORK_ID_CAN_MIGRATE(netId, false);
		}
	}
	else
	{
		printf("~r~CTL: ~w~Model is missing from your image files.");
	}
}
*/

void CreateMapObject(float x, float y, float z, uint model, float qX, float qY, float qZ, float qW, bool dynamic)
{
	if (CreateObject(model, x, y, z, 0.0f, &freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], true))
	{
		SET_OBJECT_QUATERNION(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], qX, qY, qZ, qW);
		SET_OBJECT_DYNAMIC(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], dynamic);
		freeMem->m_map_mod.obj_count++;
	}
}

void EBCreateFunnyCarEntity(uint model, float offX, float offY, float offZ, float rotX, float rotY, float rotZ, int objNumber)
{
	if(!IS_CHAR_IN_ANY_CAR(myPlayerPed))
		return;

	//INTERIOR_INSTANCE_INDEX myInterior;
	//GET_INTERIOR_FROM_CHAR(myPlayerPed, &myInterior); //doesn't sem to make any difference, regardless of setting room key before or after attaching

	if (IS_THIS_MODEL_A_VEHICLE(model) && model != VEHICLE_CABLECAR) //rip cablecar
	{
		if (SpawnVehicle(model, 0.0f, 0.0f, 0.0f, 0.0f, (VEHICLE_INDEX*)&freeMem->m_object_car_entities[objNumber], false, true))
		{
			SET_CAR_COLLISION(freeMem->m_object_car_entities[objNumber], false);
			//SET_ROOM_FOR_CAR_BY_KEY(freeMem->m_object_car_entities[objNumber], myInterior);
			ATTACH_CAR_TO_CAR(freeMem->m_object_car_entities[objNumber], GetPlayerVeh(), 0, offX, offY, offZ, rotX, rotY, rotZ);
		}
		return;
	}

	if (IS_THIS_MODEL_A_PED(model))
	{
		if (CreatePed(model, 0.0f, 0.0f, 0.0f, 0.0f, (PED_INDEX*)&freeMem->m_object_car_entities[objNumber], true, true))
		{
			SET_CHAR_COLLISION(freeMem->m_object_car_entities[objNumber], false);
			//SET_ROOM_FOR_CHAR_BY_KEY(freeMem->m_object_car_entities[objNumber], myInterior);
			ATTACH_PED_TO_CAR(freeMem->m_object_car_entities[objNumber], GetPlayerVeh(), 0, offX, offY, offZ, rotX, rotY, false, false);
		}
	}
	else
	{
		if (CreateObject(model, 0.0f, 0.0f, 0.0f, 0.0f, (OBJECT_INDEX*)&freeMem->m_object_car_entities[objNumber], true))
		{
			//ADD_OBJECT_TO_INTERIOR_ROOM_BY_KEY(freeMem->m_object_car_entities[objNumber], myInterior);
			SET_OBJECT_HEALTH(freeMem->m_object_car_entities[objNumber], 165.0f);
			SET_OBJECT_COLLISION(freeMem->m_object_car_entities[objNumber], false);
			//ADD_OBJECT_TO_INTERIOR_ROOM_BY_KEY(freeMem->m_object_car_entities[objNumber], myInterior);
			ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[objNumber], GetPlayerVeh(), 0, offX, offY, offZ, rotX, rotY, rotZ);
		}
	}
}

void EBCreateMapObject(uint model, float x, float y, float z, float h, float qx, float qy, float qz, float qw, bool dynamic, int quaternion, bool no_offset)
{
	//should use bitTest instead
	if (IS_MODEL_IN_CDIMAGE(model) && !bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_BLOCKED_OBJECT_SPAWNS)) //lol fuck with them make it print model missing.
	{
		REQUEST_MODEL(model);
		while (!HAS_MODEL_LOADED(model)) 
			WAIT(0);

		bool poolSpace = SpaceInNetObjectPool(1);

		if (no_offset)
			CREATE_OBJECT_NO_OFFSET(model, x, y, z, &freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], poolSpace);
		else
			CREATE_OBJECT(model, x, y, z, &freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], poolSpace);
			
		if (!poolSpace)
		{
			printf("NO POOL SPACE for myMapObject number %i.", freeMem->m_map_mod.obj_count);
			SET_OBJECT_ALPHA(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], 150);
		}

		SET_OBJECT_HEADING(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], h);

		if (dynamic)
			SET_OBJECT_DYNAMIC(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], true);
		else
			FREEZE_OBJECT_POSITION(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], true);

		SET_OBJECT_INVINCIBLE(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], true);
		SET_OBJECT_LIGHTS(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], true);

		if (quaternion == 1)
			SET_OBJECT_QUATERNION(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], qx, qy, qz, qw);
		else if(quaternion == 2)
			SET_OBJECT_ROTATION(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], qx, qy, qz); 

		if (poolSpace && NetworkGame)
		{
			NETWORK_INDEX netId;

			GET_NETWORK_ID_FROM_OBJECT(freeMem->m_map_mod.object[freeMem->m_map_mod.obj_count], &netId);
			SET_NETWORK_ID_CAN_MIGRATE(netId, false);
		}

		freeMem->m_map_mod.obj_count++;
	}
	else
		printf("~r~CTL: ~w~Model %i missing from your image files.", model);
}

void ClearCurrentlySpawnedMap()
{
	NETWORK_INDEX netId;
	for (int i = 0; i < MAX_MAP_OBJECTS; i++)
	{
		if (DOES_OBJECT_EXIST(freeMem->m_map_mod.object[i]))
			DeleteObject(&freeMem->m_map_mod.object[i]);
	}

	if (DOES_VEHICLE_EXIST(freeMem->m_map_mod.vehicle))
		DeleteCar(&freeMem->m_map_mod.vehicle);

	if (DOES_BLIP_EXIST(freeMem->m_map_mod.blip))
		REMOVE_BLIP(freeMem->m_map_mod.blip);

	PROCESS_MISSION_DELETION_LIST();

	if (freeMem->m_map_mod.obj_count > 0 && NetworkGame)
		WAIT(1100); //seems to fix the issue. fuck it they will have to wait..


	freeMem->m_map_mod.obj_count = 0;
}

bool ShouldDeletionsSkipObject(const OBJECT_INDEX object) //, char* *type)
{
	if (attached_object == object) 
		return true;

	for (int i = 0; i < MAX_MAP_OBJECTS; i++)
	{
		if (freeMem->m_map_mod.object[i] == object) 
			return true;
	}

	for (int i = 0; i < MAX_CAR_OBJECTS; i++)
	{
		if (freeMem->m_object_car_entities[i] == object) 
			return true;
	}

	for (int i = 0; i < 3; i++)
	{
		if (myNeonCarObjects[i] == object) 
			return true;
	}

	for (int z = 0; z < MAX_TIMED_RAMPS; z++) //max timed ramps = 4
	{
		for (int i = 0; i < 3; i++) //max ramp objects = 3
		{
			if (DOES_OBJECT_EXIST(freeMem->m_timed_ramps[z].m_ramps[i]))
			{
				if (freeMem->m_timed_ramps[z].m_ramps[i] == object)
					return !CanObjectMigrate(freeMem->m_timed_ramps[z].m_ramps[i]);
			}

		}
	}

	for (int z = 0; z < 3; z++) //max actionpads = 3
	{
		if (freeMem->m_action_pad[z].m_padObject == object || freeMem->m_action_pad[z].m_displayObject == object) 
			return true;

		for (int i = 0; i < 3; i++)
		{
			if (freeMem->m_action_pad[z].m_grids[i] == object) 
				return true;
		}
	}

	for (int i = 0; i < 16; i++)
	{
		if (freeMem->m_walk_water_object[i] == object || freeMem->m_walk_on_air.m_object[i] == object) 
			return true;
	}

	return false;
}

bool ShouldDeletionsSkipVehicle(const VEHICLE_INDEX vehicle)
{
	if (freeMem->m_map_mod.vehicle == vehicle || ctlMem->m_chauffeur.vehicle == vehicle)
		return true;

	for (int i = 0; i < MAX_ACTIONPADS; i++)
	{
		if (freeMem->m_action_pad[i].m_generatedVeh == vehicle)
			return true;
	}

	return false;
}

bool ShouldDeleionsSkipPed(const PED_INDEX targetPed)
{
	if (ctlMem->m_chauffeur.driver == targetPed) 
		return true;

	for (int i = 0;i < MAX_INTERIOR_PEDS; i++)
	{
		if (ctlMem->m_interior_peds.ped[i] == targetPed) 
			return true;
	}

	return false;
}

void LocalUniversalDelete(int desired_distance, int current_type, bool print) //should only really be called from the UniversalDelete function itself - Nathan
{
	float distance, x, y, z, mx, my, mz, set_distance = TO_FLOAT(desired_distance);
	bool distanceRiggedToInfinite; 
	char* typeString = "RIP";

	if (desired_distance == 0)
	{
		set_distance = 3000.0f;
		desired_distance = 3000;
		distanceRiggedToInfinite = true;
	}

	if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId())) //should both return true in singleplayer.
	{
		PED_INDEX netPedTmp;

		GET_PLAYER_CHAR(GetMenuPlayerId(), &netPedTmp);
		GET_CHAR_COORDINATES(netPedTmp, &mx, &my, &mz);
	}
	else //for network
	{
		printf("~r~CTL: ~w~Player has no character, can't grab co-ordinates to delete from.");
		return;
	}

	switch (current_type)
	{
	case 0: //peds
		CLEAR_AREA_OF_CHARS(mx, my, mz, set_distance); typeString = "peds"; 
		break; //this risks deleting our shit like airWalkers etc hmmmph.. //unlesswe set them as mission objects! :D
	case 1: //objects
		CLEAR_AREA_OF_OBJECTS(mx, my, mz, set_distance); typeString = "objects"; 
		if (!NetworkGame) //not an issue in multiplayer as get caught by network scans.
		{
			for (int i = 0; i < MAX_TIMED_RAMPS; i++)
			{
				if (freeMem->m_timed_ramps[i].m_rampTick != -1) //if scheduled for deletion, continue (tick count is present)
					continue;

				for (int z = 0; z < 3; z++) //up to 3 objects per ramp.
				{
					float tx, ty, tz, dist_from_ramp;
					if (DOES_OBJECT_EXIST(freeMem->m_timed_ramps[i].m_ramps[z])) 
					{
						GET_OBJECT_COORDINATES(freeMem->m_timed_ramps[i].m_ramps[z], &tx, &ty, &tz);
						GET_DISTANCE_BETWEEN_COORDS_3D(mx, my, mz, tx, ty, tz, &dist_from_ramp);
						if (dist_from_ramp < set_distance) //within range nuke this ramp.
						{
							for (int j = 0; j < 3; j++)
								if(DOES_OBJECT_EXIST(freeMem->m_timed_ramps[i].m_ramps[j]))
									DELETE_OBJECT(&freeMem->m_timed_ramps[i].m_ramps[j]);

							break; //break out and continue checking other ramp sets.
						}
					}
				}
			}
		}
		break;
	default: //vehicles
		CLEAR_AREA_OF_CARS(mx, my, mz, set_distance); typeString = "vehicles"; 
		break;
	}

	//FLUSH_ALL_SPAWN_BLOCKING_AREAS(); //might be needed... 

	if (print)
	{
		if(distanceRiggedToInfinite)
			printf("~r~CTL: ~w~Cleared area of ~y~%s, ~w~distance: ~y~infinite.", typeString);
		else
		{
			if (GetMenuPlayerId() == myPlayerIndex)
				printf("~r~CTL: ~w~Cleared area of ~y~%s ~w~within ~y~%i ~w~distance.", typeString, desired_distance);
			else
				printf("~r~CTL: ~w~Cleared area of ~y~%s ~w~near %s ~w~within ~y~%i ~w~distance.", typeString, NetworkPlayerGetNameWithColour(GetMenuPlayerId()), desired_distance);
		}
	}
}

void UniversalDelete(const int desired_distance, const int current_type, const bool print, const bool request_control, const int attempts) //should make bool request control... so as we can clean up only vehicles we have control of before processing fcar protect.. think that was my lil secret to make it work better
{
	float distance, x, y, z, mx, my, mz, set_distance = TO_FLOAT(desired_distance);
	VEHICLE_INDEX myVeh;// = GetPlayerVeh(); //we do this here instead of inside the ShouldDeletionsSkipVehicle function to stop it being called over and over.
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
	VEHICLE_INDEX selectedPlayerVeh = INVALID_HANDLE;
	PED_INDEX tmp, driver;
	int deletedCount = 0;
	int skippedCount = 0;
	int deletedAttachmentsCount = 0;
	bool grabbedSkipped = false;
	uint model;
	uint typeForPrint = current_type;

	if (NetworkGame)
	{
		if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
		{
			PED_INDEX netPedTmp;

			GET_PLAYER_CHAR(GetMenuPlayerId(), &netPedTmp);
			GET_CHAR_COORDINATES(netPedTmp, &mx, &my, &mz);

			if (IS_CHAR_IN_ANY_CAR(netPedTmp))
				GET_CAR_CHAR_IS_USING(netPedTmp, &selectedPlayerVeh);
		}
		else
		{
			if (desired_distance == 0)
				mx = 0.0f, my = 0.0f, mz = 0.0f;
			else
			{
				printf("~r~CTL: ~w~Player has no character, can't grab co-ordinates to delete from.");
				return;
			}
		}

		for (int j = 0; j < attempts; j++)
		{
			for (int i = 0; i < 7000; i++) //Was 8250 but i'm yet to see a ID above 5000 so
			{
				switch (current_type)
				{
					case 0:
					{
						if (!DOES_PED_EXIST_WITH_NETWORK_ID(i) || i % 100 == 1) //i %100 == 1 returns if it's a players ped.
							continue;

						GET_PED_FROM_NETWORK_ID(i, &tmp);

						if (ShouldDeleionsSkipPed(tmp))
						{
							if (!grabbedSkipped)
								skippedCount++;

							continue;
						}

						GET_CHAR_COORDINATES(tmp, &x, &y, &z);
					}
					break;
					case 1:
					{
						if (!DOES_OBJECT_EXIST_WITH_NETWORK_ID(i))
							continue;

						GET_OBJECT_FROM_NETWORK_ID(i, &tmp);

						if (ShouldDeletionsSkipObject(tmp))
						{
							if (!grabbedSkipped)
								skippedCount++;

							continue; //checks for Actionpad & timed ramp objects..
						}

						GET_OBJECT_COORDINATES(tmp, &x, &y, &z);
						GET_OBJECT_MODEL(tmp, &model);
					}
					break;
					default:
					{
						if (!DOES_VEHICLE_EXIST_WITH_NETWORK_ID(i))
							continue;

						GET_VEHICLE_FROM_NETWORK_ID(i, &tmp);

						//maybe also skip if the driver exists and is a player.
						if (ShouldDeletionsSkipVehicle(tmp) || tmp == myVeh || tmp == selectedPlayerVeh) //skips actionpad vehicles
						{
							if (!grabbedSkipped)
								skippedCount++;

							continue; 
						}

						GET_CAR_MODEL(tmp, &model);

						switch (current_type)
						{
						case 4:
						case 5:
							//if (!CAN_BE_DESCRIBED_AS_A_CAR(tmp)) //not tested
							//	continue;
							if (!IS_THIS_MODEL_A_CAR(model))
								continue;
							break;
						case 6:
						case 7:
							if (!IS_THIS_MODEL_A_BIKE(model))
								continue;
							break;
						case 8:
						case 9:
							if (!IS_THIS_MODEL_A_BOAT(model))
								continue;
							break;
						case 10:
						case 11:
							if (!IS_THIS_MODEL_A_HELI(model))
								continue;
							break;
						}

						GET_CAR_COORDINATES(tmp, &x, &y, &z);

						if (current_type % 2 == 1) //if number is odd (abandoned only)
						{
							int psngrs;

							GET_DRIVER_OF_CAR(tmp, &driver);
							GET_NUMBER_OF_PASSENGERS(tmp, &psngrs); //do player detection on this shit //don't want to exclude vehicles because there's a ped in them.. or hm,, na, people fuck shit up.

							if (psngrs > 0 || DOES_CHAR_EXIST(driver))
								continue;
						}
					}
					break;
				}

				GET_DISTANCE_BETWEEN_COORDS_2D(mx, my, x, y, &distance);
				if (desired_distance != 0 && distance > set_distance)
					continue;

				if (request_control)
					REQUEST_CONTROL_OF_NETWORK_ID(i);

				if (!HAS_CONTROL_OF_NETWORK_ID(i))
				{
					//if attempt = max_attempts - 1
					//add to mission deletion list?
#ifdef Developer
					for (int j = 0; j < 16; j++)
					{
						if (j == myPlayerIndex || !IsPlayerValid(j))
							continue;

						CTL_FORCE_REMOVE_NETWORK_ENTITY(i, j);
					}
					CTL_FORCE_REMOVE_NETWORK_ENTITY(i, myPlayerIndex);
#endif
					continue;
				}

				switch (current_type)
				{
					case 0:
						//ADD_PED_TO_MISSION_DELETION_LIST(tmp, 29);
						SET_NETWORK_ID_CAN_MIGRATE(i, false); //just incase deleting attachments takes a while.
						//deletedAttachmentsCount = deletedAttachmentsCount + DeleteObjectsOnPed(tmp);
						deletedAttachmentsCount += DeleteObjectsOnPed(tmp);
						//DeleteChar(&tmp);
						SET_NETWORK_ID_CAN_MIGRATE(i, true); //just incase deleting takesa while.
						DELETE_CHAR(&tmp); //prevents extra DeleteObjectsOnPed call.
						deletedCount++;
						break;
					case 1:
						SET_NETWORK_ID_CAN_MIGRATE(i, true);
						DELETE_OBJECT(&tmp);
						//DeleteObject(&tmp);
						deletedCount++;
						break;
					default:
						SET_NETWORK_ID_CAN_MIGRATE(i, false); //just incase deleting attachments takes a while.
						//deletedAttachmentsCount = deletedAttachmentsCount + DeleteObjectsOnVehicle(tmp); //should probably count attachments seperately..
						deletedAttachmentsCount += DeleteObjectsOnVehicle(tmp); //should probably count attachments seperately..
						//DeleteCar(tmp);
						SET_NETWORK_ID_CAN_MIGRATE(i, true);
						DELETE_CAR(&tmp);
						deletedCount++;
						break;
				}
			}

			WAIT(75);
			grabbedSkipped = true;
		}
	}
	else
	{
		LocalUniversalDelete(desired_distance, current_type, print);
		return; 
	}

	if (print)
	{
		char* buffer = BUFFER128;
		strcpy(buffer, "~r~CTL: ~w~Deleted ~y~", 128); //22
		straddi(buffer, deletedCount, 128); //max 3 i guess (unlikely) but 3 so, //25
		stradd(buffer, " ~w~", 128); //29
		stradd(buffer, delete_strings[typeForPrint], 128); //vehicles.abandoned is the longest //18 //=//47

		if (deletedAttachmentsCount > 0)
		{
			stradd(buffer, " (+", 128); //51
			straddi(buffer, deletedAttachmentsCount, 128); //55 to be safe
			stradd(buffer, " attachments)", 128); //13+55 = 68
		}
		stradd(buffer, " within ~y~", 128); //11+68 = //79
		if (desired_distance == 0)
			stradd(buffer, "infinite", 128); //8+79 = //87
		else
			straddi(buffer, desired_distance, 128);

		stradd(buffer, " ~w~distance", 128); //12+87 = //99 //STRING SHOULD SPLIT HERE.
		//NEED TO SPLIT STRINGS/BUFFERS HERE!
		if (GetMenuPlayerId() != myPlayerIndex && desired_distance != 0) //don't bother printing selected player name if distance is infinite.
		{
			stradd(buffer, " of ", 128);
			stradd(buffer, GET_PLAYER_NAME(GetMenuPlayerId()), 128); //with colour it too long probbaly.
		}
		if (skippedCount > 0)
		{
			//STRTNM3 will conveniently put a ", " inbetween the strings for us.
			char* buffer2 = BUFFER64;
			strcpy(buffer2, "skipped ~y~", 64);
			straddi(buffer2, skippedCount, 64);
			stradd(buffer2, ".", 64);
			PRINT_STRING_WITH_TWO_LITERAL_STRINGS_NOW("STRTNM3", buffer, buffer2, 5000, true);
		}
		else
		{
			stradd(buffer, ".", 255);
			printf(buffer);
		}
		//ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	}
	if(typeForPrint != 1) 
		PROCESS_MISSION_DELETION_LIST(); //objects as they don't get added seemingly.
}

void callUniversalDelete()
{
	UniversalDelete(delete_distance, delete_selection, true, true, 9); //8 loops, waits alone = 675ms, the other 325ms is for the Deletiin for loop , adjust as appropriate.
}

void callLocalUniversalDelete()
{
	LocalUniversalDelete(delete_distance, delete_selection, true);
}

void DeleteAllSpawnedEntities() //TODO Nath add bool canWait.
{
	NETWORK_INDEX NetworkId;
	bool isHost = (myPlayerIndex == GET_HOST_ID());

	if (DOES_CHAR_EXIST(ctlMem->m_chauffeur.driver))
		DeleteChar(&ctlMem->m_chauffeur.driver);

	if (DOES_VEHICLE_EXIST(ctlMem->m_chauffeur.vehicle))
		DeleteCar(&ctlMem->m_chauffeur.vehicle);

	float pX, pY, pZ, pH;
	PED_INDEX networkPlayerPed;
	for (int i = 0; i < 16; i++)
	{
		if (DOES_OBJECT_EXIST(freeMem->m_walk_on_air.m_object[i]) || DOES_OBJECT_EXIST(freeMem->m_walk_water_object[i]))
		{
			if (COMPARE_STRING(GET_NAME_OF_ZONE(pX, pY, pZ), "LIBERTY")) //out at sea
			{
				float destX, destY, destZ;
				if (GET_CLOSEST_CAR_NODE(pX, pY, pZ, &destX, &destY, &destZ))
				{
					GET_CHAR_HEADING(networkPlayerPed, &pH);
					ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
					printf("~r~CTL: ~w~Warping %s ~w~back to shore.", NetworkPlayerGetNameWithColour(i));
					if (IS_CHAR_ON_FOOT(networkPlayerPed))
					{
						//this does get calles by TeleportNetworkPlayerToCoordinate however this way we don't wait for teleport on foot success.. who cares, if they have mods they can fix themselves.
						TeleportNetworkPlayerOnFoot(i, destX, destY, destZ, pH, false);
					}
					else
					{ //already auto-disables if they get into a boat or heli.
						TeleportNetworkPlayerToCoordinate(i, destX, destY, destZ, pH);
					}
				}
			}
		}
		if (DOES_OBJECT_EXIST(freeMem->m_walk_on_air.m_object[i]))
		{
			DeleteObject(&freeMem->m_walk_on_air.m_object[i]);
			CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i);
			CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, i + 16);
			CLEAR_BIT(&ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, i + 16);
		}

		if (DOES_OBJECT_EXIST(freeMem->m_walk_water_object[i])) //should probably make it more obvious to the user that this happens //or detect people at sea and killzone them?
		{
			DeleteObject(&freeMem->m_walk_water_object[i]);
			CLEAR_BIT(&ctlMem->m_bit_sets.m_removeWeapons_driveWater, i + 16);
		}
	}

	if (DOES_VEHICLE_EXIST(bladesHeli))
	{
		PED_INDEX bladesDriver;
		GET_DRIVER_OF_CAR(bladesHeli, &bladesDriver); //reusing var.
		if (DOES_CHAR_EXIST(bladesDriver))
			DeleteChar(&bladesDriver); 
		
		DeleteCar(&bladesHeli);
	}

	for (int i = 0; i < 3; i++)
	{
		if (DOES_OBJECT_EXIST(freeMem->m_action_pad[i].m_padObject))
			DeleteAndClearActionPadVars(i, 0); //takes care of attached grids, display obj, generated vehicle & eryting.
	}

	for (int i = 0; i < MAX_TIMED_RAMPS; i++) //4 maximum ramps
	{
		for (int z = 0; z < 3; z++) //up to 3 objects per ramp.
		{
			if (DOES_OBJECT_EXIST(freeMem->m_timed_ramps[i].m_ramps[z]))
			{
				bool GotEem = false;

				if (freeMem->m_timed_ramps[i].m_rampTick == -1) //no timer / net control is not locked (we must request)
				{
					GET_NETWORK_ID_FROM_OBJECT(freeMem->m_timed_ramps[i].m_ramps[z], &NetworkId);

					if (DOES_OBJECT_EXIST_WITH_NETWORK_ID(NetworkId))
						GotEem = RequestNetworkControl(NetworkId, "ramp");
					else
						GotEem = true;
				}
				else
					GotEem = true;

				if (GotEem)
					DeleteObject(&freeMem->m_timed_ramps[i].m_ramps[z]);
				
			}
		}
	}

	if (DOES_OBJECT_EXIST(attached_object))
	{
		DeleteObject(&attached_object);
		if (!IS_IN_SPECTATOR_MODE())
			SET_CHAR_VISIBLE(myPlayerPed, true);
	}
	
	for (int i = 0; i < MAX_CAR_OBJECTS; i++)
	{
		if (DOES_OBJECT_EXIST(freeMem->m_object_car_entities[i]))
		{
			GET_NETWORK_ID_FROM_OBJECT(freeMem->m_object_car_entities[i], &NetworkId);
			if (!RequestNetworkControl(NetworkId, "vehicle attachment (vehicle)"))  //checks if network id exists.
				continue;

			SET_NETWORK_ID_CAN_MIGRATE(NetworkId, true);
			DELETE_OBJECT((OBJECT_INDEX*)&freeMem->m_object_car_entities[i]);
		}
		if (DOES_VEHICLE_EXIST(freeMem->m_object_car_entities[i]))
		{
			GET_NETWORK_ID_FROM_VEHICLE(freeMem->m_object_car_entities[i], &NetworkId);
			if (!RequestNetworkControl(NetworkId, "vehicle attachment (object)"))
				continue;

			SET_NETWORK_ID_CAN_MIGRATE(NetworkId, true);
			DELETE_CAR((VEHICLE_INDEX*)&freeMem->m_object_car_entities[i]);
		}
		if (DOES_CHAR_EXIST(freeMem->m_object_car_entities[i]))
		{
			GET_NETWORK_ID_FROM_PED(freeMem->m_object_car_entities[i], &NetworkId);
			if (!RequestNetworkControl(NetworkId, "vehicle attachment (ped)"))
				continue;

			SET_NETWORK_ID_CAN_MIGRATE(NetworkId, true);
			DELETE_CHAR((PED_INDEX*)&freeMem->m_object_car_entities[i]);
		}
	}
	
	if (DOES_VEHICLE_EXIST(myObjectCar))
	{
		GET_NETWORK_ID_FROM_VEHICLE(myObjectCar, &NetworkId);

		if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(NetworkId))
		{
			if (RequestNetworkControl(NetworkId, "Object vehicle"))
			{
				SET_NETWORK_ID_CAN_MIGRATE(NetworkId, true);
				SET_CAR_VISIBLE(myObjectCar, true);
				MARK_CAR_AS_NO_LONGER_NEEDED(&myObjectCar);
			}
		}
		else
			MARK_CAR_AS_NO_LONGER_NEEDED(&myObjectCar);
	}

	for (int i = 0; i < 3; i++)
	{
		if (DOES_OBJECT_EXIST(myNeonCarObjects[i]))
		{
			GET_NETWORK_ID_FROM_OBJECT(myNeonCarObjects[i], &NetworkId); //we can't afford to wait for this here
			if (!RequestNetworkControl(NetworkId, "vehicle neon"))
				continue;

			SET_NETWORK_ID_CAN_MIGRATE(NetworkId, true);
			DELETE_OBJECT(&myNeonCarObjects[i]);
		}
	}
	ClearCurrentlySpawnedMap(); //Waits 1sec is stuff spawned, deletes myMapObjects[70] and myMapVehicle 
	DeleteLastSpawnedObjects(max_spawner_objs);
}

void WorldOptRigTrafficGens()
{
	if (!bitTest(ctlMem->m_bit_sets.m_vehicle, disableTrafficGenerators)) //looped otherwise
	{
		SET_CAR_DENSITY_MULTIPLIER(1.0f);
		SET_RANDOM_CAR_DENSITY_MULTIPLIER(1.0f);
	}
}

PLAYER_INDEX IsThisPedAPlayer(PED_INDEX ped)
{
	PED_INDEX playerPed;

	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i))// || !PLAYER_HAS_CHAR(i)) 
			continue; //causing freezing without this

		GET_PLAYER_CHAR(i, &playerPed);
		if (playerPed == ped) 
			return i;
	}

	return INVALID_PLAYER_INDEX;
}

void SetFreezeWeather()
{
	FORCE_WEATHER_NOW(time_weather_data.m_weather);
}

void disableFreezeTime(int un1, int un2){ bitReset(&ctlMem->m_bit_sets.m_player, freezeTime); }
void disableSyncTimeRealWorld(int un1, int un2){ bitReset(&ctlMem->m_bit_sets.m_player, syncTimeToRealWorld); }

void enableSyncTimeRealWorld(int un1, int un2)
{
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	if(CartelXEXPresent)
	{
		disableFreezeTime(0, 0);
		int hour, minute;
		CTL_GET_LOCAL_TIME(&hour, &minute);
		printf("~r~CTL: ~w~Local time grabbed ~y~%i:%i.", hour, minute);
	}
	else
	{
		bitReset(&ctlMem->m_bit_sets.m_player, syncTimeToRealWorld);
		printf("~r~CTL: ~w~This feature requires Cartel.xex.");
	}
}


void WorldWeatherTimeOptions()
{
	GET_TIME_OF_DAY(&time_weather_data.m_timeHour, &time_weather_data.m_timeMinute);
	GET_CURRENT_WEATHER(&time_weather_data.m_weather);

	SetHeader("World -> Weather/Time");
	AddItemChar("Set weather", true, weather_strings, 10, (int*)&time_weather_data.m_weather, SetFreezeWeather, nullptr);
	AddItemInt("Set hour", true, 0, 23, &time_weather_data.m_timeHour, 1, worldOptsSetTime, nullptr);
	AddItemInt("Set minute", true, 0, 59, &time_weather_data.m_timeMinute, 1, worldOptsSetTime, nullptr);
	AddItemBool("Freeze time", &ctlMem->m_bit_sets.m_player, freezeTime, disableSyncTimeRealWorld, nullptr);
	AddItemBool("Freeze weather", &ctlMem->m_bit_sets.m_player, freezeWeather, nullptr, nullptr);
	AddItemBool("Sync game time to real-world time", &ctlMem->m_bit_sets.m_player, syncTimeToRealWorld, enableSyncTimeRealWorld, "Syncs to Xbox clock.");
}

void setPadVehModel(int pad_index, int un2)
{
	SetHeader(freeMem->m_action_pad[pad_index].m_padName);
	AddItemBool("Invincible spawns", &ctlMem->m_bit_sets.m_vehicle, spawnVehiclesInvincible, nullptr, "Makes every vehicle spawned immune from damage.");
	AddItemWithParam("Sultan", pad_index, setPadVehicleGenModelSultan, "Set generation model to Sultan.");
	AddItemWithParam("Sultan RS", pad_index, setPadVehicleGenModelSultanRS, "Set generation model to Sultan RS.");
	AddItemWithParam("Infernus", pad_index, setPadVehicleGenModelInfernus, "Set generation model to Infernus.");
	AddItemWithParam("Comet", pad_index, setPadVehicleGenModelComet, "Set generateion model to Comet.");
	AddItemWithParam("Turismo", pad_index, setPadVehicleGenModelTurismo, "Set generateion model to Turismo.");
	AddItemWithParam("Futo", pad_index, setPadVehicleGenModelFuto, "Set generateion model to Futo.");
	AddItemWithParam("Calvacade", pad_index, setPadVehicleGenModelCavalcade, "Set generateion model to Cavalcade.");
	AddItemWithParam("NRG 900", pad_index, setPadVehicleGenModelNrg900, "Set generateion model to NRG 900.");
	AddItemWithParam("Maverick", pad_index, setPadVehicleGenModelMaverick, "Set generateion model to Maverick.");
}

void TeleportToActionPad(int pad_index)
{
	//BlockYVehicleExitJustPressed(); //once we rig to BUTTON_Y
	if (DOES_OBJECT_EXIST(freeMem->m_action_pad[pad_index].m_padObject))
	{
		vector4 target;
		GET_OBJECT_HEADING(freeMem->m_action_pad[pad_index].m_padObject, &target.h);
		GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(freeMem->m_action_pad[pad_index].m_padObject, 0.0f, -8.0f, 1.0f, &target.x, &target.y, &target.z);
		if (!TeleportLocalPlayerInVehicle(target.x, target.y, target.z, target.h))
			TeleportLocalPlayerOnFoot(target.x, target.y, target.z, target.h);
	}
}

void PadSettings(int pad_index,int unused1)
{
	SetHeader(freeMem->m_action_pad[pad_index].m_padName);
	if (freeMem->m_action_pad[pad_index].m_currentMode == PAD_MODE_TELEPORT_TO_SAVED_DESTINATION)
		AddItemWithParam("Set teleport destination", pad_index, SetActionPadTeleportCoordinates, "Set destination to current location.");
	else if (freeMem->m_action_pad[pad_index].m_currentMode == PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED || freeMem->m_action_pad[pad_index].m_currentMode == PAD_MODE_SPAWN_VEHICLE_NORMAL)//it'll be == PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED
		AddItemMenuWithParam("Set vehicle model", pad_index, setPadVehModel, "Set the vehicle this pad will spawn.");

	//slingshot power level/ apply force amount..
	//PAD_MODE_PAYNSPRAY (nothing needed for this i don't think...... 

	AddItemWithParam("Teleport to pad", pad_index, TeleportToActionPad, nullptr);
	AddItemWithParam("Delete pad", pad_index, DeleteAndClearActionPadVars, "Remove this pad from the world.");
}

char* padDescription(int padIndex)
{
	if (freeMem->m_action_pad[padIndex].m_currentMode == PAD_MODE_PAYNSPRAY)
		return "Delete this Pay-N-Spray pad.";

	if (freeMem->m_action_pad[padIndex].m_currentMode == PAD_MODE_SLINGSHOT)
		return "Delete this Slingshot pad.";

	if (freeMem->m_action_pad[padIndex].m_currentMode == PAD_MODE_TELEPORT_TO_SAVED_DESTINATION)
		return "Set teleport destination or delete.";

	if (freeMem->m_action_pad[padIndex].m_currentMode == PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED)
		return "Set the vehicle this pad generates, or delete.";

	return "None";
}

void ActionPadOptions()
{
	SetHeader("World -> Action pads");
	AddItemWithParam("Spawn teleporter", PAD_MODE_TELEPORT_TO_SAVED_DESTINATION, SpawnActionPadInFront, "Teleports players.");
	AddItemWithParam("Spawn vehicle generator", PAD_MODE_SPAWN_VEHICLE_NORMAL, SpawnActionPadInFront, "Generates/Spawns vehicles.");
	AddItemWithParam("Spawn vehicle gen (ID Protected)", PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED, SpawnActionPadInFront, "Locks control to driver until ditched.");
	AddItemWithParam("Spawn pay-n-spray", PAD_MODE_PAYNSPRAY, SpawnActionPadInFront, "Respray players vehicles.");
	AddItemWithParam("Spawn slingshot pad", PAD_MODE_SLINGSHOT, SpawnActionPadInFront, "Slingshot players vehicles.");
	AddItemBool("Pad notifications", &ctlMem->m_bit_sets.m_player, aPadNotifications, nullptr, "Get notified when a pad acts on a player.");
	
	for (int i = 0; i < MAX_ACTIONPADS; i++)
	{
		if (DOES_OBJECT_EXIST(freeMem->m_action_pad[i].m_padObject))
			AddItemMenuWithParamAlt(freeMem->m_action_pad[i].m_padName, i, PadSettings, "~PAD_X~ = Delete.", DeleteAndClearActionPadVars);
	}
}

void WorldWaterOptions()
{
	SetHeader("Water Options");
	AddItemFloat("Wave Height", false, -1000.0, 1000.0, (float*)0x82A98C8C, 1.0, 0, nullptr, nullptr);
	AddItemFloat("Water Ripples", false, -1000.0, 1000.0, (float*)0x82009E7C, 1.0, 0, nullptr, nullptr);
	AddItemBool("Drive/walk on water", &ctlMem->m_bit_sets.m_removeWeapons_driveWater, (NetworkGame ? myPlayerIndex + 16 : 16), WalkWaterToggleScanSelectedPlayerId, "This version is improved & also fixes tunnel blocking.");
	AddItemBool("Drive/walk - object visibility", &ctlMem->m_bit_sets.m_player, driveWaterVisible, nullptr, "Useful for making players realise what's up.");
}

void RampOptions()
{
	SetHeader("World -> Ramps");
	AddItemInt("Ramp timer", false, 0, 60, &freeMem->m_timed_ramps->m_deletionDelaySeconds, 5, nullptr, "Seconds until deletion, 0 to disable.");
	AddItemInt("Spawn boat ramp", false, 1, 3, &freeMem->m_timed_ramps->m_rampSpawnAmount, 1, SpawnBoatRampInFront, nullptr);
	AddItemInt("Spawn halfpipe", false, 1, 3, &freeMem->m_timed_ramps->m_rampSpawnAmount, 1, SpawnHalfPipeInFront, nullptr);
	AddItemInt("Spawn barge", false, 1, 3, &freeMem->m_timed_ramps->m_rampSpawnAmount, 1, SpawnBargeRampInFront, nullptr);
	AddItemInt("Spawn oil tank", false, 1, 3, &freeMem->m_timed_ramps->m_rampSpawnAmount, 1, SpawnOilTankRampInFront, nullptr);
}

void EBMapsFourxFourTrails()
{
	SetHeader("Object Maps -> 4x4 Trails");
	AddItem("Fun Land", FourxFourTrailFunLand, "Credit: SP-JESTER");
	AddItem("Long Horn", FourxFourTrailLongHorn, "Credit: SP-JESTER");
	AddItem("The Reef", FourxFourTrailTheReef, "Credit: SP-JESTER");
	AddItem("The Split", FourxFourTrailTheSplit, "Credit: SP-JESTER");
	AddItem("Hummer Track 1", HummerTrial1, "Credit: MrNaikkeli");
	AddItem("Hummer Track 2", HummerTrial2, "Credit: MrNaikkeli");
	AddItem("Hummer Track 3", HummerTrial3, "Credit: MrNaikkeli");
}

void EBMapsAirport() //need to categorise these into "Ramps" and "Other" as 99% of people want ramps.
{
	SetHeader("Object Maps -> Airport");
	AddItem("Air Tower", AirTowerByNightrule, "(70) By: Nightrule"); //the no.1 IMO //70 objects
	AddItem("Playground", AirportPlayground, "(66) By: IIV NATHAN VII"); //66 Objects
	AddItem("Stunt Park", AirportStuntPark, "(69) By: OM4R 7"); //69 Objects
	AddItem("Arena", AirportArenaByNightrule, "(67) By: Nightrule");  //67 Objects
	AddItem("High Car Shopping Centre", AirportHighCarShoppingCentre, "(60) By: Kuttner"); //60 Objects
	AddItem("Derby Plaza", AirportDerbyPlaza, "(53) By: Fendron"); //53 Objects
	AddItem("Ocean Overpass", AirportOceanOverpass, "(65) By: InTheSevens"); //65 Objects
	AddItem("Simple Ramp", AirportSimpleRamp, "(11) By: OhManMyBad"); //11 Objects
	AddItem("Vert-Ramp", AirportVertRamp, "(32) By: DJ Atomica"); //32 Objects
	AddItem("Halfpipe", AirportHalfPipe, "(54) By: DJ Atomica"); //54 Objects
	AddItem("Half Loop", AirportHalfLoop, "(34) By: DJ Atomica"); //34 Objects
	AddItem("Crazy Loop", AirportCrazyLoop, "(52) By: Fendron"); //52 Objs
	AddItem("Vertical Ride", AirportVerticalRide, "(70) By: Fendron"); //70 Objects
	AddItem("Super Halfpipe", AirportSuperHalfpipe, "(62) By: Tijn1117"); //62 Objects
	AddItem("Simple Loop", AirportSimpleLoop, "(50) By: sergun99090"); //50 Objects
	AddItem("Simple Loop 2", AirportSimpleLoopTwo, "(44) By: sergun99090"); //44 Objects
	AddItem("Ramp Teil 1", AirportRampTeilOne, "(50) By: LT Winning"); //50 Objects
	AddItem("Ramp Teil 7", AirportRampTeilSeven, "(48) By: LT Winning"); //48 Objects
	AddItem("Ramp Teil 7", AirportRampTeilSeven, "(48) By: LT Winning"); //48 Objects
	AddItem("Ramp Teil 8", AirportRampTeilEight, "(48) By: LT Winning"); //48 Objects
	AddItem("Up's & Down's", AirportUpsAndDowns, "(70) By: Tijn1117"); //70 Objects
	AddItem("Fullpipe Madness", AirportFullpipeMadness, "(70) By: Fendron"); //70 objects
	AddItem("Furious Ramp", AirportFuriousRamp, "(70) By: Kuttner"); //70 objects
	AddItem("Good Fly", AirportGoodFly, "(70) By: Kuttner"); //70 Objects
	AddItem("Up & Over", AirportUpAndOver, "(70) By: Kuttner"); //70 Objects
	AddItem("Super Jumpy Track", AirportSuperJumpyTrack, "(70) By: Tijn1117"); //70 Objects
	AddItem("Dragstrip", AirportDragStrip, "(41) By: InTheSevens & OhManMyBad");  //41 Objects
	AddItem("Runway Race Track", AirportAroundAirportRaceTrack, "(66) By: OhManMyBad"); //66 Objects
	AddItem("Nascar", AirportNascar, "By: IIV NATHAN VII");  //need to fix/just add from source.
	AddItem("Race Track", AirportRaceTrack, "(63) By: Fendron"); //63 Objects
	AddItem("Rally Track", AirportRallyTrack, "(42) By: InTheSevens & OhManMyBad"); //42 Objects
	AddItem("Fun World", AirportFunWorld, "(68) By: xiTz ZaeZae o");  //68 objs
	AddItem("Stunting Tour", AirportStuntingTour, "(70) By: Tizixoriginal"); //70 Objects
	AddItem("Bike Suntpark", AirportBikeStuntpark, "(27) By: Carrythxd");  //27 Objects
	AddItem("The Bowl", AirportTheBowl, "(56) By: Jumper"); //56 Objects
	AddItem("Twister Slide", AirportTwisterSlide, "(70) By: Fendron"); //70 Objects
	AddItem("WTF! Track", AirportWTFTrack, "(70) By: Kuttner"); //70 Objects
	AddItem("Fast Tiger", AirportMapFastTiger, "(70) By: Tijn1117");  //70 Objects
	AddItem("Ball of Steel", AirportBallOfSteel, "(49) By: Tijn1117"); //49 objects
	AddItem("Simple Flower Pool", AirportSimpleFlowerPool, "(16) By: Fendron");
	AddItem("Sky High Air Base", AirportSkyHighAirBase, "(37) By: InTheSevens & OhManMyBad");
	AddItem("xiTz Sky Ramp o", AirportSkyRamp, "(64) By: xiTz LaDy ReD o"); //64 Objects
	AddItem("Round Around", AirportRoundAround, "(70) By: Kuttner"); //70 Objects
	AddItem("Swimming Paradise", AirportSwimmingParadise, "(69) By: Tizixoriginal"); //69 Objects
	AddItem("Trials Evolution Track", AirportTrialsEvolutionTrack, "(67) By: XTREME0235"); //67 Objects
	AddItem("Ultimate Bike Jump", AirportUltimateBikeJump, "(16) By: Elite ISO Mods"); //16 Objects
	AddItem("The Chill Out Garage", AirportChillOutGarage, "(68) By: xiTz ZaeZae o"); //68 Objects
	AddItem("Car Showroom 2", AirportCarShowroomTwo, "(70) By: Tizixoriginal"); //70 Objects
	AddItem("Ramp on Airport", AirportRampOnAirport, "(59) By: MiauczekPL"); //59 Objects
	AddItem("The Trap House", AirportTrapHouse, "(64) By: xiTz ZaeZae o"); //64 Objects
	AddItem("Nightclub", AirportNightClub, "(68) By: Fendron"); //68 Objects
	AddItem("Base", AirportBase, "(47) By: ijustkilledu007"); //47 Objects
}

void EBMapsMisc()
{
	SetHeader("Object Maps -> Misc");
	AddItem("Beach Ramp", FireflyIlandBeachRamp, "(37) By: gta4tank"); //37 Objects			
	AddItem("Beach Bunker's", FireflyIslandBeachBunkers, "(45) By: Youriiiii"); //45 Objects
	AddItem("Sultan Ramp's", MapModSultanRSRamps, "(12) By: Jumper"); //12 Objects
	AddItem("Leap of Faith Ramp", LeapOfFaithRamp, "(47) By: mike6649"); //47 Objects
	AddItem("Tower Jump's", TowerJumps, "(70) By: Fendron"); //70 Objects
	AddItem("Porta Cabin Park", PortaCabinPark, "(45) By: TheLastCommand"); //45 Objects //ehhhh
	AddItem("RP House", RPHouse, "(26) By: jelle1212"); //26 Objects
	AddItem("Southern Police Station", SouthernPoliceStation, "(69) By: aTk Pirate"); //69 Objects
	AddItem("Summer Cottage", MapSummerCottage, "(68) By: TheLastCommand");
	AddItem("Store", MapStoreByTheLastCommand, "(69) By: TheLastCommand"); //this one is crap might get rid
}


void saveMapFile(int un1, int grabCurrentLocationForTp) //could probably just check if X was pressed recently... or just use the second arg.
{
	if (GetSpawnedObjectCount() == 0)
		printf("~r~CTL: ~w~It helps if you spawn something to save first %s...", NetworkPlayerGetNameWithColour(myPlayerIndex));

	if (showAndGetKeyboardInput("Save map_name.ini", false, "map_name", 20, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		//check we got objects
		char* filePath = BUFFER64;
		sprintf(filePath, "object_maps\\%s.ini", ctlMem->m_char_buffers.keyboard_input_buffer);

		char* noteString = BUFFER128;
		bool note_provided = false;
		sprintf(noteString, "By %s (Xbox 360 - Cartel)", GET_PLAYER_NAME(myPlayerIndex));

		WAIT(500);

		if (showAndGetKeyboardInput("Add note? ~PAD_B~ to skip.", false, noteString, 100, 0, 0.5f, 0.5f, 200, 0, 0))
		{
			note_provided = true;
			sprintf(noteString, "Note=%s", ctlMem->m_char_buffers.keyboard_input_buffer);
		}
		if (CTL_OPEN_FILE_FOR_WRITING(filePath))
		{
			if (grabCurrentLocationForTp) //line 1
			{
				CTL_WRITE_LINE_TO_OPENED_FILE("[Player]");
				CTL_WRITE_LINE_TO_OPENED_FILE("Teleport=1");
				float mX, mY, mZ;
				GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
				char* xBuff = BUFFER32;
				strcpy(xBuff, "x=", 32);
				stradd(xBuff, CTL_FLOAT_TO_STRING(mX), 32);
				CTL_WRITE_LINE_TO_OPENED_FILE(xBuff);
				char* yBuff = BUFFER32;
				strcpy(yBuff, "y=", 32);
				stradd(yBuff, CTL_FLOAT_TO_STRING(mY), 32);
				CTL_WRITE_LINE_TO_OPENED_FILE(yBuff);
				char* zBuff = BUFFER32;
				strcpy(zBuff, "z=", 32);
				stradd(zBuff, CTL_FLOAT_TO_STRING(mZ), 32);
				CTL_WRITE_LINE_TO_OPENED_FILE(zBuff);
			}
			if (note_provided)
			{
				CTL_WRITE_LINE_TO_OPENED_FILE("[Note]");
				char* noteLine = BUFFER128;
				sprintf(noteLine, "Note=%s", noteString);
				CTL_WRITE_LINE_TO_OPENED_FILE(noteString);
			}

			int object_count = 1;
			for (int i = 0; i < GetSpawnedObjectCount(); i++)
			{
				WAIT(0);
				if (DOES_OBJECT_EXIST(spawnerMem[i].o))
				{
					char* objCountLineBuff = BUFFER16;
					sprintf(objCountLineBuff, "[%i]", object_count);
					CTL_WRITE_LINE_TO_OPENED_FILE(objCountLineBuff);

					float ox, oy, oz, oh;
					GET_OBJECT_COORDINATES(spawnerMem[i].o, &ox, &oy, &oz);
					GET_OBJECT_HEADING(spawnerMem[i].o, &oh);

					char* xCoordBuff = BUFFER32;
					strcpy(xCoordBuff, "x=", 32);
					stradd(xCoordBuff, CTL_FLOAT_TO_STRING(ox), 32);
					CTL_WRITE_LINE_TO_OPENED_FILE(xCoordBuff);

					char* yCoordBuff = BUFFER32;
					strcpy(yCoordBuff, "y=", 32);
					stradd(yCoordBuff, CTL_FLOAT_TO_STRING(oy), 32);
					CTL_WRITE_LINE_TO_OPENED_FILE(yCoordBuff);

					char* zCoordBuff = BUFFER32;
					strcpy(zCoordBuff, "z=", 32);
					stradd(zCoordBuff, CTL_FLOAT_TO_STRING(oz), 32);
					CTL_WRITE_LINE_TO_OPENED_FILE(zCoordBuff);

					char* hCoordBuff = BUFFER32;
					strcpy(hCoordBuff, "h=", 32);
					stradd(hCoordBuff, CTL_FLOAT_TO_STRING(oh), 32);
					CTL_WRITE_LINE_TO_OPENED_FILE(hCoordBuff);

					uint objModel;
					GET_OBJECT_MODEL(spawnerMem[i].o, &objModel);
					char* modelBuff = BUFFER32;
					sprintf(modelBuff, "Model=%i", objModel);
					CTL_WRITE_LINE_TO_OPENED_FILE(modelBuff);

					float qx, qy, qz, qw;
					GET_OBJECT_QUATERNION(spawnerMem[i].o, &qx, &qy, &qz, &qw);

					char* qxCoordBuff = BUFFER32;
					strcpy(qxCoordBuff, "qx=", 32);
					stradd(qxCoordBuff, CTL_FLOAT_TO_STRING(qx), 32);
					CTL_WRITE_LINE_TO_OPENED_FILE(qxCoordBuff);

					char* qyCoordBuff = BUFFER32;
					strcpy(qyCoordBuff, "qy=", 32);
					stradd(qyCoordBuff, CTL_FLOAT_TO_STRING(qy), 32);
					CTL_WRITE_LINE_TO_OPENED_FILE(qyCoordBuff);

					char* qzCoordBuff = BUFFER32;
					strcpy(qzCoordBuff, "qz=", 32);
					stradd(qzCoordBuff, CTL_FLOAT_TO_STRING(qz), 32);
					CTL_WRITE_LINE_TO_OPENED_FILE(qzCoordBuff);

					char* qwCoordBuff = BUFFER32;
					strcpy(qwCoordBuff, "qw=", 32);
					stradd(qwCoordBuff, CTL_FLOAT_TO_STRING(qw), 32);
					CTL_WRITE_LINE_TO_OPENED_FILE(qwCoordBuff);

					CTL_WRITE_LINE_TO_OPENED_FILE("offz=0.0"); //fuck knows what this is even used for.
					
					CTL_WRITE_LINE_TO_OPENED_FILE("Dynamic=0");
					CTL_WRITE_LINE_TO_OPENED_FILE("Collision=1");
					CTL_WRITE_LINE_TO_OPENED_FILE("Visible=1");
					object_count++;
				}
			}
			CTL_WRITE_LINE_TO_OPENED_FILE("#Created using Cartel menu on Xbox 360.");
			CTL_CLOSE_OPENED_WRITING_FILE();
			printf("~r~CTL: ~w~File saved to ~y~Cartel\\%s", filePath);
		}
	}
}

void saveMapFileTP(int un1, int un2)
{
	saveMapFile(un1, 1);
}

void loadMapFileGivenPath(char* filePath) //will load filename
{
	const int lineCount = CTL_GET_LINES_IN_FILE(filePath);
	int object_count = 0;
	char* NoteString = BUFFER128;  //max 100
	if (CTL_OPEN_FILE_FOR_READING(filePath))
	{
		ClearCurrentlySpawnedMap(); //mayaswell clear this one out too.. (In-built menu maps)..
		DeleteLastSpawnedObjects(max_spawner_objs);
		vector3 startPos;
		bool teleport_to_start;
		bool have_we_skipped_line = false;
		char* skipped_line;
		for (int i = 1; i < lineCount; i++)
		{
			//WAIT(0);
			char* lineStr;
			if (have_we_skipped_line)
			{
				lineStr = skipped_line;
				have_we_skipped_line = false;
			}
			else
				lineStr = CTL_READ_LINE_FROM_OPENED_FILE(i); //line 1

#ifdef Developer
			PRINTSTRING(lineStr);
			PRINTNL();
#endif
			if (COMPARE_STRING(lineStr, "[Player]")) //line 1
			{
				//change this so it saves and teleports after, map doesn't load quick enough and fall to death often.
				if (COMPARE_STRING(CTL_READ_LINE_FROM_OPENED_FILE(i + 1), "Teleport=1")) //line 2
				{
					//StringToFloat
					startPos.x = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 2) + 2); //skip "x="
					startPos.y = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 3) + 2); //skip "y="
					startPos.z = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 4) + 2); //skip "z="
					teleport_to_start = true;
					i += 4; //skip to line after co-ordinates
					continue;
				}
			}
			else if (COMPARE_STRING(lineStr, "[Note]")) //Line 1 probably
			{
				strcpy(NoteString, (CTL_READ_LINE_FROM_OPENED_FILE(i + 1) + 5), 100);
				i += 1;
				continue;
			}
			else
			{
				//Saw one .ini that was not ordered incrementally, checking first is '[' and last is ']' could work or something for that, for now IDGAF.
				char tempBuff[8];
				sprintf(tempBuff, "[%i]", (object_count + 1));
				if (COMPARE_STRING(tempBuff, lineStr)) //object number i
				{
					float objX = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 1) + 2); //skip "x="
					float objY = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 2) + 2); //skip "y="
					float objZ = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 3) + 2); //skip "z="
					float objH = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 4) + 2); //skip "h="
					int objModel;									//Skip "Model="
					if (STRING_TO_INT((CTL_READ_LINE_FROM_OPENED_FILE(i + 5) + 6), &objModel))
					{
						float obj_QX = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 6) + 3); //skip "qx="
						float obj_QY = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 7) + 3); //skip "qy="
						float obj_QZ = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 8) + 3); //skip "qz="
						float obj_QW = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 9) + 3); //skip "qw="
						//offz not always present for PC, we should account for the same. (Bot will insert if not present woop).
						float offz = CTL_STRING_TO_FLOAT(CTL_READ_LINE_FROM_OPENED_FILE(i + 10) + 5); //skip "offz="
						//char* debugging = CTL_READ_LINE_FROM_OPENED_FILE(i + 11);
						//bool obj_Dynamic = debugging[8] == '1'; //Dynamic=0
						bool obj_Dynamic = CTL_READ_LINE_FROM_OPENED_FILE(i + 11)[8] == '1'; //Dynamic=s
						char* Collision_String = BUFFER128;
						strcpy(Collision_String, CTL_READ_LINE_FROM_OPENED_FILE(i + 12), 128);
						bool obj_Collision = true;
						bool obj_Visible = true;
						//if (!COMPARE_STRING(GET_STRING_FROM_STRING(Collision_String, 0, 1), "[") && (i + 13) < lineCount) //if(Collision_String[0] != '[') ?? no??
						if (Collision_String[0] != '[' && (i + 13) < lineCount)
						{
							obj_Collision = Collision_String[10] == '1'; //Collision=1 //won't overflow because assigned 128 chars
							obj_Visible = CTL_READ_LINE_FROM_OPENED_FILE(i + 13)[8] == '1'; //Visible=
							i += 13;
						}
						else
						{
							i += 11; //stay on same line next iteration to catch [int] //re-reading same line is OK, previous not ok
							have_we_skipped_line = true;
							skipped_line = Collision_String;
						}
						ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
						if (object_count < max_spawner_objs - 1) //1 object means object_count == 1
							printf("~r~CTL: ~w~Loading object %i", object_count + 1);
						else
						{
							printf("~r~CTL: ~w~Object limit hit (%i) Cannot proceed anymore.", object_count + 1);
							continue;
						}

						//TODO Nath: Create another variant of CreateObject that has an OnAllMachines argument. (maybe)
						bool poolSpace = SpaceInNetObjectPool(1);
						bool createSuccess = false;
						if (object_count < 70 && poolSpace)
							createSuccess = CreateObject(objModel, objX, objY, objZ, objH, &spawnerMem[object_count].o, true);
						else
							createSuccess = CreateObjectLocally(objModel, objX, objY, objZ, objH, &spawnerMem[object_count].o);
						if (createSuccess)
						{
							sprintf(spawnerMem[object_count].name, "%i: 0x%x", object_count + 1, objModel);
							GET_GAME_TIMER(&spawnerMem[object_count].creationTime);
							SET_OBJECT_QUATERNION(spawnerMem[object_count].o, obj_QX, obj_QY, obj_QZ, obj_QW);
							SET_OBJECT_DYNAMIC(spawnerMem[object_count].o, obj_Dynamic);
							SET_OBJECT_COLLISION(spawnerMem[object_count].o, obj_Collision);
							SET_OBJECT_VISIBLE(spawnerMem[object_count].o, obj_Visible);

							object_count++;
							WAIT(1);
						}
					}
				}
			}
		}
		CTL_CLOSE_READING_FILE();

		if (teleport_to_start) //TODO: Rig this properly later.
			SET_CHAR_COORDINATES(myPlayerPed, startPos.x, startPos.y, startPos.z);

		//maybe print object_count

		if (NoteString[0] != '\0')
		{
			char* noteBuff = BUFFERMAX;
			sprintf(noteBuff, "~y~%s", NoteString);
			PRINT_STRING_WITH_TWO_LITERAL_STRINGS_NOW("STRTNM3", "~r~CTL: ~w~Note found in file", noteBuff, 5000, true);
		}

	}
	else
		printf("~r~CTL: ~w~Unable to open GTA IV\\Cartel\\%s", filePath);
}

void loadLocalMapFile(int un1, int un2)
{
	if (showAndGetKeyboardInput("Load map_name.ini", false, "", 20, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		char* filePath = BUFFER64;
		sprintf(filePath, "object_maps\\%s.ini", ctlMem->m_char_buffers.keyboard_input_buffer);
		loadMapFileGivenPath(filePath);
	}
}

void ObjectMaps()
{
	SetHeader("World -> Object Maps");
	AddItemMenu("4x4 Trails", EBMapsFourxFourTrails, "Off-roading"); //EB page 1 (Map Mods 01 - 20)
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_AIRPORT], EBMapsAirport, nullptr); //EB Page 1-2. //"Airport"
	AddItemMenu("Misc", EBMapsMisc, nullptr);
	AddItem("P-House Demolition Derby", PHouseDemolitionDerby, "(57) By: eabettebalterze"); //57 Objects
	AddItem("Delete current map", ClearCurrentlySpawnedMap, nullptr);
}

bool HasAirWalkerFromOtherUser(PLAYER_INDEX PlayerID, OBJECT_INDEX *scan_obj, PLAYER_INDEX *Controller)
{
	return ScanNetworkObjectPoolForClosestModelToPlayer(PlayerID, OBJECT_CJ_GAME_GRID, true, true, 4.0f, false, &*Controller, &*scan_obj, (TO_FLOAT(PlayerID) + 250.0f));
	//return (ScanNetworkObjectPoolForClosestModelToPlayer(PlayerID, OBJECT_CJ_GAME_GRID, true, true, 4.0f, false, &*Controller, &*scan_obj));
}

void setWalkOnAir(int unused, int unused2)
{
	float pX, pY, pZ, pH;
	PED_INDEX targetPed;
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, GetMenuPlayerId()))
	{	//create the object beneath them to avoid waiting in cartel_helper.sco
		OBJECT_INDEX walker; PLAYER_INDEX controller;
		if (HasAirWalkerFromOtherUser(GetMenuPlayerId(), &walker, &controller))
		{
			if (controller == GetMenuPlayerId())
				printf("~r~CTL: %s ~w~already has an air object.", NetworkPlayerGetNameWithColour(controller));
			else
			{
				char* buffer = BUFFER128;
				strcpy(buffer, "~r~CTL: ", 128);
				stradd(buffer, NetworkPlayerGetNameWithColour(GetMenuPlayerId()), 128);
				stradd(buffer, " ~w~has an air object from ", 128);
				stradd(buffer, NetworkPlayerGetNameWithColour(controller), 128);
				stradd(buffer, ".", 128);
				printf(buffer);
			}
			CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, GetMenuPlayerId());
			return;
		}
		if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
		{
			GET_PLAYER_CHAR(GetMenuPlayerId(), &targetPed);
			GET_CHAR_COORDINATES(targetPed, &pX, &pY, &freeMem->m_walk_on_air.m_height[GetMenuPlayerId()]);
			freeMem->m_walk_on_air.m_height[GetMenuPlayerId()] -= 1.0f;
			GET_CHAR_HEADING(targetPed, &pH);
			//not ideal would need to be invisible but could work if (CreateObject(OBJECT_CJ_CR_BACKROUND, pX, pY, freeMem->m_walk_on_air.m_height[GetMenuPlayerId()], pH, &freeMem->m_walk_on_air.m_object[GetMenuPlayerId()], true)) //create here to when helper picks up the loop it doesn't wait to spawn this object.
			//cool but too deep by the looks of it. if (CreateObject(OBJECT_EC_JETL_OW2, pX, pY, freeMem->m_walk_on_air.m_height[GetMenuPlayerId()], pH, &freeMem->m_walk_on_air.m_object[GetMenuPlayerId()], true)) //create here to when helper picks up the loop it doesn't wait to spawn this object.
			// //smaller than the black box/background but taller... if (CreateObject(OBJECT_EC_JETL_OW2, pX, pY, freeMem->m_walk_on_air.m_height[GetMenuPlayerId()], pH, &freeMem->m_walk_on_air.m_object[GetMenuPlayerId()], true)) //create here to when helper picks up the loop it doesn't wait to spawn this object.
			if (CreateObject(OBJECT_CJ_GAME_GRID, pX, pY, freeMem->m_walk_on_air.m_height[GetMenuPlayerId()], pH, &freeMem->m_walk_on_air.m_object[GetMenuPlayerId()], true)) //create here to when helper picks up the loop it doesn't wait to spawn this object.
			{
				//ATTACH_OBJECT_TO_PED_PHYSICALLY(freeMem->m_walk_water_object[GetMenuPlayerId()], targetPed, BONE_ROOT, BONE_ROOT, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 100.0f, true);
				SET_BIT(&ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, GetMenuPlayerId() + 16); //ascend when shooting
				SET_OBJECT_HEALTH(freeMem->m_walk_on_air.m_object[GetMenuPlayerId()], (TO_FLOAT(GetMenuPlayerId()) + 250.0f));
			}
		}
	}
	else
	{
		if (DOES_OBJECT_EXIST(freeMem->m_walk_on_air.m_object[GetMenuPlayerId()])) //can probably remove this once checked out the processes.
		{	//would get deleted immediately if they leave so probs okay to remove active checks and just use has char here.
			if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
			{
				GET_PLAYER_CHAR(GetMenuPlayerId(), &targetPed);
				GET_CHAR_COORDINATES(targetPed, &pX, &pY, &pZ);
				if (COMPARE_STRING(GET_NAME_OF_ZONE(pX, pY, pZ), "LIBERTY")) //out at sea
				{
					float destX, destY, destZ;
					if (GET_CLOSEST_CAR_NODE(pX, pY, pZ, &destX, &destY, &destZ))
					{
						//GET_SAFE_POSITION_FOR_CHAR
						GET_CHAR_HEADING(targetPed, &pH);
						if(IS_CHAR_IN_ANY_CAR(targetPed))
						{ //already auto-disables if they get into a boat or heli.
							ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
							printf("~r~CTL: ~w~Warping %s ~w~back to shore.", NetworkPlayerGetNameWithColour(GetMenuPlayerId())); //only notify if there's likely to be a wait.
							TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), destX, destY, destZ + 0.5f, pH);
						}
						else
						{
							//this does get called by TeleportNetworkPlayerToCoordinate however this way we don't wait for teleport on foot success.. who cares, if they got mods they can fix themselves.
							TeleportNetworkPlayerOnFoot(GetMenuPlayerId(), destX, destY, destZ + 1.0f, pH, false);
						}
					}
				}
			}
			if (IS_PED_ATTACHED_TO_OBJECT(myPlayerPed, freeMem->m_walk_on_air.m_object[GetMenuPlayerId()])) //no need to wait so don't call detach function
			{
				DETACH_PED(myPlayerPed, true);
				WAIT(10);
				CLEAR_CHAR_TASKS_IMMEDIATELY(myPlayerPed);
				bitReset(&ctlMem->m_bit_sets.m_player, attachedToWalker); //NEW
				WAIT(90);
			}
			DeleteObject(&freeMem->m_walk_on_air.m_object[GetMenuPlayerId()]);
		}
		CLEAR_BIT(&ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, (GetMenuPlayerId() + 16)); //clear invisibility toggle.

	}
}

void setWalkOnAirObjectVisibility(int unused, int unused2) //don't need to loop visibility this way..
{
	if (DOES_OBJECT_EXIST(freeMem->m_walk_on_air.m_object[GetMenuPlayerId()]))
		SET_OBJECT_VISIBLE(freeMem->m_walk_on_air.m_object[GetMenuPlayerId()], !IS_BIT_SET(ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, GetMenuPlayerId() + 16));
	else
		printf("~r~CTL: ~w~Visibility huh? On what? And where? You didn't turn it on, the object doesn't exist.");
}

void setWalkOnAirObjectHeight()
{
	float oX, oY, oZ;

	if (DOES_OBJECT_EXIST(freeMem->m_walk_on_air.m_object[GetMenuPlayerId()]))
	{
		GET_OBJECT_COORDINATES(freeMem->m_walk_on_air.m_object[GetMenuPlayerId()], &oX, &oY, &oZ);
		SET_OBJECT_COORDINATES(freeMem->m_walk_on_air.m_object[GetMenuPlayerId()], oX, oY, freeMem->m_walk_on_air.m_height[GetMenuPlayerId()]);
	}
}

void attachToWalker(int unused, int unused2)
{
	static SEQUENCE_INDEX SitWalkerAnimTask; //seems to only repeat over network if its a global var, 

	OBJECT_INDEX target_walker; PLAYER_INDEX controller;
	if (!HasAirWalkerFromOtherUser(GetMenuPlayerId(), &target_walker, &controller))
	{
		target_walker = freeMem->m_walk_on_air.m_object[GetMenuPlayerId()];
	}
	//need to disable/kill own walker if we attach..
	if (DOES_OBJECT_EXIST(target_walker))
	{
		REQUEST_ANIMS("amb@comedy");

		////NEW / TESTING
		disableFlymod();
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			VEHICLE_INDEX myVeh;
			GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
			REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION(myPlayerPed, myVeh);
			WAIT(100);
		}
		//END NEW/TESTING

		detachLocalPlayerFromVehicleOrWalker();
		if (attachToWalkerSelect == 0)
			ATTACH_PED_TO_OBJECT(myPlayerPed, target_walker, 0, 0.0f, 4.4300f, 0.7099f, 0, 0, true, false);
		else
			ATTACH_PED_TO_OBJECT(myPlayerPed, target_walker, 0, 0.0f, -4.4300f, 0.7099f, 3.15f, 0, true, false);

		printf("~r~CTL: ~w~Press ~PAD_Y~ to detach.");
		while (!HAVE_ANIMS_LOADED("amb@comedy")) 
			WAIT(0);

		OPEN_SEQUENCE_TASK(&SitWalkerAnimTask);
		TASK_PLAY_ANIM_WITH_FLAGS(0, "jeer_a", "amb@comedy", 8.0, 0, 0);
		TASK_PLAY_ANIM_WITH_FLAGS(0, "jeer_b", "amb@comedy", 8.0, 0, 0);
		TASK_PLAY_ANIM_WITH_FLAGS(0, "cheer_a", "amb@comedy", 8.0, 0, 0);
		TASK_PLAY_ANIM_WITH_FLAGS(0, "cheer_b", "amb@comedy", 8.0, 0, 0);
		TASK_PLAY_ANIM_WITH_FLAGS(0, "clap_a", "amb@comedy", 8.0, 0, 0);
		TASK_PLAY_ANIM_WITH_FLAGS(0, "laugh_f", "amb@comedy", 8.0, 0, 0);

		//Might need to check if player is sat on pad and not playing any of the above anims, if so, start the sequence again.
		SET_SEQUENCE_TO_REPEAT(SitWalkerAnimTask, true); //This needs to go before CLOSE_SEQUENCE_TASK but it doesn't sync over network I think. //uhh it does right?
		CLOSE_SEQUENCE_TASK(SitWalkerAnimTask);
		TASK_PERFORM_SEQUENCE(myPlayerPed, SitWalkerAnimTask);
		bitSet(&ctlMem->m_bit_sets.m_player, attachedToWalker);
	}
	else
		printf("~r~CTL: ~w~It helps if the walker you're trying to attach to exists in the first place.");
}

void WorldAirOptions()
{
	
	SetHeader("Air options");
	AddItemBool("Walk on Air", &ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, GetMenuPlayerId(), setWalkOnAir, (GetMenuPlayerId() == myPlayerIndex) ? "Because why the fuck not?" : "Toggle to reset position.");

	if (GetMenuPlayerId() != myPlayerIndex)
		AddItemChar("Attach self to walker", false, attachToWalkerStrings, 2, &attachToWalkerSelect, attachToWalker, "Dangle those feet.");

	AddItemFloat("Walk on Air height", true, -50.0f, 1000.0f, &freeMem->m_walk_on_air.m_height[GetMenuPlayerId()], airIncAmount, 2.0f, setWalkOnAirObjectHeight, nullptr);
	AddItemFloat("Height adjust amount", true, 0.250f, 5.0f, &airIncAmount, 0.25f, 2.0f, RefreshCurrentPage, "Stick to 0.25 if you wanna play elevators.");
	AddItemBool("Walk on Air - object invisibility", &ctlMem->m_bit_sets.m_walkOnAir_walkerInvisibility, (GetMenuPlayerId() + 16), setWalkOnAirObjectVisibility, "Make players walker invisible.");
	AddItemBool("Height Control - Shoot = Ascend", &ctlMem->m_bit_sets.m_unlimitedAmmo_walkOnAirShootToAscend, (GetMenuPlayerId() + 16), nullptr, GetMenuPlayerId() == myPlayerIndex ? nullptr : "Only detects within 150 distance.");
	AddItem("INFO: Crouching = Descend", nullptr, nullptr);
}

void WorldRunwayPlanesOn()
{		
	SWITCH_AMBIENT_PLANES(true); 
}

void WorldRunwayPlanesOff()
{	
	SWITCH_AMBIENT_PLANES(false); 
}

const char* RadioName(int radioIndex)
{
	switch (radioIndex)
	{
	case RADIO_VIBE: return "The Vibe";
	case RADIO_LIBERTY_ROCK: return "Liberty Rock";
	case RADIO_JAZZ: return "Jazz Nation Radio";
	case RADIO_MASSIVE: return "Massive B";
	case RADIO_K109: return "K109";
	case RADIO_WKTT: return "WKTT";
	case RADIO_LCHC: return "L.C.H.C";
	case RADIO_JOURNEY: return "The Journey";
	case RADIO_FUSION: return "Fusion FM";
	case RADIO_BEAT: return "Beat 102.7";
	case RADIO_BROKER: return "Radio Broker";
	case RADIO_VLADIVOSTOK: return "VLADIVOSTOK FM";
	case RADIO_PLR: return "PLR - PUBLIC LIBERTY RADIO";
	case RADIO_SAN_JUAN_SOUNDS: return "SAN JUAN SOUNDS";
	case RADIO_ELECTRO: return "ELECTRO-CHOC";
	case RADIO_CLASSICS: return "CLASSICS";
	case RADIO_IF99: return "IF99 - INTERNATIONAL FUNK";
	case RADIO_TUFF: return "TUFF GONG";
	case RADIO_MP3: return "INDEPENDENCE FM";
	case RADIO_INTEGRITY: return "RADIO INTEGRITY";
	}

	return "NULL";
}

eRadioStation GetHighestRadioIndexForCurrentEpisode()
{
	if (myCurrentEpisode == EPISODE_ORIGINAL)
		return RADIO_TUFF; //max;
	return RADIO_INTEGRITY;
}

eRadioStation GetNextRadioShiftUp()
{
	if (GET_PLAYER_RADIO_STATION_INDEX() == GetHighestRadioIndexForCurrentEpisode())
		return RADIO_VIBE;
	return GET_PLAYER_RADIO_STATION_INDEX() + 1;
}

eRadioStation GetNextRadioShiftDown()
{
	if (GET_PLAYER_RADIO_STATION_INDEX() == RADIO_VIBE)
		return GetHighestRadioIndexForCurrentEpisode();
	return GET_PLAYER_RADIO_STATION_INDEX() - 1;
}

const char* GetNextRadioName(bool up)
{
	if (up)
		return RadioName(GetNextRadioShiftUp());
	else
		return  RadioName(GetNextRadioShiftDown());

	return "NULL";
}

void printNextRadio(bool up)
{
	printf("~r~CTL: ~w~Tuning in to ~y~%s.", GetNextRadioName(up));
}

void EnableRadioOnFoot(int unused1, int unused2) 
{	
	SET_MOBILE_PHONE_RADIO_STATE(true); 
	SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true); 
}

void DisableRadioOnFoot(int unused1, int unused2) 
{ 
	SET_MOBILE_PHONE_RADIO_STATE(false); 
}

void RetuneRadioUp(int unused1, int unused2) 
{		
	printNextRadio(true); 
	RETUNE_RADIO_UP();
} //da fk is Bobby Konders? lmao

void RetuneRadioDown(int unused1, int unused2) 
{	
	printNextRadio(false); 
	RETUNE_RADIO_DOWN();
}

void SkipRadioTrack(int un1, int un2)
{
	SKIP_RADIO_FORWARD();
}

void printCurrentTrack(int un1, int un2)
{
	char* gxt_buff = BUFFER16;
	int track_id;
	if (IS_GAME_IN_CONTROL_OF_MUSIC())
		track_id = GET_AUDIBLE_MUSIC_TRACK_TEXT_ID();
	else
		GENERATE_RANDOM_INT_IN_RANGE(290, 295, &track_id);
	
	sprintf(gxt_buff, "TM_82_%i", track_id);
	printf("~r~CTL: ~w~%s", GET_STRING_FROM_TEXT_FILE(gxt_buff) + 5); //skip "ZiT! "
}

#ifdef Developer
int vladTrackSelect;

void radTrackTest(int un1, int un2)
{
	//FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_STATION_VLADIVOSTOK_INTRO_TRACK", 2, vladTrackSelect); //last arg seems to do nothing.
	FREEZE_RADIO_STATION("VLADIVOSTOK");
	RETUNE_RADIO_TO_STATION_INDEX(RADIO_VLADIVOSTOK);
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_STATION_VLADIVOSTOK_INTRO_TRACK", 0, 0); //works consistently, need to find the names.
	WAIT(500);
	UNFREEZE_RADIO_STATION("VLADIVOSTOK");
	//UNFREEZE_RADIO_STATION("VLADIVOSTOK");
	//i think maybe minute and seconds until finish? hmm..
	//FORCE_INITIAL_PLAYER_STATION("VLADIVOSTOK");
	//RETUNE_RADIO_TO_STATION_INDEX(RADIO_VLADIVOSTOK);
}
void radioTest(int un1, int un2)
{
	RETUNE_RADIO_TO_STATION_INDEX(RADIO_VLADIVOSTOK);
	FREEZE_RADIO_STATION("VLADIVOSTOK");
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_STATION_VLADIVOSTOK_INTRO_TRACK", 2, 9); //this is the primary native.. other shit can be played with around it.
	FORCE_INITIAL_PLAYER_STATION("VLADIVOSTOK");
	
	//FORCE_RADIO_TRACK
}

/*RADIO_ADVERTS.RADIO_NEWS_NEWS.RADIO_WEATHER_WEATHER.RADIO_JAZZ_NATION_MUSIC.RADIO_JAZZ_NATION_IDENTS.RADIO_JAZZ_NATION_DJSOLO.RADIO_FUSION_FM_MUSIC.RADIO_FUSION_FM_IDENTS.RADIO_FUSION_FM_DJSOLO.RADIO_AFRO_BEAT_MUSIC.RADIO_AFRO_BEAT_IDENTS.RADIO_AFRO_BEAT_DJSOLO.RADIO_BEAT_95_MUSIC.RADIO_NY_CLASSICS_MUSIC.RADIO_THE_VIBE_MUSIC.RADIO_THE_VIBE_DJSOLO.RADIO_THE_VIBE_IDENTS.RADIO_CLASSICAL_AMBIENT_MUSIC.RADIO_CLASSICAL_AMBIENT_IDENTS.RADIO_CLASSICAL_AMBIENT_DJSOLO.RADIO_BOBBY_KONDERS_MUSIC.RADIO_BABYLON_MUSIC.RADIO_BABYLON_DJSOLO.RADIO_BABYLON_IDENTS.RADIO_SAN_JUAN_SOUNDS_MUSIC.RADIO_SAN_JUAN_SOUNDS_DJSOLO.RADIO_SAN_JUAN_SOUNDS_IDENTS.RADIO_LIBERTY_ROCK_MUSIC.RADIO_LIBERTY_ROCK_IDENTS.RADIO_LIBERTY_ROCK_DJSOLO.RADIO_DANCE_ROCK_MUSIC.RADIO_DANCE_ROCK_IDENTS.RADIO_DANCE_ROCK_DJSOLO.RADIO_HARDCORE_MUSIC.RADIO_HARDCORE_IDENTS.RADIO_HARDCORE_DJSOLO.RADIO_DANCE_MIX_MUSIC.RADIO_K109_THE_STUDIO_MUSIC.RADIO_K109_THE_STUDIO_IDENTS.RADIO_K109_THE_STUDIO_DJSOLO.RADIO_VLADIVOSTOK_MUSIC.RADIO_VLADIVOSTOK_DJSOLO.RADIO_VLADIVOSTOK_IDENTS.RADIO_WKTT_MUSIC.RADIO_WKTT_IDENTS.RADIO_PLR_MUSIC.RADIO_PLR_IDENTS.RADIO_LAZLOW_MUSIC.RADIO_LAZLOW_IDENTS.END_CREDITS_TRACK_LIST.RADIO_NEWS_01.RADIO_NEWS_02.GTA4_RADIO_STATIONS_01.RADIO_STATION_ADVERTS_GENERIC.RADIO_STATION_ADVERTS_RUSSIAN.RADIO_STATION_ADVERTS_SPANISH.RADIO_STATION_AFRO_BEAT.RADIO_STATION_BABYLON.RADIO_STATION_BEAT_95.RADIO_STATION_BOBBY_KONDERS.RADIO_STATION_CLASSICAL_AMBIENT.RADIO_STATION_DANCE_MIX.RADIO_STATION_DANCE_ROCK.RADIO_STATION_FUSION_FM.RADIO_STATION_HARDCORE.RADIO_STATION_JAZZ_NATION.RADIO_STATION_K109_THE_STUDIO.RADIO_STATION_LAZLOW.RADIO_STATION_LIBERTY_ROCK.RADIO_STATION_NY_CLASSICS.RADIO_STATION_PLR.RADIO_STATION_SAN_JUAN_SOUNDS.RADIO_STATION_THE_VIBE.RADIO_STATION_VLADIVOSTOK.RADIO_STATION_VLADIVOSTOK_INTRO_TRACK.RADIO_STATION_WKTT.RADIO_TRACK_CATEGORY_WEIGHTS_FROM_BACK_TO_BACK_MUSIC.RADIO_TRACK_CATEGORY_WEIGHTS_FROM_MUSIC.RADIO_TRACK_CATEGORY_MIN_TIME_BETWEEN_REPEATS.RADIO_TRACK_CATEGORY_WEIGHTS_NO_BACK_TO_BACK_MUSIC.SPEECH_CONTEXTS.M_M_FATCOP_BLACK.M_M_FATCOP_WHITE.M_Y_C*/
void radioTes2(int un1, int un2)
{
	RETUNE_RADIO_TO_STATION_INDEX(RADIO_VLADIVOSTOK);
	FREEZE_RADIO_STATION("VLADIVOSTOK");
	//FORCE_RADIO_TRACK("VLADIVOSTOK", "TO_NEWS_01", 0, 0);
	//FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_STATION_VLADIVOSTOK_WILD_HORSES_TRACK", 2, 0);
	//FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_STATION_VLADIVOSTOK_KINGRING_TRACK", 2, 9);
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_VLADIVOSTOK_MUSIC", 2, 3);
	FORCE_INITIAL_PLAYER_STATION("VLADIVOSTOK");
	//FORCE_INITIAL_PLAYER_STATION("VLADIVOSTOK");

	//FORCE_RADIO_TRACK
}

int argOne = 0; int argTwo = 0;
void radiotes3(int un1, int un2)
{
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_VLADIVOSTOK_MUSIC", argOne, argTwo); //SPLIN : Liniya Zhizni
}
void radiotes4(int un1, int un2)
{
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_VLADIVOSTOK_DJSOLO", argOne, argTwo); 
}
void radiotes5(int un1, int un2)
{
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_VLADIVOSTOK_IDENTS", argOne, argTwo); 
}
void radiotes6(int un1, int un2)
{
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_STATION_VLADIVOSTOK_INTRO_TRACK", argOne, argTwo);
}
void radiotes7(int un1, int un2)
{
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_STATION_VLADIVOSTOK", argOne, argTwo);
}

void callTest(char* param, int unused)
{
	FORCE_RADIO_TRACK("VLADIVOSTOK", param, 0, 0);
}

void formatTest(int un1, int un2)
{

	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_VLADIVOSTOK_WILD_HORSES", 0, 0);
}
void formatTestTwo(int un1, int un2)
{
	FORCE_RADIO_TRACK("VLADIVOSTOK", "RADIO_VLADIVOSTOK_WILD_HORSES_STEREO", 0, 0);
}
#endif


void WorldOptsRadioOnFoot()
{
	SetHeader("World -> Radio on foot");
	AddItemAlt("Radio on foot", EnableRadioOnFoot, "~PAD_A~ = Turn on, ~PAD_X~ = Turn off.", DisableRadioOnFoot, nullptr);
	AddItemAlt("Re-tune/Switch station", RetuneRadioDown, "~PAD_A~ = Left, ~PAD_X~ = Right.", RetuneRadioUp, nullptr);
	AddItem("Skip current track", SkipRadioTrack, nullptr);
	AddItem("Print current track", printCurrentTrack, nullptr);
#ifdef Developer
	AddItem("MAMA", formatTest, nullptr);
	AddItem("MAMA test 2", formatTestTwo, nullptr);
	AddItem("Roman mission radio test", radioTest, nullptr);
	AddItem("test 2", radioTes2, nullptr);
	AddItemInt("Track Vlad", false, 1, 10, &vladTrackSelect, 1, radTrackTest, nullptr);
	AddItemInt("ArgOne", false, 1, 300, &argOne, 1, nullptr, nullptr);
	AddItemInt("ArgTwo", false, 1, 300, &argTwo, 1, nullptr, nullptr);
	

#endif
}

void CreateChauffeurDriver()
{
	//Currently do Disband elsewhere. including inside SpawnChauffeurInCurrentVehicle.
	//DisbandCurrentChauffeur(0, 0); //will do nothing id doesn't exist, but will make current one drive off if it does exist/deal with clean-up.
	CreatePed(PED_M_Y_GMAF_HI_02, 0.0f, 0.0f, 0.0f, 0.0f, &ctlMem->m_chauffeur.driver, true, true);
	SET_CHAR_WILL_LEAVE_CAR_IN_COMBAT(ctlMem->m_chauffeur.driver, false);
	SET_CHAR_CAN_BE_SHOT_IN_VEHICLE(ctlMem->m_chauffeur.driver, false);
	SET_GROUP_CHAR_DUCKS_WHEN_AIMED_AT(ctlMem->m_chauffeur.driver, false);
	SET_CHAR_NEVER_LEAVES_GROUP(ctlMem->m_chauffeur.driver, true);
	SET_CHAR_CAN_BE_SHOT_IN_VEHICLE(ctlMem->m_chauffeur.driver, false);
	SET_CHAR_WILL_USE_COVER(ctlMem->m_chauffeur.driver, false);
	SET_CHAR_BULLETPROOF_VEST(ctlMem->m_chauffeur.driver, true);
	SET_CHAR_WILL_USE_CARS_IN_COMBAT(ctlMem->m_chauffeur.driver, true);
	SET_CHAR_WILL_LEAVE_CAR_IN_COMBAT(ctlMem->m_chauffeur.driver, false);
	SET_PED_DONT_DO_EVASIVE_DIVES(ctlMem->m_chauffeur.driver, true);
	SET_PED_DIES_WHEN_INJURED(ctlMem->m_chauffeur.driver, false);
	SET_CHAR_CANT_BE_DRAGGED_OUT(ctlMem->m_chauffeur.driver, true);
	SET_CHAR_INVINCIBLE(ctlMem->m_chauffeur.driver, true);
	SET_CHAR_WILL_FLY_THROUGH_WINDSCREEN(ctlMem->m_chauffeur.driver, false);
	SET_CHAR_CAN_SMASH_GLASS(ctlMem->m_chauffeur.driver, false);
}

void ChaufferSpawnVehicle(uint model)
{
	float sX, sY, sZ, sH;

	GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, 4.0f, 0.0f, &sX, &sY, &sZ);
	GET_CHAR_HEADING(myPlayerPed, &sH);

	if (SpawnVehicle(model, sX, sY, sZ, (sH - 90.0f), &ctlMem->m_chauffeur.vehicle, false, true))
	{
		int chaufferMaxPassengers;

		SET_CAR_PROOFS(ctlMem->m_chauffeur.vehicle, true, true, true, true, true);
		SET_CAN_BURST_CAR_TYRES(ctlMem->m_chauffeur.vehicle, false);
		LOCK_CAR_DOORS(ctlMem->m_chauffeur.vehicle, DOOR_LOCK_NONE);
		CreateChauffeurDriver();
		WARP_CHAR_INTO_CAR(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle);
		GET_MAXIMUM_NUMBER_OF_PASSENGERS(ctlMem->m_chauffeur.vehicle, &chaufferMaxPassengers);

		if (chaufferMaxPassengers > 1)
			TASK_ENTER_CAR_AS_PASSENGER(myPlayerPed, ctlMem->m_chauffeur.vehicle, 5000, SEAT_PASSENGER_3); //warps you if not in after the time.. //SEAT_PASSENGER_3 = 2
		else
			TASK_ENTER_CAR_AS_PASSENGER(myPlayerPed, ctlMem->m_chauffeur.vehicle, 5000, SEAT_PASSENGER_1); //SEAT_PASSENGER_1 = 0
	}
}

//for making them follow a player
//match the players speed if within so much distance
//tick every 500ms or so to task to their coordinates
//otherwise if far away go fast.
bool ClearChauffeurTarget(int unused1, int unused2)
{
	ctlMem->m_chauffeur.target_player = -1;

	if (!DOES_CHAR_EXIST(ctlMem->m_chauffeur.driver) || !DOES_VEHICLE_EXIST(ctlMem->m_chauffeur.vehicle))
	{
		printf("~r~CTL: ~w~Chauffeur or their vehicle doesn't exist, or was disbanded.");
		return false;
	}
	CLEAR_CHAR_TASKS_IMMEDIATELY(ctlMem->m_chauffeur.driver);  //warps them out annoyingly.
	WARP_CHAR_INTO_CAR(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle);
	return true;
}

void DisbandCurrentChauffeur(int unused1, int unused2)
{
	ctlMem->m_chauffeur.target_player = -1;

	NETWORK_INDEX NetworkId;
	//if we're in the car task leave dont close door? lol //yeah as user can stop them so, no reason not to click this unless you wanna ditch.
	if (DOES_CHAR_EXIST(ctlMem->m_chauffeur.driver))
	{
		if (DOES_VEHICLE_EXIST(ctlMem->m_chauffeur.vehicle))
		{
			GET_NETWORK_ID_FROM_VEHICLE(ctlMem->m_chauffeur.vehicle, &NetworkId);
			TASK_CAR_DRIVE_WANDER(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle, 10.0f, DRIVINGSTYLE_NORMAL);
			ReleaseControlOfNetworkId(NetworkId);
			MARK_CAR_AS_NO_LONGER_NEEDED(&ctlMem->m_chauffeur.vehicle);

			//Added 20th-Jan-2026 @ 00:48am
			//GET_NETWORK_ID_FROM_PED(ctlMem->m_chauffeur.driver, &NetworkId);
			//ReleaseControlOfNetworkId(NetworkId); //what about the task?
			////
			MARK_CHAR_AS_NO_LONGER_NEEDED(&ctlMem->m_chauffeur.driver);
		}
		else
			DeleteChar(&ctlMem->m_chauffeur.driver);
	}
	else
	{
		if (DOES_VEHICLE_EXIST(ctlMem->m_chauffeur.vehicle))
			DeleteCar(&ctlMem->m_chauffeur.vehicle);
	}
}

void ChaufferDriveWander(int unused1, int unused2)
{
	if (ClearChauffeurTarget(0,0))
	{
		TASK_CAR_DRIVE_WANDER(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle, ctlMem->m_chauffeur.speed, DRIVINGSTYLE_NORMAL); //DRIVINGSTYLE_NORMAL = 0
		ctlMem->m_chauffeur.grab_coords_tick = GetTickCount() + 4000; //stop it catching too soon..
		printf("~r~CTL: ~w~En route to ~y~nowhere.");
	}
} 

void ChaufferDriveWaypoint(int unused1, int unused2)
{
	vector3 waypoint;

	if (ClearChauffeurTarget(0,0))
	{
		BLIP_INDEX waypointBlip = GET_FIRST_BLIP_INFO_ID(BLIP_WAYPOINT);

		if (DOES_BLIP_EXIST(waypointBlip))
		{
			GET_BLIP_COORDS(waypointBlip, &waypoint);
			CLEAR_CHAR_TASKS_IMMEDIATELY(ctlMem->m_chauffeur.driver);
			WARP_CHAR_INTO_CAR(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle); //clearing tasks immediately warps you out.
			TASK_CAR_DRIVE_TO_COORD_NOT_AGAINST_TRAFFIC(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle, waypoint.x, waypoint.y, waypoint.z, ctlMem->m_chauffeur.speed, ctlMem->m_chauffeur.driving_style, 0, 1, 1, 1);
			printf("~r~CTL: ~w~En route to ~y~waypoint.");
			ctlMem->m_chauffeur.grab_coords_tick = GetTickCount() + 4000; //stop it catching too soon..
		}
		else
			printf("~r~CTL: ~w~You need to set a ~y~waypoint ~w~on the map.");
	}
}

void ChaufferUnStick(int unused1, int unused2)
{
	float mX, mY, mZ, rX, rY, rZ, rH;

	if (!DOES_VEHICLE_EXIST(ctlMem->m_chauffeur.vehicle))
	{
		printf("~r~CTL: ~w~Chauffeur vehicle no longer exists.");
		return;
	}

	GET_CAR_COORDINATES(ctlMem->m_chauffeur.vehicle, &mX, &mY, &mZ);
	GET_CLOSEST_CAR_NODE_WITH_HEADING(mX, mY, mZ, &rX, &rY, &rZ, &rH);
	SET_CAR_COORDINATES(ctlMem->m_chauffeur.vehicle, rX, rY, rZ);
	SET_CAR_HEADING(ctlMem->m_chauffeur.vehicle, (ctlMem->m_chauffeur.driving_style == DRIVINGSTYLE_REVERSE) ? (rH - 180.0f) : rH);
	SET_CAR_ON_GROUND_PROPERLY(ctlMem->m_chauffeur.vehicle); //if we're not in the vehicle/ mark it as no longer needed in the loop.
}

void chauffeurSetTargetPlayer(PLAYER_INDEX targetId, int unusued1)
{
	if (ClearChauffeurTarget(0,0))
	{
		ctlMem->m_chauffeur.target_player = targetId;
		printf("~r~CTL: ~w~Now following %s.", NetworkPlayerGetNameWithColour(targetId));
	}
}

void chauffeurSelectTargetPlayer()
{
	SetHeader("Chauffeur (follow player)");

	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i) || i == myPlayerIndex) 
			continue;

		AddItemWithParamAlt(GET_PLAYER_NAME(i), i, chauffeurSetTargetPlayer, "Follow this player, ~PAD_X~ = Stop.", ClearChauffeurTarget, nullptr);;
	}
}

bool RemoveChauffeurFromCurrentVehicle(int un1, int un2)
{
	if (DOES_CHAR_EXIST(ctlMem->m_chauffeur.driver))
	{
		ctlMem->m_chauffeur.target_player = -1;
		DeleteChar(&ctlMem->m_chauffeur.driver);
		if (DOES_VEHICLE_EXIST(ctlMem->m_chauffeur.vehicle))
		{
			//if (IS_CHAR_IN_CAR(myPlayerPed, ctlMem->m_chauffeur.vehicle))
			WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, ctlMem->m_chauffeur.vehicle, SEAT_DRIVER);
			ctlMem->m_chauffeur.vehicle = 0;
		}
		RefreshCurrentPage(0, 0);
		return true;
	}
	return false;
}

bool SpawnChauffeurInCurrentVehicle()
{
	VEHICLE_INDEX myVeh;

	DisbandCurrentChauffeur(0,0);
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);

	if (DOES_VEHICLE_EXIST(myVeh))
	{
		PED_INDEX driver;

		GET_DRIVER_OF_CAR(myVeh, &driver);

		//does char exist driver
		if (myPlayerPed == driver || !DOES_CHAR_EXIST(driver))
		{
			int maxPass, passAmount;

			GET_NUMBER_OF_PASSENGERS(myVeh, &passAmount);
			GET_MAXIMUM_NUMBER_OF_PASSENGERS(myVeh, &maxPass);

			if (passAmount < maxPass)
			{
				VehOptIncrementSeat(0,0);
				CreateChauffeurDriver();
				WARP_CHAR_INTO_CAR(ctlMem->m_chauffeur.driver, myVeh);
				ctlMem->m_chauffeur.vehicle = myVeh;
				printf("~r~CTL: ~w~Your ~y~chauffaur ~w~is now driving this ~y~%s.", VehiclesModelName(myVeh));
				RefreshCurrentPage(0, 0);
				return true;
			}
			else
				printf("~r~CTL: ~w~No seats available, can't create ctlMem->m_chauffeur.");
		}
		else
			printf("~r~CTL: ~w~Drivers seat must be empty or contain you.");
	}
	else
		printf("~r~CTL: ~w~Here we are again.. there's no vehicle for me to work with!");

	return false;
}

void chauffeurOptions()
{
	//need bool InvincibleChaufferAndDriver
	SetHeader("Chauffeur");
	bitSet(&ctlMem->m_bit_sets.m_player, inChauffeurSubmenu);

	AddItemChar("Driving style", false, drivingStyles, 3, &ctlMem->m_chauffeur.driving_style, nullptr, "Only effects driving to waypoint, click drive to update.");
	AddItemFloat("Drive speed", false, 5.0f, 100.0f, &ctlMem->m_chauffeur.speed, 1.0f, 1, nullptr, "Select drive again to update.");

	if(DOES_CHAR_EXIST(ctlMem->m_chauffeur.driver) && DOES_VEHICLE_EXIST(ctlMem->m_chauffeur.vehicle) && IS_CHAR_IN_CAR(myPlayerPed, ctlMem->m_chauffeur.vehicle))
		AddItemMenuAlt("Spawn chauffeur vehicle", SpawnVehicleSetup, "~PAD_X~ = Remove from current vehicle.", RemoveChauffeurFromCurrentVehicle); //~PAD_X~ = Remove chauffeur from vehicle.
	else
		AddItemMenuAlt("Spawn chauffeur vehicle", SpawnVehicleSetup, "~PAD_X~ = Spawn in current vehicle.", SpawnChauffeurInCurrentVehicle); //~PAD_X~ = Spawn inside current vehicle. //make sure we're driving or drivers is free, and that there's room for us to warp/shuffle somewhere

	AddItemAlt("Drive wander", ChaufferDriveWander, "Drive around randomly, ~PAD_X~ = Stop.", ClearChauffeurTarget, nullptr);
	AddItemAlt("Drive to waypoint", ChaufferDriveWaypoint, "You'll get there.. eventually, ~PAD_X~ = Stop.", ClearChauffeurTarget, nullptr);
	AddItem("Disband", DisbandCurrentChauffeur, "Send them on their way.");
	AddItemBoolAlt("Auto un-stuck", &ctlMem->m_bit_sets.m_vehicle, chauffeurStuckChecks, nullptr, "~PAD_X~ = Manual unstick.", ChaufferUnStick, nullptr);
	AddItemMenu("Follow player", chauffeurSelectTargetPlayer, nullptr);
}

void setRunwayPlanes()
{
	SWITCH_AMBIENT_PLANES(!bitTest(ctlMem->m_bit_sets.m_vehicle, disabledRunwayPlanes));
}


void WorldOptions()
{
	SetHeader("World");
	bitReset(&ctlMem->m_bit_sets.m_player, inChauffeurSubmenu);
	AddItemMenu("Weather/Time", WorldWeatherTimeOptions, nullptr);
	AddItemMenu("Air", WorldAirOptions, nullptr);
	AddItemMenu("Water", WorldWaterOptions, nullptr);
	AddItemMenu("Ramps", RampOptions, "Spawn ramps in front of you that delete automatically.");
	AddItemMenu("Action pads", ActionPadOptions, "Spawn a pad that acts upon nearby players.");
	AddItemMenu("Object maps", ObjectMaps, "Map mods, some converted from PC.");
	AddItemMenu("Object spawner", ObjectSpawnerMainPage, "Object Spawner made by xStormsnoutx ~BLIP_71~");
	AddItemMenu("Chauffeur", chauffeurOptions, "Spawn a driver, fuck it why not?");
	AddItemCharAlt(ctlMem->m_commonStrings[GXT_STRING_DELETE], false, delete_strings, 12, &delete_selection, callUniversalDelete, "~PAD_A~ = Universal (Scan), ~PAD_X~ = Clear area.", callLocalUniversalDelete, nullptr); //"Delete"
	AddItemRadius("Delete distance", 0, 500, &delete_distance, 10, nullptr, "Set the distance limit for deletions, 0 for no limit.");
	AddItem("Delete spawned/skippable entities", DeleteAllSpawnedEntities, "Delete everything you've spawned that's currently saved.");
	AddItemBool("Disable traffic", &ctlMem->m_bit_sets.m_vehicle, disableTrafficGenerators, WorldOptRigTrafficGens, "Disable local vehicle generators.");
	AddItemBool("Disable runway planes", &ctlMem->m_bit_sets.m_vehicle, disabledRunwayPlanes, setRunwayPlanes, nullptr);
	AddItemMenu("Radio on foot", WorldOptsRadioOnFoot, "Listen to tunes while walking.");
}

bool is_any_player_touching_object(OBJECT_INDEX object) //Untested IS_CHAR_TOUCHING_OBJECT_ON_FOOT
{
	if (!DOES_OBJECT_EXIST(object))
		return false;

	PED_INDEX playerPed;
	VEHICLE_INDEX playerVehicle;
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i) || !PLAYER_HAS_CHAR(i))
			continue;

		GET_PLAYER_CHAR(i, &playerPed);
		if (IS_CHAR_TOUCHING_OBJECT_ON_FOOT(playerPed, object) || IS_CHAR_TOUCHING_OBJECT(playerPed, object)) //this (i suspect it does vehicles aswell anyways).
			return true;

		if (IS_CHAR_IN_ANY_CAR(playerPed))
		{
			GET_CAR_CHAR_IS_USING(playerPed, &playerVehicle);
			if (IS_VEHICLE_TOUCHING_OBJECT(playerVehicle, object)) //and this
				return true;
		}
	}
	
	return false;
}

void WorldLoopedOptions()
{
	static vector3 lastChauffeurCoordinates;
	float pX, pY, pZ, pH, distance, mX, mY, mZ;
	VEHICLE_INDEX networkPlayerVehicle;

	PED_INDEX networkPlayerPed, networkPlayerVehicleDriver;
	NETWORK_INDEX networkPlayerVehicleId;

	ObjectSpawnerLogic();

	MissileLoop();

	if (DOES_VEHICLE_EXIST(ctlMem->m_chauffeur.vehicle))
	{
		if (DOES_CHAR_EXIST(ctlMem->m_chauffeur.driver))
		{
			if (ctlMem->m_chauffeur.target_player != INVALID_PLAYER_INDEX) //we're targetting a player
			{
				if (IsPlayerValid(ctlMem->m_chauffeur.target_player) && PLAYER_HAS_CHAR(ctlMem->m_chauffeur.target_player))
				{
					vector3 targetCoords;
					float targetSpeed = 15.0f;
					float distanceFromUs, theirSpeed;

					GET_PLAYER_CHAR(ctlMem->m_chauffeur.target_player, &networkPlayerPed);

					if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
					{
						uint networkPlayerVehicleModel, ourChauffeurVehicleModel;
						vector3 theirModelMin, theirModelMax, ourModelMin, ourModelMax;

						GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
						GET_CAR_MODEL(networkPlayerVehicle, &networkPlayerVehicleModel);
						GET_CAR_MODEL(ctlMem->m_chauffeur.vehicle, &ourChauffeurVehicleModel);
						GET_MODEL_DIMENSIONS(networkPlayerVehicleModel, &theirModelMin, &theirModelMax);
						GET_MODEL_DIMENSIONS(ourChauffeurVehicleModel, &ourModelMin, &ourModelMax);
						GET_CAR_COORDINATES(networkPlayerVehicle, &pX, &pY, &pZ);
						GET_CAR_COORDINATES(ctlMem->m_chauffeur.vehicle, &mX, &mY, &mZ);
						GET_DISTANCE_BETWEEN_COORDS_3D(mX, mY, mZ, pX, pY, pZ, &distanceFromUs);
						GET_CAR_SPEED(networkPlayerVehicle, &theirSpeed);

						if (distanceFromUs > 70.0f)
							targetSpeed = 100.0f;
						else
						{						
							if (distanceFromUs > (ourModelMax.y + theirModelMax.y + 40.0f)) //was 20, 10 might even be too much
								targetSpeed = (theirSpeed + 10.0f);
							else if (distanceFromUs > (ourModelMax.y + theirModelMax.y + 10.0f))
								targetSpeed = (theirSpeed + 6.0f);
							else if (distanceFromUs > (ourModelMax.y + theirModelMax.y + 5.0f))
								targetSpeed = (theirSpeed + 2.0f);
						}

						//now get offset from vehicle so we stop behind them ..
						GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(networkPlayerVehicle, 0.0f, -(ourModelMax.y + theirModelMax.y + 2.0f), 0.0f, &targetCoords.x, &targetCoords.y, &targetCoords.z);

					}
					else
					{
						GET_CHAR_COORDINATES(networkPlayerPed, &targetCoords.x, &targetCoords.y, &targetCoords.z);
						targetSpeed = ctlMem->m_chauffeur.speed;
					}

					TASK_CAR_DRIVE_TO_COORD_NOT_AGAINST_TRAFFIC(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle, targetCoords.x, targetCoords.y, targetCoords.z, targetSpeed, ctlMem->m_chauffeur.driving_style, 0, 1, 2.0f, 2.0f);
				}
				else
				{
					printf("~r~CTL: ~w~Target player for chauffeur left.");
					ctlMem->m_chauffeur.target_player = -1;
				}
			}


			//REVS SAT STILL = 0.2000 (tested in Comet and dogshit vehicle).
			if (ctlMem->m_chauffeur.grab_coords_tick < GetTickCount())
			{
				float chauffeurSpeed;

				GET_CAR_SPEED(ctlMem->m_chauffeur.vehicle, &chauffeurSpeed);

				if (chauffeurSpeed < 8.0f) //hardly moving..
				{
					//should maybe increment timer again and stop it catching if .. fuck i dunno beeeers.
					if (!IS_CAR_STOPPED_AT_TRAFFIC_LIGHTS(ctlMem->m_chauffeur.vehicle))
					{
						float tempX, tempY, tempZ, distance;

						GET_CAR_COORDINATES(ctlMem->m_chauffeur.vehicle, &tempX, &tempY, &tempZ);
						GET_DISTANCE_BETWEEN_COORDS_3D(tempX, tempY, tempZ, lastChauffeurCoordinates.x, lastChauffeurCoordinates.y, lastChauffeurCoordinates.z, &distance);

						if (distance < 1.0f && GET_VEHICLE_ENGINE_REVS(ctlMem->m_chauffeur.vehicle) > 0.20f) //not moved 1.0f in 2 seconds... and we're trying to move.. we're stuck..
						{
							ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
							printf("~r~CTL: ~w~Looks like your Chauffeur got stuck.");
							ChaufferUnStick(0,0);
						}
					}
					else
					{
						GET_CAR_COORDINATES(ctlMem->m_chauffeur.vehicle, &lastChauffeurCoordinates.x, &lastChauffeurCoordinates.y, &lastChauffeurCoordinates.z);
						//think the timer should also go in this else statement..
					}
				}

				ctlMem->m_chauffeur.grab_coords_tick = (GetTickCount() + 4000); //check every 4 seconds.
				GET_CAR_COORDINATES(ctlMem->m_chauffeur.vehicle, &lastChauffeurCoordinates.x, &lastChauffeurCoordinates.y, &lastChauffeurCoordinates.z);
			}

			if (!IS_CHAR_IN_CAR(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle)) 
				WARP_CHAR_INTO_CAR(ctlMem->m_chauffeur.driver, ctlMem->m_chauffeur.vehicle); 

			//if (IS_CHAR_IN_CAR(myPlayerPed, ctlMem->m_chauffeur.vehicle))
			//{
				//can implement distance moved checks etc
				//IS_CAR_STOPPED_AT_TRAFFIC_LIGHTS(ctlMem->m_chauffeur.vehicle)
			//}
			//else
			if (!IS_CHAR_IN_CAR(myPlayerPed, ctlMem->m_chauffeur.vehicle))
			{
				float mX, mY, mZ, cX, cY, cZ, distanceFromChauffer;

				GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
				GET_CAR_COORDINATES(ctlMem->m_chauffeur.vehicle, &cX, &cY, &cZ);
				GET_DISTANCE_BETWEEN_COORDS_3D(mX, mY, mZ, cX, cY, cZ, &distanceFromChauffer);

				if (distanceFromChauffer > 20.0f) //it spawns near us so this means we haven't entered it yet or we fucked off.
				{
					printf("~r~CTL: ~w~Chauffeur abandoned.");  //could do 2D but meh.
					SET_CHAR_CANT_BE_DRAGGED_OUT(ctlMem->m_chauffeur.driver, false);
					SET_CHAR_INVINCIBLE(ctlMem->m_chauffeur.driver, false);
					DisbandCurrentChauffeur(0,0);
				}
			}
		}
		else
		{
			if (ctlMem->m_chauffeur.driver != 0)
			{
				printf("~r~CTL: ~w~Something happened to your Chauffeur!");
				DisbandCurrentChauffeur(0,0);
			}
		}
	}

	//SECURITY CHECK
#ifdef Security
	if (!bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SECURITY_BIT_MACHINE_ID_MATCHED))
	{
#ifdef Developer
		PRINTSTRING("ADMIN MACHINE ID SECURITY BIT FAILED IN WORLD_OPTIONS.C\n");
		WAIT(100);
#endif
		freeMem++; //cause a memory mis-alignmend / crash.
	}
#endif

	if (bitTest(ctlMem->m_bit_sets.m_player, syncTimeToRealWorld)) //&& bitTest(*(int*)stickyBitSet, STICK_XEX_PRESENT) //shouldn't be able to enable this without it.
	{
		CTL_GET_LOCAL_TIME(&time_weather_data.m_timeHour, &time_weather_data.m_timeMinute);
		SET_TIME_OF_DAY(time_weather_data.m_timeHour, time_weather_data.m_timeMinute);
	}

	if (bitTest(ctlMem->m_bit_sets.m_player, freezeTime))
		SET_TIME_OF_DAY(time_weather_data.m_timeHour, time_weather_data.m_timeMinute);

	//if (time_weather_data.m_weather != 0)
	if (bitTest(ctlMem->m_bit_sets.m_player, freezeWeather))
		FORCE_WEATHER_NOW(time_weather_data.m_weather); 

	for (int z = 0; z < MAX_TIMED_RAMPS; z++)
	{
		if (freeMem->m_timed_ramps[z].m_rampTick < GetTickCount() && freeMem->m_timed_ramps[z].m_rampTick != -1) //tbye byes time for rampy. //-1 = disabled / not locked net control.
		{
			for (int i = 2; i > -1; i--)
			{
				//TEST is_any_player_touching_object stability. //doesn't detect feet or wheels how fucking fantastic.
				if (DOES_OBJECT_EXIST(freeMem->m_timed_ramps[z].m_ramps[i]) && !is_any_player_touching_object(freeMem->m_timed_ramps[z].m_ramps[i]))
				{
					DeleteObject(&freeMem->m_timed_ramps[z].m_ramps[i]);
				}
			}
		}
	}

	if (bitTest(ctlMem->m_bit_sets.m_vehicle, disableTrafficGenerators)) //TODO NATH: Probably don't need to loop this..
	{
		SET_CAR_DENSITY_MULTIPLIER(0.0f);
		SET_RANDOM_CAR_DENSITY_MULTIPLIER(0.0f);
	}

	if (NetworkGame)
	{
		int rand;

		for (int i = 0; i < MAX_ACTIONPADS; i++)
		{
			if (!DOES_OBJECT_EXIST(freeMem->m_action_pad[i].m_padObject)) 
				continue;
			
			int timer;
			GET_GAME_TIMER(&timer);
			SET_RANDOM_SEED((uint)timer);
			GENERATE_RANDOM_INT(&rand);

			if (freeMem->m_action_pad[i].m_currentMode == PAD_MODE_SPAWN_VEHICLE_NORMAL)
			{
				if (freeMem->m_action_pad[i].m_modeData) //vehicle model has been set and stored here if it holds a value other than 0
				{
					vector3 vehModelMin, vehModelMax;

					GET_MODEL_DIMENSIONS(freeMem->m_action_pad[i].m_modeData, &vehModelMin, &vehModelMax);

					if (DOES_VEHICLE_EXIST(freeMem->m_action_pad[i].m_generatedVeh))
					{
						PED_INDEX driver;

						//could leave vehicle migratable... and 
						//SetVehicleEngineRevs(myActionPads[i].padGeneratedVeh, 8000.0f);

						GET_CAR_COORDINATES(freeMem->m_action_pad[i].m_generatedVeh, &pX, &pY, &pZ);
						GET_DRIVER_OF_CAR(freeMem->m_action_pad[i].m_generatedVeh, &driver);
						GET_DISTANCE_BETWEEN_COORDS_3D(pX, pY, pZ, freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z, &distance);
						//maybe checking if char is entering car would allow it to migrate earlier??
						PED_INDEX tmp_netPed;
						VEHICLE_INDEX tmp_vehWouldEnter;
						bool playerMayEnterVehicle;
						for (int j = 0; j < 16; j++)
						{
							//if (!IsPlayerValid(j) || j == myPlayerIndex)
							if (!IsPlayerValid(j)) //testing
								continue;
							if (!PLAYER_HAS_CHAR(j))
								continue;
							GET_PLAYER_CHAR(j, &tmp_netPed);
							if (!DOES_CHAR_EXIST(tmp_netPed))
								continue;
							if (IS_CHAR_GETTING_IN_TO_A_CAR(tmp_netPed))
							{
								GET_VEHICLE_PLAYER_WOULD_ENTER(i, &tmp_vehWouldEnter); //add distance checks on this if they somehow spoof/fuck with this it (I doubt it)
								playerMayEnterVehicle = (tmp_vehWouldEnter == freeMem->m_action_pad[i].m_generatedVeh);
								if (playerMayEnterVehicle)
								{
#ifdef Developer
									printf("~r~CTL DBG: ~w~playerMayEnterVehicle... %s, ~w~migrating.. ", NetworkPlayerGetNameWithColour(j));
#endif
									break;
								}
							}
						}
						if (DOES_CHAR_EXIST(driver) || playerMayEnterVehicle)
						{
							GET_NETWORK_ID_FROM_VEHICLE(freeMem->m_action_pad[i].m_generatedVeh, &freeMem->m_action_pad[i].m_generatedVehNetIndex); //I think netId potentially changes, will do this for now

							if (HAS_CONTROL_OF_NETWORK_ID(freeMem->m_action_pad[i].m_generatedVehNetIndex))
							{
								int tmp = freeMem->m_action_pad[i].m_generatedVeh;

								SET_NETWORK_ID_CAN_MIGRATE(freeMem->m_action_pad[i].m_generatedVehNetIndex, true); //enable driver to move
								MARK_CAR_AS_NO_LONGER_NEEDED(&tmp); //we need to keep the vehicle stored so this is a way around it.. //make sure this doesn't interfere with network ID if get issues
								//Seems to stop anybody except the player getting control while they're in the car! lol
							}
							if (distance > 5.5f)
							{
								freeMem->m_action_pad[i].m_generatedVeh = 0;

								if (bitTest(ctlMem->m_bit_sets.m_player, aPadNotifications))
								{
									const PLAYER_INDEX potentialPlayer = IsThisPedAPlayer(driver); //returns -1 if false, playerindex if true
									if (potentialPlayer != INVALID_PLAYER_INDEX)
										printf("~r~CTL PAD %i: %s ~w~took a generated vehicle, re-spawning.", (i + 1), NetworkPlayerGetNameWithColour(potentialPlayer));
								}

								return;
							}
						}
						else
						{
							if (distance > 2.10f)
							{
								if (HAS_CONTROL_OF_NETWORK_ID(freeMem->m_action_pad[i].m_generatedVehNetIndex)) //control is locked now
								{
									SET_CAR_COORDINATES(freeMem->m_action_pad[i].m_generatedVeh, freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z);
									GET_OBJECT_HEADING(freeMem->m_action_pad[i].m_padObject, &pH);
									SET_CAR_HEADING(freeMem->m_action_pad[i].m_generatedVeh, (pH - 180.0f));
									//vehicle moved back
								}
							}

							if (IS_CAR_DEAD(freeMem->m_action_pad[i].m_generatedVeh) || IS_CAR_ON_FIRE(freeMem->m_action_pad[i].m_generatedVeh))
							{
								ReleaseControlOfNetworkId(freeMem->m_action_pad[i].m_generatedVehNetIndex);
								DELETE_CAR(&freeMem->m_action_pad[i].m_generatedVeh); //respawn
							}


							else
							{
								//if (IS_CAR_UPSIDEDOWN(myActionPads[i].padGeneratedVeh))
								//	SET_CAR_ON_GROUND_PROPERLY(myActionPads[i].padGeneratedVeh); //i think this freezes at distance
							}
							//is upside down
							//could check heading..
						}
					}
					else
					{
						GET_OBJECT_HEADING(freeMem->m_action_pad[i].m_padObject, &pH);

						if (SpawnVehicle(freeMem->m_action_pad[i].m_modeData, freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z, (pH - 180.0f), &freeMem->m_action_pad[i].m_generatedVeh, false, true))
						{
							LOCK_CAR_DOORS(freeMem->m_action_pad[i].m_generatedVeh, DOOR_LOCK_NONE); //bypass options to spawn locked
							GET_NETWORK_ID_FROM_VEHICLE(freeMem->m_action_pad[i].m_generatedVeh, &freeMem->m_action_pad[i].m_generatedVehNetIndex); //this need to be here as it's used later.
							SET_CAR_ENGINE_ON(freeMem->m_action_pad[i].m_generatedVeh, true, true);
							continue;
						}
					}
				}
			}

			
			if (freeMem->m_action_pad[i].m_currentMode == PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED)
			{
				if (freeMem->m_action_pad[i].m_modeData) //vehicle model has been set and stored here if it holds a value other than 0
				{
					vector3 vehModelMin, vehModelMax;

					GET_MODEL_DIMENSIONS(freeMem->m_action_pad[i].m_modeData, &vehModelMin, &vehModelMax);

					if (DOES_VEHICLE_EXIST(freeMem->m_action_pad[i].m_generatedVeh))
					{
						PED_INDEX driver;

						//could leave vehicle migratable... and 
						//SetVehicleEngineRevs(myActionPads[i].padGeneratedVeh, 8000.0f);

						GET_CAR_COORDINATES(freeMem->m_action_pad[i].m_generatedVeh, &pX, &pY, &pZ);
						GET_DRIVER_OF_CAR(freeMem->m_action_pad[i].m_generatedVeh, &driver);
						GET_DISTANCE_BETWEEN_COORDS_3D(pX, pY, pZ, freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z, &distance);

						if (DOES_CHAR_EXIST(driver))
						{
							GET_NETWORK_ID_FROM_VEHICLE(freeMem->m_action_pad[i].m_generatedVeh, &freeMem->m_action_pad[i].m_generatedVehNetIndex); //I think netId potentially changes, will do this for now

							if (HAS_CONTROL_OF_NETWORK_ID(freeMem->m_action_pad[i].m_generatedVehNetIndex))
							{
#ifdef Developer
								printf("~r~CTL DBG: ~w~Driver detected, migrating.");
#endif
								int tmp = freeMem->m_action_pad[i].m_generatedVeh;

								SET_NETWORK_ID_CAN_MIGRATE(freeMem->m_action_pad[i].m_generatedVehNetIndex, true); //enable driver to move
								MARK_CAR_AS_NO_LONGER_NEEDED(&tmp); //we need to keep the vehicle stored so this is a way around it.. //make sure this doesn't interfere with network ID if get issues
								//Seems to stop anybody except the player getting control while they're in the car! lol
							}
							if (distance > 5.5f)
							{
								freeMem->m_action_pad[i].m_generatedVeh = 0;

								if (bitTest(ctlMem->m_bit_sets.m_player, aPadNotifications))
								{
									const PLAYER_INDEX potentialPlayer = IsThisPedAPlayer(driver); //returns -1 if false, playerindex if true
									if (potentialPlayer != INVALID_PLAYER_INDEX)
										printf("~r~CTL PAD %i: %s ~w~took a generated vehicle, re-spawning.", (i + 1), NetworkPlayerGetNameWithColour(potentialPlayer));
								}

								return;
							}
						}
						else
						{
							if (distance > 2.10f)
							{
								if (HAS_CONTROL_OF_NETWORK_ID(freeMem->m_action_pad[i].m_generatedVehNetIndex)) //control is locked now
								{
									SET_CAR_COORDINATES(freeMem->m_action_pad[i].m_generatedVeh, freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z);
									GET_OBJECT_HEADING(freeMem->m_action_pad[i].m_padObject, &pH);
									SET_CAR_HEADING(freeMem->m_action_pad[i].m_generatedVeh, (pH - 180.0f));
									//vehicle moved back
								}
							}

							if (IS_CAR_DEAD(freeMem->m_action_pad[i].m_generatedVeh) || IS_CAR_ON_FIRE(freeMem->m_action_pad[i].m_generatedVeh))
							{
								ReleaseControlOfNetworkId(freeMem->m_action_pad[i].m_generatedVehNetIndex);
								DELETE_CAR(&freeMem->m_action_pad[i].m_generatedVeh); //respawn
							}

							
							else
							{
								//if (IS_CAR_UPSIDEDOWN(myActionPads[i].padGeneratedVeh))
								//	SET_CAR_ON_GROUND_PROPERLY(myActionPads[i].padGeneratedVeh); //i think this freezes at distance
							} 
							//is upside down
							//could check heading..
						}
					}
					else
					{
						GET_OBJECT_HEADING(freeMem->m_action_pad[i].m_padObject, &pH);

						if (SpawnVehicle(freeMem->m_action_pad[i].m_modeData, freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z, (pH - 180.0f), &freeMem->m_action_pad[i].m_generatedVeh, false, true))
						{
							LOCK_CAR_DOORS(freeMem->m_action_pad[i].m_generatedVeh, DOOR_LOCK_NONE); //bypass options to spawn locked
							GET_NETWORK_ID_FROM_VEHICLE(freeMem->m_action_pad[i].m_generatedVeh, &freeMem->m_action_pad[i].m_generatedVehNetIndex); //this need to be here as it's used later.
							SET_CAR_ENGINE_ON(freeMem->m_action_pad[i].m_generatedVeh, true, true);
							continue;
						}
					}
				}
			}
			
			if (freeMem->m_action_pad[i].m_currentMode == PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED || freeMem->m_action_pad[i].m_currentMode == PAD_MODE_SPAWN_VEHICLE_NORMAL || freeMem->m_action_pad[i].m_currentMode == PAD_MODE_PAYNSPRAY || freeMem->m_action_pad[i].m_currentMode == PAD_MODE_SLINGSHOT)
			{
				//NEW V (we didn't have the if / check for network vehicles before)..
				if (!GetClosestNetworkPlayerVehicleToCoordinate(freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z, false, &networkPlayerVehicle))
					networkPlayerVehicle = GET_CLOSEST_CAR(freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z, 70.0f, false, 70);

				if (DOES_VEHICLE_EXIST(networkPlayerVehicle) && IS_CAR_IN_AREA_2D(networkPlayerVehicle, freeMem->m_action_pad[i].m_corner1.x, freeMem->m_action_pad[i].m_corner1.y, freeMem->m_action_pad[i].m_corner2.x, freeMem->m_action_pad[i].m_corner2.y, false))
				{
					GET_DRIVER_OF_CAR(networkPlayerVehicle, &networkPlayerPed);

					//if (DOES_CHAR_EXIST(networkPlayerPed))
					//else
					if(!DOES_CHAR_EXIST(networkPlayerPed))
					{
						if (!ShouldDeletionsSkipVehicle(networkPlayerVehicle)) //checks for PadGeneratedVeh's and mapVehs
						{
							GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);
							
							if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
								DeleteCar(&networkPlayerVehicle);
							else
								REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);

							//checking for passengers isn't reliable as IS_CAR_PASSENGER_SEAT_FREE will always return false if the vehicle doesn't have that seat. (e.g. 2 door car or bike)
						}
					}
				}

				//get closest car to padobj, if no driver or passengers, delete..
			}

			for (int R = 0; R < 16; R++)
			{
				if (!IsPlayerValid(R) || !PLAYER_HAS_CHAR(R)) 
					continue;

				GET_PLAYER_CHAR(R, &networkPlayerPed);
				GET_CHAR_COORDINATES(networkPlayerPed, &pX, &pY, &pZ);

				if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
				{
					GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
					if (!IS_CAR_IN_AREA_2D(networkPlayerVehicle, freeMem->m_action_pad[i].m_corner1.x, freeMem->m_action_pad[i].m_corner1.y, freeMem->m_action_pad[i].m_corner2.x, freeMem->m_action_pad[i].m_corner2.y, false)) 
						continue;
				}
				else
				{
					if (!IS_CHAR_IN_AREA_2D(networkPlayerPed, freeMem->m_action_pad[i].m_corner1.x, freeMem->m_action_pad[i].m_corner1.y, freeMem->m_action_pad[i].m_corner2.x, freeMem->m_action_pad[i].m_corner2.y, false)) 
						continue;
				}

				GET_DISTANCE_BETWEEN_COORDS_3D(freeMem->m_action_pad[i].m_coords.x, freeMem->m_action_pad[i].m_coords.y, freeMem->m_action_pad[i].m_coords.z, pX, pY, pZ, &distance);
				if (distance > 5.0f) 
					continue; //essentially a height check

				if (freeMem->m_action_pad[i].m_currentMode == PAD_MODE_TELEPORT_TO_SAVED_DESTINATION)
				{
					if (freeMem->m_action_pad[i].m_modeData) //Set to 1 when destination has been set
					{
						if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
						{
							GET_DRIVER_OF_CAR(networkPlayerVehicle, &networkPlayerVehicleDriver);

							if (networkPlayerPed == networkPlayerVehicleDriver) //don't run multiple times on same vehicle (attempt to reduce net control fucking up)
							{
								GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);

								if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
								{
									SET_CAR_COORDINATES(networkPlayerVehicle, freeMem->m_action_pad[i].m_teleportDest.x, freeMem->m_action_pad[i].m_teleportDest.y, freeMem->m_action_pad[i].m_teleportDest.z);
									if (bitTest(ctlMem->m_bit_sets.m_player, aPadNotifications))
										printf("~r~CTL PAD %i: ~w~Teleported %s ~w~to saved location.", (i + 1), NetworkPlayerGetNameWithColour(R));
								}
								else
									REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
							}
						}
						else
						{
							TeleportNetworkPlayerOnFoot(R, freeMem->m_action_pad[i].m_teleportDest.x, freeMem->m_action_pad[i].m_teleportDest.y, freeMem->m_action_pad[i].m_teleportDest.z, 0.0f, false);
							if (bitTest(ctlMem->m_bit_sets.m_player, aPadNotifications))
								printf("~r~CTL PAD %i: ~w~Teleported %s ~w~to saved location.", (i + 1), NetworkPlayerGetNameWithColour(R));
						}
					}
					else
						printf("~r~CTL PAD %i: ~w~Unable to teleport %s ~w~as no location is set.", (i + 1), NetworkPlayerGetNameWithColour(R));
				}

				if (freeMem->m_action_pad[i].m_currentMode == PAD_MODE_PAYNSPRAY)
				{
					if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
					{
						GET_DRIVER_OF_CAR(networkPlayerVehicle, &networkPlayerVehicleDriver);

						if (networkPlayerVehicleDriver == networkPlayerPed) //stop requesting control after releasing bc players as passengers. (clusterfuck prevention)
						{
							GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);
							if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
							{
								int Cols[4];

								for (int T = 0; T < 4; T++)
									GENERATE_RANDOM_INT_IN_RANGE(0, 133, &Cols[T]);

								CHANGE_CAR_COLOUR(networkPlayerVehicle, Cols[0], Cols[1]);
								SET_EXTRA_CAR_COLOURS(networkPlayerVehicle, Cols[2], Cols[3]);
								SET_VEHICLE_DIRT_LEVEL(networkPlayerVehicle, 0.0f);
								FIX_CAR(networkPlayerVehicle);
								SET_CAR_FORWARD_SPEED(networkPlayerVehicle, 0.0f);
								GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(freeMem->m_action_pad[i].m_padObject, rand % 2 == 1 ? -5.0f : 5.0f, -12.5f, +1.25f, &pX, &pY, &pZ); //move them out of the way..
								SET_CAR_COORDINATES_NO_OFFSET(networkPlayerVehicle, pX, pY, pZ);
								GET_OBJECT_HEADING(freeMem->m_action_pad[i].m_padObject, &pH);
								SET_CAR_HEADING(networkPlayerVehicle, pH);

								if (bitTest(ctlMem->m_bit_sets.m_player, aPadNotifications))
									printf("~r~CTL PAD %i: ~w~Re-sprayed %s's ~w~%s.", (i + 1), NetworkPlayerGetNameWithColour(R), VehiclesModelName(networkPlayerVehicle));
							}
							else
								REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
						}

					}
				}

				if (freeMem->m_action_pad[i].m_currentMode == PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED) //check if player is in a car that's not the one we've spawned, move em away (NOT ON FOOT!)
				{
					if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
					{
						if (networkPlayerVehicle != freeMem->m_action_pad[i].m_generatedVeh)
						{
							GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);

							if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
							{
								GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(freeMem->m_action_pad[i].m_padObject, ((rand % 2 == 1) ? -5.0f : 5.0f), -12.5f, +1.5f, &pX, &pY, &pZ); //move them out of the way..
								SET_CAR_COORDINATES_NO_OFFSET(networkPlayerVehicle, pX, pY, pZ); //gget tf out le way
							}
							else
								REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
						}
					}
				}

				if (freeMem->m_action_pad[i].m_currentMode == PAD_MODE_SLINGSHOT)
				{
					if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
					{
						GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
						GET_DRIVER_OF_CAR(networkPlayerVehicle, &networkPlayerVehicleDriver);

						if (networkPlayerPed == networkPlayerVehicleDriver)
						{
							GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);

							if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
							{
								float Rands[3];

								for (int T = 0; T < 3; T++)
									GENERATE_RANDOM_FLOAT_IN_RANGE(-25.0f, 25.0f, &Rands[T]);

								FIX_CAR(networkPlayerVehicle);
								APPLY_FORCE_TO_CAR(networkPlayerVehicle, true, 0.0f, 0.0f, 1000.0f, Rands[0], Rands[1], Rands[2], true, true, true, true);
								if (bitTest(ctlMem->m_bit_sets.m_player, aPadNotifications))
									printf("~r~CTL PAD %i: ~w~Slung %s's ~w~%s into outer space.", (i + 1), NetworkPlayerGetNameWithColour(R), VehiclesModelName(networkPlayerVehicle));

								freeMem->m_action_pad[i].m_playerTick[R] = (GetTickCount() + 23000); //22 seconds will test and reduce with time. //15 seems to be the time taken to hit the floor.
								MARK_CAR_AS_NO_LONGER_NEEDED(&networkPlayerVehicle); //NOTE Nath check this isn't causing players vehicles to be deleted, i recall lots of players on foot kinda begging for a vehicle lol - Nath
							}
							else
								REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId);
						}
					}
				}
			}
		}

		for (int i = 0; i < 16; i++)
		{
			if (IsPlayerValid(i))
			{
				if(IsPlayerACartelModerator(i))
					SET_BIT(&ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, (i + 16));
				if(IsPlayerACartelAdmin(i))
					SET_BIT(&ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, i);
			}
		}
	}
}

void ClearNeonCarObjects() //called when attaching neons
{
	NETWORK_INDEX netId;
	VEHICLE_INDEX veh;
	PLAYER_INDEX player = GetMenuPlayerId();

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		GET_CAR_CHAR_IS_USING(myPlayerPed, &veh);

	if (player == myPlayerIndex)
	{
		//could look at requesting network control..
		for (int i = 0; i < 3; i++)
		{
			if (DOES_OBJECT_EXIST(myNeonCarObjects[i]))
			{
				GET_NETWORK_ID_FROM_OBJECT(myNeonCarObjects[i], &netId);

				if (RequestNetworkControl(netId, "vehicle neons"))
				{
					if (bitTest(ctlMem->m_bit_sets.m_vehicle, deleteLastNeonVehObjects) || veh == myNeonCar)
						DeleteObject(&myNeonCarObjects[i]);
					else
						SET_NETWORK_ID_CAN_MIGRATE(netId, true);
				}
			}

			myNeonCarObjects[i] = 0; //doesn't exist so clear out array.
		}
	}
	else
	{
		if (bitTest(ctlMem->m_bit_sets.m_vehicle, deleteLastNeonVehObjects))
		{
			PED_INDEX playerPed;
			GET_PLAYER_CHAR(player, &playerPed);
			GET_CAR_CHAR_IS_USING(playerPed, &veh);
			DeleteObjectsOnVehicle(veh); //might need to do uint bs = DeleteObjectsOnVehicle.
		}
	}
}