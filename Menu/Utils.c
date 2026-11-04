#include "stdafx.h"
#include "Utils.h"
#include "Functions.h"

PLAYER_INDEX myPlayerIndex;
PED_INDEX myPlayerPed;
int myCurrentEpisode;
bool NetworkGame;
bool CartelXEXPresent;

typedef struct Table_s
{
	HashTable* m_table;
	int m_count;
} Table;

byte* function_stub;
uint ctl_memory_buffer;
size_t ctl_memory_size;

vector2 convert_pixels_to_screenspace(float x, float y)
{
	vector2 v;

	v.x = x * (1.0f / 1280.0f);
	v.y = y * (1.0f / 720.0f);

	return v;
}

uint stub_function(void* value1, void* value2)
{
	__getFrame(0);
	__getFrame(1);
	__nop(5);
	return 1;
}

scrProgram* scrProgram_GetProgram(uint hash)
{
	Table table = *(Table*)0x82B07020;
	for (int i = 0; i < table.m_count; i++)
	{
		if (hash == table.m_table[i].m_hash)
			return (scrProgram*)table.m_table[i].m_pointer;
	}

	return nullptr;
}

scrThread* grab_scrthread(const char* script)
{
	uint* sm_Threads = *(uint**)0x83192788;
	short sm_ThreadsCount = *(short*)0x8319278C;
	uint hash = GET_HASH_KEY(script);

	for (int i = 0; i < sm_ThreadsCount; i++)
	{
		scrThread* thread = (scrThread*)sm_Threads[i];
		if (thread && thread->m_Serialized.m_Prog == hash)
			return thread;
	}

	return nullptr;
}

bool swap_script_threads(const char* from, const char* to)
{
	uint* sm_Threads = *(uint**)0x83192788;
	uint our_hash = GET_HASH_KEY(from);
	uint other_hash = GET_HASH_KEY(to);
	int other_index = -1;
	int our_index = -1;

	for (int i = 0; i < 138; i++)
	{
		scrThread* thread = (scrThread*)sm_Threads[i];
		if (thread)
		{
			if (thread->m_Serialized.m_Prog == our_hash)
				our_index = i;
			else if(thread->m_Serialized.m_Prog == other_hash)
				other_index = i;
		}
	}

	if (other_index != -1 && our_index != -1 && our_index > other_index)
	{
		uint our_value = sm_Threads[our_index];
		sm_Threads[our_index] = sm_Threads[other_index];
		sm_Threads[other_index] = our_value;
		return true;
	}

	return false;
}

/*void setup_function_pointers()
{
	scrThread* thread = grab_scrthread();
	if (thread)
	{
		uint prog_id = thread->m_Serialized.m_Prog;
		scrProgram* program = scrProgram_GetProgram(prog_id);

		if (program)
			function_stub = (byte*)((uint)stub_function + (uint)program->Opcodes);
	}
}

void pointer_call(void* address, void* param1, void* param2, uint* ret, int param_count)
{
	static uint ret_opcode = 0x30020100; //ret(2, 1)
	static uint void_opcode = 0x61300201; //push1 - ret(2, 1)

	if (!function_stub)
		return;

	uint pointer_address = SwapEndian32((int)address);
	function_stub[5] = param_count > 0 ? 49 : 44; //load : drop
	function_stub[7] = param_count > 1 ? 49 : 44; //load : drop
	function_stub[8] = 46; //call
	*(uint*)&function_stub[9] = pointer_address;
	*(uint*)&function_stub[13] = ret ? ret_opcode : void_opcode;
	uint ret_value = stub_function(param1, param2);

	if (ret)
		*ret = ret_value;
}*/

void SetBit(int* bitSet, int bitIndex, bool set)
{
	if (set)
		SET_BIT(&*bitSet, bitIndex);
	else
		CLEAR_BIT(&*bitSet, bitIndex);
}

bool setup_memory()
{
	//TODO: Write this address so we can read it if memory.sco is already loaded (if menu .sco restarts).
	uint params[2] = { (uint)&ctl_memory_buffer, (uint)&ctl_memory_size };

	if (!DOES_SCRIPT_EXIST("cartel_memory"))
	{
	//	printf("Heap script not found!");
		return false;
	}

	if (GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT("cartel_memory") > 0)
	{
		TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cartel_memory");
		TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cartel_helper"); //we call this function (setup_memory()) before we call start_cartel_helper()
	}

	REQUEST_SCRIPT("cartel_memory");
	while (!HAS_SCRIPT_LOADED("cartel_memory"))
		WAIT(0);

	//THREADID thread = START_NEW_SCRIPT_WITH_ARGS("cartel_memory", params, 2, 1024);
	freeMem->m_CartelMemory_ThreadId = START_NEW_SCRIPT_WITH_ARGS("cartel_memory", params, 2, 1024);
	MARK_SCRIPT_AS_NO_LONGER_NEEDED("cartel_memory");

	//while (thread && ctl_memory_buffer == NULL)
	while (freeMem->m_CartelMemory_ThreadId && ctl_memory_buffer == NULL)
		WAIT(0);

	if(ctl_memory_buffer)
		ctl_memory_buffer += -(ctl_memory_buffer % 4);

	//return thread;
	return freeMem->m_CartelMemory_ThreadId;
}

uint get_free_memory()
{
	return ctl_memory_buffer;
}

