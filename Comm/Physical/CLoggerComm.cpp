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
		return E_NEXUS_OPEN_FAIL;

	return E_NEXUS_OK;
}

TCommErr CLoggerComm::WriteToFile(IN CData *a_pData, char* a_sPrefix)
{
	if (!a_pData)
		return E_NEXUS_FAIL;

	int l_iBufferSize = a_pData->GetSize();
	DWORD l_iBytesWritten = 0;
	byte *Buffer = new byte[l_iBufferSize];
	a_pData->GetData(Buffer, 0, l_iBufferSize);

	// For each byte we have 2 chars and a space
	int l_iStringBufferLen = (l_iBufferSize * 3) + strlen(a_sPrefix);
	char *StringBuffer = new char[l_iStringBufferLen];
	strcpy(StringBuffer, a_sPrefix);
	BinToHexString(Buffer, StringBuffer + strlen(a_sPrefix), l_iBufferSize);

	BOOL l_bRes = WriteFile(m_hFile, StringBuffer, l_iStringBufferLen, &l_iBytesWritten, NULL);
	if (!l_bRes)
		return E_NEXUS_BUSY;

	WriteFile(m_hFile, "\r\n", 2, &l_iBytesWritten, NULL);
	FlushFileBuffers(m_hFile);

	return E_NEXUS_OK;
}

TCommErr CLoggerComm::Send(IN CData *a_pData, IN IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
	return WriteToFile(a_pData, "SEND: ");
}


TCommErr CLoggerComm::Disconnect()
{
	CloseHandle(m_hFile);

	return E_NEXUS_OK;
}

TCommErr CLoggerComm::Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
	return WriteToFile(a_pData, "RECEIVE: ");
}

TCommErr CLoggerComm::SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn /*= NULL*/, OUT IMetaData *a_pMetaDataOut /*= NULL*/, IN DWORD a_dwTimeoutMs)
{
	return Send(a_pDataIn, a_pMetaDataIn, a_dwTimeoutMs);
}
