#include "ObjectSpawnerRepository.h"

#define spawnerObjects spawnerMem
static int selectedObjectIndex = -1;

SpawnerObject* GetSpawnerObjectsArray(void) {
	return spawnerObjects;
}

void CreateSortedSpawnerObjectView(const SpawnerObject* sourceArray, short* outIndices, int* outCount)
{
    int count = 0;

    for (int i = 0; i < MAX_SPAWNER_OBJECTS; i++) {
        if (DOES_OBJECT_EXIST(sourceArray[i].o)) {
            //outIndices[count++] = i;
            outIndices[count] = (short)i;
            count++;
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (sourceArray[outIndices[i]].creationTime > sourceArray[outIndices[j]].creationTime) {
                short tmp = outIndices[i];
                outIndices[i] = outIndices[j];
                outIndices[j] = tmp;
            }
        }
    }

    if (outCount)
        *outCount = count;
}

bool DoesObjectWithArrayIndexExist(int index)
{
    return (index >= 0 && index < MAX_SPAWNER_OBJECTS && DOES_OBJECT_EXIST(spawnerObjects[index].o));
}

SpawnerObject* GetSpawnerObjectFromIndex(int index)
{
    return &spawnerObjects[index];
}

Object GetSelectedObject()
{
    if (!DoesObjectWithArrayIndexExist(selectedObjectIndex))
        return 0;
    return spawnerObjects[selectedObjectIndex].o;
}

SpawnerObject* GetSelectedSpawnerObject()
{
    if (!DoesObjectWithArrayIndexExist(selectedObjectIndex))
        return nullptr;
    return &spawnerObjects[selectedObjectIndex];
}

int GetSelectedObjectArrayIndex()
{
    return selectedObjectIndex;
}

void SetSelectedObjectIndex(int newIndex)
{
    selectedObjectIndex = newIndex;
    SetSpawnerCameraTarget(GetSelectedObject());
}

bool CanTakeObjectAmount(int amount)
{
    return GetSpawnedObjectCount() + amount <= MAX_SPAWNER_OBJECTS;
}

int GetSpawnedObjectCount()
{
    int count = 0;
    for (int i = 0; i < MAX_SPAWNER_OBJECTS; i++)
    {
        if (DOES_OBJECT_EXIST(spawnerObjects[i].o))
            count++;
    }
    return count;
}

bool AddObjectToSpawnerList(Object objToAdd, const char *name, int* obj_array_index)
{
    if (!CanTakeObjectAmount(1))
        return false;

    for (int i = 0; i < MAX_SPAWNER_OBJECTS; i++)
    {
        if (!DOES_OBJECT_EXIST(spawnerObjects[i].o))
        {
            spawnerObjects[i].o = objToAdd;
            GET_GAME_TIMER(&spawnerObjects[i].creationTime);

            if(!IS_STRING_NULL(name) && GET_LENGTH_OF_LITERAL_STRING(name) > 0)
            {
                strcpy(spawnerObjects[i].name, name, 32);
                spawnerObjects[i].name[31] = '\0';
            }
            else
            {
                strcpy(spawnerObjects[i].name, "Unnamed Object", 32);
                spawnerObjects[i].name[31] = '\0';
            }

            SetSelectedObjectIndex(i);
            if (obj_array_index) *obj_array_index = i;

            SET_OBJECT_INVINCIBLE(objToAdd, bitTest(ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_INVINCIBLE));
            SET_OBJECT_COLLISION(objToAdd, bitTest(ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_COLLISION));

            return true;
        }
    }

    return false;
}

int GetNextObjectIndex(bool upwards)
{
    int count = 0;
    SpawnerObject* objectsArray = GetSpawnerObjectsArray();
    short sortedIndices[300];
    CreateSortedSpawnerObjectView(objectsArray, sortedIndices, &count);
    Object current = GetSelectedObject();

    for (int i = 0; i < count; i++)
    {
        int index = sortedIndices[i];
        if (objectsArray[index].o == current)
        {
            int next = upwards ? (i + 1) % count : (i - 1 + count) % count;
            return sortedIndices[next];
        }
    }

    return -1;
}

void DeleteLastSpawnedObjects(int count)
{
    if (count <= 0)
        return;

    int total = 0;
    SpawnerObject* objectsArray = GetSpawnerObjectsArray();
    short sortedIndices[300];
    CreateSortedSpawnerObjectView(objectsArray, sortedIndices, &total);

    for (int i = total - 1; i >= 0 && count > 0; i--)
    {
        int index = sortedIndices[i];
        if (DOES_OBJECT_EXIST(objectsArray[index].o))
        {
            DeleteObjectAtIndex(index);
            count--;
        }
    }
}

void DeleteObjectAtIndex(int index) {
	if (DoesObjectWithArrayIndexExist(index)) {
		DeleteObject(&spawnerObjects[index].o);
    	//spawnerObjects[index] = (SpawnerObject){0};
        selectedObjectIndex = -1;
	}
}

bool CloneSelectedObject()
{
    if (!CanTakeObjectAmount(1))
        return false;

    SpawnerObject* obj = GetSelectedSpawnerObject();
    if (!DOES_OBJECT_EXIST(obj->o))
        return false;

    Object clone = CloneObject(obj->o);

    int obj_array_index;
    if (AddObjectToSpawnerList(clone, obj->name, &obj_array_index))
    {
        strcpy(spawnerObjects[obj_array_index].name, spawnerObjects[selectedObjectIndex].name, 32);
        spawnerObjects[obj_array_index].name[31] = '\0';

        SetSelectedObjectIndex(obj_array_index);
        return true;
    }
    return false;
}