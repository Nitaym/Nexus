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
	if (!WriteMessage(a_pData, "SEND: "))
		return E_NEXUS_FAIL;

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
	if (!WriteMessage(a_pData, "RECEIVE: "))
		return E_NEXUS_FAIL;

	return E_NEXUS_OK;
}

TCommErr CLoggerComm::SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn /*= NULL*/, OUT IMetaData *a_pMetaDataOut /*= NULL*/, IN DWORD a_dwTimeoutMs)
{
	return Send(a_pDataIn, a_pMetaDataIn, a_dwTimeoutMs);
}


bool CLoggerComm::WriteMessage(char* a_sMessage)
{	
	DWORD l_iBytesWritten = 0;

#ifdef WIN32
	BOOL l_bRes = WriteFile(m_hFile, a_sMessage, strlen(a_sMessage), &l_iBytesWritten, NULL);
	if (!l_bRes)
		return false;

	WriteFile(m_hFile, "\r\n", 2, &l_iBytesWritten, NULL);
	FlushFileBuffers(m_hFile);

#else
	l_iBytesWritten = write(m_hFile, StringBuffer, l_iStringBufferLen);
	if (l_iBytesWritten < 0)
		return false;
#endif	

	return true;
}

bool CLoggerComm::WriteMessage(std::string a_sMessage)
{
	return WriteMessage(a_sMessage.c_str());
}

bool CLoggerComm::WriteMessage(IN CData *a_pData, char* a_sPrefix)
{
	if (!a_pData)
		return false;

	int l_iBufferSize = a_pData->GetSize();
	DWORD l_iBytesWritten = 0;
	byte *Buffer = new byte[l_iBufferSize];
	a_pData->GetData(Buffer, 0, l_iBufferSize);

	// For each byte we have 2 chars and a space
	int l_iStringBufferLen = (l_iBufferSize * 3) + strlen(a_sPrefix);
	char *StringBuffer = new char[l_iStringBufferLen];
	strcpy(StringBuffer, a_sPrefix);
	BinToHexString(Buffer, StringBuffer + strlen(a_sPrefix), l_iBufferSize);
	// Null Terminate
	StringBuffer[l_iStringBufferLen] = 0;

	return WriteMessage(StringBuffer);
}