#include "stdafx.h"
#include "Utils.h"
#include "MenuExecutionHandling.h"
#include "MenuUiHandling.h"
#include "Functions.h"
#include "Network_Options.h"
#include "MilitaryMissiles.h"

void unlockAchievement(int achievement, int unused1)
{
	if (HAS_ACHIEVEMENT_BEEN_PASSED(achievement))
		printf("~r~CTL: ~w~You have already unlocked the achievement: ~y~%s", GetCurrentMenuItemText());
	else
	{
		AWARD_ACHIEVEMENT(achievement);
		printf("~r~CTL: ~w~Unlocked: ~y~%s ~w~(%s)", GetCurrentMenuItemText(), GetCurrentMenuItemDesc());
	}
}

void unlockAllAchievements(int unused1, int unused2)
{
	for (int i = 0; i < 65; i++)
		if (!HAS_ACHIEVEMENT_BEEN_PASSED(i))
			AWARD_ACHIEVEMENT(i);

	printf("~r~CTL: ~w~All achievements unlocked.");
}

void achievementUnlockOptions()
{
	SetHeader("Achievement unlocker");
	AddItemAlt("Unlock all (~PAD_X~)", nullptr, "Prepare for XNotify spam.", unlockAllAchievements, nullptr);
	//bool, only display locked. meh not worth the effort IMO you can only unlock once anyway.
	AddItemWithParam("Off The Boat", 1, unlockAchievement, "Complete the first mission.");
	AddItemWithParam("One Hundred And Eighty", 2, unlockAchievement, "In a darts game score 180 with 3 darts.");
	AddItemWithParam("Pool Shark", 3, unlockAchievement, "Beat a friend at pool.");
	AddItemWithParam("King of QUB3D", 4, unlockAchievement, "Beat the High Score in QUB3D.");
	AddItemWithParam("Finish Him", 5, unlockAchievement, "Complete 10 melee counters in 4 minutes.");
	AddItemWithParam("Genetically Superior", 6, unlockAchievement, "Come first in 20 singleplayer street races.");
	AddItemWithParam("Wheelie Rider", 7, unlockAchievement, "Do a wheelie lasting at least 500 feet on a motorbike.");
	AddItemWithParam("Gobble Gobble", 8, unlockAchievement, "Score 3 strikes in a row, a turkey, in 10-pin bowling.");
	AddItemWithParam("Driving Mr. Bellic", 9, unlockAchievement, "Unlock the special ability of taxi.");
	AddItemWithParam("Rolled Over", 10, unlockAchievement, "Do 5 car rolls in a row from one crash.");
	AddItemWithParam("Walk Free", 11, unlockAchievement, "Lose a 4 star wanted rating by outrunning the cops.");
	AddItemWithParam("Courier Service", 12, unlockAchievement, "Complete all 10 package delivery jobs.");
	AddItemWithParam("Retail Therapy", 13, unlockAchievement, "Unlock the special ability of buying guns from a friend.");
	AddItemWithParam("Chain Reaction", 14, unlockAchievement, "You must blow up 10 vehicles in 10 seconds.");
	AddItemWithParam("One Man Army", 15, unlockAchievement, "Survive 5 minutes on 6 star wanted level.");
	AddItemWithParam("Lowest Point", 16, unlockAchievement, "Complete mission 'Roman's Sorrow'.");
	AddItemWithParam("Order Fulfilled", 17, unlockAchievement, "Complete all 10 Exotic Export orders.");
	AddItemWithParam("Manhunt", 18, unlockAchievement, "Complete the most wanted side missions from the police computer.");
	AddItemWithParam("Cleaned The Mean Streets", 19, unlockAchievement, "Capture 20 criminals through the police computer.");
	AddItemWithParam("Fed The Fish", 20, unlockAchievement, "Complete the mission 'Uncle Vlad'.");
	AddItemWithParam("It'll Cost Ya", 21, unlockAchievement, "Complete a taxi ride without skipping from one island to another.");
	AddItemWithParam("Sightseer", 22, unlockAchievement, "Fly on all helicopter tours of Liberty City.");
	AddItemWithParam("Warm Coffee", 23, unlockAchievement, "Successfully date a girl to be invited into her house.");
	AddItemWithParam("That's How We Roll!", 24, unlockAchievement, "Unlock the special ability of helicopter.");
	AddItemWithParam("Half Million", 25, unlockAchievement, "Reach a balance of $500,000.");
	AddItemWithParam("Impossible Trinity", 26, unlockAchievement, "Complete mission 'Museum Piece'.");
	AddItemWithParam("Full Exploration", 27, unlockAchievement, "Unlock all the islands.");
	AddItemWithParam("You Got The Message", 28, unlockAchievement, "Deliver all 30 cars ordered through text message.");
	AddItemWithParam("Dare Devil", 29, unlockAchievement, "Complete 100% of the unique stunt jumps.");
	AddItemWithParam("Assassin's Greed", 30, unlockAchievement, "Complete all 9 assassin missions.");
	AddItemWithParam("Endangered Species", 31, unlockAchievement, "Collect every hidden package in the game.");
	AddItemWithParam("Under The Radar", 32, unlockAchievement, "Fly underneath the main bridges in the game that cross water with a helicopter.");
	AddItemWithParam("Dial B For Bomb", 33, unlockAchievement, "Unlock the special ability of phoning for a bomb to be placed.");
	AddItemWithParam("Gracefully Taken", 34, unlockAchievement, "Complete mission 'I'll Take Her'.");
	AddItemWithParam("Liberty City (5)", 35, unlockAchievement, "After meeting all possible friends, the ones left alive all like you above 90%.");
	AddItemWithParam("No More Strangers", 36, unlockAchievement, "Meet all random characters.");
	AddItemWithParam("That Special Someone", 37, unlockAchievement, "Complete mission 'That Special Someone'.");
	AddItemWithParam("You Won!", 38, unlockAchievement, "Complete the final mission.");
	AddItemWithParam("Liberty City Minute", 39, unlockAchievement, "Complete the story missions in less than 30 hours.");
	AddItemWithParam("Key To The City", 40, unlockAchievement, "Achieve 100% in 'Game progress' statistic.");
	AddItemWithParam("Teamplayer", 41, unlockAchievement, "Kill 5 players who are not in your team, in any ranked multiplayer team game.");
	AddItemWithParam("Cut Your Teeth", 42, unlockAchievement, "Earn a personal rank promotion in multiplayer.");
	AddItemWithParam("Join The Midnight Club", 43, unlockAchievement, "Win a ranked multiplayer race without damaging your vehicle too much and with damage enabled.");
	AddItemWithParam("Fly The Co-op", 44, unlockAchievement, "Beat our time in ranked versions of 'Deal Breaker', 'Hangman's NOOSE' and 'Bomb da Base II'.");
	AddItemWithParam("Taking It For The Team", 45, unlockAchievement, "Be on the winning team in all ranked multiplayer team games.");
	AddItemWithParam("Top Of The Food Chain", 46, unlockAchievement, "Kill 20 players with a pistol in a ranked multiplayer deathmatch.");
	AddItemWithParam("Top The Midnight Club", 47, unlockAchievement, "Come first in 20 different ranked standard multiplayer races.");
	AddItemWithParam("Wanted", 48, unlockAchievement, "Achieve the highest personal rank in multiplayer.");
	AddItemWithParam("Auf Wiedersehen Petrovic", 49, unlockAchievement, "Win all ranked multiplayer variations, all races and 'Cops 'n Crooks', as both sides.");
	AddItemWithParam("Let Sleeping Rockstars Lie", 50, unlockAchievement, "Kill a Rockstar developer in a ranked multiplayer match.");
	AddItemWithParam("Key To The City", 51, unlockAchievement, "Achieve 100% in the 'Game Progress' statistic.");
	AddItemWithParam("The Ballad of Gay Tony", 52, unlockAchievement, "Complete the Ballad of Gay Tony storyline.");
	AddItemWithParam("The Lost and Damned", 53, unlockAchievement, "Complete The Lost and Damned storyline.");
	//NOTE: Perhaps the episodic achievements start at 0 on their respective episodes?
	AddItemWithParam("No More Strangers", 54, unlockAchievement, "Meet all random characters in The Ballad of Gay Tony.");
	AddItemWithParam("That's How We Roll!", 55, unlockAchievement, "Unlock the special ability of the helicopter in The Ballad of Gay Tony.");
	AddItemWithParam("Liberty City Minute", 56, unlockAchievement, "Complete all story missions in less than 30 hours.");
	AddItemWithParam("Impossible Trinity", 57, unlockAchievement, "Complete the mission 'Museum Piece'.");
	AddItemWithParam("Cleaned the Mean Streets", 58, unlockAchievement, "Capture 20 criminals via the police computer in The Ballad of Gay Tony.");
	AddItemWithParam("Endangered Species", 59, unlockAchievement, "Collect all 200 pigeons in The Ballad of Gay Tony.");
	AddItemWithParam("Sightseer", 60, unlockAchievement, "Complete all helicopter tours in Liberty City.");
	AddItemWithParam("Retail Therapy", 61, unlockAchievement, "Unlock the special ability to buy weapons from a friend in The Ballad of Gay Tony.");
	AddItemWithParam("Wheelie Rider", 62, unlockAchievement, "Perform a wheelie lasting at least 500 feet on a motorbike in The Ballad of Gay Tony.");
	AddItemWithParam("That Special Someone", 63, unlockAchievement, "Complete the mission 'That Special Someone' in The Ballad of Gay Tony.");
	AddItemWithParam("No More Strangers", 64, unlockAchievement, "Meet all random characters in The Ballad of Gay Tony.");
	AddItemWithParam("It'll Cost You", 65, unlockAchievement, "Complete a taxi ride without skipping from one island to another in The Ballad of Gay Tony.");
}

void clearBrief()			{	CLEAR_BRIEF();				}
void startCreditsMusic()	{	START_END_CREDITS_MUSIC();	}
void stopCreditsMusic()		{	STOP_END_CREDITS_MUSIC();	}


void resetCam()
{
	SET_CAM_ACTIVE(scriptedCam, false);
	SET_CAM_PROPAGATE(scriptedCam, false);
	ACTIVATE_SCRIPTED_CAMS(false, false);
	UNATTACH_CAM(scriptedCam);
	DESTROY_CAM(scriptedCam);
	while (DOES_CAM_EXIST(scriptedCam))
		WAIT(0);
	scriptedCam = INVALID_HANDLE;
}


void MissileConfigPage()
{
	SetHeader("Military Missile Configuration");
	AddItemChar("Missile Chassis", false, ctlMem->StormMissileVars.MissileChassisStrings, 3, &ctlMem->StormMissileVars.selectedMissileChassis, nullptr, nullptr);
	AddItemChar("Explosion Type", false, ctlMem->StormMissileVars.WarheadTypeStrings, 4, &ctlMem->StormMissileVars.selectedExplosionType, nullptr, nullptr);
	AddItemFloat("Climb Time", false, 0.0f, 20.0f, &ctlMem->StormMissileVars.missile_climb_time, 0.1f, 2, nullptr, "Useful for Javelin Effect");
	AddItemFloat("Burn Time", false, 1.0f, 60.0f, &ctlMem->StormMissileVars.missile_burn_time, 0.1f, 2, nullptr, "Sets the rockets fuel");
	AddItemFloat("Proximity Fuse", false, 0.0f, 10.0f, &ctlMem->StormMissileVars.missile_proximity_fuse,0.1f, 2, nullptr, "Distance for detonation");
	AddItemFloat("Velocity", false, 3.0f, 50.0f, &ctlMem->StormMissileVars.missile_velocity, 0.1f, 2, nullptr, "Sets the missile cruise speed");
	AddItemBool("Flame Trail", &ctlMem->StormMissileVars.bullshit2, ctlMem->StormMissileVars.missile_flame_trail, nullptr, "Adds fire effect");
	AddItemBoolAlt("Equip with cam", &ctlMem->StormMissileVars.bullshit, ctlMem->StormMissileVars.missile_equip_with_cam, nullptr, "~BUTTON_X~ to reset cam", resetCam, nullptr);
	AddItemFloat("Set cam offset", false, -10.0f, 10.0f, &ctlMem->StormMissileVars.missile_cam_offset, 0.1f, 2, nullptr, nullptr);
	AddItemChar("Missile Link Mode", false, ctlMem->StormMissileVars.LinkModeStrings, 3, &ctlMem->StormMissileVars.selectedLinkMode, nullptr, "Drasticly improves connectivity to missile(recommended)");
}

void MilitaryMissileOptions()
{
	SetHeader("Military Missiles - by xStormsnoutx");
	AddItemMenu("Missile Config", MissileConfigPage, nullptr);
	AddItem("Prefill Target: Nearby Player", MissilePrefillTarget, "Tries to prefill target with nearest online players car");
	AddItemWithParam("Give Heatseekers", MISSILE_HEATSEEKER, CreateAndGiveMilitaryMissile, nullptr);
	AddItemWithParam("Give Homing Missile", MISSILE_HOMING, CreateAndGiveMilitaryMissile, nullptr);
	AddItemWithParam("Give TV Missile", MISSILE_TV, CreateAndGiveMilitaryMissile, "~PAD_LT~ = Slow ~PAD_RT~ = Fast ~PAD_Y~ = Explode");
	AddItemWithParam("Give Bomb", MISSILE_BOMB, CreateAndGiveMilitaryMissile, nullptr);
}

void gameOptions()
{
	SetHeader("Game/misc");
	AddItemMenu("Military Missiles", MilitaryMissileOptions, "Made by xStormsnoutx ~BLIP_71~");
	AddItemMenu("Memory Peek & Poke", MemoryOptions, "If it's not .data, you will crash on retail.");
	AddItemMenu("Achievement unlocker", achievementUnlockOptions, "Unlock selected or all achievements.");
	AddItemAlt("GTA IV (End credits music)", startCreditsMusic, "~PAD_X~ = Stop.", stopCreditsMusic, nullptr);
	AddItem("Clear brief", clearBrief, "Text in pause menu brief.");
}