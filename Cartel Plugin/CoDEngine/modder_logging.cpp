#include "stdafx.h"
#include "Defines.h"
#include "caller.h"
#include <unordered_set>
#include <algorithm>
#include "natives.h"
#include "modder_logging.h"

char modder_db_dat_path[64];
char aliases_db_dat_path[64];

std::unordered_set<UINT64> localModderMachineSet;

#ifdef Moderator
std::vector<std::tuple<UINT64, std::string>> localModderAliasSet;
void LoadPlayerAliasData(void)  //make this a native so only mods/admins call it.
{
	printf("[CARTEL::Debug] Clearing localModderAliasSet before pulling data in\n");
	localModderAliasSet.clear();
	std::ifstream file(PLAYER_ALIAS_FILE);
	if (!file.is_open())
	{
		printf("Error opening %s for reading\n", PLAYER_ALIAS_FILE);
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string idStr, alias;

		if (std::getline(ss, idStr, '|') && std::getline(ss, alias))
		{
			UINT64 id = std::stoull(idStr);
			localModderAliasSet.push_back(std::make_tuple(id, alias));
		}
	}
	printf("Successfully read %s, closing.\n", PLAYER_ALIAS_FILE);
	file.close();
}

bool GetPlayerAlias(UINT64 id, std::string& outAlias) 
{
	// Using iterator instead of range-based for loop
	for (std::vector<std::tuple<UINT64, std::string>>::iterator it = localModderAliasSet.begin(); it != localModderAliasSet.end(); ++it)
	{
		if (std::get<0>(*it) == id)
		{
			outAlias = std::get<1>(*it);
			return true;
		}
	}
	return false;
}

char alias_tag[16];
void CTL_MACHINE_HAS_ALIAS(scrThread_Info* cxt)
{
	UINT64 MachineID = *(UINT64*)cxt->Params[0].Int; //Yes, take a copy, ya never know.. players jumble all the time.
	std::string players_alias;
	if (GetPlayerAlias(MachineID, players_alias))
	{
		strncpy(alias_tag, players_alias.c_str(), 16);
		cxt->ResultPtr[0].Bool = TRUE;
	}
	else
		cxt->ResultPtr[0].Bool = FALSE;
}

void CTL_GET_QUERIED_ALIAS(scrThread_Info* cxt)
{
	cxt->ResultPtr[0].String = alias_tag;
}

bool AddModderAlias(UINT64 id, const std::string& alias) {
	auto entry = std::make_tuple(id, alias);

	if (std::find(localModderAliasSet.begin(), localModderAliasSet.end(), entry) == localModderAliasSet.end())
	{
		std::ofstream file(PLAYER_ALIAS_FILE, std::ios::app);
		if (file.is_open())
		{
			file << id << "|" << alias << "\n";
			localModderAliasSet.push_back(entry);
			return true;
		}
		else
			printf("Error: Unable to open %s for writing.", PLAYER_ALIAS_FILE);
	}
	return false;
}

void CTL_GIVE_MACHINE_ALIAS(scrThread_Info* cxt)
{
	UINT64 MachineID = *(UINT64*)cxt->Params[0].Int; //Yes, take a copy, ya never know.. players jumble all the time.
	char* provided_alias = cxt->Params[1].String;
	std::string converted_alias = provided_alias;
	if (AddModderAlias(MachineID, converted_alias))
		cxt->ResultPtr[0].Bool = TRUE;
	else
		cxt->ResultPtr[0].Bool = FALSE;
}

bool RemovePlayerAlias(UINT64 id)  //add .tmp file stuff for safety, and improve this awful logic
{
	// First, remove from the unordered_set
	bool found;
	auto it = localModderAliasSet.begin();
	while (it != localModderAliasSet.end()) 
	{
		if (std::get<0>(*it) == id) 
		{
			found = true;
			break;
		}
		++it;
	}

	// Open the file and remove the corresponding line
	std::ifstream file(PLAYER_ALIAS_FILE);
	if (!file.is_open()) 
	{
		printf("Error: Could not open %s.", PLAYER_ALIAS_FILE);
		return false;
	}

	std::string line;
	std::stringstream updatedContent;
	bool aliasFound = false;

	// Read each line from the file and rewrite the ones that don't match the ID
	while (std::getline(file, line)) 
	{
		std::istringstream ss(line);
		std::string idStr, alias;
		if (std::getline(ss, idStr, '|') && std::getline(ss, alias)) 
		{
			UINT64 fileId = std::stoull(idStr);
			if (fileId == id) 
			{
				aliasFound = true;  // Found the alias, so skip it
				continue;
			}
		}
		updatedContent << line << "\n";
	}
	file.close();

	// If we found and removed the alias, rewrite the file with updated content
	if (aliasFound) 
	{
		std::ofstream outFile(PLAYER_ALIAS_FILE);
		if (!outFile.is_open())
		{
			printf("Error: Could not write to %s.", PLAYER_ALIAS_FILE);
			return false;
		}
		outFile << updatedContent.str();
		outFile.close();
		if (it != localModderAliasSet.end())
			localModderAliasSet.erase(it);  // Remove from set
		return true;  // Successfully removed
	}

	return false;  // Alias not found in the file
}

void CTL_REMOVE_MACHINE_ALIAS(scrThread_Info* cxt)
{
	UINT64 MachineID = *(UINT64*)cxt->Params[0].Int; //Yes, take a copy, ya never know.. players jumble all the time.
	if (RemovePlayerAlias(MachineID))
		cxt->ResultPtr[0].Bool = TRUE;
	else
		cxt->ResultPtr[0].Bool = FALSE;
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// modder_db.dat stuff AKA logging reliably detected modders machine id's incase they join stealthy later. ////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LoadModderDBMachineData(const char* filePath)
{
	static bool Loaded_ModderDB = false;
	if (!Loaded_ModderDB)
	{
		printf("[CARTEL::Debug] Clearing modderMachineSet before pulling new data in.\n");
		localModderMachineSet.clear();
		std::ifstream inFile(filePath);
		if (!inFile)
		{
			printf("[CARTEL::modder_db] Failed to open %s.\n", filePath);
			return;
		}
		std::string line;
		int lineNum = 0;
		while (std::getline(inFile, line))
		{
			++lineNum;
			std::istringstream iss(line);
			UINT64 machineID;
			if (iss >> machineID)
			{
				localModderMachineSet.insert(machineID);
			}
			else
			{
				printf("[CARTEL::modder_db] Warning: Could not parse line %d: '%s'\n", lineNum, line.c_str());
			}
		}
		printf("[CARTEL::modder_db] Loaded %i entries from %s\n", localModderMachineSet.size(), filePath);
		inFile.close();
		Loaded_ModderDB = true;
	}
	else
		printf("[CARTEL::Debug] Skipping ModderDB init as already done, entries presently == %i\n", localModderMachineSet.size());
}

//byte* MIDpoint says this is a pointer
//(Byte*) says upcoming is a pointer
//&MachineID grabs pointer of MachineID.
//byte* MIDPoint = (byte*)&MachineID;

void CTL_REMOVE_MACHINE_FROM_MODDER_FILE(scrThread_Info* cxt) //un-finished
{
	UINT64 MachineID = *(UINT64*)cxt->Params[0].Int;  //Yes, take a copy, shit happens.
	if (localModderMachineSet.find(MachineID) != localModderMachineSet.end()) //check if it's in the list
	{
		std::ifstream file(MODDER_DB_DAT_PATH);
		if (!file.is_open()) 
		{
			printf("[CARTEL::modder_db] Error: Could not open %s.", MODDER_DB_DAT_PATH);
			cxt->ResultPtr[0].Bool = FALSE;
			return;
		}

		std::string line;
		std::stringstream updatedContent;
		bool removeTargetFound = false;
		int lineNum = 0;
		// Read each line from the file and rewrite the ones that don't match the ID
		while (std::getline(file, line))
		{
			++lineNum;
			std::istringstream iss(line);
			UINT64 readMachineID;
			if (iss >> readMachineID)
			{
				if (readMachineID == MachineID)
				{
					removeTargetFound = true;
					continue; //skip we need to remove it.
				}
				updatedContent << line << "\n";
			}
		}
		file.close();

		// If we found and removed the alias, rewrite the file with updated content
		if (removeTargetFound)
		{
			std::ofstream tempFile(MODDER_DB_DAT_PATH);
			if (!tempFile.is_open())
			{
				printf("[CARTEL::modder_db] Error: Could not write to file %s.\n", MODDER_DB_DAT_PATH);
				cxt->ResultPtr[0].Bool = FALSE;
				return;
			}
			tempFile << updatedContent.str();
			tempFile.close();

			localModderMachineSet.erase(MachineID);  // Keep set in sync
			cxt->ResultPtr[0].Bool = TRUE;
			return;
		}
	}
	cxt->ResultPtr[0].Bool = FALSE;
}

bool AddMachineToModderFile(UINT64 MachineID)
{
	if (localModderMachineSet.find(MachineID) != localModderMachineSet.end()) //Check if the MachineID is already in memory.
	{ //No need to append to file
		return false;
	}

	std::ofstream outFile(MODDER_DB_DAT_PATH, std::ofstream::app);  // Open file in append mode
	if (!outFile)
	{
		printf("[CARTEL::modder_db] Unable to open %s for writing\n", MODDER_DB_DAT_PATH);
		return false;
	}

	outFile << MachineID << std::endl; //Append the new MachineID to the file
	outFile.close();

	localModderMachineSet.insert(MachineID); //Update the in-memory set with the new MachineID
	return true;
}

void CTL_LOG_MACHINE_TO_MODDER_FILE(scrThread_Info* cxt)
{
	UINT64 MachineID = *(UINT64*)cxt->Params[0].Int;  //Yes, take a copy, shit happens.
	cxt->ResultPtr[0].Bool = AddMachineToModderFile(MachineID);
}

void CTL_IS_MACHINE_IN_MODDER_FILE(scrThread_Info* cxt)
{
	UINT64 MachineID = *(UINT64*)cxt->Params[0].Int; //Yes, take a copy, ya never know.. players jumble all the time.
	cxt->ResultPtr[0].Bool = localModderMachineSet.find(MachineID) != localModderMachineSet.end();
}


//XuidCheckData playerData[16];
/* //the fuckers are spoofing machineId so we will just junk this for now, ultimately we are not losing much if anything at all. (They linger for a couple of seconds before getting a connection fail)
void XUIDCheckThread(XuidCheckData* checkData) 
{
	if (localModderMachineSet.find(checkData->m_machineId) != localModderMachineSet.end()) //their machine ID is in the modder_db file.
	{
		//junked stuff here will change...
	}
	else //no point checking if they are modding if we already have them in the file.
	{
#ifdef Moderator
		if (GrabXUID(checkData->m_gamertag) != checkData->m_Xuid)
		{
			printf("[NATH XEX TEST] %s detected with a mis-matched XUID, logging to modder file.\n", checkData->m_gamertag);

			if (AddMachineToModderFile(checkData->m_machineId)) //save the script having to do it, will return if they are already in-memory.
				printf("[NATH XEX TEST] %s machineId written to modder_db file\n", checkData->m_gamertag);
			else
				printf("[NATH XEX TEST] Failed to write %s machineId to modder_db file.\n", checkData->m_gamertag); //if the machine is already here we won't be calling this code.
				//printf("[NATH XEX TEST] Not writing %s machineId to modder_db file (already present)\n", checkData->m_gamertag);
		}
		else
			printf("%s seems to have a valid xuid/gt pair!\n", checkData->m_gamertag);
#else
		if (GrabXUID(checkData->m_gamertag) != checkData->m_Xuid)
			AddMachineToModderFile(checkData->m_machineId);
#endif
	}
}
*/
/*
XUID GrabXUID(char* gamertag) //works //17599 Dash - I suspect this returns 0 if the tag is invalid.. should use this for spoofing tags as THIS function doesn't crash you.
{
	ZeroMemory((void*)0x81B1E000, 0xF);
	((void(*)(XUID, int, char*, int, int, int))0x81829D60)(0x9000006F93463, 0, gamertag, 0x18, 0x81B1E000, 0); //XUserFindUser
	return *(XUID*)0x81B1E000;
}*/


XUID GrabXUID(char* gamertag)
{
	BYTE buffer[0x10]; 
	ZeroMemory(buffer, sizeof(buffer));
	((void(*)(XUID, int, char*, int, void*, int))0x81829D60)(0x9000006F93463, 0, gamertag, 0x18, buffer, 0); //XUserFindUser
	return *(XUID*)buffer;
}

int checking_xuid_index_storage[16] = { INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX, INVALID_PLAYER_INDEX };
bool player_has_mismatched_xuid[16]; //they wre false by fdefault // = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };
char provided_player_name_for_checking[16][16]; //Cartel Menu will overwrite player names sometimes for different reasons, so in those instances we pass a backup which is their clean tag.

void XUIDCheckThread(int* pIndex)
{
	int PlayerIndex = *pIndex;
	CPlayerInfo* playerInfo = NetworkInterface::GetPlayerInfo(PlayerIndex);
	//printf("[NATH XEX TEST] XUIDCheckThread(int* pIndex) (%d) (%s)\n", PlayerIndex, provided_player_name_for_checking[PlayerIndex]);
	if (playerInfo)
	{
		//printf("[NATH XEX TEST] XUIDCheckThread... if(playerInfo)...\n");
		if (GrabXUID(provided_player_name_for_checking[PlayerIndex]) != playerInfo->m_GamerHandle)
		{
			player_has_mismatched_xuid[PlayerIndex] = TRUE;
			printf("[CARTEL::XuidCheck] %s's XUID doesn't match their gamertag.\n", provided_player_name_for_checking[PlayerIndex]);
		}
		//else
			//printf("[Cartel.xex] XUIDCheckThread.. %s's XUID matches their gamertag.\n", provided_player_name_for_checking[PlayerIndex]);

		checking_xuid_index_storage[PlayerIndex] = INVALID_PLAYER_INDEX; //we have stopped checking.
	}
}

void CTL_INITIATE_SPOOFER_CHECK(scrThread_Info* cxt)
{
	int PlayerIndex = cxt->Params[0].Int;
	//printf("[NATH XEX TEST] CTL_INITIATE_SPOOFER_CHECK(%d)\n", PlayerIndex);
	if (checking_xuid_index_storage[PlayerIndex] != INVALID_PLAYER_INDEX) //we are currently checking this slot
	{
		//printf("[NATH XEX TEST] CTL_INITIATE_SPOOFER_CHECK(%d)... we are already checking.. this shouldn't be happening.\n", PlayerIndex);
		return;
	}
	player_has_mismatched_xuid[PlayerIndex] = FALSE; //nuke any stray data (probably not needed just good hygeine)
	strncpy(provided_player_name_for_checking[PlayerIndex], cxt->Params[1].String, 16);
	//printf("[NATH XEX TEST] CTL_INITIATE_SPOOFER_CHECK(%d)... Starting XUIDCheckThread and passing &checking_xuid_index_storage[%d]\n", PlayerIndex, PlayerIndex);
	checking_xuid_index_storage[PlayerIndex] = PlayerIndex;
	ExCreateThread(0, 0, 0, 0, (LPTHREAD_START_ROUTINE)XUIDCheckThread, &checking_xuid_index_storage[PlayerIndex], 0);
}

void CTL_IS_PLAYER_IP_SPOOFED(scrThread_Info* cxt)
{

}

void CTL_IS_PLAYER_SPOOFING(scrThread_Info* cxt)
{
	int PlayerIndex = cxt->Params[0].Int;
	if (checking_xuid_index_storage[PlayerIndex] != INVALID_PLAYER_INDEX)
	{	//we are currently checking this slot
		cxt->ResultPtr[0].Int = -1; //pendding / in progress
		return;
	}
	cxt->ResultPtr[0].Int = player_has_mismatched_xuid[PlayerIndex];
}

void CTL_CLEAR_PLAYER_SPOOFING_STATUS(scrThread_Info* cxt)
{
	int PlayerIndex = cxt->Params[0].Int;
	checking_xuid_index_storage[PlayerIndex] = INVALID_PLAYER_INDEX;
	player_has_mismatched_xuid[PlayerIndex] = FALSE;
}