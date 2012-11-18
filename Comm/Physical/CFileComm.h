#pragma once

#include "../../General/Utils.h"
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

	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);


	TCommErr SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn = NULL, OUT IMetaData *a_pMetaDataOut = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT);
};

}
