#include "CFileComm.h"

using namespace Nexus;


CFileComm::CFileComm() : m_sInputFilename(""), m_sOutputFilename(""), m_iReadBufferSize(4096)
{

}

bool CFileComm::IsConnected()
{
	return true;
}


TCommErr CFileComm::Connect()
{
    m_bIsConnected = true;

    if (m_sInputFilename != "")
    {
        // I use c_str() here because (I'm not sure why) gcc reported an error
        // here when I used std::string
        m_oInputFile.open(m_sInputFilename.c_str());
        m_bIsConnected = m_oInputFile.is_open();
        m_oInputFile.seekg(0, ios::beg);
    }
    if (m_sOutputFilename != "")
    {
        m_oOutputFile.open(m_sOutputFilename.c_str());
        m_bIsConnected = m_oOutputFile.is_open();
    }

    return E_NEXUS_OK;
}

TCommErr CFileComm::Disconnect()
{
    m_oInputFile.close();
    m_oOutputFile.close();

    return E_NEXUS_OK;
}

TCommErr CFileComm::Send(IN CData *a_pData, IN IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
    if (m_oOutputFile.is_open())
    {
        char* l_sBuffer = new char[a_pData->GetSize()];
        a_pData->GetData((BYTE*)l_sBuffer, 0, a_pData->GetSize());
        m_oOutputFile << l_sBuffer;
        SAFE_DELETE_ARRAY(l_sBuffer);
    }

	return E_NEXUS_OK;
}


TCommErr CFileComm::Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData, IN DWORD a_dwTimeoutMs)
{
    if (m_oInputFile.is_open())
    {
        char* l_sBuffer = new char[m_iReadBufferSize];

        if (!m_oInputFile.eof())
        {
            m_oInputFile.read(l_sBuffer, m_iReadBufferSize);
            a_pData->SetData((BYTE*)l_sBuffer, m_iReadBufferSize);
        }

        SAFE_DELETE_ARRAY(l_sBuffer);
    }

    return E_NEXUS_OK;
}

TCommErr CFileComm::SendReceive(IN CData *a_pDataIn, OUT CData *a_pDataOut, IN IMetaData *a_pMetaDataIn /*= NULL*/, OUT IMetaData *a_pMetaDataOut /*= NULL*/, IN DWORD a_dwTimeoutMs)
{
	return Send(a_pDataIn, a_pMetaDataIn, a_dwTimeoutMs);
}

void Nexus::CFileComm::SetReadBufferSize(int a_iBufferSize)
{
    m_iReadBufferSize = a_iBufferSize;
}

void Nexus::CFileComm::SetInputFilename(std::string a_sFilename)
{
    m_sInputFilename = a_sFilename;
}

void Nexus::CFileComm::SetOutputFilename(std::string a_sFilename)
{
    m_sOutputFilename = a_sFilename;
}
