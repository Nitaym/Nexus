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
#include "General/Utils.h"
#include "Comm/Base/ICommBase.h"

#define MAX_PORT_NAME_LEN 20

namespace Nexus
{

enum EUartBaudRate
{
#ifdef WIN32
	ubrBaud1200 = 1200,
	ubrBaud2400 = 2400,
	ubrBaud4800 = 4800,
	ubrBaud9600 = 9600,
	ubrBaud19200 = 19200,
	ubrBaud38400 = 38400,
	ubrBaud57600 = 57600,
	ubrBaud115200 = 115200,
	ubrBaud256000 = 256000,
#else
	ubrBaud1200 = B1200,
	ubrBaud2400 = B2400,
	ubrBaud4800 = B4800,
	ubrBaud9600 = B9600,
	ubrBaud19200 = B19200,
	ubrBaud38400 = B38400,
	ubrBaud57600 = B57600,
	ubrBaud115200 = B115200,
	ubrBaud230400 = B230400,
	ubrBaud460800 = B460800,
#endif
};

enum EUartStopBits
{
	esbOne = 0,
#ifdef WIN32
	esbOne5 = 1,
#endif
	esbTwo = 2,
};

enum EUartParity
{
	eupNoParity          = 0,
	eupOddParity         = 1,
	eupEvenParity        = 2,
#ifdef WIN32
	eupMarkParity        = 3,
	eupSpaceParity       = 4,
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
	char m_strPortName[MAX_PORT_NAME_LEN];
	// Baud rate
	unsigned int m_iBaudRate; 

	// Parity & Stop bits
	EUartParity m_eParity;
	EUartStopBits m_eStopBits;

public:
	CUart();

	// Connect - Connecting if not already connected
	TCommErr Connect();
// 	void SetBaudRate(EUartBaudRate a_eBaudRate);
	void SetBaudRate(unsigned int a_eBaudRate);
	void SetPortName(const char* a_strPortName);
	void SetStopBits(EUartStopBits a_eStopBits);
	void SetParity(EUartParity a_eParity);

	bool IsConnected();

	// Disconnect - Disconnects if not already disconnected
	TCommErr Disconnect();

	TCommErr Send(NX_IN CData *a_pData, NX_IN IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);

	// Receive - Receives data waiting from the uart port, waits for data if no data available
	//			 a_pData->MaxSize = Read up to size MaxSize bytes
	TCommErr Receive(NX_INOUT CData *a_pData, NX_OUT IMetaData *a_pMetaData = NULL, NX_IN DWORD a_dwTimeoutMs = DEFAULT_TIMEOUT);
};


}