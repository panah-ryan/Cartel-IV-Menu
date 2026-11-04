#pragma once

#define MAX_INTERIOR_PEDS 3

typedef struct ctl_interior_peds
{
	PED_INDEX ped[MAX_INTERIOR_PEDS];
	int ped_count;
	uint interior_key;
}ctl_interior_peds;

VEHICLE_SEAT GetPedSeatIndex(PED_INDEX target_ped);
bool CreatePed(uint model, float x, float y, float z, float h, PED_INDEX *spawnedPed, bool invincible, bool lockNetControl);
void LivenMPInteriorsLoop();
void PlayPedAnimations(char* animSet, char* animName);
VEHICLE_SEAT GetPedSeatIndex(PED_INDEX target_ped);
void pedAnimationOptions();

typedef enum INTERIOR_KEYS
{
	TRIANGLE_CLUB_MAIN = -2096494209,
	TRIANGLE_CLUB_EXITWAY = 889160557,
	TRIANGLE_CLUB_ENTRANCE = -933277074,
	HONKERS_ENTRANCE = -1333686044,
	HONKERS_LOBBY = -534648211,
	HONKERS_MAIN = -1634702078,
	HONKERS_BACKROOM = 713594162,
} INTERIOR_KEYS;