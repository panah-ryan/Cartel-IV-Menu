#pragma once

#include "..\..\stdafx.h"
#include "..\..\maths.h"
#include "..\..\Functions.h"

#include "ObjectSpawnerRepository.h"
#include "ObjectSpawnerConstants.h"


bool CreateObjectLocally(uint model, float x, float y, float z, float h, OBJECT_INDEX* object);
bool CreateObject(uint model, float x, float y, float z, float h, OBJECT_INDEX* object, bool lockNetControl);

void RotateObjectLocal(Object obj, vector3 localAxis, float degrees);
void RotateObject(Object obj, vector3 axis, float degrees);

void PlacementTool(
    SpawnerObject *originalObject,
    int copies,
    float spacing,
    uint alignment,
    float pitch,
    float yaw,
    float roll,
    float offsetX,
    float offsetY,
    float offsetZ
);

void SpawnObjectForSpawner(uint objModel, char *name, bool suppressKeyboardBuffer);
void SpawnObjectFromHex(void);
void SpawnObjectFromNameOrDecimal(void);
Object CloneObject(Object obj);
void SnapSelectedObjectToGround(Object obj);
void AttachObject(Object obj);
bool IsStickInputSignificant(float x, float y);