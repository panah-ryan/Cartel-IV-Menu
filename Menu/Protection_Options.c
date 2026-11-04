#include "stdafx.h"
#include "Utils.h"
#include "Functions.h"
#include "MenuUiHandling.h"
#include "MenuExecutionHandling.h"
#include "Protection_Options.h"


void cartelProtectionEventOptions(int unused1, int unused2)
{
	SetHeader("Protections -> Events");
	int *ctlEventBitSet = CTL_GET_EVENT_BITSET();
	AddItemBool("Resurrect Player", ctlEventBitSet, RESURRECT_PLAYER_EVENT, nullptr, "Allow teleport on foot.");
	AddItemBool("Ragdoll Request", ctlEventBitSet, RAGDOLL_REQUEST_EVENT, nullptr, "Allow others to ragdoll you.");
	AddItemBool("Give Weapon", ctlEventBitSet, GIVE_WEAPON_EVENT, nullptr, "Allow others to give you weapons.");
	AddItemBool("Remove Weapon", ctlEventBitSet, REMOVE_WEAPON_EVENT, nullptr, "Allow others to remove a weapon.");
	AddItemBool("Remove all Weapons", ctlEventBitSet, REMOVE_ALL_WEAPONS_EVENT, nullptr, "Allow others to remove all your weapons.");
	AddItemBool("Door Break", ctlEventBitSet, DOOR_BREAK_EVENT, nullptr, "Allow others to break your vehicle doors.");
	AddItemBool("Start Explosion", ctlEventBitSet, START_EXPLOSION_EVENT, nullptr, "Render explosions created by others.");
	AddItemBool("Start Projectile", ctlEventBitSet, START_PROJECTILE_EVENT, nullptr, "Render projectiles created by others.");
	AddItemBool("Alter Wanted level", ctlEventBitSet, ALTER_WANTED_LEVEL_EVENT, nullptr, "Allow others to alter your wanted level.");
}

void cartelProtectionSyncOptions(int unused1, int unused2)
{
	SetHeader("Protections -> Cloning");
	int *ctlCloneBitSet = CTL_GET_CLONE_BITSET();
	for (int i = 0; i < 10; i++)
		AddItemBool(ctlMem->m_networkObjTypeNames[i], ctlCloneBitSet, i, nullptr, "Clone/Spawn network entities of this category.");
	/*
	AddItemBool("Player", ctlCloneBitSet, NET_OBJ_TYPE_PLAYER, nullptr, "Clone/Spawn network players.");
	AddItemBool("Ped", ctlCloneBitSet, NET_OBJ_TYPE_PED, nullptr, "Clone/spawn network peds.");
	AddItemBool("Dummy Ped", ctlCloneBitSet, NET_OBJ_TYPE_DUMMY_PED, nullptr, "Clone/spawn network dummy peds.");
	AddItemBool("Automobile", ctlCloneBitSet, NET_OBJ_TYPE_AUTOMOBILE, nullptr, "Clone/spawn network automobiles.");
	AddItemBool("Object", ctlCloneBitSet, NET_OBJ_TYPE_OBJECT, nullptr, "Clone/spawn network objects.");
	AddItemBool("Bike", ctlCloneBitSet, NET_OBJ_TYPE_BIKE, nullptr, "Clone/spawn network bikes.");
	AddItemBool("Train", ctlCloneBitSet, NET_OBJ_TYPE_TRAIN, nullptr, "Clone/spawn network trains.");
	AddItemBool("Boat", ctlCloneBitSet, NET_OBJ_TYPE_BOAT, nullptr, "Clone/spawn network boats.");
	AddItemBool("Heli", ctlCloneBitSet, NET_OBJ_TYPE_HELI, nullptr, "Clone/spawn network helis.");
	AddItemBool("Plane", ctlCloneBitSet, NET_OBJ_TYPE_PLANE, nullptr, "Clone/spawn network planes.");
	*/

}

char* model_protect_strings[3] = { "Off", "Self", "Self + Kick if host" };
char* object_protect_strings[4] = { "Off", "Player only", "Vehicle only", "Both" };
void cartelProtectToggleVehNetId(int unused1, int unused2)
{
	if (!NetworkGame)
		return; 

	if (!bitTest(ctlMem->m_bit_sets.m_vehicle, blockVehicleNetMigration))
	{
		if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
		{
			NETWORK_INDEX netId;
			GET_NETWORK_ID_FROM_VEHICLE(GetPlayerVeh(), &netId);
			ReleaseControlOfNetworkId(netId);
		}
	}
}

void cartelProtectionOptons(int unused1, int unused2)
{
	if(CartelXEXPresent)
	{
		SetHeader("Protections (RGH)");
		AddItemMenu("Events", cartelProtectionEventOptions, "Switch events on or off.");
		AddItemMenu("Cloning", cartelProtectionSyncOptions, "Switch cloning/sync on or off.");
		AddItem("All known script level crashes are fixed", nullptr, "Have fun out there.");
	}
	else
	{
		SetHeader("Protections (Retail)");
		AddItem("Blackscreen is fixed", nullptr, "You will not be notified or freeze.");
		AddItem("Freeze guns are fixed", nullptr, "You will not freeze/crash from them.");
		AddItemBool("Group hack", &ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_GROUP_HACK, nullptr, "Fix risky changes to your players group.");
		AddItemChar("Player Models", false, model_protect_strings, 3, (int*)&ctlMem->m_retail_prot.m_model_prot_mode, nullptr, "Set players peds invisible/kick when a bad model.");
		AddItemBool("Peds", &ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_SUPERLOD, nullptr, "Prevent freeze caused by bad ped models.");
		AddItemBool("Glitchy Peds", &ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_CUTSCENE, nullptr, "Prevent freeze caused by glitchy ped models.");
		AddItemBool("Gamertag", &ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_TIDLE_CRASH, nullptr, "Overwrite spoofed gamertags that freeze when shown.");
		AddItemChar("Object attach", false, object_protect_strings, 4, (int*)&ctlMem->m_retail_prot.m_obj_prot_mode, nullptr, "Automatically detach objects others attach to you.");
		AddItemBool("Explosions (TEST)", &ctlMem->m_bit_sets.m_network, NETWORK_SCO_PROT_EXPLOSION_LAG, nullptr, "Clear explosions when there are too many.");
	}
	AddItemBool("Detach objects on everyones screen", &ctlMem->m_bit_sets.m_network, NETWORK_PROT_FLIP_MODEL_OBJECT_ATTACHED, nullptr, "Flips your player model temporarily.");
	AddItemBool("Restrict vehicle control", &ctlMem->m_bit_sets.m_vehicle, blockVehicleNetMigration, cartelProtectToggleVehNetId, "Stops modders controlling your vehicle.");
	AddItemBool("Report attacks to others", &ctlMem->m_bit_sets.m_menuPreferences, PREF_REPORT_ATTACKS_TO_OTHERS, nullptr, "Let Cartel users know what's up, host could auto-kick attacker.");
	AddItemBool("Print attacks against other Cartels", &ctlMem->m_bit_sets.m_menuPreferences, PREF_LISTEN_FOR_NETUSER_ATTACKS, nullptr, "Get notified if somebody fucks with another Cartel user.");
	AddItemBool("--Friends only", &ctlMem->m_bit_sets.m_menuPreferences, PREF_LISTEN_FOR_NETUSER_ATTACKS_FRIENDS_ONLY, nullptr, "Only display attacks against friends.");
	AddItemBool("Auto-vote-kick", &ctlMem->m_bit_sets.m_menuPreferences, PREF_VOTE_KICK_ATTACKERS, nullptr, "Auto-vote-kick non-friend offenders where accurate.");
	AddItemBool("Auto-vote-kick for friends", &ctlMem->m_bit_sets.m_menuPreferences, PREF_VOTE_KICK_FRIENDS_ATTACKER, nullptr, "Auto-vote-kick non-friend offenders where accurate.");
	AddItemBool("(HOST) Auto-kick", &ctlMem->m_bit_sets.m_network, NETWORK_PROT_AUTO_KICK, nullptr, "Console-kick offenders where accurate & host.");
	AddItemBool("- Auto-kick for online non-spoofed friends", &ctlMem->m_bit_sets.m_menuPreferences, PREF_HOST_AUTO_KICK_ATTACKERS_AGAINST_FRIENDS, nullptr, "Console-kick offences against Cartel friends.");
	AddItemBool("Auto-vote-kick dickheads", &ctlMem->m_bit_sets.m_menuPreferences, PREF_AUTO_VOTE_KICK_DICKHEADS, nullptr, "As described.");
	AddItemBool("Force ghosts to join", &ctlMem->m_bit_sets.m_menuPreferences, PREF_SET_GHOSTS_ACTIVE, nullptr, "Remove ghost status from players.");
#ifdef Developer
	AddItemBool("(ADM) Auto off-host kick attackers", &ctlMem->m_bit_sets.m_menuPreferences, PREF_ADMIN_ONLY_OFFHOST_ATTACKERS, nullptr, "Use Admin commands to combat attackers (where possible).");
	AddItemBool("(ADM) Auto off-host friends attackers", &ctlMem->m_bit_sets.m_menuPreferences, PREF_ADMIN_ONLY_OFFHOST_ATTACKS_AGAINST_FRIENDS, nullptr, "As described.");
#else
#ifdef Moderator
	AddItemBool("(MOD) Auto moderate attackers", &ctlMem->m_bit_sets.m_menuPreferences, PREF_ADMIN_ONLY_OFFHOST_ATTACKERS, nullptr, "Use Moderator commands to combat attackers (where possible).");
#endif
#endif
	//if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_ADMIN_ONLY_OFFHOST_ATTACKS_AGAINST_FRIENDS))

}