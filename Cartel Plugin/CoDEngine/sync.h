#pragma once
#include "Detour.h"
#include "Defines.h"

extern Detour<int> CDummyTask_SerializeWeaponThrow_detour;
int CDummyTask_SerializeWeaponThrow(void* _this, SYNC_SERIALIZE type, CMessage* message);

extern Detour<int> CNetObjPhysical_SerializeAttachmentDetour;
int CNetObjPhysical_SerializeAttachment(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message);

extern Detour<int> CNetObjPed_SerializeMovementAnimTaskDetour;
int CNetObjPed_SerializeMovementAnimTask(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message);

extern Detour<int> CNetObjPed_SerializeAIDataDetour;
int CNetObjPed_SerializeAIData(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message);

extern Detour<void*> CTaskQuery_AllocateDetour;
void* CTaskQuery_Allocate(int task_type);

extern Detour<BOOL> CNetCloneTaskSimplePlayerAimProjectile_CreateLocalTaskDetour;
BOOL CNetCloneTaskSimplePlayerAimProjectile_CreateLocalTask(void* _this, CPed* ped);

extern Detour<int> CNetObjPed_SerializeGameStateDataDetour;
int CNetObjPed_SerializeGameStateData(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message);

extern Detour<int> CNetObjPed_SerializeAttachmentDetour;
int CNetObjPed_SerializeAttachment(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message);

extern Detour<int> CNetObjPlayer_SerializeAppearanceDataDetour;
int CNetObjPlayer_SerializeAppearanceData(CNetworkObject* obj, SYNC_SERIALIZE type, CMessage* message, int unused);