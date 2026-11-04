#pragma once
#include "stdafx.h"
#include "Defines.h"
#include "Detour.h"

#include "caller.h" //can remove when done with test native

struct peerDataMsg
{
	byte m_PeerID;
	char _padding1[3];
	int m_playerColor;
	int m_playerTeam;
	int _0x0C;
	QWORD m_SessionID;
	XONLINE_NAT_TYPE m_NatType;
	int _0x1C;
	int _0x20;
	bool _0x24;
	char _padding2[3];
};

struct reassignConfirmMsg
{
	byte m_PeerID;
	char _padding1[3];
	int m_NumRemoteObjects;
	int m_NumLocalObjects;
	int _0x0C;
	char* m_MessageBuffer;
	size_t m_ByteSizeOfBuffer;
	size_t m_MaxByteSizeOfBuffer;
};

struct reassignNegotiateMsg
{
	byte m_PeerID;
	char _padding1[3];
	int m_NumObjects;
	int _0x08;
	char* m_MessageBuffer;
	size_t m_BytesSizeOfBuffer;
	size_t m_MaxByteSizeOfBuffer;
};

struct reassignResponseMsg
{
	byte m_PeerID;
	bool m_GotNegotiateResponse;
	char _padding[2];
	int m_ResponseType;
};

struct netComplaintMsg
{
	QWORD m_ComplainerId;
	QWORD m_ComplaineeIds[64];
	int m_NumComplainees;
};

extern Detour<void> netPeerMgr_HandleGetReadyToStartPlayingDetour;
void netPeerMgr_HandleGetReadyToStartPlaying(void* netPeerMgr, netEvent* evt);

extern Detour<bool> CMsgPeerData_ImportDetour;
bool CMsgPeerData_Import(peerDataMsg* msg, DWORD key, char* buffer, size_t size, int* count);

extern Detour<bool> CMsgReassignConfirm_ImportDetour;
bool CMsgReassignConfirm_Import(reassignConfirmMsg* msg, DWORD key, char* buffer, size_t size, int* count);

extern Detour<bool> CMsgReassignNegotiate_ImportDetour;
bool CMsgReassignNegotiate_Import(reassignNegotiateMsg* msg, DWORD key, char* buffer, size_t size, int* count);

extern Detour<bool> CMsgReassignResponse_ImportDetour;
bool CMsgReassignResponse_Import(reassignResponseMsg* msg, DWORD key, char* buffer, size_t size, int* count);

extern Detour<bool> netComplaintMsg_ImportDetour;
bool netComplaintMsg_Import(netComplaintMsg* msg, DWORD key, char* buffer, size_t size, int* count);

extern Detour<bool> CNetworkPlayerMgr_AddTemporaryPlayerDetour;
bool CNetworkPlayerMgr_AddTemporaryPlayer(void* mgr, CPlayerInfo* info, void* endpoint, peerDataMsg* data, int player_status);

extern Detour<bool> CMsgPeerData_ExportDetour;
bool CMsgPeerData_Export(peerDataMsg* msg, DWORD key, char* buffer, size_t size, int* count);

extern Detour<BOOL> GeneratePeerTokenDetour;
//extern BOOL GeneratePeerTokenHook(int dwUserIndex, QWORD* token);

BOOL GeneratePeerTokenHook(QWORD* token);

extern Detour<XAMUSER_SIGNIN_STATE> XamUserGetSigninState_detour;
XAMUSER_SIGNIN_STATE XamUserGetSigninState_hook(UINT32 dwUserIndex);

extern Detour<BOOL> rlPeerAddress_HasNetwork_detour;
BOOL rlPeerAddress_HasNetwork();

extern Detour<bool> rlGamerInfo_HasMultiplayerPrivileges_detour;
bool rlGamerInfo_HasMultiplayerPrivileges();

void CTL_FORCE_ONLINE_STATE(scrThread_Info* cxt);

void CTL_DOWNLOAD_MAP_LIST(scrThread_Info* cxt);
void CTL_GET_MAP_LIST_ENTRY(scrThread_Info* cxt);
void CTL_SET_MAP_LIST_FILTER(scrThread_Info* cxt);
void CTL_DOWNLOAD_MAP_FILE(scrThread_Info* cxt);
void CTL_DELETE_MAP_FILE_FROM_SERVER(scrThread_Info* cxt);
void CTL_UPLOAD_MAP_INI(scrThread_Info* cxt);

void CTL_IS_SERVER_AVAILABLE(scrThread_Info* cxt);

extern Detour<HRESULT> CXnIp_SecRegLookupDetour;
HRESULT CXnIp_SecRegLookupHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6);