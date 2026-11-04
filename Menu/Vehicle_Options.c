#include "stdafx.h"
#include "Vehicle_Options.h"
#include "MenuExecutionHandling.h"
#include "Functions.h"
#include "Player_Options.h"
#include "Network_Options.h"
#include "World_Options.h"
#include "Ped_Options.h"
#include "ObjectVehicles_Options.h"
#include "Utils.h"
#include "MenuUiHandling.h"
#include "Teleport_Options.h"
#include "Admin_Options.h"
#include "threading.h"


extern Page Container;

//float engineCruiseRevs = 0.0f;
int netVehSpeedBoostAmount = 2;
int selectedFancyColor;
float RotateVehAmount = 90.0f;
uint passengerWeaponsTick;
uint passengerArmourTick;
char* DoorLockString[4] = { "Unlocked", "Locked", "Locked from Outside", "Locked (allow break-in)" };
int vehColours[4]; //could probably byte these. //meh for now
Color customCarColor = { 0, 0, 0, 255 };
int cloneVehiclePos = 0;

PED_INDEX auto_boat_seat_reserve_ped;

//checks inside PlacePreviewVehicle function to ensure GetMenuPlayerId() is self and we're not in chauffeur submenu.
//#define dynamicDesc GetMenuPlayerId() == myPlayerIndex && bitTest(Container.BitSet, PB_DisplayVehiclePreview) && !bitTest(ctlMem->m_bit_sets.m_player,inChauffeurSubmenu) && !bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers) ?  "~PAD_A~ = Warp in, ~PAD_X~ = Place only." : nullptr
#define AddItemVehSetup(ItemText, model_hash) AddItemVehicleAlt(ItemText, model_hash, SpawnPlayerVehicle, vehicleSetupItemDescription(), PlacePreviewVehicle, nullptr);

char* descString = "~PAD_A~ = Warp in, ~PAD_X~ = Place only.";
const char* vehicleSetupItemDescription(void)
{
	return GetMenuPlayerId() == myPlayerIndex && bitTest(Container.BitSet, PB_DisplayVehiclePreview) && !bitTest(ctlMem->m_bit_sets.m_player, inChauffeurSubmenu) && !bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers) ? descString : nullptr;
}
VEHICLE_SEAT GetFreeVehicleSeat(VEHICLE_INDEX target_veh)
{
	PED_INDEX driver;
	int maxPassengers;

	if (!DOES_VEHICLE_EXIST(target_veh)) 
		return SEAT_INVALID;

	GET_DRIVER_OF_CAR(target_veh, &driver);
	if (!DOES_CHAR_EXIST(driver)) 
		return SEAT_DRIVER;

	GET_MAXIMUM_NUMBER_OF_PASSENGERS(target_veh, &maxPassengers);
	for (int i = 0; i < maxPassengers; i++)
	{
		if (IS_CAR_PASSENGER_SEAT_FREE(target_veh, i))
			return i;
	}

	return SEAT_INVALID;
}

bool is_vehicle_horn_sounding(VEHICLE_INDEX vehicle)
{
	if (!DOES_VEHICLE_EXIST(vehicle))
		return false;

	pool_s* pPool = (pool_s*)*(int*)0x82FB0700;
	vehicle_s* point = (vehicle_s*)((pPool->size * (vehicle >> 8)) + (int)pPool->pool);

	if (point == NULL)
		return false;

	//return point->nonZero_ifHornSounding != 0;
	return point->horn;
}

char* VehiclesModelName(const VEHICLE_INDEX vehicle)
{
	uint model;
	GET_CAR_MODEL(vehicle, &model);
	return (char*)GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));
}

//TODO make enum for these
int ItemSelectToDoorLock(int mode)
{
	switch (mode)
	{
	case 0: return DOOR_LOCK_UNLOCKED; //1; //Unlocked car
	case 1: return DOOR_LOCK_IMPOSSIBLE_TO_OPEN; //4; //Locked car impossible to open.
	case 2: return DOOR_LOCK_CAN_OPEN_FROM_INSIDE; //3; //Locked car but can open from inside.
	case 3: return 7; //locked car but player can break in.
	}

	return 0;
}

int DoorLockToItemSelect(int mode)
{
	switch (mode)
	{
	//case 1: return 0; //Unlocked car
	//case 4: return 1; //Locked car impossible to open.
	//case 3: return 2; //Locked car but can open from inside.
	case DOOR_LOCK_UNLOCKED: return 0; //Unlocked car
	case DOOR_LOCK_IMPOSSIBLE_TO_OPEN: return 1; //Locked car impossible to open.
	case DOOR_LOCK_CAN_OPEN_FROM_INSIDE: return 2; //Locked car but can open from inside.
	case 7: return 3; //locked car but player can break in.
	}

	return 0;
}

void VehOptCloneCar(int unused1, int unused2)
{
	uint targetVehModel;
	VEHICLE_INDEX clonedVeh;
	float pX, pY, pZ, pH;
	vector3 modelMin, modelMax;

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		GET_CAR_MODEL(GetPlayerVeh(), &targetVehModel);
		GET_MODEL_DIMENSIONS(targetVehModel, &modelMin, &modelMax);
		GET_CHAR_HEADING(myPlayerPed, &pH);

		if (cloneVehiclePos == 0) //Left
			GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, ((modelMin.x * 2) - 2.0f), 0.0f, 0.0f, &pX, &pY, &pZ);

		if (cloneVehiclePos == 1) //Right
			GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, ((modelMax.x * 2) + 2.0f), 0.0f, 0.0f, &pX, &pY, &pZ);

		if (cloneVehiclePos == 2) //Front
		{
			GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, (modelMax.x * 2), (modelMax.y * 3), 0.0f, &pX, &pY, &pZ);
			pH += 45.0f;
		}

		CloneVehicle(GetPlayerVeh(), pX, pY, pZ, pH, &clonedVeh);
	}
}

bool IsExtraBusAlreadyAttached(VEHICLE_INDEX busVehicle, VEHICLE_INDEX *attachedBus) //could improve this to check all busses and get the closest but meh..
{
	//could also scan pool instead but meh.
	VEHICLE_INDEX scanVeh;
	uint scanModel;
	float bX, bY, bZ, dist, tX, tY, tZ;

	for (int i = 0; i < 6000; i++)
	{
		if (!DOES_VEHICLE_EXIST_WITH_NETWORK_ID(i))
			continue;

		GET_VEHICLE_FROM_NETWORK_ID(i, &scanVeh);
		if (scanVeh == busVehicle)
			continue;

		if (!IS_CAR_ATTACHED(scanVeh))
			continue;

		GET_CAR_MODEL(scanVeh, &scanModel);
		if (scanModel != VEHICLE_BUS)
			continue;

		GET_CAR_COORDINATES(busVehicle, &bX, &bY, &bZ);
		GET_CAR_COORDINATES(scanVeh, &tX, &tY, &tZ);
		GET_DISTANCE_BETWEEN_COORDS_3D(bX, bY, bZ, tX, tY, tZ, &dist);

		//if (dist > 10.0f) continue; //works aight..
		if (dist > 9.0f)
			continue; //8 should be okay with attached..

		*attachedBus = scanVeh;
		return true;
	}

	return false;
}

uint GetVehPointer(VEHICLE_INDEX veh) //vehicle pool
{
	CPool *pool = *(CPool**)(0x82FB0700);
	return (uint)pool->m_firstPointer + ((veh / 0x100) * pool->m_structSize);
}

bool GetAttachedVehicleHost(VEHICLE_INDEX attachedVehicle, VEHICLE_INDEX *attachedHost)
{
	if (!DOES_VEHICLE_EXIST(attachedVehicle))
		return false;

	VEHICLE_INDEX attachedPointHost = *(VEHICLE_INDEX*)(GetVehPointer(attachedVehicle) + 0x1BC); //LOOK AT THIS LATER

	if (DOES_VEHICLE_EXIST(attachedPointHost))
	{
		*attachedHost = attachedPointHost;
		return true;
	}

	return false;
}

bool SpawnPlayerInVehicle(uint vehicleModel) //need to add invincibility etc 
{
	safeDetachLocalPlayer();
	float speed, mx, my, mz, mh, dirtLvl;
	bool driving = false, siren = false, respawning = false;
	VEHICLE_INDEX spawnedVeh, oldVeh = 0;
	bool upgrades[10];
	int cols[4];
	PED_INDEX driver;
	INTERIOR_INSTANCE_INDEX myInterior;
	GET_INTERIOR_FROM_CHAR(myPlayerPed, &myInterior);
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		GET_CAR_SPEED(GetPlayerVeh(), &speed);
		GET_DRIVER_OF_CAR(GetPlayerVeh(), &driver);

		if (driver == myPlayerPed)
		{
			uint model;

			driving = true;
			GET_CAR_CHAR_IS_USING(myPlayerPed, &oldVeh); //oldVeh = GetPlayerVeh();
			GET_CAR_MODEL(oldVeh, &model);
			respawning = (model == vehicleModel);
			GET_CHAR_COORDINATES(myPlayerPed, &mx, &my, &mz);

			if (respawning)
			{
				siren = IS_CAR_SIREN_ON(oldVeh);
				GET_CAR_COLOURS(oldVeh, &cols[0], &cols[1]);
				GET_EXTRA_CAR_COLOURS(oldVeh, &cols[2], &cols[3]);
				GET_VEHICLE_DIRT_LEVEL(oldVeh, &dirtLvl);

				for (int i = 0; i < 10; i++)
					upgrades[i] = IS_VEHICLE_EXTRA_TURNED_ON(oldVeh, i);
			}
		}
		else
		{
			vector3 ModelMin, ModelMax;

			GET_MODEL_DIMENSIONS(vehicleModel, &ModelMin, &ModelMax);
			GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(GetPlayerVeh(), -(ModelMax.x * 2), 1.5f, 0.0f, &mx, &my, &mz);
		}
	}
	else
		GET_CHAR_COORDINATES(myPlayerPed, &mx, &my, &mz);

	GET_CHAR_HEADING(myPlayerPed, &mh);

	if (driving && DOES_VEHICLE_EXIST(oldVeh)) //stop the awkward delay sometimes experienced where you're sat in old car..
		DeleteObjectsOnVehicle(oldVeh);

	if (!SpawnVehicle(vehicleModel, mx, my, mz, mh, &spawnedVeh, true, true)) //maybe false then set migratable? nah
		return false;

	SET_CAR_ON_GROUND_PROPERLY(spawnedVeh);

	if (driving)
	{
		if (DOES_VEHICLE_EXIST(oldVeh))
		{
			/*
			VEHICLE_INDEX attached_vehicle;
			if (GetAttachedVehicleHost(oldVeh, &attached_vehicle))
			{
				NETWORK_INDEX attached_vehicle_id;
				GET_NETWORK_ID_FROM_VEHICLE(attached_vehicle, &attached_vehicle_id);
				if (RequestNetworkControl(attached_vehicle_id, "attached vehicle"))
					DeleteCar(&attached_vehicle);
			}
			*/
			if (DOES_CHAR_EXIST(auto_boat_seat_reserve_ped))
			{
				SET_CHAR_VISIBLE(myPlayerPed, true);
				DeleteChar(&auto_boat_seat_reserve_ped); //should remain non-migratable.
			}
			if (DOES_VEHICLE_EXIST(ctlMem->m_autoBoat)) //should also remain non-migratable but just incase.
			{
				NETWORK_INDEX boat_network_id;
				GET_NETWORK_ID_FROM_VEHICLE(ctlMem->m_autoBoat, &boat_network_id);
				if (RequestNetworkControl(boat_network_id, "attached vehicle"))
					DeleteCar(&ctlMem->m_autoBoat);
			}
			DeleteCar(&oldVeh); //just did this originally
		}

		SET_CAR_ENGINE_ON(spawnedVeh, true, true);
		SET_CAR_FORWARD_SPEED(spawnedVeh, speed);

		if (respawning)
		{
			SWITCH_CAR_SIREN(spawnedVeh, siren);
			CHANGE_CAR_COLOUR(spawnedVeh, cols[0], cols[1]);
			SET_EXTRA_CAR_COLOURS(spawnedVeh, cols[2], cols[3]);
			SET_VEHICLE_DIRT_LEVEL(spawnedVeh, dirtLvl);

			for (int i = 0; i < 10; i++)
				TURN_OFF_VEHICLE_EXTRA(spawnedVeh, i, !upgrades[i]); //might need to be i + 1 for upgrades but not sure.. no issues yet.
		}
	}
	SET_ROOM_FOR_CAR_BY_KEY(spawnedVeh, myInterior);
	WARP_CHAR_INTO_CAR(myPlayerPed, spawnedVeh); //warping clears your interior.
	SET_ROOM_FOR_CHAR_BY_KEY(myPlayerPed, myInterior);
	clearFollowDriverData();

	return true;
	//MARK_CAR_AS_NO_LONGER_NEEDED(&spawnedVeh); //we do this when we ditch vehicle anyway so fuck it.
}

/*
void VehOptSetCruiseControl()
{
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicle, vehicleCruiseControl))
	{
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
			engineCruiseRevs = GET_VEHICLE_ENGINE_REVS(GetPlayerVeh());
	}
	else
		engineCruiseRevs = 0.0f;
}*/

void HandlingRespawn()
{
	uint model;

	GET_CAR_MODEL(GetPlayerVeh(), &model);
	if (model == 0)
	{
		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
		printf("~r~CTL: ~w~Yup, working on respawning that nothing, Doh!");
	}
	else
		SpawnPlayerInVehicle(model);

	//printf("~r~CTL: ~w~%s respawned.", VehiclesModelName(GetPlayerVeh()));
	//i think this works best, will keep all details if driving and model is same as current
	//will also drop you into a new vehicle if you're not the driver.
	/*
	Vehicle myCar, newCar;
	vector3 carCords;
	float heading;

	GET_CAR_CHAR_IS_USING(myPlayerPed, &myCar);
	GET_CAR_COORDINATES(myCar, &carCords.x, &carCords.y, &carCords.z);
	GET_CHAR_HEADING(myPlayerPed, &heading);
	CloneVehicle(myCar, carCords.x, carCords.y, carCords.z, heading, &newCar);
	DELETE_CAR(&myCar);
	WARP_CHAR_INTO_CAR(myPlayerPed, newCar);
	printf("~r~CTL: ~w~%s respawned.", VehiclesModelName(newCar));*/

}

void updateLocalVehColours(int unused1, int unused2)
{
	NETWORK_INDEX netId;

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &netId);

		if (RequestNetworkControl(netId, VehiclesModelName(GetPlayerVeh())))
		{
			CHANGE_CAR_COLOUR(GetPlayerVeh(), vehColours[0], vehColours[1]);
			SET_EXTRA_CAR_COLOURS(GetPlayerVeh(), vehColours[2], vehColours[3]);
		}
	}
}

void setVehColoursPrimary()
{
	NETWORK_INDEX netId;

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh = GetPlayerVeh();

		GET_NETWORK_ID_FROM_VEHICLE(myVeh, &netId);

		if (RequestNetworkControl(netId, VehiclesModelName(myVeh)))
		{
			int cols[2];

			GET_CAR_COLOURS(myVeh, &cols[0], &cols[1]);
			CHANGE_CAR_COLOUR(myVeh, cols[0], cols[0]);
			SET_EXTRA_CAR_COLOURS(myVeh, cols[0], cols[0]);
			RefreshCurrentPage(nullptr, nullptr);
		}
	}
}

void VehSetCustomCarColor()
{
	int colorIndex1, colorIndex2;

	if (!IS_CHAR_ON_FOOT(myPlayerPed))
	{
		VEHICLE_INDEX veh = GetPlayerVeh();

		GET_CAR_COLOURS(veh, &colorIndex1, &colorIndex2);
		SetCarColorForIndex(192, &customCarColor);
		CHANGE_CAR_COLOUR(veh, 192, colorIndex2);
	}
}

void VehOptFixInvincibility()
{
	ProcessVehicleAction(VEH_ACT_FIX_SET_INVINCIBLE, 0);
}

void VehOptFixNormal()
{
	ProcessVehicleAction(VEH_ACT_FIX, 0);
}

void clearFollowDriverData()
{
	ctlMem->m_follow_driver_data.tick = 0; //stop it running. //(ctlMem->m_follow_driver_data.tick > GetTickCount()
	ctlMem->m_follow_driver_data.exit_vehicle_input_pressed_timeout_tick = (GetTickCount() + 4500);
	ctlMem->m_follow_driver_data.SeatIndex = 1337; //invalid
}

void vehOptEjectWindscreen()
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh;
		uint model;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
		int SeatIndex = GetPedSeatIndex(myPlayerPed);
		GET_CAR_MODEL(myVeh, &model);

		if (IS_THIS_MODEL_A_CAR(model) || IS_THIS_MODEL_A_HELI(model))
		{
			if (SeatIndex == SEAT_DRIVER || SeatIndex == SEAT_PASSENGER_1)
			{
				clearFollowDriverData();
				SET_PED_FORCE_FLY_THROUGH_WINDSCREEN(myPlayerPed, true);
				return;
			}
			printf("~r~CTL: ~w~Unable to reach windscreen from current seat.");
			return;
		}
		printf("~r~CTL: ~w~Not compatible with current vehicle type.");
		return;
	}

	printf("~r~CTL: ~w~Where's your vehicle..?");
}

void vehOptEjectKnockOffBike()
{ 
	clearFollowDriverData();
	KNOCK_PED_OFF_BIKE(myPlayerPed); 
}

void vehOptEjectCurrent()
{
	float speed, pX, pY, pZ;
	uint vehModel;
	vector3 vehModelMin, vehModelMax;

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		GET_CAR_MODEL(GetPlayerVeh(), &vehModel);
		GET_MODEL_DIMENSIONS(vehModel, &vehModelMin, &vehModelMax); //pretty sure Min and Max values are the same value only Min is negative.
		GET_CAR_SPEED(GetPlayerVeh(), &speed);

		if (speed > 2.0f)
			GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(GetPlayerVeh(), vehModelMin.x, 0.0f, -0.5f, &pX, &pY, &pZ); //to left of vehicle //-0.5f on the Z axis to stop being put in teh air like.
		else
			GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(GetPlayerVeh(), 0.0f, 0.0f, vehModelMax.z, &pX, &pY, &pZ); //on roof

		WARP_CHAR_FROM_CAR_TO_COORD(myPlayerPed, pX, pY, pZ);

		clearFollowDriverData();
	}
}

void VehOptRemoveTyres() //local only
{
	OBJECT_INDEX objVeh;
	uint Model;

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh = GetPlayerVeh();
		GET_CAR_MODEL(myVeh, &Model);

		if (CreateObject(Model, 0, 0, 0, 0, &objVeh, true))
		{
			SET_CAR_VISIBLE(myVeh, false);
			SET_OBJECT_COLLISION(objVeh, false);
			ATTACH_OBJECT_TO_CAR(objVeh, myVeh, 0, 0, 0, 0, 0, 0, 0);
		}
	}
}


void VehOptDelete()
{
	ProcessVehicleAction(VEH_ACT_DELETE, 0);
}

void VehOptDeleteObjects()
{
	ProcessVehicleAction(VEH_ACT_DELETE_OBJECTS, 0);
}

void ProcessVehicleAction(eNetworkPlayerVehicleActions eAction, int unused1)
{
	PED_INDEX playerPed;
	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &playerPed))
	{
		VEHICLE_INDEX playerVehicle;
		NETWORK_INDEX playerVehicleNetId;
		GET_CAR_CHAR_IS_USING(playerPed, &playerVehicle);
		if (!DOES_VEHICLE_EXIST(playerVehicle))
		{
			printf("~r~CTL: %s ~w~is on foot.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
			return;
		}
		GET_NETWORK_ID_FROM_VEHICLE(playerVehicle, &playerVehicleNetId);
		if (eAction == VEH_ACT_BREAK_DOORS || RequestNetworkControl(playerVehicleNetId, VehiclesModelName(playerVehicle)))
		{
			switch (eAction)
			{
			case VEH_ACT_FIX:
			{
				FIX_CAR(playerVehicle);

				if (GetMenuPlayerId() == myPlayerIndex)
					printf("~r~CTL: ~w~%s fixed.", VehiclesModelName(playerVehicle));
				else
					printf("~r~CTL: ~w~Fixed %s's ~w~%s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_FIX_SET_INVINCIBLE:
			{
				FIX_CAR(playerVehicle);
				SET_CAR_CAN_BE_DAMAGED(playerVehicle, false);
				SET_CAR_CAN_BE_VISIBLY_DAMAGED(playerVehicle, false);
				SET_CAR_WATERTIGHT(playerVehicle, true);
				SET_CAR_STRONG(playerVehicle, true); //helps with doors coming off
				STOP_CAR_BREAKING(playerVehicle, true);
				printf("~r~CTL: ~w~Fixed %s's ~w~%s, and tried setting invincible.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_FLIP:
			{
				if (GetMenuPlayerId() == myPlayerIndex)
					SET_CAR_ON_GROUND_PROPERLY(playerVehicle);
				else
				{
					float pH;

					printf("~r~CTL: ~w~Flipped %s's ~w~%s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
					GET_CAR_HEADING(playerVehicle, &pH);
					SET_VEHICLE_QUATERNION(playerVehicle, 0.0f, 0.0f, 0.0f, 0.0f);
					SET_CAR_HEADING(playerVehicle, pH);
				}
			}
			break;
			case VEH_ACT_DELETE:
			{
				int DeletedObjsOnVeh = DeleteObjectsOnVehicle(playerVehicle);
				if (GetMenuPlayerId() == myPlayerIndex)
					printf("~r~CTL: ~w~%s deleted.", VehiclesModelName(playerVehicle));
				else
				{
					if (DeletedObjsOnVeh == 0)
						printf("~r~CTL: ~w~Deleted %s's ~w~%s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
					else
						printf("~r~CTL: ~w~Deleted %s's ~y~%s ~w~and ~y~%i ~w~attachments.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle), DeletedObjsOnVeh);
				}
				SET_NETWORK_ID_CAN_MIGRATE(playerVehicleNetId, true);
				DELETE_CAR(&playerVehicle);
			}
			break;
			case VEH_ACT_DELETE_OBJECTS:
			{
				if (GetMenuPlayerId() == myPlayerIndex)
					printf("~r~CTL: ~w~Deleted ~y~%i ~w~objects.", DeleteObjectsOnVehicle(playerVehicle));
				else
					printf("~r~CTL: ~w~Deleted ~y~%i ~w~objects on %s's ~y~%s.", DeleteObjectsOnVehicle(playerVehicle), NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_BURST_TYRES:
			{
				SET_CAN_BURST_CAR_TYRES(playerVehicle, true);
				for (int i = 0; i < 6; i++)  //https://gtamods.com/wiki/BURST_CAR_TYRE
					BURST_CAR_TYRE(playerVehicle, i);

				if (GetMenuPlayerId() != myPlayerIndex)
					printf("~r~CTL: ~w~Burst the tyres on %s's ~w~%s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_BREAK_WINDOWS:
			{
				for (int i = 0; i < 4; i++) //might be more than 4 of these on some models
					SMASH_CAR_WINDOW(playerVehicle, i);

				if (GetMenuPlayerId() != myPlayerIndex)
					printf("~r~CTL: ~w~Broken the windows on %s's ~w~%s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_BREAK_DOORS:
			{
				for (int i = 0; i < 4; i++)
					BREAK_CAR_DOOR(playerVehicle, i, false); //true = disappear, false = fall onto ground.

				if (GetMenuPlayerId() != myPlayerIndex)
					printf("~r~CTL: ~w~Broken the doors on %s's ~w~%s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_SPEED_BOOST:
			{
				float speed;

				GET_CAR_SPEED(playerVehicle, &speed);
				SET_CAR_FORWARD_SPEED(playerVehicle, (speed * TO_FLOAT(netVehSpeedBoostAmount)));

				if (GetMenuPlayerId() != myPlayerIndex)
					printf("~r~CTL: %s's ~w~%s is now going %ix its original speed.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle), netVehSpeedBoostAmount);
			}
			break;
			case VEH_ACT_ROTATE:
			{
				float vehHeading, speed;

				GET_CAR_SPEED(playerVehicle, &speed);
				GET_CAR_HEADING(playerVehicle, &vehHeading);
				SET_CAR_HEADING(playerVehicle, (vehHeading - RotateVehAmount));
				SET_CAR_FORWARD_SPEED(playerVehicle, speed);

				if (GetMenuPlayerId() != myPlayerIndex)
					printf("~r~CTL: %s's ~w~%s was rotated.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_EXPLODE:
			{
				SET_CAR_CAN_BE_DAMAGED(playerVehicle, true);
				SET_CAR_PROOFS(playerVehicle, false, false, false, false, false);
				EXPLODE_CAR(playerVehicle, true, false);

				if (GetMenuPlayerId() != myPlayerIndex)
					printf("~r~CTL: ~w~Exploded %s's ~w~%s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_SET_ON_FIRE:
			{
				SET_CAR_CAN_BE_DAMAGED(playerVehicle, true);
				SET_CAR_PROOFS(playerVehicle, false, false, false, false, false);
				SET_PETROL_TANK_HEALTH(playerVehicle, -1.0f);

				if (GetMenuPlayerId() != myPlayerIndex)
					printf("~r~CTL: %s's ~w~%s is now going up in flames.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(playerVehicle));
			}
			break;
			case VEH_ACT_CLEAN:
			{
				SET_VEHICLE_DIRT_LEVEL(playerVehicle, 0.0f); //don't know if this always updates for others but only one way to find out!
				WASH_VEHICLE_TEXTURES(playerVehicle, 100.0f);
			}
			break;
			}
		}
	}
}

void VehOptSpeedBoost()
{
	ProcessVehicleAction(VEH_ACT_SPEED_BOOST, 0);
}

void VehOptRotate()
{
	ProcessVehicleAction(VEH_ACT_ROTATE, 0);
}

void VehOptSlingshot(int version, int unused1)
{
	PED_INDEX targetPed;
	VEHICLE_INDEX targetVehicle;

	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &targetPed)) //is car checks etc, and prints included in function.
	{
		NETWORK_INDEX netId;
		GET_CAR_CHAR_IS_USING(targetPed, &targetVehicle);
		if (!DOES_VEHICLE_EXIST(targetVehicle))
			return;

		GET_NETWORK_ID_FROM_VEHICLE(targetVehicle, &netId);

		if (RequestNetworkControl(netId, VehiclesModelName(targetVehicle)))
		{
			if (GetMenuPlayerId() == myPlayerIndex)
				FREEZE_CAR_POSITION(targetVehicle, false);
			else
				printf("~r~CTL: ~w~Slingshott'd %s's ~w~%s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(targetVehicle));

			if(version == 1)
				APPLY_FORCE_TO_CAR(targetVehicle, true, 0.0f, 0.0f, 1000.0f, 0.0f, 5.0f, 0.0f, true, true, true, true);
			else
			{
				SET_VEHICLE_QUATERNION(targetVehicle, 0.7071, 0, 0, 0.7071); //facing directly up
				SET_CAR_FORWARD_SPEED(targetVehicle, 500.0f);
			}
		}
	}
}

void VehOptShuffleSeats()
{ 
	TASK_SHUFFLE_TO_NEXT_CAR_SEAT(myPlayerPed, GetPlayerVeh()); 
} //only shifts you left to right, not back seats.

void VehOptIncrementSeat(int unused0, int unused1) //extra logic in here means you should still be able to increment all seats even if there are empty seats..
{
	int myCurrentSeat = GetPedSeatIndex(myPlayerPed); //returns invalid if ped isn't in a vehicle.

	if (myCurrentSeat != SEAT_INVALID) //if SEAT_INVALID you're on foot.
	{
		VEHICLE_INDEX myVehicle;
		int maxPassengers, occupiedPassengerSeats;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVehicle);
		GET_MAXIMUM_NUMBER_OF_PASSENGERS(myVehicle, &maxPassengers);
		GET_NUMBER_OF_PASSENGERS(myVehicle, &occupiedPassengerSeats);

		//will this work on a bike hmmm..
		//was myCurrentSeat == SEAT_PASSENGER_3 but i think that will break on bikes..
		//-1 because seat indexes start at 0

		//basically if in last seat or driving.
		if (myCurrentSeat == (maxPassengers - 1) || myCurrentSeat == SEAT_DRIVER) //if in the last available seat //go to drivers or other available seats..
		{
			VEHICLE_SEAT freeSeat = GetFreeVehicleSeat(myVehicle); //checks drivers seat first then passenger seats incrementally.

			if (freeSeat != SEAT_INVALID) //think -1 is driver..
			{
				WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, myVehicle, freeSeat); //hopefully works with driver..
				return;
			}
		}
		else //not in drivers seat or last seat, we are either in seat 0 or 1 (passenger 1 or 2)
		{
			//allow switching seats even if they are empty..
			if (maxPassengers == 3) //is a 4 seat vehicle. (not a bike or 2 door)
			{
				for (int i = myCurrentSeat; i < maxPassengers; i++)
				{
					if (IS_CAR_PASSENGER_SEAT_FREE(myVehicle, i))
					{
						WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, myVehicle, i);
						return;
					}
				}

				for (int i = maxPassengers; i > -1; i++)
				{
					if (IS_CAR_PASSENGER_SEAT_FREE(myVehicle, i))
					{
						WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, myVehicle, i);
						return;
					}
				}
			}
		}

		printf("~r~CTL: ~w~This %s is full.", VehiclesModelName(myVehicle));
	}
	else
		printf("~r~CTL: ~w~Where am I supposed to warp you? Should I use my crystal ball to pick a random vehicle? - Nathan");
}

void vehOptToggleVisible()
{
	NETWORK_INDEX networkVehicleId;

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &networkVehicleId);
		if (RequestNetworkControl(networkVehicleId, VehiclesModelName(GetPlayerVeh())))
			SET_CAR_VISIBLE(GetPlayerVeh(), !bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleInvisibility));
	}
}

void VehOptSetScorched() //usable by local player only.
{
	PED_INDEX targetPed;
	NETWORK_INDEX NetId;
	VEHICLE_INDEX targetVehicle;

	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &targetPed))
	{
		if (IS_CHAR_IN_ANY_CAR(targetPed))
		{
			GET_CAR_CHAR_IS_USING(targetPed, &targetVehicle);
			GET_NETWORK_ID_FROM_VEHICLE(targetVehicle, &NetId);

			if (RequestNetworkControl(NetId, VehiclesModelName(targetVehicle)))
			{
				if (bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleScorch))
					SET_VEHICLE_RENDER_SCORCHED(targetVehicle, true);
				else
				{
					int Colours[4];

					SET_VEHICLE_RENDER_SCORCHED(targetVehicle, false);
					GET_CAR_COLOURS(targetVehicle, &Colours[0], &Colours[1]);
					GET_EXTRA_CAR_COLOURS(targetVehicle, &Colours[2], &Colours[3]);
					CHANGE_CAR_COLOUR(targetVehicle, Colours[0], Colours[1]);
					SET_EXTRA_CAR_COLOURS(targetVehicle, Colours[2], Colours[3]);
					FIX_CAR(targetVehicle); //stu said scorching doesn't de-set on network, this should help. - Nath.
				}
			}
		}
	}
}

void DoorLocker()
{
	PED_INDEX targetPed;
	VEHICLE_INDEX targetVehicle;

	if (SafeToActOnPlayerPed(GetMenuPlayerId(), &targetPed)) //will print if player not in a vehicle etc..
	{
		NETWORK_INDEX netId;

		GET_CAR_CHAR_IS_USING(targetPed, &targetVehicle);
		if (!DOES_VEHICLE_EXIST(targetVehicle))
			return;
		GET_NETWORK_ID_FROM_VEHICLE(targetVehicle, &netId);

		if (RequestNetworkControl(netId, VehiclesModelName(targetVehicle)))
		{
			LOCK_CAR_DOORS(targetVehicle, ItemSelectToDoorLock(ctlMem->m_my_veh_data.itemSelect));

			if (GetMenuPlayerId() == myPlayerIndex)
				printf("~r~CTL: ~w~Door lock set to %s.", DoorLockString[ctlMem->m_my_veh_data.itemSelect]);
			else
				printf("~r~CTL: %s's ~w~%s: %s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(targetVehicle), DoorLockString[ctlMem->m_my_veh_data.itemSelect]);
		}
	}
}

void updateVehicleUpgrades()
{
	NETWORK_INDEX NetId;

	if (!DOES_VEHICLE_EXIST(GetPlayerVeh())) 
		return;

	GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &NetId);

	if (RequestNetworkControl(NetId, VehiclesModelName(GetPlayerVeh())))
	{
		for (int i = 0; i < 10; i++)
			TURN_OFF_VEHICLE_EXTRA(GetPlayerVeh(), i + 1, !IS_BIT_SET(ctlMem->m_temp_vehicle_upgrades, i)); //i + 1 seems to work? :S
	}
}

void PassengerWepsCheck()
{
	if (!ctlMem->m_my_veh_data.checkGiveWepsTimerInt)
		printf("~r~CTL: ~w~Passenger weapons loop disabled.");
	else
	{
		passengerWeaponsTick = GetTickCount() + (ctlMem->m_my_veh_data.checkGiveWepsTimerInt * 1000);
		printf("~r~CTL: ~w~Passengers will receive weapons they do not posess every %i seconds.", ctlMem->m_my_veh_data.checkGiveWepsTimerInt);
	}
}

void PassengerArmourCheck()
{
	if (!ctlMem->m_my_veh_data.checkGiveArmourTimerInt)
		printf("~r~CTL: ~w~Passenger armour loop disabled.");
	else
	{
		passengerArmourTick = GetTickCount() + (ctlMem->m_my_veh_data.checkGiveArmourTimerInt * 1000);
		printf("~r~CTL: ~w~Passengers will receive armour if they're clean out every %i seconds.", ctlMem->m_my_veh_data.checkGiveArmourTimerInt);
	}
}

void SetVehicleEngineRevs(VEHICLE_INDEX vehid, float revs)
{
	*(float*)(((*(uint*)0x82FB070C * (vehid >> 8)) + *(uint*)0x82FB0700) + 0x1084) = revs;
}

uint GetVehicleAddrFromName(const char* vehicle)
{
	int vehicleCount = *(int*)0x82d1ca30;

	if (vehicleCount)
	{
		for (int i = 0; i <= vehicleCount * 0x110; i = i + 0x110)
		{
			char test[0x10];
			//char* test = BUFFER16; 
			strcpy(test, (const char*)(0x82D1CA40 + i), 16);
			if(COMPARE_STRING(vehicle, test))
				return (0x82D1CA40 + i);
		}

		return 0;
	}
	else 
		return 0;
}

void PlacePreviewVehicle(uint vehicle_model, VEHICLE_INDEX preview_handle)
{
	float pX, pY, pZ, pH;
	VEHICLE_INDEX spawned_vehicle;

	if(GetMenuPlayerId() != myPlayerIndex || !bitTest(Container.BitSet, PB_DisplayVehiclePreview) || bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers)) 
		return;

	if (DOES_VEHICLE_EXIST(preview_handle))
	{
		GET_CAR_COORDINATES(preview_handle, &pX, &pY, &pZ);
		GET_CAR_HEADING(preview_handle, &pH);

		if (!CloneVehicle(preview_handle, pX, pY, pZ, pH, &spawned_vehicle))
			printf("~r~CTL: ~w~Something went wrong, and we were unable to clone your vehicle.");
	}
}



void SpawnPlayerVehicle(uint vehicleModel, VEHICLE_INDEX preview_handle) //preview handle might not exist
{
	float pX, pY, pZ, pH;
	VEHICLE_INDEX spawned_vehicle;

#ifdef AdminOptions
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_VEHICLE_MODEL))
	{
		char* vehicleModelBuffer = BUFFER16;
		strcpy(vehicleModelBuffer, "", 16); //fill with null terminators.
		straddi(vehicleModelBuffer, vehicleModel, 16);
		strcpy(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, vehicleModelBuffer, 16);
		SEND_CLIENT_BROADCAST_VARIABLES_NOW(1);
		SendAdminCommandToPlayer(ADMIN_CMD_STRING_TO_INT_ADMIN_STRING_SPAWNPLAYERINVEHICLE_MODEL, 0);
		if(!IS_BIT_SET(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers))
			return;
	}
#endif

	if (bitTest(ctlMem->m_bit_sets.m_player, inChauffeurSubmenu))
	{
		DisbandCurrentChauffeur();
		ChaufferSpawnVehicle(vehicleModel);
		return;
	}

	if (GetMenuPlayerId() == myPlayerIndex && !bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers)) //aka in all players submenu.
	{
		//spawning for ourselves / not all players.
		//detachLocalPlayerFromVehicleOrWalker(); /no fucking idea why this crashes when called here or in player options, it doesn't elsewhere.
		if (DOES_VEHICLE_EXIST(preview_handle))
		{
			GET_CAR_COORDINATES(preview_handle, &pX, &pY, &pZ);
			GET_CAR_HEADING(preview_handle, &pH);
			INTERIOR_INSTANCE_INDEX myInterior;
			GET_INTERIOR_FROM_CHAR(myPlayerPed, &myInterior);
			if (CloneVehicle(preview_handle, pX, pY, pZ, pH, &spawned_vehicle))
			{
				SET_ROOM_FOR_CAR_BY_KEY(spawned_vehicle, myInterior);
				warp_local_player_into_vehicle(spawned_vehicle);
				SET_ROOM_FOR_CHAR_BY_KEY(myPlayerPed, myInterior);
				//TESTING ACTIONZ
			//	MARK_CAR_AS_NO_LONGER_NEEDED(&spawned_vehicle);
			}
		}
		else
			SpawnPlayerInVehicle(vehicleModel);

		return;
	}
	else
	{
#ifdef Developer
		if (IS_BIT_SET(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_VEHICLE_MODEL))
		{
			SpawnPlayerInVehicle(vehicleModel);
			return;
		}
#endif
		//if(GetMenuPlayerId() == 1337) //spawn for All Players (GetMenuPlayerId() is saved / restored by the function calling this.
		//the reason for this is SpawnVehicle takes GetMenuPlayerId().
		int startIndex = GetMenuPlayerId();
		int maxLoopAmount = (startIndex + 1);

		if (bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers))
		{
			maxLoopAmount = 16;
			startIndex = 0;
			ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
			printf("~r~CTL: ~w~Spawning all players a ~y~%s.", GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehicleModel)));
		}

		//bool spawnForAll = bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers);
		//for (; GetMenuPlayerId() < maxLoopAmount; GetMenuPlayerId()++) 
		for (int i = startIndex; i < maxLoopAmount; i++)
		{
			//probs need to check if on all players too..
			if (bitTest(ctlMem->m_bit_sets.m_player, playerNetworkAllOptsExcludeSelf) && i == myPlayerIndex)
				continue;

			if (IsPlayerValid(i) && PLAYER_HAS_CHAR(i))
			{
				float pX, pY, pZ, pH;
				PED_INDEX networkPlayerPed;
				VEHICLE_INDEX networkPlayerVehicle, spawnedVehicle;
				uint networkPlayerVehicleModel;
				vector3 newModelMin, newModelMax;

				GET_PLAYER_CHAR(i, &networkPlayerPed);
				GET_MODEL_DIMENSIONS(vehicleModel, &newModelMin, &newModelMax);

				if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
				{
					vector3 currentModelMin, currentModelMax;

					GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
					GET_CAR_MODEL(networkPlayerVehicle, &networkPlayerVehicleModel);
					GET_MODEL_DIMENSIONS(networkPlayerVehicleModel, &currentModelMin, &currentModelMax);
					GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(networkPlayerVehicle, (currentModelMax.x - newModelMax.x) + 2.5f, currentModelMax.y + (newModelMax.y * 1.5), 0.0f, &pX, &pY, &pZ);
				}
				else
					GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(networkPlayerPed, newModelMax.x, newModelMax.y * 2, 0.0f, &pX, &pY, &pZ);
				
				GET_CHAR_HEADING(networkPlayerPed, &pH);
				INTERIOR_INSTANCE_INDEX netPedsInterior;
				GET_INTERIOR_FROM_CHAR(networkPlayerPed, &netPedsInterior);
				if (!SpawnVehicle(vehicleModel, pX, pY, pZ, pH, &spawnedVehicle, true, true)) //needa check pool space in network..
					continue;

				SET_ROOM_FOR_CAR_BY_KEY(spawnedVehicle, netPedsInterior); //set to the same as the players so they can see it
				SET_CAR_HEADING(spawnedVehicle, (pH + 45.0f));
				//SET_CAR_AS_MISSION_CAR(spawnedVehicle); 
				//MARK_CAR_AS_NO_LONGER_NEEDED(&spawnedVehicle); //mark vehicle for cleanup/de-spawn
				//if (IS_THIS_MODEL_A_CAR(vehicleModel))
					//OPEN_CAR_DOOR(spawnedVehicle, 0); //doesn't sync
			}
			else
			{
				if (!bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers))
				{
					if (IsPlayerValid(i))
					{
						printf("~r~CTL: %s ~w~doesn't have a character, unable to get co-ordinates for spawning.", NetworkPlayerGetNameWithColour(i));
						continue;
					}
					else
					{
						printf("~r~CTL: ~w~Player is not present.");
						continue;
					}
				}
			}
		}
	}
}

//BACKUP
/*void SpawnPlayerVehicle(int vehicleModel)
{
	if (GetMenuPlayerId() == myPlayerIndex)
	{
		SpawnPlayerInVehicle(vehicleModel);
		return;
	}
	else
	{
		if (PLAYER_HAS_CHAR(GetMenuPlayerId()))
		{
			float pX, pY, pZ, pH;
			Ped networkPlayerPed;
			Vehicle networkPlayerVehicle, spawnedVehicle;
			int networkPlayerVehicleModel;
			GET_PLAYER_CHAR(GetMenuPlayerId(), &networkPlayerPed);
			vector3 newModelMin, newModelMax;
			GET_MODEL_DIMENSIONS(vehicleModel, &newModelMin, &newModelMax);

			if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
			{
				GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
				GET_CAR_MODEL(networkPlayerVehicle, &networkPlayerVehicleModel);
				vector3 currentModelMin, currentModelMax;

				GET_MODEL_DIMENSIONS(networkPlayerVehicleModel, &currentModelMin, &currentModelMax);
				GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(networkPlayerVehicle, ((currentModelMax.x - newModelMax.x) + 2.5f), (currentModelMax.y + (newModelMax.y * 1.5)), 0.0f, &pX, &pY, &pZ);
			}
			else
			{
				GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(networkPlayerPed, newModelMax.x, (newModelMax.y * 2), 0.0f, &pX, &pY, &pZ);
			}
			GET_CHAR_HEADING(networkPlayerPed, &pH);

			if (!SpawnVehicle(vehicleModel, pX, pY, pZ, pH, &spawnedVehicle, true,true))
				return;

			SET_CAR_HEADING(spawnedVehicle, (pH + 45.0f));

			//if (IS_THIS_MODEL_A_CAR(vehicleModel))
				//OPEN_CAR_DOOR(spawnedVehicle, 0); //doesn't sync
		}
		else
		{
			if (IsPlayerValid(GetMenuPlayerId()))
			{
				printf("~r~CTL: %s ~w~doesn't have a character, unable to get co-ordinates for spawning.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
				return;
			}
			else
			{
				printf("~r~CTL: ~w~Player is not present.");
				return;
			}
		}
	}
}*/

void VehSpawnSportsSuper()
{
	SetHeader("Sports Cars & Supercars");
	AddItemVehSetup("Banshee", VEHICLE_BANSHEE);
	AddItemVehSetup("Comet", VEHICLE_COMET);
	AddItemVehSetup("Coquette", VEHICLE_COQUETTE);
	AddItemVehSetup("Feltzer", VEHICLE_FELTZER);
	AddItemVehSetup("Infernus", VEHICLE_INFERNUS);
	AddItemVehSetup("Sultan", VEHICLE_SULTAN);
	AddItemVehSetup("Sultan RS", VEHICLE_SULTANRS);
	AddItemVehSetup("Super GT", VEHICLE_SUPERGT);
	AddItemVehSetup("Turismo", VEHICLE_TURISMO);

	if (myCurrentEpisode == 2)
	{
		AddItemVehSetup("Bullet", VEHICLE_BULLET);
		AddItemVehSetup("F620", VEHICLE_F620);
	}

}

void VehSpawnMuscleVintage()
{
	SetHeader("Muscle Cars & Vintage");
	AddItemVehSetup("Buccaneer", VEHICLE_BUCCANEER);
	AddItemVehSetup("Dukes", VEHICLE_DUKES);
	AddItemVehSetup("Faction", VEHICLE_FACTION);
	AddItemVehSetup("Manana", VEHICLE_MANANA);
	AddItemVehSetup("Peyote", VEHICLE_PEYOTE);
	AddItemVehSetup("Ruiner", VEHICLE_RUINER);
	AddItemVehSetup("Sabre", VEHICLE_SABRE);
	AddItemVehSetup("Sabre GT", VEHICLE_SABREGT);
	AddItemVehSetup("Stallion", VEHICLE_STALION);
	AddItemVehSetup("Vigero", VEHICLE_VIGERO);
	AddItemVehSetup("Virgo", VEHICLE_VIRGO);
	AddItemVehSetup("Voodoo", VEHICLE_VOODOO);

	if (myCurrentEpisode == 1 || myCurrentEpisode == 2)
		AddItemVehSetup("Tampa", VEHICLE_TAMPA);
}

void VehSpawnTwoDoorHatchback()
{
	SetHeader("Two-Doors & Hatchbacks");
	AddItemVehSetup("Blista Compact", VEHICLE_BLISTA);
	AddItemVehSetup("Fortune", VEHICLE_FORTUNE);
	AddItemVehSetup("Futo", VEHICLE_FUTO);
	AddItemVehSetup("Sentinel", VEHICLE_SENTINEL);
	AddItemVehSetup("Uranus", VEHICLE_URANUS);

	if (myCurrentEpisode == 1) //TLaD
		AddItemVehSetup("Rhapsody", VEHICLE_RHAPSODY);

	if (myCurrentEpisode == 2) //TBoGT
		AddItemVehSetup("Super Drop Diamond", VEHICLE_SUPERD2);
}

void VehSpawnFourDoorWagons()
{
	SetHeader("Four-Doors & Wagons");
	AddItemVehSetup("Admiral", VEHICLE_ADMIRAL);
	AddItemVehSetup("Chavos", VEHICLE_CHAVOS);
	AddItemVehSetup("Cognoscenti", VEHICLE_COGNOSCENTI);
	AddItemVehSetup("DF8-90", VEHICLE_DF8);
	AddItemVehSetup("Dilettante", VEHICLE_DILETTANTE);
	AddItemVehSetup("Emperor", VEHICLE_EMPEROR);
	AddItemVehSetup("Esperanto", VEHICLE_ESPERANTO);
	AddItemVehSetup("Feroci", VEHICLE_FEROCI);
	AddItemVehSetup("Hakumai", VEHICLE_HAKUMAI);
	AddItemVehSetup("Ingot", VEHICLE_INGOT);
	AddItemVehSetup("Intruder", VEHICLE_INTRUDER);
	AddItemVehSetup("Lokus", VEHICLE_LOKUS);
	AddItemVehSetup("Marbelle", VEHICLE_MARBELLA);
	AddItemVehSetup("Merit", VEHICLE_MERIT);
	AddItemVehSetup("Oracle", VEHICLE_ORACLE);
	AddItemVehSetup("Pinnacle", VEHICLE_PINNACLE);
	AddItemVehSetup("PMP 600", VEHICLE_PMP600);
	AddItemVehSetup("Premier", VEHICLE_PREMIER);
	AddItemVehSetup("Presidente", VEHICLE_PRES);
	AddItemVehSetup("Primo", VEHICLE_PRIMO);
	AddItemVehSetup("Schafter", VEHICLE_SCHAFTER);
	AddItemVehSetup("Solair", VEHICLE_SOLAIR);
	AddItemVehSetup("Stratum", VEHICLE_STRATUM);
	AddItemVehSetup("Stretch", VEHICLE_STRETCH);
	AddItemVehSetup("Vincent", VEHICLE_VINCENT);
	AddItemVehSetup("Washington", VEHICLE_WASHINGTON);
	AddItemVehSetup("Willard", VEHICLE_WILLARD);

	if (myCurrentEpisode == 2)
	{
		AddItemVehSetup("Buffalo", VEHICLE_BUFFALO);
		AddItemVehSetup("Schafter 2", VEHICLE_SCHAFTER2);
		AddItemVehSetup("Schafter 3", VEHICLE_SCHAFTER3);
		AddItemVehSetup("Stretch E", VEHICLE_LIMO2);
		AddItemVehSetup("Super Diamond", VEHICLE_SUPERD);
	}
}

void VehSpawnSuvPickupVans()
{
	SetHeader("SUVs, Pick-ups & Vans");
	AddItemVehSetup("Bobcat", VEHICLE_BOBCAT);
	AddItemVehSetup("Cavalcade", VEHICLE_CAVALCADE);
	AddItemVehSetup("Cavalcade FXT", VEHICLE_FXT);
	AddItemVehSetup("Contender", VEHICLE_E109);
	AddItemVehSetup("Habanero", VEHICLE_HABANERO);
	AddItemVehSetup("Huntley Sport", VEHICLE_HUNTLEY);
	AddItemVehSetup("Landstalker", VEHICLE_LANDSTALKER);
	AddItemVehSetup("Minivan", VEHICLE_MINIVAN);
	AddItemVehSetup("Moonbeam", VEHICLE_MOONBEAM);
	AddItemVehSetup("Patriot", VEHICLE_PATRIOT);
	AddItemVehSetup("Perennial", VEHICLE_PERENNIAL);
	AddItemVehSetup("Rancher", VEHICLE_RANCHER);
	AddItemVehSetup("Rebla", VEHICLE_REBLA);

	if (myCurrentEpisode == 1)
	{
		AddItemVehSetup("Slamvan", VEHICLE_SLAMVAN);
		AddItemVehSetup("Tow Truck", VEHICLE_TOWTRUCK);
	}

	if (myCurrentEpisode == 2)
	{
		AddItemVehSetup("Cavalcade 2", VEHICLE_CAVALCADE2);
		AddItemVehSetup("Serrano", VEHICLE_SERRANO);
		AddItemVehSetup("Serrano 2", VEHICLE_SERRANO2);
		AddItemVehSetup("Slamvan", VEHICLE_SLAMVAN);
	}
}

void VehSpawnCommercialIndustrial()
{
	SetHeader("Commercial & Industrial");
	AddItemVehSetup("Airtug", VEHICLE_AIRTUG);
	AddItemVehSetup("Benson", VEHICLE_BENSON);
	AddItemVehSetup("Biff", VEHICLE_BIFF);
	AddItemVehSetup("Boxville", VEHICLE_BOXVILLE);
	AddItemVehSetup("Burrito", VEHICLE_BURRITO);
	AddItemVehSetup("Flatbed", VEHICLE_FLATBED);
	AddItemVehSetup("FlyUS Feroci", VEHICLE_FEROCI2);
	AddItemVehSetup("FlyUS Perennial", VEHICLE_PERENNIAL2);
	AddItemVehSetup("Forklift", VEHICLE_FORKLIFT);
	AddItemVehSetup("Laundromat", VEHICLE_BURRITO2);
	AddItemVehSetup("Mule", VEHICLE_MULE);
	AddItemVehSetup("Packer", VEHICLE_PACKER);
	AddItemVehSetup("Phantom", VEHICLE_PHANTOM);
	AddItemVehSetup("Pony", VEHICLE_PONY);
	AddItemVehSetup("Ripley", VEHICLE_RIPLEY);
	AddItemVehSetup("Securicar", VEHICLE_STOCKADE);
	AddItemVehSetup("Speedo", VEHICLE_SPEEDO);
	AddItemVehSetup("Steed", VEHICLE_STEED);
	AddItemVehSetup("Yankee", VEHICLE_YANKEE);

	if (myCurrentEpisode == 1)
	{
		AddItemVehSetup("Gang Burrito", VEHICLE_GBURRITO);
		AddItemVehSetup("Packer 2", VEHICLE_PACKER2);
		AddItemVehSetup("Yankee (Open bed)", VEHICLE_YANKEE2);
	}
}

void VehSpawnEmergency()
{
	SetHeader("Emergency");
	AddItemVehSetup("Ambulance", VEHICLE_AMBULANCE);
	AddItemVehSetup("Enforcer", VEHICLE_NSTOCKADE);
	AddItemVehSetup("FIB Buffalo", VEHICLE_FBI);
	AddItemVehSetup("Fire Truck", VEHICLE_FIRETRUK);
	AddItemVehSetup("NOOSE Cruiser", VEHICLE_NOOSE);
	AddItemVehSetup("NOOSE Patriot", VEHICLE_POLPATRIOT);
	AddItemVehSetup("Police Cruiser", VEHICLE_POLICE);
	AddItemVehSetup("Police Patrol", VEHICLE_POLICE2);
	AddItemVehSetup("Police Stockade", VEHICLE_PSTOCKADE);

	if (myCurrentEpisode == 1) //TLaD
		AddItemVehSetup("Prison Bus", VEHICLE_PBUS);

	if (myCurrentEpisode == 2) //TBoGT
	{
		AddItemVehSetup("APC", VEHICLE_APC);
		AddItemVehSetup("Brickade", VEHICLE_AVAN);
		AddItemVehSetup("Police Buffalo", VEHICLE_POLICE3);
		AddItemVehSetup("Police Stinger", VEHICLE_POLICE4);
	}
}

void spawnBusWithExtraSeats(int un1, int un2)
{
	if (!IS_CHAR_ON_FOOT(myPlayerPed))
	{
		printf("~r~CTL: ~w~Do this on foot.");
		return;
	}
	int freeSpace = -1;
	for (int i = 0; i < MAX_RIGGED_BUSSES; i++)
	{
		if (DOES_VEHICLE_EXIST(ctlMem->m_CartelBusses[i].m_attachedBus) || DOES_VEHICLE_EXIST(ctlMem->m_CartelBusses[i].m_hostBus))
			continue; //already in use..

		freeSpace = i;
		break;
	}
	if (freeSpace == -1)
	{
		printf("~r~CTL: ~w~Maximum rigged busses have already been spawned.");
	}
	else
	{
		if (SpawnPlayerInVehicle(VEHICLE_BUS))
		{
			if (SpawnVehicle(VEHICLE_BUS, 0.0f, 0.0f, 0.0f, 0.0f, &ctlMem->m_CartelBusses[freeSpace].m_attachedBus, false, true)) //prot should release control, worry about this later..
			{
				//ctlMem->m_CartelBusses[freeSpace].m_hostBus = GetPlayerVeh();
				GET_CAR_CHAR_IS_USING(myPlayerPed, &ctlMem->m_CartelBusses[freeSpace].m_hostBus);
				if (NetworkGame)
				{
					GET_NETWORK_ID_FROM_VEHICLE(ctlMem->m_CartelBusses[freeSpace].m_attachedBus, &ctlMem->m_CartelBusses[freeSpace].m_attachedBus_NetId);
					SET_NETWORK_ID_CAN_MIGRATE(ctlMem->m_CartelBusses[freeSpace].m_attachedBus_NetId, false);
					GET_NETWORK_ID_FROM_VEHICLE(ctlMem->m_CartelBusses[freeSpace].m_hostBus, &ctlMem->m_CartelBusses[freeSpace].m_hostBus_NetId); //might need later who knows..
				}
				SET_CAR_CAN_BE_DAMAGED(ctlMem->m_CartelBusses[freeSpace].m_attachedBus, false);
				SET_CAR_CAN_BE_VISIBLY_DAMAGED(ctlMem->m_CartelBusses[freeSpace].m_attachedBus, false);
				SET_CAR_VISIBLE(ctlMem->m_CartelBusses[freeSpace].m_attachedBus, false);
				SET_CAR_COLLISION(ctlMem->m_CartelBusses[freeSpace].m_attachedBus, false); //okay if you run BEFORE attach //doesn't do shit
				ATTACH_CAR_TO_CAR(ctlMem->m_CartelBusses[freeSpace].m_attachedBus, ctlMem->m_CartelBusses[freeSpace].m_hostBus, 0, 0.0f, -6.0f, 0.0f, 0.0f, 0.0f, 0.0f); //6 is good for 3 rear passengers.
			}
		}
	}
}

void VehSpawnPubService()
{
	SetHeader("Public Service");
	AddItemVehSetup("Bus", VEHICLE_BUS);
	if (GetMenuPlayerId() == myPlayerIndex)
		AddItem("Bus + Extra seats", spawnBusWithExtraSeats, "Use the back door for extra seat usage.");

	AddItemVehSetup("Cabby", VEHICLE_CABBY);
	AddItemVehSetup("Mr Tasty", VEHICLE_MRTASTY);
	AddItemVehSetup("Roman's Taxi", VEHICLE_ROM);
	AddItemVehSetup("Romero", VEHICLE_ROMERO);
	//AddItemVehSetup("Subway High", VEHICLE_SUBWAY_HI);
	//AddItemVehSetup("Subway Low", VEHICLE_SUBWAY_LO);
	AddItemVehSetup("Taxi", VEHICLE_TAXI);
	AddItemVehSetup("Taxi 2", VEHICLE_TAXI2);
	AddItemVehSetup("Trashmaster", VEHICLE_TRASH);

	if (myCurrentEpisode == 2) // TBoGT
		AddItemVehSetup("Caddy", VEHICLE_CADDY);
}

void VehSpawnMotorcycles()
{
	SetHeader("Motorcycles");
	AddItemVehSetup("Faggio", VEHICLE_FAGGIO);
	AddItemVehSetup("Freeway", VEHICLE_BOBBER);
	AddItemVehSetup("Hellfury", VEHICLE_HELLFURY);
	AddItemVehSetup("NRG 900", VEHICLE_NRG900);
	AddItemVehSetup("PCJ 600", VEHICLE_PCJ);
	AddItemVehSetup("Sanchez", VEHICLE_SANCHEZ);
	AddItemVehSetup("Zombie", VEHICLE_ZOMBIEB); //7

	if (myCurrentEpisode == 1) //TLaD
	{
		AddItemVehSetup("Angel", VEHICLE_ANGEL);
		AddItemVehSetup("Bati 800", VEHICLE_BATI);
		AddItemVehSetup("Bati Custom", VEHICLE_BATI2);
		AddItemVehSetup("Daemon", VEHICLE_DAEMON);
		AddItemVehSetup("Diabolus", VEHICLE_DIABOLUS);
		AddItemVehSetup("Double T", VEHICLE_DOUBLE);
		AddItemVehSetup("Double T Custom", VEHICLE_DOUBLE2);
		AddItemVehSetup("Hakuchou", VEHICLE_HAKUCHOU);
		AddItemVehSetup("Hakuchou Custom", VEHICLE_HAKUCHOU2);
		AddItemVehSetup("Hexer", VEHICLE_HEXER);
		AddItemVehSetup("Innovation", VEHICLE_INNOVATION);
		AddItemVehSetup("Lycan", VEHICLE_LYCAN);
		AddItemVehSetup("Nightblade", VEHICLE_NIGHTBLADE);
		AddItemVehSetup("Revenant", VEHICLE_REVENANT);
		AddItemVehSetup("Wayfarer", VEHICLE_WAYFARER);
		AddItemVehSetup("Wolfsbane", VEHICLE_WOLFSBANE);
	}
	else if (myCurrentEpisode == 2) //TBoGT
	{
		AddItemVehSetup("Akuma", VEHICLE_AKUMA);
		AddItemVehSetup("Bati Custom", VEHICLE_BATI2);
		AddItemVehSetup("Double T", VEHICLE_DOUBLE);
		AddItemVehSetup("Faggio (classic)", VEHICLE_FAGGIO2);
		AddItemVehSetup("Hakuchou", VEHICLE_HAKUCHOU);
		AddItemVehSetup("Hexer", VEHICLE_HEXER);
		AddItemVehSetup("Police Bike", VEHICLE_POLICEB);
		AddItemVehSetup("Vader", VEHICLE_VADER);
	}
}

void VehSpawnBoats()
{
	SetHeader("Boats");
	AddItemVehSetup("Dinghy", VEHICLE_DINGHY);
	AddItemVehSetup("Jetmax", VEHICLE_JETMAX);
	AddItemVehSetup("Marquis", VEHICLE_MARQUIS);
	AddItemVehSetup("Police Predator", VEHICLE_PREDATOR);
	AddItemVehSetup("Reefer", VEHICLE_REEFER);
	AddItemVehSetup("Squalo", VEHICLE_SQUALO);
	AddItemVehSetup("Tropic", VEHICLE_TROPIC);
	AddItemVehSetup("Tug Boat", VEHICLE_TUGA);

	if (myCurrentEpisode == EPISODE_TBOGT) //TBoGT / 2
	{
		AddItemVehSetup("Blade", VEHICLE_BLADE);
		AddItemVehSetup("Floater", VEHICLE_FLOATER);
		AddItemVehSetup("Smuggler", VEHICLE_SMUGGLER);
	}
}

void VehSpawnAircraft()
{
	SetHeader("Aircraft");
	AddItemVehSetup("Annihilator", VEHICLE_ANNIHILATOR);
	AddItemVehSetup("Helitours Maverick", VEHICLE_TOURMAV);
	AddItemVehSetup("Maverick", VEHICLE_MAVERICK);
	AddItemVehSetup("Police Maverick", VEHICLE_POLMAV);

	if (myCurrentEpisode == 2)
	{
		AddItemVehSetup("Buzzard", VEHICLE_BUZZARD);
		AddItemVehSetup("Skylift", VEHICLE_SKYLIFT);
		AddItemVehSetup("Swift", VEHICLE_SWIFT);
	}
}

void VehSpawnPING()
{
	SetHeader("PING Vehicle Testing");
	AddItemVehSetup("ariel_atom_2016", 0x20690FA5);
	AddItemVehSetup("aston_martin_2013", 0x5164079C);
	AddItemVehSetup("audi_r8_2014", 0x24C9BFB5);
	AddItemVehSetup("bentley_gt_2014", 0x3465306B);
	AddItemVehSetup("bmw_m3_1988", 0xBA1F6974);
	AddItemVehSetup("bmw_m6_2013", 0x10D9036C);
	AddItemVehSetup("bugatti_veyron_2009", 0x8356445C);
	AddItemVehSetup("camaro_zl1_2012", 0xE8F7D6A2);
	AddItemVehSetup("challenger_1970", 0x7896CB05);
	AddItemVehSetup("challenger_srt8_2012", 0xCCD5D100);
	AddItemVehSetup("charger_rt_1969", 0x9A509C56);
	AddItemVehSetup("charger_srt_2012", 0x1FD5C22E);
	AddItemVehSetup("chrysler_300c_2012", 0x76D62B33);
	AddItemVehSetup("el_camino_ss_1970", 0xE53ADCB9);
	AddItemVehSetup("ferrari_458_2013", 0x990324BD);
	AddItemVehSetup("ferrari_f40_1987", 0xC7FFA9F9);
	AddItemVehSetup("ferrari_ff_2011", 0xB4BA58D2);
	AddItemVehSetup("ferrari_laf_2014", 0x3BCCF73D);
	AddItemVehSetup("jaguar_ftype_2014", 0xDDF57595);
	AddItemVehSetup("k5_blazer_1972", 0xF502052A);
	AddItemVehSetup("koenigsegg_rs_2017", 0xC5034D86);
	AddItemVehSetup("lamborghini_1988", 0x8CB49917);
	AddItemVehSetup("lamborghini_2012", 0xE657BFCA);
	AddItemVehSetup("lamborghini_2013", 0xC6B58086);
	AddItemVehSetup("maserati_mc_2012", 0x51A7135F);
	AddItemVehSetup("mclaren_p1_2014", 0xC1AB045D);
	AddItemVehSetup("mercedes_c63_2012", 0x8DDA8A47);
	AddItemVehSetup("mercedes_sls_2011", 0x18CB9050);
	AddItemVehSetup("mustang_gt_2015", 0xD63AC694);
	AddItemVehSetup("mustang_shelby_1967", 0x1ADF18DD);
	AddItemVehSetup("pagani_huayra_2012", 0xDC5EA7EF);
	AddItemVehSetup("pontiac_gto_1967", 0x8E528209);
	AddItemVehSetup("porsche_911_2010", 0x5270860D);
	AddItemVehSetup("porsche_993_1995", 0x849DCC12);
	AddItemVehSetup("shelby_cobra_1966", 0x9AAAC360);
	AddItemVehSetup("skyline_r34_1999", 0xD4F9C260);
	AddItemVehSetup("viper_gts_2013", 0x0059A2CD);
	AddItemVehSetup("ford_victoria_2011", 0x293B7AB1);
}


void VehSpawnJunkers()
{
	SetHeader("Junkers");
	AddItemVehSetup("Vigero 2", VEHICLE_VIGERO2);
	AddItemVehSetup("Sabre 2", VEHICLE_SABRE2);
	AddItemVehSetup("Emperor 2", VEHICLE_EMPEROR2);

	if (myCurrentEpisode == 1) //TLaD
		AddItemVehSetup("Regina", VEHICLE_REGINA);

	if (myCurrentEpisode == 2) //TBoGT
		AddItemVehSetup("Police Cruiser", VEHICLE_POLICEW);
}
/*freeMem->m_savedVehicleModel*/

void loadSavedVehicle(int un1, int un2)
{
	if (freeMem->m_savedVehicleModel != 0)
	{
		if (SpawnPlayerInVehicle(freeMem->m_savedVehicleModel))
		{
			VEHICLE_INDEX myVeh;
			GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
			CHANGE_CAR_COLOUR(myVeh, freeMem->m_savedVehicleColours[0], freeMem->m_savedVehicleColours[1]);
			SET_EXTRA_CAR_COLOURS(myVeh, freeMem->m_savedVehicleColours[2], freeMem->m_savedVehicleColours[3]);
			for (int i = 0; i < 10; i++)
			{
				TURN_OFF_VEHICLE_EXTRA(myVeh, i + 1, !IS_BIT_SET(freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i));
			}
			for (int i = 0; i < 4; i++)
			{
				if (IS_BIT_SET(freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 10))
					SMASH_CAR_WINDOW(myVeh, i);

				if (IS_BIT_SET(freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 14))
					BREAK_CAR_DOOR(myVeh, i, true);
			}
			for (int i = 0; i < 6; i++)
			{
				if (IS_BIT_SET(freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 18))
					BURST_CAR_TYRE(myVeh, i);
			}
		}
	}
	else
		printf("~r~CTL: ~w~No vehicle saved, make sure you've saved a vehicle and loaded.");
}

void saveCurrentVehicle(int un1, int un2)
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh;
		int cols[4];

		STORE_CAR_CHAR_IS_IN_NO_SAVE(myPlayerPed, &myVeh);
		GET_CAR_MODEL(myVeh, &freeMem->m_savedVehicleModel);
		GET_CAR_COLOURS(myVeh, &cols[0], &cols[1]);
		GET_EXTRA_CAR_COLOURS(myVeh, &cols[2], &cols[3]);
		for (int i = 0; i < 10; i++)
		{
			if (IS_VEHICLE_EXTRA_TURNED_ON(myVeh, i + 1))
				SET_BIT(&freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i);
			else
				CLEAR_BIT(&freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i);
		}
		for (int i = 0; i < 4; i++)
		{
			freeMem->m_savedVehicleColours[i] = cols[i];
			if(!IS_VEH_WINDOW_INTACT(myVeh,i))
				SET_BIT(&freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 10);
			else
				CLEAR_BIT(&freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 10);

			if(IS_CAR_DOOR_DAMAGED(myVeh, i))
				SET_BIT(&freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 14);
			else
				CLEAR_BIT(&freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 14);
		}
		for (int i = 0; i < 6; i++)
		{
			if (IS_CAR_TYRE_BURST(myVeh, i))
				SET_BIT(&freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 18);
			else
				CLEAR_BIT(&freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits, i + 18);
		}
		printf("~r~CTL: ~w~Current vehicle stored, don't forget to save.");
	}
}

void VehSpawnKeyboardInput()
{
	if (showAndGetKeyboardInput("Spawn Vehicle Model", false, "", 32, 0, 0.5, 0.5, 200, 0, 0))
	//if (GrabKeyboardInputString(32, 2, false, true, true, false, true))
	{
		uint vehModel = GET_HASH_KEY(ctlMem->m_char_buffers.keyboard_input_buffer);
		if (IS_MODEL_IN_CDIMAGE(vehModel))
		{
			if (IS_THIS_MODEL_A_VEHICLE(vehModel))
			{
				SpawnPlayerVehicle(vehModel, nullptr);
			}
			else
				printf("~r~CTL: ~w~Model ~y~%s ~w~is not a vehicle.", ctlMem->m_char_buffers.keyboard_input_buffer);
		}
		else
			printf("~r~CTL: ~w~Model ~y~%s ~w~is not present in CDIMAGEs.", ctlMem->m_char_buffers.keyboard_input_buffer);
	}
}

void SpawnVehicleSetup()
{
	static char* cloneVehicleString[3] = { "Left", "Right", "Front" };

	if (GetMenuPlayerId() == myPlayerIndex)
	{
		if(bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSpawnForAllPlayers))
		{ 
			SetHeader("Vehicle spawner (All players)");
			AddItemBool("Invincible spawns", &ctlMem->m_bit_sets.m_vehicle, spawnVehiclesInvincible, nullptr, "Makes Cartel-spawned vehicles immune from damage.");
		}
		else
		{
			if (!bitTest(ctlMem->m_bit_sets.m_player, inChauffeurSubmenu) && !bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_VEHICLE_MODEL)) //admin bit set is in user compile but wont ever be true or cause issues so meh.
			{
				SetHeader("Vehicle spawner");
				if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
					AddItemChar("Clone vehicle", false, cloneVehicleString, 3, &cloneVehiclePos, VehOptCloneCar, "Won't clone attached objects.");

				AddItemAlt("~PAD_X~ = Load vehicle, ~PAD_Y~ = Save vehicle.", nullptr, nullptr, loadSavedVehicle, saveCurrentVehicle);
				AddItemMenu("Object vehicles", VehSpawnObject, "Custom neons and funny cars.");
				AddItemBool("Preview before spawning", &Container.BitSet, PB_DisplayVehiclePreview, nullptr, "Preview & place vehicles.");
			}
			else
#ifdef AdminOptions
				SetHeader(IS_BIT_SET(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_VEHICLE_MODEL) ? "Spawn into vehicle" : "Chauffeur vehicle spawner");
#else
				SetHeader("Chauffeur vehicle spawner");
#endif
		}
	}
	else
#ifdef AdminOptions
	{
		if (IS_BIT_SET(ctlMem->m_bit_sets.m_admin, ADMIN_SELECTING_VEHICLE_MODEL))
			SetHeader("Spawn into vehicle");
		else
			SetHeader("Vehicle -> Spawn");
	}
#else
		SetHeader("Vehicle -> Spawn");
#endif

	AddItemMenu("Sports Cars & Supercars", VehSpawnSportsSuper, nullptr);
	AddItemMenu("Muscle Cars & Vintage", VehSpawnMuscleVintage, nullptr);
	AddItemMenu("Two-Doors & Hatchbacks", VehSpawnTwoDoorHatchback, nullptr);
	AddItemMenu("Four-Doors & Wagons", VehSpawnFourDoorWagons, nullptr);
	AddItemMenu("SUVs, Pick-ups & Vans", VehSpawnSuvPickupVans, nullptr);
	AddItemMenu("Commercial & Industrial", VehSpawnCommercialIndustrial, nullptr);
	AddItemMenu("Emergency", VehSpawnEmergency, nullptr);
	AddItemMenu("Public Service", VehSpawnPubService, nullptr);
	AddItemMenu("Motorcycles", VehSpawnMotorcycles, nullptr);

	if (!bitTest(ctlMem->m_bit_sets.m_player, inChauffeurSubmenu))
	{
		AddItemMenu("Boats", VehSpawnBoats, nullptr);
		AddItemMenu("Aircraft", VehSpawnAircraft, nullptr);
	}

	AddItemMenu("Junkers", VehSpawnJunkers, nullptr);
	AddItem("Spawn Vehicle (keyboard input)", VehSpawnKeyboardInput, nullptr);
	AddItemMenu("PING", VehSpawnPING, "Test submenu for PING");

}

void rigLocalAutoFlip(int un1, int un2)
{
	//pretty sure the menu changes the bool first and THEN calls this.
	if(bitTest(ctlMem->m_bit_sets.m_vehicle, local_autoflip))
		SET_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (myPlayerIndex + 16));
	else
		CLEAR_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (myPlayerIndex + 16));

	//printf("localAutoflip == %i, autoflipNetworkSetOurIndex == %i", bitTest(ctlMem->m_bit_sets.m_vehicle, local_autoflip), IS_BIT_SET(ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (myPlayerIndex + 16)));
}

void VehToggleSetBlockEjectHijack(int un1, int un2)
{
	DISABLE_PLAYER_AUTO_VEHICLE_EXIT(myPlayerPed, bitTest(ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack));
}

void VehicleToggles()
{
	SetHeader("Vehicle -> Toggles");
	
	AddItemBool("Flying vehicle", &ctlMem->m_bit_sets.m_vehicle, storm_flying_car, nullptr, "~PAD_A~ + ~PAD_LB~ & ~PAD_RB~, by xStormsnoutx.");
	AddItemBool("Invincible loop", &ctlMem->m_bit_sets.m_vehicle, vehicleInvincibleLoop, nullptr, "Actively set vehicles you drive invincible.");
	//MAYBE AddItemBool("Release invincibility on departure" 
	AddItemBool("Invincible spawns", &ctlMem->m_bit_sets.m_vehicle, spawnVehiclesInvincible, nullptr, "Makes Cartel-spawned vehicles immune from damage.");
	AddItemBool("Always clean", &ctlMem->m_bit_sets.m_vehicle, vehicleAlwaysClean, nullptr, "Test this syncs over network, then nullptr this info");
	AddItemBool("Spawn locked", &ctlMem->m_bit_sets.m_vehicle, spawnedVehiclesRequireBreakIn, nullptr, "Make players break-in to compatible spawned vehicles."); //our signature :D, whack it on by default (harmless)
	AddItemBool("Seat belt", &ctlMem->m_bit_sets.m_vehicle, vehicleSeatBelt, nullptr, "Prevents flying through windscreen.");
	AddItemBool("Bike seat superglue", &ctlMem->m_bit_sets.m_vehicle, bikeSeatSuperGlue, nullptr, "Prevents falling off bikes.");
	//AddItemBool("Cruise control", &ctlMem->m_bit_sets.m_vehicle, vehicleCruiseControl, VehOptSetCruiseControl, "Enable once at desired speed.");
	//AddItemBool("Auto-flip", &ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (myPlayerIndex + 16), nullptr, "Set vehicle upright when upside-down.");
	AddItemBool("Auto-flip", &ctlMem->m_bit_sets.m_vehicle, local_autoflip, rigLocalAutoFlip, "Set vehicle upright when upside-down.");
	AddItemBool("Auto-boat", &ctlMem->m_bit_sets.m_vehicle, autoSpawnBoatViceVersa, nullptr, "Transform your vehicle into a boat when in water.");
	AddItemBool("Bypass door locks", &ctlMem->m_bit_sets.m_vehicle, bypassVehicleDoorLocks, nullptr, "Act like vehicle door locks never existed.");
	AddItemBool("Block eject & hi-jack", &ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack, VehToggleSetBlockEjectHijack, "Won't block ejects caused by a vehicle respawn.");
	AddItemBool("Follow vehicle driver", &ctlMem->m_bit_sets.m_vehicle, autoWarpDriversNewVehicle, nullptr, "Auto-warp into respawned vehicles.");
}

bool get_current_vehicle_address(uint *vehicleAddr)
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myCar;
		uint model;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &myCar);
		GET_CAR_MODEL(myCar, &model);
		*vehicleAddr = GetVehicleAddrFromName(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));

		printf("%s", GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));  // Debugging vehicle name

		if (!*vehicleAddr)
			return false;  // Return false if no valid vehicle address is found

		return true;
	}
	return false;
}

bool SetCurrentVehicleHandling(float vehicleMass, float CentreOfMassX, float CentreOfMassY, float CentreOfMassZ, int percentSubmerged, float driveInertia, float driveForce, float brakeForce, float tractionCurveMax, float tractionCurveMin, float suspensionForce, float suspensionCompDamp, float suspensionReboundDamp, float suspensionUpperLimit, float suspensionLowerLimit, float suspensionRaise, float collisionDamagaMult, float weaponDamagaMult, float deformationDamageMult, float engineDamageMult) //, float seatOffsetDist)
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myCar;
		uint model;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &myCar);
		GET_CAR_MODEL(myCar, &model);
		uint vehicleAddr = GetVehicleAddrFromName(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));

		printf("%s", GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));  // Debugging vehicle name

		if (!vehicleAddr)
			return false;  // Return false if no valid vehicle address is found

		*(float*)(vehicleAddr + 0x10) = vehicleMass;  // Set mass
		*(float*)(vehicleAddr + 0x20) = CentreOfMassX;  // Set Center of Mass X
		*(float*)(vehicleAddr + 0x24) = CentreOfMassY;  // Set Center of Mass Y
		*(float*)(vehicleAddr + 0x28) = CentreOfMassZ;  // Set Center of Mass Z
		*(int*)(vehicleAddr + 0x30) = percentSubmerged;  // Set Percent Submerged
		*(float*)(vehicleAddr + 0x3C) = driveInertia;  // Set Drive Inertia
		*(float*)(vehicleAddr + 0x44) = driveForce;  // Set Drive Force
		*(float*)(vehicleAddr + 0x74) = brakeForce;  // Set Brake Force
		*(float*)(vehicleAddr + 0x8C) = tractionCurveMax;  // Set Traction Curve Max
		*(float*)(vehicleAddr + 0x94) = tractionCurveMin;  // Set Traction Curve Min
		*(float*)(vehicleAddr + 0xB4) = suspensionForce;  // Set Suspension Force
		*(float*)(vehicleAddr + 0xB8) = suspensionCompDamp;  // Set Suspension Compression Damp
		*(float*)(vehicleAddr + 0xBC) = suspensionReboundDamp;  // Set Suspension Rebound Damp
		*(float*)(vehicleAddr + 0xC0) = suspensionUpperLimit;  // Set Suspension Upper Limit
		*(float*)(vehicleAddr + 0xC4) = suspensionLowerLimit;  // Set Suspension Lower Limit
		*(float*)(vehicleAddr + 0xC8) = suspensionRaise;  // Set Suspension Raise
		*(float*)(vehicleAddr + 0xD4) = collisionDamagaMult;  // Set Collision Damage Multiplier
		*(float*)(vehicleAddr + 0xD8) = weaponDamagaMult;  // Set Weapon Damage Multiplier
		*(float*)(vehicleAddr + 0xDC) = deformationDamageMult;  // Set Deformation Damage Multiplier
		*(float*)(vehicleAddr + 0xE0) = engineDamageMult;  // Set Engine Damage Multiplier
		HandlingRespawn();

		return true;  // Success
	}

	return false;  // Player is not in a car
}

void handlingPresetCometNormal()
{
	uint vehicleAddr;
	if (get_current_vehicle_address(&vehicleAddr))
	{
		byte normalCometHandling[0x100] = {
		0x44, 0xB5, 0x40, 0x00, 0x3A, 0x2A, 0x64, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0x57, 0x0A, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x55, 0x3F, 0x96, 0x96, 0x97, 0x3E, 0xCC, 0xCC, 0xCD, 0x3F, 0xCC, 0xCC, 0xCD, 0x05, 0x00,
		0x00, 0x00, 0x3E, 0x75, 0xC2, 0x8F, 0x3F, 0x80, 0x00, 0x00, 0x42, 0x55, 0x55, 0x57, 0x42, 0x31,
		0xC7, 0x1D, 0xC0, 0x55, 0x55, 0x55, 0x40, 0x55, 0x55, 0x55, 0x40, 0x1D, 0xE2, 0x64, 0x3F, 0xE9,
		0xB1, 0xEB, 0x3F, 0xAC, 0xF4, 0x23, 0x3F, 0x80, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3E, 0xAE, 0x14, 0x7B, 0x3F, 0xA8, 0xF5, 0xC3, 0x3F, 0x2E, 0x14, 0x7A, 0x3F, 0x33,
		0x33, 0x33, 0x3F, 0x1C, 0x61, 0xAA, 0x3F, 0xD1, 0x89, 0xF2, 0x3F, 0xC0, 0x00, 0x00, 0x3F, 0x2A,
		0xAA, 0xAB, 0x3F, 0xA0, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x3E, 0x5F, 0x66, 0xF3, 0x40, 0x92,
		0xAD, 0x5D, 0x3D, 0xF5, 0xC2, 0x8F, 0x41, 0x05, 0x55, 0x56, 0x3F, 0x75, 0xC2, 0x8F, 0x3F, 0x85,
		0x1E, 0xB8, 0x40, 0x19, 0x99, 0x9A, 0x3E, 0x23, 0xD7, 0x0B, 0x3E, 0x38, 0x51, 0xEB, 0x3D, 0xCC,
		0xCC, 0xCD, 0xBE, 0x19, 0x99, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80,
		0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x19, 0x99, 0x9A, 0x3F, 0xC0,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0x60, 0x40, 0x44, 0x00, 0x10, 0x00, 0x00,
		0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x82, 0xD2, 0x74, 0x40, 0x82, 0xD2, 0x7E, 0x40, 0x82, 0xD2,
		0x8D, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};
		memcpy((uint*)(vehicleAddr + 0x10), normalCometHandling, 0x100);
		HandlingRespawn();
	}
}

void handlingPresetNormal()
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh;
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
		uint vehicleModel;
		GET_CAR_MODEL(myVeh, &vehicleModel);
		uint vehicleAddr;
		if (!get_current_vehicle_address(&vehicleAddr))
		{
			printf("~r~CTL: ~w~Unable to obtain vehicle memory address.");
			return;
		}
		switch (vehicleModel)
		{
		case VEHICLE_COMET:
		{
			/*
			byte normalCometHandlingData[0x100] = {
			0x44, 0xB5, 0x40, 0x00, 0x3A, 0x2A, 0x64, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0x57, 0x0A, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x55, 0x3F, 0x96, 0x96, 0x97, 0x3E, 0xCC, 0xCC, 0xCD, 0x3F, 0xCC, 0xCC, 0xCD, 0x05, 0x00,
			0x00, 0x00, 0x3E, 0x75, 0xC2, 0x8F, 0x3F, 0x80, 0x00, 0x00, 0x42, 0x55, 0x55, 0x57, 0x42, 0x31,
			0xC7, 0x1D, 0xC0, 0x55, 0x55, 0x55, 0x40, 0x55, 0x55, 0x55, 0x40, 0x1D, 0xE2, 0x64, 0x3F, 0xE9,
			0xB1, 0xEB, 0x3F, 0xAC, 0xF4, 0x23, 0x3F, 0x80, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x3E, 0xAE, 0x14, 0x7B, 0x3F, 0xA8, 0xF5, 0xC3, 0x3F, 0x2E, 0x14, 0x7A, 0x3F, 0x33,
			0x33, 0x33, 0x3F, 0x1C, 0x61, 0xAA, 0x3F, 0xD1, 0x89, 0xF2, 0x3F, 0xC0, 0x00, 0x00, 0x3F, 0x2A,
			0xAA, 0xAB, 0x3F, 0xA0, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x3E, 0x5F, 0x66, 0xF3, 0x40, 0x92,
			0xAD, 0x5D, 0x3D, 0xF5, 0xC2, 0x8F, 0x41, 0x05, 0x55, 0x56, 0x3F, 0x75, 0xC2, 0x8F, 0x3F, 0x85,
			0x1E, 0xB8, 0x40, 0x19, 0x99, 0x9A, 0x3E, 0x23, 0xD7, 0x0B, 0x3E, 0x38, 0x51, 0xEB, 0x3D, 0xCC,
			0xCC, 0xCD, 0xBE, 0x19, 0x99, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80,
			0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x19, 0x99, 0x9A, 0x3F, 0xC0,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0x60, 0x40, 0x44, 0x00, 0x10, 0x00, 0x00,
			0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x82, 0xD2, 0x74, 0x40, 0x82, 0xD2, 0x7E, 0x40, 0x82, 0xD2,
			0x8D, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			};
			*/
			byte normalCometHandlingData[256] = { 0x82, 0x04, 0x1D, 0x8C, 0x89, 0xB0, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x1D, 0xFF, 0xFF, 0xFF, 0xF2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x14, 0x5B, 0xE0, 0x84, 0x08, 0x00, 0x61, 0xB8, 0x80, 0x00, 0x29, 0x09, 0xC8, 0x00, 0x65, 0xDC, 0x1F, 0xBC, 0x68, 0xDC, 0x1B, 0xA4, 0x08, 0xDC, 0x27, 0xA6, 0x60, 0x00, 0x00, 0x2A, 0x4C, 0x3F, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x8B, 0xA5, 0xBF, 0x21, 0x00, 0x00, 0x00, 0xCD, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x01, 0x4D, 0xCD, 0xFF, 0x00, 0x02, 0xFF, 0x00, 0xCD, 0xCD, 0xCD, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0xCD, 0xCD, 0xDB, 0x72, 0x15, 0x00, 0xDB, 0x6D, 0xEE, 0xD0, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0xCD, 0xCD, 0x82, 0x02, 0x33, 0x98, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0x44, 0x5A, 0x1F, 0x58, 0xC4, 0x3B, 0xFB, 0x6E, 0x41, 0x0D, 0xD6, 0x55, 0x44, 0x5A, 0x1D, 0xB9, 0x44, 0x5A, 0x20, 0x0B, 0xC4, 0x3B, 0xFB, 0x85, 0x41, 0x0D, 0xD1, 0x6A, 0x44, 0x5A, 0x1E, 0x4C, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x49, 0x6C, 0x50, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0xCD, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xCD, 0xCD, 0xCD, 0x41, 0x13, 0x5F, 0x1A, 0x00, 0x00, 0x00
			};
			memcpy((uint*)(vehicleAddr + 0x10), normalCometHandlingData, 0x100);
		}
		break;
		} //end of vehicle model switch

	}
}

void handlingPresetGenericSuperSpeedBrakeMass()
{
	uint vehicleAddr;
	if (get_current_vehicle_address(&vehicleAddr))
	{
		byte speedHandlingData[0x100] = {
		0x50, 0x15, 0x02, 0xF9, 0x37, 0x27, 0xC5, 0xAC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3D, 0xCC, 0xCC, 0xCD, 0xBF, 0x19, 0x99, 0x9A,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x3E, 0xC8, 0xC8, 0xC9, 0x3F, 0x80,
		0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
		0x3D, 0xCC, 0xCC, 0xCD, 0x47, 0x02, 0x35, 0x01, 0x46, 0xD9, 0x03, 0x01, 0xC0, 0x55,
		0x55, 0x55, 0x40, 0x55, 0x55, 0x55, 0x40, 0x2E, 0x8C, 0x05, 0x40, 0x0E, 0xCF, 0xF7,
		0x3F, 0xE9, 0xB1, 0xE9, 0x3F, 0xBF, 0x34, 0xE4, 0x3F, 0x9C, 0x71, 0x70, 0x3F, 0x80,
		0x00, 0x01, 0x42, 0xC6, 0x00, 0x00, 0x3F, 0xA6, 0x66, 0x66, 0x3F, 0x33, 0x33, 0x34,
		0x42, 0xC6, 0x00, 0x00, 0x3F, 0xB2, 0xB8, 0xC2, 0x3F, 0x37, 0x58, 0xB4, 0x41, 0x40,
		0x00, 0x00, 0x3D, 0xAA, 0xAA, 0xAB, 0x41, 0x40, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00,
		0x3F, 0xB2, 0xB8, 0xC2, 0x3F, 0x37, 0x58, 0xB4, 0x3E, 0x0F, 0x5C, 0x29, 0x40, 0xE4,
		0x92, 0x49, 0x3F, 0x70, 0xA3, 0xD7, 0x3F, 0x87, 0xAE, 0x14, 0x40, 0x00, 0x00, 0x00,
		0x3E, 0x0F, 0x5C, 0x29, 0x3E, 0x23, 0xD7, 0x0B, 0x3E, 0x19, 0x99, 0x9A, 0xBE, 0x19,
		0x99, 0x9A, 0x3C, 0x23, 0xD7, 0x0A, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x38, 0x80, 0x00, 0x44, 0x08, 0x10,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0xD2, 0x74, 0x40, 0x82, 0xD2,
		0x7E, 0x40, 0x82, 0xD2, 0x8D, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
		};
		memcpy((uint*)(vehicleAddr + 0x10), speedHandlingData, 0x100);
		HandlingRespawn();
	}
}

void handlingPresetGenericDrift()
{
	uint vehicleAddr;
	if (get_current_vehicle_address(&vehicleAddr))
	{
		byte driftHandlingData[0x100] = {
			0x44, 0xFA, 0x00, 0x00, 0x3A, 0x2A, 0x64, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0x19, 0x99, 0x9A, 0xBE, 0x61, 0x47, 0xAE,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0x3F, 0xAA, 0xAA, 0xAB, 0x00, 0x00,
			0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x40, 0x26, 0x66, 0x66,
			0x40, 0xA0, 0x00, 0x00, 0x43, 0x23, 0x55, 0x56, 0x43, 0x08, 0x1C, 0x72, 0xC0, 0x55,
			0x55, 0x55, 0x40, 0x55, 0x55, 0x55, 0x40, 0x27, 0xAE, 0x40, 0x40, 0x03, 0xCC, 0x2B,
			0x3F, 0xCF, 0x2F, 0xC1, 0x3F, 0xA2, 0xD9, 0x69, 0x3F, 0x80, 0x00, 0x02, 0x00, 0x00,
			0x00, 0x00, 0x3F, 0x66, 0x66, 0x66, 0x3F, 0xD9, 0x99, 0x9A, 0x3E, 0x99, 0x99, 0x98,
			0x3F, 0x33, 0x33, 0x33, 0x3F, 0x51, 0xFF, 0x7E, 0x3F, 0x9C, 0x0A, 0x21, 0x3F, 0x51,
			0xEB, 0x85, 0x3F, 0x9C, 0x18, 0xFA, 0x3F, 0x93, 0x33, 0x33, 0xC0, 0x41, 0xF0, 0x7D,
			0x3E, 0x93, 0x72, 0x07, 0x40, 0x5E, 0x3C, 0xF9, 0x3D, 0xF5, 0xC2, 0x8F, 0x41, 0x05,
			0x55, 0x56, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00,
			0x3E, 0x99, 0x99, 0x9A, 0x3E, 0xD7, 0x0A, 0x3E, 0x3E, 0x4C, 0xCC, 0xCD, 0xBD, 0xA3,
			0xD7, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xAD, 0xB0, 0x20, 0x44, 0x00, 0x14,
			0x24, 0x70, 0xC4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x82, 0xD2, 0x74, 0x40, 0x82, 0xD2,
			0x7E, 0x40, 0x82, 0xD2, 0x8D, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00
		};
		memcpy((uint*)(vehicleAddr + 0x10), driftHandlingData, 0x100);
		HandlingRespawn();
	}
}

//deleted the below but someone said it looks perfect so pulled it back 
void handlingPresetOGDrift() { SetCurrentVehicleHandling(2000.0f, 0.0f, -0.15f, -0.22f, 75, 1.0f, 2.59f, 0.90f, 0.82f, 1.14f, 2.50f, 0.30f, 0.42f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }

void handlingPresetsGeneric()
{
	SetHeader("Vehicle -> Handling Editor (Pre-sets) -> Generic");
	AddItem("Drift", handlingPresetGenericDrift, nullptr); //Nicked from IVM
	AddItem("Super speed/brake/turn/mass", handlingPresetGenericSuperSpeedBrakeMass, nullptr); //Nicked from IVM
}

void handlingPresetsComet()
{
	SetHeader("Vehicle -> Handling Editor (Pre-sets) -> Comet");
	AddItem("Nada", nullptr, "Dump some handling linez yo.");
}

void printCurHandlingAddress()
{
	uint vehicleAddr;
	if (get_current_vehicle_address(&vehicleAddr))
	{
		printf("current vehicle handling addr %x", (vehicleAddr + 0x10));
	}
}

void HandlingEditorPresets()
{
	SetHeader("Vehicle -> Handling Editor (Pre-sets)");
	AddItemAlt("Standard", handlingPresetNormal, "Set vehicle to standard handling, ~PAD_X~ = Respawn.", HandlingRespawn, nullptr);
	AddItemMenu("Generic", handlingPresetsGeneric, "Use on any car I guess.");
	AddItemMenu("Comet", handlingPresetsComet, nullptr);
#ifdef Developer
	AddItem("Print curVehicle handling address", printCurHandlingAddress, nullptr);
#endif
}

void HandlingEditorManual()
{
	SetHeader("Vehicle -> Handling Editor (Manual)");
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myCar;
		uint model;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &myCar);
		GET_CAR_MODEL(myCar, &model);
		uint vehicleAddr = GetVehicleAddrFromName(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model)); //gonna have to make a list
		printf("%s", GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));//for testing

		if (!vehicleAddr)
			return;

		//NOTE surely this isn't as complicated as it's being made out to be.
		AddItemFloat("Mass", false, 0.0, 9999999.0, (float*)(vehicleAddr + 0x10), 1000, 0, nullptr, nullptr);
		AddItemFloat("CentreOfMass X", false, -10.0, 10.0, (float*)(vehicleAddr + 0x20), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("CentreOfMass Y", false, -10.0, 10.0, (float*)(vehicleAddr + 0x24), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("CentreOfMass Z", false, -10.0, 10.0, (float*)(vehicleAddr + 0x28), 0.01, 2.0, nullptr, nullptr);
		AddItemInt("Percent Submerged", false, -120.0, 120.0, (int*)(vehicleAddr + 0x30), 1, nullptr, nullptr);
		AddItemFloat("Drive Inertia", false, 0.0, 10.0, (float*)(vehicleAddr + 0x3C), 0.1, 1.0, nullptr, nullptr);
		//AddItemInt("Drive Gears", false, 0, 1000, (int*)(vehicleAddr + 0x40), nullptr, nullptr);
		AddItemFloat("Drive Force", false, 0.0, 9999.0, (float*)(vehicleAddr + 0x44), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("Brake Force", false, 0.0, 9999.0, (float*)(vehicleAddr + 0x74), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("Traction Curve Max", false, 0.0, 9999.0, (float*)(vehicleAddr + 0x8C), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("Traction Curve Min", false, 0.0, 9999.0, (float*)(vehicleAddr + 0x94), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("Suspension Force", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xB4), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("Suspension Comp Damp", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xB8), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("Suspension Rebound Damp", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xBC), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("Suspension Upper Limit", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xC0), 0.1, 1.0, nullptr, nullptr);
		AddItemFloat("Suspension Lower Limit", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xC4), 0.1, 1.0, nullptr, nullptr);
		AddItemFloat("Suspension Raise", false, -10.0, 10.0, (float*)(vehicleAddr + 0xC8), 0.01, 2.0, nullptr, nullptr);
		AddItemFloat("Collision Damage Mult", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xD4), 0.1, 1.0, nullptr, nullptr);
		AddItemFloat("Weapon Damage Mult", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xD8), 1, 0.0, nullptr, nullptr);
		AddItemFloat("Deformation Damage Mult", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xDC), 1.0, 0.0, nullptr, nullptr);
		AddItemFloat("Engine Damage Mult", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xE0), 1.0, 0.0, nullptr, nullptr);
		AddItemFloat("Seat Offset Dist", false, 0.0, 9999.0, (float*)(vehicleAddr + 0xE4), 1.0, 0.0, nullptr, nullptr);
		AddItem("Respawn Vehicle", HandlingRespawn, "Respawn vehicle to update editing.");
	}
	else
		AddItem("You are not in a vehicle.", nullptr, "Reload this page when you are.");
}

void HandlingEditor()
{
	SetHeader("Vehicle -> Handling Editor");
	AddItemMenu("Pre-sets", HandlingEditorPresets, nullptr);
	AddItemMenu("Manual", HandlingEditorManual, nullptr);
}

void VehCustomColor()
{
	SetHeader("Custom car colour");
	AddItemInt("Red", false, 0, 255, &customCarColor.r, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Green", false, 0, 255, &customCarColor.g, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Blue", false, 0, 255, &customCarColor.b, 1, nullptr, "RGB Values: 0 - 255");
	AddItem("Set colour", VehSetCustomCarColor, nullptr);
}

void applyFancyColor(int unused1, int unused2)
{
	switch (selectedFancyColor)
	{
	case 1:
	{
		vehColours[0] = 74;
		vehColours[1] = 0;
		vehColours[2] = 131;
		vehColours[3] = 74;
		break;
	}
	case 2:
	{
		vehColours[0] = 0;
		vehColours[1] = 51;
		vehColours[2] = 51;
		vehColours[3] = 0;
		break;
	}
	case 3:
	{
		vehColours[0] = 131;
		vehColours[1] = 56;
		vehColours[2] = 0;
		vehColours[3] = 89;
		break;
	}
	case 4:
	{
		vehColours[0] = 56;
		vehColours[1] = 0;
		vehColours[2] = 101;
		vehColours[3] = 59;
		break;
	}
	case 5:
	{
		vehColours[0] = 0;
		vehColours[1] = 125;
		vehColours[2] = 91;
		vehColours[3] = 0;
		break;
	}
	case 6:
	{
		vehColours[0] = 125;
		vehColours[1] = 108;
		vehColours[2] = 0;
		vehColours[3] = 101;
		break;
	}
	case 7:
	{
		vehColours[0] = 113;
		vehColours[1] = 0;
		vehColours[2] = 35;
		vehColours[3] = 118;
		break;
	}
	case 8:
	{
		vehColours[0] = 0;
		vehColours[1] = 44;
		vehColours[2] = 118;
		vehColours[3] = 0;
		break;
	}
	case 9:
	{
		vehColours[0] = 51;
		vehColours[1] = 126;
		vehColours[2] = 0;
		vehColours[3] = 125;
		break;
	}
	case 10:
	{
		vehColours[0] = 127;
		vehColours[1] = 0;
		vehColours[2] = 45;
		vehColours[3] = 131;
		break;
	}
	case 11:
	{
		vehColours[0] = 0;
		vehColours[1] = 113;
		vehColours[2] = 132;
		vehColours[3] = 0;
		break;
	}
	case 12:
	{
		vehColours[0] = 132;
		vehColours[1] = 127;
		vehColours[2] = 0;
		vehColours[3] = 45;
		break;
	}
	case 13:
	{
		vehColours[0] = 90;
		vehColours[1] = 0;
		vehColours[2] = 132;
		vehColours[3] = 85;
		break;
	}
	case 14:
	{
		vehColours[0] = 0;
		vehColours[1] = 132;
		vehColours[2] = 84;
		vehColours[3] = 0;
		break;
	}
	case 15:
	{
		vehColours[0] = 28;
		vehColours[1] = 83;
		vehColours[2] = 0;
		vehColours[3] = 132;
		break;
	}
	case 16:
	{
		vehColours[0] = 60;
		vehColours[1] = 0;
		vehColours[2] = 89;
		vehColours[3] = 36;
		break;
	}
	case 17:
	{
		vehColours[0] = 0;
		vehColours[1] = 118;
		vehColours[2] = 27;
		vehColours[3] = 0;
		break;
	}
	case 18:
	{
		vehColours[0] = 128;
		vehColours[1] = 106;
		vehColours[2] = 0;
		vehColours[3] = 131;
		break;
	}
	case 19:
	{
		vehColours[0] = 89;
		vehColours[1] = 0;
		vehColours[2] = 35;
		vehColours[3] = 0;
		break;
	}
	case 20:
	{
		vehColours[0] = 0;
		vehColours[1] = 132;
		vehColours[2] = 118;
		vehColours[3] = 0;
		break;
	}
	case 21:
	{
		vehColours[0] = 29;
		vehColours[1] = 125;
		vehColours[2] = 0;
		vehColours[3] = 51;
		break;
	}
	case 22:
	{
		vehColours[0] = 133;
		vehColours[1] = 0;
		vehColours[2] = 89;
		vehColours[3] = 89;
		break;
	}
	case 23:
	{
		vehColours[0] = 0;
		vehColours[1] = 101;
		vehColours[2] = 101;
		vehColours[3] = 0;
		break;
	}
	case 24:
	{
		vehColours[0] = 124;
		vehColours[1] = 126;
		vehColours[2] = 27;
		vehColours[3] = 127;
		break;
	}
	case 25:
	{
		vehColours[0] = 63;
		vehColours[1] = 27;
		vehColours[2] = 127;
		vehColours[3] = 63;
		break;
	}
	case 26:
	{
		vehColours[0] = 27;
		vehColours[1] = 128;
		vehColours[2] = 89;
		vehColours[3] = 51;
		break;
	}
	case 27:
	{
		vehColours[0] = 89;
		vehColours[1] = 125;
		vehColours[2] = 51;
		vehColours[3] = 99;
		break;
	}
	case 28:
	{
		vehColours[0] = 132;
		vehColours[1] = 51;
		vehColours[2] = 118;
		vehColours[3] = 35;
		break;
	}
	case 29:
	{
		vehColours[0] = 51;
		vehColours[1] = 133;
		vehColours[2] = 89;
		vehColours[3] = 63;
		break;
	}
	case 30:
	{
		vehColours[0] = 132;
		vehColours[1] = 131;
		vehColours[2] = 63;
		vehColours[3] = 27;
		break;
	}
	case 31:
	{
		vehColours[0] = 125;
		vehColours[1] = 63;
		vehColours[2] = 51;
		vehColours[3] = 35;
		break;
	}
	case 32:
	{
		vehColours[0] = 63;
		vehColours[1] = 101;
		vehColours[2] = 35;
		vehColours[3] = 63;
		break;
	}
	case 33:
	{
		vehColours[0] = 132;
		vehColours[1] = 125;
		vehColours[2] = 89;
		vehColours[3] = 133;
		break;
	}
	case 34:
	{
		vehColours[0] = 125;
		vehColours[1] = 89;
		vehColours[2] = 124;
		vehColours[3] = 124;
		break;
	}
	case 35:
	{
		vehColours[0] = 89;
		vehColours[1] = 118;
		vehColours[2] = 124;
		vehColours[3] = 89;
		break;
	}
	case 36:
	{
		vehColours[0] = 58;
		vehColours[1] = 99;
		vehColours[2] = 89;
		vehColours[3] = 35;
		break;
	}
	case 37:
	{
		vehColours[0] = 85;
		vehColours[1] = 89;
		vehColours[2] = 0;
		vehColours[3] = 132;
		break;
	}
	case 38:
	{
		vehColours[0] = 101;
		vehColours[1] = 0;
		vehColours[2] = 69;
		vehColours[3] = 101;
		break;
	}
	case 39:
	{
		vehColours[0] = 131;
		vehColours[1] = 118;
		vehColours[2] = 101;
		vehColours[3] = 128;
		break;
	}
	case 40:
	{
		vehColours[0] = 118;
		vehColours[1] = 101;
		vehColours[2] = 133;
		vehColours[3] = 118;
		break;
	}
	case 41:
	{
		vehColours[0] = 106;
		vehColours[1] = 128;
		vehColours[2] = 117;
		vehColours[3] = 127;
		break;
	}
	case 42:
	{
		vehColours[0] = 0;
		vehColours[1] = 117;
		vehColours[2] = 127;
		vehColours[3] = 51;
		break;
	}
	case 43:
	{
		vehColours[0] = 125;
		vehColours[1] = 127;
		vehColours[2] = 128;
		vehColours[3] = 125;
		break;
	}
	case 44:
	{
		vehColours[0] = 127;
		vehColours[1] = 133;
		vehColours[2] = 0;
		vehColours[3] = 131;
		break;
	}
	case 45:
	{
		vehColours[0] = 0;
		vehColours[1] = 0;
		vehColours[2] = 131;
		vehColours[3] = 133;
		break;
	}
	case 46:
	{
		vehColours[0] = 0;
		vehColours[1] = 131;
		vehColours[2] = 128;
		vehColours[3] = 0;
		break;
	}
	case 47:
	{
		vehColours[0] = 120;
		vehColours[1] = 125;
		vehColours[2] = 100;
		vehColours[3] = 133;
		break;
	}
	case 48:
	{
		vehColours[0] = 115;
		vehColours[1] = 125;
		vehColours[2] = 133;
		vehColours[3] = 59;
		break;
	}
	case 49:
	{
		vehColours[0] = 130;
		vehColours[1] = 0;
		vehColours[2] = 0;
		vehColours[3] = 0;
		break;
	}
	case 50:
	{
		vehColours[0] = 57;
		vehColours[1] = 8;
		vehColours[2] = 33;
		vehColours[3] = 69;
		break;
	}
	case 51:
	{
		vehColours[0] = 8;
		vehColours[1] = 33;
		vehColours[2] = 70;
		vehColours[3] = 27;
		break;
	}
	case 52:
	{
		vehColours[0] = 35;
		vehColours[1] = 70;
		vehColours[2] = 51;
		vehColours[3] = 35;
		break;
	}
	case 53:
	{
		vehColours[0] = 89;
		vehColours[1] = 51;
		vehColours[2] = 35;
		vehColours[3] = 99;
		break;
	}
	case 54:
	{
		vehColours[0] = 51;
		vehColours[1] = 35;
		vehColours[2] = 118;
		vehColours[3] = 51;
		break;
	}
	case 55:
	{
		vehColours[0] = 125;
		vehColours[1] = 51;
		vehColours[2] = 125;
		vehColours[3] = 131;
		break;
	}
	case 56:
	{
		vehColours[0] = 51;
		vehColours[1] = 126;
		vehColours[2] = 0;
		vehColours[3] = 51;
		break;
	}
	case 57:
	{
		vehColours[0] = 128;
		vehColours[1] = 0;
		vehColours[2] = 63;
		vehColours[3] = 126;
		break;
	}
	case 58:
	{
		vehColours[0] = 0;
		vehColours[1] = 89;
		vehColours[2] = 8;
		vehColours[3] = 0;
		break;
	}
	case 59:
	{
		vehColours[0] = 118;
		vehColours[1] = 28;
		vehColours[2] = 0;
		vehColours[3] = 124;
		break;
	}
	case 60:
	{
		vehColours[0] = 32;
		vehColours[1] = 0;
		vehColours[2] = 125;
		vehColours[3] = 32;
		break;
	}
	case 61:
	{
		vehColours[0] = 0;
		vehColours[1] = 126;
		vehColours[2] = 51;
		vehColours[3] = 0;
		break;
	}
	case 62:
	{
		vehColours[0] = 127;
		vehColours[1] = 59;
		vehColours[2] = 0;
		vehColours[3] = 128;
		break;
	}
	case 63:
	{
		vehColours[0] = 89;
		vehColours[1] = 0;
		vehColours[2] = 131;
		vehColours[3] = 92;
		break;
	}
	case 64:
	{
		vehColours[0] = 133;
		vehColours[1] = 131;
		vehColours[2] = 89;
		vehColours[3] = 133;
		break;
	}
	case 65:
	{
		vehColours[0] = 27;
		vehColours[1] = 89;
		vehColours[2] = 133;
		vehColours[3] = 51;
		break;
	}
	case 66:
	{
		vehColours[0] = 101;
		vehColours[1] = 133;
		vehColours[2] = 66;
		vehColours[3] = 101;
		break;
	}
	case 67:
	{
		vehColours[0] = 133;
		vehColours[1] = 89;
		vehColours[2] = 101;
		vehColours[3] = 133;
		break;
	}
	case 68:
	{
		vehColours[0] = 101;
		vehColours[1] = 101;
		vehColours[2] = 133;
		vehColours[3] = 122;
		break;
	}
	case 69:
	{
		vehColours[0] = 101;
		vehColours[1] = 133;
		vehColours[2] = 125;
		vehColours[3] = 101;
		break;
	}
	case 70:
	{
		vehColours[0] = 133;
		vehColours[1] = 126;
		vehColours[2] = 101;
		vehColours[3] = 133;
		break;
	}
	case 71:
	{
		vehColours[0] = 131;
		vehColours[1] = 123;
		vehColours[2] = 132;
		vehColours[3] = 131;
		break;
	}
	case 72:
	{
		vehColours[0] = 123;
		vehColours[1] = 128;
		vehColours[2] = 132;
		vehColours[3] = 123;
		break;
	}
	case 73:
	{
		vehColours[0] = 128;
		vehColours[1] = 131;
		vehColours[2] = 123;
		vehColours[3] = 129;
		break;
	}
	case 74:
	{
		vehColours[0] = 127;
		vehColours[1] = 123;
		vehColours[2] = 128;
		vehColours[3] = 124;
		break;
	}
	case 75:
	{
		vehColours[0] = 123;
		vehColours[1] = 128;
		vehColours[2] = 91;
		vehColours[3] = 123;
		break;
	}
	case 76:
	{
		vehColours[0] = 128;
		vehColours[1] = 36;
		vehColours[2] = 127;
		vehColours[3] = 5;
		break;
	}
	case 77:
	{
		vehColours[0] = 118;
		vehColours[1] = 131;
		vehColours[2] = 51;
		vehColours[3] = 101;
		break;
	}
	case 78:
	{
		vehColours[0] = 0;
		vehColours[1] = 74;
		vehColours[2] = 101;
		vehColours[3] = 28;
		break;
	}
	case 79:
	{
		vehColours[0] = 82;
		vehColours[1] = 131;
		vehColours[2] = 131;
		vehColours[3] = 0;
		break;
	}
	case 80:
	{
		vehColours[0] = 0;
		vehColours[1] = 0;
		vehColours[2] = 3;
		vehColours[3] = 0;
		break;
	}
	case 81:
	{
		vehColours[0] = 0;
		vehColours[1] = 103;
		vehColours[2] = 3;
		vehColours[3] = 3;
		break;
	}
	case 82:
	{
		vehColours[0] = 103;
		vehColours[1] = 1;
		vehColours[2] = 1;
		vehColours[3] = 79;
		break;
	}
	case 83:
	{
		vehColours[0] = 3;
		vehColours[1] = 3;
		vehColours[2] = 73;
		vehColours[3] = 4;
		break;
	}
	case 84:
	{
		vehColours[0] = 4;
		vehColours[1] = 82;
		vehColours[2] = 6;
		vehColours[3] = 6;
		break;
	}
	case 85:
	{
		vehColours[0] = 84;
		vehColours[1] = 11;
		vehColours[2] = 11;
		vehColours[3] = 86;
		break;
	}
	case 86:
	{
		vehColours[0] = 16;
		vehColours[1] = 16;
		vehColours[2] = 92;
		vehColours[3] = 23;
		break;
	}
	case 87:
	{
		vehColours[0] = 23;
		vehColours[1] = 25;
		vehColours[2] = 34;
		vehColours[3] = 34;
		break;
	}
	case 88:
	{
		vehColours[0] = 28;
		vehColours[1] = 36;
		vehColours[2] = 36;
		vehColours[3] = 27;
		break;
	}
	case 89:
	{
		vehColours[0] = 47;
		vehColours[1] = 47;
		vehColours[2] = 91;
		vehColours[3] = 52;
		break;
	}
	case 90:
	{
		vehColours[0] = 52;
		vehColours[1] = 53;
		vehColours[2] = 53;
		vehColours[3] = 51;
		break;
	}
	case 91:
	{
		vehColours[0] = 64;
		vehColours[1] = 64;
		vehColours[2] = 65;
		vehColours[3] = 69;
		break;
	}
	case 92:
	{
		vehColours[0] = 69;
		vehColours[1] = 63;
		vehColours[2] = 70;
		vehColours[3] = 70;
		break;
	}
	case 93:
	{
		vehColours[0] = 64;
		vehColours[1] = 73;
		vehColours[2] = 73;
		vehColours[3] = 58;
		break;
	}
	case 94:
	{
		vehColours[0] = 76;
		vehColours[1] = 76;
		vehColours[2] = 58;
		vehColours[3] = 2;
		break;
	}
	case 95:
	{
		vehColours[0] = 2;
		vehColours[1] = 63;
		vehColours[2] = 21;
		vehColours[3] = 21;
		break;
	}
	case 96:
	{
		vehColours[0] = 72;
		vehColours[1] = 22;
		vehColours[2] = 22;
		vehColours[3] = 72;
		break;
	}
	case 97:
	{
		vehColours[0] = 13;
		vehColours[1] = 11;
		vehColours[2] = 91;
		vehColours[3] = 19;
		break;
	}
	case 98:
	{
		vehColours[0] = 19;
		vehColours[1] = 93;
		vehColours[2] = 0;
		vehColours[3] = 0;
		break;
	}
	default:
		break;
	}
	updateLocalVehColours(0,0);
}

typedef enum eVehicleJumps
{
	VEH_JUMP_ALL = 0,
	VEH_JUMP_LEFT,
	VEH_JUMP_RIGHT,
	VEH_JUMP_BACK,
	VEH_JUMP_FRONT
}eVehicleJumps;

void doLocalVehJumps(int bounceType, int un2)
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVehicle;
		PED_INDEX vehicleDriver;
		float vehicleMass;
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVehicle);
		GET_DRIVER_OF_CAR(myVehicle, &vehicleDriver);
		GET_CAR_MASS(myVehicle, &vehicleMass);
		if (vehicleDriver == myPlayerPed)
		{
			switch (bounceType)
			{
			case VEH_JUMP_ALL: APPLY_FORCE_TO_CAR(myVehicle, true, 0.1, 0.1, 2.5f, 0.0, 0.0, 0.0, true, false, true, false);  //Bounce All
			case VEH_JUMP_LEFT: APPLY_FORCE_TO_CAR(myVehicle, 1, vehicleMass, 0.0f, vehicleMass, 0.0f, 0.0, 0.0, true, false, true, false); //Bounce Left //only goes up for some reason..
			case VEH_JUMP_RIGHT: APPLY_FORCE_TO_CAR(myVehicle, 1, -vehicleMass, 0.0f, vehicleMass, 0.0f, 0.0, 0.0, true, false, true, false); //Bounce Right 
			case VEH_JUMP_BACK: APPLY_FORCE_TO_CAR(myVehicle, 1, 0.0f, vehicleMass, vehicleMass / 2.0f, 0.0f, 0.0f, 0.0, true, false, true, false); //Bounce Back
			case VEH_JUMP_FRONT: APPLY_FORCE_TO_CAR(myVehicle, 1, 0.0f, -vehicleMass, vehicleMass / 2.0f, 0.0f, 0.0f, 0.0, true, false, true, false); //Bounce Front
			/*
			* 
			* my attempt
			* 
			* case VEH_JUMP_ALL: APPLY_FORCE_TO_CAR(myVehicle, true, 0.1, 0.1, 2.5f, 0.0, 0.0, 0.0, true, false, true, false);  //Bounce All
			case VEH_JUMP_LEFT: APPLY_FORCE_TO_CAR(myVehicle, 1, 0.1, 0.1, vehicleMass, 0.5f, 0.0, 0.0, true, false, true, false); //Bounce Left //only goes up for some reason..
			case VEH_JUMP_RIGHT: APPLY_FORCE_TO_CAR(myVehicle, 1, 0.1, 0.1, vehicleMass, -0.5f, 0.0, 0.0, true, false, true, false); //Bounce Right 
			case VEH_JUMP_BACK: APPLY_FORCE_TO_CAR(myVehicle, 1, 0.1, 0.1, vehicleMass, 0.0, -0.5f, 0.0, true, false, true, false); //Bounce Back
			case VEH_JUMP_FRONT: APPLY_FORCE_TO_CAR(myVehicle, 1, 0.1, 0.1, vehicleMass, 0.0, 0.5f, 0.0, true, false, true, false); //Bounce Front
			* 
			case VEH_JUMP_ALL: APPLY_FORCE_TO_CAR(myVehicle, true, 0.1, 0.1, 2.5, 0.0, 0.0, 0.0, true, false, true, true);  //Bounce All
			case VEH_JUMP_LEFT: APPLY_FORCE_TO_CAR(myVehicle, true, 0.1, 0.1, 1.5, -0.5, 0.0, 0.0, true, false, true, true); //Bounce Left
			case VEH_JUMP_RIGHT: APPLY_FORCE_TO_CAR(myVehicle, true, 0.1, 0.1, 1.5, 0.5, 0.0, 0.0, true, false, true, true); //Bounce Right
			case VEH_JUMP_BACK: APPLY_FORCE_TO_CAR(myVehicle, true, 0.1, 0.1, 1.5, 0.0, -0.5, 0.0, true, false, true, true); //Bounce Back
			case VEH_JUMP_FRONT: APPLY_FORCE_TO_CAR(myVehicle, true, 0.1, 0.1, 1.5, 0.0, 0.5, 0.0, true, false, true, true); //Bounce Front*/
			}
			return;
		}
		printf("~r~CTL: ~w~This only works well on your own vehicles due to network control taking too long.");
		return;
	}
	printf("~r~CTL: ~w~Where's your vehicle.. wise guy %s.", GET_PLAYER_NAME(myPlayerIndex));
}

void netPlayerVehicleFlips(int un1, int un2)
{
	//Car Bounce probs needs optimised */ - //Credit StuartB - yes this is simple but I couldn't be fked to do it since he already polished it - Nath
	SetHeader("Vehicle -> Actrions -> Jump/Bounce");
	AddItemWithParam("All", VEH_JUMP_ALL, doLocalVehJumps, nullptr);
	AddItemWithParam("Left", VEH_JUMP_LEFT, doLocalVehJumps, nullptr);
	AddItemWithParam("Right", VEH_JUMP_RIGHT, doLocalVehJumps, nullptr);
	AddItemWithParam("Back", VEH_JUMP_BACK, doLocalVehJumps, nullptr);
	AddItemWithParam("Front", VEH_JUMP_FRONT, doLocalVehJumps, nullptr);
}

void VehOptsColour()
{
	PED_INDEX driver;
	bool localPlayerIsDriving = false;

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh;
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
		GET_CAR_COLOURS(myVeh, &vehColours[0], &vehColours[1]);
		GET_EXTRA_CAR_COLOURS(myVeh, &vehColours[2], &vehColours[3]);
		GET_DRIVER_OF_CAR(myVeh, &driver);
		localPlayerIsDriving = (driver == myPlayerPed);
	}

	SetHeader("Vehicle -> Actions -> Colour"); //not gonna add for net players as it doesn't sync reliably...
	AddItemCarColor("Colour 1", localPlayerIsDriving, &vehColours[0], updateLocalVehColours, localPlayerIsDriving ? nullptr : "Press ~PAD_A~ to update colours.");
	AddItemCarColor("Colour 2", localPlayerIsDriving, &vehColours[1], updateLocalVehColours, localPlayerIsDriving ? nullptr : "Press ~PAD_A~ to update colours.");
	AddItemCarColor("Extra 1", localPlayerIsDriving, &vehColours[2], updateLocalVehColours, localPlayerIsDriving ? nullptr : "Press ~PAD_A~ to update colours.");
	AddItemCarColor("Extra 2", localPlayerIsDriving, &vehColours[3], updateLocalVehColours, localPlayerIsDriving ? nullptr : "Press ~PAD_A~ to update colours.");
	AddItem("Set all to primary colour", setVehColoursPrimary, "Sets all to vehicles current primary colour.");
	AddItemInt("Fancy colors", localPlayerIsDriving, 0, 98, &selectedFancyColor, 1, applyFancyColor, localPlayerIsDriving ? nullptr : "Press ~PAD_A~ to update colours.");
	if (localPlayerIsDriving)
	{
		AddItemBool("Colour cycle", &ctlMem->m_bit_sets.m_vehicle, cycleCarColours, nullptr, "Change colours in sequence, slowed down for network.");
		AddItemBool("Colour random", &ctlMem->m_bit_sets.m_vehicle, randomCarColours, nullptr, "Change colours at random, slowed down for network.");
		AddItemBool("All colours for cycle/random", &ctlMem->m_bit_sets.m_vehicle, cycleRandomChangeAllColours, nullptr, "Change all colours when cycling or random.");
		AddItemMenu("Custom colour", VehCustomColor, "Set a custom colour. (Doesn't appear for other players!)");
	}
}

void flapDoorTest(int unused1, int unused2)
{
	int player = GetMenuPlayerId();
	if (PLAYER_HAS_CHAR(player))
	{
		PED_INDEX ped;
		GET_PLAYER_CHAR(player, &ped);
		if (IS_CHAR_IN_ANY_CAR(ped))
		{
			VEHICLE_INDEX veh;
			GET_CAR_CHAR_IS_USING(ped, &veh);
			for (int i = 0; i < 2; i++)
			{
				CONTROL_CAR_DOOR(veh, i, 0, 1.0f);
			}
		}
	}
}

void VehOptsQuestionable()
{
	SetHeader("Vehicle -> Actions -> Questionable");
	AddItemWithParam("Set on fire", VEH_ACT_SET_ON_FIRE, ProcessVehicleAction, nullptr);
	AddItemWithParam("Explode", VEH_ACT_EXPLODE, ProcessVehicleAction, nullptr);
	AddItemWithParam("Flatten tyres", VEH_ACT_BURST_TYRES, ProcessVehicleAction, nullptr);
	AddItemWithParam("Smash windows", VEH_ACT_BREAK_WINDOWS, ProcessVehicleAction, nullptr);
	AddItemWithParam("Break doors", VEH_ACT_BREAK_DOORS, ProcessVehicleAction, nullptr);
	AddItemInt("Set forward speed", false, 2, 10, &netVehSpeedBoostAmount, 1, VehOptSpeedBoost, "Set speed to current, times (x) the number you select.");
	AddItemFloat("Rotate", false, 90.0f, 180.0f, &RotateVehAmount, 90.0f, 1, VehOptRotate, "Adjust vehicles heading.");
	AddItemWithParam("Slingshot", 1, VehOptSlingshot, "Apply force upwards, to infiniy and beyond!");
	AddItemWithParam("Slingshot v2", 2, VehOptSlingshot, "Point vehicle at sky, set forward speed.");
#ifdef Developer
	AddItem("Open front doors test", flapDoorTest, nullptr);
#endif
}

#ifdef Developer
void VehOptAdminDeleteCurrentVehNetId(int un1, int un2)
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		NETWORK_INDEX target_id;
		GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &target_id);
		PLAYER_INDEX controller = FindControllerOfNetworkId(target_id);
		if (!HAS_CONTROL_OF_NETWORK_ID(target_id) && controller != INVALID_PLAYER_INDEX)
		{
			sprintf(freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string, "%i", target_id);
			SendAdminCommandToPlayer(ADMIN_CMD_DELETE_ENTITY_WITH_NETWORK_ID_IF_HAS_CONTROL, (controller + 1));
		}
	}
}
#endif

void VehOneTimeCmds()
{
	//no native for IS_CAR_VISIBLE unfortunately.
	ctlMem->m_my_veh_data.currentLock = 1; //will return unlocked car if on foot.

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		GET_CAR_DOOR_LOCK_STATUS(GetPlayerVeh(), (uint*)&ctlMem->m_my_veh_data.currentLock);
		SetBit(&ctlMem->m_bit_sets.m_vehicle, vehicleScorch, IsVehicleScorched(GetPlayerVeh()));
	}
	else
		SetBit(&ctlMem->m_bit_sets.m_vehicle, vehicleScorch, false);

	ctlMem->m_my_veh_data.itemSelect = DoorLockToItemSelect(ctlMem->m_my_veh_data.currentLock);
	
	SetHeader("Vehicle -> Actions");
	AddItemMenu("Colours", VehOptsColour, nullptr);
	AddItemMenu("Attachments", netPlayerAttachVehObjs, "Universal neons, heli blades & objects.");
	AddItemMenu("Jump/Bounce", netPlayerVehicleFlips, nullptr);
	//AddItem("Fix", VehOptFixNormal, "Repair damage to vehicle."); //maybe add option to set invincible too..
	AddItemWithParam("Flip", VEH_ACT_FLIP, ProcessVehicleAction, "BANG; and the dirt is gone!");
	AddItemWithParam("Fix", VEH_ACT_FIX, ProcessVehicleAction, "BANG; and the dirt is gone!");
	AddItemWithParam("Clean", VEH_ACT_CLEAN, ProcessVehicleAction, "BANG; and the dirt is gone!");
#ifdef Developer
	AddItemAlt(ctlMem->m_commonStrings[GXT_STRING_DELETE], VehOptDelete, "~PAD_A~ = Vehicle, ~PAD_X~ = Objects attached, ~PAD_Y~ = Admin controller.", VehOptDeleteObjects, VehOptAdminDeleteCurrentVehNetId); // "Delete"
#else
	AddItemAlt(ctlMem->m_commonStrings[GXT_STRING_DELETE], VehOptDelete, "~PAD_A~ = Vehicle, ~PAD_X~ = Objects attached.", VehOptDeleteObjects, nullptr); // "Delete"
#endif
	AddItemChar("Set door lock", false, DoorLockString, 4, (int*)&ctlMem->m_my_veh_data.itemSelect, DoorLocker, "Lock vehicle doors accordingly.");
	AddItemAlt("Eject", vehOptEjectCurrent, "~PAD_X~ = Through windscreen, ~PAD_Y~ = Knock off bike.", vehOptEjectWindscreen, vehOptEjectKnockOffBike);
	AddItemAlt("Change seat", VehOptIncrementSeat, "~PAD_A~ = Warp, ~PAD_X~ = Shuffle.", VehOptShuffleSeats, nullptr);
	AddItem("Remove tyres", VehOptRemoveTyres, "Attaches vehicle model to vehicle as an Object");
	AddItemBool("Invisibility", &ctlMem->m_bit_sets.m_vehicle, vehicleInvisibility, vehOptToggleVisible, "Make your vehicle invisible.");
	AddItemBool("Render scorched", &ctlMem->m_bit_sets.m_vehicle, vehicleScorch, VehOptSetScorched, "Burn vehicle out visually.");
	AddItemMenu("Questionable actions", VehOptsQuestionable, "Flatten tyres, break windows etc.");
}

void VehPassengerOpts()
{
	SetHeader("Vehicle -> Passengers");
	AddItemInt("Give weapons", true, 0, 60, (int*)&ctlMem->m_my_veh_data.checkGiveWepsTimerInt, 1, PassengerWepsCheck, "Give weapons to passengers every x seconds if not 0.");
	AddItemInt("Check & give armour", true, 0, 60, (int*)&ctlMem->m_my_veh_data.checkGiveArmourTimerInt, 1, PassengerArmourCheck, "Give armour to passengers every x seconds if not 0.");
	AddItemBool("Notifications", &ctlMem->m_bit_sets.m_vehicle, passengerGiveNotifications, nullptr, "Be notified when passengers inventory is replenished.");
	AddItem("Eject", HandlingRespawn, "Eject passengers by respawning your vehicle.");
}

void VehOptUpgrades()
{
	SetHeader("Vehicle -> Upgrades");

	if (!IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		AddItem("You are not in a vehicle.", nullptr, "Reload this page when you are.");
		return;
	}
	VEHICLE_INDEX myVehicle;
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myVehicle);
	bool addedItem = false;
	for (int i = 0; i < 10; i++)
	{
		if (IS_VEHICLE_EXTRA_TURNED_ON(myVehicle, i + 1))
			SET_BIT(&ctlMem->m_temp_vehicle_upgrades, i); //VEHICLE HAS THIS UPGRADE CURRENTLY

		TURN_OFF_VEHICLE_EXTRA(myVehicle, i + 1, false);
		if (IS_VEHICLE_EXTRA_TURNED_ON(myVehicle, i + 1))
			SET_BIT(&ctlMem->m_temp_vehicle_upgrades, i + 10); //VEHICLE UPGRADE IS AVAILABLE //i + 11? idk if overlaps possible.

		TURN_OFF_VEHICLE_EXTRA(myVehicle, i + 1, !IS_BIT_SET(ctlMem->m_temp_vehicle_upgrades, i));

		if (IS_BIT_SET(ctlMem->m_temp_vehicle_upgrades, i + 10)) //if upgrade is available.
		{
			AddItemBool("Upgrade", &ctlMem->m_temp_vehicle_upgrades, i, updateVehicleUpgrades, nullptr);
			addedItem = true;
		}
	}

	if (!addedItem)
		AddItem("No upgrades available", nullptr, nullptr);
}


void VehicleOptions()
{
	SetHeader(ctlMem->m_commonStrings[GXT_STRING_VEHICLE]); //"Vehicle"
	AddItemMenu("Toggles", VehicleToggles, "Loops and settings.");
	AddItemMenuAlt("Spawn vehicle", SpawnVehicleSetup, "~PAD_X~ = Respawn (kicks out passengers)", HandlingRespawn);

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		AddItemMenu("Handling Editor", HandlingEditor, nullptr);
		AddItemMenu("Actions", VehOneTimeCmds, "Fix, change colour, delete etc.");
		AddItemMenu("Passengers", VehPassengerOpts, "Loops, eject etc.");
		AddItemMenu("Upgrades", VehOptUpgrades, nullptr);
	}
}
/*
bool PauseHelperVehLoops(void)
{
	if (!DOES_SCRIPT_EXIST("cartel_helper"))
		return false;

	SET_BIT(&ctlMem->m_bit_sets.m_debug, DEBUG_PAUSE_HELPER_VEH_LOOPS);
	while (!IS_BIT_SET(ctlMem->m_bit_sets.m_debug, DEBUG_HELPER_VEH_LOOPS_PAUSED)) WAIT(0);
	return true;
}
*/

bool vehicle_auto_boat_loop(VEHICLE_INDEX myVeh) //returns true if vehicle loop should bail
{
	//if (!DOES_VEHICLE_EXIST(myVeh))
	//	return false;

	if (DOES_VEHICLE_EXIST(ctlMem->m_autoBoat) && myVeh == ctlMem->m_autoBoat)
	{
		//if is y pressed.
		VEHICLE_INDEX originalVehicle;
		GET_CAR_CHAR_IS_USING(auto_boat_seat_reserve_ped, &originalVehicle);
		if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y))
		{
			REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION(myPlayerPed, ctlMem->m_autoBoat);
			DeleteChar(&auto_boat_seat_reserve_ped);
			DeleteCar(&ctlMem->m_autoBoat);
			return true;
		}
		float height_above_ground;
		GET_CHAR_HEIGHT_ABOVE_GROUND(myPlayerPed, &height_above_ground);
#ifdef Developer //leaving this while I have stability tested - Nath 19/12/2024
		/*
		char buffer[96];
		strcpy(buffer, "Height above ground == ", 96);
		ftoa(height_above_ground, buffer, 2);
		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
		printf(buffer);
		//use height_above_ground < 1.30f //for normal sized vehicle / boat
		//or < 2.0f and car not in water? && > 0.0f //as goes negative sometimes out at sea..*/
		
#endif
		if (height_above_ground < 2.0f && height_above_ground > 0.0f && !IS_CAR_IN_WATER(ctlMem->m_autoBoat) && !IS_CAR_IN_AIR_PROPER(ctlMem->m_autoBoat))
		{
			//could probably do with triggering a auto-boat timeout once this has been pressed.
			DeleteChar(&auto_boat_seat_reserve_ped);
			float speed;
			GET_CAR_SPEED(ctlMem->m_autoBoat, &speed);
			DeleteCar(&ctlMem->m_autoBoat);
			WARP_CHAR_INTO_CAR(myPlayerPed, originalVehicle);
			SET_CAR_COLLISION(originalVehicle, true);
			SET_CAR_FORWARD_SPEED(originalVehicle, IS_BIG_VEHICLE(originalVehicle) ? (speed * 1.5f) : (speed / 1.5f));
			SET_CHAR_VISIBLE(myPlayerPed, true);
			ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
			printf("~r~CTL: ~w~Auto-boat reverted.");
			return true;
			//convert back
		}
	}
	//lets make it a bigger and slower boat if myVEH IS_BIG_VEHICLE
	if (IS_CAR_IN_WATER(myVeh))
	{
		uint vehicle_model;
		GET_CAR_MODEL(myVeh, &vehicle_model);
		if (!DOES_VEHICLE_EXIST(ctlMem->m_autoBoat) && !IS_THIS_MODEL_A_BOAT(vehicle_model))
		{
			uint myModel;
			GET_CHAR_MODEL(myPlayerPed, &myModel);
			if (myModel == PED_PLAYER) //stop self-crash
				myModel = PED_IG_ROMAN; //we don't flip we just set ourselves invisible so this is fine.
			
			REQUEST_MODEL(myModel);
			REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION(myPlayerPed, myVeh);
			SET_CAR_COLLISION(myVeh, false); //this was missing and probably makes a difference. collision fucks you to the middle of the map we know that from old physical attach objects.
			float vX, vY, vZ, vH, speed;
			GET_CAR_COORDINATES(myVeh, &vX, &vY, &vZ);
			GET_CAR_HEADING(myVeh, &vH);
			GET_CAR_SPEED(myVeh, &speed);
			SET_CAR_AS_MISSION_CAR(myVeh); //CRASH WITHOUT THIS AS WE MARK CARS AS NO LONGER NEEDED ON SPAWN NOW.
			//SET_CAR_ENGINE_ON(myVeh, false, true);
		//	printf("~r~CTL: ~w~Auto-boat activating, hold ~PAD_LSTICK_ALL~ and press ~PAD_A~ + ~PAD_X~ to revert.");
			ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
			printf("~r~CTL: ~w~Auto-boat activated.");
			if (SpawnVehicle( (IS_BIG_VEHICLE(myVeh) ? VEHICLE_MARQUIS : VEHICLE_JETMAX), vX, vY, vZ, vH, &ctlMem->m_autoBoat, false, true))
			{
				WARP_CHAR_INTO_CAR(myPlayerPed, ctlMem->m_autoBoat);
				SET_CAR_VISIBLE(ctlMem->m_autoBoat, false);
				//ATTACH_CAR_TO_CAR(myVeh, boat_we_attach_to, 0, 0.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.0f); //looks perfect in comet but meh.
				//the attachment positions seem to make a difference to camera and water splash noise spam/annoyance.
				ATTACH_CAR_TO_CAR(myVeh, ctlMem->m_autoBoat, 0, 0.0f, 0.0f, 0.20f, 0.0f, 0.0f, 0.0f);
				//we're now driving the boat, lets stick a ped in the original vehicles drivers seat so nobody can steal it.
				while (!HAS_MODEL_LOADED(myModel))
					WAIT(0);
				
				CREATE_CHAR_INSIDE_CAR(myVeh, 0, myModel, &auto_boat_seat_reserve_ped);
				SET_CHAR_VISIBLE(myPlayerPed, false);
				CLEAR_ALL_CHAR_PROPS(auto_boat_seat_reserve_ped);
				for (int i = 0; i < 11; i++)
					SET_CHAR_COMPONENT_VARIATION(auto_boat_seat_reserve_ped, i, GET_CHAR_DRAWABLE_VARIATION(myPlayerPed, i), GET_CHAR_TEXTURE_VARIATION(myPlayerPed, i));

				int myPropIndexes[2];
				for (int i = 0; i < 2; i++)
				{
					GET_CHAR_PROP_INDEX(myPlayerPed, i, &myPropIndexes[i]);
					SET_CHAR_PROP_INDEX(auto_boat_seat_reserve_ped, i, myPropIndexes[i]);
				}
				SET_CHAR_INVINCIBLE(auto_boat_seat_reserve_ped, true);
				SET_CHAR_WILL_LEAVE_CAR_IN_COMBAT(auto_boat_seat_reserve_ped, false);
				SET_CHAR_DROWNS_IN_SINKING_VEHICLE(auto_boat_seat_reserve_ped, false);
				SET_CHAR_WILL_MOVE_WHEN_INJURED(auto_boat_seat_reserve_ped, false);
				SET_CHAR_CANT_BE_DRAGGED_OUT(auto_boat_seat_reserve_ped, true); //should try this on players lol, better than health -1
				if (NetworkGame) //attempt to fix singleplayer bug
				{
					NETWORK_INDEX NetworkId;
					GET_NETWORK_ID_FROM_PED(auto_boat_seat_reserve_ped, &NetworkId);
					if (DoesNetworkIdExist(NetworkId))
						SET_NETWORK_ID_CAN_MIGRATE(NetworkId, false);
				}
				SET_CAR_COORDINATES(ctlMem->m_autoBoat, vX, vY, vZ);
				SET_CAR_HEADING(ctlMem->m_autoBoat, vH); //idk why but this is needed
				SET_CAR_FORWARD_SPEED(ctlMem->m_autoBoat, IS_BIG_VEHICLE(myVeh) ? (speed * 1.5f) : speed);
				//we've now attached our original vehicle to a jetmax/boat and are in the drivers seat of it.
			}
		}
	}
	//only called when we are a driver.
	//autoSpawnBoatViceVersa
	return false;
}

void VehicleLoopedOptions()
{
	int networkPlayerVehicleId;
	PED_INDEX driverped;
	int myPassengers;
	//static uint cycleCarColoursNetworkDelayTick;
	/*
	if (IS_BIT_SET(ctlMem->m_bit_sets.m_vehicle, vehicleCruiseControl))
	{
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			PED_INDEX driver;
			GET_DRIVER_OF_CAR(GetPlayerVeh(), &driver); //reusing variables.
			if (driver == myPlayerPed) // if driver == GetPlayerPed
				SetVehicleEngineRevs(GetPlayerVeh(), engineCruiseRevs);
		}
	}
	*/
	if (DOES_VEHICLE_EXIST(myObjectCar))
	{
		if (!IS_CHAR_SITTING_IN_CAR(myPlayerPed, myObjectCar))
		{
			for (int i = 0; i < MAX_CAR_OBJECTS; i++)
			{
				if (DOES_OBJECT_EXIST(freeMem->m_object_car_entities[i]))
				{
					NETWORK_INDEX ObjectCarObjectNetID;

					GET_NETWORK_ID_FROM_OBJECT(freeMem->m_object_car_entities[i], &ObjectCarObjectNetID); 
					ReleaseControlOfNetworkId(ObjectCarObjectNetID);
				}
			}
		}
	}
	NETWORK_INDEX NetworkId;
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh;
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
		GET_DRIVER_OF_CAR(myVeh, &driverped);

		//Below is tested and confirmed by Nath.
		SET_CHAR_WILL_FLY_THROUGH_WINDSCREEN(myPlayerPed, !bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleSeatBelt));
		SET_CHAR_CAN_BE_KNOCKED_OFF_BIKE(myPlayerPed, bitTest(ctlMem->m_bit_sets.m_vehicle, bikeSeatSuperGlue)); //this is inverted for some bizzare reason.
		//i think the native is actually CANT_BE_KNOCKED_OFF ?

		if (bitTest(ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack))
		{ //if I recall correctly, my old method checked if the passenger was jacking / could've been if player about to enter car is jacking... respawn.
			if (IS_PED_BEING_JACKED(myPlayerPed)) //doesn't seem to work might need to trigger a respawn like before.
			{
				if (driverped == myPlayerPed)
				{
					HandlingRespawn();
					GET_CAR_CHAR_IS_USING(driverped, &myVeh);
					//Back in the day i would for loop through players to see if they were jacking and print their name, this can i imagine.. not be 100% accurate though.
				}
				else
					WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, myVeh, GetPedSeatIndex(myPlayerPed));  //probably work nicely without having to respawn it.
			}
		}

		if (driverped == myPlayerPed) //code to be executed only when local player is the driver of a vehicle..
		{
			clearFollowDriverData(); //stop getting fucked when we spawn ourselves into a car.
			if (bitTest(ctlMem->m_bit_sets.m_vehicle, autoSpawnBoatViceVersa))
			{
				if (vehicle_auto_boat_loop(myVeh)) //returns if user pressed input that will kill myVeh, so we'll cancel this one loop this one time.
					return;
			}

			int maxPassengers;

			GET_NUMBER_OF_PASSENGERS(myVeh, &myPassengers);
			GET_MAXIMUM_NUMBER_OF_PASSENGERS(myVeh, &maxPassengers);

			if (myPassengers > 0)
			{
				if (ctlMem->m_my_veh_data.checkGiveArmourTimerInt > 0 && passengerArmourTick < GetTickCount()) //Timer int set above 0 and has been more than set time in ms.
				{
					PED_INDEX passenger;
					bool given = false;

					for (int i = 0; i < maxPassengers; i++)
					{
						int armour;

						if (IS_CAR_PASSENGER_SEAT_FREE(myVeh, i))
							continue;

						GET_CHAR_IN_CAR_PASSENGER_SEAT(myVeh, i, &passenger);
						if (!DOES_CHAR_EXIST(passenger)) 
							continue;

						GET_CHAR_ARMOUR(passenger, &armour);

						if (armour < 5)
						{
							if (!HAS_CHAR_GOT_WEAPON(passenger, WEAPON_ARMOUR))
								GIVE_WEAPON_TO_CHAR(passenger, WEAPON_ARMOUR, 1, false);

							ADD_ARMOUR_TO_CHAR(passenger, 999);
							given = true;
						}
					}

					passengerArmourTick = GetTickCount() + (ctlMem->m_my_veh_data.checkGiveArmourTimerInt * 1000); //re-set timer as ran
					if (given && bitTest(ctlMem->m_bit_sets.m_vehicle, passengerGiveNotifications))
						printf("~r~CTL: ~w~Passengers armour replenished.");
				}

				if (ctlMem->m_my_veh_data.checkGiveWepsTimerInt > 0 && passengerWeaponsTick < GetTickCount()) //Timer int set above 0 and has been more than set time in ms.
				{
					PED_INDEX passenger;
					bool given = false;

					for (int i = 0; i < maxPassengers; i++)
					{
						if (IS_CAR_PASSENGER_SEAT_FREE(myVeh, i))
							continue;

						GET_CHAR_IN_CAR_PASSENGER_SEAT(myVeh, i, &passenger);
						if (!DOES_CHAR_EXIST(passenger)) 
							continue;

						GiveWeaponsToPed(passenger, WEAPON_TYPE_ADVANCED, false); //think the removing then immediately giving weapons is fucking this up
						given = true;
					}

					passengerWeaponsTick = GetTickCount() + (ctlMem->m_my_veh_data.checkGiveWepsTimerInt * 1000);
					if (given && bitTest(ctlMem->m_bit_sets.m_vehicle, passengerGiveNotifications))
						printf("~r~CTL: ~w~Passengers weapons replenished.");
				}
			}
		}
		else //we're a passenger / driver doesn't exist.
		{
			//DISABLE_PLAYER_AUTO_VEHICLE_EXIT(myPlayerPed, IS_BIT_SET(ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack)); //kudos stuart //stops team switching ejection. //only set when we load save settings or toggle it now.
			if (bitTest(ctlMem->m_bit_sets.m_vehicle, autoWarpDriversNewVehicle))
			{
				if (ctlMem->m_follow_driver_data.exit_vehicle_input_pressed_timeout_tick < GetTickCount())
				{ //don't run if we've pressed Y recently while in a vehicle..
				//EXIT
					PED_INDEX playerPed;
					for (int i = 0; i < 16; i++)
					{
						if (!IsPlayerValid(i) || !PLAYER_HAS_CHAR(i))
							continue;

						GET_PLAYER_CHAR(i, &playerPed);
						if (playerPed == driverped)
						{
							ctlMem->m_follow_driver_data.playerFollowing = i;
							ctlMem->m_follow_driver_data.tick = (GetTickCount() + 1000); //try get back in for one second if the car disappears.
							ctlMem->m_follow_driver_data.SeatIndex = GetPedSeatIndex(myPlayerPed);
							break;
						}
					}
					if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y))
					{
						uint lock;
						GET_CAR_DOOR_LOCK_STATUS(myVeh, &lock);
						//stops you getting locked in a car.
						if (lock == DOOR_LOCK_IMPOSSIBLE_TO_OPEN)
						{
							REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION(myPlayerPed, myVeh);
							ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
							//Helper prints / extra info enabled check here.
							printf("~r~CTL: ~w~Bypassed locked %s doors.", VehiclesModelName(myVeh));
						}

						clearFollowDriverData();
					}
				}
			}
		}
	}
	else
	{ //we are on foot.
		if (bitTest(ctlMem->m_bit_sets.m_vehicle, autoWarpDriversNewVehicle) && ctlMem->m_follow_driver_data.SeatIndex != 1337)
		{
			if (IsPlayerValid(ctlMem->m_follow_driver_data.playerFollowing) && PLAYER_HAS_CHAR(ctlMem->m_follow_driver_data.playerFollowing))
			{
				if (ctlMem->m_follow_driver_data.tick > GetTickCount()) //only try for one second.
				{
					PED_INDEX followingPlayersPed;
					GET_PLAYER_CHAR(ctlMem->m_follow_driver_data.playerFollowing, &followingPlayersPed);
					if (IS_CHAR_IN_ANY_CAR(followingPlayersPed))
					{
						VEHICLE_INDEX followingPlayersNewVeh;
						GET_CAR_CHAR_IS_USING(followingPlayersPed, &followingPlayersNewVeh);
						bool warped;
						if (IS_CAR_PASSENGER_SEAT_FREE(followingPlayersNewVeh, ctlMem->m_follow_driver_data.SeatIndex))
						{
							WARP_CHAR_INTO_CAR_AS_PASSENGER(myPlayerPed, followingPlayersNewVeh, ctlMem->m_follow_driver_data.SeatIndex);
							warped = true;
						}
						else
							warped = warp_local_player_into_vehicle(followingPlayersNewVeh);

						if (warped) //Helper prints / extra info enabled check here.
							printf("~r~CTL: ~w~Warped into %s's ~w~new %s.", NetworkPlayerGetNameWithColour(ctlMem->m_follow_driver_data.playerFollowing), VehiclesModelName(followingPlayersNewVeh));
					}
				}
				else
				{
					ctlMem->m_follow_driver_data.SeatIndex = 1337; //invalid
				}
			}
		}

		if (bitTest(ctlMem->m_bit_sets.m_vehicle, bypassVehicleDoorLocks))
		{
			if (IS_CHAR_TRYING_TO_ENTER_A_LOCKED_CAR(myPlayerPed)) //|| IS_CHAR_GETTING_IN_TO_A_CAR
			{
				//should get the status and make sure we couldn't enter it under normal circumstaces..
				VEHICLE_INDEX vehWouldEnter;
				GET_VEHICLE_PLAYER_WOULD_ENTER(myPlayerIndex, &vehWouldEnter);
				uint lock;
				GET_CAR_DOOR_LOCK_STATUS(vehWouldEnter, &lock);
				//this stops you warping in when you could still enter / break in in under normal circumstances.
				if (lock == DOOR_LOCK_IMPOSSIBLE_TO_OPEN || lock == DOOR_LOCK_CAN_OPEN_FROM_INSIDE || lock == DOOR_LOCK_REFUSE_TO_ENTER)
				{
					/*
					for (int i = 0; i < 3; i++)
					{
						if (IS_CAR_PASSENGER_SEAT_FREE(vehWouldEnter, i))
						{
							detachLocalPlayerFromVehicleOrWalker();
							disableFlymod();
							printf("~r~CTL: ~w~Bypassing locked %s doors.", VehiclesModelName(vehWouldEnter));
							LOCK_CAR_DOORS(myVeh, DOOR_LOCK_NONE);
							TASK_ENTER_CAR_AS_PASSENGER(myPlayerPed, vehWouldEnter, 5000, i);
							break;
						}
					}
					*/
					
					if (warp_local_player_into_vehicle(vehWouldEnter))
					{
						ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
						//Helper prints / extra info enabled check here.
						printf("~r~CTL: ~w~Bypassed locked %s doors.", VehiclesModelName(vehWouldEnter));
					}
					
				}
			}
		}
	}
}