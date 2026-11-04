#include "stdafx.h"
//#include "Globals.h"
#include "Defines.h"
#include "caller.h"
#include "natives.h"
#include "array.h"
#include "sync.h"
#include "other.h"
#include "FileIO.h"
#include "event.h"
#include "clone.h"
#include "PlayerDB.h"
//#include "PlayerDB_HTTPS.h"
#include "modder_logging.h"

#include "helpers.h"

#ifdef Cartel_Server_Included
#include "Server.h"
#endif 

#include "hv.h"

//#include "HTTPS_test.h"

using namespace::std;

#define GTAIV_TITLE_ID 0x545407F2

DWORD dwTitleID = NULL;
BOOL terminateProcess;
BOOL shutdownTitleStuff;
BOOL haveWeBootedGame;

LDR_DATA_TABLE_ENTRY* module_handle;

BOOL is_devkit;
BOOL applied_patches; 
BOOL external_unload;
BOOL initial_patch;

BOOL deployed_natives;

#ifdef Cartel_Server_Included
Server* connection;
BOOL CartelServAvailable; //return this to the menu..

Detour<void> AssetManager_HashCheck_detour;
void AssetManager_HashCheck(const char* fileName, void* fi_asset)
{
	if (strstr(fileName, "weaponinfo"))
	{
	loop:
		if (connection->GetStatus() == Server::AUTH_SUCCEEDED) //Force infinite loading if we haven't authed yet
			return;

		goto loop;
	}

	return; //Compiler reason
}

BOOL InitialPatches()
{
	AssetManager_HashCheck_detour.SetupDetour(0x821B5538, AssetManager_HashCheck);

	initial_patch = TRUE;
	return TRUE;
}


BOOL NativeRegister_Insert(DWORD hash, void(*handler)(scrThread_Info*))
{
	return ((BOOL(*)(DWORD, DWORD, void(*)(scrThread_Info*)))connection->Offset(Server::NATIVE_REGISTER))(connection->Offset(Server::NATIVE_TABLE), hash, handler);
}

Detour<void> WAIT_Native_detour;
void WAIT_Native(scrThread_Info* info) //Ingame hook so we can do stuff like inject or natives
{
	if (!deployed_natives && connection->GetStatus() == Server::AUTH_SUCCEEDED)
	{
		/*
		NativeRegister_Insert(connection->Native(Server::CTL_OFFHOST_KICK), CTL_OFFHOST_KICK);
		NativeRegister_Insert(connection->Native(Server::CTL_INIT_ALIAS_DATA), CTL_INIT_ALIAS_DATA);
		NativeRegister_Insert(connection->Native(Server::CTL_MACHINE_HAS_ALIAS), CTL_MACHINE_HAS_ALIAS);
		NativeRegister_Insert(connection->Native(Server::CTL_GET_QUERIED_ALIAS), CTL_GET_QUERIED_ALIAS);
		NativeRegister_Insert(connection->Native(Server::CTL_GIVE_MACHINE_ALIAS), CTL_GIVE_MACHINE_ALIAS);
		NativeRegister_Insert(connection->Native(Server::CTL_REMOVE_MACHINE_ALIAS), CTL_REMOVE_MACHINE_ALIAS);*/

		//My Crow Soft's Anti-Spoof hooks :-D
		// //CXNIP_SECREGLOOKUP_DETOUR
		//CXnIp_SecRegLookupDetour.SetupDetour(is_devkit ? 0x81871F90 : 0x8174A340, CXnIp_SecRegLookupHook);
		CXnIp_SecRegLookupDetour.SetupDetour(connection->Offset(Server::CXNIP_SECREGLOOKUP_DETOUR), CXnIp_SecRegLookupHook);
#ifdef Developer
		printf("Injected natives from Server!\n");
#endif
		connection->SendRecv(Server::PACKET_CHECK_SCO);

		deployed_natives = TRUE;
	}

	WAIT_Native_detour.CallOriginal(info);
}


BOOL Patches()
{
	//mod only natives here or whatever.
	WAIT_Native_detour.SetupDetour(0x82843908, WAIT_Native);
	//AssetManager_HashCheck_detour.TakeDownDetour();
	deployed_natives = FALSE;
	applied_patches = TRUE;
	CartelServAvailable = TRUE;
	//connection->SendRecv(Server::PACKET_CHECK_SCO);
	return TRUE;
}

void CTL_REQUEST_SEVER_MOTD(scrThread_Info* cxt)
{
	connection->SendRecv(Server::PACKET_GET_MOTD);
}

#endif

Caller* call = nullptr;
std::vector<std::tuple<int, CTL_ATTACK_TYPES, DWORD>> attacker_list;
PlayerDB* player_db = nullptr;

int user_session_selected = 0; //provide menu a pointer to this and set before running join.

void CTL_SET_SESSION(scrThread_Info* cxt)
{
	user_session_selected = cxt->Params[0].Int;
}

Detour<void> GtaNetwork_JoinGame_detour;
void GtaNetwork_JoinGame(scrThread_Info *cxt)
{
	if(user_session_selected != 0)
		cxt->Params[0].Int = user_session_selected;

	GtaNetwork_JoinGame_detour.CallOriginal(cxt);
	user_session_selected = 0;
}

Detour<DWORD> scrProgram_Load_detour;
DWORD scrProgram_Load(char* filename, char* scriptname)
{
	char scriptPath[MAX_PATH];

	_snprintf(scriptPath, sizeof(scriptPath), "GAME:\\Cartel\\scripts\\%s.sco", scriptname);

	if (DoesFileExist(scriptPath, nullptr))
	{
		Log("Found %s loading from GAME:\Cartel\scripts folder...\n", scriptname);
		filename = scriptPath;
	}

	return scrProgram_Load_detour.CallOriginal(filename, scriptname);
}

Detour<void> DoesScriptExist_detour;
void DoesScriptExist_hook(scrThread_Info* info)
{
	char scriptPath[MAX_PATH];

	_snprintf(scriptPath, sizeof(scriptPath), "GAME:\\Cartel\\scripts\\%s.sco", info->Params[0].String);

	if (DoesFileExist(scriptPath, nullptr))
		info->ResultPtr[0].Bool = TRUE;
	else
		DoesScriptExist_detour.CallOriginal(info);
}




unsigned int scrComputeHash(char *key)
{
	size_t len = strlen(key);
	unsigned int hash, i;

	for (hash = i = 0; i < len; ++i)
	{
		hash += tolower(key[i]);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

DWORD grab_scrthread(const char* script)
{
	DWORD* sm_Threads = *(DWORD**)0x83192788;
	short sm_ThreadsCount = *(short*)0x8319278C;
	DWORD hash = GET_HASH_KEY(script);

	for (int i = 0; i < sm_ThreadsCount; i++)
	{
		DWORD thread = sm_Threads[i];
		if (thread && *(DWORD*)(thread + 8) == hash)
			return thread;
	}

	return NULL;
}


void CTL_GRAB_LATEST_ATTACKER(scrThread_Info* cxt)
{
	scrValue* attacker = cxt->Params[0].Reference;
	scrValue* type = cxt->Params[1].Reference;
	scrValue* extra = cxt->Params[2].Reference;

	if (attacker_list.empty())
	{
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	std::tuple<int, CTL_ATTACK_TYPES, DWORD> result = attacker_list.back();

	if (attacker != nullptr && type != nullptr)
	{
		attacker->Int = std::get<0>(result);
		type->Int = std::get<1>(result);
	}

	if (extra != nullptr)
		extra->Any = std::get<2>(result);

	attacker_list.pop_back();
	cxt->ResultPtr[0].Bool = TRUE;
}

void CTL_ADD_XEX_COMPILE_TIME_TO_BRIEF(scrThread_Info* cxt)
{
	char time_compiled_buffer[64];

	_snprintf(time_compiled_buffer, sizeof(time_compiled_buffer), "Cartel XEX Compiled %s at %s.", __DATE__, __TIME__);
	ADD_TO_PREVIOUS_BRIEF(time_compiled_buffer);
}

void CTL_SPOOF_XUID_GIVEN_GAMERTAG(scrThread_Info* cxt)  //still fucked
{
	//just use GrabXUID as that works..
	char* gamertag = cxt->Params[0].String;
	XUID myXuid, result;
	if (XUserGetXUID(0, &myXuid) == 0)
	{
		DWORD ret = XUserFindUser(myXuid, 0, gamertag, 0x18, &result, 0);
		//cxt->ResultPtr[0].Bool = SUCCEEDED(ret);

		if (SUCCEEDED(ret))
			*reinterpret_cast<XUID*>(0x8318AB38) = result;

		cxt->ResultPtr[0].Bool = SUCCEEDED(ret);
	}
	else
		cxt->ResultPtr[0].Bool = FALSE;
}

void CTL_PROVIDE_STRING(scrThread_Info* cxt)
{
	if(player_db == nullptr)
		return;

	switch (cxt->Params[0].Int)
	{
	case 0: 
		player_db->m_apiKey = cxt->Params[1].String; 
		break;
	case 1: 
		player_db->m_domain = cxt->Params[1].String; 
		break;
	case 2: 
		player_db->m_userAgent = cxt->Params[1].String; 
		break;
	case 3: 
		player_db->m_path = cxt->Params[1].String; 
		break;
	}
}

void CTL_GET_LOCAL_TIME(scrThread_Info* cxt)
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	scrValue* hour = cxt->Params[0].Reference;
	scrValue* minute = cxt->Params[1].Reference;
	hour->Int = curTime.wHour;
	minute->Int = curTime.wMinute;
}

void CTL_PROVIDE_PLAYER_INFO(scrThread_Info* cxt)
{
	SYSTEMTIME curTime;
	char submitBuf[512];
	char noteString[64];
	char MacString[32];
	byte* MAC_Addr = (byte*)cxt->Params[6].Int;

	if (player_db == nullptr || player_db->HasJobToRun())
	{
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	GetSystemTime(&curTime); //Coordinated Universal Time
	_snprintf(noteString, sizeof(noteString), "Created by Cartel (%04i/%02i/%02i %02i:%02i:%02i)", curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	_snprintf(MacString, sizeof(MacString), "%02X-%02X-%02X-%02X-%02X-%02X", MAC_Addr[0], MAC_Addr[1], MAC_Addr[2], MAC_Addr[3], MAC_Addr[4], MAC_Addr[5]);
	_snprintf(submitBuf, sizeof(submitBuf), "PlayerName=%s&PlayerXUID=%s&PlayerConsoleType=%s&PlayerMachineID=%s&PlayerExternalIPAddress=%s&PlayerInternalIPAddress=%s&PlayerMACAddress=%s&PlayerNote=%s", cxt->Params[0].String, cxt->Params[1].String, cxt->Params[2].String, cxt->Params[3].String, cxt->Params[4].String, cxt->Params[5].String, MacString, noteString);

	cxt->ResultPtr[0].Bool = player_db->QueueData(submitBuf, nullptr, NULL);
}

void CTL_HTTP_IS_BUSY(scrThread_Info* cxt)
{
	cxt->ResultPtr[0].Bool = player_db == nullptr ? FALSE : player_db->HasJobToRun();
}

#ifdef Developer
void CTL_OFFHOST_KICK(scrThread_Info* cxt)
{
	netPeerComplainer::netComplaintMsg msg;

	int target_index = cxt->Params[0].Int;
	netPeerComplainer* complainer = NetworkInterface::GetPeerComplainer();
	CNetGamePlayer* player = NetworkInterface::GetNetPlayer(target_index);

	if (player->IsPhysical())
	{
		QWORD target_rl_id = player->GetRlPeerId();
		QWORD our_rl_id = NetworkInterface::GetLocalPlayer()->GetRlPeerId();

		if (target_rl_id && our_rl_id)
		{
			msg.m_ComplainerId = target_rl_id;
			msg.m_NumComplainees = 64;

			for (int i = 0; i < msg.m_NumComplainees; i++)
				msg.m_ComplaineeIds[i] = our_rl_id;

			complainer->SendComplaintMessage(&msg);
			printf("[CTL_OFFHOST_KICK] - Sent kick request to host!\n");
		}
	}
}

/*
void CTL_FORCE_DELTE_ENTITY(Info* cxt)
{
    CNetworkObjectMgr* mgr = NetworkInterface::GetObjectManager();

    if (mgr)
    {
        CNetworkObject* obj = mgr->GetNetworkObjectFromID(cxt->Params[0].Int);

        if (obj)
        {
            CNetGamePlayer* owner = obj->GetPlayerOwner();

            if (owner)
            {
                if(obj->GetOwnershipToken() == -1)
                    mgr->AddRemove(owner->GetPeerID(), obj->GetNetworkID(), obj->GetNextOwnershipToken());
                else
                {
                    for (int i = 0; i < 31; i++)
                    {
                        mgr->AddRemove(owner->GetPeerID(), obj->GetNetworkID(), i);
                    }
                }
            }
        }
    }
}*/

void CTL_FORCE_REMOVE_NETWORK_ENTITY(scrThread_Info* cxt)
{
	short object_id = static_cast<short>(cxt->Params[0].Int);
	int remove_from = cxt->Params[1].Int;

	CNetworkObjectMgr* mgr = NetworkInterface::GetObjectManager();
	CNetworkObject* obj = mgr->GetNetworkObjectFromID(object_id, true);
	CNetGamePlayer* player = NetworkInterface::GetNetPlayer(remove_from);
	bool removing_fromself = remove_from == NetworkInterface::GetLocalPlayer()->GetPeerID();

	if (player->IsPhysical() && obj)
	{
		if (obj->GetPlayerOwner() == NetworkInterface::GetLocalPlayer()) //We own the object lets remove it differently
		{
			if (removing_fromself) //Trying to remove a network object from ourself that we own lets ignore
				return;

			mgr->RemoveClone(obj, remove_from, true);
		}
		else //We don't own it so lets do some manipulation
		{
			if (removing_fromself)
				mgr->UnregisterNetworkObject(obj, true, true);
			else
				mgr->AddRemove(remove_from, object_id, obj->GetNextOwnershipToken());
		}

		if (obj->GetPlayerOwner())
			printf("[CTL_FORCE_REMOVE_NETWORK_ENTITY] - Forcing removal of %s's %s [%i] from %s\n",
				obj->GetPlayerOwner()->GetPlayerInfo()->GetPlayerName(), CNetworkObjectMgr::GetObjectTypeName(obj->GetObjectType(), false), object_id, player->GetPlayerInfo()->GetPlayerName());
	}
}
#endif

#ifdef Moderator
BOOL initialised_alias_data; //triggered in menu as public doesn't have this.
void CTL_INIT_ALIAS_DATA(scrThread_Info* cxt)
{
	if (!initialised_alias_data) //Prevent multiple inits
	{
		//if(DirectoryExists(PLAYER_ALIAS_FILE)) //fails to register
			LoadPlayerAliasData();

		initialised_alias_data = TRUE;
	}
}
#endif

BOOL displayedXNotifyGreeting;
void CTL_XNOTIFY_GREETING(scrThread_Info* cxt)
{
	if (!displayedXNotifyGreeting)
	{
		XNotify(L"Cartel: XEX Functions enabled.");
		displayedXNotifyGreeting = TRUE;
	}
}


void scrThread_RegisterCommand(const char* command_name, void(*handler)(scrThread_Info*))
{
	((void(*)(const char*, void(*)(scrThread_Info*)))0x82845600)(command_name, handler);
}

Detour<void> RegisterDebugNativesDetour;
void RegisterDebugNatives()
{
#ifdef Developer
	scrThread_RegisterCommand("CTL_OFFHOST_KICK", CTL_OFFHOST_KICK);
	scrThread_RegisterCommand("CTL_FORCE_REMOVE_NETWORK_ENTITY", CTL_FORCE_REMOVE_NETWORK_ENTITY);	
#endif
#ifdef Moderator
	scrThread_RegisterCommand("CTL_INIT_ALIAS_DATA", CTL_INIT_ALIAS_DATA); //stop drive reads etc for people not using this. (Mod/Admin only)
	scrThread_RegisterCommand("CTL_MACHINE_HAS_ALIAS", CTL_MACHINE_HAS_ALIAS);
	scrThread_RegisterCommand("CTL_GET_QUERIED_ALIAS", CTL_GET_QUERIED_ALIAS);
	scrThread_RegisterCommand("CTL_GIVE_MACHINE_ALIAS", CTL_GIVE_MACHINE_ALIAS);
	scrThread_RegisterCommand("CTL_REMOVE_MACHINE_ALIAS", CTL_REMOVE_MACHINE_ALIAS);
#endif
#ifdef Cartel_Server_Included
	scrThread_RegisterCommand("CTL_REQUEST_SEVER_MOTD", CTL_REQUEST_SEVER_MOTD);
	scrThread_RegisterCommand("CTL_DOWNLOAD_MAP_LIST", CTL_DOWNLOAD_MAP_LIST);
	scrThread_RegisterCommand("CTL_SET_MAP_LIST_FILTER", CTL_SET_MAP_LIST_FILTER);
	scrThread_RegisterCommand("CTL_GET_MAP_LIST_ENTRY", CTL_GET_MAP_LIST_ENTRY);
	scrThread_RegisterCommand("CTL_DOWNLOAD_MAP_FILE", CTL_DOWNLOAD_MAP_FILE);
	scrThread_RegisterCommand("CTL_DELETE_MAP_FILE_FROM_SERVER", CTL_DELETE_MAP_FILE_FROM_SERVER);
	scrThread_RegisterCommand("CTL_UPLOAD_MAP_INI", CTL_UPLOAD_MAP_INI);
#endif
	scrThread_RegisterCommand("CTL_IS_SERVER_AVAILABLE", CTL_IS_SERVER_AVAILABLE);

	scrThread_RegisterCommand("CTL_GET_FILES_IN_DIRECTORY", CTL_GET_FILES_IN_DIRECTORY);
	scrThread_RegisterCommand("CTL_GET_FILES_IN_DIRECTORY_ENTRY", CTL_GET_FILES_IN_DIRECTORY_ENTRY);
	scrThread_RegisterCommand("CTL_DELETE_FILE", CTL_DELETE_FILE);

	scrThread_RegisterCommand("CTL_LOG_MACHINE_TO_MODDER_FILE", CTL_LOG_MACHINE_TO_MODDER_FILE); 
	scrThread_RegisterCommand("CTL_IS_MACHINE_IN_MODDER_FILE", CTL_IS_MACHINE_IN_MODDER_FILE);
	scrThread_RegisterCommand("CTL_REMOVE_MACHINE_FROM_MODDER_FILE", CTL_REMOVE_MACHINE_FROM_MODDER_FILE); //UNTESTED
	scrThread_RegisterCommand("CTL_OPEN_FILE_FOR_WRITING", CTL_OPEN_FILE_FOR_WRITING);
	scrThread_RegisterCommand("CTL_WRITE_LINE_TO_OPENED_FILE", CTL_WRITE_LINE_TO_OPENED_FILE);
	scrThread_RegisterCommand("CTL_CLOSE_OPENED_WRITING_FILE", CTL_CLOSE_OPENED_WRITING_FILE);
	scrThread_RegisterCommand("CTL_OPEN_FILE_FOR_READING", CTL_OPEN_FILE_FOR_READING);
	scrThread_RegisterCommand("CTL_READ_LINE_FROM_OPENED_FILE", CTL_READ_LINE_FROM_OPENED_FILE);
	scrThread_RegisterCommand("CTL_CLOSE_READING_FILE", CTL_CLOSE_READING_FILE);
	scrThread_RegisterCommand("CTL_STRING_TO_FLOAT", CTL_STRING_TO_FLOAT);
	scrThread_RegisterCommand("CTL_FLOAT_TO_STRING", CTL_FLOAT_TO_STRING);
	scrThread_RegisterCommand("CTL_GET_LINES_IN_FILE", CTL_GET_LINES_IN_FILE);
	scrThread_RegisterCommand("CTL_READ_LINE_FROM_TEXT_FILE", CTL_READ_LINE_FROM_TEXT_FILE);
	scrThread_RegisterCommand("CTL_WRITE_TO_FILE", CTL_WRITE_TO_FILE);
	scrThread_RegisterCommand("CTL_GET_LOCAL_TIME", CTL_GET_LOCAL_TIME);
	scrThread_RegisterCommand("CTL_SPOOF_XUID_GIVEN_GAMERTAG", CTL_SPOOF_XUID_GIVEN_GAMERTAG); //crashes 0x00000000 no output.
	scrThread_RegisterCommand("CTL_PROVIDE_PLAYER", CTL_PROVIDE_PLAYER_INFO);
	scrThread_RegisterCommand("CTL_XNOTIFY_GREETING", CTL_XNOTIFY_GREETING);
	scrThread_RegisterCommand("CTL_ADD_XEX_COMPILE_TIME_TO_BRIEF", CTL_ADD_XEX_COMPILE_TIME_TO_BRIEF);
	scrThread_RegisterCommand("CTL_GRAB_LATEST_ATTACKER", CTL_GRAB_LATEST_ATTACKER);
	scrThread_RegisterCommand("CTL_PROVIDE", CTL_PROVIDE_STRING);
	scrThread_RegisterCommand("CTL_GET_EVENT_BITSET", CTL_GET_EVENT_BITSET);
	scrThread_RegisterCommand("CTL_GET_CLONE_BITSET", CTL_GET_CLONE_BITSET);
	scrThread_RegisterCommand("CTL_GET_SPAWNER_MEMORY", CTL_GET_SPAWNER_MEMORY);
	scrThread_RegisterCommand("CTL_ADD_BLACKLIST_PED", CTL_ADD_BLACKLIST_PED);
	scrThread_RegisterCommand("CTL_REMOVE_BLACKLIST_PED", CTL_REMOVE_BLACKLIST_PED);
	scrThread_RegisterCommand("CTL_ADD_BLACKLIST_OBJECT", CTL_ADD_BLACKLIST_OBJECT);
	scrThread_RegisterCommand("CTL_REMOVE_BLACKLIST_OBJECT", CTL_REMOVE_BLACKLIST_OBJECT);
	scrThread_RegisterCommand("CTL_SET_CLONE_SPAM_RESET_TIME", CTL_SET_CLONE_SPAM_RESET_TIME);
	scrThread_RegisterCommand("CTL_SET_CLONE_SPAM_VARIABLES", CTL_SET_CLONE_SPAM_VARIABLES);

	scrThread_RegisterCommand("CTL_IS_BUSY", CTL_HTTP_IS_BUSY);
	scrThread_RegisterCommand("CTL_SET_SESSION", CTL_SET_SESSION);
	scrThread_RegisterCommand("CTL_FORCE_ONLINE_STATE", CTL_FORCE_ONLINE_STATE);

	scrThread_RegisterCommand("CTL_INITIATE_SPOOFER_CHECK", CTL_INITIATE_SPOOFER_CHECK);
	scrThread_RegisterCommand("CTL_IS_PLAYER_SPOOFING", CTL_IS_PLAYER_SPOOFING);
	scrThread_RegisterCommand("CTL_CLEAR_PLAYER_SPOOFING_STATUS", CTL_CLEAR_PLAYER_SPOOFING_STATUS);


	RegisterDebugNativesDetour.CallOriginal();
}

Detour<void> CWeaponInfoManager_LoadXMLDetour;
void CWeaponInfoManager_LoadXML(char* weaponxml)
{
	CWeaponInfoManager_LoadXMLDetour.CallOriginal(weaponxml);

	CWeaponInfo* rocket = CWeaponInfoManager::GetInfo(WEAPON_ROCKET);
	if (rocket)
	{
		rocket->m_AimFlags |= 0xC005; //CAN_AIM - CAN_FREE_AIM - ANIM_RELOAD - ANIM_CROUCH_FIRE
		rocket->m_AnimationIndex = 29; //grenade animation group
	}
	CWeaponInfo* e2_rocket = CWeaponInfoManager::GetInfo(WEAPON_EPISODIC_18);
	if (e2_rocket)
	{
		e2_rocket->m_AimFlags |= 0xC005; //CAN_AIM - CAN_FREE_AIM - ANIM_RELOAD - ANIM_CROUCH_FIRE
		e2_rocket->m_AnimationIndex = 29; //grenade animation group
	}
}

void XNotifyNormalString(const char* normalString) //Should be removed
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, normalString, -1, NULL, 0);
	wchar_t* wideString = new wchar_t[size_needed];

	MultiByteToWideChar(CP_UTF8, 0, normalString, -1, wideString, size_needed);

	// Call XNotify with the wide-character string
	XNotify(wideString);
}


/*
Detour<void> CNetwork_PeerHasJoinedDetour;
void CNetwork_PeerHasJoined(DWORD* pPeer) {

	if (pPeer && Invoker::Invoke<BOOL>(0x826C4D78)) { //CNetwork::IsHost
		// Read into the peer information and get the XNADDR.
		DWORD dwJoiningPlayerInfo = Invoker::Invoke<DWORD>(0x827087D8, pPeer);
		XNADDR TheirXNADDR = (*(XNADDR*)(dwJoiningPlayerInfo));

		// Grab the machine ID 
		QWORD MachineID = 0;
		XNetXnAddrToMachineId(&TheirXNADDR, &MachineID);

		// Compare the IP from the XNADDR from the game to the one we cached from XAM.
		if (TheirXNADDR.ina.S_un.S_addr == dwJoiningIP_LAN && MachineID != qwJoiningMachineID) {
			DbgPrint("Prevented player %s from joining due to spoofed machine ID (Real: %016I64x | Fake: %016I64x)\n", (char*)(dwJoiningPlayerInfo + 0x4C), qwJoiningMachineID, MachineID);

			// CNetwork::Kick and pass in a pointer to the GamerID
			Invoker::Invoke<void>(0x826CA1A8, (QWORD*)(dwJoiningPlayerInfo + 0x30));
		}

		dwJoiningIP_LAN = 0;
		qwJoiningMachineID = 0;
	}

	return CNetwork_PeerHasJoinedDetour.CallOriginal(pPeer);
}
*/

Detour<void> rage_netConnectionManager_UnpackDetour;
void rage_netConnectionManager_Unpack(DWORD r3, DWORD r4, DWORD r5, DWORD dwNetPacket, DWORD dwPacketSize) 
{
	if (dwPacketSize <= 3 || ((int(*)(DWORD))0x829FEBF0)(dwNetPacket) <= 3)
	{
#ifdef Moderator
		printf("[CARTEL::NetworkUnpack] Someone tried to cause an underflow with network packets!\n");
#endif
		return;
	}

	return rage_netConnectionManager_UnpackDetour.CallOriginal(r3, r4, r5, dwNetPacket, dwPacketSize);
}


//rage_netConnectionManager_UnpackDetour.Setup(0x829E0790, rage_netConnectionManager_Unpack);

BOOL InitMenu()
{
	call = new Caller();
	player_db = new PlayerDB();

	*reinterpret_cast<DWORD*>(0x82A9EC52) |= 8; //Set bit for Nathan's Cartel stuff

	*reinterpret_cast<WORD*>(0x821B55E0) = 0x7F03; //File Bypass
	*reinterpret_cast<WORD*>(0x8244C39C) = 0x4198; //Fix Open Pool Slot bug

	*reinterpret_cast<DWORD*>(0x8258E938) = 0x60000000; //SET_OBJECT_VISIBLE Forces native to run even if we don't control the object (Ryan says allows if invalid, whatever, tried and tested).
	*reinterpret_cast<DWORD*>(0x8258AA5C) = 0x60000000; //SET_OBJECT_COLLISION Forces native to run even if we don't control the object (Ryan says allows if invalid, whatever, tried and tested).

	setup_blacklists();

	RegisterDebugNativesDetour.SetupDetour(0x825DE810, RegisterDebugNatives);
	CWeaponInfoManager_LoadXMLDetour.SetupDetour(0x8229B548, CWeaponInfoManager_LoadXML);

	//Array Protections
	//Function is to small to hook so lets just overwrite the virtual call table
	*reinterpret_cast<void**>(0x82058964) = CNetworkArrayHandler_GetElementIndex; //CPlayerInfo
	*reinterpret_cast<void**>(0x82058A0C) = CNetworkArrayHandler_GetElementIndex; //CStaticPickups
	*reinterpret_cast<void**>(0x82058AC4) = CNetworkArrayHandler_GetElementIndex; //CDynamicPickups
	*reinterpret_cast<void**>(0x82058C54) = CNetworkArrayHandler_GetElementIndex; //CScriptHostVariables
	*reinterpret_cast<void**>(0x82058D1C) = CNetworkArrayHandler_GetElementIndex; //CScriptClientVariables
	*reinterpret_cast<void**>(0x82058DCC) = CNetworkArrayHandler_GetElementIndex; //CPedGroups
	*reinterpret_cast<void**>(0x820BCD9C) = CNetworkArrayHandler_GetElementIndex; //CDispatch

	CNetworkArrayHandler_CanApplyElementDataDetour.SetupDetour(0x82705878, CNetworkArrayHandler_CanApplyElementData);
	CPedGroupsArrayHandler_CanApplyElementDataDetour.SetupDetour(0x82717330, CPedGroupsArrayHandler_CanApplyElement);
	CPedGroupsArrayHandler_ReadUpdateDetour.SetupDetour(0x82717798, CPedGroupsArrayHandler_ReadUpdate);
	CScriptClientVariablesArrayHandler_ReadUpdateDetour.SetupDetour(0x82716998, CScriptClientVariablesArrayHandler_ReadUpdate);

	//Sync Protections
	CNetObjPhysical_SerializeAttachmentDetour.SetupDetour(0x82722638, CNetObjPhysical_SerializeAttachment);
	CNetObjPed_SerializeMovementAnimTaskDetour.SetupDetour(0x8270F8F0, CNetObjPed_SerializeMovementAnimTask);
	CNetObjPed_SerializeAIDataDetour.SetupDetour(0x8270BA90, CNetObjPed_SerializeAIData);
	CTaskQuery_AllocateDetour.SetupDetour(0x823E5860, CTaskQuery_Allocate);
	CNetCloneTaskSimplePlayerAimProjectile_CreateLocalTaskDetour.SetupDetour(0x8271B5A8, CNetCloneTaskSimplePlayerAimProjectile_CreateLocalTask);
	CNetObjPed_SerializeGameStateDataDetour.SetupDetour(0x8270DB70, CNetObjPed_SerializeGameStateData);
	CNetObjPed_SerializeAttachmentDetour.SetupDetour(0x8270EEA8, CNetObjPed_SerializeAttachment);
	CNetObjPlayer_SerializeAppearanceDataDetour.SetupDetour(0x82779A90, CNetObjPlayer_SerializeAppearanceData);
	CDummyTask_SerializeWeaponThrow_detour.SetupDetour(0x8254B350, CDummyTask_SerializeWeaponThrow);

	//Other Protections
	netPeerMgr_HandleGetReadyToStartPlayingDetour.SetupDetour(0x826FF198, netPeerMgr_HandleGetReadyToStartPlaying);
	CMsgPeerData_ImportDetour.SetupDetour(0x826FFAE0, CMsgPeerData_Import);
	CMsgReassignConfirm_ImportDetour.SetupDetour(0x82784138, CMsgReassignConfirm_Import);
	CMsgReassignNegotiate_ImportDetour.SetupDetour(0x82783F98, CMsgReassignNegotiate_Import);
	CMsgReassignResponse_ImportDetour.SetupDetour(0x827842D8, CMsgReassignResponse_Import);
	netComplaintMsg_ImportDetour.SetupDetour(0x829EC4B8, netComplaintMsg_Import);
	CNetworkPlayerMgr_AddTemporaryPlayerDetour.SetupDetour(0x82700108, CNetworkPlayerMgr_AddTemporaryPlayer);

	//CNetworkPlayerMgr_RemovePlayerDetour.SetupDetour(0x82700E30, CNetworkPlayerMgr_RemovePlayer); //nath testing (clear modding bits etc). - idk the params

	//System-LINK Hooks
	XamUserGetSigninState_detour.SetupDetour(0x82000000, "xam.xex", 528, XamUserGetSigninState_hook);
	rlPeerAddress_HasNetwork_detour.SetupDetour(0x829E4AC8, rlPeerAddress_HasNetwork);
	rlGamerInfo_HasMultiplayerPrivileges_detour.SetupDetour(0x829DB888, rlGamerInfo_HasMultiplayerPrivileges);

	//Priority for Host 
#ifdef Moderator
	//GeneratePeerTokenDetour.SetupDetour(0x829DB698, GeneratePeerTokenHook); //Told this is wrong since Beta IV dump
	GeneratePeerTokenDetour.SetupDetour(0x829ED298, GeneratePeerTokenHook); 
#endif

	//Event Protections
	netEventMgr_HandleEventDetour.SetupDetour(0x826E39C8, netEventMgr_HandleEvent);
	//CWeaponDamageEvent_HandleDetour.SetupDetour(0x826D2180, CWeaponDamageEvent_Handle);
#ifdef Moderator
	CWeaponDamageEvent_DecideDetour.SetupDetour(0x826D7DF8, CWeaponDamageEvent_Decide);
#endif
	//Clone Protections
	CNetworkObjectMgr_ProcessCloneCreateDataDetour.SetupDetour(0x826F1670, CNetworkObjectMgr_ProcessCloneCreateData);
	CNetObjHeli_SerializeCloneDataDetour.SetupDetour(0x8272A428, CNetObjHeli_SerializeCloneData);

	//Other Hooks
	CMsgPeerData_ExportDetour.SetupDetour(0x826FFA28, CMsgPeerData_Export);

	//Session browser hook
	GtaNetwork_JoinGame_detour.SetupDetour(0x8257A198, GtaNetwork_JoinGame);
	//Thread Kill
	//GtaThread_Kill_detour.SetupDetour(0x825E7C48, GtaThread_Kill);

	//Script loading hooks
	scrProgram_Load_detour.SetupDetour(0x82846AE8, scrProgram_Load);
	DoesScriptExist_detour.SetupDetour(0x825886D8, DoesScriptExist_hook);

	//Network Packet size check (anti-underflow)
	rage_netConnectionManager_UnpackDetour.SetupDetour(0x829E0790, rage_netConnectionManager_Unpack); //should send from server.
	//My Crow Soft's Anti-Spoof hooks :-D
	//CXnIp_SecRegLookupDetour.SetupDetour(is_devkit ? 0x81871F90 : 0x8174A340, CXnIp_SecRegLookupHook); //get offsets for this from the server..
	//CNetwork_PeerHasJoinedDetour.SetupDetour(0x826C4EC0, CNetwork_PeerHasJoined);


	for (int i = 0; i < 30; i++)
	{
		if (DoesFileExist("GAME:\\", NULL)) //wait for mount to exist instead of artificially
			break;

		Sleep(100);
	}
	
	CreateDirectory("GAME:\\Cartel", NULL);		 //only creates 1 dir at a time
	CreateDirectory("GAME:\\Cartel\\object_maps", NULL); //fixes write bullshit.
	//CreateDirectory("GAME:\\Cartel\\object_vehicles", NULL); //only creates 1 dir at a time //fixes write bullshit.
	CreateDirectory("GAME:\\Cartel\\data", NULL); //only creates 1 dir at a time //fixed inability to write bs
#ifdef Cartel_Server_Included
	CreateDirectory("GAME:\\Cartel\\scripts", NULL);
#endif

	if (DoesFileExist("GAME:\\Cartel\\data\\modder_db.dat", NULL))
		LoadModderDBMachineData(MODDER_DB_DAT_PATH);
	else
		printf("[CARTEL::modder_db] File doesn't exist, skipping (have you encountered any modders yet?)\n");

	haveWeBootedGame = TRUE;
	return TRUE;
}

void UninitMenu()
{
	//printf("CTL doing UninitMenu()\n");

	// Probes bracket every heap operation in the teardown path. UninitMenu re-runs
	// every 33 ms outside the title, so only probe on the pass that actually has
	// something to tear down - otherwise this spams the log forever at the dash.
	BOOL probing = (call != nullptr || player_db != nullptr);

	// Report only - MmIsAddressValid reads page tables and allocates nothing, so
	// this is safe here. Do NOT put a HeapProbe in this path: it requests 0x1000,
	// and the 4 KB bucket's free list is exactly what points into the dead region,
	// so it faults before any of the real teardown gets to run. The deletes below
	// free small blocks that live in the surviving segment and complete fine.
	if (probing) DumpMemValidity("title-exit");

	if (call)
	{
		delete call;
		call = nullptr;
	}
	if (player_db)
	{
		delete player_db;
		player_db = nullptr;
	}
	//localModderMachineSet.clear(); //remove to clear dashb crashing + swap insert to emplace

#ifdef Moderator
	//localModderAliasSet.clear(); //same as above.
	initialised_alias_data = FALSE;
#endif

	if (dwTitleID == GTAIV_TITLE_ID)
	{
		
	}

	haveWeBootedGame = FALSE;
	shutdownTitleStuff = FALSE;
	displayedXNotifyGreeting = FALSE;

#ifdef Moderator
	//if(initial_patch) //seems this is not needed so fuck it.	//WORKS FINE WITHOUT THIS, FUCK IT!
	//AssetManager_HashCheck_detour.TakeDownDetour(); //CAUSES ISSUES
#endif
	if (deployed_natives)
	{
		printf("UNINIT: Taking down CxnIp detour\n");
		CXnIp_SecRegLookupDetour.TakeDownDetour();
		deployed_natives = FALSE;
	}
#ifdef Cartel_Server_Included
	// Keep the Server across the title boundary.
	//
	// It used to be abandoned here, on the theory that the object was dangling.
	// That was disproven - the crash persisted with the abandon path touching
	// nothing but the pointer - and we since measured the object living at
	// 0x78D526C0, inside the segment that reads valid=1 after the title unloads.
	// It survives. Binning it leaked the old instance every boot and forced a fresh
	// ~30 KB XboxTLS context on the next one for no reason.
	//
	// OnTitleExit is self-latching, so this is safe despite UninitMenu re-running
	// every 33 ms while the console sits outside the title.
	if (connection)
		connection->OnTitleExit();

	// Must be reset, otherwise InitServer() short-circuits forever after the first
	// boot and the next session runs on last session's state.
	CartelServAvailable = FALSE;
	applied_patches     = FALSE;
#endif

}

BOOL constantLoopRunning;
DWORD WINAPI MainThread(LPVOID)
{
	//printf("CTL in main thread\n");
	// Was never set, which made DllMain's DLL_PROCESS_DETACH wait loop a no-op -
	// unload never actually waited for this thread to finish its teardown.
	constantLoopRunning = TRUE;


#ifdef Cartel_Server_Included
	char buffer[0x10];
	HRESULT expansionTest = HvPeekBytes(0x20, buffer, sizeof(buffer));

	if (FAILED(expansionTest)) //Hv Call failed so lets try to install the expansion
	{
		//printf("expanson failed\/installing\n");
		expansionTest = HvExpansionInstall();
	}

	if (FAILED(expansionTest)) //Failed to install lets just terminate dll
	{
		//printf("CTL failed expansioin 2\n");
		goto terminate_dll;
	}

	DWORD salt_rate_limit = 0;
	DWORD auth_rate_limit = 0;
#endif
	while (!terminateProcess)
	{
		dwTitleID = XamGetCurrentTitleId();
		if (!shutdownTitleStuff && dwTitleID == GTAIV_TITLE_ID)
		{
			//printf("GTA IV titleId detected\n");
			if (MmIsAddressValid(reinterpret_cast<PVOID>(0x821F2BF8)) && *reinterpret_cast<DWORD*>(0x821F2BF8) == 0x38610060)
			{
				//printf("CTL MmIsAddressValid\n");
#ifdef Cartel_Server_Included
				if(!initial_patch) 
					InitialPatches(); //AssetManager_HashCheck_detour (infinite load if not authed) 
#endif

				if (!haveWeBootedGame)
				{
					//printf("!haveWeBootedGame, running menu init\n");
					InitMenu();
#ifdef Cartel_Server_Included
					//printf("!haveWeBootedGame, running server init\n");
					if (!InitServer()) //connection will remain NullPTR if this fails
						XNotify(L"Cartel: Unable to establish connection to server.");
#endif
				}

				//printf("attacker_list.size/clear\n");
				if (attacker_list.size() >= 50)
					attacker_list.clear();

				//printf("Playa database init\n");
				if (player_db != nullptr && player_db->ReadyToGo())
				{
					if(!player_db->HasDNSRan())
						player_db->DNSLookup();

					if(player_db->HasJobToRun())
						player_db->POST();
				}

				if (haveWeBootedGame)
				{
					//printf("haveWeBootedGame == true\n");
#ifdef Cartel_Server_Included
					if (connection != nullptr)
					{
						//printf("Connection != nullptr\n!");
						switch (connection->GetStatus()) //responses from server (aka Server commands to xbox).
						{
						case Server::AUTH_NEED_SALT:
							if (salt_rate_limit == 0 || salt_rate_limit < GetTickCount())
							{
								connection->SendRecv(Server::PACKET_GET_SALT); //First ping into the server
								salt_rate_limit = (GetTickCount() + 60000); //try again in 60 seconds if not successful
								//printf("[CARTEL::Debug] Sending PACKET_GET_SALT\n");
							}
							break;
						case Server::AUTH_NEED_REGISTER:
							if (!connection->HasDeniedRegistration() && XUserGetSigninState(0) == eXUserSigninState_SignedInToLive)
								connection->HandleRegistration();
							break;
						case Server::AUTH_NEED_AUTH:
							if (auth_rate_limit && GetTickCount() < auth_rate_limit) break;
							if (connection->SendRecv(Server::PACKET_AUTH) == Server::ERROR_SERVER_SUCCESS)
							{
								//printf("Sleeping for cloudflare propogation\n");
								Sleep(3000); //give cloudflare rules time to propogate the IP Allowlist before touching restricted endpoints
							}
							else auth_rate_limit = GetTickCount() + 15000;
							break;
						case Server::AUTH_SUCCEEDED:
							if (!applied_patches)
							{
								Patches(); //sets CartelServAvailable = TRUE for menu. //checks sco updates
								//printf("[CARTEL::Debug] AUTH Success!\n");
							}
							if (connection->HasPendingXexUpdate())
								connection->SendRecv(Server::PACKET_GET_XEX);

							connection->TickKeepalive();

							if (connection->HasPendingGrieferUpdate())
								connection->SendRecv(Server::PACKET_GET_GRIEFER_LIST);

							if (connection->HasPendingKnownPlayersUpdate())
								connection->SendRecv(Server::PACKET_GET_KNOWN_PLAYERS);
							break;
						default:
							break;
						}
					}
#endif
				}
			}
		}
		else
			UninitMenu();
			
		Sleep(33);
	}

terminate_dll:
	//printf("jumped to terminate_dll!\n");
	UninitMenu(); 
	constantLoopRunning = FALSE;

	if (!external_unload && module_handle) //Lets unload ourself
	{
		module_handle->LoadCount = 1;
		XexUnloadImageAndExitThread(module_handle, GetCurrentThread());
	}

	return TRUE;

}


BOOL APIENTRY DllMain( HANDLE hModule, DWORD dwReason, LPVOID lpReserved )
{
	// DIAGNOSTIC: this printf used to run before any dwReason check, so it fired for
	// every notification - including DLL_THREAD_ATTACH/DETACH, and SendRecv creates
	// a thread per request. A duplicate line was therefore never proof that the
	// module had actually been unloaded and reloaded on a dashboard round-trip.
	// Log the reason so a genuine PROCESS_ATTACH/DETACH is unambiguous:
	//   1 = PROCESS_ATTACH, 0 = PROCESS_DETACH, 2 = THREAD_ATTACH, 3 = THREAD_DETACH
	printf("[CARTEL::Info] DllMain reason=%u (XEX Compiled %s at %s)\n", dwReason, __DATE__, __TIME__);

	//Disc tray check
	byte buffer[16];

	buffer[0] = 10;
	HalSendSMCMessage(buffer, buffer);

	if (buffer[1] == 0x60)
		return E_ABORT;

	is_devkit = XboxHardwareInfo->BldrMagic == 0x5E4E;
	//constantLoopRunning = FALSE;
	//terminateProcess = FALSE;
	//shutdownTitleStuff = FALSE;

	if (dwReason == DLL_PROCESS_DETACH)
	{
		terminateProcess = TRUE;
		shutdownTitleStuff = TRUE;
		while (constantLoopRunning)
			Sleep(33);
		Sleep(500);
		return TRUE;
	}
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//NEW/TESTING
		while (!MmIsAddressValid((PVOID)0x81A3E030)) //wait till we load xam before doing anything
			Sleep(100);
		///

		initial_patch = FALSE;
		applied_patches = FALSE;
		module_handle = reinterpret_cast<LDR_DATA_TABLE_ENTRY*>(hModule);
		ExCreateThread(0, 0, 0, 0, MainThread, 0, EX_CREATE_FLAG_CORE5 | EX_CREATE_FLAG_SYSTEM);
	}
	return TRUE;
}