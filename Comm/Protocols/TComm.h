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
	
	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMS = DEFAULT_TIMEOUT);
	TCommErr Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// NOTICE: Only the main comm uses SendReceive, the other comm merely sends and then receives (if the WaitForOtherCommReceive is on)
	TCommErr SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn = NULL, OUT IMetaData *a_pMetaDataOut = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT); 

};

}