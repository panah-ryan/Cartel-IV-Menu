#pragma once
#include "Network_Functions.h"

extern char* attachToVehStrings[5];
void netPlayerVehDetach(); //CLEAN
//#define freeMemStartAddress 0x82AAC330
#define ByteAndXUIDGTFreeMemoryAddress			0x82A9EC30 //GT and XUID take us up to 32 bytes
#define stickyBitSet							0x82A9EC52 //used to make settings persist between sessions.
#define RetailFreemodeCrasherPlayerindexAddress 0x82A9EC56

#define MAX_KICKED_CONSOLES 12 //they can kick machineID's so ehhh. //maybe allow Admin to kick XB1 consoles by default..


//CHANGE BELOW SUM TO TRIGGER A RESET / IF YOU CHANGE ANYTHING. //and make sure it hasn't been used before -> comments
#define cartel_compare_number_for_data_presence ((hidden_number1355301() * hidden_number57()) - (hidden_number114() + 1)) //77252043 //77252271 //77252157

//MODERATOR BUILD v
//SCORES 1-4 used for storing lookOut playername string
//SCORE 5 used for MID Lower, 6 used for MID Upper
//SCORE 7 USED FOR IP ADDRESS
//SCORE 8 used for XUID Lower, 9 used for XUID Upper
//SCORE 10 Used to signify data presence (-99 is default so we'll set to -98);
//90 - 115 = saveData (TBoGT).
//115-140 //25 = savedata storage / carryover
//SCORE 142-3-4-5 = Custom spawnpoint
//SCORE 141 used to detect if spawnpoint has been set.

#define cartel_online_bitset_score_id (hidden_number57() - hidden_number32()) //25
//#define cartel_online_score_id_stickyBitSet hidden_number32()
//#define cartel_online_score_id_mMenuPrefsBitSet (hidden_number65() - hidden_number32()) //33
#define cartel_online_joinmsg_score_id ( (hidden_number114() + hidden_number32()) -  hidden_number97()) //49
//#define cartel_online_score_id_time_minute hidden_number62()
//#define cartel_online_score_id_weather hidden_number65() 
#define cartel_score_id_for_dictating_data_presence hidden_SixtyEight()  //tryin 22:21 sunny
//#define cartel_online_score_id_time_hour hidden_number69()
#define cartel_online_score_id_delete_distance 70 //meh who cares wouldn't even crash prolly 
//#define cartel_online_score_id_bitSet_storage_vehicle hidden_number92()
//#define cartel_online_score_id_bitSet_storage_player hidden_number97()
#define cartel_online_score_id_adminRemoteBitSet hidden_number114() //adminRemoteScoreBitSet

/*int m_timeHour;
	int m_timeMinute;
	uint m_weather;*/

typedef enum eNetworkBits
{
	NETWORK_GROUPHACK_BOOL,
	NETWORK_KICK_360,
	NETWORK_KICK_360_ALLOW_FRIENDS,
	NETWORK_KICK_360_CHECK_FRIENDS_ONLINE_STATUS,
	NETWORK_KICK_360_STORE_KICKED_IDENTIFIERS,
	NETWORK_KICK_360_ALLOW_PIN_SET,
	NETWORK_IGNORE_COMPLAINTS, //off-host kick.
	NETWORK_REGISTERED_BROADCAST_VARS_CALLED,
	NETWORK_HAS_BECAME_HOST, //can rename to printed host change.
	NETWORK_REGISTERED_CHAT_VARIABLES,
	NETWORK_SCO_PROT_GROUP_HACK,
	NETWORK_SCO_PROT_TIDLE_CRASH,
	NETWORK_PROT_FLIP_MODEL_OBJECT_ATTACHED,
	NETWORK_SCO_PROT_SUPERLOD,
	NETWORK_SCO_PROT_CUTSCENE, 
	NETWORK_PROT_AUTO_KICK,
	NETWORK_OBJ_PROT_WANTS_VEHICLE_RESPAWN,
	NETWORK_SCO_PROT_EXPLOSION_LAG,
	NETWORK_RAN_CRASH_HOST_FREEMODE_DICKLIST,
} eNetworkBits;

typedef struct NetworkLobbyPinCodeData
{
	int lobby_pin_code;
	int player_joined_tick[16];
} NetworkLobbyPinCodeData;

typedef enum ctl_sticky_bits //sticks between sessions
{
	STICK_XUID_STORED,
	STICK_ADVERTISE_MENU,
	STICK_USE_CARTEL_SPAWN,
	STICK_XEX_PRESENT,
	STICK_KICKED_TRYING_TO_JOIN,
	STICK_CARTEL_SCO_WAS_HERE_MEMES_COMPAT,
	STICK_DISPLAYED_XNOTIFY_GREETING,
} ctl_sticky_bits;


typedef enum eDickheadType
{
	DICKLIST_KICK_IF_HOST = 1,
	DICKLIST_KICK_IF_HOST_DISPLAY_IDENTIFIERS,
	DICKLIST_OLD_MODS_LAG_AND_VOTE_KICK,
	DICKLIST_UNAVAILABLE,
} eDickheadType;

typedef enum eCartelUserType
{
	CARTEL_NON_USER = -1,
	CARTEL_USER,
	CARTEL_PATRON,
	CARTEL_TESTER,
	CARTEL_MODERATOR,
	CARTEL_DEVELOPER,
	CARTEL_USER_HIDDEN,
	CARTEL_USER_DISABLED_ADVERTISE,
	CARTEL_GIVEAWAY_WINNER,
} eCartelUserType;

typedef enum eOnlineScoreBitSet
{
	ONLINEBIT_AUTO_PULL,
} eOnlineScoreBits;

typedef enum eSharedMenuCommands
{
	SHARED_MENUCMD_TELEPORT_TO_SENDER_ON_FOOT = 16,
	SHARED_MENUCMD_TELEPORT_TO_SENDER_IN_VEHICLE,
	SHARED_MENUCMD_TELEPORT_INTO_MY_VEHICLE


} eSharedMenuCommand;
/*
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
}NetGameEventTypes;
*/
//32 * 16 = 512 bytes / 128 ints worth, + m_Count = 129 ints worth.
//516 bytes.. + 32 = 548
typedef struct ctl_broadcast_strings
{
	union
	{
		char m_string[32];
		int m_ints[8]; //what a fackin legend. //could use 6 and 7 and it wouldn't even chop off the strip club char 18 or join msg whoop.
		//byte m_bytes[32];
	};
	int m_CommandID;
	int m_targetPlayerID;
	//bitset will be put into m_string space. along with room for 2 other ints if we memcpy directly.
} ctl_broadcast_strings;

typedef struct ctl_broadcast_vars_array
{
	int m_Count;
	ctl_broadcast_strings m_Data[16]; //MAX_NETWORK_PLAYERS
} ctl_broadcast_vars_array;

typedef struct SearchResult_s
{
	char hostName[16];
	int _0x10[2];
	int maxPlayers;
	int players; // 4 bytes 28 + 4 = 32
	int _0x20[0x53];
} SearchResult_s;
static_assert(sizeof(SearchResult_s) == 0x16C, "SearchResults_s is incorrect size");

typedef struct search_results_array
{
	SearchResult_s results[10];
} search_results_array;

typedef struct ctl_console_kick //game kicks XUID so no point doing that
{
	char gamertag[16];
	int failsCounter;
	int MachineID; //only half of it yes, but this is only usable on Xbox 360 users so meh.. (currently)
	int IPAddress; //grabs the full IP.
	char kickedReason[26];	//let the user type a reason so new hosts can see it (list synced just like R*'s XUID kicklist).
	byte MAC[8]; //yes 8, ints memba. we'll only use 6.
	//char _padding[2];
	//	bool XboxOne;  //only allow use on 360 users so meh.
} ctl_console_kick; //280 bytes (size of 10)


//extern ctl_broadcast_vars_array cartelBroadcastVars;
extern ctl_broadcast_vars_array *cartelBroadcastVars;
//extern ctl_admin_broadcast_vars_array *cartelBroadcastVars;

PLAYER_INDEX has_interior_been_generated_by_another_cartel_user(int roomKey); //-1 if false
void mark_interior_as_generated(int roomKey);

extern search_results_array *searchResults; //this is seriously gonna need caretl_memory.sco 3640bytes //910 ints worth.
//point to 0x830FB110 //10 in total.

extern int onlineScoreBitSet;
extern OBJECT_INDEX myNeonCarObjects[3];
extern VEHICLE_INDEX bladesHeli;
void netPlayerVehAttachTo(int* attach_index, int playerIdIfNot0Minus1);
void updateOnlineBitSet(int unused0, int unused1);
void cartelNetworkDisplayOptions(int unused1, int unused2);
char* cartel_user_type_string(int usertype);
eCartelUserType GetCartelUserType(PLAYER_INDEX player);
bool IsPlayerValid(PLAYER_INDEX playerIndex);
bool IsPlayerOnFriendsList(PLAYER_INDEX PlayerID, bool checkOnlineStatus);
bool VoteKickPlayer(PLAYER_INDEX PlayerID);
void GetCartelSpawnCoordinatesForPlayer(PLAYER_INDEX PlayerID, float *x, float *y, float *z, float *h);
void setMenuAdvertise(); //call after setting the bit menuAdvertise currently inside ctlMem->m_bit_sets.m_player
void PlayerListSubMenu();
void netPlayerAllPlayersMenu();
void netPlayerAttachVehObjs();
void NetworkLoopedOptions();
void netPlayerQuestionableActions();
void NetworkOptUnkickConsole(int index, int unused1);
void netPlayerVehDetach();//CLEAN
void NetworkGameOptions();
bool PlayerIpAddressMatch(PLAYER_INDEX playerId, uint ip0, uint ip1, uint ip2, uint ip3);
void detachLocalPlayerFromVehicleOrWalker();
void SetPedsAppearanceToPlayers(Player playerIndex);
void SpawnPlayerInCloneOfVehicle(Vehicle vehicle);
void WalkWaterToggleScanSelectedPlayerId(int unused1, int unused2);