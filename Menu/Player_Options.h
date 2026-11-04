#pragma once

typedef enum ctl_weapon_bits //32 bits in 1 value.
{
	unlimitedAmmo,
	unlimitedClip,
	forceAutoAimPlayers, 
	forceAutoAimPeds,  
	forceNeverTargettedPed,  
	forceNeverTargettedVehicle, //will probably end up being a loop (TODO:) so no need to set on loading as the bitset will be grabbed.
	gravityGun,
	gravityGunExplodeMode
}ctl_weapon_bits;

typedef enum ctl_player_bits //32 bits in 1 value.
{
	playerGodMode,
	freezeTime,
	freezeWeather,
	aPadNotifications,
	Invisibility,
	neverWanted,
	//forceAutoAimPlayers, //move to weapon options?
	superRun,
	driveWaterVisible,
	superJump,
	//unlimitedAmmo,
	flyMod,
	sharedIsSelectedPlayerMuted,
	playerRagdoll,
	createdSpliffObjPTFX, 
	loopAnims,
	drunkMode,
	//gravityGun,
	//gravityGunExplodeMode,
	playerNetworkAllOptsExcludeSelf,
	playerESP,
	SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED,
	playerESPDrawLines,
	playerESPDrawBoxes,
	networkRank10All,
	inChauffeurSubmenu,
	attachedToWalker, //used to check for y input / detaching / clearing tasks.
	printedNewDickheadIdentifiers, //print cap
	SECURITY_BIT_STORE_XUID_FUNCTION_CALLED,
	smoke_spliff_mode,
	syncTimeToRealWorld,
	//unlimitedClip,
	//will create and move these to ctlMem->m_bit_sets.m_network but we still have room atm so no point
}ctl_player_bits;

typedef struct ctl_player_spliff
{
	PTFX_ID obj_smoke;
	PTFX_ID veh_smoke;
	OBJECT_INDEX spliff_obj;
	int spliff_tick;
}ctl_player_spliff;

extern int wantedLvlInt;

void PlayerOptions();
void worldOptsToggleFlymod();
void setWantedLvl();
void stopPlayerAnim(bool loopAnims, int unused0);
void playerOptUpdateMultiplayerModel(int unused1, int unused2);
void PlayPedAnimations(char* animSet, char* animName);
void PlayerLoopedOptions();
void changePlayerModel(int playerModel, int unused0);
void playerWeaponOptions();
void disableFlymod();
bool GetClosestNetworkPlayerVehicleToCoordinate(float mX, float mY, float mZ, bool checkOnScreen, VEHICLE_INDEX *closest);
void playerOptChangeModel(); //for admin purposes

void playerOptAutoAimPlayers(int un1, int un2); //called in loading settings to do the rigging.
void playerOptAutoAimPeds(int un1, int un2); //called in loading settings to do the rigging.
void rigNeverWantedGlobal(int un1, int un2);