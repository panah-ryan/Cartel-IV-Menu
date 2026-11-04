#pragma once

#include "ObjectSpawnerFunctions.h"
#include "ObjectSpawnerGlobals.h"
#include "ObjectSpawnerRepository.h"


 void Action_SpawnSelectedObject(int unk1, int unk2);
 void Action_SpawnSelectedObjectAlt(int unk1, int unk2);
 void Action_EditSelectedObject(int index, int unk2);
 void Action_PlacementTool(int index, int unk2);
 void Action_PlacementToolPreview(int index, int unk2);
 void Action_DeleteLastSpawnedObjects(int unk1, int unk2);

void Action_SetSelectedObjectFrozen(int index, int unk2);
void Action_SetSelectedObjectDynamic(int index, int unk2);
void Action_SetSelectedObjectCollision(int index, int unk2);
void Action_SetSelectedObjectVisible(int index, int unk2);
void Action_SetSelectedObjectInvincible(int index, int unk2);
void Action_SetSelectedObjectPickupable(int index, int unk2);
void Action_GiveSelectedObjectToPlayer(int index, int unk2);
void Action_SetSelectedObjectNonFrozen(int index, int unk2);
void Action_SetSelectedObjectNonDynamic(int index, int unk2);
void Action_SetSelectedObjectNonCollision(int index, int unk2);
void Action_SetSelectedObjectNonVisible(int index, int unk2);
void Action_SetSelectedObjectNonInvincible(int index, int unk2);

void Reset_spacing();
void Increase_spacing();
void Decrease_spacing();

void Reset_pitch();
void Increase_pitch();
void Decrease_pitch();

void Reset_roll();
void Increase_roll();
void Decrease_roll();

void Reset_yaw();
void Increase_yaw();
void Decrease_yaw();

void Reset_offsetX();
void Increase_offsetX();
void Decrease_offsetX();

void Reset_offsetY();
void Increase_offsetY();
void Decrease_offsetY();

void Reset_offsetZ();
void Increase_offsetZ();
void Decrease_offsetZ();