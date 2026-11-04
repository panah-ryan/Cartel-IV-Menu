#include "stdafx.h"
#include "MenuExecutionHandling.h"
#include "MenuUiHandling.h"
#include "Utils.h"
#include "ItemExecution.h"

static int AddItemCounter = 0;
extern Page Container;

void ExecutionEntry()
{
	MainMenu();
}

void LoopedExecutionEntry()
{
	//ONLY PUT STUFF BELOW LoopedOptions(); otherwise our myPlayerPed variable may become null
	LoopedOptions();
}

static void ResetCurrentItem()
{
	Container.Item[AddItemCounter].BitSet = null;
	Container.Item[AddItemCounter].Execute = nullptr;
	Container.Item[AddItemCounter].AlternateExecute = nullptr;
	Container.Item[AddItemCounter].AlternateExecute2 = nullptr;
	Container.Item[AddItemCounter].Ui.Description = nullptr;
	Container.Item[AddItemCounter].Selection.Value.Int = 0;
	Container.Item[AddItemCounter].Selection.EndIndex.Int = 0;
	Container.Item[AddItemCounter].Selection.color = nullptr;
	Container.Item[AddItemCounter].Selection.Precision = 1.0f;
	Container.Item[AddItemCounter].Selection.StartIndex.Int = 0;
	Container.Item[AddItemCounter].Selection.Type = MST_None;
	Container.Item[AddItemCounter].Selection.CharArray = nullptr;
	Container.Item[AddItemCounter].value_two.value = nullptr;
	Container.Item[AddItemCounter].value_two.has_value = false;
}

void SetCurrentMenuInvalid(bool value, char* reason)
{
	if (value)
	{
		if (!bitTest(Container.BitSet, PB_IsCurrentMenuInvalid) && (reason != nullptr || reason[0] != NULL))
			printf("~r~CTL: ~w~%s", reason);

		bitSet(&Container.BitSet, PB_IsCurrentMenuInvalid);
	}
	else
		bitReset(&Container.BitSet, PB_IsCurrentMenuInvalid);
}

void SetHeader(const char* HeaderText)
{
	AddItemCounter = 0;
	Container.TotalItemCount = 0;
	Container.Ui.HeaderText[Container.CurrentMenuLevel] = HeaderText;
}

void SetHeaderNoUpdate(const char* HeaderText)
{
	Container.Ui.HeaderText[Container.CurrentMenuLevel] = HeaderText;
}

void SetHeaderForwarded()
{
	AddItemCounter = 0;
	Container.TotalItemCount = 0;
	Container.Ui.HeaderText[Container.CurrentMenuLevel] = Container.Ui.HeaderText[Container.CurrentMenuLevel - 1];
}

void SetMenuDisplayVehiclePreview(bool set)
{
	set ? bitSet(&Container.BitSet, PB_DisplayVehiclePreview) : bitReset(&Container.BitSet, PB_DisplayVehiclePreview);
}

void SetMenuInNetworkMenu(bool set)
{
	set ? bitSet(&Container.BitSet, PB_IsInNetPlayerMenu) : bitReset(&Container.BitSet, PB_IsInNetPlayerMenu);
}

void RefreshCurrentPage(uint param, int unused)
{
	UpdateMenuLevel(0, (void*)param, nullptr);
}

void AddItemAlt(const char* ItemText, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.Type = MST_None;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemIntAlt(const char* ItemText, bool ExecuteOnChange, int MinValue, int MaxValue, int* StartIndex, int incAmount, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.StartIndex.Int = MinValue;
		Container.Item[AddItemCounter].Selection.EndIndex.Int = MaxValue;
		Container.Item[AddItemCounter].Selection.Value_pointer.Int = StartIndex;
		Container.Item[AddItemCounter].Selection.iIncrement = incAmount;

		if (ExecuteOnChange)
			bitSet(&Container.Item[AddItemCounter].BitSet, ICB_ExecuteOnChange);

		Container.Item[AddItemCounter].Selection.Type = MST_Int;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemByteAlt(const char* ItemText, bool ExecuteOnChange, int MinValue, int MaxValue, byte* StartIndex, int incAmount, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.StartIndex.Int = MinValue;
		Container.Item[AddItemCounter].Selection.EndIndex.Int = MaxValue;
		Container.Item[AddItemCounter].Selection.Value_pointer.Byte = StartIndex;
		Container.Item[AddItemCounter].Selection.iIncrement = incAmount;

		if (ExecuteOnChange)
			bitSet(&Container.Item[AddItemCounter].BitSet, ICB_ExecuteOnChange);

		Container.Item[AddItemCounter].Selection.Type = MST_Byte;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemCarColorAlt(const char* ItemText, bool ExecuteOnChange, int* StartIndex, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.StartIndex.Int = 0;
		Container.Item[AddItemCounter].Selection.EndIndex.Int = myCurrentEpisode == 2 ? 136 : 133;
		Container.Item[AddItemCounter].Selection.Value_pointer.Int = StartIndex;
		Container.Item[AddItemCounter].Selection.iIncrement = 1;

		if (ExecuteOnChange)
			bitSet(&Container.Item[AddItemCounter].BitSet, ICB_ExecuteOnChange);

		Container.Item[AddItemCounter].Selection.Type = MST_CarColor;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemRGBAAlt(const char* ItemText, byte* StartIndex, byte* color, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.StartIndex.Int = 0;
		Container.Item[AddItemCounter].Selection.EndIndex.Int = 255;
		Container.Item[AddItemCounter].Selection.Value_pointer.Byte = StartIndex;
		Container.Item[AddItemCounter].Selection.iIncrement = 1;
		Container.Item[AddItemCounter].Selection.Type = MST_RGB;
		Container.Item[AddItemCounter].Selection.color = color;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemCharAlt(const char* ItemText, bool ExecuteOnChange, char** strings, int ArraySize, int* StartIndex, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.StartIndex.Int = 0;
		Container.Item[AddItemCounter].Selection.EndIndex.Int = ArraySize - 1;
		Container.Item[AddItemCounter].Selection.Value_pointer.Int = StartIndex;

		if (ExecuteOnChange)
			bitSet(&Container.Item[AddItemCounter].BitSet, ICB_ExecuteOnChange);

		Container.Item[AddItemCounter].Selection.Type = MST_Char;
		Container.Item[AddItemCounter].Selection.CharArray = strings;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemFloatAlt(const char* ItemText, bool ExecuteOnChange, float MinValue, float MaxValue, float* StartIndex, float incAmount, float Precision, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.StartIndex.Float = MinValue;
		Container.Item[AddItemCounter].Selection.EndIndex.Float = MaxValue;
		Container.Item[AddItemCounter].Selection.Value_pointer.Float = StartIndex;

		if (ExecuteOnChange)
			bitSet(&Container.Item[AddItemCounter].BitSet, ICB_ExecuteOnChange);

		Container.Item[AddItemCounter].Selection.Type = MST_Float;
		Container.Item[AddItemCounter].Selection.Precision = Precision;
		Container.Item[AddItemCounter].Selection.fIncrement = incAmount;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemBoolAlt(const char* ItemText, int* StartIndex, int Bit, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.Value_pointer.Int = StartIndex;
		bitSet(&Container.Item[AddItemCounter].BitSet, ICB_ExecuteOnChange);
		Container.Item[AddItemCounter].Selection.Type = MST_Bool;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].Selection.Value.Int = Bit;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemMenuAlt(const char* ItemText, void(*Callback)(), const char* description, void(*AlternateExecute)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.Type = MST_Menu;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemMenusAlt(const char* ItemText, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.Type = MST_Menus;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemMenusWithParamAlt(const char* ItemText, uint Param, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();

		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Selection.Type = MST_Menus;
		Container.Item[AddItemCounter].Selection.Value.Int = Param;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;

		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemMenuWithParamAlt(const char* ItemText, uint Param, void(*Callback)(), const char* description, void(*AlternateExecute)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Selection.Value.Int = Param;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.Type = MST_MenuParam;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemWithParamAlt(const char* ItemText, uint Param, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Selection.Type = MST_Param;
		Container.Item[AddItemCounter].Selection.Value.Int = Param;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemWithTwoParamsAlt(const char* ItemText, uint Param1, uint Param2, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Selection.Type = MST_Param;
		Container.Item[AddItemCounter].Selection.Value.Int = Param1;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		Container.Item[AddItemCounter].value_two.has_value = true;
		Container.Item[AddItemCounter].value_two.value = Param2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemPlayer(PLAYER_INDEX player, void(*Callback)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Selection.DynamicId = player;
		Container.Item[AddItemCounter].Ui.ItemText = GET_PLAYER_NAME(player);
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Selection.Type = MST_Player;
		Container.Item[AddItemCounter].HasAlternateExecution = true;
		Container.Item[AddItemCounter].Ui.Description = "Press ~PAD_X~ to open their profile.";
		AddItemCounter++;
	}
	Container.TotalItemCount++;
}

void AddItemRadiusAlt(const char* ItemText, int MinValue, int MaxValue, int* radius, int incAmount, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Selection.Type = MST_Radius;
		Container.Item[AddItemCounter].Selection.StartIndex.Int = MinValue;
		Container.Item[AddItemCounter].Selection.EndIndex.Int = MaxValue;
		Container.Item[AddItemCounter].Selection.Value_pointer.Int = radius;
		Container.Item[AddItemCounter].Selection.iIncrement = incAmount;
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemVehicleAlt(const char* ItemText, uint model_hash, void(*Callback)(), const char* description, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Selection.Type = MST_Vehicle;
		Container.Item[AddItemCounter].Selection.Value.Int = model_hash;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

void AddItemScriptAlt(const char* ItemText, const char* sco_file, int stack_size, void(*Callback)(), const char* description, uint extra_param, void(*AlternateExecute)(), void(*AlternateExecute2)())
{
	if (Container.TotalItemCount >= Container.ItemStartIndex && AddItemCounter < MaxDisplayableItems)
	{
		ResetCurrentItem();
		Container.Item[AddItemCounter].Ui.ItemText = ItemText;
		Container.Item[AddItemCounter].Selection.Type = MST_Script;
		Container.Item[AddItemCounter].Selection.Value_pointer.Char = (char*)sco_file;
		Container.Item[AddItemCounter].Selection.Value.Int = stack_size;
		Container.Item[AddItemCounter].Execute = Callback;
		Container.Item[AddItemCounter].Ui.Description = description;
		Container.Item[AddItemCounter].value_two.value = extra_param;
		Container.Item[AddItemCounter].AlternateExecute = AlternateExecute;
		Container.Item[AddItemCounter].AlternateExecute2 = AlternateExecute2;
		AddItemCounter++;
	}

	Container.TotalItemCount++;
}

int GetMenuPlayerId()
{
	return bitTest(Container.BitSet, PB_IsInNetPlayerMenu) ? Container.playerId : myPlayerIndex;
}