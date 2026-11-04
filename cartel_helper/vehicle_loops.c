#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"
#include "cartel_chat.h"
#include "Functions.h"
#include "vehicle_loops.h"
#include "../Menu/Functions.h"
#include "../Menu/Admin_Options.h"
#include "../Menu/maths.c"

#define MAX_INPUT 128.0f
#define DEADZONE_PERCENT 0.1f

VEHICLE_INDEX lastVeh;
void cartel_vehicle_loops()
{
	VEHICLE_INDEX my_vehicle;
	PED_INDEX vehicle_driver;

	static uint cycleCarColoursNetworkDelayTick;
	NETWORK_INDEX NetworkId;
	GET_CAR_CHAR_IS_USING(myPlayerPed, &my_vehicle);

	if (DOES_VEHICLE_EXIST(lastVeh))
	{
		if (my_vehicle != lastVeh) //make sure it's not our current vehicle if we're in one.
		{ //will also be true it we're on foot.
			//we should do this always regrdless of protection status... as some menus break things and don't do this..
			GET_NETWORK_ID_FROM_VEHICLE(lastVeh, &NetworkId);
			if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(NetworkId))
			{
				if (HAS_CONTROL_OF_NETWORK_ID(NetworkId))
				{				
					if (lastVeh != ctlMem->m_autoBoat)// && lastVeh != ctlMem->m_chauffeur.vehicle)
					{
						//probs need to check this isn't our autoboat.
						//SET_CAR_AS_MISSION_CAR(lastVeh); //prevents despawning when nearby. / wont delete until we get in a new car seemingly / perfect
						//MARK_CAR_AS_NO_LONGER_NEEDED(&lastVeh); //make sure the last vehicle de-spawn / mark for cleanup
#ifdef Developer
						PRINTSTRING("set lastVeh migratable as we've ditched it.\n");
#endif
						FREEZE_CAR_POSITION(lastVeh, false);
						SET_NETWORK_ID_CAN_MIGRATE(NetworkId, true); //make sure we don't hoard control of network id's
						lastVeh = 0;
					}
					//if (DOES_VEHICLE_EXIST(my_vehicle)) //if we're in a new vehicle and it's not a funny car.
					//	lastVeh = my_vehicle;
				}
			}
		}
	}

	if (DOES_VEHICLE_EXIST(my_vehicle))
	{
		GET_NETWORK_ID_FROM_VEHICLE(my_vehicle, &NetworkId);
		GET_DRIVER_OF_CAR(my_vehicle, &vehicle_driver);

		if (vehicle_driver == myPlayerPed) //we are driving
		{
#pragma region vehicle_network_control
			if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(NetworkId))
			{
				if (HAS_CONTROL_OF_NETWORK_ID(NetworkId))
				{
					if (bitTest(ctlMem->m_bit_sets.m_vehicle, blockVehicleNetMigration))
						SET_NETWORK_ID_CAN_MIGRATE(NetworkId, false);

					if (bitTest(ctlMem->m_bit_sets.m_weapons, forceNeverTargettedVehicle))
						SET_VEHICLE_CAN_BE_TARGETTED(my_vehicle, false);

					lastVeh = my_vehicle;
				}
				else
				{
					if (!bitTest(ctlMem->m_bit_sets.m_vehicle, printed_vehicle_controller))
					{
						ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
						PLAYER_INDEX controller = FindControllerOfNetworkId(NetworkId);
						if (controller != INVALID_PLAYER_INDEX)
							printf("~r~CTL: %s ~w~has control of this %s.", NetworkPlayerGetNameWithColour(controller), VehiclesModelName(my_vehicle));
						else
							printf("~r~CTL: ~w~Someone locked control of this %s and left the game.", VehiclesModelName(my_vehicle));

						bitSet(&ctlMem->m_bit_sets.m_vehicle, printed_vehicle_controller);
					}
				}
			}
			else
			{
				if (NetworkGame && !bitTest(ctlMem->m_bit_sets.m_vehicle, printed_vehicle_controller))
				{
					printf("~r~CTL: ~w~This %s only exists locally (not on other machines).", VehiclesModelName(my_vehicle));
					bitSet(&ctlMem->m_bit_sets.m_vehicle, printed_vehicle_controller);
				}
			}
#pragma endregion

#pragma region always_clean
			if (bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleAlwaysClean))
			{
				WASH_VEHICLE_TEXTURES(my_vehicle, 100);
				SET_VEHICLE_DIRT_LEVEL(my_vehicle, 0.0f);
			}
#pragma endregion

#pragma region rainbow_car
			if (GET_BITS_IN_RANGE(ctlMem->m_bit_sets.m_vehicle, cycleCarColours, randomCarColours)) //13 and 14
			{
				if (cycleCarColoursNetworkDelayTick < GetTickCount())
				{
					int AmountToChange = bitTest(ctlMem->m_bit_sets.m_vehicle, cycleRandomChangeAllColours) ? 4 : 1;
					int carCols[4];

					GET_CAR_COLOURS(my_vehicle, &carCols[0], &carCols[1]);
					GET_EXTRA_CAR_COLOURS(my_vehicle, &carCols[2], &carCols[3]);

					if (bitTest(ctlMem->m_bit_sets.m_vehicle, randomCarColours))
					{
						for (int i = 0; i < AmountToChange; i++)
							GENERATE_RANDOM_INT_IN_RANGE(0, 133, &carCols[i]);
					}
					else //cycleCarColours bit is true..
					{
						for (int i = 0; i < AmountToChange; i++)
							carCols[i] = carCols[i] < 133 ? carCols[i] + 1 : 0;
					}

					CHANGE_CAR_COLOUR(my_vehicle, carCols[0], carCols[1]);
					SET_EXTRA_CAR_COLOURS(my_vehicle, carCols[2], carCols[3]);

					//brackets are necessary otherwise it fucks up (tested without initially.) - Nath. //was 420 for good measure but meh fuck it..
					cycleCarColoursNetworkDelayTick = (GetTickCount() + (NetworkGame ? 400 : 100)); //250 seems to skip some..
				}
			}
#pragma endregion

#pragma region vehicle_invincible
			if (bitTest(ctlMem->m_bit_sets.m_vehicle, vehicleInvincibleLoop))
			{
				STOP_CAR_BREAKING(my_vehicle, true);
				SET_CAR_CAN_BE_DAMAGED(my_vehicle, false);
				SET_CAR_CAN_BE_VISIBLY_DAMAGED(my_vehicle, false);
				SET_CAR_WATERTIGHT(my_vehicle, true);
				SET_CAR_PROOFS(my_vehicle, true, true, true, true, true);
				SET_CAN_BURST_CAR_TYRES(my_vehicle, false);
				SET_CAR_STRONG(my_vehicle, true);
			}
#pragma endregion

#pragma region flying_car
			if (bitTest(ctlMem->m_bit_sets.m_vehicle, storm_flying_car))
			{
				int LTPressed = GET_CONTROL_VALUE(0, 6);
				int RTPressed = GET_CONTROL_VALUE(0, 4);

				bool isYawLeft = IS_BUTTON_PRESSED(0, BUTTON_LB);
				bool isYawRight = IS_BUTTON_PRESSED(0, BUTTON_RB);

				int leftX, leftY, rightX, rightY;
				GET_POSITION_OF_ANALOGUE_STICKS(0, &leftX, &leftY, &rightX, &rightY);

				float inputPitch = (float)leftY;
				float inputRoll = (float)leftX;
				float inputYaw = 0.0f;

				if (isYawLeft) inputYaw += 64.0f;
				if (isYawRight) inputYaw -= 64.0f;

				float thrustInput = (float)RTPressed / 255.0f;

				float vehicleMass;
				GET_CAR_MASS(my_vehicle, &vehicleMass);

				vector3 speedVector;
				GET_CAR_SPEED_VECTOR(my_vehicle, &speedVector, true);

				float referenceSpeed = 50.0f;
				float rudereffektivität = clamp(speedVector.y / referenceSpeed, 0.2f, 1.0f);

				quaternion vehicleRotation;
				GET_VEHICLE_QUATERNION(my_vehicle, &vehicleRotation.x, &vehicleRotation.y, &vehicleRotation.z, &vehicleRotation.w);
				if (IS_BUTTON_PRESSED(0, BUTTON_A) && speedVector.y > 20.0f)
				{
					APPLY_FORCE_TO_CAR(my_vehicle, true, 0, 0, 23.0, 0, 0, 10, true, true, true, true);
				}

				if (!IS_VEHICLE_ON_ALL_WHEELS(my_vehicle) && speedVector.y > 20.0f)
				{

					float baseRotationSpeed = 0.5f;
					float rotationSpeed = baseRotationSpeed * rudereffektivität;

					vector3 localRight = quaternion_rotate_vector(vehicleRotation, (vector3) { 1.0f, 0.0f, 0.0f });
					vector3 localForward = quaternion_rotate_vector(vehicleRotation, (vector3) { 0.0f, 1.0f, 0.0f });
					vector3 localUp = quaternion_rotate_vector(vehicleRotation, (vector3) { 0.0f, 0.0f, 1.0f });

					quaternion pitchQ = quaternion_from_axis_angle(localRight, inputPitch * rotationSpeed);
					quaternion rollQ = quaternion_from_axis_angle(localForward, inputRoll * rotationSpeed);
					quaternion yawQ = quaternion_from_axis_angle(localUp, inputYaw * rotationSpeed);

					quaternion combinedInput = quaternion_multiply(yawQ, quaternion_multiply(pitchQ, rollQ));
					quaternion targetRotation = quaternion_multiply(combinedInput, vehicleRotation);

					float smoothing = 0.05f;
					quaternion smoothedRotation = quaternion_lerp(vehicleRotation, targetRotation, smoothing);

					SET_VEHICLE_QUATERNION(my_vehicle, smoothedRotation.x, smoothedRotation.y, smoothedRotation.z, smoothedRotation.w);

					float baseAcceleration = 1.0f; // Wie stark der Schub pro Frame wirkt
					float baseDeceleration = 0.2f; // Wie stark das Flugzeug abbremst, wenn kein Schub
					float dragFactor = 0.0001f; // Luftwiderstand

					vector3 vehRot = quaternion_to_euler(vehicleRotation);

					// Schub-Einfluss
					if (thrustInput > 0.0f)
					{
						speedVector.y += baseAcceleration * thrustInput;
					}
					else
					{
						speedVector.y -= baseDeceleration; // Bremse bei Leerlauf
					}

					// Pitch-Einfluss (Steigflug bremst, Sinkflug beschleunigt)
					speedVector.y -= SIN(vehRot.x) * 0.5f; // Steigwinkel wirkt negativ auf die Geschwindigkeit

					// Luftwiderstand bremst ab proportional zur Geschwindigkeit
					speedVector.y -= speedVector.y * dragFactor;

					// Geschwindigkeit begrenzen
					if (speedVector.y < 0.0f) speedVector.y = 0.0f;
					if (speedVector.y > 180.0f) speedVector.y = 180.0f;

					// Geschwindigkeit setzen
					SET_CAR_FORWARD_SPEED(my_vehicle, speedVector.y);
				}
			}

#pragma endregion
		} //if driving
		else
		{ //we're a passenger
			if (DOES_CHAR_EXIST(vehicle_driver) && DOES_VEHICLE_EXIST_WITH_NETWORK_ID(NetworkId))
			{
				PLAYER_INDEX driversPlayerIndex = IsThisPedAPlayer(vehicle_driver);
				if (driversPlayerIndex != INVALID_PLAYER_INDEX) //a player is driving the vehicle & we grabbed their playerIndex.
				{
					if (!DOES_PLAYER_HAVE_CONTROL_OF_NETWORK_ID(driversPlayerIndex, NetworkId))
					{	//Someone else is controlling the vehicle we're a passenger of, and it's not the driver.
						PLAYER_INDEX controllersPlayerIndex = FindControllerOfNetworkId(NetworkId);
						if (controllersPlayerIndex != INVALID_PLAYER_INDEX && controllersPlayerIndex != myPlayerIndex)
						{
							ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
							printf("~r~CTL: %s ~w~is controlling this %s.", NetworkPlayerGetNameWithColour(controllersPlayerIndex), VehiclesModelName(my_vehicle));
						}
					}
				}
			}
#pragma region releasing_control
			if (my_vehicle == lastVeh && lastVeh != ctlMem->m_chauffeur.vehicle) //should only run if we've been in the drivers seat and once now.
			{ //we switched to passenger seat.
				GET_NETWORK_ID_FROM_VEHICLE(lastVeh, &NetworkId);
				if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(NetworkId))
				{
					if (HAS_CONTROL_OF_NETWORK_ID(NetworkId))
					{
						if (lastVeh != ctlMem->m_autoBoat)
						{ //this gets taken care of / don't want to do it twice and freeze.
							//SET_CAR_AS_MISSION_CAR(lastVeh); //stops vehicle de-spawning when nearby.
							//MARK_CAR_AS_NO_LONGER_NEEDED(&lastVeh); //mark for de-spawning
							 
							
							//if (bitTest(ctlMem->m_bit_sets.m_weapons, forceNeverTargettedVehicle))
								//SET_VEHICLE_CAN_BE_TARGETTED(my_vehicle, !READ_LOBBY_PREFERENCE(419)); //our lobby preference is NOT always the current setting of the lobby we are in.
								//SET_VEHICLE_CAN_BE_TARGETTED(my_vehicle, !GetAutoAimPreference());

							SET_NETWORK_ID_CAN_MIGRATE(NetworkId, true);
#ifdef Developer
							//PRINTSTRING("marked lastVeh as no longer needed & set migratable as we switched to passenger seat.\n");
							PRINTSTRING("set lastVeh migratable as we switched to passenger seat.\n");
#endif
							lastVeh = 0;
						}
					}
				}
			}
#pragma endregion
		}
	}
	else //my_vehicle doesn't exist
		bitReset(&ctlMem->m_bit_sets.m_vehicle, printed_vehicle_controller);
}