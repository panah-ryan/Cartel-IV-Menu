#include "stdafx.h"
#include "Functions.h"
#include "ItemExecution.h"
#include "MenuExecutionHandling.h"
#include "MenuUiHandling.h"

void menuPrefsDisplay(int unused1, int unused2)
{
	SetHeader("Preferences -> Display");
	AddItemBool("Draw menu open controls/buttons", &ctlMem->m_bit_sets.m_menuPreferences, PREF_DRAW_OPEN_CONTROLS, nullptr, nullptr);
	AddItemMenu("Network Display", cartelNetworkDisplayOptions, "Text chat, num 360 users, etc.");
}

int buttonHoldSelect;
int buttonPressSelect;

void setMenuOpenButtons(int unused1, int unused2)
{
	if (buttonHoldSelect == buttonPressSelect)
	{
		printf("~r~CTL: ~w~You want to fuck yourself huh?! What is wrong with you!");
		return;
	}
	ctlMem->m_padButtonsMandatoryDrawAlpha = 255; //don't exceed 255 this is a byte.
	Container.Ui.OpenButton1 = buttonHoldSelect + 4; //leftStick X/Y and rightstick X/Y skipped/useless.
	Container.Ui.OpenButton2 = buttonPressSelect + 4;
	update_menu_open_controls_string();
	printf("~r~CTL: ~w~Menu open buttons changed, don't forget to save.");
}

void menuPrefsOpenInputs(int unused1, int unused2)
{
	buttonHoldSelect = Container.Ui.OpenButton1 - 4;
	buttonPressSelect = Container.Ui.OpenButton2 - 4;
	SetHeader("Open buttons");
	AddItemChar("Hold", false, ctlMem->m_padButtonStrings, 16, &buttonHoldSelect, nullptr, "Hold this button.");
	AddItemChar("Press", false, ctlMem->m_padButtonStrings, 16, &buttonPressSelect, nullptr, "Press this button.");
	AddItem("Set controls", setMenuOpenButtons, nullptr);
}

void save_offline_load_pref(int unused1, int unused2)
{
	if (NetworkGame)
	{
		printf("~r~CTL: ~w~This only works offline / Singleplayer.");
		bitFlip(&ctlMem->m_bit_sets.m_menuPreferences, PREF_SP_AUTOLOAD);
	}
	else
		save_data_to_gamesave(0, 0);
}


void menuPrefsColourBackgroundReset(int unused1, int unused2)
{
	Container.Ui.BackgroundColor = (Color){ 100, 100, 100, 100 };
}

void menuPrefsColourBackground(int unused1, int unused2)
{
	SetHeader("Preferences -> Menu Colours (Background)");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &Container.Ui.BackgroundColor.r, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &Container.Ui.BackgroundColor.g, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &Container.Ui.BackgroundColor.b, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Alpha", false, 100, 255, &Container.Ui.BackgroundColor.a, 1, nullptr, "RGB Values: 100 - 255");
	AddItem("Reset", menuPrefsColourBackgroundReset, nullptr);
}

void menuPrefsColourHeaderReset(int unused1, int unused2)
{
	//Container.Ui.HeaderColor = (Color) { 189, 0, 0, 255 };
	Container.Ui.HeaderColor = (Color) { 255, 190, 30, 255 }; //New for textured Header "twatleft" / "twatright" //red and green matches early access discord colour near enough
}

void menuPrefsColourHeader(int unused1, int unused2)
{
	SetHeader("Preferences -> Menu Colours (Header)");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &Container.Ui.HeaderColor.r, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &Container.Ui.HeaderColor.g, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &Container.Ui.HeaderColor.b, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Alpha", false, 100, 255, &Container.Ui.HeaderColor.a, 1, nullptr, "RGB Values: 100 - 255");
	AddItem("Reset", menuPrefsColourHeaderReset, nullptr);
}


void menuPrefsColourHeaderTextReset(int unused1, int unused2)
{
	Container.Ui.HeaderTextColor = (Color){ 255, 255, 255, 255 };
}

void menuPrefsColourHeaderText(int unused1, int unused2)
{
	SetHeader("Preferences -> Menu Colours (Header Text)");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &Container.Ui.HeaderTextColor.r, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &Container.Ui.HeaderTextColor.g, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &Container.Ui.HeaderTextColor.b, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Alpha", false, 100, 255, &Container.Ui.HeaderTextColor.a, 1, nullptr, "RGB Values: 100 - 255");
	AddItem("Reset", menuPrefsColourHeaderTextReset, nullptr);
}

void menuPrefsColourSubtitleReset(int unused1, int unused2)
{
	Container.Ui.SubtitleColor = (Color) { 0, 0, 0, 255 };
}

void menuPrefsColourSubtitle(int unused1, int unused2)
{
	SetHeader("Preferences -> Menu Colours (Subtitle)");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &Container.Ui.SubtitleColor.r, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &Container.Ui.SubtitleColor.g, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &Container.Ui.SubtitleColor.b, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Alpha", false, 100, 255, &Container.Ui.SubtitleColor.a, 1, nullptr, "RGB Values: 100 - 255");
	AddItem("Reset", menuPrefsColourSubtitleReset, nullptr);
}

void menuPrefsColourTexteReset(int unused1, int unused2)
{
	Container.Ui.TextColor = (Color) { 255, 255, 255, 255 };
}

void menuPrefsColourText(int unused1, int unused2)
{
	SetHeader("Preferences -> Menu Colours (Text)");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &Container.Ui.TextColor.r, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &Container.Ui.TextColor.g, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &Container.Ui.TextColor.b, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Alpha", false, 100, 255, &Container.Ui.TextColor.a, 1, nullptr, "RGB Values: 100 - 255");

	AddItem("Reset", menuPrefsColourTexteReset, nullptr);
}

void menuPrefsColourSelectedTextReset(int unused1, int unused2)
{
	Container.Ui.SelectedTextColor = (Color) { 0, 0, 0, 255 };
}

void menuPrefsColourSelectedText(int unused1, int unused2)
{
	SetHeader("Preferences -> Menu Colours (Selected Text)");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &Container.Ui.SelectedTextColor.r, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &Container.Ui.SelectedTextColor.g, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &Container.Ui.SelectedTextColor.b, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Alpha", false, 100, 255, &Container.Ui.SelectedTextColor.a, 1, nullptr, "RGB Values: 100 - 255");
	AddItem("Reset", menuPrefsColourSelectedTextReset, nullptr);
}

void menuPrefsColourScrollBarReset(int unused1, int unused2)
{
	Container.Ui.ScrollBarColor = (Color) { 255, 255, 255, 255 };
}

void menuPrefsColourScrollBar(int unused1, int unused2)
{
	SetHeader("Preferences -> Menu Colours (Scroll Bar)");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &Container.Ui.ScrollBarColor.r, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &Container.Ui.ScrollBarColor.g, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &Container.Ui.ScrollBarColor.b, 1, nullptr, "RGB Values: 0 - 255");
	AddItemInt("Alpha", false, 100, 255, &Container.Ui.ScrollBarColor.a, 1, nullptr, "RGB Values: 100 - 255");
	AddItem("Reset", menuPrefsColourScrollBarReset, nullptr);
}

void menuPrefsColours(int unused1, int unused2)
{
	SetHeader("Preferences -> Menu Colours");
	AddItemMenu("Header", menuPrefsColourHeader, nullptr);
	AddItemMenu("Header Text", menuPrefsColourHeaderText, nullptr);
	AddItemMenu("Subtitle", menuPrefsColourSubtitle, nullptr);
	AddItemMenu("Text", menuPrefsColourText, nullptr);
	AddItemMenu("Background", menuPrefsColourBackground, nullptr);
	AddItemMenu("Selected Text", menuPrefsColourSelectedText, nullptr);
	AddItemMenu("Scroll Bar", menuPrefsColourScrollBar, nullptr);
}

#pragma region Hud_colours
uint GetHealthHUDColourAddress(void)
{
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL: return 0x82FB3051;
	case EPISODE_TLAD: return 0;
	//case EPISODE_TBOGT: return 0x82FB3051 + 0x1C;
	case EPISODE_TBOGT: return 0x82FB306D;
	}
	return 0;
}

uint GetArmourHUDColourAddress(void)
{
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL: return 0x82FB302D;
	case EPISODE_TLAD: return 0;
	case EPISODE_TBOGT: return 0x82FB3071;
	}
	return 0;
}

void menuPrefsHUDColoursHealthReset(int unused1, int unused2)
{
	byte* HealthColor = (byte*)GetHealthHUDColourAddress();
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL:
	{
		HealthColor[0] = 87;
		HealthColor[1] = 124;
		HealthColor[2] = 88; //OG colours
	}
	break;
	case EPISODE_TBOGT:
	{
		HealthColor[0] = 225;
		HealthColor[1] = 225;
		HealthColor[2] = 225; //TBOgt colours
	}
	break;
	}
}

void menuPrefsHUDColoursArmourReset(int unused1, int unused2)
{
	byte* ArmorColor = (byte*)GetArmourHUDColourAddress;
	switch (myCurrentEpisode)
	{
	case EPISODE_ORIGINAL:
	{
		ArmorColor[0] = 74;
		ArmorColor[1] = 148;
		ArmorColor[2] = 160; //OG Colours
	}
	break;
	case EPISODE_TLAD: return;
	case EPISODE_TBOGT:
	{
		ArmorColor[0] = 131;
		ArmorColor[1] = 66;
		ArmorColor[2] = 147; //OG Colours
	}
	break;
	}
}

void menuPrefsHUDColoursHealth(int unused1, int unused2)
{
	byte* HealthColor = (byte*)GetHealthHUDColourAddress();
	SetHeader("Preferences -> HUD Colours (Health)");
	AddItemByte(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &HealthColor[0], 1, nullptr, "RGB Values: 0 - 255");
	AddItemByte(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &HealthColor[1], 1, nullptr, "RGB Values: 0 - 255");
	AddItemByte(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &HealthColor[2], 1, nullptr, "RGB Values: 0 - 255");
	AddItem("Reset", menuPrefsHUDColoursHealthReset, nullptr);
}


void menuPrefsHUDColoursArmour(int unused1, int unused2)
{
	byte* ArmorColor = (byte*)GetArmourHUDColourAddress();
	SetHeader("Preferences -> HUD Colours (Armour)");
	AddItemByte(ctlMem->m_commonStrings[GXT_STRING_RED], false, 0, 255, &ArmorColor[0], 1, nullptr, "RGB Values: 0 - 255");
	AddItemByte(ctlMem->m_commonStrings[GXT_STRING_GREEN], false, 0, 255, &ArmorColor[1], 1, nullptr, "RGB Values: 0 - 255");
	AddItemByte(ctlMem->m_commonStrings[GXT_STRING_BLUE], false, 0, 255, &ArmorColor[2], 1, nullptr, "RGB Values: 0 - 255");
	AddItem("Reset", menuPrefsHUDColoursArmourReset, nullptr);
}

void setPauseColours(int unused1, int unused2)
{
	byte cols[4];
	byte* PauseColor = (byte*)0x82FB30BD; //this is the one we set
	cols[0] = PauseColor[0];
	cols[1] = PauseColor[1];
	cols[2] = PauseColor[2]; //we set these in menu

	//need to make these match the above
	PauseColor = (byte*)0x82FB30D1;
	PauseColor[0] = cols[0];
	PauseColor[1] = cols[1];
	PauseColor[2] = cols[2];
	PauseColor = (byte*)0x82FB30D5;
	PauseColor[0] = cols[0];
	PauseColor[1] = cols[1];
	PauseColor[2] = cols[2];
	PauseColor = (byte*)0x82FB30C1;
	PauseColor[0] = cols[0];
	PauseColor[1] = cols[1];
	PauseColor[2] = cols[2];
}

void menuPrefsHUDColoursHUDTextReset(int unused1, int unused2)
{
	byte cols[3] = { 140,140,140 };
	byte* PauseColor = (byte*)0x82FB30BD;
	PauseColor[0] = cols[0];
	PauseColor[1] = cols[1];
	PauseColor[2] = cols[2];
	PauseColor = (byte*)0x82FB30D1;
	PauseColor[0] = cols[0];
	PauseColor[1] = cols[1];
	PauseColor[2] = cols[2];
	PauseColor = (byte*)0x82FB30D5;
	PauseColor[0] = cols[0];
	PauseColor[1] = cols[1];
	PauseColor[2] = cols[2];
	PauseColor = (byte*)0x82FB30C1;
	PauseColor[0] = cols[0];
	PauseColor[1] = cols[1];
	PauseColor[2] = cols[2];
}

void menuPrefsHUDColoursPauseText(int unused1, int unused2)
{
	SetHeader("Preferences -> HUD Colours (Pause text)");
	byte* PauseColor = (byte*)0x82FB30BD;
	AddItemRGBA(ctlMem->m_commonStrings[GXT_STRING_RED], &PauseColor[0], PauseColor, setPauseColours, "RGB Values: 0 - 255");
	AddItemRGBA(ctlMem->m_commonStrings[GXT_STRING_GREEN], &PauseColor[1], PauseColor, setPauseColours, "RGB Values: 0 - 255");
	AddItemRGBA(ctlMem->m_commonStrings[GXT_STRING_BLUE], &PauseColor[2], PauseColor, setPauseColours, "RGB Values: 0 - 255");
	AddItem("Reset", menuPrefsHUDColoursHUDTextReset, nullptr);
}

void menuPrefsHUDColours(int unused1, int unused2)
{
	SetHeader("Preferences -> HUD Colours");
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_HEALTH], menuPrefsHUDColoursHealth, nullptr); //"Health"
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_ARMOUR], menuPrefsHUDColoursArmour, nullptr); //"Armour" - "Armor"
	AddItemMenu("Pause Menu Text", menuPrefsHUDColoursPauseText, nullptr);
}



#pragma endregion

void setInputsMoveMenu(int un1, int un2)
{
	SET_PLAYER_CONTROL(myPlayerIndex, !bitTest(CanMoveMenu, 0));
}

void menuPreferenceOptions()
{
	SetHeader(ctlMem->m_commonStrings[GXT_STRING_PREFERENCES]); //"Preferences"
	AddItemMenu(ctlMem->m_commonStrings[GXT_STRING_DISPLAY], menuPrefsDisplay, "On-screen draws, etc."); //"Display"
	//could do colours then menu / hud as think there's gxt's for those.
	AddItemMenu("Open buttons", menuPrefsOpenInputs, "Change the open buttons for Cartel.");
	AddItemMenu("Menu Colours", menuPrefsColours, nullptr);
	AddItemBool("Move menu", &CanMoveMenu, 0, setInputsMoveMenu, "~PAD_LSTICK_ALL~ to move (left analogue).");
	AddItemMenu("HUD Colours", menuPrefsHUDColours, nullptr);
	AddItemBool("Auto-load settings online", &onlineScoreBitSet, ONLINEBIT_AUTO_PULL, updateOnlineBitSet, "Auto pull saved menu settings when online.");
	AddItemBool("Auto-load settings offline", &ctlMem->m_bit_sets.m_menuPreferences, PREF_SP_AUTOLOAD, save_offline_load_pref, "Auto load saved menu settings from gamesave.");
}