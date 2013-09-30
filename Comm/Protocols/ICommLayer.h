/****************************************************************************
* Comm/Protocols/ICommLayer.h
* Description:
*	Defines common interface for all communication layers
*	(CommLayer is never the physical layer)
*****************************************************************************/

#pragma once

#include "Comm/Base/ICommBase.h"

namespace Nexus
{

class ICommLayer : public ICommBase
{
protected:
	ICommBase *m_pUnderlyingComm;
	bool m_bIsConnected;
	
	// !! Private constructor !! This class shouldn't be instantiated
	ICommLayer();

public:
	void SetUnderlyingComm(ICommBase *a_pNewUnderlyingComm);
    ICommBase* UnderlyingComm() { return m_pUnderlyingComm; }

	virtual TCommErr Connect();
	virtual TCommErr Disconnect();
	virtual bool IsConnected();
	virtual TCommErr Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMS = DEFAULT_TIMEOUT);
	virtual TCommErr Receive(NX_OUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);
};

}