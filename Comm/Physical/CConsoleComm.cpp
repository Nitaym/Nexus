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

TCommErr CConsoleComm::Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData, NX_IN DWORD a_dwTimeoutMs)
{
    char* l_sBuffer = new char[a_pData->GetSize()];
    cout << a_pData->GetString();
    SAFE_DELETE_ARRAY(l_sBuffer);

	return E_NEXUS_OK;
}


TCommErr CConsoleComm::Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData, NX_IN DWORD a_dwTimeoutMs)
{
    return E_NEXUS_OK;
}

TCommErr CConsoleComm::SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn /*= NULL*/, NX_OUT IMetaData *a_pMetaDataOut /*= NULL*/, NX_IN DWORD a_dwTimeoutMs)
{
	return Send(a_pDataIn, a_pMetaDataIn, a_dwTimeoutMs);
}