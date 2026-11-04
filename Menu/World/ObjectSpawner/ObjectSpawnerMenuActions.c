#include "ObjectSpawnerMenuActions.h"
#include "ObjectSpawnerCore.h"
#include "ObjectSpawnerRepository.h"
#include "ObjectSpawnerGlobals.h"
#include "../../MenuExecutionHandling.h"

void Action_SpawnSelectedObject(int unk1, int unk2) 
{
    const char* item = GetCurrentMenuItemText();
    SpawnObjectForSpawner(GET_HASH_KEY(item), (char*)GetCurrentMenuItemText(), true);
    EnterObjectSpawnerMode();

}

void Action_SpawnSelectedObjectAlt(int unk1, int unk2)
{
    const char* item = GetCurrentMenuItemDesc();
    SpawnObjectForSpawner(GET_HASH_KEY(item), (char*)GetCurrentMenuItemText(), true);
    EnterObjectSpawnerMode();

}

void Action_EditSelectedObject(int index, int unk2) 
{
    SetSelectedObjectIndex(index);
    EnterObjectSpawnerMode();
}

void Action_PlacementTool(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    PlacementTool(
        obj,
        placementToolConfig.copies,
        placementToolConfig.spacing,
        placementToolConfig.alignment,
        placementToolConfig.pitch,
        placementToolConfig.roll,
        placementToolConfig.yaw,
        placementToolConfig.offsetX,
        placementToolConfig.offsetY,
        placementToolConfig.offsetZ
    );
}

void Action_PlacementToolPreview(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    PlacementTool(
        obj,
        placementToolConfig.copies,
        placementToolConfig.spacing,
        placementToolConfig.alignment,
        placementToolConfig.pitch,
        placementToolConfig.roll,
        placementToolConfig.yaw,
        placementToolConfig.offsetX,
        placementToolConfig.offsetY,
        placementToolConfig.offsetZ
    );
    WAIT(5000);
    DeleteLastSpawnedObjects(placementToolConfig.copies);
}

void Action_DeleteLastSpawnedObjects(int index, int unk2)
{
    printf("Delete called with %i items", selectedItemsToDelete);
    DeleteLastSpawnedObjects(selectedItemsToDelete);
    RefreshCurrentPage(0, 0);
}

void Action_SetSelectedObjectFrozen(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    FREEZE_OBJECT_POSITION(obj->o, true);
}

void Action_SetSelectedObjectDynamic(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_DYNAMIC(obj->o, true);
}

void Action_SetSelectedObjectCollision(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_COLLISION(obj->o, true);
}

void Action_SetSelectedObjectVisible(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_VISIBLE(obj->o, true);
}

void Action_SetSelectedObjectInvincible(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_INVINCIBLE(obj->o, true);
}

void Action_SetSelectedObjectPickupable(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_AS_STEALABLE(obj->o, true);
}

void Action_GiveSelectedObjectToPlayer(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_AS_STEALABLE(obj->o, true);
    ALLOW_PLAYER_TO_CARRY_NON_MISSION_OBJECTS(myPlayerIndex, true);
    GIVE_PED_PICKUP_OBJECT(myPlayerPed, obj->o, true);
}

void Action_SetSelectedObjectNonFrozen(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    FREEZE_OBJECT_POSITION(obj->o, false);
}

void Action_SetSelectedObjectNonDynamic(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_DYNAMIC(obj->o, false);
}

void Action_SetSelectedObjectNonCollision(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_COLLISION(obj->o, false);
}

void Action_SetSelectedObjectNonVisible(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_VISIBLE(obj->o, false);
}

void Action_SetSelectedObjectNonInvincible(int index, int unk2)
{
    SpawnerObject* obj = GetSpawnerObjectFromIndex(index);
    if (!DOES_OBJECT_EXIST(obj->o)) return;
    SET_OBJECT_INVINCIBLE(obj->o, false);
}

void Reset_spacing() { placementToolConfig.spacing = 1.0f; }
void Increase_spacing() { placementToolConfig.spacing += 1.0f; }
void Decrease_spacing() { placementToolConfig.spacing -= 1.0f; }

void Reset_pitch() { placementToolConfig.pitch = 0.0f; }
void Increase_pitch() { placementToolConfig.pitch += 1.0f; }
void Decrease_pitch() { placementToolConfig.pitch -= 1.0f; }

void Reset_roll() { placementToolConfig.roll = 0.0f; }
void Increase_roll() { placementToolConfig.roll += 1.0f; }
void Decrease_roll() { placementToolConfig.roll -= 1.0f; }

void Reset_yaw() { placementToolConfig.yaw = 0.0f; }
void Increase_yaw() { placementToolConfig.yaw += 1.0f; }
void Decrease_yaw() { placementToolConfig.yaw -= 1.0f; }

void Reset_offsetX() { placementToolConfig.offsetX = 0.0f; }
void Increase_offsetX() { placementToolConfig.offsetX += 1.0f; }
void Decrease_offsetX() { placementToolConfig.offsetX -= 1.0f; }

void Reset_offsetY() { placementToolConfig.offsetY = 0.0f; }
void Increase_offsetY() { placementToolConfig.offsetY += 1.0f; }
void Decrease_offsetY() { placementToolConfig.offsetY -= 1.0f; }

void Reset_offsetZ() { placementToolConfig.offsetZ = 0.0f; }
void Increase_offsetZ() { placementToolConfig.offsetZ += 1.0f; }
void Decrease_offsetZ() { placementToolConfig.offsetZ -= 1.0f; }