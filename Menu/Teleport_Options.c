#include "stdafx.h"
#include "Teleport_Options.h"
#include "MenuExecutionHandling.h"
#include "Functions.h"
#include "Utils.h"
#include "Vehicle_Options.h"
#include "Network_Options.h"
#include "Player_Options.h"

int teleAirportSelect = 0;
int teleHappySelect = 0; //move these to bytes eventually..
int telePrisonSelect = 0;
int teleStripclubSelect = 0;
vector4 savedDest = { 0.0f, 0.0f, -1337.0f, 0.0f };
PLAYER_INDEX TeleportSelectedPlayerId; //might not need this but will chexk when im not fucked up on codeine. (wohoo wisdom teeth)

void TeleportIntoNetworkPlayersVehicle(PLAYER_INDEX playerIndex, int unused) //needs optimising but works for now.
{
	VEHICLE_INDEX wouldEnter;
	PED_INDEX target_ped;
	VEHICLE_INDEX target_veh, our_veh;

	netPlayerVehDetach();

	if (PLAYER_HAS_CHAR(playerIndex))
	{
		GET_PLAYER_CHAR(playerIndex, &target_ped);

		if (IS_CHAR_IN_ANY_CAR(target_ped))
		{
			GET_CAR_CHAR_IS_USING(target_ped, &target_veh);
			//NEW - Crash prevention attempt
			GET_VEHICLE_PLAYER_WOULD_ENTER(myPlayerIndex, &wouldEnter);
			if (wouldEnter == target_veh)
				return;
			////
			VEHICLE_SEAT freeSeat = GetFreeVehicleSeat(target_veh);

			if (freeSeat == SEAT_INVALID) //Vehicle is full. //UNCOMMENT THIS/TESTING
			{
				uint target_model;

				GET_CAR_MODEL(target_veh, &target_model);

				//TODO NATH: Add an option to do this without having to teleport in.
				if (target_model == VEHICLE_BUS) //lets do some magic to get in anyway even tho it's full.
				{
					float bX, bY, bZ;
					VEHICLE_INDEX attachedBus;
					bool warpedExtra = false;

					if (IS_CAR_ATTACHED(target_veh)) //likely already an attachedBus..don't do any more..
					{
						VEHICLE_INDEX attachedHost;

						if (GetAttachedVehicleHost(target_veh, &attachedHost))
						{
							uint tmp_model;

							GET_CAR_MODEL(attachedHost, &tmp_model);

							if (tmp_model == VEHICLE_BUS)
							{
								if (warp_local_player_into_vehicle(attachedHost))
									return;
							}
						}

						printf("~r~CTL: %s's ~w~Bus has no free seats, and is already rigged for extra.", NetworkPlayerGetNameWithColour(playerIndex));
						return;
					}

					GET_CAR_COORDINATES(target_veh, &bX, &bY, &bZ);
					int freeSpace = -1;
					if (IsExtraBusAlreadyAttached(target_veh, &attachedBus)) //a Cartel user has already rigged this bus!
					{
						for (int i = 2; i >= 0; i--)
						{
							if (IS_CAR_PASSENGER_SEAT_FREE(attachedBus, i))
							{
								WARP_CHAR_INTO_CAR_AS_PASSENGER(myPlayerPed, attachedBus, i);
								warpedExtra = true;
								break;
							}
						}
					}
					else //create extra bus and attach
					{
						
						for (int i = 0; i < MAX_RIGGED_BUSSES; i++)
						{
							if (DOES_VEHICLE_EXIST(ctlMem->m_CartelBusses[i].m_attachedBus) || DOES_VEHICLE_EXIST(ctlMem->m_CartelBusses[i].m_hostBus))
								continue; //already in use..

							freeSpace = i;
							break;
						}
						if (freeSpace != -1) //space available.
						{
							if (SpawnVehicle(VEHICLE_BUS, 0.0f, 0.0f, 0.0f, 0.0f, &ctlMem->m_CartelBusses[freeSpace].m_attachedBus, false, true)) //prot should release control, worry about this later..
							{
								ctlMem->m_CartelBusses[freeSpace].m_hostBus = target_veh;
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
								ATTACH_CAR_TO_CAR(ctlMem->m_CartelBusses[freeSpace].m_attachedBus, target_veh, 0, 0.0f, -6.0f, 0.0f, 0.0f, 0.0f, 0.0f); //6 is good for 3 rear passengers.
								WARP_CHAR_INTO_CAR_AS_PASSENGER(myPlayerPed, ctlMem->m_CartelBusses[freeSpace].m_attachedBus, 2); //back seat aw yea
								warpedExtra = true;
							}
						}
					}

					if (warpedExtra)
					{
						clearFollowDriverData();
						detachLocalPlayerFromVehicleOrWalker();
						disableFlymod();
						printf("~r~CTL: ~w~With some Cartel magic, this bus has 4 extra seats - Nathan");
					}
					else
					{
						if (freeSpace == -1)
							printf("~r~CTL: ~w~Maximum rigged busses have already been spawned.");
						else
							printf("~r~CTL: %s's ~w~Bus has no free seats, and is already rigged for extra!");
					}

				} //if VEHICLE_BUS
				else
				{
					printf("~r~CTL: %s's ~w~%s has no free seats", NetworkPlayerGetNameWithColour(playerIndex), VehiclesModelName(target_veh));

					if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y) && IS_CHAR_IN_ANY_CAR(myPlayerPed))
					{
						GET_CAR_CHAR_IS_USING(myPlayerPed, &our_veh);
						WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, our_veh, GetPedSeatIndex(myPlayerPed));
					}
				}
			}
			else
			{
				detachLocalPlayerFromVehicleOrWalker();
				disableFlymod();
				clearFollowDriverData();
				WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, target_veh, freeSeat); //pretty sure it works on foot too //gettin warped out after few secs, wut? //protection follow veh driver mod fucking it up/needa debug.
			}
		}
		else
		{
		/*
			if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y)) //should be true but whatever
			{
				if (IS_CHAR_IN_ANY_CAR(our_ped))
				{
					GET_CAR_CHAR_IS_USING(our_ped, &our_veh);
					WARP_CHAR_FROM_CAR_TO_CAR(our_ped, our_veh, GetPedSeatIndex(our_ped)); //cancel out any exits.
					clearFollowDriverData();
				}
			}
			*/
			printf("~r~CTL: %s ~w~is on foot.", NetworkPlayerGetNameWithColour(playerIndex));
			//cancel out exit vehicle etc.
		}
	}
}

void TeleportToNetworkPlayer(PLAYER_INDEX playerIndex, int unused) //moved variables to top to look cleaner and renamed some variables so they make more sense
{
	PED_INDEX target_ped, driver;
	NETWORK_INDEX our_veh_net_id;
	uint our_veh_model, target_veh_model;
	float dx, dy, dz, dh;
	vector3 our_min, our_max, target_min, target_max;
	VEHICLE_INDEX target_veh, our_veh;

	if (PLAYER_HAS_CHAR(playerIndex))
	{
		GET_PLAYER_CHAR(playerIndex, &target_ped);
		if (IS_CHAR_IN_ANY_CAR(target_ped))
			GET_CAR_CHAR_IS_USING(target_ped, &target_veh); //pretty sure we can run this even if they are not in a vehicle and it will just give us 0 - Nath
	}
	else
	{
		printf("~r~CTL: ~w~Unable to get the co-ordinates of a holy ghost.");
		return;
	}

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed)) //Lets grab our car if we are in one so we don't need to call it later
	{
		GET_CAR_CHAR_IS_USING(myPlayerPed, &our_veh);

		if (target_veh != our_veh) //don't request control if we're sat in their car / warp out.
		{
			GET_NETWORK_ID_FROM_VEHICLE(our_veh, &our_veh_net_id);

			if (RequestNetworkControl(our_veh_net_id, VehiclesModelName(our_veh))) //Grab control of the network id if we dont already have it
			{
				GET_CAR_MODEL(our_veh, &our_veh_model);
				GET_MODEL_DIMENSIONS(our_veh_model, &our_min, &our_max);
			}
		}
	}

	if (IS_CHAR_IN_ANY_CAR(target_ped)) //Target is in vehicle
	{
		GET_CAR_MODEL(target_veh, &target_veh_model);
		GET_MODEL_DIMENSIONS(target_veh_model, &target_min, &target_max);

		if (IS_CHAR_IN_ANY_CAR(myPlayerPed)) //We are in a vehicle
		{
			if (HAS_CONTROL_OF_NETWORK_ID(our_veh_net_id))
			{
				GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(target_veh, target_max.y + our_max.y >= 8.5f ? -(target_max.x + our_max.x + 1.5f) : 0.0f, target_max.y + our_max.y >= 8.5f ? 0.0f : -(target_max.y + our_max.y + 2.0f), 0.0f, &dx, &dy, &dz);
				GET_CHAR_HEADING(target_ped, &dh);
				TeleportLocalPlayerInVehicle(dx, dy, dz, dh);
			}
			else
			{
				GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(target_veh, 0.0f, 0.0f, target_max.z + 0.1f, &dx, &dy, &dz); //On roof
				GET_CAR_HEADING(target_veh, &dh);
				TeleportLocalPlayerOnFoot(dx, dy, dz + 0.1f, dh); //will warp out
			}
		}
		else //We are on foot
		{
			//Keep this because I'm considering warping in if they're driving at speed..
			/*
			if (warp_local_player_into_vehicle(target_veh))
			{
				if (IS_BIT_SET(ctlMem->m_bit_sets.m_player, flyMod)) //Shouldn't we toggle this before any teleports happen?
				{
					printf("~r~CTL: ~w~Fly mod disabled.");
					bitReset(&ctlMem->m_bit_sets.m_player, flyMod);
					worldOptsToggleFlymod();
				}
				return;
			}
			*/

			//No seats available and not returned (target in vehicle and we are on foot).
			GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(target_veh, 0.0f, 0.0f, target_max.z + 0.95f, &dx, &dy, &dz); //On roof, vehModelMax.z should return half the height of the vehicle (should work nicely)
			GET_CAR_HEADING(target_veh, &dh);
			TeleportLocalPlayerOnFoot(dx, dy, dz, dh);
		}
	}
	else //Target is on foot
	{
		bool we_control_veh = IS_CHAR_IN_ANY_CAR(myPlayerPed) && HAS_CONTROL_OF_NETWORK_ID(our_veh_net_id);
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(target_ped, 0.0f, we_control_veh ? -(our_max.y + 1.5f) : -3.0f, we_control_veh ? -0.5 : 0.0f, &dx, &dy, &dz);
		GET_CHAR_HEADING(target_ped, &dh);
		we_control_veh ? TeleportLocalPlayerInVehicle(dx, dy, dz, dh) : TeleportLocalPlayerOnFoot(dx, dy, dz, dh);
	}
}

void TeleportNetworkPlayerToMe(PLAYER_INDEX player, int unused)
{
	float tx, ty, tz, th;
	PED_INDEX target_ped;
	uint our_veh_model, target_veh_model;
	vector3 our_min, our_max, target_min, target_max;
	VEHICLE_INDEX our_veh, target_veh;
	NETWORK_INDEX target_veh_net_id;

	GET_CHAR_HEADING(myPlayerPed, &th);

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed)) //Lets grab our car if we are in one so we don't need to call it later
	{
		GET_CAR_CHAR_IS_USING(myPlayerPed, &our_veh);
		GET_CAR_MODEL(our_veh, &our_veh_model);
		GET_MODEL_DIMENSIONS(our_veh_model, &our_min, &our_max);
	}

	if (PLAYER_HAS_CHAR(player))
	{
		GET_PLAYER_CHAR(player, &target_ped);

		if (IS_CHAR_ON_FOOT(target_ped))
		{
			if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
			{
				//Target player for teleporting is on foot, local/destination player is in a vehicle
				if (IS_THIS_MODEL_A_CAR(our_veh_model))
					GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(our_veh, 0.0f, 0.0f, our_max.z + 0.1f, &tx, &ty, &tz); //on vehicles roof
				else
					GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(our_veh, 0.0f, our_max.y + 2.25f, 0.0f, &tx, &ty, &tz); //in front of vehicle.
			}
			else
			{
				GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, 3.0f, 1.0f, &tx, &ty, &tz);
				GET_GROUND_Z_FOR_3D_COORD(tx, ty, tz, &tz);
				tz += 1.0f; //fix GroundZ for player.
			}

			TeleportNetworkPlayerOnFoot(player, tx, ty, tz, th - 180.0f, true);
		}
		else
		{
			//Target player teleporting to local player is in a vehicle
			GET_CAR_CHAR_IS_USING(target_ped, &target_veh);
			GET_NETWORK_ID_FROM_VEHICLE(target_veh, &target_veh_net_id);

			if (RequestNetworkControl(target_veh_net_id, VehiclesModelName(target_veh)))
			{
				if (!DOES_VEHICLE_EXIST(target_veh))
					return; //stop potential freeze from wait

				GET_CAR_MODEL(target_veh, &target_veh_model);
				GET_MODEL_DIMENSIONS(target_veh_model, &target_min, &target_max);

				if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
				{
					if (target_max.y + our_max.y >= 8.5f) //big busses or something (go to the side)
						GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(our_veh, -(target_max.x + our_max.x + 1.5f), 0.0f, 0.0f, &tx, &tx, &tz);
					else
					{
						GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(our_veh, -(target_max.x + our_max.x), ((target_max.y + our_max.y) * 2), 0.0f, &tx, &ty, &tz);
						th -= 135.0f;
					}

					SET_CAR_COORDINATES_NO_OFFSET(target_veh, tx, ty, tz);
					SET_CAR_HEADING(target_veh, th);
				}
				else
				{
					//Target teleporting is in a vehicle and local player is on foot
					GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, -(target_max.x * 2), ((target_max.y * 2) + 1.5f), 0.0f, &tx, &ty, &tz);
					GET_CHAR_HEADING(myPlayerPed, &th); //th -= 135.0f;
					SET_CAR_COORDINATES_NO_OFFSET(target_veh, tx, ty, tz);
					SET_CAR_HEADING(target_veh, th - 135.0f);
				}

				//TODO Interior scenes are broken when teleporting into a scene sometimes..
				//Need to check out the issues surrounding that and what's required to fix.
				//using this, it seems to grab the scene before you actually arrive at your destination..
				/*
				if (IS_INTERIOR_SCENE())
				{
					int Viewport, interiorKey;
					GET_GAME_VIEWPORT_ID(&Viewport);
					GET_KEY_FOR_VIEWPORT_IN_ROOM(Viewport, &interiorKey);
					SET_ROOM_FOR_CHAR_BY_KEY(myPed, interiorKey);
					SET_ROOM_FOR_CAR_BY_KEY(myVeh, interiorKey);
				}
				*/
			}
		}
	}
	else
		printf("~r~CTL: ~w~There is no point teleporting a holy ghost.");
}


void TeleportLocalPlayerOnFoot(float x, float y, float z, float h)
{
	if(IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh;
		PED_INDEX driver;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
		GET_DRIVER_OF_CAR(myVeh, &driver);
		bool toMark = driver == myPlayerPed; //helps stop littering non migratable vehicles..
		WARP_CHAR_FROM_CAR_TO_COORD(myPlayerPed, x, y, z);

		if (toMark)
			MARK_CAR_AS_NO_LONGER_NEEDED(&myVeh);
		//unlock release contorl of vehicle etc? should make a func..
	}
	else
		SET_CHAR_COORDINATES_NO_OFFSET(myPlayerPed, x, y, z);

	SET_CHAR_HEADING(myPlayerPed, h);
	SET_CAM_BEHIND_PED(myPlayerPed);
	REQUEST_COLLISION_AT_POSN(x, y, z); //TESTING (Should make loading the new area much faster).
	clearFollowDriverData();
	/*
	if (IS_INTERIOR_SCENE()) //catches the scene you're leaving and fucks up
	{
		int Viewport, interiorKey;
		GET_GAME_VIEWPORT_ID(&Viewport);
		GET_KEY_FOR_VIEWPORT_IN_ROOM(Viewport, &interiorKey);
		SET_ROOM_FOR_CHAR_BY_KEY(myPed, interiorKey);
	}*/
}

bool WaitForGroundZ(float startHeightOffset) //Necessary for SET_CAR_ON_GROUND_PROPERLY to work..
{
	float mX, mY, mZ;
	float groundZ = 0.0f;

	GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
	uint tick = GetTickCount() + 4000; //4 sec should be enough time to load...

	while (groundZ == 0.0f) //could make this increment the ground height offset each couple seconds or something...
	{
		GET_GROUND_Z_FOR_3D_COORD(mX, mY, mZ + startHeightOffset, &groundZ); //could increase this but don't really wanna...
		WAIT(0);

		if (tick < GetTickCount()) //4 Secs have passed..
			return false;
	}

	return true;
}

bool TeleportNetworkPlayerOnFoot(const PLAYER_INDEX playerIndex, float x, float y, float z, float h, bool checkTeleported)
{
	bool grabbed_their_armour_weapons;
	int playerArmour, playerCurrentWeapon, playerCurrentAmmo;
	PED_INDEX playerPed;
	WEAPON_TYPE playerWeapons[10], playerAmmos[10]; //get all weapons and ammo amounts.
	if (IsPlayerValid(playerIndex) && PLAYER_HAS_CHAR(playerIndex))
	{
		grabbed_their_armour_weapons = true;
		GET_PLAYER_CHAR(playerIndex, &playerPed);
		GET_CHAR_ARMOUR(playerPed, &playerArmour);
		GET_CURRENT_CHAR_WEAPON(playerPed, &playerCurrentWeapon);
		GET_AMMO_IN_CHAR_WEAPON(playerPed, playerCurrentWeapon, &playerCurrentAmmo);
		//weapon indexes are 0 to 36
		for (int i = 0; i < 10; i++) //8 is confirmed to work.
		{
			playerWeapons[i] = -1; 
			playerAmmos[i] = -1;
		}
		
		for (int i = 0; i < 36; i++)
		{
			if (i != WEAPON_ROCKET && i != WEAPON_EPISODIC_18)
			{
				if (HAS_CHAR_GOT_WEAPON(playerPed, i) && i != playerCurrentWeapon)
				{
					for (int j = 0; j < 10; j++)
					{
						if (playerWeapons[j] == -1) //space available here
						{
							playerWeapons[j] = i;
							GET_AMMO_IN_CHAR_WEAPON(playerPed, i, &playerAmmos[j]);
							break;
						}
					}
				}
			}
		}
	}

	SetHostByte(true);
	RESURRECT_NETWORK_PLAYER(playerIndex, x, y, z - 1.0f, h); //z - 1.0 as it sets +1.0
	SetHostByte(GET_HOST_ID() == myPlayerIndex);

	if (playerIndex == myPlayerIndex || !checkTeleported) //we don't needa run detection on ourselves.
	{
		if (grabbed_their_armour_weapons)
		{
			if (playerArmour > 0)
			{
				GIVE_WEAPON_TO_CHAR(playerPed, WEAPON_ARMOUR, 1, false);
				ADD_ARMOUR_TO_CHAR(playerPed, playerArmour); //restore their armour.
			}
			for (int i = 0; i < 10; i++)
			{
				if (playerWeapons[i] != -1)
					GIVE_WEAPON_TO_CHAR(playerPed, playerWeapons[i], playerAmmos[i], false);
			}
			GIVE_WEAPON_TO_CHAR(playerPed, playerCurrentWeapon, playerCurrentAmmo, true);
			
		}
		return true;
	}

	if (grabbed_their_armour_weapons)
	{
		uint tick = GetTickCount() + 1150;
		float curX, curY, curZ, dist;
		char* playersName = NetworkPlayerGetNameWithColour(playerIndex);

		while (tick > GetTickCount())
		{
			//added safety check / we will get fucked if they leave while checking their teleport success / acting on a ped that no longer exists.

			GET_PLAYER_CHAR(playerIndex, &playerPed);
			GET_CHAR_COORDINATES(playerPed, &curX, &curY, &curZ);
			GET_DISTANCE_BETWEEN_COORDS_3D(curX, curY, curZ, x, y, z, &dist);

			if (dist < 15.0f)
			{
				float freshDist;

				WAIT(100);//they will rubber-band if using retail "Warp-back" protection.
				if (!IsPlayerValid(playerIndex) || !PLAYER_HAS_CHAR(playerIndex))
				{
					printf("~r~CTL: %s ~w~left game or has no ped, can't check teleport success.", playersName);
					return false;
				}
				GET_PLAYER_CHAR(playerIndex, &playerPed); //never know might change and crash.
				GET_CHAR_COORDINATES(playerPed, &curX, &curY, &curZ);
				GET_DISTANCE_BETWEEN_COORDS_3D(curX, curY, curZ, x, y, z, &freshDist);

				if (freshDist < 15.0f)
				{
					if (playerArmour > 0)
					{
						GIVE_WEAPON_TO_CHAR(playerPed, WEAPON_ARMOUR, 1, false);
						ADD_ARMOUR_TO_CHAR(playerPed, playerArmour); //restore their armour.
					}
					for (int i = 0; i < 10; i++)
					{
						if (playerWeapons[i] != -1)
							GIVE_WEAPON_TO_CHAR(playerPed, playerWeapons[i], playerAmmos[i], false); //give them their weapons back.
					}
					GIVE_WEAPON_TO_CHAR(playerPed, playerCurrentWeapon, playerCurrentAmmo, true);
					return true; //Teleport Successful.
				}
			}
			WAIT(0);
		} 

		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
		printf("~r~CTL: ~w~Looks like %s ~w~has teleport protection.", playersName); //returns if successful or player left so will only print if failed to teleport player.
	}

	return false;
}

bool TeleportLocalPlayerInVehicle(float x, float y, float z, float h)
{
	VEHICLE_INDEX myVeh;

	if (!IS_CHAR_IN_ANY_CAR(myPlayerPed))
		return false;

	GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);

	if (NetworkGame)
	{
		NETWORK_INDEX netId;

		GET_NETWORK_ID_FROM_VEHICLE(myVeh, &netId);
		if (!RequestNetworkControl(netId, VehiclesModelName(myVeh)))
			return false;
	}

	SET_CAR_COORDINATES_NO_OFFSET(myVeh, x, y, z); //did i read -1 will set the ground automatically?
	SET_CAR_HEADING(myVeh, h);
	SET_CAM_BEHIND_PED(myPlayerPed);
	REQUEST_COLLISION_AT_POSN(x, y, z); //TESTING (Should make loading new part of map much faster).

	if (WaitForGroundZ(10.0f)) //WAITS FOR GROUND Z @ 10.0f above current z 
		SET_CAR_ON_GROUND_PROPERLY(myVeh);

	//SET_CAR_ON_GROUND_PROPERLY(myVeh); //doesn't work while area not rendered.
	/*
	if (IS_INTERIOR_SCENE())
	{
		int Viewport, interiorKey;
		GET_GAME_VIEWPORT_ID(&Viewport);
		GET_KEY_FOR_VIEWPORT_IN_ROOM(Viewport, &interiorKey);
		SET_ROOM_FOR_CHAR_BY_KEY(myPed, interiorKey);
		SET_ROOM_FOR_CAR_BY_KEY(myVeh, interiorKey);
	}
	*/
	return true;
}

bool warp_local_player_into_vehicle(VEHICLE_INDEX target_veh)
{
	PED_INDEX driver;

	if (!DOES_VEHICLE_EXIST(target_veh)) 
		return false;

	GET_DRIVER_OF_CAR(target_veh, &driver);

	if (!DOES_CHAR_EXIST(driver))
	{
		detachLocalPlayerFromVehicleOrWalker();
		disableFlymod();

		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
			WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, target_veh, SEAT_DRIVER);
		else
			WARP_CHAR_INTO_CAR(myPlayerPed, target_veh);

		clearFollowDriverData();
		return true;
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (IS_CAR_PASSENGER_SEAT_FREE(target_veh, i))
			{
				detachLocalPlayerFromVehicleOrWalker();
				disableFlymod();

				if(IS_CHAR_IN_ANY_CAR(myPlayerPed))
					WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, target_veh, i);
				else
					WARP_CHAR_INTO_CAR_AS_PASSENGER(myPlayerPed, target_veh, i);

				clearFollowDriverData();
				return true;
			}
		}
	}
	
	return false;
}

void teleOptsNearestVeh()
{
	VEHICLE_INDEX nearbyVehicle;
	PED_INDEX nearbyDriver;
	float mX, mY, mZ;

	GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
	nearbyVehicle = GET_CLOSEST_CAR(mX, mY, mZ, 70.0f, false, 70);

	if (DOES_VEHICLE_EXIST(nearbyVehicle))
		warp_local_player_into_vehicle(nearbyVehicle);
	else 
		printf("~r~CTL: ~w~No vehicles detected.");
}

bool TeleportNetworkPlayerToCoordinate(PLAYER_INDEX playerIndex, float x, float y, float z, float h)
{
	if (IsPlayerValid(playerIndex))
	{
		if (PLAYER_HAS_CHAR(playerIndex))
		{
			PED_INDEX networkPlayerPed;

			GET_PLAYER_CHAR(playerIndex, &networkPlayerPed);

			if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
			{
				VEHICLE_INDEX networkPlayerVehicle;
				NETWORK_INDEX networkPlayerVehicleId;

				GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
				GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);

				if (RequestNetworkControl(networkPlayerVehicleId, VehiclesModelName(networkPlayerVehicle))) //could check if our car, ehhh //Resurrecting players in a vehicle breaks their movement.
				{
					SET_CAR_FORWARD_SPEED(networkPlayerVehicle, 0.0f);
					SET_CAR_COORDINATES_NO_OFFSET(networkPlayerVehicle, x, y, z);
					SET_CAR_HEADING(networkPlayerVehicle, h);
					return true;
				}
			}
			else
			{
				if (TeleportNetworkPlayerOnFoot(playerIndex, x, y, z, h, true)) //last arg checks they arrived there for 1100ms
					return true;
			}
		}
		else
			printf("~r~CTL: ~w~Player has no character, cannot proceed.");
	}
	else
		printf("~r~CTL: ~w~Player isn't present, cannot proceed.");

	return false;
}

void teleOptsAirport() //use this as a template in future..
{
	const int index = teleAirportSelect; //makes duplicating this function easier..
	vector4 localOnFoot[5] = { 0 };
	vector4 localInVeh[5] = { 0 };
	vector4 netPlayer[5] = { 0 };

	//SET CAR ON GROUND PROPERLY? think it crashes you if your not rendered..
	localOnFoot[0].x = 2273.6958f; localOnFoot[0].y = 840.9515f; localOnFoot[0].z = 8.7964f; localOnFoot[0].h = 145.0f; //Hangar (on foot)
	localOnFoot[1].x = 2340.8933f; localOnFoot[1].y = 361.6059f; localOnFoot[1].z = 6.0852f; localOnFoot[1].h = 91.1919f; //Pick-up/Drop-off (on foot)
	localOnFoot[2].x = 2645.3364f; localOnFoot[2].y = 396.5244f; localOnFoot[2].z = 6.7861f; localOnFoot[2].h = 90.0f; //Control tower (on foot)
	localOnFoot[3].x = 2171.5681f; localOnFoot[3].y = 449.1583f; localOnFoot[3].z = 6.0852f; localOnFoot[3].h = 270.0f; //Police station (on foot)
	localOnFoot[4].x = 2320.7332f; localOnFoot[4].y = 38.1777f; localOnFoot[4].z = 5.8142f; localOnFoot[4].h = 0.0f; //Radio tower (on foot)

	localInVeh[0].x = 2280.3030f; localInVeh[0].y = 835.8983f; localInVeh[0].z = 5.7748f; localInVeh[0].h = 137.6057f; //Hangar (in vehicle)
	localInVeh[1].x = 2335.2993f; localInVeh[1].y = 419.0698f; localInVeh[1].z = 5.2958f; localInVeh[1].h = 6.7754f; //Pick-up/Drop-off (in vehicle)
	localInVeh[2].x = 2660.2839f; localInVeh[2].y = 414.2696f; localInVeh[2].z = 5.8299f; localInVeh[2].h = 0.0f; //Control tower(in vehicle)
	localInVeh[3].x = 2167.5549f; localInVeh[3].y = 491.1136f; localInVeh[3].z = 6.0763f; localInVeh[3].h = 270.0f; //Police station (in vehicle)
	localInVeh[4].x = 2321.1509f; localInVeh[4].y = 15.6989f; localInVeh[4].z = 5.8304f; localInVeh[4].h = 270.0f; //Radio tower (in vehicle)

	netPlayer[0].x = 2262.6887f; netPlayer[0].y = 842.0259f; netPlayer[0].z = 5.7739f; netPlayer[0].h = 135.6093f; //Hangar (net player)
	netPlayer[1].x = 2334.3018f; netPlayer[1].y = 399.2935f; netPlayer[1].z = 5.9199f; netPlayer[1].h = 1.2715f; //Pick-up / Drop-off (net player)
	netPlayer[2].x = 2624.2061f; netPlayer[2].y = 394.2089; netPlayer[2].z = 5.8309; netPlayer[2].h = 90.0f; //Control tower (net player)
	netPlayer[3].x = 2169.8616f; netPlayer[3].y = 420.4823f; netPlayer[3].z = 6.2590f; netPlayer[3].h = 0.0f; //Police Station (net player)
	netPlayer[4].x = 2319.4033f; netPlayer[4].y = 54.9634f; netPlayer[4].z = 5.8330f; netPlayer[4].h = 270.0f; //Radio tower (net player)

	if (GetMenuPlayerId() == myPlayerIndex)
	{
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			NETWORK_INDEX netId;
			PED_INDEX driver;

			GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &netId);
			GET_DRIVER_OF_CAR(GetPlayerVeh(), &driver);

			if (driver == myPlayerPed || RequestNetworkControl(netId, VehiclesModelName(GetPlayerVeh())))
				TeleportLocalPlayerInVehicle(localInVeh[index].x, localInVeh[index].y, localInVeh[index].z, localInVeh[index].h);
			else //not driving & can't get control
				TeleportLocalPlayerOnFoot(localOnFoot[index].x, localOnFoot[index].y, localOnFoot[index].z, localOnFoot[index].h); //will warp out if in vehicle.
		}
		else //we're on foot.
			TeleportLocalPlayerOnFoot(localOnFoot[index].x, localOnFoot[index].y, localOnFoot[index].z, localOnFoot[index].h);
	}
	else //Network Player
		TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), netPlayer[index].x, netPlayer[index].y, netPlayer[index].z, netPlayer[index].h);
}

//leave this as is fuck it.. mess but it works..
void teleOptsHappinessIsland()
{
	if (GetMenuPlayerId() == myPlayerIndex)
	{
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			PED_INDEX driver;

			GET_DRIVER_OF_CAR(GetPlayerVeh(), &driver);

			if (driver == myPlayerPed)
			{
				if (teleHappySelect == 0) //Docks (Right)
				{
					TeleportLocalPlayerInVehicle(-409.3407f, -980.3322f, 3.4544f, 0.0f);
					return;
				}
				if (teleHappySelect == 1) //Flag (circle in middle)
				{
					TeleportLocalPlayerInVehicle(-649.2365f, -953.3941f, 4.6002f, 270.0f);
					return;
				}
				if (teleHappySelect == 2) //Hot Coffee!
				{
					TeleportLocalPlayerInVehicle(-649.2365f, -953.3941f, 4.6002f, 270.0f);
					printf("~r~CTL: ~w~You don't wanna do that %s, ~w~here.", NetworkPlayerGetNameWithColour(myPlayerIndex));
					return;
				}
				if (teleHappySelect == 3) //Docks (Left)
				{
					TeleportLocalPlayerInVehicle(-802.1412f, -750.1516f, 4.8581f, 0.0f);
					return;
				}
				if (teleHappySelect == 4) //Heart of LC
				{
					printf("~r~CTL: ~w~That won't end well.. %s, ~w~try on foot.", NetworkPlayerGetNameWithColour(myPlayerIndex));
					return;
				}
			}
			else
			{
				//request control of vehicle.. if fail warp.
				NETWORK_INDEX netId;

				GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &netId);

				if (RequestNetworkControl(netId, VehiclesModelName(GetPlayerVeh())))
				{
					if (teleHappySelect == 0) //Docks (Right)
					{
						TeleportLocalPlayerInVehicle(-409.3407f, -980.3322f, 3.4544f, 0.0f);
						return;
					}
					if (teleHappySelect == 1) //Flag (circle in middle)
					{
						TeleportLocalPlayerInVehicle(-649.2365f, -953.3941f, 4.6002f, 270.0f);
						return;
					}
					if (teleHappySelect == 2) //Hot Coffee!
					{
						TeleportLocalPlayerInVehicle(-649.2365f, -953.3941f, 4.6002f, 270.0f);
						printf("~r~CTL: ~w~You don't wanna do that %s, ~w~here.", NetworkPlayerGetNameWithColour(myPlayerIndex));
						return;
					}
					if (teleHappySelect == 3) //Docks (Left)
					{
						TeleportLocalPlayerInVehicle(-802.1412f, -750.1516f, 4.8581f, 0.0f);
						return;
					}
					if (teleHappySelect == 4) //Heart of LC
					{
						printf("~r~CTL: ~w~That won't end well.. %s, ~w~try on foot.", NetworkPlayerGetNameWithColour(myPlayerIndex));
						return;
						//and yourself..
					}
				}
				else //no net control, just go on foot/warp out.
				{
					if (teleHappySelect == 0) //Docks (Right)
						TeleportLocalPlayerOnFoot(-402.8266f, -991.0560f, 4.0401f, 90.0f);
					
					if (teleHappySelect == 1) //Flag (circle in middle)
					{
						int rand;

						GENERATE_RANDOM_INT_IN_RANGE(0, 5, &rand);

						switch (rand)
						{
							case 0:
							{
								TeleportLocalPlayerOnFoot(-647.2728f, -972.2892f, 5.7674f, 290.0f);
								return;
							}
							break;
							case 1:
							{
								TeleportLocalPlayerOnFoot(-643.4442f, -979.8593f, 5.7674f, 310.0f);
								return;
							}
							break;
							case 2:
							{
								TeleportLocalPlayerOnFoot(-616.8629f, -996.2561, 5.7674f, 352.0f);
								return;
							}
							break;
							case 3:
							{
								TeleportLocalPlayerOnFoot(-608.2029f, -996.3854f, 5.7674f, 355.91f);
								return;
							}
							break;
							case 4:
							{
								TeleportLocalPlayerOnFoot(-588.5614f, -989.0712f, 5.7674f, 17.46f);
								return;
							}
							break;
						}
					}

					if (teleHappySelect == 2) //Hot Coffee!
						TeleportLocalPlayerOnFoot(-605.4880f, -749.6970f, 93.6072f, 4.75550f);
					
					if (teleHappySelect == 3) //Docks (Left)
						TeleportLocalPlayerOnFoot(-807.5168f, -755.8574f, 5.2459f, 0.1f);
					
					if (teleHappySelect == 4) //Heart of LC
						TeleportLocalPlayerOnFoot(-609.5836f, -755.6691f, 65.9908f, 14.0852f);
					
				}
			}
		}
		else //local player on foot
		{
			if (teleHappySelect == 0) //Docks (Right)
				TeleportLocalPlayerOnFoot(-402.8266f, -991.0560f, 4.0401f, 90.0f);
			
			if (teleHappySelect == 1) //Flag (circle in middle)
			{
				int rand;

				GENERATE_RANDOM_INT_IN_RANGE(0, 5, &rand);

				switch (rand)
				{
					case 0:
					{
						TeleportLocalPlayerOnFoot(-647.2728f, -972.2892f, 5.7674f, 290.0f);
						return;
					}
					break;
					case 1:
					{
						TeleportLocalPlayerOnFoot(-643.4442f, -979.8593f, 5.7674f, 310.0f);
						return;
					}
					break;
					case 2:
					{
						TeleportLocalPlayerOnFoot(-616.8629f, -996.2561, 5.7674f, 352.0f);
						return;
					}
					break;
					case 3:
					{
						TeleportLocalPlayerOnFoot(-608.2029f, -996.3854f, 5.7674f, 355.91f);
						return;
					}
					break;
					case 4:
					{
						TeleportLocalPlayerOnFoot(-588.5614f, -989.0712f, 5.7674f, 17.46f);
						return;
					}
					break;
				}
			}

			if (teleHappySelect == 2) //Hot Coffee!
				TeleportLocalPlayerOnFoot(-605.4880f, -749.6970f, 93.6072f, 4.75550f);
			
			if (teleHappySelect == 3) //Docks (Left)
				TeleportLocalPlayerOnFoot(-807.5168f, -755.8574f, 5.2459f, 0.1f);
			
			if (teleHappySelect == 4) //Heart of LC
				TeleportLocalPlayerOnFoot(-609.5836f, -755.6691f, 65.9908f, 14.0852f);
			
		}

	}
	else
	{
		if (teleHappySelect == 0) //Docks
			TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), -411.1671f, -962.8109f, 3.4630f, 33.8967f);
		
		if (teleHappySelect == 1) //Flag (circle in middle)
			TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), -611.1614f, -980.5181f, 4.6002f, 0.0f);
		
		if (teleHappySelect == 2) //Hot Coffee mmmmmaybe if on foot..
		{
			printf("~r~CTL: ~w~Just no...");
			return;
		}

		if (teleHappySelect == 3) //Docks (Left)
		{
			TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), -771.5511f, -743.6017f, 4.8586f, 270.0f);
			return;
		}

		if (teleHappySelect == 4) //Heart of LC
		{
			if (IsPlayerValid(GetMenuPlayerId()) && PLAYER_HAS_CHAR(GetMenuPlayerId()))
			{
				PED_INDEX netPed;

				GET_PLAYER_CHAR(GetMenuPlayerId(), &netPed);

				if (IS_CHAR_IN_ANY_CAR(netPed))
				{
					VEHICLE_INDEX netVeh;
					uint netVehModel;

					GET_CAR_CHAR_IS_USING(netPed, &netVeh);
					GET_CAR_MODEL(netVeh, &netVehModel);
					printf("~r~CTL: %s's ~w~%s would get messed up by collisions.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()), VehiclesModelName(netVehModel));
					return;
				}

				TeleportNetworkPlayerOnFoot(GetMenuPlayerId(), -606.4880f, -753.1838f, 65.9908f, 82.8697f, true);
			}
		}
	}
}

void teleOptsPrison() //use this as a template in future..
{
	const int index = telePrisonSelect; //makes duplicating this function easier..
	vector4 localOnFoot[4] = { 0 };
	vector4 localInVeh[4] = { 0 };
	vector4 netPlayer[4] = { 0 };

	localOnFoot[0].x = -1166.7988f; localOnFoot[0].y = -452.6568f; localOnFoot[0].z = 3.1840f; localOnFoot[0].h = 270.0f; //Car Park (on foot)
	localOnFoot[1].x = -1051.5090f; localOnFoot[1].y = -480.4315f; localOnFoot[1].z = 2.2623f; localOnFoot[1].h = 51.0f; //Cage (on foot)
	localOnFoot[2].x = -1051.5090f; localOnFoot[2].y = -480.4315f; localOnFoot[2].z = 9.3001f; localOnFoot[2].h = 51.0f; //Cage top (on foot)
	localOnFoot[3].x = -978.1655f;  localOnFoot[3].y = -394.1946f; localOnFoot[3].z = 13.5032f; localOnFoot[3].h = 275.0f; //Guard tower (on foot)

	localInVeh[0].x = -1153.2035f; localInVeh[0].y = -479.9334f; localInVeh[0].z = 2.7250f; localInVeh[0].h = 309.0f; //Car park (in vehicle)
	localInVeh[1].x = -1055.7432f; localInVeh[1].y = -443.9141f; localInVeh[1].z = 2.1355f; localInVeh[1].h = 139.9466f; //Cage (in vehicle)
	localInVeh[2].x = -1055.7432f; localInVeh[2].y = -443.9141f; localInVeh[2].z = 9.3001f; localInVeh[2].h = 139.9466f; //Cage top (in vehicle)
	localInVeh[3].x = -959.5445f; localInVeh[3].y = -385.9876f; localInVeh[3].z = 3.7706f; localInVeh[3].h = 110.2118f; //Guard tower (in vehicle)

	netPlayer[0].x = -1118.4806f; netPlayer[0].y = -480.0438f; netPlayer[0].z = 2.6538f; netPlayer[0].h = 49.0f; //Car park (net player)
	netPlayer[1].x = -1073.7872f; netPlayer[1].y = -460.0159f; netPlayer[1].z = 2.2623f; netPlayer[1].h = 0.0f; //Cage (net player)
	netPlayer[2].x = -1073.7872f; netPlayer[2].y = -460.0159f; netPlayer[2].z = 9.3001f; netPlayer[2].h = 0.0f; //Cage top (net player)
	netPlayer[3].x = -964.8743f; netPlayer[3].y = -404.5859f; netPlayer[3].z = 3.6641f; netPlayer[3].h = 68.1227f; //Guard tower (net player)

	if (GetMenuPlayerId() == myPlayerIndex)
	{
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			NETWORK_INDEX netId;
			PED_INDEX driver;

			GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &netId);
			GET_DRIVER_OF_CAR(GetPlayerVeh(), &driver);

			if (driver == myPlayerPed || RequestNetworkControl(netId, VehiclesModelName(GetPlayerVeh())))
				TeleportLocalPlayerInVehicle(localInVeh[index].x, localInVeh[index].y, localInVeh[index].z, localInVeh[index].h);
			else //not driving & can't get control
				TeleportLocalPlayerOnFoot(localOnFoot[index].x, localOnFoot[index].y, localOnFoot[index].z, localOnFoot[index].h); //will warp out.
		}
		else //we're on foot.
			TeleportLocalPlayerOnFoot(localOnFoot[index].x, localOnFoot[index].y, localOnFoot[index].z, localOnFoot[index].h); //will warp out.
	}
	else //Network Player
		TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), netPlayer[index].x, netPlayer[index].y, netPlayer[index].z, netPlayer[index].h);
}

void teleOptsStripclubs() //use this as a template in future..
{
	const int index = teleStripclubSelect; //makes duplicating this function easier..
	vector4 localOnFoot[5] = { 0 };
	vector4 localInVeh[5] = { 0 };
	vector4 netPlayer[5] = { 0 };

	localOnFoot[0].x = 1201.6127f; localOnFoot[0].y = 1725.3087f; localOnFoot[0].z = 19.1062f; localOnFoot[0].h = 186.6222f; //Triangle Club Ship Container (on foot)
	localOnFoot[1].x = -1551.9777f; localOnFoot[1].y = 30.6922f; localOnFoot[1].z = 11.9478f; localOnFoot[1].h = 128.3379f; //Honkers (On Wall)

	localInVeh[0].x = 1220.6604f; localInVeh[0].y = 1719.2008f; localInVeh[0].z = 16.4579f; localInVeh[0].h = 121.3421f; //Triangle Club car park(in vehicle) //clear area of cars??
	localInVeh[1].x = -1608.1487f; localInVeh[1].y = -27.6763; localInVeh[1].z = 9.785f; localInVeh[1].h = 317.760; //Honkers (Facing sign in car park)

	netPlayer[0].x = 1224.7180f; netPlayer[0].y = 1693.7327f; netPlayer[0].z = 16.3463f; netPlayer[0].h = 15.2206f; //Triangle Club car park 
	netPlayer[1].x = -1609.718f; netPlayer[1].y = -3.734f; netPlayer[1].z = 9.859f; netPlayer[1].h = 324.6670f; //Honkers (Facing oter sign)

	if (GetMenuPlayerId() == myPlayerIndex)
	{
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			NETWORK_INDEX netId;
			PED_INDEX driver;

			GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &netId);
			GET_DRIVER_OF_CAR(GetPlayerVeh(), &driver);

			if (driver == myPlayerPed || RequestNetworkControl(netId, VehiclesModelName(GetPlayerVeh())))
				TeleportLocalPlayerInVehicle(localInVeh[index].x, localInVeh[index].y, localInVeh[index].z, localInVeh[index].h);
			else //not driving & can't get control
				TeleportLocalPlayerOnFoot(localOnFoot[index].x, localOnFoot[index].y, localOnFoot[index].z, localOnFoot[index].h); //will warp out if in vehicle.
		}
		else //we're on foot.
			TeleportLocalPlayerOnFoot(localOnFoot[index].x, localOnFoot[index].y, localOnFoot[index].z, localOnFoot[index].h);
	}
	else //Network Player
		TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), netPlayer[index].x, netPlayer[index].y, netPlayer[index].z, netPlayer[index].h);
}

void teleOptsHoveBeach()
{
	if (GetMenuPlayerId() == myPlayerIndex)
	{
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			NETWORK_INDEX netId;
			PED_INDEX driver;

			GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &netId);
			GET_DRIVER_OF_CAR(GetPlayerVeh(), &driver);

			if (driver == myPlayerPed || RequestNetworkControl(netId, VehiclesModelName(GetPlayerVeh())))
				TeleportLocalPlayerInVehicle(1016.8945f, -720.9865f, 14.2565f, 270.0f);
			else
				TeleportLocalPlayerOnFoot(1010.0518f, -712.2844f, 16.6436f, 180.0f); //will warp out if in vehicle
		}
		else
			TeleportLocalPlayerOnFoot(1010.0518f, -712.2844f, 16.6436f, 180.0f); //will warp out if in vehicle
	}
	else
		TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), 964.5299f, -706.9893f, 15.3267f, 180.0f);
}

int getLocalTeleportType()
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		NETWORK_INDEX netId;
		PED_INDEX driver;

		GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &netId);
		GET_DRIVER_OF_CAR(GetPlayerVeh(), &driver);

		if (driver == myPlayerPed || RequestNetworkControl(netId, VehiclesModelName(GetPlayerVeh())))
			return 2; //in vehicle
	}

	return 1; //on foot
}

void teleOptWaypoint()
{
	vector3 waypos;
	BLIP_INDEX waypoint = GET_FIRST_BLIP_INFO_ID(BLIP_WAYPOINT);

	if (DOES_BLIP_EXIST(waypoint))
	{
		float ground, myHeading;

		GET_BLIP_COORDS(waypoint, &waypos);
		GET_GROUND_Z_FOR_3D_COORD(waypos.x, waypos.y, 1000.0f, &ground);
		GET_CHAR_HEADING(myPlayerPed, &myHeading);

		if (GetMenuPlayerId() == myPlayerIndex)
		{
			int TeleportType = getLocalTeleportType();

			if (TeleportType == 1) //Yes teleporting twice is necessary, as groundZ can not be calculated at too far a distance.
				TeleportLocalPlayerOnFoot(waypos.x, waypos.y, ground, myHeading);
			else
				TeleportLocalPlayerInVehicle(waypos.x, waypos.y, ground, myHeading); 

			REQUEST_COLLISION_AT_POSN(waypos.x, waypos.y, ground); //halfs the loading time.

			if (ground == 0)
			{
				while (ground == 0) //returns flat 0 when out of distance..
				{
					GET_GROUND_Z_FOR_3D_COORD(waypos.x, waypos.y, 1000.0f, &ground);
					ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
					printf("~r~CTL: ~w~Obtaining ground height.."); //debug
					WAIT(0);
				}

				CLEAR_PRINTS();
			}

			if (TeleportType == 1)
				TeleportLocalPlayerOnFoot(waypos.x, waypos.y, (ground + 1.0f), myHeading); //+ 1 because uses no_offset which is literally just -1.0f
			else
			{
				TeleportLocalPlayerInVehicle(waypos.x, waypos.y, ground, myHeading);
				SET_CAR_ON_GROUND_PROPERLY(GetPlayerVeh());  //seems to work okay as ground height is a good sign of the area getting loaded it seems.
			}
		}
		else
		{																		//if you hit z -100.0f game will put you on top of the map
			if (TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), waypos.x, waypos.y, ((ground == 0) ? -98.0f : ground), myHeading)) //lil hidden easter egg with the heading.
				printf("~r~CTL: ~w~Teleported %s ~w~to ~y~waypoint.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
			//beward the infinite fall glitch.. in vehicles with a passenger i think, or if a lone passenger..
		}
	}
	else
		printf("~r~CTL: ~w~You need to set a ~y~waypoint ~w~on the map.");
}

void saveLocationTeleport()
{
	GET_CHAR_COORDINATES(myPlayerPed, &savedDest.x, &savedDest.y, &savedDest.z);
	GET_CHAR_HEADING(myPlayerPed, &savedDest.h);
}

void readLocationTeleport()
{
	if (savedDest.z == -1337.0f)
	{
		printf("~r~CTL: ~w~You need to save a location first.");
		return;
	}

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		TeleportLocalPlayerInVehicle(savedDest.x, savedDest.y, savedDest.z, savedDest.h);
	else
		TeleportLocalPlayerOnFoot(savedDest.x, savedDest.y, savedDest.z, savedDest.h);
}

void TeleportSelectedPlayerIdToNetPlayer(PLAYER_INDEX targetPlayer)
{
	float dX, dY, dZ, dH;
	PED_INDEX targetPed;

	if (IsPlayerValid(targetPlayer) && PLAYER_HAS_CHAR(targetPlayer))
	{
		GET_PLAYER_CHAR(targetPlayer, &targetPed);
		GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(targetPed, 3.0f, 3.0f, 1.0f, &dX, &dY, &dZ);

		//3 in front and 3 to right of target..
		//heading will want to be.. - 90 + - 45 (maybe a tad less we'll see).
		GET_CHAR_HEADING(targetPed, &dH);
		dH -= 135.0f;

		if (TeleportNetworkPlayerToCoordinate(TeleportSelectedPlayerId, dX, dY, dZ, dH))
		{
			char* buffer = BUFFER128;

			if (!IsPlayerValid(TeleportSelectedPlayerId) || !IsPlayerValid(targetPlayer))
			{
				printf("~r~CTL: ~w~One of the players left, cancelled to avoid freeze.");
				return;
			}
			
			//Don't use sprintf here as it displays the same name twice (same stack pointer i guess?)
			strcpy(buffer, "~r~CTL: ~w~Teleported ", 128);
			stradd(buffer, NetworkPlayerGetNameWithColour(TeleportSelectedPlayerId), 128);
			stradd(buffer, " ~w~to ", 128);
			stradd(buffer, NetworkPlayerGetNameWithColour(targetPlayer), 128);
			stradd(buffer, ".", 128);
			printf(buffer);
		}
	}
	else
		printf("~r~CTL: ~w~Unable to obtain player co-ordinates, ghost or they left.");
}

void teleOptNetPlayerToNetPlayer() //works
{
	//need to stop GetMenuPlayerId() from updating..
	SetHeaderForwarded();

	for (int i = 0; i < 16; i++)
	{
		if (i == myPlayerIndex || i == GetMenuPlayerId()) 
			continue;

		if (IsPlayerValid(i))
			AddItemWithParam((char*)GET_PLAYER_NAME(i), i, TeleportSelectedPlayerIdToNetPlayer, "Teleport player to this player.");
	}
}

void teleToCartelSpawn()
{
	float x, y, z, h;

	GetCartelSpawnCoordinatesForPlayer(myPlayerIndex, &x, &y, &z, &h);
	TeleportLocalPlayerOnFoot(x, y, z, h);
}


void teleOptBackToLand()
{
	if (PLAYER_HAS_CHAR(GetMenuPlayerId()))
	{
		float pX, pY, pZ, pH;
		PED_INDEX SelectedPlayerPed;
		GET_PLAYER_CHAR(GetMenuPlayerId(), &SelectedPlayerPed);
		if (COMPARE_STRING(GET_NAME_OF_ZONE(pX, pY, pZ), "LIBERTY")) //out at sea
		{
			GET_CHAR_HEADING(SelectedPlayerPed, &pH);
			float destX, destY, destZ;
			if (GET_CLOSEST_CAR_NODE(pX, pY, pZ, &destX, &destY, &destZ))
			{
				if (!IS_CHAR_IN_ANY_BOAT(SelectedPlayerPed) && !IS_CHAR_IN_ANY_HELI(SelectedPlayerPed))
				{
					if (TeleportNetworkPlayerToCoordinate(GetMenuPlayerId(), destX, destY, destZ, pH))
					{
						ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
						printf("~r~CTL: ~w~Teleported %s ~w~back to shore.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
					}
				}
				else
					printf("~r~CTL: %s ~w~is in a helicopter or boat, not teleporting.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
			}
			else
				printf("~r~CTL: ~w~Unable to get coordinates for teleporting %s.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
		}
	}
	else
		printf("~r~CTL: %s ~w~is not that far out at sea, not teleported.", NetworkPlayerGetNameWithColour(GetMenuPlayerId()));
}

void TeleportOptions()
{
	static char* TeleStringsAirport[5] = { "Hangar", "Pick-up/Drop-off", "Control tower", "Police station", "Radio tower" };
	static char* TeleStringsHappyIsland[5] = { "Docks (Right)", "Flag (middle)", "Hot Coffee!", "Docks (Left)", "Heart of LC" };
	static char* TeleStringsPrison[4] = { "Car park", "Cage", "Cage (on-top)", "Guard tower" };
	static char* TeleStringsStripClubs[3] = { "Triangle Club" , "Honkers", "Unknown" };

	//this needs to work with both Local and network players..
	

	SetHeader("Teleport locations");
	if (GetMenuPlayerId() == myPlayerIndex)
	{
		AddItem("Cartel HQ", teleToCartelSpawn, "Will remove you from your vehicle."); //need to figure out how to properly handle when in a vehicle etc.
		AddItem("Into nearest vehicle", teleOptsNearestVeh, "Yes, you can join the unsuspecting peds journey.");
	}
	else
	{
		AddItemMenu("Another player", teleOptNetPlayerToNetPlayer, "Teleport this player to another player.");
		TeleportSelectedPlayerId = GetMenuPlayerId();
	}

	AddItem("Waypoint", teleOptWaypoint, "Waypoint, set on the map.");
	AddItem("Back to land/shore", teleOptBackToLand, "Only works at sea when not using a boat or heli.");
	AddItemChar(ctlMem->m_commonStrings[GXT_STRING_AIRPORT], false, TeleStringsAirport, 5, &teleAirportSelect, teleOptsAirport, nullptr); //"Airport"
	//AddItemChar("Airport", false, TeleStringsAirport, 5, &teleAirportSelect, teleOptsAirport, nullptr);
	AddItemChar("Happiness Island", false, TeleStringsHappyIsland, 5, &teleHappySelect, teleOptsHappinessIsland, nullptr);
	AddItemChar(ctlMem->m_commonStrings[GXT_STRING_PRISON], false, TeleStringsPrison, 4, &telePrisonSelect, teleOptsPrison, nullptr); //"Prison"
	AddItemChar("Strip Clubs", false, TeleStringsStripClubs, 3, &teleStripclubSelect, teleOptsStripclubs, nullptr);
	AddItem("Hove beach", teleOptsHoveBeach, nullptr);
	AddItem("Stored location", readLocationTeleport, "Teleport to the saved location.");

	if(GetMenuPlayerId() == myPlayerIndex)
		AddItem("Store current location", saveLocationTeleport, "Save current location for future teleporting.");
}


