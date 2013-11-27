/*****************************************
* Comm/Protocols/CSocket.h
* Description:
* TCP/IP Socket Communication Module
*
* TODO:
* - Add handling of client disconnections
******************************************/

#pragma once

#ifdef WIN32
#include <winsock2.h>
#endif

#include "../../General/Utils.h"
#include "../../Comm/Base/ICommBase.h"

#ifdef WIN32
#include <Windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <memory>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#endif

namespace Nexus
{

typedef struct
{
    sockaddr Info;
    SOCKET ClientSocket;
} TServerSocketClient;
typedef shared_ptr<TServerSocketClient> PServerSocketClient;


class CServerSocketMetaData : public IMetaData
{
public:
    shared_ptr<TServerSocketClient> SelectedClient;
    // How many bytes should we read?
    int ReadSize;

    CServerSocketMetaData()
    {
        ReadSize = -1;
    }

    virtual IMetaData* Clone()
    {
        IMetaData* l_pMetaData = new CServerSocketMetaData;
        ((CServerSocketMetaData*)l_pMetaData)->SelectedClient = this->SelectedClient;
        ((CServerSocketMetaData*)l_pMetaData)->ReadSize= this->ReadSize;

        return l_pMetaData;
    }

    virtual void CopyFrom( IMetaData *a_pSource )
    {
        this->SelectedClient = ((CServerSocketMetaData*)a_pSource)->SelectedClient;
        this->ReadSize = ((CServerSocketMetaData*)a_pSource)->ReadSize;
    }
};

class CServerSocket : public ICommBase
{
private:
#ifdef WIN32
	SOCKET m_hListenSocket;
#else
	// Linux implementation
#endif

	// True if the module is connected
	bool m_bIsConnected;

	std::string m_sIP;
	WORD m_wPort;
	
	DWORD m_iBufferSize;

    HANDLE m_hIsListeningEvent;
    HANDLE m_hIsTerminatedEvent;
	HANDLE m_hClientConnected;

    std::vector<PServerSocketClient> clients;

    void RemoveClient(int a_iClientNumber);
    void RemoveClient(SOCKET a_pSocket);

public:
	CServerSocket();
    ~CServerSocket();


    bool Initialize(WORD a_wPort);

	bool SetReceiveBufferSize(DWORD a_iSize);

	// Connect - Connecting if not already connected
	TCommErr Connect();

	bool IsConnected();

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// Receive - Receives data waiting from a selected client socket, waits for data if no data available
	TCommErr Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

    int GetClientCount();
    PServerSocketClient GetClient(int a_iIndex);


    // Some status functions
	// Returns true is server is currently in listening mode
    bool IsListening();
	// Waits until the server is in listening mode
    bool WaitIsListening();
	// Returns true if server is terminating
    bool IsTerminated();
	// Wait until at least one client is connected
	bool WaitClientConnected();

    bool AcceptClient();
};


}