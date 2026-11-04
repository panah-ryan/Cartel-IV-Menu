#include "stdafx.h"
#include "Defines.h"
#include "other.h"
#include <algorithm>
#include "natives.h"
#include "Server.h"

#include "FileIO.h"

#include "modder_logging.h"

int& session_flags = *reinterpret_cast<int*>(0x830FAFB0);

#ifdef Moderator
Detour<BOOL> GeneratePeerTokenDetour;
//BOOL GeneratePeerTokenHook(int dwUserIndex, QWORD* token)
BOOL GeneratePeerTokenHook(QWORD* token)
{
	//BOOL ret = GeneratePeerTokenDetour.CallOriginal(dwUserIndex, token);
	BOOL ret = GeneratePeerTokenDetour.CallOriginal(token);
	*(DWORD*)token = 0;
	*(WORD*)(token + 4) = 0;
	return ret;
}
#endif

Detour<void> netPeerMgr_HandleGetReadyToStartPlayingDetour;
void netPeerMgr_HandleGetReadyToStartPlaying(void* netPeerMgr, netEvent* evt)
{
	if (session_flags > 7 && session_flags < 11) //Session flags show we are already playing
	{
		CNetGamePlayer* peer = NetworkInterface::GetPlayerFromConnectionID(netPeerMgr, evt->m_CxnId);
		if (peer->IsPhysical())
		{
			printf("[CARTEL::Get Ready to Start Playing] - %s tried to send GetReadyToStartPlaying message when we are already playing!\n", peer->GetPlayerInfo()->GetPlayerName());
			attacker_list.push_back(std::make_tuple(peer->GetPeerID(), ATTACK_TELL_NET, NULL));
		}

		return;
	}

	netPeerMgr_HandleGetReadyToStartPlayingDetour.CallOriginal(netPeerMgr, evt);
}

Detour<bool> CMsgPeerData_ImportDetour;
bool CMsgPeerData_Import(peerDataMsg* msg, DWORD key, char* buffer, size_t size, int* count)
{
	bool ret = CMsgPeerData_ImportDetour.CallOriginal(msg, key, buffer, size, count); //grab the values
	if (msg->m_PeerID > 15 || msg->m_PeerID < 0)
	{
		//printf("[CARTEL::Peer Data] - Host sent us an invalid peer id. Fixing to prevent crash but we should leave game to be safe.\n");
		printf("[CARTEL::Peer Data] - Host sent us an invalid peer id. Returning to single-player, this session is fucked.\n");
		attacker_list.push_back(std::make_tuple(GET_HOST_ID(), ATTACK_PEER_DATA, NULL));
		for (int i = 0; i < 16; i++) //We can't ever be sent to a game with no open slots (host wouldn't allow it)
		{
			if (!NetworkInterface::GetNetPlayer(i)->IsPhysical()) //We found an open slot in the game lets set our peer id to that slot
				msg->m_PeerID = i;
		}
		XNotify(L"Cartel: Invalid Peer ID from Host - Leaving to avoid crash.");
	}

	return ret;
}

Detour<bool> CMsgPeerData_ExportDetour;
bool CMsgPeerData_Export(peerDataMsg* msg, DWORD key, char* buffer, size_t size, int* count)
{
	msg->m_NatType = XONLINE_NAT_OPEN;

	return CMsgPeerData_ExportDetour.CallOriginal(msg, key, buffer, size, count);
}

Detour<bool> CMsgReassignConfirm_ImportDetour;
bool CMsgReassignConfirm_Import(reassignConfirmMsg* msg, DWORD key, char* buffer, size_t size, int* count)
{
	bool ret = CMsgReassignConfirm_ImportDetour.CallOriginal(msg, key, buffer, size, count);

	if (msg->m_PeerID > 15 || msg->m_PeerID < 0) //Check if peer id is valid return false if it isn't this causes the message to not be used
	{
		printf("[CARTEL::Reassign Confirm] - Got invalid peer id for reassignment.\n");
		attacker_list.push_back(std::make_tuple(INVALID_PLAYER_INDEX, ATTACK_REASSIGN, NULL));
		return false;
	}

	return ret;
}

Detour<bool> CMsgReassignNegotiate_ImportDetour;
bool CMsgReassignNegotiate_Import(reassignNegotiateMsg* msg, DWORD key, char* buffer, size_t size, int* count)
{
	bool ret = CMsgReassignNegotiate_ImportDetour.CallOriginal(msg, key, buffer, size, count);

	if (msg->m_PeerID > 15 || msg->m_PeerID < 0) //Check if peer id is valid return false if it isn't this causes the message to not be used
	{
		printf("[CARTEL::Reassign Negotiate] - Got invalid peer id for reassignment.\n");
		attacker_list.push_back(std::make_tuple(INVALID_PLAYER_INDEX, ATTACK_REASSIGN, NULL));
		return false;
	}

	return ret;
}

Detour<bool> CMsgReassignResponse_ImportDetour;
bool CMsgReassignResponse_Import(reassignResponseMsg* msg, DWORD key, char* buffer, size_t size, int* count)
{
	bool ret = CMsgReassignResponse_ImportDetour.CallOriginal(msg, key, buffer, size, count);

	if (msg->m_PeerID > 15 || msg->m_PeerID < 0) 
	{
		printf("[CARTEL::Reassign Response] - Got invalid peer id for reassignment.\n");
		attacker_list.push_back(std::make_tuple(INVALID_PLAYER_INDEX, ATTACK_REASSIGN, NULL));
		msg->m_PeerID = 0; //set valid peer id
		msg->m_ResponseType = 5; //set to invalid response type so code doesn't do anything
	}

	return ret; 
}

Detour<bool> netComplaintMsg_ImportDetour;
bool netComplaintMsg_Import(netComplaintMsg* msg, DWORD key, char* buffer, size_t size, int* count)
{
	//Lets just not even reply to this
	/*
	if (msg->m_NumComplainees == 64)
	{
		QWORD firstComplaint = msg->m_ComplaineeIds[0];
		for (int i = 1; i < 64; i++)
		{
			if (msg->m_ComplaineeIds[i] != firstComplaint) //We have different complainees (not all coming from the same person).
				return false;
		}
		int playerThatsKicking = INVALID_PLAYER_INDEX;
		for (int i = 0; i < 16; i++)
		{
			CNetGamePlayer* player = NetworkInterface::GetNetPlayer(i);
			if (player->IsPhysical())
			{
				if (firstComplaint == player->GetRlPeerId())
				{
					playerThatsKicking = i;
					break;
				}
			}
		}
		int playerTargetted = INVALID_PLAYER_INDEX;
		if (playerThatsKicking != INVALID_PLAYER_INDEX)
		{
			for (int i = 0; i < 16; i++)
			{
				CNetGamePlayer* player = NetworkInterface::GetNetPlayer(i);
				if (player->IsPhysical())
				{
					if (msg->m_ComplainerId == player->GetRlPeerId())
					{
						playerTargetted = i;
						break;
					}
				}
			}
			if (playerTargetted == INVALID_PLAYER_INDEX)
				printf("[CARTEL] %s tried to off-host kick somoone!", GET_PLAYER_NAME(playerThatsKicking));
			else
				printf("[CARTEL] %s tried to off-host kick %s!", GET_PLAYER_NAME(playerThatsKicking), GET_PLAYER_NAME(playerTargetted));
		}
	}*/
	return false;
}

QWORD qwJoiningMachineID = 0;
DWORD dwJoiningIP_LAN;

Detour<HRESULT> CXnIp_SecRegLookupDetour;
HRESULT CXnIp_SecRegLookupHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6) 
{
	if (r5) 
	{
		QWORD MachineID = _byteswap_uint64(*(QWORD*)(r5 + 0x18));

		qwJoiningMachineID = MachineID;
		dwJoiningIP_LAN = ((XNADDR*)r5)->ina.S_un.S_addr; //should probably change to inaOnline.
	}

	return  CXnIp_SecRegLookupDetour.CallOriginal(r3, r4, r5, r6);
}

extern Server* connection;

enum PLAYER_CHECK_RESULT : byte
{
	PLAYER_OK = 0,
	PLAYER_GRIEFER = 1,
	PLAYER_IMPERSONATOR = 2,
	PLAYER_SPOOFING = 3,
};

static bool CNetworkIsGameInProgress() { return ((bool(*)())0x826C4CA8)(); } //CNetwork::

//if (CNetwork::IsGameInProgress()) //Game is already in progress
PLAYER_CHECK_RESULT CheckPlayer(CPlayerInfo* info, bool AreWeHost)
{
	if (!info)
		return PLAYER_OK;

#ifdef Cartel_Server_Included
	//CROWS ANTI SPOOFER SHIT
	XNADDR TheirXNADDR = info->m_peerAddress;

	// Grab the machine ID 
	QWORD MachineID = 0;
	XNetXnAddrToMachineId(&TheirXNADDR, &MachineID);

	// Compare the IP from the XNADDR from the game to the one we cached from XAM.
	//if (TheirXNADDR.ina.S_un.S_addr == dwJoiningIP_LAN && MachineID != qwJoiningMachineID) 
	//TODO: This will fail sometimes if multiple people join in quick succession, should probably put the joiningIp/machineId etc in an array so we can check a couple at once.
	//bool weHaveJoined = NetworkInterface::GetLocalPlayerInfo()->m_State == 2; //STATUS_ACTIVE == 2
	
	bool ret_spoofing = false;
	//if (TheirXNADDR.ina.S_un.S_addr == dwJoiningIP_LAN && MachineID != qwJoiningMachineID) 
	if (TheirXNADDR.ina.S_un.S_addr != dwJoiningIP_LAN || MachineID != qwJoiningMachineID) 
	{
#ifdef Moderator
		if(TheirXNADDR.ina.S_un.S_addr == dwJoiningIP_LAN)
			printf("[CARTEL::Spoofer] %s spoofed MachineID (Real: %016I64x | Fake: %016I64x)\n", info->m_Gamertag, qwJoiningMachineID, MachineID);
		
		if(CNetworkIsGameInProgress())
			ret_spoofing = true;
#else
		if(CNetworkIsGameInProgress())
			return PLAYER_SPOOFING;
#endif
	}

	//NetDll_XNetXnAddrToInAddr should do it (non-spoofed IP pulling)
	// Build all identifiers once
	// 
	QWORD mid = info->GetPlayerMachineId();
	//QWORD mid = AreWeHost ? qwJoiningMachineID : info->GetPlayerMachineId();
	byte* midBytes = (byte*)&mid;
	char mid_str[17] = { 0 };
	_snprintf(mid_str, sizeof(mid_str), "%02x%02x%02x%02x%02x%02x%02x%02x", midBytes[0], midBytes[1], midBytes[2], midBytes[3], midBytes[4], midBytes[5], midBytes[6], midBytes[7]);

	unsigned int ip = info->GetPlayerIPAddress();
	//unsigned int ip = AreWeHost ? dwJoiningIP_LAN : info->GetPlayerIPAddress();
	byte* ipBytes = (byte*)&ip;
	char ip_str[16] = { 0 };
	_snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", ipBytes[0], ipBytes[1], ipBytes[2], ipBytes[3]);

	const BYTE* mac = info->GetPlayerMACAddress();
	char mac_str[13] = { 0 };
	_snprintf(mac_str, sizeof(mac_str), "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	byte* xuidBytes = (byte*)&info->m_GamerHandle;
	char xuid_str[17] = { 0 };
	_snprintf(xuid_str, sizeof(xuid_str), "%02x%02x%02x%02x%02x%02x%02x%02x", xuidBytes[0], xuidBytes[1], xuidBytes[2], xuidBytes[3], xuidBytes[4], xuidBytes[5], xuidBytes[6], xuidBytes[7]);

#ifdef Moderator
	//MODERATOR ONLY
	//Extra BS yes but moderator only so who gaf and would have to re-assemble anyway to make it pretty.
	//cost of this is microseconds
	char playerDataString[255];
	unsigned int lan_ip = info->GetPlayerLANIPAddress();
	byte* lan_ipBytes = (byte*)&lan_ip;;

	//TODO: Pull this string from the server for mods.
	char* console_string = strstr(mid_str, "000000fa") == 0 ? "[One]" : "[360]"; //I am being nice and assembling in the original format for consistency.
	_snprintf(playerDataString, sizeof(playerDataString), "%s - %s - XUID 0x%02X%02X%02X%02X%02X%02X%02X%02X - IP %s - MID 0x%02X%02X%02X%02X 0x%02X%02X%02X%02X - MAC %02X-%02X-%02X-%02X-%02X-%02X - LAN %d.%d.%d.%d", console_string, info->m_Gamertag, xuidBytes[0], xuidBytes[1], xuidBytes[2], xuidBytes[3], xuidBytes[4], xuidBytes[5], xuidBytes[6], xuidBytes[7], ip_str, midBytes[0], midBytes[1], midBytes[2], midBytes[3], midBytes[4], midBytes[5], midBytes[6], midBytes[7], mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], lan_ipBytes[0], lan_ipBytes[1], lan_ipBytes[2], lan_ipBytes[3]);
	writeToTextFile("GAME:\\Cartel\\ctl_player_log.txt", "CARTEL", playerDataString);

	if (ret_spoofing && AreWeHost)
		return PLAYER_SPOOFING;
	///////
#endif

	if (!connection)
		return PLAYER_OK;

	// ---Griefer check
	// Lowercase gamertag for comparison
	char gt_lower[16] = { 0 };
	for (int j = 0; info->m_Gamertag[j] && j < 15; j++)
		gt_lower[j] = tolower(info->m_Gamertag[j]);

	if (!connection->GetGrieferList().empty())
	{
		const std::vector<byte>& glist = connection->GetGrieferList();
		int gcount = (int)(glist.size() / 34);
		for (int i = 0; i < gcount; i++)
		{
			byte        type = glist[i * 34];
			const char* value = (const char*)glist.data() + (i * 34) + 1;
			switch (type)
			{
			case GRIEFER_TYPE_MACHINEID:
				if (strcmp(value, mid_str) == 0) 
					return PLAYER_GRIEFER;
			case GRIEFER_TYPE_IP:
				if (strcmp(value, ip_str) == 0) 
					return PLAYER_GRIEFER;
			case GRIEFER_TYPE_GAMERTAG:
				if (strcmp(value, gt_lower) == 0) 
					return PLAYER_GRIEFER;
			case GRIEFER_TYPE_MAC:
				if (strcmp(value, mac_str) == 0) 
					return PLAYER_GRIEFER;
			case GRIEFER_TYPE_XUID:
				if (strcmp(value, xuid_str) == 0) 
					return PLAYER_GRIEFER;
			}
		}
	}
	// -- Impersonation check 
	if (!connection->GetKnownPlayersList().empty())
	{
		char combined_h1[64] = { 0 };
		char combined_h2[64] = { 0 };
		byte hash1[16] = { 0 };
		byte hash2[16] = { 0 };
		byte gt_hash[8] = { 0 };
		byte xuid_hash[8] = { 0 };
		const byte* matched_kp_gt = nullptr;

		// Hash gamertag and xuid for blob lookup
		XeCryptSha((byte*)gt_lower, strlen(gt_lower), nullptr, 0, nullptr, 0, gt_hash, sizeof(gt_hash));
		XeCryptSha((byte*)xuid_str, strlen(xuid_str), nullptr, 0, nullptr, 0, xuid_hash, sizeof(xuid_hash));

		const std::vector<byte>& klist = connection->GetKnownPlayersList();
		int kcount = (int)(klist.size() / 48);

		for (int i = 0; i < kcount; i++)
		{
			const byte* entry = klist.data() + (i * 48);
			const byte* kp_gt = entry;        // 8 bytes
			const byte* kp_xuid = entry + 8;    // 8 bytes
			const byte* kp_hash1 = entry + 16;   // 16 bytes
			const byte* kp_hash2 = entry + 32;   // 16 bytes

			// Check 1 - gamertag match, verify xuid+mid+ip+mac hash
			if (memcmp(gt_hash, kp_gt, 8) == 0) //Gamertag they joined with matches gamertag server has listed
			{
				_snprintf(combined_h1, sizeof(combined_h1), "%s%s%s%s", xuid_str, mid_str, ip_str, mac_str);
				XeCryptSha((byte*)combined_h1, strlen(combined_h1), nullptr, 0, nullptr, 0, hash1, sizeof(hash1)); // Hash xuid + mid + ip + mac (for gamertag match check)
				
				if (memcmp(hash1, kp_hash1, 16) != 0)
					matched_kp_gt = kp_gt;
				break;
			}

			// Check 2 - xuid match, verify gamertag+mid+ip+mac hash
			if (memcmp(xuid_hash, kp_xuid, 8) == 0) //XUID They joined with matched xuid server has listed
			{	//Hash gamertag + mid + ip + mac (for xuid match check)
				_snprintf(combined_h2, sizeof(combined_h2), "%s%s%s%s", gt_lower, mid_str, ip_str, mac_str);
				XeCryptSha((byte*)combined_h2, strlen(combined_h2), nullptr, 0, nullptr, 0, hash2, sizeof(hash2));
				
				if (memcmp(hash2, kp_hash2, 16) != 0)
					matched_kp_gt = kp_gt;
				break;
			}
		}

		if (matched_kp_gt != nullptr)
		{	//lets send the matched entries gamertag hash back to the server so they can notify.
			connection->m_kicked_impersonator = AreWeHost;
			_snprintf(connection->m_pending_impersonation_hash, 17, "%02x%02x%02x%02x%02x%02x%02x%02x", matched_kp_gt[0], matched_kp_gt[1], matched_kp_gt[2], matched_kp_gt[3], matched_kp_gt[4], matched_kp_gt[5], matched_kp_gt[6], matched_kp_gt[7]);
			connection->SendRecv(Server::PACKET_REPORT_IMPERSONATION); //needa offload this to a threaaad
			return PLAYER_IMPERSONATOR;
		}
	}

	if (ret_spoofing)
		return PLAYER_SPOOFING;
#endif
	return PLAYER_OK;
}

Detour<bool> CNetworkPlayerMgr_AddTemporaryPlayerDetour;
bool CNetworkPlayerMgr_AddTemporaryPlayer(void* mgr, CPlayerInfo* info, void* endpoint, peerDataMsg* data, int player_status)
{
	std::string player_name = std::string(info->m_Gamertag);
	if (std::count(player_name.begin(), player_name.end(), '~') % 2) //We found odd amount of ~ lets remove them
	{
		std::replace(player_name.begin(), player_name.end(), '~', '-'); //Lets replace all ~ with - then write new gamertag
		strncpy(info->m_Gamertag, player_name.c_str(), sizeof(info->m_Gamertag));
		attacker_list.push_back(std::make_tuple(data->m_PeerID, ATTACK_PLAYER_NAME, NULL));
		//no don't add to any db or files here as it can be spoofed ezily
		//AddMachineToModderFile(info->GetPlayerMachineId()); //data should still be there even if we kick them  //people can spoof MID's that will reach host before failing to connect... need to re-engineer this.
	}

	bool AreWeHost = (GET_HOST_ID() == GET_PLAYER_ID());

	PLAYER_CHECK_RESULT result = CheckPlayer(info, AreWeHost);
	if (result != PLAYER_OK)
	{
		const char* category =
			result == PLAYER_GRIEFER ? "Griefer" :
			result == PLAYER_IMPERSONATOR ? "Impersonator" :
			result == PLAYER_SPOOFING ? "Spoofer" : "Unknown";

		if (AreWeHost)
		{
			//Should probably generate random for the XUID and overwrite before kicking.. (stop them adding innocents to blacklist)
			XeCryptRandom((byte*)&info->m_GamerHandle, 8);
			//CNetwork::Kick and pass in a pointer to the GamerID
			//((void(*)(QWORD*))(0x826CA1A8))(&info->m_GamerID);
			//NETWORK_KICK_PLAYER(data->m_PeerID);
			printf("[CARTEL::%s] Kicked: %s\n", category, info->m_Gamertag);
			attacker_list.push_back(std::make_tuple(data->m_PeerID, ATTACK_PLAIN_KICK_RESPONSE, result));
			//return false; //this caused bad peer id to go to the player, and then fucks the lobby.. needs changing.
		}
		else
			printf("[CARTEL::%s] Detected: %s\n", category, info->m_Gamertag);
	}

	return CNetworkPlayerMgr_AddTemporaryPlayerDetour.CallOriginal(mgr, info, endpoint, data, player_status);
}

/*
void CTL_IS_PLAYER_SPOOFING_CHECK_IN_PROGRESS(scrThread_Info* cxt) //make sure we clear these, will be no need for a CLEAR native as they will expire, we will just store checked in the .sco
{
	cxt->ResultPtr[0].Bool = checking_xuid_index_storage[cxt->Params[0].Int] != INVALID_PLAYER_INDEX; //playerindex is present in their slot so we are checking
}

void CTL_IS_PLAYER_SPOOFING(scrThread_Info* cxt)
{
	int PlayerIndex = cxt->Params[0].Int;
	if (checking_xuid_index_storage[PlayerIndex] != INVALID_PLAYER_INDEX) //we are checking this slot currently.. (contains playerindex)
	{
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}
	if (player_has_mismatched_xuid[PlayerIndex])
	{
		player_has_mismatched_xuid[PlayerIndex] = FALSE; //return true once, menu won't bother checking again.
		cxt->ResultPtr[0].Bool = TRUE;
		return;
	}
	checking_xuid_index_storage[PlayerIndex] = PlayerIndex;
	ExCreateThread(0, 0, 0, 0, (LPTHREAD_START_ROUTINE)XUIDCheckThread, &checking_xuid_index_storage[PlayerIndex], 0);
}
*/
BOOL forceOnlineState = FALSE;
void CTL_FORCE_ONLINE_STATE(scrThread_Info* cxt)
{
	forceOnlineState = cxt->Params[0].Bool;
}

Detour<XAMUSER_SIGNIN_STATE> XamUserGetSigninState_detour;
XAMUSER_SIGNIN_STATE XamUserGetSigninState_hook(UINT32 dwUserIndex)
{
	if (forceOnlineState)
	{
		XAMUSER_SIGNIN_STATE state = XamUserGetSigninState(dwUserIndex);

		if (state == eXamUserSigninState_SignedInLocally)
			return eXamUserSigninState_SignedInToLive;

		return state;
	}
	return XamUserGetSigninState_detour.CallOriginal(dwUserIndex);
}

Detour<BOOL> rlPeerAddress_HasNetwork_detour;
BOOL rlPeerAddress_HasNetwork()
{
	return forceOnlineState ? TRUE : rlPeerAddress_HasNetwork_detour.CallOriginal();
}

Detour<bool> rlGamerInfo_HasMultiplayerPrivileges_detour;
bool rlGamerInfo_HasMultiplayerPrivileges()
{
	return forceOnlineState ? TRUE : rlGamerInfo_HasMultiplayerPrivileges_detour.CallOriginal();
}

//std::tuple<int, CTL_ATTACK_TYPES, DWORD> result = attacker_list.back();

/////////////////                 SERVER RELATED NATIVES                //////////////////////
extern BOOL CartelServAvailable; 
void CTL_IS_SERVER_AVAILABLE(scrThread_Info* cxt)
{
#ifdef Cartel_Server_Included
	cxt->ResultPtr[0].Bool = CartelServAvailable;
#else
	cxt->ResultPtr[0].Bool = FALSE;
#endif
}

#ifdef Cartel_Server_Included
extern int num_server_map_entries;
extern byte server_map_list_order_mode;
extern Server* connection;
extern std::vector<std::pair<std::string, std::string>> menu_map_item_entries;
char map_list_filter[16];

void CTL_SET_MAP_LIST_FILTER(scrThread_Info* cxt)
{
	strncpy(map_list_filter, cxt->Params[0].String, 16);
	printf("[CARTEL::Server] Map list filter set to: %s\n", map_list_filter);
}

void CTL_DOWNLOAD_MAP_LIST(scrThread_Info* cxt) //return BOOL ----- PROVIDE WITH POINTER TO ITEM NAME AND INFO ARRAYS... do some logic so you can't press A while they all say "LOADING..." or something.
{
	scrValue* entry_count = cxt->Params[1].Reference;
	if (CartelServAvailable)
	{
		//static int last_downloaded_map_list_tick = 0;
		static int last_server_map_list_order_mode = -1;

		server_map_list_order_mode = cxt->Params[0].Int;
		//if (last_downloaded_map_list_tick == 0 || last_downloaded_map_list_tick < GetTickCount())
		if (last_server_map_list_order_mode != server_map_list_order_mode)// || last_downloaded_map_list_tick < GetTickCount()) //stop em spamming same page
		{
			last_server_map_list_order_mode = server_map_list_order_mode;
			for (int i = 0; i < 2; i++)	//lazy way to try again if the socket is dead.
			{
				if (connection->SendRecv(Server::PACKET_GET_MAP_LIST) != Server::ERROR_SERVER_SUCCESS)
				{
					printf("[CARTEL::Server] Failed to get map list, perhaps the socket is dead, giving it a retry %i.\n", i);
					continue;
				}
				entry_count->Int = num_server_map_entries;
				cxt->ResultPtr[0].Bool = TRUE;
				return;
			}
		}
		else //we're on the same page, no point re-pulling right now.
		{
			if (num_server_map_entries > 0) //we have already got some data locally
			{
				entry_count->Int = num_server_map_entries;
				cxt->ResultPtr[0].Bool = TRUE;
				return;
			}
		}
	}

	printf("[CARTEL::Server] Failed to retrive map list.\n");
	entry_count->Int = 0;
	cxt->ResultPtr[0].Bool = FALSE;
}

char requested_mapName[32];
char requested_mapItemInfo[64];

char requested_mapCreator[16]; //only used internally during downloading

void CTL_DELETE_MAP_FILE_FROM_SERVER(scrThread_Info* cxt)
{
	if (CartelServAvailable)
	{
		strncpy(requested_mapName, cxt->Params[0].String, 32); //capped in menu to 20 chars..
		strncpy(requested_mapItemInfo, cxt->Params[1].String, sizeof(requested_mapItemInfo));
		size_t tilde_pos = strcspn(requested_mapItemInfo, "~");
		if (requested_mapItemInfo[tilde_pos] == '~')
			requested_mapItemInfo[tilde_pos - 1] = '\0';

		//requested_mapItemInfo[strcspn(requested_mapItemInfo, "~")] = '\0';	//strip out any optional "~PAD_X~ = Delete" sent by server for our own maps.
		printf("[CARTEL::Server] Requesting deletion of %s with info %s.\n", requested_mapName, requested_mapItemInfo);
		if (connection->SendRecv(Server::PACKET_DELETE_MAP_INI) != E_FAIL)
		{
			cxt->ResultPtr[0].Bool = TRUE; //request sent
			return;
		}
	}
	cxt->ResultPtr[0].Bool = FALSE;
}

volatile bool ini_download_in_progress = false;
void INIDownloadThread(void)
{
	ini_download_in_progress = TRUE;
	connection->SendRecv(Server::PACKET_GET_MAP_FILE);
	ini_download_in_progress = FALSE;
}

void CTL_DOWNLOAD_MAP_FILE(scrThread_Info* cxt)
{
	if (CartelServAvailable)
	{
		if (ini_download_in_progress)
		{
			PRINT_STRING_WITH_LITERAL_STRING_NOW("STRING", "~r~CTL: ~w~Download is already in progress, please wait.", 5000, 1);
			return;
		}
		strncpy(requested_mapName, cxt->Params[0].String, 32); //capped in menu to 20 chars..
		strncpy(requested_mapItemInfo, cxt->Params[1].String, sizeof(requested_mapItemInfo));
		size_t tilde_pos = strcspn(requested_mapItemInfo, "~");
		if (requested_mapItemInfo[tilde_pos] == '~')
			requested_mapItemInfo[tilde_pos - 1] = '\0';

		//requested_mapItemInfo[strcspn(requested_mapItemInfo, "~")] = '\0';	//strip out any optional "~PAD_X~ = Delete" sent by server for our own maps.
		printf("[CARTEL::Server] Requesting %s with info %s.\n", requested_mapName, requested_mapItemInfo);
		ExCreateThread(0, 0, 0, 0, (LPTHREAD_START_ROUTINE)INIDownloadThread, 0, 0);
	}
}

void CTL_GET_MAP_LIST_ENTRY(scrThread_Info* cxt)
{
	int entry_to_get = cxt->Params[0].Int;

	if (entry_to_get < 0 || entry_to_get >= menu_map_item_entries.size())
	{
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	scrValue* map_name = cxt->Params[1].Reference;
	scrValue* map_itemInfo = cxt->Params[2].Reference;

	auto& entry = menu_map_item_entries[entry_to_get];
	std::string& creator = std::get<0>(entry);
	std::string& info = std::get<1>(entry);

	map_name->String = const_cast<char*>(creator.c_str());
	map_itemInfo->String = const_cast<char*>(info.c_str());
	cxt->ResultPtr[0].Bool = TRUE;
}

volatile bool ini_upload_in_progress = false;
void INIUploadThread(void)
{
	ini_upload_in_progress = TRUE;
	connection->SendRecv(Server::PACKET_UPLOAD_MAP_INI);
	ini_upload_in_progress = FALSE;
}


char uploadingFilePath[128]; //global so we can offload this to a thread later
void CTL_UPLOAD_MAP_INI(scrThread_Info* cxt)
{
	if (CartelServAvailable)
	{
		if (ini_download_in_progress)
		{
			PRINT_STRING_WITH_LITERAL_STRING_NOW("STRING", "~r~CTL: ~w~Download is in progress, wait for it to finish before uploading.", 5000, 1);
			return;
		}
		if (ini_upload_in_progress)
		{
			PRINT_STRING_WITH_LITERAL_STRING_NOW("STRING", "~r~CTL: ~w~Upload is already in progress, please wait.", 5000, 1);
			return;
		}
		strncpy(uploadingFilePath, cxt->Params[0].String, sizeof(uploadingFilePath));
		ExCreateThread(0, 0, 0, 0, (LPTHREAD_START_ROUTINE)INIUploadThread, 0, 0);
		//server will want map name and user.name, we already have user.name, and the map name will be on the end of this path.
	}
}
#endif
