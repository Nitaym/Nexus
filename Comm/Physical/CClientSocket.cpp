#include "CClientSocket.h"
#include <sstream>
#include <string.h>

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
#endif


#ifdef WIN32
#define SOCKET_CLEANUP WSACleanup
#define SOCKET_TEST(socket) (socket != INVALID_SOCKET)
#define SOCKET_GETLASTERROR WSAGetLastError()
#define SOCKET_TEST_RESULT(res) (res != SOCKET_ERROR)

#define SOCKET_WRITE(socket, buffer, bufflen) 	send(socket, (char*)buffer, bufflen, 0)
#define SOCKET_READ(socket, buffer, bufflen) 	recv(m_hSocket, (char*)l_pBuffer, m_iBufferSize, 0)

#else
#define SOCKET_CLEANUP
#define SOCKET_TEST(socket) (socket >= 0)
#define SOCKET_GETLASTERROR errno
#define SOCKET_TEST_RESULT(res) (res >= 0)

#define SOCKET_WRITE(socket, buffer, bufflen) 	write(socket, (char*)buffer, bufflen);
#define SOCKET_READ(socket, buffer, bufflen) 	read(socket, (char*)buffer, bufflen);

#endif

using namespace Nexus;

CClientSocket::CClientSocket()
{
#ifdef WIN32
	m_hSocket = INVALID_SOCKET;
#endif
	m_iBufferSize = 4096;
}

CClientSocket::~CClientSocket()
{
    Disconnect();
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
	int l_iResult;

#ifdef WIN32
	WSADATA l_oWSAData;

	// Initialize Winsock
	l_iResult = WSAStartup(MAKEWORD(2,2), &l_oWSAData);
	if (l_iResult != 0)
	{
		dprintf("CSocket::CSocket> WSAStartup failed: %d\n", l_iResult);
		return E_NEXUS_FAIL;
	}
#endif

	addrinfo *l_pAddrInfo = NULL;
	addrinfo l_oHints;

	memset(&l_oHints, 0, sizeof(l_oHints) );
	l_oHints.ai_family = AF_UNSPEC;
	l_oHints.ai_socktype = SOCK_STREAM;
	l_oHints.ai_protocol = IPPROTO_TCP;


	stringstream ss;
	ss << m_wPort;
	l_iResult = getaddrinfo(m_sIP.c_str(), ss.str().c_str(), &l_oHints, &l_pAddrInfo);
	if (l_iResult != 0)
	{
		SOCKET_CLEANUP;
		dprintf("CSocket::CSocket> getaddrinfo failed: %d\n", l_iResult);
		return E_NEXUS_FAIL;
	}

#ifdef WIN32
	m_hSocket = socket(l_pAddrInfo->ai_family, l_pAddrInfo->ai_socktype, l_pAddrInfo->ai_protocol);
#else
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
#endif
	if (!SOCKET_TEST(m_hSocket))
	{
		dprintf("CSocket::CSocket> Error at socket(): %ld\n", SOCKET_GETLASTERROR);
		SOCKET_CLEANUP;
		freeaddrinfo(l_pAddrInfo);
		return E_NEXUS_FAIL;
	}


	// Connect to server.
	l_iResult = connect(m_hSocket, l_pAddrInfo->ai_addr, (int)l_pAddrInfo->ai_addrlen);
	if (!SOCKET_TEST_RESULT(l_iResult))
	{
#ifdef WIN32
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
#else
		close(m_hSocket);
		m_hSocket = 0;
#endif
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple code we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(l_pAddrInfo);

	if (!SOCKET_TEST(m_hSocket))
	{
		dprintf("CClientSocket::Connect> Unable to connect to server!\n");
		SOCKET_CLEANUP;
		return E_NEXUS_FAIL;
	}

    m_bIsConnected = true;
	return E_NEXUS_OK;
}

TCommErr CClientSocket::Disconnect()
{
    if (IsConnected())
    {
        m_bIsConnected = false;
		int l_iResult;

        // shutdown the send half of the connection since no more data will be sent
#ifdef WIN32
        l_iResult = shutdown(m_hSocket, SD_BOTH);
		if (l_iResult == SOCKET_ERROR) {
			dprintf("CClientSocket::Disconnect> shutdown failed: %d\n", SOCKET_GETLASTERROR);
			closesocket(m_hSocket);
			WSACleanup();
			return E_NEXUS_FAIL;
		}

		// cleanup
		closesocket(m_hSocket);
		WSACleanup();
#else
		l_iResult = shutdown(m_hSocket, SHUT_RDWR);
		if (l_iResult != 0) {
			l_iResult = close(m_hSocket);
			return E_NEXUS_FAIL;
		}
		close(m_hSocket);
#endif
    }

	return E_NEXUS_OK;
}

TCommErr CClientSocket::Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData /* = NULL */, NX_IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
	if (!m_bIsConnected)
		return E_NEXUS_NOT_CONNECTED;

	int l_iBufferLength = a_pData->GetSize();
	byte *l_pBuffer = new byte[l_iBufferLength];
	a_pData->GetData(l_pBuffer, 0, l_iBufferLength);

	int l_iResult = SOCKET_WRITE(m_hSocket, (char*)l_pBuffer, l_iBufferLength);
	if (!SOCKET_TEST_RESULT(l_iResult))
	{
		dprintf("CClientSocket::Send> Send failed: %d\n", SOCKET_GETLASTERROR);
#ifdef WIN32
		closesocket(m_hSocket);
		WSACleanup();
#else
		close(m_hSocket);
#endif
		SAFE_DELETE_ARRAY(l_pBuffer);
		return E_NEXUS_FAIL;
	}

	SAFE_DELETE_ARRAY(l_pBuffer);
	return E_NEXUS_OK;
}

TCommErr CClientSocket::Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData /* = NULL */, NX_IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
    byte *l_pBuffer = new byte[m_iBufferSize];

    // Read
    int l_iResult = SOCKET_READ(m_hSocket, (char*)l_pBuffer, m_iBufferSize);
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
            dprintf("CClientSocket::Receive> recv failed: %d\n", SOCKET_GETLASTERROR);

		Disconnect();

        return E_NEXUS_FAIL;
    }

	return E_NEXUS_OK;
}

TCommErr CClientSocket::SendReceive(NX_IN Nexus::CData *a_pDataIn, NX_OUT Nexus::CData *a_pDataOut, NX_IN Nexus::IMetaData *a_pMetaDataIn /*= NULL*/, NX_OUT Nexus::IMetaData *a_pMetaDataOut /*= NULL*/, NX_IN DWORD a_dwTimeoutMs /*= INFINITE_TIMEOUT*/)
{
    if (Send(a_pDataIn, a_pMetaDataIn, a_dwTimeoutMs) == E_NEXUS_OK)
    {
        if (Receive(a_pDataOut, a_pMetaDataOut) == E_NEXUS_OK)
        {
            return E_NEXUS_OK;
        }
    }

    return E_NEXUS_FAIL;
}
