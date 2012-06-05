#include "TComm.h"


CTComm::CTComm(void)
{

}


CTComm::~CTComm(void)
{
}

TCommErr CTComm::Connect()
{
	return E_NEXUS_OK;
}

TCommErr CTComm::Disconnect()
{
	return E_NEXUS_OK;
}

TCommErr CTComm::Send(IN CData *a_pData, IN IMetaData *a_pMetaData /*= NULL*/, IN DWORD a_dwTimeoutMS /*= DEFAULT_TIMEOUT*/)
{
	if (m_pOtherUnderlyingComm != NULL)
		m_pOtherUnderlyingComm->Send(a_pData, a_pMetaData, a_dwTimeoutMS);

	return m_pUnderlyingComm->Send(a_pData, a_pMetaData, a_dwTimeoutMS);
}

TCommErr CTComm::Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData /*= NULL*/, IN DWORD a_dwTimeoutMs /*= DEFAULT_TIMEOUT*/)
{
	if (m_pOtherUnderlyingComm != NULL)
		m_pOtherUnderlyingComm->Receive(a_pData, a_pMetaData, a_dwTimeoutMs);

	return m_pUnderlyingComm->Receive(a_pData, a_pMetaData, a_dwTimeoutMs);
}

TCommErr CTComm::SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn /*= NULL*/, OUT IMetaData *a_pMetaDataOut /*= NULL*/, IN DWORD a_dwTimeoutMs /*= INFINITE_TIMEOUT*/)
{
	if (m_pOtherUnderlyingComm != NULL)
		m_pOtherUnderlyingComm->SendReceive(a_pDataIn, a_pDataOut, a_pMetaDataIn, a_pMetaDataOut, a_dwTimeoutMs);

	return m_pUnderlyingComm->SendReceive(a_pDataIn, a_pDataOut, a_pMetaDataIn, a_pMetaDataOut, a_dwTimeoutMs);
}

void CTComm::SetOtherUnderlyingComm(ICommBase *a_pNewUnderlyingComm)
{
	m_pOtherUnderlyingComm = a_pNewUnderlyingComm;
}
