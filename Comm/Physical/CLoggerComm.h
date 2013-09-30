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
	bool WriteMessage(NX_IN CData *a_pData, std::string a_sPrefix);

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);


	TCommErr SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn = NULL, NX_OUT IMetaData *a_pMetaDataOut = NULL, NX_IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT);
};

}
