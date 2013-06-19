/*****************************************************************
* General/Utils.h
* Description:
*	Operating system dependent API function wrappers
******************************************************************/

#pragma once

// Include stuff for windows & linux
#ifdef WIN32
// Windows
#include <Windows.h>

#else
// Linux
#include <pthread.h>
#endif


#include "General/General.h"
#include <stdio.h>
#include <string>


using namespace std;



#ifdef WIN32

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

inline void BinToHexString(byte* a_bBuffer, char* a_sString, int a_iLength);

inline void PrintStringAsHex(string strData);

inline void PrintArrayAsHex(BYTE *pArray, DWORD dwSize);

}
