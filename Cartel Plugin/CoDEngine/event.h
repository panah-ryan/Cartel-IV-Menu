#pragma once
#include "stdafx.h"
#include "Defines.h"

void CTL_GET_EVENT_BITSET(scrThread_Info* cxt);

extern Detour<void> netEventMgr_HandleEventDetour;
void netEventMgr_HandleEvent(netEventMgr* manager, CNetworkEvent* pEvent, CMessage* message, int peer, short messageSeq, int eventId);

//Detour<void> CWeaponDamageEvent_HandleDetour;
//void CWeaponDamageEvent_Handle(void* classPointer, CMessage* message, int peer)

#ifdef Moderator
extern Detour<int> CWeaponDamageEvent_DecideDetour;
int CWeaponDamageEvent_Decide(char* classPointer, byte peerid);
#endif
//CWeaponDamageEvent_DecideDetour.SetupDetour(0x82607Df8, CWeaponDamageEvent_Decide);