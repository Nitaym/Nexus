#include "Comm/Protocols/ICommLayer.h"


class CTComm : public ICommLayer
{
private:
	ICommBase *m_pOtherUnderlyingComm;

public:
	CTComm(void);
	~CTComm(void);

	TCommErr Connect();
	TCommErr Disconnect();

	void SetOtherUnderlyingComm(ICommBase *a_pNewUnderlyingComm);
	
	bool Register();

	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMS = DEFAULT_TIMEOUT);
	TCommErr Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn = NULL, OUT IMetaData *a_pMetaDataOut = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT); 

};

