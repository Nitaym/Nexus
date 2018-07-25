/*****************************************************************
* General/Utils.h
* Description:
*	Operating system dependent API function wrappers
******************************************************************/

#pragma once

// Include stuff for windows & linux
#ifdef _WIN32
// Windows
#include <Windows.h>

#else
// Linux
#include <pthread.h>
#include <errno.h>
#endif


#include "General/General.h"
#include <stdio.h>
#include <string>


using namespace std;



#ifdef _WIN32

typedef HANDLE HTHREAD;

#else

#define HANDLE int

typedef pthread_t HTHREAD;

void Sleep(DWORD dwMilliseconds);
DWORD GetTickCount();

#endif

typedef void* (*typeCallbackFunc)(void *a_pParam);

namespace Nexus
{

	HTHREAD NexusCreateThread(typeCallbackFunc a_pFunction, void *a_pParam);

	void BinToHexString(byte* a_bBuffer, char* a_sString, int a_iLength);

	inline void PrintStringAsHex(string strData);

	inline void PrintArrayAsHex(BYTE *pArray, DWORD dwSize);

#ifndef _WIN32
	int GetUnixBaudRate(unsigned int a_eBaudRate);
#endif

}