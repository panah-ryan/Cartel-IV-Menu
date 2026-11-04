
#include "stdafx.h"
#include "Functions.h"
#include "MenuExecutionHandling.h"
#include "Utils.h"


int* ConvertHexStringToPointer(char* hexString) //works!
{
	int address = HexToIntNath(hexString);
	return (int*)address;
}

//	int* address = ConvertHexStringToPointer("8318AB38");
//	WriteStringHexToAddress("80008000", address);

//int* address = (int*)

//WriteStringHexToAddress("80008500", address);
//WriteStringHexToAddress("80008500", (int*)0x8318AB38);
void WriteStringHexToAddress(char* hexString, int* address) //works!
{
	int value = HexToIntNath(hexString);

	unsigned char* byteAddress = (unsigned char*)address;

	// Manually set the bytes for the integer value
	byteAddress[0] = (value >> 24) & 0xFF;
	byteAddress[1] = (value >> 16) & 0xFF;
	byteAddress[2] = (value >> 8) & 0xFF;
	byteAddress[3] = value & 0xFF;

	//printf("Hex string 0x%s written to memory address: .", hexString, (unsigned int)address);
}
/*
void MemorySetIntHex()  //works!
{
	if (GrabKeyboardInputString(8, 8, true, true, true, true))
	{
		WriteStringHexToAddress(ctlMem->m_char_buffers.keyboard_input_buffer, ctlMem->m_mem_editor.inputted_address);
		printf("~r~CTL: ~w~0x%s written to 0x.", ctlMem->m_char_buffers.keyboard_input_buffer, ctlMem->m_mem_editor.inputted_address_string);
	}
}
*/

void MemorySetIntDec() //works
{
	if (showAndGetKeyboardInput("Set int (Decimal)", false, "", 10, 0, 0.5f, 0.5f, 200, 0, 0))
	//if (GrabKeyboardInputString(10, 1, false, true, true, false, true))
	{
		int value;
		if (STRING_TO_INT(ctlMem->m_char_buffers.keyboard_input_buffer, &value))
		{
			//memcpy((int*)ctlMem->m_mem_editor.inputted_address, &value, 8); //keep this as 8, user must understand variable types or fu.
			memcpy((int*)ctlMem->m_mem_editor.inputted_address, &value, 4); 
			printf("~r~CTL: ~y~0x%s ~w~= ~y~%i", ctlMem->m_mem_editor.inputted_address_string, value);//: 0x%08X.\n", value, (unsigned int)address);
		}
		else {
			printf("~r~CTL: ~w~Failed to convert input string to integer.");
		}
	}
}

void MemoryReadInt() //works
{
	if (ctlMem->m_mem_editor.inputted_address != NULL)
	{
		int address = *(int*)ctlMem->m_mem_editor.inputted_address;
		printf("~r~CTL: ~y~0x%s ~w~== ~y~%i.", ctlMem->m_mem_editor.inputted_address_string, address);
	}
	else
		printf("~r~CTL: ~w~Invalid memory address.");
}

void MemorySetByteDec() 
{
	if (showAndGetKeyboardInput("Set byte (Decimal)", false, "", 3, 0, 0.5f, 0.5f, 200, 0, 0))
	//if (GrabKeyboardInputString(3, 1, false, true, true, false, true))
	{
		int value;
		if (STRING_TO_INT(ctlMem->m_char_buffers.keyboard_input_buffer, &value))
		{
			memcpy((byte*)ctlMem->m_mem_editor.inputted_address, &value, 1); //copy 1 byte
			printf("~r~CTL: ~y~0x%s ~w~= ~y~%i", ctlMem->m_mem_editor.inputted_address_string, value);//: 0x%08X.\n", value, (unsigned int)address);
		}
		else {
			printf("~r~CTL: ~w~Failed to convert input string to integer.");
		}
	}
}

void MemoryReadByte() 
{
	if (ctlMem->m_mem_editor.inputted_address != NULL)
	{
		byte address = *(byte*)ctlMem->m_mem_editor.inputted_address;
		printf("~r~CTL: ~y~0x%s ~w~== ~y~%i.", ctlMem->m_mem_editor.inputted_address_string, address);
	}
	else
		printf("~r~CTL: ~w~Invalid memory address.");
}

void MemorySetShortDec()
{
	if (showAndGetKeyboardInput("Set short (Decimal)", false, "", 5, 0, 0.5f, 0.5f, 200, 0, 0))
	//if (GrabKeyboardInputString(5, 1, false, true, true, false, true))
	{
		int value;
		if (STRING_TO_INT(ctlMem->m_char_buffers.keyboard_input_buffer, &value))
		{
			memcpy((short*)ctlMem->m_mem_editor.inputted_address, &value, 2); //copy 2 bytes
			printf("~r~CTL: ~y~0x%s ~w~= ~y~%i", ctlMem->m_mem_editor.inputted_address_string, value);//: 0x%08X.\n", value, (unsigned int)address);
		}
		else {
			printf("~r~CTL: ~w~Failed to convert input string to integer.");
		}
	}
}

void MemoryReadShort()
{
	if (ctlMem->m_mem_editor.inputted_address != NULL)
	{
		short address = *(short*)ctlMem->m_mem_editor.inputted_address;
		printf("~r~CTL: ~y~0x%s ~w~== ~y~%i.", ctlMem->m_mem_editor.inputted_address_string, address);
	}
	else
		printf("~r~CTL: ~w~Invalid memory address.");
}

void MemoryReadString()  //WORKS
{
	if (ctlMem->m_mem_editor.inputted_address != NULL)
	{
		char* address = (char*)ctlMem->m_mem_editor.inputted_address;

		if (address[0] == '\0')
			printf("~r~CTL: ~y~0x%s ~w~Contains an empty string.", ctlMem->m_mem_editor.inputted_address_string);
		else
			printf("~r~CTL: ~y~0x%s ~w~== ~y~%s", ctlMem->m_mem_editor.inputted_address_string, address);
	}
	else
		printf("~r~CTL: ~w~Invalid memory address.");
}

void MemoryWriteString() //works
{
	if (showAndGetKeyboardInput("Write string", false, "", 64, 0, 0.5f, 0.5f, 200, 0, 0))
	//if (GrabKeyboardInputString(64, 1, false, false, false, false, true))
	{
		if (ctlMem->m_mem_editor.inputted_address != NULL)
		{
			strcpy((char*)ctlMem->m_mem_editor.inputted_address, ctlMem->m_char_buffers.keyboard_input_buffer, 64); //MaxKeyboardInputLength ?
			printf("~r~CTL: ~y~0x%s ~w~= ~y~%s", ctlMem->m_mem_editor.inputted_address_string, ctlMem->m_char_buffers.keyboard_input_buffer);
		}
		else
			printf("~r~CTL: ~w~Invalid memory address.");
	}
}

void MemoryInputAddress()  //Works
{
	//if (GrabKeyboardInputString(8, 8, true, true, true, true, true)) 
	if (showAndGetKeyboardInput("Input memory address", true, "", 8, 0, 0.5f, 0.5f, 200, 0, 0))
	{
		unsigned int specifiedAddress = HexToIntNath(ctlMem->m_char_buffers.keyboard_input_buffer);
		int* convertedAddress = ConvertHexStringToPointer(ctlMem->m_char_buffers.keyboard_input_buffer);

		if (convertedAddress != NULL)
		{
			ctlMem->m_mem_editor.inputted_address = convertedAddress;
			printf("~r~CTL: ~w~Address 0x%s specified.", ctlMem->m_char_buffers.keyboard_input_buffer);
			for (int i = 0; i < 8; i++)
				ctlMem->m_mem_editor.inputted_address_string[i] = ctlMem->m_char_buffers.keyboard_input_buffer[i];
		}
		else
			printf("~r~CTL: ~w~Invalid memory address.");
	}
}


void MemoryOptions()
{
	SetHeader("Memory Editor");
	AddItem("Input Address (hex)", MemoryInputAddress, nullptr); //works
	AddItem("Read string", MemoryReadString, nullptr); //works
	AddItem("Write string", MemoryWriteString, nullptr); //works
	AddItem("Read integer (dec)", MemoryReadInt, nullptr); //works
	AddItem("Write integer (dec)", MemorySetIntDec, nullptr); //works 
	AddItem("Read short (dec)", MemoryReadShort, nullptr); 
	AddItem("Write short (dec)", MemorySetShortDec, nullptr); 
	AddItem("Read byte (dec)", MemoryReadByte, nullptr);
	AddItem("Write byte (dec)", MemorySetByteDec, nullptr); 
	//AddItem("Set int (hex) - works if full int", MemorySetIntHex, nullptr); //works if whole value but meh.
}