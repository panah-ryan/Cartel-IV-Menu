// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#define Developer
//#define Moderator


#ifdef Developer
#define Moderator
#define Cartel_Server_Included
#endif

#ifdef Moderator
#define Cartel_Server_Included
#endif

#define Cartel_Server_Included

#pragma once

#include <xtl.h>
#include <xboxmath.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <map>
#include "XConfig.h"
#include <xkelib.h>
#include <winnt.h>
#include <string>
#include <vector>
#include <stack>
#include <tuple>

struct vector3
{
	float x;
	float y;
	float z;
};

enum CTL_ATTACK_TYPES
{
	ATTACK_OUT_OF_BOUNDS_ARRAY,
	ATTACK_GROUP_HACK,
	ATTACK_CLONE_SPAM, //object type
	ATTACK_CLONE_PLAYER, //player index they sent
	ATTACK_CLONE_PED, //model hash
	ATTACK_CLONE_DUMMYPED, //model hash
	ATTACK_CLONE_CAR, //model hash
	ATTACK_CLONE_OBJECT, //model hash
	ATTACK_CLONE_BIKE, //model hash
	ATTACK_CLONE_TRAIN, //model hash
	ATTACK_CLONE_HELI, //model hash
	ATTACK_CLONE_BOAT, //model hash
	ATTACK_CLONE_PLANE, //model hash
	ATTACK_EVENT_REQUEST_PICKUP,
	ATTACK_EVENT_RESURRECT,
	ATTACK_EVENT_REMOVE_WEAPON,
	ATTACK_EVENT_EXPLOSION,
	ATTACK_EVENT_WANTED_LEVEL,
	ATTACK_EVENT_CREATE_PICKUP,
	ATTACK_EVENT_RADIO_STATION,
	ATTACK_TELL_NET,
	ATTACK_PEER_DATA,
	ATTACK_REASSIGN,
	ATTACK_PLAYER_NAME,
	ATTACK_SYNC_ATTACHMENT, //network id of attachment
	ATTACK_SYNC_ANIM_TASK, //network id
	ATTACK_SYNC_RELATIONSHIP_GROUP, //network id
	ATTACK_SYNC_TASK_QUERY,
	ATTACK_SYNC_FREEZE_GUN, //if ped network id
	ATTACK_SYNC_WEAPON_INDEX, //network id
	ATTACK_SYNC_ANIMATION_GROUP, //network id
	ATTACK_SYNC_MODEL_CHANGE, //network id
	ATTACK_SINGLEPLAYER_KICK,
	ATTACK_SYNC_INVALID_WEAPON_THROW,
	ATTACK_PLAIN_KICK_RESPONSE,
	ATTACK_COUNT
};

extern std::vector<std::tuple<int, CTL_ATTACK_TYPES, DWORD>> attacker_list;

#define MODDER_DB_DAT_PATH "GAME:\\Cartel\\data\\modder_db.dat"
#define PLAYER_ALIAS_FILE "GAME:\\Cartel\\data\\aliases_db.dat"

#include "helpers.h"