#pragma once
#include "stdafx.h"

typedef enum THREAD_UPDATE_STATUS
{
	THREAD_UPDATE_EVERYFRAME,
	THREAD_UPDATE_LONG

} THREAD_UPDATE_STATUS;

void init_thread_data();
bool register_thread(bool(*thread_update)(void*), uint hash, THREAD_UPDATE_STATUS status, int next_execution, uint param, bool has_param);
bool remove_thread(uint hash);
void process_threads();