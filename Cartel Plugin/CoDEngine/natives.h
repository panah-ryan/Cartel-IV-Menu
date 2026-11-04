#pragma once
#include "caller.h"

extern Caller* call;

static BOOL IS_THIS_MODEL_A_PED(unsigned int model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_PED", model_hash); }
static BOOL IS_THIS_MODEL_A_VEHICLE(unsigned int model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_VEHICLE", model_hash); }
static BOOL IS_THIS_MODEL_A_BIKE(unsigned int model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_BIKE", model_hash); }
static BOOL IS_THIS_MODEL_A_BOAT(unsigned int model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_BOAT", model_hash); }
static BOOL IS_THIS_MODEL_A_CAR(unsigned int model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_CAR", model_hash); }
static BOOL IS_THIS_MODEL_A_HELI(unsigned int model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_HELI", model_hash); }
static BOOL IS_THIS_MODEL_A_TRAIN(unsigned int model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_TRAIN", model_hash); }
static void GET_DISTANCE_BETWEEN_COORDS_3D(float x1, float y1, float z1, float x2, float y2, float z2, float* pDist) { call->call<int>("GET_DISTANCE_BETWEEN_COORDS_3D", x1, y1, z1, x2, y2, z2, pDist); }
static void GET_CHAR_COORDINATES(int ped, float* pX, float* pY, float* pZ) { call->call<int>("GET_CHAR_COORDINATES", ped, pX, pY, pZ); }
static void GET_PLAYER_CHAR(int playerIndex, int* pPed) { call->call<int>("GET_PLAYER_CHAR", playerIndex, pPed); }
static void CLEAR_PRINTS() { call->call<int>("CLEAR_PRINTS"); }
static void PRINT_STRING_WITH_LITERAL_STRING_NOW(const char* gxtName, const char* text, int timeMS, int color) { call->call<int>("PRINT_STRING_WITH_LITERAL_STRING_NOW", gxtName, text, timeMS, color); }
static BOOL IS_SCREEN_FADED_OUT() { return call->call<BOOL>("IS_SCREEN_FADED_OUT"); }
static int GET_CURRENT_EPISODE() { return call->call<int>("GET_CURRENT_EPISODE"); }
static int GET_PLAYER_ID() { return call->call<int>("GET_PLAYER_ID"); }
static int GET_HOST_ID() { return call->call<int>("GET_HOST_ID"); }
static char* GET_PLAYER_NAME(int playerIndex) { return call->call<char*>("GET_PLAYER_NAME", playerIndex); }
static UINT GET_HASH_KEY(const char* key) { return call->call<UINT>("GET_HASH_KEY", key); }
static BOOL DOES_OBJECT_EXIST_WITH_NETWORK_ID(int networkId) { return call->call<BOOL>("DOES_OBJECT_EXIST_WITH_NETWORK_ID", networkId); }
static void GET_OBJECT_FROM_NETWORK_ID(int networkId, int* object) { call->call<int>("GET_OBJECT_FROM_NETWORK_ID", networkId, object); }
static void ADD_TO_PREVIOUS_BRIEF(const char* string) { call->call<int>("ADD_TO_PREVIOUS_BRIEF", string); }
static void SET_OBJECT_LIGHTS(int obj, bool lights) { call->call<int>("SET_OBJECT_LIGHTS", lights); }
static void SET_OBJECT_COLLISION(int obj, bool collision) { call->call<int>("SET_OBJECT_COLLISION", collision); }

static BOOL IS_NETWORK_SESSION() { return call->call<BOOL>("IS_NETWORK_SESSION"); }

static BOOL NETWORK_JOIN_GAME(int session_index) { return call->call<BOOL>("NETWORK_JOIN_GAME", session_index); }
static BOOL NETWORK_JOIN_GAME_PENDING() { return call->call<BOOL>("NETWORK_JOIN_GAME_PENDING"); }

static void NETWORK_KICK_PLAYER(int player_index) { call->call<int>("NETWORK_KICK_PLAYER", player_index); }

static void REQUEST_MODEL(UINT32 model) { call->call<int>("REQUEST_MODEL", model); }
static BOOL HAS_MODEL_LOADED(UINT32 model) { return call->call<BOOL>("HAS_MODEL_LOADED", model); }
static void MARK_MODEL_AS_NO_LONGER_NEEDED(UINT32 model) { call->call<int>("MARK_MODEL_AS_NO_LONGER_NEEDED", model); }
static UINT32 GET_PLAYERSETTINGS_MODEL_CHOICE() { return call->call<UINT32>("GET_PLAYERSETTINGS_MODEL_CHOICE"); }
static void CHANGE_PLAYER_MODEL(int playerIndex, UINT32 model) { call->call<int>("CHANGE_PLAYER_MODEL", playerIndex, model); }
static void SET_PLAYERSETTINGS_MODEL_VARIATIONS_CHOICE(int playerIndex) { call->call<int>("SET_PLAYERSETTINGS_MODEL_VARIATIONS_CHOICE", playerIndex); }

static void REQUEST_SCRIPT(char* scriptName) { call->call<int>("REQUEST_SCRIPT", scriptName); } //will need to set active script thread to main.sco or we'll get yeeted.
static BOOL HAS_SCRIPT_LOADED(char* scriptName) { return call->call<BOOL>("HAS_SCRIPT_LOADED", scriptName); }
static int START_NEW_SCRIPT(char* scriptName, int stacksize) { return call->call<int>("START_NEW_SCRIPT", scriptName, stacksize); }
static void MARK_SCRIPT_AS_NO_LONGER_NEEDED(char* scriptName) { call->call<int>("MARK_SCRIPT_AS_NO_LONGER_NEEDED", scriptName); }
static void TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(char* script_name) { call->call<int>("TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME", script_name); }

static void SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME() { call->call<int>("SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME"); }