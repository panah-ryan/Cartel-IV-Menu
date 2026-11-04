#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"
#include "cartel_chat.h"
#include "../Menu/Functions.h"
#include "Functions.h"

ctl_chat_message emptyMessage = { 0, 255, 0, "/0" };

void set_up_draw(bool centred, float width, float height, uint r, uint g, uint b, uint a)
{
	SET_TEXT_FONT(0);
	SET_TEXT_BACKGROUND(0);
	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 255);
	SET_TEXT_EDGE(0, 0, 0, 0, 255);
	SET_TEXT_COLOUR(r, g, b, a);
	SET_TEXT_SCALE(width, height);
	SET_TEXT_PROPORTIONAL(1);
	SET_TEXT_JUSTIFY(1);
	SET_TEXT_CENTRE(centred);
	SET_TEXT_RIGHT_JUSTIFY(0);
}

bool isMessageDisplayed(const ctl_chat_message *msg) 
{ //could probbaly make this faster by just passing timestamp alone
	for (int i = 0; i < MAX_DISPLAY_MESSAGES; i++)
	{
		if (ctlMem->m_CartelChatDisplay[i].timestamp == msg->timestamp)
			return true;
	}

	//pretty sure the rest of this is not necessary.
	/*
	byte krypto_key[16] = { 0x6d, 0x52, 0x39, 0x40, 0x66, 0x38, 0x4a, 0x5e, 0x54, 0x21, 0x32, 0x6c, 0x43, 0x39, 0x5a, 0x79 };
	char* chatMessage = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	strcpy(chatMessage, msg->message, CHAT_MSG_SIZE); //yeah we can't write other players / don't want to overwrie this.
	int key_length = sizeof(krypto_key); //eh obscures it a tiny bit why not.
	int message_length = GET_LENGTH_OF_LITERAL_STRING(chatMessage);

	for (int i = 0; i < message_length; ++i)
		chatMessage[i] = (char)chatMessage[i] ^ (char)krypto_key[i % key_length];

	for (int i = 0; i < MAX_DISPLAY_MESSAGES; i++)
	{
		//if (ctlMem->m_CartelChatDisplay[i].timestamp == msg->timestamp && COMPARE_STRING(ctlMem->m_CartelChatDisplay[i].message, msg->message)) 
		if (ctlMem->m_CartelChatDisplay[i].timestamp == msg->timestamp && COMPARE_STRING(ctlMem->m_CartelChatDisplay[i].message, chatMessage)) 
			return true;
	}
	*/
	return false;
}


void addNewMessageToDisplay(uint timer)
{
	for (int i = 0; i < MAX_CHAT_MESSAGES; i++) //max players aka 16
	{
		if (!IsPlayerValid(i)) 
			continue;

		if (ctlMem->m_cartelChatVariables.m_Data[i].target_player != 16 && ctlMem->m_cartelChatVariables.m_Data[i].target_player != myPlayerIndex)
			continue; //message is not for us
		
		if (timer - ctlMem->m_cartelChatVariables.m_Data[i].timestamp > MESSAGE_TIMEOUT) 
			continue;

		if (NETWORK_IS_PLAYER_MUTED_BY_ME(i) || NETWORK_IS_PLAYER_BLOCKED_BY_ME(i))
			continue;

		if (!isMessageDisplayed(&ctlMem->m_cartelChatVariables.m_Data[i])) //yes this is correct //now compares timestamp alone for speed.
		{ //will return false once we've added this to the display array, so this will only run once per message.
			//DECRYPTION
			//mR9@f8J^T!2lC9Zy
			if (ctlMem->m_cartelChatVariables.m_Data[i].encryptedMessageHash != GET_HASH_KEY(ctlMem->m_cartelChatVariables.m_Data[i].message))
				continue; //message hash doesn't match / they haven't sent all the data yet.

			byte krypto_key[16] = { 0x6d, 0x52, 0x39, 0x40, 0x66, 0x38, 0x4a, 0x5e, 0x54, 0x21, 0x32, 0x6c, 0x43, 0x39, 0x5a, 0x79 };
			char* message_decrypted = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
			int message_length = GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_cartelChatVariables.m_Data[i].message);
			strcpy(message_decrypted, ctlMem->m_cartelChatVariables.m_Data[i].message, CHAT_MSG_SIZE);
			for (int i = 0; i < message_length; ++i)
				message_decrypted[i] = (char)message_decrypted[i] ^ (char)krypto_key[i % sizeof(krypto_key)];

			if (!IsStrValid(message_decrypted))
			{ //displaying this message would crash us.
				strcpy(message_decrypted, "Invalid string of text replaced.\0", CHAT_MSG_SIZE);
			}

			for (int i = 0; i < MAX_DISPLAY_MESSAGES; i++)
			{
				if (ctlMem->m_CartelChatDisplay[i].timestamp > 0) 
				{//we are displaying a message and need to re-order them
					for (int j = MAX_DISPLAY_MESSAGES - 1; j > 0; j--)
					{
						strcpy(ctlMem->m_CartelChatDisplay[j].message, ctlMem->m_CartelChatDisplay[j - 1].message, CHAT_MSG_SIZE);
						ctlMem->m_CartelChatDisplay[j].target_player = ctlMem->m_CartelChatDisplay[j - 1].target_player;
						ctlMem->m_CartelChatDisplay[j].timestamp = ctlMem->m_CartelChatDisplay[j - 1].timestamp;
					}
					break; //messages successfully re-ordered.
				}
			}

			//add to display
			strcpy(ctlMem->m_CartelChatDisplay[0].message, message_decrypted, CHAT_MSG_SIZE);
			ctlMem->m_CartelChatDisplay[0].message[CHAT_MSG_SIZE - 1] = '\0';
			ctlMem->m_CartelChatDisplay[0].target_player = i; //dual purpose, used to display who it's from when drawing.
			ctlMem->m_CartelChatDisplay[0].timestamp = ctlMem->m_cartelChatVariables.m_Data[i].timestamp;
			break;
		}
	}
}

void displayMessages() 
{
	//kill this if admin has disabled it.
	int num_messages = 0;
	for (int i = 0; i < MAX_DISPLAY_MESSAGES; i++) 
	{
		if (ctlMem->m_CartelChatDisplay[i].timestamp > 0) 
		{
			//message[105]; //less might also work
			char* message = BUFFER128;
			//target player isn't needed here so we store the playerindex that sent the message into it. (It's dual purpose, this is correct even if t looks fucked).
			sprintf(message, "~COL_NET_%i~%s: ~w~%s", (ctlMem->m_CartelChatDisplay[i].target_player + 1), GET_PLAYER_NAME(ctlMem->m_CartelChatDisplay[i].target_player), ctlMem->m_CartelChatDisplay[i].message);
			set_up_draw(false, 0.3, 0.3, 255, 255, 255, 255);
			DISPLAY_TEXT_WITH_LITERAL_STRING(0.05, 0.7 - (i * 0.03), "STRING", message);
			num_messages++;
		}
	}
	if (num_messages > 0)
	{
		set_up_draw(false, 0.3, 0.3, 0, 200, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.05, 0.7 - num_messages * 0.03, "STRING", "~BLIP_24~ Cartel Chat ~BLIP_82~");
	}
}

void cleanUpDisplayMessages(uint timer) 
{
	for (int i = 0; i < MAX_DISPLAY_MESSAGES; i++) 
	{
		if (ctlMem->m_CartelChatDisplay[i].timestamp > 0 && timer - ctlMem->m_CartelChatDisplay[i].timestamp > (ctlMem->m_chat_msg_display_time_seconds * 1000))
		{
			ctlMem->m_CartelChatDisplay[i].timestamp = emptyMessage.timestamp;
			strcpy(ctlMem->m_CartelChatDisplay[i].message, emptyMessage.message, CHAT_MSG_SIZE);
			ctlMem->m_CartelChatDisplay[i].target_player = emptyMessage.target_player;
		}
	}
}

void cartel_chat_display()
{
	if (bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_CARTEL_CHAT) && bitTest(ctlMem->m_bit_sets.m_network, NETWORK_REGISTERED_CHAT_VARIABLES) && bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_SETTING_ACCEPTED_CHAT_RULES))
	{
		uint timer;
		GET_NETWORK_TIMER(&timer);
		cleanUpDisplayMessages(timer); 
		addNewMessageToDisplay(timer);        
		displayMessages();
	}
}