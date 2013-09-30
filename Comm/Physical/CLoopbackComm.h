#include "Comm/Protocols/ICommLayer.h"

namespace Nexus
{


/****************************************************************************
* Comm/Physical/CLoopbackComm.h
* Description:
*	Returns the last sent packet
*****************************************************************************/
class CLoopbackComm : public ICommBase
{
private:
	CData *m_pLastData;
	IMetaData *m_pLastMetadata;

public:
	CLoopbackComm (void);
	~CLoopbackComm (void);

	bool IsConnected();

	TCommErr Connect();
	TCommErr Disconnect();

	TCommErr Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMS = DEFAULT_TIMEOUT);
	TCommErr Receive(NX_OUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn = NULL, NX_OUT IMetaData *a_pMetaDataOut = NULL, NX_IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT); 

};

}