#pragma once

#include "General/Utils.h"
#include "Comm/Base/ICommBase.h"

#include <fstream>
#include <iostream>

namespace Nexus
{


class CFileComm : public ICommBase
{
private:
	// True if the module is connected
	bool m_bIsConnected;

    std::string m_sInputFilename;
    std::string m_sOutputFilename;

    std::ifstream m_oInputFile;
    std::ofstream m_oOutputFile;

    int m_iReadBufferSize;

public:
	CFileComm();

	// Connect - Connecting if not already connected
	TCommErr Connect();
	bool IsConnected();

    void SetInputFilename(std::string a_sFilename);
    void SetOutputFilename(std::string a_sFilename);

    void SetReadBufferSize(int a_iBufferSize);

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);


	TCommErr SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn = NULL, NX_OUT IMetaData *a_pMetaDataOut = NULL, NX_IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT);
};

}
