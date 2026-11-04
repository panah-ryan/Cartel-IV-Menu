#pragma once

#define MaxMenuLevels 10
#define MaxDisplayableItems 10

typedef enum MenuSelectionType
{
	MST_None,
	MST_Param,
	MST_Int,
	MST_Float,
	MST_Bool,
	MST_Menu,
	MST_Menus, //all executes for menu levels
	MST_MenuParam,
	MST_Player,
	MST_Char,
	MST_CarColor,
	MST_RGB,
	MST_Radius,
	MST_Byte,
	MST_Vehicle,
	MST_Script
} MenuSelectionType;

typedef enum ItemContainerBits
{
	ICB_ExecuteOnChange, //Gets whether the item executes on left, right scroll
	ICB_IsItemDisabled //Gets whether the item is disabled
} ItemContainerBits;

typedef enum PageBits
{
	PB_IsMenuOpen,
	PB_IsCurrentMenuInvalid, //ex: invalid selected player index
	PB_LastMenuDirection, //Back = 0, Forward = 1
	PB_DisplayVehiclePreview,
	PB_IsInNetPlayerMenu,
	PB_IsInObjectSpawner,
	PB_ObjSpawner_SuppressBInputOnce,
} PageBits;

typedef union flint_pointer
{
	int* Int;
	float* Float;
	byte* Byte;
	char* Char;
} flint_pointer;

typedef struct Color
{
	int r;
	int g;
	int b;
	int a;
} Color;

#define Color(r, g, b, a) (Color) { r, g, b, a }

typedef struct ItemContainer
{
	int BitSet;
	void (*Execute)(void*, void*);

	union
	{
		void (*AlternateExecute)(void*, void*);
		bool HasAlternateExecution;
	};

	union
	{
		void (*AlternateExecute2)(void*, void*);
		bool Has2ndAlternateExecute;
	};

	struct
	{
		bool has_value;
		uint value;
	} value_two;

	struct
	{
		const char* ItemText;
		const char* Description;
	} Ui;

	struct
	{
		MenuSelectionType Type : 8;
		char DynamicId : 8;
		short _padding : 16;
		flint Value;
		flint_pointer Value_pointer;

		union
		{
			char** CharArray;
			int iIncrement;
			float fIncrement;
		};

		float Precision;
		flint StartIndex;
		flint EndIndex;
		byte* color;
	} Selection;

} ItemContainer;

typedef struct MenuLevel
{
	void (*UpdateMenuLevel)(void*, void*);
	char CursorIndex : 8;
	char ItemStartIndex : 8;
	short _padding : 16;
} MenuLevel;

typedef struct Page
{
	int BitSet;
	int CursorIndex;
	int TotalItemCount;
	int ItemStartIndex;
	int CurrentMenuLevel;
	PLAYER_INDEX playerId;
	MenuLevel Level[MaxMenuLevels];

	struct
	{
		char* MenuName;
		const char* HeaderText[MaxMenuLevels];
		uint HeaderFont;
		uint TextFont;
		vector2 DrawPos;
		Size DrawSize;
		Color HeaderColor;
		Color HeaderTextColor;
		Color SubtitleColor;
		Color TextColor;
		Color BackgroundColor;
		Color SelectedTextColor;
		Color ScrollBarColor;
		int OpenButton1;
		int OpenButton2;
	} Ui;

	ItemContainer Item[MaxDisplayableItems];

} Page;

extern Page Container;
extern int CanMoveMenu;

void InitMenuDraw();
void HandleMenuUi();
void UpdateMenuLevel(int directionandamount, void* param, void(*customExecute)(void*, void*));
void DisableUnusedInputs();
void ShutDownMenu(); //used in keyboard shortcut for chat when menu is closed only.
void DrawText(const char* text, int font, float x, float y, float scalex, float scaley, Color color, int justify, bool edge);
const char* GetCurrentMenuItemText();
const char* GetCurrentMenuItemDesc();
void ParseMenuControls();