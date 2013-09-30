#include "Comm/Physical/CLoopbackComm.h"

using namespace Nexus;


CLoopbackComm::CLoopbackComm(void)
{

}

CLoopbackComm::~CLoopbackComm(void)
{
}

TCommErr CLoopbackComm::Connect()
{
	return E_NEXUS_OK;
}

TCommErr CLoopbackComm::Disconnect()
{
	return E_NEXUS_OK;
}

bool CLoopbackComm::IsConnected()
{
	return true;
}

TCommErr CLoopbackComm::Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData /*= NULL*/, NX_IN DWORD a_dwTimeoutMS /*= DEFAULT_TIMEOUT*/)
{
	if (a_pData && m_pLastData)
		m_pLastData = a_pData->Clone();
	if (a_pMetaData && m_pLastMetadata)
		m_pLastMetadata = a_pMetaData->Clone();

	return E_NEXUS_OK;
}

TCommErr CLoopbackComm::Receive(NX_OUT CData *a_pData, NX_OUT IMetaData *a_pMetaData /*= NULL*/, NX_IN DWORD a_dwTimeoutMs /*= DEFAULT_TIMEOUT*/)
{
	if (a_pData)
		a_pData->CopyFrom(m_pLastData);
	if (a_pMetaData)
		a_pMetaData->CopyFrom(m_pLastMetadata);

	return E_NEXUS_OK;
}

TCommErr CLoopbackComm::SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn /*= NULL*/, NX_OUT IMetaData *a_pMetaDataOut /*= NULL*/, NX_IN DWORD a_dwTimeoutMs /*= INFINITE_TIMEOUT*/)
{
	if (a_pDataIn && a_pDataOut)
		a_pDataOut->CopyFrom(a_pDataIn);
	if (a_pMetaDataIn && m_pLastMetadata)
		a_pMetaDataOut->CopyFrom(a_pMetaDataIn);

	return E_NEXUS_OK;
}