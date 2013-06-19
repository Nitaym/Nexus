/*****************************************
* Comm/Protocols/CSocket.h
* Description:
* TCP/IP Socket Communication Module
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

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#endif

namespace Nexus
{

class CClientSocket : public ICommBase
{
private:
#ifdef WIN32
	SOCKET m_hSocket;
#else
	// Linux implementation
#endif

	// True if the module is connected
	bool m_bIsConnected;

	std::string m_sIP;
	WORD m_wPort;
	
	DWORD m_iBufferSize;

public:
	CClientSocket();
    ~CClientSocket();

	bool SetConnectionParameters(std::string a_sIP, WORD a_wPort);
	bool SetReceiveBufferSize(DWORD a_iSize);

	// Connect - Connecting if not already connected
	TCommErr Connect();

	bool IsConnected();

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// Receive - Receives data waiting from the socket, waits for data if no data available
	//			 a_pData->MaxSize = Read up to size MaxSize bytes
	TCommErr Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);
};
}