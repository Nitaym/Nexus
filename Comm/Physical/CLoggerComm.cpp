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
#ifdef WIN32
	m_hFile = CreateFile(L"ICommLog.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
		return E_NEXUS_OPEN_FAIL;

#else
	m_hFile = open("ICommLog.txt", O_WRONLY);
	if (m_hFile == -1)
		return E_NEXUS_OPEN_FAIL;
#endif

	return E_NEXUS_OK;
}


TCommErr CLoggerComm::Send(IN CData *a_pData, IN IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
	int l_iBufferSize = a_pData->GetSize();
	DWORD l_iBytesWritten = 0;
	byte *Buffer = new byte[l_iBufferSize];
	a_pData->GetData(Buffer, 0, l_iBufferSize);

	// For each byte we have 2 chars and a space
	char *StringBuffer = new char[l_iBufferSize * 3];
	BinToHexString(Buffer, StringBuffer, l_iBufferSize);

#ifdef WIN32
	BOOL l_bRes = WriteFile(m_hFile, StringBuffer, l_iBufferSize * 3, &l_iBytesWritten, NULL);
	if (!l_bRes)
		return E_NEXUS_BUSY;

	WriteFile(m_hFile, "\r\n", 2, &l_iBytesWritten, NULL);
	FlushFileBuffers(m_hFile);

#else
	l_iBytesWritten = write(m_hFile, StringBuffer, l_iBufferSize * 3);
	if (l_iBytesWritten < 0)
		return E_NEXUS_BUSY;
#endif

	return E_NEXUS_OK;
}


TCommErr CLoggerComm::Disconnect()
{
#ifdef WIN32
	CloseHandle(m_hFile);
#else
	close(m_hFile);
#endif

	return E_NEXUS_OK;
}

TCommErr CLoggerComm::Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
	return E_NEXUS_BUSY;
}

TCommErr CLoggerComm::SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn /*= NULL*/, OUT IMetaData *a_pMetaDataOut /*= NULL*/, IN DWORD a_dwTimeoutMs)
{
	return Send(a_pDataIn, a_pMetaDataIn, a_dwTimeoutMs);
}
