/*H**********************************************************************
* FILENAME :	natives32.h					START DATE :	10 Sept 16
*
* DESCRIPTION :
*		Native functions to be used with all RDR targets and platforms of SC-CL.
*
* NOTES :
*		This file is part of SC-CL's include library.
*
* LICENSE :
*
*		Copyright 2016 SC-CL
*
*		Redistribution and use in source and binary forms, with or without
*		modification, are permitted provided that the following conditions are met:
*
*		* Redistributions of source code must retain the above copyright
*		notice, this list of conditions and the following disclaimer.
*
*		* Redistributions in binary form must reproduce the above copyright
*		notice, this list of conditions and the following disclaimer in the
*		documentation and/or other materials provided with the distribution.
*
*		* Neither SC-CL nor its contributors may be used to endorse or promote products
*		derived from this software without specific prior written permission.
*
*		* Redistribution of this software in source or binary forms shall be free
*		of all charges or fees to the recipient of this software.
*
*		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*		ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*		WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*		DISCLAIMED. IN NO EVENT SHALL SC-CL BE LIABLE FOR ANY
*		DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*		(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*		LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*		SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* AUTHORS :
*		Rocko Tompkins
*		Nathan James
*H*/
#pragma once
#include "types.h"
#include "constants.h"

//Fix for intellisense nonsense
#ifndef _MSC_VER
#define _native __attribute((native))
#define _native32(hash) __attribute((native(hash)))
#define l ;
#else
#define _native
#define _native32(hash)
#define l {};
#endif

#define SIN SIN_DEGREE
#define COS COS_DEGREE
#define TAN TAN_DEGREE
#define ATAN ATAN_DEGREE
#define ATAN2 ATAN2_DEGREE

#pragma region CUSTOM
//PURPOSE: Fault script with said message, function name, file name, and file line
extern _native void SCRIPT_ASSERT(const char* message, const char* func_name, const char* file_name, int file_line)l
#pragma endregion

#pragma region BUILTINS
extern _native void WAIT(int ms)l
extern _native void WAITUNWARPED(int ms)l
extern _native void WAITUNPAUSED(int ms)l
extern _native THREADID START_NEW_SCRIPT(const char* scriptName, int stacksize)l
extern _native THREADID START_NEW_SCRIPT_WITH_ARGS(const char* scriptname, uint* params, int paramcount, int stacksize)l
extern _native void SETTIMERA(int value)l
extern _native float TIMESTEP()l
extern _native void PRINTSTRING(const char* value)l
extern _native void PRINTFLOAT(float value)l
extern _native void PRINTINT(int value)l
extern _native void PRINTNL(void)l
extern _native void PRINTVECTOR(float x, float y, float z)l
extern _native float SQRT(float value)l
extern _native float POW(float a, float b)l
extern _native float EXP(float f)l
extern _native float VMAG(float x, float y, float z)l
extern _native float VDIST(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native float VDIST2(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native int SHIFT_LEFT(int val, int shifts)l
extern _native int SHIFT_RIGHT(int val, int shifts)l
extern _native float FLOOR(float value)l
extern _native float CEIL(float value)l
extern _native float ROUND(float f)l
extern _native float TO_FLOAT(int value)l
extern _native void SNAPSHOT_GLOBALS(void)l

//GET_LATEST_CONSOLE_COMMAND
//RESET_LATEST_CONSOLE_COMMAND
//GET_CONSOLE_COMMAND_TOKEN
//GET_NUM_CONSOLE_COMMAND_TOKENS
#pragma endregion

#pragma region AMBIENCE_AUDIO_ENTITY
extern _native void AMBIENCE_AUDIO_ENTITY_UPDATE_TERRITORY(const char* pram0)l
extern _native void AMBIENCE_AUDIO_ENTITY_UPDATE_LOCATION(int pram0, int pram1)l
extern _native void AMBIENCE_AUDIO_VALIDATE_REGION(int pram0, int pram1)l
#pragma endregion

#pragma region DYNAMIC_MIXER
extern _native void DYNAMICMIXER_TRIGGERSTATE()l
extern _native void DYNAMICMIXER_TRIGGERSTATE_PERSISTENT(int pram0, int pram1)l
extern _native void DYNAMICMIXER_DETRIGGERSTATE(int pram0, int pram1)l
extern _native32(0xADCC16A2) void _DYNAMICMIXER_TRIGGERSTATE_PREDUEL()l
#pragma endregion

#pragma region PED_AUDIO_MANAGER
extern _native int CAN_ACTOR_ENUM_PLAY_SPEECH_CONTEXT(int pram0, int pram1)l
extern _native void REGISTER_ACTOR_SPEECH_CONTEXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void FINISH_REGISTERING_ACTOR_SPEECH_CONTEXTS()l
extern _native void REGISTER_NUM_SPEECH_CONTEXTS(int pram0)l

//REGISTER_NUM_CONTEXT_TYPES
#pragma endregion

#pragma region AUDIO
extern _native int NEW_SCRIPTED_CONVERSATION()l
extern _native void ADD_LINE_TO_CONVERSATION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void ADD_NEW_CONVERSATION_SPEAKER(int pram0, int pram1, int pram2)l
extern _native void ADD_NEW_FRONTEND_CONVERSATION_SPEAKER(int pram0, int pram1)l
extern _native void START_SCRIPT_CONVERSATION(int pram0, int pram1)l
extern _native int IS_SCRIPTED_CONVERSATION_ONGOING()l
extern _native void PAUSE_SCRIPTED_CONVERSATION()l
extern _native void RESTART_SCRIPTED_CONVERSATION()l
extern _native int ABORT_SCRIPTED_CONVERSATION(int pram0)l
extern _native void SET_MAX_SCRIPTED_CONVERSATION_DISTANCE()l
extern _native void RESET_MAX_SCRIPTED_CONVERSATION_DISTANCE()l
extern _native void HAS_SCRIPTED_CONVERSATION_PLAYED_RECENTLY()l
extern _native void UNREGISTER_SCRIPT_WITH_AUDIO()l
extern _native int REQUEST_MISSION_AUDIO_BANK(const char* pram0)l
extern _native void MISSION_AUDIO_BANK_NO_LONGER_NEEDED()l
extern _native void AMBIENT_AUDIO_BANK_NO_LONGER_NEEDED()l
extern _native int GET_SOUND_ID()l
extern _native void RELEASE_SOUND_ID(int pram0)l
extern _native int IS_SOUND_ID_VALID(int pram0)l
extern _native void PLAY_SOUND(int pram0)l
extern _native void PLAY_SOUND_INIT_PARAMS()l
extern _native void PLAY_SOUND_INIT_PARAMS_PERSISTENT()l
extern _native void PLAY_SOUND_FRONTEND(const char* SoundName)l
extern _native void PLAY_SOUND_FRONTEND_INITPARAMS(int pram0, int pram1)l
extern _native void PLAY_SOUND_FRONTEND_PERSISTENT(int pram0, int pram1)l
extern _native void PLAY_SOUND_FROM_POSITION(const char* SoundName, vector3 Position)l
extern _native void PLAY_SOUND_FROM_POSITION_INITPARAMS()l
extern _native void PLAY_SOUND_FROM_POSITION_PERSISTENT(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void PLAY_WALLA_SOUND_FROM_POSITION_PERSISTENT(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int PLAY_SOUND_FROM_ACTOR(int pram0, int pram1, int pram2)l
extern _native int PLAY_WALLA_SOUND_FROM_ACTOR(int pram0, int pram1, int pram2)l
extern _native int PLAY_SOUND_FROM_OBJECT(int pram0, int pram1)l
extern _native void AUDIO_SET_VEHICLE_DAMAGED()l
extern _native void STOP_SOUND(int pram0)l
extern _native int HAS_SOUND_FINISHED(int pram0)l
extern _native void AUDIO_ATTACH_MICROPHONE_TO_ACTOR(int pram0)l
extern _native void AUDIO_SET_MISSION_NAME_FOR_JOURNAL(int pram0)l
extern _native void AUDIO_CLEAR_MISSION_NAME_FOR_JOURNAL()l
extern _native void AUDIO_RESET_SPEECH_HISTORY()l
extern _native void AUDIO_RESET_SCRIPTED_SPEECH_HISTORY()l
extern _native void AUDIO_HAS_CONVERSATION_PLAYED_ALREADY()l
extern _native void AUDIO_CLEAR_CONVERSATION_HISTORY()l
extern _native void SAY_SINGLE_LINE_CONTEXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void SAY_SINGLE_LINE_STRING(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void SAY_SINGLE_LINE_STRING_WITH_REPLY(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void SAY_SINGLE_LINE_CONTEXT_OVER_PAIN(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native void SAY_SINGLE_LINE_STRING_OVER_PAIN(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void SAY_SINGLE_LINE_STRING_WITH_REPLY_OVER_PAIN(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void SAY_SINGLE_LINE_STRING_THROUGH_BLOCKED_SPEECH()l
extern _native void SAY_SINGLE_LINE_STRING_WITH_BACKUPS(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void SAY_SINGLE_LINE_SCRIPTED(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native int SAY_SINGLE_LINE_STRING_BEAT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native int SAY_SINGLE_LINE_STRING_SCRIPTED_INTERRUPT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11, int pram12)l
extern _native void AUDIO_PLAY_VOCAL_EFFECT(int pram0, int pram1, int pram2)l
extern _native void AUDIO_PLAY_VOCAL_EFFECT_EVEN_IF_DEAD()l
extern _native void AUDIO_PLAY_PAIN(int pram0, int pram1)l
extern _native void AUDIO_TRIGGER_PLAYER_KILLED_EVERYONE_SPEECH()l
extern _native void TREAT_AMBIENT_SPEECH_AS_SCRIPTED()l
extern _native void SET_AMBIENT_VOICE_NAME(int pram0, int pram1)l
extern _native void CLEAR_ALTERNATE_SPEECH_CONTEXT_FOR_PAIN(int pram0)l
extern _native void CANCEL_CURRENTLY_PLAYING_AMBIENT_SPEECH(int pram0)l
extern _native int IS_AMBIENT_SPEECH_PLAYING(int pram0)l
extern _native int IS_SCRIPTED_SPEECH_PLAYING(int pram0)l
extern _native int IS_ANY_SPEECH_PLAYING(int pram0)l
extern _native void AUDIO_STOP_PAIN(int pram0)l
extern _native void AUDIO_PLAY_PLAYER_HOGTIE_LINE(int pram0)l
extern _native void AUDIO_TRIGGER_PLAYER_LOOTING_MALE_SPEECH()l
extern _native void AUDIO_TRIGGER_PLAYER_LOOTING_FEMALE_SPEECH()l
extern _native void AUDIO_TRIGGER_PLAYER_LOOTING_FEMALE_ZOMBIE_SPEECH()l
extern _native void AUDIO_TRIGGER_PLAYER_LOOTING_MALE_ZOMBIE_SPEECH()l
extern _native void AUDIO_SHUT_OFF_WALLA()l
extern _native void AUDIO_TURN_ON_WALLA()l
extern _native void SET_LOCAL_PLAYER_VOICE(int pram0)l
extern _native void SET_LOCAL_PLAYER_PAIN_VOICE(int pram0)l
extern _native void AUDIO_TURN_OFF_PAIN_VOCALS()l
extern _native void AUDIO_TURN_ON_PAIN_VOCALS()l
extern _native void AUDIO_TURN_OFF_VOCALS_EFFECTS()l
extern _native void AUDIO_SET_PLAYER_MOOD(int pram0, int pram1)l
extern _native void AUDIO_ALLOW_PREDUEL_SPEECH(int pram0)l
extern _native void AUDIO_DISALLOW_PREDUEL_SPEECH(int pram0)l
extern _native void AUDIO_CLEAR_PLAYER_DISABLED_CONTEXT_LIST()l
extern _native void AUDIO_ADD_TO_PLAYER_DISABLED_CONTEXT_LIST(int pram0)l
extern _native void AUDIO_DISABLE_PLAYER_TAUNTS()l
extern _native void AUDIO_ENABLE_PLAYER_TAUNTS()l
extern _native void AUDIO_INIT_CAUCASIAN_ARMY_AE_RANGE(int pram0, int pram1)l
extern _native void AUDIO_INIT_MEXICAN_ARMY_AE_RANGE(int pram0, int pram1)l
extern _native void AUDIO_INIT_MISSION_CHARACTER_AE_RANGE(int pram0, int pram1)l
extern _native void AUDIO_INIT_RCM_CHARACTER_AE_RANGE(int pram0, int pram1)l
extern _native void AUDIO_INIT_FAC_INVALID_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_CATTLE_RUSTLER_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_DRUNKNDIRTY_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_GENERIC_CRIMINAL_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_INDIAN_LAW_ENFORCEMENT_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_INDIAN_RAIDER_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_LAW_ENFORCEMENT_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_MEXICAN_BANDITO_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_MEXICAN_LAW_ENFORCEMENT_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_MEXICAN_REBEL_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_MEXICAN_SOLDIER_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_SMUGGLERS_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_FAC_U_S_LAW_ENFORCEMENT_VALUE(int pram0)l
extern _native void AUDIO_INIT_ZOMBIE_BRUISER_AE_RANGE(int pram0, int pram1)l
extern _native void AUDIO_INIT_ZOMBIE_FAST_AE_RANGE(int pram0, int pram1)l
extern _native void AUDIO_INIT_ZOMBIE_TOXIC_AE_RANGE(int pram0, int pram1)l
extern _native void AUDIO_INIT_ZOMBIE_MP_PLAYER_AE_RANGE(int pram0, int pram1)l
extern _native void AUDIO_INIT_ZOMBIE_MP_FEMALE_VALUE(int pram0)l
extern _native void AUDIO_INIT_FAC_ZOMBIE_VALUE(int pram0)l
extern _native void AUDIO_SET_CURRENT_AREA_IS_UNDER_ZOMBIE_ATTACK(int pram0)l
extern _native void AUDIO_INIT_ZOMBIE_ZONE()l
extern _native void AUDIO_INIT_ZOMBIE_PACK_INFO()l
extern _native void AUDIO_INIT_NUN_AE_RANGE(int pram0, int pram1, int pram2, int pram3)l
extern _native void AUDIO_SET_MAX_CREATURE_FOOTSTEP_UPDATES()l
extern _native void AUDIO_SET_GLOBAL_LAW_VALUES(int pram0, int pram1, int pram2)l
extern _native void STOP_PED_SPEAKING(int pram0, int pram1)l
extern _native void AUDIO_MISSION_INIT()l
extern _native void AUDIO_MISSION_RELEASE()l
extern _native void AUDIO_PLAY_DISTANT_THUNDER()l
extern _native void AUDIO_MUSIC_FORCE_TRACK(const char* SongName, const char* Mood, int pram2, int pram3, int pram4, float pram5, int pram6)l
extern _native void AUDIO_MUSIC_FORCE_TRACK_HASH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void AUDIO_MUSIC_SCRIPT_LOOPING()l
extern _native void AUDIO_MUSIC_SCRIPT_LOOPING_HASH()l
extern _native void AUDIO_MUSIC_SET_MOOD(const char* Mood, int pram1, int pram2, int pram3)l
extern _native void AUDIO_MUSIC_ONE_SHOT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int AUDIO_MUSIC_PLAY_PREPARED()l
extern _native int AUDIO_MUSIC_GET_RANDOM_TRACK_FROM_PLAYLIST(int pram0)l
extern _native int AUDIO_MUSIC_IS_PREPARED()l
extern _native void AUDIO_MUSIC_SET_STATE(int pram0)l
extern _native void AUDIO_MUSIC_RELEASE_CONTROL(int pram0, int pram1)l
extern _native void AUDIO_MUSIC_SUSPEND(int pram0)l
extern _native void AUDIO_MUSIC_RESUME()l
extern _native void AUDIO_IS_MUSIC_PLAYING()l
extern _native int AUDIO_IS_SCRIPTED_MUSIC_PLAYING()l
extern _native void AUDIO_IS_FRONTEND_MUSIC_PLAYING()l
extern _native void AUDIO_MUSIC_SET_SUSPENSE_ALLOWED()l
extern _native void AUDIO_MG_START()l
extern _native void AUDIO_MG_STOP()l
extern _native void AUDIO_MG_FILLET_START(int pram0, int pram1, int pram2)l
extern _native void AUDIO_MG_FILLET_HIT_FINGER()l
extern _native void AUDIO_MG_FILLET_HIT_TABLE()l
extern _native void AUDIO_MG_FILLET_PULL_FROM_TABLE()l
extern _native void AUDIO_MG_FILLET_END()l
extern _native void AUDIO_ADD_ALTERNATE_CONTEXT(int pram0, int pram1, int pram2)l
extern _native void AUDIO_CLEAR_ALL_ALTERNATE_CONTEXTS()l
extern _native int LOAD_AUDIO_METADATA(int pram0, int pram1, int pram2)l
extern _native void UNLOAD_AUDIO_METADATA()l
extern _native int LOAD_AUDIO_SPEECH_DATA(int pram0)l
extern _native void SET_AUDIO_SEARCH_PATH()l
extern _native bool GET_AUDIO_BANK_LOADING_STATUS(const char* RefName, const char* BankPath)l
extern _native void LOAD_AUDIO_BANK(const char* RefName, const char* BankPath)l
extern _native void MAKE_AUDIO_SLOT_STATIC(const char* pram0)l
extern _native32(0xBAEC56D1) void _AUDIO_SET_MS_BETWEEN_PLAYER_TALKS_TO_SELF(int pram0)l

//AUDIO_PLAY_ALL_LINES_FOR_ACTOR
#pragma endregion

#pragma region CORE
extern _native void DISABLE_ACTOR_REFCOUNTING(int pram0)l
extern _native const char* GET_SCRIPT_NAME()l
extern _native int GET_SHORT_SCRIPT_NAME()l
extern _native void TERMINATE_THIS_SCRIPT()l
extern _native int GET_THIS_SCRIPT_ID()l
extern _native bool IS_SCRIPT_VALID(int ScriptID)l
extern _native bool DOES_SCRIPT_EXIST(const char* ScriptPath)l
extern _native int IS_EXITFLAG_SET()l
extern _native32(0x4417C9F2) int _IS_ANY_NAMED_SCRIPT_RUNNING(int pram0)l
extern _native32(0x5719022) void _TERMINATE_ALL_CHILD_SCRIPTS(int pram0)l
extern _native void TERMINATE_SCRIPT(int ScriptID)l
extern _native void ADD_PERSISTENT_SCRIPT(int pram0)l
extern _native void REMOVE_PERSISTENT_SCRIPT(int pram0)l
extern _native int SCRIPT_MAX_ALLOWED_INSTRUCTIONS()l
extern _native int SCRIPT_USED_INSTRUCTIONS()l
extern _native int SCRIPT_REMAINING_INSTRUCTIONS()l
extern _native void RAND_SET_SEED(int pram0)l
extern _native int RAND_INT_RANGE(int min, int max)l
extern _native float RAND_FLOAT_RANGE(float Min, float Max)l
extern _native int RAND_INT_RANGE_DIFFERENT(int pram0, int pram1, int pram2)l
extern _native int RAND_FLOAT_GAUSSIAN(int pram0, int pram1)l
extern _native void FILE_START_PATH(int pram0)l
extern _native void FILE_ADD_TO_PATH(int pram0)l
extern _native void FILE_END_PATH()l
extern _native int FILE_GET_CURRENT_PATH()l
extern _native int GET_X(int pram0)l
extern _native int GET_Y(int pram0)l
extern _native int GET_Z(int pram0)l
extern _native void VNORMALIZE(int pram0)l
extern _native void VCROSS(int pram0, int pram1, int pram2)l
extern _native int VDOT(int pram0, int pram1)l
extern _native void VSCALE(vector3* Vector, float Scale)l
extern _native32(0xF1A53C41) void _VDIRECTION_VECTOR_FROM_EULERS(int pram0, int pram1, int pram2, int pram3)l
extern _native32(0xF76F2BB3) void _VROTATE_VECTOR_FROM_EULERS(int pram0, int pram1, int pram2, int pram3)l
extern _native32(0x65DAA654) void _VROTATE_EULER_FROM_EULERS(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void ROTATE_VECTOR_XZ(int pram0, int pram1, int pram2)l
extern _native32(0x141201A3) void _CONSTRUCT_MATRIX_AND_TRANSFORM()l
extern _native float FABS(float f)l
extern _native int ABS(int pram0)l
extern _native float SIN_DEGREE(float x)l
extern _native float COS_DEGREE(float x)l
extern _native float TAN_DEGREE(float x)l
extern _native float ATAN_DEGREE(float x)l
extern _native float ATAN2_DEGREE(float x, float y)l
extern _native32(0x9C40E671) int _GET_VECTOR_HEADING_DEGS(int pram0)l
extern _native32(0xADF7D54B) int _GET_VECTOR_HEADING_RADS(int pram0)l
extern _native int GET_ANGLE_BETWEEN_VECTORS_DEGS(int pram0, int pram1)l
extern _native void VECTOR_FROM_HEADING_DEGS(int pram0, int pram1)l
extern _native32(0xBB3CDF72) void _TRANSFORM_WORLD_TO_SCREEN()l
extern _native int GET_CURRENT_GAME_TIME()l
extern _native int GET_CURRENT_UNWARPED_TIME()l
extern _native int GET_SYSTEM_TIME()l
extern _native int GET_TIMESTAMP()l
extern _native int GET_UTC_TIME()l
extern _native int GET_UNWARPED_REALTIME_SECONDS()l
extern _native int GET_PROFILE_TIME()l
extern _native int GET_LAST_FRAME_TIME()l
extern _native void LOG_MESSAGE(char* message)l
extern _native void LOG_WARNING(char* message)l
extern _native void LOG_ERROR(char* message)l
extern _native int GET_TARGET_ACTOR()l
extern _native void GRINGO_SET_TARGET_OBJECT(int pram0, int pram1, int pram2)l
extern _native int GET_TARGET_OBJECT()l
extern _native int IS_GRINGO_VALID(int pram0)l
extern _native void GET_GRINGO_ACTIVATION_SPHERE(int pram0, int pram1, int pram2)l
extern _native void DISABLE_GRINGO_STREAMING_CHECKS()l
extern _native void ENABLE_GRINGO_STREAMING_CHECKS()l
extern _native void GRINGO_ALLOW_ACTIVATION(int pram0, int pram1)l
extern _native int GRINGO_IS_ACTIVATION_ALLOWED(int pram0)l
extern _native bool IS_DEBUGKEY_DOWN(int keyCode)l
extern _native bool IS_DEBUGKEY_PRESSED(eKey KeyCode)l
extern _native bool IS_BUTTON_DOWN(Controller Controller, eButton Button, int Unk1, int Unk0)l
extern _native bool IS_BUTTON_PRESSED(Controller Controller, eButton Button, int Unk1, int Unk0)l
extern _native bool IS_BUTTON_RELEASED(Controller Controller, eButton Button, int Unk1, int Unk0)l
extern _native float GET_ANALOG_BUTTON_VALUE(Controller Controller, eButton Analog, bool Unk1)l
extern _native float GET_STICK_X(Controller Controller, bool IsRightStick, int unk1)l
extern _native float GET_STICK_Y(Controller Controller, bool IsRightStick, int unk1)l
extern _native void RESET_TIME_SINCE_LAST_INPUT(int pram0)l
extern _native int GET_TIME_SINCE_LAST_INPUT(int pram0)l
extern _native int GET_TIME_SINCE_LAST_MOVESTICK_INPUT(int pram0)l
extern _native int GET_TIME_SINCE_LAST_BUTTON_INPUT(int pram0)l
extern _native void SET_DEBUG_FADE_STATE(int pram0)l
extern _native void RETRIEVE_GAME_STATE(int pram0, int pram1, int pram2)l
extern _native void STORE_GAME_STATE(int pram0, int pram1, int pram2)l
extern _native void RESET_STORED_DATA()l
extern _native void RESET_GAME()l
extern _native int IS_GAME_RESETTING()l
extern _native int GET_CMD_LINE_PARAM(int pram0, int pram1)l
extern _native int GET_DISTRICTS_NAME()l
extern _native int LOAD_GAME(int pram0)l
extern _native int LOAD_SOFT_SAVE(int pram0)l
extern _native int SAVE_GAME(int pram0)l
extern _native void SAVE_SOFT_SAVE(int pram0)l
extern _native void SAVE_MANAGER_HARD_SAVE(int pram0)l
extern _native int SAVE_MANAGER_HARD_LOAD(int pram0)l
extern _native void SAVE_MANAGER_CREATE_SAVE_DISPLAY_NAME(int pram0)l
extern _native void SAVE_MANAGER_REGISTER_DATA(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SAVE_MANAGER_SET_SAVE_VERSION(int pram0)l
extern _native void SAVE_MANAGER_SET_SAVE_VERSION_FOR_TYPE(int pram0, int pram1)l
extern _native void SAVE_MANAGER_CREATE_SAVE_FILE(int pram0, int pram1, int pram2)l
extern _native void SAVE_MANAGER_REGISTER_STATS(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SAVE_MANAGER_REGISTER_PROFILE_STATS(int pram0, int pram1)l
extern _native int SAVE_MANAGER_IS_SAVING_DISABLED()l
extern _native int DOES_FILE_EXIST(int pram0)l
extern _native32(0xB427CB25) void _ARE_BUMPER_BUTTONS_SWAPPED()l
extern _native int IS_DISK_CACHE_PRIMED()l
extern _native int IS_PLAYER_SIGNED_IN()l
extern _native int GET_LOCAL_PLAYER_NAME()l
extern _native int GET_NUM_WORLD_CAMERAS()l
extern _native int GET_WORLD_CAMERA_AT_INDEX(int pram0, int pram1, int pram2)l
extern _native int GET_CLOSEST_WORLD_CAMERA(int pram0, int pram1, int pram2, int pram3)l
extern _native void PAUSE_GAME(int pram0)l
extern _native void UNPAUSE_GAME()l
extern _native int IS_GAME_PAUSED()l
extern _native void SET_SCRIPT_CUTSCENE_ACTIVE(int pram0)l
extern _native void SET_MISSION_INFO(int pram0, int pram1)l
extern _native void CLEAR_MISSION_INFO()l
extern _native void GET_COMMANDLINE_START_POS(int pram0, int pram1)l
extern _native void SET_START_POS(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int IS_STARTPOS_IN_COMMANDLINE()l
extern _native32(0x3B004817) int _WAS_LAST_RESET_FOR_MULTIPLAYER()l
extern _native void SCRIPT_DONE_LOADING()l
extern _native int LAUNCH_NEW_SCRIPT(const char* ScriptDir, int Unk0)l
extern _native int LAUNCH_NEW_SCRIPT_WITH_ARGS(int pram0, int pram1, int pram2, int pram3)l
extern _native int IS_LAUNCH_RETAIL()l
extern _native int IS_SIMULATE_START_PRESS()l
extern _native int IS_SIMULATE_START_MULTIPLAYER()l
extern _native int IS_DISPLAY_WIDESCREEN()l
extern _native int GET_EXP_MODE_PROMPT_STATE()l
extern _native void SET_EXP_MODE_PROMPT_STATE(int pram0)l
extern _native int GET_GAME_EDITION()l

//SET_DEBUG_DRAW
//GET_DEBUG_DRAW_STATE
//UNK_0x6DE957C6
//SCRIPT_BREAKPOINT
//GRINGO_DEBUG_CHECK_SOUND
//DEBUG_DRAW_VECTOR
//DEBUG_DRAW_LINE
//DEBUG_DRAW_SPHERE
//DEBUG_DRAW_STRING
//DRAW_STRING_CURRENT_FONT
//UNK_0xD1D88EB8
//UNK_0x21E19CD5
//UNK_0x1B6FE39B
//UNK_0xAAEBAC28
//SCRIPT_GETTESTREGION
//SCRIPT_GETTESTMISSION
//SCRIPT_GETTESTSCRIPT
//SCRIPT_GETTESTSCRIPTCONTINUEMARK
//SCRIPT_SETTESTSCRIPTCONTINUEMARK
//SCRIPT_WANTAUTOMATION
//SCRIPT_GETTESTTYPE
//SAVE_MANAGER_IS_SP_SAVING_DISABLED
//WRITE_TO_FILE
//IS_DEV_BUILD
//IS_PS3
#pragma endregion

#pragma region PLAY_STATS
extern _native void PLAYSTAT_INT(int pram0, int pram1)l
extern _native void PLAYSTAT_INT3(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void PLAYSTAT_STRING(int pram0, int pram1)l
extern _native void PLAYSTAT_DEED_NAMED(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void PLAYSTAT_MP_DEED_START(int pram0)l
extern _native void PLAYSTAT_MP_DEED_COMPLETE(int pram0)l
extern _native void PLAYSTAT_MP_DEED_COMPLETE_EX(int pram0, int pram1, int pram2, int pram3)l
extern _native void PLAYSTAT_MP_COOP_COMPLETE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
#pragma endregion

#pragma region TARGETING
extern _native int GET_ACTOR_UNDER_RETICLE(int pram0, int pram1)l
extern _native void GET_RETICLE_TARGET_POINT()l
extern _native void SET_PLAYER_PERFECT_ACCURACY()l
extern _native void OVERRIDE_PLAYER_TARGETING_WEIGHTS(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_ACTOR_BASE_SCORE(int pram0, int pram1, int pram2)l
extern _native void SET_ACTOR_HARDLOCK_BIAS(int pram0, int pram1, int pram2)l
extern _native void SET_ACTOR_USE_FULLSCREEN_ACQUISITION(int pram0, int pram1)l
extern _native void SET_ACTOR_CAN_BE_TARGETED(int pram0, int pram1)l
extern _native void SET_ACTOR_CAN_BE_TARGETED_CASUAL_ONLY(int pram0, int pram1)l
extern _native void SET_ACTOR_CAN_BE_HARDLOCKED(int pram0, int pram1)l
extern _native void ACTOR_CAN_BE_HARDLOCKED()l
extern _native void SET_ACTOR_CAN_BE_SOFTLOCKED(int pram0, int pram1)l
extern _native void SET_ACTOR_CAN_BE_AIMASSISTED(int pram0, int pram1)l
extern _native void SET_ACTOR_CAN_BE_BUMPTARGETED(int pram0, int pram1)l
extern _native void SET_ACTOR_ONLY_HARDLOCK_IF_HOSTILE(int pram0, int pram1)l
extern _native void IS_ACTOR_IN_ACCURACY_RANGE()l
extern _native void SET_CAN_ACTOR_HARDLOCK_NEUTRALS(int pram0, int pram1)l
extern _native int CALCULATE_NORMALIZED_SCREEN_DISTANCE_FROM_RETICLE(int pram0, int pram1)l
#pragma endregion

#pragma region CAMERA
extern _native Camera GET_GAME_CAMERA()l
extern _native void CAMERA_RESET(int pram0)l
extern _native void SET_GAME_CAMERA_DRIFTZ()l
extern _native bool CAMERA_PROBE(vector3* Result, vector3 Source, vector3 Target, Actor Owner, int Flag)l
extern _native void END_GAME_CAMERA_ARC_TRANSITIONS()l
extern _native int GET_GAME_CAMERA_RESET_POSITION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int SET_CAMERA_FOLLOW_ACTOR(int pram0)l
extern _native int SET_CAMERA_FOLLOW_ACTOR_EX(int pram0, int pram1)l
extern _native int GET_LOOKSTICK_INVERT_Y()l
extern _native void SET_LOOKSTICK_INVERT_Y(int pram0)l
extern _native void SET_GAME_CAMERA_CURVE_OVERRIDE(int pram0, int pram1)l
extern _native void RESET_GAME_CAMERA_CURVE_OVERRIDES()l
extern _native void FORCE_VEHICLE_CINEMATIC_CAMERA(int pram0)l
extern _native void IS_VEHICLE_CINEMATIC_CAMERA_FORCED_ON()l
extern _native void SET_GAME_CAMERA_VEHICLE_MODE(int pram0)l
extern _native void ALLOW_GAME_CAMERA_AUTO_CENTERING(int pram0, int pram1)l
extern _native int GET_GAME_CAMERA_AUTO_CENTERING_STATE(int pram0)l
extern _native void ALLOW_GAME_CAMERA_AUTO_TILTING(int pram0, int pram1)l
extern _native int GET_GAME_CAMERA_AUTO_TILTING_STATE(int pram0)l
extern _native void SET_GAME_CAMERA_FOCUS(int pram0, int pram1, int pram2, int pram3)l
extern _native void ENABLE_GAME_CAMERA_FOCUS(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void DISABLE_GAME_CAMERA_FOCUS()l
extern _native int IS_GAME_CAMERA_FOCUS_ENABLED()l
extern _native int IS_SWITCH_CAMERA_BUTTON_ENABLED()l
extern _native void SET_SWITCH_CAMERA_BUTTON_ENABLED(int pram0)l

//SET_DEBUG_CAMERA_MODE
#pragma endregion

#pragma region CAMERA_HANDLER
extern _native int CREATE_CAMERA_IN_LAYOUT(int pram0, int pram1, int pram2)l
extern _native int GET_CURRENT_CAMERA_TYPE_FROM_CHANNEL(int pram0)l
extern _native void SET_CURRENT_CAMERA_ON_CHANNEL(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void REMOVE_CAMERA_FROM_CHANNEL(int pram0, int pram1)l
extern _native int GET_CAMERA_CHANNEL_POSITION(int pram0, int pram1)l
extern _native int GET_CAMERA_CHANNEL_DIRECTION(int pram0, int pram1)l
extern _native int IS_CURRENT_CAMERA_CHANNEL_TRANSITIONING(int pram0)l
extern _native int IS_CAMERA_ACTIVE_ON_CHANNEL(int pram0, int pram1)l
extern _native void INIT_CAMERA_FROM_GAME_CAMERA(int pram0)l
extern _native void INIT_CAMERA_FROM_CHANNEL(int pram0, int pram1)l
extern _native void SET_CAMERA_POSITION(int pram0, int pram1, int pram2, int pram3)l
extern _native void GET_CAMERA_POSITION(Camera Cam, vector3* OutPosition)l
extern _native void SET_CAMERA_DIRECTION(Camera Cam, vector3 Direction, bool Unk1)l
extern _native void GET_CAMERA_DIRECTION(Camera Cam, vector3* OutDirection)l
extern _native void GET_CAMERA_UP_VECTOR(int pram0, int pram1)l
extern _native void SET_CAMERA_FOV(int pram0, int pram1)l
extern _native int GET_CAMERA_FOV(int pram0)l
extern _native void SET_CAMERA_ASPECT_RATIO(int pram0, int pram1)l
extern _native int GET_CAMERA_ASPECT_RATIO(int pram0)l
extern _native void SET_CAMERA_NEAR_CLIP_PLANE(int pram0, int pram1)l
extern _native void SET_CAMERA_FAR_CLIP_PLANE(int pram0, int pram1)l
extern _native void SET_CAMERA_ORIENTATION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SET_CAMERA_TARGET_POSITION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SET_CAMERA_TARGET_OBJECT(int pram0, int pram1, int pram2)l
extern _native void SET_CAMERA_COLLISION_PARAMETERS(int pram0, int pram1, int pram2)l
extern _native void ADD_CAMERA_COLLISION_EXCLUSION(int pram0, int pram1, int pram2)l
extern _native void REMOVE_CAMERA_COLLISION_EXCLUSION(int pram0, int pram1)l
extern _native void RESET_CAMERA_TARGET(int pram0, int pram1)l
extern _native void SET_CAMERA_COLLISION_ENABLED(int pram0, int pram1)l
extern _native void SET_CAMERA_TARGETDOF_FOCAL_LENGTH()l
extern _native void SET_CAMERA_TARGETDOF_USING_SOFT_DOF(int pram0, int pram1, int pram2)l
extern _native void RESET_CAMERA_TARGETDOF(int pram0)l
extern _native int SET_CAMERA_LIGHTING_SCHEME(int pram0, int pram1)l
extern _native int CAMERA_GET_CURRENT_TRANSITION_TYPE(int pram0)l
extern _native int CAMERA_IS_VISIBLE_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int CAMERA_IS_VISIBLE_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int CAMERA_IS_VISIBLE_POINT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int CAMERA_IS_VISIBLE_VOLUME(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int CREATE_CAMERASHOT_IN_LAYOUT(int pram0, int pram1)l
extern _native void INIT_CAMERASHOT_FROM_GAME_CAMERA(int pram0)l
extern _native void FORCE_CAMERASHOT_UPDATE(int pram0)l
extern _native void SET_CAMERASHOT_POSITION(int pram0, int pram1, int pram2, int pram3)l
extern _native void GET_CAMERASHOT_POSITION(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_DIRECTION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void GET_CAMERASHOT_DIRECTION(int pram0, int pram1)l
extern _native void GET_CAMERASHOT_UP_VECTOR()l
extern _native void GET_CAMERASHOT_X_VECTOR()l
extern _native void SET_CAMERASHOT_FOV(int pram0, int pram1)l
extern _native int GET_CAMERASHOT_FOV(int pram0)l
extern _native void SET_CAMERASHOT_NEAR_CLIP_PLANE(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_FAR_CLIP_PLANE(int pram0, int pram1)l
extern _native int GET_CAMERASHOT_FAR_CLIP_PLANE(int pram0)l
extern _native void SET_CAMERASHOT_ORIENTATION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SET_CAMERASHOT_TARGET_POSITION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SET_CAMERASHOT_TARGET_OBJECT(int pram0, int pram1, int pram2)l
extern _native void SET_CAMERASHOT_TARGET_OBJECT_OFFSETS(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void SET_CAMERASHOT_TARGET_OBJECT_ROLL(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGET_OBJECT_BONE()l
extern _native void SET_CAMERASHOT_FROM_LENS(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGETDOF_OBJECT(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGETDOF_TARGET_OFFSET(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGETDOF_FIXED_DISTANCE(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGETDOF_FOCAL_LENGTH(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGETDOF_CUTOFF_DISTANCE(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGETDOF_SMOOTHING(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGETDOF_USING_SOFT_DOF(int pram0, int pram1, int pram2)l
extern _native void SET_CAMERASHOT_TARGETDOF_FILTERTYPE(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_TARGETDOF_FSTOP(int pram0, int pram1)l
extern _native void RESET_CAMERASHOT_TARGETDOF(int pram0)l
extern _native void SET_CAMERASHOT_COLLISION_PARAMETERS(int pram0, int pram1, int pram2)l
extern _native void ADD_CAMERASHOT_COLLISION_EXCLUSION(int pram0, int pram1, int pram2)l
extern _native void ADD_CAMERASHOT_COLLISION_BOUNDFLAG(int pram0, int pram1)l
extern _native void RESET_CAMERASHOT_TARGET(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_PERSPECTIVE(int pram0, int pram1)l
extern _native void SET_CAMERASHOT_CONTROL_SEQUENCE_VEC3(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void ADD_CAMERA_SHOT_TRANSITION_HOLD()l
extern _native int ADD_CAMERA_SHOT_TRANSITION_INDEFINITE(int pram0, int pram1, int pram2)l
extern _native void ADD_CAMERA_SHOT_TRANSITION_EASE_OUT()l
extern _native int ADD_CAMERA_SHOT_TRANSITION_EASE_IN_OUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void SET_FIXED_TRANSITION_T(int pram0, int pram1)l
extern _native void SET_TRANSITION_COLLISION_PARAMS(int pram0, int pram1, int pram2)l
extern _native int GET_CAMERA_SHOT_TRANSITION(int pram0)l
extern _native int IS_PROCESSING_CAMERA_SHOT_TRANSITION(int pram0)l
extern _native void END_CURRENT_CAMERA_SHOT_TRANSITION(int pram0)l
extern _native void SET_CAMERASHOT_COLLISION_ENABLED(int pram0, int pram1)l
extern _native void SET_CUTSCENE_STREAMING_LOAD_SCENE(int pram0, int pram1)l
extern _native int ATTACH_CAMERASHOT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11)l
extern _native void DETACH_CAMERASHOT(int pram0)l
extern _native int CAMERASHOT_IS_VISIBLE_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void CAMERASHOT_ADD_ARC_BEHAVIOR(int pram0, int pram1)l
extern _native void CAMERASHOT_ADD_LOOKSTICK_ROTATION_BEHAVIOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int CREATE_AIMRAMP_IN_LAYOUT(int pram0, int pram1)l
extern _native void UPDATE_AIMRAMP(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int CREATE_CUTSCENEOBJECT_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3)l
extern _native int GET_CAMERA_FROM_CUTSCENEOBJECT(int pram0)l
extern _native void PLAY_CUTSCENEOBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native int GET_CAMERASHOT_FROM_CUTSCENEOBJECT(int pram0, int pram1)l
extern _native int CUTSCENEOBJECT_ADD_TRANSITION_HOLD(int pram0, int pram1, int pram2, int pram3)l
extern _native int CUTSCENEOBJECT_ADD_TRANSITION_LERP(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int CUTSCENEOBJECT_ADD_TRANSITION_EASE_IN(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int CUTSCENEOBJECT_ADD_TRANSITION_EASE_OUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int CUTSCENEOBJECT_ADD_TRANSITION_EASE_IN_OUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int CUTSCENEOBJECT_ADD_TRANSITION_INDEFINITE(int pram0, int pram1, int pram2)l
extern _native int CUTSCENEOBJECT_ADD_TRANSITION_DECORATOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int CUTSCENEOBJECT_ADD_TRANSITION_FIXED(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int CUTSCENEOBJECT_GET_CURRENT_TRANSITION_TYPE(int pram0)l
extern _native void END_CURRENT_TRANSITION_FROM_CUTSCENEOBJECT(int pram0)l
extern _native void SET_CUTSCENEOBJECT_PAUSED(int pram0, int pram1)l
extern _native int IS_CUTSCENEOBJECT_PAUSED(int pram0)l
extern _native int CHECK_CUTSCENE_COLLISIONS(int pram0)l
extern _native int GET_CUTSCENEOBJECT_SEQUENCE(int pram0)l
extern _native void CUTSCENEOBJECT_SET_RECENTER_GAMECAM(int pram0, int pram1)l
extern _native int SET_CUTSCENEINPUTS_TARGET_GUID(int pram0, int pram1, int pram2)l
extern _native void ADD_CAMERATRANSITION_EVENT_HUDFADEIN(int pram0, int pram1, int pram2, int pram3)l
extern _native void ADD_CAMERATRANSITION_EVENT_HUDFADEOUT(int pram0, int pram1, int pram2, int pram3)l
extern _native void ADD_CAMERATRANSITION_EVENT_GAMECAMERARESET(int pram0, int pram1, int pram2, int pram3)l
extern _native void ADD_CAMERATRANSITION_EVENT_GAMECAMERARESETTILT(int pram0, int pram1, int pram2)l
extern _native void ADD_CAMERATRANSITION_EVENT_CUTGAMECAMERABEHINDPLAYER(int pram0, int pram1, int pram2)l
extern _native int IS_CUTSCENE_TUNER_PLAYINGBACK()l
extern _native void CANCEL_CUTSCENE_TUNER_PLAYBACK()l
extern _native int GET_CUTSCENE_TUNER_CUTSCENEOBJECT()l
extern _native int GET_CUTSCENE_TUNER_INPUTSOBJECT()l
extern _native int GET_CUTSCENE_TUNER_SCRIPT_NAME(int pram0)l
extern _native int GET_CUTSCENE_TUNER_CUTSCENE_INDEX()l
extern _native void CREATE_CAMERA_FOCUS_POINT()l
extern _native int CREATE_CAMERA_FOCUS_POINT_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native int CREATE_CAMERA_FOCUS_POINT_OBJECT_WITH_TUNING(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int IS_CAMERA_FOCUS_ACTIVE(int pram0)l
extern _native void SET_CAMERA_FOCUS_ENABLED(int pram0, int pram1)l
extern _native int IS_CAMERA_FOCUS_ENABLED(int pram0)l
extern _native void SET_CAMERA_FOCUS_PROMPT_ENABLED(int pram0, int pram1)l
extern _native void IS_CAMERA_FOCUS_PROMPT_ENABLED()l
extern _native void SET_CAMERA_FOCUS_PLAYER_INPUT_DISABLED(int pram0, int pram1)l
extern _native void SET_CAMERA_FOCUS_PLAYER_INVULNERABLE(int pram0, int pram1)l
extern _native void SET_CAMERA_FOCUS_PROMPT_TEXT(int pram0, int pram1)l
extern _native void SET_CAMERA_FOCUS_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l

//SET_CAMERASHOT_ASPECT_RATIO
#pragma endregion

#pragma region INVENTORY
extern _native int ADD_ITEM(const char* ItemName, Actor Actor, int Unk1)l
extern _native int ADD_ITEM_BY_CRC(int pram0, int pram1, int pram2)l
extern _native int HAS_INVENTORY_COMPONENT(int pram0)l
extern _native int GET_ITEM_COUNT(int pram0, int pram1)l
extern _native int GET_ITEM_COUNT_BY_CRC(int pram0, int pram1)l
extern _native int GET_MAX_ITEM_COUNT(int pram0)l
extern _native void SET_MAX_ITEM_COUNT(int pram0, int pram1)l
extern _native void ADD_ACCESSORY()l
extern _native int ADD_ACCESSORY_BY_CRC(int pram0, int pram1, int pram2)l
extern _native int ADD_COLLECTABLE(int pram0, int pram1, int pram2)l
extern _native void REMOVE_COLLECTABLE(int pram0, int pram1)l
extern _native void READY_ITEM(const char* ItemName, Actor Actor)l
extern _native int HAS_ITEM(int pram0, int pram1)l
extern _native void HAS_ACCESSORY()l
extern _native void DELETE_ITEM(int pram0, int pram1, int pram2)l
extern _native void DELETE_ACCESSORY(int pram0, int pram1)l
extern _native int ACTOR_GET_ITEM_CRC_AT_INDEX(int pram0, int pram1)l
extern _native int ACTOR_GET_ACCESSORY_CRC_AT_INDEX(int pram0, int pram1)l
extern _native int ACTOR_GET_COLLECTABLE_CRC_AT_INDEX(int pram0, int pram1)l
extern _native int IS_ITEM_WEAPON_BY_CRC(int pram0)l
extern _native int GET_ITEM_IN_HAND_EQUIPSLOT(int pram0)l
extern _native int GET_ITEM_EQUIPSLOT(int pram0)l
extern _native void ACTOR_DISABLE_WEAPON_RENDER(int pram0, int pram1, int pram2)l
extern _native void ACTOR_FORCE_WEAPON_RENDER(int pram0, int pram1, int pram2)l
extern _native int IS_WEAPON_DRAWN(int pram0)l
extern _native void GIVE_WEAPON_TO_ACTOR(Actor Actor, int WeaponID, int Unk0, int Unk1, int _Unk1)l
extern _native void ACTOR_SET_NEXT_WEAPON(int pram0, int pram1)l
extern _native int ACTOR_PUT_WEAPON_IN_HAND(int pram0, int pram1, int pram2)l
extern _native int ACTOR_HAS_WEAPON_IN_HAND(int pram0, int pram1)l
extern _native void ACTOR_PUT_ITEM_AWAY()l
extern _native void ACTOR_HAS_WEAPON_SET_AS_NEXT()l
extern _native int ACTOR_SET_NEXT_EQUIP_SLOT_FROM_WEAPON_ENUM(int pram0, int pram1, int pram2)l
extern _native int ACTOR_SET_NEXT_EQUIP_SLOT(int pram0, int pram1, int pram2)l
extern _native int ACTOR_GET_NEXT_EQUIP_SLOT(int pram0)l
extern _native int ACTOR_GET_CURRENT_EQUIP_SLOT(int pram0)l
extern _native int ACTOR_HAS_WEAPON(int pram0, int pram1)l
extern _native int ACTOR_GET_BEST_WEAPON_OF_TYPE(int pram0, int pram1)l
extern _native void DELETE_WEAPON_FROM_ACTOR(int pram0, int pram1)l
extern _native Weapon GET_WEAPON_EQUIPPED(Actor Actor, int Index)l
extern _native int GET_WEAPON_IS_EXTERNALLY_CREATED(int pram0, int pram1)l
extern _native int GET_WEAPON_IN_HAND(int pram0)l
extern _native int GET_WEAPON_IN_HAND_CRC(int pram0)l
extern _native void GET_WEAPON_IN_HAND_NAME()l
extern _native int GET_WEAPON_ENUM_FROM_CRC(int pram0)l
extern _native void ACTOR_USE_ITEM_NOW()l
extern _native void SET_EQUIP_SLOT_ENABLED(int pram0, int pram1, int pram2)l
extern _native void GET_EQUIP_SLOT_ENABLED()l
extern _native void EQUIP_ACCESSORY(int pram0, int pram1, int pram2)l
extern _native void DEEQUIP_ACCESSORY(int pram0, int pram1)l
extern _native int HAS_ACCESSORY_ENUM(int pram0, int pram1)l
extern _native int IS_ACCESSORY_EQUIPPPED(int pram0, int pram1)l
extern _native int DROP_ACCESSORY_ENUM(int pram0, int pram1)l
extern _native void ACTOR_SET_WEAPON_AMMO(int pram0, int pram1, int pram2)l
extern _native void ACTOR_SET_WEAPON_AMMO_BY_CRC(int pram0, int pram1, int pram2)l
extern _native void ACTOR_ADD_WEAPON_AMMO(int pram0, int pram1, int pram2)l
extern _native int ACTOR_GET_WEAPON_AMMO(int pram0, int pram1)l
extern _native void ACTOR_DISCARD_WEAPON_AMMO(int pram0, int pram1)l
extern _native int ACTOR_HAS_VARIABLE_MESH(int pram0, int pram1)l
extern _native int GET_AMMOENUM_FOR_WEAPONENUM(int pram0)l
extern _native int GET_WEAPONENUM_FOR_AMMOENUM(int pram0, int pram1)l
extern _native void SET_WEAPON_GOLD(Actor Actor, int Weapon, bool Gold)l
extern _native int GET_WEAPON_GOLD(Actor Actor, Weapon Weapon)l
extern _native int IS_GOLDEN_GUNS_ON()l
extern _native void FIRE_PROJECTILE(Actor Actor, const char* WeapGroup, float pram2, vector3* Origin, vector3* Target)l
extern _native void ACTOR_ADD_INV_AMMO(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ACTOR_SET_INV_AMMO(int pram0, int pram1, int pram2, int pram3)l
extern _native void ACTOR_SET_INV_AMMO_MAX_AMOUNT(int pram0, int pram1, int pram2)l
extern _native void ACTOR_SET_INV_AMMO_INFINITE(Actor Actor, int WeapId, bool Infinite)l
extern _native int ACTOR_GET_INV_AMMO(int pram0, int pram1, int pram2)l
extern _native int ACTOR_GET_INV_AMMO_MAX_AMOUNT(int pram0, int pram1)l
extern _native bool ACTOR_GET_INV_AMMO_INFINITE(Actor Actor, int pram1)l
extern _native void ACTOR_SHOULD_DROP_ITEMS_ON_DEATH(int pram0, int pram1, int pram2)l
extern _native void ACTOR_SET_DROP_ITEM_ON_DEATH_ENUMERATED(int pram0, int pram1, int pram2)l
extern _native int CREATE_WEAPON_PICKUP(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void REMOVE_ALL_PICKUPS()l
extern _native int GET_NUM_WEAPONS_IN_INVENTORY(int pram0)l
extern _native int GET_NUM_ACCESSORIES_IN_INVENTORY(int pram0)l
extern _native int GET_NUM_COLLECTABLES_IN_INVENTORY(int pram0)l
extern _native void DELETE_ALL_WEAPONS_FROM_ACTOR(int pram0)l
extern _native void DELETE_ALL_ACCESSORIES_FROM_ACTOR(int pram0)l
extern _native void DELETE_ALL_INVENTORY_FROM_ACTOR(int pram0)l
extern _native void SETUP_ASSOCIATED_FRAGMENTS(int pram0)l
extern _native int GRINGOITEM_GET_ACTION()l
extern _native void GRINGOITEM_CLEAR_FOR_NEW_USE(int pram0)l
extern _native void SET_PICKUPS_ARENT_NEW()l
#pragma endregion

#pragma region INVENTORY_MANAGER
extern _native void SET_INDICATOR_DRAW(int pram0, int pram1)l
#pragma endregion

#pragma region AMBIENT
extern _native void AMBIENT_SET_UPDATES_ENABLED(int pram0)l
extern _native void AMBIENT_SET_POINT_SPACING(int pram0)l
extern _native void AMBIENT_SET_SLOPE_VALUES(int pram0, int pram1, int pram2, int pram3)l
extern _native void AMBIENT_RESET_USED_CELLS()l
extern _native void AMBIENT_SET_SCAN_CENTER(int pram0, int pram1, int pram2)l
extern _native void AMBIENT_SET_SCAN_CENTER_PLAYER()l
extern _native void AMBIENT_RESET_FILTER(int pram0)l
extern _native void AMBIENT_SET_RANDOM_SEARCH_ORDER(int pram0)l
extern _native void AMBIENT_SET_ONESHOT_QUERIES(int pram0)l
extern _native void AMBIENT_SET_SEARCH_CENTER(int pram0, int pram1, int pram2)l
extern _native void AMBIENT_SET_SEARCH_CENTER_ACTOR(int pram0)l
extern _native void AMBIENT_SET_SEARCH_CENTER_PLAYER()l
extern _native void AMBIENT_SET_SLOPE_FILTER(int pram0, int pram1, int pram2)l
extern _native void AMBIENT_ENABLE_SLOPE_FILTER(int pram0)l
extern _native void AMBIENT_SET_SLOPE_FILTER_PRECISE(int pram0, int pram1, int pram2)l
extern _native void AMBIENT_ENABLE_SLOPE_FILTER_PRECISE(int pram0)l
extern _native void AMBIENT_SET_BUMP_FILTER(int pram0, int pram1, int pram2)l
extern _native void AMBIENT_ENABLE_BUMP_FILTER(int pram0)l
extern _native void AMBIENT_SET_DISTANCE_FILTER(int pram0, int pram1)l
extern _native void AMBIENT_ENABLE_DISTANCE_FILTER(int pram0)l
extern _native void AMBIENT_SET_ELEVATION_FILTER(int pram0, int pram1)l
extern _native void AMBIENT_SET_QUADRANT_FILTER(int pram0, int pram1, int pram2, int pram3)l
extern _native void AMBIENT_ENABLE_QUADRANT_FILTER(int pram0)l
extern _native void AMBIENT_ENABLE_OFFSCREEN_FILTER(int pram0)l
extern _native void AMBIENT_ENABLE_MATERIAL_AT_POINT_FILTER(int pram0)l
extern _native void AMBIENT_SET_MATERIAL_AT_POINT_FILTER(int pram0)l
extern _native void AMBIENT_ENABLE_MATERIAL_IN_AREA_INCLUDES_FILTER(int pram0, int pram1)l
extern _native void AMBIENT_SET_MATERIAL_IN_AREA_INCLUDES_FILTER(int pram0, int pram1, int pram2)l
extern _native void AMBIENT_ENABLE_MATERIAL_IN_AREA_EXCLUDES_FILTER()l
extern _native void AMBIENT_SET_MATERIAL_IN_AREA_EXCLUDES_FILTER(int pram0, int pram1, int pram2)l
extern _native32(0x30C67D05) void _AMBIENT_ENABLE_ELEVATION_IN_AREA_RANGE_FILTER(int pram0)l
extern _native32(0xAC99007) void _AMBIENT_SET_ELEVATION_IN_AREA_RANGE_FILTER(int pram0, int pram1)l
extern _native void AMBIENT_ENABLE_ELEVATION_DERIVATIVE_IN_AREA_FILTER(int pram0)l
extern _native void AMBIENT_SET_ELEVATION_DERIVATIVE_IN_AREA_FILTER(int pram0, int pram1)l
extern _native void AMBIENT_SET_EXCLUSION_VOLUME(int pram0)l
extern _native int AMBIENT_GET_POINT(int pram0, int pram1)l
#pragma endregion

#pragma region OBJECT_HANDLER
extern _native void VERIFY_TYPE_COUNT(int pram0)l
extern _native void VERIFY_EVENT_COUNT(int pram0)l
extern _native int UNK_0x26011C78(int pram0)l
extern _native void LOG_OBJECT2(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void LOG_OBJECT3(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native bool IS_LAYOUTREF_VALID(Layout Layout)l
extern _native bool IS_OBJECT_VALID(Object Object)l
extern _native bool IS_ITERATOR_VALID(Iterator iterator)l
extern _native int IS_PERS_CHAR_VALID(int pram0)l
extern _native int IS_POPSET_VALID(int pram0)l
extern _native int IS_ZONE_VALID(int pram0)l
extern _native int IS_CRIME_VALID(int pram0)l
extern _native int GET_OBJECT_TYPE(int pram0)l
extern _native int GET_NUM_OBJECTS_OF_TYPE(int pram0)l
extern _native int GET_MAX_NUM_OBJECTS_OF_TYPE(int pram0)l
extern _native Layout FIND_NAMED_LAYOUT(const char* LayoutName)l
extern _native const char* GET_ACTOR_NAME(Actor actor)l
extern _native int GET_OBJECT_NAME(int pram0)l
extern _native int GET_LAYOUT_NAME(int pram0)l
extern _native int GET_OBJECT_MODEL_NAME(int pram0)l
extern _native void GET_POPULATION_SET_NAME()l
extern _native int GET_COVER_LOCATION_FROM_OBJECT(int pram0)l
extern _native int GET_GRINGO_FROM_OBJECT(int pram0)l
extern _native int GET_PROP_FROM_OBJECT(int pram0)l
extern _native int GET_OBJECT_FROM_GRINGO(int pram0)l
extern _native int GET_OBJECT_FROM_ACTOR(int pram0)l
extern _native int GET_OBJECT_FROM_VOLUME(int pram0)l
extern _native int GET_OBJECT_FROM_PERS_CHAR(int pram0)l
extern _native int GET_OBJECT_FROM_SQUAD(int pram0)l
extern _native int GET_OBJECT_FROM_CRIME(int pram0)l
extern _native int GET_OBJECT_FROM_PHYSINST(int pram0)l
extern _native int GET_ACTOR_FROM_OBJECT(int pram0)l
extern _native int GET_VOLUME_FROM_OBJECT(int pram0)l
extern _native int GET_PERS_CHAR_FROM_OBJECT(int pram0)l
extern _native int GET_ITERATOR_FROM_OBJECT(int pram0)l
extern _native int GET_SQUAD_FROM_OBJECT(int pram0)l
extern _native int GET_LAYOUT_FROM_OBJECT(int pram0)l
extern _native int GET_MOBILE_LAYOUT_FROM_OBJECT(int pram0)l
extern _native int GET_CRIME_FROM_OBJECT(int pram0)l
extern _native int GET_CAMERA_FROM_OBJECT(int pram0)l
extern _native int GET_NAV_QUERY_FROM_OBJECT(int pram0)l
extern _native Layout CREATE_LAYOUT(const char* layoutName)l
extern _native int CREATE_MOBILE_LAYOUT(int pram0)l
extern _native int CREATE_CORPSE_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10)l
extern _native int CREATE_CORPSE_IN_LAYOUT_RANDOM(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void CREATE_CORPSE_VARIATION_IN_LAYOUT()l
extern _native void CREATE_CORPSE_VARIATION_IN_LAYOUT_RANDOM()l
extern _native int CREATE_COVER_LOCATION_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int CREATE_POINT_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native int CREATE_POINT_LIGHT_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int CREATE_VOLUME_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11)l
extern _native int CREATE_VOLUME_SET_IN_LAYOUT(int pram0, int pram1)l
extern _native int CREATE_GRINGO_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int CREATE_GRINGO_ON_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int CREATE_GRINGO_IN_LAYOUT_BY_ID(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int CREATE_GRINGO_ON_OBJECT_BY_ID(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int CREATE_PROPSET_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native Object CREATE_PROP_IN_LAYOUT(Layout Layout, const char* PropName, const char* FragmentPath, vector3 Position, vector3 Rotation, bool Frozen)l
extern _native int CREATE_PROP_IN_LAYOUT_BY_ID(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void CREATE_COVER_PROP_IN_LAYOUT()l
extern _native void CREATE_SPAWN_POINT_IN_LAYOUT()l
extern _native int CREATE_PATH_IN_LAYOUT(int pram0, int pram1, int pram2)l
extern _native int CREATE_PATH_IN_LAYOUT_FROM_TABLE(int pram0, int pram1, int pram2)l
extern _native int CREATE_OBSTACLE_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10)l
extern _native int CREATE_OBSTACLE_ON_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10)l
extern _native int CREATE_SQUAD_IN_LAYOUT(int pram0, int pram1)l
extern _native int CREATE_FORMATION_IN_LAYOUT(int pram0, int pram1, int pram2)l
extern _native int CREATE_CRIME_IN_LAYOUT(int pram0)l
extern _native int CREATE_AI_SPEECH_PACKAGE_IN_LAYOUT(int pram0, int pram1)l
extern _native int MARK_LAYOUT_CONSIDER_WHOLE(int pram0, int pram1)l
extern _native void RELEASE_LAYOUT_REF(int pram0)l
extern _native void RELEASE_LAYOUT_OBJECTS(int pram0)l
extern _native void DESTROY_LAYOUT_OBJECTS(int pram0)l
extern _native int INIT_MOBILE_LAYOUT_OBJECTS(int pram0)l
extern _native int STORE_MOBILE_LAYOUT_TRANSFORMS(int pram0)l
extern _native void MAKE_LAYOUT_OBJECTS_RELATIVE()l
extern _native int TRANSFORM_OBJECT_RELATIVE(int pram0, int pram1)l
extern _native void MARK_OBJECT_FOR_AGGRESSIVE_CLEANUP(int pram0, int pram1)l
extern _native void RELEASE_OBJECT_REF(int pram0)l
extern _native void RELEASE_ACTOR(Actor Actor)l
extern _native void RELEASE_VOLUME(int pram0)l
extern _native void RELEASE_PERS_CHAR(int pram0)l
extern _native void DESTROY_VOLUME(int pram0)l
extern _native void DESTROY_ACTOR(int pram0)l
extern _native void DESTROY_LAYOUT(int pram0)l
extern _native void DESTROY_OBJECT(Object Object)l
extern _native void DESTROY_ITERATOR(int pram0)l
extern _native void DESTROY_PERS_CHAR(int pram0)l
extern _native void DESTROY_POINT_LIGHT(int pram0)l
extern _native void DESTROY_POPULATION_SET(int pram0)l
extern _native void DESTROY_ZONE(int pram0)l
extern _native void DESTROY_CRIME(int pram0)l
extern _native int GIVE_OBJECT_TO_ACTOR(int pram0, int pram1)l
extern _native int GIVE_OBJECT_TO_LAYOUT(int pram0, int pram1)l
extern _native int GET_OBJECT_OWNER(int pram0)l
extern _native void DESTROY_GC_OBJECTS(int pram0, int pram1)l
extern _native32(0x65C3D8F6) int _WAS_LAST_OBJECT_ALREADY_IN_GAME()l
extern _native Layout GET_AMBIENT_LAYOUT()l
extern _native int GET_ART_GRINGO_LAYOUT()l
extern _native Layout GET_GC_LAYOUT()l
extern _native int CREATE_OBJECTSET_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3)l
extern _native int IS_OBJECTSET_VALID(int pram0)l
extern _native int ADD_OBJECT_TO_OBJECTSET(int pram0, int pram1)l
extern _native int GET_OBJECTSET_SIZE(int pram0)l
extern _native int GET_OBJECT_FROM_OBJECTSET(int pram0)l
extern _native int GET_OBJECTSET_FROM_OBJECT(int pram0)l
extern _native int GET_INDEXED_OBJECT_IN_OBJECTSET(int pram0, int pram1)l
extern _native int IS_OBJECT_IN_OBJECTSET(int pram0, int pram1)l
extern _native void REMOVE_OBJECT_FROM_OBJECTSET(int pram0, int pram1)l
extern _native void CLEAN_OBJECTSET(int pram0)l
extern _native void DISBAND_OBJECTSET(int pram0)l
extern _native void DESTROY_OBJECTSET(int pram0)l
extern _native void SET_CORPSE_PERMANENT()l
extern _native int IS_POINT_LIGHT_VALID(int pram0)l
extern _native void SET_ENABLE_POINT_LIGHT(int pram0, int pram1)l
extern _native int IS_OBJECT_IN_VOLUME(int pram0, int pram1)l
extern _native int LOCATE_NAMED_POINT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int LOCATE_NAMED_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int LOCATE_NAMED_VOLUME(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int LOCATE_ACTORS_IN_VOLUME(int pram0, int pram1, int pram2, int pram3)l
extern _native int LOCATE_GRINGOS_IN_VOLUME(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_VOLUME_PARAMS(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void ADD_TO_VOLUME_SET(int pram0, int pram1)l
extern _native int COPY_VOLUME(int pram0, int pram1, int pram2, int pram3)l
extern _native void SCALE_VOLUME(int pram0, int pram1)l
extern _native void SET_ACTOR_STAY_OUTSIDE_OF_VOLUME(int pram0, int pram1, int pram2, int pram3)l
extern _native void ADD_ACTOR_STAY_OUTSIDE_OF_VOLUME(int pram0, int pram1)l
extern _native void SET_ACTOR_STAY_WITHIN_VOLUME(int pram0, int pram1, int pram2, int pram3)l
extern _native void ADD_ACTOR_STAY_WITHIN_VOLUME(int pram0, int pram1)l
extern _native void CLEAR_ACTOR_STAY_WITHIN_VOLUME(int pram0)l
extern _native void SET_ACTOR_VOLUME_PARAMETERS(int pram0, int pram1, int pram2)l
extern _native void REMOVE_ACTOR_STAY_WITHIN_VOLUME(int pram0, int pram1)l
extern _native void REMOVE_ACTOR_STAY_OUTSIDE_OF_VOLUME(int pram0, int pram1)l
extern _native void ADD_CORPSE_RETAINMENT_VOLUME_OBJ(int pram0)l
extern _native void REMOVE_CORPSE_RETAINMENT_VOLUME_OBJ(int pram0)l
extern _native void ADD_CORPSE_REMOVAL_VOLUME_OBJ(int pram0)l
extern _native void REMOVE_CORPSE_REMOVAL_VOLUME_OBJ(int pram0)l
extern _native void ADD_CORPSE_PREVENT_INTERFERENCE_VOLUME_OBJ(int pram0)l
extern _native void REMOVE_CORPSE_PREVENT_INTERFERENCE_VOLUME_OBJ(int pram0)l
extern _native void TOGGLE_COVER_PROPS(int pram0)l
extern _native void TOGGLE_COVER_PROP()l
extern _native int CREATE_ZONE_VOLUME(int pram0)l
extern _native int CREATE_VOLUME_SPAWNING_ZONE_VOLUME(int pram0)l
extern _native int CREATE_POPULATION_SET(int pram0)l
extern _native int CREATE_POPULATION_SET_IN_LAYOUT(int pram0, int pram1, int pram2)l
extern _native int CREATE_NAMED_POPULATION_SET(int pram0, int pram1)l
extern _native int CREATE_GATEWAY_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native int CREATE_GATEWAY_FROM_PARAMS_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11, int pram12, int pram13, int pram14, int pram15, int pram16, int pram17)l
extern _native void CREATE_GATEWAY_TYPE(int pram0, int pram1)l
extern _native int SNAP_ACTOR_TO_GRINGO(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native32(0x6745191B) int _RMPTFX_EFFECT_START(int pram0, int pram1, int pram2, int pram3)l
extern _native32(0x276EFF8E) int _RMPTFX_EFFECT_START_AND_FADE_IN(int pram0, int pram1, int pram2, int pram3)l
extern _native32(0xE18028C1) int _RMPTFX_EFFECT_STOP(int pram0)l
extern _native32(0x7246F438) int _RMPTFX_EFFECT_STOP_AND_RESET(int pram0)l
extern _native32(0x1A59E608) int _RMPTFX_EFFECT_STOP_AND_FADE_OUT(int pram0)l
extern _native int RESET_RMPTFX_ALL()l
extern _native void RESET_RMPTFX_IN_VOLUME(int pram0)l
extern _native int CREATE_RMPTFX_EMITTER_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int CREATE_RMPTFX_EMITTER_ON_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native int CREATE_RMPTFX_EMITTER_ON_BONE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native int CREATE_RMPTFX_EMITTER_ON_CURVE(int pram0, int pram1, int pram2, int pram3)l
extern _native int IS_RMPTFX_FINISHED(int pram0)l
extern _native int SET_RMPTFX_SCALE(int pram0, int pram1)l
extern _native int HIDE_RMPTFX_EFFECT(int pram0)l
extern _native int SHOW_RMPTFX_EFFECT(int pram0)l
extern _native void START_RECORDING_SHOOT_EVENTS_FOR_ACTOR(int pram0)l
extern _native void STOP_RECORDING_SHOOT_EVENTS_FOR_ACTOR(int pram0)l
extern _native int ADD_FORMATION_LOCATION(int pram0, int pram1)l
extern _native void GET_FORMATION_LOCATION(int pram0, int pram1, int pram2)l
extern _native int GET_NUM_FORMATION_LOCATIONS(int pram0)l
extern _native int GET_CRIME_POSITION(int pram0, int pram1)l
extern _native int GET_CRIME_TYPE(int pram0)l
extern _native int GET_CRIME_BEGIN_TIMESTAMP(int pram0)l
extern _native int GET_CRIME_END_TIMESTAMP(int pram0)l
extern _native int GET_CRIME_WITNESSED(int pram0)l
extern _native int GET_CRIME_CRIMINAL(int pram0)l
extern _native int GET_CRIME_VICTIM(int pram0)l
extern _native int GET_CRIME_WORLD_REGION(int pram0)l
extern _native int GET_CRIME_FACTION(int pram0)l
extern _native int GET_CRIME_COUNTER(int pram0)l
extern _native int IS_CRIME_TALLIED(int pram0)l
extern _native int IS_CRIME_IN_PROGRESS(int pram0)l
extern _native int GET_CRIME_OBJECTSET(int pram0)l
extern _native void SET_CRIME_OBJECTSET(int pram0, int pram1)l
extern _native int SET_CRIME_POSITION(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_CRIME_TYPE(int pram0, int pram1)l
extern _native void SET_CRIME_BEGIN_TIMESTAMP(int pram0, int pram1)l
extern _native void SET_CRIME_END_TIMESTAMP(int pram0, int pram1)l
extern _native void SET_CRIME_WITNESSED(int pram0, int pram1)l
extern _native void SET_CRIME_CRIMINAL(int pram0, int pram1)l
extern _native void SET_CRIME_VICTIM(int pram0, int pram1)l
extern _native void SET_CRIME_WORLD_REGION(int pram0, int pram1)l
extern _native void SET_CRIME_FACTION(int pram0, int pram1)l
extern _native void SET_CRIME_COUNTER(int pram0, int pram1)l
extern _native void SET_CRIME_TALLIED(int pram0, int pram1)l
extern _native void SET_CRIME_IN_PROGRESS(int pram0, int pram1)l
extern _native Iterator CREATE_OBJECT_ITERATOR(Layout layout)l
extern _native int CREATE_NAMED_OBJECT_ITERATOR(int pram0, int pram1)l
extern _native Object START_OBJECT_ITERATOR(Iterator Iterator)l
extern _native int OBJECT_ITERATOR_NEXT(int pram0)l
extern _native int OBJECT_ITERATOR_PREV(int pram0)l
extern _native int OBJECT_ITERATOR_CURRENT(int pram0)l
extern _native void OBJECT_ITERATOR_RESET(int pram0)l
extern _native void ITERATE_ON_PARTIAL_NAME(int pram0, int pram1)l
extern _native void ITERATE_ON_PARTIAL_MODEL_NAME(int pram0, int pram1)l
extern _native void ITERATE_ON_OBJECT_TYPE(Iterator Iterator, int Type)l
extern _native void ITERATE_IN_AREA()l
extern _native void ITERATE_IN_SPHERE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ITERATE_IN_VOLUME(int pram0, int pram1)l
extern _native void ITERATE_EVERYWHERE(int pram0)l
extern _native void ITERATE_IN_LAYOUT(int pram0, int pram1)l
extern _native void ITERATE_IN_SET(Iterator Iterator, IterationSet IterationSet)l
extern _native void ITERATE_IN_EVENT_TRAP(int pram0, int pram1)l
extern _native void ITERATE_IN_VOLUME_SET(int pram0, int pram1)l
extern _native int GET_ITERATOR_PARENT(int pram0)l
extern _native int GET_NUM_ITERATOR_MATCHES(int pram0)l
extern _native int FIND_OBJECT_IN_OBJECT(int pram0, int pram1)l
extern _native Object FIND_OBJECT_IN_LAYOUT(Layout Layout, const char* ObjectName)l
extern _native Actor FIND_ACTOR_IN_LAYOUT(Layout Layout, const char* ActorName)l
extern _native int FIND_VOLUME_IN_LAYOUT(int pram0, int pram1)l
extern _native void CLEAR_AMBIENT_OBJECTS_SPHERE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void CLEAR_AMBIENT_OBJECTS_VOLUME(int pram0, int pram1)l
extern _native int GET_OBJECT_POSITION(int pram0, int pram1)l
extern _native int ROTATE_OBJECT_AROUND_AXIS(int pram0, int pram1, int pram2)l
extern _native int GET_OBJECT_AXIS(int pram0, int pram1, int pram2)l
extern _native int GET_OBJECT_RELATIVE_POSITION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GET_OBJECT_RELATIVE_OFFSET(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GET_OBJECT_RELATIVE_ORIENTATION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GET_OBJECT_RELATIVE_ORIENTATION_IN_OBJECT_SPACE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int PREPARE_CORPSE_FOR_ANIMAL_CONSUMPTION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int GET_POSITION_OBJECT_SPACE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GET_OBJECT_ORIENTATION(int pram0, int pram1)l
extern _native int GET_OBJECT_HEADING(int pram0)l
extern _native int SET_OBJECT_POSITION(Object Obj, vector3 Position)l
extern _native int SET_OBJECT_POSITION_ON_GROUND(Object Obj, vector3 Position)l
extern _native int SET_OBJECT_ORIENTATION(int pram0, int pram1, int pram2, int pram3)l
extern _native int SNAP_OBJECT_TO_GROUND(int pram0, int pram1, int pram2, int pram3)l
extern _native int GET_OBJECT_NAMED_BONE_POSITION(int pram0, int pram1, int pram2)l
extern _native int GET_OBJECT_NAMED_BONE_ORIENTATION(int pram0, int pram1, int pram2)l
extern _native int TELEPORT_OBJECT_TO_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int IS_OBJECT_ATTACHED(int pram0)l
extern _native int GET_OBJECT_ATTACHEMENT(int pram0)l
extern _native int GET_OBJECT_ATTACHED_TO(int pram0)l
extern _native int ATTACH_OBJECTS(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native int ATTACH_OBJECTS_NO_DRV(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int ATTACH_OBJECTS_USING_LOCATOR(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int ATTACH_OBJECTS_CONTINUOUS(int pram0, int pram1, int pram2)l
extern _native int ATTACH_OBJECTS_PHYSICAL(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int ATTACH_SET_ROTATIONAL_CONSTRAINT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int IS_ATTACHMENT_VALID(int pram0)l
extern _native void REMOVE_OBJECT_ATTACHMENT(int pram0)l
extern _native void REMOVE_OBJECT_FROM_ATTACHMENT(int pram0)l
extern _native void REMOVE_ALL_OBJECT_ATTACHMENTS(int pram0)l
extern _native void SET_ATTACHMENT_LOCAL_ROTATION(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_ATTACHMENT_LOCAL_OFFSET(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_ATTACHMENT_IGNORE_ROTATION(int pram0, int pram1)l
extern _native void REFERENCE_OBJECT(int pram0)l
extern _native void REFERENCE_ACTOR(int pram0)l
extern _native void DEREFERENCE_OBJECT(int pram0)l
extern _native void DEREFERENCE_ACTOR(int pram0)l
extern _native void INIT_NATIVE_ACTORENUM_PLAYER(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_HUMAN(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_ANIMAL(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_RIDEABLE_ANIMAL(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_FLYING_ANIMAL(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_VEHICLE(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_TRAIN(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_DLC_PLAYER(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_DLC_HUMAN(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_DLC_ANIMAL()l
extern _native void INIT_NATIVE_ACTORENUM_DLC_RIDEABLE_ANIMAL(int pram0, int pram1, int pram2, int pram3)l
extern _native void INIT_NATIVE_ACTORENUM_DLC_FLYING_ANIMAL()l
extern _native void INIT_NATIVE_ACTORENUM_DLC_VEHICLE()l
extern _native void INIT_NATIVE_ACTORENUM_DLC_TRAIN()l
extern _native int IS_ACTORENUM_DLC(int pram0)l
extern _native int IS_ACTORENUM_INSTALLED(int pram0)l
extern _native void IS_ACTORENUM_PLACEHOLDER()l
extern _native void SET_LOCK_OFFSET_ACTORENUM_ANIMAL(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native Actor CREATE_ACTOR_IN_LAYOUT(Layout Layout, const char* ActorName, eActor ActorID, vector3 Position, vector3 Rotation)l
extern _native int CREATE_PLAYER_ACTOR_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native int CREATE_PERS_CHAR_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void CREATE_ACTOR_VARIATION_IN_LAYOUT()l
extern _native Actor RESPAWN_PLAYER_ACTOR_IN_LAYOUT(Layout Layout, Actor Actor, const char* ActorInLayout, eActor ActorID, vector3 Position, vector3 Rotation, int unk0)l
extern _native int IS_ACTOR_INITED(int pram0)l
extern _native int GET_ACTOR_ENUM(int pram0)l
extern _native int GET_ACTOR_ENUM_STRING(int pram0)l
extern _native int GET_ACTOR_ENUM_FROM_STRING(int pram0)l
extern _native int GET_ACTOR_ENUM_STRING_FROM_ENUM(int pram0)l
extern _native int GET_ACTOR_ENUM_VARIATION_COUNT(int pram0)l
extern _native int GET_ACTOR_ENUM_FACTION(int pram0)l
extern _native void SWITCH_ACTOR_ENUM_VARIATION(int pram0, int pram1)l
extern _native void SWITCH_PLAYER_TO_ENUM(int pram0, int pram1)l
extern _native void TURN_ACTOR_INTO_ZOMBIE(int pram0, int pram1)l
extern _native void SET_ACTOR_HARD_LOCK_AQUIRE_BONE_CASUAL(int pram0, int pram1)l
extern _native void SET_ACTOR_HARD_LOCK_AQUIRE_BONE(int pram0, int pram1)l
extern _native void GET_CURRENT_ACTOR_ENUM_VARIATION()l
extern _native int REQUEST_ASSET(const char* AssetDir, eAssetType AssetType)l
extern _native int GET_ASSET_ID(const char* AssetPath, eAssetType AssetType)l
extern _native int REMOVE_ASSET(int pram0, int pram1)l
extern _native int AE_GET_ASSET_ID(int pram0, int pram1)l
extern _native int GET_ASSET_NAME(int pram0, int pram1)l
extern _native int IS_ASSETTYPEID_VALID(int pram0)l
extern _native int IS_ASSETTYPEID_VALID_BY_STRING(int pram0, int pram1)l
extern _native int GET_CORPSE_ACTOR_ENUM(int pram0)l
extern _native int GET_PREVIOUS_ACTOR_FROM_CORPSE(int pram0)l
extern _native int REQUEST_ARTICULATED_CORPSE(int pram0)l
extern _native int REQUEST_FIXED_CORPSE(int pram0, int pram1)l
extern _native void SET_PROP_COLLIDE_WITH_WORLD(int pram0, int pram1)l
extern _native void SET_PROP_COLLIDE_WITH_MOVABLES(int pram0, int pram1)l
extern _native void SET_PROP_COLLIDE_WITH_OBJECT(int pram0, int pram1, int pram2)l
extern _native void SET_OBJECT_COLLIDE_WITH_WORLD(int pram0, int pram1)l
extern _native void SET_OBJECT_COLLIDE_WITH_MOVABLES(int pram0, int pram1)l
extern _native void SET_OBJECT_COLLIDE_WITH_OBJECT(int pram0, int pram1, int pram2)l
extern _native int SET_PROP_AI_OBSTACLE_ENABLED(int pram0, int pram1)l
extern _native int GET_CURVE_FROM_OBJECT(int pram0)l
extern _native void DESTROY_CURVEQUERY(int pram0)l
extern _native void DESTROY_CAMERA(int pram0)l
extern _native void DESTROY_CAMERA_SHOT()l
extern _native int CREATE_WORLD_SECTOR(int pram0, int pram1)l
extern _native int IS_WORLD_SECTOR_LOADED(int pram0)l
extern _native void MARK_REGION_READY(int pram0)l
extern _native int GET_WORLD_SECTOR_CHILD_OBJECT(int pram0, int pram1, int pram2)l
extern _native void DEBUG_SELECT_OBJECT()l
extern _native void DEBUG_ASSERT_ON_SELECTED_DESTRUCTION()l
extern _native int SAVE_POP_SET_BIRDS_SPAWNING_FROM_TREES(int pram0)l
extern _native void SET_TOWN_VOLUME_FOR_AMBIENT_PEDS(int pram0)l
extern _native int SAVE_POP_SET_HUMAN_SPAWNING_IN_TOWN(int pram0)l
extern _native int SAVE_POP_SET_HUMAN_SPAWNING_IN_TOWN_CONVERSATION_FULL(int pram0)l
extern _native int SAVE_POP_SET_HUMAN_SPAWNING_IN_TOWN_CONVERSATION_RESPONSE(int pram0)l
extern _native void SET_AGRESSIVE_EVENT_TOWN_BBX()l
extern _native void SET_AGRESSIVE_EVENT_TIME_RANGE()l
extern _native void SET_AGRESSIVE_EVENT_LOOP()l
extern _native void SET_AGRESSIVE_EVENT_ON(int pram0)l
extern _native void SET_TOWN_DENSITY(int pram0)l
extern _native void SET_WEATHER_POP_DENSITY(int pram0, int pram1)l
extern _native void SET_SPAWN_PEDS_ON_SIDEWALK()l
#pragma endregion

#pragma region VEHICLES
extern _native bool GET_ACTOR_MOST_RECENT_VEHICLE(int pram0)l
extern _native int IS_ACTOR_ON_TRAIN(int pram0, int pram1)l
extern _native int IS_ACTOR_ON_BOAT(int pram0)l
extern _native bool IS_ACTOR_VEHICLE(Actor Actor)l
extern _native int IS_ACTOR_DRAFT_VEHICLE(int pram0)l
extern _native int IS_ACTOR_DRIVING_VEHICLE(int pram0)l
extern _native int IS_ACTOR_JACKING_VEHICLE(int pram0)l
extern _native int IS_ACTOR_INSIDE_VEHICLE(int pram0)l
extern _native bool IS_ACTOR_RIDING_VEHICLE(Actor Actor)l
extern _native int GET_VEHICLE(int pram0)l
extern _native int GET_ACTOR_VEHICLE_STATE(int pram0, int pram1, int pram2)l
extern _native int GET_NUM_DRAFTED_ACTORS(int pram0)l
extern _native bool SET_ACTOR_IN_VEHICLE(Actor Source, Actor Destination, int SeatID)l
extern _native int GET_DRAFT_ACTOR(int pram0, int pram1)l
extern _native bool IS_ACTOR_DRAFTED(int actor)l
extern _native int GET_ACTOR_DRAFTED_TO(int pram0)l
extern _native int IS_SEAT_OCCUPIED(int pram0, int pram1)l
extern _native Actor GET_ACTOR_IN_VEHICLE_SEAT(Vehicle Veh, int SeatID)l
extern _native int GET_NUM_AVAILABLE_SEATS(int pram0)l
extern _native int GET_NUM_OCCUPIED_SEATS(int pram0)l
extern _native bool ENABLE_VEHICLE_SEAT(Actor Vehicle, int SeatId, bool Enable)l
extern _native void STOP_VEHICLE(int pram0)l
extern _native void START_VEHICLE(Actor Vehicle)l
extern _native int ACCESSORIZE_VEHICLE_HORSES(int pram0, int pram1)l
extern _native int GET_NUM_DRAFT_POSITIONS(int pram0)l
extern _native int ATTACH_DRAFT_TO_VEHICLE(int pram0, int pram1, int pram2, int pram3)l
extern _native void DETACH_DRAFT_FROM_VEHICLE_BY_ACTOR()l
extern _native void DETACH_DRAFT_FROM_VEHICLE_BY_INDEX()l
extern _native int TRAIN_CREATE_NEW_TRAIN(int pram0, int pram1, int pram2)l
extern _native void TRAIN_DESTROY_TRAIN(int pram0)l
extern _native void TRAIN_RELEASE_TRAIN(int pram0, int pram1)l
extern _native int TRAIN_IS_VALID(int pram0)l
extern _native void TRAIN_ADD_TRAIN_CAR_FROM_ENUM(int pram0, int pram1)l
extern _native int TRAIN_GET_VELOCITY(int pram0)l
extern _native void TRAIN_SET_TARGET_POS(int pram0, int pram1)l
extern _native void TRAIN_SET_SPEED()l
extern _native void TRAIN_SET_TARGET_SPEED(int pram0, int pram1)l
extern _native void TRAIN_SET_MAX_ACCEL(int pram0, int pram1)l
extern _native void TRAIN_SET_MAX_DECEL(int pram0, int pram1)l
extern _native void TRAIN_SET_ENGINE_ENABLED(int pram0, int pram1)l
extern _native int TRAIN_GET_NUM_CARS(int pram0)l
extern _native void TRAIN_SET_POSITION_DIRECTION(int pram0, int pram1, int pram2)l
extern _native void TRAIN_SET_POSITION_DIRECTION_PRECISELY(int pram0, int pram1, int pram2, int pram3)l
extern _native void TRAIN_SET_FX()l
extern _native void TRAIN_PLAY_WHISTLE_SEQUENCE(int pram0, int pram1)l
extern _native void TRAIN_ENABLE_VISUAL_DEBUG(int pram0)l
extern _native void TRAIN_DESTROY_CAR(int pram0, int pram1)l
extern _native int TRAIN_GET_LOD_LEVEL(int pram0)l
extern _native void TRAIN_FORCE_HIGH_LOD(int pram0, int pram1)l
extern _native int TRAIN_GET_CAR(int pram0, int pram1)l
extern _native void TRAIN_SET_PARTICLE_EFFECTS_ENABLED(int pram0, int pram1, int pram2)l
extern _native void TRAIN_SET_AUTOPILOT_ENABLE(int pram0, int pram1)l
extern _native void TRAIN_SET_AUDIO_ENABLE()l
extern _native int TRAIN_SET_JUNCTION_STATE(int pram0, int pram1, int pram2)l
extern _native void TRAIN_GET_CURVE_NETWORK_POINT()l
extern _native int TRAIN_GET_NEAREST_POI_DISTANCE(int pram0)l
extern _native void TRAIN_GET_POSITION(int pram0, int pram1)l
extern _native void SET_BOAT_EXTRA_STEER(int pram0, int pram1)l
extern _native void SET_VEHICLE_ENGINE_RUNNING(Actor Vehicle, bool Running)l
extern _native int IS_VEHICLE_ENGINE_RUNNING(int pram0)l
extern _native void SET_VEHICLE_ALLOWED_TO_DRIVE(Actor Vehicle, bool Allowed)l
extern _native void IS_VEHICLE_ALLOWED_TO_DRIVE()l
extern _native void SET_VEHICLE_PASSENGERS_ALLOWED(int pram0, int pram1)l
extern _native void SET_VEHICLE_EJECTION_ENABLED(int pram0, int pram1)l
extern _native void VEHICLE_SET_HANDBRAKE(Actor Vehicle, bool Handbreak)l
extern _native void IS_ANY_VEHICLE_WHEEL_DETACHED()l
extern _native int GET_VEHICLE_WHEEL_COUNT_INITIAL(int pram0)l
extern _native int GET_VEHICLE_WHEEL_COUNT_CURRENT(int pram0)l
#pragma endregion

#pragma region EVENT
extern _native int IS_EVENT_VALID(int pram0)l
extern _native Object GET_EVENT_FROM_OBJECT(Object pram0)l
extern _native int GET_OBJECT_FROM_EVENT(int pram0)l
extern _native int COPY_EVENT(int pram0, int pram1)l
extern _native Layout GET_EVENT_LAYOUT()l
extern _native int GET_EVENT_TYPE(int pram0)l
extern _native int GET_EVENT_TARGET_AS_OBJECT(int pram0)l
extern _native int GET_EVENT_TARGET_AS_PHYSINST(int pram0)l
extern _native int GET_EVENT_PERPETRATOR(int pram0)l
extern _native int ADD_NEW_EVENT_RESPONSE(int pram0, int pram1, int pram2, int pram3)l
extern _native int ADD_NEW_RANGED_EVENT_RESPONSE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int REMOVE_EVENT_RESPONSE(int pram0, int pram1, int pram2)l
extern _native void RANGED_EVENT_RESPONSE_INIT_COMPLETE()l
extern _native void REGISTER_FOR_CREATION_EVENT(int pram0)l
extern _native int GET_EVENT_TIME(int pram0)l
extern _native IterationSet GET_OBJECTSET_FOR_EVENT_TYPE(int SetId)l
extern _native int CREATE_EVENT_TRAP(int pram0, int pram1, int pram2)l
extern _native void EVENT_TRAP_ON_VOLUME(int pram0, int pram1)l
extern _native void EVENT_TRAP_ON_SPHERE()l
extern _native void EVENT_TRAP_ON_PERPETRATOR(int pram0, int pram1)l
extern _native void EVENT_TRAP_ON_TARGET(int pram0, int pram1)l
extern _native void EVENT_TRAP_ON_OWNER(int pram0, int pram1)l
extern _native void EVENT_TRAP_STORE_EVENTS(int pram0, int pram1)l
extern _native void EVENT_TRAP_CLEAR_EVENTS(int pram0)l
extern _native int EVENT_TRAP_SUCCESSFUL_TRAP(int pram0)l
extern _native void EVENT_TRAP_CLEAR_TRAP_FLAG(int pram0)l
extern _native int GET_NUM_EVENT_RESPONSES()l
extern _native int GET_EVENT_RESPONSE_ID(int pram0)l
extern _native int GET_EVENT_FOR_RESPONSE(int pram0)l
#pragma endregion

#pragma region RIDING
extern _native void SET_MOST_RECENT_MOUNT(int pram0, int pram1)l
extern _native int GET_MOST_RECENT_MOUNT(int pram0)l
extern _native int GET_MOST_RECENT_RIDER(int pram0)l
extern _native int IS_ACTOR_HORSE(int pram0)l
extern _native int IS_ACTOR_MULE(int pram0)l
extern _native int IS_ACTOR_RIDING(int pram0)l
extern _native int IS_ACTOR_RIDING_AND_IN_SADDLE(int pram0)l
extern _native int IS_ACTOR_MOUNTED(int pram0)l
extern _native int GET_RIDER(int pram0)l
extern _native int GET_MOUNT(int pram0)l
extern _native void SET_MOUNTS_AS_PASSENGER(int pram0, int pram1)l
extern _native int ACTOR_MOUNT_ACTOR(int pram0, int pram1)l
extern _native void REMOVE_HORSE_ACCESSORY()l
extern _native int DOES_HORSE_HAVE_ACCESSORY(int pram0, int pram1)l
extern _native int ACCESSORIZE_HORSE(int pram0, int pram1)l
extern _native void HORSE_ENABLE_AUTO_JUMP_FOR_AI_RIDERS()l
extern _native void HORSE_AUTO_JUMP_ENABLED_FOR_AI_RIDERS()l
extern _native int HORSE_ADD_REPULSION_EXCLUSION_VOLUME(int pram0)l
extern _native int HORSE_REMOVE_REPULSION_EXCLUSION_VOLUME(int pram0)l
extern _native int HORSE_SET_CURR_FRESHNESS(int pram0, int pram1)l
extern _native int HORSE_GET_CURR_FRESHNESS(int pram0)l
extern _native int HORSE_LOCK_FRESHNESS(int pram0)l
extern _native int HORSE_UNLOCK_FRESHNESS(int pram0)l
extern _native void HORSE_SET_INFINITE_FRESHNESS_CHEAT(bool Activate)l
#pragma endregion

#pragma region PROP
extern _native int GET_PHYSINST_FROM_OBJECT(int pram0)l
extern _native int GET_NEW_PHYSINST_IN_SPHERE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int IS_PROP_FIXED(int pram0)l
extern _native void SET_PROP_FIXED(int pram0, int pram1)l
extern _native void REMOVE_PHYSINST()l
extern _native int GET_CENTER_OF_GRAVITY(int pram0, int pram1)l
extern _native int HAS_PROP_BEEN_DAMAGED(int pram0)l
extern _native void GET_PROP_HEALTH()l
extern _native void SET_PROP_HEALTH(int pram0, int pram1)l
extern _native void RESET_PROPS_IN_WORLD()l
extern _native void RESET_PROPS_IN_VOLUME(int pram0, int pram1)l
extern _native void RESET_PROP(int pram0)l
extern _native void SET_PROP_TARGETABLE(int pram0, int pram1, int pram2)l
extern _native void SET_PROP_TARGETABLE_ACQUISITION_RADIUS(int pram0, int pram1)l
extern _native void SET_PROP_TARGETABLE_TARGET_BOX_SIZE(int pram0, int pram1, int pram2)l
extern _native void SET_PROP_TARGETABLE_SCORE_BIAS(int pram0, int pram1)l
extern _native void SET_PROP_TARGETABLE_AS_ENEMY(int pram0, int pram1)l
extern _native void SET_PROP_CAUSE_ARM_UP()l
extern _native int IS_PROP_STREAMED_IN(int pram0)l
extern _native void GRAVE_IS_DUG_UP()l
extern _native void GRAVE_SET_DUG_UP(int pram0, int pram1)l
extern _native int GET_GRAVE_FROM_OBJECT(int pram0)l
extern _native int PLAY_SIMPLE_PROP_ANIMATION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void SET_DRAW_OBJECT(int pram0, int pram1)l
extern _native void GET_DRAW_OBJECT()l
extern _native int PROP_IS_VARIABLE_MESH_ENABLED(int pram0, int pram1)l
extern _native int PROP_ENABLE_VARIABLE_MESH(int pram0, int pram1, int pram2)l
extern _native int SET_PROP_REMAP(int pram0, int pram1)l
extern _native void CLEAR_PROP_REMAP()l
#pragma endregion

#pragma region CURVES
extern _native int ENABLE_CURVE(int pram0, int pram1)l
extern _native int ARE_CURVES_IN_RANGE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int START_CURVE_QUERY(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int GET_CURVE_BY_NAME(int pram0, int pram1)l
extern _native int IS_CURVE_QUERY_VALID(int pram0)l
extern _native int GET_NUM_POINTS_IN_CURVE_QUERY(int pram0)l
extern _native int GET_NUM_CURVES_IN_CURVE_QUERY(int pram0)l
extern _native int GET_POINT_FROM_CURVE_QUERY(int pram0, int pram1, int pram2)l
extern _native int GET_CURVE_FROM_CURVE_INDEX_IN_CURVE_QUERY(int pram0, int pram1)l
extern _native int GET_CURVE_FROM_POINT_INDEX_IN_CURVE_QUERY(int pram0, int pram1)l
extern _native int GET_CLOSEST_POINT_TO_CURVE_CURVE_QUERY(int pram0, int pram1, int pram2)l
extern _native void REMOVE_CURVE_FROM_CURVE_QUERY_USING_CURVE(int pram0, int pram1)l
extern _native32(0x4D89A35) void _TRAVEL_DISTANCE_ON_CURVE_FROM_CURVE_POINT(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native32(0x19D652F9) void _TRAVEL_DISTANCE_ON_CURVE_FROM_CURVE_POINT_2(int pram0, int pram1, int pram2, int pram3)l
extern _native int CURVE_CALCULATE_DISTANCE_BY_STEP_SIZE(int pram0, int pram1, int pram2, int pram3)l
extern _native int CURVE_CALCULATE_DISTANCE_BY_FORCED_COMPONENT_SUBDIVISION_BOUNDS(int pram0, int pram1, int pram2, int pram3)l
extern _native void RELEASE_CURVE(int pram0)l
extern _native int GET_CURVE_TYPE(int pram0)l
extern _native int GET_CURVE_NAME(int pram0)l
extern _native void GET_CURVE_POINT(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_CURVE_ACTIVE(int pram0, int pram1)l
extern _native void SET_CURVE_WEIGHT(int pram0, int pram1)l
extern _native void CURVE_NETWORK_POINT_GET_DISTANT_POINT()l
#pragma endregion

#pragma region SOCIAL_CLUB
extern _native void UI_CHALLENGE_CREATE(int pram0, int pram1, int pram2)l
extern _native void UI_CHALLENGE_SET_DESCRIPTION(int pram0, int pram1)l
extern _native void UI_CHALLENGE_SET_TITLE_TEXTURE(int pram0, int pram1)l
extern _native void UI_CHALLENGE_SET_PROGRESS(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void UI_CHALLENGE_SET_PROGRESS_DETAIL(int pram0, int pram1)l
extern _native void UI_CHALLENGE_SET_TROPHY_TYPE(int pram0, int pram1)l
extern _native void UI_CHALLENGE_SET_OBJECTIVE(int pram0, int pram1, int pram2)l
extern _native void UI_CHALLENGE_SET_OBJECTIVE_STYLE(int pram0, int pram1, int pram2)l
extern _native void UI_CHALLENGE_SET_OBJECTIVE_STYLE_B(int pram0, int pram1, int pram2)l
extern _native void UI_CHALLENGE_SET_COLUMN_HEADER(int pram0, int pram1, int pram2)l
extern _native void UI_CHALLENGE_SET_TIME_HEADER(int pram0, int pram1)l
extern _native void UI_CHALLENGE_SET_TIME_INFO(int pram0, int pram1, int pram2)l
extern _native void UI_CHALLENGE_MAKE_CURRENT(int pram0)l
#pragma endregion

#pragma region SHOP
extern _native void SHOP_CLEAR()l
extern _native void SHOP_REFRESH(int pram0)l
extern _native void SHOP_SET_PLAYER_BANK(int pram0)l
extern _native int SHOP_ADD_ITEM(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void SHOP_SET_ITEM_QUANTITY(int pram0, int pram1)l
extern _native int SHOP_GET_ITEM_QUANTITY(int pram0)l
extern _native void SHOP_SET_ITEM_PURCHASE_PRICE(int pram0, int pram1)l
extern _native int SHOP_GET_ITEM_PURCHASE_PRICE(int pram0)l
extern _native int SHOP_GET_ITEM_SELL_PRICE(int pram0)l
extern _native void SHOP_SET_ITEM_BLOCKED(int pram0, int pram1)l
extern _native int SHOP_IS_ITEM_BLOCKED(int pram0)l
extern _native int SHOP_GET_ITEM_MISC_FLAG(int pram0)l
extern _native int SHOP_IS_SELL_SELECTED()l
extern _native void SHOP_SET_CURRENT_TAB(int pram0)l
extern _native void SHOP_LOCK_INPUT(int pram0)l
extern _native void SATCHEL_SET_ENABLED(int pram0)l
extern _native void SATCHEL_TOGGLE_NO_PAUSE_OUTFIT(int pram0)l
extern _native int SATCHEL_CREATE_OUTFIT(int pram0, int pram1, int pram2)l
extern _native void SATCHEL_SET_OUTFIT_STATE(int pram0, int pram1)l
extern _native int SATCHEL_GET_OUTFIT_STATE(int pram0)l
extern _native void SATCHEL_SET_OUTFIT_TEXTURE(int pram0, int pram1)l
extern _native void SATCHEL_ALLOW_OUTFIT_CHANGE(int pram0)l
extern _native void SATCHEL_SET_CURRENT_OUTFIT(int pram0)l
#pragma endregion

#pragma region STAT
extern _native void CREATE_STAT(int pram0, int pram1, int pram2, int pram3)l
extern _native void HIDE_STAT(int pram0, int pram1)l
extern _native void UPDATE_STAT(int pram0, int pram1, int pram2)l
extern _native void ENABLE_JOURNAL_REPLAY(int pram0)l
extern _native void SET_JOURNAL_INFO_LABEL(int pram0, int pram1)l
extern _native int GET_SAGPLAYER_STAT_FLOAT(int pram0)l
extern _native int GET_SAGPLAYER_STAT_INT(int pram0)l
extern _native void SET_SAGPLAYER_STAT_FLOAT(int pram0, int pram1)l
extern _native void SET_SAGPLAYER_STAT_INT(int pram0, int pram1)l
extern _native int GET_NUM_KILLS_LAST_DEADEYE()l
#pragma endregion

#pragma region JOURNAL
extern _native void SET_EXCLUSIVE_JOURNAL_ID(int pram0)l
extern _native void RESET_EXCLUSIVE_JOURNAL_ID()l
extern _native void TOGGLE_COOP_JOURNAL_UI(int pram0)l
extern _native void TOGGLE_JOURNAL_UI(int pram0, int pram1)l
extern _native int GET_JOURNAL_ENTRY(int pram0)l
extern _native int CREATE_JOURNAL_ENTRY(int pram0, int pram1, int pram2, int pram3)l
extern _native int CREATE_JOURNAL_ENTRY_BY_HASH(int pram0, int pram1, int pram2, int pram3)l
extern _native int IS_JOURNAL_ENTRY_IN_LIST(int pram0, int pram1)l
extern _native int GET_NUM_JOURNAL_ENTRIES_IN_LIST(int pram0)l
extern _native int GET_JOURNAL_ENTRY_IN_LIST(int pram0, int pram1)l
extern _native void PREPEND_JOURNAL_ENTRY(int pram0, int pram1)l
extern _native void APPEND_JOURNAL_ENTRY(int pram0, int pram1)l
extern _native void REMOVE_JOURNAL_ENTRY(int pram0, int pram1)l
extern _native void CLEAR_JOURNAL_ENTRY(int pram0)l
extern _native void PREPEND_JOURNAL_ENTRY_DETAIL(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void APPEND_JOURNAL_ENTRY_DETAIL(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void CLEAR_JOURNAL_ENTRY_DETAIL_LIST(int pram0, int pram1)l
extern _native int GET_JOURNAL_ENTRY_NUM_DETAILS(int pram0)l
extern _native int GET_JOURNAL_ENTRY_DETAIL_HASH_BY_INDEX(int pram0, int pram1)l
extern _native int GET_JOURNAL_ENTRY_DETAIL_STYLE_BY_HASH(int pram0, int pram1)l
extern _native void SET_JOURNAL_ENTRY_DETAIL_STYLE(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_JOURNAL_ENTRY_DETAIL_STYLE_BY_HASH(int pram0, int pram1, int pram2, int pram3)l
extern _native int IS_JOURNAL_ENTRY_TARGETED(int pram0)l
extern _native int GET_JOURNAL_ENTRY_TYPE(int pram0)l
extern _native int IS_JOURNAL_ENTRY_UPDATED(int pram0)l
extern _native int GET_JOURNAL_ENTRY_MISC_FLAG(int pram0)l
extern _native int GET_TARGETED_JOURNAL_ENTRY()l
extern _native void TARGET_JOURNAL_ENTRY(int pram0)l
extern _native void SET_JOURNAL_ENTRY_UPDATED(int pram0, int pram1)l
extern _native void SET_JOURNAL_ENTRY_TROPHY(int pram0, int pram1)l
extern _native void DEACTIVATE_JOURNAL_ENTRY(int pram0, int pram1)l
extern _native void SET_JOURNAL_ENTRY_PROGRESS(int pram0, int pram1, int pram2, int pram3)l
extern _native int GET_JOURNAL_ENTRY_PROGRESS(int pram0)l
extern _native void SET_JOURNAL_ENTRY_DISALLOW_TRACKING(int pram0, int pram1)l
extern _native int GET_JOURNAL_ENTRY_DISALLOW_TRACKING(int pram0)l
extern _native void SET_JOURNAL_ENTRY_CURRENT_OBJECTIVE(int pram0, int pram1, int pram2)l
extern _native int GET_LAST_NOTE_OBJECTIVE()l
extern _native void APPEND_JOURNAL_NOTE()l
#pragma endregion

#pragma region MISC
extern _native int CREATE_OBJECT_LOCATOR(int pram0, int pram1)l
extern _native int OBJECT_LOCATOR_ATTACH_TO_OBJECT_BONE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int OBJECT_LOCATOR_ATTACH_TO_FRAGMENT_LOCATOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int OBJECT_LOCATOR_ATTACH_TO_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native int OBJECT_LOCATOR_GET_LOCATOR_LOCAL_ORIENTATION(int pram0, int pram1)l
extern _native int OBJECT_LOCATOR_GET_LOCATOR_LOCAL_POSITION(int pram0, int pram1)l
#pragma endregion

#pragma region OBJECT
extern _native int ROTATE_OBJECT_UPRIGHT_TO_GROUND(int pram0, int pram1, int pram2)l
extern _native void VISIBLE_RECTS_BOOLEAN()l
#pragma endregion

#pragma region LEASH
extern _native int CREATE_LEASH_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11, int pram12, int pram13, int pram14)l
extern _native int LEASH_CONSTRAIN(int pram0)l
extern _native int LEASH_RESTART(int pram0)l
extern _native int LEASH_SET_CONSTRAINT_LENGTH(int pram0, int pram1)l
extern _native int LEASH_SET_LEASH_LENGTH(int pram0, int pram1)l
extern _native int LEASH_RELEASE_CONSTRAINT(int pram0)l
extern _native int LEASH_ATTACH_TO_WORLD(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native int LEASH_ATTACH_TO_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native int LEASH_ATTACH_TO_FRAGMENT_LOCATOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10)l
extern _native int LEASH_ATTACH_TO_OBJECT_BONE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11)l
extern _native int LEASH_ATTACH_TO_OBJECT_BONE_VISUAL(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11)l
extern _native void LEASH_SET_LOCATOR_POSITION_VISUAL()l
extern _native int LEASH_DETACH_OBJECT(int pram0, int pram1)l
extern _native int LEASH_IS_BROKEN(int pram0)l
extern _native int LEASH_BREAK(int pram0, int pram1)l
extern _native int CREATE_ROPE_FOR_BRIDGE_LEFT(int pram0, int pram1, int pram2)l
extern _native int CREATE_ROPE_FOR_BRIDGE_RIGHT(int pram0, int pram1, int pram2)l
extern _native int LEASH_STAY_CONSTRAINED(int pram0, int pram1)l
extern _native void SET_LEASH_COLLIDES()l
#pragma endregion

#pragma region WORLD_INTERSECTION
extern _native int FIND_INTERSECTION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int FIND_GROUND_INTERSECTION(int pram0, int pram1, int pram2, int pram3)l
extern _native int FIND_GROUND_INTERSECTION_WITH_MATERIAL(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int FIND_WATER_INTERSECTION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native int GET_MATERIAL_AT_VECTOR(int pram0)l
extern _native int GET_ACTOR_GROUND_MATERIAL(int pram0)l
extern _native int IS_POSITION_INDOORS(int pram0, int pram1, int pram2)l
#pragma endregion

#pragma region CUTSCENE
extern _native int CUTSCENE_MANAGER_DOES_CUTSCENE_EXIST(int pram0)l
extern _native void CUTSCENE_MANAGER_LOAD_CUTFILE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int CUTSCENE_MANAGER_IS_CUTFILE_LOADED()l
extern _native int CUTSCENE_MANAGER_GET_LOADED_CUTFILE()l
extern _native void CUTSCENE_MANAGER_RESUME_LOADING()l
extern _native void CUTSCENE_MANAGER_LOAD_CUTSCENE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int CUTSCENE_MANAGER_IS_CUTSCENE_LOADED()l
extern _native void CUTSCENE_MANAGER_UNLOAD_CUTSCENE()l
extern _native void CUTSCENE_MANAGER_PLAY_CUTSCENE()l
extern _native void CUTSCENE_MANAGER_STOP_CUTSCENE(int pram0, int pram1)l
extern _native int CUTSCENE_MANAGER_IS_CUTSCENE_PLAYING()l
extern _native int CUTSCENE_MANAGER_IS_CUTSCENE_FINISHED()l
extern _native void CUTSCENE_MANAGER_SET_TRANSITION_FROM_GAMECAMERA()l
extern _native void CUTSCENE_MANAGER_SET_TRANSITION_TO_GAMECAMERA()l
extern _native void CUTSCENE_MANAGER_RESET_GAMECAMERA_ON_EXIT(int pram0, int pram1)l
extern _native void CUTSCENE_MANAGER_ORIENT_GAMECAMERA_ON_EXIT(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void CUTSCENE_MANAGER_SKIP_UI_STACK_POP()l
extern _native void CUTSCENE_MANAGER_SET_SKIP_UI_STACK_POP(int pram0)l
extern _native void CUTSCENE_MANAGER_SET_HIDE_NONCUTSCENE_ACTORS(int pram0)l
extern _native int CUTSCENE_MANAGER_CAN_SET_POST_CUTSCENE_POSES()l
extern _native void CUTSCENE_MANAGER_CLEAR_CAN_SET_POST_CUTSCENE_POSES()l
extern _native int CUTSCENE_MANAGER_GET_CURRENT_TIME()l
extern _native int CUTSCENE_MANAGER_GET_CURRENT_FRAME()l
extern _native int CUTSCENE_MANAGER_GET_TOTAL_FRAMES()l
extern _native void CUTSCENE_MANAGER_ENUMERATE_CUTXML_NAMES()l
extern _native void CUTSCENE_MANAGER_GET_NUM_CUTXML_NAMES()l
extern _native void CUTSCENE_MANAGER_GET_CUTXML_NAME()l
extern _native int CUTSCENE_MANAGER_GET_NUM_CUTSCENE_ACTORS()l
extern _native int CUTSCENE_MANAGER_GET_CUTSCENE_ACTOR_NAME(int pram0)l
extern _native int CUTSCENE_MANAGER_GET_CUTSCENE_ACTOR(int pram0)l
extern _native void CUTSCENE_MANAGER_GET_CUTSCENE_ACTORENUM()l
extern _native int CUTSCENE_MANAGER_GET_CUTSCENE_ACTOR_BY_INDEX_START_ORIENT(int pram0, int pram1, int pram2)l
extern _native int CUTSCENE_MANAGER_GET_CUTSCENE_ACTOR_BY_INDEX_END_ORIENT(int pram0, int pram1, int pram2)l
extern _native int CUTSCENE_MANAGER_GET_NUM_CUTSCENE_PROPS()l
extern _native int CUTSCENE_MANAGER_GET_CUTSCENE_PROP_NAME(int pram0)l
extern _native int CUTSCENE_MANAGER_GET_CUTSCENE_PROP(int pram0)l
extern _native void CUTSCENE_MANAGER_GET_CUTSCENE_PROP_BY_NAME()l
extern _native int CUTSCENE_MANAGER_GET_CUTSCENE_PROP_BY_INDEX_START_ORIENT(int pram0, int pram1, int pram2)l
extern _native int CUTSCENE_MANAGER_GET_CUTSCENE_PROP_BY_INDEX_END_ORIENT(int pram0, int pram1, int pram2)l
extern _native int CUTSCENE_MANAGER_HIDE_ACTOR(int pram0)l
extern _native int CUTSCENE_MANAGER_SHOW_ACTOR(int pram0)l
extern _native int CUTSCENE_MANAGER_GET_INITIAL_STREAMING_LOAD_SCENE_EXT(int pram0, int pram1)l
extern _native void CUTSCENE_MANAGER_GET_FINAL_STREAMING_LOAD_SCENE_EXT()l
extern _native void CUTSCENE_MANAGER_SET_FINAL_STREAMING_LOAD_SCENE_EXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void CUTSCENE_MANAGER_SET_ASSET_OVERRIDE()l
extern _native void CUTSCENE_MANAGER_SET_ASSET_OVERRIDE_ACTORENUM()l
extern _native void CUTSCENE_MANAGER_SET_ASSET_OVERRIDE_ACTOR(int pram0, int pram1)l
extern _native int CUTSCENE_MANAGER_GET_SCRIPT_EVENT_DATA()l
extern _native void CUTSCENE_MANAGER_GET_SCRIPT_EVENT_DESCRIPTION()l
extern _native void CUTSCENE_MANAGER_CLEAR_SCRIPT_EVENT()l
extern _native void CUTSCENE_MANAGER_SET_WAS_JOHN_NOW_JACK_IN_RCM_CUTSCENE(int pram0)l
#pragma endregion

#pragma region GATEWAY
extern _native int GATEWAY_GET_ACTOR(int pram0)l
extern _native void GATEWAY_SET_ACTOR(int pram0, int pram1)l
extern _native int GATEWAY_GET_VOLUME(int pram0)l
extern _native int GATEWAY_GET_MARKER(int pram0)l
extern _native int GATEWAY_UPDATE(int pram0)l
extern _native void GATEWAY_DISABLE_ALL(int pram0)l
extern _native void GATEWAY_DISABLE(int pram0, int pram1)l
extern _native int GATEWAYS_ARE_DISABLED()l
extern _native void GATEWAY_IS_DISABLED()l
#pragma endregion

#pragma region VOLUME
extern _native int IS_VOLUME_VALID(int pram0)l
extern _native int IS_POINT_IN_VOLUME(int pram0, int pram1, int pram2, int pram3)l
extern _native int IS_ACTOR_IN_VOLUME(int pram0, int pram1)l
extern _native void DELETE_PROJECTILES_IN_VOLUME(int pram0)l
extern _native void GET_VOLUME_CENTER(int pram0, int pram1)l
extern _native void SET_VOLUME_ENABLED(int pram0, int pram1)l
extern _native int IS_VOLUME_ENABLED(int pram0)l
extern _native int GET_VOLUME_HEADING(int pram0)l
extern _native void GET_VOLUME_SCALE(int pram0, int pram1)l
extern _native void GENERATE_RANDOM_POINT_IN_VOLUME(int pram0, int pram1)l
extern _native void FIND_VOL_SURFACE_POINTS_FOR_POINT(int pram0, int pram1, int pram2, int pram3)l
#pragma endregion

#pragma region TURRET
extern _native void IS_USING_TURRET()l
#pragma endregion

#pragma region STRING
extern _native bool IS_STRING_VALID(const char* pram0)l
extern _native bool STRINGS_ARE_EQUAL(const char* Str1, const char* Str2)l
extern _native int STRINGS_ARE_EQUAL_CLAMPED(int pram0, int pram1, int pram2)l
extern _native int STRING_CONTAINS_STRING(int pram0, int pram1)l
extern _native float STRING_TO_FLOAT(const char* Str)l
extern _native int STRING_TO_INT(const char* pram0)l
extern _native int STRING_TO_HASH(int pram0)l
extern _native int STRING_LENGTH(const char* pram0)l
extern _native int STRINGTABLE_LENGTH(int pram0)l
extern _native int STRING_LOWER(int pram0)l
extern _native int STRING_UPPER(int pram0)l
extern _native int STRING_TOKENIZE(int pram0, int pram1)l
extern _native int STRING_NUM_TOKENS()l
extern _native int STRING_GET_TOKEN(int pram0)l
extern _native void STRING_CLEAR_TOKENIZER()l
extern _native void SS_INIT(int pram0)l
extern _native int SS_REGISTER(int pram0, int pram1, int pram2)l
extern _native const char* SS_GET_STRING(int pram0, int pram1)l
extern _native int SS_GET_STRING_ID(int pram0, int pram1)l
extern _native void SS_FINALIZE()l
extern _native void SS_UNFINALIZE()l
extern _native int SS_SET_TABLE_SIZE(int pram0, int pram1)l
#pragma endregion

#pragma region POPULATION
extern _native void BEGIN_POPULATION_DEFINITION()l
extern _native void END_POPULATION_DEFINITION()l
extern _native void LINK_ACTORENUM_TO_POPULATION(int pram0, int pram1, int pram2)l
extern _native void LINK_ACTORENUM_TO_POPULATION_TIMED(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void UNLINK_ACTORENUM_FROM_POPULATION()l
extern _native void SET_ACCESSORYSET_ON_SPAWN(int pram0, int pram1, int pram2)l
extern _native void SET_ZONE_POPULATION_ANIMAL_DOMESTICATION(int pram0, int pram1)l
extern _native void SET_ZONE_POPULATION_COUNT(int pram0, int pram1)l
extern _native void SET_ZONE_POPULATION_COUNT_RANDOM(int pram0, int pram1, int pram2)l
extern _native void SET_ZONE_POPULATION_DENSITY(int pram0, int pram1)l
extern _native void SET_ZONE_POPULATION_TYPE(int pram0, int pram1)l
extern _native void SET_ZONE_POPULATION_SPAWN_MULTI_PER_POINT(int pram0, int pram1)l
extern _native void SET_ZONE_RESTRICT_ACTORS(int pram0, int pram1)l
extern _native void SET_ZONE_POPULATION_IS_FLOCK(int pram0, int pram1)l
extern _native void SET_ZONE_POPULATION_MAX_FLOCK_SIZE(int pram0, int pram1)l
extern _native void SET_ZONE_POPULATION_MIN_FLOCK_SIZE(int pram0, int pram1)l
extern _native void SET_ZONE_POPULATION_MAX_PER_CELL(int pram0, int pram1)l
extern _native void SET_FLOCK_DEFAULT_POPULATION_MAX_PER_CELL(int pram0)l
extern _native void SET_INDIVIDUAL_DEFAULT_POPULATION_MAX_PER_CELL(int pram0)l
extern _native void SET_ZONE_PRIORITY(int pram0, int pram1)l
extern _native void SET_ZONE_FORCE_SPAWN_DISTANCE(int pram0, int pram1)l
extern _native void SET_ZONE_RESERVE_GRINGOS(int pram0, int pram1)l
extern _native void SET_ZONE_SPAWN_ONLY_AT_GRINGOS(int pram0, int pram1)l
extern _native void CLEAR_ZONE_ALLOWED_GRINGO_TYPE_LIST(int pram0)l
extern _native void ADD_TO_ZONE_ALLOWED_GRINGO_TYPE_LIST(int pram0, int pram1)l
extern _native void SET_ZONE_RESPECT_VOLUME_RESTRICTIONS(int pram0, int pram1, int pram2)l
extern _native void SET_DEFAULT_POPULATION_DENSITY(int pram0)l
extern _native void SET_DEFAULT_POPULATION_TYPE(int pram0)l
extern _native int GET_RAND_ACTORENUM_FROM_POPULATION_NATIVE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void MAKE_NEXT_RAND_ACTORENUMS_UNIQUE(int pram0)l
extern _native int IS_POPULATION_SET_READY(int pram0, int pram1, int pram2)l
extern _native int IS_POPULATION_SET_REQUIRED_RESIDENT(int pram0)l
extern _native int FIND_NAMED_POPULATION_SET(int pram0)l
extern _native int GET_NUM_ACTORENUMS_IN_POPULATION(int pram0)l
extern _native int GET_ACTORENUM_IN_POPULATION(int pram0, int pram1)l
extern _native int GET_ACTORENUM_IN_POPULATION_WEIGHT(int pram0, int pram1)l
extern _native int MARKETING_GET_AMBIENT_DENSITY_LEVEL()l
extern _native void SET_ZONE_ALLOWED_SPAWN_READY_TOO_CLOSE(int pram0, int pram1)l
extern _native void AMBIENT_SPAWN_PRESTREAM_SET(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void AMBIENT_SPAWN_PRESTREAM_CLEAR()l
#pragma endregion

#pragma region PHYSICS
extern _native int GET_PHYSINST_VELOCITY(int pram0, int pram1)l
extern _native void IS_PHYSINST_ACTIVE()l
extern _native void SET_PHYSINST_COLLIDES_AGAINST_INACTIVE(int pram0, int pram1)l
extern _native void SET_PHYSINST_FROZEN(int pram0, int pram1)l
extern _native void IS_PHYSINST_FROZEN()l
extern _native void SET_PHYSINST_HIDE()l
extern _native void IS_PHYSINST_HIDE()l
extern _native void BREAK_OFF_ABOVE(int pram0, int pram1)l
extern _native int GET_LOCATOR_OFFSETS(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_INFINITE_MASS_VS_MOVERS(int pram0, int pram1)l
extern _native void SET_GLOBAL_DISABLE_SPU_COLLIDER_UPDATE(int pram0)l
extern _native void SET_GLOBAL_AGGRESSIVE_CORPSE_RECYCLING(int pram0)l
extern _native void LIQUID_TEST_SET_VELOCITY_SCALE()l
extern _native void CLEAN_CACHE_ENTRIES()l
#pragma endregion

#pragma region PER_CHARS
extern _native int ACTIVATE_ACTOR_FOR_PERS_CHAR(int pram0)l
extern _native int DEACTIVATE_ACTOR_FOR_PERS_CHAR(int pram0)l
extern _native void DEACTIVATE_ACTORS_FOR_PERS_CHARS_IN_VOLUME(int pram0)l
extern _native int GET_ACTOR_FROM_PERS_CHAR(int pram0)l
extern _native int GET_PERS_CHAR_DEATH_TIMESTAMP(int pram0)l
extern _native int GET_PERS_CHAR_FROM_ACTOR(int pram0)l
extern _native int SET_PERS_CHAR_SAFE_ZONE(int pram0, int pram1)l
extern _native int IS_PERS_CHAR_ALIVE(int pram0)l
extern _native int REVIVE_PERS_CHAR(int pram0, int pram1)l
extern _native void SET_PERS_CHAR_EXEMPT_FROM_AMBIENT_RESTRICTIONS(int pram0, int pram1)l
extern _native void SET_PERS_CHAR_ENABLED(int pram0, int pram1)l
extern _native void SET_PERS_CHAR_ALLOW_SPAWN_ELSEWHERE(int pram0, int pram1)l
#pragma endregion

#pragma region PATH
extern _native void SET_PATH_LOOPING(int pram0, int pram1)l
extern _native int ADD_POINT_TO_PATH(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_POINT_IN_PATH()l
extern _native int GET_NUM_PATH_POINTS(int pram0)l
extern _native int GET_PATH_NUM_POINTS(int pram0)l
extern _native void GET_PATH_POINT(int pram0, int pram1, int pram2)l
extern _native void ESTIMATE_PATH_LENGTH()l
extern _native void ESTIMATE_DISTANCE_ALONG_PATH()l
extern _native void ESTIMATE_TWO_DISTANCES_ALONG_PATH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
#pragma endregion

#pragma region NAVQUERY
extern _native int CREATE_NAV_QUERY(int pram0, int pram1)l
extern _native int NAV_QUERY_IS_DONE(int pram0)l
extern _native int NAV_QUERY_CAN_PATH_TO_POINT(int pram0)l
extern _native void NAV_QUERY_RECEIVE_CAN_PATH_TO_POINT(int pram0, int pram1)l
extern _native void NAV_QUERY_START_CAN_PATH_TO_POINT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void NAV_QUERY_STOP(int pram0)l
#pragma endregion

#pragma region LASSO
extern _native int GET_LASSO_TARGET(int pram0)l
extern _native int GET_LASSO_USER(int pram0)l
extern _native int GET_HOGTIED_MASTER(int pram0)l
extern _native int GET_ATTACHED_HOGTIE_VICTIM(int pram0)l
extern _native void DETACH_LASSO(int pram0)l
extern _native void FREE_FROM_HOGTIE(int pram0)l
extern _native void LASSO_EVENT(int pram0, int pram1)l
extern _native int SET_INTENDED_HOGTIE_MASTER(int pram0, int pram1)l
extern _native int IS_ACTOR_BEING_DRAGGED(int pram0, int pram1)l
extern _native int IS_ACTOR_HOGTIED(int pram0)l
extern _native int IS_ACTOR_IN_HOGTIE(int pram0)l
extern _native int GET_ACTOR_HOGTIE_STATE(int actor)l
extern _native void HOGTIE_ACTOR(int pram0)l
extern _native int ATTACH_HOGTIE_ACTOR_TO_ACTOR(int pram0, int pram1)l
extern _native void SET_HOGTIE_ATTACH_VICTIM(int pram0, int pram1)l
extern _native void CLEAR_HOGTIE_ATTACH_VICTIM(int pram0)l
extern _native int IS_ACTOR_HOGTIE_ATTACHED(int pram0)l
extern _native int IS_HOGTIE_PUTDOWN_OBSTRUCTED(int pram0)l
extern _native int IS_HOGTIE_PICKUP_OBSTRUCTED(int pram0, int pram1)l
extern _native int IS_HOGTIE_CUTFREE_OBSTRUCTED(int pram0, int pram1)l
extern _native int IS_HOGTIE_HORSE_PICKUP_OBSTRUCTED(int pram0, int pram1)l
extern _native int IMMEDIATELY_LASSO_TARGET(int pram0, int pram1, int pram2)l
#pragma endregion

#pragma region GREETING
extern _native void SET_GREETING_CONTEXT(int pram0)l
extern _native void SET_NON_VERBAL_GREETING_PROBABILITY(int pram0)l
extern _native void SET_GREETING_LOOK_AT_ANGLE_DEFAULT(int pram0)l
extern _native void SET_GREETING_LOOK_AT_ANGLE_ACTOR_SPECIFIC(int pram0, int pram1)l
extern _native void SET_GREETING_PROBABILITY_NPC_DEFAULT(int pram0)l
extern _native void SET_GREETING_PROBABILITY_PLAYER_DEFAULT(int pram0)l
extern _native void SET_GREETING_PROBABILITY_ACTOR_SPECIFIC(int pram0, int pram1)l
extern _native void SET_GREETING_MIN_TIME_INTERVAL(int pram0)l
extern _native void SET_GREETING_MIN_TIME_INTERVAL_SAME_TARGET(int pram0)l
extern _native void SET_GREETING_MAX_DISTANCE(int pram0)l
extern _native void SET_GREETING_MIN_MOVEMENT_SPEED(int pram0)l
extern _native void SET_GREETING_MAX_MOVEMENT_ANGLE(int pram0)l
extern _native void SET_GREETING_ANIM_SIGNAL_TIMEOUT_DURATION(int pram0)l
#pragma endregion

#pragma region GAME
extern _native void DISABLE_PLAYER_GRINGO_USE(int pram0, int pram1)l
extern _native int IS_MISSION_SCRIPT()l
extern _native void SET_IS_MISSION_SCRIPT(int pram0)l
extern _native void SET_SCRIPT_AVOIDS_STRINGTABLE(int pram0)l
extern _native int GET_GAME_STATE()l
extern _native void SET_PAUSE_SCRIPT(int pram0)l
extern _native void ENABLE_USE_CONTEXTS(int pram0)l
extern _native int ARE_USE_CONTEXTS_ENABLED()l
extern _native int IS_SCRIPT_USE_CONTEXT_VALID(int pram0)l
extern _native int ADD_SCRIPT_USE_CONTEXT_IN_VOLUME(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native int ADD_SCRIPT_USE_CONTEXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int ADD_SCRIPT_USE_CONTEXT_STICK(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int IS_SCRIPT_USE_CONTEXT_PRESSED(int pram0)l
extern _native int WAS_SCRIPT_USE_CONTEXT_EVER_PRESSED(int pram0)l
extern _native void SET_USE_CONTEXT_TEXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void RELEASE_SCRIPT_USE_CONTEXT(int pram0)l
extern _native int NET_MAILBOX_IS_SIGNED_INTO_SC()l
extern _native int NET_MAILBOX_GET_MAX_NUM_CHALLENGES()l
extern _native void NET_MAILBOX_GET_NUM_CHALLENGES()l
extern _native int NET_MAILBOX_GET_CHALLENGE_BY_INDEX(int pram0)l
extern _native int NET_MAILBOX_GET_CHALLENGE_BY_ID(int pram0)l
extern _native int NET_MAILBOX_IS_CHALLENGE_VALID(int pram0)l
extern _native int SC_CHALLENGE_LAUNCH(int pram0)l
extern _native void SC_CHALLENGE_CLEAN_UP(int pram0)l
extern _native int SC_CHALLENGE_IS_RUNNING(int pram0)l
extern _native int SC_CHALLENGE_IS_ACTIVE(int pram0)l
extern _native int SC_CHALLENGE_GET_COMMUNITY_TOTAL(int pram0)l
extern _native int SC_CHALLENGE_GET_COMMUNITY_VALUE(int pram0)l
extern _native void SC_CHALLENGE_PROCESS_EXPIRATION(int pram0)l
extern _native int SC_CHALLENGE_GET_EXPIRATION_STATE(int pram0)l
extern _native void SC_CHALLENGE_RESET_EXPIRATION_STATE(int pram0)l
extern _native int SC_CHALLENGE_IS_VAR_VALID(int pram0, int pram1)l
extern _native int SC_CHALLENGE_GET_VAR_FLOAT(int pram0, int pram1)l
extern _native int SC_CHALLENGE_GET_VAR_INT(int pram0, int pram1)l
extern _native int SC_CHALLENGE_GET_VAR_BOOL(int pram0, int pram1)l
extern _native void SC_CHALLENGE_RELEASE(int pram0)l
extern _native int SC_CHALLENGE_GET_LEADERBOARD_ID(int pram0, int pram1)l
extern _native int SC_CHALLENGE_GET_MIN_LB_REFRESH_DELAY_SECS()l
extern _native int SC_CHALLENGE_GET_MIN_SUBMIT_DELAY_SECS()l
#pragma endregion

#pragma region COVER
extern _native int FIND_NEAREST_COVER_LOCATION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int FIND_COVER_LOCATIONS_IN_VOLUME(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void GET_COVER_LOCATION_BASE_POSITION(int pram0, int pram1)l
extern _native int GET_COVER_LOCATION_DIRECTION(int pram0)l
extern _native void GET_COVER_LOCATION_POSITION()l
extern _native int IS_COVER_LOCATION_VALID(int pram0)l
extern _native int ADD_AI_COVERSET_FOR_PROPSET(int pram0)l
#pragma endregion

#pragma region ANIMATION
extern _native int IS_OBJECT_ANIMATOR_VALID(int pram0)l
extern _native int CREATE_OBJECT_ANIMATOR(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int CREATE_OBJECT_ANIMATOR_ON_OBJECT(int pram0, int pram1, int pram2, int pram3)l
extern _native int IS_OBJECT_ANIMATOR_READY(int pram0)l
extern _native int GET_OBJECT_ANIMATOR_ON_OBJECT(int pram0)l
extern _native int SET_OBJECT_ANIMATOR_NODE(int pram0, int pram1)l
extern _native int SET_OBJECT_ANIMATOR_PHASE(int pram0, int pram1)l
extern _native int SET_OBJECT_ANIMATOR_RATE(int pram0, int pram1)l
extern _native int GET_OBJECT_ANIMATOR_PHASE(int pram0)l
extern _native int SET_OBJECT_ANIMATOR_BONE_RANGE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int SET_OBJECT_ANIMATOR_ANIMATION(int pram0, int pram1, int pram2)l
extern _native void IS_OBJECT_ANIMATOR_ANIMATION_PLAYING()l
extern _native void DESTROY_OBJECT_ANIMATOR(int pram0)l
extern _native int GET_OBJECT_FROM_ANIMATOR(int pram0)l
extern _native int LINK_OBJECT_ANIMATOR_TO_ACTOR(int pram0, int pram1, int pram2)l
#pragma endregion

#pragma region ACTOR
extern _native bool IS_ACTOR_VALID(Actor Actor)l
extern _native int GET_ACTORENUM_FROM_STRING(const char* ActorName)l //case sensitive
extern _native bool IS_ACTOR_ON_FOOT(int actor)l
extern _native int GET_ACTOR_OFFSET_WORLD_COORDS(int pram0, int pram1, int pram2)l
extern _native int TRANSFORM_ACTOR_TO_WORLD(int pram0, int pram1, int pram2)l
extern _native int ACTORS_IN_RANGE(int pram0, int pram1, int pram2)l
extern _native int GET_ACTOR_VELOCITY(int pram0, int pram1)l
extern _native int GET_ACTOR_HEIGHT(int pram0)l
extern _native void SET_GLOBAL_ACTOR_WEAPON_BIAS(int pram0)l
extern _native void RESET_GLOBAL_ACTOR_WEAPON_BIAS()l
extern _native int LOCATE_ACTOR_OF_TYPE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int IS_AREA_OBSTRUCTED(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int IS_AREA_OBSTRUCTED2(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int GET_ACTORENUM_SPECIES(int pram0)l
extern _native int GET_ACTORENUM_ACTOR_BASE_TYPE(int pram0)l
extern _native int GET_ACTORENUM_AVATAR_GROUP(int pram0)l
extern _native int GET_ACTORENUM_MP_ANIM_SET_NAME(int pram0)l
extern _native int GET_ACTORENUM_MP_VOICE_NAME(int pram0)l
extern _native void SET_ACTOR_STAMINA()l
extern _native int GET_ACTOR_GAIT_TYPE(int pram0)l
extern _native int GET_ACTOR_POSTURE(int pram0)l
extern _native void SET_ACTOR_POSTURE(int pram0, int pram1)l
extern _native void RESET_ACTOR_GAITS(int pram0, int pram1)l
extern _native void GET_ACTOR_TYPE()l
extern _native int IS_ACTOR_MALE(int pram0)l
extern _native void SET_ACTOR_SEX(int pram0, int pram1)l
extern _native void SET_ACTOR_IS_COMPANION(int pram0, int pram1)l
extern _native void SET_ACTOR_IS_THE_BEASTMASTER(int pram0, int pram1)l
extern _native int SET_ACTOR_TIME_OF_LAST_CRIME(int pram0, int pram1)l
extern _native void DESTROY_IMPAIRED_ACTORS()l
extern _native int IS_PLAYER_WEAPON_ZOOMED(int pram0)l
extern _native bool IS_ACTOR_ANIMAL(Actor Actor)l
extern _native int IS_ACTOR_CROUCHING(int pram0)l
extern _native int IS_ACTOR_FLYING(int pram0)l
extern _native bool IS_ACTOR_HUMAN(int actor)l
extern _native int IS_ACTOR_JUMPING(int pram0)l
extern _native bool IS_ACTOR_SHOOTING(Actor Actor)l
extern _native int IS_ACTOR_BLINDFIRING(int pram0)l
extern _native int IS_ACTOR_RELOADING(int pram0)l
extern _native int IS_ACTOR_THROWING(int pram0)l
extern _native int IS_ACTOR_WHISTLING(int pram0)l
extern _native int IS_ACTOR_ON_LADDER(int pram0)l
extern _native int IS_ACTOR_OUTDOORS(int pram0)l
extern _native void SUSPEND_MOVER(int pram0)l
extern _native void ENABLE_MOVER(int pram0)l
extern _native void SET_MOVER_FROZEN(int pram0, int pram1)l
extern _native int IS_MOVER_FROZEN(int pram0)l
extern _native int SET_ACTOR_USE_COARSE_BOUNDS(int pram0, int pram1)l
extern _native void SUPRESS_MOVER_COLLISIONS(int pram0, int pram1, int pram2)l
extern _native int IS_ACTOR_ON_GROUND(int actor)l
extern _native void IS_ACTOR_ON_PATH()l
extern _native void IS_ACTOR_ON_VEHICLEPATH()l
extern _native int IS_ACTOR_IN_WATER(int pram0)l
extern _native int IS_ACTOR_IN_WATER_OFDEPTH(int pram0, int pram1)l
extern _native int GET_ACTOR_STUCK_STATE(int pram0)l
extern _native void IS_ACTOR_IN_FIRE_VOLUME()l
extern _native int IS_ACTOR_RIDEABLE(int pram0)l
extern _native void SET_ACTOR_RIDEABLE(Actor Actor, bool Rideable)l
extern _native int GET_VEHICLE_BUMP_COUNT(int pram0)l
extern _native void RESET_VEHICLE_BUMP_COUNT(int pram0)l
extern _native void SET_CUSTOM_ANIM_SPEED(int pram0, int pram1)l
extern _native int ACTOR_RESET_ANIMS(int pram0, int pram1)l
extern _native32(0x817B6952) int _SCHEDULE_STOP_CUSTOM_UNFREEZE(int pram0)l
extern _native32(0x4A1D2E25) int _SCHEDULE_STOP_CUSTOM_UNSUSPEND(int pram0)l
extern _native32(0xC17BAD12) int _SCHEDULE_STOP_REPLICATION(int pram0)l
extern _native void SET_ACTOR_CAN_PLAY_BORED_IDLES(int pram0, int pram1)l
extern _native void SET_ACTOR_CAN_PLAY_GESTURES(int pram0, int pram1)l
extern _native void ACTOR_ENABLE_VARIABLE_MESH(int pram0, int pram1, int pram2)l
extern _native int ACTOR_IS_VARIABLE_MESH_ENABLED(int pram0, int pram1)l
extern _native void ACTOR_SET_GRABBED_BY_CUTSCENE(int pram0, int pram1)l
extern _native int ACTOR_IS_GRABBED_BY_CUTSCENE(int pram0)l
extern _native void ACTOR_IS_HIDDEN_BY_CUTSCENE()l
extern _native int IS_ACTOR_FULLY_FADED_EXT(int pram0, int pram1)l
extern _native int SET_ACTOR_HEARING_MAX_RANGE(int pram0, int pram1)l
extern _native void GET_ACTOR_VISION_FIELD_OF_VIEW()l
extern _native int SET_ACTOR_VISION_FIELD_OF_VIEW(int pram0, int pram1)l
extern _native int GET_ACTOR_VISION_MAX_RANGE(int pram0)l
extern _native int SET_ACTOR_VISION_MAX_RANGE(int pram0, int pram1, int pram2)l
extern _native int SET_UNIVERSAL_VISION_RANGE_MULTIPLIER(int pram0)l
extern _native int SET_ACTOR_VISION_XRAY(int pram0, int pram1)l
extern _native int GET_ACTOR_CURRENT_WEAPON_AI_PARAMETERS(int pram0, int pram1)l
extern _native int GET_ACTOR_ALLOW_BUMP_REACTIONS(int pram0)l
extern _native void SET_ACTOR_ALLOW_BUMP_REACTIONS(int pram0, int pram1)l
extern _native void SET_ACTOR_ALLOW_NM_BUMP_REACTIONS(int pram0, int pram1)l
extern _native void SET_RCM_ACTOR_CALL_OVER_ENABLE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void SET_RCM_WAS_JOHN_NOW_JACK(int pram0, int pram1)l
extern _native void SET_RCM_ACTOR_CALL_OVER_SUPPRESS(int pram0)l
extern _native void SET_ACTOR_ALLOW_PLAYER_GREET_RESPONSES(int pram0, int pram1)l
extern _native void SET_ACTOR_ALLOW_WEAPON_REACTIONS(int pram0, int pram1)l
extern _native void SET_ACTOR_ALLOW_WEAPON_REACTION_FLEE(int pram0, int pram1)l
extern _native int GET_ACTOR_WEAPON_REACTION_ACTOR_TYPE(int pram0)l
extern _native void SET_ACTOR_WEAPON_REACTION_ACTOR_TYPE(int pram0, int pram1)l
extern _native void SET_PLAYER_CAUSE_WEAPON_REACTIONS()l
extern _native void SET_ACTOR_WEAPON_REACTION_NO_FLEE_HACK(int pram0, int pram1)l
extern _native void SET_ACTOR_OBSERVED_TARGETED_REACTIONS()l
extern _native void SET_PLAYER_CAUSE_WEAPON_REACTION_COMBAT(int pram0, int pram1)l
extern _native void SET_ACTOR_REACT_TO_LASSO()l
extern _native void SET_ACTOR_ALLOW_DISARM(int pram0, int pram1)l
extern _native void SET_ANIMAL_CAN_ATTACK(int pram0, int pram1)l
extern _native int GET_CURRENT_GRINGO(int pram0)l
extern _native void SET_ACTOR_GRINGO_RESTRICTION(int pram0, int pram1)l
extern _native void CLEAR_ACTOR_GRINGO_RESTRICTION(int pram0)l
extern _native int MAKE_ACTOR_READY_FOR_ACTION(int pram0, int pram1)l
extern _native int IS_ACTOR_READY_FOR_ACTION(int pram0)l
extern _native void REPORT_GRINGO_USE_PHASE(int pram0, int pram1)l
extern _native void CLEAR_ALL_CORPSES()l
extern _native int CAN_PLAYER_DIE()l
extern _native void CLEAR_ACTOR_MAX_SPEED(int pram0)l
extern _native void SET_ACTOR_MAX_SPEED(int pram0, int pram1)l
extern _native void SET_ACTOR_MAX_SPEED_ABSOLUTE(int pram0, int pram1)l
extern _native void CLEAR_ACTOR_MIN_SPEED(int pram0)l
extern _native void SET_ACTOR_MIN_SPEED(int pram0, int pram1)l
extern _native void SET_ACTOR_MIN_SPEED_ABSOLUTE()l
extern _native void GET_ACTOR_MAX_SPEED()l
extern _native int GET_ACTOR_MAX_SPEED_ABSOLUTE(int pram0)l
extern _native void GET_ACTOR_MIN_SPEED()l
extern _native void SET_ACTOR_SPEED(int pram0, int pram1)l
extern _native void CLEAR_LAST_ATTACK(int pram0)l
extern _native int GET_LAST_ATTACK_TARGET(int pram0)l
extern _native int GET_LAST_ATTACK_TIME(int pram0)l
extern _native int GET_DAMAGE_BY_LOCAL_PLAYER(int pram0)l
extern _native int GET_ACTOR_COMBAT_CLASS(int pram0)l
extern _native32(0x8062BD74) void _DLC_SHOTGUN_SPREAD_OVERRIDE(int pram0, int pram1, int pram2)l
extern _native int BEGIN_DUEL(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11, int pram12)l
extern _native void CANCEL_DUEL()l
extern _native int ADD_DUEL_HOSTAGE(int pram0, int pram1)l
extern _native int GET_CURRENT_DUEL_SCORE(int pram0)l
extern _native int SET_DUEL_DIFFICULTY(int pram0, int pram1, int pram2, int pram3)l
extern _native void GET_ACTOR_EXEMPT_FROM_AMBIENT_RESTRICTIONS()l
extern _native void SET_ACTOR_EXEMPT_FROM_AMBIENT_RESTRICTIONS(int pram0, int pram1)l
extern _native void ADD_CAPABILITY(int pram0, int pram1)l
extern _native void REMOVE_CAPABILITY(int pram0, int pram1)l
extern _native int HAS_CAPABILITY(int pram0, int pram1)l
extern _native int GET_LAST_ON_SCREEN_TIME_FOR_ACTOR(int pram0)l
extern _native int NET_SET_NODE_REPLICATION(int pram0, int pram1, int pram2)l
extern _native void SET_ACTOR_ACTION_SIGNAL(int pram0, int pram1, int pram2)l
extern _native void TOGGLE_ACTOR_ACTION_SIGNAL_ON(int pram0, int pram1, int pram2)l
extern _native void TOGGLE_ACTOR_ACTION_SIGNAL_OFF(int pram0)l
extern _native int GET_ACTOR_MELEE_TARGETED_BY(int pram0)l
extern _native void SET_ACTOR_CAN_DEADEYE_TAG_ANYTHING()l
extern _native void SET_ACTOR_AUTO_TRANSITION_TO_DRIVER_SEAT(int pram0, int pram1)l
extern _native void SET_ACTOR_FLY_FX()l
extern _native void SET_ACTOR_MOVE_CONFLICT_HIGH_PRIORITY(int pram0, int pram1)l
extern _native void SET_ACTOR_MOVE_CONFLICT_ALLOWED_TO_RUN_OVER_SMALL_ANIMALS(int pram0, int pram1)l
extern _native void SET_ACTOR_IS_AMBIENT(int pram0, int pram1)l
extern _native void SET_ACTOR_IS_SHOPKEEPER(int pram0, int pram1, int pram2)l
extern _native void SET_ACTOR_SHOULD_TAUNT(int pram0, int pram1)l
extern _native void SET_ACTOR_CAN_BUMP(int pram0, int pram1)l
extern _native int SET_ACTOR_MAX_FRESHNESS(int pram0, int pram1)l
extern _native int GET_ACTOR_MAX_FRESHNESS(int pram0)l
extern _native void MAKE_BIRD_FLY_FROM_POINT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
#pragma endregion

#pragma region DECORATOR
extern _native int DECOR_GET_BOOL_VERBOSE(int pram0, int pram1, int pram2)l
extern _native int DECOR_GET_FLOAT_VERBOSE(int pram0, int pram1, int pram2)l
extern _native int DECOR_GET_INT_VERBOSE(int pram0, int pram1, int pram2)l
extern _native int DECOR_SET_BOOL(Actor Actor, const char* DecorName, bool set)l
extern _native int DECOR_SET_FLOAT(int pram0, int pram1, int pram2)l
extern _native int DECOR_SET_INT(int pram0, int pram1, int pram2)l
extern _native int DECOR_SET_VECTOR(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int DECOR_SET_STRING(int pram0, int pram1, int pram2)l
extern _native int DECOR_SET_OBJECT(int pram0, int pram1, int pram2)l
extern _native int DECOR_CHECK_STRING(Object decor, const char* EventType, const char* EventName)l
extern _native int DECOR_GET_STRING_HASH(int pram0, int pram1)l
extern _native int DECOR_GET_BOOL(int pram0, int pram1)l
extern _native int DECOR_GET_FLOAT(int pram0, int pram1)l
extern _native int DECOR_GET_INT(int pram0, int pram1)l
extern _native int DECOR_GET_VECTOR(int pram0, int pram1, int pram2)l
extern _native int DECOR_GET_OBJECT(int pram0, int pram1)l
extern _native int DECOR_CHECK_EXIST(int pram0, int pram1)l
extern _native int DECOR_REMOVE(int pram0, int pram1)l
extern _native int DECOR_REMOVE_ALL(int pram0)l
#pragma endregion

#pragma region WEAPON_SCRIPT
extern _native void INIT_NATIVE_WEAPONENUM_PISTOL(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_RIFLE(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_REPEATER(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_SNIPERRIFLE(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_SHOTGUN(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_THROWN(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_THROWN_EXPLODING(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_BOW(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_MELEE(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_LASSO(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_TURRET(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_CANNON(int pram0, int pram1, int pram2)l
extern _native void INIT_NATIVE_WEAPONENUM_AMMO(int pram0, int pram1)l
extern _native int GET_WEAPON_ENUM_STRING_FROM_ENUM(int pram0)l
extern _native const char* GET_WEAPON_DISPLAY_NAME(eWeapon WeaponID)l
extern _native int GET_WEAPON_INTERNAL_NAME(int pram0)l
extern _native32(0xBE06C265) void _GET_WEAPON_ICON_NAME()l
extern _native int GET_WEAPON_FRAGMENT_NAME(int pram0)l
extern _native void INIT_NATIVE_WEAPONENUM_CATEGORY(int pram0, int pram1)l
extern _native void FINALIZE_WEAPONENUM_CATEGORIES()l
extern _native void SET_WEAPONENUM_LOCKED(int pram0, int pram1)l
extern _native int IS_WEAPONENUM_LOCKED(int pram0)l
extern _native int GET_WEAPON_CATEGORY_FROM_ENUM(int pram0)l
extern _native int GET_AMMO_ENUM(int pram0)l
extern _native void SET_AMMO_DROP_BIAS(int pram0, int pram1)l
extern _native int GET_AMMO_ENUM_STRING_FROM_ENUM(int pram0)l
extern _native int GET_AMMO_ENUM_ICON_NAME(int pram0)l
extern _native int GET_WEAPON_MAX_AMMO(int pram0)l
extern _native void RESOLVE_DLC_WEAPONENUM(int pram0)l
extern _native void ADD_IDLEFX_TO_WEAPON(int pram0, int pram1, int pram2, int pram3, int pram4)l
#pragma endregion

#pragma region GRINGO
extern _native int IS_GRINGO_ACTIVE()l
extern _native int IS_GRINGO_READY(int pram0)l
extern _native int GRINGO_IS_ACTIVE(int pram0)l
extern _native int GRINGO_GET_TARGET(int pram0)l
extern _native void GRINGO_WAIT(int pram0)l
extern _native void GRINGO_FORCE_UPDATE()l
extern _native void GRINGO_STOP()l
extern _native int GRINGO_STAY_ACTIVE(int pram0, int pram1)l
extern _native int GRINGO_DEACTIVATE(int pram0)l
extern _native int GRINGO_DEACTIVATE_AND_RESET_ACTORS(int pram0)l
extern _native void GRINGO_ENABLE_TYPE(int pram0)l
extern _native void GRINGO_DISABLE_TYPE(int pram0)l
extern _native int GRINGO_ENABLE_SPAWN(int pram0, int pram1)l
extern _native int LOCATE_GRINGO_OF_TYPE(int pram0, int pram1, int pram2, int pram3)l
extern _native int LOCATE_GRINGO_OF_TYPE_BY_ID(int pram0, int pram1, int pram2)l
extern _native int LOCATE_GRINGOS_OF_TYPE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int LOCATE_GRINGO_OF_NAME(int pram0, int pram1, int pram2, int pram3)l
extern _native int LOCATE_GRINGOS_OF_NAME(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GRINGO_ACTOR_CAN_USE(int pram0, int pram1, int pram2)l
extern _native int GRINGO_SETUP_PROP_ASSOCIATION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10)l
extern _native int GRINGO_SETUP_ATTR_ASSOCIATION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11)l
extern _native void GRINGO_CLEAR_PROP_ASSOCIATION()l
extern _native void GRINGO_LOAD_ANIMATION(int pram0)l
extern _native void GRINGO_UNLOAD_ANIMATION(int pram0)l
extern _native void GRINGO_LOAD_ANIMATION_FOR_USER(int pram0, int pram1)l
extern _native void GRINGO_UNLOAD_ANIMATION_FOR_USER(int pram0, int pram1)l
extern _native int GRINGO_HAS_ANIMSET_LOADED(int pram0, int pram1)l
extern _native void GRINGO_LOAD_ANIMATION_FOR_ACTOR_ENUM_EXT(int pram0, int pram1, int pram2, int pram3)l
extern _native void GRINGO_UNLOAD_ANIMATION_FOR_ACTOR_ENUM_EXT(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_HAS_ANIMSET_LOADED_FOR_ACTOR_ENUM_EXT(int pram0, int pram1, int pram2, int pram3)l
extern _native void GRINGO_ENABLE_PLAYER_CONTROL(int pram0, int pram1)l
extern _native void GRINGO_IS_USABLE_BY_PLAYER()l
extern _native void GRINGO_SET_USABLE_BY_PLAYER(int pram0, int pram1, int pram2)l
extern _native int GRINGO_USABLE_BY_ACTOR_ENUM(int pram0, int pram1, int pram2)l
extern _native int GRINGO_DEBUG_IS_SELECTED()l
extern _native32(0xA0E660E) int _GRINGO_PLAYER_OVERRIDE_ON()l
extern _native int GRINGO_GET_MY_OBJECT_REF()l
extern _native int GRINGO_GET_MY_GRINGO_ID()l
extern _native32(0xBBB2780E) int _GRINGO_IS_BREAK_TOGGLE_SET()l
extern _native void GRINGO_SET_ALL_USES_OWNERSHIP(int pram0, int pram1)l
extern _native void GRINGO_SET_USE_OWNERSHIP(int pram0, int pram1, int pram2)l
extern _native void GRINGO_SET_CHILD_USE_ACTIVATIONS(int pram0, int pram1)l
extern _native void GRINGO_SET_COMMON_LAYER_USE_ACTIVATIONS(int pram0, int pram1)l
extern _native int GRINGO_HANDLES_MOVEMENT(int pram0)l
extern _native int IS_GRINGO_COMPONENT_VALID(int pram0)l
extern _native int GRINGO_COMPONENT_CHECK_NAME(int pram0, int pram1)l
extern _native int GRINGO_GET_COMPONENT_HASH(int pram0)l
extern _native int GRINGO_IS_COMPONENT_OF_TYPE(int pram0, int pram1)l
extern _native int GRINGO_GET_FIRST_NAMED_CHILD(int pram0, int pram1, int pram2)l
extern _native int GRINGO_GET_FIRST_CHILD(int pram0, int pram1)l
extern _native int GRINGO_GET_INDEX_OF_NEXT_NAMED_CHILD(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_GET_INDEX_OF_NEXT_CHILD(int pram0, int pram1, int pram2)l
extern _native int GRINGO_GET_CHILD_COMPONENT_COUNT(int pram0)l
extern _native int GRINGO_GET_INDEXED_CHILD_COMPONENT(int pram0, int pram1)l
extern _native int GRINGO_GET_PROP_FROM_COMPONENT(int pram0)l
extern _native int GRINGO_GET_PROP_FROM_COMPONENT_EXT(int pram0, int pram1)l
extern _native int GRINGO_GET_PARENT_COMPONENT(int pram0, int pram1)l
extern _native int GRINGO_GET_USER_POS_WITH_OFFSET(int pram0, int pram1, int pram2)l
extern _native void GRINGO_SET_MESSAGE_RETURN(int pram0)l
extern _native int GRINGO_GET_MSG_COMPONENT_CONTEXT()l
extern _native int GRINGO_HAS_PENDING_MESSAGE()l
extern _native int GRINGO_GET_MESSAGE_TYPE()l
extern _native int GRINGO_GET_REQUESTING_ACTOR()l
extern _native void GRINGO_SET_REQUEST_STRING(int pram0)l
extern _native void GRINGO_SET_REQUEST_FAILURE_REASON(int pram0)l
extern _native int GRINGO_IS_FORCE_QUITTING()l
extern _native int GRINGO_IS_CAPABLE_OF_USE(int pram0, int pram1)l
extern _native int GRINGO_SHOULD_SUSPEND_MOVER(int pram0)l
extern _native int GRINGO_SHOULD_FIX_MOVER(int pram0)l
extern _native int GRINGO_GET_USE_COMPONENT_POSITION(int pram0, int pram1)l
extern _native int GRINGO_GET_USE_COMPONENT_POSITION_EXT(int pram0, int pram1, int pram2)l
extern _native int GRINGO_GET_USE_COMPONENT_OFFSET_POSITION_EXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int GRINGO_GET_USE_COMPONENT_EXT(int pram0, int pram1)l
extern _native int GRINGO_GET_USE_COMPONENT_POSITION_LOCAL(int pram0, int pram1)l
extern _native int GRINGO_GET_USE_REQUIRES_OBSTACLE(int pram0)l
extern _native int GRINGO_IS_CHARACTER_BLEND_PAUSED(int pram0)l
extern _native int GRINGO_GET_TASKED_USER_EXTERNAL(int pram0, int pram1)l
extern _native void GRINGO_SET_REUSE_DELAY(int pram0, int pram1)l
extern _native int GRINGO_GET_USE_COMPONENT_HEADING(int pram0)l
extern _native int GRINGO_GET_USE_COMPONENT_HEADING_EXT(int pram0, int pram1)l
extern _native void GRINGO_SET_AVAILABILITY(int pram0, int pram1)l
extern _native int GRINGO_GET_AVAILABILITY(int pram0, int pram1)l
extern _native int GRINGO_SET_AVAILABILITY_EXT(int pram0, int pram1, int pram2)l
extern _native int GRINGO_GET_REQUEST_MID_ACTION()l
extern _native void GRINGO_SET_COMPONENT_USER(int pram0, int pram1)l
extern _native int GRINGO_GET_COMPONENT_USER(int pram0)l
extern _native void GRINGO_CLEAR_COMPONENT_USER(int pram0)l
extern _native int GRINGO_QUERY_NAMED_COMPONENT_USER(int pram0, int pram1)l
extern _native int GRINGO_REWARD_ACTOR(int pram0, int pram1, int pram2)l
extern _native int GRINGO_REPORT_USE_FINISHED(int pram0, int pram1)l
extern _native int GRINGO_WAS_USE_SUCCESSFUL(int pram0)l
extern _native void GRINGO_SATISFY_MOTIVES_OF_ACTOR(int pram0, int pram1)l
extern _native int GRINGO_PLAY_ANIM_ON_ACTOR(int pram0, int pram1, int pram2)l
extern _native int GRINGO_IS_ACTOR_PLAYING_ANIM(int pram0, int pram1)l
extern _native int GRINGO_RETURN_ACTOR_TO_DEFAULT_ANIMS(int pram0)l
extern _native int GRINGO_OWNS_ACTOR_ANIMS(int pram0)l
extern _native int GRINGO_PLAY_ANIM_ON_ACTOR_WITH_PROP_COMPONENT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void GRINGO_ACTOR_DROP_ATTACHED_PROP(int pram0)l
extern _native int ATTACH_PROP_TO_ANIM(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int GRINGO_ATTACH_PROP_TO_ANIM(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GRINGO_GET_PROP_ATTACH_SLOT(int pram0)l
extern _native int ATTACH_SLOT_FROM_STRING(int pram0)l
extern _native int GRINGO_ANIM_GET_AST_FILENAME(int pram0)l
extern _native int GRINGO_ANIM_GET_ACT_FILENAME(int pram0)l
extern _native int GRINGO_GET_ACTOR_SPECIFIC_AST_FILENAME(int pram0, int pram1)l
extern _native int GRINGO_ANIM_GET_ACT_ROOTNODE(int pram0)l
extern _native int GRINGO_ANIM_IS_SUBNODE_PLAYING_BY_HASH(int pram0, int pram1, int pram2)l
extern _native int GRINGO_ANIM_PLAY_NODE_BY_HASH(int pram0, int pram1, int pram2)l
extern _native int GRINGO_ACTOR_FACE(int pram0, int pram1, int pram2)l
extern _native int GRINGO_ACTOR_MOVE_TO(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GRINGO_ACTOR_MOVE_TO_AND_FACE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int GRINGO_ACTOR_MOVE_TO_AND_FACE_WITH_USER_OFFSET(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void GRINGO_SET_PROP_COLLISIONS(int pram0, int pram1)l
extern _native void GRINGO_REINIT_PROP(int pram0)l
extern _native int GRINGO_IS_PROP_READY(int pram0)l
extern _native int GRINGO_GET_PHYSINST(int pram0)l
extern _native void GRINGO_PROP_RESET_GRACEFULLY(int pram0)l
extern _native int GRINGO_IS_TARGET_OBJECT_READY()l
extern _native int GET_GRINGO_BOOL_ATTR(int pram0, int pram1, int pram2)l
extern _native int GET_GRINGO_INT_ATTR(int pram0, int pram1, int pram2)l
extern _native int GET_GRINGO_OBJECT_REF_ATTR(int pram0, int pram1, int pram2)l
extern _native int GET_GRINGO_STRUCT_ATTR(int pram0, int pram1, int pram2, int pram3)l
extern _native int GET_GRINGO_FLOAT_ATTR(int pram0, int pram1, int pram2)l
extern _native int GET_GRINGO_VECTOR_ATTR(int pram0, int pram1, int pram2)l
extern _native int GET_GRINGO_RELATIVE_POSITION_ATTR(int pram0, int pram1, int pram2)l
extern _native int GET_GRINGO_RELATIVE_ORIENTATION_ATTR(int pram0, int pram1, int pram2)l
extern _native int GET_GRINGO_STRING_ATTR(int pram0, int pram1, int pram2)l
extern _native int SET_GRINGO_BOOL_ATTR(int pram0, int pram1, int pram2)l
extern _native int SET_GRINGO_INT_ATTR(int pram0, int pram1, int pram2)l
extern _native int SET_GRINGO_OBJECT_REF_ATTR(int pram0, int pram1, int pram2)l
extern _native int SET_GRINGO_FLOAT_ATTR(int pram0, int pram1, int pram2)l
extern _native int SET_GRINGO_VECTOR_ATTR(int pram0, int pram1, int pram2)l
extern _native int GRINGO_QUERY_BOOL(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_QUERY_INT(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_QUERY_OBJECT_REF(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_QUERY_STRUCT(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GRINGO_QUERY_FLOAT(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_QUERY_STRING(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_QUERY_PROP(int pram0, int pram1)l
extern _native int GRINGO_UPDATE_BOOL(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_UPDATE_INT(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_UPDATE_OBJECT_REF(int pram0, int pram1, int pram2, int pram3)l
extern _native int GRINGO_UPDATE_STRUCT(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int GRINGO_GET_ATTRIBUTE_COUNT(int pram0)l
extern _native int GRINGO_GET_ATTRIBUTE(int pram0, int pram1)l
extern _native int GRINGO_IS_ATTRIBUTE_VALID(int pram0)l
extern _native int GRINGO_GET_ATTRIBUTE_HASH(int pram0)l
extern _native int GRINGO_GET_VECTOR_ATTR_BY_HANDLE(int pram0, int pram1)l
extern _native int GRINGO_GET_STRING_ATTR_BY_HANDLE(int pram0, int pram1)l
extern _native int GRINGO_GET_FLOAT_ATTR_BY_HANDLE(int pram0, int pram1)l
extern _native int GRINGO_GET_ATTR_TYPE_BY_HANDLE(int pram0)l
#pragma endregion

#pragma region DLC
extern _native void DLC_PRE_INIT_CONTENT()l
extern _native void DLC_INIT_CONTENT()l
extern _native void DLC_PREINITIALIZE_FRAGMENT_STUBS(int pram0)l
extern _native void DLC_INIT_STRINGTABLE_STREAMABLES(int pram0, int pram1)l
extern _native void DLC_IS_CONTENT_PURCHASED_FLAGS()l
#pragma endregion

#pragma region NET
extern _native void NET_SET_TUNING_PARAM()l
extern _native void UNK_0x50E637D7()l
extern _native void NET_LOG(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void NET_DUMP_STATE()l
extern _native int NET_ENABLE_MULTIPLAYER(int pram0)l
extern _native int NET_IS_MANAGER_INITIALIZED()l
extern _native int NET_IS_IN_SESSION()l
extern _native int NET_IS_ONLINE_AVAILABLE()l
extern _native int NET_IS_CONNECTED_FOR_PLAY()l
extern _native int NET_GET_PLAYMODE()l
extern _native void NET_APPLY_PROTOCOL_MASK()l
extern _native void NET_SET_SOCIAL_CLUB_URLS()l
extern _native int NET_IS_SESSION_HOST()l
extern _native int NET_IS_SESSION_CLIENT()l
extern _native void NET_GET_MAC_ADDRESS32(int pram0)l
extern _native int NET_GET_NAT_TYPE()l
extern _native int NET_IS_BUSY()l
extern _native int NET_GET_NET_TIME()l
extern _native void NET_ENABLE_KICKING(int pram0)l
extern _native void NET_IS_LOCAL_GAMER_ONLINE()l
extern _native int NET_GET_LOCAL_GAMER_NAME()l
extern _native void NET_APPLY_RELEVANCY_OVERRIDE()l
extern _native void NET_CLEAR_RELEVANCY_OVERRIDE()l
extern _native int GET_SLOT_FOR_HOST()l
extern _native int GET_NUM_PLAYERS()l
extern _native int NET_START_NEW_SCRIPT(int pram0, int pram1)l
extern _native int NET_SCRIPTMSG_SEND(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int NET_SCRIPTMSG_ISPENDING(int pram0, int pram1)l
extern _native int NET_SCRIPTMSG_GETNEXT(int pram0, int pram1, int pram2)l
extern _native int NET_SCRIPTMSG_REGISTER_HANDLER(int pram0, int pram1)l
extern _native int NET_SCRIPTMSG_QUERY_HANDLER(int pram0, int pram1)l
extern _native void REGISTER_HOST_BROADCAST_VARIABLES(int pram0, int pram1)l
extern _native void REGISTER_CLIENT_BROADCAST_VARIABLES(int pram0, int pram1)l
extern _native void UNREGISTER_HOST_BROADCAST_VARIABLES()l
extern _native void UNREGISTER_CLIENT_BROADCAST_VARIABLES()l
extern _native32(0xF81E2097) int _NET_IS_SLOT_VALID_IN_CLIENT_BROADCAST_VARIABLES(int pram0, int pram1)l
extern _native32(0x64C2DD40) int _NET_IS_CLIENT_DATA_VALID_FOR_ALL_SLOTS(int pram0, int pram1)l
extern _native int IS_DATA_VALID_FOR_HOST_BROADCAST_VARIABLES(int pram0)l
extern _native int NET_IS_OBJECT_LOCAL(int pram0)l
extern _native int NET_REQUEST_OBJECT(int pram0)l
extern _native void NET_OBJECT_REPLICATION_MODE_START(int pram0, int pram1)l
extern _native void NET_OBJECT_REPLICATION_MODE_END(int pram0)l
extern _native int NET_OBJECT_GET_REPLICATION_MODE(int pram0)l
extern _native void NET_OBJECT_SET_REPLICATION_MODE(int pram0, int pram1)l
extern _native int NET_OBJECT_LOCK_OWNERSHIP(int pram0, int pram1)l
extern _native32(0x1306549E) void _NET_SET_REPLICATE_ALWAYS_TO_LAYOUT(int pram0, int pram1)l
extern _native32(0x5C4CAE3A) void _NET_SET_LAYOUT_OBJECTS_REPLICATE_TO_LAYOUT(int pram0)l
extern _native int NET_ACTOR_GET_SCRIPT_INT(int pram0)l
extern _native void NET_ACTOR_SET_EQUIP_TYPE(int pram0)l
extern _native void NET_ACTOR_SET_SCRIPT_INT(int pram0, int pram1)l
extern _native void NET_OBJECT_SET_SCRIPT_INT(int pram0, int pram1)l
extern _native int NET_OBJECT_GET_SCRIPT_INT(int pram0)l
extern _native void NET_ACTOR_SET_ALLOW_BLIP_SYNC(int pram0, int pram1)l
extern _native void NET_ACTOR_SET_GRINGO_NAVIGATION_COMPLETE(int pram0, int pram1)l
extern _native int NET_GET_SESSION_GAMER_COUNT()l
extern _native int AWARD_ACHIEVEMENT(int pram0)l
extern _native int HAS_ACHIEVEMENT_BEEN_PASSED(int pram0)l
extern _native void ARE_ACHIEVEMENTS_READY()l
extern _native int AWARD_AVATAR(int pram0)l
extern _native int NET_GET_POSSE_COUNT()l
extern _native int NET_IS_POSSE_LEADER()l
extern _native int NET_GET_POSSE_LEADER_SLOT()l
extern _native int NET_GET_GAMER_POSSE_LEADER(int pram0)l
extern _native int NET_GET_GAMER_POSSE_SIZE(int pram0)l
extern _native int NET_POSSE_REMOVE_GAMER(int pram0)l
extern _native int NET_POSSE_IS_INVITE_PRESENT(int pram0)l
extern _native int NET_POSSE_IS_REQUEST_PRESENT(int pram0)l
extern _native int NET_RUN_SEARCH_BOT(int pram0)l
extern _native int NET_GET_NUMBER_OF_SESSIONS()l
extern _native int NET_IS_SEARCHBOT_BUSY()l
extern _native int NET_SEARCHBOT_FILTER_RESET(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void NET_SEARCHBOT_USE_QUICKMATCH()l
extern _native int NET_SESSION_QUICK_JOIN_NATIVE(int pram0)l
extern _native int NET_SESSION_LEAVE_SESSION()l
extern _native int NET_IS_FACTION_SAFE(int pram0)l
extern _native int NET_SESSION_START_GAMEPLAY()l
extern _native int NET_SESSION_END_GAMEPLAY()l
extern _native int NET_SESSION_SET_INVITABLE(int pram0)l
extern _native int NET_SET_SLOTS_PRIORITY_CLIENTS(int pram0)l
extern _native void NET_SET_SESSION_CLOSED_FOR_JOIN(int pram0)l
extern _native int NET_SESSION_IS_GAMEPLAY_STARTED()l
extern _native void NET_SET_QUICKMATCH_PLAYLIST_RANGE(int pram0, int pram1)l
extern _native void NET_SESSION_REQUEST_BECOME_HOST()l
extern _native void NET_SET_THIS_SCRIPT_IS_NET_SCRIPT(int pram0)l
extern _native void NET_UNREGISTER_AS_NET_SCRIPT()l
extern _native int NET_GET_SCRIPT_STATUS()l
extern _native int NET_IS_PLAYER_PARTICIPANT(int pram0)l
extern _native int NET_IS_HOST_OF_THIS_SCRIPT()l
extern _native int NET_GET_HOST_OF_THIS_SCRIPT()l
extern _native void NET_ALLOW_PLAYERS_TO_JOIN(int pram0)l
extern _native int NET_IS_SCRIPT_REGISTERED_AS_NET_SCRIPT()l
extern _native int NET_SCRIPT_GET_NUM_PARTICIPANTS()l
extern _native void SET_RICH_PRESENCE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void NET_GAMERDATA_SET(int pram0, int pram1)l
extern _native int NET_GET_KILL_EFFECT_ON()l
extern _native void NET_VOICE_BROADCAST_ENABLE()l
extern _native void NET_VOICE_BROADCAST_DISABLE()l
extern _native int NET_ARE_UNLOCKS_READY()l
extern _native int NET_IS_UNLOCKED(int pram0)l
extern _native void NET_GET_OVERLOAD_STATE_FOR_SLOT()l
extern _native int NET_GET_AREA_OVERLOAD_STATE_FOR_SLOT(int pram0)l
extern _native32(0x842ADE0A) void _NET_SET_OVERLOAD_REDUCTION_LEVEL()l
extern _native void NET_SET_SYNC_PRIORITY_LIMITS()l
extern _native void UPDATE_PROFILE_STAT(int pram0, int pram1, int pram2)l
extern _native void UPDATE_STRING_PROFILE_STAT(int pram0, int pram1)l
extern _native32(0x97F15B69) void _NET_AH_LAG_HACK_KILL_PROTECTION_ENABLE()l
extern _native32(0xCA0739A8) void _NET_AH_LAG_HACK_MOVE_PROTECTION_ENABLE()l
extern _native void NET_BROADCAST_EXPLODE_TARGET_EVENT(int pram0)l
extern _native void DO_FILE_CRC()l
extern _native void FLAG_FILE_CRC_MISMATCH()l
#pragma endregion

#pragma region PLAYER_NAMES
extern _native void NET_GAMER_SET_ACTOR_OVERRIDE(int pram0, int pram1)l
extern _native void NET_GAMER_SET_TEAM(int pram0, int pram1)l
extern _native int NET_GAMER_SETMY_JPN_TITLE(int pram0)l
extern _native int NET_GAMER_GET_JPN_TITLE(int pram0)l
extern _native void NET_GAMER_BARKER_ACCEPT(int pram0)l
extern _native void NET_GAMER_BARKER_REJECT(int pram0)l
extern _native void NET_GAMER_SET_TITLE(int pram0, int pram1)l
extern _native void NET_GAMER_ICON_SET_LIFE()l
extern _native void NET_GAMER_ICON_FORCE_VISIBILITY(int pram0, int pram1)l
extern _native void NET_GAMER_ICON_RESET_TIMER(int pram0, int pram1)l
extern _native void NET_GAMER_ICON_FORCE_HIDDEN()l
extern _native void NET_GAMER_ICON_FORCE_TEXT_VISIBLE(int pram0)l
extern _native void NET_GAMER_ICON_FORCE_VISIBLE(int pram0)l
extern _native void NET_GAMER_ICONS_SHOW_LOCAL(int pram0)l
extern _native void NET_GAMER_ICONS_SET_STEALTH(int pram0)l
extern _native void NET_GAMER_BLIPS_SET_STEALTH(int pram0)l
extern _native int NET_HUD_TUNE_VALUE(int pram0, int pram1)l
extern _native void NET_GAMER_SET_ICON_STEALTH_OVERRIDE(int pram0, int pram1)l
extern _native void NET_GAMER_SET_BLIP_STEALTH_OVERRIDE(int pram0, int pram1)l
extern _native void NET_GAMER_SET_BLIP_OVERRIDE(int pram0, int pram1)l
extern _native void NET_GAMER_BLIPS_FORCE_VISIBLE(int pram0)l
extern _native void NET_GAMER_BLIPS_SHOW_POSSE_MEMBERS(int pram0)l
extern _native void NET_GAMER_SET_BLIP_FORCE_HIDDEN()l
extern _native void NET_GAMER_BLIPS_SHOW_ON_FULL_MAP(int pram0)l
extern _native void NET_GAMER_IS_BLIP_VISIBLE()l
extern _native void NET_GAMER_BLIPS_TREAT_COVER_AS_ALIVE(int pram0)l
extern _native int NET_POSSE_GET_LEADER_WAYPOINT(int pram0)l
extern _native int NET_POSSE_IS_LEADER_WAYPOINT_VALID()l
#pragma endregion

#pragma region UI_NET_SCORE_GRAPH
extern _native void NET_SCOREGRAPH_SETUP(int pram0, int pram1, int pram2)l
extern _native void NET_SCOREGRAPH_CLEAR_MARKERS()l
extern _native void NET_SCOREGRAPH_ADD_PLAYER_SCORE(int pram0, int pram1, int pram2, int pram3)l
extern _native void NET_SCOREGRAPH_ADD_PLAYER_LABEL(int pram0, int pram1, int pram2, int pram3)l
extern _native void NET_SCOREGRAPH_ADD_TEAM_SCORE(int pram0, int pram1, int pram2, int pram3)l
extern _native void NET_SCOREGRAPH_ADD_TEAM_LABEL(int pram0, int pram1, int pram2, int pram3)l
#pragma endregion

#pragma region UI_NET_SCORE_TABLE_CURRENT_ITEM
extern _native void NET_PLAYER_LIST_SET_CURRENT_ITEM(int pram0)l
extern _native int NET_PLAYER_LIST_SET_CURRENT_ITEM_BY_SLOT(int pram0)l
extern _native int NET_PLAYER_LIST_SET_CURRENT_ITEM_MSCORE_STRING(int pram0, int pram1)l
extern _native int NET_PLAYER_LIST_SET_CURRENT_ITEM_MSCORE_INT(int pram0, int pram1)l
extern _native int NET_PLAYER_LIST_SET_CURRENT_ITEM_TEAM(int pram0)l
extern _native void NET_PLAYER_LIST_SET_CURRENT_ITEM_PRIORITY(int pram0)l
extern _native int NET_PLAYER_LIST_SET_CURRENT_ITEM_DEAD(int pram0)l
#pragma endregion

#pragma region UI_NET_SCORE_TABLE
extern _native void NET_PLAYER_LIST_RESET()l
extern _native int NET_PLAYER_LIST_ADD_ITEM(int pram0, int pram1)l
extern _native int NET_PLAYER_LIST_ADD_GAMER_SLOT(int pram0, int pram1)l
extern _native void NET_PLAYER_LIST_SET_HIGHLIGHT(int pram0)l
extern _native void NET_PLAYER_LIST_SET_TOP_TEAM(int pram0)l
extern _native void NET_PLAYER_LIST_SET_TEAM_SCORE(int pram0, int pram1, int pram2)l
extern _native void NET_PLAYER_LIST_SET_TEAM_SORT()l
extern _native void NET_PLAYER_LIST_SET_TITLE(int pram0)l
extern _native void NET_PLAYER_LIST_SET_TEMPLATE(int pram0)l
extern _native void NET_PLAYER_LIST_SET_HEADER(int pram0, int pram1)l
extern _native void NET_PLAYER_LIST_SET_DESCRIPTION(int pram0)l
extern _native void NET_PLAYER_LIST_TIMER_SET(int pram0)l
extern _native void NET_PLAYER_LIST_TIMER_ENABLE_FLASHING(int pram0)l
#pragma endregion

#pragma region UI_NET
extern _native int NET_GET_AND_CLEAR_GAME_MODE_REQUEST()l
extern _native int NET_GET_AND_CLEAR_PLAYLIST_REQUEST()l
extern _native int NET_GET_AND_CLEAR_QUIT_GAME_REQUEST()l
extern _native int NET_GET_FREE_ROAM_MODE()l
extern _native void NET_SET_FREE_ROAM_MODE(int pram0)l
extern _native void NET_REGISTER_GAME_TYPE(int pram0, int pram1)l
extern _native void NET_REGISTER_PLAYLIST_TYPE(int pram0, int pram1, int pram2)l
extern _native void NET_SET_PLAYLIST_LOCKED(int pram0, int pram1)l
extern _native int NET_GET_JOINWISH_PAD()l
extern _native int NET_AUTHENTICATE_GAMER(int pram0, int pram1)l
extern _native void NET_GET_PLAYER_COLOR_IDX()l
extern _native int NET_GET_GAMER_HEX_COLOR(int pram0, int pram1)l
extern _native int NET_GET_GAMER_RGB_COLOR(int pram0, int pram1, int pram2, int pram3)l
extern _native int NET_IS_GAMER_RADAR_BLIP_VISIBLE(int pram0)l
extern _native32(0x8DEC3E03) void _NET_SYS_UI_SET_POSITION(int pram0)l
extern _native void NET_PLAYER_BARKER_RESET(int pram0, int pram1, int pram2, int pram3)l
extern _native int NET_GET_USING_SPHERE_CURVES(int pram0)l
extern _native void NET_PLAYER_SHOW_CONTEXT_MENU(int pram0, int pram1)l
extern _native void NET_TICKER_REPORTF(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void NET_TICKER_CLEAR()l
extern _native void NET_XP_TOTAL_REPORT_CHANGE(int pram0, int pram1)l
#pragma endregion

#pragma region NET_SESSION
extern _native32(0x79AFAB1F) int _NET_SESSION_GET_NORMALIZE_POSITION(int pram0, int pram1)l
extern _native32(0x581CAC89) int _NET_SESSION_GET_CURRENT_SEARCH_GENERATION()l
extern _native32(0xA174152C) int _NET_SESSION_GET_GENERATION_FOR_NET_SESSION_OBJECT(int pram0)l
extern _native32(0x183A3F0) int _NET_SESSION_GET_JOINED_GENERATION_FROM_NET_SESSION_OBJECT(int pram0)l
extern _native int NET_JOIN_SESSION_FROM_OBJ(int pram0)l
extern _native32(0xE9EAC45C) int _NET_GET_SESSION_GAMEMODE_TYPE(int pram0)l
extern _native int IS_SESSION_CURRENTLY_JOINED_SESSION(int pram0)l
extern _native void NET_SESSION_SET_GAME_LOCALE_EX(int pram0)l
extern _native void NET_SESSION_SET_GAME_MODE_TYPE(int pram0)l
#pragma endregion

#pragma region NET_GAME_TYPE_INSTANCE_DATA
extern _native32(0x55C5BB93) int _NET_GAMETYPE_ADD_GAMETYPE_ENTRY(int pram0)l
extern _native32(0xFAD5A270) int _NET_GAMETYPE_GET_GAMETYPE_ENTRY(int pram0, int pram1)l
extern _native int GAME_INSTANCE_ITERATOR_START(int pram0)l
extern _native int GAME_INSTANCE_ITERATOR_NEXT(int pram0)l
extern _native int GAME_INSTANCE_SET_REGION(int pram0, int pram1)l
extern _native int ADD_PLAYLIST_TO_DB(int pram0, int pram1)l
extern _native int GET_PLAYLIST_FROM_DB(int pram0, int pram1)l
extern _native int GET_PLAYLIST_FROM_DB_BY_NAME(int pram0, int pram1)l
#pragma endregion

#pragma region NET_STATS
extern _native void NET_UPDATE_LEADERBOARD(int pram0, int pram1, int pram2, int pram3)l
extern _native void NET_START_LB_UPDATE(int pram0)l
extern _native void NET_END_LB_UPDATE()l
extern _native void NET_CREATE_FRIEND_SCORE_READER(int pram0)l
extern _native void NET_REPORT_FRIEND_SCORES(int pram0, int pram1, int pram2)l
extern _native void NET_GET_LOCAL_GAMER_SCORE_INFO(int pram0)l
extern _native void NET_GET_NEAREST_FRIEND_SCORE_INFO(int pram0)l
extern _native int NET_GET_NEAREST_FRIEND_NAME()l
extern _native int NET_GET_SLOT_OF_NEAREST_FRIEND()l
extern _native int NET_IS_FRIEND_SCORE_INFO_AVAILABLE()l
extern _native int NET_GET_LOCAL_GAMER_RANK()l
extern _native int NET_GET_NEAREST_FRIEND_RANK()l
extern _native void NET_LB_FRIENDS_SELECT_NEAREST()l
extern _native int NET_CHALLENGE_LB_CREATE_READER(int pram0, int pram1)l
extern _native void NET_CHALLENGE_LB_DESTROY_READER()l
extern _native int NET_CHALLENGE_LB_DOES_READER_EXIST()l
extern _native int NET_CHALLENGE_LB_IS_READ_COMPLETE()l
extern _native int NET_CHALLENGE_LB_IS_LOCAL_GAMER_INFO_VALID()l
extern _native int NET_CHALLENGE_LB_GET_LOCAL_GAMER_NAME()l
extern _native int NET_CHALLENGE_LB_GET_LOCAL_GAMER_STATS(int pram0)l
extern _native int NET_CHALLENGE_LB_IS_TOP_FRIEND_INFO_VALID()l
extern _native int NET_CHALLENGE_LB_GET_TOP_FRIEND_NAME()l
extern _native int NET_CHALLENGE_LB_GET_TOP_FRIEND_STATS(int pram0)l
extern _native int NET_CHALLENGE_LB_IS_TOP_SCORER_INFO_VALID()l
extern _native int NET_CHALLENGE_LB_GET_TOP_SCORER_NAME()l
extern _native int NET_CHALLENGE_LB_GET_TOP_SCORER_STATS(int pram0)l
extern _native int NET_CHALLENGE_LB_IS_SCORER_ABOVE_PLAYER_INFO_VALID()l
extern _native int NET_CHALLENGE_LB_GET_SCORER_ABOVE_PLAYER_NAME()l
extern _native int NET_CHALLENGE_LB_GET_SCORER_ABOVE_PLAYER_STATS(int pram0)l
#pragma endregion

#pragma region ENTITY_COMPONENT
extern _native void SET_DRAW_ACTOR(Actor pram0, bool ToDraw)l
extern _native bool GET_DRAW_ACTOR(Actor Actor)l
#pragma endregion

#pragma region PPP_ELEMENTS
extern _native void WANTEDFX_SET_OPACITY(int pram0)l
extern _native void FIRE_BLOODSPLATFX(int pram0, int pram1, int pram2, int pram3)l
extern _native void CLEAR_DEATHFX()l
extern _native void BINOCULARSFX_ENABLED(int pram0)l
extern _native int BINOCULARSFX_ISLOADED()l
extern _native void ZOMBIEMODE_ENABLED()l
extern _native void TEXTUREFX_SET_OPACITY(int pram0)l
extern _native void TEXTUREFX_SET_POSITION(int pram0, int pram1)l
extern _native void TEXTUREFX_ENABLED(int pram0)l
extern _native void TEXTUREFX_SET_NAME(int pram0)l
extern _native void TEXTUREFX_SET_SCALE(int pram0)l
#pragma endregion

#pragma region WORLD_MOVIE_PLAYER
extern _native32(0x7614AEBA) void _START_WORLD_MOVIE(int pram0, int pram1)l
extern _native32(0x69FC319E) void _STOP_WORLD_MOVIE()l
extern _native32(0xD036DF91) int _IS_WORLD_MOVIE_PLAYING()l
#pragma endregion

#pragma region ICONS
extern _native void UI_ADDICON(int pram0, int pram1)l
#pragma endregion

#pragma region GRAVESTONE_TEXT
extern _native int IS_GRAVESTONE_SECTOR_READY()l
extern _native void SET_CURRENT_GRAVESTONE_SECTOR(int pram0)l
extern _native int CREATE_GRAVESTONE_TEXT_IN_LAYOUT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int CREATE_WANTEDPOSTER_TEXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native Object CREATE_MP_TEXT(Object Prop, const char* UnkNullStr, const char* GXTTextToDisplay, vector3 Position, vector3 Rotation, bool MaybeIsStringLiteral)l
#pragma endregion

#pragma region FX
extern _native void ENABLE_PIP(int pram0, int pram1, int pram2)l
extern _native int IS_PIP_ENABLED(int pram0)l
extern _native void IS_PIP_RENDERING()l
extern _native void CLEAR_DECALS()l
extern _native void CREATE_DECAL(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void CREATE_DIRECTION_DECAL(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native void CREATE_DECAL_WITH_NORMAL(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10)l
extern _native void CREATE_FOOTPRINT()l
extern _native32(0x13A0D25) void _FX_SET_MELEE_MARKER_POS(int pram0, int pram1, int pram2)l
extern _native32(0x1182C34F) void _FX_SET_MELEE_MARKER_SIZE()l
extern _native32(0xD0FB6AF0) void _FX_SET_MELEE_MARKER_COLOR(int pram0, int pram1, int pram2, int pram3)l
extern _native32(0xC00F8181) void _FX_SET_MELEE_MARKER_STROBE_INTENSITY(int pram0)l
extern _native32(0x4897DD37) void _FX_ENABLE_SCRIPT_MELEE_MARKER(int pram0)l
extern _native void PPP_LOAD_PRESET(int pram0)l
extern _native void PPP_UNLOAD_PRESET(int pram0)l
extern _native void PPP_SET_ELEMENT_MAGNITUDE(int pram0, int pram1)l
extern _native int PPP_GET_ELEMENT_MAGNITUDE(int pram0)l
extern _native void CANCEL_DEADEYE()l
extern _native void FIRE_SHOCK(int pram0)l
extern _native void SET_SHOCK_SPEED(int pram0)l
extern _native void SET_SHOCK_AMPLITUDE(int pram0)l
extern _native void DOF_PUSH()l
extern _native void DOF_POP()l
extern _native void DOF_SET(int pram0, int pram1, int pram2, int pram3)l
extern _native void DOF_SET_KERNEL(int pram0)l
extern _native int ADD_GLOW_TO_OBJECT(int pram0, int pram1, int pram2, int pram3)l
extern _native void REMOVE_GLOW_INDICATOR(int pram0)l
extern _native void CREATE_OBJECT_GLOW(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void DESTROY_OBJECT_GLOW(int pram0)l
extern _native void CLEAR_TUMBLEWEEDS()l
extern _native void ALLOW_TUMBLEWEEDS(int pram0)l
extern _native int ADD_ZOMBIE_TO_ACTOR(int pram0, int pram1, int pram2)l
extern _native void ADD_BLOOD_TO_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void ADD_BLOOD_TO_CORPSE()l
extern _native void CLEAR_CHARACTER_BLOOD()l
extern _native void CLEAR_PLAYER_BLOOD()l
extern _native void SET_PLAYER_BLOOD_FADE_RATE(int pram0)l
extern _native void PRICK_PLAYER_FINGER(int pram0)l
extern _native void BURN_ACTOR(int pram0, int pram1, int pram2)l
extern _native void LIMIT_BLOOD_ON_ACTOR(int pram0, int pram1)l
extern _native void LOAD_PTFX_DLC_ASSETS(int pram0)l
extern _native void ADDSHADER(int pram0, int pram1)l
#pragma endregion

#pragma region TIME_OF_DAY
extern _native int GET_MINUTES_FROM_TIME_OF_DAY(int pram0)l
extern _native int GET_TIME_OF_DAY_AS_INT()l
extern _native int GET_TOTAL_MINUTES(int pram0)l
extern _native int CONVERT_TIME_OF_DAY_TO_INT(int pram0)l
extern _native void SET_DAY(int pram0)l
extern _native int GET_HOUR(Time T)l
extern _native int GET_MINUTE(Time T)l
extern _native int GET_SECOND(Time T)l
extern _native int GET_DAY(Time T)l
extern _native void ADD_TIME(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ADD_TIME_USING_TIME_OF_DAY(int pram0, int pram1)l
extern _native Time GET_TIME_OF_DAY()l
extern _native Time MAKE_TIME_OF_DAY(int Hour, int Minute, int Second)l
extern _native int MAKE_TIME_OF_DAY_EX(int pram0, int pram1, int pram2, int pram3)l
extern _native int IS_LATER_THAN(int pram0, int pram1)l
extern _native int IS_EARLIER_THAN(int pram0, int pram1)l
extern _native int TIME_IS_IN_RANGE(int pram0, int pram1, int pram2)l
extern _native void ADVANCE_TIME_HOURS(int pram0)l
extern _native void SET_TIME_OF_DAY(Time UnixTime)l
extern _native void SET_TIME_ACCELERATION(int pram0)l
extern _native int GET_TIME_ACCELERATION()l
extern _native void SET_TIME_WARP()l
extern _native void CANCEL_TIME_WARP(int pram0)l
#pragma endregion

#pragma region HUD_GUI
extern _native int GUI_MAKE_WINDOW(GUIWindow Parent, int pram1, const char* WindowName, const char* pram3)l
extern _native int GUI_MAKE_TEXT(int GUIHandle, SizedArray Position, const char* MenuTitle, const char* GXTText, float pram4)l
extern _native int GUI_MAKE_OVERLAY(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native GUIWindow GUI_WINDOWS_WITH_VALUES(int GUIHandle, const char* WindowName)l
extern _native int GUI_MOVE_WINDOW_ABS(int pram0, int pram1, int pram2)l
extern _native int GUI_CLOSE_WINDOW(int pram0)l
extern _native int GUI_WINDOW_VALID(int pram0)l
extern _native int GUI_SET_TEXT(int TextHandle, const char* MenuName)l
extern _native int GUI_SET_TEXT_JUSTIFY(int TextHandle, int JustifyType)l
extern _native int GUI_SET_TEXT_COLOR(int TextHandle, float* ColorArea)l
extern _native GUIWindow GUI_MAIN_WINDOW()l
extern _native void UNK_0xB498108A()l
extern _native void UNK_0xC7DEB717()l
extern _native int GUI_MESSAGE_RELEASED_INPUT(int pram0)l
extern _native void F2VSTR()l
extern _native int F2STR(int pram0, int pram1, int pram2)l
extern _native const char* I2STR(const char* pram0)l
extern _native int V2STR(int pram0)l
extern _native int FLOAT_TO_STRING_VERBOSE(int pram0)l
extern _native const char* FLOAT_TO_STRING_FORMATED(float Flt, int unk3, int Precision)l
extern _native const char* INT_TO_STRING(int i)l
extern _native const char* VECTOR_TO_STRING(vector3* Vec)l
#pragma endregion

#pragma region HUD
extern _native void UI_SET_PROMPT_STRING(int pram0, int pram1)l
extern _native void UI_SET_PROMPT_ICON(int pram0, int pram1)l
extern _native void UI_HIDE_PROMPT(int pram0)l
extern _native int FLASH_SET_BOOL(int pram0, int pram1, int pram2)l
extern _native int FLASH_SET_INT(const char* scaleformName, const char* scaleformVarName, int value)l
extern _native int FLASH_SET_FLOAT(int pram0, int pram1, int pram2)l
extern _native int FLASH_SET_ARRAY_INT(int pram0, int pram1, int pram2, int pram3)l
extern _native int FLASH_SET_STRING(const char* scaleformName, const char* scaleformVarName, int pram2, int pram3)l
extern _native int FLASH_SET_ARRAY_STRING(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void FLASH_GET_BOOL()l
extern _native int FLASH_GET_INT(const char* scaleformName, const char* scaleformVarName)l
extern _native int FLASH_GET_FLOAT(int pram0, int pram1)l
extern _native int FLASH_SET_ARRAY_STRING_FORMATTED(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native void PRINT_BIG(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void PRINT_SMALL_B(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void PRINT_HELP_B(char* txt, float time, bool isStringLiteral, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void PRINT_OBJECTIVE_B(const char* txt, float time, bool isStringLiteral, int printType, bool pram4, int pram5, bool pram6, int pram7)l
extern _native void PRINT_MONEY(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void PRINT_BIG_FORMAT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void PRINT_SMALL_FORMAT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native void PRINT_OBJECTIVE_FORMAT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10)l
extern _native void PRINT_HELP_FORMAT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int GET_LAST_PRINTED_OBJECTIVE()l
extern _native void CLEAR_PRINTED_OBJECTIVE()l
extern _native int HUD_IS_SHOWING_SMALL_TEXT()l
extern _native int HUD_IS_SHOWING_BIG_TEXT()l
extern _native int HUD_IS_SHOWING_OBJECTIVE()l
extern _native int HUD_IS_SHOWING_HELP_TEXT()l
extern _native void HUD_CLEAR_SMALL_TEXT()l
extern _native void HUD_CLEAR_BIG_TEXT()l
extern _native void HUD_CLEAR_COUNTER()l
extern _native void HUD_CLEAR_OBJECTIVE()l
extern _native void HUD_CLEAR_HELP()l
extern _native void HUD_CLEAR_SMALL_TEXT_QUEUE()l
extern _native void HUD_CLEAR_BIG_TEXT_QUEUE()l
extern _native void HUD_CLEAR_OBJECTIVE_QUEUE()l
extern _native void HUD_CLEAR_HELP_QUEUE()l
extern _native void SET_RADAR_STREAMING()l
extern _native int ADD_BLIP_FOR_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int ADD_BLIP_FOR_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int ADD_BLIP_FOR_COORD(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void CHANGE_BLIP_ICON(int pram0, int pram1)l
extern _native void SET_BLIP_POS(int pram0, int pram1)l
extern _native void SET_BLIP_SCALE(int pram0, int pram1)l
extern _native void SET_BLIP_COLOR(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SET_BLIP_PRIORITY(int pram0, int pram1)l
extern _native void SET_BLIP_BLINK(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_BLIP_FLAG(int pram0, int pram1, int pram2)l
extern _native void SET_BLIP_VISIBLE(int pram0, int pram1)l
extern _native int IS_BLIP_VISIBLE(int pram0)l
extern _native int IS_BLIP_VALID(int pram0)l
extern _native void REMOVE_BLIP(int pram0)l
extern _native int GET_BLIP_ON_ACTOR(int pram0)l
extern _native Blip GET_BLIP_ON_OBJECT(Object Obj)l
extern _native int GET_BLIP_ICON(int pram0)l
extern _native int SET_CURRENT_MAP(int pram0)l
extern _native void SET_STAMINA_BLINK(int pram0)l
extern _native void SET_DEADEYE_BLINK(int pram0)l
extern _native void SET_HUD_MAP_SCALE_WALK(int pram0)l
extern _native void SET_HUD_MAP_SCALE_DRIVE(int pram0)l
extern _native void SET_HUD_MAP_SCALE_OVERRIDE(int pram0, int pram1)l
extern _native int GET_RADAR_RADIUS()l
extern _native void ABORT_HUD_MAP_SCALE_OVERRIDE()l
extern _native void IS_HUD_MAP_SCALE_OVERRIDE_SET()l
extern _native void UNK_0x2148AC15(int pram0, int pram1)l
extern _native void UNK_0x444C3C32(int pram0, int pram1, int pram2)l
extern _native void SET_BLIP_HUDMAP_ONLY(int pram0, int pram1)l
extern _native void SET_BLIP_PAUSEMAP_ONLY(int pram0, int pram1)l
extern _native int GET_BLIP_IMPAIRMENT_MASK(int pram0)l
extern _native void SET_BLIP_IMPAIRMENT_MASK(int pram0, int pram1)l
extern _native void SET_BLIP_MAX_DISTANCE(int pram0, int pram1)l
extern _native void SET_BLIP_MIN_DISTANCE(int pram0, int pram1)l
extern _native32(0x6077F3AE) void _SET_BLIP_HEIGHT_ENABLED(int pram0, int pram1)l
extern _native void SET_BLIP_NAME(int pram0, int pram1)l
extern _native void CLEAR_REGIONS()l
extern _native int APPEND_REGION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void SET_REGION_VISITED(int pram0, int pram1)l
extern _native int IS_HUD_MAP_DRAW_ENABLED()l
extern _native void SET_HUD_MAP_DRAW_ENABLED(int pram0)l
extern _native void SET_HUD_MAP_DRAW_ENABLED_TIMED(int pram0, int pram1, int pram2)l
extern _native int IS_HUD_MAP_VISIBLE()l
extern _native void SET_RADAR_TILES_VISIBLE(int pram0)l
extern _native void HUD_SET_SHOOT_BLIP_MAX_DISTANCE(int pram0)l
extern _native void HUD_SET_SHOOT_BLIP_ENABLED_FOR_ACTOR(int pram0, int pram1)l
extern _native void HUD_CLEAR_SHOOT_BLIP_ICON_FOR_ACTOR(int pram0)l
extern _native int HUD_GET_SHOOT_BLIP_ICON_FOR_ACTOR(int pram0)l
extern _native void HUD_SET_SHOOT_BLIP_ICON_FOR_ACTOR(int pram0, int pram1)l
extern _native void SET_STAT_MESSAGE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11, int pram12)l
extern _native void CLEAR_STAT_MESSAGE()l
extern _native int IS_STAT_MESSAGE_SHOWING()l
extern _native void HUD_TIMER_DISPLAY(int pram0)l
extern _native int HUD_TIMER_GET()l
extern _native void HUD_TIMER_SET()l
extern _native void HUD_TIMER_COUNTUP()l
extern _native void HUD_TIMER_COUNTDOWN(int pram0)l
extern _native void HUD_TIMER_PAUSE()l
extern _native void HUD_TIMER_UNPAUSE()l
extern _native void WANTEDFX_ENABLED(int pram0)l
extern _native void WANTEDFX_SET_LEVEL(int pram0)l
extern _native void HUD_COUNTER_DISPLAY(int pram0)l
extern _native void HUD_COUNTER_SET(int pram0)l
extern _native void HUD_ENABLE(int pram0)l
extern _native void HUD_SET_FADE_COLOR(int pram0, int pram1, int pram2, int pram3)l
extern _native void HUD_FADE_OUT(int pram0, int pram1, int pram2)l
extern _native void HUD_FADE_TO_LOADING_SCREEN()l
extern _native void HUD_FADE_IN(int pram0, int pram1)l
extern _native void HUD_FADE_IN_NOW(int pram0, int pram1)l
extern _native int HUD_IS_FADED()l
extern _native int HUD_IS_FADING()l
extern _native void UI_SHOW_MISSION_LOADINGSCREEN(int pram0)l
extern _native void UI_SHOW_RANDOM_LOADINGSCREEN()l
extern _native void FLASH_INTRO_SHUTDOWN()l
extern _native void FLASH_INTRO_FADE_LOGO(int pram0)l
extern _native void FLASH_INTRO_FADE_PRESS_START(int pram0)l
extern _native int FLASH_INTRO_ARE_LEGALS_COMPLETED()l
extern _native void MOVIE_PLAYER_STOP_MOVIE()l
extern _native int UI_IS_SHOWING_DIALOG()l
extern _native void UI_TRANSITION_TO(int pram0)l
extern _native int UI_GET_REBOOT_REASON(int pram0)l
extern _native void UI_SEND_EVENT(int pram0)l
extern _native void UI_ENTER(const char* uiLayer)l
extern _native void CLEAR_NEWSPAPER()l
extern _native void SHOW_NEWSPAPER(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_NEWSPAPER_INFO(int pram0, int pram1)l
extern _native void SET_GPS_PATH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native int IS_GPS_RECOMPUTE(int pram0)l
extern _native void CLEAR_GPS_PATH(int pram0)l
extern _native int GET_USER_DEFINED_WAYPOINT(int pram0)l
extern _native int IS_USER_DEFINED_WAYPOINT_CLEARED()l
extern _native int FIND_TRAFFIC_PATH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int IS_TELEPORTATION(int pram0)l
extern _native int GET_LAST_NEAREST_POINT(int pram0, int pram1)l
extern _native int HUD_SET_MINIGAME_TYPE_LAYOUT(int pram0)l
extern _native void HUD_STAMINA_OVERRIDE(int pram0, int pram1, int pram2)l
extern _native int UI_CHEAT_GET_VALUE(int pram0)l
extern _native void UI_CHEAT_SET_VALUE(int pram0, int pram1)l
extern _native int UI_CHEAT_GET_STATE(int pram0)l
extern _native void UI_CHEAT_SET_STATE(int pram0, int pram1)l
extern _native void UI_CHEAT_SET_CODE(int pram0, int pram1)l
extern _native void UI_SET_HAS_CHEATED(int pram0)l
extern _native int UI_HAS_CHEATED()l
extern _native void UI_DUMP_MESSAGE_QUEUE_TO_NOTES(int pram0)l
extern _native void UI_CLEAR_MESSAGE_QUEUE(int pram0)l
extern _native int UI_IS_MESSAGE_QUEUE_EMPTY(int pram0)l
extern _native void UI_REMOVE_MESSAGE_IN_QUEUE(int pram0, int pram1, int pram2)l
extern _native void HUD_SET_CENTER_BLIP_SHOWN(int pram0)l
extern _native int IS_DLC_ZOMBIEPACK_ACTIVE()l
extern _native int IS_HARDCORE_ACTIVE()l
extern _native32(0x3842B89F) void _HUD_SET_ZOMBIE_THEME()l
extern _native32(0xFDB5FC03) void _HUD_SET_RED_DEAD_THEME()l
#pragma endregion

#pragma region WORLD
extern _native int CLEAR_AREA_OF_TREE_TYPE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void RESET_TREE_TYPE_CLEARING()l
extern _native void RESET_THIS_TREE_TYPE_CLEARING(int pram0)l
extern _native int SET_DUST_LEVEL(int pram0)l
extern _native int SET_DUST_LEVEL_MODIFIER(int pram0)l
extern _native int SET_DUST_LEVEL_MID(int pram0)l
extern _native int SET_DUST_LEVEL_FAR(int pram0)l
extern _native int CLEAR_AREA_OF_GRASS(int pram0, int pram1, int pram2, int pram3)l
extern _native int CLEAR_AREA_OF_BREAKABLE_TREES(int pram0, int pram1, int pram2, int pram3)l
extern _native void RESET_THIS_BREAKABLE_TREE_CLEARING(int pram0)l
extern _native void RESET_ALL_BREAKABLE_TREE_CLEARINGS()l
extern _native void RELOAD_SMICTIONARY_LIST()l
extern _native void ZOMBIE_DLC_IS_ACTIVE()l
extern _native int ZOMBIE_DLC_LOAD_ASSETS()l
extern _native void ZOMBIE_DLC_LOAD_ASSETS_MP()l
extern _native int ZOMBIE_DLC_UNLOAD_ASSETS()l
extern _native int ZOMBIE_DLC_IS_LOAD_COMPLETE()l
extern _native void ZOMBIE_DLC_IS_UNLOAD_COMPLETE()l
extern _native void SET_PHOSPHORUS_AMMO_ACTIVE(int pram0)l
extern _native void IS_PHOSPHORUS_AMMO_ACTIVE()l
extern _native void CREATE_FIRE_ON_OBJECT()l
extern _native int CREATE_FIRE_IN_VOLUME(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void STOP_ALL_FIRES()l
extern _native32(0x8011737F) void _STOP_ALL_FIRES_OLD()l
extern _native int CREATE_FIRE_PROPERTY()l
extern _native void GET_FIRE_PROPERTY()l
extern _native void STOP_ALL_FIRE_IN_SPHERE(int pram0, int pram1, int pram2, int pram3)l
extern _native void STOP_ALL_FIRE_IN_VOLUME()l
extern _native void REPLACE_WORLD_SECTOR()l
extern _native void REPLACE_WORLD_SECTOR_LOAD_BOUNDING_BOX(int pram0)l
extern _native void ENABLE_WORLD_SECTOR(int pram0)l
extern _native void DISABLE_WORLD_SECTOR(int pram0)l
extern _native void ENABLE_CHILD_SECTOR(int pram0)l
extern _native void DISABLE_CHILD_SECTOR(int pram0)l
extern _native void HIDE_CHILD_SECTOR()l
extern _native void SHOW_CHILD_SECTOR()l
extern _native int FIRE_CREATE_HANDLE()l
extern _native int FIRE_IS_HANDLE_VALID(int pram0)l
extern _native void FIRE_RELEASE_HANDLE(int pram0, int pram1)l
extern _native void FIRE_RELEASE_INFINITE_HANDLE()l
extern _native int FIRE_FIND_HANDLE_FROM_ATTACHED_ACTOR(int pram0)l
extern _native void FIRE_CREATE_ON_ACTOR(int pram0, int pram1)l
extern _native int FIRE_CREATE_IN_VOLUME(int pram0, int pram1, int pram2, int pram3)l
extern _native int FIRE_IS_ACTOR_ON_FIRE(int pram0)l
extern _native void FIRE_STOP_ALL_FIRES()l
extern _native void FIRE_STOP_ON_ACTOR(int pram0)l
extern _native void FIRE_STOP_FLAMES_IN_VOLUME(int pram0)l
extern _native int FIRE_GET_OWNER(int pram0)l
extern _native void FIRE_SET_OWNER(int pram0, int pram1)l
extern _native void FIRE_SET_DAMAGE_ALLOWED(int pram0, int pram1)l
extern _native void FIRE_SET_CONTROL_VOLUME(int pram0, int pram1)l
extern _native void FIRE_SET_MAX_FLAMES(int pram0, int pram1)l
extern _native void FIRE_SET_TARGET_FILL_PERCENT(int pram0, int pram1)l
extern _native void FIRE_SET_GROW_RATE(int pram0, int pram1)l
extern _native void FIRE_SET_DECAY_RATE()l
extern _native void FIRE_SET_EXPIRE_ALLOWED(int pram0, int pram1)l
extern _native void FIRE_SET_GROW_ALLOWED(int pram0, int pram1)l
extern _native int COUNT_FLAMES_IN_SPHERE(int pram0, int pram1, int pram2, int pram3)l
extern _native int COUNT_FLAMES_IN_VOLUME(int pram0)l
extern _native int FIRE_ARE_ANY_FLAMES_IN_VOLUME(int pram0)l
extern _native int FIRE_SET_GUNS_BLAZING_ACTIVE(bool Activate)l
#pragma endregion

#pragma region WORLD_PROP
extern _native void IS_PROP_BROKEN()l
extern _native int GET_PROP_VELOCITY(int pram0, int pram1)l
extern _native int SET_PROP_VELOCITY(int pram0, int pram1)l
extern _native int SET_PROP_ANGULAR_VELOCITY_DEGS(int pram0, int pram1)l
extern _native int SET_PROP_VELOCITY_ON_AXIS(int pram0, int pram1, int pram2)l
extern _native int IS_PHYSINST_VALID(int pram0)l
extern _native int IS_PHYSINST_READY(int pram0)l
extern _native int IS_PHYSINST_IN_LEVEL(int pram0)l
extern _native void SET_SLEEP_TOLERANCE(int pram0, int pram1)l
extern _native void SET_BRIDGE_STIFFNESS(int pram0, int pram1)l
extern _native void ACTIVATE_PHYSINST(int pram0)l
extern _native int HAS_PHYSINST_BROKEN_APART(int pram0)l
extern _native int LOCATE_PHYSINST_OF_TYPE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int LOCATE_PHYSINST_OF_PARTIAL_TYPE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int LOCATE_PHYSINSTS_OF_PARTIAL_TYPE(int pram0, int pram1, int pram2, int pram3)l
extern _native void ATTACH_PHYSINST_TO_WORLD2()l
extern _native int ATTACH_PHYSINST_TO_WORLD2_HIGH_QUALITY(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void RELEASE_CONSTRAINT(int pram0)l
extern _native void HIDE_PHYSINST(int pram0)l
extern _native void SHOW_PHYSINST(int pram0)l
extern _native void SET_SECTOR_PROPS_SUPER_LOCKED(int pram0, int pram1)l
extern _native void LIGHTS_SET_ON_TIME(int pram0, int pram1)l
extern _native void LIGHTS_SET_OFF_TIME(int pram0, int pram1)l
extern _native void SET_PROP_NO_FADE()l
#pragma endregion

#pragma region STREAM
extern _native void STREAMING_REQUEST_ACTOR(int pram0, int pram1, int pram2)l
extern _native int STREAMING_IS_ACTOR_LOADED(int pram0, int pram1)l
extern _native void STREAMING_EVICT_ACTOR(int pram0, int pram1)l
extern _native int STREAMING_IS_WORLD_LOADED()l
extern _native int STREAMING_IS_WORLD_LOADED_PRIORITY(int pram0)l
extern _native int STREAMING_ARE_BOUNDS_LOADED(int pram0, int pram1, int pram2, int pram3)l
extern _native int STREAMING_REQUEST_PROP(int pram0, int pram1)l
extern _native int STREAMING_IS_PROP_LOADED(int pram0)l
extern _native void STREAMING_EVICT_PROP(int pram0)l
extern _native void STREAMING_REQUEST_PROPSET(int pram0)l
extern _native int STREAMING_IS_PROPSET_LOADED(int pram0)l
extern _native void STREAMING_EVICT_PROPSET(int pram0)l
extern _native void STREAMING_REQUEST_GRINGO(int pram0)l
extern _native int STREAMING_IS_GRINGO_LOADED(int pram0)l
extern _native void STREAMING_EVICT_GRINGO(int pram0)l
extern _native void STREAMING_REQUEST_GRINGO_DICTIONARY(int pram0)l
extern _native int STREAMING_IS_GRINGO_DICTIONARY_LOADED(int pram0)l
extern _native void STREAMING_EVICT_GRINGO_DICTIONARY(int pram0)l
extern _native void STREAMING_REQUEST_SCRIPT(int pram0)l
extern _native int STREAMING_IS_SCRIPT_LOADED(int pram0)l
extern _native void STREAMING_EVICT_SCRIPT(int pram0)l
extern _native void STREAMING_LOAD_ALL_REQUESTS_NOW()l
extern _native void STREAMING_LOAD_SCENE_EXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void STREAMING_UNLOAD_SCENE()l
extern _native void STREAMING_GET_POI_POS()l
extern _native void STREAMING_OVERRIDE_MAIN_POI(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void STREAMING_RELEASE_MAIN_POI()l
extern _native int STREAMING_HAS_OVERRIDE_MAIN_POI()l
extern _native void STREAMING_GET_OVERRIDE_MAIN_POI_POS(int pram0)l
extern _native void STREAMING_ENABLE_POI_STREAMING(int pram0)l
extern _native void STREAMING_PREVENT_SNIPER_MODE(int pram0)l
extern _native void STREAMING_SET_CUTSCENE_MODE(int pram0)l
extern _native void STREAMING_ENABLE_BOUNDS(int pram0)l
extern _native void STREAMING_LOAD_BOUNDS(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void STREAMING_LOAD_BOUNDS_EXT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void STREAMING_UNLOAD_BOUNDS()l
extern _native void STREAMING_DISPLAY_DEBUG_INFO()l
extern _native void STREAMING_EVICT_ALL()l
extern _native int IS_PLAYER_TELEPORTING()l
extern _native void DUMP_MEMORY_STATS()l
extern _native void REPORT_METRICS_PERFORMANCE()l
extern _native void STREAMING_SET_POI_LIMIT(int pram0)l
extern _native void STREAMING_ENABLE_FORCE_FRAGMENT_HIGH_LOD(int pram0)l
extern _native void STREAMING_DISABLE_FORCE_FRAGMENT_HIGH_LOD(int pram0)l
extern _native void STREAMING_ENABLE_FORCE_CHILD_SECTOR_HIGH_LOD()l
extern _native void STREAMING_DISABLE_FORCE_CHILD_SECTOR_HIGH_LOD()l
extern _native void RESIZE_ACTOR_SET()l
#pragma endregion

#pragma region EXPLOSION
extern _native32(0xE7023D23) void _CREATE_EXPLOSION(vector3* coords, const char* explosionName, bool pram2, vector3* damageVector, bool pram4)l
extern _native void ENABLE_REPLICATION_SET_EXPLOSION(int pram0)l
#pragma endregion

#pragma region WEATHER
extern _native void SET_WEATHER(eWeather WeatherType, Time Time)l
extern _native int GET_WEATHER()l
extern _native void SET_WEATHER_COMPLEX(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9, int pram10, int pram11, int pram12, int pram13, int pram14)l
extern _native void SET_WIND(int pram0, int pram1, int pram2)l
extern _native void SET_AUTO_WIND()l
extern _native void SET_RAIN_AMOUNT(float Amount)l
extern _native void SET_LIGHTNING_AMOUNT(float Amount)l
extern _native void SET_WEATHER_TYPE_GOOD(int pram0)l
extern _native void SET_WEATHER_TYPE_RAINY(int pram0)l
extern _native void TRIGGER_LIGHTNING_SEQUENCE()l
extern _native void TRIGGER_CLOUD_LIGHTNING_SEQUENCE()l
extern _native void ENABLE_WEATHER_SPHERE(int pram0, int pram1)l
extern _native int IS_WEATHER_SPHERE_ENABLED(int pram0)l
extern _native32(0x21A68D47) void _SET_SKY_IS_INTRO_RUNNING()l
#pragma endregion

#pragma region RENDER
extern _native void RENDERING_ENABLE_CHARACTER_CLIPPING(int pram0)l
extern _native void PRINT_FRAME_TIME(int pram0)l
#pragma endregion

#pragma region DOOR
extern _native int FIND_NEAREST_DOOR(int pram0, int pram1)l
extern _native int GET_DOOR_FROM_OBJECT(int pram0)l
extern _native int IS_DOOR_VALID(int pram0)l
extern _native int IS_DOOR_LOCKED(int pram0)l
extern _native void SET_DOOR_LOCK(int pram0, int pram1)l
extern _native int IS_DOOR_CLOSED(int pram0)l
extern _native int IS_DOOR_OPENED(int pram0)l
extern _native int IS_DOOR_OPENING(int pram0)l
extern _native int IS_DOOR_CLOSING(int pram0)l
extern _native void SET_DOOR_AUTO_CLOSE(int pram0, int pram1)l
extern _native void SET_DOOR_CURRENT_SPEED(int pram0, int pram1, int pram2)l
extern _native void OPEN_DOOR(int pram0, int pram1, int pram2)l
extern _native void OPEN_DOOR_DIRECTION(int pram0, int pram1)l
extern _native void OPEN_DOOR_FAST(int pram0, int pram1)l
extern _native void OPEN_DOOR_DIRECTION_FAST(int pram0, int pram1)l
extern _native void CLOSE_DOOR(int pram0, int pram1)l
extern _native void CLOSE_DOOR_FAST(int pram0)l
extern _native void SET_ALL_DOOR_LOCKS_VISIBLE(int pram0)l
extern _native void SET_DOOR_LOCK_VISIBLE(int pram0, int pram1)l
extern _native int IS_DOOR_OPEN_IN_DIRECTION(int pram0, int pram1)l
#pragma endregion

#pragma region UI
extern _native void UI_STACKPOP()l
extern _native void UI_STACKPUSH(char* scaleformLayer)l
extern _native void UI_POP(const char* pram0)l
extern _native void UI_PUSH(const char* pram0)l
extern _native void UI_GOTO(int pram0)l
extern _native void UI_EXIT(const char* uiLayer)l
extern _native void UI_SHOW(const char* uiLayer)l
extern _native void UI_REFRESH(const char* pram0)l
extern _native void UI_HIDE(const char* uiLayer)l
extern _native void UI_ENABLE(const char* uiLayer)l
extern _native void UI_DISABLE(const char* uiLayer)l
extern _native bool UI_ISACTIVE(const char* UiLayer)l
extern _native void UI_ACTIVATE(const char* UiLayer)l
extern _native void UI_DEACTIVATE(const char* pram0)l
extern _native void UI_EXCLUDE(const char* uiLayer)l
extern _native void UI_INCLUDE(const char* uiLayer)l
extern _native int UI_ISFOCUSED(const char* UiLayer)l
extern _native void UI_FOCUS(const char* uiLayer)l
extern _native void UI_UNFOCUS(const char* uiLayer)l
extern _native void UI_RESTORE(char* uiLayer)l
extern _native void UI_SUPPRESS(char* uiLayer)l
extern _native void UI_SENDEVENT()l
extern _native int UI_GET_SELECTED_INDEX(const char* MenuID, bool UNK0)l
extern _native void UI_SET_STYLE(int pram0, int pram1)l
extern _native void UI_ADD_CHILD(int pram0, int pram1)l
extern _native void UI_SET_DATA(int pram0, int pram1, int pram2)l
extern _native int UI_GET_NUM_CHILDREN(int pram0)l
extern _native void UI_ANIM_SETUP(int pram0, int pram1, int pram2, int pram3)l
extern _native void UI_ANIM_RESTART(int pram0)l
extern _native int UI_ANIM_GET_TIME(int pram0)l
extern _native void UI_REGISTER_STREAMING_TEXTURE(int pram0, int pram1, int pram2, int pram3)l
extern _native void UI_SET_STRING(char* entry, char* string)l
extern _native void UI_SET_STRING_FORMAT(const char* GXTName, const char* FormatString, const char* String1, const char* String2, const char* String3)l
extern _native void UI_SET_MONEY(int pram0, int pram1, int pram2)l
extern _native const char* UI_GET_STRING(const char* GXTEntry)l
extern _native int UI_GET_STRING_BY_HASH(int pram0)l
extern _native void UI_MESSAGEBOX_SET_DESCRIPTION(int pram0, int pram1)l
extern _native void UI_LABEL_SET_TEXT(int pram0, int pram1)l
extern _native void UI_LABEL_SET_VALUE(int pram0, int pram1)l
extern _native void UI_LABEL_SET_VALUE_B(int pram0, int pram1)l
extern _native void UI_BUTTON_SET_TEXT(int pram0, int pram1)l
extern _native void UI_TEXTURE_SET_NAME(int pram0, int pram1)l
extern _native void UI_TEXTURE_SET_HASH(int pram0, int pram1)l
extern _native void UI_SET_TEXT(char* entry, char* text)l
extern _native void UI_SET_TEXT_HASH(int pram0, int pram1)l
extern _native void UI_SET_PROGRESS_BAR_VAL(int pram0, int pram1)l
extern _native void UI_SET_PROGRESS_BAR_CHANGE(int pram0, int pram1)l
extern _native32(0xD792B93B) void _UI_SET_AUTO_EXIT_TIME(int pram0, int pram1)l
extern _native void UI_SET_ICON(int pram0, int pram1)l
extern _native void UI_DISABLE_INPUT()l
extern _native int UI_IS_FUIMOVIE_LOADED(int pram0)l
extern _native void UI_DISABLE_USE_CONTEXT_PROMPT()l
#pragma endregion

#pragma region UI_STRING_TABLE
extern _native void STRINGTABLE_APPEND_TABLE(int pram0)l
extern _native void REQUEST_STRING_TABLE(const char* StringTablePath)l
extern _native bool HAS_STRING_TABLE_LOADED(const char* StringTablePath)l
extern _native void REMOVE_STRING_TABLE(int pram0)l
#pragma endregion

#pragma region SAG_ACTOR
extern _native void TELEPORT_ACTOR(Actor Actor, vector3* Position, bool Unk1, bool _Unk1, bool __Unk1)l
extern _native void TELEPORT_ACTOR_WITH_HEADING(Actor Actor, vector3 Position, float Heading, bool Unk1, bool _Unk1, bool __Unk1)l
extern _native void GET_MAX_SPEED()l
extern _native void GET_POSITION(int pram0, vector3* outCoords)l
extern _native float GET_HEADING(int pram0)l
extern _native int GET_ACTOR_AXIS(int pram0, int pram1, int pram2)l
extern _native void SET_ACTOR_HEADING(Actor Actor, float Heading, bool Unk1)l
extern _native void SET_ACTOR_ONE_SHOT_DEATH(int pram0, int pram1)l
extern _native int GET_ACTOR_ONE_SHOT_DEATH_STATUS(int pram0)l
extern _native int GET_PHYSINST_FROM_ACTOR(int pram0)l
extern _native int CAN_ACTOR_HOGTIE_TARGET(int pram0, int pram1)l
extern _native int IS_ACTOR_PLAYER(int pram0)l
extern _native bool IS_ACTOR_LOCAL_PLAYER(int actor)l
extern _native int GET_DEBUG_PADINDEX()l
extern _native Controller GET_PLAYER_PADINDEX(int ActorId)l
extern _native int GET_PLAYER_PADINDEX_NO_ACTOR(int pram0)l
extern _native void SET_PLAYER_PADINDEX(int pram0, int pram1)l
extern _native void SET_ENABLE_NAV_STICK_INPUT()l
extern _native void SET_PLAYER_CONTROL(int pram0, int pram1, int pram2, int pram3)l
extern _native void SET_PLAYER_ENABLE_MOUNT_USE_CONTEXTS(int pram0, int pram1)l
extern _native void SET_PLAYER_ALLOW_PICKUP()l
extern _native void SET_PLAYER_MELEE_MODE_SELECTED()l
extern _native void SET_PLAYER_DISABLE_TARGETING(int pram0, int pram1)l
extern _native int IS_PLAYER_CONTROLLABLE(int pram0)l
extern _native int IS_PLAYER_IN_COMBAT(int pram0)l
extern _native int IS_PLAYER_IN_COMBAT_WITHIN(int pram0, int pram1)l
extern _native void SET_RETICLE_DRAW_DISABLED_BY_SCRIPT(int pram0, int pram1)l
extern _native void SET_PLAYER_CONTROL_RUMBLE(int pram0, int pram1)l
extern _native void RESET_RUMBLE()l
extern _native int GET_PLAYER_CONTROL_CONFIG(int pram0)l
extern _native void SET_PLAYER_CONTROL_CONFIG(int pram0, int pram1)l
extern _native void PLAYER_RUMBLE(int pram0, int pram1, int pram2)l
extern _native void SET_PLAYER_CURRENT_NOTORIETY(int pram0, int pram1)l
extern _native void SET_PLAYER_CURRENT_HONOR(int pram0, int pram1)l
extern _native void SET_PLAYER_COMBATMODE(int pram0)l
extern _native int GET_PLAYER_COMBATMODE()l
extern _native void SET_PLAYER_COMBATMODE_OVERRIDE(int pram0)l
extern _native void SET_PLAYER_COMBATMODE_EXCLUSION(int pram0)l
extern _native void SET_PLAYER_VEHICLE_INPUT()l
extern _native void ADD_PLAYER_CONTROL_HORSE_FOLLOW(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void REM_PLAYER_CONTROL_HORSE_FOLLOW(int pram0, int pram1)l
extern _native void CLEAR_PLAYER_CONTROL_HORSE_FOLLOW()l
extern _native void IS_PLAYER_IN_HORSE_FOLLOW()l
extern _native Actor GET_PLAYER_ACTOR(int PlayerId)l
extern _native int IS_LOCAL_PLAYER(int pram0)l
extern _native bool IS_LOCAL_PLAYER_VALID(Actor actorId)l
extern _native int IS_SLOT_VALID(int SlotID)l
extern _native int GET_SLOT_ACTOR(int pram0)l
extern _native int GET_ACTOR_SLOT(int pram0)l
extern _native int GET_LOCAL_SLOT()l
extern _native const char* GET_SLOT_NAME(int SlotID)l
extern _native int GET_SLOT_POSITION(int pram0, int pram1)l
extern _native int GET_SLOT_FACING(int pram0, int pram1)l
extern _native int IS_PLAYER_TARGETTING_ACTOR(int pram0, int pram1, int pram2)l
extern _native void IS_PLAYER_TARGETTING_OBJECT()l
extern _native int IS_PLAYER_DEADEYE(int pram0)l
extern _native void SET_PLAYER_DEADEYE_MODE(int pram0, int pram1)l
extern _native void SET_FORCE_PLAYER_AIM_MODE(int pram0, int pram1)l
extern _native void SET_PLAYER_ENDLESS_READYMODE(int pram0, int pram1)l
extern _native int GET_PLAYER_ZOOM_STATE(int pram0)l
extern _native int IS_PLAYER_USING_COVER(int pram0)l
extern _native void ATTACH_PLAYER_TO_COVER(int pram0, int pram1, int pram2)l
extern _native void SIMULATE_PLAYER_INPUT_GAIT(int pram0, int pram1, int pram2, int pram3)l
extern _native void ACTOR_POP_NEXT_GAIT(int pram0, int pram1, int pram2)l
extern _native void ACTOR_SET_MAX_GAIT()l
extern _native int IS_ACTOR_USING_COVER(int pram0)l
extern _native int IS_ACTOR_USING_LEDGE(int pram0)l
extern _native void SET_PLAYER_DEADEYE_POINTS(int pram0, int pram1, int pram2)l
extern _native void ADD_PLAYER_DEADEYE_POINTS(int pram0, int pram1, int pram2)l
extern _native int GET_PLAYER_DEADEYE_POINTS(int pram0)l
extern _native void SET_DISABLE_DEADEYE(int pram0, int pram1)l
extern _native void SET_DEADEYE_POINT_MODIFIER(int pram0, int pram1)l
extern _native void SET_MAX_DEADEYE_POINTS(int pram0, int pram1)l
extern _native void SET_DEADEYE_MULTILOCK_ENABLE(int pram0, int pram1)l
extern _native void SET_DEADEYE_TARGETPAINT_ENABLE(int pram0, int pram1)l
extern _native void SET_DEADEYE_INVULNERABILITY(int pram0, int pram1)l
extern _native void SET_DEADEYE_DAMAGE_SCALING(int UNK0, float Scaling)l
extern _native void SET_DEADEYE_TIME_LIMIT()l
extern _native void SET_DEADEYE_REGENERATION_RATE(int pram0, int pram1, int pram2)l
extern _native void SET_DEADEYE_REGENERATION_RATE_MULTIPLIER(int pram0, int pram1)l
extern _native void SET_DEADEYE_TIMESCALE(int UNK0, float TimeScale)l
extern _native void SET_INFINITE_DEADEYE(int UNK0, bool Infinite)l
extern _native int GET_TIME_PLAYER_SHOT_CLOSE(int pram0)l
extern _native void SET_WAGON_TO_WAGON_JACK_ENABLE()l
extern _native void SET_PLAYER_POSTURE(int pram0, int pram1, int pram2)l
extern _native void SET_ACTOR_ALLOW_DISMOUNT(int pram0, int pram1)l
extern _native void SET_ACTOR_INVULNERABILITY(Actor Actor, bool Invulnerable)l
extern _native int GET_ACTOR_INVULNERABILITY(int pram0)l
extern _native void SET_TOUGH_ACTOR(int pram0, int pram1)l
extern _native void SET_ACTOR_UNKILLABLE(int pram0, int pram1)l
extern _native void SET_ACTOR_PERMANENT()l
extern _native void SET_ACTOR_PERMANENT_DEAD(int pram0)l
extern _native void SET_ACTOR_FROZEN_AFTER_CORPSIFY()l
extern _native void CLEAR_ACTOR_PROOF(int pram0, int pram1)l
extern _native void CLEAR_ACTOR_PROOF_ALL()l
extern _native int GET_ACTOR_PROOF(int pram0)l
extern _native void SET_ACTOR_PROOF(int pram0, int pram1)l
extern _native void SET_ACTOR_OVERHEALTH_MODE(int pram0, int pram1)l
extern _native void ACTOR_REPAIR_INCAPACITATION(int pram0)l
extern _native int GET_ACTOR_INCAPACITATED(int pram0)l
extern _native void SET_ALLOW_RIDE_BY_AI(int pram0, int pram1)l
extern _native void GET_ALLOW_RIDE_BY_PLAYER()l
extern _native void SET_ALLOW_RIDE_BY_PLAYER(Actor Actor, bool Ride)l
extern _native void SET_ALLOW_RIDE()l
extern _native void GET_ALLOW_RIDE()l
extern _native void SET_ALLOW_JACK(int pram0, int pram1)l
extern _native void SET_ALLOW_EXECUTE(int pram0, int pram1)l
extern _native void SET_ALLOW_DEADEYE_LOCKS(int pram0, int pram1)l
extern _native void SET_DEADEYE_LOCKS_ON_HEAD_ONLY(int pram0, int pram1)l
extern _native void SET_ALLOW_MELEE_SPECIAL_MOVE()l
extern _native void SET_ALLOW_LASSO_MINI_GAME()l
extern _native void ACTOR_DISMOUNT_NOW(int pram0)l
extern _native int IS_ACTOR_REACTING(int pram0)l
extern _native int GET_ACTOR_UPDATE_PRIORITY(int pram0)l
extern _native void SET_ACTOR_UPDATE_PRIORITY(int pram0, int pram1)l
extern _native32(0xA4E29C31) void _SET_ACTOR_FORCE_HIGH_LOD_UPDATE(int pram0, int pram1)l
extern _native void ACTOR_FORCE_NEXT_UPDATE(int pram0)l
extern _native int IS_ANY_ACTOR_IN_SPHERE(int pram0, int pram1)l
extern _native void SET_NPC_TO_NPC_CRIPPLE_DISABLE(int pram0)l
extern _native void SET_NPC_TO_NPC_DAMAGE_SCALE_FACTOR(int pram0)l
extern _native void SET_PLAYER_TO_PLAYER_DAMAGE_SCALE_FACTOR(int pram0)l
extern _native void SET_NPC_TO_ACTOR_DAMAGE_SCALE_FACTOR(int pram0, int pram1)l
extern _native void SET_ACTOR_LOW_DROP_DAMAGE()l
extern _native void SET_ACTOR_MEDIUM_DROP_DAMAGE()l
extern _native void SET_ACTOR_HIGH_DROP_DAMAGE()l
extern _native void SET_ACTOR_DEATH_DROP_DISTANCE(int pram0, int pram1)l
extern _native void SET_DAMAGE_SCALE_ENABLE(int pram0, int pram1)l
extern _native void SET_CRIPPLE_ENABLE(int pram0, int pram1)l
extern _native void SET_CRIPPLE_FLAG(int pram0, int pram1)l
extern _native int IS_ACTOR_CRIPPLED(int pram0, int pram1)l
extern _native int IS_ACTOR_HANDSUP(int pram0)l
extern _native void SET_ALLOW_COLD_WEATHER_BREATH(int pram0, int pram1)l
extern _native void SET_DLC_FALLBACK_AVATAR(int pram0)l
extern _native void SET_EMOTION(int pram0, int pram1, int pram2)l
extern _native void SET_ACTOR_STOP_UPDATE()l
extern _native void GET_ACTOR_STOP_UPDATE()l
extern _native int IS_ACTOR_IN_ROOM(int pram0)l
extern _native void REGISTER_TRAFFIC_OBJECTSET(int pram0)l
extern _native void REGISTER_TRAFFIC_ACTOR(int pram0, int pram1)l
extern _native void REGISTER_GPS_CURVE_OBJECTSET(int pram0)l
extern _native void SET_PLAYER_TARGET_WEIGHT()l
extern _native void RESET_PLAYER_TARGET_WEIGHT()l
extern _native void SET_HARDLOCK_TARGET_ANGLE_WEIGHTING()l
extern _native void SET_ZOMBIE_TARGET_MODE(int pram0)l
extern _native void SET_ACTOR_SKIP_VISIBILITY_CHECK(int pram0, int pram1)l
extern _native void GET_ACTOR_SKIP_VISIBILITY_CHECK()l
extern _native void FEED_CODE_WARP_DIST(int pram0)l
extern _native void SET_ACTOR_STREAMING_HIGH_PRIORITY(int pram0, int pram1)l
#pragma endregion

#pragma region MOTIVE_MANAGER
extern _native void SET_MOTIVE_BY_ENUM(int pram0, int pram1, int pram2)l
#pragma endregion

#pragma region HEALTH
extern _native int GET_LAST_ATTACKER(int pram0)l
extern _native int GET_LAST_HIT_TIME(int pram0)l
extern _native int GET_LAST_HIT_WEAPON(int pram0)l
extern _native int GET_LAST_HIT_FLAGS(int pram0)l
extern _native int GET_LAST_DAMAGE(int pram0)l
extern _native bool GET_LAST_HIT_ZONE(Actor Actor, int* out)l
extern _native void GET_CORPSE_LAST_HIT_WEAPON()l
extern _native void GET_CORPSE_LAST_HIT_ZONE()l
extern _native void CLEAR_LAST_HIT(int pram0)l
extern _native void KILL_ACTOR(int pram0)l
extern _native void KILL_ACTOR_WITH_KILLER(int pram0, int pram1)l
extern _native int IS_ACTOR_ALIVE(int pram0)l
extern _native bool IS_ACTOR_DEAD(Actor actor)l
extern _native int IS_ACTOR_RAGDOLL(int pram0)l
extern _native void SET_ACTOR_HEALTH(int pram0, int pram1)l
extern _native int GET_ACTOR_HEALTH(int pram0)l
extern _native int GET_ACTOR_MAX_HEALTH(int pram0)l
extern _native void SET_ACTOR_MAX_HEALTH(int pram0, int pram1)l
extern _native32(0x7A207FFE) int _ACTOR_HAS_KO_POINTS(int pram0)l
extern _native void SET_ACTOR_KO_POINTS(int pram0, int pram1)l
extern _native void GET_ACTOR_KO_POINTS()l
extern _native int GET_ACTOR_MAX_KO_POINTS(int pram0)l
extern _native void SET_ACTOR_KNOCKOUTTIME(int pram0, int pram1)l
extern _native void SET_ACTOR_KNOCKOUTTIME_DEFAULT()l
extern _native int IS_ACTOR_DRUNK(int pram0)l
extern _native void SET_ACTOR_DRUNK(int pram0, int pram1)l
extern _native void SET_ACTOR_PASSED_OUT(int pram0, int pram1)l
extern _native void SET_ACTOR_HANGING_FROM_NOOSE(int pram0, int pram1)l
extern _native32(0x6287203C) void _SET_HIT_INFO_DDA_LEVEL(int pram0)l
extern _native32(0x1082715D) void _RESET_HIT_INFO_DDA_LEVEL()l
#pragma endregion

#pragma region FACTION
extern _native void RELOAD_FACTIONS(int pram0)l
extern _native void RESET_FACTIONS()l
extern _native int GET_ACTOR_FACTION(int pram0)l
extern _native void SET_ACTOR_FACTION(int pram0, int pram1)l
extern _native int IS_FACTION_VALID(int pram0)l
extern _native void VERIFY_FACTION_ENUM(int pram0, int pram1)l
extern _native void SET_FACTION_IS_LAWFUL_TO_ATTACK(int pram0, int pram1)l
extern _native void GET_FACTION_IS_LAWFUL_TO_ATTACK()l
extern _native void SET_FACTIONS_STATUS_ONE_WAY(int pram0, int pram1, int pram2)l
extern _native void SET_FACTIONS_STATUS_TWO_WAY(int pram0, int pram1, int pram2)l
extern _native void SET_AMBIENT_FACTIONS_STATUS_TWO_WAY(int pram0, int pram1, int pram2)l
extern _native void RESET_FACTIONS_STATUS_TWO_WAY(int pram0, int pram1)l
extern _native void RESET_AMBIENT_FACTIONS_STATUS_TWO_WAY(int pram0, int pram1)l
extern _native void GET_FACTIONS_STATUS()l
extern _native void SET_FACTION_TO_FACTION_ACCURACY_SCALE_FACTOR(int pram0, int pram1, int pram2)l
extern _native void SET_FACTION_TO_FACTION_DAMAGE_SCALE_FACTOR(int pram0, int pram1, int pram2)l
extern _native void CLEAR_FACTION_STATUS_TO_INDIVIDUAL_ACTOR(int pram0, int pram1)l
extern _native int GET_FACTION_STATUS_TO_INDIVIDUAL_ACTOR(int pram0, int pram1)l
extern _native void SET_FACTION_STATUS_TO_INDIVIDUAL_ACTOR(int pram0, int pram1, int pram2)l
#pragma endregion

#pragma region ACTOR_SET
extern _native int CREATE_ACTORSET_IN_LAYOUT(int pram0, int pram1, int pram2)l
extern _native int IS_ACTORSET_VALID(int pram0)l
extern _native void FIND_NAMED_ACTORSET()l
extern _native int DESTROY_ACTORSET(int pram0)l
extern _native int DISBAND_ACTORSET(int pram0)l
extern _native int ADD_ACTORSET_MEMBER(int pram0, int pram1)l
extern _native int REMOVE_ACTORSET_MEMBER(int pram0, int pram1)l
extern _native int IS_ACTOR_IN_ACTORSET(int pram0, int pram1)l
extern _native int GET_ACTOR_FROM_ACTORSET(int pram0, int pram1)l
extern _native int GET_ACTORSET_SIZE(int pram0)l
#pragma endregion

#pragma region AI_CONVERSE
extern _native void AI_CONVERSE_SET_GREETING_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_GOSSIP_AMBIENT_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_GOSSIP_REPLY_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_GOODBYE_START_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_GOODBYE_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_INIT_CAMPFIRE_CONTEXT_STORAGE()l
extern _native void AI_CONVERSE_SET_CAMPFIRE_INVITE_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_CAMPFIRE_CONTEXT(int pram0, int pram1)l
extern _native void AI_CONVERSE_SET_CAMPFIRE_STORY_CONTEXT(int pram0, int pram1)l
extern _native void AI_CONVERSE_SET_CAMPFIRE_STORY_DONE_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_CAMPFIRE_STORY_LEAVE_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_CAMPFIRE_RESPONSE_CONTEXT(int pram0)l
extern _native void AI_SET_CAMPFIRE_STORY_ENABLED(int pram0)l
extern _native void AI_CONVERSE_SET_GIDDYUP_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_WOAH_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_DISABLE(int pram0)l
extern _native void AI_CONVERSE_ENABLE(int pram0)l
extern _native void AI_CONVERSE_ADD_CAMPFIRE_CONVERSER(int pram0)l
extern _native void AI_CONVERSE_REMOVE_CAMPFIRE_CONVERSER(int pram0)l
extern _native void AI_CONVERSE_SET_GREET_SAUCY_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_SOLICIT_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_REJECTION_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_COY_REJECTION_CONTEXT(int pram0)l
extern _native void AI_CONVERSE_SET_GREET_PLAYER_CONTEXT(int pram0)l
#pragma endregion

#pragma region AI_ATTENTION
extern _native int ADD_FIXED_ATTENTION_TARGET(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
#pragma endregion

#pragma region WEAPON_REASONER
extern _native int ACTOR_HOLSTER_WEAPON(int pram0, int pram1)l
extern _native int ACTOR_START_FORCE_HOLSTER(int pram0, int pram1, int pram2)l
extern _native int ACTOR_END_FORCE_HOLSTER(int pram0)l
extern _native int ACTOR_DRAW_LAST_WEAPON(int pram0, int pram1)l
extern _native int ACTOR_DRAW_ANY_WEAPON(int pram0, int pram1)l
extern _native void ACTOR_DRAW_WEAPON(int pram0, int pram1, int pram2)l
extern _native int ACTOR_IS_HOLSTERED(int pram0)l
extern _native void SET_WEAPON_SELECTION_ENABLED(int pram0, int pram1)l
extern _native void SET_WEAPON_SELECTION_AI_MIN_RANGE_ACTOR(int pram0, int pram1, int pram2)l
extern _native void SET_WEAPON_SELECTION_AI_MIN_RANGE_DEFAULT(int pram0, int pram1)l
extern _native void SET_WEAPON_SELECTION_AI_MAX_RANGE_ACTOR()l
extern _native void SET_WEAPON_SELECTION_AI_BASE_PRIORITY_DEFAULT(int pram0, int pram1)l
extern _native void SET_WEAPON_SELECTION_AI_CAN_USE_INDOORS_DEFAULT(int pram0, int pram1)l
#pragma endregion

#pragma region AI_SPEECH_CONTEXT
extern _native void SPEECH_CONTEXT_INIT_DATA(int pram0)l
extern _native void SPEECH_CONTEXT_ADD_CHILD(int pram0, int pram1, int pram2)l
extern _native void SPEECH_CONTEXT_SET_TIME_RESTRICTION(int pram0, int pram1, int pram2)l
extern _native void SPEECH_CONTEXT_SET_OPPOSITE_GENDER_RESTRICTION(int pram0)l
extern _native void SPEECH_CONTEXT_SET_ETHNICITY_RESTRICTION(int pram0, int pram1)l
extern _native void SPEECH_CONTEXT_SET_RESTRICTION_IS_LAW(int pram0)l
extern _native void SPEECH_CONTEXT_SET_WEATHER_RESTRICTION_GOOD(int pram0)l
extern _native void SPEECH_CONTEXT_SET_WEATHER_RESTRICTION_RAINY(int pram0)l
extern _native void SPEECH_CONTEXT_SET_TARGET_PLAYER(int pram0)l
extern _native void SPEECH_CONTEXT_SET_TARGET_INITIAL_FACTION(int pram0, int pram1)l
extern _native void SPEECH_CONTEXT_SET_PLAYER_IDENTITY_RESTRICTION(int pram0, int pram1)l
extern _native void SPEECH_CONTEXT_SET_ALLOW_PHRASE_REUSE(int pram0, int pram1)l
extern _native void DISABLE_NONCOMBAT_SPEECH_UNIVERSAL(int pram0)l
extern _native void DISABLE_NONCOMBAT_SPEECH_INDIVIDUAL(int pram0, int pram1, int pram2)l
#pragma endregion

#pragma region AI_TASKS
extern _native void TASK_ACTION_PERFORM(int pram0, int pram1)l
extern _native void TASK_ACTION_PERFORM_AT_POSITION()l
extern _native void TASK_ACTION_PERFORM_ON_TARGET()l
extern _native void TASK_ANIMAL_PATROL(int pram0, int pram1)l
extern _native void TASK_ANIMAL_CIRCLE_AGGRESSIVELY(int pram0, int pram1)l
extern _native void TASK_ANIMAL_FOLLOW_AGGRESSIVELY()l
extern _native void TASK_ANIMAL_HUNT()l
extern _native void TASK_BE_DEAD()l
extern _native void TASK_BE_DEAD_RANDOM()l
extern _native void TASK_BIRD_FLY_NEAR_COORD()l
extern _native void TASK_BIRD_LAND()l
extern _native void TASK_BIRD_LAND_AT_COORD(int pram0, int pram1)l
extern _native void TASK_BIRD_SOAR()l
extern _native void TASK_BIRD_SOAR_AT_COORD(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_CROUCH(int pram0, int pram1)l
extern _native void TASK_DISMOUNT(int pram0, int pram1)l
extern _native void TASK_DIVE(int pram0, int pram1)l
extern _native void TASK_DIVEAWAYFROM()l
extern _native void TASK_DIVETOWARD()l
extern _native void TASK_DOOR_ACTION()l
extern _native void TASK_DRAW_HOLSTER_WEAPON(int pram0, int pram1)l
extern _native void TASK_FACE_ACTOR(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_FACE_COORD(int pram0, int pram1, int pram2)l
extern _native void TASK_FLEE_COORD(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void TASK_FLEE_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void TASK_FLEE_ACTORSET(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void TASK_FOLLOW_ACTOR(int pram0, int pram1)l
extern _native void TASK_FOLLOW_AND_ATTACK_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native void TASK_FOLLOW_AND_ATTACK_OBJECT_ALONG_PATH()l
extern _native void TASK_FOLLOW_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8, int pram9)l
extern _native void TASK_FOLLOW_OBJECT_ALONG_PATH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void TASK_FOLLOW_OBJECT_AT_DISTANCE(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_FOLLOW_OBJECT_IN_FORMATION()l
extern _native void TASK_FOLLOW_PATH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void TASK_FOLLOW_PATH_FROM_NEAREST_POINT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void TASK_FOLLOW_PATH_FROM_POINT()l
extern _native void TASK_GO_NEAR_ACTORSET(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_GO_NEAR_COORD(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_GO_NEAR_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void TASK_GO_TO_COORD(int pram0, int pram1, int pram2)l
extern _native void TASK_GO_TO_COORD_AND_STAY(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_GO_TO_COORD_NONSTOP(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_GO_TO_COORD_PRECISELY(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void TASK_GO_TO_COORD_USING_MATERIAL()l
extern _native void TASK_GO_TO_OBJECT(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void TASK_GUARD_PATROL_AUTO()l
extern _native void TASK_GUARD_PATROL_PATH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void TASK_GUARD_STAND(int pram0, int pram1, int pram2)l
extern _native void TASK_HIDE_AT_COVER(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void TASK_HORSE_ACTION(int pram0, int pram1)l
extern _native void TASK_JUMP_OVER_OBSTRUCTION()l
extern _native void TASK_JUMP_TO_OBJECT()l
extern _native void TASK_KILL_CHAR(int pram0, int pram1)l
extern _native void TASK_LEDGE_ACTION()l
extern _native void TASK_MELEE_ATTACK(int pram0, int pram1, int pram2)l
extern _native void TASK_MOUNT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void TASK_PLAY_ANIM()l
extern _native void TASK_POINT_GUN_AT_COORD(int pram0, int pram1, int pram2)l
extern _native void TASK_POINT_GUN_AT_OBJECT(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_RESPOND_TO_HORSE_WHISTLE(int pram0, int pram1)l
extern _native void TASK_SEARCH(int pram0, int pram1, int pram2)l
extern _native void TASK_SEARCH_FOR_OBJECT(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_SEEK_COVER_FROM_ACTOR(int pram0, int pram1)l
extern _native void TASK_SEEK_COVER_FROM_COORD()l
extern _native void TASK_SEQUENCE_PERFORM(int pram0, int pram1)l
extern _native void TASK_SEQUENCE_PERFORM_REPEATEDLY(int pram0, int pram1, int pram2)l
extern _native void TASK_SHOOT_AT_COORD(int pram0, int pram1, int pram2)l
extern _native void TASK_SHOOT_AT_COORD_FROM_POSITION(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_SHOOT_ENEMIES_FROM_ANY_COVER(int pram0, int pram1)l
extern _native void TASK_SHOOT_ENEMIES_FROM_COVER(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_SHOOT_ENEMIES_FROM_POSITION(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_SHOOT_ENEMIES_FROM_PREFERRED_COVER()l
extern _native void TASK_SHOOT_FROM_COVER(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void TASK_SHOOT_FROM_POSITION(int pram0, int pram1, int pram2)l
extern _native void TASK_SIMPLE_BEHAVIOR()l
extern _native void TASK_STAND_STILL(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_STEALTH_ATTACK()l
extern _native void TASK_SURROUND_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void TASK_TAUNT_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void TASK_TAUNT_ACTOR_IN_PLACE(int pram0, int pram1, int pram2)l
extern _native void TASK_TR_ACTION()l
extern _native void TASK_TR_ACTION_ON_ACTOR()l
extern _native void TASK_USE_GRINGO(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void TASK_USE_GRINGO_GROUP()l
extern _native void TASK_USE_LASSO(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void TASK_USE_TURRET(int pram0, int pram1, int pram2)l
extern _native void TASK_USE_TURRET_AGAINST_COORD(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_USE_TURRET_AGAINST_OBJECT(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_VEHICLE_ENTER(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_VEHICLE_ENTER_SPECIFIC_LOCATION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void TASK_VEHICLE_LEAVE(int pram0)l
extern _native void TASK_WANDER(int pram0, int pram1)l
extern _native void TASK_WANDER_IN_BOX(int pram0, int pram1, int pram2, int pram3)l
extern _native void TASK_WANDER_IN_VOLUME(int pram0, int pram1, int pram2)l
extern _native void TASK_WARN_CHAR()l
extern _native void TASK_GO_NEAR_ACTOR(int pram0, int pram1, int pram2, int pram3)l
extern _native int GET_TASK_STATUS(int pram0, int pram1)l
extern _native int IS_TASKED_TO_USE_GRINGO_FOREVER(int pram0, int pram1)l
extern _native int GET_NTH_TASK_STATUS(int pram0, int pram1)l
extern _native int GET_TASK_SEQUENCE_CURRENT_TASK_INDEX(int pram0, int pram1)l
extern _native int GET_TASK_NEXT_POINT_ON_PATH(int pram0, int pram1)l
extern _native void TASK_CLEAR(int pram0)l
extern _native void TASK_OVERRIDE_CLEAR_MOVETYPE(int pram0)l
extern _native void TASK_OVERRIDE_CLEAR_POSTURE(int pram0)l
extern _native void TASK_OVERRIDE_SET_MOVETYPE(int pram0, int pram1)l
extern _native void TASK_OVERRIDE_SET_MOVESPEED_ABSOLUTE(int pram0, int pram1)l
extern _native void TASK_OVERRIDE_SET_MOVESPEED_NORMALIZED(int pram0, int pram1)l
extern _native void TASK_OVERRIDE_SET_POSTURE(int pram0, int pram1)l
extern _native void TASK_PRIORITY_SET(int pram0, int pram1)l
extern _native int TASK_SEQUENCE_OPEN()l
extern _native void TASK_SEQUENCE_CLOSE()l
extern _native void TASK_SEQUENCE_RELEASE(int pram0, int pram1)l
extern _native void SET_ACTOR_FACE_STYLE(int pram0, int pram1)l
extern _native void AI_SET_ENABLE_STICKUP_OVERRIDE(int pram0, int pram1)l
extern _native void AI_SET_ENABLE_HORSE_CHARGE_REACTIONS(int pram0, int pram1)l
extern _native void AI_SET_ENABLE_DEAD_BODY_REACTIONS(int pram0, int pram1)l
extern _native void TASK_FAILURE_MODE_SET(int pram0, int pram1)l
#pragma endregion

#pragma region AI_SQUAD
extern _native int SQUAD_GET(int pram0)l
extern _native int SQUAD_IS_VALID(int pram0)l
extern _native void SQUAD_JOIN(int pram0, int pram1)l
extern _native void SQUAD_LEAVE(int pram0)l
extern _native void SQUAD_MAKE_EMPTY(int pram0)l
extern _native void SQUADS_MERGE(int pram0, int pram1)l
extern _native int SQUAD_GET_ACTOR_BY_INDEX(int pram0, int pram1)l
extern _native int SQUAD_GET_SIZE(int pram0)l
extern _native int SQUAD_COMPUTE_CENTROID(int pram0, int pram1)l
extern _native void SQUAD_SET_FACTION(int pram0, int pram1)l
extern _native int SQUAD_GOAL_ADD_BATTLE_ALLIES(int pram0, int pram1, int pram2, int pram3)l
extern _native int SQUAD_GOAL_ADD_BATTLE_DEFEND_VOLUME(int pram0, int pram1, int pram2, int pram3)l
extern _native int SQUAD_GOAL_ADD_COMBAT(int pram0, int pram1, int pram2, int pram3)l
extern _native int SQUAD_GOAL_ADD_FACTION_STATUS_WITHIN_GOAL(int pram0, int pram1, int pram2)l
extern _native void SQUAD_GOAL_ADD_FLOCK()l
extern _native void SQUAD_GOAL_ADD_FOLLOW_OBJECT_IN_FORMATION()l
extern _native void SQUAD_GOAL_ADD_FOLLOW_PATH_IN_FORMATION()l
extern _native int SQUAD_GOAL_ADD_FOLLOW_TRAFFIC_CURVE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native int SQUAD_GOAL_ADD_GENERAL_TASK(int pram0, int pram1, int pram2, int pram3)l
extern _native void SQUAD_GOAL_ADD_GUARD_POSITION()l
extern _native void SQUAD_GOAL_ADD_HUNT_ENEMIES()l
extern _native int SQUAD_GOAL_ADD_MEET_AT_POSITION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native int SQUAD_GOAL_ADD_RESTRICT_INVESTIGATION_DISTANCE_FROM_SQUAD(int pram0, int pram1, int pram2)l
extern _native int SQUAD_GOAL_ADD_SHARE_PERCEPTION(int pram0, int pram1)l
extern _native int SQUAD_GOAL_ADD_STAY_OUTSIDE_OF_VOLUME(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int SQUAD_GOAL_ADD_STAY_WITHIN_VOLUME(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int SQUAD_GOAL_ADD_ANIMAL_HERD(int pram0, int pram1, int pram2, int pram3)l
extern _native int SQUAD_GOAL_GET_STATUS(int pram0, int pram1)l
extern _native int SQUAD_GOAL_IS_VALID(int pram0)l
extern _native void SQUAD_GOAL_REMOVE()l
extern _native void SQUAD_GOALS_CLEAR(int pram0)l
extern _native int SQUAD_IS_GOAL_CONTROLLING_ACTOR(int pram0, int pram1, int pram2)l
extern _native int SQUAD_GET_SINGLE_ACTOR_CONTROLLED_BY_GOAL(int pram0, int pram1)l
extern _native int SQUAD_GOAL_LINK_MODIFIER_TO_OTHER_GOAL(int pram0, int pram1, int pram2)l
extern _native void SQUAD_FLOCK_SET_EXTERNAL_MOVEMENT_TUNING(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SQUAD_FLOCK_SET_FLOCKING_PARAMETER()l
extern _native void SQUAD_FLOCK_SET_BOOL_FLOCKING_PARAMETER()l
extern _native void SQUAD_FLOCK_ADD_EXTERNAL_ALERT(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void SQUAD_FLOCK_ADD_EXTERNAL_ATTRACTION_PATH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void SQUAD_FLOCK_ADD_EXTERNAL_REPULSION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void SQUAD_FLOCK_ADD_EXTERNAL_VELOCITY_MATCH(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void SQUAD_FLOCK_PLAYER_PROXIMITY_BOOST_SET_ENABLED(int pram0, int pram1, int pram2)l
extern _native void SQUAD_FLOCK_PLAYER_WHISTLE_BOOST_SET_ENABLED(int pram0, int pram1, int pram2)l
extern _native void SQUAD_FLOCK_EVENT_BOOST_SET_ENABLED()l
extern _native void SQUAD_ANIMAL_HERD_SET_FLOCKING_PARAMETER(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SQUAD_ANIMAL_HERD_CLEAR_EXTERNAL_INFLUENCES(int pram0, int pram1)l
extern _native void SQUAD_ANIMAL_HERD_CLEAR_EXTERNAL_INFLUENCES_FROM_OBJECT()l
extern _native void SQUAD_ANIMAL_HERD_ADD_EXTERNAL_ALERT()l
extern _native void SQUAD_ANIMAL_HERD_ADD_EXTERNAL_REPULSION(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7)l
extern _native void SQUAD_FLOCK_SET_ALLOW_STRAGGLERS()l
extern _native void SQUAD_HUNT_ENEMIES_SET_DEFAULT_BLIP_ALERTED(int pram0)l
extern _native void SQUAD_HUNT_ENEMIES_SET_DEFAULT_BLIP_INVESTIGATING(int pram0)l
extern _native void SQUAD_HUNT_ENEMIES_SET_DEFAULT_BLIP_UNALERTED(int pram0, int pram1)l
extern _native void SQUAD_GET_NUM_ELIMINATED_ACTORS()l
extern _native int SQUAD_GET_NUM_NOT_ELIMINATED_ACTORS(int pram0)l
extern _native void SQUAD_SET_NOT_ELIMINATED_IMPAIRMENT_MASK(int pram0, int pram1)l
extern _native int SQUAD_GET_NOT_ELIMINATED_IMPAIRMENT_MASK(int pram0)l
extern _native void SQUAD_BATTLE_ALLIES_SET_OBJECTIVE()l
extern _native void SQUAD_BATTLE_ALLIES_SET_FORMATION_DENSITY(int pram0, int pram1, int pram2)l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_GET_ALL_BEHAVIOR_FLAGS()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_ALL_BEHAVIOR_FLAGS()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_GET_BEHAVIOR_FLAG()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_BEHAVIOR_FLAG(int pram0, int pram1, int pram2, int pram3)l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_CURVE()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_ENQUEUE_CURVE(int pram0, int pram1, int pram2, int pram3)l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_SPEED()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_SPEED_ABSOLUTE()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_SPEED_NORMALIZED()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_TASK_PRIORITY(int pram0, int pram1, int pram2)l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_OFFSET_X()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_ALLOW_PLAYER_JOIN()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_SET_DESIRED_LEADER()l
extern _native void SQUAD_FOLLOW_TRAFFIC_CURVE_CLEAR_DESIRED_LEADER()l
extern _native int SQUAD_FOLLOW_TRAFFIC_CURVE_IS_CURVE_ALREADY_IN_LIST(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_GET_ALL_BEHAVIOR_FLAGS()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_ALL_BEHAVIOR_FLAGS()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_GET_BEHAVIOR_FLAG()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_BEHAVIOR_FLAG()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_PATH()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_SPEED()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_SPEED_ABSOLUTE()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_SPEED_NORMALIZED()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_TASK_PRIORITY()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_OFFSET_X()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_ALLOW_PLAYER_JOIN()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_NONSTOP()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_SET_DESIRED_LEADER()l
extern _native void SQUAD_FOLLOW_PATH_IN_FORMATION_CLEAR_DESIRED_LEADER()l
#pragma endregion

#pragma region AI_WORLD
extern _native void ENABLE_AMBIENT_SPAWNING(int pram0)l
extern _native void SET_AMBIENT_FORCE_WAIT_STATE(int pram0)l
extern _native void SET_POP_DENSITY_MULTIPLIER(int pram0)l
extern _native void DESTROY_AMBIENT_ACTORS_IF_POSSIBLE()l
extern _native int FORCE_AMBIENT_NUM_ACTORS_REQUESTED_FOR_MISSIONS()l
extern _native void SET_AMBIENT_NUM_ACTORS_REQUESTED_FOR_MISSIONS(int pram0)l
extern _native int GET_AMBIENT_NUM_ACTORS_GRANTED_FOR_MISSIONS()l
extern _native int GET_AMBIENT_MAX_NUM_TOTAL_ACTORS()l
extern _native void SET_AMBIENT_MAX_NUM_TOTAL_ACTORS(int pram0)l
extern _native void SET_AMBIENT_ACTOR_SPEED_SCALE_RANGE(int pram0, int pram1)l
extern _native void SET_AMBIENT_TUNING_MAX_VISIBLE_RANGE(int pram0)l
extern _native void CLEAR_AMBIENT_ALL_RESTRICTIONS()l
extern _native void CLEAR_AMBIENT_MOVE_RESTRICTIONS()l
extern _native void CLEAR_AMBIENT_SPAWN_RESTRICTIONS()l
extern _native void ADD_AMBIENT_SPAWN_TYPE_RESTRICTION(int pram0)l
extern _native void CLEAR_AMBIENT_SPAWN_TYPE_RESTRICTION(int pram0)l
extern _native void CLEAR_ALL_AMBIENT_SPAWN_TYPE_RESTRICTIONS()l
extern _native void SET_AMBIENT_STREAMING_REQUIRED_POP_ACTOR_SCALE(int pram0)l
extern _native void SET_AMBIENT_ANIMALS_AGRESSIVENESS()l
extern _native void ADD_AI_MOVE_RESTRICTION_STAY_OUTSIDE_OF_VOLUME_SET(int pram0, int pram1)l
extern _native void REMOVE_AI_MOVE_RESTRICTION_STAY_OUTSIDE_OF_VOLUME_SET(int pram0, int pram1)l
extern _native void ADD_AMBIENT_MOVE_RESTRICTION_STAY_OUTSIDE_OF_VOLUME(int pram0)l
extern _native void ADD_AMBIENT_SPAWN_RESTRICTION_STAY_OUTSIDE_OF_VOLUME(int pram0)l
extern _native void REMOVE_AMBIENT_MOVE_RESTRICTION_STAY_OUTSIDE_OF_VOLUME(int pram0)l
extern _native void REMOVE_AMBIENT_SPAWN_RESTRICTION_STAY_OUTSIDE_OF_VOLUME(int pram0)l
extern _native int DOES_AMBIENT_MOVE_RESTRICTION_VOLUME_EXIST(int pram0)l
extern _native int DOES_AMBIENT_SPAWN_RESTRICTION_VOLUME_EXIST(int pram0)l
extern _native void RELEASE_ACTOR_AS_AMBIENT(int pram0)l
extern _native int WOULD_ACTOR_BE_VISIBLE(int pram0, int pram1, int pram2)l
extern _native void DEBUG_GET_AMBIENT_BANK_DISABLE_SPAWN_NEW_ACTORS()l
extern _native int ACTIVATE_EMERGENCY_TELEPORT_FOR_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native int DEACTIVATE_EMERGENCY_TELEPORT_FOR_ACTOR(int pram0)l
extern _native int IS_POINT_IN_AMBIENT_MOVE_RESTRICTION_VOLUME(int pram0)l
extern _native void SET_ACTOR_OBEY_AMBIENT_MOVE_RESTRICTIONS(int pram0, int pram1)l
extern _native int DOES_ACTOR_OBEY_AMBIENT_MOVE_RESTRICTIONS(int pram0)l
extern _native void SET_AMBIENT_DEAD_COUNT_DECAY_RATE(int pram0)l
#pragma endregion

#pragma region AI_VISION
extern _native int CAN_ANYONE_OF_FACTION_SEE_OBJECT(int pram0, int pram1)l
#pragma endregion

#pragma region AI_SPEECH
extern _native int AI_SPEECH_ADD_PHRASE(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void AI_SPEECH_ADD_TAG_FOR_PHRASE(int pram0, int pram1, int pram2)l
extern _native void AI_SPEECH_REGISTER_EVENT(int pram0, int pram1)l
extern _native void AI_SPEECH_REGISTER_TAG(int pram0, int pram1, int pram2, int pram3)l
extern _native void AI_SPEECH_REGISTER_TAGS_BEGIN(int pram0)l
extern _native void AI_SPEECH_REGISTER_TAGS_END()l
extern _native int AI_SPEECH_GET_ALLOW_FOR_ACTOR(int pram0)l
extern _native void AI_SPEECH_SET_ALLOW_CONTEXT_FOR_ACTOR(int pram0, int pram1, int pram2)l
extern _native void AI_SPEECH_SET_ALLOW_CONTEXT_GLOBAL(int pram0, int pram1)l
extern _native void AI_SPEECH_SET_ALLOW_FOR_ACTOR(int pram0, int pram1)l
extern _native void AI_SPEECH_SET_DEFAULT_PACKAGE(int pram0)l
extern _native int AI_GET_ACTOR_CONVERSATION_TARGET(int pram0)l
extern _native int AI_IS_ACTOR_SOCIALIZING(int pram0)l
#pragma endregion

#pragma region AI_RIDE
extern _native void AI_RIDING_SET_ATTRIBUTE()l
extern _native void AI_RIDING_SET_ENABLED()l
#pragma endregion

#pragma region AI_PERCEPTION
extern _native void AI_PERCEPTION_SET_HEARING_GLOBAL_RANGE_FACTOR(int pram0)l
extern _native void AI_PERCEPTION_SET_VISUAL_ID_DISTANCE(int pram0, int pram1)l
extern _native void AI_PERCEPTION_SET_VISUAL_ID_TIME(int pram0, int pram1)l
extern _native void AI_DISABLE_PERCEPTION(int pram0)l
extern _native void AI_ENABLE_PERCEPTION(int pram0)l
#pragma endregion

#pragma region AI_NAVMESH_MOVABLE
extern _native int STREAMING_IS_MOVABLE_NAV_MESH_RESIDENT(int pram0)l
extern _native void STREAMING_REQUEST_MOVABLE_NAV_MESH(int pram0)l
extern _native void STREAMING_UNREQUEST_MOVABLE_NAV_MESH(int pram0)l
extern _native void SET_ACTOR_MOVABLE_NAV_MESH()l
#pragma endregion

#pragma region AI_NAV
extern _native void AI_CLEAR_NAV_MATERIAL_USAGE(int pram0)l
extern _native void AI_RESET_NAV_ACTOR_WIDTH()l
extern _native void AI_SET_NAV_ACTOR_WIDTH(int pram0, int pram1)l
extern _native void AI_SET_NAV_HAZARD_AVOIDANCE_ENABLED(int pram0, int pram1, int pram2)l
extern _native void AI_SET_NAV_MATERIAL_USAGE(int pram0, int pram1, int pram2)l
extern _native void AI_SET_NAV_PATHFINDING_ENABLED(int pram0, int pram1)l
extern _native void AI_SET_NAV_PATHFINDING_ENABLED_WHEN_DRIVING(int pram0, int pram1)l
extern _native void AI_SET_NAV_MAX_SLOPE()l
extern _native void AI_SET_NAV_MAX_WATER_DEPTH_LEVEL(int pram0, int pram1)l
extern _native void AI_SET_NAV_ACTOR_AVOIDANCE_MODE(int pram0, int pram1)l
extern _native void AI_SET_NAV_ACTOR_AVOIDANCE_ALLOW_TURNS(int pram0, int pram1)l
extern _native void AI_GET_NAV_ACTOR_AVOIDANCE_ALLOW_TURNS()l
extern _native void AI_RESET_NAV_SUBGRID_CELL_SIZE(int pram0)l
extern _native void AI_SET_NAV_SUBGRID_CELL_SIZE(int pram0, int pram1)l
extern _native void AI_SET_NAV_FAILSAFE_MOVEMENT_ENABLED(int pram0, int pram1)l
extern _native int AI_GET_NAV_FAILSAFE_MOVEMENT_ENABLED(int pram0)l
extern _native void AI_SET_NAV_UNALERTED_PREFER_PEDPATH()l
extern _native void AI_SET_NAV_ALLOW_TWEAK_DESIRED_MOVEMENT()l
extern _native void AI_GET_NAV_ALLOW_TWEAK_DESIRED_MOVEMENT()l
extern _native void AI_SWAP_NAV_DATA(int pram0)l
#pragma endregion

#pragma region AI_MISC
extern _native void AI_BEHAVIOR_SET_ALLOW(int pram0, int pram1, int pram2)l
extern _native void AI_ACTION_IS_ACTIVE()l
extern _native void AI_GOAL_AIM_AT_COORD()l
extern _native void AI_GOAL_AIM_AT_OBJECT(int pram0, int pram1, int pram2)l
extern _native void AI_GOAL_AIM_CLEAR(int pram0)l
extern _native void AI_GOAL_LOOK_AT_ACTOR(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6)l
extern _native void AI_GOAL_LOOK_AT_COORD(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5)l
extern _native void AI_GOAL_LOOK_AT_ACTOR_NEW(int pram0, int pram1, int pram2, int pram3)l
extern _native void AI_GOAL_LOOK_AT_COORD_NEW()l
extern _native void AI_GOAL_LOOK_AT_NEUTRAL(int pram0, int pram1)l
extern _native void AI_GOAL_LOOK_CLEAR(int pram0)l
extern _native void AI_GOAL_SHOOT_AT_OBJECT()l
extern _native void AI_GOAL_SHOOT_AT_COORD(int pram0, int pram1)l
extern _native void AI_GOAL_SHOOT_CLEAR(int pram0)l
extern _native void AI_GOAL_STAND_AT_COORD()l
extern _native void AI_GOAL_STAND_CLEAR()l
extern _native int AI_PREDICATE_FIND_NAMED(int pram0, int pram1)l
extern _native int AI_PREDICATE_IS_VALID(int pram0)l
extern _native void AI_PREDICATE_OVERRIDE_CLEAR(int pram0, int pram1)l
extern _native void AI_PREDICATE_OVERRIDE_CLEAR_ALL(int pram0)l
extern _native void AI_PREDICATE_OVERRIDE_SET_BOOL(int pram0, int pram1, int pram2)l
extern _native int AI_IGNORE_ACTOR(int pram0)l
extern _native int AI_STOP_IGNORING_ACTOR(int pram0)l
extern _native void AI_STOP_IGNORING_ACTORS()l
extern _native void AI_DONT_HARM_ACTOR(int pram0)l
extern _native void AI_CLEAR_DONT_HARM_ACTOR(int pram0)l
extern _native void AI_SET_ALLOW_HOSTILE_ATTACK_AI(int pram0)l
extern _native void AI_SET_ALLOW_HOSTILE_ATTACK_PLAYER(int pram0)l
extern _native void AI_SHOOT_TARGET_CLEAR_OFFSET()l
extern _native void AI_SHOOT_TARGET_SET_OFFSET()l
extern _native void AI_SHOOT_TARGET_SET_BONE(int pram0, int pram1, int pram2)l
extern _native void PLAYER_LOOK_AT_COORD()l
extern _native void PLAYER_STOP_LOOK_AT_COORD()l
extern _native void SET_FORCED_LOOK_ENABLE(int pram0, int pram1)l
extern _native void GET_FORCED_LOOK_ENABLE()l
extern _native void FORCE_LOOK_AT_COORD()l
extern _native void STOP_FORCE_LOOK_AT_COORD(int pram0)l
extern _native void FORCE_LOOK_AT_ACTOR(int pram0, int pram1, int pram2)l
extern _native void SET_AUTO_CONVERSATION_LOOK(int pram0, int pram1)l
extern _native void AI_GOAL_LOOK_AT_PLAYER_WHEN_WITHIN(int pram0, int pram1)l
extern _native void AI_GOAL_LOOK_AT_PLAYER_WHEN_WITHIN_CLEAR()l
extern _native void CLEAR_ACTORS_HORSE(int pram0)l
extern _native int GET_ACTORS_HORSE(int pram0)l
extern _native void SET_ACTORS_HORSE(int pram0, int pram1)l
extern _native int IS_AI_ACTOR_IN_COMBAT(int pram0)l
extern _native void IS_AI_ACTOR_PERFORMING_TASK()l
extern _native int IS_AI_ACTOR_UNALERTED(int pram0)l
extern _native void AI_SET_RANGE_ACCURACY_MODIFIER(int pram0, int pram1, int pram2)l
extern _native void AI_SET_DISARMED(int pram0, int pram1)l
extern _native int AI_IMPAIRMENT_MASK_GET_CURRENT_FOR_ACTOR(int pram0)l
extern _native int AI_IMPAIRMENT_MASK_MATCHES(int pram0, int pram1)l
extern _native void AI_ACTOR_FORCE_SPEED(int pram0, int pram1)l
extern _native void AI_ACTOR_SET_MATCH_WALK_SPEED_ENABLED(int pram0, int pram1)l
extern _native void AI_SET_WEAPON_MIN_RANGE(int pram0, int pram1)l
extern _native void AI_SET_WEAPON_MAX_RANGE(int pram0, int pram1)l
extern _native void AI_SET_WEAPON_DESIRED_RANGE(int pram0, int pram1)l
extern _native void AI_SET_BURST_DURATION(int pram0, int pram1)l
extern _native void AI_SET_BURST_DURATION_RANDOMNESS(int pram0, int pram1)l
extern _native void AI_SET_FIRE_DELAY(int pram0, int pram1)l
extern _native void AI_SET_FIRE_DELAY_RANDOMNESS(int pram0, int pram1)l
extern _native void AI_SET_SHOTS_PER_BURST(int pram0, int pram1)l
extern _native void AI_CLEAR_BURST_DURATION(int pram0)l
extern _native void AI_CLEAR_BURST_DURATION_RANDOMNESS(int pram0)l
extern _native void AI_CLEAR_FIRE_DELAY(int pram0)l
extern _native void AI_CLEAR_FIRE_DELAY_RANDOMNESS(int pram0)l
extern _native void AI_CLEAR_SHOTS_PER_BURST(int pram0)l
extern _native void AI_RESET_FIRING_FSM(int pram0)l
extern _native void AI_SET_GATLING_MAX_HORIZONTAL_DEGREES(int pram0, int pram1)l
extern _native int AI_IS_HOSTILE_OR_ENEMY(int pram0, int pram1)l
extern _native void AI_QUICK_EXIT_GRINGO(int pram0, int pram1)l
extern _native int AI_IS_AGGROING(int pram0, int pram1)l
extern _native void AI_SET_ALLOWED_MOUNT_DIRECTIONS(int pram0, int pram1)l
extern _native void AI_AVOID_IGNORE_ACTOR(int pram0, int pram1)l
extern _native void AI_AVOID_CLEAR_IGNORE_ACTOR()l
extern _native int IS_AI_ACTOR_ENGAGED_IN_COMBAT(int pram0)l
extern _native int WAS_AI_ACTOR_PLAYER_WEAPON_THREATENED_BY(int pram0, int pram1, int pram2)l
extern _native void AI_SET_ALLOW_ATTACK_HOGTIED_ACTORS(int pram0, int pram1)l
extern _native int AI_GET_IS_RETREATING(int pram0)l
extern _native void AI_HAS_PLAYER_FIRED_GUN_WITHIN()l
extern _native int AI_HAS_PLAYER_PROJECTILE_IMPACTED_WITHIN(int pram0, int pram1)l
extern _native int AI_HAS_PLAYER_PROJECTILE_NEAR_MISSED_WITHIN(int pram0, int pram1, int pram2)l
extern _native void AI_SET_PLAYER_PROJECTILE_IMPACT_HEAR_RANGE(int pram0, int pram1)l
extern _native void AI_DONT_SLOW_DOWN_TO_WALK_FOR_TURNS()l
extern _native int AI_HAS_ACTOR_BUMPED_INTO_ME(int pram0, int pram1, int pram2)l
extern _native int AI_WAS_PUSHED_OVER(int pram0, int pram1)l
extern _native void AI_WAS_PUSHED_OVER_BY()l
extern _native void AI_SELF_DEFENSE_GET_PLAYER_ATTACKED_FIRST()l
extern _native int AI_SELF_DEFENSE_GET_ATTACKED_PLAYER_FIRST(int pram0)l
extern _native void AI_SELF_DEFENSE_SET_PLAYER_ATTACKED_FIRST()l
extern _native void AI_SET_IGNORE_OPEN_AREA_MATERIAL()l
extern _native void AI_GET_IGNORE_OPEN_AREA_MATERIAL()l
extern _native void AI_SET_ENABLE_REACTION_VO()l
extern _native int AI_GET_TASK_RETREAT_FLAG(int pram0)l
extern _native void PREVENT_DESPAWN_CLEAR()l
extern _native void PREVENT_DESPAWN_SET_SPHERE(int pram0, int pram1, int pram2, int pram3)l
extern _native int AI_HAS_ACTOR_THREATENED_RECENTLY(int pram0)l
extern _native void AI_REPLACE_ALL_TR(int pram0)l
extern _native void AI_SET_TR_PROGRAM_FOR_ACTOR(int pram0, int pram1)l
#pragma endregion

#pragma region AI_MEMORY
extern _native void MEMORY_CLEAR_EVENTS(int pram0, int pram1)l
extern _native void MEMORY_CLEAR_ALL(int pram0)l
extern _native void MEMORY_CONSIDER_ACCORDING_TO_FACTION(int pram0, int pram1)l
extern _native void MEMORY_CONSIDER_AS(int pram0, int pram1, int pram2)l
extern _native void MEMORY_CONSIDER_AS_ENEMY(int pram0, int pram1)l
extern _native int MEMORY_GET_IS_IDENTIFIED(int pram0, int pram1)l
extern _native int MEMORY_GET_IS_VISIBLE(int pram0, int pram1)l
extern _native int MEMORY_GET_WAS_VISIBLE_WITHIN_TIME(int pram0, int pram1, int pram2)l
extern _native void MEMORY_IDENTIFY(int pram0, int pram1)l
extern _native void MEMORY_REPORT_POSITION(int pram0, int pram1, int pram2)l
extern _native void MEMORY_REPORT_POSITION_AUTO(int pram0, int pram1, int pram2)l
extern _native int MEMORY_GET_MUST_IDENTIFY(int pram0, int pram1)l
extern _native void MEMORY_ATTACK_ON_SIGHT(int pram0, int pram1)l
extern _native void MEMORY_CLEAR_RIDING_PREFERENCE(int pram0)l
extern _native void MEMORY_PREFER_RIDING(int pram0, int pram1)l
extern _native void MEMORY_PREFER_WALKING(int pram0, int pram1)l
extern _native void MEMORY_PREFER_MELEE(int pram0, int pram1)l
extern _native void MEMORY_FORCE_MELEE(int pram0, int pram1)l
extern _native void MEMORY_ALLOW_SHOOTING(int pram0, int pram1)l
extern _native void MEMORY_ALLOW_TAKE_COVER(int pram0, int pram1)l
extern _native void MEMORY_ALLOW_THROWING_EXPLOSIVES(int pram0, int pram1)l
extern _native void MEMORY_ALLOW_PICKUP_WEAPONS()l
extern _native void MEMORY_GET_WEAPON_DRAW_PREFERENCE()l
extern _native void MEMORY_CLEAR_WEAPON_DRAW_PREFERENCE(int pram0)l
extern _native void MEMORY_SET_WEAPON_DRAW_PREFERENCE(int pram0, int pram1)l
extern _native int MEMORY_GET_POSITION_LAST_KNOWN_TIME(int pram0, int pram1)l
extern _native void MEMORY_EVERYBODY_FORGET_ABOUT(int pram0)l
extern _native void MEMORY_EVERYBODY_FORGET_ABOUT_EVERYTHING()l
extern _native void MEMORY_SHOULD_ALWAYS_PATHFIND_IN_FORMATION(int pram0, int pram1)l
extern _native void AI_GLOBAL_CLEAR_ALL_DANGER()l
extern _native void AI_GLOBAL_CLEAR_DANGER(int pram0)l
extern _native int AI_GLOBAL_GET_PERMANENT_DANGER(int pram0)l
extern _native int AI_GLOBAL_IS_DANGER(int pram0, int pram1)l
extern _native void AI_GLOBAL_SET_PERMANENT_DANGER(int pram0, int pram1)l
extern _native void AI_GLOBAL_REPORT_DANGER(int pram0)l
extern _native void MEMORY_SET_UNARMED_RETREAT()l
#pragma endregion

#pragma region AI_COMBAT_CLASS
extern _native void COMBAT_CLASS_AI_CLEAR_ALL_ATTRIBS(int pram0)l
extern _native void COMBAT_CLASS_AI_GET_ATTRIB_BOOL()l
extern _native int COMBAT_CLASS_AI_GET_ATTRIB_FLOAT(int pram0, int pram1)l
extern _native int COMBAT_CLASS_AI_GET_RANGE_ACCURACY(int pram0)l
extern _native void COMBAT_CLASS_AI_SET_ATTRIB_BOOL(int pram0, int pram1, int pram2)l
extern _native void COMBAT_CLASS_AI_SET_ATTRIB_FLOAT(int pram0, int pram1, int pram2)l
extern _native void COMBAT_CLASS_AI_SET_FIGHT_ATTACK_DISTANCE(int pram0, int pram1, int pram2)l
extern _native void COMBAT_CLASS_AI_SET_FIGHT_DESIRED_DISTANCE(int pram0, int pram1, int pram2)l
extern _native void COMBAT_CLASS_AI_SET_FIGHT_TIME_BETWEEN_ATTACKS_MULTIPLIER()l
extern _native void COMBAT_CLASS_AI_SET_FIGHT_TIME_BETWEEN_ATTACKS(int pram0, int pram1)l
extern _native void COMBAT_CLASS_AI_SET_FRIENDLY_FIRE_CONSIDERATION(int pram0, int pram1)l
extern _native void COMBAT_CLASS_AI_SET_RANGE_ACCURACY(int pram0, int pram1)l
extern _native void COMBAT_CLASS_AI_SET_RANGE_BETWEEN_BURSTS_DELAY(int pram0, int pram1, int pram2)l
extern _native void COMBAT_CLASS_NAME_REGISTER_INT(int pram0, int pram1)l
extern _native int COMBAT_CLASS_REQUEST_EXISTS()l
extern _native int COMBAT_CLASS_REQUEST_GET_ACTOR()l
extern _native int COMBAT_CLASS_REQUEST_GET_ENUM_INT()l
extern _native void COMBAT_CLASS_REQUEST_COMPLETED()l
extern _native void COMBAT_CLASS_SERVER_SET_SCRIPT(int pram0)l
extern _native void AI_COMBAT_SET_NEW_STATE_MACHINE_ENABLED(int pram0, int pram1)l
#pragma endregion

#pragma region AI_ANIMAL
extern _native void ANIMAL_SPECIES_FLOCK_AND_TUNING_CLEAR_ALL()l
extern _native void ANIMAL_SPECIES_NEEDS_DOMESTICATION_LEVELS(int pram0)l
extern _native void ANIMAL_SPECIES_SET_SPECIAL_USE_GRINGO(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ANIMAL_SPECIES_SET_UNALERTED_BEHAVIOR(int pram0, int pram1, int pram2, int pram3)l
extern _native void ANIMAL_SPECIES_FLOCK_SET_ENABLED(int pram0, int pram1, int pram2, int pram3)l
extern _native void ANIMAL_SPECIES_FLOCK_SET_PARAMETER(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ANIMAL_SPECIES_FLOCK_SET_BOOLEAN_PARAMETER(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ANIMAL_SPECIES_ADD_EXTERNAL_PATH_ATTRACTION()l
extern _native void ANIMAL_SPECIES_REMOVE_EXTERNAL_PATH_ATTRACTION()l
extern _native void ANIMAL_SPECIES_ADD_EXTERNAL_RANDOM_NOISE(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ANIMAL_SPECIES_ADD_EXTERNAL_REPULSION(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ANIMAL_SPECIES_ADD_EXTERNAL_INFLUENCE_FLOCK_REASONER(int pram0, int pram1)l
extern _native int ANIMAL_SPECIES_TUNING_GET_ATTRIB_FLOAT(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_TUNING_SET_ATTRIB_BOOL(int pram0, int pram1, int pram2, int pram3)l
extern _native void ANIMAL_SPECIES_TUNING_SET_ATTRIB_FLOAT(int pram0, int pram1, int pram2, int pram3)l
extern _native void ANIMAL_SPECIES_TUNING_MOVE_SET_ATTRIB(int pram0, int pram1, int pram2, int pram3, int pram4)l
extern _native void ANIMAL_SPECIES_TUNING_SET_ATTACHMENT_WITH_OFFSET(int pram0, int pram1, int pram2, int pram3, int pram4, int pram5, int pram6, int pram7, int pram8)l
extern _native void ANIMAL_SPECIES_TUNING_SET_ATTACHMENT_WITH_CHILDBONE(int pram0, int pram1, int pram2, int pram3)l
extern _native void ANIMAL_SPECIES_TUNING_SET_HUNTING_PREY_PROP(int pram0, int pram1)l
extern _native void ANIMAL_SPECIES_TUNING_SET_ATTRIB_FLOAT_FROM_TIME(int pram0, int pram1, int pram2, int pram3)l
extern _native void ANIMAL_SPECIES_INIT_BEGIN(int pram0)l
extern _native void ANIMAL_SPECIES_INIT_REGISTER(int pram0, int pram1)l
extern _native void ANIMAL_SPECIES_INIT_END()l
extern _native void ANIMAL_SPECIES_GRINGO_CLEAR_ALL()l
extern _native void ANIMAL_SPECIES_GRINGO_LOAD_ALL()l
extern _native void ANIMAL_SPECIES_REL_CLEAR_ALL()l
extern _native void ANIMAL_SPECIES_REL_SET_ATTACK_GRAB_ENABLED(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_PREDATOR_AND_PREY(int pram0, int pram1)l
extern _native void ANIMAL_SPECIES_REL_SET_THREAT(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_AVOID(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_PLAY_HUNT(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_PLAY_CHASE(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_PLAY_BEG(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_PLAY_GROWL(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_PLAY_SNIFF(int pram0, int pram1, int pram2)l
extern _native int ANIMAL_SPECIES_REL_GET_CAN_ATTACK(int pram0, int pram1)l
extern _native void ANIMAL_SPECIES_REL_SET_CAN_ATTACK(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_CAN_WARN(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_SPECIES_REL_SET_EAT_GRINGO(int pram0, int pram1, int pram2)l
extern _native int ANIMAL_ACTOR_GET_DOMESTICATION(int pram0)l
extern _native void ANIMAL_ACTOR_SET_DOMESTICATION(Actor Actor, bool Domestication)l
extern _native int ANIMAL_ACTOR_GET_SPECIES(int pram0)l
extern _native void ANIMAL_TUNING_SET_ATTRIB_BOOL(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_TUNING_SET_ATTRIB_FLOAT(int pram0, int pram1, int pram2)l
extern _native void ANIMAL_ACTOR_SET_DOCILE(Actor Actor, bool Docile)l
extern _native void ANIMAL_ACTOR_GET_DOCILE()l
extern _native void ANIMAL_ACTOR_GET_GRABBED_BY()l
#pragma endregion

#pragma region ANIMATION
extern _native int GET_ACTOR_ANIM_CURRENT_TIME(int pram0, int pram1)l
extern _native int SET_ACTOR_ANIM_CURRENT_TIME(int pram0, int pram1)l
extern _native int IS_ACTOR_ANIM_PLAYING(int pram0, int pram1)l
extern _native void ACTOR_HAS_ANIM_LOADED()l
extern _native int ACTOR_HAS_ANIM_SET(int pram0, int pram1)l
extern _native void SET_ANIMATION_OVERRIDE_SCALE(int pram0, int pram1)l
extern _native int SET_PANIM_PARAMS(int pram0, int pram1, int pram2)l
extern _native int SET_PANIM_PHASE(int pram0, int pram1)l
extern _native void SET_ACTOR_ANIM_PHASE_LOCK(int pram0, int pram1)l
extern _native void RELEASE_ACTOR_ANIM_PHASE_LOCK(int pram0)l
extern _native int IS_ACTOR_ANIM_PHASE_LOCKED(int pram0)l
extern _native void SET_ACTOR_CUTSCENE_MODE(int pram0, int pram1)l
extern _native void REQUEST_ANIM_SET(int pram0, int pram1)l
extern _native int HAS_ANIM_SET_LOADED(int pram0)l
extern _native void REMOVE_ANIM_SET(int pram0)l
extern _native int SET_ANIM_SET_FOR_ACTOR(int pram0, int pram1, int pram2)l
extern _native void RESET_ANIM_SET_FOR_ACTOR(int pram0, int pram1)l
extern _native void DLC_REPLACE_EXISTING_ANIM_SET(int pram0)l
extern _native int REQUEST_ACTION_TREE(int pram0)l
extern _native int HAS_ACTION_TREE_LOADED(int pram0)l
extern _native void REMOVE_ACTION_TREE(int pram0)l
extern _native int SET_ACTION_NODE_FOR_ACTOR(int pram0, int pram1)l
extern _native int SET_REACT_NODE_FOR_ACTOR(int pram0, int pram1)l
extern _native void RESET_REACT_NODE_FOR_ACTOR(int pram0)l
extern _native void RESET_ACTIONTREE_FOR_ACTOR(int pram0)l
extern _native int SET_LINKED_ANIM_TARGET(int pram0, int pram1)l
extern _native int GET_LINKED_ANIM_TARGET(int pram0)l
extern _native int CLEAR_LINKED_ANIM_TARGET(int pram0)l
extern _native int IS_ACTOR_PERFORMING_LINKED_ANIMATION(int pram0)l
extern _native int IS_ACTION_NODE_PLAYING(int pram0, int pram1)l
extern _native int IS_ACTION_NODE_PLAYING_PARTIAL(int pram0, int pram1)l
extern _native int IS_ACTOR_PLAYING_NODE_IN_TREE(int pram0, int pram1)l
extern _native int GET_CURR_ACTION_NODE_PLAY_TIME(int pram0)l
extern _native int TOUGH_ARMOUR_GET_TUNING_REGENERATION_RATE(int pram0)l
extern _native void TOUGH_ARMOUR_SET_TUNING_HIT_DEDUCTION(int pram0, int pram1, int pram2)l
extern _native void TOUGH_ARMOUR_SET_TUNING_PAD_ARMOUR(int pram0, int pram1)l
extern _native void TOUGH_ARMOUR_SET_TUNING_REGENERATION_RATE(int pram0, int pram1)l
extern _native void SET_ACTOR_CHARACTER_CLOTH_SCALED_PINNING(int pram0, int pram1)l
extern _native void SET_ACTOR_TO_SEAT(int pram0, int pram1)l
#pragma endregion

#pragma region MINIGAME
extern _native void START_MINIGAME(int pram0)l
extern _native void PUSH_MINIGAME_INPUT(int pram0, int pram1)l
extern _native int IS_MINIGAME_RUNNING()l
extern _native void END_CURRENT_MINIGAME()l
extern _native int UNK_0x6AAD0420(int pram0, int pram1, int pram2, int pram3)l
extern _native int UNK_0x655D350B(int pram0, int pram1, int pram2, int pram3)l
extern _native int SET_CURRENT_MINIGAME_INT(int pram0, int pram1)l
extern _native void SET_MINIGAME_SCRIPT_OVERRIDE()l
extern _native void SET_MINIGAME_WIN_STATE(int pram0)l
#pragma endregion

#undef _native
#undef _native32
#undef l