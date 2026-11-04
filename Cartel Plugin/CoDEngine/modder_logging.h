#pragma once
#include <unordered_set>
#include <vector>

extern char modder_db_dat_path[64];
extern char aliases_db_dat_path[64];
extern bool AddMachineToModderFile(UINT64 MachineID);

void CTL_IS_MACHINE_IN_MODDER_FILE(scrThread_Info* cxt);
void CTL_LOG_MACHINE_TO_MODDER_FILE(scrThread_Info* cxt);
void CTL_REMOVE_MACHINE_FROM_MODDER_FILE(scrThread_Info* cxt);
void LoadModderDBMachineData(const char* filePath); //"GAME:\\Cartel\\data\\modder_db.ini"
extern std::unordered_set<UINT64> localModderMachineSet;  // Set to store Machine IDs for quick lookup


void CTL_MACHINE_HAS_ALIAS(scrThread_Info* cxt);
void CTL_GET_QUERIED_ALIAS(scrThread_Info* cxt);
void CTL_GIVE_MACHINE_ALIAS(scrThread_Info* cxt);
void CTL_REMOVE_MACHINE_ALIAS(scrThread_Info* cxt);
void LoadPlayerAliasData(void);
extern std::vector<std::tuple<UINT64, std::string>> localModderAliasSet;

void CTL_INITIATE_SPOOFER_CHECK(scrThread_Info* cxt);
void CTL_IS_PLAYER_SPOOFING(scrThread_Info* cxt);
void CTL_CLEAR_PLAYER_SPOOFING_STATUS(scrThread_Info* cxt);

/*
struct XuidCheckData
{
	byte m_playerIndex;
	char m_gamertag[16];
	QWORD m_Xuid;
	QWORD m_machineId;
};
extern XuidCheckData playerData[16];

void XUIDCheckThread(XuidCheckData* checkData);*/