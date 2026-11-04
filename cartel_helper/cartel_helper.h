#pragma once
#include "../Menu/Functions.h"

extern ctl_memory_sco *ctlMem;
extern stickyFreeMemory *freeMem;

extern PLAYER_INDEX myPlayerIndex; //set once every loop
extern PED_INDEX myPlayerPed; //set once every loop
extern int myCurrentEpisode;
extern bool NetworkGame;
extern bool CartelXEXPresent;