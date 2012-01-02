#include "CLoggerComm.h"

CLoggerComm::CLoggerComm()
{

}

bool CLoggerComm::IsConnected()
{
	return true;
}


TCommErr CLoggerComm::Connect()
{
	m_hFile = CreateFile(L"ICommLog.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
		return E_OPEN_FAIL;

	return E_OK;
}


TCommErr CLoggerComm::Send(IN CData *a_pData, IN IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
	int l_iBufferSize = a_pData->GetSize();
	DWORD l_iBytesWritten = 0;
	byte *Buffer = new byte[l_iBufferSize];
	a_pData->GetData(Buffer, 0, l_iBufferSize);
	BOOL l_bRes = WriteFile(m_hFile, Buffer, l_iBufferSize, &l_iBytesWritten, NULL);
	if (!l_bRes)
		return E_BUSY;

	WriteFile(m_hFile, "\r\n", 2, &l_iBytesWritten, NULL);

	return E_OK;
}


TCommErr CLoggerComm::Disconnect()
{
	return E_BUSY;
}

TCommErr CLoggerComm::Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
	return E_BUSY;
}
