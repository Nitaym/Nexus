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

typedef TReceiveCallback (*typeAsyncReceiverRecvCallback)(void* a_pUserParam, CData *a_pData, IMetaData *a_pMetaData);
typedef void (*typeAsyncReceiverFailCallback)(void* a_pUserParam, TCommErr a_eError, IMetaData* a_pMetaData);

class CAsyncReceiver : public ICommLayer
{
private:
	typeAsyncReceiverRecvCallback m_pUserCallback;
    typeAsyncReceiverFailCallback m_pUserFailCallback;
	void *m_pUserParam;

	// Syncing variable (kinda mutex)
	bool m_bOperatOnQueue;
	int m_iMaxPacketSize;
	queue<TReceivePacket> m_qIncomingPackets;
	IMetaData *m_pMetaDataType;

	HTHREAD m_iReceiveThread;

    int m_iRecoveryTime;

public:
    bool Terminating;

	// We have 2 constructors: one for implementation with a user callback and one without
	CAsyncReceiver();
	CAsyncReceiver(typeAsyncReceiverRecvCallback pUserCallbackFunc, typeAsyncReceiverFailCallback a_pUserCallbackFailFunc, void* a_pUserParam);
    ~CAsyncReceiver();

	// Receive data packet from the received packets queue
	TCommErr Receive(NX_OUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// Start the receiving thread
	TCommErr Connect();

	// Stop the receiving thread
	TCommErr Disconnect();
    TCommErr WaitOnDisconnect(int a_iTimeout);

	// Is the thread running = Is receiving
	bool IsConnected();
	void SetMaxPacketSize (int a_iMaxPacketSize);
	void SetMetaDataObject (NX_IN IMetaData *a_pMetaData);
    void SetErrorRecoveryTime(int a_iTimeMs);
    int ErrorRecoveryTime() { return m_iRecoveryTime; }

    void SetUserParam(void* a_pUserParam);
	void* UserParam() { return m_pUserParam; }

    static void ReadAndReport(void *a_pvParam);
private:
    void Initialize();

	static void* ReceiveThread(void *a_pvParam);
};

}
