/*********************************
* Comm/Protocols/CUart.cpp
* Description:
* Uart/RS232 Communication Module
**********************************/
#include <stdio.h> // printf
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "CUart.h"

// Linux Only //
#ifndef WIN32
#include <unistd.h>
#define DEFAULT_PORT "/dev/tty50"
#else
#define DEFAULT_PORT "\\\\.\\COM1"
#endif

#define DEFAULT_RATE ubrBaud38400

// Defines
#define MAX_READ_BUFFER_SIZE 256 // It is only for spare. Normally only a few bytes will return

using namespace Nexus;


/************************************************************
*	CUart()
*
*	Description:
*		Constructor - Initialize Module
*
*	Arguments:
*		None
*
*	Return Value
*		None
*
*************************************************************/
CUart::CUart()
{
#ifdef WIN32
    m_fd = INVALID_HANDLE_VALUE;
#else
    m_fd = -1;
#endif
    m_bIsConnected = false;
    m_eBaudRate = DEFAULT_RATE;
    m_eStopBits = esbOne;
    m_eParity = eupNoParity;
    strcpy(m_strPortName, DEFAULT_PORT);
}

/************************************************************
*	void CUart::SetBaudRate(EUartBaudRate a_eBaudRate)
*
*	Description:
*		Constructor - Initialize Module
*
*	Arguments:
*		None
*
*	Return Value
*		None
*
*************************************************************/
void CUart::SetBaudRate(EUartBaudRate a_eBaudRate)
{
    m_eBaudRate = a_eBaudRate;
}

/************************************************************
*	void CUart::SetPortName(const char *a_strPortName)
*
*	Description:
*
*
*	Arguments:
*		None
*
*	Return Value
*		None
*
*************************************************************/
void CUart::SetPortName(const char* a_strPortName)
{
    if (strlen(a_strPortName) < MAX_PORT_NAME_LEN)
    {
        strcpy(m_strPortName, a_strPortName);
    }
}

/************************************************************
*	void CUart::SetStopBits(EUartStopBits a_eStopBits)
*
*	Description:
*
*
*	Arguments:
*		None
*
*	Return Value
*		None
*
*************************************************************/
void CUart::SetStopBits(EUartStopBits a_eStopBits)
{
    m_eStopBits = a_eStopBits;
}

/************************************************************
*	void CUart::SetParity(EUartParity a_eParity)
*
*	Description:
*
*
*	Arguments:
*		None
*
*	Return Value
*		None
*
*************************************************************/
void CUart::SetParity(EUartParity a_eParity)
{
    m_eParity = a_eParity;
}


/************************************************************
*	TCommErr CUart::Connect()
*
*	Description:
*		Constructor - Initialize Module
*
*	Arguments:
*		None
*
*	Return Value
*		None
*
*************************************************************/
TCommErr CUart::Connect()
{
#ifdef WIN32
    COMMTIMEOUTS l_oTimeouts;
    DCB l_oDCB;
#endif

    // Check that we are connected
    if (m_bIsConnected)
    {
        // Not connected
        dprintf("Uart::Connect> Already connected\n");
        return E_NEXUS_ALREADY_CONNECTED;
    }
    else
    {
        // Open COM port
#ifdef WIN32
        m_fd = CreateFileA(
                   m_strPortName,
                   GENERIC_READ | GENERIC_WRITE,
                   0,
                   NULL,
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_NORMAL,
                   NULL);
        if (m_fd == INVALID_HANDLE_VALUE)

#else
        m_fd = open(m_strPortName, O_RDWR | O_NOCTTY | O_NONBLOCK);

        if (m_fd == -1)
#endif
        {
            // Failed to open COM port
            dprintf("Uart::Connect> Could not open port %s (error %d: %s)\n", m_strPortName, errno, strerror(errno));
            return E_NEXUS_OPEN_FAIL;
        }
        else
        {
#ifdef WIN32
            l_oTimeouts.ReadIntervalTimeout         = 0;
            l_oTimeouts.ReadTotalTimeoutMultiplier  = 0;
            l_oTimeouts.ReadTotalTimeoutConstant    = 100;
            l_oTimeouts.WriteTotalTimeoutMultiplier = 0;
            l_oTimeouts.WriteTotalTimeoutConstant   = 100;

            if (!SetCommTimeouts(m_fd, &l_oTimeouts))
            {
                dprintf("Uart::Connect> Could not set comm timeouts %s (error %d: %s)\n", m_strPortName, errno, strerror(errno));
                return E_NEXUS_OPEN_FAIL;
            }

            GetCommState(m_fd, &l_oDCB);
            l_oDCB.BaudRate = m_eBaudRate;
            l_oDCB.StopBits = m_eStopBits;
            l_oDCB.ByteSize = 8;
            l_oDCB.Parity = m_eParity;
            l_oDCB.fParity = false;
            if (!SetCommState(m_fd, &l_oDCB))
            {
                dprintf("Uart::Connect> Could not set comm state %s (error %d: %s)\n", m_strPortName, errno, strerror(errno));
                return E_NEXUS_OPEN_FAIL;
            }
#else
            // Change baud rate
            struct termios l_tAttribs;
            memset(&l_tAttribs, 0, sizeof(l_tAttribs));

            // Get attribs structure
            if (tcgetattr(m_fd, &l_tAttribs) < 0)
            {
                dprintf("Uart::Connect> tcgetattr faild (error %d: %s)\n", errno, strerror(errno));
                return E_NEXUS_OPEN_FAIL;
            }

            // Set everything ourselves, so there will be no surprises (leftovers from other processes)!
            l_tAttribs.c_iflag = 0;
            l_tAttribs.c_oflag = 0;
            l_tAttribs.c_cflag = CS8|CREAD|CLOCAL; // CREAD, CLOCAL neded for read(), open()
            l_tAttribs.c_lflag = 0;

            // Set Parity
            if (m_eParity != eupNoParity)
            {
                l_tAttribs.c_cflag = l_tAttribs.c_cflag | PARENB;

                switch (m_eParity)
                {
                case eupOddParity:
                    l_tAttribs.c_cflag = l_tAttribs.c_cflag | PARODD;
                    break;
                case eupEvenParity:
                    // Do nothing. Even parity is the default one
                    break;
                case eupNoParity:
#ifdef WIN32
                case eupMarkParity:
                case eupSpaceParity
#endif
                        // Not supported atm
                        break;
                }
            }

            // Set Stop Bits
            if (m_eStopBits == esbTwo)
            {
                // One is the default
                l_tAttribs.c_cflag = l_tAttribs.c_cflag | CSTOPB;
            }

            // One input byte is enough to return from read()
            // Inter-charactar timer off
            l_tAttribs.c_cc[VMIN] = 1;
            l_tAttribs.c_cc[VTIME] = 0;

            // Update out-speed in the info struct
            if (cfsetospeed(&l_tAttribs, m_eBaudRate) < 0)
            {
                dprintf("Uart::Connect> cfsetospeed failed (error %d: %s)\n", errno, strerror(errno));
                return E_NEXUS_OPEN_FAIL;
            }

            // Update in-speed in the info struct
            if (cfsetispeed(&l_tAttribs, m_eBaudRate) < 0)
            {
                dprintf("Uart::Connect> cfsetispeed failed (error %d: %s)\n", errno, strerror(errno));
                return E_NEXUS_OPEN_FAIL;
            }

            if (tcsetattr(m_fd, TCSANOW, &l_tAttribs) < 0)
            {
                dprintf("Uart::Connect> tcsetattr failed (error %d: %s)\n", errno, strerror(errno));
                return E_NEXUS_OPEN_FAIL;
            }

#endif
            m_bIsConnected = true;
            dprintf("Uart::Connect> Connected.\n");
            return E_NEXUS_OK;
        }

    }
}



/************************************************************
*	TCommErr CUart::Disconnect()
*
*	Description:
*		Disconnects if connected
*
*	Arguments:
*		None
*
*	Return Value
*		None
*
*************************************************************/
TCommErr CUart::Disconnect()
{
    // Check if we are connected
#ifdef WIN32
    CloseHandle(m_fd);
    m_fd = INVALID_HANDLE_VALUE;
#else
    close(m_fd);
    m_fd = -1;
#endif
    m_bIsConnected = false;
    dprintf("Uart::Disconnect> Disconnected\n");
    return E_NEXUS_OK;
}

/************************************************************
*	bool CUart::IsConnected()
*
*	Description:
*
*
*	Arguments:
*		None
*
*	Return Value
*		None
*
*************************************************************/
bool CUart::IsConnected()
{
    return m_bIsConnected;
}



/************************************************************
*	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT)
*
*	Description:
*		Sends a given packet to the Uart port (if connected)
*
*	Arguments:
*		CData *a_pData
*			Packet Data
*		IMetaData *a_pMetaData
*			Received packet metadata
*		DWORD a_dwTimeoutMs
*			Timeout in milliseconds
*
*	Return Value
*		TCommErr
*			E_NEXUS_OK
*			E_NOT_CONNECTED
*			E_INVALID_PACKET
*			E_WRITE_ERROR
*
*************************************************************/
TCommErr CUart::Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData /* = NULL */, NX_IN DWORD a_dwTimeoutMs /* = DEFAULT_TIMEOUT */)
{
    // UNREFENCED_PARAMETER(a_pMetaData);
    // UNREFENCED_PARAMETER(a_dwTimeoutMs);

    // Check if we are connected
    if (!m_bIsConnected)
    {
        dprintf("Uart::Send> Not connected\n");
        return E_NEXUS_NOT_CONNECTED;
    }

    // Get size
    DWORD l_dwSize = a_pData->GetSize();
    if (l_dwSize == 0)
        return E_NEXUS_INVALID_PACKET;

    // Write
    int l_intBytesWritten;
#ifdef WIN32
    BOOL l_bResult = WriteFile(m_fd, a_pData->GetString().c_str(), l_dwSize, ((LPDWORD)&l_intBytesWritten), NULL);
    if (!l_bResult)
#else
    l_intBytesWritten = write(m_fd, a_pData->GetString().c_str(), l_dwSize);
    if (l_intBytesWritten < 0)
#endif
    {
        dprintf("Uart::Send> Write error (error %d: %s)\n", errno, strerror(errno));
        return E_NEXUS_WRITE_ERROR;
    }

    return E_NEXUS_OK;
}


/************************************************************
*	TCommErr Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT)
*
*	Description:
*		Receives data waiting from the Uart port, waits for data if no data available
*
*	Arguments:
*		CData *a_pData
*			Received packet data
*		IMetaData *a_pMetaData
*			Received packet metadata
*		DWORD a_dwTimeoutMs
*			Timeout in milliseconds
*
*	Return Value
*		TCommErr
*			E_NEXUS_OK - Great
*			E_NOT_CONNECTED
*			E_READ_ERROR
*			E_TIMEOUT
*			E_ALLOCATION_ERROR - Memory allocation failed
*
*************************************************************/
TCommErr CUart::Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData /* = NULL */, NX_IN DWORD a_dwTimeoutMs /*= DEFAULT_TIMEOUT */)
{
    if (!m_bIsConnected)
    {
        return E_NEXUS_NOT_CONNECTED;
    }

    if (a_pData == NULL)
    {
        dprintf("Uart::Receive> a_pData can't be NULL\n");
        return E_NEXUS_INVALID_PARAMS;
    }

    int iBytesRead = 0;
    DWORD dwMaxSize = a_pData->GetMaxSize();
    DWORD dwStartTime = GetTickCount();
    BYTE *pBuffer = new BYTE[dwMaxSize];

    if (pBuffer == NULL)
    {
        dprintf("Uart::Receive> Error allocating memory for receive buffer at size %d\n", dwMaxSize);
        return E_NEXUS_ALLOCATION_ERROR;
    }

    do
    {
        // Assuming read for uart is non blocking
#ifdef WIN32
        int l_iRes = ReadFile(m_fd, pBuffer, dwMaxSize, (LPDWORD)&iBytesRead, NULL);

        // This is the disconnect signal
        if ((l_iRes == 0) && (GetLastError() == ERROR_BAD_COMMAND))
        {
            m_bIsConnected = false;
            return E_NEXUS_NOT_CONNECTED;
        }

        if (l_iRes == 0)
#else
        iBytesRead = read(m_fd, pBuffer, dwMaxSize);

        // 0 means EOF - Port disconnected
        if (iBytesRead == 0)
        {
            dprintf("Uart::Receive> Port disconnected (errno %d: %s)\n", errno, strerror(errno));
            delete[] pBuffer;

            // Flag disconnected! The rest is up to the user
            m_bIsConnected = false;

            return E_NEXUS_NOT_CONNECTED;
        }

        // Error
        if (iBytesRead < 0 && errno != EAGAIN)
#endif
        {
            dprintf("Uart::Receive> Error in read (errno %d: %s)\n", errno, strerror(errno));
            delete[] pBuffer;
            return E_NEXUS_READ_ERROR;
        }

        // Read something
        if (iBytesRead > 0)
        {
            dprintf("Uart::Receive> Received %d bytes\n", iBytesRead);
            a_pData->SetData(pBuffer, iBytesRead);
            delete[] pBuffer;
            return E_NEXUS_OK;
        }

        // Wait for data (iBytesRead == 0)
        Sleep(10);

    }
    while (a_dwTimeoutMs == INFINITE_TIMEOUT || (GetTickCount() - dwStartTime) < a_dwTimeoutMs);

    delete[] pBuffer;
    return E_NEXUS_TIMEOUT;
}
