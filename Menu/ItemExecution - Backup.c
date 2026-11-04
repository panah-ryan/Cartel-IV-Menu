#include "natives.h"
#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "common.h"

#include "ItemExecution.h"
#include "MenuExecutionHandling.h"
#include "Functions.h"

static int SelectedPlayerId;
int MenuBool;
extern int playerBitSet = 8;
extern actionPadData myActionPads[2];

#pragma region NathansWeatherTimedata
myWeatherTimeData timeWeatherTimescaleData;
char* weatherSetStrings[10] = { "Extra Sunny", "Sunny", "Sunny Windy", "Cloudy", "Raining", "Drizzle", "Foggy", "Lightning", "Extra Sunny 2", "Sunny Windy 2" };
char* freezeWeatherStrings[11] = { "Disabled", "Extra Sunny", "Sunny", "Sunny Windy", "Cloudy", "Raining", "Drizzle", "Foggy", "Lightning", "Extra Sunny 2", "Sunny Windy 2" };
myWorldOptionsData worldOptionsData = { 0, 0, 0, 0 };

#pragma endregion


#pragma region NathansVehData

MyVehData vehData;
byte cloneVehPlacement;

char* DoorLockString[4] = { "Unlocked", "Locked", "Locked from Outside", "Locked (allow break-in)" };
char* cloneVehicleString[3] = { "Left", "Right", "Front" };

int passengerWeaponsTick, passengerArmourTick;

void DoorLocker();
void PassengerWepsCheck();
void PassengerArmourCheck();

#pragma endregion

void PrintClientId()
{
	PRINT_WITH_NUMBER_NOW("NUMBER", SelectedPlayerId, 2500, true);
}

void PlayerListSubMenu()
{
	if (WasLastMenuDirectionForward())
		SelectedPlayerId = GetLastDynamicId();
	SetHeaderForwarded();
	AddItemWithParam("Teleport to Player", SelectedPlayerId, TeleportToNetworkPlayer, nullptr);
	AddItemWithParam("Teleport to me", SelectedPlayerId, TeleportNetworkPlayerToMe, nullptr);
	AddItem("Print ID of Client", PrintClientId, nullptr);
}

void PlayerListMenu()
{
	SetHeader("Player List");
	for (int i = 0; i < 16; i++)
		if (NetworkIsPlayeractive(i))
			AddItemPlayer(i, PlayerListSubMenu);
		else
			AddItem("Empty slot", nullptr, nullptr);
}

void VehPassengerOpts()
{
	SetHeader("Vehicle -> Passengers");
	AddItemInt("Give weapons", false, 0, 60, (int*)&vehData.checkGiveWepsTimerInt, PassengerWepsCheck, "Give weapons to passengers every x seconds if not 0.");
	AddItemInt("Check & give armour", false, 0, 60, (int*)&vehData.checkGiveArmourTimerInt, PassengerArmourCheck, "Give armour to passengers every x seconds if not 0.");
	AddItemBool("Notifications", &playerBitSet, passengerGiveNotifications, nullptr, "Be notified when passengers inventory is replenished.");
	//AddItemChar("Clone vehicle", false, cloneVehicleString, 3, &cloneVehPlacement, CloneCurrentVehicle, nullptr);
	//Eject Passengers
	//Clone Vehicle (could have an int, 1 to the right, 2 is one either side etc)
}

#pragma region VehicleSpawning

void VehSpawnSportsSuper()
{
	SetHeader("Sports Cars & Supercars");
	
	AddItemVehicle("Banshee", VEHICLE_BANSHEE, SpawnPlayerInVehicle);
	AddItemVehicle("Comet", VEHICLE_COMET, SpawnPlayerInVehicle);
	AddItemVehicle("Coquette", VEHICLE_COQUETTE, SpawnPlayerInVehicle);
	AddItemVehicle("Feltzer", VEHICLE_FELTZER, SpawnPlayerInVehicle);
	AddItemVehicle("Infernus", VEHICLE_INFERNUS, SpawnPlayerInVehicle);
	AddItemVehicle("Sultan", VEHICLE_SULTAN, SpawnPlayerInVehicle);
	AddItemVehicle("Sultan RS", VEHICLE_SULTANRS, SpawnPlayerInVehicle);
	AddItemVehicle("Super GT", VEHICLE_SUPERGT, SpawnPlayerInVehicle);
	AddItemVehicle("Turismo", VEHICLE_TURISMO, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 2)
	{
		AddItemVehicle("Bullet", VEHICLE_BULLET, SpawnPlayerInVehicle);
		AddItemVehicle("F620", VEHICLE_F620, SpawnPlayerInVehicle);
	}
	
}

void VehSpawnMuscleVintage()
{
	SetHeader("Muscle Cars & Vintage");
	AddItemVehicle("Buccaneer", VEHICLE_BUCCANEER, SpawnPlayerInVehicle);
	AddItemVehicle("Dukes", VEHICLE_DUKES, SpawnPlayerInVehicle);
	AddItemVehicle("Faction", VEHICLE_FACTION, SpawnPlayerInVehicle);
	AddItemVehicle("Manana", VEHICLE_MANANA, SpawnPlayerInVehicle);
	AddItemVehicle("Peyote", VEHICLE_PEYOTE, SpawnPlayerInVehicle);
	AddItemVehicle("Ruiner", VEHICLE_RUINER, SpawnPlayerInVehicle);
	AddItemVehicle("Sabre", VEHICLE_SABRE, SpawnPlayerInVehicle);
	AddItemVehicle("Sabre GT", VEHICLE_SABREGT, SpawnPlayerInVehicle);
	AddItemVehicle("Stallion", VEHICLE_STALION, SpawnPlayerInVehicle);
	AddItemVehicle("Vigero", VEHICLE_VIGERO, SpawnPlayerInVehicle);
	AddItemVehicle("Virgo", VEHICLE_VIRGO, SpawnPlayerInVehicle);
	AddItemVehicle("Voodoo", VEHICLE_VOODOO, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 1 || GET_CURRENT_EPISODE() == 2)
	{ //EFLC
		AddItemVehicle("Tampa", VEHICLE_TAMPA, SpawnPlayerInVehicle);
	}
}

void VehSpawnTwoDoorHatchback()
{
	SetHeader("Two-Doors & Hatchbacks");
	AddItemVehicle("Blista Compact", VEHICLE_BLISTA, SpawnPlayerInVehicle);
	AddItemVehicle("Fortune", VEHICLE_FORTUNE, SpawnPlayerInVehicle);
	AddItemVehicle("Futo", VEHICLE_FUTO, SpawnPlayerInVehicle);
	AddItemVehicle("Sentinel", VEHICLE_SENTINEL, SpawnPlayerInVehicle);
	AddItemVehicle("Uranus", VEHICLE_URANUS, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 1) //TLaD
	{
		AddItemVehicle("Rhapsody", VEHICLE_RHAPSODY, SpawnPlayerInVehicle);
	}
	if (GET_CURRENT_EPISODE() == 2) //TBoGT
	{
		AddItemVehicle("Super Drop Diamond", VEHICLE_SUPERD2, SpawnPlayerInVehicle);
	}
}

void VehSpawnFourDoorWagons()
{
	SetHeader("Four-Doors & Wagons");
	AddItemVehicle("Admiral", VEHICLE_ADMIRAL, SpawnPlayerInVehicle);
	AddItemVehicle("Chavos", VEHICLE_CHAVOS, SpawnPlayerInVehicle);
	AddItemVehicle("Cognoscenti", VEHICLE_COGNOSCENTI, SpawnPlayerInVehicle);
	AddItemVehicle("DF8-90", VEHICLE_DF8, SpawnPlayerInVehicle);
	AddItemVehicle("Dilettante", VEHICLE_DILETTANTE, SpawnPlayerInVehicle);
	AddItemVehicle("Emperor", VEHICLE_EMPEROR, SpawnPlayerInVehicle);
	AddItemVehicle("Esperanto", VEHICLE_ESPERANTO, SpawnPlayerInVehicle);
	AddItemVehicle("Feroci", VEHICLE_FEROCI, SpawnPlayerInVehicle);
	AddItemVehicle("Hakumai", VEHICLE_HAKUMAI, SpawnPlayerInVehicle);
	AddItemVehicle("Ingot", VEHICLE_INGOT, SpawnPlayerInVehicle);
	AddItemVehicle("Intruder", VEHICLE_INTRUDER, SpawnPlayerInVehicle);
	AddItemVehicle("Lokus", VEHICLE_LOKUS, SpawnPlayerInVehicle);
	AddItemVehicle("Marbelle", VEHICLE_MARBELLA, SpawnPlayerInVehicle);
	AddItemVehicle("Merit", VEHICLE_MERIT, SpawnPlayerInVehicle);
	AddItemVehicle("Oracle", VEHICLE_ORACLE, SpawnPlayerInVehicle);
	AddItemVehicle("Pinnacle", VEHICLE_PINNACLE, SpawnPlayerInVehicle);
	AddItemVehicle("PMP 600", VEHICLE_PMP600, SpawnPlayerInVehicle);
	AddItemVehicle("Premier", VEHICLE_PREMIER, SpawnPlayerInVehicle);
	AddItemVehicle("Presidente", VEHICLE_PRES, SpawnPlayerInVehicle);
	AddItemVehicle("Primo", VEHICLE_PRIMO, SpawnPlayerInVehicle);
	AddItemVehicle("Schafter", VEHICLE_SCHAFTER, SpawnPlayerInVehicle);
	AddItemVehicle("Solair", VEHICLE_SOLAIR, SpawnPlayerInVehicle);
	AddItemVehicle("Stratum", VEHICLE_STRATUM, SpawnPlayerInVehicle);
	AddItemVehicle("Stretch", VEHICLE_STRETCH, SpawnPlayerInVehicle);
	AddItemVehicle("Vincent", VEHICLE_VINCENT, SpawnPlayerInVehicle);
	AddItemVehicle("Washington", VEHICLE_WASHINGTON, SpawnPlayerInVehicle);
	AddItemVehicle("Willard", VEHICLE_WILLARD, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 2)
	{
		AddItemVehicle("Buffalo", VEHICLE_BUFFALO, SpawnPlayerInVehicle);
		AddItemVehicle("Schafter 2", VEHICLE_SCHAFTER2, SpawnPlayerInVehicle);
		AddItemVehicle("Schafter 3", VEHICLE_SCHAFTER3, SpawnPlayerInVehicle);
		AddItemVehicle("Stretch E", VEHICLE_LIMO2, SpawnPlayerInVehicle);
		AddItemVehicle("Super Diamond", VEHICLE_SUPERD, SpawnPlayerInVehicle);
	}
}

void VehSpawnSuvPickupVans()
{
	SetHeader("SUVs, Pick-ups & Vans");
	AddItemVehicle("Bobcat", VEHICLE_BOBCAT, SpawnPlayerInVehicle);
	AddItemVehicle("Cavalcade", VEHICLE_CAVALCADE, SpawnPlayerInVehicle);
	AddItemVehicle("Cavalcade FXT", VEHICLE_FXT, SpawnPlayerInVehicle);
	AddItemVehicle("Contender", VEHICLE_E109, SpawnPlayerInVehicle);
	AddItemVehicle("Habanero", VEHICLE_HABANERO, SpawnPlayerInVehicle);
	AddItemVehicle("Huntley Sport", VEHICLE_HUNTLEY, SpawnPlayerInVehicle);
	AddItemVehicle("Landstalker", VEHICLE_LANDSTALKER, SpawnPlayerInVehicle);
	AddItemVehicle("Minivan", VEHICLE_MINIVAN, SpawnPlayerInVehicle);
	AddItemVehicle("Moonbeam", VEHICLE_MOONBEAM, SpawnPlayerInVehicle);
	AddItemVehicle("Patriot", VEHICLE_PATRIOT, SpawnPlayerInVehicle);
	AddItemVehicle("Perennial", VEHICLE_PERENNIAL, SpawnPlayerInVehicle);
	AddItemVehicle("Rancher", VEHICLE_RANCHER, SpawnPlayerInVehicle);
	AddItemVehicle("Rebla", VEHICLE_REBLA, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 1)
	{
		AddItemVehicle("Slamvan", VEHICLE_SLAMVAN, SpawnPlayerInVehicle);
		AddItemVehicle("Tow Truck", VEHICLE_TOWTRUCK, SpawnPlayerInVehicle);
	}
	if (GET_CURRENT_EPISODE() == 2)
	{
		AddItemVehicle("Cavalcade 2", VEHICLE_CAVALCADE2, SpawnPlayerInVehicle);
		AddItemVehicle("Serrano", VEHICLE_SERRANO, SpawnPlayerInVehicle);
		AddItemVehicle("Serrano 2", VEHICLE_SERRANO2, SpawnPlayerInVehicle);
		AddItemVehicle("Slamvan", VEHICLE_SLAMVAN, SpawnPlayerInVehicle);
	}
}

void VehSpawnCommercialIndustrial()
{
	SetHeader("Commercial & Industrial");
	AddItemVehicle("Airtug", VEHICLE_AIRTUG, SpawnPlayerInVehicle);
	AddItemVehicle("Benson", VEHICLE_BENSON, SpawnPlayerInVehicle);
	AddItemVehicle("Biff", VEHICLE_BIFF, SpawnPlayerInVehicle);
	AddItemVehicle("Boxville", VEHICLE_BOXVILLE, SpawnPlayerInVehicle);
	AddItemVehicle("Burrito", VEHICLE_BURRITO, SpawnPlayerInVehicle);
	AddItemVehicle("Flatbed", VEHICLE_FLATBED, SpawnPlayerInVehicle);
	AddItemVehicle("FlyUS Feroci", VEHICLE_FEROCI2, SpawnPlayerInVehicle);
	AddItemVehicle("FlyUS Perennial", VEHICLE_PERENNIAL2, SpawnPlayerInVehicle);
	AddItemVehicle("Forklift", VEHICLE_FORKLIFT, SpawnPlayerInVehicle);
	AddItemVehicle("Laundromat", VEHICLE_BURRITO2, SpawnPlayerInVehicle);
	AddItemVehicle("Mule", VEHICLE_MULE, SpawnPlayerInVehicle);
	AddItemVehicle("Packer", VEHICLE_PACKER, SpawnPlayerInVehicle);
	AddItemVehicle("Phantom", VEHICLE_PHANTOM, SpawnPlayerInVehicle);
	AddItemVehicle("Pony", VEHICLE_PONY, SpawnPlayerInVehicle);
	AddItemVehicle("Ripley", VEHICLE_RIPLEY, SpawnPlayerInVehicle);
	AddItemVehicle("Securicar", VEHICLE_STOCKADE, SpawnPlayerInVehicle);
	AddItemVehicle("Speedo", VEHICLE_SPEEDO, SpawnPlayerInVehicle);
	AddItemVehicle("Steed", VEHICLE_STEED, SpawnPlayerInVehicle);
	AddItemVehicle("Yankee", VEHICLE_YANKEE, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 1)
	{
		AddItemVehicle("Gang Burrito", VEHICLE_GBURRITO, SpawnPlayerInVehicle);
		AddItemVehicle("Packer 2", VEHICLE_PACKER2, SpawnPlayerInVehicle);
		AddItemVehicle("Yankee (Open bed)", VEHICLE_YANKEE2, SpawnPlayerInVehicle);
	}
}

void VehSpawnEmergency()
{
	SetHeader("Emergency");
	AddItemVehicle("Ambulance", VEHICLE_AMBULANCE, SpawnPlayerInVehicle);
	AddItemVehicle("Enforcer", VEHICLE_NSTOCKADE, SpawnPlayerInVehicle);
	AddItemVehicle("FIB Buffalo", VEHICLE_FBI, SpawnPlayerInVehicle);
	AddItemVehicle("Fire Truck", VEHICLE_FIRETRUK, SpawnPlayerInVehicle);
	AddItemVehicle("NOOSE Cruiser", VEHICLE_NOOSE, SpawnPlayerInVehicle);
	AddItemVehicle("NOOSE Patriot", VEHICLE_POLPATRIOT, SpawnPlayerInVehicle);
	AddItemVehicle("Police Cruiser", VEHICLE_POLICE, SpawnPlayerInVehicle);
	AddItemVehicle("Police Patrol", VEHICLE_POLICE2, SpawnPlayerInVehicle);
	AddItemVehicle("Police Stockade", VEHICLE_PSTOCKADE, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 1) //TLaD
	{
		AddItemVehicle("Prison Bus", VEHICLE_PBUS, SpawnPlayerInVehicle);
	}
	if (GET_CURRENT_EPISODE() == 2) //TBoGT
	{
		AddItemVehicle("APC", VEHICLE_APC, SpawnPlayerInVehicle);
		AddItemVehicle("Brickade", VEHICLE_AVAN, SpawnPlayerInVehicle);
		AddItemVehicle("Police Buffalo", VEHICLE_POLICE3, SpawnPlayerInVehicle);
		AddItemVehicle("Police Stinger", VEHICLE_POLICE4, SpawnPlayerInVehicle);
	}
}

void VehSpawnPubService()
{
	SetHeader("Public Service");
	AddItemVehicle("Bus", VEHICLE_BUS, SpawnPlayerInVehicle);
	AddItemVehicle("Cabby", VEHICLE_CABBY, SpawnPlayerInVehicle);
	AddItemVehicle("Cable Car", VEHICLE_CABLECAR, SpawnPlayerInVehicle);
	AddItemVehicle("Mr Tasty", VEHICLE_MRTASTY, SpawnPlayerInVehicle);
	AddItemVehicle("Roman's Taxi", VEHICLE_ROM, SpawnPlayerInVehicle);
	AddItemVehicle("Romero", VEHICLE_ROMERO, SpawnPlayerInVehicle);
	AddItemVehicle("Subway High", VEHICLE_SUBWAY_HI, SpawnPlayerInVehicle);
	AddItemVehicle("Subway Low", VEHICLE_SUBWAY_LO, SpawnPlayerInVehicle);
	AddItemVehicle("Taxi", VEHICLE_TAXI, SpawnPlayerInVehicle);
	AddItemVehicle("Taxi 2", VEHICLE_TAXI2, SpawnPlayerInVehicle);
	AddItemVehicle("Trashmaster", VEHICLE_TRASH, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 2) // TBoGT
	{
		AddItemVehicle("Caddy", VEHICLE_CADDY, SpawnPlayerInVehicle);
	}
}

void VehSpawnMotorcycles()
{
	SetHeader("Motorcycles");
	AddItemVehicle("Faggio", VEHICLE_FAGGIO, SpawnPlayerInVehicle);
	AddItemVehicle("Freeway", VEHICLE_BOBBER, SpawnPlayerInVehicle);
	AddItemVehicle("Hellfury", VEHICLE_HELLFURY, SpawnPlayerInVehicle);
	AddItemVehicle("NRG 900", VEHICLE_NRG900, SpawnPlayerInVehicle);
	AddItemVehicle("PCJ 600", VEHICLE_PCJ, SpawnPlayerInVehicle);
	AddItemVehicle("Sanchez", VEHICLE_SANCHEZ, SpawnPlayerInVehicle);
	AddItemVehicle("Zombie", VEHICLE_ZOMBIEB, SpawnPlayerInVehicle); //7
	if (GET_CURRENT_EPISODE() == 1) //TLaD
	{
		AddItemVehicle("Angel", VEHICLE_ANGEL, SpawnPlayerInVehicle);
		AddItemVehicle("Bati 800", VEHICLE_BATI, SpawnPlayerInVehicle);
		AddItemVehicle("Bati Custom", VEHICLE_BATI2, SpawnPlayerInVehicle);
		AddItemVehicle("Daemon", VEHICLE_DAEMON, SpawnPlayerInVehicle);
		AddItemVehicle("Diabolus", VEHICLE_DIABOLUS, SpawnPlayerInVehicle);
		AddItemVehicle("Double T", VEHICLE_DOUBLE, SpawnPlayerInVehicle);
		AddItemVehicle("Double T Custom", VEHICLE_DOUBLE2, SpawnPlayerInVehicle);
		AddItemVehicle("Hakuchou", VEHICLE_HAKUCHOU, SpawnPlayerInVehicle);
		AddItemVehicle("Hakuchou Custom", VEHICLE_HAKUCHOU2, SpawnPlayerInVehicle);
		AddItemVehicle("Hexer", VEHICLE_HEXER, SpawnPlayerInVehicle);
		AddItemVehicle("Innovation", VEHICLE_INNOVATION, SpawnPlayerInVehicle);
		AddItemVehicle("Lycan", VEHICLE_LYCAN, SpawnPlayerInVehicle);
		AddItemVehicle("Nightblade", VEHICLE_NIGHTBLADE, SpawnPlayerInVehicle);
		AddItemVehicle("Revenant", VEHICLE_REVENANT, SpawnPlayerInVehicle);
		AddItemVehicle("Wayfarer", VEHICLE_WAYFARER, SpawnPlayerInVehicle);
		AddItemVehicle("Wolfsbane", VEHICLE_WOLFSBANE, SpawnPlayerInVehicle);
	}
	else if (GET_CURRENT_EPISODE() == 2) //TBoGT
	{
		AddItemVehicle("Akuma", VEHICLE_AKUMA, SpawnPlayerInVehicle);
		AddItemVehicle("Bati Custom", VEHICLE_BATI2, SpawnPlayerInVehicle);
		AddItemVehicle("Double T", VEHICLE_DOUBLE, SpawnPlayerInVehicle);
		AddItemVehicle("Faggio (classic)", VEHICLE_FAGGIO2, SpawnPlayerInVehicle);
		AddItemVehicle("Hakuchou", VEHICLE_HAKUCHOU, SpawnPlayerInVehicle);
		AddItemVehicle("Hexer", VEHICLE_HEXER, SpawnPlayerInVehicle);
		AddItemVehicle("Police Bike", VEHICLE_POLICEB, SpawnPlayerInVehicle);
		AddItemVehicle("Vader", VEHICLE_VADER, SpawnPlayerInVehicle);
	}
}

void VehSpawnBoats()
{
	SetHeader("Boats");
	AddItemVehicle("Dinghy", VEHICLE_DINGHY, SpawnPlayerInVehicle);
	AddItemVehicle("Jetmax", VEHICLE_JETMAX, SpawnPlayerInVehicle);
	AddItemVehicle("Marquis", VEHICLE_MARQUIS, SpawnPlayerInVehicle);
	AddItemVehicle("Police Predator", VEHICLE_PREDATOR, SpawnPlayerInVehicle);
	AddItemVehicle("Reefer", VEHICLE_REEFER, SpawnPlayerInVehicle);
	AddItemVehicle("Squalo", VEHICLE_SQUALO, SpawnPlayerInVehicle);
	AddItemVehicle("Tropic", VEHICLE_TROPIC, SpawnPlayerInVehicle);
	AddItemVehicle("Tug Boat", VEHICLE_TUGA, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 2) //TBoGT
	{
		AddItemVehicle("Blade", VEHICLE_BLADE, SpawnPlayerInVehicle);
		AddItemVehicle("Floater", VEHICLE_FLOATER, SpawnPlayerInVehicle);
		AddItemVehicle("Smuggler", VEHICLE_SMUGGLER, SpawnPlayerInVehicle);
	}
}

void VehSpawnAircraft()
{
	SetHeader("Aircraft");
	AddItemVehicle("Annihilator", VEHICLE_ANNIHILATOR, SpawnPlayerInVehicle);
	AddItemVehicle("Helitours Maverick", VEHICLE_TOURMAV, SpawnPlayerInVehicle);
	AddItemVehicle("Maverick", VEHICLE_MAVERICK, SpawnPlayerInVehicle);
	AddItemVehicle("Police Maverick", VEHICLE_POLMAV, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 2)
	{
		AddItemVehicle("Buzzard", VEHICLE_BUZZARD, SpawnPlayerInVehicle);
		AddItemVehicle("Skylift", VEHICLE_SKYLIFT, SpawnPlayerInVehicle);
		AddItemVehicle("Swift", VEHICLE_SWIFT, SpawnPlayerInVehicle);
	}
}

void VehSpawnJunkers()
{
	SetHeader("Junkers");
	AddItemVehicle("Vigero 2", VEHICLE_VIGERO2, SpawnPlayerInVehicle);
	AddItemVehicle("Sabre 2", VEHICLE_SABRE2, SpawnPlayerInVehicle);
	AddItemVehicle("Emperor 2", VEHICLE_EMPEROR2, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 1) //TLaD
		AddItemVehicle("Regina", VEHICLE_REGINA, SpawnPlayerInVehicle);
	if (GET_CURRENT_EPISODE() == 2) //TBoGT
		AddItemVehicle("Police Cruiser", VEHICLE_POLICEW, SpawnPlayerInVehicle);
}

void SpawnVehicleSetup()
{
	SetHeader("Vehicle selection");
	AddItemMenu("Sports Cars & Supercars", VehSpawnSportsSuper, nullptr);
	AddItemMenu("Muscle Cars & Vintage", VehSpawnMuscleVintage, nullptr);
	AddItemMenu("Two-Doors & Hatchbacks", VehSpawnTwoDoorHatchback, nullptr);
	AddItemMenu("Four-Doors & Wagons", VehSpawnFourDoorWagons, nullptr);
	AddItemMenu("SUVs, Pick-ups & Vans", VehSpawnSuvPickupVans, nullptr);
	AddItemMenu("Commercial & Industrial", VehSpawnCommercialIndustrial, nullptr);
	AddItemMenu("Emergency", VehSpawnEmergency, nullptr);
	AddItemMenu("Public Service", VehSpawnPubService, nullptr);
	AddItemMenu("Motorcycles", VehSpawnMotorcycles, nullptr);
	AddItemMenu("Boats", VehSpawnBoats, nullptr);
	AddItemMenu("Aircraft", VehSpawnAircraft, nullptr);	
	AddItemMenu("Junkers", VehSpawnJunkers, nullptr);

}

#pragma endregion

void VehicleToggles()
{
	SetHeader("Vehicle -> Toggles");
	AddItemBool("Invincible loop", &playerBitSet, vehicleInvincibleLoop, nullptr, "Actively set vehicles you drive invincible.");
	AddItemBool("Invincible spawns", &playerBitSet, spawnVehiclesInvincible, nullptr, "Makes every vehicle spawned immune from damage.");
	AddItemBool("Spawn locked", &playerBitSet, spawnedVehiclesRequireBreakIn, nullptr, "Force players to break-in to spawned vehicles."); //our signature :D, whack it on by default (harmless)
}

void VehicleOptions()
{
	vehData.currentLock = 1; //will return unlocked car if on foot.
	if (IS_CHAR_IN_ANY_CAR(GetPlayerPed()))
		GET_CAR_DOOR_LOCK_STATUS(GetPlayerVeh(), (uint*)&vehData.currentLock);
	vehData.itemSelect = DoorLockToItemSelect(vehData.currentLock);

	SetHeader("Vehicle Options");
	AddItemMenu("Spawn vehicle", SpawnVehicleSetup, nullptr);
	AddItemMenu("Toggles", VehicleToggles, "Loops and misc settings");
	AddItemChar("Set door lock", false, DoorLockString, 4, (int*)&vehData.itemSelect, DoorLocker, "Lock vehicle doors accordingly.");
	AddItemMenu("Passengers", VehPassengerOpts, "Loops, clone vehicle, eject etc.");
}

void SetPlayerGodMode()
{
	SET_CHAR_INVINCIBLE(GetPlayerPed(), IS_BIT_SET(playerBitSet, playerGodMode));
}


void PlayerOptions()
{
	SetHeader("Player Options");
	AddItemBool("God mode", &playerBitSet, playerGodMode, SetPlayerGodMode, "Make your character invincible.");
	//walk on water comes next.. could potentially improve it even more... hmmmmmmmmmmm
}

void worldOptsSetTime()
{
	SET_TIME_OF_DAY(worldOptionsData.timeHourSelect, worldOptionsData.timeMinuteSelect);
	if (IS_BIT_SET(playerBitSet, freezeTime))
	{
		timeWeatherTimescaleData.freezeTimeHour = worldOptionsData.timeHourSelect;
		timeWeatherTimescaleData.freezeTimeMinute = worldOptionsData.timeMinuteSelect;
	}
}

void WorldOptsSetWeather()
{
	FORCE_WEATHER_NOW(worldOptionsData.weatherSelect);
	if (timeWeatherTimescaleData.freezeWeather != 0)
		timeWeatherTimescaleData.freezeWeather = (worldOptionsData.weatherSelect + 1);
}

void WorldOptsFreezeWeather()
{
	//don't need shit ere either
	if (timeWeatherTimescaleData.freezeWeather != 0)
	{
		printf("~r~CTL: ~s~Freeze weather set to: ~y~%s.", freezeWeatherStrings[timeWeatherTimescaleData.freezeWeather]);
	}
}

void WorldOptsFreezetime()
{
	if (IS_BIT_SET(playerBitSet, freezeTime))
	{
		GET_TIME_OF_DAY(&timeWeatherTimescaleData.freezeTimeHour, &timeWeatherTimescaleData.freezeTimeMinute);
	}
}


void PadSettings(int padId)
{
	SetHeader(padId == 0 ? "World -> Action pads -> 1" : "World -> Action pads -> 2");
	AddItemWithParam("Set teleport destination", padId, SetActionPadTeleportCoordinates, "Set destination to current location.");
	AddItemWithParam("Delete pad", padId, DeleteAndClearActionPadVars, "Remove this pad from the world.");
	
}

void ActionPadOptions()
{
	SetHeader("World -> Action pads");
	AddItemWithParam("Spawn teleporter", PAD_MODE_TELEPORT_TO_SAVED_DESTINATION, SpawnActionPadInFront, "Spawn a pad that teleports players.");
	AddItemWithParam("Spawn vehicle fixer", PAD_MODE_FIX_VEHICLE, SpawnActionPadInFront, "Spawn pad that fixes players cars.");
	AddItemWithParam("Spawn vehicle spawner", PAD_MODE_SPAWN_VEHICLE, SpawnActionPadInFront, "Spawn a pad that generates vehicles.");
	//I'm thinking, spawn car, store it, wait for player to be in said vehicle, teleport em out way n spawn new one. (and make sure it's still on the pad while no player is inside it)
	AddItemBool("Pad notifications", &playerBitSet, aPadNotifications, nullptr, "Get notified when a pad acts on a player.");
	if (DOES_OBJECT_EXIST(myActionPads[0].PadObj))
		AddItemMenuWithParam("Actionpad 1 Settings", 0, PadSettings, "Set up pad 1");
	if (DOES_OBJECT_EXIST(myActionPads[1].PadObj))
		AddItemMenuWithParam("Actionpad 2 Settings", 1, PadSettings, "Set up pad 2");
	//void DeleteAndClearActionPadVars(int padIndex)
	
	//Spawn vehicle generator < string selection of commonly requested vehicles >

}

void WorldWeatherTimeOptions()
{
	SetHeader("World -> Weather/Time");
	AddItemChar("Set weather", false, weatherSetStrings, 10, &worldOptionsData.weatherSelect, WorldOptsSetWeather, nullptr);
	AddItemInt("Set time hour", false, 0, 23, &worldOptionsData.timeHourSelect, worldOptsSetTime, nullptr);
	AddItemInt("Set time minute", false, 0, 59, &worldOptionsData.timeMinuteSelect, worldOptsSetTime, nullptr);
	AddItemBool("Freeze time", &playerBitSet, freezeTime, WorldOptsFreezetime, nullptr);
	AddItemChar("Freeze weather", false, freezeWeatherStrings, 11, &timeWeatherTimescaleData.freezeWeather, WorldOptsFreezeWeather, nullptr);
}

void WorldOptions()
{
	SetHeader("World");
	AddItemMenu("Weather/Time", WorldWeatherTimeOptions, nullptr);
	AddItemMenu("Action pads", ActionPadOptions, "Spawn a pad that acts upon nearby players.");
}

void MainMenu()
{
	SetHeader("Greetings Comrade");
	AddItemMenu("Player", PlayerOptions, nullptr);
	AddItemMenu("Vehicle", VehicleOptions, nullptr);
	AddItemMenu("Network Players", PlayerListMenu, nullptr);
	AddItemMenu("World", WorldOptions, "Ramps, weather/time etc");
	//AddItemMenu("Game Options", GameOptions, nullptr);
}

#pragma region VehFunctions

void DoorLocker()
{
	if (IS_CHAR_IN_ANY_CAR(GetPlayerPed()))
	{
		const int veh = GetPlayerVeh();
		int netId;
		GET_NETWORK_ID_FROM_VEHICLE(veh, &netId);
		if (RequestNetworkControl(netId, "vehicle"))
		{
			LOCK_CAR_DOORS(veh, ItemSelectToDoorLock(vehData.itemSelect));
			printf("~r~CTL: ~s~Door lock set to %s.", DoorLockString[vehData.itemSelect]);
		}
	}
}

void PassengerWepsCheck()
{
	if (!vehData.checkGiveWepsTimerInt)
		printf("~r~CTL: ~s~Passenger weapons loop disabled.");
	else
	{
		passengerWeaponsTick = (GetTickCount() + (vehData.checkGiveWepsTimerInt * 1000));
		printf("~r~CTL: ~s~Passengers will receive weapons they do not posess every %i seconds.", vehData.checkGiveWepsTimerInt);
	}
}

void PassengerArmourCheck()
{
	if (!vehData.checkGiveArmourTimerInt)
		printf("~r~CTL: ~s~Passenger armour loop disabled.");
	else
	{
		passengerArmourTick = (GetTickCount() + (vehData.checkGiveArmourTimerInt * 1000));
		printf("~r~CTL: ~s~Passengers will receive armour if they're clean out every %i seconds.", vehData.checkGiveArmourTimerInt);
	}
}

#pragma endregion

void LoopedOptions()
{
#pragma region WorldOptionsLoops
	if (IS_BIT_SET(playerBitSet, freezeTime))
	{
		FORCE_TIME_OF_DAY(timeWeatherTimescaleData.freezeTimeHour, timeWeatherTimescaleData.freezeTimeMinute);
	}

	if (timeWeatherTimescaleData.freezeWeather != 0)
	{
		FORCE_WEATHER_NOW(timeWeatherTimescaleData.freezeWeather - 1); //index 0 is "Disabled" so -1
	}
#pragma endregion

#pragma region ActionPads
	Ped networkPlayerPed;
	float pX, pY, pZ, pH, distance;
	for (int i = 0; i < 2; i++) 
	{
		if (!DOES_OBJECT_EXIST(myActionPads[i].PadObj)) continue;
		for (int R = 0; R < 16; R++)
		{
			if (!NetworkIsPlayerActive(R) || !PLAYER_HAS_CHAR(R)) continue;
			GET_PLAYER_CHAR(R, &networkPlayerPed);
			GET_CHAR_COORDINATES(networkPlayerPed, &pX, &pY, &pZ);

			if (myActionPads[i].currentMode == PAD_MODE_SPAWN_VEHICLE)
			{
				if (myActionPads[i].modeData) //vehicle model has been set and stored here if it holds a value other than 0
				{
					if (DOES_VEHICLE_EXIST(myActionPads[i].padGeneratedVeh))
					{
						Ped driver;
						GET_DRIVER_OF_CAR(myActionPads[i].padGeneratedVeh, &driver);
						if (DOES_CHAR_EXIST(driver))
						{
							GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(myActionPads[i].PadObj, 0.0f, -8.0f, +1.5f, &pX, &pY, &pZ); //car is on object soo..
							SET_CAR_COORDINATES_NO_OFFSET(myActionPads[i].padGeneratedVeh, pX, pY, pZ);
							myActionPads[i].padGeneratedVeh = 0; //spawn another
							const int potentialPlayer = IsThisPedAPlayer(driver); //returns -1 if false, playerindex if true
							if (potentialPlayer != -1) //playerindex is stored here.
							{
								if (IS_BIT_SET(playerBitSet, aPadNotifications))
									printf("~r~CTL PAD %i: %s ~s~took a %s, re-spawning.", i, NetworkPlayerGetNameWithColour(potentialPlayer), VehiclesModelName(myActionPads[i].modeData));


							}
						}
					}
					else
					{
						vector3 vehModelMin, vehModelMax;
						GET_MODEL_DIMENSIONS(myActionPads[i].modeData, &vehModelMin, &vehModelMax);
						GET_OBJECT_HEADING(myActionPads[i].PadObj, &pH);
						SpawnVehicle(myActionPads[i].modeData, myActionPads[i].padCoordinates.x, myActionPads[i].padCoordinates.y, (myActionPads[i].padCoordinates.z + vehModelMax.z), pH, &myActionPads[i].padGeneratedVeh);
					}
				}
			}
			else
			{
				if (!IS_CHAR_IN_AREA_2D(networkPlayerPed, myActionPads[i].Corner1X, myActionPads[i].Corner1Y, myActionPads[i].Corner2X, myActionPads[i].Corner2Y, false)) continue;
				GET_DISTANCE_BETWEEN_COORDS_3D(myActionPads[i].padCoordinates.x, myActionPads[i].padCoordinates.y, myActionPads[i].padCoordinates.z, pX, pY, pZ, &distance);
				if (distance > 6.0f) continue; //essentially a height check
				if (myActionPads[i].currentMode == PAD_MODE_GIVE_ARMOUR_HEALTH_WEAPONS)
				{
					GiveWeaponsToPed(networkPlayerPed, GET_CURRENT_EPISODE()); //fuck TLaD they can have poor weps.
					if (IS_BIT_SET(playerBitSet, aPadNotifications))
						printf("~r~CTL: %s ~s~received weapons from actionpad %i.", NetworkPlayerGetNameWithColour(R), i);
				}
				if (myActionPads[i].currentMode == PAD_MODE_TELEPORT_TO_SAVED_DESTINATION)
				{
					if (myActionPads[i].modeData) //Set to 1 when destination has been set
					{
						//check if on foot or in vehicle.
						if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
						{
							Vehicle networkPlayerVehicle;
							int networkPlayerVehicleId;
							GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
							GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);
							REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId); //wait for loop to come back round
							if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
							{
								SET_CAR_COORDINATES(networkPlayerVehicle, myActionPads[i].teleportDest.x, myActionPads[i].teleportDest.y, myActionPads[i].teleportDest.z);
								if (IS_BIT_SET(playerBitSet, aPadNotifications))
									printf("~r~CTL PAD %i: ~s~Teleported %s ~s~to saved location.", i, NetworkPlayerGetNameWithColour(R));
							}
						}
						else
						{
							TeleportNetworkPlayerOnFoot(R, myActionPads[i].teleportDest.x, myActionPads[i].teleportDest.y, myActionPads[i].teleportDest.z, 0.0f);
							if (IS_BIT_SET(playerBitSet, aPadNotifications))
								printf("~r~CTL PAD %i: ~s~Teleported %s ~s~to saved location.", i, NetworkPlayerGetNameWithColour(R));
						}
					}
					else
						printf("~r~CTL PAD %i: ~s~Unable to teleport %s ~s~as no location is set.", i, NetworkPlayerGetNameWithColour(R));
				}
				if (myActionPads[i].currentMode == PAD_MODE_FIX_VEHICLE)
				{
					if (IS_CHAR_IN_ANY_CAR(networkPlayerPed))
					{
						Vehicle networkPlayerVehicle;
						int networkPlayerVehicleId;
						GET_CAR_CHAR_IS_USING(networkPlayerPed, &networkPlayerVehicle);
						GET_NETWORK_ID_FROM_VEHICLE(networkPlayerVehicle, &networkPlayerVehicleId);
						REQUEST_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId); //wait for loop to come back round
						if (HAS_CONTROL_OF_NETWORK_ID(networkPlayerVehicleId))
						{
							FIX_CAR(networkPlayerVehicle);
							GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(myActionPads[i].PadObj, 0.0f, -8.0f, +1.5f, &pX, &pY, &pZ); //car is on object soo..
							SET_CAR_COORDINATES_NO_OFFSET(networkPlayerVehicle, pX, pY, pZ);
							if (IS_BIT_SET(playerBitSet, aPadNotifications))
								printf("~r~CTL PAD %i: ~s~Fixed %s's ~s~%s.", i, NetworkPlayerGetNameWithColour(R), VehiclesModelName(networkPlayerVehicle));
							//get offset dump them y + 5 or something so they not stuck on the pad
						}
					}
					else
					{
						GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(myActionPads[i].PadObj, 3.0f, 5.0f, +1.0f, &pX, &pY, &pZ); //move em out the way silently..
						GET_OBJECT_HEADING(myActionPads[i].PadObj, &pH);
						TeleportNetworkPlayerOnFoot(R, pX, pY, pZ, pH);
					}
				}
			}
		}
	}
#pragma endregion


#pragma region VehicleLoops
	if (IS_CHAR_IN_ANY_CAR(GetPlayerPed()))
	{
		Ped driverped;

		Vehicle myVeh = GetPlayerVeh();
		GET_DRIVER_OF_CAR(myVeh, &driverped);

		if (driverped == GetPlayerPed()) //code to be executed only when local player is the driver of a vehicle..
		{

			if (bit_test(playerBitSet, vehicleInvincibleLoop))
			{
				SET_CAR_CAN_BE_DAMAGED(myVeh, false);
				SET_CAR_CAN_BE_VISIBLY_DAMAGED(myVeh, false);
				SET_CAR_WATERTIGHT(myVeh, true);
				SET_CAR_PROOFS(myVeh, true, true, true, true, true);
				SET_CAN_BURST_CAR_TYRES(myVeh, false);
				//SET_CAR_STRONG(myVeh, true); //idek what this does
			}
			uint myPassengers; 
			
			GET_NUMBER_OF_PASSENGERS(myVeh, &myPassengers);
			if (myPassengers > 0)
			{
				if (vehData.checkGiveArmourTimerInt > 0 && (passengerArmourTick < GetTickCount())) //Timer int set above 0 and has been more than set time in ms.
				{
					Ped passenger; 
					bool given = false;

					for (int i = 0; i < 3; i++)
					{
						uint armour;

						if (IS_CAR_PASSENGER_SEAT_FREE(myVeh, i)) 
							continue;
						GET_CHAR_IN_CAR_PASSENGER_SEAT(myVeh, i, &passenger);
						GET_CHAR_ARMOUR(passenger, &armour);
						if (armour < 5)
						{
							if (!HAS_CHAR_GOT_WEAPON(passenger, WEAPON_ARMOUR))
								GIVE_WEAPON_TO_CHAR(passenger, WEAPON_ARMOUR, 1, false);
							ADD_ARMOUR_TO_CHAR(passenger, 999);
							given = true;
						}
					}
					passengerArmourTick = (GetTickCount() + (vehData.checkGiveArmourTimerInt * 1000)); //re-set timer as ran
					if (given && bit_test(playerBitSet, passengerGiveNotifications)) 
						printf("~r~CTL: ~s~Passengers armour replenished.");
				}

				if (vehData.checkGiveWepsTimerInt > 0 && (passengerWeaponsTick < GetTickCount())) //Timer int set above 0 and has been more than set time in ms.
				{
					Ped passenger; 
					bool given = false;

					for (int i = 0; i < 3; i++)
					{
						if (IS_CAR_PASSENGER_SEAT_FREE(myVeh, i)) 
							continue;
						GET_CHAR_IN_CAR_PASSENGER_SEAT(myVeh, i, &passenger);
						GiveWeaponsToPed(passenger, WEAPON_TYPE_ADVANCED);
						given = true;
					}
					passengerWeaponsTick = (GetTickCount() + (vehData.checkGiveWepsTimerInt * 1000));
					if(given && IS_BIT_SET(playerBitSet, passengerGiveNotifications)) 
						printf("~r~CTL: ~s~Passengers weapons replenished.");
				}
			}
		}
	}
#pragma endregion
}