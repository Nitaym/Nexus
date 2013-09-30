#include "CServerSocket.h"

#define dprintf if (m_pDebug != NULL) m_pDebug->Write

using namespace Nexus;


DWORD WINAPI AcceptThreadFunc(LPVOID lpThreadParameter)
{
    CServerSocket* Server = (CServerSocket*) lpThreadParameter;

    while (!Server->IsTerminated())
    {
        // Wait until the socket is listening
        // TODO: This takes a lot of time! Should be removed to increase performance
        Server->WaitIsListening();

        // Now get handling them new clients
        Server->AcceptClient();
    }

    return 0;
}



CServerSocket::CServerSocket()
{
#ifdef WIN32
    m_hListenSocket = INVALID_SOCKET;
    m_iBufferSize = 4096;

    this->m_hIsListeningEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    this->m_hIsTerminatedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
}

CServerSocket::~CServerSocket()
{
    Disconnect();
}

bool Nexus::CServerSocket::Initialize(WORD a_wPort)
{
    WSAData wsaData;
    SOCKADDR_IN address;
    int res;

    // First initialize Winsock
    if (!(WSAStartup(MAKEWORD(2,2), &wsaData) == 0))
        return false;

    m_hListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hListenSocket == INVALID_SOCKET)
        return false;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(a_wPort);

    res = bind(m_hListenSocket, (SOCKADDR*)&address, sizeof(address));
    if (res == SOCKET_ERROR)
    {
        closesocket(m_hListenSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool CServerSocket::SetReceiveBufferSize(DWORD a_iBufferSize)
{
	m_iBufferSize = a_iBufferSize;
	return true;
}

bool CServerSocket::IsConnected()
{
	return m_bIsConnected;
}

TCommErr CServerSocket::Connect()
{
#ifdef WIN32
    // Start listening
    if (SOCKET_ERROR == listen(m_hListenSocket, SOMAXCONN))
        return E_NEXUS_OPEN_FAIL;

    // Create the thread that accepts the clients
    if (CreateThread(NULL, NULL, AcceptThreadFunc, this, 0, NULL) == 0)
        return E_NEXUS_OPEN_FAIL;

    // That's it. The server is now in listening mode
    SetEvent(this->m_hIsListeningEvent);

    return E_NEXUS_OK;

#endif
	return E_NEXUS_NOT_SUPPORTED;
}

TCommErr CServerSocket::Disconnect()
{
#ifdef WIN32
    // Clean everything up
    shutdown(this->m_hListenSocket, SD_BOTH);
    closesocket(this->m_hListenSocket);
    WSACleanup();

    return E_NEXUS_OK;
#endif

    return E_NEXUS_NOT_SUPPORTED;
}

TCommErr CServerSocket::Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData /* = NULL */, NX_IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
    TServerSocketClient* l_pSelectedClient = ((CServerSocketMetaData*)a_pMetaData)->SelectedClient;
    
    int l_iBufferLength = a_pData->GetSize();
	byte *l_pBuffer = new byte[l_iBufferLength];
	a_pData->GetData(l_pBuffer, 0, l_iBufferLength);

#ifdef WIN32
	int l_iResult = send(l_pSelectedClient->ClientSocket, (char*)l_pBuffer, l_iBufferLength, 0);
	if (l_iResult == SOCKET_ERROR)
	{
		dprintf("CServerSocket::Send> Send failed: %d\n", WSAGetLastError());
		closesocket(m_hListenSocket);
		WSACleanup();
		SAFE_DELETE_ARRAY(l_pBuffer);

		return E_NEXUS_FAIL;
	}

#endif
	return E_NEXUS_OK;
}

TCommErr CServerSocket::Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData /* = NULL */, NX_IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
#ifdef WIN32
    CServerSocketMetaData* l_pMetadata = (CServerSocketMetaData*)a_pMetaData;
    TServerSocketClient* l_pSelectedClient = l_pMetadata->SelectedClient;
    int l_iBufferSize = m_iBufferSize;

    // Allow custom buffers
    if (l_pMetadata->ReadSize != -1)
        l_iBufferSize = l_pMetadata->ReadSize;

    byte *l_pBuffer = new byte[l_iBufferSize];

    // Read
    int l_iResult = recv(l_pSelectedClient->ClientSocket, (char*)l_pBuffer, l_iBufferSize, 0);
    if (l_iResult > 0)
    {
        a_pData->SetData(l_pBuffer, l_iResult);
        SAFE_DELETE_ARRAY(l_pBuffer);
    }
    else
    {
        SAFE_DELETE_ARRAY(l_pBuffer);

        if (l_iResult == 0)
            dprintf("CServerSocket::Receive> Connection closed\n");
        else
            dprintf("CServerSocket::Receive> recv failed: %d\n", WSAGetLastError());

        return E_NEXUS_FAIL;
    }

#endif
	return E_NEXUS_OK;
}


int Nexus::CServerSocket::GetClientCount()
{
    return this->clients.size();
}

Nexus::PServerSocketClient Nexus::CServerSocket::GetClient(int a_iIndex)
{
    return this->clients[a_iIndex];
}

bool Nexus::CServerSocket::IsListening()
{
    return (WaitForSingleObject(this->m_hIsListeningEvent, 0) == WAIT_OBJECT_0);
}

bool Nexus::CServerSocket::WaitIsListening()
{
    return (WaitForSingleObject(this->m_hIsListeningEvent, INFINITE) == WAIT_OBJECT_0);
}

bool Nexus::CServerSocket::IsTerminated()
{
    return (WaitForSingleObject(this->m_hIsTerminatedEvent, 0) == WAIT_OBJECT_0);
}


bool Nexus::CServerSocket::AcceptClient()
{
    PServerSocketClient l_pClientStruct;
    int l_iSizeOfInfo = 0;

    l_pClientStruct = new TServerSocketClient();
    l_iSizeOfInfo = sizeof(l_pClientStruct->Info);

    // Wait for the next client
    l_pClientStruct->ClientSocket = accept(this->m_hListenSocket, &l_pClientStruct->Info, &l_iSizeOfInfo);

    if (l_pClientStruct->ClientSocket != SOCKET_ERROR)
    {
        this->clients.push_back(l_pClientStruct);

        return true;
    }

    return false;
}
