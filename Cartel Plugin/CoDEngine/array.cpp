#include "stdafx.h"
#include "Defines.h"
#include "array.h"
#include "natives.h"
#include "Detour.h"

//Protections...
int CNetworkArrayHandler_GetElementIndex(networkArrayHandler* handler, int index, int peer)
{
	handler->m_ShouldSkipMessage = false; //Lets clear our custom variable
	if (index >= handler->m_ArraySize || index < 0) //Index exceeds our array size
	{
		printf("[Array] - Invalid index %i would exceed bounds of %s array!\n", index, handler->GetHandlerName());
		attacker_list.push_back(std::make_tuple(peer, ATTACK_OUT_OF_BOUNDS_ARRAY, NULL));
		handler->m_ShouldSkipMessage = true; //set our custom variable to skip the message
		return 0; //return first index so any function pulling data should be safe
	}

	//Lets call the original code, just reversed since code is simple
	if (handler->m_Type == DISPATCH_ORDER_ARRAY_HANDLER || handler->m_Type == CLIENT_VARIABLES_ARRAY_HANDLER)
		return ((handler->m_ArraySize >> 4) * peer) + index;

	return index;
}

Detour<bool> CNetworkArrayHandler_CanApplyElementDataDetour;
bool CNetworkArrayHandler_CanApplyElementData(networkArrayHandler* handler, int index, int peer, bool empty)
{
	if (index == 0 && handler->m_ShouldSkipMessage) //We found a message to skip, lets say we can't apply the data
		return false;

	return CNetworkArrayHandler_CanApplyElementDataDetour.CallOriginal(handler, index, peer, empty); //Was safe so lets continue
}

Detour<bool> CPedGroupsArrayHandler_CanApplyElementDataDetour;
bool CPedGroupsArrayHandler_CanApplyElement(CPedGroupsArrayHandler* handler, int index, int peer, bool empty)
{
	if (index == 0 && handler->m_ShouldSkipMessage) //We found a message to skip, lets say we can't apply the data
		return false;

	return CPedGroupsArrayHandler_CanApplyElementDataDetour.CallOriginal(handler, index, peer, empty); //Was safe so lets continue
}

Detour<bool> CPedGroupsArrayHandler_ReadUpdateDetour;
bool CPedGroupsArrayHandler_ReadUpdate(CPedGroupsArrayHandler* handler, CMessage* buffer, int index)
{
	if (index >= 16 || (NetworkInterface::GetLocalPlayer()->GetPeerID() != index && NetworkInterface::AreWeInNetworkGame()))
	{
		CPlayerPed* ourPed = NetworkInterface::GetLocalPlayerPed();
		if (ourPed && ourPed->GetNetworkObject())
		{
			//skip reading created by and seperation value (not important)
			int seek_bits = buffer->m_buffer.m_CursorPos + buffer->m_buffer.m_BaseBitOffset + 2 + 7;
			for (int i = 0; i < 8; i++)
			{
				WORD netId = buffer->PeekShort(seek_bits);
				if (netId == ourPed->GetNetworkObject()->GetNetworkID()) //Network ID is the same as ours (means we are being added to a group)
				{
					if (index < 16 && NetworkInterface::GetNetPlayer(index)->IsPhysical())
						printf("[Ped Group] - Removing ourself from %s's group...\n", NetworkInterface::GetPlayerInfo(index)->GetPlayerName());
					else
						printf("[Ped Group] - Removing ourself from %i group...\n", index);

					attacker_list.push_back(std::make_tuple(index, ATTACK_GROUP_HACK, NULL));
					handler->SkipUpdate(buffer, index);
					return false; //added on Ryans instructon.
				}

				seek_bits += 12;
			}

			return CPedGroupsArrayHandler_ReadUpdateDetour.CallOriginal(handler, buffer, index);
		}
	}

	//Normally wouldn't run under these conditions so lets not either
	handler->SkipUpdate(buffer, index);
	return false;
}

struct freemode_client_vars_original
{
	int m_status;
	int _0x04;
	int _0x08;
	int m_readyUpStatus;
	int m_gameModeSuggestion;
	int m_voteToKickIndex;
};

struct freemode_client_vars_tlad
{
	int _0x00;
	int _0x04;
	int _0x08;
	int m_voteToKickIndex;
	int m_gameModeSuggestion;
	int _0x14;
	int _0x18;
};

struct freemode_client_vars_tbogt
{
	int _0x00;
	int _0x04;
	int _0x08;
	int m_voteToKickIndex;
	int m_gameModeSuggestion;
	int _0x14;
	int _0x18;
	int _0x1C;
	int _0x20;
	int _0x24;
	int _0x28;
	int _0x2C;
	int _0x30;
	int _0x34;
	int _0x38;
	int _0x3C;
	int _0x40;
	int _0x44;
	int _0x48;
	int _0x4C;
	int _0x50;
	int _0x54;
	int _0x58;
	int _0x5C;
	int _0x60;
	int _0x64;
	int _0x68;
	int _0x6C;
	int _0x70;
	int _0x74;
	int _0x78;
	int _0x7C;
};

Detour<bool> CScriptClientVariablesArrayHandler_ReadUpdateDetour;
bool CScriptClientVariablesArrayHandler_ReadUpdate(CScriptVariablesArrayHandler* handler, CMessage* message, int index)
{
	CScriptClientVariablesArrayHandler_ReadUpdateDetour.CallOriginal(handler, message, index);

	if (handler->m_Identifier == GAME_MODE_FREE_MODE)
	{
		int game_mode = GET_CURRENT_EPISODE();

		if (game_mode == EPISODE_ORIGINAL)
		{
			freemode_client_vars_original* vars = reinterpret_cast<freemode_client_vars_original*>(handler->m_Variables);
			int player_index = index / 6;
			int array_index = index % 6;

			if (array_index == 4 && (vars[player_index].m_gameModeSuggestion > GAME_MODE_FREE_MODE || vars[player_index].m_gameModeSuggestion < GAME_MODE_SINGLE_PLAYER))
			{
				if (NetworkInterface::GetNetPlayer(player_index)->IsPhysical())
					printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", NetworkInterface::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_gameModeSuggestion = GAME_MODE_SINGLE_PLAYER;
				attacker_list.push_back(std::make_tuple(player_index, ATTACK_SINGLEPLAYER_KICK, NULL));
			}
			else if (array_index == 5 && (vars[player_index].m_voteToKickIndex > 15 || vars[player_index].m_voteToKickIndex < -1))
			{
				if (NetworkInterface::GetNetPlayer(player_index)->IsPhysical())
					printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", NetworkInterface::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_voteToKickIndex = -1;
				attacker_list.push_back(std::make_tuple(player_index, ATTACK_SINGLEPLAYER_KICK, NULL));
			}
		}
		else if (game_mode == EPISODE_TLAD)
		{
			freemode_client_vars_tlad* vars = reinterpret_cast<freemode_client_vars_tlad*>(handler->m_Variables);
			int player_index = index / 7;
			int array_index = index % 7;

			if (array_index == 3 && (vars[player_index].m_voteToKickIndex > 24 || vars[player_index].m_voteToKickIndex < -1))
			{
				if (NetworkInterface::GetNetPlayer(player_index)->IsPhysical())
					printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", NetworkInterface::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_voteToKickIndex = -1;
				attacker_list.push_back(std::make_tuple(player_index, ATTACK_SINGLEPLAYER_KICK, NULL));
			}
			else if (array_index == 4 && (vars[player_index].m_gameModeSuggestion > GAME_MODE_FREE_MODE || vars[player_index].m_gameModeSuggestion < GAME_MODE_SINGLE_PLAYER))
			{
				if (NetworkInterface::GetNetPlayer(player_index)->IsPhysical())
					printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", NetworkInterface::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_gameModeSuggestion = GAME_MODE_SINGLE_PLAYER;
				attacker_list.push_back(std::make_tuple(player_index, ATTACK_SINGLEPLAYER_KICK, NULL));
			}
		}
		else if (game_mode == EPISODE_TBOGT)
		{
			freemode_client_vars_tbogt* vars = reinterpret_cast<freemode_client_vars_tbogt*>(handler->m_Variables);
			int player_index = index / 32;
			int array_index = index % 32;

			if (array_index == 3 && (vars[player_index].m_voteToKickIndex > 15 || vars[player_index].m_voteToKickIndex < -1))
			{
				if (NetworkInterface::GetNetPlayer(player_index)->IsPhysical())
					printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", NetworkInterface::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_voteToKickIndex = -1;
				attacker_list.push_back(std::make_tuple(player_index, ATTACK_SINGLEPLAYER_KICK, NULL));
			}
			else if (array_index == 4 && (vars[player_index].m_gameModeSuggestion > 30 || vars[player_index].m_gameModeSuggestion < GAME_MODE_SINGLE_PLAYER))
			{
				if (NetworkInterface::GetNetPlayer(player_index)->IsPhysical())
					printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", NetworkInterface::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_gameModeSuggestion = GAME_MODE_SINGLE_PLAYER;
				attacker_list.push_back(std::make_tuple(player_index, ATTACK_SINGLEPLAYER_KICK, NULL));
			}
		}
	}

	return true;
}