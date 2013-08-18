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

#define dprintf if (m_pDebug != NULL) m_pDebug->Write

namespace Nexus
{

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
	virtual TCommErr Send (NX_IN CData *a_pData, NX_IN IMetaData *a_pMetatData = NULL, NX_IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT) = 0;
	virtual TCommErr Receive(NX_OUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT) = 0;
	virtual TCommErr SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn = NULL, NX_OUT IMetaData *a_pMetaDataOut = NULL, NX_IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT)
	{
		// Default implementation of this virtual function
		return E_NEXUS_NOT_SUPPORTED;
	}

	// Built in methods
	ICommBase()						{ m_pDebug = NULL; }
	void SetDebug(CDebug *a_pDebug)	{ m_pDebug = a_pDebug; }
    CDebug* GetDebug() { return m_pDebug; }
};
}