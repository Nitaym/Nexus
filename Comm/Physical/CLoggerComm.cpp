#include "CLoggerComm.h"

using namespace Nexus;


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

TCommErr CLoggerComm::Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData, NX_IN DWORD a_dwTimeoutMs)
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

TCommErr CLoggerComm::Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData, NX_IN DWORD a_dwTimeoutMs)
{
	if (!WriteMessage(a_pData, "RECEIVE: "))
		return E_NEXUS_FAIL;

	return E_NEXUS_OK;
}

TCommErr CLoggerComm::SendReceive(NX_IN CData *a_pDataIn, NX_OUT CData *a_pDataOut, NX_IN IMetaData *a_pMetaDataIn /*= NULL*/, NX_OUT IMetaData *a_pMetaDataOut /*= NULL*/, NX_IN DWORD a_dwTimeoutMs)
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
	l_iBytesWritten = write(m_hFile, a_sMessage, strlen(a_sMessage));
	if (l_iBytesWritten < 0)
		return false;
#endif

	return true;
}

bool CLoggerComm::WriteMessage(std::string a_sMessage)
{
	return WriteMessage((char*)a_sMessage.c_str());
}

bool CLoggerComm::WriteMessage(NX_IN CData *a_pData, std::string a_sPrefix)
{
	if (!a_pData)
		return false;

	int l_iBufferSize = a_pData->GetSize();
	byte *Buffer = new byte[l_iBufferSize];
	a_pData->GetData(Buffer, 0, l_iBufferSize);

	// For each byte we have 2 chars and a space
	int l_iStringBufferLen = (l_iBufferSize * 3) + a_sPrefix.length();
	char *StringBuffer = new char[l_iStringBufferLen];
	strcpy(StringBuffer, a_sPrefix.c_str());
	BinToHexString(Buffer, StringBuffer + a_sPrefix.length(), l_iBufferSize);
	// Null Terminate
	StringBuffer[l_iStringBufferLen] = 0;

    SAFE_DELETE(Buffer);
	return WriteMessage(StringBuffer);
}
