#include "stdafx.h"
#include "MenuExecutionHandling.h"
#include "Keyboard_Options.h"
#include "Functions.h"
#include "Utils.h"

void loadScoKeyboard()
{
	//if (GrabKeyboardInputString(20, 1, false, true, false, false, true))
	if(showAndGetKeyboardInput("Load game script (.sco)", false, "", 25, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		if (DOES_SCRIPT_EXIST(ctlMem->m_char_buffers.keyboard_input_buffer))
		{
			if (GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(ctlMem->m_char_buffers.keyboard_input_buffer) > 0)
			{
				printf("~r~CTL: ~w~%s.sco terminated.", ctlMem->m_char_buffers.keyboard_input_buffer);
				TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(ctlMem->m_char_buffers.keyboard_input_buffer);
			}
			else
			{
				printf("~r~CTL: ~w~%s.sco loaded.", ctlMem->m_char_buffers.keyboard_input_buffer);
				REQUEST_SCRIPT(ctlMem->m_char_buffers.keyboard_input_buffer);

				while(!HAS_SCRIPT_LOADED(ctlMem->m_char_buffers.keyboard_input_buffer)) 
					WAIT(0);

				START_NEW_SCRIPT(ctlMem->m_char_buffers.keyboard_input_buffer, 1024);
				MARK_SCRIPT_AS_NO_LONGER_NEEDED(ctlMem->m_char_buffers.keyboard_input_buffer);
			}
		}
		else
			printf("~r~CTL: ~w~%s.sco not found.", ctlMem->m_char_buffers.keyboard_input_buffer);
	}
}

void addon110()
{
	SetHeader(".sco loader: addon1-20.sco");
	AddItemScript("addon1.sco", "addon1", 1024, nullptr, nullptr);
	AddItemScript("addon2.sco", "addon2", 1024, nullptr, nullptr);
	AddItemScript("addon3.sco", "addon3", 1024, nullptr, nullptr);
	AddItemScript("addon4.sco", "addon4", 1024, nullptr, nullptr);
	AddItemScript("addon5.sco", "addon5", 1024, nullptr, nullptr);
	AddItemScript("addon6.sco", "addon6", 1024, nullptr, nullptr);
	AddItemScript("addon7.sco", "addon7", 1024, nullptr, nullptr);
	AddItemScript("addon8.sco", "addon8", 1024, nullptr, nullptr);
	AddItemScript("addon9.sco", "addon9", 1024, nullptr, nullptr);
	AddItemScript("addon10.sco", "addon10", 1024, nullptr, nullptr);
	AddItemScript("addon11.sco", "addon11", 1024, nullptr, nullptr);
	AddItemScript("addon12.sco", "addon12", 1024, nullptr, nullptr);
	AddItemScript("addon13.sco", "addon13", 1024, nullptr, nullptr);
	AddItemScript("addon14.sco", "addon14", 1024, nullptr, nullptr);
	AddItemScript("addon15.sco", "addon15", 1024, nullptr, nullptr);
	AddItemScript("addon16.sco", "addon16", 1024, nullptr, nullptr);
	AddItemScript("addon17.sco", "addon17", 1024, nullptr, nullptr);
	AddItemScript("addon18.sco", "addon18", 1024, nullptr, nullptr);
	AddItemScript("addon19.sco", "addon19", 1024, nullptr, nullptr);
	AddItemScript("addon20.sco", "addon20", 1024, nullptr, nullptr);
	AddItemScript("addon21.sco", "addon21", 1024, nullptr, nullptr);
	AddItemScript("addon22.sco", "addon22", 1024, nullptr, nullptr);
	AddItemScript("addon23.sco", "addon23", 1024, nullptr, nullptr);
	AddItemScript("addon24.sco", "addon24", 1024, nullptr, nullptr);
	AddItemScript("addon25.sco", "addon25", 1024, nullptr, nullptr);
}

void auto110()
{
	SetHeader(".sco loader: cartelauto1-10.sco");
	AddItemScript("cartelauto1.sco", "cartelauto1", 1024, nullptr, nullptr);
	AddItemScript("cartelauto2.sco", "cartelauto2", 1024, nullptr, nullptr);
	AddItemScript("cartelauto3.sco", "cartelauto3", 1024, nullptr, nullptr);
	AddItemScript("cartelauto4.sco", "cartelauto4", 1024, nullptr, nullptr);
	AddItemScript("cartelauto5.sco", "cartelauto5", 1024, nullptr, nullptr);
	AddItemScript("cartelauto6.sco", "cartelauto6", 1024, nullptr, nullptr);
	AddItemScript("cartelauto7.sco", "cartelauto7", 1024, nullptr, nullptr);
	AddItemScript("cartelauto8.sco", "cartelauto8", 1024, nullptr, nullptr);
	AddItemScript("cartelauto9.sco", "cartelauto9", 1024, nullptr, nullptr);
	AddItemScript("cartelauto10.sco", "cartelauto10", 1024, nullptr, nullptr);
}

void loadCartelAutoScos() //called once in main void on startup.
{
	char* auto_scripts[10] = { "cartelauto1", "cartelauto2", "cartelauto3", "cartelauto4", "cartelauto5", "cartelauto6", "cartelauto7", "cartelauto8", "cartelauto9", "cartelauto10" };

	for (int i = 0; i < 10; i++) //1 - 10
	{
		if (DOES_SCRIPT_EXIST(auto_scripts[i])) 
		{
			if (GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(auto_scripts[i]) > 0) 
				continue; //already loaded

			REQUEST_SCRIPT(auto_scripts[i]);
			while (!HAS_SCRIPT_LOADED(auto_scripts[i])) 
				WAIT(0);

			START_NEW_SCRIPT(auto_scripts[i], 1024);
			MARK_SCRIPT_AS_NO_LONGER_NEEDED(auto_scripts[i]);
		}
	}
}

void bundledScriptLoader(int un1, int un2)
{
	SetHeader(".sco (script) loader: bundled");
#ifdef Developer
	AddItemScript("Evil B's console trainer", "eb_trainer", 1024, nullptr, "eb_trainer.sco"); //don't want em littering objects
#endif
	AddItemScript("Memes", "memes", 1024, nullptr, "Only the finest of memes.");
	AddItemScript("Buffetings menu", "buffy_menu", 1024, nullptr, "buffy_menu.sco");
	AddItemScript("XMC menu v6", "xmc_modmenu", 1024, nullptr, "xmc_modmenu.sco");
	//AddItemScript("Project 368 v2", "project368v2", 1024, nullptr, "project368v2.sco"); //insta terminates idk why
	AddItemScript("RockstarsKing menu", "rockstarsking_menu", 1024, nullptr, "rockstarsking_menu.sco");
	AddItemScript("Source menu ~PAD_DPAD_DOWN~ & ~PAD_A~", "source_menu", 1024, nullptr, "source_menu.sco");
	//AddItemScript("drs01", "drs01", 1024, nullptr, "drs01.sco");

}

void ScoLoader()
{
	SetHeader(".sco (script) loader");
	AddItemMenu("Bundled scripts", bundledScriptLoader, "Scripts that come with Cartel .img files.");
	AddItemMenu("addon1-25.sco", addon110, nullptr);
	AddItemMenu("cartelauto1-10.sco", auto110, nullptr);
	AddItem("Load .sco (keyboard/chatpad)", loadScoKeyboard, "Will terminate if already loaded.");
}