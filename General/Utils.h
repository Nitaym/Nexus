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
typedef LPTHREAD_START_ROUTINE typeCallbackFunc;

#else
#define HANDLE int

typedef pthread_t HTHREAD;

typedef void* (*typeCallbackFunc)(void *a_pParam);

HTHREAD CreateThread(typeCallbackFunc a_pFunction, void *a_pParam)
void Sleep(DWORD dwMilliseconds);
DWORD GetTickCount();

#endif

HTHREAD NexusCreateThread(typeCallbackFunc a_pFunction, void *a_pParam);

inline void BinToHexString(byte* a_bBuffer, char* a_sString, int a_iLength)
{
	for (int i = 0; i < a_iLength; i++)
	{
		sprintf(a_sString + i*3, "%02x ", a_bBuffer[i]);
	}
}

inline void PrintStringAsHex(string strData)
{
	printf("[%d]", strData.size());
	int dwSize = strData.size();
	for (int i = 0; i < dwSize; i++)
		printf(" %02X", (BYTE)strData[i]);
	printf("\n");
}

inline void PrintArrayAsHex(BYTE *pArray, DWORD dwSize)
{
	printf("[%d]", (int)dwSize);
	for (int i = 0; i < (int)dwSize; i++)
		printf(" %02X", pArray[i]);
	printf("\n");
}