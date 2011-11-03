/****************************************************************************
* Comm/Base/CDebug.h
* Description:
*	In charge of all debug prints. Debug prints are passed to an ICommBase
****************************************************************************/

#pragma once

#include "Comm/Base/ICommBase.h"

class ICommBase;

class CDebug
{
private:
	ICommBase *m_pComm;
	bool m_bIsEnabled;

public:
	//Receives an ICommBase to pass all debug writings to it
	CDebug(ICommBase *a_pComm);

	// Enable/Disable the debug writing
	void Enable();
	void Disable();

	// The write method. Same arguments as printf
	void Write(const char *format, ...);
};