#include "stdafx.h"
#include "Functions.h"
#include "Ped_Options.h"
#include "MenuExecutionHandling.h"
#include "Utils.h"
#include "Network_Options.h"
#include "Admin_Options.h"

int GetRelationshipGroupForPlayer(PLAYER_INDEX target)
{
	if(NetworkGame)
		return (target + RELATIONSHIP_GROUP_NETWORK_PLAYER_01);

	return RELATIONSHIP_GROUP_PLAYER;

//	ALLOW_GANG_RELATIONSHIPS_TO_BE_CHANGED_BY_NEXT_COMMAND
}

VEHICLE_SEAT GetPedSeatIndex(PED_INDEX target_ped)
{
	VEHICLE_INDEX target_veh;

	if (!DOES_CHAR_EXIST(target_ped)) 
		return SEAT_INVALID;

	if (IS_CHAR_IN_ANY_CAR(target_ped))
	{
		PED_INDEX pedInSeat = 0;
		int i;

		GET_CAR_CHAR_IS_USING(target_ped, &target_veh);
		GET_DRIVER_OF_CAR(target_veh, &pedInSeat);

		if (pedInSeat == target_ped)
			return SEAT_DRIVER;
		else
		{
			for (i = 0; i < 3; i++)
			{
				GET_CHAR_IN_CAR_PASSENGER_SEAT(target_veh, i, &pedInSeat);

				if (target_ped == pedInSeat)
					return i;
			}
		}
	}

	return SEAT_INVALID;
}

//Only use the below function if the player is in the interior.
//going well but animations not playing.
bool CreatePed(uint model, float x, float y, float z, float h, PED_INDEX *spawnedPed, bool invincible, bool lockNetControl)
{
	if (!IS_MODEL_IN_CDIMAGE(model))
	{
		printf("~r~CTL: ~w~Ped is missing from your .img files, can't spawn.");
		return false;
	}

	REQUEST_MODEL(model);
	while (!HAS_MODEL_LOADED(model)) 
		WAIT(0);

	CREATE_CHAR(5, model, x, y, z, &*spawnedPed, true);
	if (invincible)
		SET_CHAR_INVINCIBLE(*spawnedPed, true);

	if (lockNetControl)
	{
		NETWORK_INDEX netId;

		GET_NETWORK_ID_FROM_PED(*spawnedPed, &netId);
		SET_NETWORK_ID_CAN_MIGRATE(netId, false);
	}

	MARK_MODEL_AS_NO_LONGER_NEEDED(model);
	return true;
}

void CreateInteriorPed(float x, float y, float z, float h, uint model)
{
	REQUEST_ANIMS("missstripclublo");
	REQUEST_ANIMS("missstripclub");

	if (CreatePed(model, x, y, z, h, &ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count], false, true))
	{
		SET_CHAR_RANDOM_COMPONENT_VARIATION(ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count]);
		SET_CHAR_HEADING(ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count], h);

		//SET_CHAR_ONLY_DAMAGED_BY_PLAYER(ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count], true); //not tested
		//SET_CHAR_WILL_COWER_INSTEAD_OF_FLEEING(ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count], true); //not working
		//SET_CHAR_WILL_MOVE_WHEN_INJURED(ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count], false); //not working //maybe they need to be in group?

		if (model == PED_F_Y_STRIPPERC01 || model == PED_F_Y_STRIPPERC02)
		{
			if (IS_CHAR_MODEL(ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count], PED_F_Y_STRIPPERC01))
				SET_AMBIENT_VOICE_NAME(ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count], "STRIPPER_1");
			else
				SET_AMBIENT_VOICE_NAME(ctlMem->m_interior_peds.ped[ctlMem->m_interior_peds.ped_count], "STRIPPER_2");
		}

		ctlMem->m_interior_peds.ped_count++;
	}
}

PLAYER_INDEX has_interior_been_generated_by_another_cartel_user(int roomKey)
{
	if (roomKey == TRIANGLE_CLUB_MAIN || roomKey == TRIANGLE_CLUB_ENTRANCE || roomKey == TRIANGLE_CLUB_EXITWAY)
	{
		for (int i = 0; i < 16; i++)
		{
			//can prob remove the active checks, iffy if they crash though this will stay the same even after they leave..
			if (!IsPlayerValid(i) || i == myPlayerIndex) 
				continue;
			if (freeMem->m_cartelBroadcastVars.m_Data[i].m_string[18] == 'T') return i;
		}
	}
	if (roomKey == HONKERS_ENTRANCE || roomKey == HONKERS_LOBBY || roomKey == HONKERS_MAIN || roomKey == HONKERS_BACKROOM)
	{
		for (int i = 0; i < 16; i++)
		{
			if (!IsPlayerValid(i) || i == myPlayerIndex) 
				continue;
			if (freeMem->m_cartelBroadcastVars.m_Data[i].m_string[18] == 'H') return i;
		}
	}
	return -1; //invalid playerid.
}


void mark_interior_as_generated(int roomKey)
{
#ifdef Moderator
	//currently responses don't use the broadcast variables, they are merely a clone of your health/armour to confirm receipt.
	if (bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_SEND_IN_PROG) || bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_CMD_RESPONSE_IN_PROG)) return;
#endif
	//only runs in network session
	if (roomKey == 0) //we've left / not in an interior anymore and need to clear out our advertisement saying we've loaded it.
	{
		//COMPARE_STRING probably stops at the first terminator.. which would explain why it's not clearing out / catching it.
		if (freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string[18] == 'H' || freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string[18] == 'T')
		{
			freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string[18] = '0'; //
			setMenuAdvertise();
			SEND_CLIENT_BROADCAST_VARIABLES_NOW(1); //checks above ensure this doesn't get spammed.
		}
		return;
	}

	if (roomKey == TRIANGLE_CLUB_MAIN || roomKey == TRIANGLE_CLUB_ENTRANCE || roomKey == TRIANGLE_CLUB_EXITWAY)
	{
		if (freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string[18] != 'T')
		{
			freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string[18] = 'T';
			SEND_CLIENT_BROADCAST_VARIABLES_NOW(1);
		}
	}
	if (roomKey == HONKERS_ENTRANCE || roomKey == HONKERS_LOBBY || roomKey == HONKERS_MAIN || roomKey == HONKERS_BACKROOM)
	{
		if (freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string[18] != 'H')
		{
			freeMem->m_cartelBroadcastVars.m_Data[myPlayerIndex].m_string[18] = 'H';
			SEND_CLIENT_BROADCAST_VARIABLES_NOW(1);
		}
	}
}


void LivenMPInteriorsLoop()
{
	if (!NetworkGame)
		return;

	NETWORK_INDEX NetworkId;
	if (IS_INTERIOR_SCENE())
	{
		uint localRoomKey;

		GET_KEY_FOR_CHAR_IN_ROOM(myPlayerPed, &localRoomKey); //0 when outside.. //could be easy way to detect others.

		if (localRoomKey == TRIANGLE_CLUB_MAIN || localRoomKey == TRIANGLE_CLUB_ENTRANCE || localRoomKey == TRIANGLE_CLUB_EXITWAY)
		{
			PLAYER_INDEX generated_by = has_interior_been_generated_by_another_cartel_user(TRIANGLE_CLUB_MAIN);
			if (generated_by != INVALID_PLAYER_INDEX)
			{
				//printf("~r~CTL: ~w~Player %s ~w~has this interior taken care of, returning.", NetworkPlayerGetNameWithColour(generated_by));
				return;
			}
			mark_interior_as_generated(TRIANGLE_CLUB_MAIN);

			if (DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[0]) || DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[1])) //we are in the triangle club and ped exists
			{
				uint pedKeyOne = TRIANGLE_CLUB_MAIN;
				uint pedKeyTwo = TRIANGLE_CLUB_MAIN;

				if(DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[0]))
					GET_KEY_FOR_CHAR_IN_ROOM(ctlMem->m_interior_peds.ped[0], &pedKeyOne);

				if(DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[1]))
					GET_KEY_FOR_CHAR_IN_ROOM(ctlMem->m_interior_peds.ped[1], &pedKeyTwo);

				if (pedKeyOne != TRIANGLE_CLUB_MAIN || pedKeyTwo != TRIANGLE_CLUB_MAIN) //we are in the triangle club but the peds aren't.. (User telepored from inside an interior to another interior)
				{
					for (int i = 0; i < MAX_INTERIOR_PEDS; i++)
					{
						if (DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[i]))
							DeleteChar(&ctlMem->m_interior_peds.ped[i]); //re-spawn in triangle club..
					}
					ctlMem->m_interior_peds.ped_count = 0;
				}
			}
			else
			{
				printf("~r~CTL: ~w~Generating Triangle Club entities...");
				CreateInteriorPed(1173.8314, 1693.0646, 16.7270, 355.5881, PED_F_Y_STRIPPERC01); 
				CreateInteriorPed(1177.7380, 1680.9800, 16.7270, 223.7068, PED_F_Y_STRIPPERC02);
				ctlMem->m_interior_peds.interior_key = TRIANGLE_CLUB_MAIN;
				while (!HAVE_ANIMS_LOADED("missstripclublo") || !HAVE_ANIMS_LOADED("missstripclub")) 
					WAIT(0);

				for (int i = 0; i < 2; i++)
					SET_ROOM_FOR_CHAR_BY_KEY(ctlMem->m_interior_peds.ped[i], TRIANGLE_CLUB_MAIN); //Triangle Club
				
				TASK_PLAY_ANIM_WITH_ADVANCED_FLAGS(ctlMem->m_interior_peds.ped[0], "pole_dance_a", "missstripclublo", 8.0f, true, false, false, false, false, false, false, -1);
				TASK_PLAY_ANIM_WITH_ADVANCED_FLAGS(ctlMem->m_interior_peds.ped[1], "Dance_Rick", "missstripclub", 8.0f, true, false, false, false, false, false, false, -1); //works
				REMOVE_ANIMS("missstripclublo");
				REMOVE_ANIMS("missstripclub");
				CLEAR_PRINTS();
			}
		}

		if (localRoomKey == HONKERS_ENTRANCE || localRoomKey == HONKERS_LOBBY || localRoomKey == HONKERS_MAIN || localRoomKey == HONKERS_BACKROOM)
		{
			PLAYER_INDEX generated_by = has_interior_been_generated_by_another_cartel_user(HONKERS_MAIN);
			if (generated_by != INVALID_PLAYER_INDEX)
			{
				//printf("~r~CTL: ~w~Player %s ~w~has this interior taken care of, returning.", NetworkPlayerGetNameWithColour(generated_by));
				return;
			}
			mark_interior_as_generated(HONKERS_MAIN);

			if (DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[0]) || DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[1])) //we are in honkers and ped exists 
			{
				uint pedKeyOne = HONKERS_MAIN;
				uint pedKeyTwo = HONKERS_MAIN;

				if (DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[0]))
					GET_KEY_FOR_CHAR_IN_ROOM(ctlMem->m_interior_peds.ped[0], &pedKeyOne);

				if (DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[1]))
					GET_KEY_FOR_CHAR_IN_ROOM(ctlMem->m_interior_peds.ped[1], &pedKeyTwo);

				if (pedKeyOne != HONKERS_MAIN || pedKeyTwo != HONKERS_MAIN) ///we are in Honkers club but the peds aren't.. (User telepored from inside an interior to another interior)
				{
					for (int i = 0; i < MAX_INTERIOR_PEDS; i++)
						if (DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[i]))
							DeleteChar(&ctlMem->m_interior_peds.ped[i]); //re-spawn in Honkers

					ctlMem->m_interior_peds.ped_count = 0;
				}
			}
			else
			{
				CreateInteriorPed(-1572.6274f, 10.7402f, 11.1043f, 92.7613f, PED_F_Y_STRIPPERC01);
				CreateInteriorPed(-1560.8610f, 10.7447f, 11.1043f, 269.8597f, PED_F_Y_STRIPPERC02);
				ctlMem->m_interior_peds.interior_key = HONKERS_MAIN;
				printf("~r~CTL: ~w~Generating Honkers entities...");

				while (!HAVE_ANIMS_LOADED("missstripclublo") || !HAVE_ANIMS_LOADED("missstripclub")) 
					WAIT(0);

				for (int i = 0; i < 2; i++)
					SET_ROOM_FOR_CHAR_BY_KEY(ctlMem->m_interior_peds.ped[i], HONKERS_MAIN); 
				
				TASK_PLAY_ANIM_WITH_ADVANCED_FLAGS(ctlMem->m_interior_peds.ped[0], "pole_dance_a", "missstripclublo", 8.0f, true, false, false, false, false, false, false, -1);
				TASK_PLAY_ANIM_WITH_ADVANCED_FLAGS(ctlMem->m_interior_peds.ped[1], "Dance_Rick", "missstripclub", 8.0f, true, false, false, false, false, false, false, -1); //works
				REMOVE_ANIMS("missstripclublo");
				REMOVE_ANIMS("missstripclub");
				CLEAR_PRINTS();
			}
		}
	}
	else
	{
		//not inside interior.
		for (int i = 0; i < MAX_INTERIOR_PEDS; i++)
		{
			if (DOES_CHAR_EXIST(ctlMem->m_interior_peds.ped[i]))
			{
				GET_NETWORK_ID_FROM_PED(ctlMem->m_interior_peds.ped[i], &NetworkId);
				ReleaseControlOfNetworkId(NetworkId);
				MARK_CHAR_AS_NO_LONGER_NEEDED(&ctlMem->m_interior_peds.ped[i]);
			}
		}
		ctlMem->m_interior_peds.ped_count = 0;
		ctlMem->m_interior_peds.interior_key = 0;
		mark_interior_as_generated(0); //should clear it out. //contains checks to ensure no network spam occurs.
	}
}
