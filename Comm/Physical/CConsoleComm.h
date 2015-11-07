#pragma once

#include "../../General/Utils.h"
#include "Comm/Base/ICommBase.h"

#include <iostream>

namespace Nexus
{


class CConsoleComm: public ICommBase
{
private:
	bool writeHex;
	bool addNewline;
public:
	CConsoleComm();

	// Connect - Connecting if not already connected
	TCommErr Connect();
	bool IsConnected();

	void WriteType(bool hex, bool newline);

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);


	TCommErr SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn = NULL, NX_OUT IMetaData *a_pMetaDataOut = NULL, NX_IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT);
};

}
