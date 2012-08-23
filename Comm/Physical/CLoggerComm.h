#pragma once

#include "../../General/Utils.h"
#include "Comm/Base/ICommBase.h"

#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#else
#endif

namespace Nexus
{


class CLoggerComm : public ICommBase
{
private:
	// True if the module is connected
	bool m_bIsConnected;

	// file descriptor
#ifdef WIN32
	HANDLE m_hFile;
#else
	int m_hFile;
#endif


public:
	CLoggerComm();

	// Connect - Connecting if not already connected
	TCommErr Connect();
	bool IsConnected();

	bool WriteMessage(char* a_sMessage);
	bool WriteMessage(std::string a_sMessage);
	bool WriteMessage(IN CData *a_pData, char* a_sPrefix);

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);


	TCommErr SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn = NULL, OUT IMetaData *a_pMetaDataOut = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT);
};

}