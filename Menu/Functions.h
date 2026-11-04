#pragma once
#include "Utils.h"
#include "MenuUiHandling.h"
#include "World_Options.h"
#include "Network_Options.h"
#include "Vehicle_Options.h"
#include "Ped_Options.h"
#include "Player_Options.h"
#include "Keyboard_Options.h" //ctl_mem_editor
#include "Security.h"

#define MAX_AUTH_USERS 56 //never more than 76

extern int max_spawner_objs; //shouldn't be needed in helper, if it is we'll just move it to freeMem.
#define MAX_MAP_OBJECTS 70
#define MAX_NETWORK_STRING 32
#define MAX_KEYBOARD_INPUT_SIZE 96
#define GAMESAVE_LOCATION 0x82BD0418

#define CHAT_JOINING_MESSAGE "~b~has joined the chat."

#define BROADCAST_ID_ADMIN 1
#define BROADCAST_ID_HOST 2
#define BROADCAST_ID_CHAT 3
#define BROADCAST_ID_ATTACKS 4

//#define freeMemStartAddress 0x82AAC32F //doesn't align by 4
#define freeMemStartAddress 0x82AAC330

extern CAMERA_INDEX scriptedCam;

typedef struct {
	Object o;
	int creationTime;
	char* name;
} SpawnerObject;

typedef struct freemode_client_vars_original
{
	int m_status;
	int _0x04;
	int _0x08;
	int m_readyUpStatus;
	int m_gameModeSuggestion;
	int m_voteToKickIndex;
}freemode_client_vars_original;

typedef struct freemode_client_vars_tlad
{
	int _0x00;
	int _0x04;
	int _0x08;
	int m_voteToKickIndex;
	int m_gameModeSuggestion;
	int _0x14;
	int _0x18;
}freemode_client_vars_tlad;

typedef struct freemode_client_vars_tbogt
{
	int _0x00;
	int _0x04;
	int _0x08;
	int m_voteToKickIndex;
	int m_gameModeSuggestion;
	int _0x14;
	int _0x18;
	int _0x1C;
	int _0x20;
	int _0x24;
	int _0x28;
	int _0x2C;
	int _0x30;
	int _0x34;
	int _0x38;
	int _0x3C;
	int _0x40;
	int _0x44;
	int _0x48;
	int _0x4C;
	int _0x50;
	int _0x54;
	int _0x58;
	int _0x5C;
	int _0x60;
	int _0x64;
	int _0x68;
	int _0x6C;
	int _0x70;
	int _0x74;
	int _0x78;
	int _0x7C;
}freemode_client_vars_tbogt;

typedef enum NetworkObjectType
{
	NET_OBJ_TYPE_PLAYER,
	NET_OBJ_TYPE_PED,
	NET_OBJ_TYPE_DUMMY_PED,
	NET_OBJ_TYPE_AUTOMOBILE,
	NET_OBJ_TYPE_OBJECT,
	NET_OBJ_TYPE_BIKE,
	NET_OBJ_TYPE_TRAIN,
	NET_OBJ_TYPE_BOAT,
	NET_OBJ_TYPE_HELI,
	NET_OBJ_TYPE_PLANE,
	NET_OBJ_TYPE_COUNT,
}NetworkObjectType;


typedef enum LobbyPreferences
{
	LOBBY_PREF_AUTO_AIM = 419,
	LOBBY_PREF_POLICE = 420,
	LOBBY_PREF_PROBABLY_UNUSED = 421, 
	LOBBY_PREF_FRIENDLY_FIRE = 422,
}LobbyPreferences;

typedef struct miscbitSet_storage
{
	int m_sharedMenuControlCMDS_playerAllowLists; //i = player allowed, +16 = start allowed commands.
	int m_infiniteArmour_instantRevive; //index = armour, index + 16 = revive on spot with resurrect.
	int m_unlimitedAmmo_walkOnAirShootToAscend;
	int m_moneyDrop_vehicleAutoFlip;
	int m_removeWeapons_driveWater;
	int m_walkOnAir_walkerInvisibility; //Normal = status, +16 = airWalker object invisibility	
	int m_neverWanted_vehAutoFix;
	int m_vehicleNetMonitor_printedCtlUser;
	int m_hornSpeedBoost_hasBoosted; //on/off + 16 = horn has sounded for speedboost. (cap per obtaining control)
	int m_capDickheadNotify_instantReviveCap;
	int m_network;
	int m_player;
	int m_vehicle;
	int m_loggedPlayer_UNUSED; //
	int m_admin; //first = admin functionality / receipt, last 16 bits used to signify playerindex is a admin scanned cartel user
	int m_debug;
	int m_menuPreferences;
	int m_detectedModding_detectedIVMenu; 
	int m_offhostKicked_triedCrashingHostsFreemode;
	int m_CartelAdmin_CartelModerator;
	int m_UploadedData_UNUSED;
	int m_loggedToModderFile_CheckedTagForModdedChars; 
	int m_CheckedForAlias_NotADickhead;
	int m_obj_spawner;
	int m_weapons;
	int m_CrossReferencedXUID_StartedCheck;
	int m_detectedXUIDSpoof_unused;
} miscbitSet_storage;

typedef enum ctl_obj_spawner_bits
{
	PREF_OBJ_SPAWNER_INVISIBLE,
	PREF_OBJ_SPAWNER_INVINCIBLE,
	PREF_OBJ_SPAWNER_DYNAMIC,
	PREF_OBJ_SPAWNER_COLLISION,
	PREF_OBJ_SPAWNER_PICKUP_ABLE,
	PREF_OBJ_SPAWNER_FROZEN,
	PREF_OBJ_SPAWNER_MIGRATABLE,
	PREF_OBJ_SPAWNER_SPEED_IS_DIMENSIONS,
	PREF_OBJ_SPAWNER_SPHERE_AROUND_EDIT_OBJECT,
	PREF_OBJ_SPAWNER_DRAW_ORIGIN_FOR_EDIT_OBJECT,
}ctl_obj_spawner_bits;

typedef enum ctl_menu_pref_bits
{
	PREF_TOP_DRAW_JOINING_GHOSTS_PLAYERS, //handled in cartel_helper.sco, we set it in this script, and it reads.
	PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS, //as above
	PREF_MINIMAP_DRAW_TALKING_PLAYERS, //as above
	PREF_HIDE_PLAYER_NAMES,
	PREF_CARTEL_CHAT, //going to write in menu and read in helper i think.
	PREF_SETTING_ACCEPTED_CHAT_RULES,
	PREF_T_TO_TYPE_CHAT,
	PREF_CONTROLLER_CHAT_SHORTCUT,
	PREF_CUSTOM_SPAWN,
	PREF_SP_AUTOLOAD,
	PREF_DRAW_OPEN_CONTROLS,
	PREF_REPORT_ATTACKS_TO_OTHERS,
	PREF_LISTEN_FOR_NETUSER_ATTACKS,
	PREF_HOST_AUTO_KICK_ATTACKERS_AGAINST_FRIENDS,
	PREF_VOTE_KICK_FRIENDS_ATTACKER,
	PREF_VOTE_KICK_ATTACKERS,
	PREF_AUTO_VOTE_KICK_DICKHEADS,
	PREF_ADMIN_ONLY_OFFHOST_ATTACKERS,
	PREF_ADMIN_ONLY_OFFHOST_ATTACKS_AGAINST_FRIENDS,
	PREF_SET_GHOSTS_ACTIVE,
	PREF_DROPSHADOW_LOBBY_INFO,
	PREF_LISTEN_FOR_NETUSER_ATTACKS_FRIENDS_ONLY,
}ctl_menu_pref_bits;

typedef enum CTL_ADMIN_BITSET
{
	ADMIN_CMD_RESPONSE_IN_PROG,
	ADMIN_CMD_SEND_IN_PROG,
	ADMIN_BLOCKED_OBJECT_SPAWNS,
	ADMIN_ONLY_ACCEPT_MODERATOR_COMMANDS,
	ADMIN_SECURITY_BIT_GET_XUID_FROM_FREE_MEM_CALLED,//GetPlayerXuidFromFreeMemoryAddressObfuscated
	ADMIN_SELECTING_VEHICLE_MODEL,
	ADMIN_SELECTING_PLAYER_MODEL,
	ADMIN_SELECTING_ANIMATION,
	ADMIN_LOOP_CALLED,
	ADMIN_SUPPRESS_NATH_AUTO_INVITE,
	//ADMIN_IS_PLAYER_ADMIN_FUNCTION_CALLED,
	ADMIN_SECURITY_BIT_MACHINE_ID_MATCHED,
	//16 and above is used to set player as a detected user.
} CTL_ADMIN_BITSET;


#define ADMIN_PLAYERID_ALL_PLAYERS 17 
typedef enum eCartelAdminCommandIDs //health //use armour for playerIndex
{
	ADMIN_CMD_USER_SCAN = -1, //does nothing just responds. //to stop bad returns if someone adds broadcast vars but doesn't set values. ( 0 )
	ADMIN_CMD_DO_NOT_USE = 0,
	ADMIN_CMD_DELETE_SPAWNED_ENTITIES = 1,
	ADMIN_CMD_SINGLEPLAYER,
	ADMIN_CMD_FIND_GAME,
	ADMIN_CMD_REJOIN_GAME,
	ADMIN_CMD_CRASH,
	ADMIN_CMD_DELETE_OBJECTS,
	ADMIN_CMD_DELETE_OBJECTS_REQUEST_CONTROL,
	ADMIN_CMD_DELETE_VEHICLES,
	ADMIN_CMD_DELETE_VEHICLES_REQUEST_CONTROL,
	ADMIN_CMD_BLOCK_OBJECT_SPAWNING_ON,
	ADMIN_CMD_BLOCK_OBJECT_SPAWNING_OFF,
	ADMIN_CMD_TP_TO_SENDING_ADMIN_ON_FOOT,
	ADMIN_CMD_TP_TO_SENDING_ADMIN_IN_VEHICLE,
	ADMIN_CMD_TP_INTO_SENDING_ADMIN_VEHICLE,
	ADMIN_CMD_DIE, //dunno howto sneakily do this.
	ADMIN_CMD_TP_TO_CARTEL_SPAWN,
	ADMIN_CMD_TP_TO_DEV_ONLY_AIRPORT_SPAWN_HEADING_1, //facing away from hangar
	ADMIN_CMD_TP_TO_DEV_ONLY_AIRPORT_SPAWN_HEADING_2, //facing hangar
	ADMIN_CMD_HOST_WEATHER_EXTRA_SUNNY,
	ADMIN_CMD_HOST_WEATHER_SUNNY,
	ADMIN_CMD_HOST_WEATHER_SUNNY_WINDY,
	ADMIN_CMD_HOST_WEATHER_CLOUDY,
	ADMIN_CMD_HOST_WEATHER_RAINING,
	ADMIN_CMD_HOST_WEATHER_DRIZZLE,
	ADMIN_CMD_HOST_WEATHER_FOGGY,
	ADMIN_CMD_HOST_WEATHER_LIGHTNING,
	ADMIN_CMD_HOST_WEATHER_EXTRA_SUNNY_2,
	ADMIN_CMD_HOST_WEATHER_SUNNY_WINDY_2,
	ADMIN_CMD_KICK_PLAYER_BASE_ID_0,
	ADMIN_CMD_KICK_PLAYER_ID_15 = (ADMIN_CMD_KICK_PLAYER_BASE_ID_0 + 15), //make sure this is right after BASE_ID_0
	ADMIN_CMD_KICK_CONSOLE_BASE_ID_0,
	ADMIN_CMD_KICK_CONSOLE_ID_15 = (ADMIN_CMD_KICK_CONSOLE_BASE_ID_0 + 15),
	ADMIN_CMD_VOTE_KICK_TRUE_PLAYERID_BASE_0,
	ADMIN_CMD_VOTE_KICK_TRUE_PLAYERID_15 = (ADMIN_CMD_VOTE_KICK_TRUE_PLAYERID_BASE_0 + 15),
	ADMIN_CMD_VOTE_KICK_CLEAR,
	ADMIN_CMD_SET_TIME_HOUR_0_BASE,
	ADMIN_CMD_SET_TIME_HOUR_23 = (ADMIN_CMD_SET_TIME_HOUR_0_BASE + 23),
	ADMIN_CMD_READ_ADMIN_BROADCAST_STRING,
	ADMIN_CMD_READ_ADMIN_BROADCAST_STRING_DISPLAY_ADMIN_GAMERTAG,
	ADMIN_CMD_CREATE_CHAUFFEUR_AND_TARGET_SENDING_ADMIN,
	ADMIN_CMD_DISABLE_TRAFFIC,
	ADMIN_CMD_ENABLE_TRAFFIC,
	ADMIN_CMD_SPOOF_GAMERTAG_TO_ADMIN_BROADCAST_STRING, //gna have to start appending to the end due to version /enum mismatch posibility.
	ADMIN_CMD_RESTORE_XUID_GT,
	ADMIN_CMD_STRING_TO_INT_ADMIN_STRING_SPAWNPLAYERINVEHICLE_MODEL, //untested
	ADMIN_CMD_STRING_TO_INT_ADMIN_STRING_CHANGEPLAYER_MODEL, //untested
	ADMIN_CMD_CLONE_ADMIN_VEHICLE,
	ADMIN_CMD_CLONE_ADMIN_PED,
	ADMIN_CMD_QUERY_ORIGINAL_GAMERTAG, //not yet implemented. //have them send it back.
	ADMIN_CMD_READ_STRING_STORE_ANIMATION_SET,
	ADMIN_CMD_READ_STRING_PLAY_ANIMATION_FROM_SET,
	ADMIN_CMD_STOP_ANIMS,
	ADMIN_CMD_QUERY_COMPILE_DATE_TIME,
	ADMIN_CMD_DISABLE_MODERATOR_ABILITIES, //TEST 
	ADMIN_CMD_REENABLE_MODERATOR_ABILITIES,  //TEST
	ADMIN_CMD_STRING_TO_INT_ADMIN_PLAYERBITSET_CLONE,
	ADMIN_CMD_STRING_TO_INT_ADMIN_VEHICLEBITSET_CLONE,
	ADMIN_CMD_STRING_TO_INT_ADMIN_STICKYBITSET_CLONE,
	ADMIN_CMD_PULL_STICKYBITSET,
	ADMIN_CMD_PULL_PLAYERBITSET,
	ADMIN_CMD_PULL_VEHICLEBITSET,
	ADMIN_CMD_SET_JOINMSG_USER,
	ADMIN_CMD_SET_JOINMSG_PATRON,
	ADMIN_CMD_SET_JOINMSG_TESTER,
	ADMIN_CMD_SET_JOINMSG_MODERATOR,
	ADMIN_CMD_SET_JOINMSG_INVALID,
	ADMIN_CMD_RESET_ONLINE_SCORES,
	ADMIN_CMD_AUTO_INVITE_NATH_ON,
	ADMIN_CMD_AUTO_INVITE_NATH_OFF,
	ADMIN_CMD_PULL_MODERATOR_LOOKOUT_DATA,
	ADMIN_CMD_PULL_MODERATOR_LOOKOUT_TAG,
	ADMIN_CMD_PULL_ALL_BITSETS,
	ADMIN_CMD_SHAKE_PAD,
	ADMIN_CMD_ADVERTISE_OFF,
	ADMIN_CMD_ADVERTISE_ON,
	ADMIN_CMD_UNKICK_KICKED_CONSOLE_INDEX, //done
	ADMIN_CMD_OVERWRITE_KICKED_CONSOLE_INDEX_TAG,  //receiving implemented just not bothered with sending or even locally doing this.
	ADMIN_CMD_OVERWRITE_KICKED_CONSOLE_REASON, //done
	ADMIN_CMD_DISABLE_CHAT_TRUE,
	ADMIN_CMD_DISABLE_CHAT_FALSE,
	ADMIN_CMD_RING_PHONE, //lol too good not too //needs doing
	ADMIN_CMD_END_CREDITS_MUSIC, //mayb //needs doing
	ADMIN_CMD_KICK_ALL_PLAYERS, 
	ADMIN_CMD_ATTACH_TO_ADMINS_VEHICLE, //use m_ints[7] for for the attach index. 
	ADMIN_CMD_DETACH_FROM_VEHICLE,
	ADMIN_CMD_SUGGEST_GAMEMODE, //use m_ints[7] for the gamemode to suggest.
	ADMIN_CMD_DELETE_ENTITY_WITH_NETWORK_ID_IF_HAS_CONTROL, //try using m_ints[7]; //TODO Nath
	//BIT_AUTOINVITE_NATH_IF_ON_FRIENDS_LIST


} eCartelAdminCommandIDs;

typedef enum eSaveBytes
{
	SBYT_TIME_HOUR,
	SBYT_TIME_MINUTE,
	SBYT_WEATHER,
	SBYT_SAVE_DATA_VERSION,
	SBYT_MENU_BUTTON_HOLD,
	SBYT_MENU_BUTTON_PRESS,
	SBYT_HEADER_COL_1,
	SBYT_HEADER_COL_2,
	SBYT_HEADER_COL_3,
	SBYT_HEADERTXT_COL_1,
	SBYT_HEADERTXT_COL_2,
	SBYT_HEADERTXT_COL_3,
	SBYT_SUBTITLE_COL_1,
	SBYT_SUBTITLE_COL_2,
	SBYT_SUBTITLE_COL_3,
	SBYT_TEXT_COL_1,
	SBYT_TEXT_COL_2,
	SBYT_TEXT_COL_3,
	SBYT_BACKGROUND_COL_1,
	SBYT_BACKGROUND_COL_2,
	SBYT_BACKGROUND_COL_3,
	SBYT_SEL_TEXT_COL_1,
	SBYT_SEL_TEXT_COL_2,
	SBYT_SEL_TEXT_COL_3,
	SBYT_SCROLLBAR_COL_1,
	SBYT_SCROLLBAR_COL_2,
	SBYT_SCROLLBAR_COL_3,
	SBYT_HEALTH_COL_1,
	SBYT_HEALTH_COL_2,
	SBYT_HEALTH_COL_3,
	SBYT_ARMOUR_COL_1, //broken needs work
	SBYT_ARMOUR_COL_2,
	SBYT_ARMOUR_COL_3,
	SBYT_PAUSE_TXT_COL_1, //needs implementing
	SBYT_PAUSE_TXT_COL_2,
	SBYT_PAUSE_TXT_COL_3,
	SBYT_CHAT_MSG_DISPLAY_SECONDS,
	SBYT_HEADER_ALPHA,
	SBYT_HEADERTXT_ALPHA,
	SBYT_SUBTITLE_ALPHA,
	SBYT_TEXT_ALPHA,
	SBYT_BACKGROUND_ALPHA,
	SBYT_SEL_TEXT_ALPHA,
	SBYT_SCROLLBAR_ALPHA,

}eSaveBytes;
#define SAVE_DATA_VERSION_BYTE 14
typedef struct saveData_storage
{
	ctl_player_bits m_playerBits; //int
	ctl_weapon_bits m_weaponBits;
	ctl_vehicle_bits m_vehicleBits;
	ctl_menu_pref_bits m_menuPreferencesBits;
	ctl_sticky_bits m_stickyBits;
	//byte SBYT[40]; //use enum eSaveBytes
	byte SBYT[48]; //use enum eSaveBytes
	vector2 menuPos; //needs menu option
	uint m_savedVehicleModel;
	byte m_savedVehicleColours[4];
	int m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits; //first 10 = upgrades, windows = 4, doors = 4, tyres = 6
	//vector2 keyboardPos; //maybe
	//byte keyboardCols[3]; //maybe

}saveData_storage;
static_assert(sizeof(saveData_storage) <= 100, "Save data usage exceeds 100 bytes!!!");
void save_data_to_gamesave(int unused1, int unused2);
//void read_data_from_gamesave(int unused1, int unused2);

typedef enum debug_bit_set
{
	DEBUG_IS_HELPER_LOADED,
	DEBUG_SWAPPED_FREEMODE_THREAD,
	DEBUG_OG_VEHICLES_ARE_MP_SAFE,
	DEBUG_IN_SPAWNER_EDIT_SUBMENUS,
	DEBUG_ENTERED_PLAYERSETTINGS_MENU_RESTORE_INPUT_ON_EXIT,
	DEBUG_ENTERED_PLAYERSETTINGS_MENU_FREEZE_WEATHER_ON_EXIT,
	DEBUG_LOCAL_USER_IS_GIVEAWAY_WINNER,
	DEBUG_CARTEL_LOGO_NOT_PRESENT,
}debug_bit_set;

typedef struct ctl_host_broadcast_vars_array
{
	int m_Count;
	ctl_console_kick m_kicked_storage[MAX_KICKED_CONSOLES]; 
} ctl_host_broadcast_vars_array;

enum eObjectProtMode 
{
	OBJ_PROT_MODE_DISABLED = 0,
	OBJ_PROT_MODE_PLAYER_ONLY = 1,
	OBJ_PROT_MODE_VEHICLE_ONLY = 2,
	OBJ_PROT_MODE_BOTH = 3,
};

enum eModelProtMode {
	MODEL_PROT_OFF = 0,
	MODEL_PROT_SELF = 1,
	MODEL_PROT_LOBBY = 2,
};

typedef enum eDodgyPedType {
	DODGY_PED_NONE = 0,
	DODGY_PED_PLAYER = 1,
	DODGY_PED_SUPERLOD = 2,
	DODGY_PED_CUTSCENE = 3,
	DODGY_PED_INVALID = 4,
}eDodgyPedType;

typedef enum eSaveCarryover
{
	SAVE_CARRY_NONE,
	SAVE_CARRY_TO_MP,
	SAVE_CARRY_TO_SP,
}eSaveCarryover;


/*//These should be obfuscated
#define PLAYERDB_API_FQDN "playerdb.pprx.net" //	m_PPRX_DOMAIN_STRING[20];
#define PLAYERDB_API_PORT 80
#define PLAYERDB_API_PATH "PlayerDB/Create" //		m_PPRX_PATH_STRING[16]; 
#define PLAYERDB_API_USER_AGENT "s9c6xpfctck18hqk"	m_PPRX_USER_AGENT_STRING[20]
#define PLAYERDB_API_KEY "cG5ldC1zeXN0ZW0tYXBpLTAyOjh1Yyp3JktSU2FkcVdyMnU4VW52TnF1IWgkMChoYXwj" //m_PPRX_API_KEY[72];
#define PLAYERDB_MAX_BODY_SIZE 512*/

typedef struct stickyFreeMemory //memory that sticks between sessions and menu restarts.
{	//leave stickybitset and byteXUID etc alone for now.
	//int m_detectedModding_detectedIVMenu; //NOTE: Keep this at the top as the XEX uses this static address.
	//int m_loggedToModderFile_CheckedTagForModdedChars; //NOTE: Don't reposition this as the XEX uses this static address.
	ctl_map_mod m_map_mod;
	OBJECT_INDEX m_object_car_entities[34]; //keep in free mem so sticks between menu restarts.
	char m_PPRX_USER_AGENT_STRING[20];
	OBJECT_INDEX m_walk_water_object[16]; //keep in free mem so sticks between menu restarts.
	int m_cartel_join_pin; //necessary to be in free memory to read once go from SP -> MP //currently unused but use some pin logic for 360 kicker.
	char m_last_games_host_gamertag[16]; //necessary to be in free memory to read oncegone from MP -> SP
	ctl_timed_ramps m_timed_ramps[4];  //keep in free mem so sticks between menu restarts.
	ctl_broadcast_vars_array m_cartelBroadcastVars; //keep in free mem so as works without cartel_memory.sco if they don't crash.
	char m_PPRX_DOMAIN_STRING[20];  //"playerdb.pprx.net"
	ctl_host_broadcast_vars_array m_cartelHostVariables; //containes kicked console data.
	char m_PPRX_API_KEY[72];
	int m_AdminCommandsTick; //used for either sending or receiving, we will never do both when released.. (Admins just wont listen).
	ctl_action_pad m_action_pad[MAX_ACTIONPADS];
	walkAirData m_walk_on_air; //keep in free mem so sticks between menu restarts.
	char m_PPRX_PATH_STRING[16]; // "PlayerDB/Create"
	ctl_player_spliff player_spliff;
	byte m_saved_data_copy[sizeof(saveData_storage)]; //COULD REMOVE IF NEEDED //Used to copy gamesave over to MP/Scores as that location gets zerod
	int m_carryover_mode; //0 = none, 1 = SP -> MP, 2 = MP -> SP //byte won't work unless array of 4
	uint m_savedVehicleModel;
	byte m_savedVehicleColours[4];
	char m_PPRX_HTTP_STRING[8]; // "http://"
	int m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits; //first 10 = upgrades, windows = 4, doors = 4, tyres = 6
	THREADID m_CartelHelper_ThreadId;;
	THREADID m_Cartel_ThreadId;
	THREADID m_CartelMemory_ThreadId; 
	//char m_pprx_base_url[64];
}stickyFreeMemory;
static_assert(sizeof(stickyFreeMemory) <= 3212, "FREE MEMORY USAGE EXCEEDS 3212 BYTES!!");
extern stickyFreeMemory *freeMem;

typedef struct sharedBuffer_storage //could char* all of these and set them in init_ctl_memory.
{
	char* keyboard_input_buffer; //BUFFER128 allocated in init_char_buffers() from init_ctl_mem()
	char joining_players_bar_buffer[96]; //this is used in a diff script so we do this to avoid any possible funky business.
	char* player_name_backup[16]; //BUFFER16's allocated in init_char_buffers() from init_ctl_mem()
	//char keyboard_input_buffer[96];
	//char joining_players_bar_buffer[96];
	//char player_name_backup[16][16];
} sharedBuffer_storage;

typedef struct ctl_keyboard
{
	char* keys[5][4][10];
	char* key_mappings[84][2];
	char made_by_storm[27];
	int start_pressed_tick; //used to DEACTIVATE_FRONTEND and DISABLE_PAUSE_MENU for 1 sec after START pressed by keyboard
}ctl_keyboard;

#define CHAT_MSG_SIZE 52
typedef struct ctl_chat_message
{
	uint timestamp;
	int target_player;
	int encryptedMessageHash; //used for making sure entire message got through.
	char message[CHAT_MSG_SIZE];
} ctl_chat_message;

typedef struct ctl_chat_broadcast_vars_array
{
	int m_Count;
	ctl_chat_message m_Data[16];
} ctl_chat_broadcast_vars_array;

typedef struct ctl_attack_network_notifier  //make byte last_notified_attack_type[16] for print capping.  //set 255 by default.
{
	byte offender_attackType[4]; //more room here should we need it. (8 bytes per player total vs 12)
	uint extraData;

} ctl_attack_network_notifier;

typedef struct ctl_attack_network_notifier_array
{
	int m_Count;
	ctl_attack_network_notifier m_Data[16];
} ctl_attack_network_notifier_array;

typedef struct retail_prot_choces
{
	byte m_obj_prot_mode; //needs adding to net preferences and the ability to actually change them.
	byte m_model_prot_mode;
}retail_prot_choices;

typedef struct ctl_network_display
{
	char hostName[34]; //includes COL_NET etc.
	char frameRateString[12];
	char emptySlots_360UserCountString[19];
	char playerCountString[18];
	char moddingString[17];
	int currentFrameRate;
}ctl_network_display;


#define MAX_RIGGED_BUSSES 3 //2 will carry the entire lobby under most circumstances so eh..
typedef struct ctl_extra_seat_busses
{
	VEHICLE_INDEX m_hostBus;
	NETWORK_INDEX m_hostBus_NetId;
	VEHICLE_INDEX m_attachedBus;
	NETWORK_INDEX m_attachedBus_NetId;
}ctl_extra_seat_busses;

/*
typedef struct Spawner_Objects_Array
{
	SpawnerObject m_spawned_objects[300]; //needs moving to CTL_GET_SPAWNER_MEMORY
}Spawner_Objects_Array;
extern Spawner_Objects_Array *spawnerMem;*/


//STORMS MISSILE SHIT
#define MAX_MISSILES 3

typedef enum
{
	MISSILE_HEATSEEKER,
	MISSILE_HOMING,
	MISSILE_TV,
	MISSILE_BOMB,
} MissileMode;

typedef struct
{
	Object missile;
	Vehicle target;
	MissileMode mode;
	int firedTime;
	vector3 helperVector;
	EXPLOSION_TAG missile_warhead_type;
	int burn_time;
	int climb_time;
	float proximity_fuse;
	float velocity;
	bool flame_trail;
	bool equiped_with_cam;
	float cam_offset;
	int link_mode;
} StormMilitaryMissile;
///////////////////////
//MilitaryMissile militaryMissile[MAX_MISSILES];


extern SpawnerObject* spawnerMem;
#define MAX_RECENTLY_KICKED_CONSOLES 20

typedef struct military_missile_vars
{
	int bullshit;
	int bullshit2;
	float missile_burn_time;
	float missile_climb_time;
	float missile_proximity_fuse;
	float missile_velocity;
	bool missile_flame_trail;
	bool missile_equip_with_cam;
	float missile_cam_offset;
	TextureDict txdPolice;
	TextureDict spaceHud;
	Vehicle lastTarget;
	int lockStartTime;
	vector3 fired_pos;
	char* WarheadTypeStrings[4];
	char* MissileChassisStrings[3];
	char* LinkModeStrings[3];
	EXPLOSION_TAG WarheadTypes[4];
	uint MissileChassisInts[3];
	int selectedMissileChassis;
	int selectedExplosionType;
	int selectedLinkMode;
} military_missile_vars;

typedef enum em_commonStringsGXT
{
	GXT_STRING_KICK,
	GXT_STRING_KICK_PLAYER,
	GXT_STRING_MUTE,
	GXT_STRING_VEHICLE,
	GXT_STRING_PLAYERS_LIST,
	GXT_STRING_RECENT_PLAYERS,
	GXT_STRING_JOINED,
	GXT_STRING_WEAPONS,
	GXT_STRING_DELETE,
	GXT_STRING_DISPLAY,
	GXT_STRING_FRIENDS,
	GXT_STRING_INVITE,
	GXT_STRING_PREFERENCES,
	GXT_STRING_RED,
	GXT_STRING_GREEN,
	GXT_STRING_BLUE,
	GXT_STRING_AUTO_AIM,
	GXT_STRING_MALE, //KEEP THIS 1 BEFORE FEMALE AS WE POINT HERE FOR STRING ITEM SELECTIONS.
	GXT_STRING_FEMALE,
	GXT_STRING_PLAYER_MODEL,
	GXT_STRING_ANIMATION,
	GXT_STRING_HOST,
	GXT_STRING_PLAYERS,
	GXT_STRING_HEALTH,
	GXT_STRING_ARMOUR,
	GXT_STRING_PRESS,
	GXT_STRING_HOLD,
	GXT_STRING_ALLOW, //	0xB595515F = Autoris?e //french //seemingly only used for auto-aim so meh we will leave this here for now.
	GXT_STRING_DONT_CARE,
	GXT_STRING_OFF, //MO_OFF = Sans
	GXT_STRING_ON, //MO_ON = Avec //French
	GXT_STRING_POLICE, //POLICE_ON_OFF = 	POLICE_ON_OFF = Polizei //german
	GXT_STRING_FRIENDLY_FIRE, //	FRIENDLY_FIRE = Friendly Fire (english is the longest surprisingly). 
	GXT_STRING_WELCOME,
	GXT_STRING_AIRPORT,
	GXT_STRING_PRISON,
	//GXT_STRING_SAVE,
	//GXT_STRING_LOAD,
	//MO_ON = Avec


	GXT_STRING_COUNT,

}em_commonStringsGXT;

typedef struct ctl_memory_sco //cartel_memory.sco string space memory.
{
	ctl_secure_id ObfuscatedIdentifiers[MAX_AUTH_USERS]; //go with Me Ryan and R07 for tests.. //NEVER MORE THAN 23 OF THESE!
	miscbitSet_storage m_bit_sets;
	sharedBuffer_storage m_char_buffers;
	NetworkLobbyPinCodeData m_lobby_pin_data; //no point moving to free mem as if meny reboots it will clear bitsets etc.
	autoWarpDriversVehData m_follow_driver_data; 
	ctl_keyboard m_keyboard;
	chauffeurData m_chauffeur;
	ctl_interior_peds m_interior_peds; //pretty sure if menu terminates these will run task wander or whatever anyway / be marked as no longr script controlled.
	ctl_mem_editor m_mem_editor;
	int m_temp_vehicle_upgrades;
	ctl_veh_data m_my_veh_data; //needs re-doing is meh
	ctl_chat_broadcast_vars_array m_cartelChatVariables;
	ctl_chat_message m_CartelChatDisplay[8];
	uint m_chat_msg_display_time_seconds;
	//PLAYER_INDEX SelectedPlayerId; //didn't actually end up using this in helper yet but eh who knows what the future holds.
	retail_prot_choices m_retail_prot;
	ctl_network_display m_netDisplay;
	char* m_padButtonStrings[16];
	byte m_padButtonsMandatoryDrawAlpha;
	char* m_networkObjTypeNames[10];
	VEHICLE_INDEX m_autoBoat;
	//VEHICLE_INDEX m_objectCar;
	ctl_attack_network_notifier_array m_CartelNetAttackVars;
	scrThread *main_sco; //unused i think
	int m_hostFreemodeCrasherTick; //used for dicklist/mods ->singleplayer host, prevents it sticking/fucking the whole lobby /menu pausing.
	char* m_game_mode_strings[30];
	char* m_commonStrings[GXT_STRING_COUNT];
	SpawnerObject m_RETAIL_spawned_objects[70]; 
	uint m_CartelMenuCompileDateHourHash;
	char* m_assembled_strings[4];
	uint m_commonStructsCombinedSize;
	uint m_language;
	ctl_extra_seat_busses m_CartelBusses[MAX_RIGGED_BUSSES];
	ctl_console_kick m_recent_kicked_storage[MAX_RECENTLY_KICKED_CONSOLES];
	StormMilitaryMissile militaryMissile[MAX_MISSILES];
	military_missile_vars StormMissileVars;
}ctl_memory_sco;
static_assert(sizeof(ctl_memory_sco) <= 8188, "cartel_memory.sco usage EXCEEDS 8188 BYTES!!"); 
extern ctl_memory_sco *ctlMem;

typedef enum eassembled_strings_from_gxt
{
	ASSEMBLED_STR_PRESS_AND_HOLD_MENU_OPEN,
	ASSEMBLED_STR_AUTO_AIM_PLAYERS,
	ASSEMBLED_STR_AUTO_AIM_PEDS,


	ASSEMBLED_STR_COUNT,
}eassembled_strings_from_gxt;


typedef struct NetworkDataStruct
{
	int _0x00;
	void* classPointer; //0x04
	int _0x08;
	int _0x0C;
	int idFlags;
} network_s;

typedef struct PoolStruct
{
	void* pool;
	byte* valid;
	int _0x08;
	int size;
} pool_s;

typedef struct CAutoMobile
{
	char _0x00[0x118];
	int renderFlags;
	char _0x11C[0x940];
	int nonZero_ifHornSounding; //0xA5C
	char _0xA60[0x86C];
	bool horn;
} vehicle_s;

typedef enum CTL_WEAPON_TYPES
{
	WEAPON_TYPE_ADVANCED,
	WEAPON_TYPE_POOR,
	WEAPON_TYPE_TBOGT
} CTL_WEAPON_TYPES;

typedef enum eEpisode
{
	EPISODE_ORIGINAL,
	EPISODE_TLAD,
	EPISODE_TBOGT,
}eEpisode;

typedef enum eTempCarActions
{
	TEMP_CAR_SLOW_DOWN_SOFTLY = 1,
	TEMP_CAR_SLOW_DOWN_HARDLY_MAKE_UNUSABLE = 2,
	TEMP_CAR_SLOW_DOWN_SOFTLY_THEN_DRIVE_BACKWARDS = 3,
	TEMP_CAR_SLOW_DOWN_HARDLY_TURN_LEFT_WHILE_SLOWING_DOWN = 4,
	TEMP_CAR_SLOW_DOWN_HARDLY_TURN_RIGHT_WHILE_SLOWING_DOWN = 5,
	TEMP_CAR_SLOW_DOWN_HARDLY = 6,
	TEMP_CAR_TURN_LEFT = 7,
	TEMP_CAR_TURN_RIGHT = 8,
	TEMP_CAR_DRIVE_FORWARDS_HIGH_RPM = 9,
	TEMP_CAR_TURN_RIGHT_SOFT = 10,
	TEMP_CAR_TURN_LEFT_SOFT = 11
} eTempCarActions;

typedef struct CPlayerPed
{
	void* vftable;
	char _0x04[0x64];
	void* m_netObject; //0x68
	int _0x6C;
	char _0x70[0xA0];
	char _0x110[8];
	int m_flags; //0x118
	char _0x11C[0x104];
	void* m_playerInfo; //0x220
	char _0x224[0x5C];
	uint m_inventory;
	char _0x284[0xB14];
	int _0xD98;
} CPlayerPed; 


typedef struct CPlayerInfo //From Ryan
{
	union //0x00
	{
		byte lan_ip_octets[4];
		int m_lan_ip;
	};
	union //0x04
	{
		byte wan_ip_octets[4];
		int m_wan_ip;
	};
	byte _2port_6mac_2pad[12]; //0x08 added 2 bytes for sc-cl
	char _0x14[4];
	int m_machineId[2]; //0x18
	char _0x20[8];
	uint m_rlPeerIdUpper; //0x28
	uint m_rlPeerIdLower; //0x2C
	char _0x30[0x10];
	int m_xuid[2]; //0x40
	char _0x48[4];
	char m_gamertag[16]; //0x4C
	char _0x5C[0x474];
	int m_state; //0x4D0
	char _0x4D4[0x90];
	int m_colorIndex; //0x564
	char _0x568[0x10];
	CPlayerPed* m_playerPed; //0x578
} CPlayerInfo;

/*
typedef struct CPlayerInfoBackup
{
	char _0x00[0x18];
	byte m_machineId[8]; //0x18
	char _0x20[8];
	uint m_rlPeerIdUpper; //0x28
	uint m_rlPeerIdLower; //0x2C
	char _0x30[0x1C];
	char m_gamertag[16]; //0x4C
	char _0x5C[0x474];
	int m_state; //0x4D0
	char _0x4D4[0x90];
	int m_colorIndex; //0x564
	char _0x568[0x10];
	CPlayerPed* m_playerPed; //0x578
} CPlayerInfoBackup;
*/

/*typedef struct
{
	int FirstPointer;   // Offset 0
	int BoolAddress;    // Offset 4
	int MaxHandle;      // Offset 8
	int StructSize;     // Offset 12
	int unk1;           // Offset 16
	int HandlesInPool;  // Offset 20
} POOL;*/
typedef struct CPool
{
	int m_firstPointer;   //0
	int m_bools;    //4
	int m_maxHandle;      //8
	int m_structSize;     //0xC
	int _0x10;           //0x10
	int m_handleCount;  //0x14
} CPool;


enum NetGameEventTypes
{
	REQUEST_CONTROL_EVENT, //Reply, ExtraData
	GIVE_CONTROL_EVENT, //Reply, ExtraData
	OBJECT_ID_FREED_EVENT,
	WEAPON_DAMAGE_EVENT, //Reply
	REQUEST_PICKUP_EVENT, //Reply
	GAME_CLOCK_AND_WEATHER_EVENT,
	RESURRECT_PLAYER_EVENT,
	RESURRECTED_LOCAL_PLAYER_EVENT,
	GIVE_WEAPON_EVENT,
	REMOVE_WEAPON_EVENT,
	REMOVE_ALL_WEAPONS_EVENT,
	VEHICLE_COMPONENT_CONTROL_EVENT, //Reply
	REQUEST_FIRE_EVENT,
	START_FIRE_EVENT,
	REQUEST_EXPLOSION_EVENT,
	START_EXPLOSION_EVENT,
	START_PROJECTILE_EVENT,
	SESSION_SETTINGS_CHANGED_EVENT,
	ALTER_WANTED_LEVEL_EVENT,
	CREATE_PICKUP_EVENT,
	CHANGE_RADIO_STATION_EVENT,
	UPDATE_GPS_EVENT,
	RAGDOLL_REQUEST_EVENT,
	MARK_AS_NO_LONGER_NEEDED_EVENT,
	PLAYER_TAUNT_EVENT,
	DOOR_BREAK_EVENT,
	HOST_VARIABLES_VERIFY_EVENT, //Reply
	EVENT_COUNT
};

void CarryOverSaveSettings(); 
void CopyGamesaveToOnlineScores(); //ideally we just came from SP.
void init_ctl_mem();
void init_free_mem();
void draw_menu_open_inputs();
int indexOfAfterIndex(const char* str, char c, int startIndex, int size);
network_s* GetNetworkDataFromObject(uint pool, uint object);
bool start_cartel_helper();
int DeleteObjectsOnVehicle(VEHICLE_INDEX vehicle); //returns number of deleted objects
int DeleteObjectsOnPed(PED_INDEX ped); //returns number of deleted objects
int get_chat_rules_acceptance();
void DeleteObject(OBJECT_INDEX *object);
void DeleteCar(VEHICLE_INDEX *vehicle);
void DeleteChar(PED_INDEX *ped);
bool showAndGetKeyboardInput(const char *title, bool useHexMode, const char *editString, int maxLength, int fadeMs, float posX, float posY, int r, int g, int b);
void BlockYVehicleExitJustPressed();
bool IsVehicleScorched(VEHICLE_INDEX vehicle);
uint GetHandleFromPool(CPool* pool, uint Pointer_Address);
VEHICLE_INDEX GetPlayerVeh();
bool IsPlayerInvincible(PLAYER_INDEX playerIndex);
bool IsPlayerAMultiplayerModel(PLAYER_INDEX playerIndex);
bool SpawnVehicle(uint model, float x, float y, float z, float h, VEHICLE_INDEX* spawnedVehicle, bool netMigratable, bool onAllMachines);
bool CloneVehicle(VEHICLE_INDEX targetVeh, float x, float y, float z, float h, VEHICLE_INDEX* clonedVehicle);
CPlayerInfo* GetPlayerPointer(PLAYER_INDEX playerIndex);
bool CanNetworkIdMigrate(uint pool_address, uint object);
#define CanObjectMigrate(object) CanNetworkIdMigrate(0x82DE5A50, object)
void fix_tell_net_loop(); //stops user receiving blackscreen events.
void SetTellNetResponseEnabled(bool enable);

void GiveWeaponsToPed(PED_INDEX ped, CTL_WEAPON_TYPES type, bool armor);
bool GrabKeyboardInputString(int max_size, int min_size, bool upper_case, bool block_spaces, bool block_special_chars, bool is_hex, bool clear_buffer);
int HexToIntNath(const char* hex);
void GetOffsetFromCamera(float offset, float* x, float* y, float* z, float* heading);
void SetHostByte(bool set);
bool IsStrValid(const char* str);
void SetCarColorForIndex(int index, Color* color);
void fix_freeze_guns_in_memory();
Color GetCarColorForIndex(int index);
bool IsPlayerValid(PLAYER_INDEX player);
char* CWeaponInfoManager_GetInfo(WEAPON_TYPE weapon_index);

void CreateByteSet(uint *ByteSet, uint b0, uint b1, uint b2, uint b3);
void GetByteSetH2(uint ByteSet, uint *b0, uint *b1, uint *b2, uint *b3);
bool IsPlayerValid(PLAYER_INDEX player);

void dynamic_read_data_from_saved(int block_autoSave, int un2); //if IS_NETWORK_SESSION it reads from scores, else gamesave
//void read_saved_data(int unused0, int unused1);
void save_data(int unused0, int unused1);

void update_menu_open_controls_string();
extern void init_strings_assembled_from_gxt();
extern void init_common_strings_from_gxt();
extern void init_gamemode_strings();
extern void init_net_obj_type_strings();
extern void init_object_spawner_name_buffers();
bool safeDetachLocalPlayer();
void make_player_settings_legit(int un1, int un2);