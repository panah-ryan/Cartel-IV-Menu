#pragma once

#include "MenuUiHandling.h"

void ExecutionEntry();
void LoopedExecutionEntry();
int GetMenuPlayerId();

///<summary>Sets the header for the new menu.</summary>
///<param name="HeaderText">Name of the header.</param>
void SetHeader(const char* HeaderText);
void SetHeaderNoUpdate(const char* HeaderText);
void SetHeaderForwarded();
void RefreshCurrentPage(uint param, int unused);
void SetCurrentMenuInvalid(bool value, char* reason);
void SetMenuDisplayVehiclePreview(bool set);
void SetMenuInNetworkMenu(bool set);

///<summary>Creates a default clickable item.</summary>
///<param name="ItemText">Name of the item.</param>
///<param name="Callback">Function to execute when clicked. Use nullptr if we don't want any function to execute.</param>
///<param name="description">Sets the description of the item to show at the bottom of the menu.</param>
///<param name="AlternateExecute">Allows for having another function call. When users presses X on option this will execute.</param>
void AddItemAlt(const char* ItemText, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItem(ItemText, Callback, description) AddItemAlt(ItemText, Callback, description, nullptr, nullptr)
///<summary>Creates an Integer scroller.</summary>
///<param name="ItemText">Name of the item.</param>
///<param name="ExecuteOnChange">If turned on, everytime we scroll it will call our Callback function.</param>
///<param name="MinValue">Lowest value we can scroll down too.</param>
///<param name="MaxValue">Highest value we can scroll up too.</param>
///<param name="StartIndex">Pointer to our value we wish to modify.</param>
///<param name="incAmount">Amount we increment on each scroll. (Must be divisible by MaxValue and MinValue otherwise scroll will get messed up.)</param>
///<param name="Callback">Function to execute when clicked. Use nullptr if we don't want any function to execute.</param>
///<param name="description">Sets the description of the item to show at the bottom of the menu.</param>
///<param name="AlternateExecute">Allows for having another function call. When users presses X on option this will execute.</param>
void AddItemIntAlt(const char* ItemText, bool ExecuteOnChange, int MinValue, int MaxValue, int* StartIndex, int incAmount, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemInt(ItemText, ExecuteOnChange, MinValue, MaxValue, StartIndex, incAmount, Callback, description) AddItemIntAlt(ItemText, ExecuteOnChange, MinValue, MaxValue, StartIndex, incAmount, Callback, description, nullptr, nullptr)
void AddItemByteAlt(const char* ItemText, bool ExecuteOnChange, int MinValue, int MaxValue, byte* StartIndex, int incAmount, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemByte(ItemText, ExecuteOnChange, MinValue, MaxValue, StartIndex, incAmount, Callback, description) AddItemByteAlt(ItemText, ExecuteOnChange, MinValue, MaxValue, StartIndex, incAmount, Callback, description, nullptr, nullptr)
///<summary>Creates a Float scroller.</summary>
///<param name="ItemText">Name of the item.</param>
///<param name="ExecuteOnChange">If turned on, everytime we scroll it will call our Callback function.</param>
///<param name="MinValue">Lowest value we can scroll down too.</param>
///<param name="MaxValue">Highest value we can scroll up too.</param>
///<param name="StartIndex">Pointer to our value we wish to modify.</param>
///<param name="incAmount">Amount we increment on each scroll. (Must be divisible by MaxValue and MinValue otherwise scroll will get messed up.)</param>
///<param name="Precision">How many decimal places we wish to show. ie. (0 = 0 | 1 = 0.1 | 2 = 0.01 | 3 = 0.001)</param>
///<param name="Callback">Function to execute when clicked. Use nullptr if we don't want any function to execute.</param>
///<param name="description">Sets the description of the item to show at the bottom of the menu.</param>
///<param name="AlternateExecute">Allows for having another function call. When users presses X on option this will execute.</param>
void AddItemFloatAlt(const char* ItemText, bool ExecuteOnChange, float MinValue, float MaxValue, float* StartIndex, float incAmount, float Precision, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemFloat(ItemText, ExecuteOnChange, MinValue, MaxValue, StartIndex, incAmount, Precision, Callback, description) AddItemFloatAlt(ItemText, ExecuteOnChange, MinValue, MaxValue, StartIndex, incAmount, Precision, Callback, description, nullptr, nullptr)
///<summary>Creates a Boolean toggle.</summary>
///<param name="ItemText">Name of the item.</param>
///<param name="StartIndex">Pointer to our value we wish to modify.</param>
///<param name="Bit">Our bit value we wish to check/change.</param>
///<param name="Callback">Function to execute when clicked. Use nullptr if we don't want any function to execute.</param>
///<param name="description">Sets the description of the item to show at the bottom of the menu.</param>
///<param name="AlternateExecute">Allows for having another function call. When users presses X on option this will execute.</param>
void AddItemBoolAlt(const char* ItemText, int* StartIndex, int Bit, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemBool(ItemText, StartIndex, Bit, Callback, description) AddItemBoolAlt(ItemText, StartIndex, Bit, Callback, description, nullptr, nullptr)
///<summary>Creates a Submenu.</summary>
///<param name="ItemText">Name of the item.</param>
///<param name="Callback">Function to execute when clicked. Use nullptr if we don't want any function to execute.</param>
///<param name="description">Sets the description of the item to show at the bottom of the menu.</param>
///<param name="AlternateExecute">Allows for having another function call. When users presses X on option this will execute.</param>
void AddItemMenuAlt(const char* ItemText, void(*Callback)(), const char* description, void(*AlternateExecute)());
#define AddItemMenu(ItemText, Callback, description) AddItemMenuAlt(ItemText, Callback, description, nullptr)
///<summary>Creates a Submenu with a value to pass.</summary>
///<param name="ItemText">Name of the item.</param>
///<param name="Param">Value we wish to pass to the submenu.</summary>
///<param name="Callback">Function to execute when clicked. Use nullptr if we don't want any function to execute.</param>
///<param name="description">Sets the description of the item to show at the bottom of the menu.</param>
///<param name="AlternateExecute">Allows for having another function call. When users presses X on option this will execute.</param>
void AddItemMenuWithParamAlt(const char* ItemText, uint Param, void(*Callback)(), const char* description, void(*AlternateExecute)());
#define AddItemMenuWithParam(ItemText, Param, Callback, description) AddItemMenuWithParamAlt(ItemText, Param, Callback, description, nullptr)
///<summary>Creates a default clickable item that takes a param.</summary>
///<param name="ItemText">Name of the item.</param>
///<param name="Param">Value we wish to pass to the function.</summary>
///<param name="Callback">Function to execute when clicked. Use nullptr if we don't want any function to execute.</param>
///<param name="description">Sets the description of the item to show at the bottom of the menu.</param>
///<param name="AlternateExecute">Allows for having another function call. When users presses X on option this will execute.</param>
void AddItemWithParamAlt(const char* ItemText, uint Param, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemWithParam(ItemText, Param, Callback, description) AddItemWithParamAlt(ItemText, Param, Callback, description, nullptr, nullptr)
///<summary>Creates a submenu for a player.</summary>
///<param name="PlayerId">Player Index we want to create the menu for.</param>
///<param name="Callback">Function to execute when clicked. Use nullptr if we don't want any function to execute.</param>
void AddItemPlayer(PLAYER_INDEX player, void(*Callback)());

void AddItemCharAlt(const char* ItemText, bool ExecuteOnChange, char** strings, int ArraySize, int* StartIndex, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemChar(ItemText, ExecuteOnChange, strings, ArraySize, StartIndex, Callback, description) AddItemCharAlt(ItemText, ExecuteOnChange, strings, ArraySize, StartIndex, Callback, description, nullptr, nullptr)

void AddItemCarColorAlt(const char* ItemText, bool ExecuteOnChange, int* StartIndex, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemCarColor(ItemText, ExecuteOnChange, StartIndex, Callback, description) AddItemCarColorAlt(ItemText, ExecuteOnChange, StartIndex, Callback, description, nullptr, nullptr)

void AddItemRGBAAlt(const char* ItemText, byte* StartIndex, byte* color, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemRGBA(ItemText, StartIndex, color, Callback, description) AddItemRGBAAlt(ItemText, StartIndex, color, Callback, description, nullptr, nullptr)

void AddItemRadiusAlt(const char* ItemText, int MinValue, int MaxValue, int* radius, int incAmount, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemRadius(ItemText, MinValue, MaxValue, radius, incAmount, Callback, description) AddItemRadiusAlt(ItemText, MinValue, MaxValue, radius, incAmount, Callback, description, nullptr, nullptr)

void AddItemWithTwoParamsAlt(const char* ItemText, uint Param1, uint Param2, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemWithTwoParams(ItemText, Param1, Param2, Callback, description) AddItemWithTwoParamsAlt(ItemText, Param1, Param2, Callback, description, nullptr, nullptr)

void AddItemVehicleAlt(const char* ItemText, uint model_hash, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemVehicle(ItemText, model_hash, Callback, description) AddItemVehicleAlt(ItemText, model_hash, Callback, description, nullptr, nullptr)

void AddItemScriptAlt(const char* ItemText, const char* sco_file, int stack_size, void(*Callback)(), const char* description, uint extra_param, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemScript(ItemText, sco_file, stack_size, Callback, description) AddItemScriptAlt(ItemText, sco_file, stack_size, Callback, description, NULL, nullptr, nullptr)

void AddItemMenusWithParamAlt(const char* ItemText, uint Param, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)());
#define AddItemMenusWithParam(ItemText, Param, Callback, description) AddItemMenusWithParamAlt(ItemText, Param, Callback, description, nullptr, nullptr)