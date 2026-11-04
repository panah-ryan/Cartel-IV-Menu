#include "stdafx.h"
#include "MenuUiHandling.h"
#include "MenuExecutionHandling.h"
#include "Utils.h"
#include "Functions.h"
#include "ItemExecution.h"
#include "Network_Options.h"
#include "Player_Options.h"

int CanMoveMenu = 0;
Page Container;

Texture headSet;
Texture uncheckedbox;
Texture checkedbox;

TextureDict webborders;
TextureDict browser;
Texture HeaderTexture;
Texture CursorTexture;

Texture CartelLogo = NULL;

VEHICLE_INDEX displayed_veh = NULL;

#define GetRelativeCursorIndex Container.CursorIndex - Container.ItemStartIndex

void SetupContainer(Page* menu, void(*Entrypoint)())
{
	menu->BitSet = null;
	menu->CursorIndex = 0;
	menu->TotalItemCount = 0;
	menu->ItemStartIndex = 0;
	menu->CurrentMenuLevel = 0;
	menu->Item[0].BitSet = null;
	menu->Ui.HeaderFont = null;
	menu->Ui.TextFont = null;
	menu->Level[0].UpdateMenuLevel = Entrypoint;
	//menu->Ui.MenuName = "Cartel";
	menu->Ui.MenuName = "\0\0\0\0\0\0\0";
	menu->Ui.DrawPos.x = 0.854f;
	menu->Ui.DrawPos.y = 0.1452f;
	menu->Ui.DrawSize.x = 0.21f;
	menu->Ui.DrawSize.y = 0.083f;
	//menu->Ui.HeaderColor = Color(189, 0, 0, 255); //OG Cartel Red
	menu->Ui.HeaderColor = Color(255, 190, 30, 255); //New for textured Header "twatleft" / "twatright" //red and green matches early access discord colour near enough
	menu->Ui.HeaderTextColor = Color(255, 255, 255, 255);
	menu->Ui.SubtitleColor = Color(0, 0, 0, 255);
	menu->Ui.TextColor = Color(255, 255, 255, 255);
	menu->Ui.BackgroundColor = Color(100, 100, 100, 100);
	menu->Ui.SelectedTextColor = Color(0, 0, 0, 255);
	menu->Ui.ScrollBarColor = Color(255, 255, 255, 255);
	menu->Ui.OpenButton1 = BUTTON_RB;
	menu->Ui.OpenButton2 = BUTTON_LB;
	menu->playerId = INVALID_PLAYER_INDEX;
	sprintf(Container.Ui.MenuName, "%s%s%s%s%s%s", ctlMem->m_keyboard.key_mappings[KEY_C][1], ctlMem->m_keyboard.key_mappings[KEY_A][0], ctlMem->m_keyboard.key_mappings[KEY_R][0], ctlMem->m_keyboard.key_mappings[KEY_T][0], ctlMem->m_keyboard.key_mappings[KEY_E][0], ctlMem->m_keyboard.key_mappings[KEY_L][0]);
}

const char* GetCurrentMenuItemText()
{
	return Container.Item[GetRelativeCursorIndex].Ui.ItemText;
}

const char* GetCurrentMenuItemDesc()
{
	return Container.Item[GetRelativeCursorIndex].Ui.Description;
}

void InitMenuDraw()
{
	SetupContainer(&Container, ExecutionEntry);
	//bitSet(&Container.BitSet, PB_DisplayVehiclePreview);
	ExecutionEntry();
}

bool HasPlayerOpenedMenu()
{
	if (IS_BUTTON_PRESSED(0, Container.Ui.OpenButton1))
		return IS_BUTTON_JUST_PRESSED(0, Container.Ui.OpenButton2);

	return false;
}

void ShutDownMenu()
{
	bitReset(&Container.BitSet, PB_IsMenuOpen);
	SET_CINEMATIC_BUTTON_ENABLED(true);
	SET_HIDE_WEAPON_ICON(false); 
	SET_PLAYER_DISABLE_JUMP(myPlayerIndex, false);
	SET_PLAYER_CAN_USE_COVER(myPlayerIndex, true);

	//SET_DISABLE_PLAYER_SHOVE_ANIMATION(ped, false); //TESTING
	//DISPLAY_AMMO(true); //doesn't work //maybe try running before SET_HIDE_WEAPON_ICON

	BLOCK_PED_WEAPON_SWITCHING(myPlayerPed, false); //think this will always exist but whatever
	DESTROY_MOBILE_PHONE();
	char* cellphone = NetworkGame ? "mpcellphone" : "spcellphone";
	REQUEST_SCRIPT(cellphone);

	while (!HAS_SCRIPT_LOADED(cellphone))
		WAIT(0);

	START_NEW_SCRIPT(cellphone, 1024);
	MARK_SCRIPT_AS_NO_LONGER_NEEDED(cellphone);
	DESTROY_MOBILE_PHONE();

	RELEASE_TEXTURE(headSet);
	RELEASE_TEXTURE(uncheckedbox);
	RELEASE_TEXTURE(checkedbox);

	RELEASE_TEXTURE(CursorTexture);

	//if(bitTest(ctlMem->m_bit_sets.m_debug, DEBUG_OBTAINED_CARTEL_LOGO))
		//RELEASE_TEXTURE(CartelLogo); //don't release this as causes flashing. (we only set it once).

	//RELEASE_TEXTURE(HeaderTexture); //causes flashing when menu closed/opened fast (PING been busting my balls)
}

void OpenMenuControls()
{
	if (HasPlayerOpenedMenu())
	{
		if (bitTest(Container.BitSet, PB_IsMenuOpen))
			ShutDownMenu();
		else
		{
			bitSet(&Container.BitSet, PB_IsMenuOpen);
			PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
		}
	}
}

void UpdateMenuLevel(int directionandamount, void* param, void(*customExecute)(void*, void*))
{
	//printf("directionandamount == %i, param == %i", directionandamount, (int*)param);
	if (directionandamount > 0 && directionandamount <= MaxMenuLevels)
	{
		bitSet(&Container.BitSet, PB_LastMenuDirection);
		Container.Level[Container.CurrentMenuLevel].CursorIndex = Container.CursorIndex;
		Container.Level[Container.CurrentMenuLevel].ItemStartIndex = Container.ItemStartIndex;
		Container.CurrentMenuLevel++;

		if (Container.CurrentMenuLevel < MaxMenuLevels)
		{
			Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel =
   				customExecute != nullptr ? customExecute : Container.Item[GetRelativeCursorIndex].Execute;

			Container.ItemStartIndex = 0;
			Container.CursorIndex = 0;

			if (Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel != nullptr)
				Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel(param, nullptr);
		}

		UpdateMenuLevel(directionandamount - 1, param, customExecute);
	}
	else if (directionandamount < 0 && directionandamount >= -MaxMenuLevels)
	{
		bitReset(&Container.BitSet, PB_LastMenuDirection);

		if (Container.CurrentMenuLevel == 0)
		{
			ShutDownMenu();
			return;
		}
		else
		{
			Container.CurrentMenuLevel--;

			if (Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel != nullptr)
			{
				Container.Level[Container.CurrentMenuLevel + 1] = (MenuLevel) { 0 };
				Container.ItemStartIndex = 0;
				Container.CursorIndex = 0;

				if (Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel != nullptr)
					Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel(param, nullptr);

				if (!bitTest(Container.BitSet, PB_IsCurrentMenuInvalid) && Container.ItemStartIndex + Container.CursorIndex < Container.TotalItemCount)
				{
					Container.ItemStartIndex = Container.Level[Container.CurrentMenuLevel].ItemStartIndex;
					Container.CursorIndex = Container.Level[Container.CurrentMenuLevel].CursorIndex;
				}
			}
		}

		UpdateMenuLevel(directionandamount + 1, param, nullptr);
	}
	else if (directionandamount == 0)
	{
		if (Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel != nullptr)
			Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel(param, nullptr);
	}
}

void DisableUnusedInputs()
{
	REQUEST_STREAMED_TXD("network", false);

	WEAPON_TYPE currentWep;
	SET_PLAYER_DISABLE_JUMP(myPlayerIndex, true);
	FORCE_PED_TO_LOAD_COVER(myPlayerPed, false); //try to cancel it out //seems to half work atleast, worth keeping.
	SET_PLAYER_CAN_USE_COVER(myPlayerIndex, false); //maybe only while in when menu is open?

	//should just for loop through weapon slots and if it's non-melee use it.
	GET_CURRENT_CHAR_WEAPON(myPlayerPed, &currentWep);

	if (currentWep <= WEAPON_GRENADE || currentWep == WEAPON_EPISODIC_8 || currentWep == WEAPON_EPISODIC_16) 
	{
		if (HAS_CHAR_GOT_WEAPON(myPlayerPed, WEAPON_PISTOL))
			SET_CURRENT_CHAR_WEAPON(myPlayerPed, WEAPON_PISTOL, true);

		else if (HAS_CHAR_GOT_WEAPON(myPlayerPed, WEAPON_DEAGLE))
			SET_CURRENT_CHAR_WEAPON(myPlayerPed, WEAPON_DEAGLE, true);
	}

	BLOCK_PED_WEAPON_SWITCHING(myPlayerPed, true);
	TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(NetworkGame ? "mpcellphone" : "spcellphone");
	SET_HIDE_WEAPON_ICON(true);  //trying after weapon stuff to see if ammo count disappears.

	if (myCurrentEpisode == EPISODE_ORIGINAL)
		setGlobalAtIndex(482, 6);
	else if (myCurrentEpisode == EPISODE_TLAD)
		setGlobalAtIndex(485, 6);
	else
		setGlobalAtIndex(1111, 6);

	SET_CINEMATIC_BUTTON_ENABLED(false);
	CREATE_MOBILE_PHONE(2);

	//HeaderTexture = GET_TEXTURE(webborders, "twatright");
	CursorTexture = GET_TEXTURE(browser, "topbarpattern");

	while (!HAS_STREAMED_TXD_LOADED("network"))
		WAIT(0);

	headSet = GET_TEXTURE_FROM_STREAMED_TXD("network", "icon_w_headset_on1");
	uncheckedbox = GET_TEXTURE_FROM_STREAMED_TXD("network", "icon_w_notconnected");
	checkedbox = GET_TEXTURE_FROM_STREAMED_TXD("network", "icon_w_tasks_completed");

	REQUEST_STREAMED_TXD("cartel", false);

	if (CartelLogo == NULL && !bitTest(ctlMem->m_bit_sets.m_debug, DEBUG_CARTEL_LOGO_NOT_PRESENT)) //don't keep trying once we've already determined we have/don't have it //we don't release this texture cuz it causes flashing.
	{
		HeaderTexture = GET_TEXTURE(webborders, "twatright"); //we don't unload this due to flashing 
		uint loadTimer = (GetTickCount() + 3000);
		while (loadTimer > GetTickCount()) //only try for 3 seconds.
		{
			if (HAS_STREAMED_TXD_LOADED("cartel")) //we add this to the script_network.img so the user might not necessarily have it.
			{
				CartelLogo = GET_TEXTURE_FROM_STREAMED_TXD("cartel", "cartel_01");
				break;
			}
			WAIT(0);
		}
		if (CartelLogo == NULL)
		{
			printf("~r~CTL: ~w~Cartel's logo not found, drawing text instead.");
			bitSet(&ctlMem->m_bit_sets.m_debug, DEBUG_CARTEL_LOGO_NOT_PRESENT);
		}
	}
	//while (!HAS_STREAMED_TXD_LOADED("cartel"))
		//WAIT(0);

}

bool IsAutoScrollActive(int* HoldCounter, int ScrollInput)
{
	bool IsControlPressed = IS_BUTTON_PRESSED(0, ScrollInput);

	if (IsControlPressed)
		(*HoldCounter)++;
	else
	{
		*HoldCounter = 0;
		return false;
	}

	if (*HoldCounter <= 30)
	{
		if (*HoldCounter < 10)
			return false;

		switch (*HoldCounter)
		{
		case 10:
		case 15:
		case 20:
		case 24:
		case 27:
		case 29:
			return IsControlPressed;
		default:
			return false;
		}
	}

	return IsControlPressed;
}

void ParseMenuControls()
{
	static int HoldUpCounter = 0, HoldDownCounter = 0, HoldLeftCounter = 0, HoldRightCounter = 0;
	static char* SavedDescription = nullstr;
	ItemContainer* CurrentItem = &Container.Item[GetRelativeCursorIndex];
	int leftx, lefty, rightx, righty;

	if (CanMoveMenu)
	{
		GET_POSITION_OF_ANALOGUE_STICKS(0, &leftx, &lefty, &rightx, &righty);
		lefty = ~lefty + 1;

		if (leftx)
			Container.Ui.DrawPos.x += (leftx * 0.00005f);

		if (lefty)
			Container.Ui.DrawPos.y -= (lefty * 0.00005f);

		if (Container.Ui.DrawPos.x < 0.0f)
			Container.Ui.DrawPos.x = 0.0f;

		if (Container.Ui.DrawPos.y < 0.0f)
			Container.Ui.DrawPos.y = 0.0f;

		if (Container.Ui.DrawPos.x > 1.0f)
			Container.Ui.DrawPos.x = 1.0f;

		if (Container.Ui.DrawPos.y > 1.0f)
			Container.Ui.DrawPos.y = 1.0f;
	}

	if (Container.TotalItemCount > 1)
	{
		if (IS_BUTTON_JUST_PRESSED(0, DPAD_UP) || (IsAutoScrollActive(&HoldUpCounter, DPAD_UP)))
		{
			if (Container.CursorIndex <= 0)
			{
				Container.CursorIndex = Container.TotalItemCount - 1;

				if (Container.TotalItemCount > MaxDisplayableItems)
				{
					Container.ItemStartIndex = Container.TotalItemCount - MaxDisplayableItems;
					Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel(nullptr, nullptr);
				}
			}
			else if (Container.TotalItemCount > MaxDisplayableItems && Container.ItemStartIndex != 0 && Container.CursorIndex == Container.ItemStartIndex)
			{
				Container.ItemStartIndex--;
				Container.CursorIndex--;
				Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel(nullptr, nullptr);
			}
			else
				Container.CursorIndex--;

			PLAY_AUDIO_EVENT("RADIO_RETUNE_BEEP");
		}
		if (IS_BUTTON_JUST_PRESSED(0, DPAD_DOWN) || (IsAutoScrollActive(&HoldDownCounter, DPAD_DOWN)))
		{
			if (Container.CursorIndex >= Container.TotalItemCount - 1)
			{
				Container.CursorIndex = 0;

				if (Container.TotalItemCount > MaxDisplayableItems)
				{
					Container.ItemStartIndex = 0;
					Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel(nullptr, nullptr);
				}
			}
			else if (Container.TotalItemCount > MaxDisplayableItems && Container.CursorIndex >= MaxDisplayableItems - 1 && Container.CursorIndex == Container.ItemStartIndex + MaxDisplayableItems - 1)
			{
				Container.CursorIndex++;
				Container.ItemStartIndex = Container.CursorIndex - (MaxDisplayableItems - 1);
				Container.Level[Container.CurrentMenuLevel].UpdateMenuLevel(nullptr, nullptr);
			}
			else
				Container.CursorIndex++;

			PLAY_AUDIO_EVENT("RADIO_RETUNE_BEEP");
		}
	}

	MenuSelectionType CurrentType = CurrentItem->Selection.Type;
	switch (CurrentType)
	{
	case MST_Int:
	case MST_Char:
	case MST_CarColor:
	case MST_Radius:
	{
		if (IS_BUTTON_JUST_PRESSED(0, DPAD_LEFT) || (IsAutoScrollActive(&HoldLeftCounter, DPAD_LEFT)))
		{
			if (*CurrentItem->Selection.Value_pointer.Int <= CurrentItem->Selection.StartIndex.Int)
				*CurrentItem->Selection.Value_pointer.Int = CurrentItem->Selection.EndIndex.Int;
			else
				*CurrentItem->Selection.Value_pointer.Int -= (CurrentType == MST_Char) ? 1 : CurrentItem->Selection.iIncrement;

			if (bitTest(CurrentItem->BitSet, ICB_ExecuteOnChange))
				CurrentItem->Execute(CurrentItem->Selection.Value_pointer.Int, (void*)CurrentItem->value_two.value);
		}
		else if (IS_BUTTON_JUST_PRESSED(0, DPAD_RIGHT) || (IsAutoScrollActive(&HoldRightCounter, DPAD_RIGHT)))
		{
			if (*CurrentItem->Selection.Value_pointer.Int >= CurrentItem->Selection.EndIndex.Int)
				*CurrentItem->Selection.Value_pointer.Int = CurrentItem->Selection.StartIndex.Int;
			else
				*CurrentItem->Selection.Value_pointer.Int += (CurrentType == MST_Char) ? 1 : CurrentItem->Selection.iIncrement;

			if (bitTest(CurrentItem->BitSet, ICB_ExecuteOnChange))
				CurrentItem->Execute(CurrentItem->Selection.Value_pointer.Int, (void*)CurrentItem->value_two.value);
		}
	}
	break;
	case MST_RGB:
	case MST_Byte:
	{
		int value = (*CurrentItem->Selection.Value_pointer.Int >> 24) & 0xFF;
		if (IS_BUTTON_JUST_PRESSED(0, DPAD_LEFT) || (IsAutoScrollActive(&HoldLeftCounter, DPAD_LEFT)))
		{
			if (value <= CurrentItem->Selection.StartIndex.Int)
				value = CurrentItem->Selection.EndIndex.Int;
			else
				value -= CurrentItem->Selection.iIncrement;

			if (bitTest(CurrentItem->BitSet, ICB_ExecuteOnChange))
				CurrentItem->Execute(CurrentItem->Selection.Value_pointer.Byte, (void*)CurrentItem->value_two.value);
		}
		else if (IS_BUTTON_JUST_PRESSED(0, DPAD_RIGHT) || (IsAutoScrollActive(&HoldRightCounter, DPAD_RIGHT)))
		{
			if (value >= CurrentItem->Selection.EndIndex.Int)
				value = CurrentItem->Selection.StartIndex.Int;
			else
				value += CurrentItem->Selection.iIncrement;

			if (bitTest(CurrentItem->BitSet, ICB_ExecuteOnChange))
				CurrentItem->Execute(CurrentItem->Selection.Value_pointer.Byte, (void*)CurrentItem->value_two.value);
		}
		*CurrentItem->Selection.Value_pointer.Byte = value;
	}
	break;
	case MST_Float:
	{
		if (IS_BUTTON_JUST_PRESSED(0, DPAD_LEFT) || (IsAutoScrollActive(&HoldLeftCounter, DPAD_LEFT)))
		{
			if (*CurrentItem->Selection.Value_pointer.Float <= CurrentItem->Selection.StartIndex.Float + 0.000015f)
				*CurrentItem->Selection.Value_pointer.Float = CurrentItem->Selection.EndIndex.Float;
			else
				*CurrentItem->Selection.Value_pointer.Float -= CurrentItem->Selection.fIncrement;

			if (bitTest(CurrentItem->BitSet, ICB_ExecuteOnChange))
				CurrentItem->Execute(CurrentItem->Selection.Value_pointer.Float, (void*)CurrentItem->value_two.value);
		}
		else if (IS_BUTTON_JUST_PRESSED(0, DPAD_RIGHT) || (IsAutoScrollActive(&HoldRightCounter, DPAD_RIGHT)))
		{
			if (*CurrentItem->Selection.Value_pointer.Float >= CurrentItem->Selection.EndIndex.Float)
				*CurrentItem->Selection.Value_pointer.Float = CurrentItem->Selection.StartIndex.Float;
			else
				*CurrentItem->Selection.Value_pointer.Float += CurrentItem->Selection.fIncrement;

			if (bitTest(CurrentItem->BitSet, ICB_ExecuteOnChange))
				CurrentItem->Execute(CurrentItem->Selection.Value_pointer.Float, (void*)CurrentItem->value_two.value);
		}
	}
	break;
	default:
		break;
	}
	if (IS_BUTTON_JUST_PRESSED(0, BUTTON_A))
	{
		switch (CurrentType)
		{
		case MST_Bool:
		{
			bool bit_value = IS_BIT_SET(*CurrentItem->Selection.Value_pointer.Int, CurrentItem->Selection.Value.Int);
			if (!bit_value)
				SET_BIT(CurrentItem->Selection.Value_pointer.Int, CurrentItem->Selection.Value.Int);
			else
				CLEAR_BIT(CurrentItem->Selection.Value_pointer.Int, CurrentItem->Selection.Value.Int);

			if (CurrentItem->Execute != nullptr)
				CurrentItem->Execute((void*)!bit_value, (void*)CurrentItem->value_two.value);

			PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
			return;
		}
		break;
		case MST_Script:
		{
			if (DOES_SCRIPT_EXIST(CurrentItem->Selection.Value_pointer.Char))
			{
				bool script_running = GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(CurrentItem->Selection.Value_pointer.Char);
				if (script_running)
					TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(CurrentItem->Selection.Value_pointer.Char);
				else
				{
					REQUEST_SCRIPT(CurrentItem->Selection.Value_pointer.Char); //crashes without the request and wait. - Nathan
					while (!HAS_SCRIPT_LOADED(CurrentItem->Selection.Value_pointer.Char))
						WAIT(0);

					START_NEW_SCRIPT(CurrentItem->Selection.Value_pointer.Char, CurrentItem->Selection.Value.Int);
					MARK_SCRIPT_AS_NO_LONGER_NEEDED(CurrentItem->Selection.Value_pointer.Char);
				}

				if (CurrentItem->Execute != nullptr)
					CurrentItem->Execute(CurrentItem->Selection.Value_pointer.Int, (void*)CurrentItem->value_two.value);
			}
			else
				printf("~r~CTL: ~w~Script %s.sco was not found!", CurrentItem->Selection.Value_pointer.Char);
			PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
			return;
		}
		break;
		case MST_Menu:
		case MST_MenuParam:
		case MST_Menus:
		{
			void* param = (CurrentItem->Selection.Type == MST_MenuParam ||
				CurrentItem->Selection.Type == MST_Param ||
				CurrentItem->Selection.Type == MST_Menu ||
				CurrentItem->Selection.Type == MST_Menus)
				? (void*)CurrentItem->Selection.Value.Int
				: CurrentItem->Selection.Value_pointer.Int;
			UpdateMenuLevel(1, param, CurrentItem->Execute);
			PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
			return;
		}
		case MST_Player:
			if (IsPlayerValid(CurrentItem->Selection.DynamicId))
			{
				Container.playerId = CurrentItem->Selection.DynamicId;
				UpdateMenuLevel(1, NULL, NULL);
				PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
			}
			return;
		default:
			PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
			break;
		}
		if (CurrentItem->Execute != nullptr)
		{
			void* param = (CurrentType == MST_MenuParam || CurrentType == MST_Param ||
				CurrentType == MST_Menu || CurrentType == MST_Menus)
				? (void*)CurrentItem->Selection.Value.Int
				: CurrentItem->Selection.Value_pointer.Int;

			if (CurrentType == MST_Vehicle)
				CurrentItem->Execute((void*)CurrentItem->Selection.Value.Int, (void*)displayed_veh);
			else
				CurrentItem->Execute(param, (void*)CurrentItem->value_two.value);

			PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
		}
	}
	if (IS_BUTTON_JUST_PRESSED(0, BUTTON_X) && CurrentItem->HasAlternateExecution && CurrentItem->AlternateExecute != nullptr)
	{
		void* param = (CurrentItem->Selection.Type == MST_MenuParam ||
			CurrentItem->Selection.Type == MST_Param ||
			CurrentItem->Selection.Type == MST_Menu ||
			CurrentItem->Selection.Type == MST_Menus)
			? (void*)CurrentItem->Selection.Value.Int
			: CurrentItem->Selection.Value_pointer.Int;

		if (CurrentItem->Selection.Type == MST_Player)
		{
			NETWORK_SHOW_PLAYER_PROFILE_UI(CurrentItem->Selection.DynamicId);
		}
		else if (CurrentItem->Selection.Type == MST_Vehicle)
		{
			CurrentItem->AlternateExecute((void*)CurrentItem->Selection.Value.Int, (void*)displayed_veh);
		}
		else if (CurrentItem->Selection.Type == MST_Menus)
		{
			UpdateMenuLevel(1, param, CurrentItem->AlternateExecute);
			PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
			return;
		}
		else
		{
			CurrentItem->AlternateExecute(param, (void*)CurrentItem->value_two.value);
		}

		PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
	}

	if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y) && CurrentItem->Has2ndAlternateExecute && CurrentItem->AlternateExecute2 != nullptr)
	{
		void* param = (CurrentItem->Selection.Type == MST_MenuParam ||
			CurrentItem->Selection.Type == MST_Param ||
			CurrentItem->Selection.Type == MST_Menu ||
			CurrentItem->Selection.Type == MST_Menus)
			? (void*)CurrentItem->Selection.Value.Int
			: CurrentItem->Selection.Value_pointer.Int;

		if (CurrentItem->Selection.Type == MST_Menus)
		{
			UpdateMenuLevel(1, param, CurrentItem->AlternateExecute2);
		}
		else
		{
			CurrentItem->AlternateExecute2(param, (void*)CurrentItem->value_two.value);
		}

		BlockYVehicleExitJustPressed();
		PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
	}
	if (bitTest(Container.BitSet, PB_IsInNetPlayerMenu))
	{
		if (IS_BUTTON_JUST_PRESSED(0, BUTTON_RB))
		{
			PLAYER_INDEX index = Container.playerId;

			for (int i = 0; i < 16; i++)
			{
				index++;
				if (index == 16)
					index = 0;

				if (IsPlayerValid(index))
					break;
			}

			Container.playerId = index;
			UpdateMenuLevel(0, NULL, NULL);
		}
		else if (IS_BUTTON_JUST_PRESSED(0, BUTTON_LB))
		{
			PLAYER_INDEX index = Container.playerId;

			for (int i = 0; i < 16; i++)
			{
				index--;
				if (index == -1)
					index = 15;

				if (IsPlayerValid(index))
					break;
			}

			Container.playerId = index;
			UpdateMenuLevel(0, NULL, NULL);
		}
	}
	if (IS_BUTTON_JUST_PRESSED(0, BUTTON_B))
	{
		UpdateMenuLevel(-1, NULL, NULL);
		PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_UNREADY");
	}
}

void DrawRect(float x, float y, float width, float height, Color color)
{
	//vector2 pos = convert_pixels_to_screenspace(x, y);
	//vector2 wh = convert_pixels_to_screenspace(width, height);

	DRAW_RECT(x, y, width, height, color.r, color.g, color.b, color.a);
}

void DrawText(const char* text, int font, float x, float y, float scalex, float scaley, Color color, int justify, bool edge)
{
	//vector2 pos = convert_pixels_to_screenspace(x, y);

	SET_TEXT_USE_UNDERSCORE(true);
	edge ? SET_TEXT_EDGE(true, 255, 255, 255, 255) : SET_TEXT_EDGE(false, 0, 0, 0, 255);
	SET_TEXT_DROPSHADOW(false, 0, 0, 0, 255);
	SET_TEXT_FONT(font);
	SET_TEXT_SCALE(scalex, scaley);
	SET_TEXT_COLOUR(color.r, color.g, color.b, color.a);

	if (justify == 0)
	{
		SET_TEXT_WRAP(0.0f, 1.0f);
		SET_TEXT_RIGHT_JUSTIFY(false);
		SET_TEXT_CENTRE(false);
	}
	else if (justify == 1)
	{
		SET_TEXT_WRAP(0.0f, 1.0f);
		SET_TEXT_RIGHT_JUSTIFY(false);
		SET_TEXT_CENTRE(true);
	}
	else if (justify == 2)
	{
		SET_TEXT_WRAP(0.0f, x);
		SET_TEXT_RIGHT_JUSTIFY(true);
		SET_TEXT_CENTRE(false);
		x -= GET_STRING_WIDTH_WITH_STRING("STRING", text) + 0.01f;
	}

	DISPLAY_TEXT_WITH_LITERAL_STRING(x, y, "STRING", text);
}

void DrawBar(float x, float y, float width, float height, float thickness, Color color)
{
	DrawRect(x, y, width, thickness, color);
	DrawRect(x - (width / 2.0f) + (thickness / 4.0f), y + (height / 2.0f), thickness * 0.5f, height, color);
	DrawRect(x + (width / 2.0f) - (thickness / 4.0f), y + (height / 2.0f), thickness * 0.5f, height, color);
	DrawRect(x, y + height - (thickness / 2.0f), width, thickness, color);
}

void DrawSprite(Texture text, float x, float y, float w, float h, float angle, Color color)
{
	//vector2 pos = convert_pixels_to_screenspace(x, y);
	//vector2 wh = convert_pixels_to_screenspace(w, h);

	if (text != NULL)
		DRAW_SPRITE(text, x, y, w, h, angle, color.r, color.g, color.b, color.a);
}

void DrawInfoBox(char* text, float x, float y, float xBoxAdjust, float xscale, float yscale, int font, Color color, int align)
{
	Color blackcolor = Color(0, 0, 0, 255);
	Color whitecolor = Color(255, 255, 255, 255);
	float width = GET_STRING_WIDTH_WITH_STRING("STRING", text);

	if (align == 1)
	{
		DrawText(text, font, x, y + 0.003f, xscale, yscale, whitecolor, align, false);
		DrawRect(x, y + (0.03f / 2.0f), width * (xscale * 2.0f) + 0.015f, 0.03f, blackcolor);
		DrawBar(x, y, width * (xscale * 2.0f) + 0.015f, 0.03f, 0.002f, color);
	}
	else if (align == 2)
	{
		DrawText(text, font, x + xBoxAdjust, y + 0.003f, xscale, yscale, whitecolor, align, false);
		DrawRect(x - (width * xscale), y + (0.03f / 2), width * (xscale * 2) + 0.015f, 0.03f, blackcolor);
		DrawBar(x - (width * xscale), y, width * (xscale * 2) + 0.015f, 0.03f, 0.0015f, color);
	}
}

void DrawBackgroundAndHeader()
{
	char* buffer = BUFFER16;
	char* net_buffer = BUFFER128;

	//DrawRect(Container.Ui.DrawPos.x, Container.Ui.DrawPos.y, Container.Ui.DrawSize.width, Container.Ui.DrawSize.height, Container.Ui.HeaderColor);
	DrawSprite(HeaderTexture, Container.Ui.DrawPos.x, Container.Ui.DrawPos.y, Container.Ui.DrawSize.height, Container.Ui.DrawSize.width, 270.0f, Container.Ui.HeaderColor); 
	//0.0015f is fine.

	if(CartelLogo == NULL)
		DrawText(Container.Ui.MenuName, Container.Ui.HeaderFont, Container.Ui.DrawPos.x, Container.Ui.DrawPos.y - 0.0275f, 0.4f, 0.8f, Container.Ui.HeaderTextColor, 1, false);
	else
		DrawSprite(CartelLogo, Container.Ui.DrawPos.x, Container.Ui.DrawPos.y + 0.0012f, 0.105f, 0.078f, 0.0f, Color(255, 255, 255, 255));

	sprintf(buffer, "%i / %i", Container.CursorIndex + 1, Container.TotalItemCount);
	DrawRect(Container.Ui.DrawPos.x, Container.Ui.DrawPos.y + 0.058f, Container.Ui.DrawSize.width, Container.Ui.DrawSize.h - 0.05f, Container.Ui.SubtitleColor);

	if (bitTest(Container.BitSet, PB_IsInNetPlayerMenu))
	{
		int index = Container.playerId;
		sprintf(net_buffer, "%s - ~COL_NET_%i~%s", Container.Ui.HeaderText[Container.CurrentMenuLevel], index + 1, GET_PLAYER_NAME(index));
		DrawText(net_buffer, Container.Ui.TextFont, Container.Ui.DrawPos.x - 0.099f, Container.Ui.DrawPos.y + 0.0475f, 0.15f, 0.3f, Container.Ui.TextColor, 0, false);
	}
	else
		DrawText(Container.Ui.HeaderText[Container.CurrentMenuLevel], Container.Ui.TextFont, Container.Ui.DrawPos.x - 0.099f, Container.Ui.DrawPos.y + 0.0475f, 0.15f, 0.3f, Container.Ui.TextColor, 0, false);

	DrawText(buffer, Container.Ui.TextFont, Container.Ui.DrawPos.x + 0.1f, Container.Ui.DrawPos.y + 0.048f, 0.15f, 0.3f, Container.Ui.TextColor, 2, false);
}

int GetItemCountWithMaxItemLimits()
{
	return Container.TotalItemCount > MaxDisplayableItems ? MaxDisplayableItems : Container.TotalItemCount;
}

void DrawItems()
{
	uint display_model = NULL;

	for (int i = 0; i < GetItemCountWithMaxItemLimits(); i++)
	{
		bool selected = (Container.CursorIndex == i + Container.ItemStartIndex);
		float yrectpos = Container.Ui.DrawPos.y + 0.058f + 0.033f + (i * 0.033f);
		float ytextpos = Container.Ui.DrawPos.y + 0.0475f + 0.033f + (i * 0.033f);
		DrawSprite(CursorTexture, Container.Ui.DrawPos.x, yrectpos, Container.Ui.DrawSize.width, 0.033f, 0.0f, selected ? Container.Ui.ScrollBarColor : Container.Ui.BackgroundColor);
		//DrawRect(Container.Ui.DrawPos.x, yrectpos, Container.Ui.DrawSize.width, 0.033f, selected ? Container.Ui.ScrollBarColor : Container.Ui.BackgroundColor);

		if (Container.Item[i].Selection.Type == MST_Player)
		{
			RGBA playerColor = { 0, 0, 0, 255 };
			GET_PLAYER_RGB_COLOUR(Container.Item[i].Selection.DynamicId, &playerColor.r, &playerColor.g, &playerColor.b);

			Color text_color = Color(playerColor.r, playerColor.g, playerColor.b, 255);
			DrawText(GET_PLAYER_NAME(Container.Item[i].Selection.DynamicId), Container.Ui.TextFont, Container.Ui.DrawPos.x - 0.099f, ytextpos, 0.15f, 0.3f, selected ? Container.Ui.SelectedTextColor : text_color, 0, false);
		}
		else
			DrawText(Container.Item[i].Ui.ItemText, Container.Ui.TextFont, Container.Ui.DrawPos.x - 0.099f, ytextpos, 0.15f, 0.3f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor, 0, false);

		switch (Container.Item[i].Selection.Type)
		{
		case MST_None:
		case MST_Param:
			break;
		case MST_Menu:
		case MST_Menus: /////new
		case MST_MenuParam:
			DrawText(">>", Container.Ui.TextFont, Container.Ui.DrawPos.x + 0.1f, ytextpos, 0.15f, 0.3f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor, 2, false);
			break;
		case MST_Int:
		case MST_Radius:
		case MST_CarColor:
		{
			char* buffer = BUFFER16;
			sprintf(buffer, "%i", *Container.Item[i].Selection.Value_pointer.Int);
			DrawText(buffer, Container.Ui.TextFont, Container.Ui.DrawPos.x + 0.047f, ytextpos + 0.002f, 0.15f, 0.3f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor, 2, false);
			DrawBar(Container.Ui.DrawPos.x + 0.075f, yrectpos - 0.009f, 0.05f, 0.019f, 0.0015f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor);
			DrawRect(Container.Ui.DrawPos.x + 0.05f + (((0.05f / (Container.Item[i].Selection.EndIndex.Int - Container.Item[i].Selection.StartIndex.Int)) * (*Container.Item[i].Selection.Value_pointer.Int - Container.Item[i].Selection.StartIndex.Int)) / 2), yrectpos, ((0.05f / (Container.Item[i].Selection.EndIndex.Int - Container.Item[i].Selection.StartIndex.Int)) * (*Container.Item[i].Selection.Value_pointer.Int - Container.Item[i].Selection.StartIndex.Int)), 0.019f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor);

			if (selected)
			{
				if (Container.Item[i].Selection.Type == MST_Radius)
				{
					vector3 pos;
					if (DOES_CHAR_EXIST(myPlayerPed))
					{
						GET_CHAR_COORDINATES(myPlayerPed, &pos.x, &pos.y, &pos.z);
						DRAW_CHECKPOINT_WITH_ALPHA(pos.x, pos.y, pos.z - 4.0f, TO_FLOAT(*Container.Item[i].Selection.Value_pointer.Int), Container.Ui.HeaderColor.r, Container.Ui.HeaderColor.g, Container.Ui.HeaderColor.b, Container.Ui.HeaderColor.a);
					}
				}
				else if (Container.Item[i].Selection.Type == MST_CarColor)
				{
					DrawBar(Container.Ui.DrawPos.x - 0.13f, Container.Ui.DrawPos.y + 0.06f, 0.04f, 0.07f, 0.002f, Container.Ui.SelectedTextColor);
					DrawRect(Container.Ui.DrawPos.x - 0.13f + 0.001f, Container.Ui.DrawPos.y + 0.095f, 0.04 - 0.002f, 0.07 - 0.001f, GetCarColorForIndex(*Container.Item[i].Selection.Value_pointer.Int));
				}
			}
		}
		break;
		case MST_RGB:
		case MST_Byte:
		{
			char* buffer = BUFFER16;

			sprintf(buffer, "%i", (*Container.Item[i].Selection.Value_pointer.Int >> 24) & 0xFF);
			DrawText(buffer, Container.Ui.TextFont, Container.Ui.DrawPos.x + 0.047f, ytextpos + 0.002f, 0.15f, 0.3f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor, 2, false);
			DrawBar(Container.Ui.DrawPos.x + 0.075f, yrectpos - 0.009f, 0.05f, 0.019f, 0.0015f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor);
			DrawRect(Container.Ui.DrawPos.x + 0.05f + (((0.05f / (Container.Item[i].Selection.EndIndex.Int - Container.Item[i].Selection.StartIndex.Int)) * (((*Container.Item[i].Selection.Value_pointer.Int >> 24) & 0xFF) - Container.Item[i].Selection.StartIndex.Int)) / 2), yrectpos, ((0.05f / (Container.Item[i].Selection.EndIndex.Int - Container.Item[i].Selection.StartIndex.Int)) * (((*Container.Item[i].Selection.Value_pointer.Int >> 24) & 0xFF) - Container.Item[i].Selection.StartIndex.Int)), 0.019f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor);

			if (selected && Container.Item[i].Selection.Type == MST_RGB)
			{
				DrawBar(Container.Ui.DrawPos.x - 0.13f, Container.Ui.DrawPos.y + 0.06f, 0.04f, 0.07f, 0.002f, Container.Ui.SelectedTextColor);

				Color selectColor = Color(Container.Item[i].Selection.color[0], Container.Item[i].Selection.color[1], Container.Item[i].Selection.color[2], Container.Item[i].Selection.color[3]);
				DrawRect(Container.Ui.DrawPos.x - 0.13f + 0.001f, Container.Ui.DrawPos.y + 0.095f, 0.04f - 0.002f, 0.07f - 0.001f, selectColor);
			}
		}
		break;
		case MST_Float:
		{
			char* res = BUFFER16;
			memset(res, 0, 10);
			ftos(*Container.Item[i].Selection.Value_pointer.Float, Container.Item[i].Selection.Precision, res);
			DrawText(res, Container.Ui.TextFont, Container.Ui.DrawPos.x + 0.047f, ytextpos + 0.002f, 0.15f, 0.3f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor, 2, false);
			DrawBar(Container.Ui.DrawPos.x + 0.075f, yrectpos - 0.009f, 0.05f, 0.019f, 0.0015f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor);
			DrawRect(Container.Ui.DrawPos.x + 0.05f + (((0.05f / (Container.Item[i].Selection.EndIndex.Float - Container.Item[i].Selection.StartIndex.Float)) * (*Container.Item[i].Selection.Value_pointer.Float - Container.Item[i].Selection.StartIndex.Float)) / 2), yrectpos, ((0.05f / (Container.Item[i].Selection.EndIndex.Float - Container.Item[i].Selection.StartIndex.Float)) * (*Container.Item[i].Selection.Value_pointer.Float - Container.Item[i].Selection.StartIndex.Float)), 0.019f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor);
		}
		break;
		case MST_Bool:
		{
			bool toggled = IS_BIT_SET(*Container.Item[i].Selection.Value_pointer.Int, Container.Item[i].Selection.Value.Int);
			DrawSprite(toggled ? checkedbox : uncheckedbox, Container.Ui.DrawPos.x + 0.095f, yrectpos, 0.016f, 0.028f, 0.0f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor);
		}
		break;
		case MST_Char:
		{
			char* buffer = BUFFER64;

			sprintf(buffer, "< %s >", Container.Item[i].Selection.CharArray[*Container.Item[i].Selection.Value_pointer.Int]);
			DrawText(buffer, Container.Ui.TextFont, Container.Ui.DrawPos.x + 0.1f, ytextpos, 0.15f, 0.3f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor, 2, false);
		}
		break;
		case MST_Player:
		{
			//TODO Re-do alot of this
			bool inGhost = false;
			PLAYER_INDEX playerIndex = Container.Item[i].Selection.DynamicId;
			CPlayerInfo* player = GetPlayerPointer(playerIndex);

			if (NETWORK_PLAYER_HAS_HEADSET(playerIndex))
				DrawSprite(headSet, Container.Ui.DrawPos.x + 0.095f, yrectpos, 0.016f, 0.020f, 0.0f, selected ? Container.Ui.SelectedTextColor : Color(255, 255, 255, 255));

			char* xboxOne = "";
			if (player->m_machineId[1] != 250)
				xboxOne = (selected) ? "(X)" : "~COL_NET_3~(X)";

			char* ghost = "";
			if (!IS_NETWORK_PLAYER_ACTIVE(playerIndex))
			{
				if(player->m_playerPed)
					ghost = (selected) ? "(G)" : "~COL_NET_8~(G)";
				else
					ghost = (selected) ? "(HG)" : "~COL_NET_8~(HG)";
				inGhost = true;
			}

			char* cartelUser = "";
			//if (GetCartelUserType(playerIndex) != CARTEL_NON_USER) //shouldn't need this/compares strings (now compares hashes actually but meh we don't need to do this here)
			if(IS_BIT_SET(ctlMem->m_bit_sets.m_vehicleNetMonitor_printedCtlUser, (playerIndex + 16))) //we printed their joinmsg aka detected as cartel user.
				cartelUser = (selected) ? "(C)" : "~w~(C)";

			char* modder = "";
			//if(inGhost || !IsPlayerAMultiplayerModel(playerIndex) || IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, playerIndex)) //junk this when we're sure only model changes get caught.(they should)
			if(inGhost || IS_BIT_SET(ctlMem->m_bit_sets.m_detectedModding_detectedIVMenu, playerIndex))
				modder = (selected) ? "(M)" : "~COL_NET_4~(M)";

			char* host = "";
			if(GET_HOST_ID() == playerIndex)
				host = (selected) ? "(H)" : "~COL_NET_1~(H)";

			if (xboxOne[0] != '\0' || ghost[0] != '\0' || modder[0] != '\0' || host[0] != '\0' || cartelUser[0] != '\0')
			{
				char* buffer = BUFFERMAX; //256 should be
				sprintf(buffer, "%s%s%s%s%s", cartelUser, xboxOne, ghost, modder, host);
				DrawText(buffer, Container.Ui.TextFont, Container.Ui.DrawPos.x + 0.085f, ytextpos, 0.15f, 0.3f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor, 2, false);
			}
		}
		break;
		case MST_Vehicle:
		{
			if (selected && bitTest(Container.BitSet, PB_DisplayVehiclePreview))
				display_model = Container.Item[i].Selection.Value.Int;
		}
		break;
		case MST_Script:
		{
			bool toggled = GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(Container.Item[i].Selection.Value_pointer.Char);
			DrawSprite(toggled ? checkedbox : uncheckedbox, Container.Ui.DrawPos.x + 0.095f, yrectpos, 0.016f, 0.028f, 0.0f, selected ? Container.Ui.SelectedTextColor : Container.Ui.TextColor);
		}
		break;
		}
	}

	static float prev_heading = 360.0f;

	//HACK will consider making a bool ShouldSuppressPreviewingVehicles function
	if (display_model && IS_MODEL_IN_CDIMAGE(display_model) && bitTest(Container.BitSet, PB_DisplayVehiclePreview) && GetMenuPlayerId() == myPlayerIndex && !bitTest(ctlMem->m_bit_sets.m_player, inChauffeurSubmenu))
	{
		if (DOES_VEHICLE_EXIST(displayed_veh) && IS_CAR_MODEL(displayed_veh, display_model))
		{
			float x, y, z, heading;
			vector3 min, max;

			GET_MODEL_DIMENSIONS(display_model, &min, &max);
			GetOffsetFromCamera(10.0f + max.y, &x, &y, &z, &heading);

			if (prev_heading < 0.0f)
				prev_heading += 360.0f;

			prev_heading -= 1.5f;
			SET_CAR_HEADING(displayed_veh, prev_heading);
			SET_CAR_COORDINATES(displayed_veh, x, y, z);
			SET_CAR_ON_GROUND_PROPERLY(displayed_veh);
			SET_CAR_PROOFS(displayed_veh, true, true, true, true, true);
			SET_CAR_COLLISION(displayed_veh, false);
			SET_VEHICLE_ALPHA(displayed_veh, 175);
		}
		else
		{
			if (DOES_VEHICLE_EXIST(displayed_veh))
			{
				uint tmpModel; //
				GET_CAR_MODEL(displayed_veh, &tmpModel); //
				MARK_MODEL_AS_NO_LONGER_NEEDED(tmpModel); //

				DELETE_CAR(&displayed_veh);
			}

			if (HAS_MODEL_LOADED(display_model))
			{
				SpawnVehicle(display_model, 0.0f, 0.0f, 0.0f, 0.0f, &displayed_veh, true, false);
				/*
				SET_CAR_EXISTS_ON_ALL_MACHINES(displayed_veh, false); //why we already create it localy only.
				int vehNetId;
				GET_NETWORK_ID_FROM_VEHICLE(displayed_veh, &vehNetId);
				if (DOES_VEHICLE_EXIST_WITH_NETWORK_ID(vehNetId))
					SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(vehNetId, false);

				SET_CAR_STOP_CLONING(displayed_veh, true); //On a vehicle that's only local huh? This probably caused our issue with crashing when using this and people join.
				*/
			}
			else
				REQUEST_MODEL(display_model);
		}
	}
	else
	{
		if (DOES_VEHICLE_EXIST(displayed_veh))
			DELETE_CAR(&displayed_veh);
	}
}

void DrawDescription()
{
	float ybottomrectpos = Container.Ui.DrawPos.y + 0.058f + 0.033f + (GetItemCountWithMaxItemLimits() * 0.033f);
	float ybottomtextpos = Container.Ui.DrawPos.y + 0.0475f + 0.033f + (GetItemCountWithMaxItemLimits() * 0.033f);

	DrawSprite(HeaderTexture, Container.Ui.DrawPos.x, ybottomrectpos, 0.033f, Container.Ui.DrawSize.width, 90.0f, Container.Ui.HeaderColor); //rotated 90 degrees and swapped widhth and height around.
	//DrawRect(Container.Ui.DrawPos.x, ybottomrectpos, Container.Ui.DrawSize.w, 0.033f, Container.Ui.HeaderColor);
	if (Container.Item[GetRelativeCursorIndex].Ui.Description)
		DrawText(Container.Item[GetRelativeCursorIndex].Ui.Description, Container.Ui.TextFont, Container.Ui.DrawPos.x - 0.099f, ybottomtextpos, 0.13f, 0.27f, Container.Ui.TextColor, 0, false);
}

void DrawMenu()
{
	DrawBackgroundAndHeader();
	DrawItems();
	DrawDescription();
}

void DrawPlayerHighlight(int index)
{
	vector3 pos;
	PED_INDEX ped;
	int ir, ig, ib;
	
	if (PLAYER_HAS_CHAR(index))
	{
		GET_PLAYER_CHAR(index, &ped);
		GET_PLAYER_RGB_COLOUR(index, &ir, &ig, &ib);
		GET_CHAR_COORDINATES(ped, &pos.x, &pos.y, &pos.z);
		DRAW_CHECKPOINT_WITH_ALPHA(pos.x, pos.y, pos.z - (IS_CHAR_IN_ANY_CAR(ped) ? 0.5f : 1.0f), 1.3f, ir, ig, ib, 255);
	}
}

void BlockPunchAnims()
{
	char* anims[5] = { "hook_l", "hook_r", "jab", "kick_a", "kick_b" }; //TODO: Nath, need to pull anims from knife too

	for (int i = 0; i < 5; i++)
	{
		if (IS_CHAR_PLAYING_ANIM(myPlayerPed, "melee_unarmed_base", anims[i]))
			SET_CHAR_ANIM_CURRENT_TIME(myPlayerPed, "melee_unarmed_base", anims[i], 1.0f); //doesn't stop anim over network but stops your dude moving position / handy for accurate teleport positions while menu is open.
	}
}

void HandleMenuUi()
{
	//if (GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT("computermain")) //meh this is a SUPER niche/minor thing and you can use the computers without menu getting in the way so...
		//ShutDownMenu(); //also need to stop UI drawing..

	OpenMenuControls();

	if (bitTest(Container.BitSet, PB_IsMenuOpen))
	{
		BlockPunchAnims();
		if (IS_PAUSE_MENU_ACTIVE())
		{
			ShutDownMenu();
		}

		if (bitTest(Container.BitSet, PB_IsCurrentMenuInvalid))
		{
			UpdateMenuLevel(-1, NULL, NULL);
			bitReset(&Container.BitSet, PB_IsCurrentMenuInvalid);
		}
		
		if (bitTest(Container.BitSet, PB_IsInNetPlayerMenu))
		{
			DrawPlayerHighlight(Container.playerId);

			if (!IsPlayerValid(Container.playerId))
				UpdateMenuLevel(-1, NULL, NULL);
		}

		DisableUnusedInputs();
		ParseMenuControls();
		DrawMenu();
	}
}