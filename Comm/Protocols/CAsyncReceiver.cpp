/************************************************************************************
* Comm/Protocols/CAsyncReceiver.cpp
* Description:
*	Incoming packet buffer - Manages an incoming packets queue, receives packet 
*	automatically by its own thread
*************************************************************************************/

#include "CAsyncReceiver.h"

//#define dprintf if (m_pDebug != NULL) m_pDebug -> Write
#define dprintf printf

/*****************************************************************
*	CAsyncReceiver::CAsyncReceiver()
*
*	Description:
*		Constuctor
*
*	Arguments:
*		None
*
*	Return Value:
*		None
*		
*****************************************************************/
CAsyncReceiver::CAsyncReceiver()
{
	m_pUserCallback = NULL;
	m_bOperatOnQueue = false;
	m_bIsConnected = false;
	m_iMaxPacketSize = 4096;
}


/*****************************************************************
*	CAsyncReceiver::CAsyncReceiver(typeAsyncReceiverCallback a_pUserCallbackFunc)
*
*	Description:
*		Constructor with a user callback
*
*	Arguments:
*		Callback function to be called when something has been received
*
*	Return Value:
*		None
*		
*****************************************************************/
CAsyncReceiver::CAsyncReceiver(typeAsyncReceiverCallback a_pUserCallbackFunc)
{
	m_pUserCallback = a_pUserCallbackFunc;
	m_bOperatOnQueue = false;
	m_bIsConnected = false;
}

/*****************************************************************
*	TCommErr CAsyncReceiver::Connect()
*
*	Description:
*		Start the receiving thread
*
*	Arguments:
*		None
*
*	Return Value:
*		TCommErr
*		
*****************************************************************/
TCommErr CAsyncReceiver::Connect()
{
	dprintf("CAsyncReceiver::Connect\n");

	if (m_bIsConnected)
		return E_NEXUS_ALREADY_CONNECTED;

	m_bIsConnected = true;

	m_iReceiveThread = NexusCreateThread((typeCallbackFunc)ReceiveThread, this);

	return E_NEXUS_OK;
}

/*****************************************************************
*	TCommErr CAsyncReceiver::Disconnect()
*
*	Description:
*		Stop the receiving thread.
*
*	Arguments:
*		None
*
*	Return Value:
*		TCommErr
*		
*****************************************************************/
TCommErr CAsyncReceiver::Disconnect()
{
	dprintf("CAsyncReceiver::Disconnect\n");

	if (!m_bIsConnected)
		return E_NEXUS_NOT_CONNECTED;

	m_bIsConnected = false;

	return E_NEXUS_OK;
}

/*****************************************************************
*	bool CAsyncReceiver::IsConnected()
*
*	Description:
*		Is thread running
*
*	Arguments:
*		None
*
*	Return Value:
*		True is thread is running
*		
*****************************************************************/
bool CAsyncReceiver::IsConnected()
{
	return m_bIsConnected;
}

/*****************************************************************
*	TCommErr CAsyncReceiver::Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
*
*	Description:
*		Receive a single packet from queue
*
*	Arguments:
*		CData *a_pData
*			Received packet data
*		IMetaData *a_pMetaData
*			Received packet's metatdata
*		DWORD a_dwTimeoutMs
*			Timeout (in ms)
*
*	Return Value:
*		TCommErr
*		
*****************************************************************/
TCommErr CAsyncReceiver::Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData /* = NULL */, IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
	if (!m_qIncomingPackets.empty())
	{
		// Get next packet from the queue
		// Sync
		m_bOperatOnQueue = true;
		TReceivePacket l_stPacket = (TReceivePacket)m_qIncomingPackets.front();
		m_qIncomingPackets.pop();
		m_bOperatOnQueue = false;

		// Get size of the packet that we got from the queue
		if (l_stPacket.a_pData->GetSize() == 0)
		{
			return E_NEXUS_INVALID_PACKET;
		}

		// Fill user supplied buffers with the information
		a_pData->SetString(l_stPacket.a_pData->GetString());
		if (a_pMetaData && l_stPacket.a_pMetaData)
			a_pMetaData->CopyFrom(l_stPacket.a_pMetaData); // Copy metadata to the user's metadata

		// Free memory
		SAFE_DELETE(l_stPacket.a_pData);
		SAFE_DELETE(l_stPacket.a_pMetaData);

		return E_NEXUS_OK;
	}
	else return E_NEXUS_NOTHING_TO_READ;
}


/*****************************************************************
*	void* CAsyncReceiver::ReceiveThread(void *m_pvParam)
*
*	Description:
*		[Static] Receive thread function
*
*	Arguments:
*		void *a_pvParam
*			This should be the address of the calling instance
*
*	Return Value:
*		NULL
*		
*****************************************************************/
void* CAsyncReceiver::ReceiveThread(void *a_pvParam)
{
	dprintf("CAsyncReceiver::ReceiveThread\n");

	CAsyncReceiver *l_pThis = (CAsyncReceiver*)a_pvParam;
	TReceivePacket l_stPacket;
	int l_iRes = E_FAIL;
	TReceiveCallback eCallbackRes;

	l_stPacket.a_pData = 0;
	l_stPacket.a_pMetaData = 0;

	while (l_pThis->m_bIsConnected)
	{
		// Make packet
		l_stPacket.a_pData = new CData();

		(*l_stPacket.a_pData).SetMaxSize(l_pThis->m_iMaxPacketSize);
		if (l_pThis->m_pMetaDataType != NULL)
			l_stPacket.a_pMetaData = l_pThis->m_pMetaDataType->Clone();

		if (l_pThis->m_pUnderlyingComm != NULL)
			l_iRes = l_pThis->m_pUnderlyingComm->Receive(l_stPacket.a_pData, l_stPacket.a_pMetaData, DEFAULT_TIMEOUT);

		if (l_iRes == E_NEXUS_TIMEOUT)
		{
			continue;
		}

		if (l_iRes == E_NEXUS_OK)
		{
			if (l_pThis->m_pUserCallback != NULL)
			{
				// Initiate user callback
				eCallbackRes = (*l_pThis->m_pUserCallback)(l_stPacket.a_pData, l_stPacket.a_pMetaData);
				switch (eCallbackRes)
				{
					case TReceiveCallback_PacketPutInQueue:
					{
						// Wait for queue operations to end
						while (l_pThis->m_bOperatOnQueue);

						// add to incoming packets queue
						l_pThis->m_qIncomingPackets.push(l_stPacket);
						break;
					}

					case TReceiveCallback_DoNothing:
					{
						SAFE_DELETE(l_stPacket.a_pData);
						SAFE_DELETE(l_stPacket.a_pMetaData);
						break;
					}

					default:
						break;
						// Do nothing
				}
			}
			else
			{
				// Wait for queue operations to end, for synchronizations
				while (l_pThis->m_bOperatOnQueue);

				// Add to incoming packet queue
				l_pThis->m_qIncomingPackets.push(l_stPacket);
			}
		}
		else
		{
			SAFE_DELETE(l_stPacket.a_pData);
			SAFE_DELETE(l_stPacket.a_pMetaData);
			printf(" # CAsyncReceiver::ReceiveThread> Error in Receive %d\n", l_iRes);
		}
	}

	return NULL;
}


/*****************************************************************
*	void CAsyncReceiver::SetMaxPacketSize(int a_iMaxPacketSize)
*
*	Description:
*		Sets received packets maximum size
*
*	Arguments:
*		a_iMaxPacketSize
*			The new maximum packet size
*
*	Return Value:
*		None
*		
*****************************************************************/
void CAsyncReceiver::SetMaxPacketSize(int a_iMaxPacketSize)
{
	m_iMaxPacketSize = a_iMaxPacketSize;
}

/*****************************************************************
*	void CAsyncReceiver::SetMetaDataObject(IN IMetaData *a_pMetaData)
*
*	Description:
*		Sets the packet's meta data object
*
*	Arguments:
*		IN IMetaData *a_pMetaData
*			The new object
*
*	Return Value:
*		None
*		
*****************************************************************/
void CAsyncReceiver::SetMetaDataObject(IN IMetaData *a_pMetaData)
{
	m_pMetaDataType = a_pMetaData;
}