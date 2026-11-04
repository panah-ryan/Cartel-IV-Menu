#pragma once

#define IP_as_Int32(a, b, c, d)   ((d) | ((c) << 8) | ((b) << 16) | ((a) << 24))

typedef enum InviteVersion
{
	INVITE_NORMAL = 0, //from freemode anyway..
	INVITE_CARTEL = 1,
} InviteVersion;

/*
typedef struct playerIdentifiers 
{
	byte lan_ip_octets[4];     //player point + 0                   // IP address (zero if not static/DHCP)
	byte wan_ip_octets[4];     //player point + 4            // Online IP address (zero if not online)
	short wPortOnline;   //player point + 8             // Online port
	byte mac_addr[6];    //player point + 10? //0x16        // Ethernet MAC address
	byte abOnline[20];   //player point + 16?            // Online identification
} playerIdentifiers;
*/

typedef struct playerIdentifiers
{
	union
	{
		byte lan_ip_octets[4];                     // IP address (zero if not static/DHCP) //make sure to & 0xFF
		int lan_ip;
	};
	union
	{
		byte wan_ip_octets[4];              // Online IP address (zero if not online)
		int wan_ip;		//0x4
	};
	byte _2port_6mac_2pad[10]; //try change to _4pad[12];
	//invicible char _paddingz[2]; here i think
	//short wPortOnline; //this is registering as an int and sc-cl is stopping me getting the first 2 bytes of mac addres.    // Online port
	//byte mac_addr[6];			// Ethernet MAC address

	char _padding[4];			//what the fuck could be stored here?		
	int machineId[2];	//[0] = 0x18, [1] = 0x1C
	char _padding2[32];
	int xuid[2];		//[0] = 0x40, [1] = 0x44
	char _padding3[4];
	char gamertag[16];	//0x4C
} playerIdentifiers;

/*

typedef struct playerIdentifiers 
{
	union
	{
		byte lan_ip_octets[4];                     // IP address (zero if not static/DHCP) //make sure to & 0xFF
		int lan_ip;
	};
	union
	{
		byte wan_ip_octets[4];              // Online IP address (zero if not online)
		int wan_ip;
	};
	short wPortOnline; //this is registering as an int and sc-cl is stopping me getting the first 2 bytes of mac addres.    // Online port
	//char _pad[2]; //if we're able to get the above to become an actual short.
	byte mac_addr[6];			// Ethernet MAC address
	char _padding[4];			//what the fuck could be stored here?		
	int machineId[2];  
	char _padding2[32]; 
	int xuid[2]; 
	char _padding3[4];
	char gamertag[16];
} playerIdentifiers;
*/

extern int gameModeSelect;
extern int episodeSelect;
extern int inviteStringSelect;

extern int netPlayerSpectating;

extern int networkHostArmourRegenSeconds; //could easily be in a byte set / a byte itself
extern int networkHostHealthRegenSeconds; //could easily be in a byte set / a byte itself
extern int networkHostWeaponRegenSeconds; //could easily be in a byte set / a byte itself

extern VEHICLE_INDEX attachVeh;

const char* GetGamemodeString(int Gamemode);
void UploadScores();
void DownloadScores();
void VoteKick(PLAYER_INDEX kickTarget);
void SuggestGamemode(int gamemode);
void KickAllPlayers();
void DickheadScannerLoop();
bool getCustomSpawnPointCoords(vector4 *spawn);
void setCustomSpawnPoint(int unused1, int unused2);
void teleportCustomSpawnpoint(int unused1, int unused2);
//void read_bits_from_online_scores(int unused1, int unused2);
void save_bits_to_online_scores(int unused1, int unused2);
bool DoesNetworkIdExist(NETWORK_INDEX netId);
bool RequestNetworkControl(const NETWORK_INDEX netId, char* entity_name);
bool ReleaseControlOfNetworkId(NETWORK_INDEX netId);
PLAYER_INDEX FindControllerOfNetworkId(const NETWORK_INDEX Network_ID);
const char* SafeNetworkPlayerGetName(PLAYER_INDEX playerIndex);
char* SafeNetworkPlayerGetNameWithColour(PLAYER_INDEX playerindex);
char* NetworkPlayerGetNameWithColour(PLAYER_INDEX playerindex);
bool SafeToActOnPlayerPed(PLAYER_INDEX player, PED_INDEX* ped);
const char* getName(PLAYER_INDEX playerIndex);
uint GetPlayerPointerNath(PLAYER_INDEX playerIndex); //Supports retail
bool ScanNetworkObjectPoolForClosestModelToPlayer(PLAYER_INDEX PlayerID, uint searchModel, bool birdsEye, bool searchNonMigratableOnly, float maxDistanceFromPlayer, bool onlyObjectsWithinWalkWaterObjectHeightRange, PLAYER_INDEX *Controller, OBJECT_INDEX *scanobj, float searchObjHealth);
int GetAutoAimPreference(); //need to just get rid of these and make enum lobby preferences
int GetPolicePreference();
int GetFriendlyFirePreference();
int maxSlotsForGameMode(eGameMode gameMode);
void DoLeaveGameCleanup();
void NetworkGameRejoinSession(); //used in Admin.c
void NetworkChangeGameMode();
bool IsPlayerUsingXboxOne(PLAYER_INDEX playerIndex);
void NetworkRunSearch(int param);
void NetworkJoinGame(int param, int unused1);
char* GetGamemodeScriptName(int gameMode);
void printGameInfo(int param, int unused1);

bool HasStoredXUIDAndGamertagInFreeMem();
void StoreXUIDAndGamertagInFreeMem();
void SpoofGamertag(const char* tag);
void RestoreXUIDAndGamertagInFreeMem();
void SpoofXUID(char* xuidString);
void NetworkOptSpoofXUIDTextInput();
void NetworkOptsSpoofGTandXUIDTextInput();
void NetworkOptSpoofXuidTextInput();
void NetworkOptsSpoofGamertagTextInput();
void NetworkOptSpoofRecentPlayer(int metPlayerIndex, int unused1);
void NetworkOptOpenRecentPlayerProfile(int metPlayerIndex, int unused1);
void NetworkOptOpenRecentPlayerFeedback(int metPlayerIndex, int unused1);

void NetworkGameOptFindNewGame(int unused1, int unused2);
void NetworkGameOptSingleplayer(int unused1, int unused2);

int GetNumberOfKickedConsoles();
uint GetByteSetH1(uint ByteSet, int byteIndex); //is this even used

//int GetPlayerIpAddress(PLAYER_INDEX playerIndex);
int GetPlayerXuid(PLAYER_INDEX playerIndex); //gets unique half (upper) 
int GetPlayerMachineId(PLAYER_INDEX playerIndex); //gets unique half (lower)

int PlayerIsADickhead(PLAYER_INDEX playerId, char* *name);
int GetPlayerMacAddress(PLAYER_INDEX playerIndex); //untested + probably misses 2 bytes off of the end.
void logPlayer(PLAYER_INDEX playerId);
bool AddPlayerToKickStorage(PLAYER_INDEX PlayerID, uint *kicked_array_index);
void NetworkOptUnkickConsole(int index, int unused1);
void NetworkOptKickedConsoleGiveReason(int index, int unused1);
void NetworkOptKickedConsolePrintReason(int index, int unused1);
void NetworkPrintNoConsolesKicked(int unused1, int unused2);
void NetworkGamePrintNotHost(int unused1, int unused2);
void NetworkGameHostKickAll(int unused1, int unused2);

void netHostOptSetArmourRegen(int unused1, int unused2);
void netHostOptSetHealthRegen(int unused1, int unused2);
void netHostOptSetWeaponRegen(int unused1, int unused2);
int GetMoneyAmountForRank(int Rank);
void toggle360kicker(int unused1, int unused2);
void toggle360KickerAllowCurrent(int un1, int un2);
void generateLobby360JoinPin(int unused1, int unused2);
void setLobby360JoinPin(int unused1, int unused2);
void printLobby360JoinPin(int unused1, int unused2);
void printJoinPinInfo(int unused1, int unused2);
void setJoinPin(int unused1, int unused2);

void NetworkOptClearAllRecentJoins(int un1, int un2);
void NetworkOptWhitelistRecentJoin(int index, int unused1);
void NetworkOptClearRecentJoin(int index, int unused1); //can use in join logic to empty them when they join successfully.

int GetNumberOfRecentlyKickedPlayers();
bool IsPlayerInRecentListWhitelisted(PLAYER_INDEX playerId, int* recent_kicked_arr_index);

bool AddPlayerToRecentlyKickedStorage(PLAYER_INDEX PlayerID); //only local as doesn't blacklist kick by default.

void NetGameInviteFriend(int friendIndex, int unused0);
void NetGameFriendOpenProfile(int friendIndex, int unused0);
void NetworkInviteFriendCustom(int unused0, int unused1);

void ToggleEspLines(int unused1, int unused2);
void ToggleEspBoxes(int unused1, int unused2);
void toggleRank10All(int unused1, int unused2);
void toggleNetworkAllPlayersExcludeSelf(int unused1, int unused2);
void UpdateSharedCMDAllowList(int unused1, int unused2);
void setMenuAdvertise();

void SharedMenuControlReceiveLoop();
void SendSharedMenuCommandToPlayer(int SharedMenuCommandID, int unused1);

//Crash stuff
void TeleportAwayFromDanger(float mx, float my, float mz);
void networkBasicCrashFreezeGun(int unused1, int unused2);
void networkBasicCrashGroupHack(int unused1, int unused2);
void networkBasicCrashSuperlodNormal(int unused1, int unused2);
void networkBasicCrashSuperlodTryTeleport(int unused1, int unused2);

void GetCartelSpawnCoordinatesForPlayer(PLAYER_INDEX PlayerID, float *x, float *y, float *z, float *h);
bool IsDriveWaterExclusionArea(float x, float y, float z, float distance);
void TeleportToCartelSpawnForPlayerID(PLAYER_INDEX PlayerID);
void CartelSpawnPointInteriorObjectLoop();
void CartelSpawnPointLoop();
void detachLocalPlayerFromVehicleOrWalker();

void netPlayerSpectate(PLAYER_INDEX player, int unused); 
void netPlayerCloneVeh(int unused1, int unused2);
bool Is_Vehicles_Type_Modified(uint vehicleModel);