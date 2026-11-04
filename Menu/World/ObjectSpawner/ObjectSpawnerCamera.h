#pragma once
#include "..\..\maths.h"
#include "ObjectSpawnerConstants.h"
#include "..\..\stdafx.h"


void InitializeSpawnerCamera(Object obj);
void UpdateSpawnerCamera();
void DestroySpawnerCamera();
void SetSpawnerCameraTarget(Object target);
void ZoomSpawnerCamera(float amount);
void RotateSpawnerCamera(float yawDelta, float pitchDelta);
float GetSpawnerCameraYaw();
float GetSpawnerCameraPitch();
float CalculateOptimalCameraDistance(Object obj);