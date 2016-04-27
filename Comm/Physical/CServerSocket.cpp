#include "CServerSocket.h"


#ifdef WIN32

#define dprintf if (m_pDebug != NULL) m_pDebug->Write

using namespace Nexus;
using namespace std;

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
    m_iLastClientRead = 0;
    m_hListenSocket = INVALID_SOCKET;
    m_iBufferSize = 4096;

    this->m_hIsListeningEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    this->m_hIsTerminatedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	this->m_hClientConnected = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
}

CServerSocket::~CServerSocket()
{
    Disconnect();

	CloseHandle(m_hClientConnected);
	CloseHandle(m_hIsListeningEvent);
	CloseHandle(m_hIsTerminatedEvent);
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

	m_bIsConnected = true;

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
    PServerSocketClient l_pSelectedClient = NULL;
    CServerSocketMetaData l_pMetaData;
    TCommErr result;
    int l_iLastError;
    
    if (a_pMetaData != NULL)
        l_pSelectedClient = ((CServerSocketMetaData*)a_pMetaData)->SelectedClient;

    // If no client is selected, send to everyone! (recursive)
    if (l_pSelectedClient == NULL)
    {
        for (unsigned int i = 0; i < this->clients.size(); i++)
        {
            l_pMetaData.SelectedClient = GetClient(i);
            result = Send(a_pData, &l_pMetaData,a_dwTimeoutMs);

            if (result != E_NEXUS_OK)
                return result;
        }
    }


    if (l_pSelectedClient == NULL)
        return E_NEXUS_INVALID;

    int l_iBufferLength = a_pData->GetSize();
	byte *l_pBuffer = new byte[l_iBufferLength];
	a_pData->GetData(l_pBuffer, 0, l_iBufferLength);

#ifdef WIN32
	int l_iResult = send(l_pSelectedClient->ClientSocket, (char*)l_pBuffer, l_iBufferLength, 0);
	if (l_iResult == SOCKET_ERROR)
	{
        l_iLastError = WSAGetLastError();
        switch (l_iLastError)
        {
        case WSAECONNRESET:
            RemoveClient(l_pSelectedClient->ClientSocket);
            break;
        case WSANOTINITIALISED:
            RemoveClient(l_pSelectedClient->ClientSocket);
            break;
        }

		dprintf("CServerSocket::Send> Send failed: %d\n", l_iLastError);
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
    shared_ptr<TServerSocketClient> l_pSelectedClient;
    int l_iLastError;

	int l_iBufferSize = m_iBufferSize;

	if (!WaitClientConnected())
		return E_NEXUS_FAIL;

	if (a_pMetaData != NULL)
	{
		l_pSelectedClient = l_pMetadata->SelectedClient;

		// Allow custom buffers
		if (l_pMetadata->ReadSize != -1)
			l_iBufferSize = l_pMetadata->ReadSize;
	}

	// If no client is selected, read from the first one
	if (l_pSelectedClient == NULL)
    {
        // This could be a round robin, but it's not very fair
        // since reads could take a while
//         if (m_iLastClientRead >= GetClientCount())
//             m_iLastClientRead = 0;
// 
//         l_pSelectedClient = GetClient(m_iLastClientRead);
//         m_iLastClientRead++;

        l_pSelectedClient = GetClient(0);
    }

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

        // Do what needs to be done
        l_iLastError = WSAGetLastError();
        switch (l_iLastError)
        {
        // Client should be removed
        case WSAECONNRESET:
            RemoveClient(l_pSelectedClient->ClientSocket);

            return E_NEXUS_PEER_DISCONNECTED;

            break;
        // For some weird reason, this also happens
        case WSANOTINITIALISED:
            RemoveClient(l_pSelectedClient->ClientSocket);
            break;
        default:
            if (l_iResult == 0)
            {
                dprintf("CServerSocket::Receive> Connection closed\n");
                RemoveClient(l_pSelectedClient->ClientSocket);
            }
            else
            {
                dprintf("CServerSocket::Receive> recv failed: %d\n", l_iLastError);
            }

            return E_NEXUS_FAIL;
        }
    }

#endif
	return E_NEXUS_OK;
}


int Nexus::CServerSocket::GetClientCount()
{
    return this->clients.size();
}

PServerSocketClient CServerSocket::GetClient(int a_iIndex)
{  
    //return std::make_shared<TServerSocketClient>(*this->clients[a_iIndex]);
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

bool Nexus::CServerSocket::WaitClientConnected()
{
	return (WaitForSingleObject(this->m_hClientConnected, INFINITE) == WAIT_OBJECT_0);
}

bool Nexus::CServerSocket::AcceptClient()
{
    PServerSocketClient l_pClientStruct(new TServerSocketClient());
    int l_iSizeOfInfo = 0;
    int l_iLastError = 0;

    l_iSizeOfInfo = sizeof(l_pClientStruct->Info);

    // Wait for the next client
    l_pClientStruct->ClientSocket = accept(this->m_hListenSocket, &(l_pClientStruct->Info), &l_iSizeOfInfo);

    if (l_pClientStruct->ClientSocket != SOCKET_ERROR)
    {
        dprintf("CServerSocket::AcceptClient> Client connected\n");

        this->clients.push_back(l_pClientStruct);

		SetEvent(this->m_hClientConnected);

        return true;
    }
    else
    {
        l_iLastError = WSAGetLastError();
//         switch (l_iLastError)
//         {
//         default:
            dprintf("CServerSocket::AcceptClient> Error accepting: %d\n", l_iLastError);
//             break;
//         }
    }

    return false;
}

void Nexus::CServerSocket::RemoveClient(SOCKET a_pSocket)
{
    unsigned int l_iClient = 0;
    for (l_iClient = 0; l_iClient < clients.size(); l_iClient++)
    {
        if (clients[l_iClient]->ClientSocket == a_pSocket)
        {
            break;
        }
    }

    if (l_iClient < clients.size())
        RemoveClient((int)l_iClient);
}

void Nexus::CServerSocket::RemoveClient(int a_iClientNumber)
{
    this->clients.erase(clients.begin() + a_iClientNumber);
        
    if (clients.size() == 0)
        ResetEvent(this->m_hClientConnected);
}


#endif