#include "CClientSocket.h"

#define dprintf if (m_pDebug != NULL) m_pDebug->Write

using namespace Nexus;

CClientSocket::CClientSocket()
{
#ifdef WIN32
	m_hSocket = INVALID_SOCKET;
	m_iBufferSize = 4096;
#endif
}

bool CClientSocket::SetConnectionParameters(std::string a_sIP, WORD a_wPort)
{
	m_sIP = a_sIP;
	m_wPort = a_wPort;

	return true;
}

bool CClientSocket::SetReceiveBufferSize(DWORD a_iBufferSize)
{
	m_iBufferSize = a_iBufferSize;
	return true;
}

bool CClientSocket::IsConnected()
{
	return m_bIsConnected;
}

TCommErr CClientSocket::Connect()
{
#ifdef WIN32
	WSADATA l_oWSAData;

	// Initialize Winsock
	int l_iResult = WSAStartup(MAKEWORD(2,2), &l_oWSAData);
	if (l_iResult != 0)
	{
		dprintf("CSocket::CSocket> WSAStartup failed: %d\n", l_iResult);
		return E_NEXUS_FAIL;
	}

	addrinfo *l_pAddrInfo = NULL;
	addrinfo l_oHints;

	ZeroMemory(&l_oHints, sizeof(l_oHints) );
	l_oHints.ai_family = AF_UNSPEC;
	l_oHints.ai_socktype = SOCK_STREAM;
	l_oHints.ai_protocol = IPPROTO_TCP;


	char *l_sPort = new char[20];
	_itoa(m_wPort, l_sPort, 10);
	l_iResult = getaddrinfo(m_sIP.c_str(), l_sPort, &l_oHints, &l_pAddrInfo);
	if (l_iResult != 0)
	{
		WSACleanup();
		dprintf("CSocket::CSocket> getaddrinfo failed: %d\n", l_iResult);
		return E_NEXUS_FAIL;
	}

	m_hSocket = socket(l_pAddrInfo->ai_family, l_pAddrInfo->ai_socktype, l_pAddrInfo->ai_protocol);

	if (m_hSocket == INVALID_SOCKET)
	{
		dprintf("CSocket::CSocket> Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		freeaddrinfo(l_pAddrInfo);
		return E_NEXUS_FAIL;
	}


	// Connect to server.
	l_iResult = connect(m_hSocket, l_pAddrInfo->ai_addr, (int)l_pAddrInfo->ai_addrlen);
	if (l_iResult == SOCKET_ERROR)
	{
		closesocket(m_hSocket);
		m_hSocket= INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple code we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(l_pAddrInfo);

	if (m_hSocket == INVALID_SOCKET)
	{
		dprintf("CClientSocket::Connect> Unable to connect to server!\n");
		WSACleanup();
		return E_NEXUS_FAIL;
	}
#endif
	return E_NEXUS_OK;
}

TCommErr CClientSocket::Disconnect()
{
#ifdef WIN32
	// shutdown the send half of the connection since no more data will be sent
	int l_iResult = shutdown(m_hSocket, SD_SEND);
	if (l_iResult == SOCKET_ERROR) {
		dprintf("CClientSocket::Disconnect> shutdown failed: %d\n", WSAGetLastError());
		closesocket(m_hSocket);
		WSACleanup();
		return E_NEXUS_FAIL;
	}

	// cleanup
	closesocket(m_hSocket);
	WSACleanup();
#endif
	return E_NEXUS_OK;
}

TCommErr CClientSocket::Send(IN CData *a_pData, IN IMetaData *a_pMetaData /* = NULL */, IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
	// Send an initial buffer

	int l_iBufferLength = a_pData->GetSize();
	byte *l_pBuffer = new byte[l_iBufferLength];
	a_pData->GetData(l_pBuffer, 0, l_iBufferLength);

#ifdef WIN32
	int l_iResult = send(m_hSocket, (char*)l_pBuffer, l_iBufferLength, 0);
	if (l_iResult == SOCKET_ERROR)
	{
		printf("CClientSocket::Send> Send failed: %d\n", WSAGetLastError());
		closesocket(m_hSocket);
		WSACleanup();
		SAFE_DELETE_ARRAY(l_pBuffer);

		return E_NEXUS_FAIL;
	}

#endif
	return E_NEXUS_OK;
}

TCommErr CClientSocket::Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData /* = NULL */, IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
#ifdef WIN32
    byte *l_pBuffer = new byte[m_iBufferSize];

	int l_iResult = recv(m_hSocket, (char*)l_pBuffer, m_iBufferSize - 1, 0);
	if (l_iResult > 0)
	{
		a_pData->SetData(l_pBuffer, l_iResult);
		SAFE_DELETE_ARRAY(l_pBuffer);
	}
	else
	{
		SAFE_DELETE_ARRAY(l_pBuffer);

		if (l_iResult == 0)
			dprintf("CClientSocket::Receive> Connection closed\n");
		else
			dprintf("CClientSocket::Receive> recv failed: %d\n", WSAGetLastError());

		return E_NEXUS_FAIL;
	}
#endif
	return E_NEXUS_OK;
}
