#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"
#include "protections.h"
#include "display.h"
#include "cartel_chat.h"
#include "Functions.h"
#include "network_loops.h"
#include "vehicle_loops.h"
#include "../Menu/Functions.h"
#include "../Menu/Admin_Options.h"
#include "local_loops.h"

ctl_memory_sco *ctlMem;
stickyFreeMemory *freeMem;

PLAYER_INDEX myPlayerIndex;
PED_INDEX myPlayerPed;
int myCurrentEpisode;
bool NetworkGame;
bool CartelXEXPresent;

extern uint displayLogoTick;

void main(uint cartel_memory_address)
{
	THIS_SCRIPT_IS_SAFE_FOR_NETWORK_GAME();
	freeMem = (stickyFreeMemory*)freeMemStartAddress;
	ctlMem = (ctl_memory_sco*)cartel_memory_address; //be careful here, compile time is done immediately before starting, other things won't be.
	REQUEST_STREAMED_TXD("cartel", false);
	WAIT(250); //allow security flags to be set etc by Cartel..
	//WAIT(400);

	NetworkGame = IS_NETWORK_SESSION();
	myCurrentEpisode = GET_CURRENT_EPISODE();
	CartelXEXPresent = bitTest(*(int*)stickyBitSet, STICK_XEX_PRESENT);

	if (NetworkGame)
		REQUEST_MODEL(PED_M_Y_GMAF_HI_02); //we switch to this model for detaching objects

	char* time_stamp_buffer = BUFFER64;
	sprintf(time_stamp_buffer, "Cartel Helper Compiled %s at %s.", __DATE__, __TIME__);
	ADD_TO_PREVIOUS_BRIEF(time_stamp_buffer);

	char* compileTime_date_hour_buff = BUFFER16;
	sprintf(compileTime_date_hour_buff, "%s%s", __DATE__, GET_STRING_FROM_STRING(__TIME__, 0, 2)); //used for version hashing.
	if (ctlMem->m_CartelMenuCompileDateHourHash != GET_HASH_KEY(compileTime_date_hour_buff))
	{
		PRINT_STRING_WITH_TWO_LITERAL_STRINGS("STRTNM3", "~r~CTL: ~w~Cartel.sco and cartel_helper.sco's compiled date/hour do not match", "this could mean these versions are incompatible, waiting 10 seconds to start cartel_helper.sco..." , 10000, 1);
		WAIT(10000);
		printf("~r~CTL: ~w~Helper starting.");
		WAIT(200);
	}
	if (ctlMem->m_commonStructsCombinedSize != (sizeof(ctl_memory_sco) + sizeof(stickyFreeMemory)))
	{
		PRINT_STRING_WITH_TWO_LITERAL_STRINGS("STRTNM3", "~r~CTL: ~w~Cartel.sco and cartel_helper.sco's memory allocation size does not match", "this increases the likelihood these versions are incompatible with each-other, waiting 10 seconds", 10000, 1);
		WAIT(10000);
		printf("~r~CTL: ~w~Helper starting.");
		WAIT(200);
	}
	GENERATE_RANDOM_INT_IN_RANGE(1000, 1500, &money_amount_ped_prot_marker);
	displayLogoTick = (GetTickCount() + 4750);
	while (true)
	{
		WAIT(0);
		draw_intro_logo_loop();
#ifdef Developer
		if (bitTest(ctlMem->m_bit_sets.m_debug, DEBUG_IS_HELPER_LOADED))
		{
			bitReset(&ctlMem->m_bit_sets.m_debug, DEBUG_IS_HELPER_LOADED);
			printf("~r~CTL HELPER: ~w~I've just read the bit you set, and am actively executing!");
		}
#endif
		myPlayerIndex = GET_PLAYER_ID();
		GET_PLAYER_CHAR(myPlayerIndex, &myPlayerPed);
		if (NetworkGame)
		{
			if (bitTest(ctlMem->m_bit_sets.m_network, NETWORK_REGISTERED_BROADCAST_VARS_CALLED)) //this doesn't get detected right away so don't do any crashing etc
			{//bit only gets set on network.
				cartel_chat_display();
				//bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_LOOP_CALLED) ? cartel_protections() : UNPAUSE_GAME(); //not set fast enough for protections
				!bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_LOOP_CALLED) ? frame_rate_counter() : cartel_protections();
				bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_LOOP_CALLED) ? cartel_network_loops() : GET_PLAYER_ID(); //ideally use for something else down the line but eh
				localLoops();
			}
			else
				DEACTIVATE_FRONTEND(); //will block user inputs combined with loop DisableUnusedInputs in cartel.sco network loop

			frame_rate_counter(); //sorts the string for network display now so it's only sprintf'd when required.
			cartel_network_display(); //don't make it obvious :D, always run this regardless.
		}
		else
		{
			cartel_network_loops(); //calculates indexes to loop through dependant on is network session. drive/walk on water, walk on air etc
			localLoops();
		}

		cartel_vehicle_loops();
	}
}
