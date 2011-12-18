/*****************************************
* Comm/Protocols/CUart.h
* Description:
* Uart/RS232 Communication Module
******************************************/

#pragma once

#ifndef WIN32
// For changing baud rate
#include <termios.h>
#endif
#include "Comm/Base/ICommBase.h"
#include "General/Utils.h"

#define MAX_PORT_NAME_LEN 20

enum EUartBaudRate
{
#ifdef WIN32
	ubrBaud1200 = 1200,
	ubrBaud2400 = 2400,
	ubrBaud4800 = 4800,
	ubrBaud9600 = 9600,
	ubrBaud19200 = 19200,
	ubrBaud38400 = 38400,
#else
	ubrBaud1200 = B1200,
	ubrBaud2400 = B2400,
	ubrBaud4800 = B4800,
	ubrBaud9600 = B9600,
	ubrBaud19200 = B19200,
	ubrBaud38400 = B38400,
#endif
};

class CUart : public ICommBase
{
private:
	// True if the module is connected
	bool m_bIsConnected;
	// Uart file descriptor
	HANDLE m_fd;
	// Uart port name 
	_TCHAR m_strPortName[MAX_PORT_NAME_LEN];
	// Baud rate
	EUartBaudRate m_eBaudRate; 

public:
	CUart();

	// Connect - Connecting if not already connected
	TCommErr Connect();
	void SetBaudRate(EUartBaudRate a_eBaudRate);
	void SetPortName(const char *a_strPortName);

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(IN CData *a_pData, IN IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// Receive - Receives data waiting from the uart port, waits for data if no data available
	//			 a_pData->MaxSize = Read up to size MaxSize bytes
	TCommErr Receive(INOUT CData *a_pData, OUT IMetaData *a_pMetaData = NULL, IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);
};