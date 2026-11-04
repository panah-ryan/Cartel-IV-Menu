#include "stdafx.h"
#include "threading.h"
#include "Utils.h"

#define MAX_THREADS 20

typedef struct ThreadUpdate
{
	bool(*fpThread_update)(void*);
	int iTimer;
	int iLastRan;
	uint dwThreadId;
	uint dwParam;
	THREAD_UPDATE_STATUS eStatus : 8;
	_BOOL8 bHasParam : 8;
	short _padding : 16;
} ThreadUpdate;

typedef struct ThreadData
{
	ThreadUpdate sUpdateThread[MAX_THREADS];
	int iThreadTimer;
	int iThreadCount;
} ThreadData;

ThreadData sThreadData;

void init_thread_data()
{
	sThreadData.iThreadTimer = 0;
	sThreadData.iThreadCount = 0;
	memset(sThreadData.sUpdateThread, 0, sizeof(sThreadData.sUpdateThread));
}

bool register_thread(bool(*thread_update)(void*), uint hash, THREAD_UPDATE_STATUS status, int next_execution, uint param, bool has_param)
{
	for (int i = 0; i < MAX_THREADS; i++)
	{
		ThreadUpdate* thread = &sThreadData.sUpdateThread[i];
		if (thread->fpThread_update == nullptr && thread->dwThreadId == NULL)
		{
			thread->fpThread_update = thread_update;
			thread->eStatus = status;
			thread->iTimer = status == THREAD_UPDATE_EVERYFRAME ? 0 : next_execution;
			thread->iLastRan = 0;
			thread->dwThreadId = hash;
			thread->dwParam = has_param ? param : NULL;
			thread->bHasParam = has_param;
			sThreadData.iThreadCount++;
			return true;
		}
	}

	return false;
}

bool remove_thread(uint hash)
{
	for (int i = 0; i < MAX_THREADS; i++)
	{
		ThreadUpdate* thread = &sThreadData.sUpdateThread[i];
		if (thread->dwThreadId == hash)
		{
			memset(thread, 0, sizeof(ThreadUpdate));
			sThreadData.iThreadCount--;
			return true;
		}
	}

	return false;
}

void process_threads()
{
	if (sThreadData.iThreadCount <= 0) //we dont have any threads just return
		return;

	for (int i = 0; i < MAX_THREADS; i++)
	{
		ThreadUpdate* thread = &sThreadData.sUpdateThread[i];
		if (thread->fpThread_update && thread->dwThreadId) //found valid thread
		{
			if (thread->eStatus == THREAD_UPDATE_EVERYFRAME || thread->iTimer + thread->iLastRan < GetTickCount()) //runs everyframe or hit our time to run
			{
				bool ret = thread->fpThread_update((void*)thread->dwParam);

				//pointer_call(thread->fpThread_update, thread->bHasParam ? (void*)thread->dwParam : nullptr, nullptr, (uint*)&ret, thread->bHasParam ? 1 : 0);
				thread->iLastRan = GetTickCount();

				if (ret) //thread returned true so lets terminate this thread
					remove_thread(thread->dwThreadId);
			}
		}
	}
}