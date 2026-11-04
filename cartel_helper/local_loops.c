#include "../Menu/stdafx.h" //developer defines etc (MAKE SURE TO RELOAD THIS SOLUTION/VS ONCE MADE CHANGES)
#include "../Menu/Functions.h"
#include "cartel_helper.h"

void localLoops(void)
{
	if (bitTest(ctlMem->m_bit_sets.m_player, drunkMode) && !IS_PED_RAGDOLL(myPlayerPed)) //NOT TESTED
		SET_DRUNK_CAM(myPlayerPed, 0.00010000, 1000);


	if (bitTest(ctlMem->m_bit_sets.m_player, neverWanted)) //can remove this to use the net loop //ctlMem->m_bit_sets.m_neverWanted_vehAutoFix
	{
		//START_FIRING_AMNESTY();
		if (IS_WANTED_LEVEL_GREATER(myPlayerIndex, 0)) //stop event spam R07 has reported MD doing, the cause is looping clear wanted level constantly.
			CLEAR_WANTED_LEVEL(myPlayerIndex);
	}

	//should create a weaponBitSet and but unlimitedClip and Ammo next to eachother so i can use GET_BITS_IN_RANGE instead to make this better.
	if (IS_CHAR_SHOOTING(myPlayerPed))
	{
		WEAPON_TYPE ourCurrent;
		GET_CURRENT_CHAR_WEAPON(myPlayerPed, (WEAPON_TYPE*)&ourCurrent);

		if (bitTest(ctlMem->m_bit_sets.m_weapons, unlimitedClip))
		{
			if (ourCurrent == WEAPON_GRENADE || ourCurrent == WEAPON_MOLOTOV)
				GIVE_WEAPON_TO_CHAR(myPlayerPed, ourCurrent, AMMO_MAX, false);
			else
			{
				int maxAmmo;
				GET_MAX_AMMO_IN_CLIP(myPlayerPed, ourCurrent, &maxAmmo);
				SET_AMMO_IN_CLIP(myPlayerPed, ourCurrent, maxAmmo);
			}
		}

		if (bitTest(ctlMem->m_bit_sets.m_weapons, unlimitedAmmo))
		{
			int ourMaxAmmo, ourCurrentAmmo;
			GET_AMMO_IN_CHAR_WEAPON(myPlayerPed, ourCurrent, &ourCurrentAmmo);
			if (ourCurrentAmmo < 20)
			{
				GET_MAX_AMMO(myPlayerPed, ourCurrent, &ourMaxAmmo);
				ADD_AMMO_TO_CHAR(myPlayerPed, ourCurrent, (ourMaxAmmo - ourCurrentAmmo));
			}
		}
	}

	if (!IS_THREAD_ACTIVE(freeMem->m_Cartel_ThreadId))
	{
		//DeleteAllSpawnedEntities();
		PRINT_STRING_WITH_TWO_LITERAL_STRINGS_NOW("STRTNM3", "~r~CTL: ~y~Cartel.sco ~w~has ~r~terminated, ~w~oh no! Where is barbara with big titties!?", "Please make note of what you were doing when this happened so we have a chance to fix.", 15000, 1);
		WAIT(15000);
		printf("~r~CTL: ~w~Terminating myself to avoid crashing.");
		TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cartel_memory");
		TERMINATE_THIS_SCRIPT();
	}
	if (!IS_THREAD_ACTIVE(freeMem->m_CartelMemory_ThreadId)) //memory is started before this script (helper)
	{
		DESTROY_THREAD(freeMem->m_Cartel_ThreadId); //this could be named differently so.. //TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("Cartel");
		//DeleteAllSpawnedEntities();
		PRINT_STRING_WITH_TWO_LITERAL_STRINGS_NOW("STRTNM3", "~r~CTL: ~y~cartel_memory.sco ~w~has ~r~terminated, ~w~oh no! WTF DID YOU DO!?", "Please make note of what you were doing when this happened so we have a chance to fix.", 15000, 1);
		WAIT(15000);
		printf("~r~CTL: ~w~Terminating myself to avoid crashing.");
		TERMINATE_THIS_SCRIPT();
	}
	//Thread check for memory.sco can just be in helper as both scripts watch out for eachother, if either menu or helper terminates, memory will terminate after which the remaining script will also terminate.
}