/************************************************************
* Comm/Base/ICommBase.h
* Description:
*	Defines a common interface for all communication modules
*************************************************************/

#pragma once

#include "General/General.h"
#include "CData.h"
#include "IMetaData.h"
#include "CommErrors.h"
#include "CDebug.h"

#define INFINITE_TIMEOUT 100
#define DEFAULT_TIMEOUT 40

class CDebug;

class ICommBase
{
protected:
	CDebug *m_pDebug;

public:
	// Abstract functions (=0 means abstract)
	virtual TCommErr Connect() = 0;
	virtual TCommErr Disconnect() = 0;
	virtual bool IsConnected() = 0;
	virtual TCommErr Send (IN CData *a_pData, IN IMetaData *a_pMetatData = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT) = 0;
	virtual TCommErr Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT) = 0;
	virtual TCommErr SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn = NULL, OUT IMetaData *a_pMetaDataOut = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT)
	{
		// Default implementation of this virtual function
		return E_NEXUS_NOT_SUPPORTED;
	}

	// Built in methods
	ICommBase()						{ m_pDebug = NULL; }
	void SetDebug(CDebug *a_pDebug)	{ m_pDebug = a_pDebug; }
};