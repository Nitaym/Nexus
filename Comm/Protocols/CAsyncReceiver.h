/*******************************************************************************************
* Comm/Protocols/CAsyncReceiver.h
* Description:
*	Incoming packet buffer - Manages an incoming packets queue, receives packet
*	automatically by its own thread
*******************************************************************************************/

#pragma once

#define NOMINMAX

#include <queue>
#include "General/Utils.h"
#include "General/General.h"
#include "Comm/Protocols/ICommLayer.h"
#include "Comm/Protocols/CallbackValues.h"

using namespace std; // for the queue

namespace Nexus
{
// Struct for the queue elements
struct TReceivePacket
{
	CData *a_pData;
	IMetaData *a_pMetaData;
};

typedef TReceiveCallback (*typeAsyncReceiverCallback)(CData *a_pData, IMetaData *a_pMetaData);

class CAsyncReceiver : public ICommLayer
{
private:
	typeAsyncReceiverCallback m_pUserCallback;
	void *m_pUserParam;

	// Syncing variable (kinda mutex)
	bool m_bOperatOnQueue;
	int m_iMaxPacketSize;
	queue<TReceivePacket> m_qIncomingPackets;
	IMetaData *m_pMetaDataType;

	HTHREAD m_iReceiveThread;

public:
	// We have 2 constructors: one for implementation with a user callback and one without
	CAsyncReceiver();
	CAsyncReceiver(typeAsyncReceiverCallback pUserCallbackFunc);

	// Receive data packet from the received packets queue
	TCommErr Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// Start the receiving thread
	TCommErr Connect();

	// Stop the receiving thread
	TCommErr Disconnect();
    TCommErr WaitOnDisconnect(int a_iTimeout);

	// Is the thread running = Is receiving
	bool IsConnected();
	void SetMaxPacketSize (int a_iMaxPacketSize);
	void SetMetaDataObject (IN IMetaData *a_pMetaData);

private:
    void Initialize();
	static void* ReceiveThread(void *a_pvParam);
};

}
