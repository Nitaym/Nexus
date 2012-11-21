#pragma once

#include "../../General/Utils.h"
#include "Comm/Base/ICommBase.h"

#include <iostream>

namespace Nexus
{


class CConsoleComm: public ICommBase
{
private:

public:
	CConsoleComm();

	// Connect - Connecting if not already connected
	TCommErr Connect();
	bool IsConnected();


	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);


	TCommErr SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn = NULL, OUT IMetaData *a_pMetaDataOut = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT);
};

}
