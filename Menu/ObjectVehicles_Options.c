#include "stdafx.h"
#include "Utils.h"
#include "Vehicle_Options.h"
#include "MenuExecutionHandling.h"
#include "Functions.h"
#include "World_Options.h"
#include "Network_Options.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types" 

VEHICLE_INDEX myObjectCar;
int neonSelect;
char* NeonStrings[6] = { "Red", "Blue", "Yellow", "Green", "Pink", "Orange" };
char* MinecraftColourStrings[6] = { "Red", "Blue", "Yellow", "Green", "Pink", "Orange" };
int minecraftFunnyColourSelect = 0;

void ClearFunnyCarEntities() //called when spawning funny car
{
	NETWORK_INDEX netId;

	if (bitTest(ctlMem->m_bit_sets.m_vehicle, deleteLastFunnyVehObjects)) //spawning vehicle in general will delete objects
	{
		//could look at requesting network control..
		for (int i = 0; i < MAX_CAR_OBJECTS; i++)
		{
			if (DOES_OBJECT_EXIST(freeMem->m_object_car_entities[i]))
			{
				GET_NETWORK_ID_FROM_OBJECT(freeMem->m_object_car_entities[i], &netId);
				if (!RequestNetworkControl(netId, "vehicle attachment (object)")) 
					continue;
				
				DELETE_OBJECT((OBJECT_INDEX*)&freeMem->m_object_car_entities[i]);
				continue;
			}
			if (DOES_VEHICLE_EXIST(freeMem->m_object_car_entities[i]))
			{
				GET_NETWORK_ID_FROM_VEHICLE(freeMem->m_object_car_entities[i], &netId);
				if (!RequestNetworkControl(netId, "vehicle attachment (vehicle)")) 
					continue;
				
				DELETE_CAR((VEHICLE_INDEX*)&freeMem->m_object_car_entities[i]);
				continue;
			}
			if (DOES_CHAR_EXIST(freeMem->m_object_car_entities[i]))
			{
				GET_NETWORK_ID_FROM_PED(freeMem->m_object_car_entities[i], &netId);
				if (!RequestNetworkControl(netId, "vehicle attachment (ped)"))
					continue;
				
				DELETE_CHAR((PED_INDEX*)&freeMem->m_object_car_entities[i]);
				continue;
			}

			freeMem->m_object_car_entities[i] = NULL; //doesn't exist so clear out array.
		}

		//testing vehicle stuff again //causing crashes?
		/*
		if (DOES_VEHICLE_EXIST(myObjectCar))
		{
			GET_NETWORK_ID_FROM_VEHICLE(myObjectCar, &netId);
			if (RequestNetworkControl(netId, VehiclesModelName(myObjectCar)))
			{
				SET_CAR_VISIBLE(myObjectCar, true);
				MARK_CAR_AS_NO_LONGER_NEEDED(&myObjectCar);
				//DELETE_CAR(&myObjectCar);  //crashes, next best thing above.
			}
		}
		*/
	}
	else
	{
		for (int i = 0; i < MAX_CAR_OBJECTS; i++)
		{
			//probably need to check exists.
			//maybe make a DoesNetworkIdExist function that returns Does ped/vehicle/object exist with network id.
			netId = NULL;

			if (DOES_OBJECT_EXIST(freeMem->m_object_car_entities[i]))
				GET_NETWORK_ID_FROM_OBJECT(freeMem->m_object_car_entities[i], &netId);

			if (DOES_VEHICLE_EXIST(freeMem->m_object_car_entities[i])) 
				GET_NETWORK_ID_FROM_VEHICLE(freeMem->m_object_car_entities[i], &netId);

			if (DOES_CHAR_EXIST(freeMem->m_object_car_entities[i])) 
				GET_NETWORK_ID_FROM_PED(freeMem->m_object_car_entities[i], &netId);

			if (netId != 0 && DoesNetworkIdExist(netId))
			{
				if (HAS_CONTROL_OF_NETWORK_ID(netId))
					SET_NETWORK_ID_CAN_MIGRATE(netId, true);
			}

			freeMem->m_object_car_entities[i] = NULL; //clear out array after sorting out.
		}
	}
}

void ClearFunnyCarEntitiesAndSetVisible()
{
	NETWORK_INDEX netId;

	ClearFunnyCarEntities();

	if (DOES_VEHICLE_EXIST(myObjectCar))
	{
		GET_NETWORK_ID_FROM_VEHICLE(myObjectCar, &netId);
		if (RequestNetworkControl(netId, "object car"))
		{
			SET_CAR_VISIBLE(myObjectCar, true);
			myObjectCar = NULL;// get rid..
		}
	}
}

void SpawnFunnyVehicleModel(uint model)
{
	bool noSpawn = false;
	PED_INDEX driver;
	uint curModel;

	if (!IS_MODEL_IN_CDIMAGE(model))
	{
		printf("~r~CTL: ~w~Model is missing from your .img files.");
		return;
	}

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
		GET_DRIVER_OF_CAR(myVeh, &driver);
		GET_CAR_MODEL(myVeh, &curModel);

		if (model == curModel && driver == myPlayerPed)
		{
			if (model == VEHICLE_ANNIHILATOR)
				for (int i = 1; i < 6; i++)
					TURN_OFF_VEHICLE_EXTRA(myVeh, i, false);

			noSpawn = true;
		}
	}

	ClearFunnyCarEntities();
	if (!noSpawn)
		SpawnPlayerInVehicle(model); //deletes objects on vehicle if driving..

	GET_CAR_CHAR_IS_USING(myPlayerPed, &myObjectCar);
	if (noSpawn)
		DeleteObjectsOnVehicle(myObjectCar);

	SET_CAR_CAN_BE_DAMAGED(myObjectCar, false);
	SET_CAR_VISIBLE(myObjectCar, false);
}

//Start EvilB Funny Helis
void EBFunnyHeliAH6Littlecock() //TboGT Only//       sub_d8("AH-6 Littlecock.", 3000);
{
	SpawnFunnyVehicleModel(VEHICLE_BUZZARD);
	for (int i = 1; i < 5; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.3437f, 1.1012f, 0.3805f, 0.0f, 0.0f, 3.351f, 0);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.1493f, 0.2942f, 0.3398f, 1.7135f, 0.0f, 0.2438f, 1);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, 0.0f, -0.325f, 0.3267f, -4.7032f, 0.0f, 0.2298f, 2);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, 0.1435f, -1.0018f, 0.431f, 1.416f, 0.0f, 0.2197f, 3);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.171f, 0.2617f, 0.1005f, 1.637f, 3.248f, 0.2207f, 4);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.4375f, 0.1295f, 0.1295f, -1.3913f, 0.0f, 3.374f, 5);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, 0.1693f, -1.09f, 0.2258f, -1.321f, 0.0f, 3.3865f, 6);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.2448f, -1.988f, 0.0297f, 0.4713f, -2.4f, 0.14f, 7);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, 0.8575f, -1.8052f, 0.032f, -2.6825f, 0.69f, -2.9902f, 8);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, 0.2117f, -1.374f, 0.5335f, 2.0323f, 0.0f, -2.9238f, 9);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.2523f, -1.9418f, 0.0557f, -2.1635f, 0.828f, 0.0f, 10);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, 0.8288f, -1.6587f, 0.0f, -2.2305f, -0.654f, 0.2705f, 11);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, 0.8742f, -1.9458f, -0.4058f, -2.124f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, 0.9067f, -1.6738f, -0.4605f, 1.2005f, 3.1652f, 0.0f, 13);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.2318f, -2.2197f, -0.3553f, 3.9433f, 0.228f, 0.3678f, 14);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.2822f, -2.0342f, -0.3845f, 1.274f, 3.2145f, 0.2603f, 15);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.3447f, 1.0117f, -0.0518f, 4.4253f, 0.0f, 0.1975f, 16);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.445f, 1.5892f, 0.1203f, 0.9937f, 0.0f, 0.229f, 17);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.4278f, 1.4965f, -0.2105f, -1.4438f, 0.0f, 3.42f, 18);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.4905f, 1.8605f, -0.2715f, 0.359f, 0.0f, 0.2018f, 19);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.518f, 2.028f, -0.0945f, 2.0135f, 0.0f, -2.9728f, 20);
	EBCreateFunnyCarEntity(OBJECT_AH_SINCHAIR, -0.5345f, 2.0222f, -0.2635f, -1.0298f, 0.0f, 0.2138f, 21);
	EBCreateFunnyCarEntity(OBJECT_VENTSTEAM_REF_MS, -0.7973f, 1.749f, -0.3205f, 1.0925f, 0.0f, 0.454f, 22);

	printf("~r~CTL: ~w~AH-6 Littlecock by ~y~cerberusxxxx ~w~(23 Objects)");
}

void EBFunnyHeliBehemoth() //TBoGT only
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(OBJECT_SUBLIGHT_MS, 0.0f, 4.6394f, -0.3113f, 0.0f, 0.0f, -3.1082f, 0);
	EBCreateFunnyCarEntity(OBJECT_SUBLIGHT_MS, 0.0f, 4.6394f, -0.3113f, 0.0f, 0.0f, -3.1082f, 1);
	EBCreateFunnyCarEntity(OBJECT_SUBLIGHT_MS, 0.0f, 4.6394f, -0.3113f, 0.0f, 0.0f, -3.1082f, 2);
	EBCreateFunnyCarEntity(OBJECT_SUBLIGHT_MS, 0.0f, 4.6394f, -0.3113f, 0.0f, 0.0f, -3.1082f, 3);
	EBCreateFunnyCarEntity(OBJECT_SUBLIGHT_MS, 0.0f, 4.6394f, -0.3113f, 0.0f, 0.0f, -3.1082f, 4);
	EBCreateFunnyCarEntity(OBJECT_BM_SATDISH01, 0.0f, 0.0f, 2.3539f, 0.0f, 0.0f, 3.1184f, 5);
	EBCreateFunnyCarEntity(OBJECT_PRIS_SPOTBIG, -1.2974f, 4.0409f, 0.9074f, 1.6306f, -0.9536f, -1.5662f, 6);
	EBCreateFunnyCarEntity(OBJECT_PRIS_SPOTBIG, 1.2024f, 4.1229f, 0.8354f, -1.4642f, -1.0133f, -1.5628f, 7);
	EBCreateFunnyCarEntity(OBJECT_PRIS_SPOTBIG, -1.1889f, 3.9371f, -0.3575f, 1.5281f, -0.9335f, -1.5666f, 8);
	EBCreateFunnyCarEntity(OBJECT_PRIS_SPOTBIG, 1.2579f, 3.8845f, -0.3258f, -1.5843f, -1.0212f, -1.5707f, 9);
	EBCreateFunnyCarEntity(OBJECT_EC_JET, 0.0f, -5.643f, 2.5964f, 0.0f, 3.1347f, 3.1503f, 10);
	EBCreateFunnyCarEntity(OBJECT_LEV6LDBARELBURN, -1.3242f, -9.4783f, -0.5824f, 1.4607f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(OBJECT_LEV6LDBARELBURN, 1.3546f, -9.6245f, -0.4061f, 1.6752f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(OBJECT_PRIS_SPOTBIG, -0.0061f, 3.7634f, 1.2122f, 3.2008f, 3.1577f, 1.5689f, 13);
	EBCreateFunnyCarEntity(OBJECT_PRIS_SPOTBIG, -0.0829f, 1.5518f, 2.27f, 4.7299f, 1.534f, 0.0f, 14);
	EBCreateFunnyCarEntity(OBJECT_PRIS_SPOTBIG, -1.0956f, -5.3122f, 2.2778f, 1.7843f, 1.526f, 0.0f, 15);
	EBCreateFunnyCarEntity(OBJECT_PRIS_SPOTBIG, 1.341f, -5.3144f, 2.2401f, 1.9157f, 1.6297f, 0.0f, 16);
	EBCreateFunnyCarEntity(0x2189d250, 0.0f, 2.4897f, 0.2933f, -0.2849f, 0.0f, 0.0f, 17); //entries 17 (this line) to 25 are 50b3/ vehicles not objects. //VEHICLE_APC?
	EBCreateFunnyCarEntity(0x2189d250, 0.0f, 0.0f, 0.5324f, 0.0f, 0.0f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x2189d250, -0.4916f, -0.0158f, 0.5305f, 0.0f, -1.2515f, 0.0f, 19);
	EBCreateFunnyCarEntity(0x2189d250, 0.0f, 0.0f, 0.9351f, 0.0f, 0.0f, 0.0f, 20);
	EBCreateFunnyCarEntity(0x2189d250, 0.4049f, -0.0108f, 0.6065f, 0.0f, 1.2622f, 0.0f, 21);
	EBCreateFunnyCarEntity(0x2189d250, 0.0f, 0.0772f, 0.2549f, 0.0f, 3.1403f, 0.0f, 22);
	EBCreateFunnyCarEntity(0x2189d250, 0.0f, 2.5493f, 0.2928f, -0.1616f, -3.1409f, 0.0f, 23);
	EBCreateFunnyCarEntity(0x2189d250, -0.1045f, -4.4132f, 0.3718f, -0.2054f, 1.2569f, 3.1244f, 24);
	EBCreateFunnyCarEntity(0x2189d250, 0.0766f, -4.2935f, 0.3953f, -0.207f, -1.1141f, 3.1497f, 25);
	printf("~r~CTL: ~w~Behemoth by ~y~cerberusxxxx ~w~(26 Objects)");
}

void EBFunnyHeliCouch()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	EBCreateFunnyCarEntity(OBJECT_CJ_COUCH8, 1.105f, 2.89001f, 0.064999f, 0.0f, 0.0f, 3.243f, 0);
	printf("~r~CTL: ~w~Couch Heli by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyHeliSpiderFly()
{
	SpawnFunnyVehicleModel(VEHICLE_MAVERICK);
	for (int i = 1; i < 5; i++) //yeah they only do 1-4 on this one for some reason
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0x1b9f769f, 0.0f, 2.2225f, 1.0f, 0.0f, 1.19f, -1.2303f, 0);
	EBCreateFunnyCarEntity(0xdb02c6ba, 0.0f, 4.0073f, 2.4143f, 0.0f, 0.0f, 3.2363f, 1);
	EBCreateFunnyCarEntity(0xb76637d8, 1.1777f, 2.7323f, 2.6147f, 0.0f, 0.0f, -1.453f, 2);
	EBCreateFunnyCarEntity(0xb76637d8, -0.8723f, 2.4218f, 2.569f, 0.0f, 0.0f, 4.8352f, 3);
	EBCreateFunnyCarEntity(0xe6d1aca3, 2.9688f, 1.9578f, 1.0f, 0.0f, -0.4043f, 0.1972f, 4);
	EBCreateFunnyCarEntity(0xe6d1aca3, 4.1048f, 2.1643f, 0.7323f, 0.0f, 0.8742f, 0.1977f, 5);
	EBCreateFunnyCarEntity(0xe6d1aca3, -2.4285f, 0.8425f, 1.0f, 0.0f, 0.4033f, 0.0945f, 6);
	EBCreateFunnyCarEntity(0xe6d1aca3, -3.5605f, 0.763f, 0.704f, 0.0f, -0.876f, 0.05f, 7);
	EBCreateFunnyCarEntity(0xe6d1aca3, -1.5105f, -1.101f, 1.0f, 0.0f, 0.4037f, 0.7305f, 8);
	EBCreateFunnyCarEntity(0xe6d1aca3, 3.0925f, -0.2095f, 1.0f, 0.0f, -0.4037f, -0.415f, 9);
	EBCreateFunnyCarEntity(0xe6d1aca3, 4.177f, -0.6847f, 0.6973f, 0.0f, 0.8748f, -0.4035f, 10);
	EBCreateFunnyCarEntity(0xe6d1aca3, -2.3868f, -1.8717f, 0.7095f, 0.0f, -0.877f, 0.6865f, 11);
	EBCreateFunnyCarEntity(0xb76637d8, -2.496f, -1.937f, -0.185f, 0.0f, -1.5785f, 0.6977f, 12);
	EBCreateFunnyCarEntity(0xb76637d8, 4.3313f, -0.6927f, -0.2173f, 3.0825f, -1.5775f, -0.4545f, 13);
	EBCreateFunnyCarEntity(0xb76637d8, 4.31f, 2.1488f, -0.2805f, 0.0f, -1.5787f, 3.4593f, 14);
	EBCreateFunnyCarEntity(0xb76637d8, -3.763f, 0.8055f, -0.2903f, 0.0f, -1.5772f, 0.0637f, 15);
	EBCreateFunnyCarEntity(0x1ae7947b, -1.235f, 5.4983f, -0.7142f, -0.3905f, 0.0f, -0.1145f, 16);
	printf("~r~CTL: ~w~Spider Fly by ~y~ZzLoik69zZ ~w~(17 Objects)");
}

void EBFunnyHeliFlyingCarpet()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	EBCreateFunnyCarEntity(0x51562026, -0.7f, 1.7f, 0.5f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x51562026, -0.600001f, 1.9f, 0.405001f, 3.157f, 0.0f, 0.129f, 1);
	EBCreateFunnyCarEntity(0x81f4e985, -0.655004f, 3.16503f, 0.745002f, 0.129f, 0.0f, 2.408f, 2);
	for (int i = 0; i < 3; i++)
		SET_OBJECT_COLLISION(freeMem->m_object_car_entities[i], true);

	printf("~r~CTL: ~w~Flying Carpet by ~y~Anon Reporter ~w~(3 Objects)");
}

void EBFunnyHeliUFOFooFighter() //TBoGT only
{
	if (myCurrentEpisode == 2)
	{
		SpawnFunnyVehicleModel(VEHICLE_BUZZARD);
		for (int i = 1; i < 5; i++)
			TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

		EBCreateFunnyCarEntity(0x1553c997, 0.0f, 0.0f, 1.0f, 0.0f, 1.0637f, 1.583f, 0);
		EBCreateFunnyCarEntity(0x1553c997, 0.0f, 0.1018f, 6.0528f, 0.0065f, 4.207f, 1.5783f, 1);
		EBCreateFunnyCarEntity(0xe946e6ba, 0.0f, 0.1088f, 4.894f, 0.0f, 0.0f, 0.0f, 2);
		EBCreateFunnyCarEntity(0xe946e6ba, 0.0f, 0.0815f, 2.1438f, 0.0363f, 3.1292f, 0.0155f, 3);
		EBCreateFunnyCarEntity(0xd6ec4205, 1.9272f, 0.159f, 3.5307f, 0.0f, 1.5723f, 0.0f, 4);
		EBCreateFunnyCarEntity(0xd6ec4205, -1.8502f, -0.0115f, 3.5077f, 0.0f, 4.696f, 0.0f, 5);
		EBCreateFunnyCarEntity(0xd6ec4205, 0.0f, -1.8088f, 3.515f, 1.5415f, 0.0f, 0.0f, 6);
		EBCreateFunnyCarEntity(0xd6ec4205, 0.0f, 1.8472f, 3.4968f, 4.6777f, 0.0f, 0.0f, 7);
		EBCreateFunnyCarEntity(0xed723bb4, 0.0f, -3.3308f, 3.5538f, 1.527f, 0.0f, 0.0f, 8);
		EBCreateFunnyCarEntity(0xed723bb4, 0.8138f, -3.524f, 3.5848f, 1.5207f, 0.0f, 0.0f, 9);
		EBCreateFunnyCarEntity(0xed723bb4, -1.0337f, -3.6732f, 3.5008f, 1.4975f, 0.0f, 0.0f, 10);
		EBCreateFunnyCarEntity(0x3a8020ff, 4.452f, 0.0588f, 3.196f, 0.0f, 2.1342f, 0.0f, 11);
		EBCreateFunnyCarEntity(0x3a8020ff, -4.192f, 0.0f, 3.2363f, 0.0f, 2.1477f, 3.16f, 12);
		EBCreateFunnyCarEntity(0x3a8020ff, -0.121f, 4.0915f, 3.1528f, 0.0f, 2.1838f, -1.501f, 13);
		EBCreateFunnyCarEntity(0x3a8020ff, 0.0f, -4.2965f, 3.2215f, 0.0f, 2.102f, -1.6205f, 14);
		printf("~r~CTL: ~w~UFO (Foo Fighter) by ~y~cerberusxxxx ~w~(15 Objects)");
	}
	else
		printf("~r~CTL: ~w~TBoGT Only.");
}

void EBFunnyHeliUFOSmooth() //TBoGT only
{
	if (myCurrentEpisode == 2)
	{
		SpawnFunnyVehicleModel(VEHICLE_BUZZARD);
		for (int i = 1; i < 5; i++)
			TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

		EBCreateFunnyCarEntity(0x948f1a0d, 0.0f, 1.0505f, 0.4835f, 0.0f, 0.0f, 0.0f, 0);
		EBCreateFunnyCarEntity(0x948f1a0d, 0.0f, -4.626f, 0.4835f, 0.0f, 0.0f, 3.125f, 1);
		EBCreateFunnyCarEntity(0x948f1a0d, 3.0905f, -1.6948f, 0.4835f, 0.0f, 0.0f, -1.5695f, 2);
		EBCreateFunnyCarEntity(0x948f1a0d, -2.987f, -1.8038f, 0.4835f, 0.0f, 0.0f, 1.613f, 3);
		EBCreateFunnyCarEntity(0x948f1a0d, 1.8492f, 0.141f, 0.4835f, 0.0f, 0.0f, -0.765f, 4);
		EBCreateFunnyCarEntity(0x948f1a0d, -2.1125f, -3.7028f, 0.4835f, 0.0f, 0.0f, 2.3618f, 5);
		EBCreateFunnyCarEntity(0x948f1a0d, -2.0143f, 0.2683f, 0.4835f, 0.0f, 0.0f, 0.7537f, 6);
		EBCreateFunnyCarEntity(0x948f1a0d, 1.923f, -3.8093f, 0.4835f, 0.0f, 0.0f, -2.4218f, 7);
		EBCreateFunnyCarEntity(0xd6ec4205, 0.0f, 0.0f, 0.754f, -1.9395f, 0.0f, 0.0f, 8);
		EBCreateFunnyCarEntity(0xd6ec4205, 0.0f, -2.795f, 0.754f, 1.8172f, 0.0f, 0.0f, 9);
		EBCreateFunnyCarEntity(0xd6ec4205, 1.259f, -1.5693f, 0.754f, 0.0f, 1.7672f, 0.0f, 10);
		EBCreateFunnyCarEntity(0xd6ec4205, -1.4145f, -1.874f, 0.754f, 0.0f, -1.9273f, 0.0f, 11);
		EBCreateFunnyCarEntity(0xe946e6ba, -2.2935f, -1.777f, -0.3858f, 0.0f, 1.674f, 0.0f, 12);
		printf("~r~CTL: ~w~UFO (Smooth) by ~y~cerberusxxxx ~w~(13 Objects)");
	}
	else
		printf("~r~CTL: ~w~TBoGT Only.");
}

void EBFunnyHeliHarryPotterBroomStick()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true); //idk if any point to this but eh whatever.

	EBCreateFunnyCarEntity(0xe6f19814, -0.665f, 2.4443f, -0.0263f, -0.7775f, 0.0f, 0.0718f, 0);
	printf("~r~CTL: ~w~Harry Potter Broomstick by ~y~DsA x Hotsauce ~w~(1 Object)");
}

void EBFunnyHeliHotAirBalloon()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true); //idk if any point to this but eh whatever.

	EBCreateFunnyCarEntity(0x125365ae, 0.186f, 2.529f, -0.867f, 0.0f, 0.0f, -1.5485f, 0);
	SET_OBJECT_COLLISION(freeMem->m_object_car_entities[0], true);
	EBCreateFunnyCarEntity(0xf814ad1e, -1.31f, 1.4173f, 0.59f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xf814ad1e, 1.7813f, 1.511f, 0.5862f, 0.0f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xf814ad1e, -1.337f, 3.7212f, 0.6127f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xf814ad1e, 1.7298f, 3.7858f, 0.6003f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x7c772d51, 0.7382f, 2.2675f, 45.794f, 3.1317f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x7c772d51, 0.8155f, 3.0235f, -22.3382f, 0.0f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.262f, 2.491f, 3.4622f, 3.1608f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.262f, 2.491f, 3.4622f, 3.1608f, 0.0f, 0.0f, 8);
	printf("~r~CTL: ~w~Hot-Air Balloon by ~y~Ti jo P1CK UP ~w~(9 Objects)");
}

void EBFunnyHeliWhale()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0xc2f91b5f, -0.042f, -8.7f, 5.5625f, 0.0502f, -0.2623f, -1.5655f, 0);
	EBCreateFunnyCarEntity(0x8f5ff909, 0.0f, -7.5817f, 2.5588f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x8f5ff909, 0.0f, -7.487f, 1.123f, 1.2947f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x8f5ff909, 0.0f, -5.128f, 0.7263f, 0.0f, 3.5395f, 1.931f, 3);
	EBCreateFunnyCarEntity(0x8f5ff909, 0.765f, -3.1538f, 1.0f, 0.0f, -0.589f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x8f5ff909, -1.4678f, -3.4845f, 0.909f, 0.1793f, -0.2058f, -0.249f, 5);
	EBCreateFunnyCarEntity(0x43ec0bea, 0.2875f, -3.1975f, 0.6322f, 0.0f, 0.0f, 1.754f, 6);
	EBCreateFunnyCarEntity(0xb76637d8, -0.5755f, 0.7928f, 1.9835f, 0.705f, 0.1813f, -1.1457f, 7);
	EBCreateFunnyCarEntity(0xb76637d8, 0.597f, 0.829f, 1.856f, -0.6578f, 0.2408f, -2.0855f, 8);
	EBCreateFunnyCarEntity(0xd20167be, 0.7295f, 0.5483f, 2.2188f, -1.449f, 0.0785f, -0.4018f, 9);
	EBCreateFunnyCarEntity(0xd20167be, -0.6602f, 0.57f, 2.3978f, -1.6835f, 0.0f, 0.5118f, 10);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, -8.0465f, 3.8243f, 1.0117f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, -7.4833f, 3.1243f, 0.676f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0xf2cff3e4, 0.0f, 1.5267f, 2.729f, 0.0f, 0.0f, 1.907f, 13);
	EBCreateFunnyCarEntity(0x63401612, 0.8535f, 3.083f, 0.8673f, 0.0055f, 2.8823f, -2.0415f, 14);
	EBCreateFunnyCarEntity(0xed723bb4, 1.58f, 4.4895f, 0.4127f, -0.258f, 0.1663f, 0.0f, 15);
	EBCreateFunnyCarEntity(0xef72f88a, 0.0f, -7.0432f, 0.1215f, 3.8893f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0xef72f88a, -0.3883f, -7.3672f, -0.2902f, -0.0625f, 0.5193f, 0.0f, 17);
	EBCreateFunnyCarEntity(0xef72f88a, 0.311f, -7.4845f, -0.3125f, -0.1928f, -0.3485f, 0.0f, 18);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, -6.6507f, -0.2975f, -2.4023f, 0.0f, 0.0f, 19);
	EBCreateFunnyCarEntity(0x67ab0048, 0.137f, -5.7218f, 1.0f, 0.0667f, -1.1905f, 1.4222f, 20);
	EBCreateFunnyCarEntity(0x67ab0048, -0.3155f, -3.6435f, 1.1382f, 0.0f, -1.1955f, 0.0f, 21);
	EBCreateFunnyCarEntity(0x67ab0048, 0.359f, -2.9455f, 0.7007f, 0.0f, -1.0622f, -3.157f, 22);
	EBCreateFunnyCarEntity(0xd57ceb37, -5.0905f, -3.644f, -0.324f, 0.0f, 0.6115f, -1.6648f, 23);
	EBCreateFunnyCarEntity(0xd57ceb37, -0.467f, -10.2612f, -0.2735f, 0.0f, 0.0f, -1.664f, 24);
	EBCreateFunnyCarEntity(0xd57ceb37, -0.0835f, -10.2432f, -0.2625f, 0.0f, 0.0f, 1.5242f, 25);
	EBCreateFunnyCarEntity(0xd57ceb37, 5.2653f, -3.0218f, -0.3663f, 0.0f, -0.3263f, 1.5817f, 26);
	EBCreateFunnyCarEntity(0xeef0ac28, 0.2673f, 0.806f, 10.011f, -0.2973f, 0.0f, 0.0f, 27);
	EBCreateFunnyCarEntity(0xeef0ac28, 0.2368f, -6.9622f, 12.3625f, -0.268f, 0.0f, 0.0f, 28);
	EBCreateFunnyCarEntity(0xeef0ac28, 0.046f, -13.0118f, 10.4542f, -0.2567f, 0.0f, 0.0f, 29);
	printf("~r~CTL: ~w~Whale Heli by ~y~cerberusxxxx ~w~(30 Objects)");
}

void EBFunnyHeliRecliner()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0x1f399d95, -0.2f, 2.74501f, 0.064999f, 0.0f, 0.0f, 3.24f, 0);
	printf("~r~CTL: ~w~Recliner by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyHeliSpaceship()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0x206c172, 2.1922f, 0.8512f, 0.5925f, -1.5693f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x206c172, -2.2075f, 0.8512f, 0.5932f, -1.5685f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x206c172, 1.1255f, 1.706f, 0.2115f, 1.568f, -0.7632f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x206c172, -1.1583f, 1.7152f, 0.2115f, 1.5687f, 0.7633f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x1d5cdea4, -0.2107f, 1.7807f, -0.1833f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x1d5cdea4, 0.1768f, 1.7805f, -0.1833f, 0.0f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x2553daa9, -0.6845f, 2.69f, 0.2757f, 0.0f, 0.0f, -3.2055f, 6);
	EBCreateFunnyCarEntity(0x2553daa9, 0.6835f, 2.6905f, 0.2753f, 0.0f, 0.0f, -3.2055f, 7);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 2.3503f, 0.101f, -1.5675f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 2.421f, 0.121f, 0.297f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 2.1825f, 0.8397f, -0.4305f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 2.454f, 1.5537f, -1.5685f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 3.1258f, 0.0952f, -1.4725f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 3.8715f, 0.1655f, 0.2103f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 3.7208f, 0.9077f, 0.6227f, 0.0f, 0.0f, 14);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 2.3417f, -0.0728f, -1.562f, 0.0f, 0.0f, 15);
	EBCreateFunnyCarEntity(0xb76637d8, 0.8375f, 3.0605f, -0.4495f, 0.0f, 0.0f, -1.5505f, 16);
	EBCreateFunnyCarEntity(0xb76637d8, -0.704f, 3.0608f, -0.4498f, 0.0f, 0.0f, -1.5507f, 17);
	EBCreateFunnyCarEntity(0x81e9e088, -2.6215f, 1.5527f, 0.57f, 1.6005f, 0.0f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x81e9e088, 2.6593f, 1.5527f, 0.57f, 1.6005f, 0.0f, 0.0f, 19);
	EBCreateFunnyCarEntity(0xf0083468, -2.625f, 2.0482f, 0.5832f, -1.5788f, -1.608f, 0.0f, 20);
	EBCreateFunnyCarEntity(0xf0083468, 2.6675f, 2.0482f, 0.5832f, -1.5788f, -1.608f, 0.0f, 21);
	EBCreateFunnyCarEntity(0xd25a681, 0.0f, 1.0145f, -0.1473f, 0.0f, 0.0f, 0.0f, 22);
	EBCreateFunnyCarEntity(0xb8eee857, -0.002f, 3.9178f, 0.012f, 0.0f, 0.0f, 3.2515f, 23);
	printf("~r~CTL: ~w~Spaceship by ~y~ZzLoik69zZ ~w~(24 Objects)");
}

void EBFunnyHeliCavalcade()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0x779f23aa, -0.2f, 2.74501f, 0.5f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Cavalcade Heli by ~y~Anon Reporter ~w~(1 Vehicle)");
}

void EBFunnyHeliDinghy()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0x3d961290, -0.345f, 3.32501f, -0.0800014f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Dinghy Heli by ~y~Anon Reporter ~w~(1 Vehicle)");
}

void EBFunnyHeliOVni()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0x26b6b845, -0.6305f, 3.2118f, 0.0565f, -1.5145f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x26b6b845, -0.612f, 3.1973f, 0.049f, -1.5145f, 0.0f, -3.1413f, 1);
	EBCreateFunnyCarEntity(0x26b6b845, -0.5858f, 3.1895f, 0.0657f, -1.5145f, 0.0f, -1.592f, 2);
	EBCreateFunnyCarEntity(0x26b6b845, -0.565f, 3.1877f, 0.0235f, -1.5145f, 0.0f, 1.591f, 3);
	EBCreateFunnyCarEntity(0xfeb2fdaf, -0.7043f, 3.0423f, 0.2317f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xfeb2fdaf, -0.7043f, 2.8868f, 1.0f, 0.0f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xcb26803d, -0.6333f, 1.634f, 0.0057f, 0.0f, -1.4705f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xcb26803d, -0.6188f, 4.7293f, -0.0035f, 0.0f, -1.471f, 0.0f, 7);
	EBCreateFunnyCarEntity(0xcb26803d, 0.942f, 3.132f, -0.004f, 0.0f, -1.4795f, 0.0f, 8);
	EBCreateFunnyCarEntity(0xcb26803d, -2.0673f, 3.15f, -0.0555f, 0.0f, -1.6278f, 0.0f, 9);
	EBCreateFunnyCarEntity(0xb76637d8, -0.6738f, 2.681f, 0.0862f, 0.0f, -1.557f, -1.5518f, 10);
	printf("~r~CTL: ~w~Ovni by ~y~ZzLoik69zZ ~w~(11 Objects)");
}

void EBFunnyHeliMiniFalcon()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0xd3031ea2, 0.0f, 0.0f, 2.8148f, 0.0f, 0.0f, 1.5665f, 0);
	EBCreateFunnyCarEntity(0xd3031ea2, 0.0f, 0.0f, 4.017f, 3.146f, 0.0f, 1.5678f, 1);
	EBCreateFunnyCarEntity(0x29a565bc, -2.074f, -4.7068f, 3.416f, -0.4503f, -1.555f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x29a565bc, -0.4787f, -5.1427f, 3.464f, -0.2503f, -1.5895f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x29a565bc, 0.3563f, -5.1295f, 3.4378f, -0.1865f, 1.5815f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x29a565bc, 2.002f, -4.7597f, 3.4265f, -0.4922f, 1.5512f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x29a565bc, -0.8245f, -5.0895f, 3.4358f, 0.0f, 1.5488f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x3c990668, 0.0f, -5.1545f, 3.4105f, -1.5963f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x3c990668, 0.8715f, -5.1057f, 3.418f, -1.5513f, 0.0f, 0.1665f, 8);
	EBCreateFunnyCarEntity(0x3c990668, 1.6942f, -4.9475f, 3.406f, -1.5323f, 0.0f, 0.2973f, 9);
	EBCreateFunnyCarEntity(0x3c990668, 2.4335f, -4.6322f, 3.386f, -1.529f, 0.0f, 0.5207f, 10);
	EBCreateFunnyCarEntity(0x3c990668, 3.252f, -4.1635f, 3.4488f, -1.5605f, 0.0f, 0.5493f, 11);
	EBCreateFunnyCarEntity(0x3c990668, -0.8695f, -5.077f, 3.4443f, -1.5363f, 0.0f, -0.1887f, 12);
	EBCreateFunnyCarEntity(0x3c990668, -1.757f, -4.8952f, 3.3953f, -1.6348f, 0.0f, -0.3708f, 13);
	EBCreateFunnyCarEntity(0x3c990668, -2.559f, -4.4467f, 3.3613f, -1.5503f, 0.0f, -0.474f, 14);
	EBCreateFunnyCarEntity(0x3c990668, -3.1228f, -4.1653f, 3.388f, -1.5618f, 0.0f, -0.537f, 15);
	EBCreateFunnyCarEntity(0xafdf659e, 2.5305f, 4.0357f, 3.893f, 1.5695f, 0.0f, 1.8912f, 16);
	EBCreateFunnyCarEntity(0xafdf659e, 2.3838f, 6.0715f, 3.8833f, 1.55f, 0.0f, -1.6122f, 17);
	EBCreateFunnyCarEntity(0xafdf659e, 2.8623f, 6.9483f, 3.881f, 1.543f, 0.0f, -1.2635f, 18);
	EBCreateFunnyCarEntity(0xafdf659e, 2.395f, 6.386f, 3.173f, 1.55f, 0.0f, -1.6175f, 19);
	EBCreateFunnyCarEntity(0xafdf659e, 1.687f, 6.4823f, 3.1703f, 1.5693f, 0.0f, 1.877f, 20);
	EBCreateFunnyCarEntity(0xafdf659e, 3.682f, 4.4678f, 3.1435f, 1.5697f, 0.0f, -1.2576f, 21);
	EBCreateFunnyCarEntity(0xafdf659e, -0.7635f, 6.172f, 3.9148f, 1.5618f, 0.0f, -1.579f, 22);
	EBCreateFunnyCarEntity(0xafdf659e, -3.241f, 4.9528f, 3.8925f, 1.5482f, 0.0f, 1.209f, 23);
	EBCreateFunnyCarEntity(0xafdf659e, -1.2685f, 6.7257f, 3.9135f, 1.5692f, 0.0f, -1.9068f, 24);
	EBCreateFunnyCarEntity(0xafdf659e, -2.0515f, 6.4657f, 3.1858f, 1.5987f, 0.0f, 1.5805f, 25);
	EBCreateFunnyCarEntity(0x58c7ad74, -2.2305f, 1.6292f, 3.9655f, 0.0f, 0.0f, 3.8068f, 26);
	EBCreateFunnyCarEntity(0xafdf659e, -3.1815f, 4.86f, 3.1065f, 1.5603f, 0.0f, 1.197f, 27);
	EBCreateFunnyCarEntity(0xafdf659e, -2.485f, 7.091f, 3.2278f, 1.6115f, 0.0f, 1.2145f, 28);
	EBCreateFunnyCarEntity(0x5a22016d, 4.4997f, 2.8537f, 2.732f, 0.0f, -0.1093f, 0.9482f, 29);
	EBCreateFunnyCarEntity(0x5a22016d, 4.342f, 2.7058f, 4.2397f, 3.124f, -0.1885f, 0.918f, 30);
	EBCreateFunnyCarEntity(0x2d593870, 0.0f, 0.0f, 2.5483f, 1.6007f, 0.0f, 0.0f, 31);
	EBCreateFunnyCarEntity(0x61bbb992, -0.7475f, -3.0088f, 3.324f, 0.202f, 0.0f, 0.0f, 32);
	EBCreateFunnyCarEntity(0x61bbb992, 0.7405f, -2.9625f, 3.305f, 0.22f, 0.0f, 0.0f, 33);
	EBCreateFunnyCarEntity(0x61bbb992, 0.0f, -4.034f, 3.2903f, 0.0f, 0.0f, 0.0f, 34);
	EBCreateFunnyCarEntity(0x61bbb992, -1.9105f, -3.5263f, 3.2738f, 0.1485f, 0.0f, 0.0f, 35);
	EBCreateFunnyCarEntity(0x61bbb992, 1.8162f, -3.603f, 3.2555f, 0.159f, 0.0f, 0.0f, 36);
	printf("~r~CTL: ~w~Mini Falcon by ~y~cerberusxxxx ~w~(37 Objects)");
}

void EBFunnyHeliMarquis()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0xc1ce1183, 0.220017f, 9.50038f, -0.420037f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Marquis Heli by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyHeliHeliPlane()
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0xa1e4599e, -0.3115f, -3.988f, -0.9867f, 0.0f, 0.0f, 3.1243f, 0);
	EBCreateFunnyCarEntity(0xedf775cb, -0.5752f, -3.5473f, 0.414f, 0.0f, 1.0545f, -1.6255f, 1);
	EBCreateFunnyCarEntity(0x1ae7947b, -1.7143f, -8.3475f, 2.0757f, 0.0f, 1.5455f, -1.565f, 2);
	EBCreateFunnyCarEntity(0x1ae7947b, -1.7143f, -8.3475f, 2.0757f, 0.0f, 1.5455f, -1.565f, 3);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.8927f, -8.3475f, 2.0757f, 0.0f, 1.5455f, -1.565f, 4);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.8927f, -8.3475f, 2.0757f, 0.0f, 1.5455f, -1.565f, 5);
	printf("~r~CTL: ~w~Heli Plane by ~y~R3B3LY0N ~w~(6 Objects)");
}

void EBFunnyHeliCableCar()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	EBCreateFunnyCarEntity(0xc6c3242d, -0.2f, 0.580036f, 8.40033f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Cable Car Heli by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyHeliMinecraftPlane() //move up away from setup functions after rigging
{
	SpawnFunnyVehicleModel(0x31f0b376);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	uint Model = OBJECT_CJ_GAME_CUBE_C1;
	int Colour = minecraftFunnyColourSelect;

	switch (Colour)
	{
	case 0: Model = OBJECT_CJ_GAME_CUBE_1; break; //Red (change _C1 to _1 to add original glow back). 
	case 1: Model = OBJECT_CJ_GAME_CUBE_2; break; //Blue 
	case 2: Model = OBJECT_CJ_GAME_CUBE_3; break; //Yellow
	case 3: Model = OBJECT_CJ_GAME_CUBE_4; break; //Green
	case 4: Model = OBJECT_CJ_GAME_CUBE_5; break; //Pink
	case 5: Model = OBJECT_CJ_GAME_CUBE_6; break; //Orange
	}

	EBCreateFunnyCarEntity(Model, -0.6462f, 4.005f, 0.341f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(Model, 0.1615f, 2.9932f, 0.341f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(Model, -1.4858f, 2.9932f, 0.341f, 0.0f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(Model, -0.6462f, 2.0627f, 0.341f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(Model, -0.6462f, 1.099f, 0.341f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(Model, -0.6462f, 0.0975f, 0.341f, 0.0f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(Model, -0.6462f, 0.0975f, 1.1765f, 0.0f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(Model, 1.0865f, 2.9932f, 0.341f, 0.0f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(Model, -2.4088f, 2.9932f, 0.341f, 0.0f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(Model, -0.6462f, 2.9932f, 0.9218f, 0.0f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.6492f, 4.6f, 0.4195f, 0.0f, 0.0f, 4.7185f, 10);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.6663f, 4.5955f, 0.3312f, 1.5898f, 0.0f, -1.7062f, 11);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.67f, 4.6075f, 0.3893f, -0.8255f, 0.0f, 1.626f, 12);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.5505f, 4.604f, 0.387f, 0.736f, 0.0f, 1.5295f, 13);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.0402f, 3.0082f, -0.809f, 0.0f, -3.104f, 0.0f, 14);
	EBCreateFunnyCarEntity(0x7f53dd7e, -1.28f, 3.0082f, -0.809f, 0.0f, -3.104f, 0.0f, 15);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.5812f, 3.0195f, -0.4258f, 0.4347f, 0.0f, 1.6018f, 16);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.6525f, 3.0195f, -0.3958f, -0.4443f, 0.0f, 1.6018f, 17);

	for (int i = 0; i < 10; i++)
		SET_OBJECT_LIGHTS(freeMem->m_object_car_entities[i], bitTest(ctlMem->m_bit_sets.m_vehicle, minecraftFunnyCarGlow)); //works but only on some and of a night time..

	printf("~r~CTL: ~w~Minecraft Plane (%s) by ~y~EroiK I StyLeR~w~ (18 Objects)", MinecraftColourStrings[minecraftFunnyColourSelect]); //I don't start at 0 for this functions paramater.
}

void EBFunnyHeliCaterpillarFly()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	EBCreateFunnyCarEntity(0x1b9f769f, 0.0f, 2.2225f, 1.0f, 0.0f, 1.19f, -1.2303f, 0);
	EBCreateFunnyCarEntity(0xdb02c6ba, 0.0f, 4.0073f, 2.4143f, 0.0f, 0.0f, 3.2363f, 1);
	EBCreateFunnyCarEntity(0xb76637d8, 1.1777f, 2.7323f, 2.6147f, 0.0f, 0.0f, -1.453f, 2);
	EBCreateFunnyCarEntity(0xb76637d8, -0.8723f, 2.4218f, 2.569f, 0.0f, 0.0f, 4.8352f, 3);
	EBCreateFunnyCarEntity(0xe6d1aca3, 2.9688f, 1.9578f, 1.0f, 0.0f, -0.4043f, 0.1972f, 4);
	EBCreateFunnyCarEntity(0xe6d1aca3, 4.1048f, 2.1643f, 0.7323f, 0.0f, 0.8742f, 0.1977f, 5);
	EBCreateFunnyCarEntity(0xe6d1aca3, -2.4285f, 0.8425f, 1.0f, 0.0f, 0.4033f, 0.0945f, 6);
	EBCreateFunnyCarEntity(0xe6d1aca3, -3.5605f, 0.763f, 0.704f, 0.0f, -0.876f, 0.05f, 7);
	EBCreateFunnyCarEntity(0xe6d1aca3, -1.5105f, -1.101f, 1.0f, 0.0f, 0.4037f, 0.7305f, 8);
	EBCreateFunnyCarEntity(0xe6d1aca3, 3.0925f, -0.2095f, 1.0f, 0.0f, -0.4037f, -0.415f, 9);
	EBCreateFunnyCarEntity(0xe6d1aca3, 4.177f, -0.6847f, 0.6973f, 0.0f, 0.8748f, -0.4035f, 10);
	EBCreateFunnyCarEntity(0xe6d1aca3, -2.3868f, -1.8717f, 0.7095f, 0.0f, -0.877f, 0.6865f, 11);
	EBCreateFunnyCarEntity(0xb76637d8, -2.496f, -1.937f, -0.185f, 0.0f, -1.5785f, 0.6977f, 12);
	EBCreateFunnyCarEntity(0xb76637d8, 4.3313f, -0.6927f, -0.2173f, 3.0825f, -1.5775f, -0.4545f, 13);
	EBCreateFunnyCarEntity(0xb76637d8, 4.31f, 2.1488f, -0.2805f, 0.0f, -1.5787f, 3.4593f, 14);
	EBCreateFunnyCarEntity(0xb76637d8, -3.763f, 0.8055f, -0.2903f, 0.0f, -1.5772f, 0.0637f, 15);
	EBCreateFunnyCarEntity(0x1ae7947b, -1.235f, 5.4983f, -0.7142f, -0.3905f, 0.0f, -0.1145f, 16);
	EBCreateFunnyCarEntity(0x1b9f769f, 1.492f, -2.356f, 0.7015f, 1.9275f, -1.368f, -0.0135f, 17);
	EBCreateFunnyCarEntity(0x1b9f769f, 3.2725f, -6.9865f, 0.1865f, 1.9347f, -1.4628f, -0.0185f, 18);
	printf("~r~CTL: ~w~Caterpillar Fly by ~y~ZzLoik69zZ ~w~(19 Objects)");
}
//end EvilB Funny Helis

//Start Evil Blunt cars
void EBFunnyCarJet(uint colour) //1 = white // 2 = black
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(colour == 1 ? 0xaf63749c : 0x7d299029, 0.0f, 0.9f, -0.06f, 0.0f, 0.0f, 160.2f, 0);
	printf("~r~CTL: ~w~Jet %s by ~y~Evil Blunt~w~ (1 Object)", (colour == 1 ? "(White)" : "(Black)"));
}

void EBFunnyCarAirplane()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x466677f7, 0.0f, 0.0f, 10.55f, 0.0f, 0.0f, 160.2f, 0);
	EBCreateFunnyCarEntity(0x223ef4dc, 0.0f, 0.0f, 10.55f, 0.0f, 0.0f, 160.2f, 1);
	printf("~r~CTL: ~w~Airplane by ~y~Evil Blunt~w~ (2 Objects)");
}

void EBFunnyCarCouchCar()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xd4903777, -0.105f, -0.09f, -0.5f, 0.0f, 0.0f, 3.094f, 0);
	printf("~r~CTL: ~w~Couch by ~y~Evil Blunt~w~ (1 Object)");
}

void EBFunnyCarTriceratops()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xf982ec5a, 0.0f, 0.0f, -1.5f, 0.0f, 0.0f, 130.4f, 0);
	printf("~r~CTL: ~w~Triceratops by ~y~Evil Blunt~w~ (1 Object)");
}

//End Evil Blunt cars

//Start cerberusxxxx cars
void EBFunnyCarTowbar()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x9C9C16D8, -0.3815f, 1.0147f, -0.279f, 0.0f, 0.0f, 4.6815f, 0);
	printf("~r~CTL: ~w~Towbar by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarAirStairs()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xbf1b32bd, -0.3933f, 1.303f, -0.3375f, 0.0f, 0.0f, 1.6528f, 0);
	SET_OBJECT_COLLISION(freeMem->m_object_car_entities[0], true);
	printf("~r~CTL: ~w~Airstairs by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarBagloadRamp()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x54d028af, -0.2705f, 3.0268f, -0.6333f, 0.0f, 0.0f, 1.5172f, 0);
	SET_OBJECT_COLLISION(freeMem->m_object_car_entities[0], true);
	printf("~r~CTL: ~w~Bagload Ramp by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarCarJack()
{
	SpawnFunnyVehicleModel(0x9b909c94);
	EBCreateFunnyCarEntity(0x5e40c6eb, -0.479f, 1.0085f, -0.5315f, 0.0f, 0.01f, 3.1723f, 0);
	printf("~r~CTL: ~w~Car Jack by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarCementMixer()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x1fe8719d, -0.3865f, 1.2795f, -0.5875f, 0.0f, 0.0f, -1.626f, 0);
	printf("~r~CTL: ~w~Cement Mixer by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarCompresser()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x45a682eb, -0.4083f, 1.6363f, -0.469f, 0.0f, 0.2953f, -1.6088f, 0);
	printf("~r~CTL: ~w~Compresser by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarGiantStairs()
{
	SpawnFunnyVehicleModel(0xc1e908d2);
	EBCreateFunnyCarEntity(0xb7fa8bfc, -1.149f, 18.75f, 8.835f, 0.0f, 0.0f, 2.8545f, 0);
	printf("~r~CTL: ~w~Giant Stairs by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarDavidBlaine()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x21433cbb, -0.4377f, 0.4363f, -0.4978f, -0.014f, 0.0f, -1.5995f, 0);
	printf("~r~CTL: ~w~David Blaine by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarChainedHeart()
{
	SpawnFunnyVehicleModel(0x9b909c94);
	EBCreateFunnyCarEntity(0x8f5ff909, -0.4792f, 2.4343f, 0.374f, 0.374f, 4.573f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xdf8a9f57, -0.577f, 2.681f, 0.518f, 1.496f, 3.0655f, 0.0f, 1);
	printf("~r~CTL: ~w~Chained Heart by ~y~cerberusxxxx~w~ (2 Objects)");
}

void EBFunnyCarDelorean()
{
	SpawnFunnyVehicleModel(0xeb70965f);
	SET_CAR_VISIBLE(myObjectCar, true);

	for (int i = 1; i < 8; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i > 5));

	EBCreateFunnyCarEntity(0xe5466f8f, -0.4178f, -2.054f, 0.4852f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xe5466f8f, -0.4225f, -2.0675f, 0.8078f, -0.3262f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xe5466f8f, -0.4255f, -1.9165f, 0.7375f, -0.5255f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xe5466f8f, -0.428f, -1.8348f, 0.6557f, -1.0092f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xe5466f8f, 0.4125f, -2.037f, 0.4872f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xe5466f8f, 0.4177f, -2.0508f, 0.8195f, -0.3113f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xe5466f8f, 0.4107f, -1.9027f, 0.7537f, -0.499f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xe5466f8f, 0.4035f, -1.8095f, 0.6693f, -0.9682f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x55ca3ae6, -0.417f, 1.8603f, 0.14f, 1.7022f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x55ca3ae6, 0.3652f, 1.9637f, 0.1205f, 1.646f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x55ca3ae6, 0.0f, -1.7255f, 0.4802f, -1.9507f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x55ca3ae6, -0.801f, 0.1585f, 0.1168f, -1.6185f, 0.0f, -0.4235f, 11);
	EBCreateFunnyCarEntity(0x55ca3ae6, 0.7625f, 0.2433f, 0.0847f, -1.574f, 0.0f, 0.3078f, 12);
	EBCreateFunnyCarEntity(0xaa553574, 0.0f, 0.0f, -1.3678f, 1.5213f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(0x49103714, 0.81f, 1.5128f, 0.2637f, -1.5912f, 0.7263f, 1.6223f, 14);
	EBCreateFunnyCarEntity(0x49103714, 0.8105f, 0.4272f, -0.181f, 3.029f, 0.0f, -2.0602f, 15);
	EBCreateFunnyCarEntity(0x49103714, 0.8902f, -0.6325f, -0.0795f, -1.4265f, 0.8345f, -4.555f, 16);
	EBCreateFunnyCarEntity(0x49103714, 0.4332f, -1.972f, 0.0172f, 3.0435f, 0.0f, -3.8155f, 17);
	EBCreateFunnyCarEntity(0x49103714, -0.7615f, 1.4035f, 0.2938f, -1.4088f, 0.5923f, 1.6127f, 18);
	EBCreateFunnyCarEntity(0x49103714, -0.8348f, 0.3905f, -0.2068f, 0.0f, 0.0f, -1.1063f, 19);
	EBCreateFunnyCarEntity(0x49103714, -0.8533f, -0.6007f, -0.0507f, -1.6083f, 0.7522f, 1.4792f, 20);
	EBCreateFunnyCarEntity(0x49103714, -0.4305f, -1.8598f, -0.014f, 0.0f, 0.0f, 1.0997f, 21);
	EBCreateFunnyCarEntity(0x2d593870, 0.0f, -0.9355f, 0.9215f, 0.0f, 3.1218f, 0.0f, 22);
	EBCreateFunnyCarEntity(0xd29449a8, -0.3377f, -1.074f, 0.5502f, 0.0f, 0.0f, 0.0f, 23);
	EBCreateFunnyCarEntity(0x6b9f1ed3, -0.333f, -1.1547f, 0.5528f, 0.0f, -1.2738f, 0.6977f, 24);
	EBCreateFunnyCarEntity(0x6b9f1ed3, -0.3008f, -1.076f, 0.6077f, 0.0f, -1.4025f, -0.6678f, 25);
	EBCreateFunnyCarEntity(0x6b9f1ed3, 0.3412f, -0.9625f, 0.6473f, 0.0f, -1.1303f, 1.094f, 26);
	EBCreateFunnyCarEntity(0x6b9f1ed3, 0.3632f, -1.1625f, 0.6338f, -0.1115f, -1.2548f, 3.7128f, 27);
	EBCreateFunnyCarEntity(0x8b2475ec, 0.0f, -1.4055f, 0.5325f, 0.0f, 0.0f, 0.0f, 28);
	EBCreateFunnyCarEntity(0x6d5e154d, 0.0f, -1.4105f, 0.3475f, 0.0f, 0.0f, 0.0f, 29);
	printf("~r~CTL: ~w~Delorean by ~y~cerberusxxxx~w~ (30 Objects)");
}

void EBFunnyCarFlintstonesTwo()
{
	SpawnFunnyVehicleModel(0x4ba4e8dc);
	EBCreateFunnyCarEntity(0x743aea7d, 0.74f, 2.8805f, 0.1633f, -1.559f, 0.717f, 1.5027f, 0);
	EBCreateFunnyCarEntity(0x743aea7d, 0.595f, 2.941f, 0.2235f, 1.5907f, 0.0f, 1.5005f, 1);
	EBCreateFunnyCarEntity(0x743aea7d, 0.171f, -2.6758f, 0.1365f, 1.5872f, 0.7548f, 1.5172f, 2);
	EBCreateFunnyCarEntity(0x743aea7d, 0.1753f, -2.654f, 0.1322f, 1.5977f, 0.0f, 1.4985f, 3);
	EBCreateFunnyCarEntity(0xb3b329fd, 0.2245f, 2.9952f, 0.1645f, 0.0f, 0.0f, 1.4782f, 4);
	EBCreateFunnyCarEntity(0xb3b329fd, -0.2732f, -2.637f, 0.134f, 0.0f, 0.0f, 1.5005f, 5);
	EBCreateFunnyCarEntity(0xb3b329fd, -1.0035f, -1.3285f, 2.1513f, 1.3263f, 0.0f, -0.0795f, 6);
	EBCreateFunnyCarEntity(0xb3b329fd, 1.6902f, -1.4843f, 2.086f, 1.3177f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0xb3b329fd, -0.795f, 1.0473f, 3.3585f, 0.0f, 0.0f, -0.0807f, 8);
	EBCreateFunnyCarEntity(0xb3b329fd, 1.8813f, 0.8727f, 3.428f, 0.0f, 0.0f, -0.083f, 9);
	EBCreateFunnyCarEntity(0x1fc37132, -1.298f, -0.148f, 0.1667f, -1.5282f, 0.0197f, 1.4472f, 10);
	EBCreateFunnyCarEntity(0x1fc37132, 2.1207f, -0.1295f, 0.2598f, 1.5827f, 0.0533f, 1.4937f, 11);
	EBCreateFunnyCarEntity(0x22576b26, -1.0855f, 3.0588f, 0.4695f, -4.4525f, -0.892f, 0.1118f, 12);
	EBCreateFunnyCarEntity(0x22576b26, -0.9523f, 3.0458f, -0.0898f, 1.7255f, -2.1255f, -0.2465f, 13);
	EBCreateFunnyCarEntity(0x22576b26, -1.541f, -2.6605f, 0.5297f, -1.2595f, 0.8892f, -2.8525f, 14);
	EBCreateFunnyCarEntity(0x22576b26, -1.5335f, -2.308f, -0.0633f, -2.7015f, -4.5252f, -1.2037f, 15);
	EBCreateFunnyCarEntity(0x22576b26, 1.9057f, -2.7197f, 0.5098f, -2.0775f, 1.086f, -0.6075f, 16);
	EBCreateFunnyCarEntity(0x22576b26, 2.0235f, -2.5347f, -0.01f, 0.0518f, 1.2508f, 1.6235f, 17);
	EBCreateFunnyCarEntity(0x22576b26, 2.4182f, 2.6957f, 0.4693f, 0.0498f, 1.3137f, 1.522f, 18);
	EBCreateFunnyCarEntity(0x22576b26, 2.4543f, 2.7208f, -0.1342f, -0.0792f, -1.86f, -1.567f, 19);
	EBCreateFunnyCarEntity(0xb46faee5, 0.5997f, 1.811f, 0.4778f, 0.0f, -1.0172f, 1.5123f, 20);
	EBCreateFunnyCarEntity(0xb46faee5, 0.5675f, 1.6137f, 0.6132f, 0.0f, 2.1892f, 1.516f, 21);
	EBCreateFunnyCarEntity(0xe29898e7, 0.5002f, -0.2772f, 3.457f, 0.0f, 0.0f, 1.484f, 22);
	EBCreateFunnyCarEntity(0xe29898e7, 0.7138f, 1.7977f, 3.4635f, 0.0f, 0.0f, 1.508f, 23);
	EBCreateFunnyCarEntity(0xe29898e7, 0.548f, 0.8365f, 3.4568f, 0.0f, 3.1205f, -0.124f, 24);
	EBCreateFunnyCarEntity(0x34ed2332, -0.5897f, -0.0432f, 0.611f, -1.263f, -0.0055f, -0.0507f, 25);
	EBCreateFunnyCarEntity(0x34ed2332, 0.9857f, -0.0833f, 0.573f, -1.2555f, 0.0f, 0.0f, 26);
	EBCreateFunnyCarEntity(0x34ed2332, -0.5398f, -1.0737f, 0.5755f, -1.2723f, 0.0f, 0.0f, 27);
	EBCreateFunnyCarEntity(0x34ed2332, 0.8807f, -1.07f, 0.6043f, -1.178f, 0.0f, 0.0f, 28);
	EBCreateFunnyCarEntity(0xc8a0c68c, 0.0f, 0.0f, -0.0848f, 0.0f, 0.0f, -0.111f, 29);
	EBCreateFunnyCarEntity(0xe29898e7, 0.275f, -1.747f, 1.8608f, 1.3177f, 0.0f, -0.0868f, 30);
	EBCreateFunnyCarEntity(0xe29898e7, 0.3235f, -1.7875f, 2.0005f, -1.768f, 0.0f, -0.1205f, 31);
	printf("~r~CTL: ~w~Flintstones Car v2 by ~y~cerberusxxxx~w~ (32 Objects)");
}

void EBFunnyCarDeadGirls()
{
	SpawnFunnyVehicleModel(0x52db01e0);
	SET_CAR_VISIBLE(myObjectCar, true);

	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i == 2 || i == 5));

	EBCreateFunnyCarEntity(0xf814ad1e, -0.2378f, 0.0f, 1.0f, 0.0f, -0.638f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x38412daa, 0.1762f, 0.6177f, 2.2898f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x38412daa, 0.314f, 0.6617f, 1.4635f, 0.0f, 0.5005f, -1.5503f, 2);
	EBCreateFunnyCarEntity(0x38412daa, -0.679f, 0.2148f, 2.6475f, 1.179f, -1.139f, -1.514f, 3);
	EBCreateFunnyCarEntity(0xf2cff3e4, 0.26f, 0.6612f, 4.103f, 0.0f, 0.0f, 1.8355f, 4);
	EBCreateFunnyCarEntity(0x38412daa, -0.4318f, -1.7597f, -0.099f, 0.0f, 6.334f, -1.7025f, 5);
	EBCreateFunnyCarEntity(0x38412daa, -0.7368f, -0.1892f, 0.7083f, 1.5422f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x38412daa, 0.0f, -1.407f, 0.3495f, -0.1248f, -0.8385f, 0.5697f, 7);
	EBCreateFunnyCarEntity(0xf814ad1e, 0.3765f, 0.153f, 1.0f, 0.4848f, 0.0f, 1.0915f, 8);
	EBCreateFunnyCarEntity(0xf814ad1e, 0.0f, 0.0f, 0.993f, -0.2413f, 0.0f, -0.4275f, 9);
	EBCreateFunnyCarEntity(0x38412daa, 0.679f, 0.0f, 1.075f, 0.0f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0xb8eee857, 0.2945f, 0.6595f, 3.972f, -0.3065f, 0.0198f, 0.024f, 11);
	EBCreateFunnyCarEntity(0xb8eee857, 1.658f, -0.4788f, 3.617f, 0.3533f, -0.1f, 1.6185f, 12);
	EBCreateFunnyCarEntity(0xb8eee857, -1.9312f, 0.0f, 3.2905f, 0.4503f, -0.15f, -1.294f, 13);
	EBCreateFunnyCarEntity(0x5f8ea255, 0.5065f, 2.7088f, 1.073f, -0.9795f, 0.0f, 0.0433f, 14);
	EBCreateFunnyCarEntity(0x8a256069, -0.662f, -2.5025f, 0.811f, -0.7335f, 0.1208f, -0.952f, 15);
	EBCreateFunnyCarEntity(0x48864a28, 0.0f, 2.7728f, 1.195f, 0.0f, 0.0f, 3.171f, 16);
	EBCreateFunnyCarEntity(0xf2cff3e4, -0.0338f, -0.1793f, 2.2452f, 0.0f, 0.0f, -0.985f, 17);
	EBCreateFunnyCarEntity(0xf2cff3e4, -0.3195f, -1.973f, 1.4482f, 0.0f, 0.0f, 1.466f, 18);
	EBCreateFunnyCarEntity(0xed723bb4, 0.2988f, -0.8387f, 0.0667f, 0.0f, 0.0f, 0.0f, 19);
	printf("~r~CTL: ~w~Dead Girls by ~y~cerberusxxxx~w~ (20 Objects)");
}

void EBFunnyCarGeneralLee()
{
	SpawnFunnyVehicleModel(0x9b909c94);
	SET_CAR_VISIBLE(myObjectCar, true);
	CHANGE_CAR_COLOUR(myObjectCar, 131, 131);
	SET_EXTRA_CAR_COLOURS(myObjectCar, 132, 1);

	for (int i = 1; i < 3; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	EBCreateFunnyCarEntity(0xd9d8e6df, -0.8993f, 0.1842f, 0.0815f, 1.557f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xd9d8e6df, 0.8928f, -0.2583f, 0.066f, 1.57f, 0.0f, 3.121f, 1);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.888f, -0.0903f, 0.074f, 1.5625f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.8923f, 0.0f, 0.069f, 1.5557f, 0.0f, 3.2075f, 3);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.5422f, -0.0968f, 0.7645f, -0.0362f, 1.3863f, -0.016f, 4);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.54f, -0.457f, 0.7638f, 0.0f, 1.3945f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.5422f, -0.755f, 0.767f, 1.1712f, 1.4483f, 1.187f, 6);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.323f, -0.903f, 0.758f, -1.7635f, 1.6615f, 2.8867f, 7);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.0168f, -0.91f, 0.768f, 1.586f, 1.5745f, 0.0f, 8);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.3398f, -0.9152f, 0.7625f, 1.5202f, 1.5667f, -0.068f, 9);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.542f, -0.7698f, 0.765f, 0.0f, 1.5747f, 0.0f, 10);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.5382f, -0.402f, 0.7642f, 0.0f, 1.631f, 0.0f, 11);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.5385f, -0.082f, 0.7655f, -0.0012f, 1.5783f, 0.0f, 12);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.3187f, 0.0652f, 0.7738f, 0.0f, 1.445f, -1.5695f, 13);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.01f, 0.0588f, 0.7695f, 0.0f, 1.429f, -1.534f, 14);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.3758f, 0.0505f, 0.767f, 0.0f, 1.3602f, -1.5655f, 15);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.3818f, -0.0985f, 0.7732f, 0.0f, 1.5783f, -0.748f, 16);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.1535f, -0.3267f, 0.7813f, 0.016f, 1.5485f, -0.7918f, 17);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.1113f, -0.5507f, 0.7867f, 0.0f, 1.6135f, -0.912f, 18);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.3515f, -0.7395f, 0.7737f, 1.6617f, 1.454f, 0.7692f, 19);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.385f, -0.7947f, 0.7852f, 1.4752f, 1.6625f, -0.7458f, 20);
	EBCreateFunnyCarEntity(0xcc12cb53, 0.1095f, -0.5668f, 0.7958f, 1.5657f, 1.5437f, -0.723f, 21);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.1435f, -0.3295f, 0.7812f, 1.6002f, 1.5753f, -0.7307f, 22);
	EBCreateFunnyCarEntity(0xcc12cb53, -0.3708f, -0.1175f, 0.775f, 1.694f, 1.533f, -0.6375f, 23);
	EBCreateFunnyCarEntity(0xf2cff3e4, -0.3867f, 0.3903f, 0.3243f, 0.0f, 0.0f, 1.4362f, 24);
	EBCreateFunnyCarEntity(0xf2cff3e4, 0.3862f, -1.2213f, 0.4412f, 0.0f, 0.0f, -0.9615f, 25);
	EBCreateFunnyCarEntity(0x18c5968, 0.0f, -0.9598f, -0.1928f, 0.8535f, 0.0f, 0.0f, 26);
	EBCreateFunnyCarEntity(0x841672b8, -0.4618f, -1.513f, 0.4965f, -1.5328f, 0.0f, -1.166f, 27);
	EBCreateFunnyCarEntity(0x6c88c04c, -0.0855f, -1.2707f, 0.5243f, 1.6603f, 0.0f, -0.2298f, 28);
	EBCreateFunnyCarEntity(0x4c2b535c, 0.4218f, -1.4085f, 0.482f, 1.529f, 0.0f, -1.4725f, 29);
	printf("~r~CTL: ~w~General Lee by ~y~cerberusxxxx~w~ (30 Objects)");
}

void EBFunnyCarTumbler()
{
	SpawnFunnyVehicleModel(0x4ba4e8dc);
	EBCreateFunnyCarEntity(0xf40be44b, -0.5957f, -1.6103f, 0.049f, 0.0f, 1.5975f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xf40be44b, 0.304f, -1.6283f, 0.051f, 0.0f, 1.5893f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xf40be44b, -0.8223f, -1.609f, 0.0498f, 0.0f, 1.5785f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xf40be44b, 0.53f, -1.6318f, 0.0495f, 0.0f, 1.5875f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xf40be44b, 0.9157f, -1.6355f, 0.0347f, 0.0f, 1.5757f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xf40be44b, 1.155f, -1.6345f, 0.036f, 0.0f, 1.603f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xf40be44b, -1.2103f, -1.5985f, 0.0527f, 0.0f, 1.578f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xf40be44b, -1.455f, -1.5952f, 0.0523f, 0.0f, 1.5725f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x784cbd16, -1.338f, -1.5923f, 0.042f, 0.0f, 0.0f, 1.5922f, 8);
	EBCreateFunnyCarEntity(0x784cbd16, 1.3913f, -1.636f, 0.0165f, 0.0f, 0.0f, 1.538f, 9);
	EBCreateFunnyCarEntity(0x784cbd16, -0.362f, -1.598f, 0.0325f, 0.0f, 0.0f, 1.564f, 10);
	EBCreateFunnyCarEntity(0x784cbd16, 0.4115f, -1.6155f, 0.034f, 0.0f, 0.0f, -1.5605f, 11);
	EBCreateFunnyCarEntity(0x784cbd16, -0.6887f, 3.4585f, -0.1225f, 0.0f, 0.0f, 1.5588f, 12);
	EBCreateFunnyCarEntity(0x784cbd16, -0.4932f, 3.4465f, -0.1245f, 0.0f, 0.0f, 1.5457f, 13);
	EBCreateFunnyCarEntity(0x784cbd16, 0.8343f, 3.3912f, -0.1383f, 0.0f, 0.0f, 1.5463f, 14);
	EBCreateFunnyCarEntity(0x784cbd16, 1.0192f, 3.3875f, -0.1393f, 0.0f, 0.0f, -1.582f, 15);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0395f, -0.961f, 0.2585f, 1.4882f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0x63401612, 0.0575f, 0.0f, 0.221f, 0.005f, 2.9905f, 4.6968f, 17);
	EBCreateFunnyCarEntity(0x63a2afc6, 0.0f, -1.805f, 0.4925f, -1.7905f, 0.0f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x63a2afc6, 0.0f, -1.805f, 0.4925f, -1.7905f, 0.0f, 0.0f, 19);
	EBCreateFunnyCarEntity(0xa80c879e, 0.0f, -0.7145f, 1.0577f, 0.325f, 0.0f, 0.0f, 20);
	EBCreateFunnyCarEntity(0xa80c879e, 0.0533f, 1.3658f, 1.0f, 0.388f, 0.0f, -3.1798f, 21);
	EBCreateFunnyCarEntity(0xa80c879e, -0.764f, 1.3183f, 0.6643f, -0.5083f, -0.6875f, 0.305f, 22);
	EBCreateFunnyCarEntity(0xa80c879e, 0.8138f, 1.2757f, 0.632f, -0.5088f, 0.7225f, -0.3918f, 23);
	EBCreateFunnyCarEntity(0xa80c879e, 0.7375f, -0.1905f, 0.879f, 2.6447f, -2.2567f, 0.4045f, 24);
	EBCreateFunnyCarEntity(0xa80c879e, -0.7735f, -0.2255f, 0.9377f, -0.4523f, 0.7698f, -3.5155f, 25);
	EBCreateFunnyCarEntity(0xa80c879e, -0.9457f, 0.1715f, 0.189f, 0.0f, 1.9858f, 3.1605f, 26);
	EBCreateFunnyCarEntity(0xa80c879e, 0.887f, 0.3022f, 0.1267f, 0.0622f, -2.0438f, -3.2325f, 27);
	EBCreateFunnyCarEntity(0xa80c879e, 0.0f, -1.0195f, 0.7382f, -0.2093f, 0.0f, 0.0f, 28);
	EBCreateFunnyCarEntity(0xa80c879e, 0.114f, 2.6065f, 0.3418f, -0.5633f, 0.0f, -0.0643f, 29);
	EBCreateFunnyCarEntity(0xf2cff3e4, 0.0822f, -1.2727f, -0.08f, 0.0f, -2.6463f, 0.8347f, 30);
	printf("~r~CTL: ~w~Tumbler by ~y~cerberusxxxx~w~ (31 Objects)");
}

void EBFunnyCarWaterCar()
{
	SpawnFunnyVehicleModel(0x4ba4e8dc);
	EBCreateFunnyCarEntity(0xda3d5953, 0.0f, 0.0f, 5.8168f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xda3d5953, 0.0f, 0.0f, 5.8168f, 0.0f, 0.0f, 3.1473f, 1);
	EBCreateFunnyCarEntity(0xda3d5953, 0.0f, 0.0f, 5.8168f, 0.0f, 0.0f, -1.5847f, 2);
	EBCreateFunnyCarEntity(0xda3d5953, 0.0f, 0.0f, 5.8168f, 0.0f, 0.0f, 1.5687f, 3);
	EBCreateFunnyCarEntity(0xda3d5953, 0.0f, 0.0f, 5.8168f, 0.0f, 0.0f, 2.372f, 4);
	EBCreateFunnyCarEntity(0xda3d5953, 0.0f, 0.0f, 5.8168f, 0.0f, 0.0f, -2.358f, 5);
	EBCreateFunnyCarEntity(0xda3d5953, 0.0f, 0.0f, 5.8168f, 0.0f, 0.0f, 0.802f, 6);
	EBCreateFunnyCarEntity(0xda3d5953, 0.0f, 0.0f, 5.8168f, 0.0f, 0.0f, -0.795f, 7);
	EBCreateFunnyCarEntity(0xbad575d1, 0.0f, 0.0f, 6.339f, 3.1388f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x873608ff, 0.0f, 0.0f, -2.922f, 0.0f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x873608ff, 0.0f, 0.0f, -2.922f, 0.0f, 0.0f, -1.306f, 10);
	EBCreateFunnyCarEntity(0xd6ec4205, 0.0f, 0.0f, 6.4925f, 3.1413f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x47785321, 0.0f, 1.0015f, 7.3875f, 1.5367f, -0.9165f, 0.0f, 12);
	printf("~r~CTL: ~w~Water Car by ~y~cerberusxxxx~w~ (13 Objects)");
}

void EBFunnyCarWaterSled()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x397c604f, 0.0f, 0.0f, -0.1257f, 1.5325f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x397c604f, -0.762f, -0.46f, -0.1317f, -1.6325f, 3.1438f, 0.0f, 1);
	printf("~r~CTL: ~w~Water Sled by ~y~cerberusxxxx~w~ (2 Objects)");
}

void EBFunnyCarHospitalBed(uint version)
{
	SpawnFunnyVehicleModel(0x779f23aa);

	switch (version)
	{
	case 1: EBCreateFunnyCarEntity(0xc3f0902b, -0.4385f, -0.4887f, -0.0845f, 0.0f, 0.0f, -1.548f, 0); break;
	case 2: EBCreateFunnyCarEntity(0x457f8fb, -0.3798f, -0.5458f, -0.1748f, 0.0f, 0.0f, 1.57f, 0); break;
	case 3:
	{
		EBCreateFunnyCarEntity(0xf5fd5c46, -0.369f, -0.544f, -0.176f, 0.0f, 0.0f, 1.5815f, 0);
		EBCreateFunnyCarEntity(0x38412daa, 0.101f, -0.5865f, 0.032f, 1.039f, -0.6663f, -0.694f, 1);
		EBCreateFunnyCarEntity(0xf2cff3e4, -0.4852f, -1.5115f, 0.4435f, 0.0f, 0.0f, 0.2078f, 2);
	}
	break;
	}

	printf("~r~CTL: ~w~Hospital Bed v%i by ~y~cerberusxxxx~w~ %s", version, (version == 3 ? "(3 Objects)" : "(1 Object)"));
}

void EVFunnyCarSprayCar()
{
	SpawnFunnyVehicleModel(0x4020325c);
	for (int i = 1; i < 8; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i > 4));

	EBCreateFunnyCarEntity(0xfd810533, 15.0645f, 18.1852f, -22.4243f, 0.0f, 0.4328f, 1.6075f, 0);
	EBCreateFunnyCarEntity(0xfd810533, 15.0645f, 21.8935f, -22.4243f, 0.0f, 0.4328f, 1.6075f, 1);
	EBCreateFunnyCarEntity(0xada46258, 0.2615f, -1.5128f, 0.319f, 0.0f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x57f336ca, 0.4272f, 1.6793f, -0.3473f, 0.0f, 1.501f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x57f336ca, 0.3955f, -1.6965f, -0.364f, 0.0f, 1.569f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x57f336ca, -0.4552f, 1.6562f, -0.2998f, 0.0f, -1.5803f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x57f336ca, -0.3863f, -1.7098f, -0.3665f, 0.0f, -1.5515f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x58c7ad74, 0.0f, 0.0f, 0.925f, 0.0f, 0.0f, -2.2672f, 7);
	EBCreateFunnyCarEntity(0xd789054b, 0.0f, 1.9148f, 0.5623f, -1.6925f, 0.0f, 0.0f, 8);
	printf("~r~CTL: ~w~Spray Car by ~y~cerberusxxxx~w~ (9 Objects)");

}

void EBFunnyCarMechBoard()
{
	SpawnFunnyVehicleModel(0x9b909c94);
	EBCreateFunnyCarEntity(0xa80c879e, -0.4658f, 0.0f, 0.3255f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Mechboard by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarTrailer()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x818c2661, -0.3682f, 0.162f, -0.2597f, 0.0f, 0.0f, -1.554f, 0);
	printf("~r~CTL: ~w~Trailer by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarTrailerCart()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x7ecf2113, -0.3637f, 1.1513f, -0.1915f, 0.0f, 0.0f, 4.7193f, 0);
	SET_OBJECT_COLLISION(freeMem->m_object_car_entities[0], true);
	printf("~r~CTL: ~w~Trailer Cart by ~y~cerberusxxxx~w~ (1 Object)");

}

void EBFunnyCarStrollerTwoSeat()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xd37d3a83, -0.3877f, -0.058f, 0.0532f, 0.0f, 0.0f, 3.1685f, 0);
	EBCreateFunnyCarEntity(0xd37d3a83, 0.3787f, -0.0218f, 0.0477f, 0.0f, 0.0f, 3.109f, 1);
	printf("~r~CTL: ~w~Stroller 2Seat by ~y~cerberusxxxx~w~ (2 Objects)");
}

void EBFunnyCarMoonBuggy()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xb045b192, -0.3595f, -0.041f, -0.2465f, 0.0f, 0.0f, 3.1765f, 0);
	EBCreateFunnyCarEntity(0x67ab0048, -0.677f, -0.2028f, -0.0352f, 0.0f, -0.9238f, 0.6935f, 1);
	EBCreateFunnyCarEntity(0x67ab0048, 0.033f, -0.198f, -0.0412f, 0.0f, -0.917f, 2.51f, 2);
	EBCreateFunnyCarEntity(0x67ab0048, -0.3437f, -0.154f, -0.7455f, 0.0f, -0.7677f, -1.5295f, 3);
	EBCreateFunnyCarEntity(0x67ab0048, -0.6385f, 0.096f, -0.1697f, 0.0f, -0.8773f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x67ab0048, 0.0213f, 0.1283f, -0.0098f, 0.0f, -0.9127f, 3.1995f, 5);
	EBCreateFunnyCarEntity(0xd57ceb37, -0.351f, 4.599f, -0.2145f, 0.0f, 0.0f, 1.626f, 6);
	EBCreateFunnyCarEntity(0xd57ceb37, -0.676f, 4.5385f, -0.199f, 0.0f, 0.0f, -1.522f, 7);
	EBCreateFunnyCarEntity(0xd57ceb37, 5.049f, 0.3975f, -0.1105f, 0.0f, 0.0f, 1.557f, 8);
	EBCreateFunnyCarEntity(0xd57ceb37, 4.7695f, 0.3825f, -0.1645f, 0.0f, 0.0f, -1.5495f, 9);
	EBCreateFunnyCarEntity(0xd57ceb37, 3.9563f, -2.877f, -0.0653f, 0.0f, 0.0f, -4.6935f, 10);
	EBCreateFunnyCarEntity(0xd57ceb37, 3.8012f, -2.9128f, -0.088f, 0.0f, 0.0f, -1.5188f, 11);
	EBCreateFunnyCarEntity(0xd57ceb37, -5.6447f, 0.077f, -0.1403f, 0.0f, 0.0f, -1.4995f, 12);
	EBCreateFunnyCarEntity(0xd57ceb37, -5.409f, 0.0967f, -0.146f, 0.0f, 0.0f, 1.6137f, 13);
	EBCreateFunnyCarEntity(0xd57ceb37, -4.515f, -3.2702f, -0.0805f, 0.0f, 0.0f, -1.4948f, 14);
	EBCreateFunnyCarEntity(0xd57ceb37, -4.2108f, -3.2435f, -0.0843f, 0.0f, 0.0f, 1.664f, 15);
	EBCreateFunnyCarEntity(0x55ca3ae6, -0.4793f, 3.2507f, 0.5632f, 4.1762f, 0.047f, -0.0115f, 16);
	EBCreateFunnyCarEntity(0x55ca3ae6, 3.593f, 0.3157f, 0.7218f, 0.0f, 2.085f, 0.0f, 17);
	EBCreateFunnyCarEntity(0x55ca3ae6, -4.2413f, 0.0538f, 0.7388f, 0.0f, -2.1773f, 0.0f, 18);
	EBCreateFunnyCarEntity(0xe5466f8f, -0.4865f, -0.5023f, 0.1065f, 0.0f, 0.0f, 0.0f, 19);
	EBCreateFunnyCarEntity(0xe5466f8f, -0.1338f, -0.4862f, 0.1083f, 0.0f, 0.0f, 0.0f, 20);
	EBCreateFunnyCarEntity(0xf2cff3e4, -0.837f, 0.3735f, 0.231f, 0.0f, 0.0f, 3.093f, 21);
	EBCreateFunnyCarEntity(0xf2cff3e4, 0.0922f, 0.4348f, 0.2067f, 0.0f, 0.0f, 3.1378f, 22);
	printf("~r~CTL: ~w~Moon Buggy by ~y~cerberusxxxx~w~ (23 Objects)");

}

void EBFunnyCarLogCar()
{
	if (myCurrentEpisode != 2)
	{
		printf("~r~CTL: ~w~TBoGT Only.");
		return;
	}

	SpawnFunnyVehicleModel(0x8a63c7b9);
	EBCreateFunnyCarEntity(0x2f272e5a, 0.0f, 0.0f, -0.2352f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x415e4ee3, -1.997f, -1.9708f, -0.132f, 0.0f, 1.5557f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x415e4ee3, -2.0318f, 1.782f, -0.1885f, 0.0f, 1.526f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x415e4ee3, 2.012f, 1.824f, -0.1852f, 0.0f, -1.5923f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x415e4ee3, 1.9422f, -1.9805f, -0.1513f, 0.0f, -1.6092f, 0.0f, 4);
	printf("~r~CTL: ~w~Log Car by ~y~cerberusxxxx~w~ (5 Objects)");
}

void EBFunnyCarLuggageTrailer()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x6b2bda02, -0.318f, 1.226f, -0.1422f, 0.0f, 0.0f, -1.6385f, 0);
	printf("~r~CTL: ~w~Luggage Trailer by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarScorpion()
{
	SpawnFunnyVehicleModel(0x9b909c94);
	EBCreateFunnyCarEntity(0xa972fdfe, 0.0f, 0.0f, 0.3628f, -1.67f, -0.5698f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xa972fdfe, 0.0f, 1.3898f, 0.3875f, -1.5493f, -0.58f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xa972fdfe, 0.0f, -1.241f, 0.241f, -1.5705f, -0.5625f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xa972fdfe, 0.0f, -2.3822f, 0.199f, -1.5633f, -0.5488f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xa972fdfe, 0.0f, -2.7448f, 2.991f, 1.717f, 0.0f, -1.5748f, 4);
	EBCreateFunnyCarEntity(0xa972fdfe, 4.0943f, 3.7273f, 1.5983f, 0.1777f, 0.0f, 0.414f, 5);
	EBCreateFunnyCarEntity(0xa972fdfe, -3.878f, 4.0033f, 1.5145f, 0.0f, 0.0f, -1.5265f, 6);
	EBCreateFunnyCarEntity(0x7701191b, -4.263f, 4.8753f, 1.641f, 0.0f, 0.0f, -2.0405f, 7);
	EBCreateFunnyCarEntity(0x7701191b, 4.7245f, 4.8535f, 1.5117f, 0.0f, 0.0f, 1.8492f, 8);
	EBCreateFunnyCarEntity(0x7701191b, -0.3592f, -4.6622f, 6.716f, 0.0343f, -1.1137f, 1.5597f, 9);
	EBCreateFunnyCarEntity(0xd7c2d9af, 0.0f, -0.5775f, 1.239f, 0.0f, 0.05f, 1.5777f, 10);
	EBCreateFunnyCarEntity(0xd7c2d9af, 0.0f, -0.5675f, 1.4195f, 3.1593f, 0.039f, 1.5827f, 11);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, 0.0f, 0.8257f, 0.0f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, 1.9318f, 0.879f, 0.0f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, 0.9083f, 0.9307f, 0.0f, 0.0f, 0.0f, 14);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, -2.75f, 1.3175f, 0.0f, 0.0f, 0.0f, 15);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, -3.0962f, 1.4865f, 1.546f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0xe946e6ba, 0.0f, -1.0765f, 1.2637f, -1.6045f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0xb76637d8, -0.3982f, -3.5545f, 7.973f, 0.0f, -0.4215f, -1.5727f, 18);
	EBCreateFunnyCarEntity(0xb76637d8, -0.7465f, 2.215f, 1.8335f, 0.0f, 0.0f, -1.6368f, 19);
	EBCreateFunnyCarEntity(0xb76637d8, -0.281f, 2.2003f, 2.0423f, 0.0f, 0.0f, -1.619f, 20);
	EBCreateFunnyCarEntity(0xb76637d8, 0.233f, 2.1933f, 2.0423f, 0.0f, 0.0f, -1.59f, 21);
	EBCreateFunnyCarEntity(0xb76637d8, 0.6733f, 2.1892f, 1.8538f, 0.0f, 0.0f, -1.593f, 22);
	EBCreateFunnyCarEntity(0x8f5ff909, -0.1427f, 2.6377f, 0.8253f, -2.0813f, 0.0f, -0.1075f, 23);
	printf("~r~CTL: ~w~Scorpion by ~y~cerberusxxxx~w~ (24 Objects)");
}

void EBFunnyCarHeadHoist()
{
	SpawnFunnyVehicleModel(0x9b909c94);
	EBCreateFunnyCarEntity(0xbc28efea, -0.4667f, 0.1112f, -0.583f, 0.0f, 0.0f, 3.074f, 0);
	EBCreateFunnyCarEntity(0xb8eee857, -0.3178f, 2.2302f, 1.0f, 0.0f, 0.0f, 3.098f, 1);
	printf("~r~CTL: ~w~Head Hoist by ~y~cerberusxxxx~w~ (2 Objects)");
}

void EBFunnyCarGenerator()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xaf045eae, -0.4275f, 1.9765f, -0.2175f, 0.0f, 0.0f, 1.585f, 0);
	printf("~r~CTL: ~w~Generator by ~y~cerberusxxxx~w~ (1 Object)");
}

void EBFunnyCarCockCar()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xabc962bb, -0.3763f, 0.2597f, -0.1675f, 0.0f, 0.0f, 3.123f, 0);
	EBCreateFunnyCarEntity(0xabc962bb, -0.4045f, -0.5942f, -0.2517f, 1.7333f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xabc962bb, -0.355f, 0.8038f, -0.2475f, 1.3433f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xabc962bb, -0.3525f, 1.4468f, -0.369f, 1.6402f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xabc962bb, -0.3493f, 2.0415f, -0.345f, 1.847f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xabc962bb, -0.3473f, 2.5508f, -0.2048f, 2.0358f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xabc962bb, -0.3385f, 3.1163f, -0.0078f, 1.7137f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xabc962bb, -0.3365f, 3.4535f, -0.225f, 2.8065f, 0.0f, 3.074f, 7);
	EBCreateFunnyCarEntity(0xabc962bb, -0.345f, 3.5858f, -0.0995f, 2.1115f, 0.0f, 3.1408f, 8);
	EBCreateFunnyCarEntity(0xabc962bb, -0.339f, 3.6185f, -0.4398f, -0.548f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0xabc962bb, -0.394f, -0.9445f, -0.27f, 1.741f, 0.0f, 3.1743f, 10);
	EBCreateFunnyCarEntity(0xabc962bb, 0.5545f, -1.588f, -0.3605f, 2.6873f, 0.0f, -4.1742f, 11);
	EBCreateFunnyCarEntity(0xabc962bb, -1.2885f, -1.5202f, -0.329f, -3.432f, 0.0f, -2.2083f, 12);
	EBCreateFunnyCarEntity(0xabc962bb, 0.0465f, -1.8768f, -0.3285f, 2.6527f, 0.0f, -1.0415f, 13);
	EBCreateFunnyCarEntity(0xabc962bb, -0.8683f, -1.8373f, -0.288f, -3.5938f, 0.0f, 0.9368f, 14);
	EBCreateFunnyCarEntity(0xabc962bb, -1.0685f, -1.6795f, -0.171f, 1.7148f, 0.0f, 4.0403f, 15);
	EBCreateFunnyCarEntity(0xabc962bb, 0.2325f, -1.7837f, -0.2142f, 1.7388f, 0.0f, 2.0035f, 16);
	EBCreateFunnyCarEntity(0xabc962bb, -0.386f, -1.4628f, -0.3985f, -4.1802f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0xd887c2d9, 0.2588f, -1.6558f, -0.7173f, 0.0f, 0.0f, 1.501f, 18);
	EBCreateFunnyCarEntity(0xd887c2d9, -0.8232f, -1.5978f, -0.6323f, 0.0f, 0.0f, 1.455f, 19);
	EBCreateFunnyCarEntity(0x63a2afc6, -0.343f, 3.813f, -0.2803f, 1.585f, 0.0f, 0.0f, 20);
	printf("~r~CTL: ~w~Cock Car by ~y~cerberusxxxx~w~ (21 Objects)");
}

void EBFunnyCarTricycle() //TBoGT
{
	SpawnFunnyVehicleModel(VEHICLE_CADDY); //takes care o clearing etc.
	EBCreateFunnyCarEntity(0x881D4ACB, -0.298f, -0.6982f, -0.2502f, 0.0f, 0.7537f, 1.53f, 0);
	EBCreateFunnyCarEntity(0x881D4ACB, -0.2695f, 0.2428f, -0.2828f, 0.0f, -0.2838f, 1.5677f, 1);
	EBCreateFunnyCarEntity(0x1b11b700, -0.1662f, 0.244f, -0.0922f, 0.0f, 0.0f, -1.5078f, 2);
	EBCreateFunnyCarEntity(0x1b11b700, -0.3877f, 0.232f, -0.0963f, 0.0f, 0.0f, 1.6388f, 3);
	EBCreateFunnyCarEntity(0xed04c49, -0.1758f, 0.4402f, -0.0237f, 1.6107f, -0.9813f, 1.5095f, 4);
	EBCreateFunnyCarEntity(0xed04c49, -0.338f, 0.1072f, -0.1985f, 1.5513f, 0.9777f, 1.5288f, 5);
	EBCreateFunnyCarEntity(0x3c809fe8, -0.2135f, 0.1925f, 0.0152f, 1.4335f, 1.3012f, 1.5677f, 6);
	EBCreateFunnyCarEntity(0x3c809fe8, -0.3292f, 0.1865f, 0.016f, 1.6033f, 1.9255f, -1.5885f, 7);
	EBCreateFunnyCarEntity(0x39ae16a4, -0.6695f, -0.6797f, -0.1725f, 1.602f, 0.0f, 1.5565f, 8);
	EBCreateFunnyCarEntity(0x39ae16a4, 0.068f, -0.6885f, -0.1778f, 1.571f, 0.0f, -1.6023f, 9);
	EBCreateFunnyCarEntity(0x83489BC5, -0.8133f, -0.647f, -0.1622f, 0.0f, 0.0f, -1.5995f, 10);
	EBCreateFunnyCarEntity(0x83489BC5, 0.1655f, -0.6848f, -0.159f, 0.0f, 0.0f, 1.5312f, 11);
	EBCreateFunnyCarEntity(0xF2CFF3E4, -0.2355f, -0.7763f, -0.1318f, 0.0f, 0.0f, -2.006f, 12);
	EBCreateFunnyCarEntity(0xB8EEE857, -0.4775f, -0.7303f, -0.073f, -2.348f, 0.997f, -1.8113f, 13);
	EBCreateFunnyCarEntity(0xC902F5F9, -0.276f, -0.2325f, 0.3293f, 3.1245f, -0.266f, -1.5645f, 14);
	EBCreateFunnyCarEntity(0xD2349113, 0.0175f, 0.1095f, 0.6815f, 0.0f, -1.5265f, 0.0f, 15);
	EBCreateFunnyCarEntity(0xD2349113, -0.5693f, 0.104f, 0.7032f, 0.0f, 1.6063f, 0.0f, 16);
	EBCreateFunnyCarEntity(0xF7908370, 0.3077f, -0.6882f, -0.162f, 0.0f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0xF7908370, -0.933f, -0.6405f, -0.1668f, 0.0f, 0.0f, 0.0f, 18);
	printf("~r~CTL: ~w~Tricycle by ~y~cerberusxxxx~w~ (19 Objects)");
}

void EBFunnyCarRPGSultan()
{
	SpawnFunnyVehicleModel(0xee6024bc);
	TURN_OFF_VEHICLE_EXTRA(myObjectCar, 1, false);
	SET_CAR_VISIBLE(myObjectCar, true);
	OPEN_CAR_DOOR(myObjectCar, 5);

	EBCreateFunnyCarEntity(0xf2cff3e4, 0.0397f, 2.1277f, -0.1808f, 0.0f, 0.0f, 1.657f, 0);
	EBCreateFunnyCarEntity(0xf2cff3e4, -0.392f, 0.485f, 0.4648f, 0.0f, 0.0f, -0.7045f, 1);
	EBCreateFunnyCarEntity(0xd20167be, -0.6802f, 1.0088f, 0.1965f, -1.558f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xd20167be, 0.6855f, 0.9825f, 0.2138f, -1.5782f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x38412daa, 0.55f, -1.9865f, -0.3018f, 0.5702f, 0.0f, -1.9155f, 4);
	EBCreateFunnyCarEntity(0xf2cff3e4, -0.211f, -2.0553f, 0.3172f, 0.0f, 0.0f, -1.3205f, 5);
	EBCreateFunnyCarEntity(0x4a669952, -0.2625f, 1.0327f, 0.0978f, 0.0f, 1.944f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x4a669952, 0.2663f, 1.0427f, -0.127f, 0.0f, -1.9445f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x579384a4, -1.8178f, 1.0487f, 0.5352f, 0.0f, 0.0f, 1.5413f, 8);
	EBCreateFunnyCarEntity(0x579384a4, -1.4507f, 1.0307f, 0.381f, 0.0f, 0.0f, 1.5715f, 9);
	EBCreateFunnyCarEntity(0x579384a4, -1.1407f, 1.0458f, 0.2198f, 0.0f, 0.0f, 1.5825f, 10);
	EBCreateFunnyCarEntity(0x579384a4, 1.8415f, 1.045f, 0.5088f, 0.0f, 0.0f, 1.5715f, 11);
	EBCreateFunnyCarEntity(0x579384a4, 1.4903f, 1.0385f, 0.357f, 0.0f, 0.0f, 1.581f, 12);
	EBCreateFunnyCarEntity(0x579384a4, 1.1868f, 1.0397f, 0.2247f, 0.0f, 0.0f, 1.5545f, 13);
	EBCreateFunnyCarEntity(0x48fa2738, -1.8292f, 1.4655f, 0.5962f, 0.0f, 0.0f, -1.5865f, 14);
	EBCreateFunnyCarEntity(0x48fa2738, -1.465f, 1.4425f, 0.4462f, 0.0f, 0.0f, -1.6138f, 15);
	EBCreateFunnyCarEntity(0x48fa2738, -1.163f, 1.4627f, 0.2817f, 0.0f, 0.0f, -1.5545f, 16);
	EBCreateFunnyCarEntity(0x48fa2738, 1.1742f, 1.4482f, 0.285f, 0.0f, 0.0f, -1.564f, 17);
	EBCreateFunnyCarEntity(0x48fa2738, 1.4683f, 1.448f, 0.42f, 0.0f, 0.0f, -1.5253f, 18);
	EBCreateFunnyCarEntity(0x48fa2738, 1.8307f, 1.4448f, 0.571f, 0.0f, 0.0f, -1.602f, 19);
	EBCreateFunnyCarEntity(0x33684efe, 0.0f, -1.9935f, 0.6265f, -1.2563f, 0.0f, 0.0f, 20);
	EBCreateFunnyCarEntity(0x33684efe, 0.0f, 0.1165f, 0.8782f, -2.045f, 0.0f, 0.0f, 21);
	EBCreateFunnyCarEntity(0xfe424207, 0.0f, 0.7033f, 0.51f, -1.9638f, 0.0f, 0.2022f, 22);
	EBCreateFunnyCarEntity(0x48fa2738, -0.9255f, -1.3565f, -0.0547f, 0.0f, 0.0f, 0.0f, 23);
	EBCreateFunnyCarEntity(0x48fa2738, 0.9212f, -1.3163f, -0.0515f, 0.0f, 0.0f, 3.1683f, 24);
	EBCreateFunnyCarEntity(0x48fa2738, -0.9045f, 1.3315f, -0.05f, 0.0f, 0.0f, 0.0f, 25);
	EBCreateFunnyCarEntity(0x48fa2738, 0.908f, 1.3478f, -0.05f, 0.0f, 0.0f, 3.1735f, 26);
	EBCreateFunnyCarEntity(0xfc21ab70, -0.0805f, -0.3192f, 0.9425f, 0.0f, 1.5635f, -0.3783f, 27);
	EBCreateFunnyCarEntity(0xfc21ab70, 0.109f, -0.2365f, 0.9435f, 0.0f, 1.5452f, 2.7403f, 28);
	EBCreateFunnyCarEntity(0x40d434d8, 0.0f, 1.652f, 0.3935f, -1.5528f, 1.3525f, -1.5705f, 29);
	printf("~r~CTL: ~w~RPG Sultan by ~y~cerberusxxxx ~w~(30 Objects)");
}
//END cerberusxxxx cars

//start Anon Reporter cars

void EBFunnyCarSabreGTCustom()
{
	SpawnFunnyVehicleModel(0x9b909c94);
	SET_CAR_VISIBLE(myObjectCar, true);

	for (int i = 1; i < 3; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, false);

	EBCreateFunnyCarEntity(0xf186bf2b, 0.775013f, 1.37502f, 0.199996f, -1.56401f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.775013f, 1.42502f, -0.100008f, -1.56401f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.775013f, -1.80002f, -0.100008f, 1.51301f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.775013f, -1.70002f, 0.174996f, 1.39401f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x33684efe, 2.66731E-06f, 0.825015f, 0.174996f, 0.884004f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xfdb87e5, 0.750013f, 0.200007f, 0.449999f, 1.41101f, 1.20701f, -0.187001f, 5);
	EBCreateFunnyCarEntity(0x73b33124, -0.500004f, -0.3f, 0.800004f, 0.0f, 0.0f, 1.59801f, 6);
	EBCreateFunnyCarEntity(0x73b33124, 0.55001f, -0.3f, 0.800004f, 0.0f, 0.0f, 1.59801f, 7);
	EBCreateFunnyCarEntity(0x6a299b19, -0.500004f, -2.17502f, 0.0749943f, -0.255001f, 0.0f, -0.136001f, 8);
	EBCreateFunnyCarEntity(0x6a299b19, 0.375008f, -2.17502f, 0.0749943f, -0.255001f, 0.0f, 0.221001f, 9);
	EBCreateFunnyCarEntity(0x3bf5f0e5, -0.500004f, 1.65003f, -0.175009f, -0.374002f, 0.0f, 3.29801f, 10);
	EBCreateFunnyCarEntity(0x3bf5f0e5, 0.600011f, 1.65003f, -0.175009f, -0.374002f, 0.0f, 3.24701f, 11);
	EBCreateFunnyCarEntity(0x8079978d, -0.900009f, 1.27502f, 0.199996f, -1.49601f, 0.0f, 1.53001f, 12);
	EBCreateFunnyCarEntity(0x8079978d, 0.900015f, 1.25002f, 0.199996f, 1.53001f, 0.0f, 1.59801f, 13);
	EBCreateFunnyCarEntity(0x33684efe, -0.0299993f, -0.810002f, 0.755001f, 1.892f, 0.0f, 0.0f, 14);
	printf("~r~CTL: ~w~Sabre GT (Custom) by ~y~Anon Reporter ~w~(15 Objects)");
}

void EBFunnyCarSolar()
{
	SpawnFunnyVehicleModel(0x779f23aa);
	SET_CAR_VISIBLE(myObjectCar, true);

	for (int i = 1; i < 4; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i == 1));

	EBCreateFunnyCarEntity(0x1ae7947b, -0.440001f, -1.84001f, 0.234999f, 1.54001f, 0.0f, 0.0140001f, 0);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.440003f, -1.84001f, 0.234999f, 1.54001f, 0.0f, 0.0140001f, 1);
	EBCreateFunnyCarEntity(0xe751b6d6, -0.12f, -2.46001f, 0.259999f, -0.0980005f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xe751b6d6, 0.200001f, -2.30001f, 0.259999f, -0.0420002f, 0.0f, 3.12202f, 3);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.840004f, 1.78001f, 0.179999f, -1.45601f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x4dd2ce79, -0.2f, -0.620001f, 0.5f, 1.21801f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x13e8daa2, -0.2f, -0.3f, 0.5f, -0.616003f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x5d796dc6, -0.2f, -0.3f, 0.660001f, 0.0f, 0.0f, 0.0f, 7);
	printf("~r~CTL: ~w~Solar by ~y~Anon Reporter ~w~(8 Objects)");

}

void EBFunnyCarCableCar()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xc6c3242d, 0.140014f, -1.12003f, 7.94031f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Cable Car by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyCarDinghy()
{
	SpawnFunnyVehicleModel(0x3f637729);
	EBCreateFunnyCarEntity(0x3d961290, -0.0549997f, 0.280001f, -0.660003f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Dinghy by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyCarPredator() //can't enter vehicle once left
{
	SpawnFunnyVehicleModel(0x779f23aa);
	EBCreateFunnyCarEntity(0x1915bc55, -0.490001f, 0.135001f, -0.805003f, 0.0f, 0.0f, 3.136f, 0);
	EBCreateFunnyCarEntity(0xe2e7d4ab, 0.0450002f, 0.925001f, -0.380001f, 0.0f, 0.0f, 0.0f, 1);
	printf("~r~CTL: ~w~Predator by ~y~Anon Reporter ~w~(2 Objects)");
}

void EBFunnyCarRocketCarSingle()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xdb0061b6, -0.380007f, -0.179995f, 0.0199804f, 0.0f, 0.0f, 1.58401f, 0);
	EBCreateFunnyCarEntity(0x1b42315d, -0.360007f, -0.560011f, 0.0199804f, 0.0f, 0.0f, 0.0f, 1);
	printf("~r~CTL: ~w~Single Rocket Car by ~y~Anon Reporter ~w~(2 Objects)");
}

void EBFunnyCarRocketCarTwoSeat()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xdb0061b6, -0.420009f, -0.159994f, 0.0199804f, 0.0f, 0.0f, 1.56601f, 0);
	EBCreateFunnyCarEntity(0xdb0061b6, 0.340022f, -0.159994f, 0.0199804f, 0.0f, 0.0f, 1.56601f, 1);
	EBCreateFunnyCarEntity(0xaf64c193, -0.380007f, -0.440006f, 0.0399813f, 0.0f, 0.0f, 3.15002f, 2);
	EBCreateFunnyCarEntity(0xaf64c193, 0.320021f, -0.420005f, 0.0399813f, 0.0f, 0.0f, 3.24002f, 3);
	printf("~r~CTL: ~w~Two Seat Rocket Car by ~y~Anon Reporter ~w~(4 Objects)");
}

void EBFunnyCarRowBoat()
{
	SpawnFunnyVehicleModel(0x432ea949);
	EBCreateFunnyCarEntity(0x640e65f6, -0.2f, 1.41502f, -0.165006f, 0.0f, 0.0f, 2.967f, 0);
	printf("~r~CTL: ~w~Row Boat by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyCarTuga()
{
	SpawnFunnyVehicleModel(0x779f23aa);
	EBCreateFunnyCarEntity(0x3f724e66, -0.635001f, -5.81002f, 1.515f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Tuga by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyCarEvilDuckyvTwo() //TBoGT only I think..
{
	SpawnFunnyVehicleModel(0x39da2754);
	for (int i = 1; i < 6; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i == 2));

	SET_CAR_VISIBLE(myObjectCar, true);
	EBCreateFunnyCarEntity(0x895edc84, -0.515001f, -2.17f, 0.5f, 0.0f, 0.0f, 1.55f, 0);
	EBCreateFunnyCarEntity(0x819d503d, 0.535002f, -2.19001f, 0.5f, 0.0f, 0.0f, 1.57501f, 1);
	EBCreateFunnyCarEntity(-3059060, -0.620001f, 1.80001f, -0.0250017f, 3.72529E-09f, 0.0f, -3.72529E-09f, 2);
	EBCreateFunnyCarEntity(-3059060, 0.220001f, 1.80001f, -0.0250017f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x6a299b19, -0.515001f, 1.48501f, -0.0250017f, -0.525001f, 0.0f, 3.25001f, 4);
	EBCreateFunnyCarEntity(0x6a299b19, 0.640003f, 1.59001f, -0.130002f, -0.550002f, 0.0f, 3.15001f, 5);
	EBCreateFunnyCarEntity(0x3bf5f0e5, -0.410001f, -1.665f, -0.0250017f, -0.675002f, 0.0f, -0.0500001f, 6);
	EBCreateFunnyCarEntity(0x3bf5f0e5, 0.325002f, -1.77001f, 0.0799986f, -0.525001f, 0.0f, 0.125f, 7);
	EBCreateFunnyCarEntity(0x3abe6c1, -0.830002f, 1.38001f, 0.289999f, -1.57501f, 0.0f, 1.57501f, 8);
	EBCreateFunnyCarEntity(0x3abe6c1, 0.850003f, 1.38001f, 0.289999f, 1.55001f, 0.0f, 1.55001f, 9);
	EBCreateFunnyCarEntity(0x6d5e154d, -0.2f, 1.27501f, 0.0799986f, 0.0f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x6d5e154d, 0.220001f, 1.27501f, 0.0799986f, 0.0f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0xaf64c193, 0.0100007f, -0.3f, 1.13f, 0.0f, 0.0f, 3.07501f, 12);
	EBCreateFunnyCarEntity(0xaf64c193, -0.410001f, -0.3f, 1.34f, -1.2f, -1.60001f, 0.0500003f, 13);
	EBCreateFunnyCarEntity(0x3c809fe8, 0.535002f, 1.59001f, 0.5f, 0.025f, 3.72529E-09f, 1.60001f, 14);
	EBCreateFunnyCarEntity(0x3c809fe8, -0.515001f, 1.59001f, 0.5f, 0.025f, 3.72529E-09f, 1.60001f, 15);
	EBCreateFunnyCarEntity(0xe7ae78ea, 0.430002f, -1.455f, 0.289999f, -1.57501f, 0.0f, 1.57501f, 16);
	EBCreateFunnyCarEntity(0x7b90b1b2, 0.115001f, -0.3f, 2.18001f, 0.350001f, 0.400001f, 0.400001f, 17);
	EBCreateFunnyCarEntity(0x7b90b1b2, 0.0100007f, -0.3f, 2.18001f, 0.550002f, 0.550002f, 3.72501f, 18);
	printf("~r~CTL: ~w~Evil Ducky v2 by ~y~Anon Reporter ~w~(19 Objects)");
}

void EBFunnyCarEvilDuckyvThree() //TBoGT only?
{
	SpawnFunnyVehicleModel(0xee6024bc);
	TURN_OFF_VEHICLE_EXTRA(myObjectCar, 1, false);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0x895edc84, -0.515001f, -2.17f, 0.5f, 0.0f, 0.0f, 1.55f, 0);
	EBCreateFunnyCarEntity(0x819d503d, 0.535002f, -2.19001f, 0.5f, 0.0f, 0.0f, 1.57501f, 1);
	EBCreateFunnyCarEntity(-3059060, -0.620001f, 1.80001f, -0.0250017f, 3.72529E-09f, 0.0f, -3.72529E-09f, 2);
	EBCreateFunnyCarEntity(-3059060, 0.220001f, 1.80001f, -0.0250017f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x6a299b19, -0.515001f, 1.48501f, -0.0250017f, -0.525001f, 0.0f, 3.25001f, 4);
	EBCreateFunnyCarEntity(0x6a299b19, 0.640003f, 1.59001f, -0.130002f, -0.550002f, 0.0f, 3.15001f, 5);
	EBCreateFunnyCarEntity(0x3bf5f0e5, -0.410001f, -1.665f, -0.0250017f, -0.675002f, 0.0f, -0.0500001f, 6);
	EBCreateFunnyCarEntity(0x3bf5f0e5, 0.325002f, -1.77001f, 0.0799986f, -0.525001f, 0.0f, 0.125f, 7);
	EBCreateFunnyCarEntity(0x3abe6c1, -0.830002f, 1.38001f, 0.289999f, -1.57501f, 0.0f, 1.57501f, 8);
	EBCreateFunnyCarEntity(0x3abe6c1, 0.850003f, 1.38001f, 0.289999f, 1.55001f, 0.0f, 1.55001f, 9);
	EBCreateFunnyCarEntity(0x6d5e154d, -0.2f, 1.27501f, 0.0799986f, 0.0f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x6d5e154d, 0.220001f, 1.27501f, 0.0799986f, 0.0f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0xaf64c193, 0.0100007f, -0.3f, 1.13f, 0.0f, 0.0f, 3.07501f, 12);
	EBCreateFunnyCarEntity(0xaf64c193, -0.410001f, -0.3f, 1.34f, -1.2f, -1.60001f, 0.0500003f, 13);
	EBCreateFunnyCarEntity(0x3c809fe8, 0.535002f, 1.59001f, 0.5f, 0.025f, 3.72529E-09f, 1.60001f, 14);
	EBCreateFunnyCarEntity(0x3c809fe8, -0.515001f, 1.59001f, 0.5f, 0.025f, 3.72529E-09f, 1.60001f, 15);
	EBCreateFunnyCarEntity(0xe7ae78ea, 0.430002f, -1.455f, 0.289999f, -1.57501f, 0.0f, 1.57501f, 16);
	EBCreateFunnyCarEntity(0x7b90b1b2, 0.115001f, -0.3f, 2.18001f, 0.350001f, 0.400001f, 0.400001f, 17);
	EBCreateFunnyCarEntity(0x7b90b1b2, 0.0100007f, -0.3f, 2.18001f, 0.550002f, 0.550002f, 3.72501f, 18);
	printf("~r~CTL: ~w~Evil Ducky v3 by ~y~Anon Reporter ~w~(19 Objects)");
}

void EBFunnyCarEvilFuckyVFour() //TBoGT only?
{
	SpawnFunnyVehicleModel(0xc1e908d2);
	SET_CAR_VISIBLE(myObjectCar, true);
	for (int i = 1; i < 4; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i == 3));

	EBCreateFunnyCarEntity(0xaf64c193, -0.0299993f, -0.810002f, 0.880001f, 0.0f, 0.0f, -3.19201f, 0);
	EBCreateFunnyCarEntity(0x3129b913, -0.425002f, -0.615002f, 1.085f, -1.554f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x3129b913, 0.340004f, -1.02001f, 1.04f, 1.932f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x7b90b1b2, -0.0199987f, -0.750003f, 1.80501f, 0.504001f, 0.378f, 0.378f, 3);
	EBCreateFunnyCarEntity(0x7b90b1b2, 0.0250016f, -0.885004f, 1.76001f, 0.504001f, 0.378f, 3.94801f, 4);
	EBCreateFunnyCarEntity(0x7b90b1b2, -0.0649991f, -0.750003f, 1.76001f, 0.504001f, 0.378f, 2.39401f, 5);
	EBCreateFunnyCarEntity(0x7b90b1b2, -0.0649991f, -0.795003f, 1.76001f, 0.504001f, 0.378f, 5.838f, 6);
	EBCreateFunnyCarEntity(0x881d4acb, -0.470002f, 1.23001f, -0.175005f, 1.638f, 0.0f, -1.764f, 7);
	EBCreateFunnyCarEntity(0x881d4acb, 0.430005f, 1.36501f, -0.175005f, 4.704f, 0.0840001f, -1.596f, 8);
	EBCreateFunnyCarEntity(0x33684efe, -0.0199987f, 0.330005f, 0.0499968f, 0.924001f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x33684efe, -0.0199987f, -1.87501f, 0.229998f, -1.134f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0xfe424207, -0.785004f, 1.36501f, 0.364999f, 0.0f, 0.0f, 1.596f, 11);
	EBCreateFunnyCarEntity(0xfe424207, 0.745007f, 1.36501f, 0.364999f, 0.0f, 0.0f, 1.596f, 12);
	EBCreateFunnyCarEntity(0x3abe6c1, -0.335001f, 1.14001f, 0.319999f, 0.0f, 0.0f, 1.554f, 13);
	EBCreateFunnyCarEntity(0x3abe6c1, 0.295004f, 1.14001f, 0.319999f, 0.0f, 0.0f, 1.554f, 14);
	EBCreateFunnyCarEntity(0x507edd6d, -0.920005f, -1.42501f, 0.274998f, -1.512f, 0.0f, 1.596f, 15);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.790007f, -1.74001f, 0.00499645f, 1.219f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0xea0506d, -0.0199987f, -2.19001f, 0.319999f, 1.484f, 1.484f, 1.484f, 17);
	printf("~r~CTL: ~w~Evil Ducky v4 by ~y~Anon Reporter ~w~(18 Objects)");
}

void EBFunnyCarGlassCar() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x779f23aa);
	EBCreateFunnyCarEntity(0x8ae7827e, -1.15f, 0.650003f, -0.260003f, -0.702003f, -1.65601f, -4.01402f, 0);
	EBCreateFunnyCarEntity(0x7ebe173b, 0.940004f, -0.994994f, -0.355003f, 0.0f, 0.0f, 3.09602f, 1);
	EBCreateFunnyCarEntity(0x8ae7827e, 1.13001f, 0.650003f, -0.260003f, -0.702003f, -1.65601f, -4.06802f, 2);
	EBCreateFunnyCarEntity(0x7ebe173b, 1.035f, 0.175002f, -0.355003f, 0.0f, 0.0f, 3.09602f, 3);
	EBCreateFunnyCarEntity(0x99f4fa1b, -0.770002f, -1.72501f, 0.214999f, 0.0f, 0.0f, 0.214999f, 4);
	EBCreateFunnyCarEntity(0x99f4fa1b, 0.655003f, -1.82001f, 0.214999f, 0.0f, 0.0f, -0.0360001f, 5);
	EBCreateFunnyCarEntity(0xfe83a5e0, -0.960003f, -0.490001f, 0.975002f, 3.24002f, 1.56601f, 1.62001f, 6);
	EBCreateFunnyCarEntity(0xfe83a5e0, 0.940004f, -0.490001f, 0.975002f, 3.24002f, 1.56601f, 1.62001f, 7);
	EBCreateFunnyCarEntity(0x4ca34312, -0.00999932f, -0.490001f, 1.64f, 1.56601f, 0.0180001f, -0.0180001f, 8);
	EBCreateFunnyCarEntity(0xfe83a5e0, -0.00999932f, -0.00999932f, 0.975002f, 1.54801f, 1.54801f, 1.51201f, 9);
	EBCreateFunnyCarEntity(0x9718e067, 0.085001f, 0.650003f, -0.070002f, 1.60201f, 0.0f, -0.0540002f, 10);
	EBCreateFunnyCarEntity(0xfe83a5e0, -0.00999935f, -0.00999935f, 0.975002f, 3.22202f, 1.56601f, 0.0900005f, 11);
	printf("~r~CTL: ~w~Glass Car by ~y~Anon Reporter ~w~(12 Objects)");

}
//end Anon Reporter cars

//start misc cars
void EBFunnyCarBedCar()
{
	SpawnFunnyVehicleModel(0x3f637729);
	EBCreateFunnyCarEntity(0xe2306ccb, 0.0f, 0.46f, -0.73f, 0.0f, 0.0f, 3.14f, 0);
	EBCreateFunnyCarEntity(0x38412daa, 0.4f, 0.35f, -0.6f, 0.3f, 0.0f, -0.4f, 1);
	printf("~r~CTL: ~w~Bed by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (2 Objects)");
}

void EBFunnyCarUmbrellaBoatCar()
{
	SpawnFunnyVehicleModel(0x3f637729);
	EBCreateFunnyCarEntity(0x640e65f6, 0.04f, 0.0f, -0.4f, 0.0f, 0.0f, 3.14f, 0);
	SET_OBJECT_COLLISION(freeMem->m_object_car_entities[0], true);
	EBCreateFunnyCarEntity(0x1cf1e332, 0.0f, 1.75f, -0.4f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x1cf1e332, 0.0f, -1.95f, -0.4f, 0.0f, 0.0f, 0.0f, 2);
	printf("~r~CTL: ~w~Umbrella Boat Car by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (3 Objects)");

}

void EBFunnyCarBackToTheFuture()
{
	SpawnFunnyVehicleModel(0xf26ceff9);
	SET_CAR_VISIBLE(myObjectCar, true);
	CHANGE_CAR_COLOUR(myObjectCar, 6, 6);
	SET_EXTRA_CAR_COLOURS(myObjectCar, 17, 133);

	for (int i = 1; i < 5; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i != 3));

	EBCreateFunnyCarEntity(0x7830a73c, -0.3f, -1.75f, -0.65f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x7830a73c, 0.3f, -1.75f, -0.65f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xa79efa0a, 0.31f, -1.69f, 0.36f, -1.315f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xa79efa0a, -0.31f, -1.69f, 0.36f, -1.315f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xeddf809, 0.0f, 1.1f, 0.191f, -1.62f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x7329df5e, 0.0f, -1.11f, 0.46f, 1.83f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x55ca3ae6, 0.0f, 2.12f, -0.25f, 1.62f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x55ca3ae6, -0.27f, 2.12f, -0.25f, 1.62f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x55ca3ae6, 0.27f, 2.12f, -0.25f, 1.62f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x55ca3ae6, -0.56f, 2.12f, -0.25f, 1.62f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x55ca3ae6, 0.56f, 2.12f, -0.25f, 1.62f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0xf4a206e4, 0.0f, -1.67f, 0.56f, 3.16f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0xaf6834a4, -0.84f, -1.12f, 0.0695f, 0.0f, -1.26f, 0.0f, 12);
	EBCreateFunnyCarEntity(0xaf6834a4, 0.84f, -1.12f, 0.0695f, 0.0f, 1.26f, 0.0f, 13);
	EBCreateFunnyCarEntity(0x63a2afc6, -0.3f, -2.2f, 0.4f, -1.74f, 0.0f, 0.0f, 14);
	EBCreateFunnyCarEntity(0x63a2afc6, 0.3f, -2.2f, 0.4f, -1.74f, 0.0f, 0.0f, 15);
	printf("~r~CTL: ~w~Back to the Future Car by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (16 Objects)");

}

void EBFunnyCarCockpitCar() // TBoGT Only
{
	SpawnFunnyVehicleModel(0x6827cf72);
	EBCreateFunnyCarEntity(0x948f1a0d, 0.07f, 1.75f, 1.0f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Cockpit by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (1 Object)");
}

void EBFunnyCarGraveAngel()
{
	SpawnFunnyVehicleModel(0x3f637729);
	EBCreateFunnyCarEntity(4770203, -0.4f, 0.1f, 1.25f, 0.0f, 0.0f, 3.14f, 0);
	printf("~r~CTL: ~w~Grave Angel by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (1 Object)");
}

void EBFunnyCarRPGTruck()
{
	SpawnFunnyVehicleModel(0x52db01e0);
	SET_CAR_VISIBLE(myObjectCar, true);

	for (int i = 1; i < 5; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i == 2 || i == 3));

	EBCreateFunnyCarEntity(0xdb691de0, 0.0f, 2.8355f, -0.5313f, 0.858f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xdb691de0, -0.8153f, 2.846f, -0.54f, 0.892f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x33684efe, -0.2853f, 0.915f, 0.4125f, 0.6657f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x33684efe, 0.2903f, 0.9237f, 0.3902f, 0.617f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x98297fb3, -0.3565f, -1.2488f, 0.0592f, 0.0f, 0.0f, 1.5692f, 4);
	EBCreateFunnyCarEntity(0x98297fb3, 0.3383f, -1.2448f, 0.0512f, 0.0f, 0.0f, -1.561f, 5);
	EBCreateFunnyCarEntity(0x4a669952, -0.095f, -1.0908f, 1.3813f, 0.0f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xaf122d49, -0.4393f, 0.2718f, 1.1045f, -0.454f, 0.0f, 3.166f, 7);
	EBCreateFunnyCarEntity(0xaf122d49, 0.4305f, 0.2673f, 1.1115f, -0.4588f, 0.0f, 3.1428f, 8);
	EBCreateFunnyCarEntity(0x579384a4, -0.0165f, -1.4925f, 1.3185f, 0.0f, 0.0f, -1.572f, 9);
	EBCreateFunnyCarEntity(0x48fa2738, 0.0f, -1.9107f, 1.3805f, 0.0f, 0.0f, 1.5565f, 10);
	EBCreateFunnyCarEntity(0x975bd0ea, 0.5355f, 0.2083f, 0.1925f, 1.4807f, -0.1448f, 1.6855f, 11);
	printf("~r~CTL: ~w~RPG Truck by ~y~Colt~w~ (12 Objects)");
}

void EBFunnyCarDragsterBikeOne()
{
	SpawnFunnyVehicleModel(0x779f23aa);
	EBCreateFunnyCarEntity(0xdb0061b6, -0.4542f, -0.1247f, 0.1442f, 0.0f, 0.0f, 1.544f, 0);
	EBCreateFunnyCarEntity(0xdb0061b6, 0.4285f, -0.1548f, 0.1442f, 0.0f, 0.0f, 1.5505f, 1);
	EBCreateFunnyCarEntity(0x3ff24020, -0.9125f, -1.2837f, -0.497f, 0.0f, 0.0f, -1.5598f, 2);
	EBCreateFunnyCarEntity(0x3ff24020, -1.065f, -1.2877f, -0.4948f, 0.0f, 0.0f, -1.5565f, 3);
	EBCreateFunnyCarEntity(0x3ff24020, 0.8717f, -1.339f, -0.4948f, 0.0f, 0.0f, 1.5515f, 4);
	EBCreateFunnyCarEntity(0x3ff24020, 1.016f, -1.335f, -0.4948f, 0.0f, 0.0f, 1.5608f, 5);
	EBCreateFunnyCarEntity(0x3ff24020, 0.0805f, 1.4817f, -0.413f, 0.0f, 0.0f, 1.597f, 6);
	EBCreateFunnyCarEntity(0x3ff24020, -0.048f, 1.4817f, -0.413f, 0.0f, 0.0f, -1.5533f, 7);
	EBCreateFunnyCarEntity(0x455b19af, -0.0627f, -1.3607f, 0.0125f, 0.0f, 0.0f, -0.0307f, 8);
	EBCreateFunnyCarEntity(0xfe740be2, -0.4562f, -0.1625f, -0.246f, 0.0f, 0.0f, -1.5593f, 9);
	EBCreateFunnyCarEntity(0xfe740be2, 0.4635f, -0.1605f, -0.2515f, 0.0f, 0.0f, -4.802f, 10);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.1755f, 0.7747f, -0.2518f, 0.313f, 3.1535f, 0.0028f, 11);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.1515f, 0.7555f, -0.1318f, -0.3175f, -0.0288f, 0.0f, 12);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.0f, -1.3255f, -0.4035f, 0.0f, 0.0f, -1.5752f, 13);
	EBCreateFunnyCarEntity(0x63a2afc6, 0.1298f, -0.8575f, -0.1965f, -0.6438f, 1.6083f, 0.9863f, 14);
	EBCreateFunnyCarEntity(0xcb26803d, -0.6152f, -1.4852f, 0.0105f, 0.7955f, 0.0f, 1.541f, 15);
	EBCreateFunnyCarEntity(0xcb26803d, 0.529f, -1.4622f, 0.0105f, 0.7955f, 0.0f, 1.541f, 16);
	EBCreateFunnyCarEntity(0x2acb53a, 0.475f, 0.8405f, -0.3255f, 0.0f, 1.0558f, -1.5862f, 17);
	EBCreateFunnyCarEntity(0x2acb53a, -0.4055f, 0.8332f, -0.3108f, 0.0f, 0.9803f, -1.575f, 18);
	printf("~r~CTL: ~w~Dragster Bike 1 by ~y~R3B3LY0N~w~ (19 Objects)");
}

void EBFunnyCarEarthShip()
{
	SpawnFunnyVehicleModel(0x1d06d681);
	EBCreateFunnyCarEntity(0x2553daa9, -0.4153f, -0.1045f, -0.192f, -0.1853f, 0.0f, -3.18f, 0);
	EBCreateFunnyCarEntity(0x2553daa9, 0.4305f, -0.1135f, -0.1718f, -0.1592f, 0.0f, -3.2075f, 1);
	EBCreateFunnyCarEntity(0x2553daa9, -0.4398f, -1.194f, -0.0445f, -0.1275f, 0.0f, -3.18f, 2);
	EBCreateFunnyCarEntity(0x2553daa9, 0.443f, -1.1992f, -0.035f, -0.1853f, 0.0f, -3.18f, 3);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 0.4315f, 0.5255f, -2.1718f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 1.1793f, 0.0805f, -4.2305f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 0.5165f, -0.2825f, 1.564f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, -0.253f, -0.2838f, 1.4885f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, -1.0555f, -0.234f, 1.0278f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x206c172, -1.0773f, -1.1148f, 0.076f, -1.5922f, -2.5155f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x206c172, 1.0822f, -1.1328f, 0.0778f, -1.5695f, -0.6195f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x206c172, -2.047f, -0.2383f, 0.42f, 1.623f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x206c172, 2.0535f, -0.2555f, 0.4257f, 1.621f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0x7603c7d7, 2.1275f, 0.486f, 0.21f, 1.5735f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(0x7603c7d7, -2.142f, 0.5048f, 0.21f, 1.5732f, 0.0f, 0.0f, 14);
	EBCreateFunnyCarEntity(0x26b6b845, 0.4425f, -1.0082f, -0.118f, 0.0f, -1.275f, -3.1585f, 15);
	EBCreateFunnyCarEntity(0x26b6b845, -0.4465f, -1.008f, -0.1057f, 0.0f, 1.275f, -3.1575f, 16);
	printf("~r~CTL: ~w~Earth Ship by ~y~ZzLoik69zZ~w~ (17 Objects)");
}

void EBFunnyCarFlintstonesCar()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0x2de0f4b1, -0.0282f, 0.0f, -0.4505f, 0.0f, 0.0f, -0.0973f, 0);
	EBCreateFunnyCarEntity(0xd57ceb37, -0.9675f, 1.0888f, -0.1608f, 0.0f, 0.0f, -1.531f, 1);
	EBCreateFunnyCarEntity(0xd57ceb37, -1.0338f, -1.1795f, -0.1383f, 0.0f, 0.0f, -1.534f, 2);
	EBCreateFunnyCarEntity(0xd57ceb37, 0.9605f, 1.0887f, -0.127f, 0.0f, 0.0f, 1.5692f, 3);
	EBCreateFunnyCarEntity(0xd57ceb37, 0.942f, -1.2033f, -0.1138f, 0.0f, 0.0f, 1.5535f, 4);
	EBCreateFunnyCarEntity(0x81e9e088, 0.3722f, -0.377f, -0.0763f, 1.5818f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x81e9e088, -0.4052f, -0.3758f, -0.0728f, 1.5818f, 0.0f, 0.0f, 6);
	printf("~r~CTL: ~w~Flintstones Car by ~y~ZzLoik69zZ~w~ (7 Objects)");
}

void EBFunnyCarHotRod()
{
	SpawnFunnyVehicleModel(0xc1e908d2);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, -0.307f, 0.8612f, 1.7542f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, -0.908f, -621.0f, 0.395f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, -0.7765f, -0.4345f, 0.395f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, -0.8693f, -0.4198f, -1.5798f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, -0.1065f, -0.4198f, -1.5798f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 0.6562f, -0.4198f, -1.5798f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 1.4245f, -0.4198f, -1.5798f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 2.1877f, -0.4f, 0.0f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 2.1585f, 0.3472f, 1.5798f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 1.4035f, 0.3472f, 1.5798f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 0.505f, -0.402f, 0.0f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x2553daa9, -0.3995f, -0.7198f, -0.3503f, -0.1092f, 0.0f, -3.1762f, 11);
	EBCreateFunnyCarEntity(0x2553daa9, 0.4135f, -0.7198f, -0.3503f, -0.1092f, 0.0f, -3.1762f, 12);
	EBCreateFunnyCarEntity(0xf0083468, 0.7478f, 0.407f, -0.2088f, 1.5815f, 0.0f, 0.0855f, 13);
	EBCreateFunnyCarEntity(0xf0083468, -0.7043f, 0.407f, -0.2088f, 1.5815f, 0.0f, -0.063f, 14);
	EBCreateFunnyCarEntity(0xf0083468, -0.7195f, 0.358f, 0.1165f, 1.583f, 0.0f, -0.0602f, 15);
	EBCreateFunnyCarEntity(0xf0083468, 0.7323f, 0.4048f, 0.109f, 1.5832f, 0.0f, 0.0845f, 16);
	EBCreateFunnyCarEntity(0xd0afce9b, -0.4087f, -0.4672f, -0.4693f, 0.0f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0xd0afce9b, 0.4095f, -0.44672f, -0.4693f, 0.0f, 0.0f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x41303af8, 0.0f, 1.7305f, 0.2885f, 0.0f, 0.0f, 0.0f, 19);
	EBCreateFunnyCarEntity(0x41303af8, -0.596f, 1.7197f, -0.1625f, 0.0f, 0.0f, 0.0f, 20);
	EBCreateFunnyCarEntity(0x41303af8, 0.4325f, 1.7197f, -0.1625f, 0.0f, 0.0f, 0.0f, 21);
	EBCreateFunnyCarEntity(0xb8eee857, 0.0f, 2.2847f, 0.1012f, 0.0f, 0.0f, -3.2395f, 22);
	EBCreateFunnyCarEntity(0xd25a681, 0.0f, -1.2068f, 0.7237f, -2.113f, 0.0f, 0.0f, 23);
	printf("~r~CTL: ~w~Hot Rod by ~y~ZzLoik69zZ~w~ (24 Objects)");
}

void EBFunnyCarLolCar() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x39f9c898);
	SET_CAR_VISIBLE(myObjectCar, true);
	EBCreateFunnyCarEntity(0xbb389d8a, 0.525001f, -2.3f, 0.124998f, -0.3f, 0.0f, -1.6f, 0);
	EBCreateFunnyCarEntity(0xbb389d8a, -0.500001f, -2.32501f, 0.199999f, 0.4f, 0.0f, -1.8f, 1);
	EBCreateFunnyCarEntity(-3059060, -1.175f, -1.72501f, 0.274999f, 0.0f, 0.0f, 0.1f, 2);
	EBCreateFunnyCarEntity(-3059060, 0.775004f, -1.65001f, 0.274999f, 0.0f, 0.0f, -0.1f, 3);
	EBCreateFunnyCarEntity(0x2e9c1b79, -1.025f, 1.57501f, 0.199999f, -1.4f, 0.0f, 1.6f, 4);
	EBCreateFunnyCarEntity(0x2e9c1b79, 1.00001f, 1.57501f, 0.199999f, 1.6f, 0.0f, 1.6f, 5);
	EBCreateFunnyCarEntity(0xf096b408, 0.025001f, 1.65001f, 0.0499979f, -0.3f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xe1ba5804, 0.025001f, -2.47501f, 0.5f, 0.5f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x679db86f, 0.025001f, -4.20002f, 1.7f, 0.0f, 0.0f, -2.3f, 8);
	EBCreateFunnyCarEntity(0xaf045eae, -0.0499993f, -4.27502f, -0.250003f, 1.49012E-08f, 0.0f, -1.6f, 9);
	printf("~r~CTL: ~w~LoL Car by ~y~ChrisTuffurlo9l~w~ (10 Objects)");
}

void EBFunnyCarMiniKart()
{
	SpawnFunnyVehicleModel(VEHICLE_INFERNUS);
	EBCreateFunnyCarEntity(0xd6a0b5b1, -0.7597f, 0.6315f, -0.4115f, 0.0f, 1.5748f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xb817cf53, -0.8792f, 1.1862f, -0.395f, 0.0f, 1.5523f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xb817cf53, -0.9713f, 0.0755f, -0.3875f, 0.0f, 1.5542f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xb817cf53, 0.1063f, 1.186f, -0.4073f, 0.0f, -1.5512f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xb817cf53, 0.114f, 0.084f, -0.404f, 0.0f, -1.55f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x2553daa9, -0.3818f, 0.1473f, -0.3018f, -0.3547f, 0.0f, 3.1278f, 5);
	EBCreateFunnyCarEntity(0xa563ea8f, -0.3777f, 1.0425f, -0.2345f, 0.6995f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xb817cf53, -0.3977f, 0.4195f, 0.2643f, 0.0f, -4.0818f, 1.6522f, 7);
	EBCreateFunnyCarEntity(0x678c228f, -0.403f, 0.575f, 0.2265f, -1.9927f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0xd0afce9b, -0.2028f, 0.1417f, -0.4227f, 0.0f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0xd0afce9b, -0.5415f, 0.1417f, -0.4227f, 0.0f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x572bbd50, -0.8628f, 0.5085f, -0.329f, 0.0f, 0.0f, -1.5555f, 11);
	EBCreateFunnyCarEntity(0x572bbd50, 0.0983f, 0.528f, -0.329f, 0.0f, 0.0f, 1.565f, 12);
	EBCreateFunnyCarEntity(0x8ecd7fa4, 0.0155f, 0.4895f, -0.218f, 3.1252f, 0.0f, 1.569f, 13);
	EBCreateFunnyCarEntity(0x8d05d23d, 0.0363f, 0.4945f, -0.1603f, 0.0f, 0.0f, 1.5725f, 14);
	EBCreateFunnyCarEntity(0x8b5702c9, 0.0555f, 0.9035f, -0.495f, 0.0f, 0.0f, 1.5915f, 15);
	EBCreateFunnyCarEntity(0xe809475c, -0.3873f, 0.7342f, 0.319f, 0.6458f, 0.0f, 0.0f, 16);
	printf("~r~CTL: ~w~Mini Kart by ~y~DsA x Hotsauce~w~ (17 Objects)");
}

void EBFunnyCarMinecraftCar() //YES COLOUR BITCHES - Nathan >:-D 
{
	//WHY THE FUCK IS THIS CAR NOT ENTERABLE BUT IT IS WITH EVILB's?
	SpawnFunnyVehicleModel(0xee6024bc);
	uint Model = OBJECT_CJ_GAME_CUBE_C1;
	int Colour = minecraftFunnyColourSelect;

	switch (Colour)
	{
	case 0: Model = OBJECT_CJ_GAME_CUBE_1; break; //Red (change _C1 to _1 to add original glow back). 
	case 1: Model = OBJECT_CJ_GAME_CUBE_2; break; //Blue 
	case 2: Model = OBJECT_CJ_GAME_CUBE_3; break; //Yellow
	case 3: Model = OBJECT_CJ_GAME_CUBE_4; break; //Green
	case 4: Model = OBJECT_CJ_GAME_CUBE_5; break; //Pink
	case 5: Model = OBJECT_CJ_GAME_CUBE_6; break; //Orange
	}

	EBCreateFunnyCarEntity(Model, -0.3408f, 1.558f, 0.025f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(Model, 0.371f, 1.552f, 0.0242f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(Model, -0.346f, 0.852f, 0.0287f, 0.0f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(Model, 0.3705f, 0.8468f, 0.022f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(Model, -0.3475f, 0.1717f, 0.0273f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(Model, 0.371f, 0.1548f, 0.017f, 0.0f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(Model, -0.3485f, -0.5383f, 0.024f, 0.0f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(Model, 0.3675f, -0.536f, 0.024f, 0.0f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(Model, -0.3445f, -1.2088f, 0.026f, 0.0f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(Model, 0.3655f, -1.2065f, 0.0177f, 0.0f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(Model, -0.3228f, -0.3378f, 0.4527f, 0.0f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(Model, 0.3283f, -0.354f, 0.454f, 0.0f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(Model, -0.318f, 0.0917f, 0.3028f, -0.6987f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(Model, 0.3418f, 0.0925f, 0.307f, -0.6967f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(0x1c5e78f9, -0.8388f, 1.1932f, -0.0615f, 0.0f, 0.0f, -1.6078f, 14);
	EBCreateFunnyCarEntity(0x1c5e78f9, 0.8563f, 1.195f, -0.064f, 0.0f, 0.0f, 1.5768f, 15);
	EBCreateFunnyCarEntity(0x1c5e78f9, 0.846f, -1.0142f, -0.0655f, 0.0f, 0.0f, 1.5475f, 16);
	EBCreateFunnyCarEntity(0x1c5e78f9, -0.8485f, -1.0147f, -0.0638f, 0.0f, 0.0f, -1.5823f, 17);
	EBCreateFunnyCarEntity(0x2553daa9, -0.3385f, -0.2358f, -0.1067f, 0.0f, 0.0f, 3.1445f, 18);
	EBCreateFunnyCarEntity(0x572bbd50, 0.0f, -1.7015f, -0.0797f, 0.0f, 0.0f, 0.0f, 19);
	EBCreateFunnyCarEntity(0x572bbd50, 0.0f, -1.6963f, 0.0262f, 0.0f, 0.0f, 0.0f, 20);
	EBCreateFunnyCarEntity(0x455b19af, 0.017f, 2.0493f, -0.064f, 0.0f, -0.003f, 3.1485f, 21);
	EBCreateFunnyCarEntity(0x41303af8, -0.0745f, 1.5615f, 0.3405f, 0.0f, 0.0f, 0.0f, 22);

	for (int i = 0; i < 14; i++)
		SET_OBJECT_LIGHTS(freeMem->m_object_car_entities[i], bitTest(ctlMem->m_bit_sets.m_vehicle, minecraftFunnyCarGlow)); //works but only on some and of a night time..

	printf("~r~CTL: ~w~Minecraft Car (%s) by ~y~EroiK I StyLeR~w~ (23 Objects)", MinecraftColourStrings[minecraftFunnyColourSelect]); //I don't start at 0 for this functions paramater.
}

void EBFunnyCarDragsterBikeTwo()
{
	SpawnFunnyVehicleModel(0xee6024bc);
	EBCreateFunnyCarEntity(0x2553daa9, -0.3533f, -0.218f, -0.1115f, 0.0f, 0.0f, 3.1835f, 0);
	EBCreateFunnyCarEntity(0x41303af8, -0.309f, -1.243f, 0.1718f, 0.0f, 0.0f, -3.129f, 1);
	EBCreateFunnyCarEntity(0xfe740be2, -0.5022f, 0.0f, -0.0505f, 0.0f, 0.0f, -1.5482f, 2);
	EBCreateFunnyCarEntity(0xfe740be2, -0.2158f, 0.0f, -0.0515f, 0.0f, 0.0f, 1.5762f, 3);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.3048f, -1.2645f, -0.0135f, 0.0f, 0.0f, 1.5895f, 4);
	EBCreateFunnyCarEntity(0x3ff24020, 0.328f, -1.255f, -0.011f, 0.0f, 0.0f, 1.5827f, 5);
	EBCreateFunnyCarEntity(0x3ff24020, 0.5035f, -1.2512f, -0.011f, 0.0f, 0.0f, 1.5848f, 6);
	EBCreateFunnyCarEntity(0x3ff24020, -0.993f, -1.2975f, -0.013f, 0.0f, 0.0f, -1.5183f, 7);
	EBCreateFunnyCarEntity(0x3ff24020, -1.1778f, -1.3043f, -0.0135f, 0.0f, 0.0f, -1.51f, 8);
	EBCreateFunnyCarEntity(0x3ff24020, -0.4172f, 1.8185f, -0.0202f, 0.0f, 0.0f, 4.764f, 9);
	EBCreateFunnyCarEntity(0x3ff24020, -0.3565f, 1.8273f, -0.0155f, 0.0f, 0.0f, 1.6055f, 10);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.161f, 1.078f, -0.0353f, -0.0072f, -1.6275f, 0.116f, 11);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.5713f, 1.0452f, -0.039f, 0.0f, 1.5848f, -0.058f, 12);
	EBCreateFunnyCarEntity(0x3129b913, -0.3333f, 0.6835f, 0.2385f, -3.9455f, 3.1708f, -0.0775f, 13);
	EBCreateFunnyCarEntity(0x3129b913, -0.3965f, 0.687f, 0.2505f, 0.8087f, 0.0587f, 0.0495f, 14);
	printf("~r~CTL: ~w~Dragster Bike 2 by ~y~EroiK I StyLeR~w~ (15 Objects)");
}
//end misc cars

//start cerberusxxxx trucks
void EBFunnyTruckFuckPacker()
{
	SpawnFunnyVehicleModel(0x21eee87d);
	SET_CAR_VISIBLE(myObjectCar, true);

	for (int i = 1; i < 4; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, true);

	//EBCreateFunnyCarEntity(0xe1a05228, 0.0f, -8.3875f, -1.3875f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(OBJECT_EC_TRAILER_FB, 0.0f, -8.3875f, -1.3875f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -0.2513f, 0.845f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -0.252f, 1.7235f, 0.0f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -0.2653f, 2.6068f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -0.2312f, 3.4628f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -1.1503f, 3.449f, 0.0f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -1.2115f, 1.7748f, 0.0f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -2.6602f, 0.849f, 0.0f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -3.5895f, 0.8605f, 0.0f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -8.425f, 0.8362f, 0.0f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -4.3962f, 0.86f, 0.0f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -2.6695f, 1.751f, 0.0f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -2.6933f, 2.6083f, 0.0f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -4.41f, 1.784f, 0.0f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -4.406f, 2.6363f, 0.0f, 0.0f, 0.0f, 14);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -5.8312f, 0.8572f, 0.0f, 0.0f, 0.0f, 15);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -5.8507f, 1.7795f, 0.0f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -5.8582f, 2.6478f, 0.0f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -6.7587f, 0.8417f, 0.0f, 0.0f, 0.0f, 18);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -6.7687f, 2.6525f, 0.0f, 0.0f, 0.0f, 19);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -8.423f, 1.722f, 0.0f, 0.0f, 0.0f, 20);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -8.4222f, 2.5573f, 0.0f, 0.0f, 0.0f, 21);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -8.4287f, 3.4393f, 0.0f, 0.0f, 0.0f, 22);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -10.1033f, 1.0098f, -0.8352f, 0.0f, 0.0f, 23);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -9.453f, 1.6043f, 0.7607f, 0.0f, 0.0f, 24);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -9.4775f, 2.6055f, 0.829f, 0.0f, 0.0f, 25);
	EBCreateFunnyCarEntity(OBJECT_CJ_GAME_CUBE_3, 0.0f, -10.089f, 3.1838f, 0.8118f, 0.0f, 0.0f, 26);
	EBCreateFunnyCarEntity(0xaa14500c, 0.0f, 2.9857f, 1.705f, 0.0f, 0.0f, 3.2305f, 27);
	EBCreateFunnyCarEntity(0xb3615bc4, 0.0f, 2.721f, 0.1237f, -1.5815f, 0.0f, 0.0f, 28);
	EBCreateFunnyCarEntity(0x55ca3ae6, 0.0f, 4.7555f, 0.0287f, 1.573f, 0.0f, 0.0f, 29);
	printf("~r~CTL: ~w~Fuck Packer by ~y~cerberusxxxx~w~ (30 Objects)");
}

void EBFunnyTruckLegoPhantom()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	EBCreateFunnyCarEntity(0xd205564b, 0.0f, 0.0f, 3.5443f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xd205564b, 0.0f, 0.0f, 3.5443f, 0.0f, 0.0f, 3.1343f, 1);
	EBCreateFunnyCarEntity(0xd7c2d9af, 0.0f, -0.1482f, 6.3305f, 0.0f, -1.5658f, -1.5935f, 2);
	EBCreateFunnyCarEntity(0xd7c2d9af, 0.0f, 0.1697f, 6.3143f, 0.0f, -1.549f, 1.544f, 3);
	EBCreateFunnyCarEntity(0x63401612, -1.9148f, 1.4067f, 4.9285f, 1.5825f, -0.5628f, -1.5438f, 4);
	EBCreateFunnyCarEntity(0x63401612, 1.8915f, 0.9488f, 5.5038f, -1.528f, -0.2213f, -1.5765f, 5);
	EBCreateFunnyCarEntity(0xafdf659e, 0.173f, 0.468f, 1.9288f, 1.5957f, 1.583f, 1.5838f, 6);
	EBCreateFunnyCarEntity(0xafdf659e, -0.1273f, 0.5415f, 1.8992f, 0.0f, -1.5815f, 0.0f, 7);
	EBCreateFunnyCarEntity(0xafdf659e, 0.1683f, -0.2515f, 1.9342f, 0.0f, 1.577f, 0.0f, 8);
	EBCreateFunnyCarEntity(0xafdf659e, -0.1085f, -0.198f, 1.8942f, 0.0f, -1.5797f, 0.0f, 9);
	EBCreateFunnyCarEntity(0xe3c3eae4, 0.8948f, 0.5595f, -1.1715f, 0.0f, 0.0f, 3.043f, 10);
	EBCreateFunnyCarEntity(0xe3c3eae4, -0.7705f, 0.5412f, -1.1868f, 0.0f, 0.0f, 3.0725f, 11);
	EBCreateFunnyCarEntity(0xb345db6f, -0.7322f, 0.0835f, 0.4075f, 0.0f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0xb345db6f, 0.753f, 0.0417f, 0.231f, 0.0f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(0xd205564b, 0.0f, 0.0f, 6.8065f, 0.0f, 3.1338f, 0.0f, 14);
	EBCreateFunnyCarEntity(0xabc962bb, -2.104f, 3.4233f, 3.7248f, -0.6238f, -3.1105f, 0.0f, 15);
	EBCreateFunnyCarEntity(0xabc962bb, -2.11f, 3.338f, 3.4603f, -1.9635f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0xabc962bb, 2.0725f, 3.0698f, 4.698f, -1.89f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0xabc962bb, 2.073f, 3.1595f, 5.0695f, -1.355f, 3.17f, 0.0f, 18);
	printf("~r~CTL: ~w~Lego Phantom by ~y~cerberusxxxx~w~ (19 Objects)");
}


void EBFunnyTruckWoodenAPC() //tbogt
{
	SpawnFunnyVehicleModel(0x2189d250);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, -1.3538f, 1.3862f, -1.5698f, -0.0113f, 1.5773f, 0);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, -2.2305f, 1.4052f, 1.566f, 0.0f, -1.5503f, 1);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, -2.7258f, 0.8702f, 1.575f, 1.5987f, -1.542f, 2);
	EBCreateFunnyCarEntity(0x77cf6cd0, 1.3447f, -1.6973f, 0.969f, 1.5905f, 0.9862f, 0.036f, 3);
	EBCreateFunnyCarEntity(0x77cf6cd0, -1.3632f, -1.7193f, 0.981f, 1.5855f, -0.9093f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x77cf6cd0, 1.3505f, 0.2658f, 0.969f, 1.5768f, 1.0055f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x77cf6cd0, -1.339f, 0.3272f, 0.9797f, 1.5442f, -0.9217f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x77cf6cd0, -1.447f, 0.0135f, 0.0375f, 1.5442f, -1.9427f, 0.0158f, 7);
	EBCreateFunnyCarEntity(0x77cf6cd0, 1.448f, 0.0f, -0.0043f, 1.585f, 1.902f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, -0.2865f, 1.407f, 1.5797f, 0.0f, 1.5733f, 9);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, 0.7637f, 1.4072f, 1.5595f, 0.0f, -1.5857f, 10);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, 2.2863f, 1.2558f, 1.3927f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, 3.294f, 0.5222f, 0.0f, 1.5967f, -0.0133f, 12);
	EBCreateFunnyCarEntity(0x77cf6cd0, -0.5348f, -2.761f, 0.3545f, 0.0f, 0.9243f, 3.128f, 13);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.5032f, -2.7625f, 0.3853f, 0.0f, -0.9838f, 3.1553f, 14);
	EBCreateFunnyCarEntity(0x77cf6cd0, -1.1812f, 2.283f, 0.8658f, 1.396f, -0.9382f, 0.0f, 15);
	EBCreateFunnyCarEntity(0x77cf6cd0, 1.1675f, 2.259f, 0.8302f, 1.3438f, 1.008f, 0.0f, 16);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, 1.7825f, 1.3468f, -1.58f, 3.3585f, 1.5863f, 17);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.947f, 2.2605f, 0.999f, 1.291f, 0.3342f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x77cf6cd0, -0.9955f, 2.2678f, 1.0297f, 1.3937f, -0.3433f, -0.019f, 19);
	EBCreateFunnyCarEntity(0x77cf6cd0, -0.8127f, -2.1945f, -0.011f, 0.9365f, 3.108f, 1.5647f, 20);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.7455f, -2.236f, 0.0115f, 0.9172f, 0.0f, 1.628f, 21);
	EBCreateFunnyCarEntity(0x77cf6cd0, -0.5587f, 2.6938f, -0.1118f, -1.0808f, 0.0232f, 1.528f, 22);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.41f, 2.7367f, -0.1667f, -1.1355f, 0.0f, -1.5643f, 23);
	EBCreateFunnyCarEntity(0x77cf6cd0, -0.0073f, 2.9127f, -0.3088f, 1.5852f, -2.4465f, -1.584f, 24);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, 0.9108f, 1.235f, 0.6238f, 0.0f, 0.0f, 25);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, -0.931f, 1.2085f, -0.5813f, 0.0f, 0.0f, 26);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.0f, -0.024f, 2.0497f, 1.5845f, 0.0f, 0.0f, 27);
	EBCreateFunnyCarEntity(0x77cf6cd0, 0.795f, 0.0f, 1.071f, 0.2665f, 0.0f, -1.5778f, 28);
	EBCreateFunnyCarEntity(0x77cf6cd0, -0.7623f, 0.0f, 1.043f, 0.2178f, 0.0f, 1.5475f, 29);
	EBCreateFunnyCarEntity(0xf2cff3e4, 0.0f, 0.0f, 2.0665f, 0.0f, 0.0f, 1.0715f, 30);
	printf("~r~CTL: ~w~Wooden APC by ~y~cerberusxxxx~w~ (31 Objects)");
}

void EBFunnyTruckGardenAPC() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x2189d250);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0x78b1074f, 0.0f, 0.0f, 1.6035f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x78b1074f, 0.0f, -0.629f, 1.213f, 0.9595f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x78b1074f, -0.0305f, -0.4798f, 1.4772f, -1.2705f, 0.0f, 1.755f, 2);
	EBCreateFunnyCarEntity(0x78b1074f, -0.3012f, 0.0f, 1.49f, 0.0f, -1.0575f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x78b1074f, -0.3725f, -0.277f, 1.269f, 0.0f, 0.9048f, -2.7175f, 4);
	EBCreateFunnyCarEntity(0x78b1074f, 0.0f, 0.463f, 1.367f, 0.0f, 0.9773f, -4.5943f, 5);
	EBCreateFunnyCarEntity(0x78b1074f, 0.2682f, -0.348f, 1.372f, 0.0f, 0.7463f, -1.0295f, 6);
	EBCreateFunnyCarEntity(0x78b1074f, 0.4282f, 0.1f, 1.4608f, 0.0f, 0.755f, 0.3897f, 7);
	EBCreateFunnyCarEntity(0x5ab7a851, -0.332f, -2.1613f, 1.6367f, -0.2107f, 2.9715f, 0.1323f, 8);
	EBCreateFunnyCarEntity(0x5ab7a851, 0.4537f, -2.0035f, 1.7015f, 0.0f, 2.9658f, 3.2213f, 9);
	EBCreateFunnyCarEntity(0x5ab7a851, -0.0747f, -1.091f, 1.7205f, 0.0f, 2.9515f, -1.5323f, 10);
	EBCreateFunnyCarEntity(0x5ab7a851, 0.1878f, -2.8777f, 0.927f, 0.0f, 1.352f, 1.5705f, 11);
	EBCreateFunnyCarEntity(0x5ab7a851, 0.0f, -2.8622f, 0.1365f, 0.0f, 1.1892f, 7.969f, 12);
	EBCreateFunnyCarEntity(0x5ab7a851, 0.0f, 2.8265f, 1.0565f, 0.0f, 2.7378f, -1.4513f, 13);
	EBCreateFunnyCarEntity(0x5ab7a851, -1.4175f, -1.7845f, 0.7427f, 0.0f, 1.411f, 0.3735f, 14);
	EBCreateFunnyCarEntity(0x5ab7a851, -1.0588f, -2.5198f, 0.8888f, 0.0f, 1.419f, 0.5455f, 15);
	EBCreateFunnyCarEntity(0x5ab7a851, -1.6033f, -0.3883f, 0.6585f, 1.783f, 0.0f, 1.3445f, 16);
	EBCreateFunnyCarEntity(0x5ab7a851, 0.7865f, -1.8875f, 0.6597f, 0.0f, 1.0907f, 0.0f, 17);
	EBCreateFunnyCarEntity(0x5ab7a851, 1.5227f, -0.6613f, 0.4825f, 0.0f, -1.5035f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x5ab7a851, 1.1907f, -0.9788f, 1.379f, 0.0f, 3.882f, -0.5418f, 19);
	EBCreateFunnyCarEntity(0x5ab7a851, -0.6708f, -1.0755f, 0.9582f, 0.0f, -1.0118f, 0.0f, 20);
	EBCreateFunnyCarEntity(0x59007f07, 0.0f, 0.0f, 0.5742f, 0.0f, 0.0f, 0.0f, 21);
	EBCreateFunnyCarEntity(0x4749db9a, 0.4443f, 1.215f, 0.5085f, 0.0f, 0.0f, 0.0f, 22);
	EBCreateFunnyCarEntity(0x4749db9a, -0.421f, 1.2037f, 0.5025f, 0.0f, 0.0f, 0.0f, 23);
	EBCreateFunnyCarEntity(0x27be9c84, 0.1797f, 1.469f, 0.65f, 0.0f, 0.0f, 0.0f, 24);
	EBCreateFunnyCarEntity(0x28bd1db5, -0.736f, 1.1747f, 0.7372f, 0.0f, -0.801f, 0.0f, 25);
	EBCreateFunnyCarEntity(0x28bd1db5, 0.684f, 1.1425f, 0.7167f, 0.0f, 0.7323f, 0.0f, 26);
	printf("~r~CTL: ~w~Garden APC by ~y~cerberusxxxx~w~ (27 Objects)");
}


void EBFunnyTruckHappyDuck()
{
	SpawnFunnyVehicleModel(0x2189d250);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0xed723bb4, -0.6845f, -1.957f, 0.727f, 1.5765f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xed723bb4, -0.5455f, -1.9775f, 0.8528f, 1.5742f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xed723bb4, -0.5847f, -1.9537f, 0.3883f, 1.5622f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xed723bb4, -0.2943f, -1.9925f, 0.8437f, 1.5582f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xed723bb4, -0.2833f, -1.9557f, 0.3913f, 1.595f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, -1.9967f, 0.842f, 1.5627f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xed723bb4, 0.0f, -1.9645f, 0.35f, 1.5647f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xed723bb4, 0.317f, -2.0227f, 0.815f, 1.5297f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0xed723bb4, 0.6837f, -2.017f, 0.7953f, 1.5557f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0xed723bb4, 0.322f, -2.0483f, 0.3268f, 1.5365f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0xed723bb4, 0.5978f, -2.0205f, 0.367f, 1.561f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0xed723bb4, 0.6925f, -2.0515f, 0.604f, 1.5427f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0xaf64c193, 0.0f, -2.1348f, 1.5042f, -1.1298f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0x38412daa, 0.45f, -3.2173f, 1.538f, -1.6895f, 0.119f, 0.7772f, 13);
	EBCreateFunnyCarEntity(0xeef0ac28, 0.0f, 0.0f, 0.8257f, 0.236f, 0.0f, 0.0f, 14);
	EBCreateFunnyCarEntity(0x66e87fe0, -2.3303f, 1.479f, -0.1625f, 1.537f, 0.0f, 1.6157f, 15);
	EBCreateFunnyCarEntity(0x66e87fe0, 2.3003f, 1.5505f, -0.1657f, 0.0f, -1.537f, 0.0f, 16);
	EBCreateFunnyCarEntity(0x66e87fe0, -2.4195f, -1.5328f, -0.064f, 0.0f, 1.61f, 0.0f, 17);
	EBCreateFunnyCarEntity(0x66e87fe0, 2.3823f, -1.5012f, -0.1218f, 0.0f, -1.5728f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x2f83bfe1, 0.0f, 3.1085f, 0.0297f, 7.968f, 8.6885f, 0.069f, 19);
	EBCreateFunnyCarEntity(0xb8eee857, 0.0f, 4.077f, 0.5807f, 0.0f, 0.0f, -3.151f, 20);
	printf("~r~CTL: ~w~Happy Duck v1 by ~y~cerberusxxxx~w~ (21 Objects)");

}
//end cerberusxxxx trucks
//start misc trucks
void EBFunnyTruckMonsterTruck()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	EBCreateFunnyCarEntity(0x5be14b23, -0.005f, 1.9995f, 0.2328f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xf40be44b, -1.4255f, 0.6835f, -0.6207f, 0.0f, 1.5435f, 0.0275f, 1);
	EBCreateFunnyCarEntity(0xf40be44b, 0.7985f, 0.6772f, -0.6142f, 0.0f, 1.5625f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xf40be44b, 1.0323f, 0.6667f, -0.613f, 0.0f, 1.5585f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xf40be44b, -1.1735f, 0.694f, -0.6213f, 0.0f, 1.5232f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xf40be44b, -1.106f, 3.6695f, -0.6683f, 0.0f, 1.5632f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xf40be44b, 0.71f, 3.6373f, -0.6422f, 0.0f, 1.5765f, 0.027f, 6);
	EBCreateFunnyCarEntity(0xf40be44b, 0.9583f, 3.6428f, -0.6427f, 0.0f, 1.5862f, 0.0f, 7);
	EBCreateFunnyCarEntity(0xf40be44b, -1.3782f, 3.66f, -0.677f, 0.0f, 1.5573f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x3ff24020, -1.2935f, 0.6893f, -0.6153f, 0.0f, 0.411f, -1.5203f, 9);
	EBCreateFunnyCarEntity(0x3ff24020, -1.2505f, 3.6505f, -0.6845f, 0.0f, 1.241f, -1.575f, 10);
	EBCreateFunnyCarEntity(0x3ff24020, 1.2298f, 0.668f, -0.6088f, 0.0f, 0.826f, 1.5687f, 11);
	EBCreateFunnyCarEntity(0x3ff24020, 1.1618f, 3.6465f, -0.6408f, 0.0f, -0.1125f, 1.5683f, 12);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.0348f, 3.6488f, -0.5025f, 0.0f, 0.0f, 1.5797f, 13);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.0348f, 0.812f, -0.4528f, 0.0f, 0.0f, 1.5797f, 14);
	EBCreateFunnyCarEntity(0xf814ad1e, 0.575f, 0.873f, -0.4978f, -0.7555f, -1.228f, -0.784f, 15);
	EBCreateFunnyCarEntity(0xf814ad1e, 0.6108f, 3.6952f, -0.5783f, 1.279f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0xf814ad1e, -0.568f, 3.5733f, -0.5305f, 1.29f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0xf814ad1e, -0.5425f, 0.8733f, -0.512f, -1.3182f, 0.0f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x928b899d, 0.0f, 3.5885f, 1.26f, -1.613f, 0.0f, -3.1315f, 19);
	EBCreateFunnyCarEntity(0x455b19af, 0.0f, 4.351f, 0.577f, 0.0f, 0.0f, 3.1448f, 20);
	EBCreateFunnyCarEntity(0x803002a3, 0.8232f, 1.898f, 3.0425f, 0.0f, 1.5748f, 1.5533f, 21);
	EBCreateFunnyCarEntity(0x41303af8, -0.0912f, 3.8518f, 1.3005f, 0.0f, 0.0f, 0.0f, 22);
	EBCreateFunnyCarEntity(0xead6e4a4, 0.0f, 0.04f, 0.5372f, 0.0f, 0.0f, 0.0f, 23);
	EBCreateFunnyCarEntity(0xfe740be2, -0.685f, 2.157f, 0.5147f, 0.0f, 0.0f, -1.5652f, 24);
	EBCreateFunnyCarEntity(0xfe740be2, 0.708f, 2.208f, 0.508f, 0.0f, 0.0f, 1.576f, 25);
	EBCreateFunnyCarEntity(0xb76637d8, 0.3362f, 3.971f, 0.8913f, -1.1773f, 0.0f, -1.598f, 26);
	EBCreateFunnyCarEntity(0xb76637d8, -0.4987f, 3.9825f, 0.7625f, 0.4668f, 0.0148f, -1.5822f, 27);
	EBCreateFunnyCarEntity(0x572bbd50, 0.0f, -0.3945f, 0.647f, 0.0f, 0.0f, 0.0f, 28);
	EBCreateFunnyCarEntity(0xd0afce9b, -0.4423f, 0.2752f, 0.4787f, 0.154f, 0.0f, 0.0f, 29);
	EBCreateFunnyCarEntity(0xd0afce9b, 0.4235f, 0.2752f, 0.4903f, 0.1932f, 0.0f, 0.0f, 30);
	printf("~r~CTL: ~w~Monster Truck By ~y~R3B3LY0N~w~ & ~y~Ti jo P1CK UP~w~ (31 Objects)");
}

void EBFunnyTruckMonsterTruckChasis()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	EBCreateFunnyCarEntity(0xf40be44b, -1.4255f, 0.6835f, -0.6207f, 0.0f, 1.5435f, 0.0275f, 0);
	EBCreateFunnyCarEntity(0xf40be44b, 0.7985f, 0.6772f, -0.6142f, 0.0f, 1.5625f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xf40be44b, 1.0323f, 0.6667f, -0.613f, 0.0f, 1.5585f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xf40be44b, -1.1735f, 0.694f, -0.6213f, 0.0f, 1.5232f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xf40be44b, -1.106f, 3.6695f, -0.6683f, 0.0f, 1.5632f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xf40be44b, 0.71f, 3.6373f, -0.6422f, 0.0f, 1.5765f, 0.027f, 5);
	EBCreateFunnyCarEntity(0xf40be44b, 0.9583f, 3.6428f, -0.6427f, 0.0f, 1.5862f, 0.0f, 6);
	EBCreateFunnyCarEntity(0xf40be44b, -1.3782f, 3.66f, -0.677f, 0.0f, 1.5573f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x3ff24020, -1.2935f, 0.6893f, -0.6153f, 0.0f, 0.411f, -1.5203f, 8);
	EBCreateFunnyCarEntity(0x3ff24020, -1.2505f, 3.6505f, -0.6845f, 0.0f, 1.241f, -1.575f, 9);
	EBCreateFunnyCarEntity(0x3ff24020, 1.2298f, 0.668f, -0.6088f, 0.0f, 0.826f, 1.5687f, 10);
	EBCreateFunnyCarEntity(0x3ff24020, 1.1618f, 3.6465f, -0.6408f, 0.0f, -0.1125f, 1.5683f, 11);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.0348f, 3.6488f, -0.5025f, 0.0f, 0.0f, 1.5797f, 12);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.0348f, 0.812f, -0.4528f, 0.0f, 0.0f, 1.5797f, 13);
	EBCreateFunnyCarEntity(0xf814ad1e, 0.575f, 0.873f, -0.4978f, -0.7555f, -1.228f, -0.784f, 14);
	EBCreateFunnyCarEntity(0xf814ad1e, 0.6108f, 3.6952f, -0.5783f, 1.279f, 0.0f, 0.0f, 15);
	EBCreateFunnyCarEntity(0xf814ad1e, -0.568f, 3.5733f, -0.5305f, 1.29f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0xf814ad1e, -0.5425f, 0.8733f, -0.512f, -1.3182f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0x803002a3, 0.8232f, 1.898f, 3.0425f, 0.0f, 1.5748f, 1.5533f, 18);
	printf("~r~CTL: ~w~Monster Truck Chasis By ~y~R3B3LY0N~w~ & ~y~Ti jo P1CK UP~w~ (19 Objects)");
}

void EBFunnyTruckBritishAPC() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x2189d250);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0x819d503d, -1.11f, -0.159999f, 0.990002f, 0.0f, 0.0f, 1.404f, 0);
	EBCreateFunnyCarEntity(0x819d503d, 1.06001f, -0.0199987f, 0.780001f, 0.0f, 0.0f, 1.768f, 1);
	EBCreateFunnyCarEntity(0x4a18cf44, 0.010001f, 3.06002f, 0.850002f, 0.0f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xd0afce9b, 0.500003f, -1.98001f, -0.0600026f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xdcc87847, 0.010001f, -2.54001f, 0.289999f, -1.17f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xd6df5631, 0.010001f, -2.05001f, 0.780001f, 0.598002f, 0.0f, 0.0f, 5);
	printf("~r~CTL: ~w~British APC by ~y~ChrisTuffurlo9l~w~ (6 Objects)");
}

void EBFunnyTruckBrokenPlaneTrailer() //TBoGT Only
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	EBCreateFunnyCarEntity(0x948f1a0d, 0.0f, 1.8f, 1.0f, -0.37f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x1e123ce2, 0.0f, -10.85f, -1.14f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x4cdc5d52, 0.0f, -9.2f, 1.7f, 0.0f, 0.0f, 0.0f, 2);
	printf("~r~CTL: ~w~Broken Plane Trailer by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (3 Objects)");
}

void EBFunnyTruckWingBus()
{
	SpawnFunnyVehicleModel(0xd577c962);
	EBCreateFunnyCarEntity(0x7d299029, 0.0f, -3.78f, -0.82f, 0.0f, 0.0f, 3.14f, 0);
	printf("~r~CTL: ~w~Wing Bus by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (1 Object)");
}

void EBFunnyTruckMerikaVan() //TBoGT Only
{
	SpawnFunnyVehicleModel(0xef1e8f88);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0x6e0a268d, -0.5f, -4.5f, 1.5f, 1.0f, 0.0f, 3.14f, 0);
	EBCreateFunnyCarEntity(0x6e0a268d, 0.5f, -4.5f, 1.5f, 1.0f, 0.0f, 3.14f, 1);
	EBCreateFunnyCarEntity(0x128d049c, -0.16f, 2.9f, 0.05f, 1.57f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x128d049c, -0.16f, 2.9f, 0.9f, 1.57f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x128d049c, -0.16f, 2.9f, 1.75f, 1.57f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x9215c587, -1.4f, 1.0f, 1.0f, -1.57f, -1.57f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x9215c587, 1.4f, 1.0f, 1.0f, -1.57f, -1.57f, 0.0f, 6);
	printf("~r~CTL: ~w~Merika Van by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (6 Objects)");
}

void EBFunnyTruckSemiHauler()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0xe1a05228, 0.0f, -10.8f, -1.1f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x465f9abe, 0.0f, -5.0f, 1.3f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x69ba8953, 0.0f, -11.0f, 1.35f, 0.0f, 0.0f, 0.0f, 2);
	printf("~r~CTL: ~w~Semi Hauler by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (3 Objects)");
}

void EBFunnyTruckDeathTractorTrailer()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0x72624c72, -0.2f, -11.0101f, -1.18001f, 0.0f, 0.0f, -0.0240001f, 0);
	EBCreateFunnyCarEntity(0x2ff750a6, 0.710004f, 5.58003f, 0.5f, 1.008f, 1.584f, 3.04801f, 1);
	EBCreateFunnyCarEntity(0x2ff750a6, -0.620002f, 5.58003f, 0.43f, 0.528002f, 1.512f, 1.512f, 2);
	EBCreateFunnyCarEntity(0x33684efe, 0.500003f, 2.57001f, 0.920002f, 0.0960003f, 0.0f, -0.264001f, 3);
	EBCreateFunnyCarEntity(0x33684efe, -0.480001f, 2.57001f, 0.990002f, 0.144f, 0.0f, 0.168f, 4);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.780005f, 4.18002f, 0.5f, -0.552002f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.780005f, 3.90002f, 0.710001f, -1.224f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x2cdff719, -1.39001f, 3.76002f, -0.620005f, -1.536f, 0.0f, 1.632f, 7);
	EBCreateFunnyCarEntity(0x2cdff719, 1.42498f, 3.78501f, -0.585003f, 1.536f, 0.0f, 1.536f, 8);
	EBCreateFunnyCarEntity(0xdd16f3a4, 0.010001f, 1.87001f, 0.359999f, 0.0f, 0.0f, -3.12402f, 9);
	EBCreateFunnyCarEntity(0x16de5527, -1.46001f, 0.400003f, 0.149998f, 0.0f, 0.0f, -1.57301f, 10);
	EBCreateFunnyCarEntity(0x16de5527, 1.41001f, 0.330003f, 0.219999f, 0.0220001f, 0.0f, 1.62801f, 11);
	EBCreateFunnyCarEntity(0x29377074, 0.010001f, 4.25002f, 1.27f, 0.0f, 0.0f, 0.0f, 12);

	for (int i = 1; i < 3; i++)
		SET_OBJECT_COLLISION(freeMem->m_object_car_entities[i], true);

	printf("~r~CTL: ~w~Death Tractor Trailer by ~y~Anon Reporter~w~ (13 Objects)");
}

void EBFunnyTruckKillerFlatbed()
{
	SpawnFunnyVehicleModel(0x50b0215a);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0xf6b15a0c, -0.619998f, 4.17999f, 1.34f, -1.8f, 0.0f, 0.719999f, 0);
	EBCreateFunnyCarEntity(0xf6b15a0c, 0.640003f, 4.21501f, 1.34f, -1.824f, 0.0f, -0.720002f, 1);
	EBCreateFunnyCarEntity(0x21c6a68a, -1.77501f, 5.58002f, 1.025f, -1.512f, -1.512f, 5.42403f, 2);
	EBCreateFunnyCarEntity(0x21c6a68a, 1.90001f, 5.79002f, -0.445003f, 1.992f, 1.632f, 4.34402f, 3);
	EBCreateFunnyCarEntity(0xf6b15a0c, 0.0100007f, 4.32001f, -0.0250017f, -1.536f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x301f2ce5, 0.115001f, 6.10502f, 1.025f, 1.344f, 1.464f, 1.368f, 5);
	EBCreateFunnyCarEntity(0xa869946f, 0.0100007f, 2.32501f, 1.34f, 0.12f, 0.0f, -0.0240001f, 6);
	EBCreateFunnyCarEntity(0x7329df5e, -0.725002f, 2.11501f, 1.76f, 0.144f, 0.0f, 0.0480001f, 7);
	EBCreateFunnyCarEntity(0x7329df5e, 0.745003f, 2.11501f, 1.865f, 0.0f, 0.0f, -0.0960003f, 8);
	EBCreateFunnyCarEntity(-3059060, -0.305f, 3.69001f, 1.76f, 0.0f, 0.0f, -3.19201f, 9);
	EBCreateFunnyCarEntity(-3059060, 0.220001f, 3.69001f, 1.76f, 0.0f, 0.0f, 0.024f, 10);
	EBCreateFunnyCarEntity(0x6d5e154d, 0.430002f, 3.06001f, 1.34f, 0.0f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x6d5e154d, -0.515001f, 3.06001f, 1.34f, 0.0f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0x3bf5f0e5, -0.410001f, 2.01001f, 2.60001f, -0.528002f, 0.0f, 3.14401f, 13);
	EBCreateFunnyCarEntity(0x3bf5f0e5, 0.535002f, 2.01001f, 2.60001f, -0.480001f, 0.0f, 3.12001f, 14);

	SET_OBJECT_COLLISION(freeMem->m_object_car_entities[5], true);
	printf("~r~CTL: ~w~Killer Flatbed by ~y~Anon Reporter~w~ (15 Objects)");
}

void EBFunnyTruckOversizedLoad()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0xd819bc1, -0.0399993f, -10.94f, -1.02001f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xd91e0e0c, 0.120001f, -3.50001f, 0.58f, 0.0f, 0.0f, -1.58201f, 1);
	EBCreateFunnyCarEntity(0x1ae7947b, -0.360001f, 3.94002f, 0.5f, 0.0f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xd91e0e0c, 0.120001f, -6.14002f, 0.58f, 0.0f, 0.0f, -1.58201f, 3);
	EBCreateFunnyCarEntity(0xd91e0e0c, 0.120001f, -8.70003f, 0.58f, 0.0f, 0.0f, -1.58201f, 4);
	EBCreateFunnyCarEntity(0xacc7bcca, -0.2f, -6.22003f, 3.30001f, 0.0f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.440002f, 3.94002f, 0.5f, 0.0f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x5d796dc6, -0.2f, -6.22003f, 4.50002f, 0.0f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0xf6b15a0c, 0.040001f, 3.94002f, 0.0999982f, -1.54001f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0xf6b15a0c, 0.040001f, 4.02002f, 0.740001f, -1.86201f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.840004f, 5.70003f, 0.179999f, -1.41401f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.840004f, 5.70003f, 0.179999f, 0.0f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.840004f, 5.70003f, 0.179999f, -2.12801f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0xf186bf2b, -0.520002f, 5.70003f, 0.179999f, -1.41401f, 0.0f, 0.616003f, 13);
	EBCreateFunnyCarEntity(0xf186bf2b, 1.96001f, 4.82003f, 0.179999f, -1.41401f, 0.0f, -0.574002f, 14);

	for (int i = 10; i < 15; i++) //objects 10 - 14
		SET_OBJECT_COLLISION(freeMem->m_object_car_entities[i], true);

	printf("~r~CTL: ~w~Oversized Load by ~y~Anon Reporter~w~ (15 Objects)");
}

void EBFunnyTruckSchoolBus()
{
	SpawnFunnyVehicleModel(0xd577c962);
	EBCreateFunnyCarEntity(0x9df75694, 0.200002f, 3.62002f, 0.740001f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xe751b6d6, -0.600002f, 5.38003f, 0.900002f, 0.06f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xcf72a90c, -0.760002f, 4.50002f, -0.220003f, 0.0f, 0.0f, 3.18f, 2);
	EBCreateFunnyCarEntity(0xe751b6d6, 1.00001f, 5.38003f, 0.900002f, 0.06f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xe751b6d6, 0.280002f, 5.38003f, 0.900002f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xfa9edd70, -1.08f, 0.0200013f, 0.900002f, 0.0f, 0.0f, 1.56f, 5);
	EBCreateFunnyCarEntity(0xfa9edd70, 1.48001f, 0.0200013f, 0.900002f, 0.0f, 0.0f, 1.56f, 6);
	EBCreateFunnyCarEntity(0xe751b6d6, -1.0f, -1.1f, 0.900002f, 0.0f, 0.0f, 1.56f, 7);
	EBCreateFunnyCarEntity(0xe751b6d6, 1.48001f, -1.1f, 0.900002f, 0.0f, 0.0f, 1.56f, 8);
	EBCreateFunnyCarEntity(0x71056188, -0.360001f, -2.54001f, 0.820001f, 0.0f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x3bf5f0e5, -0.840003f, 6.50003f, -0.140003f, -0.6f, 0.0f, 3.24f, 10);
	EBCreateFunnyCarEntity(0x3bf5f0e5, 1.32001f, 6.50003f, -0.140003f, -0.66f, 0.0f, 3.24f, 11);

	printf("~r~CTL: ~w~School Bus by ~y~Anon Reporter~w~ (12 Objects)");
}

void EBFunnyTruckSemiWithTrailer()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	SET_CAR_VISIBLE(myObjectCar, true);
	EBCreateFunnyCarEntity(0xdff55b87, -0.2f, -11.3601f, -1.18001f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Semi With Trailer by ~y~Anon Reporter~w~ (1 Object)");
}

void EBFunnyTruckSemiWithTrailerVThree()  //might be TBoGT only
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0x8df09a1d, -0.0399993f, -10.94f, -1.18001f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x8ae7827e, -1.32f, 0.500003f, 0.5f, 0.0f, 0.0f, 3.01002f, 1);
	EBCreateFunnyCarEntity(0x8ae7827e, 1.32001f, 0.500003f, 0.5f, 0.0f, 0.0f, -0.0420014f, 2);
	EBCreateFunnyCarEntity(0xf186bf2b, -0.760002f, 0.340003f, 2.02001f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.760004f, 0.340003f, 2.02001f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xf186bf2b, 0.840004f, 4.10002f, 0.660001f, -0.882004f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x2189d250, -0.0399993f, -9.82003f, 1.94001f, 0.0f, 0.0f, 3.16402f, 6); //Vehicle _APC? /0x2189d250
	EBCreateFunnyCarEntity(0x2189d250, -0.200001f, -6.78003f, 1.70001f, 1.56801f, 1.56801f, 4.71803f, 7);
	EBCreateFunnyCarEntity(0x2189d250, 0.280001f, -6.78003f, 1.70001f, 1.56801f, 1.56801f, 7.88202f, 8);
	EBCreateFunnyCarEntity(0x2189d250, -0.0399993f, -4.14001f, 1.94001f, 0.0f, 0.0f, 6.27202f, 9);

	printf("~r~CTL: ~w~Semi With Trailer v3 by ~y~Anon Reporter~w~ (10 Objects)");

}

void EBFunnyTruckLocobrico()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	EBCreateFunnyCarEntity(0x261f51fa, 0.0f, 0.2612f, -0.2682f, -11.0105f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x261f51fa, 0.0f, 1.639f, -0.3608f, 1.5405f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x261f51fa, 0.0f, 3.1453f, -0.2595f, 1.5675f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x261f51fa, 0.0f, 0.185f, -0.4333f, -1.582f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x3dafb5fd, -0.5678f, 2.7193f, 0.6752f, 0.0f, 0.7382f, -1.5695f, 4);
	EBCreateFunnyCarEntity(0x1d5cdea4, 0.0f, 3.7195f, 0.1338f, 0.0f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 4.64f, 0.178f, -2.4135f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 4.4137f, -0.993f, 5.411f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x2dca551c, -0.037f, 2.8532f, 1.2167f, 3.631f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x2553daa9, -0.5872f, 1.6398f, 0.596f, 0.0f, 0.0f, -3.1443f, 9);
	EBCreateFunnyCarEntity(0x2553daa9, 0.5815f, 1.6728f, 0.575f, 0.0f, 0.0f, -3.1777f, 10);
	EBCreateFunnyCarEntity(0x206c172, 0.0f, 3.0618f, 0.0225f, 0.0f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x41303af8, 0.0f, 3.9388f, 0.3077f, 0.0f, 0.0f, 0.0f, 12);
	EBCreateFunnyCarEntity(0x1d5cdea4, 0.0f, 1.0745f, 0.338f, 0.0f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(0x1d5cdea4, 0.0f, -0.7553f, 0.338f, 0.0f, 0.0f, 0.0f, 14);
	EBCreateFunnyCarEntity(0x1d5cdea4, 0.0f, -2.591f, 0.338f, 0.0f, 0.0f, 0.0f, 15);
	EBCreateFunnyCarEntity(0x1d5cdea4, 0.0f, -3.3105f, 0.338f, 0.0f, 0.0f, 0.0f, 16);
	printf("~r~CTL: ~w~Locobrico by ~y~ZzLoik69zZ~w~ (17 Objects)");
}
//end misc trucks

//start misc vikes
void EBFunnyBike4Wheel()
{
	SpawnFunnyVehicleModel(0x2ef89e46);
	EBCreateFunnyCarEntity(0xb66daf34, 0.0f, 0.1023f, -0.4622f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xa4f54db, -0.1892f, -0.6978f, -0.22f, 0.0f, 0.0f, -1.5743f, 1);
	EBCreateFunnyCarEntity(0xa4f54db, 0.2245f, -0.6978f, -0.2175f, 0.0f, 0.0f, 1.6027f, 2);
	EBCreateFunnyCarEntity(0xa4f54db, 0.2323f, 0.74f, -0.225f, 0.0f, 0.0f, 1.6292f, 3);
	EBCreateFunnyCarEntity(0xa4f54db, -0.1847f, 0.7187f, -0.22f, 0.0f, 0.0f, -1.506f, 4);
	EBCreateFunnyCarEntity(0x3bf5f0e5, 0.0475f, 0.728f, 0.2837f, -0.505f, 0.0f, 3.1383f, 5);
	EBCreateFunnyCarEntity(0x6a299b19, 0.0355f, -0.6325f, -0.04f, -0.3903f, 0.0f, 0.0f, 6);
	EBCreateFunnyCarEntity(0x63a2afc6, 0.0f, -0.9782f, -0.1282f, -1.6048f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x63a2afc6, 0.0f, -0.947f, -0.0108f, -1.6445f, 0.0f, 0.0f, 8);
	printf("~r~CTL: ~w~4 Wheel Bike by ~y~Ti jo P1CK UP~w~ (9 Objects)");
}

void EBFunnyBikeDeathBike()
{
	SpawnFunnyVehicleModel(0x92e56a2c);
	EBCreateFunnyCarEntity(0xf6c440d4, 0.0f, 0.9725f, -0.246f, 0.0f, 0.0f, 1.5767f, 0);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.0f, 0.0f, -0.1872f, 1.5368f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.0f, 0.0f, -0.1872f, 1.5368f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.0f, 0.0f, -0.1872f, 1.5368f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xd57ceb37, 0.1542f, 1.572f, -0.1708f, 0.0f, 0.0f, 1.561f, 4);
	EBCreateFunnyCarEntity(0xd57ceb37, -0.169f, 1.583f, -0.1717f, 0.0f, 0.0f, -1.5682f, 5);
	EBCreateFunnyCarEntity(0xd57ceb37, -0.4298f, -0.8588f, -0.1193f, 0.0f, 0.0f, 1.5653f, 6);
	EBCreateFunnyCarEntity(0xd57ceb37, 0.4145f, -0.8565f, -0.1115f, 0.0f, 0.0f, 4.724f, 7);
	EBCreateFunnyCarEntity(0xd57ceb37, -0.7103f, -0.844f, -0.119f, 0.0f, 0.0f, 1.5668f, 8);
	EBCreateFunnyCarEntity(0xd57ceb37, 0.7032f, -0.8795f, -0.1088f, 0.0f, 0.0f, -1.5693f, 9);
	printf("~r~CTL: ~w~Death Bike by ~y~Ti jo P1CK UP~w~ (10 Objects)");
}

void EBFunnyBikeDickRider()
{
	SpawnFunnyVehicleModel(0x47b9138a);
	EBCreateFunnyCarEntity(0xb570f614, 0.0f, -0.09f, -0.11f, -1.6f, 3.15f, 0.0f, 0);
	printf("~r~CTL: ~w~Dick Rider by ~y~Evil Blunt ~w~(1 Object)");
}

void EBFunnyBikeFourWheeler()
{
	SpawnFunnyVehicleModel(0x2ef89e46);
	EBCreateFunnyCarEntity(0x2ef89e46, -0.0199984f, -0.029998f, 0.00499645f, 0.0f, 0.0f, 0.0f, 0); //spawned as vehicle on PS3/PC from what I can tell. (Object Vehicles don't crash on xbox so meh).
	EBCreateFunnyCarEntity(0x784cbd16, -0.275f, 0.749999f, -0.13f, 0.0f, 0.0f, -1.57001f, 1);
	EBCreateFunnyCarEntity(0x784cbd16, -0.275f, -0.765f, -0.13f, 0.0f, 0.0f, -1.57001f, 2);
	EBCreateFunnyCarEntity(0x784cbd16, 0.235f, 0.749999f, -0.13f, 0.0f, 0.0f, -4.67003f, 3);
	EBCreateFunnyCarEntity(0x784cbd16, 0.235f, -0.69f, -0.16f, 0.0f, 0.0f, -4.67003f, 4);
	printf("~r~CTL: ~w~Four Wheeler by: ~y~Anon Reporter ~w~(5 Objects)");
}

void EBFunnyBikeThreeWheeler()
{
	SpawnFunnyVehicleModel(0x2ef89e46);
	EBCreateFunnyCarEntity(0x7e8b4993, -0.235f, -0.685004f, -0.200007f, 0.0f, 0.0f, 1.59f, 0);
	EBCreateFunnyCarEntity(0x7e8b4993, 0.255004f, -0.685004f, -0.200007f, 0.0f, 0.0f, 4.74001f, 1);
	EBCreateFunnyCarEntity(0x784cbd16, 0.010002f, 0.78501f, -0.200007f, 0.0f, 0.0f, 1.56f, 2);
	EBCreateFunnyCarEntity(0x2ef89e46, 0.010002f, -0.0199974f, -0.0250049f, 0.0f, 0.0f, 0.0f, 3);
	printf("~r~CTL: ~w~Three Wheeler by: ~y~Anon Reporter ~w~(3 Objects)");
}

void EBFunnyBikeBatBike()
{
	SpawnFunnyVehicleModel(0x47b9138a);
	EBCreateFunnyCarEntity(0xf40be44b, 0.0f, 1.8832f, 0.0488f, 0.0f, 1.55f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xf40be44b, -0.1887f, 1.8837f, 0.0445f, 0.0f, 1.5443f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xf40be44b, 0.0f, -1.6248f, 0.108f, 0.0f, 1.5595f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xf40be44b, 0.1258f, -1.6248f, 0.1095f, 0.0f, -1.5845f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xe8577885, -0.5868f, 1.78f, 0.0265f, -1.547f, -4.669f, -1.4587f, 4);
	EBCreateFunnyCarEntity(0xe8577885, 0.7705f, 1.8267f, 0.0427f, -0.0235f, -1.6013f, 0.0045f, 5);
	EBCreateFunnyCarEntity(0x6535994a, -0.5667f, 2.769f, 0.036f, -1.57f, 0.0f, -3.0832f, 6);
	EBCreateFunnyCarEntity(0x6535994a, 0.6987f, 2.808f, 0.044f, 1.5578f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x6535994a, -0.5632f, 2.6717f, 0.0357f, -1.5832f, -0.02f, 0.0265f, 8);
	EBCreateFunnyCarEntity(0x6535994a, 0.6998f, 2.7318f, 0.0482f, -1.5958f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x6535994a, -0.233f, 1.832f, 0.1405f, -1.535f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x6535994a, 0.3843f, 1.8292f, 0.1482f, -1.533f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x3c809fe8, -0.294f, 2.2137f, -0.0597f, 3.1435f, 0.0f, 1.5925f, 12);
	EBCreateFunnyCarEntity(0x3c809fe8, 0.4723f, 2.2055f, -0.0705f, 3.1533f, 0.0f, 1.5722f, 13);
	EBCreateFunnyCarEntity(0xe8577885, -0.045f, -1.9473f, 0.1502f, 0.1922f, 1.644f, 1.775f, 14);
	EBCreateFunnyCarEntity(0xe8577885, 0.1895f, -1.9248f, 0.113f, -0.0553f, -1.6325f, -1.4832f, 15);
	EBCreateFunnyCarEntity(0xe8577885, 0.0565f, 0.0f, 0.07f, 1.8452f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0xe8577885, 0.0633f, -0.3383f, -0.311f, -0.784f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0xe8577885, 0.0353f, 1.8052f, -0.089f, -1.0665f, 0.0f, 3.1575f, 18);
	EBCreateFunnyCarEntity(0xe8577885, 0.081f, -0.116f, -0.058f, 1.801f, 0.0f, -3.1333f, 19);
	EBCreateFunnyCarEntity(0xf2cff3e4, 0.1175f, -0.8338f, 0.138f, 0.0f, 0.0f, 1.4392f, 20);
	EBCreateFunnyCarEntity(0x881d4acb, -0.131f, 1.1795f, -0.2517f, 0.8967f, 6.406f, -0.336f, 21);
	EBCreateFunnyCarEntity(0x881d4acb, 0.2872f, 1.2035f, -0.1685f, 2.1885f, 2.751f, 0.3315f, 22);
	EBCreateFunnyCarEntity(0xedc6be0f, 0.0713f, 0.8067f, 0.091f, 0.0352f, 0.537f, 1.6327f, 23);
	EBCreateFunnyCarEntity(0x87d03d2f, -0.2515f, -0.1427f, -0.0427f, -1.6998f, 0.0f, 0.0f, 24);
	EBCreateFunnyCarEntity(0x87d03d2f, 0.3302f, -0.1412f, 0.0503f, -1.7435f, 0.0f, 0.0f, 25);
	EBCreateFunnyCarEntity(0x89f88169, -0.2488f, 0.1528f, -0.1868f, 4.7218f, 0.0f, 0.984f, 26);
	EBCreateFunnyCarEntity(0x89f88169, 0.3587f, 0.1125f, -0.1795f, 1.5605f, 0.0f, 2.095f, 27);
	printf("~r~CTL: ~w~Bat Bike by ~y~cerberusxxxx ~w~(28 Objects)");
}

void EBFunnyBikePogoStick()
{
	SpawnFunnyVehicleModel(0x9229e4eb);
	EBCreateFunnyCarEntity(0x87d03d2f, 0.0f, 0.3749f, -0.7565f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xed04c49, -0.0651f, 0.3699f, -0.2739f, 1.5841f, 0.0f, 1.632f, 1);
	EBCreateFunnyCarEntity(0xed04c49, 0.1169f, 0.3763f, -0.2753f, 1.5773f, 0.0f, 1.5902f, 2);
	EBCreateFunnyCarEntity(0xd2349113, -0.2216f, 0.3621f, 0.4595f, 1.5649f, 0.0f, 1.644f, 3);
	EBCreateFunnyCarEntity(0xd2349113, 0.2383f, 0.3941f, 0.4617f, 0.0f, -1.5734f, 0.0651f, 4);
	EBCreateFunnyCarEntity(0x1db93ab9, 0.0f, 0.3704f, 0.5734f, 3.1479f, 0.0f, 0.0f, 5);
	printf("~r~CTL: ~w~Pogo Stick by ~y~cerberusxxxx ~w~(6 Objects)");
}

void EBFunnyBikeUnicycle()
{
	SpawnFunnyVehicleModel(0x9229e4eb);
	EBCreateFunnyCarEntity(0xd2349113, 0.0f, -0.1803f, -0.3152f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0x1b11b700, 0.1455f, -0.1832f, -0.188f, 0.0f, 0.0f, -1.5383f, 1);
	EBCreateFunnyCarEntity(0x1b11b700, -0.14f, -0.1904f, -0.1917f, 0.0f, 0.0f, 1.612f, 2);
	EBCreateFunnyCarEntity(0xed04c49, -0.0761f, -0.2078f, -0.2085f, 1.5698f, 0.0f, 1.6383f, 3);
	EBCreateFunnyCarEntity(0xed04c49, 0.1133f, -0.1839f, -0.2143f, 1.6067f, 0.0f, 1.6413f, 4);
	EBCreateFunnyCarEntity(0xb7a13798, 0.0f, -0.1132f, 0.2809f, 1.5477f, 0.0f, 0.0f, 5);
	printf("~r~CTL: ~w~Unicycle by ~y~cerberusxxxx ~w~(6 Objects)");
}

void EBFunnyBikeGiroBike() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x11f76c14);
	EBCreateFunnyCarEntity(0xb3615bc4, 0.0f, -0.6882f, -0.1663f, -1.1585f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xaee195ab, 0.0f, 0.0f, 7.337f, 0.0f, 1.574f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xaee195ab, 0.394f, 0.0f, 7.3845f, 0.0f, -1.574f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xaee195ab, -0.383f, 0.0f, 7.3317f, 0.0f, 1.5728f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x55ca3ae6, 0.0f, 1.1887f, 0.6105f, 1.9945f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, 0.7975f, 0.4477f, -1.1193f, 0.0f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x63401612, 0.806f, 3.669f, 0.1712f, 1.5892f, 0.0f, 1.5665f, 6);
	EBCreateFunnyCarEntity(0x63401612, -0.863f, 3.6803f, 0.1813f, 4.7123f, 0.0f, 1.5625f, 7);
	EBCreateFunnyCarEntity(0xed723bb4, -1.0035f, 2.7098f, 0.1755f, 1.5485f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0xed723bb4, 0.9585f, 2.716f, 0.1763f, 1.5747f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0xbd4cf67d, 0.0f, -6.004f, 1.7697f, 2.062f, 0.0f, 3.1473f, 10);
	EBCreateFunnyCarEntity(0x67105bd6, 0.0f, -6.8685f, 1.6755f, 1.322f, 0.0f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x2a0710f4, 0.211f, -0.758f, -0.13f, 0.0f, 1.59f, 0.0f, 12);
	EBCreateFunnyCarEntity(0x2a0710f4, -0.2375f, -0.748f, -0.1195f, 0.0f, -1.564f, 0.0f, 13);
	EBCreateFunnyCarEntity(0x88c9c86e, 0.0f, -0.7888f, -0.2785f, 0.0f, 0.0f, 0.0f, 14);
	EBCreateFunnyCarEntity(0x6cb90840, 0.0f, -4.546f, 1.1422f, 0.1135f, 0.0f, 0.0f, 15);
	EBCreateFunnyCarEntity(0x6cb90840, 0.0f, -4.9455f, 4.7498f, -0.3673f, 0.0f, 0.0f, 16);
	EBCreateFunnyCarEntity(0x6cb90840, 0.0f, -3.6422f, 7.9858f, -1.9828f, 0.0f, 0.0f, 17);
	EBCreateFunnyCarEntity(0x6cb90840, 0.0f, -0.39f, 6.597f, 3.547f, 0.0f, 0.0f, 18);
	EBCreateFunnyCarEntity(0x6cb90840, 0.0f, 0.6532f, 4.2393f, 2.9178f, 0.0f, 0.0f, 19);
	printf("~r~CTL: ~w~Giro Bike by ~y~cerberusxxxx ~w~(20 Objects)");
}

void EBFunnyBikeButtPlugBMX(bool withTrailer)
{
	SpawnFunnyVehicleModel(0x2ef89e46);
	EBCreateFunnyCarEntity(0x1b11b700, -0.1623f, -1.0135f, -0.1675f, 0.0f, 0.0f, 1.6382f, 0);
	EBCreateFunnyCarEntity(0x1b11b700, 0.0933f, -1.0023f, -0.1758f, 0.0f, -0.038f, -1.5293f, 1);
	EBCreateFunnyCarEntity(0x1b11b700, -0.231f, 0.4045f, -0.1948f, 0.0f, 0.0f, 1.591f, 2);
	EBCreateFunnyCarEntity(0x1b11b700, 0.0318f, 0.404f, -0.196f, 0.0f, 0.0f, 4.747f, 3);
	EBCreateFunnyCarEntity(0xf086a873, 0.0f, -0.4438f, 0.1565f, 0.0f, 0.5532f, -1.5165f, 4);
	EBCreateFunnyCarEntity(0xf086a873, -0.1065f, -0.4593f, 0.1458f, 0.0f, 0.5895f, -1.528f, 5);
	EBCreateFunnyCarEntity(0xf086a873, -0.0435f, 0.3425f, 0.3598f, 0.0f, 1.4942f, 1.5425f, 6);
	EBCreateFunnyCarEntity(0xf086a873, -0.1603f, 0.3135f, 0.3655f, 0.0f, 1.482f, 1.607f, 7);
	EBCreateFunnyCarEntity(0xf086a873, -0.0178f, 0.09f, 0.3208f, 0.0f, 0.344f, 4.7633f, 8);
	EBCreateFunnyCarEntity(0xf086a873, -0.1593f, 0.0852f, 0.311f, 0.0f, 0.363f, 4.7235f, 9);
	EBCreateFunnyCarEntity(0xf086a873, -0.0635f, -0.3197f, -0.0337f, 0.0f, 0.2323f, 4.7668f, 10);
	EBCreateFunnyCarEntity(0x3c809fe8, -0.0768f, 0.0902f, 0.1185f, 0.0f, 0.8177f, -1.5195f, 11);
	EBCreateFunnyCarEntity(0x3c809fe8, -0.3843f, 0.2658f, 0.6143f, 0.0f, 0.0f, 3.3025f, 12);
	EBCreateFunnyCarEntity(0x3c809fe8, 0.1713f, 0.327f, 0.6213f, 0.0f, 0.0f, 0.0443f, 13);
	EBCreateFunnyCarEntity(0x3c809fe8, -0.06f, -0.331f, 0.0997f, 0.0f, 1.4315f, 1.601f, 14);
	EBCreateFunnyCarEntity(0xb37b15a0, 0.1703f, 0.0172f, -0.1475f, 1.5913f, -0.048f, -1.426f, 15);
	EBCreateFunnyCarEntity(0xb37b15a0, -0.2488f, -0.1633f, -0.121f, 1.5633f, -6.3262f, 1.8735f, 16);
	EBCreateFunnyCarEntity(0x5c58553c, -0.1018f, 0.4332f, 0.509f, -0.169f, -0.016f, 3.2323f, 17);
	EBCreateFunnyCarEntity(0x5c58553c, -0.043f, -0.3743f, 0.5012f, -1.884f, -0.0388f, 0.033f, 18);
	EBCreateFunnyCarEntity(0xa2c093c7, 0.0f, -0.1035f, -0.092f, -0.3975f, -0.5172f, -0.811f, 19);
	EBCreateFunnyCarEntity(0xa2c093c7, -0.1822f, -0.1388f, -0.0823f, -0.3825f, 0.0123f, 1.8375f, 20);

	if (withTrailer)
	{
		EBCreateFunnyCarEntity(0xbac345f6, 0.0f, -2.5005f, -0.2248f, 0.0f, 0.0f, 0.0f, 21);
		EBCreateFunnyCarEntity(0xac2f3abc, -0.0965f, -2.168f, 0.9085f, 0.0f, 0.0f, 1.5887f, 22);
		EBCreateFunnyCarEntity(0x38412daa, 0.4485f, -2.595f, 0.2398f, 1.6132f, -0.1137f, -0.594f, 23);
		EBCreateFunnyCarEntity(0x18c5968, 0.0f, -2.6063f, 0.0175f, 0.0f, 1.4055f, 1.5735f, 24);
		printf("~r~CTL: ~w~Butt Plug BMX with Trailer by ~y~cerberusxxxx ~w~(25 Objects)");
	}
	else
		printf("~r~CTL: ~w~Butt Plug BMX by ~y~cerberusxxxx ~w~(21 Objects)");
}

void EBFunnyBikeBatPod()
{
	SpawnFunnyVehicleModel(0xc9ceaf06);
	SET_CAR_VISIBLE(myObjectCar, true);

	for (int i = 1; i < 10; i++) //1 through 9
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, (i == 6));

	EBCreateFunnyCarEntity(0xd57ceb37, 0.0f, -0.75f, -0.2f, 0.0f, 0.0f, -1.57f, 0);
	EBCreateFunnyCarEntity(0xd57ceb37, 0.0f, 0.75f, -0.2f, 0.0f, 0.0f, -1.57f, 1);
	EBCreateFunnyCarEntity(0xaf6834a4, 0.2f, 0.73f, -0.19f, 0.0f, 1.57f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xaf6834a4, -0.2f, 0.73f, -0.19f, 0.0f, -1.57f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x579384a4, 0.25f, 0.85f, -0.22f, 0.0f, 0.0f, -1.57f, 4);
	EBCreateFunnyCarEntity(0x579384a4, -0.25f, 0.85f, -0.22f, 0.0f, 0.0f, -1.57f, 5);

	printf("~r~CTL: ~w~Batpod by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (6 Objects)");
}

void EBFunnyBikeHotdogCart() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x92e56a2c);
	SET_CAR_VISIBLE(myObjectCar, true);

	for (int i = 1; i < 5; i++)
		TURN_OFF_VEHICLE_EXTRA(myObjectCar, i, false);

	EBCreateFunnyCarEntity(0x43ea7253, 0.15f, -1.8f, -0.1f, 0.0f, 0.0f, -1.57f, 0);
	printf("~r~CTL: ~w~Hotdog Cart by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (1 Object)");
}

void EBFunnyBikeHovercraft()
{
	SpawnFunnyVehicleModel(0x2ef89e46);
	EBCreateFunnyCarEntity(0xdb0061b6, 0.0f, -0.25f, 0.5f, 0.0f, 0.0f, 1.57f, 0);
	EBCreateFunnyCarEntity(0xcb26803d, 0.0f, 0.25f, 0.15f, 0.0f, -1.57f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xcb26803d, 0.0f, -0.6f, 0.15f, 0.0f, -1.57f, 0.0f, 2);
	printf("~r~CTL: ~w~Hovercraft by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (3 Objects)");
}

void EBFunnyBikeParachuteBike() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x4b6c568a);
	SET_CAR_VISIBLE(myObjectCar, true);
	EBCreateFunnyCarEntity(0x58d6a0a0, 0.0f, -3.5f, 4.75f, 0.5f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Parachute Bike by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (1 Object)");
}

void EBFunnyBikeTronBike()
{
	SpawnFunnyVehicleModel(0x2ef89e46);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, 0.45f, 0.45f, -2.57f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, -0.5f, 0.45f, 2.57f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, -0.03f, 0.45f, 1.57f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, -0.04f, 0.45f, -1.57f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, 0.61f, 0.1f, 0.35f, 0.55f, 0.0f, 4);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, 0.61f, 0.1f, 0.35f, -0.55f, 0.0f, 5);
	EBCreateFunnyCarEntity(0xd20167be, 0.0f, -0.04f, 0.48f, 3.14f, 0.0f, 0.0f, 6);
	printf("~r~CTL: ~w~Tron Bike by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (7 Objects)");
}

void EBFunnyBikeWheelBarrel()
{
	SpawnFunnyVehicleModel(0x9229e4eb);
	EBCreateFunnyCarEntity(0x7cd44988, 0.0f, 1.04f, -0.05f, 0.0f, 0.3f, 1.57f, 0);
	EBCreateFunnyCarEntity(0x38412daa, 0.0f, 1.6f, -0.45f, 0.44f, -0.21f, 0.0f, 1);
	printf("~r~CTL: ~w~Wheelbarrel Bike by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (2 Objects)");
}

void EBFunnyBikePallateBike() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x4b6c568a);
	EBCreateFunnyCarEntity(0x35ac3bc2, 0.0f, -0.4f, -0.45f, 0.0f, 0.0f, 0.0f, 0);
	EBCreateFunnyCarEntity(0xda87e2aa, 0.0f, -0.57f, -0.34f, 0.0f, 0.0f, 1.57f, 1);
	EBCreateFunnyCarEntity(0x5985a1d2, 0.0f, -1.0f, 0.03f, -1.58f, 0.0f, 0.0f, 2);
	printf("~r~CTL: ~w~Pallate Bike by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (3 Objects)");
}

void EBFunnyBikeBMX() //TBoGT Only
{
	SpawnFunnyVehicleModel(0x2ef89e46);
	EBCreateFunnyCarEntity(0x1f3154ce, -0.02f, 0.02f, 0.1222f, 0.0f, -0.0935f, 1.5175f, 0);
	EBCreateFunnyCarEntity(0x1b11b700, 0.0702f, -0.5585f, -0.1458f, 0.0f, 0.0f, -1.656f, 1);
	EBCreateFunnyCarEntity(0x1b11b700, -0.1963f, -0.5325f, -0.1578f, 0.0f, 0.0f, 1.4533f, 2);
	EBCreateFunnyCarEntity(0x1b11b700, -0.123f, 0.6215f, -0.2113f, 0.0f, 0.0f, 1.5497f, 3);
	EBCreateFunnyCarEntity(0x1b11b700, 0.144f, 0.6135f, -0.2105f, 0.0f, 0.0f, -1.5812f, 4);
	printf("~r~CTL: ~w~BMX by ~y~ZzLoik69zZ~w~ & ~y~Ti jo P1CK UP~w~ (5 Objects)");
}

void EBFunnyBikePortaloo()
{
	SpawnFunnyVehicleModel(0x9229e4eb);
	EBCreateFunnyCarEntity(0x2d6a9cc9, 0.0f, 0.0f, -0.3f, 0.0f, 0.0f, 160.2f, 0);
	printf("~r~CTL: ~w~Portaloo Bike by ~y~Evil Blunt ~w~(1 Object)");
}

void EBFunnyBikeTRex()
{
	SpawnFunnyVehicleModel(0x47b9138a);
	EBCreateFunnyCarEntity(0x4ded717b, -0.0574f, 0.0f, -1.35f, 0.0f, 0.0f, 1.7288f, 0);
	printf("~r~CTL: ~w~T-Rex Bike by ~y~Evil Blunt ~w~(1 Object)");
}

void EBFunnyBikeToilet()
{
	SpawnFunnyVehicleModel(0x9229e4eb);
	EBCreateFunnyCarEntity(0x4140a534, 0.0f, -0.3f, -0.27f, 0.0f, 0.0f, 3.3f, 0);
	printf("~r~CTL: ~w~Toilet Bike 1 by ~y~Evil Blunt ~w~(1 Object)");
}

void EBFunnyBikeToiletTwo()
{
	SpawnFunnyVehicleModel(0x9229e4eb);
	EBCreateFunnyCarEntity(0xe8577885, 0.0f, -0.35f, -0.22f, 0.0f, 0.0f, 3.14f, 0);
	EBCreateFunnyCarEntity(0x30c9210b, 0.0f, 0.52f, 0.61f, 0.95f, 0.0f, 0.0f, 1);
	printf("~r~CTL: ~w~Toilet Bike 2 by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (2 Objects)");
}

void EBFunnyBikeShoppingCart()
{
	SpawnFunnyVehicleModel(0x9229e4eb);
	EBCreateFunnyCarEntity(0x11d07b58, -0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 3.268f, 0);
	printf("~r~CTL: ~w~Shopping Cart Bike by ~y~Evil Blunt ~w~(1 Object)");
}

void EBFunnyBikeShoppingCartVTwo()
{
	SpawnFunnyVehicleModel(0x9229e4eb);
	EBCreateFunnyCarEntity(0x739141b5, 0.15f, 1.4f, -0.45f, 0.0f, 0.0f, -3.14f, 0);
	EBCreateFunnyCarEntity(0xf40be44b, 0.09f, 0.94f, 0.5f, 0.0f, -1.9f, 0.0f, 1);
	printf("~r~CTL: ~w~Shopping Cart Bike 2 by ~y~OhManMyBad~w~ & ~y~InTheSevens~w~ (2 Objects)");
}

void EBFunnyBikeSkiBike()
{
	SpawnFunnyVehicleModel(0x2ef89e46);
	EBCreateFunnyCarEntity(0xec85cf10, 0.2582f, 0.1007f, -0.1648f, 0.0f, 0.2725f, -1.5338f, 0);
	EBCreateFunnyCarEntity(0xec85cf10, -0.2958f, 0.0853f, -0.1782f, 0.0f, 0.27f, -1.568f, 1);
	EBCreateFunnyCarEntity(0xbc0805e7, 0.41f, -0.7037f, 0.4498f, 0.0f, -1.805f, 1.575f, 2);
	EBCreateFunnyCarEntity(0xbc0805e7, -0.4598f, -0.714f, 0.3947f, 0.0f, 1.8337f, -1.6307f, 3);
	printf("~r~CTL: ~w~Ski Bike by ~y~Ti jo P1CK UP~w~ (4 Objects)");
}
//end misc bikes

//start misc boats
void EBFunnyBoatBus()
{
	SpawnFunnyVehicleModel(0x17df5ec2);
	EBCreateFunnyCarEntity(0xd577c962, 0.360023f, -4.80018f, 0.0399813f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Bus Boat by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyBoatCableCar()
{
	SpawnFunnyVehicleModel(0x17df5ec2);
	EBCreateFunnyCarEntity(0xc6c3242d, 0.0800114f, -2.04007f, 8.06032f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Cable Car Boat by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyBoatCargoShip()
{
	SpawnFunnyVehicleModel(0x3f724e66);
	EBCreateFunnyCarEntity(0x2b9ffc65, -0.2f, -0.3f, 2.95001f, 0.0f, 0.0f, -1.656f, 0);
	printf("~r~CTL: ~w~Cargo Ship by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyBoatRocket()
{
	SpawnFunnyVehicleModel(0x3d961290);
	EBCreateFunnyCarEntity(0xdb0061b6, -0.300004f, -0.3f, 0.800012f, 0.0f, 0.0f, 1.54801f, 0);
	printf("~r~CTL: ~w~Rocket Boat by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyBoatSeaJet(uint version)
{
	uint count = 17;

	SpawnFunnyVehicleModel(0x3d961290);
	EBCreateFunnyCarEntity(0xdb0061b6, -0.3028f, -0.8112f, 0.6118f, 0.0f, 0.0f, 1.552f, 0);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.8107f, -0.0652f, 0.0448f, 0.0f, 0.0f, 0.0f, 1);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.786f, -0.0652f, 0.0448f, 0.0f, 0.0f, 0.0f, 2);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.2177f, -0.0588f, 0.0845f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.253f, -0.053f, 0.0853f, 0.0f, 0.0f, 0.0f, 4);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.4597f, -0.272f, 0.2897f, -0.5948f, 0.9125f, 0.0f, 5);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.0005f, -0.2593f, 0.2583f, -0.565f, 3.978f, 3.1323f, 6);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.6283f, -1.5735f, 0.094f, 0.0f, 0.0f, 0.0f, 7);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.5948f, -1.5722f, 0.094f, 0.0f, 0.0f, 0.0f, 8);
	EBCreateFunnyCarEntity(0x7f53dd7e, -1.2148f, -1.6038f, 0.1145f, 0.0f, 0.0f, 0.0f, 9);
	EBCreateFunnyCarEntity(0x7f53dd7e, -1.1673f, -1.6095f, 0.1145f, 0.0f, 0.0f, 0.0f, 10);
	EBCreateFunnyCarEntity(0x7f53dd7e, -0.789f, -1.1305f, 0.2735f, 0.5878f, 1.0668f, 0.0f, 11);
	EBCreateFunnyCarEntity(0x7f53dd7e, 0.141f, -1.0905f, 0.3027f, 0.714f, -1.007f, 0.0f, 12);
	EBCreateFunnyCarEntity(0x41303af8, -0.3135f, -1.063f, 0.5517f, 0.0f, 0.0f, 0.0f, 13);
	EBCreateFunnyCarEntity(0xd0afce9b, -0.4643f, -2.2145f, 0.2895f, 0.035f, -0.0003f, 0.2575f, 14);
	EBCreateFunnyCarEntity(0x1ae7947b, -0.7905f, -1.7702f, 1.0f, 1.5415f, 0.0f, 0.0f, 15);
	EBCreateFunnyCarEntity(0x1ae7947b, 0.2315f, -1.7813f, 1.0f, 1.5483f, 0.0f, 0.0f, 16);

	if (version == 2)
	{
		EBCreateFunnyCarEntity(0xf814ad1e, -0.359f, -2.0855f, 0.45f, 0.22f, -4.7155f, -1.3463f, 17);
		EBCreateFunnyCarEntity(0xf814ad1e, -0.3438f, -5.0008f, 0.4585f, 0.0f, -1.5387f, 1.5387f, 18);
		EBCreateFunnyCarEntity(0xac2f3abc, -0.2983f, -8.0802f, 0.614f, 0.0f, 0.0f, -1.6245f, 19);
		EBCreateFunnyCarEntity(0x7f53dd7e, -0.648f, -7.9565f, 0.3272f, 0.0f, 0.0f, 0.0948f, 20);
		EBCreateFunnyCarEntity(0x7f53dd7e, -0.6125f, -7.9776f, 0.3272f, 0.0f, 0.0f, 0.0938f, 21);
		EBCreateFunnyCarEntity(0x7f53dd7e, -0.107f, -7.9603f, 0.3328f, 0.0f, 0.0f, -0.0677f, 22);
		EBCreateFunnyCarEntity(0x7f53dd7e, -0.0632f, -7.9535f, 0.3328f, 0.0f, 0.0f, -0.069f, 23);
		count = 24;
	}

	printf("~r~CTL: ~w~Sea Jet %i by ~y~R3B3LY0N~w~ (%i Objects)", version, count);
}

void EBFunnyBoatVinewood()
{
	SpawnFunnyVehicleModel(0x3d961290);
	EBCreateFunnyCarEntity(0x999adde8, 0.0f, 0.12f, 0.4103f, 0.0f, 0.0f, -3.15f, 0);
	EBCreateFunnyCarEntity(0x28da4f7f, -0.9697f, 0.0f, 7.02f, 0.0f, 0.0f, 0.938f, 1);
	EBCreateFunnyCarEntity(0x28da4f7f, 0.2385f, -0.8338f, 7.025f, 0.0f, 0.0f, -2.1957f, 2);
	EBCreateFunnyCarEntity(0x83a0c70e, 1.3395f, -0.43f, 0.5507f, 0.0f, 0.0f, 0.0f, 3);
	EBCreateFunnyCarEntity(0x83a0c70e, -1.3782f, -0.4198f, 0.5883f, 0.0f, 0.0f, -3.162f, 4);
	EBCreateFunnyCarEntity(0xedf775cb, -1.802f, -0.7767f, 1.0643f, 0.0f, -0.8837f, 0.0f, 5);

	for (int i = 0; i < 6; i++)
		SET_OBJECT_COLLISION(freeMem->m_object_car_entities[i], true);

	printf("~r~CTL: ~w~Vinewood Boat by ~y~R3B3LY0N~w~ (6 Objects)");
}

void EBFunnyBoatHexer() //TBoGT only
{
	SpawnFunnyVehicleModel(0x3d961290);
	EBCreateFunnyCarEntity(0x11f76c14, -0.320005f, -0.219997f, 0.520001f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL: ~w~Hexer by ~y~Anon Reporter ~w~(1 Object)");
}

void EBFunnyBoatYacht()
{
	SpawnFunnyVehicleModel(0x3f724e66);
	EBCreateFunnyCarEntity(0xbbf41314, 0.0f, -5.0f, 2.5f, 0.0f, 0.0f, 0.0f, 0);
	printf("~r~CTL:~w~ Yacht by ~y~OhManMyBed ~w~& ~y~InTheSevens ~w~(1 Object)");
}
//end misc noats

//MD v6 
void MDFunnyHeliModZilla()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	EBCreateFunnyCarEntity(0x680EF92B, 0.3173, -12.7528, 2.1650, 0.0, 0.0, 1.5900, 0);
	EBCreateFunnyCarEntity(0x680EF92B, 0.0390, -11.9733, -4.2900, 3.1899, 5.9200, 1.5900, 1);
	EBCreateFunnyCarEntity(0x71AC0D7C, 0.3160, -58.3771, 4.5625, 0.0, 0.0, 0.0, 2);
	EBCreateFunnyCarEntity(0x71AC0D7C, 0.3160, -58.3771, 4.5625, 0.0, 1.5900, 0.0, 3);
	EBCreateFunnyCarEntity(0x71AC0D7C, 0.3160, -58.3771, 4.5625, 0.0, 3.1500, 0.0, 4);
	EBCreateFunnyCarEntity(0x71AC0D7C, 0.3160, -58.3771, 4.5625, 0.0, 4.7500, 0.0, 5);
	EBCreateFunnyCarEntity(0x1553C997, -5.3625, -7.3820, 11.1875, 0.0, 5.7500, 4.7500, 6);
	EBCreateFunnyCarEntity(0x1553C997, 5.7035, -7.3820, 11.1875, 0.0, 5.7500, 4.7500, 7);
	EBCreateFunnyCarEntity(0xBC49368B, -5.3625, -4.4191, 11.0325, 0.0, 0.0, 0.0, 8);
	EBCreateFunnyCarEntity(0xBC49368B, -5.3625, -4.4191, 11.0325, 0.0, 0.0, 0.0, 9);
	EBCreateFunnyCarEntity(0xBC49368B, -5.3625, -4.4191, 11.0325, 0.0, 0.0, 0.0, 10);
	EBCreateFunnyCarEntity(0xBC49368B, 5.7035, -4.4191, 11.0325, 0.0, 0.0, 0.0, 11);
	EBCreateFunnyCarEntity(0xBC49368B, 5.7035, -4.4191, 11.0325, 0.0, 0.0, 0.0, 12);
	EBCreateFunnyCarEntity(0xBC49368B, 5.7035, -4.4191, 11.0325, 0.0, 0.0, 0.0, 13);
	EBCreateFunnyCarEntity(0xBC49368B, 5.7035, -4.4191, 11.0325, 0.0, 0.0, 0.0, 14);
	EBCreateFunnyCarEntity(0x1AE7947B, -5.3625, -6.0000, 11.1500, 4.7400, 0.0, 0.0, 15);
	EBCreateFunnyCarEntity(0x1AE7947B, -5.3625, -6.0000, 11.1500, 4.7400, 0.0, 0.0, 16);
	EBCreateFunnyCarEntity(0x1AE7947B, -5.3625, -6.0000, 11.1500, 4.7400, 0.0, 0.0, 17);
	EBCreateFunnyCarEntity(0x1AE7947B, 5.7035, -6.0000, 11.1500, 4.7400, 0.0, 0.0, 18);
	EBCreateFunnyCarEntity(0x1AE7947B, 5.7035, -6.0000, 11.1500, 4.7400, 0.0, 0.0, 19);
	EBCreateFunnyCarEntity(0x1AE7947B, 5.7035, -6.0000, 11.1500, 4.7400, 0.0, 0.0, 20);
	EBCreateFunnyCarEntity(0xDFD7EAE8, 5.5949, -10.6026, 0.0292, 6.2700, 4.0299, 3.1000, 21);
	EBCreateFunnyCarEntity(0xDFD7EAE8, -5.7840, -10.4907, -0.9083, 6.2700, 2.3000, 3.1000, 22);
	EBCreateFunnyCarEntity(0x466677F7, -1.0390, -46.9374, 8.5842, 0.0, 5.6500, 0.0, 23);
	EBCreateFunnyCarEntity(0x466677F7, -1.0390, -46.9374, 8.5842, 0.0, 0.6500, 0.0, 24);
	EBCreateFunnyCarEntity(0x3427A4F4, -1.9135, -93.7370, 6.5808, 1.6000, 0.0, 0.0, 25);
	EBCreateFunnyCarEntity(0x3427A4F4, -1.9135, -93.7370, 6.5808, 1.6000, 0.0, 0.0, 26);
	EBCreateFunnyCarEntity(0x3427A4F4, 1.9135, -93.7370, 3.000, 1.6000, 0.0, 0.0, 27);
	EBCreateFunnyCarEntity(0x3427A4F4, 1.9135, -93.7370, 3.000, 1.6000, 0.0, 0.0, 28);
	EBCreateFunnyCarEntity(0x4AA899BF, 0.5131, -1.9251, 2.0683, 0.4300, 6.0800, 0.0400, 29);
	EBCreateFunnyCarEntity(0x4AA899BF, 0.4539, -0.9706, -0.4908, 0.0, 0.0, 0.0, 30);
	EBCreateFunnyCarEntity(0xFE740BE2, 4.2529, -5.4129, 12.8700, 3.1400, -0.7875, 6.3000, 31);
	EBCreateFunnyCarEntity(0xFE740BE2, -4.2529, -5.4129, 12.8700, 3.1400, 0.7875, 0, 32);
	EBCreateFunnyCarEntity(0x3427A4F4, -0.1298, -11.8322, 0.3583, 4.7500, 6.2800, 6.0300, 33);
	printf("~r~CTL:~w~ Modzilla by ~y~PHANTOMISED_ ~w~(34 Objects)");
}

void MDFunnyHeliNightFighter()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0xD20167BE, -0.0138, -7.7076, 0.2225, 0.0749, 0.0, 1.0, 0);
	EBCreateFunnyCarEntity(0xD20167BE, -0.0007, -6.5695, 0.3921, 0.0749, 0.0, 1.0, 1);
	EBCreateFunnyCarEntity(0xD20167BE, 0.0070, -5.3932, 0.5808, 0.0998, 0.0, 1.0, 2);
	EBCreateFunnyCarEntity(0xD20167BE, -0.0094, -3.6292, 0.9550, 0.1494, 0.0, 1.0, 3);
	EBCreateFunnyCarEntity(0xD20167BE, -0.0070, -2.1478, 1.4300, 0.1247, 0.0, 1.0, 4);
	EBCreateFunnyCarEntity(0xD20167BE, -0.0056, -0.8053, 1.5879, 0.0, 0.0, 1.0, 5);
	EBCreateFunnyCarEntity(0xD20167BE, -2.5685, 1.0923, 0.4663, 0.0, -0.0, 1.0, 6);
	EBCreateFunnyCarEntity(0xD20167BE, -2.0973, 1.1429, 0.5883, 0.0, 0.0, 1.0, 7);
	EBCreateFunnyCarEntity(0xD20167BE, 2.5591, 1.1094, 0.4729, 0.0, 0.0, 1.0, 8);
	EBCreateFunnyCarEntity(0xD20167BE, 2.0883, 1.0909, 0.5746, 0.0, 0.0, 1.0, 9);
	// Red Neon
	EBCreateFunnyCarEntity(0xCB26803D, 0.0231, -9.5697, 0.2525, 0.0, 0.0, 1.7457, 10);
	printf("~r~CTL: ~w~Nightfighter by ~y~AddictiveGamer97 ~w~(11 Objects)");
}

void MDFunnyHeliNoob()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0xA0299E40, -4.7214, 17.8787, 0.6542, 0.0, 1.6000, 0.0, 0);
	EBCreateFunnyCarEntity(0xA0299E40, 4.7214, 17.8787, 0.6542, 0.0, -1.6000, 0.0, 1);

	// Red Neons
	EBCreateFunnyCarEntity(0xCB26803D, -0.2000, 18.2000, 0.6425, 0.0, 0.0, 1.6000, 2);
	EBCreateFunnyCarEntity(0xCB26803D, -0.2000, 18.3585, 0.6425, 0.0, 0.0, 4.7000, 3);
	printf("~r~CTL: ~w~Noob Heli by ~y~AddictiveGamer97 ~w~(4 Objects)");
}

void MDFunnyHeliFuckUp() //Blocks turrets with gas canisters
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0xEF72F88A, -2.5569, 2.2650, 0.2292, 4.7000, 0.0, 0.0, 0);
	EBCreateFunnyCarEntity(0xEF72F88A, -2.1145, 2.1952, 0.2292, 4.7000, 0.0, 0.0, 1);
	EBCreateFunnyCarEntity(0xEF72F88A, 2.5569, 2.2650, 0.2292, 4.7000, 0.0, 0.0, 2);
	EBCreateFunnyCarEntity(0xEF72F88A, 2.1145, 2.1952, 0.2292, 4.7000, 0.0, 0.0, 3);
	printf("~r~CTL: ~w~Bullet blocker (Fuckup heli) by ~y~PHANTOMISED_ ~w~(4 Objects)");
}

void MDFunnyCarPianoPlayer()
{
	SpawnFunnyVehicleModel(VEHICLE_MRTASTY);
	SWITCH_CAR_SIREN(myObjectCar, true);

	EBCreateFunnyCarEntity(0x873608FF, -0.2684, 2.0028, -0.3667, 0.0, 0.0, 6.0400, 0);
	EBCreateFunnyCarEntity(0x6FF9FA43, -0.5844, 1.2650, 0.1400, 0.0, 0.0, 0.0, 1);
	EBCreateFunnyCarEntity(0xE5FE778F, -0.5051, 0.7623, 0.0717, 0.0, 0.0, 0.0, 2);
	EBCreateFunnyCarEntity(0x23363D4D, -0.5384, 1.7174, 1.0000, 0.0, 0.0, 4.7050, 3);
	EBCreateFunnyCarEntity(0x3859BAC0, 0.1399, 1.4246, -0.0233, 0.0, 0.0, 0.0, 4);
	EBCreateFunnyCarEntity(0x3859BAC0, -0.1000, 1.4246, -0.0233, 0.0, 0.0, 0.0, 5);
	EBCreateFunnyCarEntity(0x3859BAC0, -0.3901, 1.4246, -0.0233, 0.0, 0.0, 0.0, 6);
	EBCreateFunnyCarEntity(0x3859BAC0, -0.6639, 1.4246, -0.0233, 0.0, 0.0, 0.0, 7);
	EBCreateFunnyCarEntity(0x3859BAC0, -1.0000, 1.4246, -0.0233, 0.0, 0.0, 0.0, 8);
	EBCreateFunnyCarEntity(0x3859BAC0, -1.2923, 1.4246, -0.0233, 0.0, 0.0, 0.0, 9);
	EBCreateFunnyCarEntity(0xF2CFF3E4, -1.3546, 1.4298, 1.6092, 0.0, 0.0, 3.0675, 10);
	EBCreateFunnyCarEntity(0xF2CFF3E4, -1.0000, 1.4298, 1.6092, 0.0, 0.0, 3.0675, 11);
	EBCreateFunnyCarEntity(0xF2CFF3E4, -0.6639, 1.4298, 1.6092, 0.0, 0.0, 3.0675, 12);
	EBCreateFunnyCarEntity(0xF2CFF3E4, -0.3901, 1.4298, 1.6092, 0.0, 0.0, 3.0675, 13);
	EBCreateFunnyCarEntity(0xF2CFF3E4, 0.1781, 1.4298, 1.6092, 0.0, 0.0, 3.0675, 14);
	printf("~r~CTL: ~w~Piano Player by ~y~PHANTOMISED_ ~w~(15 Objects)");
}

void MDFunnyCarCombatSultanRS()
{
	SpawnFunnyVehicleModel(VEHICLE_SULTANRS);
	SET_CAR_VISIBLE(myObjectCar, true);
	TURN_OFF_VEHICLE_EXTRA(myObjectCar, 1, false);

	EBCreateFunnyCarEntity(0xD20167BE, -0.6802, 1.0088, 0.1965, -1.558, 0.0, 0.0, 0);
	EBCreateFunnyCarEntity(0xD20167BE, 0.6855, 0.9825, 0.2138, -1.5782, 0.0, 0.0, 1);
	EBCreateFunnyCarEntity(0x4A669952, -0.2625, 1.0327, 0.0978, 0.0, 1.944, 0.0, 2);
	EBCreateFunnyCarEntity(0x4A669952, 0.2663, 1.0427, -0.127, 0.0, -1.9445, 0.0, 3);
	EBCreateFunnyCarEntity(0x579384A4, -1.8178, 1.0487, 0.5352, 0.0, 0.0, 1.5413, 4);
	EBCreateFunnyCarEntity(0x579384A4, -1.4507, 1.0307, 0.381, 0.0, 0.0, 1.5715, 5);
	EBCreateFunnyCarEntity(0x579384A4, -1.1407, 1.0458, 0.2198, 0.0, 0.0, 1.5825, 6);
	EBCreateFunnyCarEntity(0x579384A4, 1.8415, 1.045, 0.5088, 0.0, 0.0, 1.5715, 7);
	EBCreateFunnyCarEntity(0x579384A4, 1.4903, 1.0385, 0.3570, 0.0, 0.0, 1.581, 8);
	EBCreateFunnyCarEntity(0x579384A4, 1.1868, 1.0397, 0.2247, 0.0, 0.0, 1.5545, 9);
	EBCreateFunnyCarEntity(0x48FA2738, -1.8292, 1.4655, 0.5962, 0.0, 0.0, -1.5865, 10);
	EBCreateFunnyCarEntity(0x48FA2738, -1.465, 1.4425, 0.4462, 0.0, 0.0, -1.6138, 11);
	EBCreateFunnyCarEntity(0x48FA2738, -1.163, 1.4627, 0.2817, 0.0, 0.0, -1.5545, 12);
	EBCreateFunnyCarEntity(0x48FA2738, 1.1742, 1.4482, 0.285, 0.0, 0.0, -1.564, 13);
	EBCreateFunnyCarEntity(0x48FA2738, 1.4683, 1.448, 0.42, 0.0, 0.0, -1.5253, 14);
	EBCreateFunnyCarEntity(0x48FA2738, 1.8307, 1.4448, 0.571, 0.0, 0.0, -1.602, 15);
	EBCreateFunnyCarEntity(0x48FA2738, -0.9255, -1.3565, -0.0547, 0.0, 0.0, 0.0, 16);
	EBCreateFunnyCarEntity(0x48FA2738, 0.9212, -1.3163, -0.0515, 0.0, 0.0, 3.1683, 17);
	EBCreateFunnyCarEntity(0x48FA2738, -0.9045, 1.3315, -0.05, 0.0, 0.0, 0.0, 18);
	EBCreateFunnyCarEntity(0x48FA2738, 0.908, 1.3478, -0.0500, 0.0, 0.0, 3.1735, 19);
	EBCreateFunnyCarEntity(0x3859BAC0, 0.7119, -0.9927, -0.1683, 4.6950, 0.0, 0.0, 20);
	EBCreateFunnyCarEntity(0x3859BAC0, -0.7119, -0.9927, -0.1683, 4.6950, 0.0, 0.0, 21);
	EBCreateFunnyCarEntity(0x2D3AEA68, -0.0016, 1.9066, 0.1883, 1.5000, 0.0, 0.0, 22);
	EBCreateFunnyCarEntity(0x2D3AEA68, 0.0097, -2.1724, 0.3300, 4.7150, 0.0, 0.0, 23);
	EBCreateFunnyCarEntity(0xCB26803D, -0.5361, 1.9632, -0.0692, 5.5300, 6.2800, 5.2500, 24);
	EBCreateFunnyCarEntity(0xCB26803D, 0.5361, 1.9632, -0.0692, 5.5300, 6.2800, 4.2000, 25);
	EBCreateFunnyCarEntity(0xD20167BE, -0.5271, 1.0948, 0.2017, 4.7150, 0.0, 0.0, 26);
	EBCreateFunnyCarEntity(0xD20167BE, 0.5271, 1.0948, 0.2017, 4.7150, 0.0, 0.0, 27);
	printf("~r~CTL: ~w~Combat Sultan by ~y~PHANTOMISED_ ~w~(28 Objects)");
}

void MDFunnyBikeSlutRide()
{
	SpawnFunnyVehicleModel(VEHICLE_PCJ);
	EBCreateFunnyCarEntity(0x38412DAA, 0.3602, -0.6070, 0.2133, 4.6850, 6.1975, 0.6600, 0);
	printf("~r~CTL: ~w~Slut Ride by ~y~PHANTOMISED_ ~w~(1 Object)");
}
//END

//XMC v6
void XMCFunnyTruckMonsterTruck()
{
	SpawnFunnyVehicleModel(VEHICLE_PHANTOM);
	EBCreateFunnyCarEntity(0x5BE14B23, -0.2623, 2.0525, 0.2407, 0, 0, 0, 0);
	EBCreateFunnyCarEntity(0x928B899D, -0.2630, 3.6310, 1.2702, 4.6663, 0, 3.1468, 1);
	EBCreateFunnyCarEntity(0xF40BE44B, 0.7122, 0.8092, -0.5478, 0, 1.5418, 0, 2);
	EBCreateFunnyCarEntity(0xF40BE44B, -1.5710, 0.8092, -0.5478, 0, 1.5418, 0, 3);
	EBCreateFunnyCarEntity(0xF40BE44B, 0.9575, 0.8092, -0.5478, 0, 1.5418, 0, 4);
	EBCreateFunnyCarEntity(0xF40BE44B, -1.8090, 0.8092, -0.5478, 0, 1.5418, 0, 5);
	EBCreateFunnyCarEntity(0xF40BE44B, -1.5710, 3.7305, -0.5478, 0, 1.5418, 0, 6);
	EBCreateFunnyCarEntity(0xF40BE44B, 0.7122, 3.7305, -0.5478, 0, 1.5418, 0, 7);
	EBCreateFunnyCarEntity(0xF40BE44B, -1.8090, 3.7305, -0.5478, 0, 1.5418, 0, 8);
	EBCreateFunnyCarEntity(0xF40BE44B, 0.9575, 3.7305, -0.5478, 0, 1.5418, 0, 9);
	EBCreateFunnyCarEntity(0xF40BE44B, 1.2240, 0.8092, -0.5478, 0, 1.5418, 0, 10);
	EBCreateFunnyCarEntity(0xF40BE44B, -2.0780, 0.8092, -0.5478, 0, 1.5418, 0, 11);
	EBCreateFunnyCarEntity(0xB76637D8, 0.2417, 2.0260, 0.7427, 3.1242, 0, -1.5610, 12);
	EBCreateFunnyCarEntity(0xB76637D8, -0.8500, 2.0260, 0.7427, 3.1242, 0, -1.5610, 13);
	EBCreateFunnyCarEntity(0xB76637D8, -0.2168, 2.1908, 1.7035, 0, 0, 4.6952, 14);
	EBCreateFunnyCarEntity(0x572BBD50, -0.2508, -0.3298, 0.7502, 0, 0, 0, 15);
	EBCreateFunnyCarEntity(0x455B19AF, -0.2635, 4.4328, 0.7648, 0, 0, 3.1288, 16);
	EBCreateFunnyCarEntity(0xF814AD1E, -1.7610, 0.8108, -0.6930, 0, 1.5860, 0, 17);
	EBCreateFunnyCarEntity(0xF814AD1E, -1.7610, 3.7425, -0.6930, 0, 1.5860, 0, 18);
	EBCreateFunnyCarEntity(0x7F53DD7E, -0.2195, 0.8285, -0.2305, 0.7232, 0, 1.5207, 19);
	EBCreateFunnyCarEntity(0x7F53DD7E, -0.2195, 3.7613, -0.2305, 0.7232, 0, 1.5207, 20);
	EBCreateFunnyCarEntity(0x7F53DD7E, -0.3333, 3.7613, -0.2305, 0.7232, 0, 4.7585, 21);
	EBCreateFunnyCarEntity(0x7F53DD7E, -0.3333, 0.8285, -0.2305, 0.7232, 0, 4.7585, 22);
	EBCreateFunnyCarEntity(0x1AE7947B, 0.3250, 0.5665, 0.3285, 1.5662, 0, 0, 23);
	EBCreateFunnyCarEntity(0x1AE7947B, -0.2618, 0.5665, 0.3285, 1.5662, 0, 0, 24);
	EBCreateFunnyCarEntity(0x1AE7947B, -0.8925, 0.5665, 0.3285, 1.5662, 0, 0, 25);
	EBCreateFunnyCarEntity(0x3FF24020, 1.1743, 3.7443, -0.5195, 0.0017, 0, 1.5385, 26);
	EBCreateFunnyCarEntity(0x3FF24020, -1.6692, 3.7443, -0.5305, 0.0393, 0, 4.7470, 27);
	EBCreateFunnyCarEntity(0x3FF24020, 1.4160, 0.8272, -0.5305, 0.0393, 0, 1.5385, 28);
	EBCreateFunnyCarEntity(0x3FF24020, -1.9380, 0.8272, -0.5305, 0.0393, 0, 4.7470, 29);
	EBCreateFunnyCarEntity(0x41303AF8, -0.2735, 4.0410, 1.2805, 0, 0, 0, 30);
	printf("~r~CTL: ~w~Monster Truck v2 from ~y~XMC v6 ~w~(31 Objects)");
}

void XMCFunnyBoatRocket()
{
	SpawnFunnyVehicleModel(VEHICLE_DINGHY);
	EBCreateFunnyCarEntity(0xDB0061B6, -0.3290, -0.2512, 0.7510, 0, 0, 1.5480, 0);
	EBCreateFunnyCarEntity(0xF40BE44B, -0.3387, 0.4967, 0.0752, 0, 0, 0, 1);
	EBCreateFunnyCarEntity(0xF40BE44B, -0.3387, -0.9180, 0.0752, 0, 0, 0, 2);
	EBCreateFunnyCarEntity(0x1AE7947B, -0.3282, -0.8560, 0.7160, 1.5680, 0, 0, 3);
	printf("~r~CTL: ~w~Rocket Boat v2 by ~y~Boubouvirus ~w~(4 Objects)");
}

void XMCFunnyCarWheelBarrow()
{
	SpawnFunnyVehicleModel(VEHICLE_SENTINEL);
	EBCreateFunnyCarEntity(0x7CD44988, -0.40, 0, -0.35, 0, 0, 1.5, 0);
	printf("~r~CTL: ~w~Wheelbarrow from ~y~XMC v6 ~w~(1 Object)");
}

void XMCFunnyCarRustyBackToTheFuture()
{
	SpawnFunnyVehicleModel(VEHICLE_PRIMO);
	EBCreateFunnyCarEntity(0x465F9ABE, -0.0205, -0.1660, 0.2867, 0, 0, 0, 0);
	EBCreateFunnyCarEntity(0x3FF24020, 0.9523, 1.7792, -0.2047, 0, 0, 1.5883, 1);
	EBCreateFunnyCarEntity(0x3FF24020, -0.9535, 1.7792, -0.2047, 0, 0, -1.5883, 2);
	EBCreateFunnyCarEntity(0xD57CEB37, 0.9057, -1.700, -0.1695, 0, 0, 1.5360, 3);
	EBCreateFunnyCarEntity(0xD57CEB37, -0.9830, -1.700, -0.1695, 0, 0, -1.5360, 4);
	EBCreateFunnyCarEntity(0x928B899D, -0.2070, 1.7878, 0.4008, -1.5195, 0.0315, 3.1118, 5);
	EBCreateFunnyCarEntity(0x928B899D, 0.2070, 1.7878, 0.4008, -1.5195, 0, 3.1118, 6);
	EBCreateFunnyCarEntity(0xEAD6E4A4, 0.1788, -2.6883, -0.1218, 0, 0, 0, 7);
	EBCreateFunnyCarEntity(0xEAD6E4A4, -0.2802, -2.6883, -0.1218, 0, 0, 0, 8);
	EBCreateFunnyCarEntity(0xFE740BE2, 0.01195, 2.4590, -0.1002, 0.0032, 0, 3.1188, 9);
	EBCreateFunnyCarEntity(0x1AE7947B, -0.5722, -1.9208, 0.2285, 1.2820, 0, -0.0080, 10);
	EBCreateFunnyCarEntity(0x1AE7947B, -0.5722, -2.1500, -0.0170, 0, 0, 0, 11);
	EBCreateFunnyCarEntity(0x1AE7947B, 0.4717, -1.9208, 0.2285, 1.2820, 0, -0.0080, 12);
	EBCreateFunnyCarEntity(0x1AE7947B, 0.4717, -2.1500, -0.0170, 0, 0, 0, 13);
	EBCreateFunnyCarEntity(0x3859BAC0, -0.5262, 0, 1.1317, 1.5890, 0, -0.0073, 14);
	EBCreateFunnyCarEntity(0x3859BAC0, 0.4925, 0, 1.1317, 1.5890, 0, -0.0073, 15);
	EBCreateFunnyCarEntity(0xB76637D8, -0.4395, 2.1482, -0.3453, 0, -0.1850, 4.7690, 16);
	EBCreateFunnyCarEntity(0xB76637D8, 0.4395, 2.1482, -0.3453, 0, -0.1850, 4.7690, 17);
	EBCreateFunnyCarEntity(0x6A299B19, -0.7207, -3.0395, -0.0290, -0.3763, 0, 0, 18);
	EBCreateFunnyCarEntity(0x6A299B19, 0.5848, -3.0395, -0.0290, -0.3763, 0, 0, 19);
	printf("~r~CTL: ~w~Rusty Back to the Future Car from ~y~XMC v6 ~w~(20 Objects)");
}

void XMCFunnyBikeBigWheeledPCJ()
{
	SpawnFunnyVehicleModel(VEHICLE_PCJ);
	SET_CAR_VISIBLE(myObjectCar, true);

	EBCreateFunnyCarEntity(0xD57CEB37, -0.15, -0.75, -0.15, 0, 0, 4.7, 0);
	EBCreateFunnyCarEntity(0xD57CEB37, 0.15, -0.75, -0.15, 0, 0, 1.6, 1);
	printf("~r~CTL: ~w~Big PCJ by ~y~Boubouvirus ~w~(2 Objects)");
}

void XMCFunnyHeliPrivateJet()
{
	SpawnFunnyVehicleModel(VEHICLE_ANNIHILATOR);
	EBCreateFunnyCarEntity(0x05C1730B, 0, -5.3110, -1.0627, 0, 0, 3.1298, 0);
	printf("~r~CTL: ~w~Private Jet from ~y~XMC v6 ~w~(1 Object)");
}

void FunnyCarByImFoxxyNamedDrunkManAizle()
{
	SpawnFunnyVehicleModel(VEHICLE_COMET);

	CreateObject(0xAFC80ED3, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)& freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, -0.4145f, 0.5967f, -0.6700f, 0.0000f, 0.0000f, 1.6400f);

	CreateObject(0x5ECF5528, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, -0.1239f, -0.4033f, -0.3228f, 0.0000f, 0.0000f, 3.1900f);

	CreateObject(0x006A408A, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, 0.0961f, 0.9768f, 0.2250f, 0.0000f, 0.0000f, 0.0700f);

	CreateObject(0x48F004BD, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, -0.4124f, 0.3489f, 0.2094f, 0.0000f, 0.0000f, 0.0000f);

	CreateObject(0x0B2F0C17, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[4], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[4], myObjectCar, 0, -0.8782f, 0.5231f, 0.2289f, 0.0000f, 0.0000f, 2.1900f);

	CreateObject(0xD2E4333B, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[5], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[5], myObjectCar, 0, -0.4536f, 1.0866f, 0.0972f, 0.0000f, 0.0000f, 1.6400f);

	CreateObject(0x5ECF5528, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[6], true); //Second seat, (make an option for it somehow)
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[6], myObjectCar, 0, 0.6396f, -0.3952f, -0.3528f, 0.0000f, 0.0000f, 3.2600f);

	printf("~r~CTL: ~w~Funny car by ~y~Im Foxxyyy.");
}

void FunnyCarByImFoxxyyyNamedTropical()
{
	SpawnFunnyVehicleModel(VEHICLE_SULTANRS);

	CreateObject(0x1BE53A07, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, -0.2676f, -0.2149f, 2.1472f, 0.0000f, 0.0000f, 0.8100f); //CI_Beach1_Xr_

	CreateObject(0x50C34D47, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, 0.0682f, 0.3889f, -0.4844f, 0.0000f, 0.0000f, 3.1500f); //GB_bed13

	CreateObject(0x9E3E899F, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, -0.2681f, -0.0242f, 4.6478f, 0.0000f, 0.0000f, 1.6100f); //BM_flag_19

	CreateObject(0x9A96D2F0, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, -0.3651f, 0.5395f, 0.5100f, 5.7900f, 6.2500f, 0.0000f); //EC_policescreen_1

	CreateObject(0x3A574B36, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[4], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[4], myObjectCar, 0, -2.3384f, -0.2534f, 0.2494f, 0.0000f, 0.0000f, 0.0000f); //BM_barbeque2

	CreateObject(0x95F65099, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[5], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[5], myObjectCar, 0, -0.1301f, -2.2758f, -0.3700f, 0.0000f, 0.0000f, 0.0000f); //OBJECT_CJ_POT_10

	CreateObject(0x9306A8C6, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[6], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[6], myObjectCar, 0, -0.0027f, 1.2002f, 0.0667f, 1.6700f, 3.2400f, 5.7200f); //OBJECT_CJ_MOBILE_HAND_1 

	CreateObject(0x8C938A03, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[7], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[7], myObjectCar, 0, 0.4994f, 0.2176f, -0.0256f, 6.1900f, 4.6700f, 2.6800f); //CJ_QUESTION_MARK

	CreateObject(0x98952AEB, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[8], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[8], myObjectCar, 0, 2.4706f, -0.2496f, 0.5394f, 0.0000f, 0.0000f, 1.6700f); //OBJECT_CJ_PIGEON_1

	CreateObject(0x4585E2BA, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[9], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[9], myObjectCar, 0, -0.2684f, -0.2146f, 4.3561f, 0.0000f, 0.0000f, 0.0000f); //ws_sublightNoS

	CreateObject(0x98B32B00, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[10], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[10], myObjectCar, 0, 1.9649f, -0.0845f, 0.0311f, 0.0000f, 0.0000f, 0.0000f); //CJ_INT_PLANT_1

	printf("Funny car by ~y~Im Foxxyyy.");
}

void FunnyCarBintRider()
{
	SpawnFunnyVehicleModel(VEHICLE_FAGGIO);

	if(CreateObject(OBJECT_EC_DEADBINT, 0, 0, 0, 0, (Object*)&freeMem->m_object_car_entities[0], true))
		ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, 0.5021f, -0.0252f, 0.2267f, 5.0600f, 5.8700f, 0.7100f);

	printf("Funny car by ~y~Im Foxxyyy.");
}

void FunnyCarRampBus() //needs that Cartel touch, players can't enter the bus currently... store it and if empty, set the bad ramp invisible and non collision or something..
{
	SpawnFunnyVehicleModel(VEHICLE_BUS);
	SET_CAR_VISIBLE(myObjectCar, true);

	vector3 attachOffset[12] = {
		{0.0f, -10.4f, -1.5f}, {0.0f, -8.04f, -0.2f}, {0.0f, 10.0f, -1.5f}, {0.0f, 8.0f, -0.2f},
		{-3.0f, 2.01f, -1.5f}, {-3.0f, -4.11f, -1.5f}, {-1.0f, 2.01f, -0.2f}, {-1.0f, -4.11f, -0.2f},
		{3.0f, -4.11f, -1.5f}, {1.0f, -4.11f, -0.2f}, {1.0f, 0.35f, -0.2f}, {3.0f, 0.35f, -1.5f}
	};

	vector3 rotateOffset[12] = {
		{0.0f, 0.0f, 0.0f}, {0.22f, 0.0f, 0.0f}, {0.0f, 0.0f, 3.146f}, {0.22f, 0.0f, 3.146f},
		{0.0f, 0.0f, -1.56f}, {0.0f, 0.0f, -1.56f}, {0.22f, 0.0f, -1.56f}, {0.22f, 0.0f, -1.56f},
		{0.0f, 0.0f, 1.56f}, {0.22f, 0.0f, 1.56f}, {0.22f, 0.0f, 1.56f}, {0.0f, 0.0f, 1.56f}
	};

	for (int i = 0; i < 12; i++)
	{
		if (CreateObject(0x40433D8B, 0, 0, 0, 0, (Object*)&freeMem->m_object_car_entities[i], true))
		{
			ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[i], myObjectCar, 0, attachOffset[i].x, attachOffset[i].y, attachOffset[i].z, rotateOffset[i].x, rotateOffset[i].y, rotateOffset[i].z);
			FREEZE_OBJECT_POSITION(freeMem->m_object_car_entities[i], true);
			SET_OBJECT_COLLISION(freeMem->m_object_car_entities[i], true);
		}
	}

	printf("~r~CTL: ~w~Ramp Bus by ~y~OhManMyBad ~w~(12 Objects)");
}

uint NeonCutCubeModelFromColourSelect()
{
	const int selection = neonSelect; //prevent value being fucked by switch which has happened before.

	switch (selection)
	{
	case 0: return OBJECT_CJ_GAME_CUBE_C1; //red
	case 1: return OBJECT_CJ_GAME_CUBE_C2; //blue
	case 2: return OBJECT_CJ_GAME_CUBE_C3; //yellow
	case 3: return OBJECT_CJ_GAME_CUBE_C4; //green
	case 4: return OBJECT_CJ_GAME_CUBE_C5; //pink
	case 5: return OBJECT_CJ_GAME_CUBE_C6; //orange
	}

	return OBJECT_CJ_GAME_CUBE_C1; //red
}

void NeonizedInfernus()
{
	ClearFunnyCarEntities(); //only runs if enabled.
	SpawnPlayerInVehicle(VEHICLE_INFERNUS);
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myObjectCar);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, 0.3500f, 0.5500f, -0.8500f, 0.0000f, 0.0000f, -1.5500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, 0.3630f, -0.4297f, -0.8500f, 0.0000f, -3.1500f, -1.5500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, -0.3500f, -0.5500f, -0.8500f, 0.0000f, 0.0000f, 1.5500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, -0.3312f, 0.4495f, -0.8471f, 0.0000f, -3.1500f, 1.5500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[4], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[4], myObjectCar, 0, 0.0000f, -1.4500f, 0.5500f, -1.5500f, -0.6000f, 1.5500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[5], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[5], myObjectCar, 0, -0.0714f, -0.9165f, -0.1054f, 0.0000f, 0.2000f, 1.5575f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[6], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[6], myObjectCar, 0, -0.0606f, -1.0000f, -0.0463f, 0.3000f, 0.0000f, 0.0000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[7], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[7], myObjectCar, 0, 0.0830f, -0.6500f, -0.0804f, 0.2500f, 0.0000f, -3.1500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[8], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[8], myObjectCar, 0, -0.0668f, -0.7649f, -0.0856f, 0.0000f, 0.2000f, 1.5638f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[9], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[9], myObjectCar, 0, 0.0000f, -1.3449f, 0.3500f, -1.5500f, -0.4500f, 1.5500f);

	CreateObject(0x0D25A681, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[10], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[10], myObjectCar, 0, 0.0000f, -2.0714f, 0.7687f, 0.0000f, 0.0000f, 0.0000f);

	printf("~r~CTL: ~w~Neonized infernus by ~y~Im Foxxyyy.");
}

void NeonizedComet()
{
	ClearFunnyCarEntities();
	SpawnPlayerInVehicle(VEHICLE_COMET);
	//SET_CAR_VISIBLE(myObjectCar,false);
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myObjectCar);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, -0.3500f, -0.3000f, -0.8500f, 0.0000f, -0.1500f, 0.0000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, -0.3970f, 0.2945f, -0.9135f, 0.0000f, 0.0000f, 1.5500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, 0.3897f, -0.3083f, -0.9088f, -0.0773f, 0.0000f, -1.5500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, 0.4263f, 0.3442f, -0.9323f, 0.0000f, 0.0000f, 3.1500f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[4], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[4], myObjectCar, 0, -0.0694f, -1.1450f, -0.0151f, 0.3000f, 0.0000f, 0.0000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[5], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[5], myObjectCar, 0, 0.0255f, -1.1579f, -0.0303f, 0.0000f, 0.2760f, -1.5838f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[6], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[6], myObjectCar, 0, -0.0605f, 0.5388f, -0.3159f, 0.5365f, 0.0000f, 0.0000f);

	printf("~r~CTL: ~w~Neonized comet by ~y~Im Foxxyyy.");
}

void NeonizedSultan()
{
	ClearFunnyCarEntities();
	SpawnPlayerInVehicle(VEHICLE_SULTAN);
	//SET_CAR_VISIBLE(myObjectCar,false);
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myObjectCar);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, -0.2496f, -0.5812f, -0.6983f, 0.0000f, 0.0000f, 1.5800f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, -0.2676f, 0.4224f, -0.6983f, 0.0000f, 0.0000f, 6.3000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, 0.2521f, 0.4265f, -0.6983f, 0.0000f, 0.0000f, 4.7100f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, 0.2631f, -0.5607f, -0.6983f, 0.0000f, 0.0000f, 3.1299f);

	printf("~r~CTL: ~w~Neonized Sultan by ~y~Im Foxxyyy.");
}

void NeonizedSultanRS()
{
	ClearFunnyCarEntities();
	SpawnPlayerInVehicle(VEHICLE_SULTANRS);
	//SET_CAR_VISIBLE(myObjectCar,false);
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myObjectCar);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, -0.2496f, -0.5812f, -0.6983f, 0.0000f, 0.0000f, 1.5800f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, -0.2676f, 0.4224f, -0.6983f, 0.0000f, 0.0000f, 6.3000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, 0.2521f, 0.4265f, -0.6983f, 0.0000f, 0.0000f, 4.7100f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, 0.2631f, -0.5607f, -0.6983f, 0.0000f, 0.0000f, 3.1299f);

	printf("~r~CTL: ~w~Neonized Sultan RS by ~y~Im Foxxyyy.");
}

void NeonizedBanshee()
{
	ClearFunnyCarEntities();
	SpawnPlayerInVehicle(VEHICLE_BANSHEE);
	//SET_CAR_VISIBLE(myObjectCar,false);
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myObjectCar);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, -0.2842f, 0.4675f, -0.8606f, 0.0000f, 0.0000f, 0.0000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, -0.3020f, -0.4647f, -0.8606f, 0.0000f, 0.0000f, 1.5600f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, 0.3591f, -0.4776f, -0.8606f, 0.0000f, 0.0000f, 3.1200f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, 0.3775f, 0.4742f, -0.8606f, 0.0000f, 0.0000f, 4.7000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[4], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[4], myObjectCar, 0, 0.0143f, -1.8272f, 0.2000f, 4.7100f, 6.0300f, 1.5400f);

	CreateObject(0x0D25A681, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[5], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[5], myObjectCar, 0, -0.0184f, -2.3620f, 0.6244f, 0.0000f, 0.0000f, 0.0000f);

	printf("~r~CTL: ~w~Neonized Banshee by ~y~Im Foxxyyy.");
}

void NeonizedFuto()
{
	ClearFunnyCarEntities();
	SpawnPlayerInVehicle(VEHICLE_FUTO);
	//SET_CAR_VISIBLE(myObjectCar,false);
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myObjectCar);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, -0.1565f, 0.5492f, -0.7983f, 0.0000f, 0.0000f, 0.0000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, -0.1451f, -0.3867f, -0.7983f, 0.0000f, 0.0000f, 1.5900f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, 0.1999f, -0.3830f, -0.7983f, 0.0000f, 0.0000f, 3.1600f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, 0.1847f, 0.5732f, -0.7983f, 0.0000f, 0.0000f, 4.7300f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[4], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[4], myObjectCar, 0, 0.0349f, -1.2659f, 0.1081f, 3.1500f, 4.7000f, 6.2600f);

	CreateObject(0x0D25A681, 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[5], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[5], myObjectCar, 0, 0.0206f, -1.6916f, 0.6850f, 0.0000f, 0.0000f, 0.0000f);

	printf("~r~CTL: ~w~Neonized Futo by ~y~Im Foxxyyy.");
}

void NeonizedCalvacade()
{
	ClearFunnyCarEntities();
	SpawnPlayerInVehicle(VEHICLE_CAVALCADE);
	//SET_CAR_VISIBLE(myObjectCar,false);
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myObjectCar);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[0], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[0], myObjectCar, 0, -0.2602f, 0.6221f, -0.2189f, 0.0000f, 0.0000f, 0.0000f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[1], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[1], myObjectCar, 0, -0.2622f, -0.3489f, -0.2189f, 0.0000f, 0.0000f, 1.5700f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[2], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[2], myObjectCar, 0, 0.2499f, -0.3496f, -0.2189f, 0.0000f, 0.0000f, 3.1300f);

	CreateObject(NeonCutCubeModelFromColourSelect(), 0.0f, 0.0f, 0.0f, 0.0f, (Object*)&freeMem->m_object_car_entities[3], true);
	ATTACH_OBJECT_TO_CAR(freeMem->m_object_car_entities[3], myObjectCar, 0, 0.2519f, 0.6262f, -0.2189f, 0.0000f, 0.0000f, 4.7200f);

	printf("~r~CTL: ~w~Neonized Calvacade by ~y~Im Foxxyyy.");
}


void cerberusFunnyCars()
{
	SetHeader("Funny Cars by cerberusxxxx");
	AddItem("Towbar", EBFunnyCarTowbar, "(1) By: cerberusxxxx");
	AddItem("Airstairs", EBFunnyCarAirStairs, "(1) By: cerberusxxxx");
	AddItem("Tricycle", EBFunnyCarTricycle, "(19) By: cerberusxxxx"); //TBoGT Only due to vehicle model.
	AddItem("Cock Car", EBFunnyCarCockCar, "(21) By: cerberusxxxx");
	AddItem("Bagload Ramp", EBFunnyCarBagloadRamp, "(1) By: cerberusxxxx");
	AddItem("Car Jack", EBFunnyCarCarJack, "(1) By: cerberusxxxx");
	AddItem("Cement Mixer", EBFunnyCarCementMixer, "(1) By: cerberusxxxx");
	AddItem("Compresser", EBFunnyCarCompresser, "(1) By: cerberusxxxx");
	AddItem("David Blaine", EBFunnyCarDavidBlaine, "(1) By: cerberusxxxx");
	AddItem("Generator", EBFunnyCarGenerator, "(1) By: cerberusxxxx");
	AddItem("Giant Stairs", EBFunnyCarGiantStairs, "(1) By: cerberusxxxx");
	AddItem("Chained Heart", EBFunnyCarChainedHeart, "(2) By: cerberusxxxx");
	AddItem("Delorean", EBFunnyCarDelorean, "(30) By: cerberusxxxx");
	AddItem("Flintstones Car v2", EBFunnyCarFlintstonesTwo, "(32) By: cerberusxxxx");
	AddItem("Dead Girls", EBFunnyCarDeadGirls, "(20) By: cerberusxxxx");
	AddItem("General Lee", EBFunnyCarGeneralLee, "(30) By: cerberusxxxx");
	AddItem("Tumbler", EBFunnyCarTumbler, "(31) By: cerberusxxxx");
	AddItem("Water Car", EBFunnyCarWaterCar, "(13) By: cerberusxxxx");
	AddItem("Water Sled", EBFunnyCarWaterSled, "(2) By: cerberusxxxx");
	AddItem("RPG Sultan", EBFunnyCarRPGSultan, "(30) By: cerberusxxxx");
	AddItem("Head Hoist", EBFunnyCarHeadHoist, "(2) By: cerberusxxxx");
	AddItem("Log Car", EBFunnyCarLogCar, "(5) By: cerberusxxxx"); //TBoGT Only
	AddItem("Stroller 2Seat", EBFunnyCarStrollerTwoSeat, "(2) By: cerberusxxxx"); //TBoGT Only
	AddItem("Luggage Trailer", EBFunnyCarLuggageTrailer, "(1) By: cerberusxxxx");
	AddItem("Spray Car", EVFunnyCarSprayCar, "(9) By: cerberusxxxx");
	AddItem("Trailer", EBFunnyCarTrailer, "(1) By: cerberusxxxx");
	AddItem("Trailer Cart", EBFunnyCarTrailerCart, "(1) By: cerberusxxxx");
	AddItem("Moon Buggy", EBFunnyCarMoonBuggy, "(23) By: cerberusxxxx");
	AddItem("Scorpion", EBFunnyCarScorpion, "(24) By: cerberusxxxx");
	AddItem("Mechboard", EBFunnyCarMechBoard, "(1) By: cerberusxxxx");
	AddItemWithParam("Hospital Bed v1", 1, EBFunnyCarHospitalBed, "(1) By: cerberusxxxx");
	AddItemWithParam("Hospital Bed v2", 2, EBFunnyCarHospitalBed, "(1) By: cerberusxxxx");
	AddItemWithParam("Hospital Bed v3", 3, EBFunnyCarHospitalBed, "(3) By: cerberusxxxx");
}

void anonReporterFunnyCars()
{
	SetHeader("Funny Cars by Anon Reporter");

	AddItem("Cable Car", EBFunnyCarCableCar, "(1) By: Anon Reporter");
	AddItem("Row Boat", EBFunnyCarRowBoat, "(1) By: Anon Reporter");
	AddItem("Tuga", EBFunnyCarTuga, "(1) By: Anon Reporter");
	AddItem("Rocket Car Single", EBFunnyCarRocketCarSingle, "(2) By: Anon Reporter");
	AddItem("Rocket Car 2Seat", EBFunnyCarRocketCarTwoSeat, "(4) By: Anon Reporter");
	AddItem("Solar", EBFunnyCarSolar, "(8) By: Anon Reporter");
	AddItem("Dinghy", EBFunnyCarDinghy, "(1) By: Anon Reporter");
	AddItem("Predator", EBFunnyCarPredator, "(2) By: Anon Reporter");
	AddItem("Evil Ducky v2", EBFunnyCarEvilDuckyvTwo, "(19) By: Anon Reporter"); //Evil Duckys might be TBoGT only.
	AddItem("Evil Ducky v3", EBFunnyCarEvilDuckyvThree, "(19) By: Anon Reporter");
	AddItem("Evil Ducky v4", EBFunnyCarEvilFuckyVFour, "(19) By: Anon Reporter");
	AddItem("Glass Car", EBFunnyCarGlassCar, "(12) By: Anon Reporter"); //TBoGT Only
}

void EBCars()
{
	SetHeader("Funny/Object Cars");
	AddItemMenu("cerberusxxxx", cerberusFunnyCars, "They made so many they get their own section.");
	AddItemMenu("Anon Reporter", anonReporterFunnyCars, "They made so many they get their own section.");
	AddItem("Wheelbarrow", XMCFunnyCarWheelBarrow, "(1) From: XMC v6");
	AddItem("Piano Player", MDFunnyCarPianoPlayer, "(15) By: PHANTOMISED_");
	AddItem("Combat SultanRS", MDFunnyCarCombatSultanRS, "(28) By: PHANTOMISED_");
	AddItem("Bed", EBFunnyCarBedCar, "(2) By: OhManMyBad & InTheSevens");
	AddItem("Umbrella Boat Car", EBFunnyCarUmbrellaBoatCar, "(3) By: OhManMyBad & InTheSevens");
	AddItem("Ramp Bus", FunnyCarRampBus, "(12) By: OhManMyBad & InTheSevens"); //need to check EvilB didn't fuck credit up on this.
	AddItem("Cockpit", EBFunnyCarCockpitCar, "(1) By: OhManMyBad & InTheSevens"); //TBoGT only
	AddItem("Grave Angel", EBFunnyCarGraveAngel, "(1) By: OhManMyBad & InTheSevens"); //TBoGT only //not sure if it is?
	AddItem("Back to the Future Car", EBFunnyCarBackToTheFuture, "(16) By: OhManMyBad & InTheSevens"); //need to check EvilB didn't fuck credit up on this.
	AddItem("Rusty Back to the Future Car", XMCFunnyCarRustyBackToTheFuture, "(20) From: XMC v6");
	AddItem("RPG Truck", EBFunnyCarRPGTruck, "(12) By: Colt");
	AddItem("Dragster Bike 1", EBFunnyCarDragsterBikeOne, "(19) By: R3B3LY0N");
	AddItem("Dragster Bike 2", EBFunnyCarDragsterBikeTwo, "(15) By: EroiK I StyLeR");
	AddItemBool("Minecraft Car Glow", &ctlMem->m_bit_sets.m_vehicle, minecraftFunnyCarGlow, nullptr, "Want a glowing shitstorm? Doesn't disable at night.");
	AddItemChar("Minecraft Car", false, MinecraftColourStrings, 6, &minecraftFunnyColourSelect, EBFunnyCarMinecraftCar, "(23) By: EroiK I StyLeR");
	AddItem("Earth Ship", EBFunnyCarEarthShip, "(17) By: ZzLoik69zZ");
	AddItem("Flintstones Car", EBFunnyCarFlintstonesCar, "(7) By: ZzLoik69zZ");
	AddItem("Hot Rod", EBFunnyCarHotRod, "(24) By: ZzLoik69zZ");
	AddItem("Airplane", EBFunnyCarAirplane, "(2) By: Evil Blunt");
	AddItem("Couch", EBFunnyCarCouchCar, "(1) By: Evil Blunt");
	AddItem("Triceratops", EBFunnyCarTriceratops, "(1) By: Evil Blunt");
	AddItemWithParam("Jet (White)", 1, EBFunnyCarJet, "(1) By: Evil Blunt");
	AddItemWithParam("Jet (Black)", 2, EBFunnyCarJet, "(1) By: Evil Blunt");
	AddItem("LoL Car", EBFunnyCarLolCar, "(10) By: ChrisTuffurlo9l"); //TBoGT Only
	AddItem("Mini Kart", EBFunnyCarMiniKart, "(17) By: DsA x Hotsauce");
}

void EBTrucks()
{
	SetHeader("Funny/Object Trucks");
	AddItem("Fuck Packer", EBFunnyTruckFuckPacker, "(30) By: cerberusxxxx");
	AddItem("Lego Phantom", EBFunnyTruckLegoPhantom, "(19) By: cerberusxxxx");
	AddItem("Monster Truck", EBFunnyTruckMonsterTruck, "(31) By: R3B3LY0N & Ti jo P1CK UP");
	AddItem("Monster Truck v2", XMCFunnyTruckMonsterTruck, "(31) By: Boubouvirus");
	AddItem("Monster Truck Chasis", EBFunnyTruckMonsterTruckChasis, "(19) By: R3B3LY0N & Ti jo P1CK UP");
	AddItem("Wing Bus", EBFunnyTruckWingBus, "(1) By: OhManMyBad & InTheSevens");
	AddItem("Semi Hauler", EBFunnyTruckSemiHauler, "(3) By: OhManMyBad & InTheSevens");
	AddItem("Death Tractor Trailer", EBFunnyTruckDeathTractorTrailer, "(13) By: Anon Reporter");
	AddItem("Killer Flabed", EBFunnyTruckKillerFlatbed, "(15) By: Anon Reporter");
	AddItem("Oversized Load", EBFunnyTruckOversizedLoad, "(15) By: Anon Reporter");
	AddItem("School Bus", EBFunnyTruckSchoolBus, "(12) By: Anon Reporter");
	AddItem("Semi With Trailer", EBFunnyTruckSemiWithTrailer, "(1) By: Anon Reporter");
	AddItem("Locobrico", EBFunnyTruckLocobrico, "(17) By: ZzLoik69zZ");
	if (myCurrentEpisode == 2)
	{
		AddItem("Semi With Trailer v3", EBFunnyTruckSemiWithTrailerVThree, "(10) By: Anon Reporter"); //TBoGT Only bc of APC?
		AddItem("Wooden APC", EBFunnyTruckWoodenAPC, "(31) By: cerberusxxxx"); //TBoGT only
		AddItem("Garden APC", EBFunnyTruckGardenAPC, "(27) By: cerberusxxxx"); //TBoGT only
		AddItem("British APC", EBFunnyTruckBritishAPC, "(27) By: cerberusxxxx"); //TBoGT only
		AddItem("Happy Duck", EBFunnyTruckHappyDuck, "(6) By: ChrisTuffurlo9l"); //TBoGT only
		AddItem("Broken Plane Trailer", EBFunnyTruckBrokenPlaneTrailer, "(3) By: OhManMyBad & InTheSevens"); //TBoGT only
		AddItem("Merika Van", EBFunnyTruckMerikaVan, "(7) By: OhManMyBad & InTheSevens"); //TBoGT only
	}
	//ramp bus already nicked from source
}

void EBBikes()
{
	SetHeader("Funny/Object Bikes");
	AddItem("Slut Ride", MDFunnyBikeSlutRide, "(1) By: PHANTOMISED_");
	AddItem("Big PCJ", XMCFunnyBikeBigWheeledPCJ, "(2) By: Boubouvirus");
	AddItem("4-Wheel Bike", EBFunnyBike4Wheel, "(9) By: Ti jo P1CK UP");
	AddItem("Four Wheeler", EBFunnyBikeFourWheeler, "(5) By: Anon Reporter");
	AddItem("Three Wheeler", EBFunnyBikeThreeWheeler, "(4) By: Anon Reporter");
	AddItem("Death Bike", EBFunnyBikeDeathBike, "(10) By: Ti jo P1CK UP");
	AddItem("Dick Rider", EBFunnyBikeDickRider, "(1) By: Evil Blunt");
	AddItem("Bat Bike", EBFunnyBikeBatBike, "(28) By: cerberusxxxx");
	AddItem("Pogo Stick", EBFunnyBikePogoStick, "(6) By: cerberusxxxx");
	AddItem("Unicycle", EBFunnyBikeUnicycle, "(6) By: cerberusxxxx");
	AddItemWithParam("Butt Plug BMX", false, EBFunnyBikeButtPlugBMX, "(21) By: cerberusxxxx");
	AddItemWithParam("But Plug BMX with Trailer", true, EBFunnyBikeButtPlugBMX, "(25) By: cerberusxxxx");
	AddItem("Batpod", EBFunnyBikeBatPod, "(6) By: OhManMyBad & InTheSevens");
	AddItem("Hotdog Cart", EBFunnyBikeHotdogCart, "(6) By: OhManMyBad & InTheSevens");
	AddItem("Hovercraft", EBFunnyBikeHovercraft, "(3) By: OhManMyBad & InTheSevens");
	AddItem("Wheelbarrel", EBFunnyBikeWheelBarrel, "(2) By: OhManMyBad & InTheSevens");  //EvilB spelt it this way so I dunno, 
	AddItem("Giro Bike", EBFunnyBikeGiroBike, "(20) By: cerberusxxxx"); //TBoGT Only
	AddItem("Pallate Bike", EBFunnyBikePallateBike, "(3) By: OhManMyBad & InTheSevens"); //TBoGT Only
	AddItem("BMX", EBFunnyBikeBMX, "(5) By: ZzLoik69zZ & Ti jo P1CK UP"); //TBoGT Only
	AddItem("Portaloo", EBFunnyBikePortaloo, "(1) By: Evil Blunt");
	AddItem("T-Rex", EBFunnyBikeTRex, "(1) By: Evil Blunt");
	AddItem("Toilet v1", EBFunnyBikeToilet, "(1) By: Evil Blunt");
	AddItem("Toilet v2", EBFunnyBikeToiletTwo, "(2) By: OhManMyBad & InTheSevens"); //TBoGT Only ? is it?
	AddItem("Tron Bike", EBFunnyBikeTronBike, "(7) By: OhManMyBad & InTheSevens");
	AddItem("Shopping Cart v1", EBFunnyBikeShoppingCart, "(1) By: Evil Blunt");
	AddItem("Shopping Cart v2", EBFunnyBikeShoppingCartVTwo, "(2) By: OhManMyBad & InTheSevens");
	AddItem("Ski Bike", EBFunnyBikeSkiBike, "(4) By Ti jo P1CK UP");
	AddItem("Parachute Bike", EBFunnyBikeParachuteBike, "(1) By: OhManMyBad & InTheSevens"); //TBoGT Only
}

void EBBoats()
{
	SetHeader("Funny/Object Boats");
	AddItem("Bus", EBFunnyBoatBus, "(1) By: Anon Reporter");
	AddItem("Cable Car", EBFunnyBoatCableCar, "(1) By: Anon Reporter");
	AddItem("Cargo Ship", EBFunnyBoatCargoShip, "(1) By: Anon Reporter");
	AddItem("Rocket", EBFunnyBoatRocket, "(1) By: Anon Reporter");
	AddItem("Rocket v2", XMCFunnyBoatRocket, "(4) From: XMC v6"); // I think it's by XMC?
	AddItemWithParam("Sea Jet 1", 1, EBFunnyBoatSeaJet, "(17) By: R3B3LY0N");
	AddItemWithParam("Sea Jet 2", 2, EBFunnyBoatSeaJet, "(24) By: R3B3LY0N");
	AddItem("Vinewood", EBFunnyBoatVinewood, "(6) By: R3B3LY0N");
	AddItem("Hexer", EBFunnyBoatHexer, "(1) By: Anon Reporter"); //TBoGT Only
	AddItem("Yacht", EBFunnyBoatYacht, "(1) By: OhManMyBed & InTheSevens"); //TBoGT Only
}


void EBAircraft()
{
	SetHeader("Funny/Object Aircraft");
	AddItem("Modzilla", MDFunnyHeliModZilla, "(34) By: PHANTOMISED_");
	AddItem("Nightfighter", MDFunnyHeliNightFighter, "(11) By: AddictiveGamer97");
	AddItem("Noob heli", MDFunnyHeliNoob, "(4) By: AddictiveGamer97");
	AddItem("Bullet blocker", MDFunnyHeliFuckUp, "(4) By: PHANTOMISED_");
	AddItem("Private Jet", XMCFunnyHeliPrivateJet, "(1) From: XMC v6");
	AddItem("Cable car", EBFunnyHeliCableCar, "(1) By: Anon Reporter");
	AddItem("Caerpillar Fly", EBFunnyHeliCaterpillarFly, "(19) By: ZzLoik69zZ");
	AddItem("Spider Fly", EBFunnyHeliSpiderFly, "(17) By: ZzLoik69zZ");
	AddItem("Couch", EBFunnyHeliCouch, "(1) By: Anon Reporter");
	AddItem("Flying Carpet", EBFunnyHeliFlyingCarpet, "(3) By: Anon Reporter");
	AddItem("Harry Potter Broom", EBFunnyHeliHarryPotterBroomStick, "(1) By: DsA x Hotsauce");
	AddItem("Hot-Air Balloon", EBFunnyHeliHotAirBalloon, "(9) By: Ti jo P1CK UP");
	AddItem("Whale", EBFunnyHeliWhale, "(30) By: cerberusxxxx");
	AddItem("Recliner", EBFunnyHeliRecliner, "(1) By: Anon Reporter");
	AddItem("Spaceship", EBFunnyHeliSpaceship, "(24) By: ZzLoik69zZ");
	AddItem("Cavalcade", EBFunnyHeliCavalcade, "(1) By: Anon Reporter");
	AddItem("Dinghy", EBFunnyHeliDinghy, "(1) By: Anon Reporter");
	AddItem("Ovni", EBFunnyHeliOVni, "(11) By: ZzLoik69zZ");
	AddItem("Mini Falcon", EBFunnyHeliMiniFalcon, "(37) By: cerberusxxxx");
	AddItem("Marquis", EBFunnyHeliMarquis, "(1) By: Anon Reporter");
	AddItem("Heli Plane", EBFunnyHeliHeliPlane, "(6) By: R3B3LY0N");
	AddItemBool("Minecraft Plane Glow", &ctlMem->m_bit_sets.m_vehicle, minecraftFunnyCarGlow, nullptr, "Want a glowing shitstorm? Doesn't disable at night.");
	AddItemChar("Minecraft Plane", false, MinecraftColourStrings, 6, &minecraftFunnyColourSelect, EBFunnyHeliMinecraftPlane, "(18) By: EroiK I StyLeR");
	if (myCurrentEpisode == 2)
	{
		AddItem("AH-6 Littlecock", EBFunnyHeliAH6Littlecock, "(32) By: cerberusxxxx");
		AddItem("Behemoth", EBFunnyHeliBehemoth, "(26) By: cerberusxxxx");
		AddItem("UFO (Foo Fighter)", EBFunnyHeliUFOFooFighter, "(15) By: cerberusxxxx");
		AddItem("UFO (Smooth)", EBFunnyHeliUFOSmooth, "(13) By: cerberusxxxx");
		//more to do ugh.
	}
}

void VehSpawnFunny()
{
	SetHeader("Vehicle -> Spawn -> Objects -> Funny");
	AddItemMenu("Cars", EBCars, "Debug");
	AddItemMenu("Trucks", EBTrucks, "Debug");
	AddItemMenu("Bikes", EBBikes, "Debug");
	AddItemMenu("Boats", EBBoats, "Debug");
	AddItemMenu("Aircraft", EBAircraft, nullptr);
	//AddItem("Ramp plow", FunnyCarByNathanRamp, "Created by IIV NATHAN VII");
	AddItem("R* devs hard at work", FunnyCarByImFoxxyNamedDrunkManAizle, "Created by Im Foxxyyy."); //will probably rename this out of decency
	AddItem("Tropical bed", FunnyCarByImFoxxyyyNamedTropical, "Created by Im Foxxyyy.");
	AddItem("Bint rider", FunnyCarBintRider, nullptr);
	AddItemBoolAlt("Delete last", &ctlMem->m_bit_sets.m_vehicle, deleteLastFunnyVehObjects, nullptr, "Automatically delete last object vehicle, ~PAD_X~ = Current.", ClearFunnyCarEntitiesAndSetVisible, nullptr);
}

void VehSpawnNeon()
{
	SetHeader("Vehicle -> Spawn -> Objects -> Neons");
	AddItemChar("Neon colour", false, NeonStrings, 6, &neonSelect, nullptr, "The colour of neon to attach.");
	AddItem("Infernus", NeonizedInfernus, "Credit: Im Foxxyyy.");
	AddItem("Comet", NeonizedComet, "Credit: Im Foxxyyy.");
	AddItem("Sultan", NeonizedSultan, "Credit: Im Foxxyyy.");
	AddItem("Sultan RS", NeonizedSultanRS, "Credit: Im Foxxyyy.");
	AddItem("Banshee", NeonizedBanshee, "Credit: Im Foxxyyy.");
	AddItem("Futo", NeonizedFuto, "Credit: Im Foxxyyy.");
	AddItem("Calvacade", NeonizedCalvacade, "Credit: Im Foxxyyy.");
	AddItemBool("Delete last", &ctlMem->m_bit_sets.m_vehicle, deleteLastFunnyVehObjects, nullptr, "Automatically delete last object vehicle.");
}

void VehSpawnObject()
{
	SetHeader("Vehicle -> Spawn -> Objects");
	AddItemMenu("Funny vehicles", VehSpawnFunny, "Wanna drive around while making a deposit?");
	AddItemMenu("Neon vehicles", VehSpawnNeon, "Also known as underglow.");
	AddItemBoolAlt("Delete last", &ctlMem->m_bit_sets.m_vehicle, deleteLastFunnyVehObjects, nullptr, "Automatically delete last object vehicle, ~PAD_X~ = Current.", ClearFunnyCarEntitiesAndSetVisible, nullptr);
}