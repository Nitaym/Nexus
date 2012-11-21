/*******************************************************************************************
* Comm/Protocols/CAutoConnect.h
* Description:
*	Keeps you connected - Reconnects if a failure occurs
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

typedef void (*typeConnectionStateChangedCallback)(bool a_bConnectionStatus);

class CAutoConnect : public ICommLayer
{
private:
	HTHREAD m_iConnectThread;

    unsigned int m_iReconnectInterval;
    typeConnectionStateChangedCallback m_pConnectionStateChanged;

public:
    bool Terminating;

	// We have 2 constructors: one for implementation with a user callback and one without
	CAutoConnect(typeConnectionStateChangedCallback a_pCallback);
    ~CAutoConnect();

	// Receive data packet from the received packets queue
	TCommErr Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// Start the autoconnect thread
	TCommErr Connect();

	// Stop the autoconnect thread
	TCommErr Disconnect();
    TCommErr WaitOnDisconnect(int a_iTimeout);

	// Is the thread running = Is receiving
	bool IsConnected();

    // Set the interval in which the CAutoConnect checks if the underlying comm has disconnected or not
    void SetReconnectInterval( unsigned int value) { m_iReconnectInterval = value; }
    unsigned int ReconnectInterval() { return m_iReconnectInterval; }

    void ReportConnectionState(bool a_bState);
private:
    void Initialize();

	static void* ConnectThread(void *a_pvThis);
};

}
