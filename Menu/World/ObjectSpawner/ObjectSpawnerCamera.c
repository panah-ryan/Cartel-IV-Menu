#include "ObjectSpawnerCamera.h"

extern CAMERA_INDEX scriptedCam;; //remove if include functions.h
float camYaw = 0.0f;
float camDistance = 5.0f;
extern float scriptedCamPitch;
Object camTarget = INVALID_HANDLE;

float CalculateOptimalCameraDistance(Object obj)
{
    if (!DOES_OBJECT_EXIST(obj))
        return 5.0f;

    uint model;
    vector3 minDim, maxDim;
    GET_OBJECT_MODEL(obj, &model);
    GET_MODEL_DIMENSIONS(model, &minDim, &maxDim);

    float sizeX = maxDim.x - minDim.x;
    float sizeY = maxDim.y - minDim.y;
    float sizeZ = maxDim.z - minDim.z;

    float maxDimension = sizeX;
    if (sizeY > maxDimension) maxDimension = sizeY;
    if (sizeZ > maxDimension) maxDimension = sizeZ;

    float distance = maxDimension * 1.5f;
    if (distance < MIN_CAMERA_DISTANCE)
        distance = MIN_CAMERA_DISTANCE;

    return distance;
}

void InitializeSpawnerCamera(Object obj)
{
    if (DOES_CAM_EXIST(scriptedCam))
        return;

    CREATE_CAM(14, &scriptedCam);
    SET_CAM_ACTIVE(scriptedCam, true);
    SET_CAM_PROPAGATE(scriptedCam, true);
    ACTIVATE_SCRIPTED_CAMS(true, true);

    camYaw = 0.0f;
	scriptedCamPitch = 0.0f;
    camDistance = CalculateOptimalCameraDistance(obj);
    camTarget = obj;
}

void UpdateSpawnerCamera()
{
    if (!DOES_CAM_EXIST(scriptedCam) || !DOES_OBJECT_EXIST(camTarget))
        return;

    if (scriptedCamPitch < -89.0f) scriptedCamPitch = -89.0f;
    if (scriptedCamPitch > 89.0f) scriptedCamPitch = 89.0f;
    float offsetX = camDistance * COS(camYaw) * COS(scriptedCamPitch);
    float offsetY = camDistance * SIN(camYaw) * COS(scriptedCamPitch);
    float offsetZ = camDistance * SIN(scriptedCamPitch);

    vector3 camPos;
    GET_OBJECT_COORDINATES(camTarget, &camPos.x, &camPos.y, &camPos.z);

    camPos.x -= offsetX;
    camPos.y -= offsetY;
    camPos.z += offsetZ;

    SET_CAM_POS(scriptedCam, camPos.x, camPos.y, camPos.z);
    POINT_CAM_AT_OBJECT(scriptedCam, camTarget);
}

void DestroySpawnerCamera()
{
    if (DOES_CAM_EXIST(scriptedCam))
    {
        SET_CAM_ACTIVE(scriptedCam, false);
        SET_CAM_PROPAGATE(scriptedCam, false);
        ACTIVATE_SCRIPTED_CAMS(false, false);
        UNATTACH_CAM(scriptedCam);
        DESTROY_CAM(scriptedCam);
        while (DOES_CAM_EXIST(scriptedCam))
            WAIT(0);

        scriptedCam = INVALID_HANDLE;
    }
}

void SetSpawnerCameraTarget(Object target)
{
    camTarget = target;
}

void ZoomSpawnerCamera(float amount)
{
    float newDistance = camDistance + amount;
    if (newDistance > MIN_CAMERA_DISTANCE)
        camDistance = newDistance;
}

void RotateSpawnerCamera(float yawDelta, float pitchDelta)
{
    camYaw -= yawDelta;
	scriptedCamPitch -= pitchDelta;

    if (camYaw > 360.0f)
        camYaw -= 360.0f;
    else if (camYaw < 0.0f)
        camYaw += 360.0f;

	scriptedCamPitch = clamp(scriptedCamPitch, CAMERA_MIN_PITCH, CAMERA_MAX_PITCH);
}

float GetSpawnerCameraYaw()
{
    return camYaw;
}

float GetSpawnerCameraPitch()
{
    return scriptedCamPitch;
}