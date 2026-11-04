#pragma once

#include "..\..\stdafx.h"
#include "ObjectSpawnerCamera.h"
#include "ObjectSpawnerConstants.h"
#include "ObjectSpawnerUI.h"
#include "ObjectSpawnerFunctions.h"
#include "ObjectSpawnerRepository.h"
#include "..\..\maths.h"

extern int selectedMovementSpeed;
extern int selectedRotationSpeed;

void ObjectSpawnerLogic(void);
void EnterObjectSpawnerMode(void);
void ExitObjectSpawnerMode(void);

void HandleCameraControls(void);
void HandleObjectMovement(void);
void HandleObjectRotation(void);
void HandleObjectRotation_LocalMode(void);
void HandleObjectRotation_AttachMode(void);
void HandleSpawnerControls(void);
