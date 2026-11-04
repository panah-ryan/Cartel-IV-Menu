#pragma once

#define MAX_CAR_OBJECTS 37

typedef struct ctl_veh_data
{
	byte currentLock;
	byte itemSelect;
	byte checkGiveWepsTimerInt;
	byte checkGiveArmourTimerInt;
} ctl_veh_data;

typedef struct autoWarpDriversVehData
{
	VEHICLE_SEAT SeatIndex;
	PLAYER_INDEX playerFollowing;
	int tick;
	int exit_vehicle_input_pressed_timeout_tick;

} autoWarpDriversVehData;


extern char* DoorLockString[4];

typedef enum eNetworkPlayerVehicleActions
{
	VEH_ACT_FLIP,
	VEH_ACT_FIX,
	VEH_ACT_CLEAN,
	VEH_ACT_FIX_SET_INVINCIBLE,
	VEH_ACT_DELETE,
	VEH_ACT_DELETE_OBJECTS,
	VEH_ACT_BURST_TYRES,
	VEH_ACT_BREAK_WINDOWS,
	VEH_ACT_BREAK_DOORS,
	VEH_ACT_SPEED_BOOST,
	VEH_ACT_ROTATE,
	VEH_ACT_SET_ON_FIRE,
	VEH_ACT_EXPLODE,

} eNetworkPlayerVehicleActions;

typedef enum ctl_vehicle_bits //need to slowly move vehicle shit out of main bitset
{
	passengerGiveNotifications, //0
	spawnVehiclesInvincible, //1
	spawnedVehiclesRequireBreakIn, //must have this on by default :D //Cartel Signature (make sure invincible vehicles can have windows smashed).
	vehicleInvincibleLoop,
	vehicleInvisibility,
	disableTrafficGenerators,
	deleteLastFunnyVehObjects,
	deleteLastNeonVehObjects,
	vehicleScorch,
	vehicleCruiseControl, //currentlyu unused
	minecraftFunnyCarGlow, 
	vehicleSeatBelt,
	bikeSeatSuperGlue,
	cycleCarColours,
	randomCarColours,
	cycleRandomChangeAllColours,
	vehicleAlwaysClean,
	vehicleSpawnForAllPlayers,
	SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED,
	chauffeurStuckChecks,
	autoSpawnBoatViceVersa,
	bypassVehicleDoorLocks,
	blockVehicleEjectHijack,
	blockVehicleNetMigration,
	autoWarpDriversNewVehicle,
	disabledRunwayPlanes,
	Created_smoke_spliff_ptfx,
	storm_flying_car,
	printed_vehicle_controller,
	local_autoflip,
	//preventWarpIntoMyVehicle, //fill empty seats with peds maybe.. undecided.
}ctl_vehicle_bits;

void ProcessVehicleAction(int eAction, int unused1);
uint GetVehPointer(VEHICLE_INDEX veh); //vehicle pool;
bool GetAttachedVehicleHost(VEHICLE_INDEX attachedVehicle, VEHICLE_INDEX *attachedHost);
bool IsExtraBusAlreadyAttached(VEHICLE_INDEX busVehicle, VEHICLE_INDEX *attachedBus); //could improve this to check all busses and get the closest but meh..


bool is_vehicle_horn_sounding(VEHICLE_INDEX vehicle);
void clearFollowDriverData();
void VehOptIncrementSeat(int unused0, int unused1); //extra logic in here means you should still be able to increment all seats even if there are empty seats..

int DoorLockToItemSelect(int mode);
bool SpawnPlayerInVehicle(uint vehicleModel); //need to add invincibility etc 
char* VehiclesModelName(const VEHICLE_INDEX vehicle);
VEHICLE_SEAT GetFreeVehicleSeat(VEHICLE_INDEX target_veh);
void SpawnVehicleSetup();
void VehOptFixNormal();
void VehOptFixInvincibility();
void VehOptDelete();
void VehOptDeleteObjects();
void DoorLocker();
void VehicleOptions();
void VehicleLoopedOptions();
void VehOptsQuestionable();

void HandlingRespawn();