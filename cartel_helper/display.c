#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"
#include "cartel_helper.h"
#include "../Menu/Network_Options.h"
#include "../Menu/stdafx.h"

char* talking_letters[7] = { "T", "A", "L", "K", "I", "N", "G" };

bool fps_switch;
int fps_grab, fps_pre;

void frame_rate_counter()
{
	if (TIMERB() <= 500)
	{
		if (!fps_switch)
		{
			fps_grab = TIMERB();
			fps_switch = true;
			fps_pre = GET_FRAME_COUNT();
		}
	}
	if (TIMERB() >= 1000 + fps_grab)
	{
		if (fps_switch)
		{
			fps_switch = false;
			ctlMem->m_netDisplay.currentFrameRate = (GET_FRAME_COUNT() - fps_pre);
			//sprintf(ctlMem->m_netDisplay.frameRateString, "FPS: ~s~%i", ctlMem->m_netDisplay.currentFrameRate); //keep in ctlMem incase we want to pull it for protection later.
			itos(&ctlMem->m_netDisplay.frameRateString[8], ctlMem->m_netDisplay.currentFrameRate, 12);
			SETTIMERB(0);
		}
	}
}

Texture CartelLogo_02 = NULL;
bool gotTexture;
uint displayLogoTick;
void draw_intro_logo_loop()
{
	if (displayLogoTick > GetTickCount())
	{
		if (gotTexture)
		{
			DRAW_SPRITE(CartelLogo_02, 0.5f, 0.5f, 0.7f, 1.1f, 0.0f, 255, 255, 255, 150);
		}
		else
		{
			if (HAS_STREAMED_TXD_LOADED("cartel")) //should have by now..
			{
				CartelLogo_02 = GET_TEXTURE_FROM_STREAMED_TXD("cartel", "cartel_02");
				gotTexture = true;
			}
		}
	}
	else
	{
		if (CartelLogo_02 != NULL)
			RELEASE_TEXTURE(CartelLogo_02);
	}
}

/*uint dwLastFrameTimes[64] = { 0 }; //Move this to freemem
uint CalculateFPS()
{
    static uint dwLastFrameTime = GetTickCount();
    static uint dwLastFPSValue = 60;
    static uint dwLastStableValue = 60;
    static uint dwFramesSinceChange = 0;

    uint dwRunningTotal = 0;
    uint dwFrameTime = GetTickCount();

    for (uint i = 0; i < 63; i++) 
    {
        dwLastFrameTimes[i] = dwLastFrameTimes[i + 1];
        dwRunningTotal += dwLastFrameTimes[i];
    }

    dwLastFrameTimes[63] = dwFrameTime - dwLastFrameTime;
    dwRunningTotal += dwLastFrameTimes[31];
    dwLastFrameTime = dwFrameTime;

    if (dwRunningTotal != 0) 
    {
        uint dwAverage = dwRunningTotal / 64;
        if (dwAverage != 0) 
        {
            uint dwFPS = (float)(1000 / dwAverage);
            if (dwLastFPSValue != dwFPS) 
            {
                if (dwLastStableValue != dwLastFPSValue) 
                {
                    dwLastStableValue = dwLastFPSValue;
                    dwFramesSinceChange = 0;
                }

                if (dwFPS != dwLastStableValue)
                    dwFramesSinceChange++;

                if (dwFramesSinceChange > 1) 
                {
                    if (dwLastFPSValue > dwFPS)
                        dwLastFPSValue--;
                    else
                        dwLastFPSValue++;

                    if (dwLastFPSValue > 60)
                        dwLastFPSValue = 60;
                }
            }
        }
    }

    return dwLastFPSValue;
}*/

void SetUpDrawText_NathProtSyle(int r, int g, int b)
{
	SET_TEXT_SCALE(0.2500f, 0.4050f);
	SET_TEXT_COLOUR(r, g, b, 255);
	//SET_TEXT_DROPSHADOW(255, 1, 1, 1, 255);
	if(!bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_DROPSHADOW_LOBBY_INFO))
		SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
}

//TODO, figure out where to draw this when cartel chat is running! eek
//maybe just don't show it if messages are displaying.
void drawTalkingPlayers(int my_R, int my_G, int my_B)
{
	bool someone_is_talking = false;
	float name_pos = 0.7190f;
	int their_R, their_G, their_B;
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i))
			continue;

		if (!NETWORK_IS_PLAYER_TALKING(i) || NETWORK_IS_PLAYER_MUTED_BY_ME(i))
			continue;

		GET_PLAYER_RGB_COLOUR(i, &their_R, &their_G, &their_B);
		SetUpDrawText_NathProtSyle(their_R, their_G, their_B);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.0550f, name_pos, "STRING", SafeNetworkPlayerGetNameWithColour(i));
		name_pos -= 0.025f;
		someone_is_talking = true;
	}

	if (someone_is_talking)
	{
		float talking_text_pos = 0.7190f;
		for (int i = 6; i > -1; i--)
		{
			SetUpDrawText_NathProtSyle(my_R, my_G, my_B);
			SET_TEXT_CENTRE(true); //makes it go left wtf
			//DISPLAY_TEXT_WITH_LITERAL_STRING(0.0440f, talking_text_pos, "STRING", &letters[i]); //was 0425.0f
			DISPLAY_TEXT_WITH_LITERAL_STRING(0.0440f, talking_text_pos, "STRING", talking_letters[i]);
			talking_text_pos -= 0.025f;
		}
	}
}

uint GetNumber360Players()
{
	uint count = 0;
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i) || IsPlayerUsingXboxOne(i))
			continue;
		count++;
	}
	return count;
}

void Update_playercount_related_strings() //this gets called whenever someone leaves/joins as we already detect that shit for protection timers.
{
	int Xbox_360_Count = GetNumber360Players();
	if (Xbox_360_Count > 1) //includes self.
		sprintf(ctlMem->m_netDisplay.emptySlots_360UserCountString, "360 Users: ~w~%i", Xbox_360_Count);
	else
		sprintf(ctlMem->m_netDisplay.emptySlots_360UserCountString, "Empty slots: ~w~%i", (NETWORK_GET_MAX_SLOTS() - GET_NUMBER_OF_PLAYERS()));

	sprintf(ctlMem->m_netDisplay.playerCountString, "%s: ~w~%i", ctlMem->m_commonStrings[GXT_STRING_PLAYERS], GET_NUMBER_OF_PLAYERS());

	PRINTSTRING("Updated 360 users/Empty slots & player count (");
	PRINTINT(GET_NUMBER_OF_PLAYERS());
	PRINTSTRING(") string.\n");
}

void drawGameInfoBar(int my_R, int my_G, int my_B)
{
	//"Host: "
	sprintf(ctlMem->m_netDisplay.hostName, "%s: %s", ctlMem->m_commonStrings[GXT_STRING_HOST], (GET_HOST_ID() == -1) ? "~s~Changing..." : SafeNetworkPlayerGetNameWithColour(GET_HOST_ID())); //this will run before protections for a few seconds..
	
	SetUpDrawText_NathProtSyle(my_R, my_G, my_B);
	DISPLAY_TEXT_WITH_LITERAL_STRING(0.0550f, 0.9550f, "STRING", ctlMem->m_netDisplay.hostName);

	USE_PREVIOUS_FONT_SETTINGS();
	DISPLAY_TEXT_WITH_LITERAL_STRING(0.2800f, 0.9550f, "STRING", ctlMem->m_netDisplay.frameRateString);
	
	USE_PREVIOUS_FONT_SETTINGS();
	DISPLAY_TEXT_WITH_LITERAL_STRING(0.4350f, 0.9550f, "STRING", ctlMem->m_netDisplay.emptySlots_360UserCountString); //was x 0.4300f

	USE_PREVIOUS_FONT_SETTINGS();
	DISPLAY_TEXT_WITH_LITERAL_STRING(0.6300f, 0.9550f, "STRING", ctlMem->m_netDisplay.playerCountString);

	int num_modding = 0;
	for (int i = 0; i < 16; i++)
	{
		if (!IsPlayerValid(i)) //If a player goes from a lobby with active players/modders in slots, to another lobby, we got a problem (XEX only clears at start of _AddPlayerDetour) .. This avoids Stagnant data potential
			continue;
#ifdef Moderator
		if (IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i) || IS_BIT_SET(ctlMem->m_bit_sets.m_admin, (i + 16))) //admin + 16 = picked up by admin scan.
#else
		if (IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, i))
#endif
			num_modding = (num_modding + 1);
	}
	//sprintf(ctlMem->m_netDisplay.moddingString, "Modding: ~s~%i", num_modding);
	itos(&ctlMem->m_netDisplay.moddingString[12], num_modding, 17); //more efficient unless translated. //verified 2 less assembly instructions this way and less shit to copy in.

	USE_PREVIOUS_FONT_SETTINGS();
	DISPLAY_TEXT_WITH_LITERAL_STRING(0.8300f, 0.9550f, "STRING", ctlMem->m_netDisplay.moddingString);
}


void cartel_network_display() //only called if(NetworkGame)
{
#pragma region joining_ghosts
	if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_TOP_DRAW_JOINING_GHOSTS_PLAYERS))
	{
		//printf("Ye helper is doing draws right now.");
		int length = 0;
		bool first = true;
		for (int i = 0; i < 16; i++)
		{
			if (!IsPlayerValid(i) || IS_NETWORK_PLAYER_ACTIVE(i))
				continue; //not present or active, not a ghost / joining

			length += (GET_LENGTH_OF_LITERAL_STRING(SafeNetworkPlayerGetNameWithColour(i)) + 10 + (i > 9 ? 2 : 1)); //additional character for double digit indexes aka ~COL_NET_XX~
			if (length > 96)
				break; //no more room for buffer.

			if (first)
			{
				strcpy(ctlMem->m_char_buffers.joining_players_bar_buffer, SafeNetworkPlayerGetNameWithColour(i), 96);
				stradd(ctlMem->m_char_buffers.joining_players_bar_buffer, " ", 96);
				first = false;
			}
			else
			{
				stradd(ctlMem->m_char_buffers.joining_players_bar_buffer, SafeNetworkPlayerGetNameWithColour(i), 96);
				if ((length + 1) < 96)
				{
					stradd(ctlMem->m_char_buffers.joining_players_bar_buffer, " ", 96);
					length += 1;
				}
			}
		}

		if (length > 0) //there's joining players/ghosts
		{
			DRAW_CURVED_WINDOW(0.2000f, 0.0150f, 0.6000f, 0.0400f, 180);
			SET_TEXT_SCALE(0.2500f, 0.4050f);
			SET_TEXT_DROPSHADOW(false, 0, 0, 0, 0);
			DISPLAY_TEXT_WITH_LITERAL_STRING(0.2060f, 0.0220f, "STRING", "~r~CTL ~w~Joining/Ghosts");
			SET_TEXT_SCALE(0.2500f, 0.4050f);
			SET_TEXT_USE_UNDERSCORE(true); //incase people spoof underscores makes for nicer viewing.
			SET_TEXT_DROPSHADOW(false, 0, 0, 0, 0);
			DISPLAY_TEXT_WITH_LITERAL_STRING(0.3420f, 0.0220f, "STRING", ctlMem->m_char_buffers.joining_players_bar_buffer);
		}
	}
#pragma endregion

#pragma region bottom_info_bar 
	//needs implementing //talking is gonna have to move due to text chat feed eek.
	if (GET_BITS_IN_RANGE(ctlMem->m_bit_sets.m_menuPreferences, PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS, PREF_MINIMAP_DRAW_TALKING_PLAYERS))
	{
		int my_R, my_G, my_B;
		GET_PLAYER_RGB_COLOUR(myPlayerIndex, &my_R, &my_G, &my_B);
		if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS))
			drawGameInfoBar(my_R, my_G, my_B);

		if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_MINIMAP_DRAW_TALKING_PLAYERS))
			drawTalkingPlayers(my_R, my_G, my_B);
	}
#pragma endregion
}