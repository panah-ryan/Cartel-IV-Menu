#pragma once
#include "stdafx.h"
#include "Defines.h"
#include "caller.h"
#include "Detour.h"

void setup_blacklists();
void CTL_GET_CLONE_BITSET(scrThread_Info* cxt);
void CTL_ADD_BLACKLIST_PED(scrThread_Info* cxt);
void CTL_REMOVE_BLACKLIST_PED(scrThread_Info* cxt);
void CTL_ADD_BLACKLIST_OBJECT(scrThread_Info* cxt);
void CTL_REMOVE_BLACKLIST_OBJECT(scrThread_Info* cxt);
void CTL_SET_CLONE_SPAM_RESET_TIME(scrThread_Info* cxt);
void CTL_SET_CLONE_SPAM_VARIABLES(scrThread_Info* cxt);

extern Detour<void> CNetworkObjectMgr_ProcessCloneCreateDataDetour;
void CNetworkObjectMgr_ProcessCloneCreateData(CNetworkObjectMgr* netObjMgr, byte peer, NetworkObjectType objectType, short objectID, byte objectFlags, CMessage* message);

extern Detour<bool> CNetObjHeli_SerializeCloneDataDetour;
bool CNetObjHeli_SerializeCloneData(CNetworkObject* net_heli, CMessage* message);