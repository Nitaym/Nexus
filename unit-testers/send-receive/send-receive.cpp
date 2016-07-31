#ifndef WIN32
#include <unistd.h>
#endif

#include "Comm/Physical/CClientSocket.h"
#include "Comm/Physical/CServerSocket.h"
#include "Comm/Physical/CUart.h"

#include "Comm/Protocols/CAsyncReceiver.h"
#include "Comm/Protocols/CAutoConnect.h"
#include "Comm/Physical/CConsoleComm.h"

using namespace Nexus;

#define SERVER_PORT 6543
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 5123



typedef enum EComType
{
	COMTYPE_COMPORT,
	COMTYPE_TCPCLIENT,
	COMTYPE_TCPSERVER,
	COMTYPE_MAX,
};


Nexus::TReceiveCallback AsyncReceiverReceived(void* a_pUserParam, CData *a_pData, IMetaData *a_pMetaData)
{
    a_pData->PrintHex();

    return Nexus::TReceiveCallback_DoNothing;
}

void CommStateChanged(bool a_bConnectionState)
{
    if (a_bConnectionState)
        printf("Connected!\n");
    else
        printf("Disconnected!\n");
}

EComType PrintMenu()
{
	int selection = -1;

	while (!((selection > 0) && (selection <= COMTYPE_MAX)))
	{
		printf("Select comm type:\n");
		printf("1. Comport (COM1, 19200, No parity, 1 Stop bit\n");
		printf("2. TCP Client (connects to %s:%d)\n", CLIENT_IP, CLIENT_PORT);
		printf("3. TCP Server (Listens on port %d)\n", SERVER_PORT);
		printf("4. Exit\n");
		cin >> selection;

		if (selection == 4)
			return COMTYPE_MAX;
	}

	return (EComType)(selection - 1);
}

int main(int argc, char* argv[])
{
	EComType comtype;
    
	CUart l_oUart;
	CServerSocket l_oServer;
	CClientSocket l_oClient;

	ICommBase* l_oComm;

    CAsyncReceiver l_oAsyncRec(AsyncReceiverReceived, NULL, NULL);
    CAutoConnect l_oAutoConnect(CommStateChanged);
    CConsoleComm l_oConsole;
    CDebug l_oDebug(&l_oConsole, NULL);


	// Ask the user what to do
	comtype = PrintMenu();

	// Do
	switch (comtype)
	{
	case COMTYPE_COMPORT:
		l_oUart.SetBaudRate(Nexus::ubrBaud19200);
#ifdef WIN32
		l_oUart.SetPortName("\\\\.\\COM1");
#else
		l_oUart.SetPortName("/dev/ttyUSB0");
#endif
		l_oComm = &l_oUart;

		break;
	case COMTYPE_TCPCLIENT:
		l_oClient.SetConnectionParameters(CLIENT_IP, CLIENT_PORT);
		l_oComm = &l_oClient;
		break;
	case COMTYPE_TCPSERVER:
		l_oServer.Initialize(SERVER_PORT);
		l_oComm = &l_oServer;
		break;
	case COMTYPE_MAX:
		exit(0);
		break;
	default:
		exit(-1);
		break;
	}

	// Prepare to communication chain
	l_oAutoConnect.SetUnderlyingComm(l_oComm);
	l_oAutoConnect.SetDebug(&l_oDebug);
    l_oAsyncRec.SetUnderlyingComm(&l_oAutoConnect);
    l_oComm->SetDebug(&l_oDebug);

	// And connect
    if (l_oComm->Connect() == E_NEXUS_OK)
	{
		l_oAutoConnect.Connect();
		l_oAsyncRec.Connect();

		// 	printf("Everything you write is being sent\n");
		printf("All that is received is printed\n");
	}


	getchar();
	getchar();

	printf("Ciao!");

    return 0;
}
