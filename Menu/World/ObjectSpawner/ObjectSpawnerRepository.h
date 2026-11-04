#pragma once

#include "..\..\stdafx.h"
#include "ObjectSpawnerConstants.h"
#include "ObjectSpawnerCamera.h"
#include "ObjectSpawnerFunctions.h"

SpawnerObject* GetSpawnerObjectsArray(void);
SpawnerObject* GetSpawnerObjectFromIndex(int index);
void CreateSortedSpawnerObjectView(const SpawnerObject* sourceArray, short* outIndices, int* outCount);
bool DoesObjectWithArrayIndexExist(int index);
Object GetSelectedObject();
SpawnerObject* GetSelectedSpawnerObject();
void SetSelectedObjectIndex(int newIndex);
bool CanTakeObjectAmount(int amount);

bool AddObjectToSpawnerList(Object objToAdd, const char *name, int* obj_array_index);
bool CloneSelectedObject();
void DeleteLastSpawnedObjects(int count);

int GetNextObjectIndex(bool upwards);
int GetSpawnedObjectCount();
int GetSelectedObjectArrayIndex();
void DeleteObjectAtIndex(int index);