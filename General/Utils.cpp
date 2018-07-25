/*********************************************************************
* General/Utils.cpp
* Description:
*	Operating system dependent API function wrappers
*********************************************************************/
#include "General/Utils.h"


#ifdef _WIN32
#pragma warning(disable : 4996)
#include <Windows.h>

using namespace Nexus;

HTHREAD Nexus::NexusCreateThread(typeCallbackFunc a_pFunction, void *a_pParam)
{
	HTHREAD l_hThread = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)a_pFunction,
		a_pParam,
		0,				// Start running
		NULL);

	return l_hThread;
}

#else

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <signal.h>
#include "General/General.h"


/*****************************************************************
*	HTHREAD CreateThread(typeCallbackFunc a_pFunction, void *a_pParam)
*
*	Description:
*		Creates a thread at the provided callback function
*
*	Arguments:
*		typeCallbackFunc pFunction
*			Address of callback function
*		void *pParam
*			Parameter to pass to the callback
*
*	Return Value:
*		HTHREAD
*			Handle to the newly created thread
*
*****************************************************************/
HTHREAD Nexus::NexusCreateThread(typeCallbackFunc a_pFunction, void *a_pParam)
{
	HTHREAD l_iThread = 0;
	int l_iResult = pthread_create(&l_iThread, NULL, a_pFunction, a_pParam);

	if (l_iResult == 0)
		return l_iThread;

	return 0;
}



/*****************************************************************
*	void Sleep(DWORD a_dwMilliseconds)
*
*	Description:
*		Blocks execution for a given number of milliseconds
*
*	Arguments:
*		DWORD dwMilliseconds
*			Number of milliseconds to sleep
*
*	Return Value:
*		void
*
*****************************************************************/
void Sleep(DWORD a_dwMilliseconds)
{
	usleep(a_dwMilliseconds * 1000);
}


/*****************************************************************
*	DWORD GetTickCount()
*
*	Description:
*		Get system time in milliseconds
*
*	Arguments:
*		None
*
*	Return Value:
*		DWORD
*			Number of milliseconds
*
*****************************************************************/
DWORD GetTickCount()
{
	struct timeval l_tTime;
	gettimeofday(&l_tTime, 0);
	// Convert to milliseconds
	return l_tTime.tv_sec * 1000 + (l_tTime.tv_usec / 1000);
}

#endif

void Nexus::BinToHexString(byte* a_bBuffer, char* a_sString, int a_iLength)
{
    for (int i = 0; i < a_iLength; i++)
    {
        sprintf(a_sString + i*3, "%02x ", a_bBuffer[i]);
    }
}

void Nexus::PrintStringAsHex(string strData)
{
    printf("[%d]", (int)strData.size());
    int dwSize = strData.size();
    for (int i = 0; i < dwSize; i++)
        printf(" %02X", (BYTE)strData[i]);
    printf("\n");
}


void Nexus::PrintArrayAsHex(BYTE *pArray, DWORD dwSize)
{
    printf("[%d]", (int)dwSize);
    for (int i = 0; i < (int)dwSize; i++)
        printf(" %02X", pArray[i]);
    printf("\n");
}

#ifndef _WIN32
int Nexus::GetUnixBaudRate(unsigned int a_eBaudRate)
{
	switch (a_eBaudRate)
	{
	case 1200:
		return B1200;
	case 2400:
		return B2400;
	case 4800:
		return B4800;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
	case 460800:
		return B460800;
	}

	return 0;
}
#endif