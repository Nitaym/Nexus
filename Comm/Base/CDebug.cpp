/*****************************************************************************
* Comm/Base/CDebug.cpp
* Description:
*	In charge of all debug prints. Debug prints are passed to an ICommBase
******************************************************************************/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "CDebug.h"

#define MAX_LINE_LEN 500
#define WRITE_TIMEOUT_MS 1000


using namespace Nexus;

/*****************************************************************
*	CDebug::CDebug(ICommBase *a_pComm)
*
*	Description:
*		Receives an ICommBase to pass all writings to it
*
*	Arguments:
*		None
*
*	Return Value:
*		None
*		
*****************************************************************/
CDebug::CDebug(ICommBase *a_pComm)
{
	m_pComm = a_pComm;
	Enable();
}

/*****************************************************************
*	void CDebug::Enable()
*
*	Description:
*		Enable all writing operation
*
*	Arguments:
*		None
*
*	Return Value:
*		void
*		
*****************************************************************/
void CDebug::Enable()
{
	m_bIsEnabled = true;
}


/*****************************************************************
*	void CDebug::Disable()
*
*	Description:
*		Disable all writing operation
*
*	Arguments:
*		None
*
*	Return Value:
*		None
*		
*****************************************************************/
void CDebug::Disable()
{
	m_bIsEnabled = false;
}

void CDebug::Write(const char *format, ...)
{
	// Don't send output if underlying is not connected
	if (!m_bIsEnabled)
		return;

	int l_iLen;
	va_list l_tAp;
	char l_strStr[MAX_LINE_LEN] = {' ', '@', ' '}; // #can use vscprintf to get len
	const int l_iPrefixLen = 3;

	// Printf into the buffer
	va_start(l_tAp, format);
	l_iLen = vsprintf(l_strStr + l_iPrefixLen, format, l_tAp);

	// Validate lens
	if (l_iLen > MAX_LINE_LEN)
	{
		printf(" # CDebug:Write - Buffer Overflow");
		return;
	}

	// Send buffer by underlying comm
	CData l_oData(l_strStr);
	m_pComm->Send(&l_oData, NULL, WRITE_TIMEOUT_MS);
}