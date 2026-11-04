#pragma once

#include "stdafx.h"
#include "maths.h"
#include "Functions.h"

/*
typedef enum
{
    MISSILE_HEATSEEKER,
    MISSILE_HOMING,
    MISSILE_TV,
    MISSILE_BOMB,
} MissileMode;
*/
/*
extern int bullshit;
extern int bullshit2;

extern float missile_burn_time;
extern float missile_climb_time;
extern float missile_proximity_fuse;
extern float missile_velocity;
extern bool missile_flame_trail;
extern bool missile_equip_with_cam;
extern float missile_cam_offset;
*/
void CreateAndGiveMilitaryMissile(MissileMode mode, int unk2);
void MissileLoop();
void MissilePrefillTarget();

//extern char* WarheadTypeStrings[4];
//extern char* MissileChassisStrings[4];
//extern char* LinkModeStrings[3];
//extern EXPLOSION_TAG WarheadTypes[4];
//extern uint MissileChassisInts[3];

//extern int selectedMissileChassis;
//extern int selectedExplosionType;
//extern int selectedLinkMode;