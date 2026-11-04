#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"
#include "../Menu/Functions.h"
#include "protections.h"


CPlayerInfo* GetPlayerPointer(PLAYER_INDEX playerIndex)
{
	CPlayerInfo** playerArray = (CPlayerInfo**)0x82C01C70;
	return playerArray[playerIndex];
}

bool IsPlayerUsingXboxOne(PLAYER_INDEX playerIndex) //works
{
	return GetPlayerPointer(playerIndex)->m_machineId[1] != 250;
}

//INFO: Make sure you check IsPlayerValid and PLAYER_HAS_CHAR before calling this.
bool IsPlayerInvincible_ctlHelper(PLAYER_INDEX playerIndex, PED_INDEX playerPed)
{
	if (IS_PLAYER_DEAD(playerIndex) || !IS_PLAYER_CONTROL_ON(playerIndex)) //Dead/Joining players have temporary godmode. (Don't want to log them to the modder_db.dat file, so we'll return false).
		return false; //players are invincible while respawning
	
	if (myCurrentEpisode == EPISODE_TBOGT) //not ideal but we will just have to skip if people are wearing parachute - TEST REQUIRED: 20th January 2026 //seems to work.
	{
		if (GET_CHAR_DRAWABLE_VARIATION(playerPed, PED_COMPONENT_SUS2) == 1) //parachute is worn - parachutes trigger godmode and will cause false flags to modder_db.dat unless we skip them.
			return false;
	}
	return GetPlayerPointer(playerIndex)->m_playerPed->m_flags & 0xFF0000;
}

PLAYER_INDEX IsThisPedAPlayer(PED_INDEX ped)
{
	PED_INDEX playerPed;
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i))// || !PLAYER_HAS_CHAR(i))
			continue; //causing freezing without this

		GET_PLAYER_CHAR(i, &playerPed);
		if (playerPed == ped)
			return i;
	}
	return INVALID_PLAYER_INDEX;
}

bool CreateObject(uint model, float x, float y, float z, float h, OBJECT_INDEX* object, bool lockNetControl) //could function pointer this to use Cartels func if that shit works
{
	if (IS_MODEL_IN_CDIMAGE(model))
	{
		REQUEST_MODEL(model);
		while (!HAS_MODEL_LOADED(model))
			WAIT(0);

		CREATE_OBJECT_NO_OFFSET(model, x, y, z, &*object, true);
		MARK_MODEL_AS_NO_LONGER_NEEDED(model);

		while (!DOES_OBJECT_EXIST(*object))
			WAIT(0);

		FREEZE_OBJECT_POSITION(*object, true);
		SET_OBJECT_HEADING(*object, h);
		SET_OBJECT_INVINCIBLE(*object, true);

		//SET_OBJECT_PROOFS(*object, true, true, true, true, true); //see if stops glass breaking..

		SET_OBJECT_HEALTH(*object, 165.0f); //stops detaching by my protection if local player has control.

		if (NetworkGame && lockNetControl)
		{
			NETWORK_INDEX netId;

			GET_NETWORK_ID_FROM_OBJECT(*object, &netId);
			SET_NETWORK_ID_CAN_MIGRATE(netId, false);
		}

		return true;
	}

	printf("~r~CTL: ~w~Model is missing from your image files.");
	return false;
}

bool IsPlayerOnFriendsList(PLAYER_INDEX PlayerID, bool checkOnlineStatus) 
{
	//const char* playerName = IS_BIT_SET(ctlMem->m_bit_sets.m_loggedPlayer_UINUSED, (PlayerID + 16)) ? ctlMem->m_char_buffers.player_name_backup[PlayerID] : GET_PLAYER_NAME(PlayerID);
	const char* playerName = ctlMem->m_char_buffers.player_name_backup[PlayerID][0] != '\0' ? ctlMem->m_char_buffers.player_name_backup[PlayerID] : GET_PLAYER_NAME(PlayerID);
	int friendCount = NETWORK_GET_FRIEND_COUNT();
	for (int i = 0; i < friendCount; i++)
	{
		if (COMPARE_STRING(NETWORK_GET_FRIEND_NAME(i), playerName))
			return (checkOnlineStatus ? (NETWORK_IS_FRIEND_ONLINE(playerName) && NETWORK_IS_FRIEND_IN_SAME_TITLE(playerName)) : true);
			//return (checkOnlineStatus ? (NETWORK_IS_FRIEND_ONLINE(NETWORK_GET_FRIEND_NAME(i)) && NETWORK_IS_FRIEND_IN_SAME_TITLE(NETWORK_GET_FRIEND_NAME(i))) : true);
	}
	return false;
}

/*
PED_INDEX GetPlayerPed() 
{
	PED_INDEX pPed; 
	GET_PLAYER_CHAR(myPlayerIndex, &pPed);
	return pPed;
}
*/

/*
VEHICLE_INDEX GetPlayerVeh() //GOING TO REMOVE //auto-boat is causing handle to change mid-loop and null-pointer crash, we might have to use this.
{
	VEHICLE_INDEX myVeh = INVALID_HANDLE;

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);

	return myVeh;
}
*/

uint GetHandleFromPool(CPool* pool, uint Pointer_Address)
{
	uint index = (Pointer_Address - pool->m_firstPointer) / pool->m_structSize;
	return (index * 0x100) + *(byte*)(pool->m_bools + index);
}

char* VehiclesModelName(const VEHICLE_INDEX vehicle)
{
	uint model;

	GET_CAR_MODEL(vehicle, &model);
	return (char*)GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));
}

bool AreCoordinatesNearKillzone(float x, float y, float z)
{
	return (x > 3560.0f || x < -3240.0f || y > 3400.0f || y < -3400.0f || z > 4050.0f || z < -130.0f); //-130 z not killzone but useless coords so
}

bool DoesNetworkIdExist(NETWORK_INDEX netId)
{
	if (!NetworkGame)
		return false;

	return DOES_VEHICLE_EXIST_WITH_NETWORK_ID(netId) || DOES_OBJECT_EXIST_WITH_NETWORK_ID(netId) || DOES_PED_EXIST_WITH_NETWORK_ID(netId);
}

bool ReleaseControlOfNetworkId(NETWORK_INDEX netId)
{
	if (!DoesNetworkIdExist(netId))
		return false;

	bool has_control = HAS_CONTROL_OF_NETWORK_ID(netId);
	if (has_control)
		SET_NETWORK_ID_CAN_MIGRATE(netId, true);

	return has_control;
}

void DeleteObject(OBJECT_INDEX *object)
{
	NETWORK_INDEX NetworkId;
	GET_NETWORK_ID_FROM_OBJECT(*object, &NetworkId);
	ReleaseControlOfNetworkId(NetworkId);
	DELETE_OBJECT(object);
}

bool is_vehicle_horn_sounding(VEHICLE_INDEX vehicle)
{
	if (!DOES_VEHICLE_EXIST(vehicle))
		return false;

	pool_s* pPool = (pool_s*)*(int*)0x82FB0700;
	vehicle_s* point = (vehicle_s*)((pPool->size * (vehicle >> 8)) + (int)pPool->pool);

	if (point == NULL)
		return false;

	return point->horn != 0;
}

bool IsDriveWaterExclusionArea(float x, float y, float z, float distance)
{
	//if walk on air bit is set... maybe do something here..
	//need to map each island on 2D axis in order to disable for train tunnels etc (i know they are present on middle island).
	//if (z < -10.0f) //player is below water height of 0/in tunnel OR above height of 45/likely in a heli..
	if (z < -2.5f) //player is below water height of 0/in tunnel OR above height of 45/likely in a heli..
		return true;

	if (z > 45.0f)
		return true;

	//if middle island and below -2.0f (for train tunnels).
	//Train tunnel entrance from main bridge
	//corner 1
	//X -15.7511
	//Y 342.3614
	//Z -3.00
	//Corner 2
	//X  200.0f
	//Y 192.7412
	//Z 30.00
	if (x > -15.7511 && x < 200.0f && y > 192.7412 && y < 342.3614 && z > -2.0f && z < 30.0f) return true;

	//main bridge train tunnels / just gonna do entire island..
	//should also exclude is player is too high up or in a heli etc..
	//BOTU / Bohan Tunnel //entrance with HEli.
	//CORNER 1 (sky)
	//X -811.9304
	//Y 766.0514
	//Z -24.0f

	//CORNER 2
	//X -1020
	//Y 792.0f
	//Z 40.0f (height of building near road).

	//BOHAN TUNNEL ENTRANCE (With Heli) - Road facing it for speed/clearance aswell. //this mod is drive/walk on water, not drive on air/under the map.
	//if (x < -811.9304 && x > -1020.0f && y > 766.0514f && y < 792.0f && z > -24.0f && z < 40.0f) 
	if (x < -811.9304 && x > -1020.0f && y > 766.0514f && y < 791.0f && z > -24.0f && z < 40.0f)
		return true;

	//Bohan Tunnel Exit / Entrance 2 (other side
	//CORNER 1 (on road)
	//X -347.0f
	//Y 345.0f
	//Z 25.0f  //third floor building height straight out of tunnel.

	//CORNER 2 (underground / partly in tunnel)
	//X -620.0f 
	//Y 310.0f
	//Z -21.0f
	//BOHAN TUNNEL EXIT/ENTRANCE 2 (Without Heli/Helipad)
	if (x > -620.0f && x < -347.0f && y > 310.0f && y < 345.0f && z > -21.0f && z < 25.0f)
		return true;


	//other tunnel //near bus terminals / 3 helipads.
	//BOX/ZONE 1
	//X -195.50f
	//Y -890.0f
	//Z -10.0f

	//X 180.0f
	//Y -690?
	//Z 20.0f
	//NEAR BUS STATION/3 HELI-PADS TUNNEL ZONE/BOX 1
	if (x > -195.50f && x < 180.0f && y > -890.0f && y < -690.0f && z > -10.0f && z < 20.0f)
		return true;


	//BOX / ZONE 2
	//X -194.0f
	//Y -870.0f
	//Z -10.0f

	//X -240.0f
	//Y -620.0f //change to -680.0f if issues.
	//Z 20.0f
	//NEAR BUS STATION/3 HELI-PADS TUNNEL ZONE/BOX 2
	if (x > -240.0f && x < -194.0f && y > -870.0f && y < -620.0f && z > -10.0f && z < 20.0f)
		return true;


	//Airport Tunnel (Doesn't block completely but you float in the air a bit).
	//X 2215.0f 
	//Y 440.0f
	//Z 35.0f

	//Corner 2
	//X 1980
	//250.0f
	//Z -5.0f

	//Airport Tunnel (Doesn't block completely but you float in the air a bit).

	if (x < 2215.0f && x > 1980.0f && y < 440.0f && y > 250.0f && z > -5.0f && z < 35.0f)
		return true;


	float bs;
	if (distance < 70.0f)
		return !GET_WATER_HEIGHT(x, y, (z - 4.0f), &bs); //COMMENTED TO TEST EXCLUSION AREAS
			//return true;

	return false;
	/*SET_OBJECT_COLLISION(driveWalkWaterObj[i], (distance < 70.0f) ? GET_WATER_HEIGHT(pX, pY, (pZ - 4.0f), &oy) : true);*/
}


void SetHostByte(bool set) //works
{
	if (!NetworkGame)
		return;

	*(byte*)0x830FB0EB = set;
}

void TeleportNetworkPlayerOnFoot(const PLAYER_INDEX playerIndex, float x, float y, float z, float h) //could function pointer this
{
	bool grabbed_their_armour_weapons;
	int playerArmour, playerCurrentWeapon, playerCurrentAmmo;
	PED_INDEX playerPed;
	WEAPON_TYPE playerWeapons[10], playerAmmos[10]; //get all weapons and ammo amounts.
	if (IsPlayerValid(playerIndex) && PLAYER_HAS_CHAR(playerIndex))
	{
		grabbed_their_armour_weapons = true;
		GET_PLAYER_CHAR(playerIndex, &playerPed);
		GET_CHAR_ARMOUR(playerPed, &playerArmour);
		GET_CURRENT_CHAR_WEAPON(playerPed, &playerCurrentWeapon);
		GET_AMMO_IN_CHAR_WEAPON(playerPed, playerCurrentWeapon, &playerCurrentAmmo);
		//weapon indexes are 0 to 36
		for (int i = 0; i < 10; i++) //8 is confirmed to work.
		{
			playerWeapons[i] = -1;
			playerAmmos[i] = -1;
		}
		for (int i = 0; i < 36; i++)
		{
			if (i != WEAPON_ROCKET && i != WEAPON_EPISODIC_18)
			{
				if (HAS_CHAR_GOT_WEAPON(playerPed, i) && i != playerCurrentWeapon)
				{
					for (int j = 0; j < 10; j++)
					{
						if (playerWeapons[j] == -1) //space available here
						{
							playerWeapons[j] = i;
							GET_AMMO_IN_CHAR_WEAPON(playerPed, i, &playerAmmos[j]);
							break;
						}
					}
				}
			}
		}
	}
	SetHostByte(true);
	RESURRECT_NETWORK_PLAYER(playerIndex, x, y, z - 1.0f, h); //z - 1.0 as it sets +1.0
	SetHostByte(GET_HOST_ID() == myPlayerIndex);
	if (playerIndex == myPlayerIndex) //we don't needa run detection on ourselves.
	{
		if (grabbed_their_armour_weapons)
		{
			if (playerArmour > 0)
			{
				GIVE_WEAPON_TO_CHAR(playerPed, WEAPON_ARMOUR, 1, false);
				ADD_ARMOUR_TO_CHAR(playerPed, playerArmour); //restore their armour.
			}
			for (int i = 0; i < 10; i++)
			{
				if (playerWeapons[i] != -1)
					GIVE_WEAPON_TO_CHAR(playerPed, playerWeapons[i], playerAmmos[i], false);
			}
			GIVE_WEAPON_TO_CHAR(playerPed, playerCurrentWeapon, playerCurrentAmmo, true);
		}
	}
}

int indexOfAfterIndex(const char* str, char c, int startIndex, int size)
{
	for (int i = (startIndex + 1); i < size; i++)
	{
		if (str[i] == c)
			return i;
	}

	return -1;
}

bool IsStrValid(const char* str)
{
	int len = GET_LENGTH_OF_LITERAL_STRING(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '~')
		{
			int index = indexOfAfterIndex(str, '~', i, len); //Find the next tilde after the current one
			if (index == -1)
				return false; //No matching closing '~' found, invalid string

			//Extract the string between the two tilde symbols
			int tagLength = index - i - 1;  // Length of the tag (excluding the '~'s)
			if (tagLength == 0)
				return false; // Reject case like "~" with no tag between the '~'s

			i = index; //Move the index to the position after the valid tag
		}
	}
	return true;
}

const char* SafeNetworkPlayerGetName(PLAYER_INDEX playerIndex)
{
	const char* playerName = GET_PLAYER_NAME(playerIndex);
	if (IsStrValid(playerName))
		return playerName;

	return "Invalid name";
}

char* SafeNetworkPlayerGetNameWithColour(PLAYER_INDEX playerindex)
{
	char* str = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	sprintf(str, "~COL_NET_%i~%s\0", (playerindex + 1), SafeNetworkPlayerGetName(playerindex));
	return (char*)str;
}

PLAYER_INDEX FindControllerOfNetworkId(const NETWORK_INDEX Network_ID)
{
	if (DoesNetworkIdExist(Network_ID))
	{
		for (int i = 0; i < 16; i++)
		{
			if (IsPlayerValid(i)) //i keep crashing on DOES_PLAYER_HAVE.... for some reason. so why not add this..
			{
				if (DOES_PLAYER_HAVE_CONTROL_OF_NETWORK_ID(i, Network_ID))
					return i;
			}
		}
	}
	return INVALID_PLAYER_INDEX;
}

char* NetworkPlayerGetNameWithColour(PLAYER_INDEX playerindex)
{
	char* str = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	sprintf(str, "~COL_NET_%i~%s\0", (playerindex + 1), GET_PLAYER_NAME(playerindex));
	return (char*)str;
}

bool netObject_IsPendingOwnerChange(uint net_object)
{
	return getByte((void*)(net_object + 0x10)) != 0xFF;
}

uint CNetGamePlayer_GetLocalPlayer(uint net_interface)
{
	if ((getByte((void*)(net_interface + 0x1F40)) & 0x40) == 0)
		return NULL;

	return netObject_IsPendingOwnerChange(net_interface + 0x40) ? net_interface + 0x40 : NULL;
}

uint CNetGamePlayer_GetNetPlayer(uint net_interface, PLAYER_INDEX playerIndex)
{
	//if (playerIndex != -1 && (getByte((void*)(net_interface + 0x1F40)) & 0x40) != 0 && playerIndex >= 0 && playerIndex < 16) //we won't pass invalid indexes in .sco
	if ((getByte((void*)(net_interface + 0x1F40)) & 0x40) != 0)
	{
		uint local_player = CNetGamePlayer_GetLocalPlayer(net_interface);
		if (local_player == NULL || getByte((void*)(local_player + 0x10)) != playerIndex)
			return *(uint*)(((playerIndex + 0x16B) << 2) + net_interface);
		return local_player;
	}

	return NULL;
}

bool IsPlayerValid(PLAYER_INDEX playerIndex)
{
	if (playerIndex == myPlayerIndex)
		return true;

	uint net_player = CNetGamePlayer_GetNetPlayer(0x83109C88, playerIndex);
	return net_player && netObject_IsPendingOwnerChange(net_player);
}

//could add more storage really, add extra IP space incase they hop VPN's etc
bool AddPlayerToKickStorage(PLAYER_INDEX PlayerID, uint *kick_array_index) 
{
	CPlayerInfo* player = GetPlayerPointer(PlayerID); //Use this in normal build once security is laxed
	if (!IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, PlayerID)) //prevent admin console kick
	{
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++) //sometimes shit catches twice, we will just check the IP and MID is not already present.
		{
			//Should we bother to check mac address aswell here? Or perhaps we should only check MachineID to grab new ids if they change them..
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID == player->m_machineId[0] && freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress == player->m_wan_ip) //this machineID and IP address combo is already stored. --- in which case they will be detected immediately and kicked.
			{
				//Duplicate dicklist kick calls should end up here, so we will clear XUID to be safe, we already have them stored in this instance.
				//xcoldmurda 2535436885809466
				//should probably randomise these but eh.
				player->m_xuid[0] = 25354368;
				player->m_xuid[1] = 85809466; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted. (we are about to kick them after-all)
				return false;
			}
		}
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
		{
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID != 0)
				continue; //somethings stored here already.

			memcpy(freeMem->m_cartelHostVariables.m_kicked_storage[i].gamertag, player->m_gamertag, 16);
			strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[i].kickedReason, "Not provided", 26); //nuke kicked reason
			freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID = player->m_machineId[0]; //upper half is just nonsense for 360 0x000000FA always
			freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress = player->m_wan_ip;
			freeMem->m_cartelHostVariables.m_kicked_storage[i].failsCounter = 0;
			for (int j = 0; j < 6; j++)
				freeMem->m_cartelHostVariables.m_kicked_storage[i].MAC[j] = player->_2port_6mac_2pad[j + 2];

			//000901F36C35753C - N1 B58 2535419794519356 (cracker bumboi)
			player->m_xuid[0] = 25354197;
			player->m_xuid[1] = 94519356; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted. (Spoofing exploit prevention)
			*kick_array_index = i;
			return true;
		}
	}
	return false; //revert back to normal XUID kicking (usually called right after), kicked data array is full.
}

/*Backup
bool AddPlayerToKickStorage(PLAYER_INDEX PlayerID, uint *kick_array_index)
{
	if (!IS_BIT_SET(ctlMem->m_bit_sets.m_CartelAdmin_CartelModerator, PlayerID)) //prevent admin console kick
	{
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++) //sometimes shit catches twice, we will just check the IP and MID is not already present.
		{
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID == GetPlayerMachineId(PlayerID) && freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress == GetPlayerIpAddress(PlayerID))
			{ //this machineID and IP address combo is already stored.
				*(int*)(GetPlayerPointerNath(PlayerID) + 0x40) = 0;
				*(int*)(GetPlayerPointerNath(PlayerID) + 0x44) = 0; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted.
				return false;
			}
		}
		for (int i = 0; i < MAX_KICKED_CONSOLES; i++)
		{
			if (freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID != 0)
				continue; //somethings stored here already.

			memcpy(freeMem->m_cartelHostVariables.m_kicked_storage[i].gamertag, (char*)GET_PLAYER_NAME(PlayerID), 16);
			strcpy(freeMem->m_cartelHostVariables.m_kicked_storage[i].kickedReason, "Not provided", 26); //nuke kicked reason
			freeMem->m_cartelHostVariables.m_kicked_storage[i].MachineID = GetPlayerMachineId(PlayerID);
			freeMem->m_cartelHostVariables.m_kicked_storage[i].IPAddress = GetPlayerIpAddress(PlayerID);
			freeMem->m_cartelHostVariables.m_kicked_storage[i].failsCounter = 0;

			//could even use a byte set for the variation bits. prop indexs, texture indexs, variation? then could do male or female mp model as last byte.
			//freeMem->m_cartelHostVariables.m_kicked_storage[i].PedVariationBits = GetPlayerPedVariationAsBits(PlayerID); //return 0 if not unique, make sure model is M_Y_MULTIPLAYER or F_Y_MULTIPLAYER when scanning.
			//freeMem->m_cartelHostVariables.m_kicked_storage[i].BeenInvincibleWhileNotDead = ... //would need timer for each player as game makes you invincible while loading in / respawning etc.
			//and compare wih other players before saving to make sure it's unique.

			*(int*)(GetPlayerPointerNath(PlayerID) + 0x40) = 0;
			*(int*)(GetPlayerPointerNath(PlayerID) + 0x44) = 0; //Set players XUID to 0 so the XUID they are using doesn't get blacklisted.
			*kick_array_index = i;

			return true;
		}
	}

	return false;
}
*/

void change_player_model(int playerModel) //best to not function ptr this as cartels has admin code we don't want to execute here.
{

	if (!IS_MODEL_IN_CDIMAGE(playerModel))
	{
		printf("~r~CTL: ~w~Model 0x%x is missing from your ISO!", playerModel);
		return;
	}

	if(!HAS_MODEL_LOADED(playerModel))
		REQUEST_MODEL(playerModel);

	float heading;
	WEAPON_TYPE savedWeapons[WEAPON_SLOT_THROWN + 1];
	int savedAmmo[WEAPON_SLOT_THROWN + 1];
	uint roomKey;
	int seatIndex = 0;
	WEAPON_TYPE currentWeapon;
	VEHICLE_INDEX playerVehicle = 0;
	int i, armor, health, currentWeaponAmmo;
	uint ammo1;

	GET_CHAR_ARMOUR(myPlayerPed, &armor);
	GET_CHAR_HEALTH(myPlayerPed, &health);

	for (i = WEAPON_SLOT_UNARMED; i <= WEAPON_SLOT_THROWN; i++)
		GET_CHAR_WEAPON_IN_SLOT(myPlayerPed, i, &savedWeapons[i], &savedAmmo[i], &ammo1);

	GET_CURRENT_CHAR_WEAPON(myPlayerPed, &currentWeapon);
	GET_AMMO_IN_CLIP(myPlayerPed, currentWeapon, &currentWeaponAmmo);

	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		PED_INDEX pedInSeat = 0;

		GET_CAR_CHAR_IS_USING(myPlayerPed, &playerVehicle);
		GET_DRIVER_OF_CAR(playerVehicle, &pedInSeat);

		if (pedInSeat == myPlayerPed)
			seatIndex = -1;
		else
		{
			for (i = 0; i < 3; i++)
			{
				GET_CHAR_IN_CAR_PASSENGER_SEAT(playerVehicle, i, &pedInSeat);

				if (myPlayerPed == pedInSeat)
				{
					seatIndex = i;
					break;
				}
			}
		}
	}
	/*
	//TODO NATH move to ctlMem so we can do this shit.
	if (DOES_OBJECT_EXIST(attached_object))
	{
		NETWORK_INDEX objNetId;

		GET_NETWORK_ID_FROM_OBJECT(attached_object, &objNetId);
		if (HAS_CONTROL_OF_NETWORK_ID(objNetId))
			DELETE_OBJECT(&attached_object);
	}
	*/
	while (!HAS_MODEL_LOADED(playerModel))
		WAIT(0);

	GET_KEY_FOR_CHAR_IN_ROOM(myPlayerPed, &roomKey);
	//GET_INTERIOR_FROM_CHAR(myPlayerPed, &myInterior);
	GET_CHAR_HEADING(myPlayerPed, &heading);
	CHANGE_PLAYER_MODEL(myPlayerIndex, playerModel);
	GET_PLAYER_CHAR(myPlayerIndex, &myPlayerPed); //REQUIRED!! - We just changed ped so our handle has changed, re-grab to avoid nullptr-ing ourselves and crashing :D
	SET_CHAR_INVINCIBLE(myPlayerPed, bitTest(ctlMem->m_bit_sets.m_player, playerGodMode)); //enables keeping of current setting
	SET_CHAR_HEADING(myPlayerPed, heading);
	ADD_ARMOUR_TO_CHAR(myPlayerPed, armor);
	SET_CHAR_HEALTH(myPlayerPed, health);

	//Weapons
	for (i = WEAPON_SLOT_UNARMED; i <= WEAPON_SLOT_THROWN; i++)
	{
		if (savedWeapons[i] != currentWeapon)
			GIVE_WEAPON_TO_CHAR(myPlayerPed, savedWeapons[i], savedAmmo[i], false);
	}

	if (currentWeapon)
	{
		int ammo;

		GET_AMMO_IN_CHAR_WEAPON(myPlayerPed, currentWeapon, &ammo);
		REMOVE_WEAPON_FROM_CHAR(myPlayerPed, currentWeapon);
		GIVE_WEAPON_TO_CHAR(myPlayerPed, currentWeapon, ammo, false);
		SET_CURRENT_CHAR_WEAPON(myPlayerPed, currentWeapon, true);
		SET_AMMO_IN_CLIP(myPlayerPed, currentWeapon, currentWeaponAmmo);
		SET_CHAR_AMMO(myPlayerPed, currentWeapon, ammo);
	}

	//Vehicle
	if (DOES_VEHICLE_EXIST(playerVehicle))
	{
		if (seatIndex == -1)
			WARP_CHAR_INTO_CAR(myPlayerPed, playerVehicle);
		else
			WARP_CHAR_INTO_CAR_AS_PASSENGER(myPlayerPed, playerVehicle, seatIndex);
	}

	if (IS_INTERIOR_SCENE())
		SET_ROOM_FOR_CHAR_BY_KEY(myPlayerPed, roomKey); //must be after warping as it clears your room key.

	FORCE_FULL_VOICE(myPlayerPed);

	if (playerModel == GET_PLAYERSETTINGS_MODEL_CHOICE())
		SET_PLAYERSETTINGS_MODEL_VARIATIONS_CHOICE(myPlayerIndex);
	else
		SET_CHAR_RANDOM_COMPONENT_VARIATION(myPlayerPed);

	if (playerModel != PED_M_Y_GMAF_HI_02 && playerModel != last_player_model) //aka running on tick delay.
		MARK_MODEL_AS_NO_LONGER_NEEDED(playerModel);
}