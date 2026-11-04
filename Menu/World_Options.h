#pragma once

#define MAX_ACTIONPADS 3
#define MAX_TIMED_RAMPS 4

extern VEHICLE_INDEX myNeonCar;

typedef struct walkAirData 
{
	OBJECT_INDEX m_object[16];
	float m_height[16]; //will get rid of an int pointer as we have two , one for heights and one for objects, then the int tick..
	int m_tick; //upates on-foot position every 250ms / greatly reduces rubber-banding on network.
}walkAirData;

typedef struct ctl_map_mod
{
	OBJECT_INDEX object[70];
	int obj_count;
	BLIP_INDEX blip;
	VEHICLE_INDEX vehicle;
}ctl_map_mod;

typedef struct chauffeurData
{
	PLAYER_INDEX target_player;
	VEHICLE_INDEX vehicle;
	PED_INDEX driver;
	float speed;
	int driving_style;
	int grab_coords_tick;
}chauffeurData;

//Maximum of 3 (don't go over this for good measure, each pad base is 4 objects + the payNspray/Arrow Sign/Satellite object
typedef struct ctl_action_pad
{
	BLIP_INDEX m_blip;
	vector2 m_corner1;
	vector2 m_corner2;
	OBJECT_INDEX m_padObject;
	OBJECT_INDEX m_grids[3];
	OBJECT_INDEX m_displayObject; //PaynSpray can, Teleport object satellite etc. (maybe make teleport spawn something on the receiving end).
	vector3 m_coords; //ensure to run GET_OBJECT_COORDINATES straight after spawning!!!
	byte m_currentMode;
	char _padding[3];
	uint m_modeData; //Vehicle model or whatever. ////Set to 1 when teleport destination has been set, resets to 0 upon user deletion of pad
	VEHICLE_INDEX m_generatedVeh; //use modedata for the vehicle model..
	NETWORK_INDEX m_generatedVehNetIndex;
	vector3 m_teleportDest;
	int m_playerTick[16]; 
	char m_padName[16];
} ctl_action_pad; 

typedef struct ctl_timed_ramps
{
	uint m_rampTick;	//FUTURE: Maybe optimise by adding a tick for each ramp.
	//int freeMem->m_timed_ramps->m_deletionDelaySeconds = 10;
	//int freeMem->m_timed_ramps->m_rampSpawnAmount = 1;
	int m_rampSpawnAmount;
	int m_deletionDelaySeconds;
	OBJECT_INDEX m_ramps[MAX_TIMED_RAMPS];
} ctl_timed_ramps;

typedef struct ctl_weather_time
{
	int m_timeHour;
	int m_timeMinute;
	uint m_weather;
	uint m_timeScale; //use float TO_FLOAT(int value)
} ctl_weather_time;

extern ctl_weather_time time_weather_data;

extern char* delete_strings[12]; //should put these in a struct to tidy it up.
extern int delete_selection; //default highlighted is objects 
extern int delete_distance;

bool HasAirWalkerFromOtherUser(PLAYER_INDEX PlayerID, OBJECT_INDEX *scan_obj, PLAYER_INDEX *Controller);

extern OBJECT_INDEX attached_object;

typedef enum ACTION_PAD_MODE {
	PAD_MODE_FIX_VEHICLE,
	PAD_MODE_TELEPORT_TO_SAVED_DESTINATION,
	PAD_MODE_GIVE_ARMOUR_HEALTH_WEAPONS,
	PAD_MODE_SPAWN_VEHICLE_ID_PROTECTED,
	PAD_MODE_PAYNSPRAY,
	PAD_MODE_SLINGSHOT,
	PAD_MODE_SPAWN_VEHICLE_NORMAL,
} ACTION_PAD_MODE;

bool CreateObject(uint model, float x, float y, float z, float h, OBJECT_INDEX* object, bool lockNetControl); 
bool CreateObjectLocally(uint model, float x, float y, float z, float h, OBJECT_INDEX* object);
void CreateChauffeurDriver();
void WorldOptRigTrafficGens();
void ChaufferSpawnVehicle(uint model);
bool SpawnChauffeurInCurrentVehicle();
void DisbandCurrentChauffeur();
void EBCreateFunnyCarEntity(uint model, float offX, float offY, float offZ, float rotX, float rotY, float rotZ, int objNumber);
void CreateMapObject(float x, float y, float z, uint model, float qX, float qY, float qZ, float qW, bool dynamic);
void EBCreateMapObject(uint model, float x, float y, float z, float h, float qx, float qy, float qz, float qw, bool dynamic, int quaternion, bool no_offset);
void WorldOptions();
void WorldLoopedOptions();
void ClearCurrentlySpawnedMap();
void DeleteAllSpawnedEntities();
void WorldAirOptions();
void callUniversalDelete();
void callLocalUniversalDelete();
void UniversalDelete(const int desired_distance, const int current_type, const bool print, const bool request_control, const int attempts);
void ClearNeonCarObjects(); //called when attaching neons
void loadLocalMapFile(int un1, int un2); //provides keyboard for input.
void loadMapFileGivenPath(char* filePath); //will load filename
bool SpaceInNetObjectPool(int amount);

void saveMapFile(int un1, int grabCurrentLocationForTp);
void saveMapFileTP(int un1, int un2);