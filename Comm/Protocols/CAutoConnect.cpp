#include "CAutoConnect.h"

using namespace Nexus;



CAutoConnect::CAutoConnect(typeConnectionStateChangedCallback a_pCallback)
{
    m_pConnectionStateChanged = a_pCallback;

    Initialize();

}

CAutoConnect::~CAutoConnect()
{
    Terminating = true;

}

void CAutoConnect::Initialize()
{
    Terminating = false;
    m_iReconnectInterval = 1000;
}



TCommErr CAutoConnect::Connect()
{
	if (m_bIsConnected)
		return E_NEXUS_ALREADY_CONNECTED;

	m_bIsConnected = true;

    m_iConnectThread = NexusCreateThread((typeCallbackFunc)ConnectThread, this);

	return E_NEXUS_OK;
}

TCommErr CAutoConnect::Disconnect()
{
	if (!m_bIsConnected)
		return E_NEXUS_NOT_CONNECTED;

	m_bIsConnected = false;

	return E_NEXUS_OK;
}

TCommErr CAutoConnect::WaitOnDisconnect(int a_iTimeout)
{
#ifdef _WIN32
    DWORD l_iResult = WaitForSingleObject(m_iConnectThread, a_iTimeout);

    switch (l_iResult)
    {
    case WAIT_OBJECT_0:
        return E_NEXUS_OK;
    case WAIT_TIMEOUT:
        return E_NEXUS_TIMEOUT;
    case WAIT_FAILED:
    default:
        return E_NEXUS_FAIL;
    }
#else
    timespec l_stTime;
    clock_gettime(CLOCK_REALTIME, &l_stTime);
    l_stTime.tv_sec += a_iTimeout / 1000;
    l_stTime.tv_nsec += (a_iTimeout % 1000) * 10^6;

    int l_iResult = pthread_timedjoin_np(m_iConnectThread, NULL, const_cast<const timespec*>(&l_stTime));
    switch (l_iResult)
    {
    case 0:
        return E_NEXUS_OK;
    case EBUSY:
        return E_NEXUS_BUSY;
    case ETIMEDOUT:
        return E_NEXUS_TIMEOUT;
    default:
        return E_NEXUS_FAIL;
    }
#endif
}

bool CAutoConnect::IsConnected()
{
	return m_bIsConnected;
}

TCommErr CAutoConnect::Receive(NX_OUT CData *a_pData, NX_OUT IMetaData *a_pMetaData /* = NULL */, NX_IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
    if (m_pUnderlyingComm != NULL)
        return m_pUnderlyingComm->Receive(a_pData, a_pMetaData, a_dwTimeoutMs);

    return E_NEXUS_FAIL;
}


void* CAutoConnect::ConnectThread(void *a_pvParam)
{
    CAutoConnect *l_pThis = (CAutoConnect*)a_pvParam;
    // Used to call the ReportConnectionState.
    static bool l_bStateReported;

    while (!l_pThis->Terminating)
    {
        // This is like waiting on an event, but much more ugly (This will be easily rewritten in C++ 11,
        // but right now using events will cause many cross-platform crap)
        if (!l_pThis->IsConnected())
        {
            // Wait, don't hog all the CPU
            Sleep(1000);
            continue;
        }

        // This part is a bit messy. I'm very sorry
        if (l_pThis->UnderlyingComm() != NULL)
        {
            if (l_pThis->UnderlyingComm()->IsConnected())
            {
                // Wait until it is disconnected
                Sleep(l_pThis->m_iReconnectInterval);
            }
            else
            {
                if (!l_bStateReported)
                {
                    l_pThis->UnderlyingComm()->Disconnect();
                    l_pThis->ReportConnectionState(false);
                    l_bStateReported = true;
                }

                // Reconnect!
                if (l_pThis->UnderlyingComm()->Connect() == E_NEXUS_OK)
                {
                    l_pThis->ReportConnectionState(true);
                    l_bStateReported = false;
                }
                else
                {
                    // Failed, let's wait and try again
                    Sleep(l_pThis->m_iReconnectInterval);
                }
            }
        }
    }

    return NULL;
}

void CAutoConnect::ReportConnectionState(bool a_bState)
{
    if (a_bState)
    {
        dprintf("CAutoConnect> Comm Connected\n");
    }
    else
    {
        dprintf("CAutoConnect> Comm Disconnected\n");
    }

    if (m_pConnectionStateChanged != NULL)
        m_pConnectionStateChanged(a_bState);
}
