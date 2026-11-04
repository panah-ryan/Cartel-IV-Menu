#include "stdafx.h"
#include "Functions.h"

/*
void Maintenance_Language_check() //put in main void execute once when finished.. //register_thread(Maintenance_Language_check(), GET_HASH_KEY("Language Check Loop"), THREAD_UPDATE_LONG, 0, NULL, false);
{
	if (GET_CURRENT_LANGUAGE() != ctlMem->m_language)
	{
		//0xC9667422 = English
		ctlMem->m_language = GET_CURRENT_LANGUAGE();
		init_net_obj_type_strings();
		init_gamemode_strings();
		init_common_strings_from_gxt();
		init_strings_assembled_from_gxt();
	}
}
*/

void Maintenance_loop()
{
	GET_PLAYER_CHAR(myPlayerIndex, &myPlayerPed); //This holds the whole menu togethor, first thing executed each time we are called/executed!
	if (!IS_THREAD_ACTIVE(freeMem->m_CartelHelper_ThreadId))
	{
		DeleteAllSpawnedEntities();
		PRINT_STRING_WITH_TWO_LITERAL_STRINGS_NOW("STRTNM3", "~r~CTL: ~y~cartel_helper.sco ~w~has ~r~terminated, ~w~oh no! Where is barbara with big titties!?", "Please make note of what you were doing when this happened so we have a chance to fix.", 15000, 1);
		WAIT(15000);
		printf("~r~CTL: ~w~Terminating myself to avoid crashing.");
		TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cartel_memory");
		TERMINATE_THIS_SCRIPT();
	}
	//Thread check for memory.sco can just be in helper as both scripts watch out for eachother, if either menu or helper terminates, memory will terminate after which the remaining script will also terminate.

	if (NETWORK_LEAVE_GAME_PENDING() || DOES_GAME_CODE_WANT_TO_LEAVE_NETWORK_SESSION()) //seems to not trigger //trying both for host
	{
		DeleteAllSpawnedEntities();
		SWITCH_STREAMING(false); //try help rgh with all the HDD writing.
	}

	if (GET_CURRENT_LANGUAGE() != ctlMem->m_language) //Maintenance_Language_check()
	{
		//0xC9667422 = English
		ctlMem->m_language = GET_CURRENT_LANGUAGE();
		init_net_obj_type_strings();
		init_gamemode_strings();
		init_common_strings_from_gxt();
		init_strings_assembled_from_gxt();
	}
}