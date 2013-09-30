#include "Comm/Protocols/ICommLayer.h"

namespace Nexus
{

class CTComm : public ICommLayer
{
private:
	ICommBase *m_pOtherUnderlyingComm;

public:
	// States whether TComm should call to the other comm receive function while receiving or not.
	bool OtherCommReceive;

	CTComm(void);
	~CTComm(void);

	TCommErr Connect();
	TCommErr Disconnect();

	bool IsConnected();

	void SetOtherUnderlyingComm(ICommBase *a_pNewUnderlyingComm);
	
	TCommErr Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMS = DEFAULT_TIMEOUT);
	TCommErr Receive(NX_OUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// NOTICE: Only the main comm uses SendReceive, the other comm merely sends and then receives (if the WaitForOtherCommReceive is on)
	TCommErr SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn = NULL, NX_OUT IMetaData *a_pMetaDataOut = NULL, NX_IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT); 

};

}