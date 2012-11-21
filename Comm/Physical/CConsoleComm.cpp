#include "CConsoleComm.h"

using namespace Nexus;


CConsoleComm::CConsoleComm()
{

}

bool CConsoleComm::IsConnected()
{
	return true;
}


TCommErr CConsoleComm::Connect()
{
    return E_NEXUS_OK;
}

TCommErr CConsoleComm::Disconnect()
{
    return E_NEXUS_OK;
}

TCommErr CConsoleComm::Send(IN CData *a_pData, IN IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
    char* l_sBuffer = new char[a_pData->GetSize()];
    cout << a_pData->GetString();
    SAFE_DELETE_ARRAY(l_sBuffer);

	return E_NEXUS_OK;
}


TCommErr CConsoleComm::Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
    return E_NEXUS_OK;
}

TCommErr CConsoleComm::SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn /*= NULL*/, OUT IMetaData *a_pMetaDataOut /*= NULL*/, IN DWORD a_dwTimeoutMs)
{
	return Send(a_pDataIn, a_pMetaDataIn, a_dwTimeoutMs);
}