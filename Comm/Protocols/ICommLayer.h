/****************************************************************************
* Comm/Protocols/ICommLayer.h
* Description:
*	Defines common interface for all communication layers
*	(CommLayer is never the physical layer)
*****************************************************************************/

#pragma once

#include "Comm/Base/ICommBase.h"

class ICommLayer : public ICommBase
{
protected:
	ICommBase *m_pUnderlyingComm;
	bool m_bIsConnected;
	
	// !! Private constructor !! This class shouldn't be instantiated
	ICommLayer();

public:
	void SetUnderlyingComm(ICommBase *a_pNewUnderlyingComm);

	virtual TCommErr Connect();
	virtual TCommErr Disconnect();
	virtual bool IsConnected();
	virtual TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMS = DEFAULT_TIMEOUT);
	virtual TCommErr Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);
};