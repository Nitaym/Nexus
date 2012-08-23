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

	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMS = DEFAULT_TIMEOUT);
	TCommErr Receive(OUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	TCommErr SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn = NULL, OUT IMetaData *a_pMetaDataOut = NULL, IN DWORD a_dwTimeoutMs = INFINITE_TIMEOUT); 

};

}