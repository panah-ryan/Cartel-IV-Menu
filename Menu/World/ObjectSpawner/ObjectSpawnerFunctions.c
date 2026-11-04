#include "ObjectSpawnerFunctions.h"
#include "..\..\World_Options.h"


Object CloneObject(Object obj)
{
    uint model;
    vector3 pos;
    quaternion q;

    GET_OBJECT_MODEL(obj, &model);
    GET_OBJECT_COORDINATES(obj, &pos.x, &pos.y, &pos.z);
    GET_OBJECT_QUATERNION(obj, &q.x, &q.y, &q.z, &q.w);

    Object clone;
    CreateObject(model, pos.x, pos.y, pos.z, 0.0f, &clone, true);

    SET_OBJECT_QUATERNION(clone, q.x, q.y, q.z, q.w);
    return clone;
}

void RotateObjectLocal(Object obj, vector3 localAxis, float degrees)
{
    if (!DOES_OBJECT_EXIST(obj))
        return;

    quaternion current;
    GET_OBJECT_QUATERNION(obj, &current.x, &current.y, &current.z, &current.w);

    vector3 worldAxis = quaternion_rotate_vector(current, localAxis);

    worldAxis = vector3_normalize(worldAxis);

    quaternion delta = CreateFromAxisAngle(worldAxis, degrees);
    quaternion result = quaternion_multiply(delta, current);
    result = quaternion_normalized(result);

    SET_OBJECT_QUATERNION(obj, result.x, result.y, result.z, result.w);
}

void RotateObject(Object obj, vector3 axis, float degrees)
{
    if (!DOES_OBJECT_EXIST(obj))
        return;

    quaternion current;
    GET_OBJECT_QUATERNION(obj, &current.x, &current.y, &current.z, &current.w);

    quaternion rotation = CreateFromAxisAngle(axis, degrees);
    quaternion result = quaternion_multiply(rotation, current);
    result = quaternion_normalized(result);

    SET_OBJECT_QUATERNION(obj, result.x, result.y, result.z, result.w);
}

void PlacementTool(
    SpawnerObject *originalObject,
    int copies,
    float spacing,
    uint alignment,
    float pitch,
    float yaw, //todo: fix yaw and roll are swapped
    float roll,
    float offsetX,
    float offsetY,
    float offsetZ
) {
    if (!DOES_OBJECT_EXIST(originalObject->o))
        return;

    if (copies < 1)
        return;

    if (!CanTakeObjectAmount(copies)) {
        printf("~r~Spawner: ~w~Object list full! Delete some objects first.");
        return;
    }

    uint model;
    GET_OBJECT_MODEL(originalObject->o, &model);

    vector3 minDim, maxDim;
    GET_MODEL_DIMENSIONS(model, &minDim, &maxDim);

    if (model == 1078148491) // CJ_BOAT_RAMP Fix
    {
        minDim.x = -3.25473f;
        minDim.y = -10.112198f;
        minDim.z = -0.94579f;

        maxDim.x = 3.467804f;
        maxDim.y = 1.099365f;
        maxDim.z = 2.20252f;
    }

    vector3 objSize = {
        maxDim.x - minDim.x,
        maxDim.y - minDim.y,
        maxDim.z - minDim.z
    };

    vector3 alignmentVec = {0.0f, 0.0f, 0.0f};
    switch (alignment)
    {
        case 0: alignmentVec.y = 1.0f; break; // Front
        case 1: alignmentVec.x = 1.0f; break; // Right
        case 2: alignmentVec.y = -1.0f; break; // Back
        case 3: alignmentVec.x = -1.0f; break; // Left
        case 4: alignmentVec.z = 1.0f; break; // Top
        case 5: alignmentVec.z = -1.0f; break; // Bottom
        default: alignmentVec.y = 1.0f; break;
    }

    vector3 stepOffset = {
        alignmentVec.x * objSize.x * spacing,
        alignmentVec.y * objSize.y * spacing,
        alignmentVec.z * objSize.z * spacing
    };

    vector3 scaledOffset = {
        offsetX * objSize.x,
        offsetY * objSize.y,
        offsetZ * objSize.z
    };

    quaternion baseQuat;
    GET_OBJECT_QUATERNION(originalObject->o, &baseQuat.x, &baseQuat.y, &baseQuat.z, &baseQuat.w);
    vector3 currentRot = quaternion_to_euler(baseQuat);

    Object previousObject = originalObject->o;

    for (int i = 0; i < copies; i++)
    {
        Object newCopy = CloneObject(previousObject);
        int id;
        if (!DOES_OBJECT_EXIST(newCopy) || !AddObjectToSpawnerList(newCopy, originalObject->name, &id)) {
            printf("~r~Spawner: ~w~Failed to add object to list. Exiting placement tool.");
            return;
        }

        currentRot.x += pitch;
        currentRot.y += yaw;
        currentRot.z += roll;

        vector3 worldPos;
        GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(
            previousObject,
            stepOffset.x + scaledOffset.x,
            stepOffset.y + scaledOffset.y,
            stepOffset.z + scaledOffset.z,
            &worldPos.x, &worldPos.y, &worldPos.z
        );

        SET_OBJECT_COORDINATES(newCopy, worldPos.x, worldPos.y, worldPos.z);
        SET_OBJECT_ROTATION(newCopy, currentRot.x, currentRot.y, currentRot.z);

        previousObject = newCopy;

        vector3 radius = {
            objSize.x * 0.5f,
            objSize.y * 0.5f,
            objSize.z * 0.5f
        };

        float offsetY_Pitch = radius.y * (1.0f - COS(pitch));
        float offsetZ_Pitch = radius.y * SIN(pitch);

        float offsetX_Yaw = radius.x * (1.0f - COS(yaw));
        float offsetY_Yaw = radius.x * SIN(yaw);

        float offsetX_Roll = radius.z * SIN(roll);
        float offsetZ_Roll = radius.z * (1.0f - COS(roll));

        float totalOffsetX = offsetX_Yaw + offsetX_Roll;
        float totalOffsetY = offsetY_Pitch + offsetY_Yaw;
        float totalOffsetZ = offsetZ_Pitch + offsetZ_Roll;

        GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(
            previousObject,
            totalOffsetX * spacing,
            totalOffsetY * spacing,
            totalOffsetZ * spacing,
            &worldPos.x, &worldPos.y, &worldPos.z
        );

        SET_OBJECT_COORDINATES(newCopy, worldPos.x, worldPos.y, worldPos.z);
    }
}

void SpawnObjectForSpawner(uint objModel, char *name, bool suppressKeyboardBuffer)
{
    if (!IS_MODEL_IN_CDIMAGE(objModel) || IS_THIS_MODEL_A_PED(objModel))
    {
        if (suppressKeyboardBuffer)
            printf("~r~Spawner: ~w~Model 0x%x not found or invalid.", objModel);
        else
            printf("~r~Spawner: ~w~Model %s (0x%x) not found or invalid.", ctlMem->m_char_buffers.keyboard_input_buffer, objModel);
        return;
    }

    vector3 modelMin, modelMax;
    GET_MODEL_DIMENSIONS(objModel, &modelMin, &modelMax);

    float spawnX, spawnY, spawnZ;
    GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(myPlayerPed, 0.0f, modelMax.y + 5.0f, modelMax.z, &spawnX, &spawnY, &spawnZ);

    if (!CanTakeObjectAmount(1)) //needs to check network pools.
    {
        printf("~r~Spawner: ~w~Object list full! Delete some objects first.");
        return;
    }

    Object spawnedObject;
    if (!CreateObject(objModel, spawnX, spawnY, spawnZ, 0.0f, &spawnedObject, true))
    {
        printf("~r~Spawner: ~w~Failed to create object.");
        return;
    }

    SET_OBJECT_VISIBLE(spawnedObject, !bitTest(ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_INVISIBLE));
    SET_OBJECT_INVINCIBLE(spawnedObject, bitTest(ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_INVINCIBLE));
    SET_OBJECT_DYNAMIC(spawnedObject, bitTest(ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_DYNAMIC));
    SET_OBJECT_COLLISION(spawnedObject, bitTest(ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_COLLISION));
    //FREEZE_OBJECT_POSITION_AND_DONT_LOAD_COLLISION(spawnedObject, bitTest(ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_FROZEN));
    FREEZE_OBJECT_POSITION(spawnedObject, bitTest(ctlMem->m_bit_sets.m_obj_spawner, PREF_OBJ_SPAWNER_FROZEN));

    int newIndex;
    if (!AddObjectToSpawnerList(spawnedObject, name, &newIndex))
    {
        printf("~r~Spawner: ~w~Failed to add object to list.");
        return;
    }
    printf("~g~Spawner: ~w~Object spawned successfully.");
}

void SpawnObjectFromHex()
{
    if (!showAndGetKeyboardInput("Spawn Object Model (Hex)", true, "0x", 10, 0, 0.5f, 0.5f, 200, 0, 0))
        return;

    uint objModel = HexToIntNath(ctlMem->m_char_buffers.keyboard_input_buffer);
    SpawnObjectForSpawner(objModel, ctlMem->m_char_buffers.keyboard_input_buffer,  0);
}

void SpawnObjectFromNameOrDecimal()
{
    if (!showAndGetKeyboardInput("Spawn Object Model (Name or Decimal)", false, "", 40, 0, 0.5f, 0.5f, 200, 0, 0))
        return;

    uint objModel;
    if (STRING_TO_INT(ctlMem->m_char_buffers.keyboard_input_buffer, (int*)&objModel))
    {
        SpawnObjectForSpawner(objModel, ctlMem->m_char_buffers.keyboard_input_buffer, 0);
        return;
    }

    const int len = GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer);
    if (len > 6)
    {
        const char* prefix = GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 0, 6);
        if (COMPARE_STRING(prefix, "MODEL_") || COMPARE_STRING(prefix, "model_") || COMPARE_STRING(prefix, "MODEL ") || COMPARE_STRING(prefix, "model "))
        {
            const char* cleaned = GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 6, len);
            strcpy(ctlMem->m_char_buffers.keyboard_input_buffer, cleaned, 96);
        }
    }
    if (len > 7)
    {
        const char* prefix = GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 0, 7);
        if (COMPARE_STRING(prefix, "OBJECT_") || COMPARE_STRING(prefix, "object_") || COMPARE_STRING(prefix, "OBJECT ") || COMPARE_STRING(prefix, "object "))
        {
            const char* cleaned = GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 7, len);
            strcpy(ctlMem->m_char_buffers.keyboard_input_buffer, cleaned, 96);
        }
    }

    for (int i = 0; i < len; i++)
    {
        if (ctlMem->m_char_buffers.keyboard_input_buffer[i] == ' ')
            ctlMem->m_char_buffers.keyboard_input_buffer[i] = '_';
    }

    objModel = GET_HASH_KEY(ctlMem->m_char_buffers.keyboard_input_buffer);
    SpawnObjectForSpawner(objModel, ctlMem->m_char_buffers.keyboard_input_buffer, 0);
}

void SnapSelectedObjectToGround(Object obj)
{
    if (!DOES_OBJECT_EXIST(obj))
        return;

    uint model;
    vector3 minDim, maxDim;
    vector3 pos;
    float groundZ;

    GET_OBJECT_MODEL(obj, &model);
    GET_MODEL_DIMENSIONS(model, &minDim, &maxDim);
    GET_OBJECT_COORDINATES(obj, &pos.x, &pos.y, &pos.z);
    GET_GROUND_Z_FOR_3D_COORD(pos.x, pos.y, pos.z, &groundZ);

    float objectHeight = maxDim.z - minDim.z;
    float halfHeight = objectHeight * 0.5f;

    SET_OBJECT_COORDINATES(obj, pos.x, pos.y, groundZ + halfHeight);
}

void AttachObject(Object obj)
{
    if (!DOES_OBJECT_EXIST(obj))
        return;

    if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
    {
        Vehicle car;
        GET_CAR_CHAR_IS_USING(myPlayerPed, &car);
        if (DOES_VEHICLE_EXIST(car))
        {
            ATTACH_OBJECT_TO_CAR(obj, car, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            FREEZE_OBJECT_POSITION(obj, true);
            printf("Spawner: Object attached to car");
        }
    }
    else
    {
        ATTACH_OBJECT_TO_PED(obj, myPlayerPed, BONE_HEAD, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
        FREEZE_OBJECT_POSITION(obj, true);
        SET_CHAR_VISIBLE(myPlayerPed, true);
        printf("Spawner: Object attached to player");
    }
}

bool IsStickInputSignificant(float x, float y) {
    return (ABSF(x) > STICK_DEADZONE || ABSF(y) > STICK_DEADZONE);
}