#include "ObjectSpawnerCore.h"
#include "..\..\stdafx.h"
#include "..\..\MenuExecutionHandling.h"

static bool spawnerActive = false;
static bool isAlternativeControlMode = false;

float attachedYaw = 0.0f;
float attachedPitch = 0.0f;
float attachedRoll = 0.0f;
float attachedOffsetX = 0.0f;
float attachedOffsetY = 0.0f;
float attachedOffsetZ = 0.0f;


void EnterObjectSpawnerMode()
{
    if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
    {
        Vehicle car;
        GET_CAR_CHAR_IS_USING(myPlayerPed, &car);
        if (DOES_VEHICLE_EXIST(car))
        {
            SET_CAR_FORWARD_SPEED(car, 0);
            FREEZE_CAR_POSITION(car, true);
        }
    }
    else FREEZE_CHAR_POSITION(myPlayerPed, true);

    SET_CHAR_VISIBLE(myPlayerPed, false);
    REMOVE_ALL_CHAR_WEAPONS(myPlayerPed);
    WAIT(100);
    bitReset(&Container.BitSet, PB_IsMenuOpen);
    spawnerActive = true;
    InitializeSpawnerCamera(GetSelectedObject());
}

void ExitObjectSpawnerMode()
{
    spawnerActive = false;
    SetSelectedObjectIndex(-1);
    DestroySpawnerCamera();
    bitSet(&Container.BitSet, PB_IsMenuOpen);
    if(bitTest(ctlMem->m_bit_sets.m_debug, DEBUG_IN_SPAWNER_EDIT_SUBMENUS)) //possibly invert this if i fucked up my logic - Nathan
        UpdateMenuLevel(-1, NULL, NULL); 
    //RefreshCurrentPage(0, 0);
    WAIT(100);
    if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
    {
        Vehicle car;
        GET_CAR_CHAR_IS_USING(myPlayerPed, &car);
        if (DOES_VEHICLE_EXIST(car))
        {
            FREEZE_CAR_POSITION(car, false);
        }
    }
    else FREEZE_CHAR_POSITION(myPlayerPed, false);

    SET_CHAR_VISIBLE(myPlayerPed, !bitTest(ctlMem->m_bit_sets.m_player, Invisibility));
}

void HandleCameraControls() 
{
    int isSpeedMode = IS_BUTTON_PRESSED(0, BUTTON_X) ? 2 : 1;
    int rightX, rightY, nvm;
    GET_POSITION_OF_ANALOGUE_STICKS(0, &nvm, &nvm, &rightX, &rightY);
    
    int inverted = IS_LOOK_INVERTED() ? 1 : -1;

    float scaledRightX = rightX;
    float scaledRightY = rightY * inverted;

    if (isAlternativeControlMode) {
        ZoomSpawnerCamera((scaledRightY * inverted) / (127 / isSpeedMode));
    }
    else {
        RotateSpawnerCamera(
            scaledRightX * ROTATION_SPEED_MULTIPLIER,
            IS_LOOK_INVERTED() ? -scaledRightY * ROTATION_SPEED_MULTIPLIER : scaledRightY * ROTATION_SPEED_MULTIPLIER
        );
    }
}

void HandleAttachedMovement(float scaledStickX, float scaledStickY, float moveSpeedMultiplier)
{
    float moveX = moveSpeedMultiplier * scaledStickX; 
    float moveY = -moveSpeedMultiplier * scaledStickY;

    attachedOffsetX += moveX;
    attachedOffsetY += moveY;
}

void HandleObjectMovement()
{
    if (!DoesObjectWithArrayIndexExist(GetSelectedObjectArrayIndex()))
        return;

    Object obj = GetSelectedObject();
    if (!DOES_OBJECT_EXIST(obj))
        return;

    bool isSpeedUpMode = IS_BUTTON_PRESSED(0, BUTTON_X);
    int stickX, stickY, nvm;
    GET_POSITION_OF_ANALOGUE_STICKS(0, &stickX, &stickY, &nvm, &nvm);
    float scaledStickX = -(float)stickX * STICK_INPUT_SCALE;
    float scaledStickY = -(float)stickY * STICK_INPUT_SCALE;

    float moveSpeedMultiplier = selectedMovementSpeed / 100.0f * MOVEMENT_SPEED_MULTIPLIER;
    if (isSpeedUpMode)
        moveSpeedMultiplier *= 2.0f;

    float yaw = GetSpawnerCameraYaw();
    float pitch = GetSpawnerCameraPitch();

    float forwardX = COS(yaw) * COS(pitch);
    float forwardY = SIN(yaw) * COS(pitch);
    float rightX = COS(yaw + 90.0f);
    float rightY = SIN(yaw + 90.0f);

    bool isAttached = IS_OBJECT_ATTACHED(obj);
    vector3 pos;
    GET_OBJECT_COORDINATES(obj, &pos.x, &pos.y, &pos.z);

    Ped attachedPed = 0;
    Vehicle attachedVehicle = 0;

    if (isAttached)
    {
        float attachX = 0.0f, attachY = 0.0f, attachZ = 0.0f;
        attachedPed = GET_PED_OBJECT_IS_ATTACHED_TO(obj);
        attachedVehicle = GET_CAR_OBJECT_IS_ATTACHED_TO(obj);

        if (DOES_CHAR_EXIST(attachedPed)) {
            GET_CHAR_COORDINATES(attachedPed, &attachX, &attachY, &attachZ);
        }
        else if (DOES_VEHICLE_EXIST(attachedVehicle)) {
            GET_CAR_COORDINATES(attachedVehicle, &attachX, &attachY, &attachZ);
        }
    }

    if (IsStickInputSignificant(stickX, stickY))
    {
        float moveX = moveSpeedMultiplier * (forwardX * scaledStickY + rightX * scaledStickX);
        float moveY = moveSpeedMultiplier * (forwardY * scaledStickY + rightY * scaledStickX);

        if (isAttached)
        {
            HandleAttachedMovement(scaledStickX, scaledStickY, moveSpeedMultiplier);
        }
        else
        {
            pos.x += moveX;
            pos.y += moveY;
        }
    }

    int lt = GET_CONTROL_VALUE(0, 6);
    int rt = GET_CONTROL_VALUE(0, 5);
    float zInput = ((float)rt - (float)lt) * (1.0f / 255.0f);
    float zMove = zInput * moveSpeedMultiplier;

    if (isAttached)
        attachedOffsetZ += zMove;
    else
        pos.z += zMove;

    if (isAttached)
    {
        if (DOES_CHAR_EXIST(attachedPed))
        {
            ATTACH_OBJECT_TO_PED(obj, attachedPed, BONE_HEAD,
                attachedOffsetX, attachedOffsetY, attachedOffsetZ,
                attachedYaw, attachedPitch, attachedRoll, 0);
        }
        else if (DOES_VEHICLE_EXIST(attachedVehicle))
        {
            ATTACH_OBJECT_TO_CAR(obj, attachedVehicle, 0,
                attachedOffsetX, attachedOffsetY, attachedOffsetZ,
                attachedYaw, attachedPitch, attachedRoll);
        }
    }
    else
    {
        SET_OBJECT_COORDINATES(obj, pos.x, pos.y, pos.z);
    }
    
}

void HandleObjectRotation()
{
    if (!DoesObjectWithArrayIndexExist(GetSelectedObjectArrayIndex()))
        return;

    if (IS_OBJECT_ATTACHED(GetSelectedObject()))
        HandleObjectRotation_AttachMode();
    else
        HandleObjectRotation_LocalMode();
}

void HandleObjectRotation_LocalMode()
{
    bool isSpeedMode = IS_BUTTON_PRESSED(0, BUTTON_X);
    float divisor = isSpeedMode ? 50.0f : 100.0f;
    float step = (float)selectedRotationSpeed / divisor;

    if (IS_BUTTON_PRESSED(0, BUTTON_LB))
        RotateObjectLocal(GetSelectedObject(), Vector3(0, 0, 1), step);
    else if (IS_BUTTON_PRESSED(0, BUTTON_RB))
        RotateObjectLocal(GetSelectedObject(), Vector3(0, 0, 1), -step);
    else if (IS_BUTTON_PRESSED(0, DPAD_UP))
        RotateObjectLocal(GetSelectedObject(), Vector3(1, 0, 0), -step);
    else if (IS_BUTTON_PRESSED(0, DPAD_DOWN))
        RotateObjectLocal(GetSelectedObject(), Vector3(1, 0, 0), step);
    else if (IS_BUTTON_PRESSED(0, DPAD_LEFT) && !isAlternativeControlMode)
        RotateObjectLocal(GetSelectedObject(), Vector3(0, 1, 0), -step);
    else if (IS_BUTTON_PRESSED(0, DPAD_RIGHT) && !isAlternativeControlMode)
        RotateObjectLocal(GetSelectedObject(), Vector3(0, 1, 0), step);
}

void HandleObjectRotation_AttachMode()
{
    bool isSpeedMode = IS_BUTTON_PRESSED(0, BUTTON_X);
    float divisor = isSpeedMode ? 50.0f : 100.0f;
    float step = (float)selectedRotationSpeed / divisor * ATTACH_ROTATION_SPEED_MULTIPLIER;

    if (IS_BUTTON_PRESSED(0, BUTTON_LB))
        attachedRoll -= step;  // Richtige Zuordnung: RB/LB steuern Roll
    else if (IS_BUTTON_PRESSED(0, BUTTON_RB))
        attachedRoll += step;

    else if (IS_BUTTON_PRESSED(0, DPAD_UP))
        attachedYaw -= step;   // DPAD_UP/DOWN steuern Yaw
    else if (IS_BUTTON_PRESSED(0, DPAD_DOWN))
        attachedYaw += step;

    else if (IS_BUTTON_PRESSED(0, DPAD_LEFT) && !isAlternativeControlMode)
        attachedPitch -= step; // DPAD_LEFT/RIGHT steuern Pitch
    else if (IS_BUTTON_PRESSED(0, DPAD_RIGHT) && !isAlternativeControlMode)
        attachedPitch += step;
}

void HandleSpawnerControls()
{
    if (IS_BUTTON_JUST_PRESSED(0, BUTTON_B))
    {
        WAIT(100);
        ExitObjectSpawnerMode();
        WAIT(100);
        return;
    }

    if (IS_BUTTON_JUST_PRESSED(0, BUTTON_BACK))
    {
        isAlternativeControlMode = !isAlternativeControlMode;
        return;
    }
    
    if (IS_BUTTON_JUST_PRESSED(0, BUTTON_A))
    {
        WAIT(100);
        if (CloneSelectedObject())
            printf("~r~Spawner: ~w~Copied ~y~%s.", spawnerMem[GetSelectedObjectArrayIndex()].name);
        else
            printf("~r~Spawner: ~w~Failed to clone object.");
        return;
    }
    else if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y))
    {
        DeleteObjectAtIndex(GetSelectedObjectArrayIndex());
        ExitObjectSpawnerMode();
        return;
    }

    if (!isAlternativeControlMode)
    {
        if (IS_BUTTON_JUST_PRESSED(0, STICK_LEFT))
        {
            printf("Spawner: Object rotation has been reseted");
            SET_OBJECT_QUATERNION(GetSelectedObject(), 0.0f, 0.0f, 0.0f, 0.0f);
            return;
        }
    }
    else
    {
        if (IS_BUTTON_JUST_PRESSED(0, STICK_LEFT))
        {
            Object obj = GetSelectedObject();
            if (!DOES_OBJECT_EXIST(obj)) return;
            AttachObject(obj);
            attachedYaw = 0.0f;
            attachedPitch = 0.0f;
            attachedRoll = 0.0f;
            vector3 objPos;
            GET_OBJECT_COORDINATES(obj, &objPos.x, &objPos.y, &objPos.z);

            Ped ped = GET_PED_OBJECT_IS_ATTACHED_TO(obj);
            Vehicle veh = GET_CAR_OBJECT_IS_ATTACHED_TO(obj);

            float attachX = 0.0f, attachY = 0.0f, attachZ = 0.0f;

            if (DOES_CHAR_EXIST(ped))
                GET_CHAR_COORDINATES(ped, &attachX, &attachY, &attachZ);
            else if (DOES_VEHICLE_EXIST(veh))
                GET_CAR_COORDINATES(veh, &attachX, &attachY, &attachZ);

            attachedOffsetX = objPos.x - attachX;
            attachedOffsetY = objPos.y - attachY;
            attachedOffsetZ = objPos.z - attachZ;
        }
        else if (IS_BUTTON_JUST_PRESSED(0, STICK_RIGHT))
        {
            SnapSelectedObjectToGround(GetSelectedObject());
        }
        else if (IS_BUTTON_JUST_PRESSED(0, DPAD_LEFT))
        {
            SetSelectedObjectIndex(GetNextObjectIndex(false));
            return;
        }
        else if (IS_BUTTON_JUST_PRESSED(0, DPAD_RIGHT))
        {
            SetSelectedObjectIndex(GetNextObjectIndex(true));
            return;
        }
    }
}

void ObjectSpawnerLogic()
{
    if (!spawnerActive)
        return;

    if (!DoesObjectWithArrayIndexExist(GetSelectedObjectArrayIndex()))
	{
        ExitObjectSpawnerMode();
        return;
    }

    HIDE_HUD_AND_RADAR_THIS_FRAME();
    SET_OBJECT_ALPHA(GetSelectedObject(), EDIT_OBJECT_ALPHA);
	/*
    if (PREF_OBJ_SPAWNER_DRAW_ORIGIN_FOR_EDIT_OBJECT) {
        DrawOriginCross(GetSelectedObject());
    }
	*/
    DrawControls(isAlternativeControlMode);
    DrawObjectInformation(GetSelectedSpawnerObject());
    if (!isAlternativeControlMode)
        DrawObjectAxes(GetSelectedObject());

    UpdateSpawnerCamera();

    HandleCameraControls();
    HandleObjectMovement();
    HandleObjectRotation();
    HandleSpawnerControls();
}