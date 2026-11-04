/*H**********************************************************************
* FILENAME :	natives32.h					START DATE :	10 Sept 16
*
* DESCRIPTION :
*		Native functions to be used with all GTAIV(1.0.7.0) targets and platforms of SC-CL.
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

#pragma region BUILTINS
extern _native int TIMERA()l
extern _native int TIMERB()l
extern _native int TIMERSYSTEM()l
extern _native void SETTIMERA(int i)l
extern _native void SETTIMERB(int i)l
extern _native void SETTIMERSYSTEM(int i)l
extern _native float TIMESTEP()l
extern _native float TIMESTEPUNWARPED()l
extern _native void WAIT(int i)l
extern _native void WAITUNWARPED(int i)l
extern _native void WAITUNPAUSED(int i)l
extern _native void PRINTSTRING(char* s)l
extern _native void PRINTFLOAT(float f)l
extern _native void PRINTFLOAT2(int width, int precision, float f)l
extern _native void PRINTINT(int i)l
extern _native void PRINTINT2(int width, int i)l
extern _native void PRINTNL()l
extern _native void PRINTVECTOR(float x, float y, float z)l
extern _native void BREAKPOINT()l
extern _native float SIN(float t)l
extern _native float COS(float t)l
extern _native float SQRT(float t)l
extern _native float POW(float a, float b)l
extern _native float EXP(float f)l
extern _native float VMAG(float x, float y, float z)l
extern _native float VMAG2(float x, float y, float z)l
extern _native float VDIST(float x1, float y1, float z1, float x2, float y2, float z2)l
extern _native float VDIST2(float x1, float y1, float z1, float x2, float y2, float z2)l
extern _native int SHIFT_LEFT(int value, int count)l
extern _native int SHIFT_RIGHT(int value, int count)l
extern _native THREADID START_NEW_SCRIPT(char* s, int stack_size)l
extern _native THREADID START_NEW_SCRIPT_WITH_ARGS(char* s, int* args, int size_of_args, int stack_size)l
extern _native THREADID START_NEW_SCRIPT_TYPED(char* s, int stack_size, int flags)l
extern _native THREADID START_NEW_SCRIPT_TYPED_WITH_ARGS(char* s, int* args, int size_of_args, int stack_size, int flags)l
extern _native float FLOOR(float f)l
extern _native float CEIL(float f)l
extern _native float ROUND(float f)l
extern _native float TO_FLOAT(float f)l
extern _native void SNAPSHOT_GLOBALS()l
#pragma endregion

extern _native void CLEAR_PRINTS()l
extern _native void PRINT_STRING_WITH_LITERAL_STRING_NOW(const char* gxtName, const char* text, int timeMS, int color)l
extern _native void TERMINATE_THIS_THREAD()l
extern _native float ATAN2(float x, float y)l
extern _native int GET_LENGTH_OF_LITERAL_STRING(const char* literalString)l
extern _native bool ARE_STRINGS_EQUAL(const char* str1, const char* str2)l
extern _native void SET_RANDOM_SEED(int seed)l
extern _native int GET_RANDOM_INT_IN_RANGE(int min, int max)l
extern _native int GET_GAME_TIMER(void)l

//PURPOSE: Fault script with said message, function name, file name, and file line
extern _native void SCRIPT_ASSERT(const char* message, const char* func_name, const char* file_name, int file_line)l

#undef _native
#undef _native32
