/***************************************************************
* Comm/Protocols/ICommLayer.cpp
* Description:
*	Provides default implementation for the interface methods
****************************************************************/

#include "ICommLayer.h"

using namespace Nexus;

/*****************************************************************
*	ICommLayer::ICommLayer()
*
*	Description:
*		Constructor
*
*	Arguments:
*		None
*
*	Return Value:
*		None
*		
*****************************************************************/
ICommLayer::ICommLayer()
{
	m_pUnderlyingComm = NULL;
	m_bIsConnected = false;
}


/*****************************************************************
*	void ICommLayer::SetUnderlyingComm(ICommBase *a_pNewUnderlyingComm)
*
*	Description:
*		Set the next comm on the chain
*
*	Arguments:
*		ICommBase *a_pNewUnderyingComm
*			The comm to be inserted next after this comm
*
*	Return Value:
*		None
*		
*****************************************************************/
void ICommLayer::SetUnderlyingComm(ICommBase *a_pNewUnderlyingComm)
{
	m_pUnderlyingComm = a_pNewUnderlyingComm;
}


/*****************************************************************
*	TCommErr ICommLayer::Connect()
*
*	Description:
*		Connecting if not already connected
*
*	Arguments:
*		None
*
*	Return Value:
*		TCommErr
*			E_NEXUS_OK - Everything is OK
*			E_NO_UNDERLYING_LAYER - No underlying layer
*		
*****************************************************************/
TCommErr ICommLayer::Connect()
{
	if (m_pUnderlyingComm != NULL)
	{
		m_bIsConnected = true;
		return E_NEXUS_OK;
	}
	else return E_NEXUS_NO_UNDERLYING_COMM;
}

/*****************************************************************
*	TCommErr ICommLayer::Disconnect()
*
*	Description:
*		Disconnecting if not already disconnected
*
*	Arguments:
*		None
*
*	Return Value:
*		TCommErr
*			E_NEXUS_OK - Everything is OK
*			E_NOT_CONNECTED
*		
*****************************************************************/
TCommErr ICommLayer::Disconnect()
{
	if (m_bIsConnected)
	{
		m_bIsConnected = false;
		return E_NEXUS_OK;
	}
	else return E_NEXUS_NOT_CONNECTED;
}


/*****************************************************************
*	bool ICommLayer::IsConnected()
*
*	Description:
*		Returns the connection status
*
*	Arguments:
*		None
*
*	Return Value:
*		True if connected
*		
*****************************************************************/
bool ICommLayer::IsConnected()
{
	return m_bIsConnected;
}

/***************************************************************************************************
*	TCommErr ICommLayer::Send(IN CData *a_pData, IN IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMS)
*
*	Description:
*		Proxy function for sending
*
*	Arguments:
*		IN CData *a_pData
*			Packet data
*		IN IMetaData *a_pMetaData
*			Packet's metadata
*		DWORD a_dwTimeoutMs
*			Timeout in milliseconds
*
*	Return Value:
*		TCommErr
*			E_NEXUS_OK - Everything is OK
*			E_NOT_CONNECTED
*			Other error from the underlying layer
*		
*****************************************************************/
TCommErr ICommLayer::Send(IN CData *a_pData, IN IMetaData *a_pMetaData /* = NULL */, IN DWORD a_dwTimeoutMS /* = DEFAULT_TIMEOUT */)
{
	if (!m_bIsConnected)
	{
		return E_NEXUS_NOT_CONNECTED;
	}

	return m_pUnderlyingComm->Send(a_pData, a_pMetaData, a_dwTimeoutMS);
}


/*******************************************************************************************************
	TCommErr ICommLayer::Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
*
*	Description:
*		Proxy function for receiving
*
*	Arguments:
*		OUT CData *a_pData
*			Received packet data
*		IMetaData *a_pMetaData
			Received packet metadata
*		DWORD a_dwTimeoutMs
*			Timeout in milliseconds
*
*	Return Value:
*		TCommErr
*			E_NEXUS_OK - Everything is OK
*			E_NOT_CONNECTED
*			Other error from the underlying layer
*		
*******************************************************************************************************/
TCommErr ICommLayer::Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData /* = NULL */, IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
	if (!m_bIsConnected)
	{
		return E_NEXUS_NOT_CONNECTED;
	}

	return m_pUnderlyingComm->Receive(a_pData, a_pMetaData, a_dwTimeoutMs);
}