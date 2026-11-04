#pragma once

//extern char ctlMem->m_char_buffers.keyboard_input_buffer[128]; //where did MAX_KEYBOARD_INPUT_SIZE go?
void MemoryOptions();

typedef struct ctl_mem_editor
{
	int* inputted_address;
	char inputted_address_string[9];
}ctl_mem_editor;
