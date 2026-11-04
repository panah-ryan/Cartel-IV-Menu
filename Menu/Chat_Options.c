#include "stdafx.h"
#include "Functions.h"
#include "MenuExecutionHandling.h"
#include "MenuUiHandling.h"
#include "Admin_Options.h"

void sendMessageToTextChat(int reciever)//, char *message)
{
	if (GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer) > 0 && GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer) < 60)
	{
		if (reciever > -1 && reciever < 17)
		{
			//encryption
			//mR9@f8J^T!2lC9Zy
			if (!bitTest(adminRemoteOnlineBitSet, BIT_ADMREMOTE_CHAT_DISABLED))
			{ //Maybe add to the feed locally only instead to throw them off.
				byte krypto_key[16] = { 0x6d, 0x52, 0x39, 0x40, 0x66, 0x38, 0x4a, 0x5e, 0x54, 0x21, 0x32, 0x6c, 0x43, 0x39, 0x5a, 0x79 };
				int message_length = GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer);
				char* message = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
				strcpy(message, ctlMem->m_char_buffers.keyboard_input_buffer, CHAT_MSG_SIZE);
				for (int i = 0; i < message_length; ++i)
					message[i] = (char)message[i] ^ (char)krypto_key[i % sizeof(krypto_key)];

				strcpy(ctlMem->m_cartelChatVariables.m_Data[myPlayerIndex].message, message, CHAT_MSG_SIZE); //this should be encrypted now.
				ctlMem->m_cartelChatVariables.m_Data[myPlayerIndex].encryptedMessageHash = GET_HASH_KEY(ctlMem->m_cartelChatVariables.m_Data[myPlayerIndex].message);
				ctlMem->m_cartelChatVariables.m_Data[myPlayerIndex].target_player = reciever;
				GET_NETWORK_TIMER(&ctlMem->m_cartelChatVariables.m_Data[myPlayerIndex].timestamp);
				SEND_CLIENT_BROADCAST_VARIABLES_NOW(BROADCAST_ID_CHAT); //could remove this to make less obvous which broadcasts are for which setup.
			}
		}
	}
}

void load_cartel_chat()
{
	if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_CARTEL_CHAT))
	{
		if (!bitTest(ctlMem->m_bit_sets.m_network, NETWORK_REGISTERED_CHAT_VARIABLES))
		{
			bitSet(&ctlMem->m_bit_sets.m_network, NETWORK_REGISTERED_CHAT_VARIABLES);
			ctlMem->m_cartelChatVariables.m_Count = sizeof(ctl_chat_broadcast_vars_array) / 4; //left from debug doesn't work without it/has to be at top of struct.
			REGISTER_CLIENT_BROADCAST_VARIABLES((uint*)&ctlMem->m_cartelChatVariables, ctlMem->m_cartelChatVariables.m_Count, BROADCAST_ID_CHAT);
		}
		if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_SETTING_ACCEPTED_CHAT_RULES))
		{
			strcpy(ctlMem->m_char_buffers.keyboard_input_buffer, CHAT_JOINING_MESSAGE, CHAT_MSG_SIZE);
			sendMessageToTextChat(16);//, CHAT_JOINING_MESSAGE); //think 16 is all players target.
		}
	}
}


void killTextChatBits(int un1, int un2)
{
	if (!bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_CARTEL_CHAT))
		bitReset(&ctlMem->m_bit_sets.m_menuPreferences, PREF_SETTING_ACCEPTED_CHAT_RULES);
}

void Chat_Options(int unused1, int unused2)
{
	SetHeader("Network Options -> Chat");
	AddItemBool("Text chat", &ctlMem->m_bit_sets.m_menuPreferences, PREF_CARTEL_CHAT, killTextChatBits, "Chat Engine by xStormsnoutx.");
	AddItemBool("Press T to type", &ctlMem->m_bit_sets.m_menuPreferences, PREF_T_TO_TYPE_CHAT, nullptr, "Requires keyboard/chatpad.");
	AddItemBool("Hold ~PAD_RSTICK_ALL~ and press ~PAD_DPAD_LEFT~ to type", &ctlMem->m_bit_sets.m_menuPreferences, PREF_CONTROLLER_CHAT_SHORTCUT, nullptr, "Brings up on-screen keyboard, right stick + dpad left.");
	AddItemInt("Show messages for x seconds", false, 5, 60, (int*)&ctlMem->m_chat_msg_display_time_seconds, 1, nullptr, "Adjust how long messages appear for.");

}