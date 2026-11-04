#pragma once
#include "stdafx.h"
#include <setjmp.h>

union scrValue
{
	int Int;
	unsigned int Uns;
	float Float;
	char* String;
	scrValue* Reference;
	DWORD Any;
	BOOL Bool;
};

struct scrThread_Info
{
	scrValue* ResultPtr;
	int ParamCount;
	scrValue* Params;
	int BufferCount;
	float* Orig[4];
	__vector4 Buffer[4];
};

unsigned int scrComputeHash(char *key);

class Caller
{
private:
	int argCount;
	scrValue Call_Args[16];
	scrValue Call_Returns[3];

public:
	Caller()
	{
		argCount = 0;
		XMemSet(Call_Args, 0, sizeof(Call_Args));
		XMemSet(Call_Returns, 0, sizeof(Call_Returns));
	}

	~Caller()
	{

	}

private:
	template <typename T>
	void push_arg(T arg)
	{
		*(T*)&Call_Args[argCount] = arg;
		argCount++;
	}

	template <typename T>
	T call_ret(DWORD hash, scrThread_Info* cxt)
	{
		void* op = ((void*(*)(DWORD, DWORD))0x828440C8)(0x831927A0, hash);
		if (op && MmIsAddressValid(op))
		{
			jmp_buf check;
			int ret = setjmp(check);
			if (!ret)
			{
				try
				{
					((void(*)(scrThread_Info*))op)(cxt);
					((void(*)(scrThread_Info*))0x82843660)(cxt);
					return *(T*)&Call_Returns[0];
				}
				catch (...)
				{
					printf("Native handler caught expection calling 0x%X!", hash);
					longjmp(check, 1337);
				}
			}
		}
		return NULL;
	}

public:
	template <typename T>
	T call(DWORD hash)
	{
		scrThread_Info cxt;
		cxt.Params = nullptr;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		return call_ret<T>(hash, &cxt);
	}

	template <typename T>
	T call(char* szName)
	{
		return call<T>(scrComputeHash(szName));
	}

	template <typename T, typename A1>
	T call(DWORD hash, A1 a1)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1>
	T call(char* szName, A1 a1)
	{
		return call<T>(scrComputeHash(szName), a1);
	}

	template <typename T, typename A1, typename A2>
	T call(DWORD hash, A1 a1, A2 a2)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2>
	T call(char* szName, A1 a1, A2 a2)
	{
		return call<T>(scrComputeHash(szName), a1, a2);
	}

	template <typename T, typename A1, typename A2, typename A3>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3>
	T call(char* szName, A1 a1, A2 a2, A3 a3)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		push_arg(a10);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		push_arg(a10);
		push_arg(a11);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		push_arg(a10);
		push_arg(a11);
		push_arg(a12);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		push_arg(a10);
		push_arg(a11);
		push_arg(a12);
		push_arg(a13);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		push_arg(a10);
		push_arg(a11);
		push_arg(a12);
		push_arg(a13);
		push_arg(a14);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		push_arg(a10);
		push_arg(a11);
		push_arg(a12);
		push_arg(a13);
		push_arg(a14);
		push_arg(a15);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		push_arg(a10);
		push_arg(a11);
		push_arg(a12);
		push_arg(a13);
		push_arg(a14);
		push_arg(a15);
		push_arg(a16);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	T call(DWORD hash, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		scrThread_Info cxt;
		cxt.Params = Call_Args;
		cxt.ResultPtr = Call_Returns;
		cxt.BufferCount = 0;
		argCount = 0;

		push_arg(a1);
		push_arg(a2);
		push_arg(a3);
		push_arg(a4);
		push_arg(a5);
		push_arg(a6);
		push_arg(a7);
		push_arg(a8);
		push_arg(a9);
		push_arg(a10);
		push_arg(a11);
		push_arg(a12);
		push_arg(a13);
		push_arg(a14);
		push_arg(a15);
		push_arg(a16);
		push_arg(a17);
		return call_ret<T>(hash, &cxt);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	T call(char* szName, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		return call<T>(scrComputeHash(szName), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}
};