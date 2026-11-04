#pragma once
#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "common.h"
#include "cartel_helper.h"

char* bad_player_model_name(uint playerModel, bool *cutScene);
CPlayerInfo* GetPlayerPointer(PLAYER_INDEX playerIndex);
PLAYER_INDEX IsThisPedAPlayer(PED_INDEX ped);

//INFO: Make sure you check IsPlayerValid and PLAYER_HAS_CHAR before calling this.
bool IsPlayerInvincible_ctlHelper(PLAYER_INDEX playerIndex, PED_INDEX playerPed); //DONT CALL WITHOUT DOING VALID AND CHAR CHECKS FIRST
bool IsPlayerOnFriendsList(PLAYER_INDEX PlayerID, bool checkOnlineStatus); //if checkOnlineStatus and not in same title we'll assume spoof / not the actual friend.
bool IsStrValid(const char* str);
bool AddPlayerToKickStorage(PLAYER_INDEX PlayerID, uint *kick_array_index);
//int GetPlayerMachineId(PLAYER_INDEX playerIndex);
//int GetPlayerIpAddress(PLAYER_INDEX playerIndex);
void TeleportNetworkPlayerOnFoot(const PLAYER_INDEX playerIndex, float x, float y, float z, float h);
void SetHostByte(bool set); //works
bool IsDriveWaterExclusionArea(float x, float y, float z, float distance);
bool is_vehicle_horn_sounding(VEHICLE_INDEX vehicle);
bool CreateObject(uint model, float x, float y, float z, float h, OBJECT_INDEX* object, bool lockNetControl);
void DeleteObject(OBJECT_INDEX *object);
//PED_INDEX GetPlayerPed();
void change_player_model(int playerModel);
bool AreCoordinatesNearKillzone(float x, float y, float z);
char* VehiclesModelName(const VEHICLE_INDEX vehicle);
uint GetHandleFromPool(CPool* pool, uint Pointer_Address);
char* SafeNetworkPlayerGetNameWithColour(PLAYER_INDEX playerindex);
char* NetworkPlayerGetNameWithColour(PLAYER_INDEX playerindex);