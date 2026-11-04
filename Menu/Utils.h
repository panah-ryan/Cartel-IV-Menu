#pragma once

typedef struct HashTable_s
{
	uint m_hash;
	void* m_pointer;
} HashTable;

typedef struct scrProgram
{
	char* ScriptName;
	uint HashCode;
	byte* Opcodes;
	uint* Statics;
	int OpcodeSize;
	short StaticSize;
	short ArgStructSize;
	short RefCount;
	short _padding;
} scrProgram;

typedef enum State
{
	RUNNING,
	BLOCKED,
	ABORTED,
	HALTED
} State;

typedef struct Serialized
{
	void* m_vftable;
	int m_ThreadId;
	uint m_Prog;
	State m_State;
	int m_PC;
	int m_FP;
	int m_SP;
	int m_TimerA;
	int m_TimerB;
	float m_Wait;
	int m_MinPC;
	int m_MaxPC;
	uint m_TLS[4];
	int m_StackSize;
	int m_CatchPC;
	int m_CatchFP;
	int m_CatchSP;
} Serialized;

typedef struct scrThread
{
	Serialized m_Serialized;
	uint* m_Stack;
	int _padding0;
	char* m_AbortReason;
	char m_ScriptName[24];
	int m_ForceCleanUpPC;
	int m_ForceCleanUpFP;
	int m_ForceCleanUpSP;
	_BOOL8 m_IsThisAMissionScript;
	_BOOL8 m_SafeForNetworkGame;
	_BOOL8 m_CanBeSaved;
	_BOOL8 m_SetPlayerControlOnInMissionCleanup;
	_BOOL8 m_ClearHelpInMissionCleanup;
	_BOOL8 m_IsThisAMiniGameScript;
	_BOOL8 m_ThisScriptAllowsNonMiniGameHelpMessages;
	_BOOL8 m_CanOneTimeCommandsRun;
	_BOOL8 m_ThreadHasBeenPaused;
	_BOOL8 m_ThisScriptCanBePaused;
	byte m_ScriptBrainType;
	char _padding1;
	int m_EntityIndexForScriptBrain;
	_BOOL8 m_ThisScriptCanRemoveBlipsCreatedByAnyScript;
	char _padding2[3];
	int _padding3;
	uint m_Flags;
} scrThread;

static_assert(sizeof(scrThread) == 0x9C, "sizeof scrThread is incorrect!");

scrProgram* scrProgram_GetProgram(uint hash);
scrThread* grab_scrthread(const char* script);
bool swap_script_threads(const char* from, const char* to);
vector2 convert_pixels_to_screenspace(float x, float y);
void SetBit(int* bitSet, int bitIndex, bool set);
bool setup_memory();
uint get_free_memory();
extern PLAYER_INDEX myPlayerIndex;
extern PED_INDEX myPlayerPed; 
extern int myCurrentEpisode;
extern bool NetworkGame;
extern bool CartelXEXPresent;