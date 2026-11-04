/*H**********************************************************************
* FILENAME :	natives32Old.h					START DATE :	10 Sept 16
*
* DESCRIPTION :
*		Native functions to be used with all GTAIV(1.0.4.0) targets and platforms of SC-CL.
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
*		Panah (Sockstress)
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

#pragma region CUSTOM
extern _native bool CTL_GIVE_MACHINE_ALIAS(int* MachineIDPointer, char* alias)l //CTL_GIVE_MACHINE_ALIAS(&ids->machineId[0], "alias here");
extern _native bool CTL_MACHINE_HAS_ALIAS(int* MachineIDPointer)l //CTL_MACHINE_HAS_ALIAS(&ids->machineId[0]);
extern _native char* CTL_GET_QUERIED_ALIAS(void)l //call immediately after CTL_MACHINE_HAS_ALIAS
extern _native bool CTL_REMOVE_MACHINE_ALIAS(int* MachineIDPointer)l //CTL_REMOVE_MACHINE_ALIAS(&ids->machineId[0]);
extern _native void CTL_INIT_ALIAS_DATA(void)l //need to call once on startup. // Admins / Mods only

extern _native bool CTL_REMOVE_MACHINE_FROM_MODDER_FILE(int* MachineIDPointer)l //CTL_REMOVE_MACHINE_FROM_MODDER_FILE(&ids->machineId[0]);
extern _native bool CTL_LOG_MACHINE_TO_MODDER_FILE(int* MachineIDPointer)l //CTL_LOG_MACHINE_TO_MODDER_FILE(&ids->machineId[0]);
extern _native bool CTL_IS_MACHINE_IN_MODDER_FILE(int* MachineIDPointer)l //CTL_LOG_MACHINE_TO_MODDER_FILE(&ids->machineId[0]);
extern _native bool CTL_PROVIDE_PLAYER(char* gamertag, char* xuid, char* consoleType, char* MachineID, char* WAN_IP, char* LAN_IP, byte* MAC_Addr)l
extern _native bool CTL_SPOOF_XUID_GIVEN_GAMERTAG(char* spoofTag)l  //dosn't work atm fucked.
extern _native bool CTL_IS_BUSY(void)l
extern _native void CTL_WRITE_TO_FILE(char* fileName, char* string)l //append .fileType i.e. .txt
extern _native void CTL_XNOTIFY_GREETING(void)l
extern _native void CTL_REQUEST_SEVER_MOTD(void)l

//extern _native char* CTL_READ_LINE_FROM_FILE(char* fileName, int line)l //first line is 1, not 0 //append .fileType i.e. .txt
extern _native float CTL_STRING_TO_FLOAT(char* str)l
extern _native char* CTL_FLOAT_TO_STRING(float ft)l
extern _native bool CTL_OPEN_FILE_FOR_READING(char* fileName)l //append .fileType i.e. ctl_player_log.txt / usage will likely be if(CTL_OPEN_FILE_FOR_READING("maps\\map_name.ini"))
extern _native char* CTL_READ_LINE_FROM_OPENED_FILE(int line)l  //this must be used incrementally, otherwise you'll cause failures.
extern _native bool CTL_CLOSE_READING_FILE(void)l

extern _native bool CTL_OPEN_FILE_FOR_WRITING(char* fileName)l //append .fileType i.e. ctl_player_log.txt / usage will likely be if(CTL_OPEN_FILE_FOR_READING("maps\\map_name.ini"))
extern _native bool CTL_WRITE_LINE_TO_OPENED_FILE(char* line)l  //this must be used incrementally, otherwise you'll cause failures.
extern _native bool CTL_CLOSE_OPENED_WRITING_FILE(void)l
///////////////

extern _native int CTL_GET_LINES_IN_FILE(char* fileName)l //append .fileType i.e. .txt
extern _native void CTL_GET_LOCAL_TIME(int* hour, int* minute)l
extern _native void CTL_PROVIDE(int stringNum, char* string)l
extern _native void CTL_ADD_XEX_COMPILE_TIME_TO_BRIEF(void)l
extern _native void CTL_FORCE_REMOVE_NETWORK_ENTITY(NETWORK_INDEX networkId, PLAYER_INDEX removeFrom)l
extern _native void CTL_OFFHOST_KICK(PLAYER_INDEX target)l
extern _native void CTL_CHANGE_OWNER_ENTITY(NETWORK_INDEX netId, PLAYER_INDEX target)l
extern _native void CTL_CHANGE_SESSION_SETTINGS(int unk0, int unk1, int unk2, int unk3, int unk4, int unk5, int unk6)l
extern _native bool CTL_GRAB_LATEST_ATTACKER(int* player_index, CTL_ATTACK_TYPES* type, uint* extra)l
//extern _native void CTL_GIVE_CONTROL_OF_NETWORK_ID(PLAYER_INDEX player, NETWORK_INDEX netId)l
extern _native int* CTL_GET_EVENT_BITSET(void)l
extern _native int* CTL_GET_CLONE_BITSET(void)l
extern _native int* CTL_GET_SPAWNER_MEMORY(void)l
extern _native void CTL_ADD_BLACKLIST_PED(unsigned int model_hash)l
extern _native void CTL_REMOVE_BLACKLIST_PED(unsigned int model_hash)l
extern _native void CTL_ADD_BLACKLIST_OBJECT(unsigned int model_hash)l
extern _native void CTL_REMOVE_BLACKLIST_OBJECT(unsigned int model_hash)l
extern _native void CTL_SET_CLONE_SPAM_RESET_TIME(int time)l //time is in ms
extern _native void CTL_SET_CLONE_SPAM_LOCKOUT_COUNTS(int other_count, int car_count)l

extern _native void CTL_SET_SESSION(int session)l //Session Browser
extern _native void CTL_FORCE_ONLINE_STATE(bool force)l //used for System-LINK

extern _native void CTL_INITIATE_SPOOFER_CHECK(PLAYER_INDEX target, char* player_name)l
extern _native int CTL_IS_PLAYER_SPOOFING(PLAYER_INDEX target)l //returns -1 while processing.
extern _native void CTL_CLEAR_PLAYER_SPOOFING_STATUS(PLAYER_INDEX target)l //hopefully junk this eventualy.

extern _native bool CTL_IS_SERVER_AVAILABLE(void)l

extern _native bool CTL_DOWNLOAD_MAP_LIST(int order_mode, int* entry_count)l
extern _native bool CTL_GET_MAP_LIST_ENTRY(int entry_num, char** name, char** info)l
extern _native bool CTL_SET_MAP_LIST_FILTER(char* creator)l
extern _native void CTL_DOWNLOAD_MAP_FILE(char* mapName, char* itemInfo)l	//will start a thread to do it and CAP you to 1 thread.
extern _native bool CTL_DELETE_MAP_FILE_FROM_SERVER(char* mapName, char* itemInfo)l
extern _native bool CTL_UPLOAD_MAP_INI(char* path)l //i'll be fucked if this works.

extern _native bool CTL_GET_FILES_IN_DIRECTORY(bool folders_instead, char* path, char* extension, int* count)l
extern _native bool CTL_GET_FILES_IN_DIRECTORY_ENTRY(int entry_num, char** entry_string_pointer)l
extern _native bool CTL_DELETE_FILE(char* path)l
#pragma endregion

#pragma region BUILTINS
//PURPOSE: Returns input number rounded up toward positive infinity
extern _native float CEIL(float value)l

//PURPOSE: Cosine (input is in degrees)
extern _native float COS(float value)l

//PURPOSE: Returns e to the power of f
extern _native float EXP(float f)l

//PURPOSE: Returns input number rounded down toward negative infinity
extern _native float FLOOR(float value)l

//PURPOSE: Returns a raised to the b'th power
extern _native float POW(float a, float b)l

//PURPOSE: Returns input number rounded to the nearest number.  
//   x.5 is rounded toward positive infinity for both negative and positive inputs.
extern _native float ROUND(float f)l

//PURPOSE: Sine (input is in degrees)
extern _native float SIN(float value)l

//PURPOSE: Square root
extern _native float SQRT(float value)l

//PURPOSE: returns the current timestep (in seconds)
extern _native float TIMESTEP(void)l

//PURPOSE: returns the current timestep unwarped (in seconds)
extern _native float TIMESTEPUNWARPED(void)l

//PURPOSE: Converts an integer to a floating-point number.
extern _native float TO_FLOAT(int value)l

//PURPOSE: Distance between two points
extern _native float VDIST(float x0, float y0, float z0, float x1, float y1, float z1)l

//PURPOSE: Square of distance between two points (much less costly to compute)
extern _native float VDIST2(float x0, float y0, float z0, float x1, float y1, float z1)l

//PURPOSE: Vector magnitude
extern _native float VMAG(float x, float y, float z)l

//PURPOSE: Square of vector magnitude (much less costly to compute)
extern _native float VMAG2(float x, float y, float z)l

//PURPOSE: Implements C++ operator << (shift left)
extern _native int SHIFT_LEFT(int val, int shifts)l

//PURPOSE: Implements C++ operator >> (shift right)
extern _native int SHIFT_RIGHT(int val, int shifts)l

//PURPOSE: Gets the current value for the timer A
//RETURNS: The integer value for timer A (in milliseconds)
extern _native int TIMERA(void)l

//PURPOSE: Gets the current value for the timer B
//RETURNS: The integer value for timer B (in milliseconds)
extern _native int TIMERB(void)l

//PURPOSE: function that starts a new script
//PARAMS:	s - Name of the script to start
//			stack_size - Stack size, in words, or zero for default
//RETURNS: the thread id of the new script (0 if no threads are available)
extern _native THREADID START_NEW_SCRIPT(const char* scriptName, int stacksize)l

//PURPOSE: function that starts a new script
//PARAMS:	s - Name of the script to start
//			args - Arg structure (can be any structure)
//			size_of_args - SIZE_OF(args)
//			stack_size - Stack size, in words, or zero for default
//RETURNS: the thread id of the new script (0 if no threads are available)
extern _native THREADID START_NEW_SCRIPT_WITH_ARGS(const char* scriptname, uint *params, int paramcount, int stacksize)l

//PURPOSE: Triggers a breakpoint in the C++ debugger, useful for programmers
extern _native void BREAKPOINT(void)l

//PURPOSE: Resets the input text label to the empty string
extern _native void CLEAR_TEXT_LABEL(const char* label)l

//PURPOSE: procedure that prints a float to the console (NOT the game screen)
extern _native void PRINTFLOAT(float value)l

//PURPOSE: procedure that prints a float to the console (NOT the game screen)
//Accepts field width and precision like C printf function
extern _native void PRINTFLOAT2(int width, int precision, float f)l

//PURPOSE: procedure that prints an integer to the console (NOT the game screen)
extern _native void PRINTINT(int value)l

//PURPOSE: procedure that prints an integer to the console (NOT the game screen)
//Accepts field width like C printf functio
extern _native void PRINTINT2(int width, int i)l

//PURPOSE: procedure that prints a newline to the console (NOT the game screen)
extern _native void PRINTNL(void)l

//PURPOSE: procedure that prints a string to the console (NOT the game screen)
extern _native void PRINTSTRING(const char* value)l

//PURPOSE: procedure that prints a vector to the console (NOT the game screen)
extern _native void PRINTVECTOR(float x, float y, float z)l

//PURPOSE: Sets the value for the timer A (in milliseconds)
extern _native void SETTIMERA(int value)l

//PURPOSE: sets the value for the timer B (in milliseconds)
extern _native void SETTIMERB(int value)l

//PURPOSE: stops script execution for I milliseconds
extern _native void WAIT(int i)l
extern _native void WAITUNPAUSED(int i)l
extern _native void WAITUNWARPED(int i)l

//Deprecated
//GET_LATEST_CONSOLE_COMMAND
//RESET_LATEST_CONSOLE_COMMAND
//GET_CONSOLE_COMMAND_TOKEN
//GET_NUM_CONSOLE_COMMAND_TOKENS
#pragma endregion

#pragma region HTML
extern _native bool DOES_WEB_PAGE_EXIST(const char* file_name)l
extern _native const char* GET_WEB_PAGE_LINK_HREF(int htmlviewport, int linkid)l
extern _native float GET_WEB_PAGE_HEIGHT(int htmlviewport)l
extern _native int CREATE_HTML_SCRIPT_OBJECT(const char* objname)l
extern _native int GET_NUMBER_OF_WEB_PAGE_LINKS(int htmlviewport)l
extern _native int GET_WEB_PAGE_LINK_AT_POSN(int htmlviewport, float x, float y)l
extern _native void ADD_TO_HTML_SCRIPT_OBJECT(int htmlobj, const char* htmlcode)l
extern _native void CREATE_HTML_VIEWPORT(int* htmlviewport)l
extern _native void DELETE_ALL_HTML_SCRIPT_OBJECTS(void)l
extern _native void DELETE_HTML_SCRIPT_OBJECT(int htmlobj)l
extern _native void GET_WEB_PAGE_LINK_POSN(int htmlviewport, int linkid, float *x, float *y)l
extern _native void LOAD_WEB_PAGE(int htmlviewport, const char* file_name)l
extern _native void RELOAD_WEB_PAGE(int htmlviewport)l
extern _native void SET_WEB_PAGE_LINK_ACTIVE(int htmlviewport, int linkid, bool active)l
extern _native void SET_WEB_PAGE_SCROLL(int htmlviewport, float scroll)l
#pragma endregion

#pragma region NY
extern _native bool CAN_PHONE_BE_SEEN_ON_SCREEN(void)l

//PURPOSE: Is this ped visible?
extern _native bool CELL_CAM_IS_CHAR_VISIBLE(PED_INDEX ped)l

//PURPOSE: Is this ped visible?.. but disregard if we can see his/her face or not.
extern _native bool CELL_CAM_IS_CHAR_VISIBLE_NO_FACE_CHECK(PED_INDEX ped)l
extern _native bool CODE_WANTS_MOBILE_PHONE_REMOVED(void)l
extern _native bool CODE_WANTS_MOBILE_PHONE_REMOVED_FOR_WEAPON_SWITCHING(void)l
extern _native bool CREATE_EMERGENCY_SERVICES_CAR(uint model, float x, float y, float z)l
extern _native bool CREATE_EMERGENCY_SERVICES_CAR_RETURN_DRIVER(uint model, float x, float y, float z, VEHICLE_INDEX* car, PED_INDEX* driver, PED_INDEX* passenger)l
extern _native bool CREATE_EMERGENCY_SERVICES_CAR_THEN_WALK(uint model, float x, float y, float z)l
extern _native bool HAS_POOL_OBJECT_COLLIDED_WITH_CUSHION(OBJECT_INDEX obj)l
extern _native bool HAS_POOL_OBJECT_COLLIDED_WITH_OBJECT(OBJECT_INDEX obj, OBJECT_INDEX otherObj)l
extern _native bool WANTED_STARS_ARE_FLASHING(void)l

extern _native float GET_MOBILE_PHONE_SCALE(void)l

extern _native int GET_NUMBER_OF_INJURED_PEDS_IN_RANGE(float x, float y, float z, float radius)l
extern _native int GET_VEHICLE_TYPE_OF_MODEL(uint model)l

extern _native void ALLOW_EMERGENCY_SERVICES(bool allow)l

//PURPOSE: Turn on/off the camera for the cell phone
extern _native void CELL_CAM_ACTIVATE(bool enable_photo_update, bool go_first_person)l

//PURPOSE: 0.5,0.5 by default
extern _native void CELL_CAM_SET_CENTRE_POS(float x, float y)l

//PURPOSE: colour of mobile picture screen 
extern _native void CELL_CAM_SET_COLOUR_BRIGHTNESS(float r, float g, float b, float bright)l

//PURPOSE: Zoom 0 to 1....
extern _native void CELL_CAM_SET_ZOOM(float zoom)l
extern _native void CREATE_MOBILE_PHONE(int mobile_phone_type)l
extern _native void DESTROY_MOBILE_PHONE(void)l
extern _native void DRAW_FRONTEND_HELPER_TEXT(const char* desc_text, const char* button_text, bool new_line)l
extern _native void DRAW_SPRITE_FRONT_BUFF(float x, float y, float w, float h, float rotation, int r, int g, int b, int a)l

//PURPOSE: Allows the script to draw the photo somewhere
extern _native void DRAW_SPRITE_PHOTO(float x, float y, float w, float h, float rotation, int r, int g, int b, int a)l
extern _native void FLASH_WEAPON_ICON(bool on)l
extern _native void GET_MOBILE_PHONE_POSITION(vector3* pos)l
extern _native void GET_MOBILE_PHONE_RENDER_ID(int *pRenderId)l
extern _native void GET_MOBILE_PHONE_ROTATION(vector3* rot)l
extern _native void GET_SCRIPT_RENDERTARGET_RENDER_ID(uint *pRenderId)l
extern _native void INIT_FRONTEND_HELPER_TEXT(void)l
extern _native void SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN(bool set)l
extern _native void SCRIPT_IS_USING_MOBILE_PHONE(bool set)l
extern _native void SET_MOBILE_PHONE_POSITION(float x, float y, float z)l
extern _native void SET_MOBILE_PHONE_ROTATION(float x, float y, float z)l
extern _native void SET_MOBILE_PHONE_SCALE(float scale)l
extern _native void SET_OBJECT_USED_IN_POOL_GAME(OBJECT_INDEX obj, bool set)l
extern _native void SET_USE_POOL_GAME_PHYSICS_SETTINGS(bool set)l

//PARAM NOTES:	arrester - The ped who is doing the arresting
//				arrestee - The ped being arrested
//PURPOSE: Set one ped to arrest another ped
extern _native void TASK_CHAR_ARREST_CHAR(PED_INDEX arrester, PED_INDEX arrestee)l

//Deprecated
//GET_RID_OF_PLAYER_PROSTITUTE
#pragma endregion

#pragma region GRAPHICS
// PURPOSE: Returns if running in hidef
extern _native bool GET_IS_HIDEF(void)l

// PURPOSE: Returns if running widescreen
extern _native bool GET_IS_WIDESCREEN(void)l

//INFO: After requesting a streamed txd, call this command to check when the txd has finished streaming. It will return TRUE once the txd has loaded. 
//PURPOSE: Checks that the streamed tecture dictionary has loaded.
extern _native bool HAS_STREAMED_TXD_LOADED(const char* txdName)l

//INFO: Triggers a named particle effect at a world position.
//  This should only be used with non-looped particle effects.
//  The particle effect will then play and tidy itself up when finished.
//  There is no access to the particle effect once it has been triggered. 
//PARAM NOTES:  STRING fx_name - the name of the particle effect to be triggered
//  VECTOR position - the world position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  FLOAT scale - size scale of the effect (default size = 1.0) 
//  BOOL - whether or not the particle effect has been created ok 
// PURPOSE: Trigger a set piece (non looped) particle effect at a world position and orientation.
extern _native bool TRIGGER_PTFX(const char* name, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)l

//INFO: Triggers a named particle effect on an object at an offset position.
//  This should only be used with non-looped particle effects.
//  The particle effect will then play and tidy itself up when finished.
//  There is no access to the particle effect once it has been triggered. 
//PARAM NOTES: STRING fx_name - the name of the particle effect to be triggered
//  OBJECT_INDEX obj - the index of the object to attach the particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  FLOAT scale - size scale of the effect (default size = 1.0) 
// PURPOSE: Trigger a set piece (non looped) particle effect on an object with an offset position and orientation.
extern _native bool TRIGGER_PTFX_ON_OBJ(const char* name, OBJECT_INDEX obj, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)l

//INFO: Triggers a named particle effect on an object bone at an offset position and rotation.
//  This should only be used with non-looped particle effects.
//  The particle effect will then play and tidy itself up when finished.
//  There is no access to the particle effect once it has been triggered. 
//PARAM NOTES:STRING STRING fx_name - the name of the particle effect to be triggered
//  OBJECT_INDEX obj - the index of the object to attach the particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  INT bone_tag - the bone tag to attach the particle effect to
//  FLOAT scale - size scale of the effect (default size = 1.0) 
// PURPOSE: Trigger a set piece (non looped) particle effect on an object bone with an offset position and orientation.
extern _native bool TRIGGER_PTFX_ON_OBJ_BONE(const char* name, OBJECT_INDEX obj, float x, float y, float z, float rot_x, float rot_y, float rot_z, uint objbone, float scale)l

//INFO: Triggers a named particle effect on a ped at an offset position.
//  This should only be used with non-looped particle effects.
//  The particle effect will then play and tidy itself up when finished.
//  There is no access to the particle effect once it has been triggered. 
//PARAM NOTES: STRING fx_name - the name of the particle effect to be triggered
//  PED_INDEX ped - the index of the ped to attach the particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  FLOAT scale - size scale of the effect (default size = 1.0) 
// PURPOSE: Trigger a set piece (non looped) particle effect on a ped with an offset position and orientation.
extern _native bool TRIGGER_PTFX_ON_PED(const char* name, PED_INDEX ped, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)l

//INFO: Triggers a named particle effect on a ped bone at an offset position and rotation.
//  This should only be used with non-looped particle effects.
//  The particle effect will then play and tidy itself up when finished.
//  There is no access to the particle effect once it has been triggered. 
//PARAM NOTES:STRING STRING fx_name - the name of the particle effect to be triggered
//  PED_INDEX ped - the index of the ped to attach the particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  INT bone_tag - the bone tag to attach the particle effect to
//  FLOAT scale - size scale of the effect (default size = 1.0) 
// PURPOSE: Trigger a set piece (non looped) particle effect on a ped bone with an offset position and orientation.
extern _native bool TRIGGER_PTFX_ON_PED_BONE(const char* name, PED_INDEX ped, float x, float y, float z, float rot_x, float rot_y, float rot_z, uint pedbone, float scale)l

//INFO: Triggers a named particle effect on a vehicle at an offset position.
//  This should only be used with non-looped particle effects.
//  The particle effect will then play and tidy itself up when finished.
//  There is no access to the particle effect once it has been triggered. 
//PARAM NOTES: STRING fxName - the name of the particle effect to be triggered
//  VEHICLE_INDEX veh - the index of the vehicle to attach the particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  FLOAT scale - size scale of the effect (default size = 1.0) 
// PURPOSE: Trigger a set piece (non looped) particle effect on a vehicle with an offset position and orientation.
extern _native bool TRIGGER_PTFX_ON_VEH(const char* name, VEHICLE_INDEX veh, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)l

//INFO: Triggers a named particle effect at an world position.
//  This should only be used with infinitely looping particle effects.
//  The particle effect will then start playing and will need to be stopped by calling STOP_PTFX.
//  The returned id of the particle effect needs to be passed into any other function that needs to be called on it. 
//PARAM NOTES:STRING fx_name - the name of the particle effect to be started
//  VECTOR position - the world position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  FLOAT scale - size scale of the effect (default size = 1.0) 
//  PTFX_ID - the id of the created particle effect - this will be 0 if creation fails 
// PURPOSE: Start a looped particle effect at a world position and orientation.
extern _native PTFX_ID START_PTFX(const char* name, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)l

//INFO: Triggers a named particle effect attached to an object with an offset position.
//  This should only be used with infinitely looping particle effects.
//  The particle effect will then start playing and will need to be stopped by calling STOP_PTFX.
//  The returned id of the particle effect needs to be passed into any other function that needs to be called on it.+ 
//PARAM NOTES:STRING fx_name - the name of the particle effect to be started
//  OBJECT_INDEX obj - the index of the object to attach this particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  FLOAT scale - size scale of the effect (default size = 1.0) 
//  PTFX_ID - the id of the created particle effect - this will be 0 if creation fails 
// PURPOSE: Start a looped particle effect on an object with an offset position and orientation.
extern _native PTFX_ID START_PTFX_ON_OBJ(const char* name, OBJECT_INDEX obj, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)l

//INFO: Triggers a named particle effect attached to an object bone with an offset position and rotation.
//  This should only be used with infinitely looping particle effects.
//  The particle effect will then start playing and will need to be stopped by calling STOP_PTFX.
//  The returned id of the particle effect needs to be passed into any other function that needs to be called on it. 
//PARAM NOTES: STRING fx_name - the name of the particle effect to be started
//  OBJECT_INDEX obj - the index of the object to attach this particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  INT bone_tag - the bone tag to attach the particle effect to
//  FLOAT scale - size scale of the effect (default size = 1.0)
// PTFX_ID - the id of the created particle effect - this will be 0 if creation fails 
// PURPOSE: Start a looped particle effect on an object bone with an offset position and orientation.
extern _native PTFX_ID START_PTFX_ON_OBJ_BONE(const char* name, OBJECT_INDEX obj, float x, float y, float z, float rot_x, float rot_y, float rot_z, uint objbone, float scale)l

//INFO: Triggers a named particle effect attached to a ped with an offset position.
//  This should only be used with infinitely looping particle effects.
//  The particle effect will then start playing and will need to be stopped by calling STOP_PTFX.
//  The returned id of the particle effect needs to be passed into any other function that needs to be called on it.+ 
//PARAM NOTES:STRING fx_name - the name of the particle effect to be started
//  PED_INDEX ped - the index of the ped to attach this particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  FLOAT scale - size scale of the effect (default size = 1.0) 
//  PTFX_ID - the id of the created particle effect - this will be 0 if creation fails 
// PURPOSE: Start a looped particle effect on a ped with an offset position and orientation.
extern _native PTFX_ID START_PTFX_ON_PED(const char* name, PED_INDEX ped, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)l

//INFO: Triggers a named particle effect attached to a ped bone with an offset position and rotation.
//  This should only be used with infinitely looping particle effects.
//  The particle effect will then start playing and will need to be stopped by calling STOP_PTFX.
//  The returned id of the particle effect needs to be passed into any other function that needs to be called on it. 
//PARAM NOTES: STRING fx_name - the name of the particle effect to be started
//  PED_INDEX ped - the index of the ped to attach this particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  INT bone_tag - the bone tag to attach the particle effect to
//  FLOAT scale - size scale of the effect (default size = 1.0)
// PTFX_ID - the id of the created particle effect - this will be 0 if creation fails 
// PURPOSE: Start a looped particle effect on a ped bone with an offset position and orientation.
extern _native PTFX_ID START_PTFX_ON_PED_BONE(const char* name, PED_INDEX ped, float x, float y, float z, float rot_x, float rot_y, float rot_z, uint pedbone, float scale)l

//INFO: Triggers a named particle effect attached to a vehicle with an offset position.
//  This should only be used with infinitely looping particle effects.
//  The particle effect will then start playing and will need to be stopped by calling STOP_PTFX.
//  The returned id of the particle effect needs to be passed into any other function that needs to be called on it.+ 
//PARAM NOTES:STRING fx_name - the name of the particle effect to be started
//  VEHICLE_INDEX veh - the index of the vehicle to attach this particle effect to
//  VECTOR position - the offset position to play the particle effect at
//  VECTOR rotation - the offset rotation of the particle effect
//  FLOAT scale - size scale of the effect (default size = 1.0) 
//  PTFX_ID - the id of the created particle effect - this will be 0 if creation fails 
// PURPOSE: Start a looped particle effect on a vehicle with an offset position and orientation.
extern _native PTFX_ID START_PTFX_ON_VEH(const char* name, VEHICLE_INDEX veh, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)l
extern _native Texture GET_TEXTURE(TextureDict dictionary, const char* textureName)l
extern _native Texture GET_TEXTURE_FROM_STREAMED_TXD(const char* txdName, const char* textureName)l
extern _native TextureDict GET_TXD(const char* txdName)l

//INFO: LOAD_TXD loads from disk instead of img (as posed to streaming I believe).
extern _native TextureDict LOAD_TXD(const char* txdName)l
extern _native uint ADD_SPHERE(float x, float y, float z, float radius, int *sphere)l

//INFO: If the checkpoint is a chevron then it will point towards point_at. If it's a flag then point_at will be ignored. 
//PURPOSE: Creates a race checkpoint.
extern _native uint CREATE_CHECKPOINT(int type, float x, float y, float z, float point_at_x, float point_at_y, float point_at_z, float size)l

//PURPOSE: Clears anys critp set timecycle modifier.
extern _native void CLEAR_TIMECYCLE_MODIFIER(void)l

//PURPOSE: Removes a checkpoint
extern _native void DELETE_CHECKPOINT(uint checkpoint)l
extern _native void DRAW_CHECKPOINT(float x, float y, float z, float radius, int r, int g, int b)l
extern _native void DRAW_CHECKPOINT_WITH_ALPHA(float x, float y, float z, float radius, int r, int g, int b, int a)l
extern _native void DRAW_COLOURED_CYLINDER(float x, float y, float z, float Unk712, float Unk713, int r, int g, int b, int a)l
extern _native void DRAW_CORONA(float x, float y, float z, float radius, any unused1, any unused2, int Unk716, int Unk717, int Unk718)l
extern _native void DRAW_CURVED_WINDOW(float x, float y, float w, float h, int alpha)l

//PARAM NOTES: RGB = integers between 0 and 255
//PURPOSE:  Adds a light with the a scene.
extern _native void DRAW_LIGHT_WITH_RANGE(float x, float y, float z, int r, int g, int b, float range, float intensity)l

//INFO: Before you can use this command you have to load the movie using SET_BINK_MOVIE. 
//PARAM NOTES: CentreX and CentreY define the centre point of the movie. between 0 (top left) and 1 (bottom right) 
//                          RGBA set between 0 and 255
//PURPOSE: Draws a movie to the screen.
extern _native void DRAW_MOVIE(float x, float y, float w, float h, float angle, int r, int g, int b, int a)l

//INFO: rects are drawn in order, with the laset rectangle  drawn on top. 
//PARAM NOTES: Centre x and y: ranges between 0 (top left) and 1 (bottom right) of the screen. drawn from the rect centre.
//                      RGBA set between 0 and 255
//PURPOSE: Renders a rectangle to the screen.
extern _native void DRAW_RECT(float x, float y, float w, float h, int r, int g, int b, int a)l
extern _native void DRAW_SPHERE(float x, float y, float z, float radius)l
extern _native void DRAW_SPRITE(uint texture, float x, float y, float w, float h, float rotation, int r, int g, int b, int a)l
extern _native void DRAW_SPRITE_WITH_FIXED_ROTATION(uint texture, float x, float y, float w, float h, float angle, int r, int g, int b, int a)l
extern _native void DRAW_SPRITE_WITH_UV(uint texture, float x, float y, float w, float h, float rotation, int r, int g, int b, int a)l
extern _native void DRAW_SPRITE_WITH_UV_COORDS(uint texture, float x, float y, float w, float h, float Unk756, float Unk757, float Unk758, float Unk759, float angle, int r, int g, int b, int a)l
extern _native void DRAW_WINDOW(float x, float y, float w, float h, const char* str, int alpha)l
extern _native void ENABLE_FOV_LOD_MULTIPLIER(bool enable)l

// PURPOSE: Sets the evolution data of a previously started looped particle effect
extern _native void EVOLVE_PTFX(PTFX_ID ptfx, const char* evolvetype, float val)l

//PURPOSE: Force the loading screen on/off regardless of fade value
extern _native void FORCE_LOADING_SCREEN(bool force)l

//PURPOSE: Turn noise overide on/off
extern _native void FORCE_NOISE_OFF(bool off)l

// PURPOSE: Returns the screen resolution
extern _native void GET_SCREEN_RESOLUTION(int *x, int *y)l

// PURPOSE: Returns the resolution of a texture
extern _native void GET_TEXTURE_RESOLUTION(uint texture, float *x, float *y)l

//PURPOSE:  Unloads a streamed testure
extern _native void MARK_STREAMED_TXD_AS_NO_LONGER_NEEDED(const char* txdName)l

//PURPOSE: Plays a loaded movie
extern _native void PLAY_MOVIE(void)l

//INFO: This command should be called when you are finished with a movie. It frees up the memory allocated by SET_MOVIE.
//PURPOSE: Frees the memory allocated to the movie.
extern _native void RELEASE_MOVIE(void)l
extern _native void RELEASE_TEXTURE(Texture texture)l

// PURPOSE: Removes all projected textures within the range of a position
extern _native void REMOVE_PROJTEX_IN_RANGE(float x, float y, float z, float range)l

// PURPOSE: Remove a particle effects (including existing particles)
extern _native void REMOVE_PTFX(PTFX_ID ptfx)l

// PURPOSE: Remove all particle effects (including existing particles) from an object
extern _native void REMOVE_PTFX_FROM_OBJECT(OBJECT_INDEX obj)l

// PURPOSE: Remove all particle effects (including existing particles) from a ped
extern _native void REMOVE_PTFX_FROM_PED(PED_INDEX ped)l

// PURPOSE: Remove all particle effects (including existing particles) from a vehicle
extern _native void REMOVE_PTFX_FROM_VEHICLE(VEHICLE_INDEX veh)l
extern _native void REMOVE_SPHERE(uint sphere)l
extern _native void REMOVE_TXD(TextureDict txd)l

//INFO:  The .img file should contain platform-specific txds. 
//      After requesting the txd, you should check HAS_STREAMED_TXD_LOADED in the same way as you do for models and animations.
//      This command is a streamed version of LOAD_TXD. 
//PURPOSE: Requests a texture dictionary to be loaded from an .img file.
extern _native void REQUEST_STREAMED_TXD(const char* txdName, bool priority)l

//INFO: To display the movie, call PLAY_MOVIE every frame. Once you've finished with the movie, call RELEASE_MOVIE to free the memory.
//PURPOSE: Allocates memory and loads the specified movie file.
extern _native void SET_CURRENT_MOVIE(const char* filename)l
extern _native void SET_GLOBAL_RENDER_FLAGS(bool Unk507, bool Unk508, bool Unk509, bool Unk510)l

//INFO: Will only work with Graphics (ie rectangles/sprites etc) and not text. 
//PURPOSE:  Masks off an area on screen where no graphics will be drawn.
extern _native void SET_MASK(float x, float y, float w, float h)l

//PURPOSE: Sets the time of the current movie.
extern _native void SET_MOVIE_TIME(float time)l

//PURPOSE: Sets the volume of the movie audio
extern _native void SET_MOVIE_VOLUME(float volume)l
extern _native void SET_PED_FIRE_FX_LOD_SCALER(float scale)l

// PURPOSE: Tells the particle fx that the camera is inside a vehicle so that certain effects are stopped 
extern _native void SET_PTFX_CAM_INSIDE_VEHICLE(bool set)l
extern _native void SET_SPRITES_DRAW_BEFORE_FADE(bool set)l

//INFO:  A timecyclemodifier is set up by an artists and can contains changes to light colour, fog colour, clip plane distance or post fx. They can be used as a filter.
//          MAKE SURE TO CALL CLEAR_TIMECYCLE_MODIFIER when you're done. If this doesn't happen the game will from that point on use that filter. 
//PURPOSE: Forces the  timecycle modifier to be used for 100%.
extern _native void SET_TIMECYCLE_MODIFIER(const char* name)l

//PURPOSE: Stops a movie playing
extern _native void STOP_MOVIE(void)l

//INFO: The stopped effect will automatically clean itself up once stopped. 
//PARAM NOTES: PTFX_ID ptfx - the id of the started particle effect to stop 
// PURPOSE: Stop a previously started looped particle effec
extern _native void STOP_PTFX(PTFX_ID ptfx)l

//INFO: If the particle effect is attached to an entity (ped, vehicle or object) then the new position is an offset from that entity.
//  If the particle effect is not attached to an entity (ped, vehicle or object) then the new position is a world position. 
//PARAM NOTES: PTFX_ID ptfx - the id of the started particle effect to update
//  VECTOR position - the new position of the particle effect
//  VECTOR rotation - the offset rotation of the particle effect 
// PURPOSE: Update the position and/or rotation offsets of a previously started looped particle effect.
extern _native void UPDATE_PTFX_OFFSETS(PTFX_ID ptfx, float x, float y, float z, float rot_x, float rot_y, float rot_z)l

// PURPOSE: Sets the colour tint of a previously started looped particle effect
extern _native void UPDATE_PTFX_TINT(PTFX_ID ptfx, float r, float g, float b, float a)l

//PURPOSE: Sets a mask active.
extern _native void USE_MASK(bool use)l

// PURPOSE: Washes the decals off a vehicle (by intensity each time it's called)
extern _native void WASH_VEHICLE_TEXTURES(VEHICLE_INDEX vehicle, int intensity)l

//Deprecated
//LINE
//DRAW_DEBUG_SPHERE
//CREATE_USER_3D_MARKER
//REMOVE_USER_3D_MARKER
//SET_SPRITE_HDR_MULTIPLIER
//ENABLE_SHADOWS
//ENABLE_FANCY_WATER
//ENABLE_DEFERRED_LIGHTING
#pragma endregion

#pragma region WATER
//INFO: This command takes the waves into account so the result may be different depending on the exact frame of calling. 
//PURPOSE: Gets the height of the water below the position including the waves.
extern _native bool GET_WATER_HEIGHT(float x, float y, float z, float *pheight)l

//INFO: This command does not take the waves into account so the result will be the same between different frames
//PURPOSE: Gets the height of the water below the position excluding the waves.
extern _native bool GET_WATER_HEIGHT_NO_WAVES(float x, float y, float z, float *height)l

//INFO: This command will returns false most of the time and true during one frame every 4 seconds or so. 
//	This always happens at the same time in the cycle that the waves follow. 
//	Make sure to wait for this command to return true before starting to record a recording and before starting to play it back. 
//	This way the recording should be in synch with the waves when played back.
//	You should probably also make sure the weather is simular as this influences the choppyness of the waves. 
//PURPOSE: Syncs the recording with the waves.
extern _native bool SYNCH_RECORDING_WITH_WATER(void)l
#pragma endregion

#pragma region CARBOMB
extern _native bool GET_IS_DEPOSIT_ANIM_RUNNING(void)l
extern _native bool GET_PLAYER_TO_PLACE_BOMB_IN_CAR(VEHICLE_INDEX vehicle)l
extern _native bool GET_TASK_PLACE_CAR_BOMB_UNSUCCESSFUL(void)l
extern _native bool IS_PLACE_CAR_BOMB_ACTIVE(void)l
#pragma endregion

#pragma region BRAINS
//PURPOSE: Checks that script brain is with a specific range of the player.
extern _native bool IS_OBJECT_WITHIN_BRAIN_ACTIVATION_RANGE(OBJECT_INDEX obj)l

//INFO: You should call this command within all brain scripts attached to world points.
//PURPOSE: Checks if the world point is out of activation range of the player called within the brain script.
extern _native bool IS_WORLD_POINT_WITHIN_BRAIN_ACTIVATION_RANGE(void)l

//PARAM NOTES: Pass in -1 to ignore the object grouping.
//PURPOSE: Associates a script brain with a certain object model.
extern _native void ALLOCATE_SCRIPT_TO_OBJECT(const char* ScriptName, uint model, int percentage_chance, float activation_range, int object_grouping_id)l

//PARAM NOTES: percentage_chance should be between 0 and 100. It determines the chance of a new ped with the model getting a brain. 
//							Set scenario_peds_only to TRUE if you only want the brain to be given to peds created as scenario peds. 
//PURPOSE: Associates a script brain with a certain ped model.
extern _native void ALLOCATE_SCRIPT_TO_RANDOM_PED(const char* ScriptName, uint model, int percentage_chance, bool scenario_peds_only)l

//INFO: This command gives a label to a brain script. The label is used by the artist when placing a 2d effect to launch the brain script. 
//			The SCRIPT line in the brain script should read a structure called coords_struct (defined in brains.sch) in the same way as an object brain script expects an OBJECT_INDEX. 
//			This structure currently contains up to 3 sets of coordinates. It always has at least one coord which gives the world position around which the brain is centred.
//			For simplicity BrainLabel has been removed so that the name the artits give the 2deffect needs to match the filename of the script. 
//PURPOSE: Registers a point in a world script point.
extern _native void REGISTER_WORLD_POINT_SCRIPT_BRAIN(const char* ScriptName, float activation_range)l
extern _native void SWITCH_OBJECT_BRAINS(int brain, bool switchstate)l
#pragma endregion

#pragma region NETWORK
extern _native bool CAN_REGISTER_MISSION_ENTITIES(int ped_count, int veh_count, int obj_count)l
extern _native bool CAN_REGISTER_MISSION_OBJECT(void)l
extern _native bool CAN_REGISTER_MISSION_PED(void)l
extern _native bool CAN_REGISTER_MISSION_VEHICLE(void)l
extern _native bool CLAN_IS_PENDING(void)l
extern _native bool DOES_GAME_CODE_WANT_TO_LEAVE_NETWORK_SESSION(void)l

//PURPOSE: Returns true if an object with the specified network ID exists on the local machine. 
extern _native bool DOES_OBJECT_EXIST_WITH_NETWORK_ID(NETWORK_INDEX netid)l

//PURPOSE: Returns true if a ped with the specified network ID exists on the local machine. 
extern _native bool DOES_PED_EXIST_WITH_NETWORK_ID(NETWORK_INDEX netid)l
extern _native bool DOES_PLAYER_HAVE_CONTROL_OF_NETWORK_ID(PLAYER_INDEX player, NETWORK_INDEX id)l

//PURPOSE: Returns true if a vehicle with the specified network ID exists on the local machine. 
extern _native bool DOES_VEHICLE_EXIST_WITH_NETWORK_ID(NETWORK_INDEX netid)l

//PURPOSE: Returns true if the local machine has ownership of the object with the specified network ID (included pending ownership change check)
extern _native bool HAS_CONTROL_OF_NETWORK_ID(NETWORK_INDEX netid)l
extern _native bool HAS_NET_ID_BEEN_CLONED(NETWORK_INDEX id)l
extern _native bool HAS_NETWORK_PLAYER_LEFT_GAME(PLAYER_INDEX playerIndex)l

//INFO: Once the function has returned true, subsequent calls will return false. 
//PURPOSE: Returns a true if the specified network player has collected the specified pick up.
extern _native bool HAS_PLAYER_COLLECTED_PICKUP(PLAYER_INDEX playerIndex, PICKUP_INDEX pikcup)l
extern _native bool IS_CHAR_ON_PLAYER_MACHINE(PED_INDEX ped, PLAYER_INDEX player)l
extern _native bool IS_DAMAGE_TRACKER_ACTIVE_ON_NETWORK_ID(NETWORK_INDEX id)l
extern _native bool IS_IN_MP_TUTORIAL(void)l
extern _native bool IS_IN_SPECTATOR_MODE(void)l
extern _native bool IS_LCPD_DATA_VALID(void)l
extern _native bool IS_NETWORK_CONNECTED(void)l
extern _native bool IS_NETWORK_GAME_PENDING(void)l
extern _native bool IS_NETWORK_GAME_RUNNING(void)l

//PURPOSE: Returns true if the given player is active.
extern _native bool IS_NETWORK_PLAYER_ACTIVE(PLAYER_INDEX playerIndex)l

//PURPOSE:  Returns true if the given player is visible on screen, and not occluded. Can be called every frame.
extern _native bool IS_NETWORK_PLAYER_VISIBLE(PLAYER_INDEX player)l
extern _native bool IS_NETWORK_SESSION(void)l
extern _native bool IS_OBJECT_ON_PLAYER_MACHINE(OBJECT_INDEX obj, PLAYER_INDEX player)l
extern _native bool IS_OBJECT_REASSIGNMENT_IN_PROGRESS(void)l
extern _native bool IS_SPHERE_VISIBLE_TO_ANOTHER_MACHINE(float x, float y, float z, float radius)l
extern _native bool IS_THIS_MACHINE_THE_SERVER(void)l
extern _native bool IS_VEHICLE_ON_PLAYER_MACHINE(VEHICLE_INDEX veh, PLAYER_INDEX player)l
extern _native bool LCPD_FIRST_TIME(void)l
extern _native bool LCPD_HAS_BEEN_CONFIGURED(void)l
extern _native bool LOCAL_PLAYER_IS_READY_TO_START_PLAYING(void)l

//PURPOSE: Accepts the unaccepted invite at the given index
extern _native bool NETWORK_ACCEPT_INVITE(int invite_index)l
extern _native bool NETWORK_ADVERTISE_SESSION(bool advertise)l
extern _native bool NETWORK_ALL_PARTY_MEMBERS_PRESENT(void)l
extern _native bool NETWORK_AM_I_BLOCKED_BY_PLAYER(PLAYER_INDEX playerIndex)l
extern _native bool NETWORK_AM_I_MUTED_BY_PLAYER(PLAYER_INDEX playerIndex)l
//extern _native bool NETWORK_CHANGE_GAME_MODE_E1(int gamemode, int slots, int episode, int maxteams, int autoaim, int cops, int friendlyfire, int unknown0)l
extern _native bool NETWORK_CHANGE_GAME_MODE_E1(int gamemode, int max_slots, int max_private_slots, int maxteams, int autoaim, int cops, int friendlyfire, int unknown0)l
extern _native bool NETWORK_CHECK_INVITE_ARRIVAL(void)l
extern _native bool NETWORK_DID_INVITE_FRIEND(const char* FRIENDNAME)l
extern _native bool NETWORK_END_SESSION_PENDING(void)l
extern _native bool NETWORK_FIND_GAME_PENDING(void)l
extern _native bool NETWORK_GET_CURRENT_GAME_CONFIG_E1(uint* config)l
extern _native bool NETWORK_GET_FRIENDLY_FIRE_OPTION(void)l
extern _native bool NETWORK_GET_HEALTH_RETICULE_OPTION(void)l
extern _native bool NETWORK_GET_TEAM_OPTION(void)l
extern _native bool NETWORK_HAVE_ACCEPTED_INVITE(void)l

//PURPOSE: Returns true if the local player has online privileges.
extern _native bool NETWORK_HAVE_ONLINE_PRIVILEGES(void)l
extern _native bool NETWORK_HAVE_SUMMONS(void)l
extern _native bool NETWORK_HOST_GAME_E1(int unk0, bool unk1, int unk2, int unk3, int unk4, int unk5, int unk6, int unk7, int unk8, int unk9)l
extern _native bool NETWORK_HOST_GAME_CNC(int unk0, int unk1)l
extern _native bool NETWORK_HOST_GAME_PENDING(void)l
extern _native bool NETWORK_HOST_GAME_SUCCEEDED(void)l
extern _native bool NETWORK_HOST_RENDEZVOUS_E1(int unk0, int unk1, int unk2, int unk3, int unk4, int unk5, int unk6)l
extern _native bool NETWORK_INVITE_FRIEND(const char* friendname, const char* inviteMessage)l
extern _native bool NETWORK_IS_BEING_KICKED(void)l
extern _native bool NETWORK_IS_COMMON_EPISODE(bool unk)l //i suspect this takes episode and checks if we have the episode present (Complete edition) - Nath

//PURPOSE: Returns true if the friend is playing the same game (e.g. GTAIV).
extern _native bool NETWORK_IS_FRIEND_IN_SAME_TITLE(const char* friend_name)l
extern _native bool NETWORK_IS_FRIEND_ONLINE(const char* friend_name)l
extern _native bool NETWORK_IS_GAME_RANKED(void)l
extern _native bool NETWORK_IS_INVITEE_ONLINE(void)l
extern _native bool NETWORK_IS_LINK_CONNECTED(void)l
extern _native bool NETWORK_IS_OPERATION_PENDING(void)l
extern _native bool NETWORK_IS_PLAYER_BLOCKED_BY_ME(PLAYER_INDEX playerIndex)l

//PURPOSE: Returns true if the given player is currently chatting.
extern _native bool NETWORK_IS_PLAYER_TALKING(PLAYER_INDEX playerIndex)l
extern _native bool NETWORK_IS_PLAYER_MUTED_BY_ME(PLAYER_INDEX playerIndex)l
extern _native bool NETWORK_IS_RENDEZVOUS(void)l
extern _native bool NETWORK_IS_RENDEZVOUS_HOST(void)l
extern _native bool NETWORK_IS_SESSION_ADVERTISED(void)l

//PURPOSE: Returns TRUE if players can be invited to this session
extern _native bool NETWORK_IS_SESSION_INVITABLE(void)l

//PURPOSE: Returns TRUE when the session has been started
extern _native bool NETWORK_IS_SESSION_STARTED(void)l
extern _native bool NETWORK_IS_TVT(void)l
extern _native bool NETWORK_JOIN_GAME(int index)l
extern _native bool NETWORK_JOIN_GAME_CNC(int index, int unk0, int unk1)l
extern _native bool NETWORK_JOIN_GAME_PENDING(void)l
extern _native bool NETWORK_JOIN_GAME_SUCCEEDED(void)l
extern _native bool NETWORK_JOIN_SUMMONS(void)l
extern _native bool NETWORK_LEAVE_GAME(void)l
extern _native bool NETWORK_LEAVE_GAME_PENDING(void)l
extern _native bool NETWORK_PLAYER_HAS_DIED_RECENTLY(PLAYER_INDEX playerIndex)l

//PURPOSE: Returns true if the given player has a headset connected.
extern _native bool NETWORK_PLAYER_HAS_HEADSET(PLAYER_INDEX player)l
extern _native bool NETWORK_RESTORE_GAME_CONFIG_E1(uint* config)l
extern _native bool NETWORK_RETURN_TO_RENDEZVOUS(void)l
extern _native bool NETWORK_SET_GAME_OPTIONS(int unk0, int unk1, int unk2, int unk3)l
extern _native bool NETWORK_SET_GAME_OPTIONS_PENDING(void)l
extern _native bool NETWORK_SET_GAME_OPTIONS_SUCCEEDED(void)l

//PURPOSE: Mutes / unmutes the remote player.
extern _native bool NETWORK_SET_PLAYER_MUTED(PLAYER_INDEX playerIndex, bool value)l
extern _native bool NETWORK_SET_SESSION_INVITABLE(bool invitable)l
extern _native bool NETWORK_START_SESSION_PENDING(void)l
extern _native bool NETWORK_START_SESSION_SUCCEEDED(void)l
extern _native bool NETWORK_STORE_SINGLE_PLAYER_GAME(void)l
extern _native bool PLAYER_WANTS_TO_JOIN_NETWORK_GAME(PLAYER_INDEX player)l

//PURPOSE: Request ownership of the object with the specified network ID from the current owner 
extern _native bool REQUEST_CONTROL_OF_NETWORK_ID(NETWORK_INDEX netid)l
extern _native bool RESTORE_SCRIPT_VALUES_FOR_NETWORK_GAME(uint* values)l
extern _native const char* NETWORK_GET_FRIEND_NAME(int friend_index)l

//PURPOSE: Given an index into the player history, returns the name of the player.
extern _native const char* NETWORK_GET_MET_PLAYER_NAME(int player_met_index)l

//PURPOSE: Returns the name of the inviting player at the given index.
extern _native const char* NETWORK_GET_UNACCEPTED_INVITER_NAME(int invite_index)l
extern _native int GET_EPISODE_INDEX_FROM_SUMMONS(void)l

//PURPOSE: Return how many times the local player has kills a ped with the specified ped model with the specified weapon (or any weapon if WEAPONTYPE_INVALID is specified)
extern _native int GET_KILL_TRACKING_RESULTS(uint model_hash, WEAPON_TYPE weapon)l
extern _native int GET_LAST_TIME_NETWORK_ID_DAMAGED(NETWORK_INDEX id)l
extern _native int GET_LCPD_COP_SCORE(void)l
extern _native int GET_LCPD_CRIMINAL_SCORE(void)l
extern _native int GET_LOCAL_GAMERLEVEL_FROM_PROFILESETTINGS(void)l

//PURPOSE: Return how many times the local player has killed a ped using the specified ped model, all kills of peds with models not registered are tracked separately and returned as a combined total
extern _native int GET_NUM_KILLS_FOR_RANK_POINTS(uint model_hash)l
extern _native int GET_ONLINE_SCORE(int score_id)l
extern _native int GET_PLAYER_LCPD_SCORE(PLAYER_INDEX player)l
extern _native int GET_PLAYER_RANK_LEVEL_DURING_MP(PLAYER_INDEX playerIndex)l
extern _native int GET_TEAM_COLOUR(int team)l

//PURPOSE: Returns the time (in milliseconds) that a specific player has been dead for. 
extern _native int HOW_LONG_HAS_NETWORK_PLAYER_BEEN_DEAD_FOR(PLAYER_INDEX playerIndex)l
extern _native int NETWORK_GET_FRIEND_COUNT(void)l

//PURPOSE: Returns the game mode.
extern _native int NETWORK_GET_GAME_MODE(void)l
extern _native int NETWORK_GET_MAX_PRIVATE_SLOTS(void)l
extern _native int NETWORK_GET_MAX_SLOTS(void)l
extern _native int NETWORK_GET_NUM_PARTY_MEMBERS(void)l

//INFO: Players met are not necessarily still in the session.
//The player history contains the 100 most recently met players.
//PURPOSE: Returns the number of players met during the current session.
extern _native int NETWORK_GET_NUM_PLAYERS_MET(void)l

//PURPOSE: Returns the number of unaccepted invites we have in the queue.
extern _native int NETWORK_GET_NUM_UNACCEPTED_INVITES(void)l
extern _native int NETWORK_GET_NUM_UNFILLED_RESERVATIONS(void)l
extern _native int NETWORK_GET_NUMBER_OF_GAMES(uint gamemode)l
extern _native int NETWORK_GET_UNACCEPTED_INVITE_GAME_MODE(int invite_index)l
extern _native int NETWORK_GET_UNACCEPTED_INVITE_EPISODE(int invite_index)l
extern _native int READ_LOBBY_PREFERENCE(int preferenceId)l
extern _native int STORE_DAMAGE_TRACKER_FOR_NETWORK_PLAYER(PLAYER_INDEX playerIndex, int ukn57, NETWORK_INDEX id)l

//INFO: -1 means the killer is unknown
//PURPOSE: Returns the player that has killed the other player last.
extern _native PLAYER_INDEX FIND_NETWORK_KILLER_OF_PLAYER(PLAYER_INDEX playerIndex)l

//INFO: This command can be used for any physical objects withinthe game (e.g. cars, peds and objects). 
//PURPOSE: Returns the player index of the player that destroyed the network object with the specified network ID.
extern _native PLAYER_INDEX GET_DESTROYER_OF_NETWORK_ID(NETWORK_INDEX id, WEAPON_TYPE* weap)l

//INFO: Returns -1 while the host is changing.. GET_PLAYER_NAME(-1) returns **Invalid** on XBOX, on PS3 you will crash. 
extern _native PLAYER_INDEX GET_HOST_ID(void)l

extern _native PLAYER_INDEX NETWORK_GET_RENDEZVOUS_HOST_PLAYER_ID(void)l
extern _native void ACTIVATE_DAMAGE_TRACKER_ON_NETWORK_ID(NETWORK_INDEX id, bool activate)l
extern _native void ADD_NETWORK_RESTART(float x, float y, float z, float radius, int unk)l
extern _native void CLEAR_SCRIPT_ARRAY_FROM_SCRATCHPAD(int scratch_pad_index)l
extern _native void DISPLAY_PLAYER_ICONS(bool display)l
extern _native void DISPLAY_PLAYER_NAMES(bool display)l
extern _native void FIND_NETWORK_RESTART_POINT(vector3* pos, float* radius, int Unk913)l

//PURPOSE: Returns the network ID of the object with the specified index 
extern _native void GET_NETWORK_ID_FROM_OBJECT(OBJECT_INDEX obj, NETWORK_INDEX *netid)l

//PURPOSE: Returns the network ID of the ped with the specified index 
extern _native void GET_NETWORK_ID_FROM_PED(PED_INDEX ped, NETWORK_INDEX *netid)l

//PURPOSE: Returns the network ID of the vehicle with the specified index 
extern _native void GET_NETWORK_ID_FROM_VEHICLE(VEHICLE_INDEX vehicle, NETWORK_INDEX *netid)l
extern _native void GET_NETWORK_TIMER(uint* timer)l

//PURPOSE: Returns the object index of the entity with the specified network ID.
extern _native void GET_OBJECT_FROM_NETWORK_ID(NETWORK_INDEX netid, OBJECT_INDEX *obj)l

//PURPOSE: Returns the ped index of the entity with the specified network ID.
extern _native void GET_PED_FROM_NETWORK_ID(NETWORK_INDEX netid, PED_INDEX* ped)l
extern _native void GET_SAFE_LOCAL_RESTART_COORDS(float x, float y, float z, float* px, float* py, float* pz)l
extern _native void GET_TEAM_RGB_COLOUR(int team, int *R, int *G, int *B)l

//PURPOSE: Returns the vehicle index of the entity with the specified network ID.
extern _native void GET_VEHICLE_FROM_NETWORK_ID(NETWORK_INDEX netid, VEHICLE_INDEX* vehicle)l
extern _native void LAUNCH_LOCAL_PLAYER_IN_NETWORK_GAME(void)l
extern _native void NETWORK_CLEAR_INVITE_ARRIVAL(void)l
extern _native void NETWORK_CLEAR_SUMMONS(void)l
extern _native void NETWORK_END_SESSION(void)l
extern _native void NETWORK_FIND_GAME_CNC(void)l
extern _native void NETWORK_FIND_GAME_E1(uint gamemode, bool ranked, uint episodewenususestwo, uint suspectedmaxteamswenususesone, uint lobbyPref419, uint lobbyPref420, uint lobbyPref422, uint unk0)l

//PURPOSE: Kicks the given player from the session. Callable only by the host.
extern _native void NETWORK_KICK_PLAYER(PLAYER_INDEX playerIndex)l
extern _native void NETWORK_SET_FRIENDLY_FIRE_OPTION(bool set)l
extern _native void NETWORK_SET_HEALTH_RETICULE_OPTION(bool set)l

//INFO: Only chat to/from that player will be transmitted/received.
//PURPOSE: Sets the voice chat focus on the given player.
extern _native void NETWORK_SET_TALKER_FOCUS(PLAYER_INDEX player)l

//PARAM NOTES: 0.0 implies no threshold and we can chat regardless of distance
//PURPOSE: Sets the distance threshold that determines how close a player must be before we can chat with him. This does
//		   not account for height.
extern _native void NETWORK_SET_TALKER_PROXIMITY(float distance)l

//INFO: Team-only chat is disabled by default.
//PURPOSE: Enables/disables team-only chat.  Applies only to in-game chat, not lobby chat.
extern _native void NETWORK_SET_TEAM_ONLY_CHAT(bool set)l

//INFO: Show the Platform profile UI represented by this friend name
extern _native void NETWORK_SHOW_FRIEND_PROFILE_UI(const char* friend_name)l

//INFO: Show the Platform feedback UI represented by this player met index
extern _native void NETWORK_SHOW_MET_PLAYER_FEEDBACK_UI(int metPlayerIndex)l

//INFO: Show the Platform profile UI represented by this player met index
extern _native void NETWORK_SHOW_MET_PLAYER_PROFILE_UI(int player_met_index)l

//INFO: Show the Platform feedback UI represented by this player index
extern _native void NETWORK_SHOW_PLAYER_FEEDBACK_UI(PLAYER_INDEX payerIndex)l

//INFO: Show the Platform profile UI represented by this player index
extern _native void NETWORK_SHOW_PLAYER_PROFILE_UI(PLAYER_INDEX playerIndex)l
extern _native void NETWORK_START_SESSION(void)l
extern _native void NETWORK_STORE_GAME_CONFIG_E1(uint* config)l

//PURPOSE: registers the struct you give it for syncing over the network.
//INFO: id must be unique to your broadcast
//each player can write to their own index in the array, and read others.
//PARAM NOTES: count - sizeof the struct you sync divided by 4
//  vars - must have m_Count at the top then myStruct m_Data[MAX_NETWORK_PLAYERS] beneath it, m_Count again is sizeof(thatStruct) / 4
//  count - probably okay to just use the m_Count as it'll be the same.
extern _native void REGISTER_CLIENT_BROADCAST_VARIABLES(uint* vars, int count, int id)l

//PURPOSE: registers the struct that the host alters and that the clients need to be updated on 
//INFO: id must be unique to your broadcast
//only host can write, players can read
//PARAM NOTES: count - sizeof the struct you sync divided by 4
//  vars - must have m_Count at the top then myStruct data beneath it, m_Count again is sizeof(thatStruct) / 4
//  count - probably okay to just use the m_Count as it'll be the same.
extern _native void REGISTER_HOST_BROADCAST_VARIABLES(uint* vars, int count, int id)l

//PARAM NOTES: killer - index of the player that was the killer.
//  victim - index of the player that died.
//  weapon - index of the weapon that the killer used.
//PURPOSE: When a player kills another player this must be called to register the death.
extern _native void REGISTER_KILL_IN_MULTIPLAYER_GAME(PLAYER_INDEX killer, PLAYER_INDEX victim, WEAPON_TYPE weapon)l

//PURPOSE: Registers a ped model for tracking when a ped using it is killed for the purposes of assigning rank points
extern _native void REGISTER_MODEL_FOR_RANK_POINTS(uint model_hash)l
extern _native void REMOVE_ALL_NETWORK_RESTART_POINTS(void)l

//PURPOSE: Reserves space in the population pool for the given number of objects. This space will also be reserved in the population pools of nearby players not running this script
extern _native void RESERVE_NETWORK_MISSION_OBJECTS(int count)l
extern _native void RESERVE_NETWORK_MISSION_OBJECTS_FOR_HOST(int count)l

//PURPOSE: Reserves space in the population pool for the given number of peds. This space will also be reserved in the population pools of nearby players not running this script.
extern _native void RESERVE_NETWORK_MISSION_PEDS(int count)l
extern _native void RESERVE_NETWORK_MISSION_PEDS_FOR_HOST(int count)l

//PURPOSE: Reserves space in the population pool for the given number of vehicles. This space will also be reserved in the population pools of nearby players not running this script.
extern _native void RESERVE_NETWORK_MISSION_VEHICLES(int count)l
extern _native void RESERVE_NETWORK_MISSION_VEHICLES_FOR_HOST(int count)l
extern _native void RESTORE_SCRIPT_ARRAY_FROM_SCRATCHPAD(uint* values, int count, int script_id, int scratch_pad_index)l
extern _native void RESURRECT_NETWORK_PLAYER(PLAYER_INDEX playerIndex, float x, float y, float z, float rotation)l
extern _native void SAVE_SCRIPT_ARRAY_IN_SCRATCHPAD(uint* values, int count, int script_id, int scratch_pad_index)l
extern _native void SEND_CLIENT_BROADCAST_VARIABLES_NOW(int id)l
extern _native void SEND_HOST_BROADCAST_VARIABLES_NOW(int id)l
extern _native void SET_CAR_EXISTS_ON_ALL_MACHINES(VEHICLE_INDEX vehicle, bool exists)l
extern _native void SET_CAR_STOP_CLONING(VEHICLE_INDEX veh, bool stop)l
extern _native void SET_DISPLAY_PLAYER_NAME_AND_ICON(PLAYER_INDEX playerIndex, bool set)l
extern _native void SET_IGNORE_SERVER_UPDATE(NETWORK_INDEX id, bool ignore)l
extern _native void SET_IK_DISABLED_FOR_NETWORK_PLAYER(PLAYER_INDEX playerIndex, bool set)l
extern _native void SET_IN_MP_TUTORIAL(bool set)l

//PURPOSE: Set the local in spectator Mode.
extern _native void SET_IN_SPECTATOR_MODE(bool spectate)l
extern _native void SET_INVINCIBILITY_TIMER_DURATION(int duration)l
extern _native void SET_LCPD_COP_SCORE(int score)l
extern _native void SET_LCPD_CRIMINAL_SCORE(int score)l

//INFO: This message is for information purposes and the player has to press button A to continue 
//  the load of the single player game.
//PURPOSE: Sets a message that will be displayed during the loading screen.
extern _native void SET_MSG_FOR_LOADING_SCREEN(const char* label)l
extern _native void SET_NETWORK_ID_CAN_MIGRATE(NETWORK_INDEX netid, bool can_migrate)l

//PURPOSE: Sets whether a script object will be created on all machines running this script.
extern _native void SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NETWORK_INDEX netID, bool set)l
extern _native void SET_NETWORK_ID_STOP_CLONING(NETWORK_INDEX id, bool stop)l
extern _native void SET_NETWORK_ID_STOP_CLONING_FOR_ENEMIES(NETWORK_INDEX id, bool stop)l
extern _native void SET_NETWORK_PED_USING_PARACHUTE(NETWORK_INDEX id)l
extern _native void SET_NETWORK_PLAYER_AS_VIP(PLAYER_INDEX playerIndex, bool set)l
extern _native void SET_NETWORK_PLAYER_CAN_RUN_IN_INTERIORS(void)l
extern _native void SET_NETWORK_VEHICLE_RESPOT_TIMER(NETWORK_INDEX id, int timer)l
extern _native void SET_NETWORK_VISIBILITY(NETWORK_INDEX id, bool set)l
extern _native void SET_OBJECT_EXISTS_ON_ALL_MACHINES(OBJECT_INDEX obj, bool exists)l
extern _native void SET_OBJECT_STOP_CLONING(OBJECT_INDEX obj, bool stop)l
extern _native void SET_ONLINE_SCORE(int score_id, int value)l
extern _native void SET_PED_EXISTS_ON_ALL_MACHINES(PED_INDEX ped, bool exists)l
extern _native void SET_PED_STOP_CLONING(PED_INDEX ped, bool stop)l
extern _native void SET_PLAYER_AS_DAMAGED_PLAYER(PLAYER_INDEX playerIndex, PLAYER_INDEX player2, bool damaged)l

// PURPOSE:
//   Sets a rich presence in xbox
// PARAMS:
//    id       - rich presence id.
//   data       - struct with all presence fields.
//   sizeOfData - sizeof Struct.
//   numFields  - number of fields in the struct.
// NOTES:
//   ALL FIELDS HAVE TO BE INTS
// EXAMPLE:
//
//   Declare a structure that represents a rich presence:
//   STRUCT STRUCT_INVITE_EVENT
//   	INT field_0
//   	INT field_1
//   ENDSTRUCT
//
//   Use the struct as you which:
//   STRUCT_INVITE_EVENT presence_0
//   presence_0.field_0 = 10
//   presence_0.field_1 = 11
//
//   Set the rich presence:
//   SET_RICH_PRESENCE(0, presence_0, sizeof(presence_0), 2)
// 
//   On 360 you set only the presence id and the values for the fields in 
//   case the presence has fields, for instance, setting the presence Playing Mission A 
//   is set by specifying the presence id and the index of the mission A.
extern _native void SET_RICH_PRESENCE(int Unk971, int Unk972, int Unk973, int Unk974, int Unk975)l
extern _native void SET_RICH_PRESENCE_TEMPLATELOBBY(int Unk976)l
extern _native void SET_RICH_PRESENCE_TEMPLATEMP1(int Unk977, int Unk978, int Unk979, int Unk980)l
extern _native void SET_RICH_PRESENCE_TEMPLATEMP2(int Unk981)l
extern _native void SET_RICH_PRESENCE_TEMPLATEMP3(int Unk982, int Unk983)l
extern _native void SET_RICH_PRESENCE_TEMPLATEMP4(int Unk984, int Unk985)l
extern _native void SET_RICH_PRESENCE_TEMPLATEMP5(int Unk986, int Unk987, int Unk988)l
extern _native void SET_RICH_PRESENCE_TEMPLATEMP6(int Unk989, int Unk990, int Unk991)l
extern _native void SET_RICH_PRESENCE_TEMPLATEPARTY(void)l
extern _native void SET_RICH_PRESENCE_TEMPLATESP1(int Unk992, int Unk993, int Unk994)l
extern _native void SET_RICH_PRESENCE_TEMPLATESP2(int Unk995)l
extern _native void SET_SYNC_WEATHER_AND_GAME_TIME(bool sync)l
extern _native void SET_TEAM_COLOUR(int team, int color)l
extern _native void SET_THIS_MACHINE_RUNNING_SERVER_SCRIPT(bool host)l

//PURPOSE: Start tracking how many times the local player kills the specified ped model with the specified weapon (or any weapon if WEAPONTYPE_INVALID is specified)
extern _native void START_KILL_TRACKING(uint model_hash, WEAPON_TYPE weapon)l

//PURPOSE: Stop tracking how many times the local player kills the specified ped model with the specified weapon (or any weapon if WEAPONTYPE_INVALID is specified)
extern _native void STOP_KILL_TRACKING(uint model_hash, WEAPON_TYPE weapon)l
extern _native void SHUTDOWN_AND_LAUNCH_NETWORK_GAME(int episode)l
extern _native void SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME(void)l 
extern _native void STOP_SYNCING_SCRIPT_ANIMATIONS(bool stop)l
extern _native void STORE_SCRIPT_VALUES_FOR_NETWORK_GAME(uint* values)l
extern _native void TELEPORT_NETWORK_PLAYER(PLAYER_INDEX player, float x, float y, float z, float rot)l
extern _native void TELL_NET_PLAYER_TO_START_PLAYING(PLAYER_INDEX playerIndex, int team)l

// PURPOSE:  forces the player colour to be used for the radar and gametag instead of team colours
extern _native void USE_PLAYER_COLOUR_INSTEAD_OF_TEAM_COLOUR(bool use)l
extern _native void UPDATE_PLAYER_LCPD_SCORE(PLAYER_INDEX player, int score)l
extern _native void WRITE_LOBBY_PREFERENCE(int preferenceId, int preference)l

//Deprecated
//AWARD_NETWORK_POINTS
//IS_RUNNING_NETWORK_SCRIPT
//GET_CAMERA_FROM_NETWORK_ID
//DOES_CAMERA_EXIST_WITH_NETWORK_ID
//GIVE_CONTROL_OF_NETWORK_ID_TO_CLIENT
//RETURN_CONTROL_OF_NETWORK_ID_TO_SERVER
//NETWORK_FIND_GAME
//NETWORK_HOST_GAME
//NETWORK_HOST_RENDEZVOUS
//NETWORK_RETURN_TO_RENDEZVOUS_PENDING
//NETWORK_RETURN_TO_RENDEZVOUS_SUCCEEDED
//NETWORK_CHANGE_GAME_MODE
//NETWORK_CHANGE_GAME_MODE_PENDING
//NETWORK_CHANGE_GAME_MODE_SUCCEEDED
//NETWORK_STORE_GAME_CONFIG
//NETWORK_RESTORE_GAME_CONFIG
//SET_NETWORK_ID_STOP_SYNCING
//CLAN_IS_ONLINE
//CLAN_CREATE
//CLAN_LEAVE
//CLAN_SHOW_MEMBERS_UI
#pragma endregion

#pragma region DATETIME
extern _native int COMPARE_TWO_DATES(int date0_0, int date0_1, int date1_0, int date1_1)l
extern _native int GET_CURRENT_DAY_OF_WEEK(void)l
extern _native int GET_HOURS_OF_DAY(void)l
extern _native int GET_MINUTES_OF_DAY(void)l
extern _native int GET_MINUTES_TO_TIME_OF_DAY(int hour, int minute)l
extern _native void FORCE_TIME_OF_DAY(int hour, int minute)l
extern _native void FORWARD_TO_TIME_OF_DAY(int hour, int minute)l
extern _native void GET_CURRENT_DATE(int* day, int* month)l
extern _native void GET_TIME_OF_DAY(int *hour, int *minute)l
extern _native void RELEASE_TIME_OF_DAY(void)l
extern _native void SET_TIME_OF_DAY(int hour, int minute)l
extern _native void SET_TIME_OF_NEXT_APPOINTMENT(int time)l
extern _native void SET_TIME_ONE_DAY_BACK(void)l
extern _native void SET_TIME_ONE_DAY_FORWARD(void)l
#pragma endregion

#pragma region CUTSCENE
//PURPOSE: Checks that the cutscene has been streamed in.
extern _native bool HAS_CUTSCENE_LOADED(void)l

//PURPOSE: Checks if the cutscene has finished playing.
extern _native bool HAS_CUTSCENE_FINISHED(void)l
extern _native bool IS_PED_IN_CUTSCENE_BLOCKING_BOUNDS(PED_INDEX ped)l
extern _native bool IS_POS_IN_CUTSCENE_BLOCKING_BOUNDS(float x, float y, float z)l

//PURPOSE: Checks if the player has skipped the cutscene
extern _native bool WAS_CUTSCENE_SKIPPED(void)l

//PURPOSE: Gets the current cutscene anim section playing. 
extern _native int GET_CUTSCENE_SECTION_PLAYING(void)l

//PURPOSE: Gets the current time in milliseconds on the whole concat scene time line..
extern _native int GET_CUTSCENE_TIME(void)l

extern _native void CLEAR_CUTSCENE(void)l
extern _native void CLEAR_NAMED_CUTSCENE(const char* name)l
extern _native void GET_CUTSCENE_PED_POSITION(int unk, vector3 *pos)l
extern _native void INIT_CUTSCENE(const char* name)l
extern _native void SET_CUTSCENE_EXTRA_ROOM_POS(float x, float y, float z)l

//PURPOSE: Starts a streamed cut scene.
extern _native void START_CUTSCENE(void)l
extern _native void START_CUTSCENE_NOW(const char* name)l

//PURPOSE: Stops the cutscene that is currently playing and skips to the end of the scene.
extern _native void STOP_CUTSCENE(void)l

//Deprecated
//STREAM_CUTSCENE
#pragma endregion

#pragma region GANG
//Deprecated
//SET_CREATE_RANDOM_GANG_MEMBERS
//SET_ONLY_CREATE_RANDOM_GANG_MEMBERS
//SET_GANG_WEAPONS
#pragma endregion

#pragma region ZONE
//PURPOSE: Get name tag of zone that is displayed from coordinates
extern _native const char* GET_NAME_OF_INFO_ZONE(float x, float y, float z)l

//PURPOSE: Get name id of zone from coordinates
extern _native const char* GET_NAME_OF_ZONE(float x, float y, float z)l
extern _native int GET_CURRENT_POPULATION_ZONE_TYPE(void)l
extern _native int GET_CURRENT_ZONE_SCUMMINESS(void)l

//PURPOSE: Return what major map area coordinate is in
extern _native int GET_MAP_AREA_FROM_COORDS(float x, float y, float z)l
extern _native void ACTIVATE_SCRIPT_POPULATION_ZONE(void)l
extern _native void DEACTIVATE_SCRIPT_POPULATION_ZONE(void)l
extern _native void FIND_PRIMARY_POPULATION_ZONE_GROUP(int *groupparam0, int *groupparam1)l
extern _native void SET_ZONE_NO_COPS(const char* name, bool set)l
extern _native void SET_ZONE_POPULATION_TYPE(const char* zone, int poptype)l
extern _native void SET_ZONE_SCUMMINESS(const char* zone, int scumminess)l
extern _native void SPECIFY_SCRIPT_POPULATION_ZONE_AREA(int Unk848, int Unk849, int Unk850, int Unk851, int Unk852, int Unk853)l
extern _native void SPECIFY_SCRIPT_POPULATION_ZONE_GROUPS(int Unk854, int Unk855, int Unk856, int Unk857, int Unk858)l
extern _native void SPECIFY_SCRIPT_POPULATION_ZONE_PERCENTAGE_COPS(int percentage)l
extern _native void SPECIFY_SCRIPT_POPULATION_ZONE_NUM_CARS(int num)l
extern _native void SPECIFY_SCRIPT_POPULATION_ZONE_NUM_PARKED_CARS(int num)l
extern _native void SPECIFY_SCRIPT_POPULATION_ZONE_NUM_PEDS(int num)l
extern _native void SPECIFY_SCRIPT_POPULATION_ZONE_NUM_SCENARIO_PEDS(int num)l
#pragma endregion

#pragma region FIRE
extern _native bool DOES_SCRIPT_FIRE_EXIST(FIRE_INDEX fire)l

//PURPOSE: Checks if a ped is on fire.
extern _native bool IS_CHAR_ON_FIRE(PED_INDEX ped)l

//PURPOSE: Checks if an explosion is in a area.
extern _native bool IS_EXPLOSION_IN_AREA(EXPLOSION_TAG type, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)l

//PURPOSE:  Checks if an explosion is in a sphere area.
extern _native bool IS_EXPLOSION_IN_SPHERE(EXPLOSION_TAG type, float x, float y, float z, float radius)l

//PURPOSE: Checks if an object is on fire.
extern _native bool IS_OBJECT_ON_FIRE(OBJECT_INDEX obj)l
extern _native bool IS_SCRIPT_FIRE_EXTINGUISHED(FIRE_INDEX fire)l

//INFO: The char that the fire is created on must not be flame proof otherwise the game will assert. 
//PURPOSE:  Creates a fire on this ped.
extern _native FIRE_INDEX START_CHAR_FIRE(PED_INDEX ped)l

//INFO: The object that the fire is created on must not be flame proof otherwise the game will assert. 
//PURPOSE:  Creates a fire on this object.
extern _native FIRE_INDEX START_OBJECT_FIRE(OBJECT_INDEX obj)l

//INFO:  These will be ignored by the fire brigade. Only the player can extinguish a script-created fire. A script-created fire must be REMOVE_SCRIPT_FIRE once and only once. 
//PURPOSE: Creates a fire at the given coords.
extern _native FIRE_INDEX START_SCRIPT_FIRE(float x, float y, float z, int num_generations_allowed, any unused)l
extern _native int GET_NUMBER_OF_FIRES_IN_AREA(float x0, float y0, float z0, float x1, float y1, float z1)l

//PURPOSE: Gets the number of non script fires within radius metres of the given coord.
extern _native int GET_NUMBER_OF_FIRES_IN_RANGE(float x, float y, float z, float radius)l

//PARAM NOTES: EXPLOSION_TAG is a list of explosion tyrpe
//PURPOSE:  Creates an explosion at the co-ordinates. 
//					Size scale between 0.01 and 1.0.
extern _native void ADD_EXPLOSION(float x, float y, float z, EXPLOSION_TAG type, float scale, bool play_sound, bool no_visual, float cam_shake)l
extern _native void ADD_EXPLOSION_WITH_DIRECTION(float x, float y, float z, EXPLOSION_TAG type, float dir_x, float dir_y, float dir_z, float scale, bool play_sound, bool no_visual, float cam_shake)l

//PURPOSE: Stops a script fire on a ped.
extern _native void EXTINGUISH_CHAR_FIRE(PED_INDEX ped)l

//PURPOSE: Extinguishes fires within the specified radius
extern _native void EXTINGUISH_FIRE_AT_POINT(float x, float y, float z, float radius)l

//PURPOSE: Stops a script fire on an object.
extern _native void EXTINGUISH_OBJECT_FIRE(OBJECT_INDEX obj)l
extern _native void GET_SCRIPT_FIRE_COORDS(FIRE_INDEX fire, float *x, float *y, float *z)l

//PURPOSE: Removes a script fire, call only once per fire.
extern _native void REMOVE_SCRIPT_FIRE(FIRE_INDEX fire)l
extern _native void SET_MAX_FIRE_GENERATIONS(int max)l

//Deprecated
//REMOVE_ALL_SCRIPT_FIRES
//START_CAR_FIRE
//SET_SCRIPT_FIRE_AUDIO
//CLEAR_ALL_SCRIPT_FIRE_FLAGS
#pragma endregion

#pragma region WEAPON
//PURPOSE: Gets the current amount of ammo in a clip
extern _native bool GET_AMMO_IN_CLIP(PED_INDEX ped, WEAPON_TYPE weapon, int* pAmmo)l

//PURPOSE: Get the current weapon a character is using - returns if it is usable (i.e. in their hand)
extern _native bool GET_CURRENT_CHAR_WEAPON(PED_INDEX ped, WEAPON_TYPE* pWeapon)l
extern _native bool GET_IS_PROJECTILE_TYPE_IN_AREA(float x0, float y0, float z0, float x1, float y1, float z1, int type)l
extern _native bool GET_IS_STICKY_BOMB_STUCK_TO_OBJECT(OBJECT_INDEX obj)l
extern _native bool GET_IS_STICKY_BOMB_STUCK_TO_VEHICLE(VEHICLE_INDEX veh)l
extern _native bool GET_MAX_AMMO(PED_INDEX ped, WEAPON_TYPE weapon, int* pMaxAmmo)l

//PURPOSE: Return if a vehicle has been damaged by a certain weapon
extern _native bool HAS_CAR_BEEN_DAMAGED_BY_WEAPON(VEHICLE_INDEX vehicle, WEAPON_TYPE weapon)l

//PURPOSE: Return if a character has been damaged by a certain weapon
extern _native bool HAS_CHAR_BEEN_DAMAGED_BY_WEAPON(PED_INDEX ped, WEAPON_TYPE weapon)l

//PURPOSE: Return if character has a weapon
extern _native bool HAS_CHAR_GOT_WEAPON(PED_INDEX ped, WEAPON_TYPE weapon)l

//PURPOSE: Return if an object has been damaged by a certain weapon
extern _native bool HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON(OBJECT_INDEX obj, WEAPON_TYPE weap)l

//PURPOSE: Queries the peds armament using the flags passed
extern _native bool IS_CHAR_ARMED(PED_INDEX ped, int slot)l

//PURPOSE: Sets the amount of ammo in a clip.
extern _native bool SET_AMMO_IN_CLIP(PED_INDEX ped, WEAPON_TYPE weapon, int ammo)l
extern _native int GET_NUMBER_OF_ACTIVE_STICKY_BOMBS_OWNED_BY_PED(PED_INDEX ped)l
extern _native int GET_NUMBER_OF_STICKY_BOMBS_STUCK_TO_OBJECT(OBJECT_INDEX obj)l
extern _native int GET_NUMBER_OF_STICKY_BOMBS_STUCK_TO_VEHICLE(VEHICLE_INDEX veh)l

//PURPOSE: Give ammo for a weapon type to a character
extern _native void ADD_AMMO_TO_CHAR(PED_INDEX ped, WEAPON_TYPE weapon, int amount)l

//PURPOSE: Clear the damaged by last weapon information on a vehicle
extern _native void CLEAR_CAR_LAST_WEAPON_DAMAGE(VEHICLE_INDEX vehicle)l

//PURPOSE: Clear the damaged by last weapon information on a character
extern _native void CLEAR_CHAR_LAST_WEAPON_DAMAGE(PED_INDEX ped)l

//PURPOSE: Clear the damaged by last weapon information on an object
extern _native void CLEAR_OBJECT_LAST_WEAPON_DAMAGE(OBJECT_INDEX obj)l
extern _native void DISABLE_STICKY_BOMB_ACTIVE_SOUND(PED_INDEX ped, bool disable)l
extern _native void ENABLE_MAX_AMMO_CAP(bool enable)l
extern _native void FORCE_CHAR_TO_DROP_WEAPON(PED_INDEX ped)l

//PURPOSE: Return the amount of ammo a character has for a weapon
extern _native void GET_AMMO_IN_CHAR_WEAPON(PED_INDEX ped, WEAPON_TYPE weapon, int* pAmmo)l
extern _native void GET_CHAR_WEAPON_IN_SLOT(PED_INDEX ped, int slot, WEAPON_TYPE* pWeapon, int* pAmmo0, uint* model)l

//PURPOSE: Returns the maximum number of bullets allowed in one clip 
extern _native void GET_MAX_AMMO_IN_CLIP(any unused, WEAPON_TYPE weapon, int* pMaxAmmo)l

//PURPOSE: Return the model used by a weapon type.
extern _native void GET_WEAPONTYPE_MODEL(WEAPON_TYPE weapontype, uint *model)l

//PURPOSE: Return which slot this weapon type is placed in
extern _native void GET_WEAPONTYPE_SLOT(WEAPON_TYPE weapon, int *slot)l

//PURPOSE: Requests the weapon model and gives the weapon to a character with ammo once the model has loaded
extern _native void GIVE_DELAYED_WEAPON_TO_CHAR(PED_INDEX ped, WEAPON_TYPE weapon, int delaytime, bool force_into_hand)l

//PURPOSE: Give a weapon to a character with ammo
extern _native void GIVE_WEAPON_TO_CHAR(PED_INDEX ped, WEAPON_TYPE weapon, int ammo, bool force_into_hand)l

//PURPOSE: Hide a characters weapons while playing a scripted cutscene
extern _native void HIDE_CHAR_WEAPON_FOR_SCRIPTED_CUTSCENE(PED_INDEX ped, bool hide)l

//PURPOSE: Remove all weapons a character has
extern _native void REMOVE_ALL_CHAR_WEAPONS(PED_INDEX ped)l

//PURPOSE: Remove a weapon a character has
extern _native void REMOVE_WEAPON_FROM_CHAR(PED_INDEX ped, WEAPON_TYPE weapon)l
extern _native void SELECT_WEAPONS_FOR_VEHICLE(VEHICLE_INDEX veh, WEAPON_TYPE weapon)l

//PURPOSE: Set the ammo a character has for a certain weapon type
extern _native void SET_CHAR_AMMO(PED_INDEX ped, WEAPON_TYPE weapon, int ammo)l

//PURPOSE: Sets the visibility flag for the characters currently equipped weapon
extern _native void SET_CHAR_CURRENT_WEAPON_VISIBLE(PED_INDEX ped, bool visble)l

//PURPOSE: Sets whether a character drops his weapons as pickups when he dies
extern _native void SET_CHAR_DROPS_WEAPONS_WHEN_DEAD(PED_INDEX ped, bool set)l

//PURPOSE: Set the weapon a character is currently using
extern _native void SET_CURRENT_CHAR_WEAPON(PED_INDEX ped, WEAPON_TYPE w, bool force_in_hand)l
extern _native void SET_DEATH_WEAPONS_PERSIST(PED_INDEX ped, bool set)l
extern _native void SET_MIN_MAX_PED_ACCURACY(PED_INDEX ped, float min, float max)l

//Deprecated
//SET_CHAR_WEAPON_SKILL
#pragma endregion

#pragma region PATH
//INFO: Defined the scripted optional scripted region which will have navmesh loaded at all times
//PARAM NOTES: Keep 'radius' as small as possible, because all navmeshes intersecting this area will be loaded
//EXTRA NOTES: This can use up game memory very rapidly - use with caution!  Be sure to remove the area when no longer required.
//			   Only one region may be defined at a time, to avoid excessive memory usage
extern _native bool ADD_NAVMESH_REQUIRED_REGION(float x, float y, float radius)l

//INFO: This applies to the navmeshes which are always loaded in around the player, as well as any additional navmesh load regions which may have been set up. 
//PARAM NOTES: This could be used at the start of a mission after the player ped has been teleported, as it may take a little time before the surrounding navmehes are streamed in.
//	After the player has been teleported in, this command could be called repeatedly until it returns true - at which point you know it is ok to start creating peds & giving them navmesh routes, combat tasks,
//PURPOSE: returns whether all the requested navmeshes have loaded.
extern _native bool ARE_ALL_NAVMESH_REGIONS_LOADED(void)l

//INFO: The commands will return FALSE if no node could be found (probably due to the nodes not being streamed in). 
//PURPOSE: Will return if can the closest vehicle node to a coord.
extern _native bool GET_CLOSEST_CAR_NODE(float x, float y, float z, float* pResX, float* pResY, float* pResZ)l
extern _native bool GET_CLOSEST_CAR_NODE_FAVOUR_DIRECTION(float x, float y, float z, float* unk0, float* unk1, float* unk2, vector3* unk3, float* heading)l

//INFO: Can be used to set a car facing in a reasonable direction when it is placed on the car node. The command will return FALSE if no node could be found (probably due to the nodes not being streamed in). 
//PURPOSE: Will return if can the closest vehicle node with a heading, to a coord.
extern _native bool GET_CLOSEST_CAR_NODE_WITH_HEADING(float x, float y, float z, float* pResX, float* pResY, float* pResZ, float* pHeading)l

//INFO: This is needed for the prostitute script to find a quiet area to park the car. 
//PURPOSE: Will return if can the closest vehicle node even if switched off, to a coord.
extern _native bool GET_CLOSEST_MAJOR_CAR_NODE(float x, float y, float z, float* pResX, float* pResY, float* pResZ)l
extern _native bool GET_CLOSEST_NETWORK_RESTART_NODE(float x, float y, float z, vector3* node, float* heading)l

//INFO: 
//PARAM NOTES: pos: Coordinates near which the search for the road segment is executed.
//	min_length: Minimum length that the road segment must have to be considered. 
//	min_lanes: Minimum number of lanes (Both directions added together) the road segment must have to be considered. 
//	south, north: Coordinates of the 2 nodes that define the road segment. (For horizontal roads the command returns the West node in south and the East node in north.)
//	lanes_going_south, lanes_going_north: Number of lanes that go either way. 
//	reserve_width: Width of the bit between the 2 directions (in meters)
//	The coordinates being passed back for both nodes are in the middle of the central reservation of there is one. (If not they are on the transition from the lanes going North and the ones going South) 
//PURPOSE: Will return if can the closest road segment of a certein length with a certain number of lanes.
extern _native bool GET_CLOSEST_ROAD(float x, float y, float z, float min_length, int min_lanes, vector3* south, vector3* north, int* lanes_going_south, int* lanes_going_north, float* reserve_width)l
extern _native bool GET_CLOSEST_STRAIGHT_ROAD(float x, float y, float z, float unk0, float unk1, float* unk2_x, float* unk2_y, float* unk2_z, float* unk3, float* unk4, float* unk5, float* unk6)l
extern _native bool GET_FURTHEST_NETWORK_RESTART_NODE(vector3* node, int* unk)l
extern _native bool GET_NEXT_CLOSEST_CAR_NODE(float x, float y, float z, float* pX, float* pY, float* pZ)l
extern _native bool GET_NEXT_CLOSEST_CAR_NODE_FAVOUR_DIRECTION(float x, float y, float z, float* pX, float* pY, float* pZ, vector3* unk0, float* heading)l
extern _native bool GET_NEXT_CLOSEST_CAR_NODE_WITH_HEADING(float x, float y, float z, float* pX, float* pY, float* pZ, float* heading)l
extern _native bool GET_NEXT_CLOSEST_CAR_NODE_WITH_HEADING_ON_ISLAND(float x, float y, float z, int island, float* pX, float* pY, float* pZ, float* heading)l

//INFO: VecCoors.Z will be calculated automatically if you give it a value of 100.0 or below. If NodeNumber is 1 then the closest node will be returned (as with the two commands above).
//	If NodeNumber is 2 then the second closest node will be returned and so on. The command will return FALSE if no node could be found (probably due to the nodes not being streamed in). 
//PURPOSE:  Will return if can the nth ( n = 1 being closest) closest vehicle node, to a coord.
extern _native bool GET_NTH_CLOSEST_CAR_NODE(float x, float y, float z, int n, float* pX, float* pY, float* pZ)l

//INFO: It will always obey one-way streets. If you want your car to go against one-way streets, you have to check to see if it's facing the right direction and rotate the car 180 degrees. 
//PURPOSE:  Will return if can the closest vehicle node,tries to return a heading which points the car in the right direction to reach FaceCoors.
extern _native bool GET_NTH_CLOSEST_CAR_NODE_FAVOUR_DIRECTION(float x, float y, float z, float* face_x, float* face_y, float* face_z, int n, vector3* nearest, float* heading)l

//INFO: Returns a heading which can be used to set a car facing in a reasonable direction when it is placed on the car node. 
//			The command will return FALSE if no node could be found (probably due to the nodes not being streamed in). 
//PURPOSE: Will return if can the nth ( n = 1 being closest) closest vehicle node with a heading, to a coord.
extern _native bool GET_NTH_CLOSEST_CAR_NODE_WITH_HEADING(float x, float y, float z, int n, float* pResX, float* pResY, float* pResZ, float* pHeading)l

//PURPOSE: Will return if can the nth ( n = 1 being closest) closest vehicle node with a heading, to a coord on an island.
extern _native bool GET_NTH_CLOSEST_CAR_NODE_WITH_HEADING_ON_ISLAND(float x, float y, float z, int n, int island, float* node_x, float* node_y, float* node_z, float* heading, int* num_lanes)l
extern _native bool GET_NTH_CLOSEST_WATER_NODE_WITH_HEADING(float x, float y, float z, int n, bool unk, vector3* node, float* heading)l

//INFO: Every time this command is called 64 nodes are batched (so you won't get the same node twice in the first 64 calls)
//	Once the node has been returned and perhaps sent to a client the coordinates to spawn a car can be found using GET_SPAWN_COORDINATES_FOR_CAR_NODE 
//PURPOSE: Will return a random node to satisfy the specified conditions.
extern _native bool GET_RANDOM_CAR_NODE(float x, float y, float z, float radius, any unused, bool avoid_dead_ends, bool avoid_highways, float* pX, float* pY, float* pZ, float* heading)l
extern _native bool GET_RANDOM_CAR_NODE_INCLUDE_SWITCHED_OFF_NODES(float x, float y, float z, float radius, any unused, bool avoid_dead_ends, bool avoid_highways, float* pX, float* pY, float* pZ, float* heading)l
extern _native bool GET_RANDOM_NETWORK_RESTART_NODE(float x, float y, float z, float radius, vector3* node, float* heading)l
extern _native bool GET_RANDOM_NETWORK_RESTART_NODE_EXCLUDING_GROUP(float x, float y, float z, float radius, vector3* node, int* unk0, int unk1)l
extern _native bool GET_RANDOM_NETWORK_RESTART_NODE_OF_GROUP(float x, float y, float z, float radius, vector3* node, int* unk0, int unk1)l
extern _native bool GET_RANDOM_NETWORK_RESTART_NODE_USING_GROUP_LIST(float x, float y, float z, float radius, vector3* node, int* Unk1024)l

// PURPOSE: Returns a random water node within the given radius of the centre point
// PARAMS:
//  VECTOR pos				centre of the circle to be considered
//  FLOAT radius			radius of the circle to be considered
//  BOOL avoid_dead_ends	not sure if this will have any effect for water nodes
//  BOOL avoid_highways		not sure if this will have any effect for water nodes
//  VECTOR &node			if the command returns TRUE then this will be filled in with the coords of the node found
// RETURNS:
// 	TRUE:					if the command finds a water node within the defined circle
//	FALSE:					if the command fails to find a water node
extern _native bool GET_RANDOM_WATER_NODE(float x, float y, float z, float radius, any unused, bool avoid_dead_ends, bool avoid_highways, bool unk, float* pX, float* pY, float* pZ, float* heading)l
extern _native bool GET_SAFE_POSITION_FOR_CHAR(float x, float y, float z, bool unknownTrue, float* pSafeX, float* pSafeY, float* pSafeZ)l
extern _native bool GET_SORTED_NETWORK_RESTART_NODE(float x, float y, float z, float radius, vector3* node, int* Unk1030, PLAYER_INDEX player, int Unk1032, float Unk1033)l
extern _native bool GET_SORTED_NETWORK_RESTART_NODE_EXCLUDING_GROUP(float x, float y, float z, float radius, vector3* node, int* unk0, int unk1, PLAYER_INDEX player, int unk2, float unk3)l
extern _native bool GET_SORTED_NETWORK_RESTART_NODE_OF_GROUP(float x, float y, float z, float radius, vector3* node, int* unk0, int unk1, PLAYER_INDEX player, int unk2, float unk3)l
extern _native bool GET_SORTED_NETWORK_RESTART_NODE_USING_GROUP_LIST(float x, float y, float z, float radius, vector3* node, int* Unk1039, PLAYER_INDEX player, int Unk1041, float Unk1042)l
extern _native bool HAVE_REQUESTED_PATH_NODES_BEEN_LOADED(void)l

//INFO: The return value becomes true when this has actually happened.
//	If called with false the nodes are released. In this case the return value is irrelevant. 
//PURPOSE: Loads/unloads all path nodes on the map.
extern _native bool LOAD_ALL_PATH_NODES(bool value)l

//INFO: Removes the navmesh required region
//PARAM NOTES: One a single region may be defined globall
extern _native bool REMOVE_NAVMESH_REQUIRED_REGION(void)l
extern _native float CALCULATE_TRAVEL_DISTANCE_BETWEEN_NODES(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native void ADD_AREA_TO_NETWORK_RESTART_NODE_GROUP_MAPPING(int unk0, int unk1)l
extern _native void ADD_GROUP_TO_NETWORK_RESTART_NODE_GROUP_LIST(int group)l
extern _native void ADD_SPAWN_BLOCKING_AREA(float x, float y, float z, float radius)l
extern _native void ADD_SPAWN_BLOCKING_DISC(float x, float y, float z, float Unk907, float Unk908)l
extern _native void CALCULATE_FURTHEST_NETWORK_RESTART_NODE(void)l
extern _native void CLEAR_NETWORK_RESTART_NODE_GROUP_LIST(void)l
extern _native void DEFINE_PED_GENERATION_CONSTRAINT_AREA(float x, float y, float z, float radius)l
extern _native void DESTROY_PED_GENERATION_CONSTRAINT_AREA(void)l
extern _native void FIND_STREET_NAME_AT_POSITION(float pX, float pY, float pZ, uint* strHash0, uint* strHash1)l
extern _native void FLUSH_ALL_PLAYER_RESPAWN_COORDS(void)l
extern _native void FLUSH_ALL_SPAWN_BLOCKING_AREAS(void)l
extern _native void GENERATE_DIRECTIONS(float x, float y, float z, int* direction, int* Unk811)l
extern _native void GET_COORDINATES_FOR_NETWORK_RESTART_NODE(int Unk914, vector3* coords, float* heading)l
extern _native void GET_PARKING_NODE_IN_AREA(float x0, float y0, float z0, float x1, float y1, float z1, float* node_x, float* node_y, float* node_z)l
extern _native void GET_SPAWN_COORDINATES_FOR_CAR_NODE(int Unk918, float x, float y, float z, vector3* coords, float* heading)l
extern _native void LOAD_PATH_NODES_IN_AREA(float x, float y, float z, float radius)l
extern _native void MARK_ROAD_NODE_AS_DONT_WANDER(float x, float y, float z)l
extern _native void REGISTER_PLAYER_RESPAWN_COORDS(PLAYER_INDEX playerIndex, float x, float y, float z)l
extern _native void RELEASE_PATH_NODES(void)l
extern _native void RESET_NETWORK_RESTART_NODE_GROUP_MAPPING(void)l
extern _native void SET_IGNORE_NO_GPS_FLAG(bool set)l
extern _native void SWITCH_PED_PATHS_OFF(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native void SWITCH_PED_PATHS_ON(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native void SWITCH_PED_ROADS_BACK_TO_ORIGINAL(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native void SWITCH_ROADS_BACK_TO_ORIGINAL(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native void SWITCH_ROADS_OFF(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native void SWITCH_ROADS_ON(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native void UNMARK_ALL_ROAD_NODES_AS_DONT_WANDER(void)l
#pragma endregion

#pragma region STREAMING
//INFO: John Whyte said that this command should be debug-only because it can be slow. Also, we usually try to use
//		model hash keys rather than indices for safety.
//PARAM NOTES:  starting with index = 0,  until the function returns FALSE
//PURPOSE: Can be used to cycle through all the ped models.
extern _native bool GET_PED_MODEL_FROM_INDEX(int index, uint* model)l

//INFO: John Whyte said that this command should be debug-only because it can be slow. Also, we usually try to use
//		model hash keys rather than indices for safety.
//PARAM NOTES:, starting with index = 0,  until the function returns FALSE
//PURPOSE: Can be used to cycle through all the vehicle models.
extern _native bool GET_VEHICLE_MODEL_FROM_INDEX(int index, uint* model)l

//PURPOSE:	Returns TRUE if the collision for this model is in memory
extern _native bool HAS_COLLISION_FOR_MODEL_LOADED(uint model)l

//PURPOSE:  Return if a model is in memory
extern _native bool HAS_MODEL_LOADED(uint model)l
extern _native bool HAVE_ANIMS_LOADED(const char* animName)l

//PURPOSE:	Return if model is available in one of the streaming image files
extern _native bool IS_MODEL_IN_CDIMAGE(uint model)l

//PURPOSE: Return if the streaming system is prioritising certain objects because it needs them in now. 
extern _native bool IS_STREAMING_PRIORITY_REQUESTS(void)l

//PURPOSE: Returns TRUE if this model is a ped
extern _native bool IS_THIS_MODEL_A_PED(uint model)l

//PURPOSE: Returns TRUE if this model is a vehicle
extern _native bool IS_THIS_MODEL_A_VEHICLE(uint model)l
extern _native bool UPDATE_LOAD_SCENE(void)l
extern _native int GET_NUM_STREAMING_REQUESTS(void)l
extern _native void ACTIVATE_INTERIOR(INTERIOR_INSTANCE_INDEX interior, bool activate)l
extern _native void ADD_NEEDED_AT_POSN(float x, float y, float z)l
extern _native void ALLOW_GAME_TO_PAUSE_FOR_STREAMING(bool allow)l
extern _native void ALLOW_NETWORK_POPULATION_GROUP_CYCLING(bool allow)l
extern _native void ATTACH_ANIMS_TO_MODEL(uint model, const char* anims)l
extern _native void ENABLE_SCENE_STREAMING(bool enable)l
extern _native void FINISH_STREAMING_REQUEST_LIST(void)l

//PURPOSE:  Load all the objects on the streaming request list
extern _native void LOAD_ALL_OBJECTS_NOW(void)l

//PURPOSE:  Load scene at position
extern _native void LOAD_SCENE(float x, float y, float z)l

//INFO: Can get the interior id from GET_INTERIOR_AT_COORDS and room key from GET_HASH_KEY(roomName)
//PURPOSE:  Load scene as if in an interior.
extern _native void LOAD_SCENE_FOR_ROOM_BY_KEY(INTERIOR_INSTANCE_INDEX interior, uint roomhash)l

//PURPOSE:  Tell streaming that this script no longer needs this model
extern _native void MARK_MODEL_AS_NO_LONGER_NEEDED(uint model)l
extern _native void PRIORITIZE_STREAMING_REQUEST(void)l
extern _native void REMOVE_ANIMS(const char* animName)l

// PURPOSE: Remove a streamable IPL
extern _native void REMOVE_IPL(const char* iplName)l
extern _native void REMOVE_IPL_DISCREETLY(const char* iplname)l
extern _native void REQUEST_ALL_SLODS_IN_WORLD(void)l
extern _native void REQUEST_ANIMS(const char* animName)l

//PURPOSE:  Tell streaming to request collision about point.
extern _native void REQUEST_COLLISION_AT_POSN(float x, float y, float z)l

//INFO:  If you need the collision for that object
// immediately you will need to call LOAD_ALL_OBJECTS_NOW and create the object in the same frame, otherwise the 
// collision management code will delete the collision you requested
//PURPOSE:  Tell streaming to request collision for a specific model.
extern _native void REQUEST_COLLISION_FOR_MODEL(uint model)l

//PURPOSE:	Requests the room and all the models within it
extern _native void REQUEST_INTERIOR_MODELS(uint model, const char* interiorName)l

// PURPOSE: Request streaming to load an IPL file
extern _native void REQUEST_IPL(const char* iplName)l

//PURPOSE: Request streaming to load a model
extern _native void REQUEST_MODEL(uint model)l

//INFO: The police models don't need to be loaded. 
//PURPOSE: For certain network game where the players will never get a wanted level (races).
extern _native void SET_DITCH_POLICE_MODELS(bool set)l

//INFO:  If we are playing back then this loads the list and does
// a load scene for the initial set of objects. If we are recording this initialises the record
//PURPOSE: Set the start point for a streaming request list.
extern _native void SET_REDUCE_PED_MODEL_BUDGET(bool set)l

//INFO: For certain multiplayer missions, is good for deathmatches and such. 
//PURPOSE: can be called to reduce the number of vehicle models streamed.
extern _native void SET_REDUCE_VEHICLE_MODEL_BUDGET(bool set)l
extern _native void SET_STREAMING_REQUEST_LIST_TIME(int time)l
extern _native void START_LOAD_SCENE(float x, float y, float z)l
extern _native void START_STREAMING_REQUEST_LIST(const char* name)l
extern _native void SWITCH_STREAMING(bool on)l
#pragma endregion

#pragma region STATS
extern _native bool CAN_THE_STAT_HAVE_STRING(int stat)l
extern _native bool GET_STAT_FRONTEND_VISIBILITY(int stat)l
extern _native float CONVERT_METRES_TO_FEET(float metres)l
extern _native float GET_FLOAT_STAT(int stat)l
extern _native float GET_PROGRESS_PERCENTAGE(void)l
extern _native int CONVERT_METRES_TO_FEET_INT(int metres)l
extern _native int FIND_MAX_NUMBER_OF_GROUP_MEMBERS(void)l
extern _native int GET_GAMER_NETWORK_SCORE(PLAYER_INDEX playerIndex, int Unk888, int Unk889)l
extern _native int GET_INT_STAT(int stat)l
extern _native int GET_STAT_FRONTEND_DISPLAY_TYPE(int stat)l
extern _native int GET_TOTAL_NUMBER_OF_STATS(void)l
extern _native void AWARD_PLAYER_MISSION_RESPECT(float respect)l
extern _native void DECREMENT_FLOAT_STAT(int stat, float val)l
extern _native void DECREMENT_INT_STAT(int stat, int amount)l
extern _native void INCREMENT_FLOAT_STAT(int stat, float val)l
extern _native void INCREMENT_FLOAT_STAT_NO_MESSAGE(int stat, float value)l
extern _native void INCREMENT_INT_STAT(int stat, int value)l
extern _native void INCREMENT_INT_STAT_NO_MESSAGE(int stat, int value)l
extern _native void PLAYSTATS_CHEAT(const char* cheat_string)l
extern _native void PLAYSTATS_FLOAT(const char* Unk785, float Unk786)l
extern _native void PLAYSTATS_INT(const char* Unk787, int Unk788)l
extern _native void PLAYSTATS_INT_FLOAT(const char* Unk789, int Unk790, float Unk791)l
extern _native void PLAYSTATS_INT_INT(const char* Unk792, int Unk793, int Unk794)l
extern _native void PLAYSTATS_MISSION_CANCELLED(const char* Unk795)l
extern _native void PLAYSTATS_MISSION_FAILED(const char* Unk796)l
extern _native void PLAYSTATS_MISSION_PASSED(const char* str0)l

// PURPOSE:
//    Tells the PlayStats that a mission was started.
// PARAM NOTES:
//    The string should be the entry in a .txt file.
extern _native void PLAYSTATS_MISSION_STARTED(const char* mission_name)l
extern _native void REGISTER_BEST_POSITION(uint Unk505, int position)l
extern _native void REGISTER_FASTEST_TIME(int unk0, int unk1)l
extern _native void REGISTER_FLOAT_STAT(int stat, float val)l
extern _native void REGISTER_GAME_TYPE_ID(int game_type)l
extern _native void REGISTER_INT_STAT(int stat, int val)l
extern _native void REGISTER_MISSION_PASSED(const char* str)l
extern _native void REGISTER_MULTIPLAYER_GAME_WIN(any unused, int Unk949)l
extern _native void REGISTER_NETWORK_BEST_GAME_SCORES(PLAYER_INDEX playerIndex, int Unk950, int Unk951)l
extern _native void REGISTER_ODDJOB_MISSION_PASSED(void)l
extern _native void REGISTER_STRING_FOR_FRONTEND_STAT(int stat, const char* str)l
extern _native void REGISTER_TRACK_NUMBER(int number)l
extern _native void SET_FLOAT_STAT(int stat, float value)l
extern _native void SET_STAT_FRONTEND_ALWAYS_VISIBLE(int stat)l
extern _native void SET_STAT_FRONTEND_DISPLAY_TYPE(int stat, int type)l
extern _native void SET_STAT_FRONTEND_NEVER_VISIBLE(int stat)l
extern _native void SET_STAT_FRONTEND_VISIBILITY(int stat, bool set)l
extern _native void SET_STAT_FRONTEND_VISIBLE_AFTER_INCREMENTED(int stat)l
extern _native void SET_INT_STAT(int stat, int value)l
extern _native void SET_MISSION_RESPECT_TOTAL(float respect)l

//INFO: This does nothing.
extern _native void SET_PLAYER_IS_IN_STADIUM(bool set)l
extern _native void SET_TOTAL_NUMBER_OF_MISSIONS(float floatstatval)l
extern _native void SHOW_UPDATE_STATS(bool show)l
extern _native void UPDATE_NETWORK_RELATIVE_SCORE(PLAYER_INDEX player, int Unk1003, int Unk1004)l
extern _native void UPDATE_NETWORK_STATISTICS(PLAYER_INDEX playerIndex, int ukn0, int ukn1, int ukn2)l

//Deprecated
//REGISTER_MISSION_GIVEN
//ARE_MEASUREMENTS_IN_METRES
//ARE_ANY_CAR_CHEATS_ACTIVATED
//SET_ALL_TAXIS_HAVE_NITRO
#pragma endregion

#pragma region SCRIPT
//PURPOSE: Returns TRUE if the script file can be found on disc
extern _native bool DOES_SCRIPT_EXIST(const char* name)l

//PURPOSE: Returns TRUE if the script program is in memory
extern _native bool HAS_SCRIPT_LOADED(const char* scriptName)l

//PURPOSE: Returns TRUE if the thread is still running
extern _native bool IS_THREAD_ACTIVE(THREADID threadId)l
extern _native int GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(const char* scriptName)l

//PURPOSE: Returns the ID of this thread
extern _native THREADID GET_ID_OF_THIS_THREAD(void)l

//PURPOSE: Stops a thread running and deletes it
extern _native void DESTROY_THREAD(THREADID ScriptHandle)l

//PURPOSE: Tell the code that this script can safely be removed from memory once there are no instances of it running
extern _native void MARK_SCRIPT_AS_NO_LONGER_NEEDED(const char* scriptName)l

//PURPOSE: Request a script program to be loaded
extern _native void REQUEST_SCRIPT(const char* scriptName)l

//PURPOSE: Terminates this script
extern _native void TERMINATE_THIS_SCRIPT(void)l
#pragma endregion

#pragma region OBJECT
//PURPOSE: Check that object exists in the world.
extern _native bool DOES_OBJECT_EXIST(OBJECT_INDEX obj)l

//INFO: Before calling physics commands you have to check that the object has physics.
//			Physics are streamed in sperateley from the drawable object, though entity physics near the player are streamed
//			in automatically.
//PURPOSE: Checks that an object has physics.
extern _native bool DOES_OBJECT_HAVE_PHYSICS(OBJECT_INDEX obj)l
extern _native bool DOES_OBJECT_HAVE_THIS_MODEL(OBJECT_INDEX obj, uint model)l

//PURPOSE: returns TRUE if the closest object within Radius is a proper object - not a dummy or building
extern _native bool DOES_OBJECT_OF_TYPE_EXIST_AT_COORDS(float x, float y, float z, float radius, uint model)l

//PURPOSE: Checks if a pickup exists.
extern _native bool DOES_PICKUP_EXIST(PICKUP_INDEX pickup)l
extern _native bool HAS_CLOSEST_OBJECT_OF_TYPE_BEEN_DAMAGED_BY_CAR(float x, float y, float z, float radius, uint model, VEHICLE_INDEX car)l
extern _native bool HAS_CLOSEST_OBJECT_OF_TYPE_BEEN_DAMAGED_BY_CHAR(float x, float y, float z, float radius, uint model, PED_INDEX ped)l

//PURPOSE: Returns TRUE if the last thing to damage the given object was the given damager ped.
//			Will also return true if the damager ped is in a vehicle that damages the 
//          first object
extern _native bool HAS_OBJECT_BEEN_DAMAGED_BY_CHAR(OBJECT_INDEX obj, PED_INDEX ped)l
extern _native bool HAS_FRAGMENT_ROOT_OF_CLOSEST_OBJECT_OF_TYPE_BEEN_DAMAGED(float x, float y, float z, float radius, uint model)l
extern _native bool HAS_OBJECT_BEEN_DAMAGED(OBJECT_INDEX obj)l

//PURPOSE: Returns TRUE if the last thing to damage the given object was the given damager vehicle.
extern _native bool HAS_OBJECT_BEEN_DAMAGED_BY_CAR(OBJECT_INDEX obj, VEHICLE_INDEX vehicle)l
extern _native bool HAS_OBJECT_BEEN_PHOTOGRAPHED(OBJECT_INDEX obj)l

//PURPOSE: Checks if an object has be removed from its root.
extern _native bool HAS_OBJECT_BEEN_UPROOTED(OBJECT_INDEX obj)l

//PARAM NOTES: This function is only valid for the current frame
//PURPOSE: Checks if the object has collided with anything this frame. 
extern _native bool HAS_OBJECT_COLLIDED_WITH_ANYTHING(OBJECT_INDEX obj)l
extern _native bool HAS_OBJECT_FRAGMENT_ROOT_BEEN_DAMAGED(OBJECT_INDEX obj)l

//PURPOSE: Checks if a pickup has beeen collected
extern _native bool HAS_PICKUP_BEEN_COLLECTED(PICKUP_INDEX pickup)l
extern _native bool IS_ANY_PICKUP_AT_COORDS(float x, float y, float z)l
extern _native bool IS_CLOSEST_OBJECT_OF_TYPE_SMASHED_OR_DAMAGED(float x, float y, float z, float radius, uint model, bool unk0, bool unk1)l
extern _native bool IS_MONEY_PICKUP_AT_COORDS(float x, float y, float z)l
extern _native bool IS_NON_FRAG_OBJECT_SMASHED(float x, float y, float z, float radius, uint model)l

//PURPOSE: Returns true if the object is attached to anything (another entity or the world)
extern _native bool IS_OBJECT_ATTACHED(OBJECT_INDEX obj)l
extern _native bool IS_OBJECT_IN_ANGLED_AREA_2D(OBJECT_INDEX obj, float x0, float y0, float x1, float y1, float angle, bool unk)l
extern _native bool IS_OBJECT_IN_ANGLED_AREA_3D(OBJECT_INDEX obj, float x0, float y0, float z0, float x1, float y1, float z1, float angle, bool flag)l
extern _native bool IS_OBJECT_IN_AREA_2D(OBJECT_INDEX obj, float x0, float y0, float x1, float y2, bool highlight_area)l
extern _native bool IS_OBJECT_IN_AREA_3D(OBJECT_INDEX obj, float x0, float y0, float z0, float x1, float y1, float z1, bool highlight_area)l

//PURPOSE: Returns TRUE if the object is in water
extern _native bool IS_OBJECT_IN_WATER(OBJECT_INDEX obj)l

//INFO:  THIS COMMAND DOESN'T TAKE GEOMETRY INTO CONSIDERATION! 
//PURPOSE: Checks if an object is on screen.
extern _native bool IS_OBJECT_ON_SCREEN(OBJECT_INDEX obj)l

//PURPOSE: Checks if the object is currently playing the animation.
extern _native bool IS_OBJECT_PLAYING_ANIM(OBJECT_INDEX obj, const char* anim_name, const char* anim_dict_name)l

//PURPOSE: Checks if an object has no active physics
extern _native bool IS_OBJECT_STATIC(OBJECT_INDEX obj)l

//PARAM NOTES: This command is only valid for the current frame
//PURPOSE: Checks that 2 objects are intersecting each other this frame. 
extern _native bool IS_OBJECT_TOUCHING_OBJECT(OBJECT_INDEX obj0, OBJECT_INDEX obj1)l

//PURPOSE: Checks if the object is upright within a define angle limit.
extern _native bool IS_OBJECT_UPRIGHT(OBJECT_INDEX obj, float angle)l
extern _native bool LOCATE_OBJECT_2D(OBJECT_INDEX obj, float x0, float y0, float x1, float y1, bool highlight_area)l
extern _native bool LOCATE_OBJECT_3D(OBJECT_INDEX obj, float x0, float y0, float z0, float x1, float y1, float z1, bool highlight_area)l

//PURPOSE: Play an object anim.
extern _native bool PLAY_OBJECT_ANIM(OBJECT_INDEX obj, const char* anim_dict_name, const char* anim_name, float blend_delta, bool loop, bool hold_last_frame)l
extern _native bool PLAYER_IS_NEAR_FIRST_PIGEON(vector3* pos)l

//INFO: This command returns TRUE when the object is in the required position.
//		You will need to work out the exact increment.
//PARAM NOTES: If stop_for_collision is TRUE then the command will also return TRUE if the object has collided with a ped or vehicle (this functionality is currently disabled). 
//PURPOSE: Moves the object by the step values each frame until it is at the correct position
extern _native bool SLIDE_OBJECT(OBJECT_INDEX obj, float x, float y, float z, float increment_x, float increment_y, float increment_z, bool stop_for_collision)l
extern _native bool SMASH_GLASS_ON_OBJECT(float x, float y, float z, float Unk75, uint model, float Unk76)l
extern _native float GET_FRAGMENT_DAMAGE_HEALTH_OF_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, uint model, bool unk)l
extern _native float GET_LEVEL_DESIGN_COORDS_FOR_OBJECT(OBJECT_INDEX obj, any unused, float* x, float* y, float* z)l

//PARAM NOTES: result = (current unbroken groups / original groups)
//														or
//							result = (current mass / undamaged mass)
//PURPOSE: Get the damage health of a fragment heap.
extern _native float GET_OBJECT_FRAGMENT_DAMAGE_HEALTH(OBJECT_INDEX obj, bool health_percentage_by_mass)l

//PURPOSE: Gets the number of pickups of type
extern _native int COUNT_PICKUPS_OF_TYPE(int type)l

//PURPOSE: Returns the index of the ped this object is attached to (-1 if is not attached)
extern _native PED_INDEX GET_PED_OBJECT_IS_ATTACHED_TO(OBJECT_INDEX obj)l

//PURPOSE: Returns the index of the vehicle this object is attached to (-1 if is not attached)
extern _native VEHICLE_INDEX GET_CAR_OBJECT_IS_ATTACHED_TO(OBJECT_INDEX obj)l
extern _native void ADD_OBJECT_TO_INTERIOR_ROOM_BY_KEY(OBJECT_INDEX obj, uint roomKey)l
extern _native void ADD_OBJECT_TO_INTERIOR_ROOM_BY_NAME(OBJECT_INDEX obj, const char* room_name)l
extern _native void ADD_PICKUP_TO_INTERIOR_ROOM_BY_KEY(PICKUP_INDEX pickup, uint room_hash)l

//PURPOSE: Adds a pickup to the named interior
extern _native void ADD_PICKUP_TO_INTERIOR_ROOM_BY_NAME(PICKUP_INDEX pickup, const char* roomName)l
extern _native void ALLOW_MULTIPLE_DRIVEBY_PICKUPS(bool allow)l
extern _native void ANCHOR_OBJECT(OBJECT_INDEX obj, bool anchor, float unk)l

//PARAM NOTES:
//				offset: Offset from centre of entity at which to apply force 
//				component: Component of the entity to apply the force too - only matters for breakable or articulated (ragdoll) physics. 
//								0 means the root or parent component. I'll add an enum list for the components of a ragdoll, but for other entities such as fragment objects 
//								you'll need to check with CJ or me to find out which component you want to apply a force to if you want to, for example, break off the leg of a chair.
//				local_force: Specifies whether the force vector passed in is in local or world coordinates. Local coordinates (TRUE) means the force will get automatically 
//								transformed into world space before being applied. 
//				local_offset: Specifies whether the offset passed in is in local or world coordinates. 
//				scale_by_,ass: Specifies whether to scale the force by mass.
//								TRUE force will be multiplied by mass, ie Force passed in is in fact an acceleration rate in m/s*s (force) or velocity change in m/s(impulse)
//								FALSE force will be applied directly and it's effect will depend on the mass of the object, ie Force passed in is a proper force in Newtons (force) or a step change in momentum kg*m/s (impulse)
//								In other words, scaling by mass is probably easier in most situations - if the mass of the object changes it's behaviour shouldn't, and it's easier to picture the effect because an acceleration rate of 10.0
//PURPOSE: Apply a force to an entity.
extern _native void APPLY_FORCE_TO_OBJECT(OBJECT_INDEX obj, uint type, float force_x, float force_y, float force_z, float offset_x, float offset_y, float offset_z, uint component, bool local_force, bool local_offset, bool scale_by_mass)l

//INFO:  None physical attachment - collision is turned off for an object. 
//PARAM NOTES: veh_bone - use 0 for the skeleton root or -1 for entity matrix. Can attach to any bone on the parent - eg attach to car's door 
//				pos - is from the root of the bone you're attaching to (the vehicles's position if bone=0) 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the vehicle itself if bone=0)
//PURPOSE: Attaches an object to another vehicle with a non physical attachement.
extern _native void ATTACH_OBJECT_TO_CAR(OBJECT_INDEX obj, VEHICLE_INDEX v, uint veh_bone, float x, float y, float z, float rot_x, float rot_y, float rot_z)l

//INFO: Physical attachment using rage constraints - entities are still physically active in the world. 
//PARAM NOTES: obj_bone - refers to the bone on the object which is being attached. 
//			   veh_bone - refers to the bone on the vehicle that you're attaching the first entity to. 
//							This lets us attach for example, a ped's hand (ragdoll) to a car door. Some objects are fragment objects, and have a skeleton with multiple pieces that break apart, and this may be useful for them. 
//				rotation - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//				constrain_rotation - specifies whether you wish to constrain rotation as well as position. In most cases the answer will be Yes. Unless you want to have a hanging/swinging thing.
//PURPOSE: Attaches an object to another vehicle using a physical attachment.
extern _native void ATTACH_OBJECT_TO_CAR_PHYSICALLY(OBJECT_INDEX obj, VEHICLE_INDEX car, uint obj_bone, uint veh_bone, float veh_pos_x, float veh_pos_y, float veh_pos_z, float obj_pos_x, float obj_pos_y, float obj_pos_z, float rot_x, float rot_y, float rot_z, float physical_strength, bool constrain_rotation)l

//INFO:  None physical attachment - collision is turned off for an object. 
//PARAM NOTES: obj_bone - use 0 for the skeleton root or -1 for entity matrix. Can attach to any bone on the parent - eg attach to ped's hand, attach to car's door 
//				pos - is from the root of the bone you're attaching to (the entity's position if bone=0) 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//PURPOSE: Attaches an object to another object with a non physical attachement.
extern _native void ATTACH_OBJECT_TO_OBJECT(OBJECT_INDEX obj0, OBJECT_INDEX obj1_attach_to, uint obj_bone, float x, float y, float z, float rot_x, float rot_y, float rot_z)l

//INFO: Physical attachment using rage constraints - entities are still physically active in the world. 
//PARAM NOTES: obj0_bone - refers to the bone on the first entity which is being attached. 
//			   obj1_bone - refers to the bone on the second entity that you're attaching the first entity to. 
//							This lets us attach for example, a ped's hand (ragdoll) to a car door. Some objects are fragment objects, and have a skeleton with multiple pieces that break apart, and this may be useful for them. 
//				rotation - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//				constrain_rotation - specifies whether you wish to constrain rotation as well as position. In most cases the answer will be Yes. Unless you want to have a hanging/swinging thing.
//PURPOSE: Attaches an object to another object using a physical attachment. More info..
extern _native void ATTACH_OBJECT_TO_OBJECT_PHYSICALLY(OBJECT_INDEX obj0, OBJECT_INDEX obj1_attach_to, uint obj0_bone, uint obj1_bone, float obj1_x, float obj1_y, float obj1_z, float obj0_x, float obj0_y, float obj0_z, float rot_x, float rot_y, float rot_z, float physical_strength, bool constrain_rotation)l

//INFO:  None physical attachment - collision is turned off for an object. 
//PARAM NOTES: ped_bone - use 0 for the skeleton root or -1 for entity matrix. Can attach to any bone on the parent - eg attach to ped's hand, attach to car's door 
//				pos - is from the root of the bone you're attaching to (the entity's position if bone=0) 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//PURPOSE: Attaches an object to another ped with a non physical attachement. 
extern _native void ATTACH_OBJECT_TO_PED(OBJECT_INDEX obj, PED_INDEX c, uint ped_bone, float x, float y, float z, float rot_x, float rot_y, float rot_z, bool detach_on_ragdoll)l

//INFO: Physical attachment using rage constraints - entities are still physically active in the world. 
//PARAM NOTES: obj_bone - refers to the bone on the first entity which is being attached. 
//			   ped_bone - refers to the bone on the second entity that you're attaching the first entity to. 
//							This lets us attach for example, a ped's hand (ragdoll) to a car door. Some objects are fragment objects, and have a skeleton with multiple pieces that break apart, and this may be useful for them. 
//				rotation - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//				constrain_rotation - specifies whether you wish to constrain rotation as well as position. In most cases the answer will be Yes. Unless you want to have a hanging/swinging thing.
//PURPOSE: Attaches an object to another ped using a physical attachment.
extern _native void ATTACH_OBJECT_TO_PED_PHYSICALLY(OBJECT_INDEX obj, PED_INDEX c, uint obj_bone, uint ped_bone, float ped_pos_X, float ped_pos_y, float ped_pos_z, float obj_pos_x, float obj_pos_y, float obj_pos_z, float rot_x, float rot_y, float rot_z, float physical_strength, bool constrain_rotation)l

//PURPOSE: Clears the last damage that has been applied to an object
extern _native void CLEAR_OBJECT_LAST_DAMAGE_ENTITY(OBJECT_INDEX obj)l
extern _native void CLEAR_ROOM_FOR_OBJECT(OBJECT_INDEX obj)l
extern _native void CONNECT_LODS(OBJECT_INDEX obj0, OBJECT_INDEX obj1)l

//INFO: 
//PARAM NOTES: pos - the coordinates the money pickups will be generated round. Must be on or above the ground.
//	      amount - the total amount of money dropped	
//PURPOSE: Creates a bunch of money pickups around the given coordinates, of the given amount
extern _native void CREATE_MONEY_PICKUP(float x, float y, float z, int amount, bool unknownTrue, PICKUP_INDEX* pPickup)l

//PARAM NOTES: network:		The new object will be created and synced on other machines if a network game is running
//PURPOSE: Create an object  with an offset (from the root the base) at the given coord.
extern _native void CREATE_OBJECT(uint model, float x, float y, float z, OBJECT_INDEX* pObj, bool network)l

//PARAM NOTES: network:		The new object will be created and synced on other machines if a network game is running
//PURPOSE: Create an object with no offset at the given coord.
extern _native void CREATE_OBJECT_NO_OFFSET(uint model, float x, float y, float z, OBJECT_INDEX* pObj, bool network)l

//PURPOSE: Creates a pickup spawner which can be referenced by the script and will spawn a pickup whenever the player gets near. This spawner can also
//         regenerate the pickup after it is collected. The spawner is removed when the script terminates.
extern _native void CREATE_PICKUP(uint model, int pickupType, float x, float y, float z, PICKUP_INDEX* pPickup, bool script_host)l

//PURPOSE: Creates a pickup spawner which can be referenced by the script and will spawn a pickup whenever the player gets near. This spawner can also
//         regenerate the pickup after it is collected. The spawner is removed when the script terminates.
extern _native void CREATE_PICKUP_ROTATE(uint model, int pickupType, uint unknown, float x, float y, float z, float rX, float rY, float rZ, PICKUP_INDEX* pPickup)l
extern _native void CREATE_PICKUP_WITH_AMMO(uint model, int pickupType, uint unknown, float x, float y, float z, PICKUP_INDEX* pPickup)l
extern _native void CREATE_TEMPORARY_RADAR_BLIPS_FOR_PICKUPS_IN_AREA(float x, float y, float z, float radius, int bliptype)l

//PURPOSE: Deletes an object instantly from the world. 
extern _native void DELETE_OBJECT(OBJECT_INDEX* pObj)l

//PURPOSE: Detaches an object from being attached.
//PARAM NOTES: 
//		no_collision_until_clear - disables collision between this object and the parent until they are clear of one another
extern _native void DETACH_OBJECT(OBJECT_INDEX obj, bool no_collision_until_clear)l
extern _native void DETACH_OBJECT_NO_COLLIDE(OBJECT_INDEX obj, bool unk)l
extern _native void DISABLE_LOCAL_PLAYER_PICKUPS(bool disable)l
extern _native void DONT_REMOVE_OBJECT(OBJECT_INDEX obj)l
extern _native void ENABLE_DISABLED_ATTRACTORS_ON_OBJECT(OBJECT_INDEX obj, bool enable)l
extern _native void FIND_NEAREST_COLLECTABLE_BIN_BAGS(float x, float y, float z)l
extern _native void FIND_NEAREST_ENTITIES_WITH_SPECIAL_ATTRIBUTE(float x, float y, float z)l

//PURPOSE: Freezes the object position so that it wont be affected by the world physics.
extern _native void FREEZE_OBJECT_POSITION(OBJECT_INDEX obj, bool frozen)l
extern _native void FREEZE_OBJECT_POSITION_AND_DONT_LOAD_COLLISION(OBJECT_INDEX obj, bool freeze)l

//PURPOSE: Freezes the object of type.
extern _native void FREEZE_POSITION_OF_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, uint model, bool frozen)l
extern _native void GET_CLOSEST_STEALABLE_OBJECT(float x, float y, float z, float radius, OBJECT_INDEX* obj)l

//PURPOSE: Get the state of a door managed by the door system
extern _native void GET_DOOR_STATE(OBJECT_INDEX obj, bool* lock_state, float* open_ratio)l

//PURPOSE: gets the interior index from coords
extern _native void GET_INTERIOR_AT_COORDS(float x, float y, float z, INTERIOR_INSTANCE_INDEX* pInterior)l

//INFO: The phase of the anim is between 0.0 and 1.0 regardless of the anim length
//PURPOSE: Gets the current phase of the anim.
extern _native void GET_OBJECT_ANIM_CURRENT_TIME(OBJECT_INDEX obj, const char* anim_name, const char* anim_dict_name, float* time)l

//PURPOSE: Gets the total anim time in seconds.
extern _native void GET_OBJECT_ANIM_TOTAL_TIME(OBJECT_INDEX obj, const char* anim_name, const char* anim_dict_name, float* time)l

//PURPOSE: Gets the co-ordinates of the object. 
extern _native void GET_OBJECT_COORDINATES(OBJECT_INDEX obj, float* pX, float* pY, float* pZ)l

//PURPOSE: Gets the objects's heading in degrees.
extern _native void GET_OBJECT_HEADING(OBJECT_INDEX obj, float* pHeading)l

//PURPOSE: Gets the objects's current health.
extern _native void GET_OBJECT_HEALTH(OBJECT_INDEX obj, float* pHealth)l
extern _native void GET_OBJECT_MASS(OBJECT_INDEX obj, float* mass)l

//PURPOSE: Get the model name for the object.
extern _native void GET_OBJECT_MODEL(OBJECT_INDEX obj, uint* pModel)l

//PURPOSE: Gets the objects's quaternion
extern _native void GET_OBJECT_QUATERNION(OBJECT_INDEX obj, float* qx, float* qy, float* qz, float* qw)l

//PURPOSE: Get the rotation velocity of an object
extern _native void GET_OBJECT_ROTATION_VELOCITY(OBJECT_INDEX obj, float* pX, float* pY, float* pZ)l

//PURPOSE: Gets the speed an object is travelling in a forward direction (m/sec).
extern _native void GET_OBJECT_SPEED(OBJECT_INDEX obj, float* pSpeed)l
extern _native void GET_OBJECT_TURN_MASS(OBJECT_INDEX obj, float* turnmass)l

//PURPOSE: Gets the current velocity of the object.
extern _native void GET_OBJECT_VELOCITY(OBJECT_INDEX obj, float* pX, float* pY, float* pZ)l

//PURPOSE:Gets a point described as an offset from that object and returns the world coordinates of that point
extern _native void GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(OBJECT_INDEX obj, float x, float y, float z, float* pOffX, float* pOffY, float* pOffZ)l
extern _native void GET_PICKUP_COORDINATES(PICKUP_INDEX pickup, float *pX, float *pY, float *pZ)l

//PURPOSE: Gets a room key from an object in that room.
extern _native void GET_ROOM_KEY_FROM_OBJECT(OBJECT_INDEX obj, uint* pRoomKey)l

//PURPOSE: Gets room jkey from the pickup
extern _native void GET_ROOM_KEY_FROM_PICKUP(PICKUP_INDEX pickup, uint *hash)l

//PURPOSE: Gets a safe coord at which to create a pickup close to the input co-ordinates
extern _native void GET_SAFE_PICKUP_COORDS(float x, float y, float z, float *pSafeX, float *pSafeY, float *pSafeZ)l

//PURPOSE: Gets the state of the closest door type.
extern _native void GET_STATE_OF_CLOSEST_DOOR_OF_TYPE(uint model, float x, float y, float z, bool* lock_state, float* open_ratio)l
extern _native void GRAB_NEARBY_OBJECT_WITH_SPECIAL_ATTRIBUTE(int attribute, OBJECT_INDEX* obj)l
extern _native bool LOOK_AT_NEARBY_ENTITY_WITH_SPECIAL_ATTRIBUTE(int Unk98, vector3* pos, float* Unk100, uint* Unk101, uint* Unk102, float* Unk103)l

//INFO: Only four objects can be targettable at any time. 
//PURPOSE: The player can auto target the object.
extern _native void MAKE_OBJECT_TARGETTABLE(OBJECT_INDEX obj, bool targettable)l

//INFO: The object will be added back into the world population and deleted when off screen.
//PURPOSE: Sets the object to be removed, cannot refer to object from script. 
extern _native void MARK_OBJECT_AS_NO_LONGER_NEEDED(OBJECT_INDEX*pObj)l
extern _native void PICKUPS_PASS_TIME(int time)l
extern _native void PLACE_OBJECT_RELATIVE_TO_CAR(OBJECT_INDEX obj, VEHICLE_INDEX car, float x, float y, float z)l

//PURPOSE: Remove objects of certain pickup type.
extern _native void REMOVE_ALL_PICKUPS_OF_TYPE(int type)l

//PURPOSE: Removes a pickup from the map.
extern _native void REMOVE_PICKUP(PICKUP_INDEX pickup)l

//PURPOSE: Removes all projected textures on a particular object 
extern _native void REMOVE_PROJTEX_FROM_OBJECT(OBJECT_INDEX obj)l
extern _native void REMOVE_TEMPORARY_RADAR_BLIPS_FOR_PICKUPS(void)l
extern _native void RENDER_WEAPON_PICKUPS_BIGGER(bool value)l
extern _native void RESET_ARMOUR_PICKUP_NETWORK_REGEN_TIME(void)l
extern _native void RESET_HEALTH_PICKUP_NETWORK_REGEN_TIME(void)l
extern _native void RESET_MONEY_PICKUP_NETWORK_REGEN_TIME(void)l
extern _native void RESET_WEAPON_PICKUP_NETWORK_REGEN_TIME(WEAPON_TYPE weaponType)l

//INFO: The object will always rotate in the direction that requires the least amount of movement. 
//			This command returns TRUE when the object is at the correct rotation. If stop_for_collison is TRUE then the command will also return TRUE if the object has collided with a ped or vehicle. 
//PARAM NOTES: rot is measured in degrees and should be between 0 and 360.
//PURPOSE: Rotates the object until it is at the rot
extern _native bool ROTATE_OBJECT(OBJECT_INDEX obj, float x, float y, bool stop_for_collision)l

//INFO: this could be useful if you have created the object on a slope and you need it to lie properly on the slope rather than hover horizontally above the ground. 
//PURPOSE: Turns on the object's physics as soon as the collision is loaded around it.
extern _native void SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(OBJECT_INDEX obj, bool set)l
extern _native void SET_ALL_PICKUPS_OF_TYPE_COLLECTABLE_BY_CAR(int pickuptype, bool set)l

//INFO:  Ordinarily it is only displayed a number of times and then the game stops mentioning it.
//PURPOSE: Displays the message to tell the player to press LB to pickup a weapon in the same slot.
extern _native void SET_ALWAYS_DISPLAY_WEAPON_PICKUP_MESSAGE(bool set)l

//PURPOSE: Sets the time before a armour pickup respawns in a network games
extern _native void SET_ARMOUR_PICKUP_NETWORK_REGEN_TIME(int timeMS)l
extern _native void SET_COLLECTABLE1_TOTAL(int total)l
extern _native void SET_DEAD_PEDS_DROP_WEAPONS(bool set)l
extern _native void SET_DO_NOT_SPAWN_PARKED_CARS_ON_TOP(PICKUP_INDEX pickup, bool set)l

//PURPOSE: Set the state of a door managed by the door system
extern _native void SET_DOOR_STATE(OBJECT_INDEX door, bool lock_state, float open_ratio)l
extern _native void SET_HEADING_OF_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, uint model, float heading)l

//PURPOSE:Sets the time before a health pickup respawns in a network games
extern _native void SET_HEALTH_PICKUP_NETWORK_REGEN_TIME(int timeMS)l
extern _native void SET_HOT_WEAPON_SWAP(bool set)l
extern _native void SET_LOAD_COLLISION_FOR_OBJECT_FLAG(OBJECT_INDEX obj, bool set)l
extern _native void SET_MONEY_PICKUP_NETWORK_REGEN_TIME(int time)l

//INFO: Setup an object to be rendered with alpha, 
extern _native void SET_OBJECT_ALPHA(OBJECT_INDEX obj, int alpha)l

//PURPOSE: Sets the phase of the anim between 0.0 and 1.0
extern _native void SET_OBJECT_ANIM_CURRENT_TIME(OBJECT_INDEX obj, const char* anim_name, const char* anim_dict_name, float time)l
extern _native void SET_OBJECT_ANIM_PLAYING_FLAG(OBJECT_INDEX obj, const char* anim_name, const char* anim_dict_name, bool set)l

//PURPOSE: Sets objects's current anim speed.
extern _native void SET_OBJECT_ANIM_SPEED(OBJECT_INDEX obj, const char* anim_name, const char* anim_dict_name, float speed)l

//PURPOSE: Sets an object to be stealable.
extern _native void SET_OBJECT_AS_STEALABLE(OBJECT_INDEX obj, bool set)l
extern _native void SET_OBJECT_CCD(OBJECT_INDEX obj, bool set)l

//PURPOSE: Sets if an object uses collision.
extern _native void SET_OBJECT_COLLISION(OBJECT_INDEX obj, bool value)l

//PURPOSE:  Sets the objects's coords.
extern _native void SET_OBJECT_COORDINATES(OBJECT_INDEX obj, float pX, float pY, float pZ)l
extern _native void SET_OBJECT_DRAW_LAST(OBJECT_INDEX obj, bool set)l

//PURPOSE: Sets the object to respond to the physics system.
extern _native void SET_OBJECT_DYNAMIC(OBJECT_INDEX obj, bool set)l

//PURPOSE: Sets the objects's heading in degrees.
extern _native void SET_OBJECT_HEADING(OBJECT_INDEX obj, float value)l

//PURPOSE: Sets the objects's current health. 
extern _native void SET_OBJECT_HEALTH(OBJECT_INDEX obj, float health)l
extern _native void SET_OBJECT_INITIAL_ROTATION_VELOCITY(OBJECT_INDEX obj, float x, float y, float z)l

//PARAM NOTES: Asserts if you pass in 0.0 to all vector components.
//PURPOSE: Sets the initial speed of an object.
extern _native void SET_OBJECT_INITIAL_VELOCITY(OBJECT_INDEX obj, float x, float y, float z)l

//PURPOSE: Set object invincible. 
extern _native void SET_OBJECT_INVINCIBLE(OBJECT_INDEX obj, bool set)l

//PURPOSE: Sets on all lights attached to the object; set works in "inversed" logic manner: FALSE-to ENABLE entity lights, TRUE-to DISABLE entity lights; 
extern _native void SET_OBJECT_LIGHTS(OBJECT_INDEX obj, bool lights)l

//PURPOSE: Sets the object is only damaged by the main player
extern _native void SET_OBJECT_ONLY_DAMAGED_BY_PLAYER(OBJECT_INDEX obj, bool set)l

//INFO: Any param set to -1.0 will be ignored, and will remain as defaults. 
//		Specifying mass = -1.0 will reset the mass back to the object's default value
//PARAM NOTES: mass: object mass (rotational inertia is calculated from new mass and volume) 
//							gravity_factor the gravity affecting this object can be modified (default is 1.0) 
//							trans_damping:  
//								x = x = constant friction (doesn't depend on speed)
//								 y = y*V = speed dependent friction (multiplied by speed)
//								 z = z*V^2 = aerodynamic drag (multiplied by speed squared) 
//							rot_damping
//    							 x = x = constant friction (doesn't depend on speed)
//    							y = y*V = speed dependent friction (multiplied by speed)
//   							z = z*V^2 = aerodynamic drag (multiplied by speed squared) 
//							collision_margin
//								 Set to thinnist part of object. Default is 10cm 
//							max_angular_speed
//								Increase for objects which are likely to spin fast, e.g. small spheres. Default is 2*PI 
//PURPOSE: Set physics parameters on this instance of an object.
extern _native void SET_OBJECT_PHYSICS_PARAMS(OBJECT_INDEX obj, float mass, float gravity_factor, float trans_damping_x, float trans_damping_y, float trans_damping_z, float rot_damping_x, float rot_damping_y, float rot_damping_z, float collision_margin, float max_angular_speed)l

//PURPOSE: Set the proofs on object: Bullet, Flame, Explosion, Colision, Melee
extern _native void SET_OBJECT_PROOFS(OBJECT_INDEX obj, bool bullet, bool flame, bool explosion, bool collision, bool melee)l

//PURPOSE: Sets the objects's rotation quaternion
extern _native void SET_OBJECT_QUATERNION(OBJECT_INDEX obj, float qx, float qy, float qz, float qw)l

//PURPOSE: This command must be called with TRUE before you can use HAS_OBJECT_COLLIDED_WITH_ANYTHING. 
extern _native void SET_OBJECT_RECORDS_COLLISIONS(OBJECT_INDEX obj, bool set)l

//INFO: Setup an object to be rendered as scorched/burned down
extern _native void SET_OBJECT_RENDER_SCORCHED(OBJECT_INDEX obj, bool set)l

//PURPOSE: Sets the objects's rotation.
extern _native void SET_OBJECT_ROTATION(OBJECT_INDEX obj, float rot_x, float rot_y, float rot_z)l
extern _native void SET_OBJECT_SCALE(OBJECT_INDEX obj, float scale)l

//PURPOSE: Set if the object is visible (i.e. rendered)
extern _native void SET_OBJECT_VISIBLE(OBJECT_INDEX obj, bool value)l
extern _native void SET_PICKUP_COLLECTABLE_BY_CAR(PICKUP_INDEX pickup, bool set)l
extern _native void SET_PICKUPS_FIX_CARS(bool set)l
extern _native void SET_PLAYER_CAN_DROP_WEAPONS_IN_CAR(bool set)l
extern _native void SET_PLAYERS_DROP_MONEY_IN_NETWORK_GAME(bool toggle)l

//INFO: To open and shut a door smoothly call every frame with lock_state=false as you increase/decrease the open/shut ratio
//		In your script tidyup you should reset the door to lock_state = false, open_ratio = 0.0 to release the door. Unless you specifically want the door to remain locked for ever more. 
//PARAM NOTES: 
//						open_ratio: 0.0 = shut, 1.0 = open (anti clockwise for swinging doors)  -1.0 = open (clockwise for swinging doors, does nothing for sliding/garage doors) 
//						lock_state = true will force the door to snap to the desired ratio. 
//PURPOSE: Sest the state of the closest door of type.
extern _native void SET_STATE_OF_CLOSEST_DOOR_OF_TYPE(uint model, float x, float y, float z, bool lock_state, float open_ratio)l
extern _native void SET_SUPPRESS_HEADLIGHT_SWITCH(bool set)l
extern _native void SET_USES_COLLISION_OF_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, uint model, bool set)l
extern _native void SET_VISIBILITY_OF_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, uint model, bool set)l
extern _native void SET_VISIBILITY_OF_NEARBY_ENTITY_WITH_SPECIAL_ATTRIBUTE(int attribute, bool set)l

//PURPOSE: Sets the time before a weapon respawns in a network games
extern _native void SET_WEAPON_PICKUP_NETWORK_REGEN_TIME(WEAPON_TYPE weaponType, int timeMS)l
extern _native void SWAP_NEAREST_BUILDING_MODEL(float x, float y, float z, float radius, uint modelfrom, uint modelto)l
extern _native void SWITCH_ARROW_ABOVE_BLIPPED_PICKUPS(bool on)l
extern _native void WINCH_CAN_PICK_OBJECT_UP(OBJECT_INDEX obj, bool can)l

//Deprecated
//CREATE_PROTECTION_PICKUP
//CREATE_LOCKED_PROPERTY_PICKUP
//CREATE_FORSALE_PROPERTY_PICKUP
//GET_ROPE_HEIGHT_FOR_OBJECT
//SET_ROPE_HEIGHT_FOR_OBJECT
//GRAB_ENTITY_ON_ROPE_FOR_OBJECT
//RELEASE_ENTITY_FROM_ROPE_FOR_OBJECT
//SET_LOCK_STATE_OF_CLOSEST_OBJECT_OF_TYPE
//LOCK_DOOR
//UPDATE_PICKUP_MONEY_PER_DAY
#pragma endregion

#pragma region VEHICLE
//PURPOSE: Checks if the taxi light is on
extern _native bool ARE_TAXI_LIGHTS_ON(VEHICLE_INDEX vehicle)l

//PURPOSE: Has a timer for a specific stuck check expired.
extern _native bool CHECK_STUCK_TIMER(VEHICLE_INDEX car, int stuck_type, int required_time)l
extern _native bool DOES_CAR_HAVE_HYDRAULICS(VEHICLE_INDEX car)l

//PURPOSE: Checks that the vehicle has a roof.
extern _native bool DOES_CAR_HAVE_ROOF(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks that a vehicle has a stuck check
extern _native bool DOES_CAR_HAVE_STUCK_CAR_CHECK(VEHICLE_INDEX vehicle)l

//PURPOSE: Check that vehicle exists in the world.
extern _native bool DOES_VEHICLE_EXIST(VEHICLE_INDEX vehicle)l
extern _native bool GET_VEHICLE_COMPONENT_INFO(VEHICLE_INDEX veh, int component, vector3* Unk45, vector3* Unk46, int* Unk47, bool unk)l

//PURPOSE: Returns TRUE if the last thing to damage the given vehicle was the given damager vehicle.
extern _native bool HAS_CAR_BEEN_DAMAGED_BY_CAR(VEHICLE_INDEX vehicle, VEHICLE_INDEX otherCar)l

//PURPOSE: Returns TRUE if the last thing to damage the given vehicle was the given damager ped.
extern _native bool HAS_CAR_BEEN_DAMAGED_BY_CHAR(VEHICLE_INDEX vehicle, PED_INDEX ped)l
extern _native bool HAS_CAR_BEEN_DROPPED_OFF(VEHICLE_INDEX car)l

//PURPOSE: Checks that a vehicle has be resprayed
extern _native bool HAS_CAR_BEEN_RESPRAYED(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks if the vehicle recording has loaded
extern _native bool HAS_CAR_RECORDING_BEEN_LOADED(uint recording)l

//PURPOSE: Checks that the vehicle has stopped because of the traffic lights.
extern _native bool HAS_CAR_STOPPED_BECAUSE_OF_LIGHT(VEHICLE_INDEX car)l

//PURPOSE: Checks if the player's car has just been resprayed in any garage. 
extern _native bool HAS_RESPRAY_HAPPENED(void)l

//PURPOSE: Checks if the vehicle big e.g plane .
extern _native bool IS_BIG_VEHICLE(VEHICLE_INDEX vehicle)l

//INFO: Will return false if the vehicle has been set as no longer needed.
//PURPOSE: Checks if the vehicle has been created by script.
extern _native bool IS_CAR_A_MISSION_CAR(VEHICLE_INDEX vehicle)l

//PURPOSE: Returns true if the vehicle is attached to anything (another entity or the world)
extern _native bool IS_CAR_ATTACHED(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks if the vehicle is dead.
extern _native bool IS_CAR_DEAD(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks if the vehicle door is intact
extern _native bool IS_CAR_DOOR_DAMAGED(VEHICLE_INDEX vehicle, int door)l

//PURPOSE: Checks that a vehicle door is open
extern _native bool IS_CAR_DOOR_FULLY_OPEN(VEHICLE_INDEX vehicle, int door)l
extern _native bool IS_CAR_HEALTH_GREATER(VEHICLE_INDEX car, int health)l
extern _native bool IS_CAR_IN_AIR_PROPER(VEHICLE_INDEX vehicle)l
extern _native bool IS_CAR_IN_ANGLED_AREA_2D(VEHICLE_INDEX veh, float x1, float y1, float x2, float y2, float angle, bool highlight_area)l
extern _native bool IS_CAR_IN_ANGLED_AREA_3D(VEHICLE_INDEX veh, float x1, float y1, float z1, float x2, float y2, float z2, bool highlight_area)l
extern _native bool IS_CAR_IN_AREA_2D(VEHICLE_INDEX vehicle, float x1, float y1, float x2, float y2, bool highlight_area)l
extern _native bool IS_CAR_IN_AREA_3D(VEHICLE_INDEX vehicle, float x1, float y1, float z1, float x2, float y2, float z2, uint highlight_area)l

//PURPOSE: Checks if the vehicle is an a garage. 
extern _native bool IS_CAR_IN_GARAGE_AREA(const char* garageName, VEHICLE_INDEX vehicle)l

//PURPOSE: Returns TRUE if the entity is in water
extern _native bool IS_CAR_IN_WATER(VEHICLE_INDEX vehicle)l
extern _native bool IS_CAR_LOW_RIDER(VEHICLE_INDEX car)l

//PURPOSE: Checks that the vehicle is a specific model.
extern _native bool IS_CAR_MODEL(VEHICLE_INDEX vehicle, uint model)l
extern _native bool IS_CAR_ON_FIRE(VEHICLE_INDEX vehicle)l

//INFO:  THIS COMMAND DOESN'T TAKE GEOMETRY INTO CONSIDERATION! 
//PURPOSE: Checks if a vehicle is on screen.
extern _native bool IS_CAR_ON_SCREEN(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks if the the vehicle is free.
extern _native bool IS_CAR_PASSENGER_SEAT_FREE(VEHICLE_INDEX vehicle, int seat)l

//PURPOSE: Checks if the vehicle is currently playing the animation.
extern _native bool IS_CAR_PLAYING_ANIM(VEHICLE_INDEX car, const char* anim_dict_name, const char* anim_name)l

//PURPOSE: Checks if the vehicle siren is on
extern _native bool IS_CAR_SIREN_ON(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks that a vehicle is stopped at lights 
extern _native bool IS_CAR_STOPPED(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks that a vehicle is stopped at lights 
extern _native bool IS_CAR_STOPPED_AT_TRAFFIC_LIGHTS(VEHICLE_INDEX vehicle)l
extern _native bool IS_CAR_STOPPED_IN_AREA_2D(VEHICLE_INDEX veh, float x1, float y1, float x2, float y2, bool highlight_area)l
extern _native bool IS_CAR_STREET_RACER(VEHICLE_INDEX car)l
extern _native bool IS_CAR_STUCK(VEHICLE_INDEX car)l

//INFO: Before using this command, you must have called ADD_UPSIDEDOWN_CAR_CHECK for vehicle index. 
//PURPOSE:  Checks that a vehicle is stuck on its roof.
extern _native bool IS_CAR_STUCK_ON_ROOF(VEHICLE_INDEX vehicle)l

//PARAM NOTES: This command is only valid for the current frame
//PURPOSE: Checks that 2 vehicles are intersecting each other this frame.
extern _native bool IS_CAR_TOUCHING_CAR(VEHICLE_INDEX vehicle, VEHICLE_INDEX otherCar)l

//PURPOSE: Checks if a vehicles tire is burst
extern _native bool IS_CAR_TYRE_BURST(VEHICLE_INDEX vehicle, int tire)l
extern _native bool IS_CAR_UPRIGHT(VEHICLE_INDEX vehicle)l
extern _native bool IS_CAR_UPSIDEDOWN(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks if the vehicle is waiting for world collision to stream in.
extern _native bool IS_CAR_WAITING_FOR_WORLD_COLLISION(VEHICLE_INDEX vehicle)l

//PURPOSE: Checks if a cop vehicle is in a 3d area
extern _native bool IS_COP_VEHICLE_IN_AREA_3D_NO_SAVE(float x0, float y0, float z0, float x1, float y1, float z1)l
extern _native bool IS_EMERGENCY_SERVICES_VEHICLE(VEHICLE_INDEX veh)l

//PURPOSE: Checks if the garage is closed
extern _native bool IS_GARAGE_CLOSED(const char* garageName)l

//PURPOSE: Checks if the garage is open 
extern _native bool IS_GARAGE_OPEN(const char* garageName)l

//PURPOSE: Checks that a specific part of a heli is broken
extern _native bool IS_HELI_PART_BROKEN(VEHICLE_INDEX heli, bool check_main_rotor, bool check_rear_rotor, bool check_tail_bone)l

//PURPOSE: Checks if any of tha pay-n-spray garages in the game a re currently opening/closing or repairing the player car. 
extern _native bool IS_PAY_N_SPRAY_ACTIVE(void)l

//PURPOSE: Checks if the playback is going on for a vehicle.
extern _native bool IS_PLAYBACK_GOING_ON_FOR_CAR(VEHICLE_INDEX car)l

//PURPOSE: Checks that vehicle model is a bike model
extern _native bool IS_THIS_MODEL_A_BIKE(uint model)l

//PURPOSE: Checks that vehicle model is a boat model
extern _native bool IS_THIS_MODEL_A_BOAT(uint model)l

//PURPOSE: Checks that vehicle model is a car model
extern _native bool IS_THIS_MODEL_A_CAR(uint model)l

//PURPOSE:  Checks that vehicle model is a heli model
extern _native bool IS_THIS_MODEL_A_HELI(uint model)l

//PURPOSE: Checks that vehicle model is a plane model
extern _native bool IS_THIS_MODEL_A_PLANE(uint model)l

//PURPOSE: Checks that vehicle model is a train model
extern _native bool IS_THIS_MODEL_A_TRAIN(uint model)l

//INFO: Must be used before using many of the other vehicle commands.
//PURPOSE: Checks that the vehicle is fit to be driven.
extern _native bool IS_VEH_DRIVEABLE(VEHICLE_INDEX vehicle)l
extern _native bool IS_VEH_STUCK(VEHICLE_INDEX veh, int time, bool unk, any unused0, any unused1)l

//PURPOSE: Returns true if a window exists and is not smashed, false otherwise.
extern _native bool IS_VEH_WINDOW_INTACT(VEHICLE_INDEX vehicle, int window)l

//PURPOSE: Checks if a vehicle extra is turned on.
extern _native bool IS_VEHICLE_EXTRA_TURNED_ON(VEHICLE_INDEX vehicle, int extra)l

//PURPOSE: Checks if the vehicle is not in contact with anything.
extern _native bool IS_VEHICLE_ON_ALL_WHEELS(VEHICLE_INDEX vehicle)l
extern _native bool IS_VEHICLE_TOUCHING_OBJECT(VEHICLE_INDEX veh, OBJECT_INDEX obj)l
extern _native bool LOCATE_CAR_2D(VEHICLE_INDEX car, float x0, float y0, float x1, float y1, bool hightlight_area)l
extern _native bool LOCATE_CAR_3D(VEHICLE_INDEX car, float x0, float y0, float z0, float x1, float y1, float z1, bool highlight_area)l
extern _native bool LOCATE_DEAD_CAR_3D(VEHICLE_INDEX car, float x0, float y0, float z0, float x1, float y1, float z1, bool highlight_area)l

//PURPOSE: Play an entity anim.
extern _native bool PLAY_CAR_ANIM(VEHICLE_INDEX car, const char* anim_name, const char* anim_dict_name, float blend_delta, bool loop, bool hold_last_frame)l
extern _native bool PLAYER_IS_INTERACTING_WITH_GARAGE(void)l

//PURPOSE: Sets the vehicle on the ground taking into account the angle of the ground.
extern _native bool SET_CAR_ON_GROUND_PROPERLY(VEHICLE_INDEX vehicle)l

//PURPOSE: Gets the the model name of the vehicle.
extern _native const char* GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(uint model)l
extern _native const char* GET_STATION_NAME(VEHICLE_INDEX train, uint station)l
extern _native float FIND_POSITION_IN_RECORDING(VEHICLE_INDEX car)l
extern _native float FIND_TIME_POSITION_IN_RECORDING(VEHICLE_INDEX car)l
extern _native float GET_ENGINE_HEALTH(VEHICLE_INDEX vehicle)l
extern _native float GET_HEIGHT_OF_VEHICLE(VEHICLE_INDEX vehicle, float x, float y, float z, bool unknownTrue1, bool unknownTrue2)l

//INFO: 1000.0 = full,  0.0 = go on fire,  -1000.0 = burnt out 
			//Petrol tank "will" explode when health reaches -1000.0
			//Petrol tank health will drop if on fire until it explodes 
//PURPOSE: Get petrol tank health for a vehicle.
extern _native float GET_PETROL_TANK_HEALTH(VEHICLE_INDEX vehicle)l

//PURPOSE: Gets the total duration of the recording 
extern _native float GET_TOTAL_DURATION_OF_CAR_RECORDING(uint recording)l
extern _native float GET_VEHICLE_ENGINE_REVS(VEHICLE_INDEX veh)l
extern _native int FIND_TRAIN_DIRECTION(VEHICLE_INDEX train)l
extern _native int GET_VEHICLE_GEAR(VEHICLE_INDEX veh)l

//PURPOSE: Gets the vehicles siren health
extern _native int GET_CAR_SIREN_HEALTH(VEHICLE_INDEX car)l
extern _native int GET_CURRENT_PLAYBACK_NUMBER_FOR_CAR(VEHICLE_INDEX car)l
extern _native int GET_TIME_TIL_NEXT_STATION(VEHICLE_INDEX train)l
extern _native uint GET_CURRENT_STATION_FOR_TRAIN(VEHICLE_INDEX train)l
extern _native uint GET_NEXT_STATION_FOR_TRAIN(VEHICLE_INDEX train)l

//PARAM NOTES: 
//	model_hash - Specifiy a certain type of vehicle to search for, or DUMMY_MODEL_FOR_SCRIPT to ignore the type check
//	SearchFlags - See the VEHICLE_SEARCH_XXXX commands at the top of commands_vehicle.sch
//PURPOSE: Get the closet random vehicle to a coord 
extern _native VEHICLE_INDEX GET_CLOSEST_CAR(float x, float y, float z, float radius, uint model, uint search_flags)l

//PURPOSE: All garages are openend and if a garage had a players control blocked that player will be released.
extern _native void ABORT_ALL_GARAGE_ACTIVITY(void)l

//PURPOSE: Activates the heli speed cheat.
extern _native void ACTIVATE_HELI_SPEED_CHEAT(VEHICLE_INDEX heli, int speed_boost_value)l
extern _native void ADD_CAR_TO_MISSION_DELETION_LIST(VEHICLE_INDEX car)l
extern _native void ADD_STUCK_CAR_CHECK(VEHICLE_INDEX car, float min_move_distance, int check_frequency)l

//INFO: This command is similar to the basic ADD_STUCK_CAR_CHECK except that the game will attempt to warp the car as soon as it becomes stuck. 
//          The three flags are used to control whether the car is warped if it has become stuck, as soon as it is upside-down or as soon as it is in water.
//PARAM NOTES: warp_method: if warp_method is negative then the car will attempt to warp back on to its last route node.
//                          If warp_method is greater than 0 then the car will try to warp to that number of closest car nodes, starting with the closest. 
//                          In both cases (last route node or closest car nodes), the car will only warp if both it and the destination coordinates are offscreen.
//PURPOSE: Will attempt to warp a vehicle out of a stuck or upsidedown or in water position.
extern _native void ADD_STUCK_CAR_CHECK_WITH_WARP(VEHICLE_INDEX car, float min_move_distance, int check_frequency, bool warp_if_struck, bool warp_if_upside_down, bool warp_if_in_water, int warp_method)l

// INFO: Use this command to tell the game that this car should be checked for being stuck on its roof.
//PURPOSE: Adds a vehicle is upside down check.
extern _native void ADD_UPSIDEDOWN_CAR_CHECK(VEHICLE_INDEX vehicle)l

//PARAM NOTES:
//				Vector force to be applied 
//				offset: Offset from centre of vehicle at which to apply force 
//				Component: Component of the vehicle to apply the force too - only matters for breakable or articulated (ragdoll) physics. 
//								0 means the root or parent component. I'll add an enum list for the components of a ragdoll, but for other entities such as fragment objects 
//								you'll need to check with CJ or me to find out which component you want to apply a force to if you want to, for example, break off the leg of a chair.
//				local_force: Specifies whether the force vector passed in is in local or world coordinates. Local coordinates (TRUE) means the force will get automatically 
//								transformed into world space before being applied. 
//				local_offset: Specifies whether the offset passed in is in local or world coordinates. 
//				scale_by_mass: Specifies whether to scale the force by mass.
//								TRUE force will be multiplied by mass, ie Force passed in is in fact an acceleration rate in m/s*s (force) or velocity change in m/s(impulse)
//								FALSE force will be applied directly and it's effect will depend on the mass of the object, ie Force passed in is a proper force in Newtons (force) or a step change in momentum kg*m/s (impulse)
//								In other words, scaling by mass is probably easier in most situations - if the mass of the object changes it's behaviour shouldn't, and it's easier to picture the effect because an acceleration rate of 10.0
//PURPOSE: Apply a force to a vehicle.
extern _native void APPLY_FORCE_TO_CAR(VEHICLE_INDEX vehicle, uint type, float force_x, float force_y, float force_z, float offset_x, float offset_y, float offset_z, uint component, bool local_force, bool local_offset, bool scale_by_mass)l

//PURPOSE: Sets the boat anchor.
extern _native void ANCHOR_BOAT(VEHICLE_INDEX boat, bool anchor)l

//INFO:  None physical attachment - collision is turned off for vehicle. 
//PARAM NOTES: veh_bone - use 0 for the skeleton root or -1 for entity matrix. Can attach to any bone on the parent - eg attach to ped's hand, attach to car's door 
//				pos - is from the root of the bone you're attaching to (the entity's position if bone=0) 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//PURPOSE: Attaches a vehicle to another vehicle with a non physical attachement.
extern _native void ATTACH_CAR_TO_CAR(VEHICLE_INDEX car0, VEHICLE_INDEX car1, uint veh_bone, float x, float y, float z, float rot_x, float rot_y, float rot_z)l

//INFO: Physical attachment using rage constraints - entities are still physically active in the world. 
//PARAM NOTES: veh1_bone - refers to the bone on the first vehicle which is being attached. 
//			   veh2_bone - refers to the bone on the second vehicle that you're attaching the first vehicle to. 
//							This lets us attach for example, a ped's hand (ragdoll) to a car door. Some objects are fragment objects, and have a skeleton with multiple pieces that break apart, and this may be useful for them. 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//				constrain_rotation - specifies whether you wish to constrain rotation as well as position. In most cases the answer will be Yes. Unless you want to have a hanging/swinging thing.
//PURPOSE: Attaches a vehicle to another vehicle using a physical attachment.
extern _native void ATTACH_CAR_TO_CAR_PHYSICALLY(VEHICLE_INDEX vehid1, VEHICLE_INDEX vehid2, uint veh1_bone, uint veh2_bone, float veh2_x, float veh2_y, float veh2_z, float veh1_x, float veh1_y, float veh1_z, float rot_x, float rot_y, float rot_z, float physical_strength, bool constrain_rotation)l

//INFO:  None physical attachment - collision is turned off for vehicle. 
//PARAM NOTES: obj_bone - use 0 for the skeleton root or -1 for entity matrix. Can attach to any bone on the parent - eg attach to ped's hand, attach to car's door 
//				pos - is from the root of the bone you're attaching to (the entity's position if bone=0) 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//PURPOSE: Attaches a vehicle to another object with a non physical attachement.
extern _native void ATTACH_CAR_TO_OBJECT(VEHICLE_INDEX car, OBJECT_INDEX obj, uint obj_bone, float x, float y, float z, float rot_x, float rot_y, float rot_z)l

//INFO: Physical attachment using rage constraints - entities are still physically active in the world. 
//PARAM NOTES: veh_bone - refers to the bone on the first vehicle which is being attached. 
//			   obj_bone - refers to the bone on the second object that you're attaching the first vehicle to. 
//							This lets us attach for example, a ped's hand (ragdoll) to a car door. Some objects are fragment objects, and have a skeleton with multiple pieces that break apart, and this may be useful for them. 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//				constrain_rotation - specifies whether you wish to constrain rotation as well as position. In most cases the answer will be Yes. Unless you want to have a hanging/swinging thing.
//PURPOSE: Attaches a vehicle to another object using a physical attachment. More info..
extern _native void ATTACH_CAR_TO_OBJECT_PHYSICALLY(VEHICLE_INDEX veh, OBJECT_INDEX obj, uint veh_bone, uint obj_bone, float obj_x, float obj_y, float obj_z, float veh_x, float veh_y, float veh_z, float rot_x, float rot_y, float rot_z, float physical_strength, bool constrain_rotation)l

//PURPOSE: Breaks a vehicle door.
extern _native void BREAK_CAR_DOOR(VEHICLE_INDEX vehicle, int door, bool disappear)l

//PURPOSE: Sets if a vehicles tire can burst.
extern _native void BURST_CAR_TYRE(VEHICLE_INDEX vehicle, int tire)l

//INFO: Colours can be found in carcols.dat
//PURPOSE: Sets the body colours of the cars
extern _native void CHANGE_CAR_COLOUR(VEHICLE_INDEX vehicle, int color1, int color2)l

//INFO:  Check with the artist for the garage name 
//PURPOSE: Sets the garage type.
extern _native void CHANGE_GARAGE_TYPE(const char* garage, uint type)l

//INFO: After this command is called the car will use ai to try and stay close to the original recording
//PURPOSE: Sets a vehicle recording for a certain vehicle from being exact to be using AI.
extern _native void CHANGE_PLAYBACK_TO_USE_AI(VEHICLE_INDEX car)l

//PURPOSE: Clears the last damage that has been applied to a vehicle
extern _native void CLEAR_CAR_LAST_DAMAGE_ENTITY(VEHICLE_INDEX vehicle)l
extern _native void CLEAR_ROOM_FOR_CAR(VEHICLE_INDEX vehicle)l

//PURPOSE:  Shuts all the doors on a vehicle
extern _native void CLOSE_ALL_CAR_DOORS(VEHICLE_INDEX vehicle)l

//PURPOSE: Close a grage door
extern _native void CLOSE_GARAGE(const char* garageName)l
extern _native void CONTROL_CAR_DOOR(VEHICLE_INDEX vehicle, int door, any unused, float angle)l

//PARAM NOTES:
//             network:		The new object will be created and synced on other machines if a network game is running
//PURPOSE: Creates a vehicle at a coord with a heading
extern _native void CREATE_CAR(uint model, float x, float y, float z, VEHICLE_INDEX* pVehicle, bool network)l

//PURPOSE: Creates a vehicle generator 
extern _native void CREATE_CAR_GENERATOR(float x, float y, float z, float heading, float max_length, float max_width, uint model, int color1, int color2, int spec1, int spec2, bool Unk66, bool alarm, bool locked, VEHICLE_GENERATOR_INDEX* handle)l
extern _native void CREATE_CARS_ON_GENERATORS_IN_AREA(float x0, float y0, float z0, float x1, float y1, float z1, int unk)l

//PURPOSE: Create a mission train
extern _native void CREATE_MISSION_TRAIN(uint carrier_config, float x, float y, float z, bool direction_flag, VEHICLE_INDEX* pTrain)l
extern _native void CREATE_RANDOM_CAR_FOR_CAR_PARK(float x, float y, float z, float radius)l
extern _native void DAMAGE_CAR(VEHICLE_INDEX car, float x, float y, float z, float unkforce0, float unkforce1, bool unk)l

//PURPOSE: Deletes all non-mission trains.
extern _native void DELETE_ALL_TRAINS(void)l

//PURPOSE: Deletes an entity instantly from the world. 
extern _native void DELETE_CAR(VEHICLE_INDEX* pVehicle)l

//PURPOSE: Deletes a scripted vehicle generator
extern _native void DELETE_CAR_GENERATOR(VEHICLE_GENERATOR_INDEX* handle)l

//PURPOSE: Deletes a mission train (must have been created by this script)
extern _native void DELETE_MISSION_TRAIN(VEHICLE_INDEX pTrain)l
extern _native void DELETE_MISSION_TRAINS(void)l

//PURPOSE: Detaches a vehicle from being attached.
extern _native void DETACH_CAR(VEHICLE_INDEX vehicle)l
extern _native void DISABLE_CAR_GENERATORS(bool disable)l
extern _native void DISABLE_CAR_GENERATORS_WITH_HELI(bool disable)l
extern _native void DONT_SUPPRESS_ANY_CAR_MODELS(void)l
extern _native void DONT_SUPPRESS_CAR_MODEL(uint model)l

//PARAM NOTES: Add Explosion will determines if an explosion happens
//PURPOSE: Explodes a vehicle.
extern _native void EXPLODE_CAR(VEHICLE_INDEX vehicle, bool add_explosion, bool keep_damage_entity)l

//PURPOSE: Explode a vehicle in a cutscene
extern _native void EXPLODE_CAR_IN_CUTSCENE(VEHICLE_INDEX car, bool explode)l
extern _native void EXPLODE_CAR_IN_CUTSCENE_SHAKE_AND_BIT(VEHICLE_INDEX car, bool flag0, bool flag1, bool flag2)l
extern _native void EXTINGUISH_CAR_FIRE(VEHICLE_INDEX vehicle)l
extern _native void FIX_CAR(VEHICLE_INDEX vehicle)l

//PURPOSE: Fixes a vehicles burts tire
extern _native void FIX_CAR_TYRE(VEHICLE_INDEX vehicle, int tire)l
extern _native void FORCE_ALL_VEHICLE_LIGHTS_OFF(bool off)l

//PURPOSE: Sets the vehicle lights status.
extern _native void FORCE_CAR_LIGHTS(VEHICLE_INDEX car, int lights)l
extern _native void FORCE_HIGH_LOD(bool force)l
extern _native void FORCE_RANDOM_CAR_MODEL(uint hash)l

//PURPOSE: Freezes the vehicle position so that it wont be affected by the world physics.
extern _native void FREEZE_CAR_POSITION(VEHICLE_INDEX vehicle, bool frozen)l
extern _native void FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION(VEHICLE_INDEX vehicle, bool frozen)l

//INFO: The phase of the anim is between 0.0 and 1.0 regardless of the anim length
//PURPOSE: Gets the current phase of the anim.
extern _native void GET_CAR_ANIM_CURRENT_TIME(VEHICLE_INDEX car, const char* anim_dict_name, const char* anim_name, float* time)l

//PURPOSE: Gets the total anim time in seconds.
extern _native void GET_CAR_ANIM_TOTAL_TIME(VEHICLE_INDEX car, const char* anim_dict_name, const char* anim_name, float* time)l
extern _native void GET_CAR_BLOCKING_CAR(VEHICLE_INDEX car0, VEHICLE_INDEX* car1)l

//PURPOSE: Gets the vehicles colours. 
extern _native void GET_CAR_COLOURS(VEHICLE_INDEX vehicle, int* pColor1, int* pColor2)l

//PURPOSE: Gets the co-ordinates of the vehicle.
extern _native void GET_CAR_COORDINATES(VEHICLE_INDEX vehicle, float* pX, float* pY, float* pZ)l

//PURPOSE: Find out how much a car has deformed at a specific position on the vehicle (local coordinates)
extern _native void GET_CAR_DEFORMATION_AT_POS(VEHICLE_INDEX vehicle, float x, float y, float z, vector3* pDeformation)l

//PURPOSE: Gets the vehicle lock status.
extern _native void GET_CAR_DOOR_LOCK_STATUS(VEHICLE_INDEX vehicle, uint* pValue)l
extern _native void GET_CAR_FORWARD_VECTOR(VEHICLE_INDEX car, vector3* vec)l
extern _native void GET_CAR_FORWARD_X(VEHICLE_INDEX vehicle, float* pValue)l
extern _native void GET_CAR_FORWARD_Y(VEHICLE_INDEX vehicle, float* pValue)l

//PURPOSE: Gets the vehicles's heading in degrees.
extern _native void GET_CAR_HEADING(VEHICLE_INDEX vehicle, float* pValue)l

//PURPOSE: Gets the vehicles's current health.
extern _native void GET_CAR_HEALTH(VEHICLE_INDEX vehicle, int* pValue)l

//PURPOSE: Return the livery of the vehicle
extern _native void GET_CAR_LIVERY(VEHICLE_INDEX car, int* livery)l

//PURPOSE: Gets the mass of the vehicle
extern _native void GET_CAR_MASS(VEHICLE_INDEX car, float* mass)l

//PURPOSE: Get the model name for the vehicle.
extern _native void GET_CAR_MODEL(VEHICLE_INDEX vehicle, uint* pValue)l

//PURPOSE: Gets the monetary value of a vehicles model.
extern _native void GET_CAR_MODEL_VALUE(VEHICLE_INDEX car, int* value)l

//PURPOSE: Gets the the pitch of the vehicle in degrees.
extern _native void GET_CAR_PITCH(VEHICLE_INDEX vehicle, float* pValue)l

//PURPOSE: Get the vehicles's roll around the Y axis in degrees
extern _native void GET_CAR_ROLL(VEHICLE_INDEX vehicle, float* pValue)l

//PURPOSE: Gets the speed a vehicle is travelling in a forward direction (m/sec).
extern _native void GET_CAR_SPEED(VEHICLE_INDEX vehicle, float* pValue)l

//PARAM NOTES: local_result gives the result in local coordinates to the vehicle (x=right, y=forward, z=up)
//PURPOSE: Get velocity vector of a vehicle. 
extern _native void GET_CAR_SPEED_VECTOR(VEHICLE_INDEX vehicle, vector3* pVector, bool local_result)l

//PURPOSE: Gets an up right value  between 1.0 being upright and -1.0 being upside down
extern _native void GET_CAR_UPRIGHT_VALUE(VEHICLE_INDEX vehicle, float* pValue)l

//PURPOSE: Gets the ped in a vehicle seat.
extern _native void GET_CHAR_IN_CAR_PASSENGER_SEAT(VEHICLE_INDEX vehicle, int seatIndex, PED_INDEX* pPed)l

//INFO:  The model returned will be streamed in apart from during the first 20 or so seconds at the start of the game. (So the script has to make sure it is loaded but only to make sure it doesn't crash) 
//              The returned model can only be the 2 basic models.
//PURPOSE: Gets the basic police car model irrespective of  wanted level.
extern _native void GET_CURRENT_BASIC_POLICE_CAR_MODEL(uint* pModel)l

//INFO: This command will give you the one that is currently loaded. 
//          The model returned will be streamed in apart from during the first 20 or so seconds at the start of the game. (So the script has to make sure it is loaded but only to make sure it doesn't crash) 
//          The returned model could be a noose, police hummer or stockade as well as the basic police cars
//PURPOSE:  Gets the basic police car model.
extern _native void GET_CURRENT_POLICE_CAR_MODEL(uint* pModel)l

//INFO:  The game loads 2 taxi models at any one time.When the script needs a taxi model it should call this command to find out which one to use. 
//          You don't need to REQUEST the model after this but you have to make sure it is loaded (HAS_MODEL_LOADED). 
//          This should return true straight away apart from during the first 20 seconds of the game running. 
//PURPOSE: Gets the cuurent taxi model loaded.
extern _native void GET_CURRENT_TAXI_CAR_MODEL(uint* pModel)l
extern _native void GET_DEAD_CAR_COORDINATES(VEHICLE_INDEX vehicle, float* pX, float* pY, float* pZ)l

//PURPOSE: Gets the door angle of a door between 0.0 and 1.0.
extern _native void GET_DOOR_ANGLE_RATIO(VEHICLE_INDEX vehicle, int door, float* pAngleRatio)l
extern _native void GET_DRIVER_OF_CAR(VEHICLE_INDEX vehicle, PED_INDEX* pPed)l
extern _native void GET_EXTRA_CAR_COLOURS(VEHICLE_INDEX vehicle, int* pColor1, int* pColor2)l

//PURPOSE: Gets the interior from the vehicle.
extern _native void GET_INTERIOR_FROM_CAR(VEHICLE_INDEX vehicle, INTERIOR_INSTANCE_INDEX* pInterior)l
extern _native void GET_KEY_FOR_CAR_IN_ROOM(VEHICLE_INDEX vehicle, uint* pKey)l

//PURPOSE: Get the max number of pasengers a vehicle can have.
extern _native void GET_MAXIMUM_NUMBER_OF_PASSENGERS(VEHICLE_INDEX vehicle, int* pMax)l

//PURPOSE: Gets the nearest cable car to a coord. 
extern _native void GET_NEAREST_CABLE_CAR(float x, float y, float z, float radius, VEHICLE_INDEX* pVehicle)l

//PURPOSE: Gets the number of vehicle colours
extern _native void GET_NUM_CAR_COLOURS(VEHICLE_INDEX vehicle, int* pNumColors)l

//PURPOSE: Return the number of liveries for this vehicle
extern _native void GET_NUM_CAR_LIVERIES(VEHICLE_INDEX car, int* num)l

//PURPOSE: Get the number of passengers in a vehicle.
extern _native void GET_NUMBER_OF_PASSENGERS(VEHICLE_INDEX vehicle, int* pNumPassengers)l

//PURPOSE: Gets a point and converts its to the vehicle's local coordinate system
extern _native void GET_OFFSET_FROM_CAR_GIVEN_WORLD_COORDS(VEHICLE_INDEX vehicle, float x, float y, float z, float* pOffX, float* pOffY, float* pOffZ)l

//PURPOSE: Gets a point described as an offset from that vehicle and returns the world coordinates of that point
extern _native void GET_OFFSET_FROM_CAR_IN_WORLD_COORDS(VEHICLE_INDEX vehicle, float x, float y, float z, float* pOffX, float* pOffY, float* pOffZ)l

//INFO: For use attaching a car to the flatbed.
extern _native void GET_OFFSETS_FOR_ATTACH_CAR_TO_CAR(VEHICLE_INDEX car0, VEHICLE_INDEX car1, vector3* Unk67, vector3* Unk68)l
extern _native void GET_PLANE_UNDERCARRIAGE_POSITION(VEHICLE_INDEX plane, float* pos)l

//INFO: Allows access to the translation of the vehicle in a recording at a given point in time
//PURPOSE: Returns the translation of the vehicle at a given time during the recording
extern _native void GET_POSITION_OF_CAR_RECORDING_AT_TIME(uint recording, float time, vector3* pos)l

//PURPOSE: Gets a random vehicle in from the back bumber of a vehicle
extern _native void GET_RANDOM_CAR_BACK_BUMPER_IN_SPHERE(float x, float y, float z, float radius, uint model, int search_flags, VEHICLE_INDEX* veh)l
extern _native void GET_RANDOM_CAR_FRONT_BUMPER_IN_SPHERE_NO_SAVE(float x, float y, float z, float radius, uint model, bool unk, VEHICLE_INDEX* handle, VEHICLE_INDEX veh)l

//PURPOSE:  Gets a random vehicle a sphere area
extern _native void GET_RANDOM_CAR_IN_SPHERE(float x, float y, float z, float radius, uint model, int search_flags, VEHICLE_INDEX* car)l
extern _native void GET_RANDOM_CAR_IN_SPHERE_NO_SAVE(float x, float y, float z, float radius, uint model, bool unk, VEHICLE_INDEX* car)l

//PURPOSE: Gets a random vehicle model from memory.
extern _native void GET_RANDOM_CAR_MODEL_IN_MEMORY(bool only_normal_cars, uint* pHash, int* vehicle_class)l
extern _native void GET_RANDOM_CAR_OF_TYPE_IN_ANGLED_AREA_NO_SAVE(float x, float y, float z, float angle, uint model, VEHICLE_INDEX *car)l
extern _native void GET_RANDOM_CAR_OF_TYPE_IN_AREA_NO_SAVE(float x0, float y0, float x1, float y1, uint model, VEHICLE_INDEX* car)l

//PURPOSE: Gets last carriage of the train with the given engine. 
extern _native void GET_TRAIN_CABOOSE(VEHICLE_INDEX train, VEHICLE_INDEX* caboose)l

//INFO: carriage will be NULL if the carriage couldn't be found. 
//PARAM NOTES: A carriage_number of 0 will return a pointer to the engine
//PURPOSE: Get the specified carriage of the train with the given engine.
extern _native void GET_TRAIN_CARRIAGE(VEHICLE_INDEX train, int carriage_number, VEHICLE_INDEX* carriage)l

//PURPOSE:  Gets the vehicles dirt level 0.0 (clean) to 15.9 (dirty). 
extern _native void GET_VEHICLE_DIRT_LEVEL(VEHICLE_INDEX vehicle, float* level)l

//PURPOSE: Gets the vehicles's quaternion
extern _native void GET_VEHICLE_QUATERNION(VEHICLE_INDEX veh, float* qx, float* qy, float* qz, float* qw)l
extern _native void HAND_VEHICLE_CONTROL_BACK_TO_PLAYER(VEHICLE_INDEX veh)l

//PURPOSE: Set the vehicle doors to a certain locked state.
extern _native void LOCK_CAR_DOORS(VEHICLE_INDEX vehicle, uint lock_state)l
extern _native void MARK_CAR_AS_CONVOY_CAR(VEHICLE_INDEX vehicle, bool convoyCar)l

//INFO: The vehicle will added back into the world population and deleted when off screen.
//PURPOSE: Sets the vehicle to be removed, cannot refer to vehicle from script.
extern _native void MARK_CAR_AS_NO_LONGER_NEEDED(VEHICLE_INDEX* pVehicle)l

//PURPOSE: Sets a mission train as no longer needed (must have been created by this script)
extern _native void MARK_MISSION_TRAIN_AS_NO_LONGER_NEEDED(VEHICLE_INDEX train)l
extern _native void MARK_MISSION_TRAINS_AS_NO_LONGER_NEEDED(void)l

//PURPOSE: Opens a vehicle door.
extern _native void OPEN_CAR_DOOR(VEHICLE_INDEX vehicle, int door)l

//PURPOSE: Opens a garage
extern _native void OPEN_GARAGE(const char* name)l

//PURPOSE: Sets the number of parked cars budget.
extern _native void OVERRIDE_NUMBER_OF_PARKED_CARS(int num)l

//PURPOSE: Pauses the playback for a recorded vehicle
extern _native void PAUSE_PLAYBACK_RECORDED_CAR(VEHICLE_INDEX car)l
extern _native void PLANE_STARTS_IN_AIR(VEHICLE_INDEX plane)l

//PURPOSE: opens a car trunk.
extern _native void POP_CAR_BOOT(VEHICLE_INDEX vehicle)l

//PURPOSE: Removes a recording from memory thats not being used.
extern _native void REMOVE_CAR_RECORDING(uint recording)l

//PURPOSE: Removes a vehicles window.
extern _native void REMOVE_CAR_WINDOW(VEHICLE_INDEX car, int windnum)l

//PURPOSE: Removes vehicles for all gens in the area.
extern _native void REMOVE_CARS_FROM_GENERATORS_IN_AREA(float x0, float y0, float z0, float x1, float y1, float z1)l

//PURPOSE: Removes a vehicle stuck check
extern _native void REMOVE_STUCK_CAR_CHECK(VEHICLE_INDEX vehicle)l

//PURPOSE:  Remove a upsidedown vehicle check
extern _native void REMOVE_UPSIDEDOWN_CAR_CHECK(VEHICLE_INDEX vehicle)l

//PURPOSE: Request a vehicle recording file.
extern _native void REQUEST_CAR_RECORDING(int recording)l

//PURPOSE: Reset vehicle wheels to original (modelled) position, or to fully extended position
extern _native void RESET_CAR_WHEELS(VEHICLE_INDEX car, bool unk)l

//PURPOSE: Resets a vehicles stuck check.
extern _native void RESET_STUCK_TIMER(VEHICLE_INDEX car, uint stuck_type)l

//PURPOSE: Sets all vehicles can be damaged
extern _native void SET_ALL_CARS_CAN_BE_DAMAGED(bool set)l

//PURPOSE: Sets all generators map and script active.
extern _native void SET_ALL_CAR_GENERATORS_BACK_TO_ACTIVE(void)l
extern _native void SET_AMBIENT_PLANES_SPEED_MULTIPLIER(float multiplier)l
extern _native void SET_BIKE_RIDER_WILL_PUT_FOOT_DOWN_WHEN_STOPPED(VEHICLE_INDEX bike, bool set)l
extern _native void SET_BLIP_THROTTLE_RANDOMLY(VEHICLE_INDEX veh, bool set)l

//PURPOSE: Sets if a vehicles tire can burst.
extern _native void SET_CAN_BURST_CAR_TYRES(VEHICLE_INDEX car, bool set)l

//PURPOSE: Set the vehicle can be resparyed
extern _native void SET_CAN_RESPRAY_CAR(VEHICLE_INDEX car, bool can)l
extern _native void SET_CAR_ALLOWED_TO_DROWN(VEHICLE_INDEX car, bool allowed)l
extern _native void SET_CAR_ALWAYS_CREATE_SKIDS(VEHICLE_INDEX car, bool set)l

//PURPOSE: Sets the phase of the anim between 0.0 and 1.0
extern _native void SET_CAR_ANIM_CURRENT_TIME(VEHICLE_INDEX car, const char* anim_dict_name, const char* anim_name, float time)l

//PURPOSE: Sets vehicles's current anim speed.
extern _native void SET_CAR_ANIM_SPEED(VEHICLE_INDEX car, const char* anim_dict_name, const char* anim_name, float speed)l

//PURPOSE: Sets a random vehicle as a mission vehicle.
extern _native void SET_CAR_AS_MISSION_CAR(VEHICLE_INDEX car)l

//PURPOSE: Sets if the vehicle can be damaged.
extern _native void SET_CAR_CAN_BE_DAMAGED(VEHICLE_INDEX vehicle, bool value)l

//PURPOSE: Sets if the vehicle shows visible damage.
extern _native void SET_CAR_CAN_BE_VISIBLY_DAMAGED(VEHICLE_INDEX vehicle, bool value)l

//PURPOSE: Sets if a vehicle uses collision.
extern _native void SET_CAR_COLLISION(VEHICLE_INDEX car, bool set)l

//INFO: Car models have a number of colour combinations preset for them in carvariations.pso.meta
//          Use GET_NUM_CAR_COLOURS first to find out how many combinations there are. 
//PURPOSE: Set some of the preselcted colour combinations.
extern _native void SET_CAR_COLOUR_COMBINATION(VEHICLE_INDEX car, int combination)l

//PURPOSE:  Sets the vehicles's coords
extern _native void SET_CAR_COORDINATES(VEHICLE_INDEX vehicle, float x, float y, float z)l

//Pretty sure INVALID_WORLD_Z is anything below -100.0f (game puts you back on the map)
//INFO: If you set the z component of VecNewCoors to INVALID_WORLD_Z  this command to automatically find the ground z for your entity if there is collision. 
//PURPOSE:  Sets the entity's coords without adding the offset.
extern _native void SET_CAR_COORDINATES_NO_OFFSET(VEHICLE_INDEX car, float x, float y, float z)l

//INFO: If the Denisty multiplier is set to 0.0 then ambients cars will stop spawning.
//PARAM NOTES: DensityMultiplier default = 1.0
//PURPOSE: Sets the amount of ambient traffic and parked vehicles being spawned.
extern _native void SET_CAR_DENSITY_MULTIPLIER(float density)l
extern _native void SET_CAR_DISTANCE_AHEAD_MULTIPLIER(VEHICLE_INDEX car, float multiplier)l

//PURPOSE: Sets a vehicle door to latch.
extern _native void SET_CAR_DOOR_LATCHED(VEHICLE_INDEX car, int door, bool latched, bool auto_latch)l

//PARAM NOTES: no_delay only has an effect when engine_on is TRUE. If no_delay is TRUE then the engine will start immediately. 
//PURPOSE: Sets the vehicles engine.
extern _native void SET_CAR_ENGINE_ON(VEHICLE_INDEX car, bool engine_on, bool no_delay)l

//PURPOSE: Sets the vehicles forward speed in m/s
extern _native void SET_CAR_FORWARD_SPEED(VEHICLE_INDEX vehicle, float speed)l

//INFO: Max allowed areas is 48. Let code know if that needs increasing
//PURPOSE: Sets all generators map and script active in the area.
extern _native void SET_CAR_GENERATORS_ACTIVE_IN_AREA(float x0, float y0, float z0, float x1, float y1, float z1, bool set)l

//PURPOSE: Sets the vehicles's heading in degrees.
extern _native void SET_CAR_HEADING(VEHICLE_INDEX vehicle, float heading)l

//PURPOSE: Sets the vehicles's current health. 
extern _native void SET_CAR_HEALTH(VEHICLE_INDEX vehicle, int Value)l

//INFO:  Ensure you only set this for car that you want to remain active inside a cutscene - not to be used for mission cars that are in the cutscene - the cutscene will deal with that itself.
//PURPOSE: Allow a MISSION vehicle to function while a cutscene is running.
extern _native void SET_CAR_IN_CUTSCENE(VEHICLE_INDEX car, bool set)l
extern _native void SET_CAR_LANE_SHIFT(VEHICLE_INDEX car, float shift)l

//PURPOSE: Sets how bright the vehicles headlights are.
extern _native void SET_CAR_LIGHT_MULTIPLIER(VEHICLE_INDEX car, float multiplier)l

//PURPOSE: Set the livery of the vehicle
extern _native void SET_CAR_LIVERY(VEHICLE_INDEX car, int livery)l
extern _native void SET_CAR_NOT_DAMAGED_BY_RELATIONSHIP_GROUP(VEHICLE_INDEX car, bool set, int group)l

//PURPOSE: Sets the vehicle is only damaged by the main player
extern _native void SET_CAR_ONLY_DAMAGED_BY_PLAYER(VEHICLE_INDEX car, bool set)l

//PURPOSE: Set the vehicle can be only damaged by peds in the given relationship group
extern _native void SET_CAR_ONLY_DAMAGED_BY_RELATIONSHIP_GROUP(VEHICLE_INDEX car, bool set, int group)l

//PURPOSE: Set the proofs on vehicle: Bullet, Flame, Explosion, Colision, Melee
extern _native void SET_CAR_PROOFS(VEHICLE_INDEX vehicle, bool bullet, bool fire, bool explosion, bool collision, bool melee)l
extern _native void SET_CAR_RANDOM_ROUTE_SEED(VEHICLE_INDEX car, int seed)l
extern _native void SET_CAR_STAY_IN_FAST_LANE(VEHICLE_INDEX car, bool set)l
extern _native void SET_CAR_STAY_IN_SLOW_LANE(VEHICLE_INDEX car, bool set)l

//PURPOSE: Sets the vehicle so it only takes 25% of the normal damage when hit. 
extern _native void SET_CAR_STRONG(VEHICLE_INDEX vehicle, bool strong)l
extern _native void SET_CAR_TRACTION(VEHICLE_INDEX car, float traction)l

//PURPOSE: Set if the vehicle is visible (i.e. rendered)
extern _native void SET_CAR_VISIBLE(VEHICLE_INDEX vehicle, bool value)l
extern _native void SET_CAR_WATERTIGHT(VEHICLE_INDEX car, bool set)l

//PURPOSE: turn on and off old style GTA IV roofs, that are not animated.
extern _native void SET_CONVERTIBLE_ROOF(VEHICLE_INDEX car, bool set)l
extern _native void SET_ENABLE_RC_DETONATE(bool set)l
extern _native void SET_ENABLE_RC_DETONATE_ON_CONTACT(bool set)l
extern _native void SET_ENGINE_HEALTH(VEHICLE_INDEX vehicle, float health)l
extern _native void SET_EXTRA_CAR_COLOURS(VEHICLE_INDEX vehicle, int color1, int color2)l
extern _native void SET_FREE_RESPRAYS(bool set)l

//PURPOSE: Sets if freebies are spawned in vehicles.
extern _native void SET_FREEBIES_IN_VEHICLE(VEHICLE_INDEX veh, bool set)l
extern _native void SET_GANG_CAR(VEHICLE_INDEX car, bool set)l

//PURPOSE: Sets the garage does not grab the game camera.
extern _native void SET_GARAGE_LEAVE_CAMERA_ALONE(const char* garageName, bool set)l

//INFO: If vehicle has not been owned by player, the player will get a wanted level when entering (if spotted by police).
//          If the player has already owned a vehicle he is free to use it. (The game will set this flag to true first time the player drives a vehicle)
//          Mission vehicles will default to true 
//PURPOSE: Sets a vehicle owned by the player so he wont get a wanted level when entering it.
extern _native void SET_HAS_BEEN_OWNED_BY_PLAYER(VEHICLE_INDEX car, bool set)l
extern _native void SET_HAS_BEEN_OWNED_FOR_CAR_GENERATOR(VEHICLE_GENERATOR_INDEX generator, bool set)l

//PURPOSE: Sets the heli blades to full speed
extern _native void SET_HELI_BLADES_FULL_SPEED(VEHICLE_INDEX heli)l
extern _native void SET_HELI_FORCE_ENGINE_ON(VEHICLE_INDEX heli, bool set)l
extern _native void SET_HELI_STABILISER(VEHICLE_INDEX heli, bool set)l
extern _native void SET_LOAD_COLLISION_FOR_CAR_FLAG(VEHICLE_INDEX car, bool set)l

//PURPOSE: Set mission train coords
extern _native void SET_MISSION_TRAIN_COORDINATES(VEHICLE_INDEX train, float x, float y, float z)l

//PARAM NOTES: Makes the player play the hotwire anim.
//PURPOSE: Sets the vehicle must be hotwired.
extern _native void SET_NEEDS_TO_BE_HOTWIRED(VEHICLE_INDEX veh, bool set)l
extern _native void SET_NO_RESPRAYS(bool set)l

//INFO:  If the Denisty multiplier is set to 0.0 then parked vehicles will stop spawning.
//PARAM NOTES: DensityMultiplier default = 1.0
//PURPOSE: Sets the amount of  parked vehicles being spawned.
extern _native void SET_PARKED_CAR_DENSITY_MULTIPLIER(float density)l

//INFO:  Petrol tank "will" explode when health reaches -1000.0 Petrol tank health will drop if on fire until it explodes 
//PARAM NOTES: 1000.0 = full 0.0 = go on fire -1000.0 = burnt out
//PURPOSE: Set petrol tank health for a vehicle.
extern _native void SET_PETROL_TANK_HEALTH(VEHICLE_INDEX vehicle, float value)l
extern _native void SET_PETROL_TANK_WEAKPOINT(VEHICLE_INDEX car, bool set)l
extern _native void SET_PLANE_THROTTLE(VEHICLE_INDEX plane, float throttle)l
extern _native void SET_PLANE_UNDERCARRIAGE_UP(VEHICLE_INDEX plain, bool set)l

//PURPOSE: Sets the playback speed
extern _native void SET_PLAYBACK_SPEED(VEHICLE_INDEX car, float speed)l

//INFO: If this is set to true the police focus circle is always focussed on the car. If this is false the player has a chance to escape police attention if undetected.
//          By default law enforcement vehicles have this bit set to true and all other cars have false. 
//PURPOSE: Sets the police focus on the vehicle.
extern _native void SET_POLICE_FOCUS_WILL_TRACK_CAR(VEHICLE_INDEX car, bool set)l
extern _native void SET_RAILTRACK_RESISTANCE_MULT(float resistance)l

//INFO: If the Denisty multiplier is set to 0.0 then ambients traffic will stop spawning.
//PARAM NOTES: DensityMultiplier default = 1.0
//PURPOSE: Sets the amount of ambient traffic being spawned.
extern _native void SET_RANDOM_CAR_DENSITY_MULTIPLIER(float density)l
extern _native void SET_RECORDING_TO_POINT_NEAREST_TO_COORS(VEHICLE_INDEX car, float x, float y, float z)l

//INFO: If this value is set to true the carriages will be rendered at a slight random angle. This will make it look like the train has derailed. 
//          In every other way the train is a normal train and it can still move. The player can still get on it etc. 
//PURPOSE: Sets the train to look derailed.
extern _native void SET_RENDER_TRAIN_AS_DERAILED(VEHICLE_INDEX train, bool set)l

//PURPOSE: Sets that rocket launcher freebie appers in the heli.
extern _native void SET_ROCKET_LAUNCHER_FREEBIE_IN_HELI(bool set)l
extern _native void SET_ROOM_FOR_CAR_BY_KEY(VEHICLE_INDEX car, uint roomkey)l
extern _native void SET_ROOM_FOR_CAR_BY_NAME(VEHICLE_INDEX car, const char* roomname)l
extern _native void SET_TARGET_CAR_FOR_MISSION_GARAGE(const char* garage, VEHICLE_INDEX car)l

//PURPOSE: Sets the light on the taxi.
extern _native void SET_TAXI_LIGHTS(VEHICLE_INDEX car, bool set)l

//PURPOSE: Set the train cruise speed
extern _native void SET_TRAIN_CRUISE_SPEED(VEHICLE_INDEX train, float speed)l

//INFO: This should be used with care because if the player gets his hands on a train that isn't forced to slow down he could drive though the barriers into the next part of the map. 
//PURPOSE: Makes the tarin slow down.
extern _native void SET_TRAIN_FORCED_TO_SLOW_DOWN(VEHICLE_INDEX train, bool set)l

//PURPOSE: Set the train to be stopped in the station 
extern _native void SET_TRAIN_IS_STOPPED_AT_STATION(VEHICLE_INDEX train)l

//PURPOSE: Sets the train spped in m/s
extern _native void SET_TRAIN_SPEED(VEHICLE_INDEX train, float speed)l

//PURPOSE: Sets the train stops for staions
extern _native void SET_TRAIN_STOPS_FOR_STATIONS(VEHICLE_INDEX train, bool set)l
extern _native void SET_UPSIDEDOWN_CAR_NOT_DAMAGED(VEHICLE_INDEX car, bool set)l

//PURPOSE: Sets the vehicle alarm status.
extern _native void SET_VEH_ALARM(VEHICLE_INDEX veh, bool set)l
extern _native void SET_VEH_ALARM_DURATION(VEHICLE_INDEX veh, int duration)l

//PURPOSE: Sets so that axles dont break easily 
extern _native void SET_VEH_HAS_STRONG_AXLES(VEHICLE_INDEX veh, bool set)l
extern _native void SET_VEH_HAZARDLIGHTS(VEHICLE_INDEX vehicle, bool on)l
extern _native void SET_VEH_INDICATORLIGHTS(VEHICLE_INDEX veh, bool set)l

//PURPOSE: Sets the vehicles interior lights
extern _native void SET_VEH_INTERIORLIGHT(VEHICLE_INDEX veh, bool set)l

//INFO: Setup a vehicle to be rendered with alpha.
extern _native void SET_VEHICLE_ALPHA(VEHICLE_INDEX veh, int alpha)l
extern _native void SET_VEHICLE_ALWAYS_RENDER(VEHICLE_INDEX veh)l

//PURPOSE: Sets if a vehicle can be targetted
extern _native void SET_VEHICLE_CAN_BE_TARGETTED(VEHICLE_INDEX veh, bool set)l
extern _native void SET_VEHICLE_DEFORMATION_MULT(VEHICLE_INDEX veh, float multiplier)l

//PURPOSE: Sets the vehicles dirt level 0.0 (clean) to 15.0 (dirty).
extern _native void SET_VEHICLE_DIRT_LEVEL(VEHICLE_INDEX vehicle, float level)l

//PURPOSE: Stop a vehicle exploding instantly when hit by another explosion
extern _native void SET_VEHICLE_EXPLODES_ON_HIGH_EXPLOSION_DAMAGE(VEHICLE_INDEX veh, bool set)l

//INFO: Passing FALSE to this command will make the player not consider this vehicle when pressing triangle to enter a vehicle. The player will instead pick the next closest vehicle to steal/enter.
//PURPOSE: Sets if the player can enter a vehicle pressing triangle.
extern _native void SET_VEHICLE_IS_CONSIDERED_BY_PLAYER(VEHICLE_INDEX veh, bool set)l

//PURPOSE: Sets the vehicles's rotation quaternion
extern _native void SET_VEHICLE_QUATERNION(VEHICLE_INDEX veh, float qx, float qy, float qz, float qw)l

//INFO: Setup a vehicle to be rendered as scorched/burned down
extern _native void SET_VEHICLE_RENDER_SCORCHED(VEHICLE_INDEX veh, bool set)l

//INFO: The steer bias value gets added to the players control input for that car. 
//          The bias is in the range -1.0f (hard right) to 1.0f (hard left). Ie If you set the bias to 0.1 the car will pull to the left a little bit. 
//          By modifying the value from frame to frame you can emulate buggered controls or somebody fighting over control of the wheel. 
//PURPOSE: Sets a vehicle control bias -1.0f (hard right) to 1.0f (hard left).
extern _native void SET_VEHICLE_STEER_BIAS(VEHICLE_INDEX veh, float bias)l

//PURPOSE: Shuts a vehicle door.
extern _native void SHUT_CAR_DOOR(VEHICLE_INDEX vehicle, int door)l
extern _native void SKIP_IN_PLAYBACK_RECORDED_CAR(VEHICLE_INDEX car, float time)l

//PURPOSE: Skips a certain amount in a car recording
extern _native void SKIP_TIME_IN_PLAYBACK_RECORDED_CAR(VEHICLE_INDEX veh, float time)l

//PURPOSE: Sets the train to stop at stations the player is allowed to be in.
extern _native void SKIP_TO_NEXT_ALLOWED_STATION(VEHICLE_INDEX train)l

//PURPOSE: Skips to the end and stops a playback
extern _native void SKIP_TO_END_AND_STOP_PLAYBACK_RECORDED_CAR(VEHICLE_INDEX car)l

//PURPOSE: Smashes a vehicles window.
extern _native void SMASH_CAR_WINDOW(VEHICLE_INDEX car, int windownum)l

//PURPOSE: Sounds a vehicle horn for the given time.
extern _native void SOUND_CAR_HORN(VEHICLE_INDEX vehicle, int duration)l

//PURPOSE: Starts the playback for a recorded vehicle
//NOTES: This will start the engine automatically
extern _native void START_PLAYBACK_RECORDED_CAR(VEHICLE_INDEX car, uint recording)l
extern _native void START_PLAYBACK_RECORDED_CAR_LOOPED(VEHICLE_INDEX car, uint recording)l

//PURPOSE: Starts the playback using the vehicle AI
extern _native void START_PLAYBACK_RECORDED_CAR_USING_AI(VEHICLE_INDEX car, uint recording)l
extern _native void START_PLAYBACK_RECORDED_CAR_WITH_OFFSET(VEHICLE_INDEX car, uint recording, float x, float y, float z)l

//PURPOSE: Sets the vehicle so it can break apart
extern _native void STOP_CAR_BREAKING(VEHICLE_INDEX car, bool stop)l

//PURPOSE: Stops the playback for a recorded vehicle
extern _native void STOP_PLAYBACK_RECORDED_CAR(VEHICLE_INDEX car)l
extern _native void STOP_VEHICLE_ALWAYS_RENDER(VEHICLE_INDEX veh)l
extern _native void SUPPRESS_CAR_MODEL(uint model)l
extern _native void SWITCH_AMBIENT_PLANES(bool on)l
extern _native void SWITCH_CAR_GENERATOR(VEHICLE_GENERATOR_INDEX generator, int type)l

//PURPOSE: Sets the vehicle siren
extern _native void SWITCH_CAR_SIREN(VEHICLE_INDEX car, bool siren)l

//PURPOSE: Sets garbage trucks active
extern _native void SWITCH_GARBAGE_TRUCKS(bool on)l

//PURPOSE: Sets if mad drivers are spawned
extern _native void SWITCH_MAD_DRIVERS(bool on)l
extern _native void SWITCH_POLICE_HELIS(bool set)l

//PURPOSE: Set random boats active.
extern _native void SWITCH_RANDOM_BOATS(bool on)l

//PURPOSE: Sets random trains active
extern _native void SWITCH_RANDOM_TRAINS(bool on)l
extern _native void SYNCH_AMBIENT_PLANES(uint Unk520, float Unk521)l
extern _native void TAKE_CAR_OUT_OF_PARKED_CARS_BUDGET(VEHICLE_INDEX car, bool out)l

//PURPOSE: Set train leaves the station
extern _native void TRAIN_LEAVE_STATION(VEHICLE_INDEX train)l

//PURPOSE: Starts the vehicle alarm
extern _native void TRIGGER_VEH_ALARM(VEHICLE_INDEX car)l

//PURPOSE: Rotates the vehicle so that it faces toward the given co-ordinate. 
extern _native void TURN_CAR_TO_FACE_COORD(VEHICLE_INDEX car, float x, float y)l

//PURPOSE: Sets a vehicle extra 
extern _native void TURN_OFF_VEHICLE_EXTRA(VEHICLE_INDEX veh, int extra, bool turnoff)l

//PURPOSE: Unpauses the playback for a recorded vehicle
extern _native void UNPAUSE_PLAYBACK_RECORDED_CAR(VEHICLE_INDEX car)l
extern _native void VEHICLE_CAN_BE_TARGETTED_BY_HS_MISSILE(VEHICLE_INDEX car, bool set)l
extern _native void VEHICLE_DOES_PROVIDE_COVER(VEHICLE_INDEX veh, bool cover)l

//Deprecated
//CREATE_CAR_GENERATOR_WITH_PLATE
//SET_CAR_HEAVY
//GET_RANDOM_CAR_OF_TYPE_IN_AREA
//IS_CAR_VISIBLY_DAMAGED
//START_RECORDING_CAR
//STOP_RECORDING_CARS
//IS_RECORDING_GOING_ON_FOR_CAR
//FIND_TIME_POSITION_IN_RECORDED_RECORDING
//SET_CAR_CAN_GO_AGAINST_TRAFFIC
//STOP_RECORDING_CAR
//DISPLAY_PLAYBACK_RECORDED_CAR
//SET_CAR_HYDRAULICS
//SET_VEHICLE_AREA_VISIBLE
//GET_VEHICLE_CLASS
//SET_CAR_LIGHTS_ON
//HAS_TRAIN_DERAILED
//IS_NEXT_STATION_ALLOWED
//IMPROVE_CAR_BY_CHEATING
//SET_CAR_HAS_BEEN_CREATED_IN_INTERIOR
//FAST_FORWARD_TO_NEXT_BUS_STOP
//TELEPORT_PLAYER_NEAR_BUS
//FORCE_AIR_DRAG_MULT
#pragma endregion

#pragma region TASK
extern _native bool CAN_PED_SHIMMY_IN_DIRECTION(PED_INDEX ped, int direction)l

//PARAM NOTES: must_be_free is true it will only return true if the point is not in use. 
//PURPOSE: Returns true if there is a scenario in the area given
extern _native bool DOES_SCENARIO_EXIST_IN_AREA(float x, float y, float z, float radius, bool must_be_free)l
extern _native bool END_PED_QUEUE_MEMBERSHIP_LIST(void)l
extern _native bool HAS_OVERRIDEN_SIT_IDLE_ANIM_FINISHED(PED_INDEX ped)l

//INFO: If the ped is performing the Use Mobile Phone task then this command will return the subtask for that task. 
//  If the ped isn't performing the task then this command will assert and return FALSE. If the ped is performing the Use Mobile Phone task then this command will return TRUE. 
//PURPOSE: Gets the current mobile phone subtask.
extern _native bool GET_MOBILE_PHONE_TASK_SUB_TASK(PED_INDEX ped, int* sub_task)l
extern _native bool GET_PED_PATH_MAY_DROP_FROM_HEIGHT(PED_INDEX ped)l
extern _native bool GET_PED_PATH_MAY_USE_CLIMBOVERS(PED_INDEX ped)l
extern _native bool GET_PED_PATH_MAY_USE_LADDERS(PED_INDEX ped)l
extern _native bool GET_PED_PATH_WILL_AVOID_DYNAMIC_OBJECTS(PED_INDEX ped)l

//PURPOSE: Checks if a ped is getting up.
extern _native bool IS_CHAR_GETTING_UP(PED_INDEX ped)l
extern _native bool IS_CHAR_SITTING_IDLE(PED_INDEX ped)l
extern _native bool IS_CHAR_USING_MAP_ATTRACTOR(PED_INDEX ped)l
extern _native bool IS_SITTING_OBJECT_NEAR(float x, float y, float z, uint model)l
extern _native bool TASK_SHIMMY_CLIMB_UP(PED_INDEX ped)l
extern _native bool TASK_SHIMMY_IN_DIRECTION(PED_INDEX ped, uint Unk109)l
extern _native bool TASK_SHIMMY_LET_GO(PED_INDEX ped)l
extern _native int GET_NAVMESH_ROUTE_RESULT(PED_INDEX ped)l
extern _native uint GET_PED_CLIMB_STATE(PED_INDEX ped)l

//INFO: if you want them to use scripted points added instead. You must call FLUSH_COVER_BLOCKING_AREAS at the end of the mission to remove them, 
//  this command can only be used during mission scripts
// MAX allowed areas is 16. Let code know if that needs increasing
//PURPOSE: Adds an axis aligned area that will block coverpoints from being automatically created.
extern _native void ADD_COVER_BLOCKING_AREA(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z, bool block_objects, bool block_vehicles, bool block_map)l

//INFO: NOTE: To take the coordinates of a cover point you'd like to create, do the following:
//    Stand behind the object you'd like to take cover behind.
//    Face towards the object you're standing behind.
//
//  Another thing to bear in mind is:
//     When setting COVERPOINT_USAGE, COVUSE_WALLTOLEFT will flip the ped out to his right hand side, and so COVUSE_WALLTORIGHT will flip the ped out to his left. 
//PARAM NOTES: COVERPOINT_USAGE: How the ped uses the cover
//                          COVERPOINT_ARC: How much cover the ped thinks the cover provides. 120 should be enough. 
//								COVARC_180 = 0, // 180 degrees to the front of the player - middle of a long low wall
//								COVARC_120,		// 120 degrees, box providing limited cover
//								COVARC_90,		// 90 degrees, Barrel or something small providing cover only from the front
//								COVARC_0TO60,	// High wall with firing only available round a corner with a wall to the left
//								COVARC_300TO0,  // High wall with firing only available round a corner with a wall to the right
//								COVARC_0TO45,   // High wall, very close to the corner with firing only available with a wall to the left
//								COVARC_315TO0   // High wall, very close to the corner with firing only available with a wall to the right
//                          COVERPOINT_HEIGHT: The height of the cover point only LOW and TO_HIGH are valid
// By default use this:
//	Corner of a high wall (wall to the left)
//		COVUSE_WALLTOLEFT, COVHEIGHT_TOOHIGH, COVARC_0TO60
//	Corner of a high wall (wall to the right)
//		COVUSE_WALLTORIGHT, COVHEIGHT_TOOHIGH, COVARC_300TO0
//	Low cover point, lots of side cover
//		COVUSE_WALLTORIGHT, COVHEIGHT_LOW, COVARC_180
//	Low cover point, limited of side cover
//		COVUSE_WALLTORIGHT, COVHEIGHT_LOW, COVARC_120
//  BOOL isPriorityToPlayer -> The player will prefer this cover point over other nearby ones
//PURPOSE: Adds a cover point in the given position to cover from direction ( in degrees ).
extern _native void ADD_COVER_POINT(float x, float y, float z, int direction, float usage, int height, int arc, uint* point)l

//INFO:  Can't be used in a sequence. 
//PURPOSE: Adds a follow navmesh route to a currently running phone task at specified pace.
extern _native void ADD_FOLLOW_NAVMESH_TO_PHONE_TASK(PED_INDEX ped, float x, float y, float z)l
extern _native void ADD_PED_QUEUE(float unk0, float unk1, float unk2, float unk3, float unk4, int unk5, int unk6, int* queue)l
extern _native void ATTACH_PED_TO_SHIMMY_EDGE(PED_INDEX ped, float x, float y, float z, float Unk39)l
extern _native void BEGIN_PED_QUEUE_MEMBERSHIP_LIST(void)l
extern _native void CHANGE_CHAR_SIT_IDLE_ANIM(PED_INDEX ped, const char* Unk2, const char* Unk3, bool Unk4)l

//PURPOSE: Clears any secondary anims playing on the ped.
extern _native void CLEAR_CHAR_SECONDARY_TASK(PED_INDEX ped)l

//PURPOSE: Clears all tasks that are being performed by the ped.
extern _native void CLEAR_CHAR_TASKS(PED_INDEX ped)l

//INFO: This should only be used when CLEAR_CHAR_TASKS does not work
//PURPOSE:  Clears a peds task immediately.
extern _native void CLEAR_CHAR_TASKS_IMMEDIATELY(PED_INDEX ped)l
extern _native void CLEAR_EVENT_PRECEDENCE(int unk0, int unk1)l

//PURPOSE: Flushes all tasks in the specified sequence, allowing the sequence to be rewritten.
extern _native void CLEAR_SEQUENCE_TASK(SEQUENCE_INDEX taskSequence)l

//PURPOSE: Closes an opened sequnce task.
extern _native void CLOSE_SEQUENCE_TASK(SEQUENCE_INDEX taskSequence)l
extern _native void DROP_OBJECT(PED_INDEX ped, any unused)l
extern _native void EXTEND_PATROL_ROUTE(float x, float y, float z, const char* Unk487, const char* Unk488)l

//PURPOSE: Removes all cover areas added by ADD_COVER_BLOCKING_AREA
extern _native void FLUSH_COVER_BLOCKING_AREAS(void)l
extern _native void FLUSH_PATROL_ROUTE(void)l
extern _native void GET_PED_AT_HEAD_OF_QUEUE(int queue, PED_INDEX* ped)l

//INFO: Before calling this command, you should call GET_SCRIPT_TASK_STATUS to make sure that SCRIPT_TASK_PERFORM_SEQUENCE is running. 
//PARAM NOTES: ReturnProgress is an INT and will be -1 before the sequence task begins. 
//  Once the sequence has begun, ReturnProgress will be given a value reflecting which task in the sequence is being performed (0 = the first task). 
//PURPOSE: Gets the progress of a sequence of tasks. 
extern _native void GET_SEQUENCE_PROGRESS(PED_INDEX ped, int* progress)l
extern _native void GET_SEQUENCE_PROGRESS_RECURSIVE(PED_INDEX ped, int* unk0, int* unk1)l

//INFO: task_name should be the name of the task command, taken from the header file: Commands_task.sch
//  If task_name is chosen to be NULL the command will search for any scripted task that is being processed, rather than a specific task.
//  ReturnStatusOut should be declared as a SCRIPTTASKSTATUS. ReturnStatusOut will be one of the entries in the header file:
//  If you're trying to get the status of a task that is performed as part of a sequence then it will never return as True because the character in question is told to TASK_PERFORM_SEQUENCE. 
//  So, check for GET_SCRIPT_TASK_STATUS ( CharID, SCRIPT_TASK_PERFORM_SEQUENCE, ReturnStatus) instead.
//  Alternatively, you can check to see if a character is performing a sequence then use GET_SEQUENCE_PROGRESS to find out which specific task within the sequence your character is performing. 
//PURPOSE: Gets the current status of a scrioted task. 
extern _native void GET_SCRIPT_TASK_STATUS(PED_INDEX ped, uint task_hash, int* status)l
extern _native void GET_SOUND_LEVEL_AT_COORDS(PED_INDEX ped, float x, float y, float z, float* level)l
extern _native void MODIFY_CHAR_MOVE_BLEND_RATIO(PED_INDEX ped, float ratio)l
extern _native void MODIFY_CHAR_MOVE_STATE(PED_INDEX ped, int state)l

//INFO: Allows a ped to perform a sequnce of tasks. Any tasks added to a sequence must take in a Null ped_index.
//PURPOSE: Opens a sequence task.
extern _native void OPEN_SEQUENCE_TASK(SEQUENCE_INDEX* pTaskSequence)l
extern _native void PED_QUEUE_CONSIDER_PEDS_WITH_FLAG_FALSE(int flagid)l
extern _native void PED_QUEUE_CONSIDER_PEDS_WITH_FLAG_TRUE(int flagid)l
extern _native void PED_QUEUE_REJECT_PEDS_WITH_FLAG_FALSE(int flagid)l
extern _native void PED_QUEUE_REJECT_PEDS_WITH_FLAG_TRUE(int flagid)l

//PURPOSE: Removes a script added cover point.
extern _native void REMOVE_COVER_POINT(uint coverPoint)l
extern _native void REMOVE_PED_QUEUE(int queue)l
extern _native void SET_ALTERNATIVE_HEAD_FOR_PED_QUEUE(int queue, float x, float y, float z, float unk)l

//PARAM NOTES: Check the drive task running on the ped is active before calling. 
//PURPOSE: Sets the drive tasks cruise speed.
extern _native void SET_DRIVE_TASK_CRUISE_SPEED(PED_INDEX ped, float speed)l
extern _native void SET_EVENT_PRECEDENCE(int unk0, int unk1)l
extern _native void SET_GUNSHOT_SENSE_RANGE_FOR_RIOT2(float range)l

//INFO: Works for the following commands TASK_ENTER_CAR_AS_DRIVER
//          TASK_ENTER_CAR_AS_PASSENGER
//          TASK_OPEN_DRIVER_DOOR
//          TASK_OPEN_PASSENGER_DOOR
//          TASK_GOTO_CHAR_OFFSET   
//PURPOSE: Sets the movement state on the next movment task.
extern _native void SET_NEXT_DESIRED_MOVE_STATE(int move_blend_ratio)l
extern _native void SET_PED_PATH_MAY_DROP_FROM_HEIGHT(PED_INDEX ped, bool value)l
extern _native void SET_PED_PATH_MAY_USE_CLIMBOVERS(PED_INDEX ped, bool value)l
extern _native void SET_PED_PATH_MAY_USE_LADDERS(PED_INDEX ped, bool value)l
extern _native void SET_PED_PATH_WILL_AVOID_DYNAMIC_OBJECTS(PED_INDEX ped, bool set)l
extern _native void SET_PED_QUEUE_MEMBERSHIP_LIST(int queue, int unk)l

//PURPOSE: Sets a sequence to repeat.
extern _native void SET_SEQUENCE_TO_REPEAT(SEQUENCE_INDEX sequence, int do_repeat)l

//PURPOSE: Gets a ped to try and acheive the given heading.
extern _native void TASK_ACHIEVE_HEADING(PED_INDEX ped, float heading)l

//INFO:  The first ped must have a gun and it must be his current weapon to use this command. 
//PARAM NOTES: Duration is measured in milliseconds.
//PURPOSE: Tells a ped to aim a gun at a ped.
extern _native void TASK_AIM_GUN_AT_CHAR(PED_INDEX ped, PED_INDEX targetPed, int duration)l

//PURPOSE: Tells the ped to aim a gun at the coord
extern _native void TASK_AIM_GUN_AT_COORD(PED_INDEX ped, float tX, float tY, float tZ, int duration)l

//PARAM NOTES: veh: if you want the ped to use a specific vehicle then specify the veh. Otherwise set veh to NULL.
//  
//  model_hash : if the ped can't find the specified vehicle or any other vehicle, a vehicle (with model model_hash) will be automatically created if the ped is off-screen. No vehicle will be created if model_hash is set to DUMMY_MODEL_FOR_SCRIPT. (In San Andreas, passing a model_hash of 1 would use the default car model which was always in memory. This has not been implemented in the Rage code yet.)
//      * If a vehicle is specified and the ped is in the vehicle the ped will just drive to the point.
//      * If a vehicle is specified and the ped isn't in the vehicle then the ped will attempt to get in the vehicle and drive to the point.
//      * If a vehicle is specified and the ped is in another vehicle then ped will get out the car and attempt to get in the correct car and then drive to the point. 
//  
//      * If no vehicle is specified then the ped will attempt to get in any nearby vehicle and drive to the point.
//      * If no vehicle is specified and the ped is already in a car then the ped will drive the car to the point. 
//  
//  If a ped tries and fails to get in a car then:
//  
//  1. if model_hash = DUMMY_MODEL_FOR_SCRIPT the ped will run to the target and get in any car it can along the route.
//  2. otherwise, when the ped is off-screen the ped will create a car (with the correct model), warp in, and drive to the point. If the ped is on-screen then the ped will run to the target and get in any car it can along the route (if the ped is near the target it will just run to the target). 
//  
//  radius is how close the car will get to the destination before stopping. Default should be 4.0
//  straight_line_dist (the distance at which the ai switches to heading for the target directly instead of following the nodes) (just like in sa). To pick default (recommended) pass in -1.
//  
//  TASK_CAR_DRIVE_TO_COORD_NOT_AGAINST_TRAFFIC does exactly the same with the only difference being cars will not consider routes that go against the flow of traffic. 
//PURPOSE: Gives a task to drive a vehicle to a coord.
extern _native void TASK_CAR_DRIVE_TO_COORD(PED_INDEX ped, VEHICLE_INDEX veh, float x, float y, float z, float speed, int driving_style, uint model, int driving_mode, float radius, int straight_line_dist)l
extern _native void TASK_CAR_DRIVE_TO_COORD_NOT_AGAINST_TRAFFIC(PED_INDEX ped, VEHICLE_INDEX veh, float x, float y, float z, float speed, int driving_style, uint model, int driving_mode, float radius, int straight_line_dist)l

//PURPOSE: Tells a ped to wander about in a car.
extern _native void TASK_CAR_DRIVE_WANDER(PED_INDEX ped, VEHICLE_INDEX vehicle, float speed, int driving_mode)l

//PARAM NOTES:INT target_reached_dist: (the distance at which the ai thinks the target has been reached and the car stops)
//  INT straight_line_dist: (the distance at which the ai switches to heading for the target directly instead of following the nodes) (just like in sa)
//  Use TASK_CAR_MISSION_PED_TARGET to target another ped instead of a vehicle. The task status is the same as that for TASK_CAR_MISSION, i.e. use
//  GET_SCRIPT_TASK_STATUS(a_ped, SCRIPT_TASK_CAR_MISSION, my_status) to check for both commands.
//  To pick default values (recommended) both parameters can be passed in as -1. 
//	DriveAgainstTraffic being true allows the car to drive on the opposite side of the road into incoming traffic
//PURPOSE: Tells a ped to perform a task when in a vehicle against another vehicle.
extern _native void TASK_CAR_MISSION(PED_INDEX ped, VEHICLE_INDEX vehicle, VEHICLE_INDEX target_veh, uint mission, float speed, int driving_mode, int target_reached_dist, int straight_line_dist)l

//PARAM NOTES::INT target_reached_dist (the distance at which the ai thinks the target has been reached and the car stops)
//  INT straight_line_dist (the distance at which the ai switches to heading for the target directly instead of following the nodes) (just like in sa)
//  To pick default values (recommended) both parameters can be passed in as -1.
//  Use TASK_CAR_MISSION_PED_TARGET to target another ped instead of a vehicle. The task status is the same as that for TASK_CAR_MISSION, i.e. use
//  GET_SCRIPT_TASK_STATUS(a_ped, SCRIPT_TASK_CAR_MISSION, my_status)   to check for both commands.
//  TASK_CAR_MISSION_NOT_AGAINST_TRAFFIC does exaclt the same apart from the fact it won't allow cars to drive against the flow of traffic. TASK_CAR_MISSION does allow cars to drive agains the flow of traffic. 
//	DriveAgainstTraffic being true allows the car to drive on the opposite side of the road into incoming traffic
//PURPOSE:  Tells a ped in a vehicle to target a coord.
extern _native void TASK_CAR_MISSION_COORS_TARGET(PED_INDEX ped, VEHICLE_INDEX vehicle, float x, float y, float z, uint mission, float speed, int driving_mode, int target_reached_dist, int straight_line_dist)l
extern _native void TASK_CAR_MISSION_COORS_TARGET_NOT_AGAINST_TRAFFIC(PED_INDEX ped, VEHICLE_INDEX vehicle, float x, float y, float z, uint mission, float speed, int driving_mode, int target_reached_dist, int straight_line_dist)l
extern _native void TASK_CAR_MISSION_NOT_AGAINST_TRAFFIC(PED_INDEX ped, VEHICLE_INDEX vehicle, VEHICLE_INDEX target_veh, uint mission, float speed, int driving_mode, int target_reached_dist, int straight_line_dist)l

//PARAM NOTES:INT target_reached_dist (the distance at which the ai thinks the target has been reached and the car stops)
//  INT straight_line_dist (the distance at which the ai switches to heading for the target directly instead of following the nodes) (just like in sa)
//  To pick default values (recommended) both parameters can be passed in as -1.
//  Use TASK_CAR_MISSION_PED_TARGET to target another ped instead of a vehicle. The task status is the same as that for TASK_CAR_MISSION, i.e. use
//  GET_SCRIPT_TASK_STATUS(a_ped, SCRIPT_TASK_CAR_MISSION, my_status)   to check for both commands.
//  TASK_CAR_MISSION_NOT_AGAINST_TRAFFIC does exaclt the same apart from the fact it won't allow cars to drive against the flow of traffic. TASK_CAR_MISSION does allow cars to drive agains the flow of traffic. 
//	DriveAgainstTraffic being true allows the car to drive on the opposite side of the road into incoming traffic
//PURPOSE: Tells a ped in a vehicle to target another ped in a vehicle.
extern _native void TASK_CAR_MISSION_PED_TARGET(PED_INDEX ped, VEHICLE_INDEX vehicle, PED_INDEX target, uint mission, float speed, int driving_mode, int target_reached_dist, int straight_line_dist)l
extern _native void TASK_CAR_MISSION_PED_TARGET_NOT_AGAINST_TRAFFIC(PED_INDEX ped, VEHICLE_INDEX veh, PED_INDEX target_ped, uint mission, float speed, int driving_mode, int target_reached_dist, int straight_line_dist)l

//INFO: These should only be used for short actions
//PURPOSE: Gives the vehicle a temporary action.
extern _native void TASK_CAR_TEMP_ACTION(PED_INDEX ped, VEHICLE_INDEX vehicle, uint action, int duration)l

//PARAM NOTES: speed in ms/s will be clamped between 0.1 - 1.0, reccomended value is the default
//                          headingDegrees: Heading that the ped will achieve.
//PURPOSE: Slides a ped to the given coord.
extern _native void TASK_CHAR_SLIDE_TO_COORD(PED_INDEX ped, float x, float y, float z, float heading, float speed)l

//PARAM NOTES: heading: Heading the ped will achieve at the point.
//                          blend_delta: How quickly the anim is blended. The larger number the quicker its blended slow 4.0, 8.0, 16.0, 1000.0 instant.
//                          loop: true to loop the anim. 
//                          extracted velocities are required if the ped does not end up in the same place where the anim started. They move the capsule of the ped.
//                          extract_forward_velocity: update the y component of the ped
//                          extract_sideways_velocity: update the x component of the ped
//                          hold_last_frame: Holds the last frame of the anim. Note if set true HAS_CHAR_ANIM_FINISHED will always return false 
//PURPOSE: Slides a ped to a specific coord and plays an anim.
extern _native void TASK_CHAR_SLIDE_TO_COORD_AND_PLAY_ANIM(PED_INDEX ped, float x, float y, float z, float heading, float speed, const char* anim_name, const char* anim_dict_name, float blend_delta, bool loop, bool extract_forward_velocity, bool extract_sideways_velocity, bool hold_last_frame, int time)l
extern _native void TASK_CHAR_SLIDE_TO_COORD_AND_PLAY_ANIM_HDG_RATE(PED_INDEX ped, float x, float y, float z, float heading, float speed, const char* anim_name, const char* anim_dict_name, float blend_delta, bool loop, bool extract_forward_velocity, bool extract_sideways_velocity, bool hold_last_frame, int time, float heading_change_rate)l

//PURPOSE:  Slide a ped to a coord and set the heading change rate.
extern _native void TASK_CHAR_SLIDE_TO_COORD_HDG_RATE(PED_INDEX ped, float x, float y, float z, float heading, float speed, float heading_change_rate)l
extern _native void TASK_CHAT_WITH_CHAR(PED_INDEX ped)l

//PURPOSE:  Stops a ped looking at an entity.
extern _native void TASK_CLEAR_LOOK_AT(PED_INDEX ped)l

//INFO: The ped needs to be positioned and oriented so that a jump will locate an edge for the ped to grab.
//  If an edge cant be found, the ped will just do a normal jump and land. If an edge can be found then the ped will climb and then stand on top of the found edge.
//  Don't place the ped too far from the object you want him to climb, even if the player can make it and you set use_player_launch_force to TRUE, it won't guarantee that the ped will make it.
//PARAM NOTES: If use_player_launch_force is TRUE then the ped will use the same launch force as the player. 
//                          If use_player_launch_force is FALSE and the ped isnt the player then the ped will jump using a smaller launch force. 
//PURPOSE: Tells a ped to perform the climb task.
extern _native void TASK_CLIMB(PED_INDEX ped, bool use_player_launch_force)l

//INFO: This command handles everything needed to make a ped get on a ladder at the top or bottom, climb/descend it, 
//  and get off at the other end. The task decides whether the ped is supposed to climb or descend by examining which end of the ladder is nearest.
//  The ped needs to be positioned right next to the ladder they are supposed to use, and should also be facing it.
//  There are two possibilities for mounting the ladder - at the base of the ladder facing towards the front of it, and at the top of the ladder facing the reverse of the ladder. See the image below.
//  If successful, the ped will get on the ladder, climb, and then get off. 
//PURPOSE: Tells a ped to perform a climb ladder task.
extern _native void TASK_CLIMB_LADDER(PED_INDEX ped, bool fast)l

//PURPOSE: Tells a ped to combat another ped.
extern _native void TASK_COMBAT(PED_INDEX ped, PED_INDEX target)l

//PURPOSE: Tells the ped to combat all hated targets in the radius about the ped.
extern _native void TASK_COMBAT_HATED_TARGETS_AROUND_CHAR(PED_INDEX ped, float radius)l

//PURPOSE:  Tells the ped to combat all hated targets in the radius about the ped for a time period.
extern _native void TASK_COMBAT_HATED_TARGETS_AROUND_CHAR_TIMED(PED_INDEX ped, float radius, float time)l

//PURPOSE: Tells the ped to combat all hated targets in the area.
extern _native void TASK_COMBAT_HATED_TARGETS_IN_AREA(PED_INDEX ped, float x, float y, float z, float radius)l

//PURPOSE: Makes the ped peform a combat roll in the direction specified.
extern _native void TASK_COMBAT_ROLL(PED_INDEX ped, int direction)l

//PURPOSE: Tells a ped to combat another ped for a timed period.
extern _native void TASK_COMBAT_TIMED(PED_INDEX ped, PED_INDEX target, int duration)l

//PURPOSE: Gives the ped a cower task.
extern _native void TASK_COWER(PED_INDEX ped)l
extern _native void TASK_DEAD(PED_INDEX ped)l
extern _native void TASK_DESTROY_CAR(PED_INDEX ped, VEHICLE_INDEX car)l
extern _native void TASK_DIE(PED_INDEX ped)l

//PARAM NOTES:The character can fire at a vehicle, coordinates or another character. At least one of target_ped and target_veh must be NULL. If there is a target character or vehicle then the target coordinates are used as an offset.
//  If target_ped and target_veh are both NULL then the character will fire at the target coordinates.
//  CharID and target_ped are both of type PED_INDEX
//  target_veh is of type VEHICLE_INDEX
//  abort_range is the distance in metres from the target at which the task will be aborted.
//  frequency_percentage should be between 0 (never shoot) and 100 (fire as fast as possible). 
//  push_underneath_driving_task_if_driving If this is set it will allow the ped to continue driving whilst shooting.
//  instead of interupting the driving task it will sit along side it. CLEAR_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK can be used to remove it
//  firing_pattern_hash allows you to set the firing pattern from a list: FIRING_PATTERN_HASH 
//PURPOSE: Tells a ped to driveby a coord, ped or vehicle.
extern _native void TASK_DRIVE_BY(PED_INDEX ped, PED_INDEX target_ped, VEHICLE_INDEX target_veh, float x, float y, float z, float abort_range, int frequency_percentage, bool push_underneath_driving_task_if_driving, uint firing_pattern_hash)l

//PURPOSE: Tells the ped vehicle to drive a point route
extern _native void TASK_DRIVE_POINT_ROUTE(PED_INDEX ped, VEHICLE_INDEX veh, float speed)l

//PURPOSE: Tells a ped to drive a point route.
extern _native void TASK_DRIVE_POINT_ROUTE_ADVANCED(PED_INDEX ped, VEHICLE_INDEX veh, float speed, int driving_style, uint model, int driving_mode)l

//PARAM NOTES: Set time to -1 to perform the task forever.
//PURPOSE: Gives the ped a ducking task. 
extern _native void TASK_DUCK(PED_INDEX ped, int time)l

//INFO: These tasks will warp the ped into the vehicle if they have not managed to enter after the specified duration.
extern _native void TASK_ENTER_CAR_AS_DRIVER(PED_INDEX ped, VEHICLE_INDEX vehicle, int duration)l
extern _native void TASK_ENTER_CAR_AS_PASSENGER(PED_INDEX ped, VEHICLE_INDEX vehicle, int duration, int seat)l

//INFO: Task doesnt work as part of a sequence. 
//PURPOSE:  Tells the driver (if there is one) and any passengers to leave the car.
extern _native void TASK_EVERYONE_LEAVE_CAR(VEHICLE_INDEX vehicle)l

//INFO: Use TASK_FLUSH_ROUTE to make the route empty.
//PURPOSE: Adds a point to a route.
extern _native void TASK_EXTEND_ROUTE(float x, float y, float z)l
extern _native void TASK_FALL_AND_GET_UP(PED_INDEX ped, int type, uint Unk206)l
extern _native void TASK_FLEE_CHAR_ANY_MEANS(PED_INDEX ped, PED_INDEX ped2, float Unk208, uint Unk209, bool Unk210, uint Unk211, uint Unk212, float Unk213)l

//PURPOSE: Clears a point route.
extern _native void TASK_FLUSH_ROUTE(void)l
extern _native void TASK_FOLLOW_FOOTSTEPS(PED_INDEX ped, PED_INDEX ped2)l
extern _native void TASK_FOLLOW_PATROL_ROUTE(PED_INDEX ped, int unk0, int unk1)l

//PURPOSE: Tells a ped to follow a point route.
extern _native void TASK_FOLLOW_POINT_ROUTE(PED_INDEX ped, int move_blend_ratio, int follow_mode)l
extern _native void TASK_FOLLOW_NAV_MESH_AND_SLIDE_TO_COORD(PED_INDEX ped, float x, float y, float z, int move_blend_ratio, int time, float radius, float angle)l
extern _native void TASK_FOLLOW_NAV_MESH_AND_SLIDE_TO_COORD_HDG_RATE(PED_INDEX ped, float x, float y, float z, int move_blend_ratio, int time, float radius, float angle, float rate)l

//INFO: IMPORTANT NOTE : Sometimes a path may not be able to be found. This could happen because there simply isn't any way to get there, or maybe a bunch of dynamic objects have blocked the way, 
//  or maybe the destination is too far away. In this case the ped will simply stand still.
//  To identify when this has happened, you can use GET_NAVMESH_ROUTE_RESULT. This will help you find situations where peds cannot get to their target. 
//PARAM NOTES: Time is an INT value specifying milliseconds. If the ped has not achieved the target within the allotted time then the ped will be teleported to the target. 
// if Time is chosen to be -1 the ped will never warp. 
//PURPOSE: Tells a ped to follow the navmesh to the given coord.
extern _native void TASK_FOLLOW_NAV_MESH_TO_COORD(PED_INDEX ped, float x, float y, float z, int move_blend_ratio, int time, float radius)l
extern _native void TASK_FOLLOW_NAV_MESH_TO_COORD_NO_STOP(PED_INDEX ped, float x, float y, float z, int move_blend_ratio, int time, float radius)l

//INFO: There is no need to specify a boat, as the task itself will figure out which boat is underneath the ped. 
//  This task does a number of linetests and is thus pretty expensive, so use with care (ie. not repeatedly!) 
//  NOTES: This task is automatically given to group peds, if they are on a boat but their leader is not. 
//PARAM NOTES: timeout can be set to -1 for no time-limit, or to a positive integer to indicate a time in milliseconds after which the ped will warp out of the boat to the nearest clear position they can find nearby. 
//PURPOSE: Tells a ped to get off a boat.
extern _native void TASK_GET_OFF_BOAT(PED_INDEX ped, int timeout)l
extern _native void TASK_GOTO_CAR(PED_INDEX ped, VEHICLE_INDEX veh, uint Unk222, float Unk223)l

//PARAM NOTES: seek_radius: How close a ped gets before the task is complete.     
//PURPOSE: Sends a ped to a ped aiming a gun.
extern _native void TASK_GOTO_CHAR_AIMING(PED_INDEX ped, PED_INDEX target_ped, float seek_radius, float aim_radius)l

//INFO:   if Time is chosen to be -1 the ped will never warp. 
//PARAM NOTES:The destination point is SeekRadiusFloat metres from the other peds position.
//  The exact destination position is calculated using the other peds heading offset by SeekAngleFloat degree
//PURPOSE: Tells the ped to go to an offset from the target ped.
extern _native void TASK_GOTO_CHAR_OFFSET(PED_INDEX ped, PED_INDEX target, int duration, float seek_radius, float seek_angle)l

//PARAM NOTES: if time is chosen to be -1 the ped will never warp. 
//PURPOSE: Tells the ped to go to a coord, without using the navemesh.
extern _native void TASK_GO_STRAIGHT_TO_COORD(PED_INDEX ped, float x, float y, float z, int move_blend_ratio, int time)l

//PARAM NOTES:  if Time is chosen to be -1 the ped will never warp. 
//PURPOSE: Tells the ped to go to an offset coord from a vehicle, without using the navmesh. 
extern _native void TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_CAR(PED_INDEX ped, VEHICLE_INDEX veh, float x, float y, float z, int move_blend_ratio, int time)l
extern _native void TASK_GO_TO_CHAR(PED_INDEX ped, PED_INDEX ped2, uint Unk234, float Unk235)l

//PARAM NOTES: If you want the ped to use a specific vehicle to get to the point then use VEHICLE_INDEX veh. Otherwise, set veh to NULL. 
//PURPOSE: Tells a ped to go to a coord by any means.
extern _native void TASK_GO_TO_COORD_ANY_MEANS(PED_INDEX ped, float x, float y, float z, int move_blend_ratio, VEHICLE_INDEX veh)l
extern _native void TASK_GO_TO_COORD_WHILE_AIMING(PED_INDEX ped, float x, float y, float z, uint Unk244, float Unk245, float Unk246, PED_INDEX ped2, float Unk248, float Unk249, float Unk250, bool Unk251)l
extern _native void TASK_GO_TO_COORD_WHILE_SHOOTING(PED_INDEX ped, float x, float y, float z, uint Unk255, float Unk256, float Unk257, PED_INDEX ped2, bool Unk259)l

//PARAM NOTES:  if Time is chosen to be -1 the ped will never warp. 
//PURPOSE: Tells a ped to go to another object.
extern _native void TASK_GO_TO_OBJECT(PED_INDEX ped, OBJECT_INDEX obj, int time, float radius)l

//PARAM NOTES: The behaviour will be the same as TASK_STAND_GUARD, but will only patrol inside the defensive area and inside max_patrol_prox, if the ped shouldn't patrol the area, set max_patrol_prox to 0.0
//  timer is used if the task is to be for a timed duration, measured in milliseconds. If not required, set to -1. 
//PURPOSE: Sets the defensive area of the ped and tells the ped to guard the area passed.
extern _native void TASK_GUARD_ANGLED_DEFENSIVE_AREA(PED_INDEX ped, float x, float y, float z, float heading, float max_patrol_prox, int timer, float area_start_x, float area_start_y, float area_start_z, float area_end_x, float area_end_y, float area_end_z, float area_width)l

//INFO: The behaviour will be the same as TASK_STAND_GUARD, if the ped shouldn't patrol the area, set max_patrol_prox to 0.0 
//PURPOSE: Tells the ped to guard its assigned area, will assert if no defensive area is assigned.
extern _native void TASK_GUARD_ASSIGNED_DEFENSIVE_AREA(PED_INDEX ped, float x, float y, float z, float heading, float max_patrol_prox, int timer)l

//PARAM NOTES: defensive_area is set the peds defensive area will be set to the position and radius of guard_radius.
//  The behaviour will be the same as TASK_STAND_GUARD, but will only patrol inside the defensive area and inside max_patrol_prox, if the ped shouldn't patrol the area, set max_patrol_prox to 0.0 
//PURPOSE: Tells the ped to guard their current position with the area passed.
extern _native void TASK_GUARD_CURRENT_POSITION(PED_INDEX ped, float max_patrol_prox, float guard_radius, bool defensive_area)l

//INFO: The behaviour will be the same as TASK_STAND_GUARD, but will only patrol inside the defensive area and inside max_patrol_prox, if the ped shouldn't patrol the area, set max_patrol_prox to 0.0 
//  The area is specified using area_start and area_radius the same format as SET_CHAR_SPHERE_DEFENSIVE_AREA. 
//PURPOSE: Sets the defensive area of the ped and tells the ped to guard the area passed.
extern _native void TASK_GUARD_SPHERE_DEFENSIVE_AREA(PED_INDEX ped, float x, float y, float z, float heading, float max_patrol_prox, int timer, float area_start_x, float area_start_y, float area_start_z, float area_radius)l

//PARAM NOTES: Set time to -1 to perform the task forever.
//PURPOSE: Gives the ped a put hands up task.
extern _native void TASK_HANDS_UP(PED_INDEX ped, int duration)l

//PARAM NOTES: VEHICLE_INDEX veh (The actual heli that will be flown)  
//  VEHICLE_INDEX target_veh (If a target vehicle needs to be specified it is done here)
//  PED_INDEX target-ped (If a target ped needs to be specified it is done here)
//  VECTOR3 pos (specify targetcoordinates here in needed.)
//  VEHICLE_MISSION Mission (Mission to be exectuted. ie MISSION_GOTO, MISSION_ATTACK, MISSION_FOLLOW) in commands_vehicle.sch
//  FLOAT speed (in m/s)
//  INT target_reached_dist (distance (in meters) at which heli thinks it's arrived.)
//							ALSO used as the hover distance for MISSION_ATTACK and MISSION_CIRCLE
//  FLOAT heli_orientation (orientation heli tries to be in. (0-360) Use -1 if not bothered. -1 Should be used in 99% of the times) 
//  INT flight_height (z coordinate the heli tries to maintain. ie 30 == 30 meters above sea level)
//  INT min_height_above_terrain (height in meters that the heli will try to stay above terrain. ie 20 == always tries to stay at least 20 meters above ground.) 
//PURPOSE: Gives the heli a mission.
extern _native void TASK_HELI_MISSION(PED_INDEX ped, VEHICLE_INDEX heli, VEHICLE_INDEX target_veh, PED_INDEX target_ped, float x, float y, float z, uint mission, float speed, int target_reached_dist, float heli_orientation, int flight_height, int min_height_above_terrain)l

//PARAM NOTES: If use_player_launch_force is TRUE then the ped will use the same launch force as the player.
//  If use_player_launch_force is FALSE and the ped isnt the player then the ped will jump using a smaller launch force. 
//PURPOSE: Force a ped to jump.
extern _native void TASK_JUMP(PED_INDEX ped, bool use_player_launch_force)l

//PARAM NOTES: Applies the vehicles breaks (while still in vehicle, and if there are any) and makes the ped stand up and get out regardless of speed.
//PURPOSE: Tells the ped to leave any vehicle. 
extern _native void TASK_LEAVE_ANY_CAR(PED_INDEX ped)l

//PURPOSE: Tells a ped to leave a vehicle
extern _native void TASK_LEAVE_CAR(PED_INDEX ped, VEHICLE_INDEX vehicle)l
extern _native void TASK_LEAVE_CAR_AND_FLEE(PED_INDEX ped, VEHICLE_INDEX veh, float x, float y, float z)l
extern _native void TASK_LEAVE_CAR_DONT_CLOSE_DOOR(PED_INDEX ped, VEHICLE_INDEX vehicle)l
extern _native void TASK_LEAVE_CAR_IMMEDIATELY(PED_INDEX ped, VEHICLE_INDEX vehicle)l
extern _native void TASK_LEAVE_CAR_IN_DIRECTION(PED_INDEX ped, VEHICLE_INDEX car, bool direction)l
extern _native void TASK_LEAVE_GROUP(PED_INDEX ped)l

//PARAM NOTES: If time is -1 the ped will perform the task forever.
//PURPOSE: Tells a ped to look at a ped.
extern _native void TASK_LOOK_AT_CHAR(PED_INDEX ped, PED_INDEX targetPed, int duration, uint look_flags)l

//PARAM NOTES: If time is -1 the ped will perform the task forever.
//PURPOSE: Gives the ped a look at coord task.
extern _native void TASK_LOOK_AT_COORD(PED_INDEX ped, float x, float y, float z, int duration, uint look_flags)l

//PARAM NOTES: If time is -1 the ped will perform the task forever.
//PURPOSE: Tells a ped to look at a object.
extern _native void TASK_LOOK_AT_OBJECT(PED_INDEX ped, OBJECT_INDEX targetObject, int duration, uint look_flags)l

//PARAM NOTES: If time is -1 the ped will perform the task forever.
//PURPOSE: Tells a ped to look at a vehicle.
extern _native void TASK_LOOK_AT_VEHICLE(PED_INDEX ped, VEHICLE_INDEX targetVehicle, int duration, uint look_flags)l

//PARAM NOTES: if WANDER is true the ped will wander, otherwise stand still for the length of the conversation 
//PURPOSE: Tells the ped to have a mobile conversation.
extern _native void TASK_MOBILE_CONVERSATION(PED_INDEX ped, bool wander)l
extern _native void TASK_OPEN_DRIVER_DOOR(PED_INDEX ped, VEHICLE_INDEX vehicle, int duration)l
extern _native void TASK_OPEN_PASSENGER_DOOR(PED_INDEX ped, VEHICLE_INDEX vehicle, int duration, int seat)l

//PARAM NOTES: Set time to -1 to perform the task forever.
extern _native void TASK_PAUSE(PED_INDEX ped, int duration)l

//INFO: Requires OPEN_SEQUENCE_TASK ... CLOSE_SEQUENCE_TASK 
//PURPOSE: Tells a ped to perform a given sequence.
extern _native void TASK_PERFORM_SEQUENCE(PED_INDEX ped, SEQUENCE_INDEX taskSequence)l

//INFO: Peds start the sequence at the Progress1th element. If the Progress1th element of the sequence is also a sequence it will start at the Progress2th element. 
//PURPOSE: Performs a sequence except that the ped will skip the elements of the sequence in the range (0 to Progress1 ).
extern _native void TASK_PERFORM_SEQUENCE_FROM_PROGRESS(PED_INDEX ped, SEQUENCE_INDEX sequence, uint progress1, uint progress2)l

//PURPOSE:Tells a ped to perform a given sequence which is not synced over the network. 
extern _native void TASK_PERFORM_SEQUENCE_LOCALLY(PED_INDEX ped, SEQUENCE_INDEX sequence)l
extern _native void TASK_PICKUP_AND_CARRY_OBJECT(PED_INDEX ped, OBJECT_INDEX obj, float x, float y, float z, bool Unk305)l

//PURPOSE: Plays a anim task on a ped.
extern _native void TASK_PLAY_ANIM(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk308, uint Unk309, uint Unk310, uint Unk311, uint Unk312, uint Unk313)l
extern _native void TASK_PLAY_ANIM_FACIAL(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk316, uint Unk317, uint Unk318, uint Unk319)l
extern _native void TASK_PLAY_ANIM_NON_INTERRUPTABLE(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk320, uint Unk321, uint Unk322, uint Unk323, uint Unk324, uint Unk325)l
extern _native void TASK_PLAY_ANIM_ON_CLONE(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk328, uint Unk329, uint Unk330, uint Unk331, uint Unk332, uint Unk333)l
extern _native void TASK_PLAY_ANIM_READY_TO_BE_EXECUTED(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk336)l
extern _native void TASK_PLAY_ANIM_SECONDARY(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk339, uint Unk340, uint Unk341, uint Unk342, uint Unk343, uint Unk344)l
extern _native void TASK_PLAY_ANIM_SECONDARY_IN_CAR(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk347, uint Unk348, uint Unk349, uint Unk350, uint Unk351, uint Unk352)l
extern _native void TASK_PLAY_ANIM_SECONDARY_NO_INTERRUPT(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk355, uint Unk356, uint Unk357, uint Unk358, uint Unk359, uint Unk360)l
extern _native void TASK_PLAY_ANIM_SECONDARY_UPPER_BODY(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk363, uint Unk364, uint Unk365, uint Unk366, uint Unk367, uint Unk368)l
extern _native void TASK_PLAY_ANIM_UPPER_BODY(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk371, uint Unk372, uint Unk373, uint Unk374, uint Unk375, uint Unk376)l
extern _native void TASK_PLAY_ANIM_WITH_ADVANCED_FLAGS(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk_8, bool loopAnim, uint Unk381_0, uint Unk382_0, uint Unk383_0, uint Unk384_0, uint Unk385_0, uint Unk386_0, int loopTimeMinusOneForever)l
extern _native void TASK_PLAY_ANIM_WITH_FLAGS(PED_INDEX ped, const char* anim_name, const char* anim_dict_name, float unknown0_8, uint loopTime99999or0, uint unk)l
extern _native void TASK_PLAY_ANIM_WITH_FLAGS_AND_START_PHASE(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float Unk390, uint Unk391, uint Unk392, float Unk393)l

//PURPOSE: Puts the ped directly into cover at the nearest point to the position specified.
extern _native void TASK_PUT_CHAR_DIRECTLY_INTO_COVER(PED_INDEX ped, float x, float y, float z, int time)l
extern _native void TASK_SAY(PED_INDEX ped, int unk)l

//PARAM NOTES:Once cover is reached the ped will hide behind it for Time milliseconds then the task will end,
//  if Time is zero the ped will hide behind it indefinitely and the task will never end. 
//PURPOSE: The ped will seek cover from the given peds location.
extern _native void TASK_SEEK_COVER_FROM_PED(PED_INDEX ped, PED_INDEX ped2, int time)l

//INFO: If after a search no cover point can be found the task will finish. 
//  Once cover is reached the ped will hide behind it for Time milliseconds then the task will end, if Time is zero the ped will hide behind it indefinitely and the task will never end. 
//PURPOSE: The ped will seek cover from the given coordinates.
extern _native void TASK_SEEK_COVER_FROM_POS(PED_INDEX ped, float x, float y, float z, int time)l

//INFO: The ped will find the nearest cover point to the coords given.
//  Once cover is reached the ped will hide behind it for Time milliseconds then the task will end, if Time is zero the ped will hide behind it indefinitely and the task will never end. 
//PARAM NOTES:
//PURPOSE: Tells a ped to seek cover to given coords cover_pos in such a way as to hide from from_pos.
extern _native void TASK_SEEK_COVER_TO_COORDS(PED_INDEX ped, float cover_pos_x, float cover_pos_y, float cover_pos_z, float from_pos_x, float from_pos_y, float from_pos_z, int time)l

//INFO: Seeks cover in such a way as to avoid fire from position fromX, fromY, fromZ. Once cover is reached the ped will hide behind it for Time milliseconds then the task will end, 
//  if Time is zero the ped will hide behind it indefinitely and the task will never end. 
//PARAM NOTES:
//PURPOSE: Task making the ped seek cover at the scripted cover point specified in CoverIndex.
extern _native void TASK_SEEK_COVER_TO_COVER_POINT(PED_INDEX ped, uint point, float x, float y, float z, int time)l
extern _native void TASK_SEEK_COVER_TO_OBJECT(PED_INDEX ped, OBJECT_INDEX obj, float x, float y, float z, int time)l

//PURPOSE:  Sets the ped to have their  decision maker set as part of a sequence. 
extern _native void TASK_SET_CHAR_DECISION_MAKER(PED_INDEX ped, DecisionMaker dm)l
extern _native void TASK_SET_COMBAT_DECISION_MAKER(PED_INDEX ped, DecisionMaker dm)l
extern _native void TASK_SET_IGNORE_WEAPON_RANGE_FLAG(PED_INDEX ped, bool ignore)l
extern _native void TASK_SHAKE_FIST(PED_INDEX ped)l
extern _native void TASK_SHIMMY(PED_INDEX ped, bool Unk422)l

//INFO: If -1 is passed to Time the ped will never give up on the task. 
//PURPOSE: Gives a shoot at ped task.
extern _native void TASK_SHOOT_AT_CHAR(PED_INDEX ped, PED_INDEX target, int time, eShootMode shootmode)l

//PURPOSE:  Tells the ped to shoot at given coord.
extern _native void TASK_SHOOT_AT_COORD(PED_INDEX ped, float x, float y, float z, int duration, eShootMode firing_type)l

//PURPOSE: Tells the ped to move across to the next seat in the vehicle. 
extern _native void TASK_SHUFFLE_TO_NEXT_CAR_SEAT(PED_INDEX ped, VEHICLE_INDEX veh)l
extern _native void TASK_SIT_DOWN(PED_INDEX ped, uint Unk429, uint Unk430, uint Unk431)l //Probbaly XYZ?
extern _native void TASK_SIT_DOWN_INSTANTLY(PED_INDEX ped, int sitType, int getUpType, int UnkTimer)l //-2 probs forever. //people report crashes with certain combonations - Nath
extern _native void TASK_SIT_DOWN_ON_NEAREST_OBJECT(PED_INDEX ped, uint Unk435, uint Unk436, float x, float y, float z, uint model, float Unk441, uint Unk442, bool Unk443)l
extern _native void TASK_SIT_DOWN_ON_OBJECT(PED_INDEX ped, uint Unk444, uint Unk445, OBJECT_INDEX obj, float x, float y, float z, float Unk450, uint unkTimeriThink, bool Unk452)l
extern _native void TASK_SIT_DOWN_ON_SEAT(PED_INDEX ped, uint Unk453False, uint Unk454False, float x, float y, float z, float angle, int Unk459MinusTwo)l
extern _native void TASK_SIT_DOWN_PLAY_ANIM(PED_INDEX ped, int unk0, int unk1, int unk2, char* unk3, char* unk4, bool unk5, bool unk6)l

//PARAM NOTES: time is an INT (-1 indicates flee forever) 
//PURPOSE: Tells a ped to flee the given ped.
extern _native void TASK_SMART_FLEE_CHAR(PED_INDEX ped, PED_INDEX fleeFromPed, float safe_distance, int duration)l
extern _native void TASK_SMART_FLEE_CHAR_PREFERRING_PAVEMENTS(PED_INDEX ped, PED_INDEX fleeFromPed, float safe_distance, int duration)l

//PARAM NOTES: time is an INT (-1 indicates flee forever) 
//PURPOSE: Tells a ped to flee the given coord.
extern _native void TASK_SMART_FLEE_POINT(PED_INDEX ped, float x, float y, float z, float safe_distance, int duration)l
extern _native void TASK_SMART_FLEE_POINT_PREFERRING_PAVEMENTS(PED_INDEX ped, float x, float y, float z, float safe_distance, int time)l
extern _native void TASK_SPACE_SHIP_GO_TO_COORD(PED_INDEX ped, VEHICLE_INDEX veh, float x, float y, float z)l

//PURPOSE: Tells the ped to defend the given position.
extern _native void TASK_STAND_GUARD(PED_INDEX ped, float x, float y, float z, float heading, float Unk461, bool Unk462, uint Unk463)l

//PARAM NOTES: If time is -1 then the character will stand forever. 
//PURPOSE: Gives the stand still task to a ped.
extern _native void TASK_STAND_STILL(PED_INDEX ped, int duration)l

//				The z coord of vPosition should be the ground position plus the ped's ground to root offset.
//PURPOSE: The ped will move or warp to the position and heading given, then start the scenario passed.
extern _native void TASK_START_SCENARIO_AT_POSITION(PED_INDEX ped, const char* scenario_name, float x, float y, float z, float heading)l

//PURPOSE: Puts the ped into the given scenario immediately.
extern _native void TASK_START_SCENARIO_IN_PLACE(PED_INDEX ped, const char* scenario_name, float Unk470)l

//PURPOSE: Tells the ped to swap their weapon.
extern _native void TASK_SWAP_WEAPON(PED_INDEX ped, bool draw_weapon)l
extern _native void TASK_SWIM_TO_COORD(PED_INDEX ped, float x, float y, float z)l
extern _native void TASK_TIRED(PED_INDEX ped, uint Unk471)l

//PURPOSE: Gives the task to set a ped duck state.
extern _native void TASK_TOGGLE_DUCK(PED_INDEX ped, int toggle)l
extern _native void TASK_TOGGLE_PED_THREAT_SCANNER(PED_INDEX ped, bool Unk473, bool Unk474, bool Unk475)l

//PURPOSE: Tells a ped to face a ped.
extern _native void TASK_TURN_CHAR_TO_FACE_CHAR(PED_INDEX ped, PED_INDEX targetPed)l

//PURPOSE: Tells a ped to turn face a coord.
extern _native void TASK_TURN_CHAR_TO_FACE_COORD(PED_INDEX ped, float x, float y, float z)l

//INFO: when called with FALSE, this command cannot be used in a sequence. 
//  If you want to use this as part of an AI sequence, consider using TASK_USE_MOBILE_PHONE_TIMED. 
//PARAM NOTES: PHONE_MODE: Specify which phone mode to put the phone into. Defaulted to Mode_ToCall.
//PURPOSE: Tells the ped to use a mobile phone.
extern _native void TASK_USE_MOBILE_PHONE(PED_INDEX ped, bool use)l

//INFO: If -1 is passed to Time the ped will never give up on the task. 
//PURPOSE:  Tells the ped to use a mobile phone for a time period.
extern _native void TASK_USE_MOBILE_PHONE_TIMED(PED_INDEX ped, int duration)l

//INFO: Make sure the scenario point or objects its attached to is loaded at the time. 
//PURPOSE: Puts the ped into the cloeset scenario to the coord.
extern _native void TASK_USE_NEAREST_SCENARIO_TO_POS(PED_INDEX ped, float x, float y, float z, float max_range)l

//INFO: Make sure the scenario point or objects its attached to is loaded at the time.
//PURPOSE: Warps the ped into the cloeset scenario to the pos.
extern _native void TASK_USE_NEAREST_SCENARIO_TO_POS_WARP(PED_INDEX ped, float x, float y, float z, float max_range)l

//PURPOSE: Tells a ped to wander.
extern _native void TASK_WANDER_STANDARD(PED_INDEX ped)l

//PURPOSE: Can warp a ped into a vehicle using a task sequence.
extern _native void TASK_WARP_CHAR_INTO_CAR_AS_DRIVER(PED_INDEX ped, VEHICLE_INDEX vehicle)l

//PURPOSE: Can warp a ped into a vehicle using a task sequence.
extern _native void TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER(PED_INDEX ped, VEHICLE_INDEX vehicle, int seat)l

//Deprecated
//TASK_SCRATCH_HEAD
//TASK_LOOK_ABOUT
//TASK_FLEE_POINT
//TASK_FLEE_CHAR
//TASK_DIVE_AND_GET_UP
//TASK_DIVE_FROM_ATTACHMENT_AND_GET_UP
//TASK_WEAPON_ROLL
//TASK_HOLD_OBJECT
//CAN_PED_DO_SHIMMY_HERE
//TASK_JETPACK
//TASK_COMPLEX_PICKUP_OBJECT
//TASK_GREET_PARTNER
//TASK_WALK_ALONGSIDE_CHAR
//SET_MELEE_ACTION_SENSE_RANGE_FOR_RIOT2
//TASK_HAND_GESTURE
//TASK_HOLSTERING_WEAPON
#pragma endregion

#pragma region PLAYER
//PURPOSE: Award an achievement to the main player
//PARAMS: AchievementId - Id of achievement to award.
extern _native bool AWARD_ACHIEVEMENT(int achievement)l

//INFO: Checks that the player is in control (not falling), is not being arrested etc
//PURPOSE: Returns TRUE if it is safe for the player to start a mission.
extern _native bool CAN_PLAYER_START_MISSION(PLAYER_INDEX player)l

//PURPOSE:  Verify if an achievement has been completed
//PARAMS: AchievementId - Id of achievement to award.
extern _native bool HAS_ACHIEVEMENT_BEEN_PASSED(int achievement)l

//INFO: Always returns FALSE but it setups up the script force clean up paramaters
extern _native bool HAS_DEATHARREST_EXECUTED(void)l
extern _native bool HAS_GAMER_CHANGED_NETWORK_MODEL_SETTINGS(void)l

//PURPOSE: Returns TRUE if the player has damaged any peds since the last time that CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_PED was called
extern _native bool HAS_PLAYER_DAMAGED_AT_LEAST_ONE_PED(PLAYER_INDEX playerIndex)l
extern _native bool HAS_PLAYER_DAMAGED_AT_LEAST_ONE_VEHICLE(PLAYER_INDEX playerIndex)l
extern _native bool HAS_PLAYER_RANK_BEEN_UPGRADED(void)l
extern _native bool IS_ATTACHED_PLAYER_HEADING_ACHIEVED(void)l
extern _native bool IS_IN_PLAYER_SETTINGS_MENU(void)l

//PURPOSE: Checks if the player is climbing.
extern _native bool IS_PLAYER_CLIMBING(PLAYER_INDEX playerIndex)l

//PURPOSE: Checks if the players control is on.
extern _native bool IS_PLAYER_CONTROL_ON(PLAYER_INDEX playerIndex)l

//PURPOSE: Returns TRUE if the given player is dead. 
extern _native bool IS_PLAYER_DEAD(PLAYER_INDEX playerIndex)l

//PURPOSE: Returns TRUE if the player is pointing his weapon at the specified ped.
extern _native bool IS_PLAYER_FREE_AIMING_AT_CHAR(PLAYER_INDEX playerIndex, PED_INDEX ped)l

//INFO: Note it might work best if ambient tasks make sure the player has been free for an ambient task for a few seconds depending on the situation.
//	(E.g. if the player hasn't done anything interesting for 5 seconds, allow cab hailing) 
//PURPOSE: Returns TRUE if the player is currently not in combat, climbing, jumping or firing a gun.
extern _native bool IS_PLAYER_FREE_FOR_AMBIENT_TASK(PLAYER_INDEX playerIndex)l
extern _native bool IS_PLAYER_IN_INFO_ZONE(PLAYER_INDEX player, const char* zone)l

//PURPOSE: Returns true if the player is online - implies he is also signed in locally.
extern _native bool IS_PLAYER_ONLINE(void)l
extern _native bool IS_PLAYER_PERFORMING_STOPPIE(PLAYER_INDEX player)l
extern _native bool IS_PLAYER_PERFORMING_WHEELIE(PLAYER_INDEX player)l

//INFO: Returns FALSE if the screen is fading due to the player being killed or arrested or failing a critical mission.
//PURPOSE: Returns TRUE if the player is in control.
extern _native bool IS_PLAYER_PLAYING(PLAYER_INDEX playerIndex)l

//PURPOSE: Returns TRUE if the specified Player is in a vehicle and is pressing the Horn button. 
extern _native bool IS_PLAYER_PRESSING_HORN(PLAYER_INDEX playerIndex)l

//INFO: the player is considered safe if they are:
//    In a car and not entering or exiting or
//    On foot and not ragdolling 
//PURPOSE: Returns if the player is in a safe state to trigger a cutscene.
extern _native bool IS_PLAYER_READY_FOR_CUTSCENE(PLAYER_INDEX player)l
extern _native bool IS_PLAYER_SIGNED_IN_LOCALLY(void)l

//INFO: Even if this command returns TRUE, player control could still have been turned off by something else like camera or cutscene code
//PURPOSE: returns FALSE if the player control has been turned off by a script.
extern _native bool IS_PLAYER_SCRIPT_CONTROL_ON(PLAYER_INDEX player)l

//PURPOSE: Returns TRUE if the player is pointing his weapon at any entity.
extern _native bool IS_PLAYER_TARGETTING_ANYTHING(PLAYER_INDEX playerIndex)l

//PURPOSE: Returns TRUE if the player is pointing his weapon at the specified ped. 
extern _native bool IS_PLAYER_TARGETTING_CHAR(PLAYER_INDEX playerIndex, PED_INDEX ped)l

//PURPOSE: Returns TRUE if the player is pointing his weapon at the specified object. 
extern _native bool IS_PLAYER_TARGETTING_OBJECT(PLAYER_INDEX playerIndex, OBJECT_INDEX obj)l
extern _native bool IS_PLAYER_VEHICLE_ENTRY_DISABLED(PLAYER_INDEX player)l
extern _native bool IS_SCORE_GREATER(PLAYER_INDEX playerIndex, int score)l
extern _native bool IS_SYSTEM_UI_SHOWING(void)l

//INFO: returns weirdly if player is niko.. wouldn't recommend using for anything critical - Nathan
extern _native bool IS_THIS_PED_A_PLAYER(PED_INDEX ped)l

//PURPOSE: Checks if the wanted level is less than a certain value.
extern _native bool IS_WANTED_LEVEL_GREATER(PLAYER_INDEX playerIndex, int level)l

//PURPOSE: Checks the player has a vaild ped.
extern _native bool PLAYER_HAS_CHAR(PLAYER_INDEX playerIndex)l
extern _native bool PLAYER_HAS_FLASHING_STAR_AFTER_OFFENCE(PLAYER_INDEX player)l

//PURPOSE: If the player has a wanted level and is outside the circle this command returns true
extern _native bool PLAYER_HAS_FLASHING_STARS_ABOUT_TO_DROP(PLAYER_INDEX playerIndex)l

//INFO: because the police haven't spotted the player for a while.
//PURPOSE: Returns true if the player in question (usually 0) has a wanted level and the stars are displayed grey.
extern _native bool PLAYER_HAS_GREYED_OUT_STARS(PLAYER_INDEX player)l
extern _native bool PLAYER_IS_PISSED_OFF(PLAYER_INDEX player)l

//PURPOSE: Returns the name of the player.
extern _native const char* GET_PLAYER_NAME(PLAYER_INDEX playerIndex)l
extern _native int GET_LEFT_PLAYER_CASH_TO_REACH_LEVEL(int playerRank)l
extern _native int GET_LOCAL_PLAYER_MP_CASH(void)l
extern _native int GET_LOCAL_PLAYER_WEAPON_STAT(WEAPON_TYPE wtype, int stat)l
extern _native int GET_NEEDED_PLAYER_CASH_FOR_LEVEL(int rank)l
extern _native int GET_NO_LAW_VEHICLES_DESTROYED_BY_LOCAL_PLAYER(void)l

//PURPOSE: Get the number of players in a team
extern _native int GET_NO_OF_PLAYERS_IN_TEAM(int team)l

//PURPOSE: Gets the number of players.
extern _native int GET_NUMBER_OF_PLAYERS(void)l
extern _native int GET_PLAYER_COLOUR(PLAYER_INDEX Player)l

//PURPOSE: Gets the team the player is in.
extern _native int GET_PLAYER_TEAM(PLAYER_INDEX Player)l

//INFO: Returns -1 if the player hasn't been arrested yet since starting a session. 
//PURPOSE: Returns the time in milliseconds since the player was arrested and got respawned outside a police station.
extern _native int GET_TIME_SINCE_LAST_ARREST(void)l

//INFO:  Returns -1 if the player hasn't been killed yet since starting a session. 
//PURPOSE: Returns the time in milliseconds since the player died.
extern _native int GET_TIME_SINCE_LAST_DEATH(void)l

//INFO: The player has to be in a car himself to do this. Can be used to detect whether the player is driving recklessly.
//	Before the player hits anything the return value is really large.
//PURPOSE: Returns (in milliseconds) how long ago the player last drove against the flow of traffic.
extern _native int GET_TIME_SINCE_PLAYER_DROVE_AGAINST_TRAFFIC(PLAYER_INDEX playerIndex)l

//INFO: The player has to be in a car himself to do this. Can be used to detect whether the player is driving recklessly.
//	Before the player hits anything the return value is really large.
//PURPOSE: Returns (in milliseconds) how long ago the player drove on the pavement.
extern _native int GET_TIME_SINCE_PLAYER_DROVE_ON_PAVEMENT(PLAYER_INDEX playerIndex)l

//INFO: The player has to be in a car himself to do this. Can be used to detect whether the player is driving recklessly.
//	Before the player hits anything the return value is really large.
//PURPOSE: Returns (in milliseconds) how long ago the player (in a car)  last hit a building.
extern _native int GET_TIME_SINCE_PLAYER_HIT_BUILDING(PLAYER_INDEX playerIndex)l

//INFO: The player has to be in a car himself to do this. Can be used to detect whether the player is driving recklessly.
//	Before the player hits anything the return value is really large.
//PURPOSE: Return (in milliseconds) how long ago the player (in a car) last hit a car.
extern _native int GET_TIME_SINCE_PLAYER_HIT_CAR(PLAYER_INDEX playerIndex)l

//INFO: The player has to be in a car himself to do this. Can be used to detect whether the player is driving recklessly.
//	Before the player hits anything the return value is really large.
//PURPOSE: Returns (in milliseconds) how long ago the player (in a car)  last hit a object.
extern _native int GET_TIME_SINCE_PLAYER_HIT_OBJECT(PLAYER_INDEX playerIndex)l

//INFO: The player has to be in a car himself to do this. Can be used to detect whether the player is driving recklessly.
//	Before the player hits anything the return value is really large.
//PURPOSE: Returns (in milliseconds) how long ago the player (in a car)  last hit a ped.
extern _native int GET_TIME_SINCE_PLAYER_HIT_PED(PLAYER_INDEX playerIndex)l

//INFO: The player has to be in a car himself to do this. Can be used to detect whether the player is driving recklessly.
//	Before the player hits anything the return value is really large.
//PURPOSE: Returns (in milliseconds) how long ago the player last ran a red light.
extern _native int GET_TIME_SINCE_PLAYER_RAN_LIGHT(PLAYER_INDEX playerIndex)l

//PURPOSE: Returns the ID of the object that the player will pick up if they press PICKUP now.
extern _native OBJECT_INDEX WHAT_WILL_PLAYER_PICKUP(PLAYER_INDEX player)l
extern _native PLAYER_INDEX CONVERT_INT_TO_PLAYERINDEX(int player_index)l

//PURPOSE: Gets the player index for the main player.
extern _native PLAYER_INDEX GET_PLAYER_ID(void)l
extern _native PLAYER_INDEX GET_PLAYER_ID_FOR_THIS_PED(PED_INDEX ped)l
extern _native uint GET_PLAYERSETTINGS_MODEL_CHOICE(void)l
extern _native void ADD_SCORE(PLAYER_INDEX playerIndex, int score)l

//PURPOSE: Multiplayer only, this explicitly allows player vs friendly player lockons in a network game: By default its disabled
extern _native void ALLOW_LOCKON_TO_FRIENDLY_PLAYERS(PLAYER_INDEX player, bool allow)l
extern _native void ALLOW_LOCKON_TO_RANDOM_PEDS(PLAYER_INDEX player, bool allow)l
extern _native void ALLOW_PLAYER_TO_CARRY_NON_MISSION_OBJECTS(PLAYER_INDEX playerIndex, bool allow)l

//INFO: The wanted level change will not take place for 10 seconds or su (emulating the time it takes a citizen to report the crime) unless APPLY_WANTED_LEVEL_CHANGE_NOW is called immediately after 
//PURPOSE: Sets the wanted level for the player.
extern _native void ALTER_WANTED_LEVEL(PLAYER_INDEX playerIndex, int level)l

//PURPOSE: Sets the wanted level only if its higher than the current.
extern _native void ALTER_WANTED_LEVEL_NO_DROP(PLAYER_INDEX playerIndex, int level)l

//PURPOSE: Sets the wanted level for the player immediately..
extern _native void APPLY_WANTED_LEVEL_CHANGE_NOW(PLAYER_INDEX playerIndex)l
extern _native void BLOCK_STATS_MENU_ACTIONS(PLAYER_INDEX player)l
extern _native void CHANGE_PLAYER_MODEL(PLAYER_INDEX playerIndex, uint model)l
extern _native void CHANGE_PLAYER_PHONE_MODEL(PLAYER_INDEX player, uint model)l
extern _native void CHANGE_PLAYER_PHONE_MODEL_OFFSETS(PLAYER_INDEX player, float x0, float y0, float z0, float x1, float y1, float z1)l

//PURPOSE: Call this once before you start checking HAS_PLAYER_DAMAGED_AT_LEAST_ONE_PED
extern _native void CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_PED(PLAYER_INDEX playerIndex)l
extern _native void CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_VEHICLE(PLAYER_INDEX player)l

//PURPOSE: Clears the players wanted level immediately
extern _native void CLEAR_WANTED_LEVEL(PLAYER_INDEX playerIndex)l
extern _native void CREATE_PLAYER(any unused, float x, float y, float z, PLAYER_INDEX* pPlayerIndex)l
extern _native void DELETE_PLAYER(PLAYER_INDEX player_index)l

//PURPOSE: Set lockon active state for mulitplayer games.
extern _native void DISABLE_PLAYER_LOCKON(PLAYER_INDEX playerIndex, bool disabled)l

//PURPOSE: Sets if the player can sprint.
extern _native void DISABLE_PLAYER_SPRINT(PLAYER_INDEX playerIndex, bool disabled)l
extern _native void DISABLE_PLAYER_VEHICLE_ENTRY(PLAYER_INDEX player, bool disable)l

//PURPOSE: Sets if the cops will be dispatched for the player.
extern _native void DONT_DISPATCH_COPS_FOR_PLAYER(PLAYER_INDEX player, bool dont)l

//PURPOSE: Forces script cleanup
extern _native void FAKE_DEATHARREST(void)l

//PURPOSE: Force higher air drag mult on any car or bike the player uses until the mission ends or this command is called again with 1.0, mult range 1.0 - 15.0  (5.0 is standard for high air resistance zones on the map)
extern _native void FORCE_AIR_DRAG_MULT_FOR_PLAYERS_CAR(PLAYER_INDEX player, float multiplier)l
extern _native void FORCE_INTERIOR_LIGHTING_FOR_PLAYER(PLAYER_INDEX player, bool force)l
extern _native void FORCE_NET_PLAYER_INVISIBLE(PLAYER_INDEX player, bool force)l

//PURPOSE: Gets the players max wanted level.
extern _native void GET_MAX_WANTED_LEVEL(int* pMaxWantedLevel)l
extern _native void GET_NUM_OF_MODELS_KILLED_BY_PLAYER(int unk0, int unk1, int* num)l

//PURPOSE: Gets the ped index from the players index.
extern _native void GET_PLAYER_CHAR(PLAYER_INDEX playerIndex, PED_INDEX* pPed)l

//INFO: When the player is created, a group is also created with the player as its leader.
//	You should never need to create another group for the player to lead. 
//PURPOSE: Gets the players group.
extern _native void GET_PLAYER_GROUP(PLAYER_INDEX playerIndex, GROUP_INDEX* pGroup)l

//PURPOSE: Gets the players max armour value.
extern _native void GET_PLAYER_MAX_ARMOUR(PLAYER_INDEX playerIndex, int* pMaxArmour)l
extern _native void GET_PLAYER_MAX_HEALTH(PLAYER_INDEX player, int* maxhealth)l

//PURPOSE: Gets the RGB colour of the player.
extern _native void GET_PLAYER_RGB_COLOUR(PLAYER_INDEX Player, int* r, int* g, int* b)l
extern _native void GET_PLAYER_WANTED_LEVEL_INCREMENT(PLAYER_INDEX player, int* increment)l

//INFO: if  the player is currently in a vehicle that vehicle will be returned.
//	If the player has not been in a vehicle yet or it somehow got destroyed/removed the value returned will be NULL. 
//PURPOSE: Returns the last vehicle that the player has been in.
extern _native void GET_PLAYERS_LAST_CAR_NO_SAVE(VEHICLE_INDEX* pVehicle)l

//PURPOSE: Gets the train the player would like to enter.
extern _native void GET_TRAIN_PLAYER_WOULD_ENTER(PLAYER_INDEX player, VEHICLE_INDEX* train)l
extern _native void GET_VEHICLE_PLAYER_WOULD_ENTER(PLAYER_INDEX player, VEHICLE_INDEX* veh)l
extern _native void GIVE_PLAYER_HELMET(PLAYER_INDEX player)l

//PURPOSE: Gives the player control of the ragdoll.
extern _native void GIVE_PLAYER_RAGDOLL_CONTROL(PLAYER_INDEX player, bool give)l

//PURPOSE: Sets the player's maximum armour to the specified value
extern _native void INCREASE_PLAYER_MAX_ARMOUR(PLAYER_INDEX player, int armour)l
extern _native void INCREASE_PLAYER_MAX_HEALTH(PLAYER_INDEX player, int maxhealth)l
extern _native void LIMIT_TWO_PLAYER_DISTANCE(float distance)l
extern _native void MAINTAIN_FLASHING_STAR_AFTER_OFFENCE(PLAYER_INDEX player, bool maintain)l
extern _native void MAKE_PLAYER_FIRE_PROOF(PLAYER_INDEX player, bool proof)l
extern _native void MAKE_PLAYER_GANG_DISAPPEAR(void)l
extern _native void MAKE_PLAYER_GANG_REAPPEAR(void)l

//INFO: Call this before starting an animated cutscene. This tries to prevent the player from dying while the cutscene is playing. 
//	All fires, explosions and projectiles are removed from the world. All car fires are extinguished and it should be impossible to damage any car. 
//	Finally, a flag is set to tell the game that a cutscene is in progress. 
//	This command relies on CLEAR_CUTSCENE being called to set everything back to normal. 
//PURPOSE: It makes the player everything-proof, disables his controls and stops anyone attacking him.
extern _native void MAKE_PLAYER_SAFE_FOR_CUTSCENE(PLAYER_INDEX player)l
extern _native void MAKE_ROOM_IN_PLAYER_GANG_FOR_MISSION_PEDS(int unk)l
extern _native void RELEASE_TWO_PLAYER_DISTANCE(void)l

//PARAM NOTES: If force_remove is set to TRUE, instantly removes helmet in a pop. If set to FALSE, will take it off in the same way that getting on/off bikes do  playing animation etc. 
//PURPOSE: Will remove the players helmet if they have one.
extern _native void REMOVE_PLAYER_HELMET(PLAYER_INDEX playerIndex, bool force_remove)l
extern _native void RESET_ACHIEVEMENTS_AWARD(void)l
extern _native void RESET_LOCAL_PLAYER_WEAPON_STAT(WEAPON_TYPE wtype, int stat)l
extern _native void RESET_NO_LAW_VEHICLES_DESTROYED_BY_LOCAL_PLAYER(void)l
extern _native void RESET_NUM_OF_MODELS_KILLED_BY_PLAYER(int unk)l

//PURPOSE: Stops all random peds from attacking the player, except cops.
extern _native void SET_ALL_RANDOM_PEDS_FLEE(PLAYER_INDEX player, bool set)l
extern _native void SET_DRAW_PLAYER_COMPONENT(int component, bool set)l

//PURPOSE: Sets all peds (including police) will ignore the player. 
extern _native void SET_EVERYONE_IGNORE_PLAYER(PLAYER_INDEX playerIndex, bool value)l
extern _native void SET_FREE_HEALTH_CARE(PLAYER_INDEX player, bool set)l
extern _native void SET_HEADING_FOR_ATTACHED_PLAYER(any unused, float unk0, float unk1)l

//PURPOSE: Peds ignore lower priority shocking events, for use in tightly scripted situations, must be reset to false when finished.
extern _native void SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(PLAYER_INDEX player, bool value)l

//PURPOSE: Sets the max wanted level the player can get.
extern _native void SET_MAX_WANTED_LEVEL(int lvl)l
extern _native void SET_PLAYER_AS_COP(PLAYER_INDEX player, bool set)l

//PURPOSE: Sets if the player can be hassled by gangs.
extern _native void SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(PLAYER_INDEX playerIndex, bool value)l

//PURPOSE: Sets whether the player is able to fire an uzi from a vehicle. 
extern _native void SET_PLAYER_CAN_DO_DRIVE_BY(PLAYER_INDEX playerIndex, bool value)l

//PURPOSE: Sets if the player can use cover.
extern _native void SET_PLAYER_CAN_USE_COVER(PLAYER_INDEX playerIndex, bool value)l
extern _native void SET_PLAYER_COLOUR(PLAYER_INDEX player, int color)l

//INFO: This command can be used to temporarily disable the players controls. The controls should be switched back on again as soon as possible. 
//	NOTES Turning this on will make vehicles proof from bullets etc. Likewise when the player controls are turned back on it will make ALL the vehicles vunerable again, 
//	so you will have to set them back with SET_CAR_CAN_BE_DAMAGED(vehicle_name, FALSE). 
//PURPOSE: Sets the players control state.
extern _native void SET_PLAYER_CONTROL(PLAYER_INDEX playerIndex, bool value)l
extern _native void SET_PLAYER_CONTROL_ADVANCED(PLAYER_INDEX playerIndex, bool unknown1, bool clear_2nd_task, bool unknown3)l
extern _native void SET_PLAYER_CONTROL_FOR_AMBIENT_SCRIPT(PLAYER_INDEX player, bool flag0, bool clear_2nd_task)l
extern _native void SET_PLAYER_CONTROL_FOR_NETWORK(PLAYER_INDEX playerIndex, bool unknownTrue, bool unknownFalse)l
extern _native void SET_PLAYER_DISABLE_CROUCH(PLAYER_INDEX player, bool set)l
extern _native void SET_PLAYER_DISABLE_JUMP(PLAYER_INDEX player, bool set)l
extern _native void SET_PLAYER_FAST_RELOAD(PLAYER_INDEX playerIndex, bool value)l

//PURPOSE: Forces the player to always aim
extern _native void SET_PLAYER_FORCED_AIM(PLAYER_INDEX player, bool set)l
extern _native void SET_PLAYER_GROUP_RECRUITMENT(PLAYER_INDEX player, bool set)l
extern _native void SET_PLAYER_GROUP_TO_FOLLOW_ALWAYS(PLAYER_INDEX playerIndex, bool value)l
extern _native void SET_PLAYER_GROUP_TO_FOLLOW_NEVER(PLAYER_INDEX player, bool set)l

//PURPOSE: Set the player invincible to all damage types.
extern _native void SET_PLAYER_INVINCIBLE(PLAYER_INDEX playerIndex, bool value)l
extern _native void SET_PLAYER_KEEPS_WEAPONS_WHEN_RESPAWNED(PLAYER_INDEX player_index)l

//PURPOSE: Set the player can only enter this vehicle, set to null if you want them to enter any vehicle.
extern _native void SET_PLAYER_MAY_ONLY_ENTER_THIS_VEHICLE(PLAYER_INDEX player, VEHICLE_INDEX veh)l

//PURPOSE: The players mood is set to normal.
extern _native void SET_PLAYER_MOOD_NORMAL(PLAYER_INDEX playerIndex)l
extern _native void SET_PLAYER_MOOD_PISSED_OFF(PLAYER_INDEX playerIndex, uint unknown150)l
extern _native void SET_PLAYER_MP_MODIFIER(PLAYER_INDEX player, int Unk12, float modifier)l
extern _native void SET_PLAYER_NEVER_GETS_TIRED(PLAYER_INDEX playerIndex, bool value)l
extern _native void SET_PLAYER_PLAYER_TARGETTING(bool set)l

//PURPOSE: Set the players current team. 
extern _native void SET_PLAYER_TEAM(PLAYER_INDEX Player, int team)l
extern _native void SET_PLAYERS_CAN_BE_IN_SEPARATE_CARS(bool set)l
extern _native void SET_PLAYERSETTINGS_MODEL_VARIATIONS_CHOICE(PLAYER_INDEX playerIndex)l

//PURPOSE: Use this to stop the police arresting the player. 
extern _native void SET_POLICE_IGNORE_PLAYER(PLAYER_INDEX playerIndex, bool value)l

//PURPOSE: Sets if police blips are displayed on the radar.
extern _native void SET_POLICE_RADAR_BLIPS(bool set)l
extern _native void SET_SCRIPT_LIMIT_TO_GANG_SIZE(int size)l
extern _native void SET_USE_LEG_IK(PLAYER_INDEX player, bool set)l

//INFO: If you set the wanted multiplier to a low value 0.01 and a cop see you shoot a ped in face you wil still get a wanted level.
//PARAM NOTES: 1.0f is the default value and it will automatically be reset when mission has finished.
//	A value of 2.0f means that the players wanted level will go up twice as fast when he commits crimes. 
//PURPOSE: Sets the chance that the player will get a wanted level.
extern _native void SET_WANTED_MULTIPLIER(float multiplier)l
extern _native void SHOW_SIGNIN_UI(void)l

//INFO: This should be called when a mission is passed or on autosave.
//PURPOSE: Any contributions to the wanted level are blocked for 5 seconds or so.
extern _native void START_FIRING_AMNESTY(void)l
extern _native void STORE_SCORE(PLAYER_INDEX playerIndex, int* value)l

//PURPOSE: Gets the players wanted level.
extern _native void STORE_WANTED_LEVEL(PLAYER_INDEX playerIndex, int* value)l

//Deprecated
//GET_PLAYER_POINTS
//IS_PLAYER_IN_REMOTE_MODE
//GET_TOTAL_NUMBER_OF_PEDS_KILLED_BY_PLAYER
//ENSURE_PLAYER_HAS_DRIVE_BY_WEAPON
//IS_PLAYER_IN_SHORTCUT_TAXI
//IS_2PLAYER_GAME_GOING_ON
//CLEAR_WANTED_LEVEL_IN_GARAGE
//USE_DETONATOR
//DO_WEAPON_STUFF_AT_START_OF_2P_GAME
//IS_PLAYER_USING_JETPACK
//GIVE_REMOTE_CONTROLLED_MODEL_TO_PLAYER
//GET_REMOTE_CONTROLLED_CAR
//REMOVE_RC_BUGGY
//TAKE_REMOTE_CONTROL_OF_CAR
//IS_PLAYER_LOGGING_IN_NP
//IS_PLAYER_ONLINE_NP
//IS_PLAYER_ONLINE_GAMESPY
#pragma endregion

#pragma region PED
extern _native bool ARE_ANY_CHARS_NEAR_CHAR(PED_INDEX ped, float range)l

//PURPOSE: Checks if any peds that hate the ped passed are in the area defined by sphere with vCentre and radius
extern _native bool ARE_ENEMY_PEDS_IN_AREA(PED_INDEX ped, float x, float y, float z, float radius)l
extern _native bool CAN_CHAR_SEE_DEAD_CHAR(PED_INDEX ped, any unused)l

//PURPOSE: Call this before trying to create a ped with a random model, to ensure that a model is available
extern _native bool CAN_CREATE_RANDOM_CHAR(bool flag0, bool flag1)l
extern _native bool CHECK_NM_FEEDBACK(PED_INDEX ped, uint feedback, uint type)l

//PURPOSE: Check that ped exists in the world.
extern _native bool DOES_CHAR_EXIST(PED_INDEX ped)l

//PURPOSE: Checks that the decision maker exists
extern _native bool DOES_DECISION_MAKER_EXIST(DecisionMaker dm)l

//PURPOSE: Checks that the group exists.
extern _native bool DOES_GROUP_EXIST(GROUP_INDEX group)l
extern _native bool GET_CHAR_ALLOWED_TO_RUN_ON_BOATS(PED_INDEX ped)l
extern _native bool GET_CHAR_ANIM_EVENT_TIME(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, int unk, float* time)l
extern _native bool GET_CHAR_ANIM_IS_EVENT(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, int flag)l
extern _native bool GET_CHAR_GRAVITY(PED_INDEX ped)l
extern _native bool GET_CHAR_LAST_DAMAGE_BONE(PED_INDEX ped, uint* pBone)l
extern _native bool GET_CHAR_MELEE_ACTION_FLAG0(PED_INDEX ped)l
extern _native bool GET_CHAR_MELEE_ACTION_FLAG1(PED_INDEX ped)l
extern _native bool GET_CHAR_MELEE_ACTION_FLAG2(PED_INDEX ped)l
extern _native bool GET_CHAR_MOVEMENT_ANIMS_BLOCKED(PED_INDEX ped)l
extern _native bool GET_CHAR_READY_TO_BE_EXECUTED(PED_INDEX ped)l
extern _native bool GET_CHAR_READY_TO_BE_STUNNED(PED_INDEX ped)l
extern _native bool GET_CHAR_SWIM_STATE(PED_INDEX ped, SWIM_STATES* state)l
extern _native bool GET_CHAR_WALK_ALONGSIDE_LEADER_WHEN_APPROPRIATE(PED_INDEX ped)l
extern _native bool GET_CHAR_WILL_COWER_INSTEAD_OF_FLEEING(PED_INDEX ped)l
extern _native bool GET_CHAR_WILL_TRY_TO_LEAVE_BOAT_AFTER_LEADER(PED_INDEX ped)l
extern _native bool GET_CHAR_WILL_TRY_TO_LEAVE_WATER(PED_INDEX ped)l

//PURPOSE: Trys to get closest random ped to a coord. 
extern _native bool GET_CLOSEST_CHAR(float x, float y, float z, float range, bool random_peds, bool mission_peds, PED_INDEX* ped)l
extern _native bool GET_CREATE_RANDOM_COPS(void)l
extern _native bool GET_GROUP_CHAR_DUCKS_WHEN_AIMED_AT(PED_INDEX ped)l

//PURPOSE: Gets whether the ped will perform low-level steering behaviour around objects
extern _native bool GET_PED_STEERS_AROUND_OBJECTS(PED_INDEX ped)l

//PURPOSE: Gets whether the ped will perform low-level steering behaviour around other peds
extern _native bool GET_PED_STEERS_AROUND_PEDS(PED_INDEX ped)l

//INFO: This command will only ever return true for anims that hold at the end (i.e. anims that loop or end automatically will always return false)
//PURPOSE: Checks if the anim specified is being held on the last frame. 
extern _native bool HAS_CHAR_ANIM_FINISHED(PED_INDEX ped, const char* anim_dict_name, const char* anim_name)l
extern _native bool HAS_CHAR_BEEN_ARRESTED(PED_INDEX ped)l

//PURPOSE: Returns TRUE if the last thing to damage the given ped was the given damager vehicle.
extern _native bool HAS_CHAR_BEEN_DAMAGED_BY_CAR(PED_INDEX ped, VEHICLE_INDEX vehicle)l

//PURPOSE: Returns TRUE if the last thing to damage the given ped was the given damager ped.
//			Will also return true if the damager ped is in a vehicle that damages the 
//          first ped but only if check_vehicle is left TRUE
extern _native bool HAS_CHAR_BEEN_DAMAGED_BY_CHAR(PED_INDEX ped, PED_INDEX damager, bool check_vehicle)l
extern _native bool HAS_CHAR_BEEN_PHOTOGRAPHED(PED_INDEX ped)l
extern _native bool HAS_CHAR_SPOTTED_CHAR(PED_INDEX ped, PED_INDEX otherChar)l
extern _native bool HAS_CHAR_SPOTTED_CHAR_IN_FRONT(PED_INDEX ped, PED_INDEX otherChar)l

//PURPOSE: Checks that any peds are shooting in the area.
extern _native bool IS_ANY_CHAR_SHOOTING_IN_AREA(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z, bool highlight_area)l

//PURPOSE: Checks if the ped is dead.
extern _native bool IS_CHAR_DEAD(PED_INDEX ped)l

//PURPOSE: Checks that the ped is ducking
extern _native bool IS_CHAR_DUCKING(PED_INDEX ped)l

//PURPOSE: Checks angle between the 2 peds.
extern _native bool IS_CHAR_FACING_CHAR(PED_INDEX ped, PED_INDEX otherChar, float angle)l

//INFO: Cannot give ped tasks if fatally injured.
//PURPOSE: Checks that the ped is going to die.
extern _native bool IS_CHAR_FATALLY_INJURED(PED_INDEX ped)l

//PURPOSE: Check if the ped is gesturing.
extern _native bool IS_CHAR_GESTURING(PED_INDEX ped)l
extern _native bool IS_CHAR_GETTING_IN_TO_A_CAR(PED_INDEX ped)l
extern _native bool IS_CHAR_HEALTH_GREATER(PED_INDEX ped, int health)l

//PARAM NOTES: This function is only valid for the current frame
//PURPOSE: Checks if the ped is in the air this frame (i.e. not touching the ground). 
extern _native bool IS_CHAR_IN_AIR(PED_INDEX ped)l

//PARAM NOTES: min and max define the midpoints of two parallel sides and width is the width of these sides.
//PURPOSE: Checks if a ped is in non axis aligned area, defined by coords of a face and distance between faces. More info..
//		   If the ped is in a vehicle, then the coords of the vehicle are used in the check. 
extern _native bool IS_CHAR_IN_ANGLED_AREA_2D(PED_INDEX ped, float min_x, float min_y, float max_x, float max_y, float width, bool highlight_area)l

//PARAM NOTES: min and max define the midpoints of two parallel sides and width is the width of these sides.
//PURPOSE: Checks if a ped is in non axis aligned area, defined by coords of a face and distance between faces. More info..
//		   If the ped is in a vehicle, then the coords of the vehicle are used in the check. 
extern _native bool IS_CHAR_IN_ANGLED_AREA_3D(PED_INDEX ped, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z, float width, bool hightlight_area)l

//PURPOSE: Checks that the ped is in a boat model.
extern _native bool IS_CHAR_IN_ANY_BOAT(PED_INDEX ped)l

//PURPOSE: Checks the ped is getting in/out OR sitting in any vehicle. 
extern _native bool IS_CHAR_IN_ANY_CAR(PED_INDEX ped)l

//PURPOSE: Checks that the ped is in a helicopter model.
extern _native bool IS_CHAR_IN_ANY_HELI(PED_INDEX ped)l

//PURPOSE: Checks that the ped is in a plane model.
extern _native bool IS_CHAR_IN_ANY_PLANE(PED_INDEX ped)l

//PURPOSE: Checks that the ped is in a police model.
extern _native bool IS_CHAR_IN_ANY_POLICE_VEHICLE(PED_INDEX ped)l

//PURPOSE: Checks that the ped is in a train model.
extern _native bool IS_CHAR_IN_ANY_TRAIN(PED_INDEX ped)l
extern _native bool IS_CHAR_IN_AREA_ON_FOOT_2D(PED_INDEX ped, float x1, float y1, float x2, float y2, bool unknownFalse)l
extern _native bool IS_CHAR_IN_AREA_2D(PED_INDEX ped, float x1, float y1, float x2, float y2, bool highlight_area)l
extern _native bool IS_CHAR_IN_AREA_3D(PED_INDEX ped, float x1, float y1, float z1, float x2, float y2, float z2, bool highlight_area)l

//PURPOSE: Checks the ped is getting in/out OR sitting in the specified vehicle. 
extern _native bool IS_CHAR_IN_CAR(PED_INDEX ped, VEHICLE_INDEX vehicle)l

//PURPOSE: Checks that the ped is in a plane or helicopter model.
extern _native bool IS_CHAR_IN_FLYING_VEHICLE(PED_INDEX ped)l

//PURPOSE: Checks if ped is in melee combat.
extern _native bool IS_CHAR_IN_MELEE_COMBAT(PED_INDEX ped)l

//PURPOSE:  Checks the ped is in the specified model.
extern _native bool IS_CHAR_IN_MODEL(PED_INDEX ped, uint model)l

//PURPOSE: Checks that the ped is in a taxi model.
extern _native bool IS_CHAR_IN_TAXI(PED_INDEX ped)l

//PURPOSE: Returns TRUE if the entity is in water
//		   The vehicle is also checked.
extern _native bool IS_CHAR_IN_WATER(PED_INDEX ped)l

//PURPOSE: Checks if the ped is a specific zone on the map.
//		   If the ped is in a vehicle, then the coords of the vehicle are used in the check. 
extern _native bool IS_CHAR_IN_ZONE(PED_INDEX ped, const char* zonename)l

//INFO: This check should be used on a ped to determine if ped is able to do anything in the game ie run a task.
//The game will assert if to you try to give an injured ped a task. This command will return true if the ped is dead as well
//The IS_CHAR_DEAD command should be used only if you want to specifically know that the ped is dead. i.e. remove a blip, set the dead ped coords.
//PURPOSE: Checks the ped status is injured. Should be used in the majority of checks on a ped. 
extern _native bool IS_CHAR_INJURED(PED_INDEX ped)l

//PURPOSE: Checks if the ped is a male ped.
extern _native bool IS_CHAR_MALE(PED_INDEX ped)l

//PURPOSE: Checks the ped a specific model
extern _native bool IS_CHAR_MODEL(PED_INDEX ped, uint model)l

//PURPOSE: Checks if a ped is on a bike.
extern _native bool IS_CHAR_ON_ANY_BIKE(PED_INDEX ped)l

//PURPOSE: Checks that ped is on foot (not in a vehicle)
extern _native bool IS_CHAR_ON_FOOT(PED_INDEX ped)l

//INFO:  THIS COMMAND DOESN'T TAKE GEOMETRY INTO CONSIDERATION! 
//PURPOSE: Checks if a ped is on screen.
extern _native bool IS_CHAR_ON_SCREEN(PED_INDEX ped)l

//PURPOSE: Checks if the ped is currently playing the animation.
extern _native bool IS_CHAR_PLAYING_ANIM(PED_INDEX ped, const char* anim_dict_name, const char* anim_name)l
extern _native bool IS_CHAR_RESPONDING_TO_ANY_EVENT(PED_INDEX ped)l

//PURPOSE: Checks the ped is responding to an event of the specified type. EVENT_INVALID can be used to test if the ped is responding to any event.
extern _native bool IS_CHAR_RESPONDING_TO_EVENT(PED_INDEX ped, int event_type)l

//PURPOSE: Checks the ped is sitting in any vehicle.
extern _native bool IS_CHAR_SITTING_IN_ANY_CAR(PED_INDEX ped)l

//PURPOSE: Checks the ped is sitting the specified vehicle.
extern _native bool IS_CHAR_SITTING_IN_CAR(PED_INDEX ped, VEHICLE_INDEX vehicle)l

//PURPOSE: Checks that a ped is shooting
extern _native bool IS_CHAR_SHOOTING(PED_INDEX ped)l

//PURPOSE: Checks that a ped is shooting in an area.
extern _native bool IS_CHAR_SHOOTING_IN_AREA(PED_INDEX ped, float min_x, float min_y, float max_x, float max_y, bool highlight_area)l

//PURPOSE: Checks if the ped is stood still or in a stationary vehicle. 
extern _native bool IS_CHAR_STOPPED(PED_INDEX ped)l
extern _native bool IS_CHAR_STUCK_UNDER_CAR(PED_INDEX ped)l

//PURPOSE: Checks that the ped is swimming. 
extern _native bool IS_CHAR_SWIMMING(PED_INDEX ped)l

//PARAM NOTES: This command is only valid for the current frame
//PURPOSE: Checks that 2 peds are intersecting each other this frame. 
//		   If any of the peds are in a vehicle, then the vehicle is used in the check. 
extern _native bool IS_CHAR_TOUCHING_CHAR(PED_INDEX ped, PED_INDEX otherChar)l

//PURPOSE: Checks that ped and object are intersecting each other this frame. 
extern _native bool IS_CHAR_TOUCHING_OBJECT(PED_INDEX ped, OBJECT_INDEX obj)l
extern _native bool IS_CHAR_TOUCHING_OBJECT_ON_FOOT(PED_INDEX ped, OBJECT_INDEX obj)l

//PARAM NOTES: This command is only valid for the current frame
//PURPOSE: Checks that ped and vehicle are intersecting each other this frame. 
//		   If the ped is in a vehicle, then the vehicle is used in the check. 
extern _native bool IS_CHAR_TOUCHING_VEHICLE(PED_INDEX ped, VEHICLE_INDEX vehicle)l

//PURPOSE: Is ped trying to enter a locked vehicle.
extern _native bool IS_CHAR_TRYING_TO_ENTER_A_LOCKED_CAR(PED_INDEX ped)l

//PURPOSE:  Checks that a ped is using any scenario.
extern _native bool IS_CHAR_USING_ANY_SCENARIO(PED_INDEX ped)l

//PURPOSE: Checks that a ped is using a specific scenario.
extern _native bool IS_CHAR_USING_SCENARIO(PED_INDEX ped, const char* scenarioName)l

//PURPOSE: Checks if a ped's IS_VISIBLE flag is set, i.e. whether it's being explicitly hidden (by code or script) or not.
//         Notes that this function returns true for all peds that have their IS_VISIBLE flag set,
//         even those that aren't actually visible from the current camera (i.e. they are outside
//         the view frustum or behind occluders).
extern _native bool IS_CHAR_VISIBLE(PED_INDEX ped)l

//PURPOSE: Cheks if the ped is waiting for world collision to stream in.
extern _native bool IS_CHAR_WAITING_FOR_WORLD_COLLISION(PED_INDEX ped)l

//PURPOSE: Checks if a cop ped is in a given area
extern _native bool IS_COP_PED_IN_AREA_3D_NO_SAVE(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)l

//PURPOSE: Checks that the ped is a leader of a specific group.
extern _native bool IS_GROUP_LEADER(PED_INDEX ped, GROUP_INDEX group)l

//PURPOSE: Checks that the ped is a member of a specific group.
extern _native bool IS_GROUP_MEMBER(PED_INDEX ped, GROUP_INDEX group)l 

//INFO: Will return false if the ped has been set as no longer needed.
//PURPOSE: Checks if the ped has been created by script.
extern _native bool IS_PED_A_MISSION_PED(PED_INDEX ped)l

//PURPOSE: Returns true if the ped is attached to a vehicle
extern _native bool IS_PED_ATTACHED_TO_ANY_CAR(PED_INDEX ped)l

//PURPOSE: Returns true if the ped is attached to the other given object
extern _native bool IS_PED_ATTACHED_TO_OBJECT(PED_INDEX ped, OBJECT_INDEX obj)l

//PURPOSE: Checks if the ped is in the process of being dragged from his vehicle by another ped. 
extern _native bool IS_PED_BEING_JACKED(PED_INDEX ped)l

//PURPOSE: Return if a ped is currently climbing.
extern _native bool IS_PED_CLIMBING(PED_INDEX ped)l

//PURPOSE: Checks if the ped is currently performing the driveby task
extern _native bool IS_PED_DOING_DRIVEBY(PED_INDEX ped)l

//PURPOSE: Checks if the ped is performing any kind of flee task. 
extern _native bool IS_PED_FLEEING(PED_INDEX ped)l
extern _native bool IS_PED_HOLDING_AN_OBJECT(PED_INDEX ped)l

//PURPOSE: Checks if the ped is in combat, if a target ped index is provided, 
// it will return true if the ped is in combat with the target ped, false otherwise
extern _native bool IS_PED_IN_COMBAT(PED_INDEX ped)l

//PURPOSE: Checks if the ped is currently behind and using cover. 
extern _native bool IS_PED_IN_COVER(PED_INDEX ped)l

//PURPOSE: Checks the ped is in any group.
extern _native bool IS_PED_IN_GROUP(PED_INDEX ped)l

//PURPOSE: Checks if the ped is in the process of dragging another ped from a vehicle.
extern _native bool IS_PED_JACKING(PED_INDEX ped)l
extern _native bool IS_PED_LOOKING_AT_CAR(PED_INDEX ped, VEHICLE_INDEX car)l
extern _native bool IS_PED_LOOKING_AT_OBJECT(PED_INDEX ped, OBJECT_INDEX obj)l
extern _native bool IS_PED_LOOKING_AT_PED(PED_INDEX ped, PED_INDEX otherChar)l
extern _native bool IS_PED_PINNED_DOWN(PED_INDEX ped)l

//PURPOSE: Is ped a ragdoll.
extern _native bool IS_PED_RAGDOLL(PED_INDEX ped)l
extern _native bool IS_PED_RETREATING(PED_INDEX ped)l
extern _native bool IS_PEDS_VEHICLE_HOT(PED_INDEX ped)l
extern _native bool IS_PLAYER_BEING_ARRESTED(void)l
extern _native bool IS_RELATIONSHIP_SET(uint Unk493, uint Unk494, uint Unk495)l
extern _native bool LOCATE_CHAR_ANY_MEANS_2D(PED_INDEX ped, float x0, float y0, float x1, float y1, bool highlight_area)l
extern _native bool LOCATE_CHAR_ANY_MEANS_3D(PED_INDEX ped, float x0, float y0, float z0, float x1, float y1, float z1, bool highlight_area)l
extern _native bool LOCATE_CHAR_ANY_MEANS_CAR_2D(PED_INDEX ped, VEHICLE_INDEX car, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_ANY_MEANS_CAR_3D(PED_INDEX ped, VEHICLE_INDEX car, float x, float y, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_ANY_MEANS_CHAR_2D(PED_INDEX ped, PED_INDEX pednext, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_ANY_MEANS_CHAR_3D(PED_INDEX ped, PED_INDEX pednext, float x, float y, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_ANY_MEANS_OBJECT_2D(PED_INDEX ped, OBJECT_INDEX obj, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_ANY_MEANS_OBJECT_3D(PED_INDEX ped, OBJECT_INDEX obj, float x, float y, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_IN_CAR_2D(PED_INDEX ped, float x0, float y0, float x1, float y1, bool highlight_area)l
extern _native bool LOCATE_CHAR_IN_CAR_3D(PED_INDEX ped, float x0, float y0, float z0, float x1, float y1, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_IN_CAR_CAR_2D(PED_INDEX ped, VEHICLE_INDEX car, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_IN_CAR_CAR_3D(PED_INDEX ped, VEHICLE_INDEX car, float x, float y, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_IN_CAR_CHAR_2D(PED_INDEX ped, PED_INDEX pednext, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_IN_CAR_CHAR_3D(PED_INDEX ped, PED_INDEX pednext, float x, float y, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_IN_CAR_OBJECT_2D(PED_INDEX ped, OBJECT_INDEX obj, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_IN_CAR_OBJECT_3D(PED_INDEX ped, OBJECT_INDEX obj, float x, float y, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_ON_FOOT_2D(PED_INDEX ped, float x0, float y0, float x1, float y1, bool highlight_area)l
extern _native bool LOCATE_CHAR_ON_FOOT_3D(PED_INDEX ped, float x0, float y0, float z0, float x1, float y1, float z1, bool highlight_area)l
extern _native bool LOCATE_CHAR_ON_FOOT_CAR_2D(PED_INDEX ped, VEHICLE_INDEX car, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_ON_FOOT_CAR_3D(PED_INDEX ped, VEHICLE_INDEX car, float x, float y, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_ON_FOOT_CHAR_2D(PED_INDEX ped, PED_INDEX pednext, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_ON_FOOT_CHAR_3D(PED_INDEX ped, PED_INDEX pednext, float x, float y, float z, bool highlight_area)l
extern _native bool LOCATE_CHAR_ON_FOOT_OBJECT_2D(PED_INDEX ped, OBJECT_INDEX obj, float x, float y, bool highlight_area)l
extern _native bool LOCATE_CHAR_ON_FOOT_OBJECT_3D(PED_INDEX ped, OBJECT_INDEX obj, float x, float y, float z, bool highlight_area)l
extern _native bool MP_GET_PROP_SETUP(PED_INDEX ped, int ukn0, int ukn1, int* ukn2, int* ukn3)l
extern _native bool SET_CHAR_ANIM_PLAYING_FLAG(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, bool set)l

//INFO:  A ped will only switch to a ragdoll if it's onscreen and within range of the player. 
//PARAM NOTES: MinTime and MaxTime sets the boundary times that the ped will ragdoll for.
//                          script_control will stop the ped from becoming injured so they can complete their behaviours. 
//                          script_control: If you require to control a ped's ragdoll even though they may take additional damage
//                          This both stops the ped from becoming injured and lowers the magnitude of any bullet impact forces. 
//PURPOSE:  Sets a ped into a rag doll state.
extern _native bool SWITCH_PED_TO_RAGDOLL(PED_INDEX ped, int min_time, int max_time, int start_task, bool abort_if_injured, bool abort_if_dead, bool script_control)l

//PARAM NOTES:  direction specifies the direction the ped should stagger and fall 
//                          height specifies the height they're expected to fall down to
//                          grab points are optional, pass in zero to ignore grab
//PURPOSE:  this performs the same function as std SWITCH_PED_TO_RAGDOLL, but gives them a specific task, which controls them to fall off a high place, fall down stairs etc,
extern _native bool SWITCH_PED_TO_RAGDOLL_WITH_FALL(PED_INDEX ped, int min_time, int max_time, int fall_type, float dir_x, float dir_y, float dir_z, float height, float grab1_x, float grab1_y, float grab1_z, float grab2_x, float grab2_y, float grab2_z)l
extern _native bool WAS_PED_KILLED_BY_HEADSHOT(PED_INDEX ped)l

//PURPOSE: Call this and make sure it returns TRUE before calling GET_PED_BONE_POSITION
extern _native bool WAS_PED_SKELETON_UPDATED(PED_INDEX ped)l
extern _native const char* GET_ANIM_GROUP_FROM_CHAR(PED_INDEX  ped)l
extern _native float GET_CHAR_MAX_MOVE_BLEND_RATIO(PED_INDEX ped)l

//PURPOSE: Gets the ped current money. 
extern _native int GET_CHAR_MONEY(PED_INDEX ped)l
extern _native int GET_DAMAGE_TO_PED_BODY_PART(PED_INDEX ped, uint part)l

//INFO: Each ped model can have several drawables for each component. Each drawable can have several textures. 
//PURPOSE: Gets the total number of different drawables for the specified part of the ped's body (component).
extern _native int GET_NUMBER_OF_CHAR_DRAWABLE_VARIATIONS(PED_INDEX ped, uint component)l

//INFO: Each ped model can have several drawables for each component. Each drawable can have several textures. 
//PURPOSE: Gets the total number of different textures for the specified drawable for the component (body part) for the ped's model.
extern _native int GET_NUMBER_OF_CHAR_TEXTURE_VARIATIONS(PED_INDEX ped, uint component, uint DrawableVariationToCheck)l
extern _native int MP_GET_AMOUNT_OF_ANCHOR_POINTS(PED_INDEX ped, int id)l
extern _native int MP_GET_AMOUNT_OF_VARIATION_COMPONENT(PED_INDEX ped, int componentid)l
extern _native int MP_GET_PREFERENCE_VALUE(int prefid)l
extern _native int MP_GET_VARIATION_SETUP(PED_INDEX ped, int component, int Unk891, int* Unk892, int* Unk893)l
extern _native OBJECT_INDEX GET_OBJECT_PED_IS_HOLDING(PED_INDEX ped)l

//INFO: Each ped model can have several drawables for each component. Each drawable can have several textures. 
//PURPOSE:  Gets the index of the current drawable (geometry) for the specified part of the ped's body (component).
extern _native uint GET_CHAR_DRAWABLE_VARIATION(PED_INDEX ped, uint component)l

//INFO: Each ped model can have several drawables for each component. Each drawable can have several textures. 
//PURPOSE: Gets the index of the current texture variation for the specified ped component (part of the body)
extern _native uint GET_CHAR_TEXTURE_VARIATION(PED_INDEX ped, uint component)l
extern _native uint GET_MODEL_PED_IS_HOLDING(PED_INDEX ped)l

extern _native void ADD_ADDITIONAL_POPULATION_MODEL(uint model)l

//PURPOSE: Increments the peds armour
extern _native void ADD_ARMOUR_TO_CHAR(PED_INDEX ped, int amount)l

//INFO: This can be used to tailor specific behaviour types.
//PARAM NOTES: For the Chance parameters, please use values between 0.0 and 1.0.
//PURPOSE: Adds a task response to the decision maker for a particular event
extern _native void ADD_CHAR_DECISION_MAKER_EVENT_RESPONSE(DecisionMaker dm, int event_type, int task_type, float friend_chance, float threat_chance, float player_chance, float other_chance, bool in_car, bool on_foot)l
extern _native void ADD_COMBAT_DECISION_MAKER_EVENT_RESPONSE(DecisionMaker dm, int event_type, int task_type, float friend_chance, float threat_chance, float player_chance, float other_chance, bool in_car, bool on_foot)l
extern _native void ADD_GROUP_DECISION_MAKER_EVENT_RESPONSE(DecisionMaker dm, uint eventid, uint responseid, float x, float y, float z, bool unknown0_1, bool unknown1_1)l
extern _native void ADD_PED_TO_MISSION_DELETION_LIST(PED_INDEX ped, bool Unk29)l

//PURPOSE: Sets an area where scenarios are blocked
extern _native void ADD_SCENARIO_BLOCKING_AREA(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)l
extern _native void ALLOW_AUTO_CONVERSATION_LOOKATS(PED_INDEX ped, bool allow)l
extern _native void ALLOW_GANG_RELATIONSHIPS_TO_BE_CHANGED_BY_NEXT_COMMAND(bool value)l
extern _native void ALLOW_REACTION_ANIMS(PED_INDEX ped, bool allow)l
extern _native void ALLOW_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(bool value)l
extern _native void ALLOW_TARGET_WHEN_INJURED(PED_INDEX ped, bool allow)l

//PURPOSE: Set ped playes head on horn anim when they die in a vehicle.
extern _native void ALWAYS_USE_HEAD_ON_HORN_ANIM_WHEN_DEAD_IN_CAR(PED_INDEX ped, bool use)l

//PARAM NOTES:
//				Vector force to be applied 
//				offset: Offset from centre of ped at which to apply force 
//				Component: Component of the entity to apply the force too - only matters for breakable or articulated (ragdoll) physics. 
//								0 means the root or parent component. I'll add an enum list for the components of a ragdoll, but for other entities such as fragment objects 
//								you'll need to check with CJ or me to find out which component you want to apply a force to if you want to, for example, break off the leg of a chair.
//				local_force: Specifies whether the force vector passed in is in local or world coordinates. Local coordinates (TRUE) means the force will get automatically 
//								transformed into world space before being applied. 
//				local_offset: Specifies whether the offset passed in is in local or world coordinates. 
//				scale_by_mass: Specifies whether to scale the force by mass.
//								TRUE force will be multiplied by mass, ie Force passed in is in fact an acceleration rate in m/s*s (force) or velocity change in m/s(impulse)
//								FALSE force will be applied directly and it's effect will depend on the mass of the object, ie Force passed in is a proper force in Newtons (force) or a step change in momentum kg*m/s (impulse)
//								In other words, scaling by mass is probably easier in most situations - if the mass of the object changes it's behaviour shouldn't, and it's easier to picture the effect because an acceleration rate of 10.0
//PURPOSE: Apply a force to a ped.
extern _native void APPLY_FORCE_TO_PED(PED_INDEX ped, int type, float force_x, float force_y, float force_z, float offset_x, float offset_y, float offset_z, uint component, bool local_force, bool local_offset, bool scale_by_mass)l

//INFO:  None physical attachment - collision is turned off for entity. 
//PARAM NOTES: veh_bone - use 0 for the skeleton root or -1 for entity matrix. Can attach to any bone on the parent - eg attach to ped's hand, attach to car's door 
//				offset - is from the root of the bone you're attaching to (the entity's position if bone=0) 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//				detach_on_death - detaches the ped when he dies
//				detach_on_ragdoll - detaches the ped when he ragdolls
//PURPOSE: Attaches a ped to a vehicle with a non physical attachement.
extern _native void ATTACH_PED_TO_CAR(PED_INDEX ped, VEHICLE_INDEX vehicle, uint veh_bone, float x, float y, float z, float rot_x, float rot_y, bool detach_on_death, bool detach_on_ragdoll)l

//INFO:  None physical attachment - collision is turned off for entity. 
//PARAM NOTES: veh_bone - use 0 for the skeleton root or -1 for entity matrix. Can attach to any bone on the parent - eg attach to ped's hand, attach to car's door 
//				offset - is from the root of the bone you're attaching to (the entity's position if bone=0) 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//				detach_on_death - detaches the ped when he dies
//				detach_on_ragdoll - detaches the ped when he ragdolls
//PURPOSE: Attaches a ped to a vehicle with a non physical attachement.
extern _native void ATTACH_PED_TO_CAR2(PED_INDEX ped, VEHICLE_INDEX vehicle, uint veh_bone, float x, float y, float z, float ro_x, float rot_y, bool detach_on_death, bool detach_on_ragdoll, bool unk)l

//INFO: Physical attachment using rage constraints - entities are still physically active in the world. 
//PARAM NOTES: ped_bone - refers to the bone on the ped which is being attached. 
//			   veh_bone - refers to the bone on the vehicle that you're attaching the ped to. 
//							This lets us attach for example, a ped's hand (ragdoll) to a car door. Some objects are fragment objects, and have a skeleton with multiple pieces that break apart, and this may be useful for them. 
//				offset - the offset from the root of the bone on the the first entity.
//				constrain_rot - specifies whether you wish to constrain rotation as well as position. In most cases the answer will be Yes. Unless you want to have a hanging/swinging thing.
//				collide_with_entity - of set the two entities will collide with each other once attached
//PURPOSE: Attaches a ped to a vehicle using a physical attachment.
extern _native void ATTACH_PED_TO_CAR_PHYSICALLY(PED_INDEX ped, VEHICLE_INDEX car, int pedbone, int vehicle_bone, float offset_x, float offset_y, float offset_z, float strength, bool constrain_rot, bool collide_with_entity)l

//INFO:  None physical attachment - collision is turned off for entity. 
//PARAM NOTES: obj_bone - use 0 for the skeleton root or -1 for entity matrix. Can attach to any bone on the parent - eg attach to ped's hand, attach to car's door 
//				offset - is from the root of the bone you're attaching to (the entity's position if bone=0) 
//				rot - is used to build a rotation matrix, relative to the bone you're attaching too (the entity itself if bone=0)
//				detach_on_death - detaches the ped when he dies
//				detach_on_ragdoll - detaches the ped when he ragdolls
//PURPOSE: Attaches a ped to a object with a non physical attachement.
extern _native void ATTACH_PED_TO_OBJECT(PED_INDEX ped, OBJECT_INDEX obj, int obj_bone, float offset_x, float offset_y, float offset_z, float rot_x, float rot_y, bool detach_on_death, bool detach_on_ragdoll)l

//INFO: Physical attachment using rage constraints - entities are still physically active in the world. 
//PARAM NOTES: ped_bone - refers to the bone on the ped which is being attached. 
//			   obj_bone - refers to the bone on the vehicle that you're attaching the ped to. 
//							This lets us attach for example, a ped's hand (ragdoll) to a car door. Some objects are fragment objects, and have a skeleton with multiple pieces that break apart, and this may be useful for them. 
//				offset - the offset from the root of the bone on the the first entity.
//				constrain_rot - specifies whether you wish to constrain rotation as well as position. In most cases the answer will be Yes. Unless you want to have a hanging/swinging thing.
//				collide_with_entity - of set the two entities will collide with each other once attached
//PURPOSE: Attaches a ped to a object using a physical attachment.
extern _native void ATTACH_PED_TO_OBJECT_PHYSICALLY(PED_INDEX ped, OBJECT_INDEX obj, int pedbone, int obj_bone, float offset_x, float offset_y, float offset_z, float strength, bool constrain_rot, bool collide_with_entity)l
extern _native void ATTACH_PED_TO_WORLD_PHYSICALLY(PED_INDEX ped, uint bone, float x, float y, float z, float bone_x, float bone_y, float bone_z, float strength, bool fix_rotation)l
extern _native void BEGIN_CHAR_SEARCH_CRITERIA(void)l
extern _native void BLEND_FROM_NM_WITH_ANIM(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, VEHICLE_INDEX veh, float blend_x, float blend_y, float blend_z)l
extern _native void BLEND_OUT_CHAR_MOVE_ANIMS(PED_INDEX ped)l

//PURPOSE: Sets that a ped can play ambient idle anims. Even if set, ped can still play ambient base anims.
extern _native void BLOCK_CHAR_AMBIENT_ANIMS(PED_INDEX ped, bool block)l

//PURPOSE:  Sets that the ped can play gesture anims
extern _native void BLOCK_CHAR_GESTURE_ANIMS(PED_INDEX ped, bool value)l

//PURPOSE: Enables/Disables head ik
extern _native void BLOCK_CHAR_HEAD_IK(PED_INDEX ped, bool block)l

//PURPOSE: Sets that a ped can use viseme anims
extern _native void BLOCK_CHAR_VISEME_ANIMS(PED_INDEX ped, bool block)l

//PURPOSE:  Sets the ped can cower whilst in cover
extern _native void BLOCK_COWERING_IN_COVER(PED_INDEX ped, bool set)l

//PURPOSE:  Set if a ped can swap weapons
extern _native void BLOCK_PED_WEAPON_SWITCHING(PED_INDEX ped, bool value)l

//PURPOSE:  Sets the ped can peek whilst in cover
extern _native void BLOCK_PEEKING_IN_COVER(PED_INDEX ped, bool set)l

//PURPOSE: Removes all ped props
extern _native void CLEAR_ALL_CHAR_PROPS(PED_INDEX ped)l
extern _native void CLEAR_ALL_CHAR_RELATIONSHIPS(PED_INDEX ped, int relgroup)l

//PURPOSE: Clears an Event response added to the decision maker via ADD_CHAR_DECISION_MAKER_EVENT_RESPONSE
extern _native void CLEAR_CHAR_DECISION_MAKER_EVENT_RESPONSE(DecisionMaker dm, int event_type)l
extern _native void CLEAR_CHAR_LAST_DAMAGE_BONE(PED_INDEX ped)l

//PURPOSE: Clears the last damage that has been applied to a ped
extern _native void CLEAR_CHAR_LAST_DAMAGE_ENTITY(PED_INDEX ped)l

//PURPOSE: Clear the prop slot for the given ped at the given position.
extern _native void CLEAR_CHAR_PROP(PED_INDEX ped, int position)l
extern _native void CLEAR_CHAR_RELATIONSHIP(PED_INDEX ped, int reltype, int relgroup)l
extern _native void CLEAR_COMBAT_DECISION_MAKER_EVENT_RESPONSE(DecisionMaker dm, int event_type)l
extern _native void CLEAR_GROUP_DECISION_MAKER_EVENT_RESPONSE(DecisionMaker dm, uint eventid)l

//PURPOSE: Clears the area allowing  peds to be created.
extern _native void CLEAR_PED_NON_CREATION_AREA(void)l

//PURPOSE Clears up the area set up by SET_PED_NON_REMOVAL_AREA
extern _native void CLEAR_PED_NON_REMOVAL_AREA(void)l
extern _native void CLEAR_RELATIONSHIP(int p0, int p1, int p2)l
extern _native void CLEAR_ROOM_FOR_CHAR(PED_INDEX ped)l
extern _native void CLEAR_ROOM_FOR_DUMMY_CHAR(DUMMY_PED_INDEX dummy_ped)l
extern _native void CLEAR_SCRIPTED_CONVERSION_CENTRE(void)l
extern _native void COPY_ANIMATIONS(PED_INDEX fromPed, PED_INDEX toPed, float speed0WorksSuspectStarTime)l

//PURPOSE: Creates a copy of a decision maker, passed in name must be unique
extern _native void COPY_CHAR_DECISION_MAKER(DecisionMaker source, DecisionMaker* pDM)l
extern _native void COPY_COMBAT_DECISION_MAKER(DecisionMaker source, DecisionMaker* pDM)l
extern _native void COPY_GROUP_CHAR_DECISION_MAKER(DecisionMaker source, DecisionMaker* pDM)l
extern _native void COPY_GROUP_COMBAT_DECISION_MAKER(DecisionMaker source, DecisionMaker* pDM)l
extern _native void COPY_SHARED_CHAR_DECISION_MAKER(DecisionMaker source, DecisionMaker* pDM)l
extern _native void COPY_SHARED_COMBAT_DECISION_MAKER(DecisionMaker source, DecisionMaker* pDM)l

//PARAM NOTES: network set true if creating a network ped.
// 	       network:		The new object will be created and synced on other machines if a network game is running
//PURPOSE: Creates a ped on foot at the specified co-ordinates with heading.
extern _native void CREATE_CHAR(any unused, uint model, float x, float y, float z, PED_INDEX* pPed, bool network)l
extern _native void CREATE_CHAR_AS_PASSENGER(VEHICLE_INDEX vehicle, any unused, uint model, int seat, PED_INDEX* pPed)l

//PURPOSE: Creates a ped inside at seat number.
extern _native void CREATE_CHAR_INSIDE_CAR(VEHICLE_INDEX vehicle, any unused, uint model, PED_INDEX* pPed)l
extern _native void CREATE_DUMMY_CHAR(any unused, uint model_hash, vector3 pos, const char* unk0, const char* unk1, float unk2, float unk3, DUMMY_PED_INDEX *dummy_ped)l

//INFO:  Every frame, the code automatically removes any group which doesn't have any leader/members. 
//          This means that you have to set a group leader in the same frame as the group is created, as otherwise you'll get an assert about the group ID being out of range (i.e. the group doesn't exist)
//          The Player already has a group so a group doens't need to be created for them.
//PURPOSE: Creates a group.
extern _native void CREATE_GROUP(any unused, GROUP_INDEX* pGroup, bool unk)l

//PURPOSE: Create a Natural motion message.
extern _native void CREATE_NM_MESSAGE(uint start_param, uint type)l

//PURPOSE: Creates a random ped on foot at the specified co-ordinate
extern _native void CREATE_RANDOM_CHAR(float x, float y, float z, PED_INDEX* pPed)l

//PURPOSE: Creates a random ped as the driver of the passed in vehicle. 
extern _native void CREATE_RANDOM_CHAR_AS_DRIVER(VEHICLE_INDEX vehicle, PED_INDEX* pPed)l
extern _native void CREATE_RANDOM_CHAR_AS_PASSENGER(VEHICLE_INDEX vehicle, int seat, PED_INDEX* pPed)l

//PURPOSE: Creates a female character on foot at the specified co-ordinate
extern _native void CREATE_RANDOM_FEMALE_CHAR(float x, float y, float z, PED_INDEX* pPed)l

//PURPOSE: Creates a male character on foot at the specified co-ordinate
extern _native void CREATE_RANDOM_MALE_CHAR(float x, float y, float z, PED_INDEX* pPed)l

//PURPOSE: Applies damage to the ped, can apply to the body armour as well.
extern _native void DAMAGE_CHAR(PED_INDEX ped, int damage, bool damage_armor)l
extern _native void DAMAGE_PED_BODY_PART(PED_INDEX ped, uint part, int damage)l

//PURPOSE: Deletes a ped instantly from the world. 
extern _native void DELETE_CHAR(PED_INDEX* pPed)l
extern _native void DELETE_DUMMY_CHAR(DUMMY_PED_INDEX* dummy_ped)l

//PURPOSE: Detaches an entity from being attached.
//PARAM NOTES: 
//		no_collision_until_clear - disables collision between this object and the parent until they are clear of one another
extern _native void DETACH_PED(PED_INDEX ped, bool no_collision_until_clear)l
extern _native void DETACH_PED_FROM_WITHIN_CAR(PED_INDEX ped, bool unknown)l

//INFO: Blocks eject caused by a player on a different team entering your vehicle.
extern _native void DISABLE_PLAYER_AUTO_VEHICLE_EXIT(PED_INDEX ped, bool disable)l

extern _native void DONT_REMOVE_CHAR(PED_INDEX ped)l
extern _native void DONT_SUPPRESS_ANY_PED_MODELS(void)l
extern _native void DONT_SUPPRESS_PED_MODEL(uint model)l
extern _native void ENABLE_ALL_PED_HELMETS(bool enable)l
extern _native void ENABLE_PED_HELMET(PED_INDEX ped, bool enable)l
extern _native void END_CHAR_SEARCH_CRITERIA(void)l

//PURPOSE: Applies a sniper rifle damage to the ped.
extern _native void EXPLODE_CHAR_HEAD(PED_INDEX ped)l
extern _native void FIRE_PED_WEAPON(PED_INDEX ped, float x, float y, float z)l

//PURPOSE: Removes any areas that are blocking screnarios
extern _native void FLUSH_SCENARIO_BLOCKING_AREAS(void)l

//PARAM NOTES: pinned = FALSE: prevents the ped from getting pinned down for Time milliseconds.
//PURPOSE: Sets the ped to be pinned down for Time milliseconds.
extern _native void FORCE_PED_PINNED_DOWN(PED_INDEX ped, bool pinned, int time)l
extern _native void FORCE_PED_TO_FLEE_WHILST_DRIVING_VEHICLE(PED_INDEX ped, bool set)l

//INFO: Please use this sparingly and remember to reset it after use.
//PURPOSE: Makes sure cover is loaded around them before a set action.
extern _native void FORCE_PED_TO_LOAD_COVER(PED_INDEX ped, bool force)l
extern _native void FORCE_RANDOM_PED_TYPE(uint type)l

//PURPOSE: Sets scenarios peds spawn in an area. 
extern _native void FORCE_SPAWN_SCENARIO_PEDS_IN_AREA(float x, float y, float z, float radius, int max_peds)l

//PURPOSE: Freezes the ped position so that it wont be affected by the world physics.
extern _native void FREEZE_CHAR_POSITION(PED_INDEX ped, bool frozen)l
extern _native void FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION(PED_INDEX ped, bool frozen)l

//PURPOSE: Gets the vehicle that the ped is in or trying to get into.
extern _native void GET_CAR_CHAR_IS_USING(PED_INDEX ped, VEHICLE_INDEX* pVehicle)l
extern _native void GET_CHAR_ANIM_BLEND_AMOUNT(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float* amount)l

//INFO: The phase of the anim is between 0.0 and 1.0 regardless of the anim length
//PURPOSE: Gets the current phase of the anim.
extern _native void GET_CHAR_ANIM_CURRENT_TIME(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float* time)l
extern _native void GET_CHAR_ANIM_TOTAL_TIME(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float* time)l

//PURPOSE: Get the peds current armour.
extern _native void GET_CHAR_ARMOUR(PED_INDEX ped, int* pArmour)l

//PURPOSE: Gets the co-ordinates of the ped. If a ped, this is at the root bone, and if the ped is
//			in a vehicle then it is the coordinates of the vehicle which are returned. 
extern _native void GET_CHAR_COORDINATES(PED_INDEX ped, float* pX, float* pY, float* pZ)l

//PARAM NOTES: If world_space is set to TRUE the result will be in world coordinates (absolute worldspace) If world_space is set to FALSE the result will be in local coordinates (relative to the character) 
//PURPOSE: Gets the displacement (in meters) that will be applied to the given ped this frame.
extern _native void GET_CHAR_EXTRACTED_DISPLACEMENT(PED_INDEX ped, bool world_space, float* pX, float* pY, float* pZ)l
extern _native void GET_CHAR_EXTRACTED_VELOCITY(PED_INDEX ped, bool world_space, float* x, float* y, float* z)l

//PURPOSE: Gets the ped's heading in degrees. If the ped is in a vehicle, then the heading of the 
//  		vehicle is returned;
extern _native void GET_CHAR_HEADING(PED_INDEX ped, float* pValue)l

//PURPOSE: Gets the ped's current health.
extern _native void GET_CHAR_HEALTH(PED_INDEX ped, int* pHealth)l

//PURPOSE: Gets the ped's height above the ground.
extern _native void GET_CHAR_HEIGHT_ABOVE_GROUND(PED_INDEX ped, float* pValue)l
extern _native void GET_CHAR_HIGHEST_PRIORITY_EVENT(PED_INDEX ped, int* event_type)l

//PURPOSE: Get the model name for the entity.
extern _native void GET_CHAR_MODEL(PED_INDEX ped, uint* pModel)l
extern _native void GET_CHAR_MOVE_ANIM_SPEED_MULTIPLIER(PED_INDEX ped, float* multiplier)l

//PURPOSE: Get the id of the prop which has been assigned at the given position for the given ped. 
extern _native void GET_CHAR_PROP_INDEX(PED_INDEX ped, int position, int* prop)l

//PURPOSE: Gets the speed a ped is travelling in a forward direction (m/sec). If the ped is in a vehicle, then the 
//			speed of the vehicle is returned; 
extern _native void GET_CHAR_SPEED(PED_INDEX ped, float* pValue)l

//PURPOSE: Gets the current velocity of the ped. If the ped is in a vehicle, then the 
//			velocity of the vehicle is returned; 
extern _native void GET_CHAR_VELOCITY(PED_INDEX ped, float* pX, float* pY, float* pZ)l
extern _native void GET_CURRENT_BASIC_COP_MODEL(uint* pModel)l
extern _native void GET_CURRENT_COP_MODEL(uint* pModel)l
extern _native void GET_DEAD_CHAR_COORDINATES(PED_INDEX ped, float* pX, float* pY, float* pZ)l

//PURPOSE: Gets a suitable position for creating a pickup near a ped
extern _native void GET_DEAD_CHAR_PICKUP_COORDS(PED_INDEX ped, float* pX, float* pY, float* pZ)l
extern _native void GET_GROUP_FORMATION(GROUP_INDEX group, int* formation)l
extern _native void GET_GROUP_FORMATION_SPACING(GROUP_INDEX group, float* spacing)l

//PURPOSE: Get the ped_index of the group leader. 
extern _native void GET_GROUP_LEADER(GROUP_INDEX group, PED_INDEX* pPed)l

//PARAM NOTES: MemberNumber: Member numbers start at 0 with a maximum of 7 members.
//PURPOSE: Gets the ped_index from the group member.
extern _native void GET_GROUP_MEMBER(GROUP_INDEX group, int index, PED_INDEX* pPed)l

//PURPOSE: Gets the number of leaders (1 or 0) and the number of followers (in the range 0-7) that are in the relevant group. 
extern _native void GET_GROUP_SIZE(GROUP_INDEX group, bool* has_leader, int* size)l

//PURPOSE: Gets the interior from the ped.
extern _native void GET_INTERIOR_FROM_CHAR(PED_INDEX ped, INTERIOR_INSTANCE_INDEX* pInterior)l

//PURPOSE: Gets the interior from the dummy ped.
extern _native void GET_INTERIOR_FROM_DUMMY_CHAR(DUMMY_PED_INDEX dummy_ped, INTERIOR_INSTANCE_INDEX* pInterior)l
extern _native void GET_KEY_FOR_CHAR_IN_ROOM(PED_INDEX ped, uint* pKey)l
extern _native void GET_KEY_FOR_DUMMY_CHAR_IN_ROOM(DUMMY_PED_INDEX dummy_ped, uint* key)l
extern _native void GET_NTH_GROUP_MEMBER(GROUP_INDEX group, int n, PED_INDEX* ped)l
extern _native void GET_NUMBER_OF_FOLLOWERS(PED_INDEX ped, int* followers)l

//PURPOSE: Gets a point described as an offset from that ped and returns the world coordinates of that point
extern _native void GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS(PED_INDEX ped, float x, float y, float z, float* pOffX, float* pOffY, float* pOffZ)l

//INFO: Check that WAS_PED_SKELETON_UPDATED returns TRUE before calling this function
//PURPOSE: Gets the coord of the peds bone
extern _native void GET_PED_BONE_POSITION(PED_INDEX ped, uint bone, float x, float y, float z, vector3* pPosition)l

//PURPOSE: Get the group index form the ped.
extern _native void GET_PED_GROUP_INDEX(PED_INDEX ped, GROUP_INDEX* group)l

//INFO: This script command will assert if you try to give fake names to more than the supported number (currently 10). 
//          After calling this command on a ped, the specified name will be displayed above the peds head in the specified colour in the same style as the network player names.
//PURPOSE: Gives the ped a fake network name and colour in order to simulate network players on script created peds.
extern _native void GIVE_PED_FAKE_NETWORK_NAME(PED_INDEX ped, const char* name, int r, int g, int b, int a)l

//PURPOSE: Returns the type of the ped 
extern _native void GET_PED_TYPE(PED_INDEX ped, uint* pType)l
extern _native void GET_RANDOM_CHAR_IN_AREA_OFFSET_NO_SAVE(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z, PED_INDEX* pPed)l
extern _native void GIVE_PED_AMBIENT_OBJECT(PED_INDEX ped, uint model)l

//PURPOSE: Give the ped a helmet
extern _native void GIVE_PED_HELMET(PED_INDEX ped)l
extern _native void GIVE_PED_HELMET_WITH_OPTS(PED_INDEX ped, bool dont_take_off)l

//INFO: After it has been given, it can be dropped or thrown. If it is to be picked up again it must be flagged as stealable using SET_OBJECT_AS_STEALABLE. 
//PARAM NOTES: The bool use_procedural_attach will default to false if not specified.
//                          use_procedural_attach = false means the object will be attached to the ped either by an attachment bone, or just the offset built into the model.
//                          use_procedural_attach = true means the code will try to work out the attachment offset based on the objects bounding box. 
//PURPOSE: Gives the ped this object using the same system as picking stuff up off the ground.
extern _native void GIVE_PED_PICKUP_OBJECT(PED_INDEX ped, OBJECT_INDEX obj, bool use_procedural_attach)l

//PURPOSE: Knocks a ped off the bike.
extern _native void KNOCK_PED_OFF_BIKE(PED_INDEX ped)l
extern _native void LISTEN_TO_PLAYER_GROUP_COMMANDS(PED_INDEX ped, bool set)l
extern _native void LOAD_CHAR_DECISION_MAKER(int type, DecisionMaker* pDM)l
extern _native void LOAD_COMBAT_DECISION_MAKER(int type, DecisionMaker* pDM)l

//INFO: The ped will added back into the world population and deleted when off screen.
//PURPOSE: Sets the ped to be removed, cannot refer to ped from script.
extern _native void MARK_CHAR_AS_NO_LONGER_NEEDED(PED_INDEX* pPed)l
extern _native void MP_SET_PREFERENCE_VALUE(int prefid, int value)l

//PURPOSE: Registers the all hated peds near the ped and within the radius as threats, these targets will now be attacked when the ped is in TaskCombat
extern _native void REGISTER_HATED_TARGETS_AROUND_PED(PED_INDEX ped, float radius)l

//PURPOSE: Registers the all hated peds in the area as threats, these targets will now be attacked when the ped is in TaskCombat 
extern _native void REGISTER_HATED_TARGETS_IN_AREA(PED_INDEX ped, float x, float y, float z, float radius)l

//PURPOSE: Registers the passed target as a threat, this target will now be attacked when the ped is in TaskCombat
extern _native void REGISTER_TARGET(PED_INDEX ped, PED_INDEX target)l
extern _native void REMOVE_ADDITIONAL_POPULATION_MODEL(uint model)l
extern _native void REMOVE_ALL_INACTIVE_GROUPS_FROM_CLEANUP_LIST(void)l

//PURPOSE: Removes any defensive area given to the ped.
extern _native void REMOVE_CHAR_DEFENSIVE_AREA(PED_INDEX ped)l

//PURPOSE: Removes the PED 
extern _native void REMOVE_CHAR_ELEGANTLY(PED_INDEX* ped)l
extern _native void REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION(PED_INDEX ped, VEHICLE_INDEX car)l

//PURPOSE: Removes a ped from a group.
extern _native void REMOVE_CHAR_FROM_GROUP(PED_INDEX ped)l

//PURPOSE: Removes a decision maker
extern _native void REMOVE_DECISION_MAKER(DecisionMaker dm)l

//INFO: This function is safe to call on a ped that does not have a fake network name given to him. 
//PURPOSE: Removes a fake network name (given by the GIVE_PED_FAKE_NETWORK_NAME command) from the specified ped.
extern _native void REMOVE_FAKE_NETWORK_NAME_FROM_PED(PED_INDEX ped)l

//INFO: If the its the players group it removes all the followers
//PURPOSE: Removes the group.
extern _native void REMOVE_GROUP(GROUP_INDEX group)l

//PURPOSE: Remove a helmet from a ped
extern _native void REMOVE_PED_HELMET(PED_INDEX ped, bool force_remove)l
extern _native void RESET_ADDITIONAL_POPULATION_MODELS(void)l
extern _native void RESET_VISIBLE_PED_DAMAGE(PED_INDEX ped)l

//INFO: This can only be applied to an injured ped
//PURPOSE: Sets an injured ped to full health.
extern _native void REVIVE_INJURED_PED(PED_INDEX ped)l
extern _native void SEARCH_CRITERIA_CONSIDER_PEDS_WITH_FLAG_FALSE(int flagid)l
extern _native void SEARCH_CRITERIA_CONSIDER_PEDS_WITH_FLAG_TRUE(int flagId)l
extern _native void SEARCH_CRITERIA_REJECT_PEDS_WITH_FLAG_FALSE(int flagid)l
extern _native void SEARCH_CRITERIA_REJECT_PEDS_WITH_FLAG_TRUE(int flagId)l

//PURPOSE: Sends the previously allocated NM message to a ped
extern _native void SEND_NM_MESSAGE(PED_INDEX ped)l
extern _native void SET_ADVANCED_BOOL_IN_DECISION_MAKER(DecisionMaker dm, int Unk844, int Unk845, int Unk846, bool Unk847)l
extern _native void SET_ALLOW_DUMMY_CONVERSIONS(bool set)l
extern _native void SET_ANIM_GROUP_FOR_CHAR(PED_INDEX ped, const char* grname)l

//INFO: The main use of this function is to prevent peds from reacting to interrupting events when you want them to be under script control.
//          e.g. To tell a ped to run past a hated ped but ignoring the reaction to attack, set blocking to true at the start, then back to false once the task is completed. 
//PARAM NOTES: set = true : the ped specified will no longer react to permanent events,
//                  e.g. will not begin combat even if the decision maker specifies that seeing a hated ped should.
//                  But the ped will still respond to temporary events like walking around other peds or vehicles. 
//PURPOSE: Sets the blocking of non-temporary events.
extern _native void SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(PED_INDEX ped, bool value)l
extern _native void SET_CAN_TARGET_CHAR_WITHOUT_LOS(PED_INDEX ped, bool set)l

//PURPOSE: Sets the peds shooting accuracy, between 0 to 100.
extern _native void SET_CHAR_ACCURACY(PED_INDEX ped, int value)l

//PURPOSE: Sets all entity's anim speeds.
extern _native void SET_CHAR_ALL_ANIMS_SPEED(PED_INDEX ped, float speed)l

//PURPOSE: Sets if the ped is allowed to duck.
extern _native void SET_CHAR_ALLOWED_TO_DUCK(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_ALLOWED_TO_RUN_ON_BOATS(PED_INDEX ped, bool set)l

//PURPOSE: Sets a non-axis aligned defensive area that a ped will use
extern _native void SET_CHAR_ANGLED_DEFENSIVE_AREA(PED_INDEX ped, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z, float width)l
extern _native void SET_CHAR_ANIM_BLEND_OUT_DELTA(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float amount)l

//PURPOSE: Sets the phase of the anim between 0.0 and 1.0
extern _native void SET_CHAR_ANIM_CURRENT_TIME(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float time)l

//PURPOSE: Sets ped's current anim speed.
extern _native void SET_CHAR_ANIM_SPEED(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float speed)l

//PURPOSE: Sets the hud item to be red but otherwise does not affect the peds behaviour
extern _native void SET_CHAR_AS_ENEMY(PED_INDEX ped, bool value)l

//PURPOSE: Sets a random ped as a mission ped.
extern _native void SET_CHAR_AS_MISSION_CHAR(PED_INDEX ped)l
extern _native void SET_CHAR_BLEEDING(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_BULLETPROOF_VEST(PED_INDEX ped, bool set)l

//PURPOSE: Sets that a ped can be knocked off.
//NOTES: Flag 1 = False, 0 = True. 
extern _native void SET_CHAR_CAN_BE_KNOCKED_OFF_BIKE(PED_INDEX ped, int flag)l

//PURPOSE: Sets if a character can be killed while in a vehicle
extern _native void SET_CHAR_CAN_BE_SHOT_IN_VEHICLE(PED_INDEX ped, bool enabled)l

//PURPOSE: Call every frame to make the ped's ragdoll or weapon bounds shatter glass when in contact
extern _native void SET_CHAR_CAN_SMASH_GLASS(PED_INDEX ped, bool set)l

//PURPOSE: Sets if a ped can be dragged out of a vehicle. 
extern _native void SET_CHAR_CANT_BE_DRAGGED_OUT(PED_INDEX ped, bool enabled)l

//PURPOSE: Set the rate of the climb anim rate
extern _native void SET_CHAR_CLIMB_ANIM_RATE(PED_INDEX ped, float rate)l

//PURPOSE: Sets if a ped uses collision.
extern _native void SET_CHAR_COLLISION(PED_INDEX ped, bool set)l

//INFO:  This command sets the drawable and texture for one component of the ped's model. Each ped model can have several drawables for each component. 
//          Each drawable can have several textures. You should check that the new drawable index is within range for the component (body part) for the ped's model (using GET_NUMBER_OF_CHAR_DRAWABLE_VARIATIONS.
//          The new texture index should be within range for the new drawable (can be checked using GET_NUMBER_OF_CHAR_TEXTURE_VARIATIONS).
//PURPOSE: Sets the drawable, texture and palette for the specified ped component. 
extern _native void SET_CHAR_COMPONENT_VARIATION(PED_INDEX ped, uint component, uint new_drawable_number, uint new_texture_number)l

//	If you set the z component of VecNewCoors to INVALID_WORLD_Z  this command to automatically find the ground z for your entity if there is collision. 
//PURPOSE:  Sets the ped's coords (peds add an -1m offset from the peds origin).
extern _native void SET_CHAR_COORDINATES(PED_INDEX ped, float x, float y, float z)l
extern _native void SET_CHAR_COORDINATES_DONT_CLEAR_PLAYER_TASKS(PED_INDEX ped, float x, float y, float z)l
extern _native void SET_CHAR_COORDINATES_DONT_WARP_GANG(PED_INDEX ped, float x, float y, float z)l
extern _native void SET_CHAR_COORDINATES_DONT_WARP_GANG_NO_OFFSET(PED_INDEX ped, float x, float y, float z)l

//INFO: If you set the z component of VecNewCoors to INVALID_WORLD_Z  this command to automatically find the ground z for your entity if there is collision. 
//PURPOSE:  Sets the ped's coords without adding the offset.
extern _native void SET_CHAR_COORDINATES_NO_OFFSET(PED_INDEX ped, float x, float y, float z)l

//PURPOSE: Gives ped a valid decision maker
extern _native void SET_CHAR_DECISION_MAKER(PED_INDEX ped, DecisionMaker dm)l

//PURPOSE: Sets the decision maker to the default ped decision maker 
extern _native void SET_CHAR_DECISION_MAKER_TO_DEFAULT(PED_INDEX ped)l

//INFO: his might be useful if you create a ped, delete him and then create him again later and need him to look identical to his first appearance. 
//PURPOSE: Sets all drawable and texture variations on the ped to 0.
extern _native void SET_CHAR_DEFAULT_COMPONENT_VARIATION(PED_INDEX ped)l
extern _native void SET_CHAR_DEFENSIVE_AREA_ATTACHED_TO_CAR(PED_INDEX ped, VEHICLE_INDEX veh, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z, float width, bool orientate_with_ped)l

//PURPOSE: Attaches a defensive area SPHERE to a ped that other peds will stay inside.
extern _native void SET_CHAR_DEFENSIVE_AREA_ATTACHED_TO_PED(PED_INDEX ped, PED_INDEX pednext, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z, float width, bool orientate_with_ped)l

//PURPOSE: Sets the heading the Ped will try to turn too.
extern _native void SET_CHAR_DESIRED_HEADING(PED_INDEX ped, float heading)l

//PURPOSE: Sets whether the ped will die instantly if it finds itself in a body of water. 
extern _native void SET_CHAR_DIES_INSTANTLY_IN_WATER(PED_INDEX ped, bool set)l

//PURPOSE: Sets whether a ped takes damage in a sinking vehicle
extern _native void SET_CHAR_DROWNS_IN_SINKING_VEHICLE(PED_INDEX ped, bool set)l

//PURPOSE: Sets whether a ped takes damage in deep water.
extern _native void SET_CHAR_DROWNS_IN_WATER(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_DRUGGED_UP(PED_INDEX ped, bool drugged)l

//INFO: Make sure that SET_CHAR_ALLOWED_TO_DUCK is set true
//PURPOSE: Sets a ped to a ducking pose.
extern _native void SET_CHAR_DUCKING(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_DUCKING_TIMED(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_FIRE_DAMAGE_MULTIPLIER(PED_INDEX ped, float multiplier)l
extern _native void SET_CHAR_FORCE_DIE_IN_CAR(PED_INDEX ped, bool set)l

//PURPOSE: Sets the peds group for gesturing 
extern _native void SET_CHAR_GESTURE_GROUP(PED_INDEX ped, const char* AnimGroup)l
extern _native void SET_CHAR_GET_OUT_UPSIDE_DOWN_CAR(PED_INDEX ped, bool set)l

//PARAM NOTES: false turns on the NO_GRAVITY inst flag
//PURPOSE: Sets whether this ped's physics inst is affected by gravity or not
extern _native void SET_CHAR_GRAVITY(PED_INDEX ped, bool set)l

//PURPOSE: Sets the ped's heading in degrees.
extern _native void SET_CHAR_HEADING(PED_INDEX ped, float heading)l

//PURPOSE: Sets the ped's current health. 
extern _native void SET_CHAR_HEALTH(PED_INDEX ped, int health)l

//INFO: Ensure you only set this for peds that you want to remain active inside a cutscene - not to be used for mission peds that are in the cutscene - the cutscene will deal with that itself. 
//PURPOSE: Allows a MISSION ped to work while a cutscene is running. 
extern _native void SET_CHAR_IN_CUTSCENE(PED_INDEX ped, bool set)l

//PURPOSE: Set ped invincible. 
extern _native void SET_CHAR_INVINCIBLE(PED_INDEX ped, bool enable)l

//PURPOSE: Set ped to be a target priority
extern _native void SET_CHAR_IS_TARGET_PRIORITY(PED_INDEX ped, bool enable)l

//PURPOSE: Sets a ped will keep thier script created task when the mission ends or MARK_CHAR_AS_NO_LONGER_NEEDED is called 
extern _native void SET_CHAR_KEEP_TASK(PED_INDEX ped, bool value)l

//PURPOSE: Sets the peds max health.
extern _native void SET_CHAR_MAX_HEALTH(PED_INDEX ped, int value)l

//PURPOSE: Set the max move blend ratio a ped can move at - needs to be called everyframe, or will revert to sprint
extern _native void SET_CHAR_MAX_MOVE_BLEND_RATIO(PED_INDEX ped, float ratio)l

//PARAM NOTES: time = -1 sets a default
//PURPOSE: Sets the duration in seconds that a ped may be in the water before drowning.
extern _native void SET_CHAR_MAX_TIME_IN_WATER(PED_INDEX ped, float time)l

//PARAM NOTES:  time = -1 sets as default
//PURPOSE: Sets the duration in seconds that a ped may be underwater before drowning.
extern _native void SET_CHAR_MAX_TIME_UNDERWATER(PED_INDEX ped, float time)l
extern _native void SET_CHAR_MELEE_ACTION_FLAG0(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_MELEE_ACTION_FLAG1(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_MELEE_ACTION_FLAG2(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_MELEE_MOVEMENT_CONSTAINT_BOX(PED_INDEX ped, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)l

//PURPOSE: Sets the peds money.
extern _native void SET_CHAR_MONEY(PED_INDEX ped, int amount)l

//INFO: Allows the peds to run faster.  Cannot be used in Network scripts
//PURPOSE: Sets the speed mulitplier of the peds move anims.
extern _native void SET_CHAR_MOVE_ANIM_SPEED_MULTIPLIER(PED_INDEX ped, float multiplier)l
extern _native void SET_CHAR_MOVEMENT_ANIMS_BLOCKED(PED_INDEX ped, bool set)l

//PURPOSE: Set the ped doens't leave the group if the leader is too far away
extern _native void SET_CHAR_NEVER_LEAVES_GROUP(PED_INDEX ped, bool value)l
extern _native void SET_CHAR_NEVER_TARGETTED(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_NOT_DAMAGED_BY_RELATIONSHIP_GROUP(PED_INDEX ped, bool set, eRelationshipGroup group)l

//PURPOSE: Sets the ped is only damaged by the main player
extern _native void SET_CHAR_ONLY_DAMAGED_BY_PLAYER(PED_INDEX ped, bool set)l

//PURPOSE: Set the ped can be only damaged by peds in the given relationship group
extern _native void SET_CHAR_ONLY_DAMAGED_BY_RELATIONSHIP_GROUP(PED_INDEX ped, bool set, eRelationshipGroup relgroup)l

//PURPOSE: Set the proofs on ped: Bullet, Flame, Explosion, Colision, Melee
extern _native void SET_CHAR_PROOFS(PED_INDEX ped, bool bullet, bool flame, bool explosion, bool collision, bool melee)l

//PURPOSE: Set the id of the prop at the given position for the given ped.
extern _native void SET_CHAR_PROP_INDEX(PED_INDEX ped, int position, uint prop)l
extern _native void SET_CHAR_PROP_INDEX_TEXTURE(PED_INDEX ped, int position, uint prop, uint texture)l
extern _native void SET_CHAR_PROVIDE_COVERING_FIRE(PED_INDEX ped, bool set)l

//INFO: Ensures that any skin tones which appear in different textures will be consistent for a character
//PURPOSE: Randomly sets the drawables and textures for a ped from those available. 
extern _native void SET_CHAR_RANDOM_COMPONENT_VARIATION(PED_INDEX ped)l
extern _native void SET_CHAR_READY_TO_BE_EXECUTED(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_READY_TO_BE_STUNNED(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_RELATIONSHIP(PED_INDEX ped, uint relationshipLevel, eRelationshipGroup relationshipGroup)l

//PURPOSE: Sets a ped into a relationship group.
extern _native void SET_CHAR_RELATIONSHIP_GROUP(PED_INDEX ped, eRelationshipGroup relationshipGroup)l

//PURPOSE: Sets the ped's rotation.
extern _native void SET_CHAR_ROTATION(PED_INDEX ped, float xr, float yr, float zr)l

//PARAM NOTES:  rate default value = 100
//PURPOSE: rate should be an integer greater than 0.  100 = Normal shoot rate, lower values will slow the rate, higher values will increase it.
extern _native void SET_CHAR_SHOOT_RATE(PED_INDEX ped, int rate)l
extern _native void SET_CHAR_SIGNAL_AFTER_KILL(PED_INDEX ped, bool set)l

//PURPOSE: Sets a defensive sphere that a ped will use.
extern _native void SET_CHAR_SPHERE_DEFENSIVE_AREA(PED_INDEX ped, float x, float y, float z, float radius)l

//PURPOSE: Sets a passenger ped will stay in the vehicle when jacked. 
extern _native void SET_CHAR_STAY_IN_CAR_WHEN_JACKED(PED_INDEX ped, bool set)l

//PURPOSE: Sets if a healthy character can be killed by a single bullet (e.g. head shot)
extern _native void SET_CHAR_SUFFERS_CRITICAL_HITS(PED_INDEX ped, bool value)l
extern _native void SET_CHAR_USES_DEAFULT_ANIM_GROUP_WHEN_FLEEING(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_USES_UPPERBODY_DAMAGE_ANIMS_ONLY(PED_INDEX ped, bool set)l

//INFO: The speeds are measured in metres per second. 
//PARAM NOTES: The velocity componets should be less than 500.
//PURPOSE: Sets the ped's current velocity.
extern _native void SET_CHAR_VELOCITY(PED_INDEX ped, float x, float y, float z)l

//PURPOSE: Set if the ped is visible (i.e. rendered)
extern _native void SET_CHAR_VISIBLE(PED_INDEX ped, bool value)l
extern _native void SET_CHAR_WALK_ALONGSIDE_LEADER_WHEN_APPROPRIATE(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_WANTED_BY_POLICE(PED_INDEX ped, bool wanted)l
extern _native void SET_CHAR_WATCH_MELEE(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_WILL_COWER_INSTEAD_OF_FLEEING(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_WILL_DO_DRIVEBYS(PED_INDEX ped, bool value)l
extern _native void SET_CHAR_WILL_FLY_THROUGH_WINDSCREEN(PED_INDEX ped, bool value)l
extern _native void SET_CHAR_WILL_LEAVE_CAR_IN_COMBAT(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_WILL_MOVE_WHEN_INJURED(PED_INDEX ped, bool value)l
extern _native void SET_CHAR_WILL_ONLY_FIRE_WITH_CLEAR_LOS(PED_INDEX ped, bool set)l

//PURPOSE: Sets the ped will remain on the boat when the mission ends
extern _native void SET_CHAR_WILL_REMAIN_ON_BOAT_AFTER_MISSION_ENDS(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_WILL_TRY_TO_LEAVE_BOAT_AFTER_LEADER(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_WILL_TRY_TO_LEAVE_WATER(PED_INDEX ped, bool set)l
extern _native void SET_CHAR_WILL_USE_CARS_IN_COMBAT(PED_INDEX ped, bool value)l
extern _native void SET_CHAR_WILL_USE_COVER(PED_INDEX ped, bool value)l
extern _native void SET_COMBAT_DECISION_MAKER(PED_INDEX ped, DecisionMaker dm)l

//PARAM NOTES: If CreateRandomCopsFlag is TRUE, cops can be created, if FALSE, they are blocked.
//PURPOSE: Set to stop random cops being created (applies both to cops on scenarios, and in the wandering population).
extern _native void SET_CREATE_RANDOM_COPS(bool set)l
extern _native void SET_DEAD_CHAR_COORDINATES(PED_INDEX ped, float x, float y, float z)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_CAN_CHANGE_TARGET(DecisionMaker dm, bool value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_CAUTION(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_FIRE_RATE(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_LOW_HEALTH(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_MOVEMENT_STYLE(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_NAVIGATION_STYLE(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_RETREATING_BEHAVIOUR(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_SIGHT_RANGE(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_STANDING_STYLE(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_TARGET_INJURED_REACTION(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_TARGET_LOSS_RESPONSE(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_TEAMWORK(DecisionMaker dm, uint value)l
extern _native void SET_DECISION_MAKER_ATTRIBUTE_WEAPON_ACCURACY(DecisionMaker dm, uint value)l
extern _native void SET_DEFAULT_TARGET_SCORING_FUNCTION(PED_INDEX ped, const char* function)l
extern _native void SET_DISABLE_PLAYER_SHOVE_ANIMATION(PED_INDEX ped, bool disable)l
extern _native void SET_DONT_ACTIVATE_RAGDOLL_FROM_PLAYER_IMPACT(PED_INDEX ped, bool set)l
extern _native void SET_FORCE_PLAYER_TO_ENTER_THROUGH_DIRECT_DOOR(PED_INDEX ped, bool set)l
extern _native void SET_GROUP_CHAR_DECISION_MAKER(PED_INDEX ped, DecisionMaker dm)l
extern _native void SET_GROUP_CHAR_DUCKS_WHEN_AIMED_AT(PED_INDEX ped, bool value)l
extern _native void SET_GROUP_COMBAT_DECISION_MAKER(PED_INDEX ped, DecisionMaker dm)l
extern _native void SET_GROUP_FOLLOW_STATUS(GROUP_INDEX group, bool set)l

//PURPOSE: Sets the formation a group can take
extern _native void SET_GROUP_FORMATION(GROUP_INDEX group, int formation)l

//PURPOSE: Sets the spacing which the group formation is currently using.
//INFO: It is only valid for one script to do this at a time!
//PARAM NOTES: Spacing = space between peds
extern _native void SET_GROUP_FORMATION_SPACING(GROUP_INDEX group, float space)l

//INFO: The player is group leader of the players group and cannot be set leader of another group
//PURPOSE: Set the ped as a group leader.
extern _native void SET_GROUP_LEADER(GROUP_INDEX group, PED_INDEX leader)l

//PURPOSE: Sets a ped as a group member.
extern _native void SET_GROUP_MEMBER(GROUP_INDEX group, PED_INDEX member)l

//PURPOSE: Sets the distance between a group member and the leader before the member leaves the group.
extern _native void SET_GROUP_SEPARATION_RANGE(GROUP_INDEX group, float range)l
extern _native void SET_HEADING_LIMIT_FOR_ATTACHED_PED(PED_INDEX ped, float default_heading, float heading_limit)l
extern _native void SET_INFORM_RESPECTED_FRIENDS(PED_INDEX ped, int Unk43, int Unk44)l
extern _native void SET_LOAD_COLLISION_FOR_CHAR_FLAG(PED_INDEX ped, bool set)l

//PARAM NOTES -1 leaves the vaules unchanged, 0 or greater will sets the new value.
//PURPOSE: Sets the money value of new created peds.
extern _native void SET_MONEY_CARRIED_BY_ALL_NEW_PEDS(int money)l
extern _native void SET_MONEY_CARRIED_BY_PED_WITH_MODEL(uint model, int m0, int m1)l

//PURPOSE: Sets a pose that the natural motion will drive too
extern _native void SET_NM_ANIM_POSE(PED_INDEX ped, const char* anim_dict_name, const char* anim_name, float phase)l

//PURPOSE: Sets a bool value in a previously created message. 
extern _native void SET_NM_MESSAGE_BOOL(uint param, bool value)l

//PURPOSE: Sets a float value in a previously created message. 
extern _native void SET_NM_MESSAGE_FLOAT(uint param, float value)l
extern _native void SET_NM_MESSAGE_INSTANCE_INDEX(uint param, PED_INDEX ped, VEHICLE_INDEX car, OBJECT_INDEX obj)l

//PURPOSE: Sets an int value in a previously created message.
extern _native void SET_NM_MESSAGE_INT(uint param, int value)l
extern _native void SET_NM_MESSAGE_STRING(uint param, const char* string)l

//PURPOSE:  Sets a vector value in a previously created message. 
extern _native void SET_NM_MESSAGE_VEC3(uint param, float x, float y, float z)l
extern _native void SET_PED_ALLOW_MISSION_ONLY_DRIVEBY_USE(PED_INDEX ped, bool set)l

//INFO: Setup a ped to be rendered with alpha, 
extern _native void SET_PED_ALPHA(PED_INDEX ped, int alpha)l
extern _native void SET_PED_COMPONENTS_TO_NETWORK_PLAYERSETTINGS_MODEL(PED_INDEX ped)l

//PARAM NOTES:  mult default = 1.0
//PURPOSE: Set the number of peds on the entire map.
extern _native void SET_PED_DENSITY_MULTIPLIER(float density)l

//PURPOSE: Sets the ped will die when injured.
extern _native void SET_PED_DIES_WHEN_INJURED(PED_INDEX ped, bool value)l
extern _native void SET_PED_DONT_DO_EVASIVE_DIVES(PED_INDEX ped, bool value)l
extern _native void SET_PED_DONT_USE_VEHICLE_SPECIFIC_ANIMS(PED_INDEX ped, bool set)l
extern _native void SET_PED_ENABLE_LEG_IK(PED_INDEX ped, bool set)l
extern _native void SET_PED_FALL_OFF_BIKES_WHEN_SHOT(PED_INDEX ped, bool set)l
extern _native void SET_PED_FORCE_FLY_THROUGH_WINDSCREEN(PED_INDEX ped, bool set)l
extern _native void SET_PED_FORCE_VISUALISE_HEAD_DAMAGE_FROM_BULLETS(PED_INDEX ped, bool set)l

//PURPOSE: If set to TRUE random peds will treat this ped as dead, reacting shocked.  Ped will still generate dead body events when dead if bTrue is set to false.
extern _native void SET_PED_GENERATES_DEAD_BODY_EVENTS(PED_INDEX ped, bool set)l
extern _native void SET_PED_HEEDS_THE_EVERYONE_IGNORE_PLAYER_FLAG(PED_INDEX ped, bool set)l
extern _native void SET_PED_HELI_PILOT_RESPECTS_MINIMUMM_HEIGHT(PED_INDEX ped, bool set)l

//PURPOSE: Sets the helmet texture index to be used instead of default, -1 uses default. This can't be set if the ped has a helmet on already.
extern _native void SET_PED_HELMET_TEXTURE_INDEX(PED_INDEX ped, uint texture)l
extern _native void SET_PED_INSTANT_BLENDS_WEAPON_ANIMS(PED_INDEX ped, bool set)l

//PURPOSE:  Set if motion blur is on or off - used in sky diving 
extern _native void SET_PED_MOTION_BLUR(PED_INDEX ped, bool set)l

//PURPOSE: Sets an area where peds will not be created
extern _native void SET_PED_NON_CREATION_AREA(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)l

//PURPOSE: Sets an area from which random peds are not removed. 
extern _native void SET_PED_NON_REMOVAL_AREA(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)l
extern _native void SET_PED_SKIPS_COMPLEX_COVER_COLLISION_CHECKS(PED_INDEX ped, bool set)l

//PURPOSE: Sets whether the ped will perform low-level steering behaviour around objects (on by default)
extern _native void SET_PED_STEERS_AROUND_OBJECTS(PED_INDEX ped, bool set)l

//PURPOSE: Sets whether the ped will perform low-level steering behaviour around other peds (off by default)
extern _native void SET_PED_STEERS_AROUND_PEDS(PED_INDEX ped, bool set)l
extern _native void SET_PED_WINDY_CLOTHING_SCALE(PED_INDEX ped, float scale)l
extern _native void SET_PED_WITH_BRAIN_CAN_BE_CONVERTED_TO_DUMMY_PED(PED_INDEX ped, bool set)l
extern _native void SET_PED_WONT_ATTACK_PLAYER_WITHOUT_WANTED_LEVEL(PED_INDEX ped, bool set)l
extern _native void SET_PLAYER_SETTINGS_GENRE(PED_INDEX ped)l
extern _native void SET_RELATIONSHIP(uint relationshipLevel, eRelationshipGroup relationshipGroup1, eRelationshipGroup relationshipGroup2)l
extern _native void SET_ROOM_FOR_CHAR_BY_KEY(PED_INDEX ped, uint key)l
extern _native void SET_ROOM_FOR_CHAR_BY_NAME(PED_INDEX ped, const char* roomname)l
extern _native void SET_ROOM_FOR_DUMMY_CHAR_BY_NAME(DUMMY_PED_INDEX dummy_ped, const char* roomname)l
extern _native void SET_ROOM_FOR_DUMMY_CHAR_BY_KEY(DUMMY_PED_INDEX dummy_ped, uint key)l

//PURPOSE: Lets you set the rotation of an attached ped using a rotation vector.
extern _native void SET_ROTATION_FOR_ATTACHED_PED(PED_INDEX ped, float xr, float yr, float zr)l

//PURPOSE: Set the number of scenario peds on the entire map
extern _native void SET_SCENARIO_PED_DENSITY_MULTIPLIER(float interior_mult, float exterior_mult)l

//PURPOSE: Set the distance that the ped can travel when seated in a vehicle. 
extern _native void SET_SCRIPTED_ANIM_SEAT_OFFSET(PED_INDEX ped, float distance)l
extern _native void SET_SCRIPTED_CONVERSION_CENTRE(float x, float y, float z)l
extern _native void SET_SENSE_RANGE(PED_INDEX ped, float range)l
extern _native void SET_SPECIFIC_PASSENGER_INDEX_TO_USE_IN_GROUPS(PED_INDEX ped, int seat)l
extern _native void SET_SWIM_SPEED(PED_INDEX ped, float speed)l

//PURPOSE: Stops the ped searching for walls and ledges when in Natural motion state for dramtic falls.
extern _native void STOP_PED_DOING_FALL_OFF_TESTS_WHEN_SHOT(PED_INDEX ped)l

//PURPOSE: Stops a peds weapon firing when dropped
extern _native void STOP_PED_WEAPON_FIRING_WHEN_DROPPED(PED_INDEX ped)l
extern _native void STORE_CAR_CHAR_IS_IN_NO_SAVE(PED_INDEX ped, VEHICLE_INDEX* car)l
extern _native void SUPPRESS_PED_MODEL(uint model)l

//PURPOSE: Sets a ped from a ragdoll state to an animated state
extern _native void SWITCH_PED_TO_ANIMATED(PED_INDEX ped, bool set)l
extern _native void TOGGLE_CHAR_DUCKING(PED_INDEX ped, bool set)l
extern _native void WARP_CHAR_FROM_CAR_TO_CAR(PED_INDEX ped, VEHICLE_INDEX vehicle, int seat)l
extern _native void WARP_CHAR_FROM_CAR_TO_COORD(PED_INDEX ped, float x, float y, float z)l
extern _native void WARP_CHAR_INTO_CAR(PED_INDEX ped, VEHICLE_INDEX vehicle)l //NOTE: Clears the ped in questions interior/room key.
extern _native void WARP_CHAR_INTO_CAR_AS_PASSENGER(PED_INDEX ped, VEHICLE_INDEX vehicle, int seat)l
extern _native void UNLOCK_RAGDOLL(PED_INDEX ped, bool value)l
extern _native void UNSET_CHAR_MELEE_MOVEMENT_CONSTAINT_BOX(PED_INDEX ped)l
extern _native void UPDATE_PED_PHYSICAL_ATTACHMENT_POSITION(PED_INDEX ped, float x0, float y0, float z0, float x1, float y1, float z1)l

//Deprecated
//STORE_CAR_CHAR_IS_IN
//LOAD_GROUP_DECISION_MAKER
//SET_GROUP_DECISION_MAKER
//START_SETTING_UP_CONVERSATION
//FINISH_SETTING_UP_CONVERSATION
//IS_CONVERSATION_AT_NODE
//IS_CHAR_HOLDING_OBJECT
//SET_GROUP_DEFAULT_TASK_ALLOCATOR
//GIVE_MELEE_ATTACK_TO_CHAR
//IS_PLAYER_IN_POSITION_FOR_CONVERSATION
//ENABLE_CONVERSATION
//CLEAR_CONVERSATION_FOR_CHAR
//START_CHAR_FACIAL_TALK
//STOP_CHAR_FACIAL_TALK
//IS_CHAR_HEAD_MISSING
//SET_UP_CONVERSATION_END_NODE_WITH_SPEECH
//SET_UP_CONVERSATION_NODE_WITH_SCRIPTED_SPEECH
//SET_UP_CONVERSATION_END_NODE_WITH_SCRIPTED_SPEECH
//FINISH_SETTING_UP_CONVERSATION_NO_SUBTITLES
//GET_CHAR_AREA_VISIBLE
//SET_CHAR_HAS_BEEN_CREATED_IN_INTERIOR
//RESTRICT_CHAR_PROPS
//ALLOW_ALL_CHAR_PROPS
//SET_CHAR_NAME_DEBUG
#pragma endregion

#pragma region PAD
extern _native bool IS_AUTO_AIMING_ON(void)l

//	ButtonLabel should be one of the names in the Control Pad Button List
// PURPOSE: Returns TRUE if the specified button on the specified controller is has been pressed this frame.
extern _native bool IS_BUTTON_JUST_PRESSED(int padIndex, PAD_BUTTON_NUMBER button)l

//	ButtonNumber should be one of the names in the PAD_BUTTON_NUMBER
// PURPOSE:  Returns TRUE if the specified button on the specified controller is being pressed/used.
extern _native bool IS_BUTTON_PRESSED(int padIndex, PAD_BUTTON_NUMBER button)l

//PURPOSE: Return if controller button related to an action has just been pressed down.
extern _native bool IS_CONTROL_JUST_PRESSED(int control, int action)l

//PURPOSE: Return if controller button related to an action is down.
extern _native bool IS_CONTROL_PRESSED(int control, int action)l

//PURPOSE: Checks for keys in Game keyboard mode
extern _native bool IS_GAME_KEYBOARD_KEY_JUST_PRESSED(int key)l

//PURPOSE: Checks for keys in Game keyboard mode
extern _native bool IS_GAME_KEYBOARD_KEY_PRESSED(int key)l
extern _native bool IS_IN_CAR_FIRE_BUTTON_PRESSED(void)l

//PURPOSE: Checks for keys in Level Design keyboard mode
extern _native bool IS_KEYBOARD_KEY_JUST_PRESSED(int key)l

//PURPOSE: Checks for keys in Level Design keyboard mode.
extern _native bool IS_KEYBOARD_KEY_PRESSED(int key)l

// PURPOSE: Checks if the gamepad look is inverted.
extern _native bool IS_LOOK_INVERTED(void)l
extern _native bool IS_MOUSE_USING_VERTICAL_INVERSION(void)l

// PURPOSE:  returns whether standard (true) or classic (false) pad control.
extern _native bool USING_STANDARD_CONTROLS(void)l
extern _native int GET_ACCEPT_BUTTON(void)l

//PURPOSE: Return controller value related to an action (range 0 to 255)
extern _native int GET_CONTROL_VALUE(int control, int action)l
extern _native int GET_PAD_STATE(int pad_number, PAD_BUTTON_NUMBER button)l
extern _native void CLEAR_SHAKE_PLAYERPAD_WHEN_CONTROLLER_DISABLED(void)l

//PARAM NOTES: A value of 0 means that the stick is in the central position. 
// PURPOSE: Gets the position of analogue stickes between -128 and 128.
extern _native void GET_POSITION_OF_ANALOGUE_STICKS(int padIndex, int* pLeftX, int* pLeftY, int* pRightX, int* pRightY)l

//PARAM NOTES: Frequency should be less than 256.
//	Duration is in milliseconds
// PURPOSE: Causes the given Pad, assigned to a Control, to shake.
extern _native void SHAKE_PAD(int control, int duration, int frequency)l
extern _native void SHAKE_PAD_IN_CUTSCENE(int control, int duration, int frequency)l
extern _native void SHAKE_PLAYERPAD_WHEN_CONTROLLER_DISABLED(void)l

//Deprecated
//IS_PC_USING_JOYPAD
//GET_MOTION_SENSOR_VALUES
//GET_PAD_ORIENTATION
//GET_PAD_PITCH_ROLL
//HAS_RELOADED_WITH_MOTION_CONTROL
//SET_ALL_MOTION_CONTROL_PREFERENCES_ON_OFF
//GET_MOTION_CONTROL_PREFERENCE
//GET_MOTION_CONTROLS_ENABLED
#pragma endregion

#pragma region MISC
extern _native bool ALLOW_ONE_TIME_ONLY_COMMANDS_TO_RUN(void)l

//PURPOSE: Returns whether the credits have reached the end
extern _native bool ARE_CREDITS_FINISHED(void)l

//PURPOSE: Will return TRUE if autosave is switched off in frontend or the saving spinning disc help message is being displayed.
extern _native bool CAN_START_MISSION_PASSED_TUNE(void)l

//PURPOSE: Returns true if the cheat code has been entered.
extern _native bool CHEAT_HAPPENED_RECENTLY(int cheat, int time)l

//PURPOSE: Checks if 2 strings are equal. (Ignores case e.g. "MyString" and "mystring" are equal)
extern _native bool COMPARE_STRING(const char* str0, const char* str1)l

//PURPOSE: Checks that the save completed.
extern _native bool DID_SAVE_COMPLETE_SUCCESSFULLY(void)l

//INFO: The command will return TRUE if it finds collision, FALSE if not. 
//PURPOSE: Trys to store the Z coordinate of the highest ground below the given point.
extern _native bool GET_GROUND_Z_FOR_3D_COORD(float x, float y, float z, float* pGroundZ)l
extern _native bool GET_IS_AUTOSAVE_OFF(void)l
extern _native bool GET_IS_DISPLAYINGSAVEMESSAGE(void)l
extern _native bool GET_MISSION_FLAG(void)l
extern _native bool GET_OVERRIDE_NO_SPRINTING_ON_PHONE_IN_MULTIPLAYER(void)l

//PURPOSE: Checks if an area is ocupied by an entity/entities..
extern _native bool IS_AREA_OCCUPIED(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z, bool building, bool vehicle, bool ped, bool object, bool dummy_ped)l

//PURPOSE: Returns TRUE if an autosave is currently in progress or has been queued to be done after other savegame operations have finished.
extern _native bool IS_AUTO_SAVE_IN_PROGRESS(void)l

//PARAM NOTES: The index (BitToCheckIndex) passed must be between 0 and 31 or the command will ASSERT. 
//PURPOSE: Checks if the integer (IntToCheck) passed has the bit set at the index (BitToCheckIndex) passed.
extern _native bool IS_BIT_SET(int val, int bitnum)l

//PARAM NOTES: is_player checks if the player fired the bullet.
//PURPOSE: Checks that a bullet is in the defined sphere.
extern _native bool IS_BULLET_IN_AREA(float x, float y, float z, float radius, bool is_player)l

//PARAM NOTES: is_player checks if the player fired the bullet.
//PURPOSE: Checks that a bullet is in the area.
extern _native bool IS_BULLET_IN_BOX(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z, bool is_player)l
extern _native bool IS_EPISODE_AVAILABLE(int episode)l
extern _native bool IS_EPISODIC_DISC_BUILD(void)l

//PURPOSE:  Only returns FALSE if no frontend fade is running
extern _native bool IS_FRONTEND_FADING(void)l

//INFO: Can be used to stop certain cheats activating. 
//PURPOSE: Chscks if the game is in an interior.
extern _native bool IS_INTERIOR_SCENE(void)l

//PURPOSE:  Checks if a memory card is in use.
extern _native bool IS_MEMORY_CARD_IN_USE(void)l

//PURPOSE: Checks if a minigame is in progress.
extern _native bool IS_MINIGAME_IN_PROGRESS(void)l

//PURPOSE: Returns TRUE if any mission-created vehicles, peds or objects are in the defined area. 
extern _native bool IS_POINT_OBSCURED_BY_A_MISSION_ENTITY(float center_x, float center_y, float center_z, float dimensions_x, float dimensions_y, float dimensions_z)l

//PURPOSE: Checks that a projectile object is in the area.
extern _native bool IS_PROJECTILE_IN_AREA(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)l

//PURPOSE:  Checks that the sniper bullet is a area.
extern _native bool IS_SNIPER_BULLET_IN_AREA(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)l

//PURPOSE: Checks if the sniper rifle control is inverted.
extern _native bool IS_SNIPER_INVERTED(void)l

//PURPOSE: Checks if the string is NULL.
extern _native bool IS_STRING_NULL(const char* str)l

//INFO: It is possible for more than one minigame script to be running at the same time.
//PURPOSE: Checks that this is a mingame script.
extern _native bool IS_THIS_A_MINIGAME_SCRIPT(void)l

//PURPOSE: Converts a string to an int
extern _native bool STRING_TO_INT(const char* str, int* intval)l
extern _native const char* GET_EPISODE_NAME(int episodeIndex)l

//PURPOSE: Checks the absolute value for an float, any negtaives will be returned as positive.
extern _native float ABSF(float value)l

//PURPOSE: Caluclates the arccos.
extern _native float ACOS(float x)l

//PURPOSE: Caluclates the arcsin.
extern _native float ASIN(float value)l

//PURPOSE: Caluclates the arctan.
extern _native float ATAN(float value)l
extern _native float ATAN2(float x, float y)l
extern _native float GET_LINE_HEIGHT(void)l
extern _native float GET_STRING_WIDTH(const char* gxtName)l
extern _native float GET_STRING_WIDTH_WITH_STRING(const char* gxtName, const char* literalString)l
extern _native float GET_STRING_WIDTH_WITH_NUMBER(const char* gxtName, int number)l
extern _native float GET_WIDTH_OF_LITERAL_STRING(const char* str)l

//PURPOSE: Caluclates the tan.
extern _native float TAN(float value)l

//PURPOSE: Checks the absolute value for an int, any negtaives will be returned as positive.
extern _native int ABSI(int value)l

//INFO: Returns if any bits in the range are true - Nath :D
extern _native int GET_BITS_IN_RANGE(int val, int rangebegin, int rangeend)l
extern _native int GET_CURRENT_EPISODE(void)l

//INFO: Returns the selected language of the player profile
//PURPOSE: Used to check the language/region of the current player
extern _native int GET_CURRENT_LANGUAGE(void)l

//PURPOSE: Gets the size of the calling scripts current stack size.
extern _native int GET_CURRENT_STACK_SIZE(void)l

//PURPOSE: Get game profile setting.
extern _native int GET_PROFILE_SETTING(int settingid)l
extern _native int READ_KILL_FRENZY_STATUS(void)l

//PURPOSE: Creates a hash key for the given string
extern _native uint GET_HASH_KEY(const char* value)l
extern _native uint REGISTER_SAVE_HOUSE(float x, float y, float z, float heading, const char* name, uint map_area)l

//PURPOSE: Sets a cheat active.
extern _native void ACTIVATE_CHEAT(int cheat)l//0 - 15

//INFO: This is where the player skin gets selected.
//PURPOSE: Activates the frontend network settings menu.
extern _native void ACTIVATE_NETWORK_SETTINGS_MENU(void)l

//PURPOSE: Displays the save menu.
extern _native void ACTIVATE_SAVE_MENU(void)l

//INFO: Currently, a maximum of 10 of each type of restart point can be defined on the map, but this can be increased if necessary.
//  Z will be calculated automatically if you give it a value of 100.0 or below. 
//  The player will also be given the correct heading when he is restarted. 
//PURPOSE: Adds a coord where the player will be restarted after he has been killed.
extern _native void ADD_HOSPITAL_RESTART(float x, float y, float z, float heading, int when_to_use)l

//INFO:  Currently, a maximum of 10 of each type of restart point can be defined on the map, but this can be increased if necessary.
//  Z will be calculated automatically if you give it a value of 100.0 or below. 
//  The player will also be given the correct heading when he is restarted. 
//PURPOSE: Adds a coord where the player will be restarted after he has been arrested.
extern _native void ADD_POLICE_RESTART(float x, float y, float z, float heading, int when_to_use)l

//INFO: The text is a literal string as set up in the script. This means the text cannot be translated but this is ok as the scrollbar is in NY where it would be in English. 
//PURPOSE: Add texts to a news scrollbar.
extern _native void ADD_STRING_TO_NEWS_SCROLLBAR(const char* str)l

//PARAM NOTES:The first two vector parameters define a box which will start the stunt jump. 
//  The 3rd and 4th vector parameters define a box which will complete the stunt jump (if the player enters that area within a few seconds) 
//  The camera will change to a fixed camera at the given position.
//  Reward: is the amount of points the player receives if he lands in the end box
//PURPOSE:  Adds a stunt jump area in the map.
extern _native void ADD_STUNT_JUMP(float start_min_x, float start_min_y, float start_min_z, float start_max_x, float start_max_y, float start_max_z, float end_min_x, float end_min_y, float end_min_z, float end_max_x, float end_max_y, float end_max_z, float cam_pos_x, float cam_pos_y, float cam_pos_z, int reward)l

//PURPOSE: Sets if the stunt jumps are active.
extern _native void ALLOW_STUNT_JUMPS_TO_TRIGGER(bool allow)l

//PURPOSE:  If this command is called with FALSE then this script will not be paused if another script calls PAUSE_GAME
extern _native void ALLOW_THIS_SCRIPT_TO_BE_PAUSED(bool allows)l
extern _native void ATTACH_PARACHUTE_MODEL_TO_PLAYER(PED_INDEX ped, OBJECT_INDEX obj)l

//PURPOSE: Clears any restart overide. 
extern _native void CANCEL_OVERRIDE_RESTART(void)l

//PURPOSE: Clears the non axis aligned area of all all non-mission vehicles 
extern _native void CLEAR_ANGLED_AREA_OF_CARS(float x0, float y0, float z0, float x1, float y1, float z1, float distance_of_opposite_face)l

//INFO: All fires and explosions in the area are also cleared
//PARAM NOTES: All projectiles in the world are removed if delete_projectiles is set to TRUE. 
//PURPOSE: Clears all non-mission cars and peds within the defined sphere.
extern _native void CLEAR_AREA(float x, float y, float z, float radius, bool delete_projectiles)l

//PURPOSE: Clears all non-mission vehicles within the defined sphere.
extern _native void CLEAR_AREA_OF_CARS(float x, float y, float z, float radius)l

//PURPOSE: Clears all non-mission peds within the defined sphere.
extern _native void CLEAR_AREA_OF_CHARS(float x, float y, float z, float radius)l

//PURPOSE: Clears all non-mission cops within the defined sphere. 
extern _native void CLEAR_AREA_OF_COPS(float x, float y, float z, float radius)l

//PURPOSE: Clears all non-mission objects within the defined sphere. 
extern _native void CLEAR_AREA_OF_OBJECTS(float x, float y, float z, float radius)l

//PARAM NOTES:The index (BitToClearIndex) passed must be between 0 and 31 or the command will ASSERT. 
//PURPOSE: Clear the bit at the index (BitToClearIndex) of the integer (IntToModify) passed.
extern _native void CLEAR_BIT(int* val, int bit)l

//PURPOSE: Clears the text that is currently displayed on the news scrollbar
extern _native void CLEAR_NEWS_SCROLLBAR(void)l
extern _native void DEACTIVATE_NETWORK_SETTINGS_MENU(void)l

//PURPOSE: Runs an auto save.
extern _native void DO_AUTO_SAVE(void)l
extern _native void ENABLE_SAVE_HOUSE(uint savehouse, bool enable)l

//PURPOSE:  Decides whether the Xbox display will fade after several minutes with no user input
extern _native void ENABLE_XBOX_SCREEN_SAVER(bool enable)l
extern _native void FAIL_KILL_FRENZY(void)l
extern _native void FIRE_SINGLE_BULLET(float x, float y, float z, float targetX, float targetY, float targetZ, uint unknown)l
extern _native void FLUSH_ALL_OUT_OF_DATE_RADAR_BLIPS_FROM_MISSION_CLEANUP_LIST(void)l
extern _native void FORCE_POPULATION_INIT(void)l
extern _native void FORCE_WEATHER(uint weather)l
extern _native void FORCE_WEATHER_NOW(uint weather)l
extern _native void FORCE_WIND(float wind)l
extern _native void GENERATE_RANDOM_FLOAT(float* value)l

//PARAM NOTES: Default vaules min float = 0 and max float = 1.0
//PURPOSE: Gets a random float from the range passed in.
extern _native void GENERATE_RANDOM_FLOAT_IN_RANGE(float min, float max, float* pValue)l
extern _native void GENERATE_RANDOM_INT(int* value)l

//PARAM NOTES: Default vaules min int = 0 and max int = 65535
//PURPOSE: Gets a random int from the range passed in.
// Be careful - this will only ever return integers between min and (max-1) inclusive.
//  e.g. GENERATE_RANDOM_INT_IN_RANGE(0, 5, out) will only ever return 0, 1, 2, 3 or 4. It will never return 5.
extern _native void GENERATE_RANDOM_INT_IN_RANGE(int min, int max, int* pValue)l

//INFO: The returned value will be between 0 (2 vectors are parallel) to 180.0 (vectors are travelling in opposite directions). 
//PURPOSE: Gets the angle in degrees between 2 vectors.
extern _native void GET_ANGLE_BETWEEN_2D_VECTORS(float x1, float y1, float x2, float y2, float* pResult)l
extern _native void GET_CURRENT_WEATHER(uint* pWeather)l
extern _native void GET_CURRENT_WEATHER_FULL(uint* weatherparam0, uint* weatherparam1, uint* weatherparam2)l

//PURPOSE: Gets the distance between 2 coords.
extern _native void GET_DISTANCE_BETWEEN_COORDS_2D(float x1, float y1, float x2, float y2, float* pDist)l

//PURPOSE: Gets the distance between 2 coords.
extern _native void GET_DISTANCE_BETWEEN_COORDS_3D(float x1, float y1, float z1, float x2, float y2, float z2, float* pDist)l

//PURPOSE: Returns the time since the last frame (in seconds)
extern _native void GET_FRAME_TIME(float* time)l

//INFO: The game timer is counted in milliseconds and starts at 0 when the game begins. This command could be useful if you want to trigger something after a number of milliseconds but cant use a WAIT
//PURPOSE: Gets the current game timer in milliseconds.
extern _native void GET_GAME_TIMER(int* pTimer)l

//PURPOSE: Gets a heading in degrees between the x and y vector components.
extern _native void GET_HEADING_FROM_VECTOR_2D(float x, float y, float* pHeading)l

//PURPOSE: Gets the offest point returned in world coords.
extern _native void GET_INTERIOR_HEADING(INTERIOR_INSTANCE_INDEX interior, float* pHeading)l

//PURPOSE: Gets the dimensions of a model. 
extern _native void GET_MODEL_DIMENSIONS(uint model, vector3* pMinVector, vector3* pMaxVector)l
extern _native void GET_OFFSET_FROM_INTERIOR_IN_WORLD_COORDS(INTERIOR_INSTANCE_INDEX interior, float x, float y, float z, vector3* pOffset)l
extern _native void IMPROVE_LOW_PERFORMANCE_MISSION_PER_FRAME_FLAG(void)l
extern _native void LIMIT_ANGLE(float angle, float* anglelimited)l

//INFO: Implemented with acitivities in mind (e.g. pool game). 
//PURPOSE: Forces all exterior peds and vehicles to freeze, regardless of player's position.
extern _native void OVERRIDE_FREEZE_FLAGS(bool boverride)l

//INFO: In some missions, you might want to restart the player at the point on the map where the mission is started. 
//  If this command is called then the next time the player is restarted, he will be positioned at this point rather than at the closest hospital or police restart point.
//  Z will be calculated automatically if you give it a value of 100.0 or below. When the player restarts, his heading will be set according to PlayerHeading. 
//PURPOSE: Ses a point where the player will restarted which overrides the defaults.
extern _native void OVERRIDE_NEXT_RESTART(float x, float y, float z, float heading)l

//PURPOSE: Sets the game pause state which causes everything but the current script and the scripts it spawns
extern _native void PAUSE_GAME(void)l

//INFO: e.g. reducing the number of real peds in favour of dummy peds
//          Can you check with Obbe before using this.
//          Has to be called every frame during the slow mission section, please only call
//          It during the specifically slow part (e.g. gunfight) and not the whole mission. 
//PURPOSE:  Makes some changes to core systems to try and improve performance.
extern _native void POPULATE_NOW(void)l
extern _native void PROCESS_MISSION_DELETION_LIST(void)l
extern _native void RELEASE_WEATHER(void)l

//PARAM NOTES:The index (BitToSetIndex) passed must be between 0 and 31 or the command will ASSERT.
//PURPOSE: Sets the bit at the index (BitToSetIndex) of the integer (IntToModify) passed.
extern _native void SET_BIT(int* val, int bit)l
extern _native void SET_BITS_IN_RANGE(int* variable, int rangebegin, int rangeend, int val)l //maybe it would want 111111 etc as the val
extern _native void SET_CLEAR_MANIFOLDS(bool set)l

//PURPOSE: Sets the end credits to fade out with screen or not (FALSE as default)
extern _native void SET_CREDITS_TO_RENDER_BEFORE_FADE(bool set)l
extern _native void SET_DEFAULT_GLOBAL_INSTANCE_PRIORITY(void)l
extern _native void SET_EXTRA_HOSPITAL_RESTART_POINT(float x, float y, float z, float Unk489, float Unk490)l
extern _native void SET_EXTRA_POLICE_STATION_RESTART_POINT(float x, float y, float z, float Unk491, float Unk492)l

//PURPOSE: Call this with FALSE before saving if you don't want the screen to fade in when loading this save. You have to call the command again with TRUE after the save has been made
extern _native void SET_FADE_IN_AFTER_LOAD(bool set)l
extern _native void SET_FAKE_WANTED_CIRCLE(float x, float y, float z)l

//INFO: iWantedLevel is the amount of stars you wish to display on the HUD - NOTE this only shows the number of stars - it doesnt do anything with your 'real' wanted level or anything with any police. 
//  It will only work if the real wanted level is 0, otherwise the real wanted level will override it. 
//PURPOSE: Draws a fake number of wanted stars.
extern _native void SET_FAKE_WANTED_LEVEL(int lvl)l

//INFO: All object instances will soon be assigned a priority from 0 to 3 by the map guys (0 being the most important). 
//  All objects with a priority less than this value will not be loaded and rendered. 
//  Therefore setting the value to 0 will only load and render the most important objects
//  The is also an in game slider in the Optimization widgets called "instance lvl" to get/set the current value for testing 
//PARAM NOTES: Possible values of 0 to 3 inclusive. Defaults to 3
//PURPOSE:  Sets the global instance priority.
extern _native void SET_GLOBAL_INSTANCE_PRIORITY(int priority)l
extern _native void SET_GRAVITY_OFF(bool set)l
extern _native void SET_KILLSTREAK(void)l

//PURPOSE: Displays the envelope icon near radar to say you have unread text messages waiting 
extern _native void SET_MESSAGES_WAITING(bool set)l

//INFO: set should be a BOOL. Every time the player starts a mission, you must remember to set this variable to TRUE.
//PURPOSE: Sets the script as a mission script.
extern _native void SET_MISSION_FLAG(bool isMission)l

//PURPOSE: Informs the game of a mini-games status. 
extern _native void SET_MINIGAME_IN_PROGRESS(bool set)l

//PARAM NOTES: default behaviour of no sprinting when on the phone. 
//PURPOSE: Sets if the player can sprint when on the phone, default is no sprint.
extern _native void SET_OVERRIDE_NO_SPRINTING_ON_PHONE_IN_MULTIPLAYER(bool set)l

//PURPOSE: Displays the relevant hud item.
extern _native void SET_PHONE_HUD_ITEM(int type, const char* gxttext, int number)l
extern _native void SET_PHYS_CCD_HANDLES_ROTATION(bool set)l

//INFO: This command (called with FALSE) is needed so that the player doesn't regain control for the short gap between the end of the first mission (The Tour) and the script that introduces the savegame bed and the radar. 
//PARAM NOTES: The default setting is TRUE.
//PURPOSE: Sets the player control at the after script has terminated.
extern _native void SET_PLAYER_CONTROL_ON_IN_MISSION_CLEANUP(bool set)l

//PURPOSE: Creates a random seed 
extern _native void SET_RANDOM_SEED(uint seed)l

//PURPOSE: Sets the sleep mode of the mobile.
extern _native void SET_SLEEP_MODE_ACTIVE(bool set)l

//PURPOSE:  Required so that the debug and main scripts are able to remove blips created by other scripts
//              when Keith's game flow code skips forward or back
extern _native void SET_THIS_SCRIPT_CAN_REMOVE_BLIPS_CREATED_BY_ANY_SCRIPT(bool allow)l

//INFO: This command will probably only be of use in cut scenes. For a slow motion effect, try setting timescale to 0.5. 
//          Remember to set the Time Scale back to this once you have finished with the special effect. 
//PARAM NOTES: normal running of the game is 1.0 
//PURPOSE: Sets the time scale of the game to make it run slower.
extern _native void SET_TIME_SCALE(float scale)l

//PURPOSE: Sets the end credits playing state
extern _native void START_CREDITS(void)l
extern _native void START_KILL_FRENZY(const char* gxtname, uint Unk512, uint Unk513, uint Unk514, uint Unk515, uint Unk516, uint Unk517, uint Unk518, bool Unk519)l
extern _native void STOP_CREDITS(void)l
extern _native void SUPPRESS_FADE_IN_AFTER_DEATH_ARREST(bool set)l

//PURPOSE:  Kills all scripts which don't have THIS_SCRIPT_IS_SAFE_FOR_NETWORK_GAME in it. 
extern _native void TERMINATE_ALL_SCRIPTS_FOR_NETWORK_GAME(void)l

//PURPOSE: Terminates all scripts with the specified name.
extern _native void TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(const char* name)l

//PURPOSE:  Tell the game not to remove this script when TERMINATE_ALL_SCRIPTS_FOR_NETWORK_GAME is called
extern _native void THIS_SCRIPT_IS_SAFE_FOR_NETWORK_GAME(void)l
extern _native void THIS_SCRIPT_SHOULD_BE_SAVED(void)l
extern _native void UNPAUSE_GAME(void)l

//Deprecated
//PICK_RANDOM_WEATHER
//IS_XBOX360_VERSION
//IS_JAPANESE_VERSION
//IS_GERMAN_VERSION
//IS_AUSSIE_VERSION
//IS_IN_ATTRACT_MODE
//SET_UP_TRIP_SKIP
//CLEAR_UP_TRIP_SKIP
//SET_UP_TRIP_SKIP_AFTER_MISSION
//SET_UP_TRIP_SKIP_FOR_SPECIFIC_VEHICLE
//SET_UP_TRIP_SKIP_TO_BE_FINISHED_BY_SCRIPT
//SET_UP_TRIP_SKIP_FOR_VEHICLE_FINISHED_BY_SCRIPT
//IS_TRIP_SKIP_WAITING_FOR_SCRIPT_TO_FADE_IN
#pragma endregion

#pragma region HUD
extern _native BLIP_INDEX GET_FIRST_BLIP_INFO_ID(uint type)l
extern _native BLIP_INDEX GET_NEXT_BLIP_INFO_ID(uint type)l
extern _native BLIP_INDEX GET_SIMPLE_BLIP_ID(void)l
extern _native bool CAN_FONT_BE_LOADED(uint font)l
extern _native bool CAN_RENDER_RADIOHUD_SPRITE_IN_MOBILE_PHONE(void)l

// PURPOSE: Check blip exist
extern _native bool DOES_BLIP_EXIST(BLIP_INDEX blip)l
extern _native bool DOES_TEXT_LABEL_EXIST(const char* gxtentry)l

//PURPOSE: Returns TRUE if DISPLAY_NON_MINIGAME_HELP_MESSAGES(TRUE) has previously been called in this script
extern _native bool DOES_THIS_MINIGAME_SCRIPT_ALLOW_NON_MINIGAME_HELP_MESSAGES(void)l

//PURPOSE: Checks that the text for the slot has loaded.
extern _native bool HAS_ADDITIONAL_TEXT_LOADED(int textIndex)l

//PURPOSE: Checks that the text block for the slot has loaded.
extern _native bool HAS_THIS_ADDITIONAL_TEXT_LOADED(const char* textName, int textIndex)l

//PURPOSE:  Returns whether blip is short range or not
extern _native bool IS_BLIP_SHORT_RANGE(BLIP_INDEX blip)l
extern _native bool IS_FONT_LOADED(uint font)l
extern _native bool IS_HELP_MESSAGE_BEING_DISPLAYED(void)l
extern _native bool IS_HUD_PREFERENCE_SWITCHED_ON(void)l
extern _native bool IS_HUD_RETICULE_COMPLEX(void)l

//PURPOSE: Checks if any message is being displayed at the bottom of the screen. 
extern _native bool IS_MESSAGE_BEING_DISPLAYED(void)l

//PURPOSE: Checks if the pause menu is active.
extern _native bool IS_PAUSE_MENU_ACTIVE(void)l

//PURPOSE: The text slot streaming the text.
extern _native bool IS_STREAMING_ADDITIONAL_TEXT(int textIndex)l
extern _native bool IS_STREAMING_THIS_ADDITIONAL_TEXT(const char* str0, int slot)l
extern _native bool IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(const char* gxtentry)l
extern _native bool IS_THIS_HELP_MESSAGE_WITH_NUMBER_BEING_DISPLAYED(const char* gxtentry, int number)l
extern _native bool IS_THIS_HELP_MESSAGE_WITH_STRING_BEING_DISPLAYED(const char* gxtentry, const char* str)l

//INFO: The command uses default parameters so if you only need to check a message that takes no extra strings or numbers then you'll only have to specify the first parameter. 
//PURPOSE: Checks that a message is being displayed filtered by the print params.
extern _native bool IS_THIS_PRINT_BEING_DISPLAYED(const char* gxtentry, SCRIPT_PRINT_EXTRA_PARAMS extra, const char* sub_gxt, const char* str1, const char* str2, int number1, int number2, int number3, int number4, int number5, int number6)l

//PURPOSE:  Gets the name assigned to the blip
extern _native const char* GET_BLIP_NAME(BLIP_INDEX blip)l
extern _native const char* GET_FIRST_N_CHARACTERS_OF_LITERAL_STRING(const char* literalString, int n)l

//INFO: Check GET_LENGTH_OF_STRING_WITH_THIS_TEXT_LABEL first
//PURPOSE: Needed by the email script to simulate the player typing a message.
extern _native const char* GET_FIRST_N_CHARACTERS_OF_STRING(const char* gxtName, uint chars)l

//INFO: This is needed by Neil to display street names in the taxi script. The TextLabelHashKey will have been returned by FIND_STREET_NAME_AT_POSITION. 
//PURPOSE: returns the string contained in the .gxt file for the given text label hash key.
extern _native const char* GET_STRING_FROM_HASH_KEY(uint hash)l

//PURPOSE: Returns a string from part of another string 
extern _native const char* GET_STRING_FROM_STRING(const char* str, int startsymb, int endsymb)l
extern _native const char* GET_STRING_FROM_TEXT_FILE(const char* gxtentry)l

//PURPOSE: Returns the number of characters in this string. 
extern _native int GET_LENGTH_OF_LITERAL_STRING(const char* literalString)l

//PURPOSE: Returns the number of characters in the string with this text label
extern _native int GET_LENGTH_OF_STRING_WITH_THIS_TEXT_LABEL(const char* gxtName)l
extern _native int GET_MENU_ITEM_ACCEPTED(int menuid)l
extern _native int GET_MENU_ITEM_SELECTED(int menuid)l

//INFO: This character should be an integer between 0 and 9. This number will be returned. If anything goes wrong then the command will return -1. 
//PURPOSE: Finds the string for the given text label. It then finds character number CharacterIndex (counting from 0).
extern _native int GET_NTH_INTEGER_IN_STRING(const char* gxtName, int index)l
extern _native int GET_NUMBER_LINES(float x, float y, const char* gxt)l
extern _native int GET_NUMBER_LINES_WITH_LITERAL_STRINGS(float x, float y, const char* gxt, const char* str1, const char* str2)l

//INFO: Pass in an empty string "" for sub_str2 if you only want to insert one substring. 
//PURPOSE: Gets the number of lines to display the text with substrings.
extern _native int GET_NUMBER_LINES_WITH_SUBSTRINGS(float x, float y, const char* gxt, const char* str2, const char* str3)l
extern _native int GET_ROUTE_SIZE(void)l
extern _native OBJECT_INDEX GET_BLIP_INFO_ID_OBJECT_INDEX(BLIP_INDEX blip)l
extern _native PED_INDEX GET_BLIP_INFO_ID_PED_INDEX(BLIP_INDEX blip)l
extern _native PICKUP_INDEX GET_BLIP_INFO_ID_PICKUP_INDEX(BLIP_INDEX blip)l
extern _native uint GET_BLIP_INFO_ID_DISPLAY(BLIP_INDEX blip)l
extern _native uint GET_BLIP_INFO_ID_ROTATION(BLIP_INDEX blip)l
extern _native uint GET_BLIP_INFO_ID_TYPE(BLIP_INDEX blip)l
extern _native uint GET_BLIP_SPRITE(BLIP_INDEX blip)l
extern _native VEHICLE_INDEX GET_BLIP_INFO_ID_CAR_INDEX(BLIP_INDEX blip)l

//PURPOSE: opens frontend menu
extern _native void ACTIVATE_FRONTEND(void)l
extern _native void ACTIVATE_MENU_ITEM(int menuid, int item, bool activate)l

//PURPOSE:  Add Radar blip for a vehicle
extern _native void ADD_BLIP_FOR_CAR(VEHICLE_INDEX vehicle, BLIP_INDEX* pBlip)l

//PURPOSE:  Add Radar blip for a ped
extern _native void ADD_BLIP_FOR_CHAR(PED_INDEX ped, BLIP_INDEX* pBlip)l

//PURPOSE:  Add Radar blip at contact point
extern _native void ADD_BLIP_FOR_CONTACT(float x, float y, float z, BLIP_INDEX* pBlip)l

//PURPOSE:  Add Radar blip at coordinate
extern _native void ADD_BLIP_FOR_COORD(float x, float y, float z, BLIP_INDEX* pBlip)l

//PURPOSE:  Add Radar blip for gang territory
extern _native void ADD_BLIP_FOR_GANG_TERRITORY(float x, float y, float z, float unk, int colour, BLIP_INDEX* blip)l

//PURPOSE:  Add Radar blip for an object
extern _native void ADD_BLIP_FOR_OBJECT(OBJECT_INDEX obj, BLIP_INDEX* pBlip)l

//PURPOSE:  Add Radar blip for a pickup
extern _native void ADD_BLIP_FOR_PICKUP(PICKUP_INDEX pickup, BLIP_INDEX* pBlip)l

//PURPOSE:  Add Radar blip for a radius
extern _native void ADD_BLIP_FOR_RADIUS(float x, float y, float z, int radius, BLIP_INDEX* blip)l
extern _native void ADD_BLIP_FOR_WEAPON(float x, float y, float z, BLIP_INDEX* blip)l

//PURPOSE:  Will add/remove the next message from the previous briefs
extern _native void ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(bool add)l

//INFO: The track is specified one at a time. This has to be called after START_GPS_RACE_TRACK and before RENDER_RACE_TRACK.
//  If the track is circular the first point needs to be added twice (as the first and last) 
//PURPOSE: Adds a point to a gps race track at the given vector.
extern _native void ADD_POINT_TO_GPS_RACE_TRACK(vector3* pos)l

//PURPOSE: Add radar blip for a pickup object in world
extern _native void ADD_SIMPLE_BLIP_FOR_PICKUP(PICKUP_INDEX pickup)l
extern _native void ADD_TO_PREVIOUS_BRIEF(const char* string)l
extern _native void ADD_TO_PREVIOUS_BRIEF_WITH_UNDERSCORE(const char* gxtentry)l

//PURPOSE:  Change Radar blip alpha
extern _native void CHANGE_BLIP_ALPHA(BLIP_INDEX blip, int alpha)l

//PURPOSE:  Change Radar blip colour
extern _native void CHANGE_BLIP_COLOUR(BLIP_INDEX blip, int colour)l

//PURPOSE:  Change display mode for Radar blip
extern _native void CHANGE_BLIP_DISPLAY(BLIP_INDEX blip, int display)l

//PURPOSE:  Change priority for Radar blip
extern _native void CHANGE_BLIP_PRIORITY(BLIP_INDEX blip, int priority)l

//PURPOSE:  Change Radar blip name with straight ascii text
extern _native void CHANGE_BLIP_NAME_FROM_ASCII(BLIP_INDEX blip, const char* blipName)l

//PURPOSE:  Change Radar blip name with text from text file
extern _native void CHANGE_BLIP_NAME_FROM_TEXT_FILE(BLIP_INDEX blip, const char* gxtName)l

//PURPOSE:  Change Radar blip rotation
extern _native void CHANGE_BLIP_ROTATION(BLIP_INDEX blip, int degrees)l

//PURPOSE:  Change Radar blip scale
extern _native void CHANGE_BLIP_SCALE(BLIP_INDEX blip, float scale)l

//PURPOSE:  Set the blip sprite to be rendered to the radar.
extern _native void CHANGE_BLIP_SPRITE(BLIP_INDEX blip, uint sprite)l
extern _native void CHANGE_BLIP_TEAM_RELEVANCE(BLIP_INDEX blip, int relevance)l

//PURPOSE: Sets the colur of the pickup blips.
extern _native void CHANGE_PICKUP_BLIP_COLOUR(int colour)l

//PURPOSE: Sets how the pickup blips get rendered to teh radar. 
extern _native void CHANGE_PICKUP_BLIP_DISPLAY(int display)l

//INFO: Sets the order of the blip rendering, the blip with a higher priority will get rendered over other blips 
//PURPOSE: Sets the pickup blip priority
extern _native void CHANGE_PICKUP_BLIP_PRIORITY(int priority)l

//PARAM NOTES: Default is 1.0
//PURPOSE: Sets the pickups blip scale
extern _native void CHANGE_PICKUP_BLIP_SCALE(float scale)l

//PURPOSE: Set the blip sprite to be rendered to the radar.
extern _native void CHANGE_PICKUP_BLIP_SPRITE(uint sprite)l

//PURPOSE:  Change Radar blip scale for territory blips
extern _native void CHANGE_TERRITORY_BLIP_SCALE(BLIP_INDEX blip, float scale_x, float scale_y)l

//INFO: You will probably always want to call this command with TRUE for clear_previous_briefs. The reason for adding this command was for the situation where the player fails a mission and then replays it.
//PURPOSE: Call this command to remove the text from the previous attempt at the mission from the previous briefs screen.
extern _native void CLEAR_ADDITIONAL_TEXT(int slot, bool clear_previous_briefs)l

//PURPOSE: Clears any messages that are currently being displayed in the Pause Menu brief screen. 
extern _native void CLEAR_BRIEF(void)l

//PURPOSE: Call to stop displaying the current help message
extern _native void CLEAR_HELP(void)l
extern _native void CLEAR_ONSCREEN_COUNTER(int counterid)l
extern _native void CLEAR_ONSCREEN_TIMER(int timerid)l

//INFO: Handy if a lot of text is about to be printed by you whilst there still is some unimportant stuff left. 
//PURPOSE: Clears the print queue that may have been built up so far.
extern _native void CLEAR_PRINTS(void)l

//PURPOSE: This is the same as CLEAR_PRINTS except that it doesn?t clear big messages. 
extern _native void CLEAR_SMALL_PRINTS(void)l
extern _native void CLEAR_THIS_BIG_PRINT(const char* gxtentry)l

//PURPOSE: Clears any instances of this message that are currently being displayed or waiting to be displayed. 
extern _native void CLEAR_THIS_PRINT(const char* gxtentry)l
extern _native void CLEAR_THIS_PRINT_BIG_NOW(uint Unk635)l
extern _native void CREATE_MENU(const char* gxtentry, float x, float y, float Unk861, int Unk862, bool Unk863, bool Unk864, int Unk865, int* menuid)l
extern _native void DEACTIVATE_FRONTEND(void)l
extern _native void DELETE_MENU(int menuid)l
extern _native void DIM_BLIP(BLIP_INDEX blip, int dim)l
extern _native void DISABLE_PAUSE_MENU(bool disabled)l
extern _native void DISPLAY_ALTIMETER_THIS_FRAME(void)l
extern _native void DISPLAY_AMMO(bool display)l

//PURPOSE: Displays the area name.
extern _native void DISPLAY_AREA_NAME(bool display)l

//PURPOSE: Dipslays the playes cash.
extern _native void DISPLAY_CASH(bool display)l

//PURPOSE: Displays blips in the frontend map.
extern _native void DISPLAY_FRONTEND_MAP_BLIPS(bool display)l
extern _native void DISPLAY_GRIME_THIS_FRAME(void)l

//INFO: Must be called every frame
//PURPOSE:  Displays the help text this frame
extern _native void DISPLAY_HELP_TEXT_THIS_FRAME(const char* gxtkey, bool curved_window)l

//PURPOSE: Displays the hud.
extern _native void DISPLAY_HUD(bool display)l
extern _native void DISPLAY_LOADING_THIS_FRAME_WITH_SCRIPT_SPRITES(void)l

//INFO: Once you call SET_MINIGAME_IN_PROGRESS TRUE, only help messages from the minigame will be displayed. 
//PURPOSE: Allows help messages from other scripts to be displayed.
extern _native void DISPLAY_NON_MINIGAME_HELP_MESSAGES(bool display)l
extern _native void DISPLAY_NTH_ONSCREEN_COUNTER_WITH_STRING(uint counter, int Unk639, int Unk640, const char* str)l
extern _native void DISPLAY_ONSCREEN_TIMER_WITH_STRING(uint timerid, bool Unk641, const char* str)l

//PURPOSE: Displays the radar.
extern _native void DISPLAY_RADAR(bool display)l

//INFO: Must be called every frame
//PURPOSE:  Displays the sniper scope this frame
extern _native void DISPLAY_SNIPER_SCOPE_THIS_FRAME(void)l

//INFO: DISPLAY_TEXT resets all text settings after use, so make sure you call SET_TEXT_SCALE, SET_TEXT_BACKGROUND etc. each time you call DISPLAY_TEXT. 
//PARAM NOTES: Display x & y are between 0, 0 (top left) and 1, 1 (bottom right)
//PURPOSE:  Displays text to main render target.
extern _native void DISPLAY_TEXT(float x, float y, const char* gxtName)l

//PARAM NOTES: number is a FLOAT. Place the number in the string by using the special ~1~ code.
//  You no longer need to put ~1~.~1~ in the string. A solitary ~1~ will display the whole number and the digits after the decimal place.
//PURPOSE: Displays on screen text with a float number.
extern _native void DISPLAY_TEXT_WITH_FLOAT(float x, float y, const char* gxtName, float value, int decimal_places)l
extern _native void DISPLAY_TEXT_WITH_LITERAL_STRING(float x, float y, const char* gxtName, const char* literalStr)l

//PARAM NOTES: Display x & y are between 0, 0 (top left) and 1, 1 (bottom right)
//  number is an INT. Place the number in the string by using the special ~1~ code. 
//PURPOSE: Displays text with a number onscreen.
extern _native void DISPLAY_TEXT_WITH_NUMBER(float x, float y, const char* gxtName, int value)l
extern _native void DISPLAY_TEXT_WITH_STRING(float x, float y, const char* gxtName, const char* gxtStringName)l
extern _native void DISPLAY_TEXT_WITH_TWO_LITERAL_STRINGS(float x, float y, const char* gxtName, const char* literalStr1, const char* literalStr2)l
extern _native void DISPLAY_TEXT_WITH_TWO_STRINGS(float x, float y, const char* gxtName, const char* gxtStringName1, const char* gxtStringName2)l;

//PARAM NOTES: Display x & y are between 0, 0 (top left) and 1, 1 (bottom right)
//  number is an INT. Place the number in the string by using the special ~1~ code. 
//PURPOSE: Displays text with 2 numbers onscreen.
extern _native void DISPLAY_TEXT_WITH_2_NUMBERS(float x, float y, const char* gxtName, int number1, int number2)l

//PARAM NOTES: Display x & y are between 0, 0 (top left) and 1, 1 (bottom right)
//  number is an INT. Place the number in the string by using the special ~1~ code. 
//PURPOSE: Displays text with 3 numbers onscreen.
extern _native void DISPLAY_TEXT_WITH_3_NUMBERS(float x, float y, const char* gxtentry, int number1, int number2, int number3)l

//INFO: Must be called every frame
//PURPOSE: Hides the loading text on this frame
extern _native void DONT_DISPLAY_LOADING_ON_FADE_THIS_FRAME(void)l
extern _native void DRAW_CURVED_WINDOW_NOTEXT(float x, float y, float w, float h, int Unk727)l
extern _native void DRAW_CURVED_WINDOW_TEXT(float x, float y, float Unk730, int Unk731, int Unk732, const char* str0, const char* str1, int Unk733)l
extern _native void DRAW_WINDOW_TEXT(float x, float y, float Unk770, any unused, const char* str0, int Unk772)l

//PURPOSE:  Flash a radar blip
extern _native void FLASH_BLIP(BLIP_INDEX blip, bool on)l

//PURPOSE:  Flash a radar blip (alternative time)
extern _native void FLASH_BLIP_ALT(BLIP_INDEX blip, bool on)l
extern _native void FLASH_RADAR(bool flash)l
extern _native void FLASH_ROUTE(bool flash)l
extern _native void FREEZE_ONSCREEN_TIMER(bool freeze)l
extern _native void GET_CORRECTED_COLOUR(int r, int g, int b, int* pR, int* pG, int* pB)l

//PURPOSE:  Returns the alpha of the blip (the value set via CHANGE_BLIP_ALPHA)
extern _native void GET_BLIP_ALPHA(BLIP_INDEX blip, int* alpha)l

//PURPOSE:  Returns the colour of the blip
extern _native void GET_BLIP_COLOUR(BLIP_INDEX blip, int* pColour)l

//PURPOSE:  Returns the location of this blip. Can only be called for blips added by ADD_BLIP_FOR_COORD
extern _native void GET_BLIP_COORDS(BLIP_INDEX blip, vector3* pVector)l
extern _native void GET_BLIP_INFO_ID_POSITION(BLIP_INDEX bip, vector3* pos)l

//PURPOSE: Gets the frontend design placement values for use in the multiplayer frontend 
extern _native void GET_FRONTEND_DESIGN_VALUE(uint widget, float* x, float* y)l
extern _native void GET_HELP_MESSAGE_BOX_SIZE(float* x, float* y)l

//PURPOSE: Returns the red, green, blue and alpha components (all integers) of the colourID passed in. 
extern _native void GET_HUD_COLOUR(uint hud_color, int* pR, int* pG, int* pB, int* pA)l
extern _native void GET_MENU_POSITION(int menuid, float* x, float* y)l

//INFO: Must be called every frame
//PURPOSE: Hides help text this frame.
extern _native void HIDE_HELP_TEXT_THIS_FRAME(void)l

//INFO: Must be called every frame
//PURPOSE: Hides the radar AND the hud this frame ONLY 
extern _native void HIDE_HUD_AND_RADAR_THIS_FRAME(void)l
extern _native void HIGHLIGHT_MENU_ITEM(int menuid, int item, bool highlight)l
extern _native void LOAD_ADDITIONAL_TEXT(const char* textName, int textIndex)l

//INFO: Call this command before you need to use the font in your script and use UNLOAD_TEXT_FONT once you have finished displaying text. Only one of these special fonts can be in memory at one time.
//  Warning LOAD_TEXT_FONT may pause the game slightly, but you can use multiple fonts in one frame: 
//PURPOSE: Loads a font that requires to be streamed
extern _native void LOAD_TEXT_FONT(uint font)l
extern _native void LOCK_PLAYER_SETTINGS_GENRE_CHANGE(int lock_bit_mask)l
extern _native void LOOP_RACE_TRACK(bool loop)l

//INFO: This is where most text should be displayed. The text will be placed at the bottom of the queue. It could take a wee while before your message pops up.
//  Text is a STRING
//  Duration is an INT in milliseconds (1000 = 1 sec).
//PURPOSE: Prints a line of text at the bottom of the screen.
extern _native void PRINT(const char* gxtName, int timeMS, int color)l
extern _native void PRINT_BIG(const char* gxtName, int timeMS, int unk)l
extern _native void PRINT_BIG_Q(const char* gxtentry, int time, int unk)l
extern _native void PRINT_HELP(const char* gxtName)l
extern _native void PRINT_HELP_FOREVER(const char* gxtName)l
extern _native void PRINT_HELP_FOREVER_WITH_NUMBER(const char* gxtName, int value)l
extern _native void PRINT_HELP_FOREVER_WITH_STRING(const char* gxtName, const char* gxtText)l
extern _native void PRINT_HELP_FOREVER_WITH_STRING_NO_SOUND(const char* gxtName, const char* gxtText)l
extern _native void PRINT_HELP_FOREVER_WITH_TWO_NUMBERS(const char* gxtentry, int number1, int number2)l
extern _native void PRINT_HELP_OVER_FRONTEND(const char* gxtentry)l

//INFO: number can be an integer or a variable that has been declared as an INT. To mark where you want the number to appear use the special control code of ~1~ in the message in the text file. 
//PURPOSE: Displays the message with a number in a box on the left of the screen for the default help time.
extern _native void PRINT_HELP_WITH_NUMBER(const char* gxtName, int value)l
extern _native void PRINT_HELP_WITH_STRING(const char* gxtName, const char* gxtText)l
extern _native void PRINT_HELP_WITH_STRING_NO_SOUND(const char* gxtName, const char* gxtText)l
extern _native void PRINT_HELP_WITH_TWO_NUMBERS(const char* gxtentry, int number1, int number2)l

//  Text is a STRING
//  Duration is an INT in milliseconds (1000 = 1 sec).
//PURPOSE: Overwrites any current message with the message.
extern _native void PRINT_NOW(const char* gxtName, int timeMS, int color)l

//INFO: To mark where you want the small string to appear use the special control code of ~a~ in the big message in the text file. 
//PARAM NOTES: Duration is an INT in millisecs
//PURPOSE: Prints a string inside another string string.
extern _native void PRINT_STRING_IN_STRING(const char* gxtName, const char* gxtText, int timeMS, int color)l

//PARAM NOTES: Duration is an INT in millisecs
//PURPOSE: Overwrites any current message with a string inside another string string.
extern _native void PRINT_STRING_IN_STRING_NOW(const char* gxtName, const char* gxtText, int timeMS, int color)l
extern _native void PRINT_STRING_WITH_LITERAL_STRING(const char* gxtentry, const char* string, int time, int color)l
extern _native void PRINT_STRING_WITH_LITERAL_STRING_NOW(const char* gxtName, const char* text, int timeMS, int color)l
extern _native void PRINT_STRING_WITH_TWO_LITERAL_STRINGS(const char* gxtentry, const char* string1, const char* string2, int time, int color)l
extern _native void PRINT_STRING_WITH_TWO_LITERAL_STRINGS_NOW(const char* gxtentry, const char* string1, const char* string2, int time, int color)l

//INFO:  The text will be placed at the bottom of the queue. It could take a wee while before your message pops up.
//PARAM NOTES:Text is a STRING
//  Number is an INT. To mark where you want the number to appear use the special control code of ~1~ in the message in the text file.
//  Duration is an INT in millisecs
//PURPOSE: Prints a line of text with a number at the bottom of the screen.
extern _native void PRINT_WITH_NUMBER(const char* gxtName, int value, int timeMS, int color)l
extern _native void PRINT_WITH_NUMBER_BIG(const char* gxtName, int value, int timeMS, int color)l

//PARAM NOTES:Text is a STRING
//  Number is an INT. To mark where you want the number to appear use the special control code of ~1~ in the message in the text file.
//  Duration is an INT in millisecs
//PURPOSE: Overwrites any current message with the message and a number.
extern _native void PRINT_WITH_NUMBER_NOW(const char* gxtName, int value, int timeMS, int color)l

//INFO:  The text will be placed at the bottom of the queue. It could take a wee while before your message pops up.
//PARAM NOTES:Text is a STRING
//  Number is an INT. To mark where you want the number to appear use the special control code of ~1~ in the message in the text file.
//  Duration is an INT in millisecs
//PURPOSE: Prints a line of text with 2 numbers at the bottom of the screen.
extern _native void PRINT_WITH_2_NUMBERS(const char* gxtName, int value1, int value2, int timeMS, int color)l
extern _native void PRINT_WITH_2_NUMBERS_BIG(const char* gxtentry, int number1, int number2, int time, int color)l

//PARAM NOTES:Text is a STRING
//  Number is an INT. To mark where you want the number to appear use the special control code of ~1~ in the message in the text file.
//  Duration is an INT in millisecs
//PURPOSE: Overwrites any current message with the message and 2 numbers.
extern _native void PRINT_WITH_2_NUMBERS_NOW(const char* gxtName, int value1, int value2, int timeMS, int color)l
extern _native void PRINT_WITH_3_NUMBERS(const char* gxtentry, int number1, int number2, int number3, int time, int color)l
extern _native void PRINT_WITH_3_NUMBERS_NOW(const char* gxtentry, int number1, int number2, int number3, int time, int color)l
extern _native void PRINT_WITH_4_NUMBERS(const char* gxtentry, int number1, int number2, int number3, int number4, int time, int color)l
extern _native void PRINT_WITH_4_NUMBERS_NOW(const char* gxtentry, int number1, int number2, int number3, int number4, int time, int color)l
extern _native void PRINT_WITH_5_NUMBERS(const char* gxtentry, int number1, int number2, int number3, int number4, int number5, int time, int color)l
extern _native void PRINT_WITH_5_NUMBERS_NOW(const char* gxtentry, int number1, int number2, int number3, int number4, int number5, int time, int color)l
extern _native void PRINT_WITH_6_NUMBERS(const char* gxtentry, int number1, int number2, int number3, int number4, int number5, int number6, int time, int color)l
extern _native void PRINT_WITH_6_NUMBERS_NOW(const char* gxtentry, int number1, int number2, int number3, int number4, int number5, int number6, int time, int color)l

//PURPOSE:  Delete radar blip
extern _native void REMOVE_BLIP(BLIP_INDEX blip)l

//PURPOSE:  Delete radar blip and sets the variable to NULL
extern _native void REMOVE_BLIP_AND_CLEAR_INDEX(BLIP_INDEX* blip)l
extern _native void RENDER_LOADING_CLOCK(float unk0, float unk1, float unk2, float unk3, float unk4, float unk5, float unk6, float unk7, float unk8, float unk9, int unk10, int unk11, int unk12, int unk13, int unk14, int unk15, const char* unk16)l

//INFO: Once a track has been defined using START_GPS_RACE_TRACK and a sequence of ADD_POINT_TO_GPS_RACE_TRACKs it can be rendered using this command. 
//  When done with the race don't forget to call SET_RACE_TRACK_RENDER false (the mission cleanup also does this) or CLEAR_GPS_RACE_TRACK
//  When the race track is being displayed other trails that belong to blips are not displayed. The one that can be specified by the player in the front end will still work though. 
//PURPOSE:  Renders a race track once contsructed.
extern _native void RENDER_RACE_TRACK(bool render)l
extern _native void RENDER_RADIOHUD_SPRITE_IN_MOBILE_PHONE(float unk0, float unk1, float unk2, float unk3, int unk4, int unk5, float unk6, bool unk7)l

//INFO: One block of text can be loaded into each slot at a time, e.g. the phone script can load a block of text into the phone text slot without affecting the text loaded into the mission text slot for the current mission.
//  In the text file, it is possible to mark a piece of text so that it is only loaded when this command is called. The block is given a name of up to seven characters ? the convention is if the .sc filename
//  has seven or less characters then that name is used in the text file, if the .sc filename has more than seven characters then the first six characters and the last one are used.
//  
//  By putting this block name after a : as part of the text key, the text will not be loaded until REQUEST_ADDITIONAL_TEXT is called using the block name. 
//  If any block names are used in the text file then they should be added to an alphabetical list at the very beginning of the text file contained within the words "start" and "end". 
//  Only one chunk of block text can be loaded into each slot at a time. When REQUEST_ADDITIONAL_TEXT is called for another block, the block text currently in memory will be lost.
//  
//  It seems that REQUEST_ADDITIONAL_TEXT will also clear any prints on screen and clear the print queue, regardless of the text block that these prints are in. If you're wondering why your text is disappearing this might be the cause.
//  
//  For episodic content, REQUEST_ADDITIONAL_TEXT will not let you override text blocks. i.e. if you have named a text block RAY1 in an episodic gxt and there is already
//  a text block called RAY1 in GTA4's gxt then REQUEST_ADDITIONAL_TEXT will always load the one from GTA4. The solution is to rename the text block to something that doesn't clash. 
//PURPOSE: Requests a block of text. 
extern _native void REQUEST_ADDITIONAL_TEXT(const char* textName, int textIndex)l

// PURPOSE: Set blip as a friendly or threat
extern _native void SET_BLIP_AS_FRIENDLY(BLIP_INDEX blip, bool value)l

//PURPOSE:  set a blip as short range - on off
extern _native void SET_BLIP_AS_SHORT_RANGE(BLIP_INDEX blip, bool value)l

//PURPOSE:  Set the coords of a coord, contact or radius blip
extern _native void SET_BLIP_COORDINATES(BLIP_INDEX blip, float x, float y, float z)l

//PURPOSE:  Set a blip marker to use a longer distance or not
extern _native void SET_BLIP_MARKER_LONG_DISTANCE(BLIP_INDEX blip, bool set)l
extern _native void SET_CLEAR_HELP_IN_MISSION_CLEANUP(bool set)l
extern _native void SET_GPS_REMAINS_WHEN_TARGET_REACHED_FLAG(bool set)l
extern _native void SET_GPS_TEST_IN_3D_FLAG(bool set)l

//PURPOSE: Set the help text box size.
extern _native void SET_HELP_MESSAGE_BOX_SIZE(int size)l
extern _native void SET_HELP_MESSAGE_BOX_SIZE_F(float size)l
extern _native void SET_HIDE_WEAPON_ICON(bool set)l
extern _native void SET_MENU_COLUMN(int menuid, int column, const char* Unk867, const char* Unk868, const char* Unk869, const char* Unk870, const char* Unk871, const char* Unk872, const char* Unk873, const char* Unk874, const char* Unk875, const char* Unk876, const char* Unk877, const char* Unk878, const char* Unk879)l
extern _native void SET_MENU_COLUMN_ORIENTATION(int menuid, int column, int orientation)l
extern _native void SET_MENU_COLUMN_WIDTH(int menuid, int column, float width)l
extern _native void SET_MENU_ITEM_WITH_NUMBER(int menuid, int item, int Unk881, const char* gxtkey, int number)l
extern _native void SET_MENU_ITEM_WITH_2_NUMBERS(int menuid, int item, int Unk880, const char* gxtkey, int number0, int number1)l
extern _native void SET_MESSAGE_FORMATTING(bool Unk700, int Unk701, int Unk702)l
extern _native void SET_MISSION_PASSED_CASH(bool negative, int cash, uint Unk511)l
extern _native void SET_MULTIPLAYER_BRIEF(const char* unk0, const char* unk1)l
extern _native void SET_MULTIPLAYER_HUD_CASH(int cash)l
extern _native void SET_MULTIPLAYER_HUD_TIME(const char* str)l
extern _native void SET_ONSCREEN_COUNTER_FLASH_WHEN_FIRST_DISPLAYED(int counterid, bool flash)l
extern _native void SET_PLAYER_ICON_COLOUR(int color)l

//PURPOSE:  Set radar to appear as if inside an interior for 1 frame:
extern _native void SET_RADAR_AS_INTERIOR_THIS_FRAME(void)l
extern _native void SET_RADAR_SCALE(float scale)l

//PURPOSE: Modifys the zoom value of the radar between 0 (no zoom), and 1 (max zoom in) and 1100 (max zoom out).
extern _native void SET_RADAR_ZOOM(float zoom)l
extern _native void SET_ROUTE(int unk, bool value)l
extern _native void SET_SELECTED_MENU_ITEM(int menuid, int item)l

//PURPOSE: Draws a box around/behind the text 
extern _native void SET_TEXT_BACKGROUND(bool value)l

//INFO: It also moves the origin of the text to the centre point. 
//PURPOSE: Centre justifies the text.
extern _native void SET_TEXT_CENTRE(bool value)l
extern _native void SET_TEXT_CENTRE_WRAPX(float wrapx)l

//PARAM NOTES: Four parameters should be INTs between 0 and 255. Alpha 0 is invisible. 
//PURPOSE: Sets the colour of the text.
extern _native void SET_TEXT_COLOUR(int r, int g, int b, int a)l
extern _native void SET_TEXT_DRAW_BEFORE_FADE(bool value)l

//PURPOSE: Draw a drop shadow behind onscreen intro text. 
extern _native void SET_TEXT_DROPSHADOW(int drop_amount, int r, int g, int b, int a)l

//PURPOSE: Draws an outline round the entire text. 
extern _native void SET_TEXT_EDGE(int edge_amount, int r, int g, int b, int a)l

//PURPOSE: Set the text font 
extern _native void SET_TEXT_FONT(uint font)l

//INFO: (Text by default is left justified) 
//PURPOSE: Fully justifies the text to the coordinates specified by SET_TEXT_WRAP.
extern _native void SET_TEXT_JUSTIFY(bool value)l

//INFO: If you just wanted to display the second line, you would call: SET_TEXT_LINE_DISPLAY(2, 2)
//  This can be set before GET_NUMBER_LINES as GET_NUMBER_LINES ignores it. It will only get used when you call DISPLAY_TEXT to display the text. 
//PARAM NOTES: StartLine is the first line you want to start with and EndLine is the last line. 
//PURPOSE: Sets which line the text will display on.
extern _native void SET_TEXT_LINE_DISPLAY(int start, int end)l
extern _native void SET_TEXT_LINE_HEIGHT_MULT(float lineHeight)l

//PURPOSE: Sets the spacing of the text proporional.
extern _native void SET_TEXT_PROPORTIONAL(bool value)l

//PURPOSE: Set the render target to be used for rendering.
extern _native void SET_TEXT_RENDER_ID(uint renderId)l

//INFO: It also moves the origin of the text to the right hand side of the text. 
//PURPOSE: Right justifies the text. 
extern _native void SET_TEXT_RIGHT_JUSTIFY(bool value)l

//PURPOSE: Sets the text scale by using a multiplier. 1.0 = normal / 2.0 = double 
extern _native void SET_TEXT_SCALE(float w, float h)l

//INFO: Allows text that has been coloured to be changed back to original.
//PURPOSE: Uses colurs as set by the text file.
extern _native void SET_TEXT_TO_USE_TEXT_FILE_COLOURS(bool value)l
extern _native void SET_TEXT_USE_UNDERSCORE(bool value)l
extern _native void SET_TEXT_VIEWPORT_ID(int id)l

//PURPOSE: Sets points where text will wrap round and displayed on a new line. 
extern _native void SET_TEXT_WRAP(float x, float y)l
extern _native void SET_TEXT_WRAPX(float wrap)l
extern _native void SET_TIMER_BEEP_COUNTDOWN_TIME(int timerid, int beeptime)l

//PURPOSE:  Sets the script widescreen format
extern _native void SET_WIDESCREEN_FORMAT(int wideformatid)l
extern _native void SHOW_BLIP_ON_ALTIMETER(int unk, bool show)l

//INFO: This command will clear the previous track OR custom-route that was set up. 
//PARAM NOTES: The colour specifies the colour in which the track will evenually be displayed.
//PURPOSE: Starts constructing a race track (circuit) to be displayed on the radar and in the front-end. 
extern _native void START_GPS_RACE_TRACK(int color)l

// PURPOSE: Switches off the waypoint blip if it is active
extern _native void SWITCH_OFF_WAYPOINT(void)l
extern _native void TURN_OFF_RADIOHUD_IN_LOBBY(void)l
extern _native void USE_PREVIOUS_FONT_SETTINGS(void)l
extern _native void UNLOAD_TEXT_FONT(void)l

//Deprecated
//SET_AREA_NAME
//RENDER_RADIOHUD_SPRITE_IN_LOBBY
//TURN_ON_RADIOHUD_IN_LOBBY
#pragma endregion

#pragma region DEBUG
//PURPOSE: Returns the number of frames
extern _native int GET_FRAME_COUNT(void)l

//PURPOSE: Fault script with said message, function name, file name, and file line
extern _native void SCRIPT_ASSERT(const char* message, const char* func_name, const char* file_name, int file_line)l

//Deprecated
//DEBUG_ON
//DEBUG_OFF
//SET_DEBUG_TEXT_VISIBLE
//OPEN_DEBUG_FILE
//CLOSE_DEBUG_FILE
//SAVE_INT_TO_DEBUG_FILE
//SAVE_FLOAT_TO_DEBUG_FILE
//SAVE_NEWLINE_TO_DEBUG_FILE
//SAVE_STRING_TO_DEBUG_FILE
//GET_MODEL_NAME_OF_CAR_FOR_DEBUG_ONLY
//GET_MODEL_NAME_FOR_DEBUG
//GET_ROOM_NAME_FROM_CHAR_DEBUG
//INIT_DEBUG_WIDGETS
//CREATE_WIDGET_GROUP
//END_WIDGET_GROUP
//ADD_WIDGET_SLIDER
//ADD_WIDGET_FLOAT_SLIDER
//ADD_WIDGET_READ_ONLY
//ADD_WIDGET_FLOAT_READ_ONLY
//ADD_WIDGET_TOGGLE
//ADD_WIDGET_STRING
//DELETE_WIDGET_GROUP
//DELETE_WIDGET
//DOES_WIDGET_GROUP_EXIST
//GET_CONSOLE_COMMAND
//RESET_CONSOLE_COMMAND
//START_NEW_WIDGET_COMBO
//ADD_TO_WIDGET_COMBO
//FINISH_WIDGET_COMBO
//ADD_TEXT_WIDGET
//GET_CONTENTS_OF_TEXT_WIDGET
//SET_CONTENTS_OF_TEXT_WIDGET
//ACTIVATE_VECTOR_MAP
//SET_PLAYER_INVISIBLE_TO_AI
//SET_ALL_PEDS_SPAWNED_ARE_COPS
//GET_NETWORK_RESTART_NODE_DEBUG
//TAKE_SCREEN_SHOT
//DISABLE_DEBUG_CAM_AND_PLAYER_WARPING
//PRINT_MISSION_DESCRIPTION
//GET_NAME_OF_SCRIPT_TO_AUTOMATICALLY_START
#pragma endregion

#pragma region CAMERA
//INFO: Set via SET_WIDESCREEN_BORDERS
//PURPOSE: Are the widescreen borders still visible on the screen.
extern _native bool ARE_WIDESCREEN_BORDERS_ACTIVE(void)l
extern _native bool CAM_IS_SPHERE_VISIBLE(CAMERA_INDEX camera, float pX, float pY, float pZ, float radius)l

//PURPOSE: Does this camera still exist.
extern _native bool DOES_CAM_EXIST(CAMERA_INDEX camera)l
extern _native bool DOES_VIEWPORT_EXIST(int viewportid)l
extern _native bool GET_VIEWPORT_POSITION_OF_COORD(float x, float y, float z, int viewport_id, float* vx, float* vy)l

//INFO: Note this may return true but does not mean its rendering to the game screen
//PURPOSE: Checks that a camera is active.
extern _native bool IS_CAM_ACTIVE(CAMERA_INDEX camera)l
extern _native bool IS_CAM_HAPPY(CAMERA_INDEX cam)l
extern _native bool IS_CAM_INTERPOLATING(void)l
extern _native bool IS_CAM_PROPAGATING(CAMERA_INDEX camera)l
extern _native bool IS_CAM_SEQUENCE_COMPLETE(uint Unk535)l
extern _native bool IS_DEBUG_CAMERA_ON(void)l
extern _native bool IS_FOLLOW_VEHICLE_CAM_OFFSET_ACTIVE(void)l
extern _native bool IS_HINT_RUNNING(void)l

//PURPOSE: Has the screen faded in.
extern _native bool IS_SCREEN_FADED_IN(void)l

//PURPOSE: Has the screen faded out
extern _native bool IS_SCREEN_FADED_OUT(void)l
extern _native bool IS_SCREEN_FADING(void)l

//PURPOSE: Is the screen fading IN ( getting clearer )
extern _native bool IS_SCREEN_FADING_IN(void)l

//PURPOSE: Is the screen fading OUT ( getting darker )
extern _native bool IS_SCREEN_FADING_OUT(void)l
extern _native bool IS_SPECIFIC_CAM_INTERPOLATING(CAMERA_INDEX cam)l
extern _native bool IS_VIEWPORT_ACTIVE(int viewportid)l
extern _native int COUNT_SCRIPT_CAMS(void)l
extern _native int COUNT_SCRIPT_CAMS_BY_TYPE_AND_OR_STATE(int type, uint Unk536, uint Unk537)l
extern _native int GET_CAM_STATE(CAMERA_INDEX cam)l
extern _native int GET_SCREEN_FADE_ALPHA(void)l
extern _native void ACTIVATE_SCRIPTED_CAMS(bool Unk538, bool Unk539)l
extern _native void ACTIVATE_VIEWPORT(int viewportid, bool activate)l
extern _native void ADD_CAM_SPLINE_NODE(CAMERA_INDEX cam, int camnode)l
extern _native void ADD_PED_TO_CINEMATOGRAPHY_AI(int Unk28, PED_INDEX ped)l

//PURPOSE: Attaches a camera to a object.
extern _native void ATTACH_CAM_TO_OBJECT(CAMERA_INDEX cam, OBJECT_INDEX obj)l

//PURPOSE: Attaches a camera to a ped.
extern _native void ATTACH_CAM_TO_PED(CAMERA_INDEX cam, PED_INDEX ped)l

//PURPOSE: Attaches a camera to a vehicle.
extern _native void ATTACH_CAM_TO_VEHICLE(CAMERA_INDEX cam, VEHICLE_INDEX veh)l
extern _native void ATTACH_CAM_TO_VIEWPORT(CAMERA_INDEX cam, int viewportid)l
extern _native void BEGIN_CAM_COMMANDS(uint* Unk540)l
extern _native void CAM_PROCESS(CAMERA_INDEX cam)l
extern _native void CAM_RESTORE(void)l
extern _native void CAM_RESTORE_JUMPCUT(void)l
extern _native void CAM_SEQUENCE_CLOSE(void)l
extern _native void CAM_SEQUENCE_GET_PROGRESS(uint Unk541, int* progress)l
extern _native void CAM_SEQUENCE_OPEN(uint* Unk542)l
extern _native void CAM_SEQUENCE_REMOVE(uint Unk543)l
extern _native void CAM_SEQUENCE_START(uint Unk544)l
extern _native void CAM_SEQUENCE_STOP(uint Unk545)l
extern _native void CAM_SEQUENCE_WAIT(uint unk, int time)l
extern _native void CAM_SET_CINEMATIC(uint unk, bool set)l
extern _native void CAM_SET_DOLLY_ZOOM_LOCK(CAMERA_INDEX cam, bool set)l
extern _native void CAM_SET_INTERP_GRAPH_POS(int cam, int Unk547)l
extern _native void CAM_SET_INTERP_GRAPH_ROT(int cam, int val)l
extern _native void CAM_SET_INTERP_STATE_DST(int unk0, int unk1)l
extern _native void CAM_SET_INTERP_STATE_SRC(int cam, int Unk548)l
extern _native void CAM_SET_INTERPOLATION_DETAILS(uint Unk546)l
extern _native void CLEAR_ROOM_FOR_VIEWPORT(int viewportid)l
extern _native void CLONE_CAM(CAMERA_INDEX cam, CAMERA_INDEX* camcopy)l
extern _native void CREATE_CAM(uint camtype_usually14, CAMERA_INDEX* camera)l
extern _native void CREATE_VIEWPORT(int* viewport)l

//PURPOSE: Destroy ALL script-controlled cameras.
extern _native void DESTROY_ALL_CAMS(void)l
extern _native void DESTROY_ALL_SCRIPT_VIEWPORTS(void)l

//PURPOSE: Destroy a single script-controlled camera.
extern _native void DESTROY_CAM(CAMERA_INDEX camera)l
extern _native void DESTROY_VIEWPORT(int viewportid)l
extern _native void DETACH_CAM_FROM_VIEWPORT(int Unk557)l
extern _native void DISABLE_HELI_CHASE_CAM_BONNET_NITRO_FIX(void)l
extern _native void DISABLE_HELI_CHASE_CAM_THIS_UPDATE(void)l
extern _native void DISABLE_INTERMEZZO_CAMS(void)l

//PURPOSE: Fade in the main viewport with fade FROM black.
extern _native void DO_SCREEN_FADE_IN(int timeMS)l
extern _native void DO_SCREEN_FADE_IN_UNHACKED(int timeMS)l

//PURPOSE: Fade OUT the main viewport with fade TO black.
extern _native void DO_SCREEN_FADE_OUT(int timeMS)l
extern _native void DO_SCREEN_FADE_OUT_UNHACKED(int timeMS)l
extern _native void ENABLE_CAM_COLLISION(int cam, bool enable)l
extern _native void ENABLE_DEBUG_CAM(bool enable)l
extern _native void END_CAM_COMMANDS(int* Unk558)l
extern _native void FORCE_GAME_TELESCOPE_CAM(bool force)l
extern _native void FORCE_NO_CAM_PAUSE(bool foce)l

//PURPOSE: Get the camera's far clip distance (in m.)
extern _native void GET_CAM_FAR_CLIP(CAMERA_INDEX cam, float* clip)l

//PURPOSE: Get the camera's far depth of field distance (in m.)
extern _native void GET_CAM_FAR_DOF(CAMERA_INDEX cam, float* fardof)l

//PURPOSE: Get the camera's field of view (in degrees.)
extern _native void GET_CAM_FOV(CAMERA_INDEX camera, float* fov)l

//PURPOSE: Get the camera's motion blur strength (0.0 to 1.0)
extern _native void GET_CAM_MOTION_BLUR(CAMERA_INDEX cam, float* blur)l

//PURPOSE: Get the camera's near clip distance (in m.)
extern _native void GET_CAM_NEAR_CLIP(CAMERA_INDEX cam, float* clip)l

//PURPOSE: Get the camera's near depth of field distance (in m.)
extern _native void GET_CAM_NEAR_DOF(CAMERA_INDEX cam, float* dof)l

//PURPOSE: Get the camera's position
extern _native void GET_CAM_POS(CAMERA_INDEX camera, float* pX, float* pY, float* pZ)l
extern _native void GET_CINEMATIC_CAM(CAMERA_INDEX* cam)l

//PURPOSE: Will give you a reference to the debug free camera.
extern _native void GET_DEBUG_CAM(CAMERA_INDEX* cam)l
extern _native void GET_FOLLOW_VEHICLE_CAM_SUBMODE(int* mode)l
extern _native void GET_FREE_CAM(CAMERA_INDEX* cam)l
extern _native void GET_GAME_CAM(CAMERA_INDEX* camera)l
extern _native void GET_GAME_CAM_CHILD(CAMERA_INDEX* camera)l
extern _native void GET_GAME_VIEWPORT_ID(int* viewportid)l
extern _native void GET_KEY_FOR_VIEWPORT_IN_ROOM(int viewportid, uint* roomkey)l
extern _native void GET_RADAR_VIEWPORT_ID(int* viewport)l
extern _native void GET_ROOT_CAM(CAMERA_INDEX* rootcam)l
extern _native void GET_SCREEN_VIEWPORT_ID(int* viewportid)l
extern _native void GET_SCRIPT_CAM(CAMERA_INDEX* cam)l
extern _native void GET_SCRIPT_DRAW_CAM(CAMERA_INDEX* cam)l
extern _native void GET_VIEWPORT_POS_AND_SIZE(int viewportid, float* x, float* y, float* xsize, float* ysize)l
extern _native void SET_CAM_ROLL(CAMERA_INDEX cam, float roll)l

//PURPOSE: Get the camera's rotation in Euler angles (in degrees.)
extern _native void GET_CAM_ROT(CAMERA_INDEX camera, float* angleX, float* angleY, float* angleZ)l
extern _native void HINT_CAM(float x, float y, float z, int Unk559, int Unk560, int Unk561, int Unk562)l
//extern _native void m(int cam, float heading)l	//set cam follow heading , "m" is a corect name

///INFO: Both cameras must be script controlled
//PURPOSE: Points a camera at to look at another cam.
extern _native void POINT_CAM_AT_CAM(CAMERA_INDEX cam, CAMERA_INDEX camnext)l

//PURPOSE: Sets the look target position.
extern _native void POINT_CAM_AT_COORD(CAMERA_INDEX cam, float x, float y, float z)l

//PURPOSE: Points the camera at an object.
extern _native void POINT_CAM_AT_OBJECT(CAMERA_INDEX cam, OBJECT_INDEX obj)l

//PURPOSE: Points the camera at a ped.
extern _native void POINT_CAM_AT_PED(CAMERA_INDEX cam, PED_INDEX ped)l

//PURPOSE: Points the camera at a vehicle.
extern _native void POINT_CAM_AT_VEHICLE(CAMERA_INDEX cam, VEHICLE_INDEX veh)l
extern _native void POINT_FIXED_CAM(float x, float y, float z, int Unk563)l
extern _native void POINT_FIXED_CAM_AT_OBJ(OBJECT_INDEX obj, int cam)l
extern _native void POINT_FIXED_CAM_AT_PED(PED_INDEX ped, int cam)l
extern _native void POINT_FIXED_CAM_AT_POS(float x, float y, float z, int cam)l
extern _native void POINT_FIXED_CAM_AT_VEHICLE(VEHICLE_INDEX veh, int cam)l
extern _native void PROSTITUTE_CAM_ACTIVATE(bool activate)l
extern _native void RESET_CAM_INTERP_CUSTOM_SPEED_GRAPH(void)l
extern _native void RESET_CAM_SPLINE_CUSTOM_SPEED_GRAPH(void)l
extern _native void SET_BLOCK_CAMERA_TOGGLE(bool set)l

//PURPOSE: Sets a camera to be active and rendering, last cam set to active renders to the game.
extern _native void SET_CAM_ACTIVE(CAMERA_INDEX camera, bool value)l
extern _native void SET_CAM_ATTACH_OFFSET(CAMERA_INDEX cam, float x, float y, float z)l
extern _native void SET_CAM_ATTACH_OFFSET_IS_RELATIVE(CAMERA_INDEX cam, bool set)l
extern _native void SET_CAM_BEHIND_PED(PED_INDEX ped)l
extern _native void SET_CAM_COMPONENT_SHAKE(CAMERA_INDEX cam, int componentid, int Unk564, float time, float x, float y, float z)l
extern _native void SET_CAM_DOF_FOCUSPOINT(CAMERA_INDEX cam, float x, float y, float z, float Unk565)l

//PURPOSE: Set the camera's far clip distance (in m.)
extern _native void SET_CAM_FAR_CLIP(CAMERA_INDEX cam, float clip)l

//PURPOSE: Set the camera's far depth of field distance (in m.)
extern _native void SET_CAM_FAR_DOF(CAMERA_INDEX cam, float fardof)l

//PURPOSE: Set the camera's field of view (in degrees.)
extern _native void SET_CAM_FOV(CAMERA_INDEX camera, float fov)l
extern _native void SET_CAM_IN_FRONT_OF_PED(PED_INDEX ped)l

//PURPOSE: Set the camera roll to be relative to a target of type object.
extern _native void SET_CAM_INHERIT_ROLL_OBJECT(CAMERA_INDEX cam, OBJECT_INDEX obj)l

//PURPOSE: Set the camera roll to be relative to a target of type ped.
extern _native void SET_CAM_INHERIT_ROLL_PED(CAMERA_INDEX cam, PED_INDEX ped)l

//PURPOSE: Set the camera roll to be relative to a target of type vehicle.
extern _native void SET_CAM_INHERIT_ROLL_VEHICLE(CAMERA_INDEX cam, VEHICLE_INDEX veh)l
extern _native void SET_CAM_INTERP_CUSTOM_SPEED_GRAPH(float speed)l
extern _native void SET_CAM_INTERP_DETAIL_ROT_STYLE_ANGLES(int Unk566)l
extern _native void SET_CAM_INTERP_DETAIL_ROT_STYLE_QUATS(int Unk567)l
extern _native void SET_CAM_INTERP_STYLE_CORE(int cam0, int cam1, int cam2, int time, int flag)l
extern _native void SET_CAM_INTERP_STYLE_DETAILED(int cam, int Unk568, int Unk569, int Unk570, int Unk571)l

//PURPOSE: Set the camera's motion blur strength (0.0 to 1.0)
extern _native void SET_CAM_MOTION_BLUR(CAMERA_INDEX cam, float blur)l

//PURPOSE: Set the camera's near clip distance (in m.)
extern _native void SET_CAM_NEAR_CLIP(CAMERA_INDEX cam, float clip)l

//PURPOSE: Set the camera's near depth of field distance (in m.)
extern _native void SET_CAM_NEAR_DOF(CAMERA_INDEX cam, float dof)l
extern _native void SET_CAM_NAME(CAMERA_INDEX cam, const char* camname)l
extern _native void SET_CAM_POINT_DAMPING_PARAMS(CAMERA_INDEX cam, float x, float y, float z)l
extern _native void SET_CAM_POINT_OFFSET(CAMERA_INDEX cam, float x, float y, float z)l
extern _native void SET_CAM_POINT_OFFSET_IS_RELATIVE(CAMERA_INDEX cam, bool set)l

//PURPOSE: Set the camera's position.
extern _native void SET_CAM_POS(CAMERA_INDEX camera, float pX, float pY, float pZ)l
extern _native void SET_CAM_PROPAGATE(CAMERA_INDEX camera, bool value)l

//PURPOSE: Set the camera's rotation in Euler angles (in degrees.) 
extern _native void SET_CAM_ROT(CAMERA_INDEX camera, float angleX, float angleY, float angleZ)l
extern _native void SET_CAM_SHAKE(CAMERA_INDEX cam, int Unk572, int shakeval)l
extern _native void SET_CAM_SPLINE_CUSTOM_SPEED_GRAPH(float speed)l

//PURPOSE: Sets the total duration of the spline, use for rounded and smoothed splines. 
extern _native void SET_CAM_SPLINE_DURATION(CAMERA_INDEX cam, int duration)l
extern _native void SET_CAM_SPLINE_PROGRESS(CAMERA_INDEX cam, float progress)l
extern _native void SET_CAM_SPLINE_SPEED_CONSTANT(CAMERA_INDEX cam, bool set)l
extern _native void SET_CAM_SPLINE_SPEED_GRAPH(CAMERA_INDEX cam, int Unk573)l
extern _native void SET_CAM_STATE(CAMERA_INDEX cam, int state)l
extern _native void SET_CAM_TARGET_PED(int camera, int ped)l
extern _native void SET_CAMERA_AUTO_SCRIPT_ACTIVATION(bool set)l
extern _native void SET_CAMERA_BEGIN_CAM_COMMANDS_REQUIRED(bool set)l
extern _native void SET_CAMERA_CONTROLS_DISABLED_WITH_PLAYER_CONTROLS(bool value)l
extern _native void SET_CAMERA_STATE(CAMERA_INDEX cam, int state)l
extern _native void SET_CAR_FOV_FADE_MULT(float multiplier)l
extern _native void SET_CAR_FOV_MAX(float maxfov)l
extern _native void SET_CAR_FOV_MIN(float minfov)l
extern _native void SET_CAR_FOV_RATE(float rate)l
extern _native void SET_CAR_FOV_START_SPEED(float speed)l
extern _native void SET_CAR_FOV_START_SPEED_BOAT(float speed)l
extern _native void SET_CAR_MOTION_BLUR_EFFECT_BOAT(float blur)l
extern _native void SET_CINEMATIC_BUTTON_ENABLED(bool set)l
extern _native void SET_COLLIDE_WITH_PEDS(bool set)l
extern _native void SET_DANCE_SHAKE_ACTIVE_THIS_UPDATE(float shake)l
extern _native void SET_DANCE_SHAKE_INACTIVE_IMMEDIATELY(void)l
extern _native void SET_DRUNK_CAM(int cam, float val, int time)l
extern _native void SET_ENABLE_NEAR_CLIP_SCAN(bool set)l
extern _native void SET_FIXED_CAM_POS(float x, float y, float z)l
extern _native void SET_FOLLOW_PED_PITCH_LIMIT_DOWN(float pitchdownlim)l
extern _native void SET_FOLLOW_PED_PITCH_LIMIT_UP(float pitchuplim)l
extern _native void SET_FOLLOW_VEHICLE_CAM_OFFSET(bool Unk574, float x, float y, float z)l
extern _native void SET_FOLLOW_VEHICLE_CAM_SUBMODE(int mode)l
extern _native void SET_FOLLOW_VEHICLE_PITCH_LIMIT_DOWN(float pitchdownlim)l
extern _native void SET_FOLLOW_VEHICLE_PITCH_LIMIT_UP(float pitchuplim)l
extern _native void SET_FORCE_LOOK_BEHIND(bool set)l
extern _native void SET_FOV_CHANNEL_SCRIPT(bool set)l
extern _native void SET_GAME_CAM_HEADING(float heading)l
extern _native void SET_GAME_CAM_PITCH(float pitch)l
extern _native void SET_GAME_CAMERA_CONTROLS_ACTIVE(bool active)l
extern _native void SET_HINT_ADVANCED_PARAMS(float Unk575, float Unk576, float Unk577, float Unk578, bool Unk579)l
extern _native void SET_HINT_FOV(float fov)l
extern _native void SET_HINT_MOVE_IN_DIST(float dist)l
extern _native void SET_HINT_TIMES(int Unk580, int Unk581, int Unk582)l
extern _native void SET_HINT_TIMES_DEFAULT(void)l
extern _native void SET_HINT_MOVE_IN_DIST_DEFAULT(void)l
extern _native void SET_INSTANT_WIDESCREEN_BORDERS(bool set)l
extern _native void SET_INTERP_FROM_GAME_TO_SCRIPT(bool Unk604, int Unk605)l
extern _native void SET_INTERP_FROM_SCRIPT_TO_GAME(bool Unk606, int Unk607)l
extern _native void SET_INTERP_IN_OUT_VEHICLE_ENABLED_THIS_FRAME(bool set)l
extern _native void SET_ROOM_FOR_VIEWPORT_BY_KEY(int viewportid, uint roomkey)l
extern _native void SET_ROOM_FOR_VIEWPORT_BY_NAME(int viewportid, const char* roomname)l
extern _native void SET_ROT_ORDER(uint order)l
extern _native void SET_SCREEN_FADE(int viewportid, int Unk778, int Unk779, bool Unk780, int r, int g, int b, int a, int Unk781, float Unk782, float Unk783)l
extern _native void SET_SNIPER_ZOOM_FACTOR(float factor)l
extern _native void SET_TELESCOPE_CAM_ANGLE_LIMITS(float Unk583, float Unk584, float Unk585, float Unk586, float Unk587, float Unk588)l
extern _native void SET_TIME_CYCLE_FAR_CLIP_DISABLED(bool set)l

//PURPOSE: Enables the high-quality depth of field effect on script cameras, which uses both near and far planes, for the current frame only.
extern _native void SET_USE_HIGHDOF(bool set)l
extern _native void SET_VIEWPORT(int viewportid, float Unk589, float Unk590, float Unk591, float Unk592)l
extern _native void SET_VIEWPORT_DESTINATION(int viewportid, float x, float y, float z, float Unk593, int Unk594, int Unk595)l
extern _native void SET_VIEWPORT_MIRRORED(int viewportid, bool set)l
extern _native void SET_VIEWPORT_PRIORITY(int viewportid, int priority)l
extern _native void SET_VIEWPORT_SHAPE(int cam, int shape)l

//INFO:  Along with this hud scrolls off etc... to give a clean screen
//PURPOSE: Requests to display borders that make the screen look widescreen OR wider in running on a widescreen telly.
extern _native void SET_WIDESCREEN_BORDERS(bool set)l
extern _native void SNAPSHOT_CAM(int cam, int Unk596)l
extern _native void UNATTACH_CAM(CAMERA_INDEX cam)l
extern _native void UNINHERIT_CAM_ROLL(CAMERA_INDEX cam)l
extern _native void UNPOINT_CAM(CAMERA_INDEX cam)l

//Deprecated
//PROPAGATE_CAM_TO_NET_PLAYER
//STOP_PROPAGATING_CAM_TO_NET_PLAYER
//IS_CAM_COLLIDING
//IS_CAM_SHAKING
#pragma endregion

#pragma region AUDIO
extern _native bool CAN_BE_DESCRIBED_AS_A_CAR(VEHICLE_INDEX veh)l
extern _native bool ENABLE_SCRIPT_CONTROLLED_MICROPHONE(void)l

//PURPOSE: Checks that a sound has finished playing. 
extern _native bool HAS_SOUND_FINISHED(int sound)l

//PURPOSE: Checks if ambient speech for ped has been disabled.
extern _native bool IS_AMBIENT_SPEECH_DISABLED(PED_INDEX ped)l

//PURPOSE: Checks if ambient speech is currently playing.
extern _native bool IS_AMBIENT_SPEECH_PLAYING(PED_INDEX ped)l

//PURPOSE: Checks if any type of speech is playing.
extern _native bool IS_ANY_SPEECH_PLAYING(PED_INDEX ped)l
extern _native bool IS_GAME_IN_CONTROL_OF_MUSIC(void)l
extern _native bool IS_LAZLOW_STATION_LOCKED(void)l

//PURPOSE: Checks if the mission complete audio is playing
extern _native bool IS_MISSION_COMPLETE_PLAYING(void)l

//PURPOSE: Checks if a mobile phone call is currently happening.
extern _native bool IS_MOBILE_PHONE_CALL_ONGOING(void)l

//PURPOSE: Checks if the moblie phone radio is active
extern _native bool IS_MOBILE_PHONE_RADIO_ACTIVE(void)l
extern _native bool IS_PAIN_PLAYING(PED_INDEX ped)l

//PURPOSE: Checks if the radio is tuning to a station
extern _native bool IS_RADIO_RETUNING(void)l
extern _native bool IS_SCRIPTED_CONVERSATION_ONGOING(void)l

//PURPOSE: Checks if scripted speech is currently playing from the given ped.
extern _native bool IS_SCRIPTED_SPEECH_PLAYING(PED_INDEX ped)l

//PURPOSE: Load in named stream.
extern _native bool PRELOAD_STREAM(const char* name)l

//PURPOSE: Load in named stream.
extern _native bool PRELOAD_STREAM_WITH_START_OFFSET(const char* StreamName, int StartOffset)l

//PURPOSE: Requests and checks that an ambient audio bank has loaded.
extern _native bool REQUEST_AMBIENT_AUDIO_BANK(const char* name)l

//PURPOSE: Requests and checks that a mission audio bank has loaded.
extern _native bool REQUEST_MISSION_AUDIO_BANK(const char* name)l

//PURPOSE: Gets the name of the players radio station. This serves as the text label.
extern _native const char* GET_PLAYER_RADIO_STATION_NAME(void)l
extern _native const char* GET_RADIO_NAME(int id)l

//PURPOSE: Returns a zone name for the emergency service call.
extern _native const char* GET_SPEECH_FOR_EMERGENCY_SERVICE_CALL(void)l
extern _native int FIND_STATIC_EMITTER_INDEX(const char* StaticEmitterName)l
extern _native int GET_AUDIBLE_MUSIC_TRACK_TEXT_ID(void)l
extern _native int GET_AUDIO_ROOM_ID(void)l
extern _native int GET_CURRENT_SCRIPTED_CONVERSATION_LINE(void)l
extern _native int GET_CUTSCENE_AUDIO_TIME_MS(void)l

//PURPOSE: Gets the index of the current players radio station
extern _native int GET_PLAYER_RADIO_STATION_INDEX(void)l

//INFO: This function returns a new SoundId, which is used for keeping track of sounds after they've been triggered -
//			use this if you need to control a sound after it's been started, for instance to stop a looping sound, or to change a sound's pitch midway through playback.
//
//			The SoundId is always an integer greater than or equal to zero; if a playback function has a SoundId field but the sound doesn't need to be altered after
//			triggering then pass a value of -1 for fire-and-forget playback, rather than getting a SoundId.
//
//			SoundIds can be reused, without needing to release them and grab a new one. If a sound's finished playing,
//			you can reuse its SoundId to kick off another one. If the sound's not finished playing,
//			it'll be stopped first (fading out or whatever is set up in RAVE by the sound designer), and the new one kicked off; usually it is safer to just get a new SoundId.
//PURPOSE: Gets an id of a triggered sound.
extern _native int GET_SOUND_ID(void)l
extern _native int GET_STATIC_EMITTER_PLAYTIME(int StaticEmitterIndex)l
extern _native int GET_STREAM_PLAYTIME(void)l
extern _native void ABORT_SCRIPTED_CONVERSATION(bool finish_current_line)l
extern _native void ADD_LINE_TO_CONVERSATION(int speaker_number, const char* context, const char* subtitle, int listener_number, int volume)l

//PURPOSE: This command should be called for each line of speech in the conversation. 
extern _native void ADD_LINE_TO_MOBILE_PHONE_CALL(int speaker_number, const char* context, const char* subtitle)l
extern _native void ADD_LINE_TO_SCRIPTED_CONVERSATION(int speaker_number, const char* context, const char* subtitle)l
extern _native void ADD_NEW_CONVERSATION_SPEAKER(int speaker_converstation_index, PED_INDEX ped, const char* voice_name)l
extern _native void ADD_NEW_FRONTEND_CONVERSATION_SPEAKER(uint Unk531, const char* Unk532)l
extern _native void AMBIENT_AUDIO_BANK_NO_LONGER_NEEDED(void)l

//PURPOSE: Stops curretly playing ambient speech.
extern _native void CANCEL_CURRENTLY_PLAYING_AMBIENT_SPEECH(PED_INDEX ped)l
extern _native void CLOSE_MIC_PED(int unk, PED_INDEX ped)l
extern _native void DISABLE_END_CREDITS_FADE(void)l
extern _native void DISABLE_FRONTEND_RADIO(void)l
extern _native void DISABLE_GPS(bool disable)l
extern _native void DISABLE_POLICE_SCANNER(void)l
extern _native void DONT_ABORT_CAR_CONVERSATIONS(bool flag0, bool flag1)l
extern _native void ENABLE_CHASE_AUDIO(bool enable)l
extern _native void ENABLE_END_CREDITS_FADE(void)l
extern _native void ENABLE_FRONTEND_RADIO(void)l
extern _native void ENABLE_GPS_IN_VEHICLE(VEHICLE_INDEX veh, bool enable)l
extern _native void ENABLE_POLICE_SCANNER(void)l
extern _native void FIX_AMBIENCE_ORIENTATION(bool fix)l
extern _native void FIX_SCRIPT_MIC_TO_CURRENT_POSISITION(void)l
extern _native void FORCE_FULL_VOICE(PED_INDEX ped)l
extern _native void FORCE_INITIAL_PLAYER_STATION(const char* stationName)l
extern _native void FORCE_RADIO_TRACK(const char* radiostation, const char* trackname, uint Unk533, uint Unk534)l
extern _native void FREEZE_RADIO_STATION(const char* stationName)l
extern _native void GET_STREAM_BEAT_INFO(float* Unk501, float* Unk502, uint* Unk503)l
extern _native void HANDLE_AUDIO_ANIM_EVENT(PED_INDEX ped, const char* AudioAnimEventName)l
extern _native void HELI_AUDIO_SHOULD_SKIP_STARTUP(VEHICLE_INDEX heli, bool skip)l
extern _native void HIGH_FALL_SCREAM(PED_INDEX ped)l
extern _native void LOCK_LAZLOW_STATION(void)l
extern _native void MISSION_AUDIO_BANK_NO_LONGER_NEEDED(void)l
extern _native void MUTE_GAMEWORLD_AND_POSITIONED_RADIO_FOR_TV(bool mute)l
extern _native void MUTE_GAMEWORLD_AUDIO(bool mute)l
extern _native void MUTE_POSITIONED_RADIO(bool mute)l
extern _native void MUTE_STATIC_EMITTER(int StaticEmitterIndex, bool mute)l

//INFO:You must call this command before using ADD_LINE_TO_MOBILE_PHONE_CALL and START_MOBILE_PHONE_CALL to trigger a phone conversation. 
//			It clears out the array containing the lines of speech.
//PURPOSE: Sets up the script for a new call.
extern _native void NEW_MOBILE_PHONE_CALL(void)l
extern _native void NEW_SCRIPTED_CONVERSATION(void)l
extern _native void ON_FIRE_SCREAM(PED_INDEX ped)l
extern _native void PANIC_SCREAM(PED_INDEX ped)l
extern _native void PAUSE_SCRIPTED_CONVERSATION(bool finish_current_line)l

//INFO: If no Pan or SpeakerMask is set by the sound designer, the sound will play at the origin, as such this command should only really be used for playing hard-panned (i.e. stereo) sounds. 
//PURPOSE: Plays back a specified sound.
extern _native void PLAY_AUDIO_EVENT(const char* name)l

//PURPOSE: Plays back a specified sound from an object
extern _native void PLAY_AUDIO_EVENT_FROM_OBJECT(const char* EventName, OBJECT_INDEX obj)l

//PURPOSE: Plays back a specified sound from a ped
extern _native void PLAY_AUDIO_EVENT_FROM_PED(const char* name, PED_INDEX ped)l

//PURPOSE: Plays back a specified sound from a vehicle
extern _native void PLAY_AUDIO_EVENT_FROM_VEHICLE(const char* name, VEHICLE_INDEX veh)l
extern _native void PLAY_FIRE_SOUND_FROM_POSITION(int sound_id, float x, float y, float z)l
extern _native void PLAY_SCRIPTED_CONVERSATION_FRONTEND(bool play)l

//INFO:If the sound is to be manipulated - i.e. stopped - then a SoundId should be acquired using GET_SOUND_ID(), otherwise use -1.
//			If this is used to play a sound for which no Pan or Speakermask is set by the sound designer, then the sound will play from the map's origin - 
//			therefore this should only be used to play frontend sounds like menu bleeps or other artificially panned effects.  
//PURPOSE: Plays back a sound with the name SoundName. Optionally can specify a sound set which contains the named sound.
extern _native void PLAY_SOUND(int SoundId, const char* SoundName)l

//INFO: The sound's position will track the object's position as it moves. If the sound doesn't need to be stopped (or have variables passed to it) then a soundId of -1 should be used, 
//			otherwise use GET_SOUND_ID to assign the sound a SoundId. 
//PURPOSE: Plays back a sound from an object's location
extern _native void PLAY_SOUND_FROM_OBJECT(int sound_id, const char* name, OBJECT_INDEX obj)l

//INFO: The sound's position will track the ped's position as it moves. If the sound doesn't need to be stopped (or have variables passed to it) then a soundId of -1 should be used, 
//			otherwise use GET_SOUND_ID to assign the sound a SoundId. 
//PURPOSE: Plays back a sound from a ped's location
extern _native void PLAY_SOUND_FROM_PED(int SoundId, const char* SoundName, PED_INDEX ped)l

//PURPOSE: Plays back a sound from an absolute position. Optionally can specify a sound set which contains the sound.
extern _native void PLAY_SOUND_FROM_POSITION(int sound_id, const char* name, float x, float y, float z)l

//INFO: The sound's position will track the vehicle's position as it moves. If the sound doesn't need to be stopped (or have variables passed to it) then a soundId of -1 should be used, 
//			otherwise use GET_SOUND_ID to assign the sound a SoundId. 
//PURPOSE: Plays back a sound from a vehicle's location
extern _native void PLAY_SOUND_FROM_VEHICLE(int SoundId, const char* SoundName, VEHICLE_INDEX veh)l

//INFO: If the sound is to be manipulated - i.e. it's a looped that needs to be stopped - then a SoundId should be acquired and used, otherwise use -1 for this parameter.
//			If the sound has a Pan or a SpeakerMask set by the sound designer then the it will play using these settings, otherwise it will play from dead ahead (0°). 
//PURPOSE: Plays back a sound "frontend" - at full volume, panned centrally. Optionally can specify a sound set which contains the sound.
extern _native void PLAY_SOUND_FRONTEND(int sound_id, const char* soundName)l

//PURPOSE: Plays a preloaded stream back from a specified object. 
extern _native void PLAY_STREAM_FROM_OBJECT(OBJECT_INDEX obj)l

//PURPOSE: Plays a preloaded stream back from a specified ped. 
extern _native void PLAY_STREAM_FROM_PED(PED_INDEX ped)l
extern _native void PLAY_STREAM_FRONTEND(void)l
extern _native void PREVIEW_RINGTONE(int RingtoneId)l

//PURPOSE: Registers a script with audio; a neccesary step for loading sound banks and using script sound commands.
extern _native void REGISTER_SCRIPT_WITH_AUDIO(bool reg)l

//INFO: This should be called once a sound has finished being manipulated by the script so that its SoundId can be released and re-used.
//PURPOSE: Releases a sound ID.
extern _native void RELEASE_SOUND_ID(int sound_id)l
extern _native void RELEASE_SCRIPT_CONTROLLED_MICROPHONE(void)l
extern _native void REMOVE_CLOSE_MIC_PED(int unk)l
extern _native void REMOVE_SCRIPT_MIC(void)l
extern _native void REPORT_CRIME(float x, float y, float z, const char* name)l
extern _native void REPORT_DISPATCH(uint unk, float x, float y, float z)l
extern _native void REPORT_POLICE_SPOTTING_SUSPECT(PED_INDEX ped)l
extern _native void REPORT_SUSPECT_ARRESTED(void)l
extern _native void REPORT_SUSPECT_DOWN(void)l
extern _native void REPORT_TAGGED_RADIO_TRACK(uint TrackTextId)l

//PURPOSE: Call this to restart a conversation that you have previously paused with PAUSE_SCRIPTED_CONVERSATION
extern _native void RESTART_SCRIPTED_CONVERSATION(void)l

//PURPOSE: Sets the radio to tune down
extern _native void RETUNE_RADIO_DOWN(void)l
extern _native void RETUNE_RADIO_TO_STATION_INDEX(int radioStation)l

//PURPOSE: Sets the radio station to a specific station. 
extern _native void RETUNE_RADIO_TO_STATION_NAME(const char* name)l

//PURPOSE: Sets the radio to tune up
extern _native void RETUNE_RADIO_UP(void)l
extern _native void SAY_AMBIENT_SPEECH(PED_INDEX ped, const char* speech, bool flag0, bool flag1, uint style)l
extern _native void SAY_AMBIENT_SPEECH_WITH_VOICE(PED_INDEX ped, const char* speech, const char* VoiceName, bool flag0, bool flag1, uint style)l

//PURPOSE: Sets a ped to use a specific voice different to the one associated with their model.
extern _native void SET_AMBIENT_VOICE_NAME(PED_INDEX ped, const char* name)l
extern _native void SET_BRIANS_MOOD(BRIANS_MOOD mood)l
extern _native void SET_GPS_VOICE_FOR_VEHICLE(VEHICLE_INDEX veh, int VoiceId)l
extern _native void SET_LOBBY_MUTE_OVERRIDE(bool set)l
extern _native void SET_LOCAL_PLAYER_VOICE(const char* name)l
extern _native void SET_LOCAL_PLAYER_PAIN_VOICE(const char* name)l
extern _native void SET_LOUD_VEHICLE_RADIO(VEHICLE_INDEX veh, bool set)l
extern _native void SET_MISSION_PICKUP_SOUND(uint model, const char* SoundName)l

//PURPOSE: Sets if the phone radio is active
extern _native void SET_MOBILE_PHONE_RADIO_STATE(bool state)l
extern _native void SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(bool set)l
extern _native void SET_MOBILE_RING_TYPE(int type)l

//PURPOSE: Sets the ped to sound angry. 
extern _native void SET_PED_IS_BLIND_RAGING(PED_INDEX ped, bool value)l

//PURPOSE: Sets the ped so they sound drunk
extern _native void SET_PED_IS_DRUNK(PED_INDEX ped, bool value)l
extern _native void SET_PED_MOBILE_RING_TYPE(PED_INDEX ped, uint type)l
extern _native void SET_PLAYER_PAIN_ROOT_BANK_NAME(const char* name)l
extern _native void SET_ROMANS_MOOD(ROMANS_MOOD mood)l
extern _native void SET_SCRIPT_MIC_LOOK_AT(float x, float y, float z)l
extern _native void SET_SCRIPT_MIC_POSITION(float x, float y, float z)l

//PURPOSE: Plays the siren audio without a driver in a vehicle
extern _native void SET_SIREN_WITH_NO_DRIVER(VEHICLE_INDEX car, bool set)l
extern _native void SET_STREAM_PARAMS(float rolloff, int UnkTime)l

//PURPOSE: Sets the radio in the garage on or off
extern _native void SET_TAXI_GARAGE_RADIO_STATE(bool radiostate)l
extern _native void SET_TRAIN_AUDIO_ROLLOFF(VEHICLE_INDEX train, float rolloff)l

//INFO: This command allows a scripter to communicate with the sound engine in complex ways, 
//			by passing a floating point value to a specific sound object. This allows some nice effects such as adjusting the pitch of a sample being to be played back, 
//			or varying a lowpass cutoff. The VariableName parameter must be set up in RAVE (the audio scripting tool) as well as instruction on its usage on a case-by-case
//			basis therefore a sound designer must be consulted with before using this command. 
//PURPOSE: Sets a variable on a sound.
extern _native void SET_VARIABLE_ON_SOUND(int sound_id, const char* varname, float value)l
extern _native void SET_VOICE_ID_FROM_HEAD_COMPONENT(PED_INDEX ped, uint component, bool IsMale)l
extern _native void SKIP_RADIO_FORWARD(void)l
extern _native void SKIP_TO_NEXT_SCRIPTED_CONVERSATION_LINE(void)l
extern _native void START_CUSTOM_MOBILE_PHONE_RINGING(int RingtoneId)l
extern _native void START_END_CREDITS_MUSIC(void)l

//INFO:  Use after setting up the phone conversation with a series of calls to ADD_LINE_TO_MOBILE_PHONE_CALL, 
//PURPOSE: Starts the actual mobile phone dialogue playing.
extern _native void START_MOBILE_PHONE_CALL(PED_INDEX ped, const char* first_voic_name, PED_INDEX second_speaker, const char* second_voice_name, bool display_subtitles, bool add_to_briefing)l
extern _native void START_MOBILE_PHONE_CALLING(void)l
extern _native void START_MOBILE_PHONE_RINGING(void)l
extern _native void START_PED_MOBILE_RINGING(PED_INDEX ped, int Unk801)l
extern _native void START_SCRIPT_CONVERSATION(bool display_subtitles, bool add_to_brief)l
extern _native void STOP_END_CREDITS_MUSIC(void)l
extern _native void STOP_MOBILE_PHONE_RINGING(void)l
extern _native void STOP_PED_MOBILE_RINGING(PED_INDEX ped)l

//INFO: This doesn't stop a piece of dialogue that has been triggered.
//			 This stops the ability to force ambient dialogue if set to true - however setting it to false, then triggering a context, then setting it to true again will allow this. 
//			 Nb. This does not sync over the network, it will only affect peds locally.
//PURPOSE: Stops a ped from saying any of their ambient dialogue.
extern _native void STOP_PED_SPEAKING(PED_INDEX ped, bool should_disable)l
extern _native void STOP_PREVIEW_RINGTONE(void)l

//INFO: SoundId must be specified when triggering the sound effect and must be referred to when calling STOP_SOUND. 
//	Calling STOP_SOUND on a sound that has finished playing will have no ill effects as long as the SoundId hasn't been released; attempting to stop an invalid SoundId will cause an assert.
//PURPOSE: Stops a playing sound by its sound id.
extern _native void STOP_SOUND(int sound_id)l

//PURPOSE: Stops a stream that is being played by the script
extern _native void STOP_STREAM(void)l
extern _native void TRIGGER_LOADING_MUSIC_ON_NEXT_FADE(void)l
extern _native void TRIGGER_MISSION_COMPLETE_AUDIO(int id)l
extern _native void TRIGGER_POLICE_REPORT(const char* name)l
extern _native void TRIGGER_VIGILANTE_CRIME(uint unk, float x, float y, float z)l
extern _native void UNFREEZE_RADIO_STATION(const char* radiostation)l
extern _native void UNLOCK_GENERIC_NEWS_STORY(int StoryId)l
extern _native void UNLOCK_LAZLOW_STATION(void)l

//PURPOSE: Sets a news story to be played a specific time
extern _native void UNLOCK_MISSION_NEWS_STORY(int id)l
extern _native void UNPAUSE_RADIO(void)l

//INFO: Doesn't need to be called in your cleanup, as TERMINATE_THIS_SCRIPT will take care of it. 
//PURPOSE: Unregisters a script with audio.
extern _native void UNREGISTER_SCRIPT_WITH_AUDIO(void)l

//Deprecated
//START_SCRIPTED_CONVERSATION
#pragma endregion

#undef _native
#undef _native32
