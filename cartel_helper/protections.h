#pragma once
#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "common.h"
#include "cartel_helper.h"

#define KICK_REASON_GROUP_HACK "Kidnap/Group Hack freeze"
#define KICK_REASON_CLONE_PLAYER "Invalid PlayerID freeze"

extern uint last_player_model;
void cartel_protections();
extern int money_amount_ped_prot_marker;
//char* bad_player_model_name(uint playerModel, bool *cutScene);
extern int reset_attack_broadcast_tick;
extern int player_valid_tick[16]; //can easily move to ctlMem should we need too.. plenty of space for now.

typedef enum eObjectProcess
{
	OBJECT_PROCESS_SKIP = 0,
	OBJECT_PROCESS_ATTACHED = 1,
	OBJECT_PROCESS_LAGLIGHT = 2,
	OBJECT_PROCESS_ATTACHED_LAGLIGHT = 3,
	OBJECT_PROCESS_ATTACHED_TO_MY_VEH = 4,
	OBJECT_PROCESS_OBJ_FREZ_VEHICLE = 5, //PC & PS3 only
	OBJECT_PROCESS_PREVIOUS_LAGLIGHT = 6,
	OBJECT_PROCESS_COLLISION_LAG_OBJ = 7, //Nathan define only (will only work with RGH/JTag)
	OBJECT_PROCESS_NEARBY_LAGLIGHT
	//NeverTiredTest = 7,
}eObjectProcess;