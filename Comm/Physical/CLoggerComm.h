#pragma once

#include "General/Utils.h"
#include "Comm/Base/ICommBase.h"


class CLoggerComm : public ICommBase
{
private:
	// True if the module is connected
	bool m_bIsConnected;
	// file descriptor
	HANDLE m_hFile;

public:
	CLoggerComm();

	// Connect - Connecting if not already connected
	TCommErr Connect();
	bool IsConnected();

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);
};