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

#define SERVER_PORT 11111
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 5123



typedef enum EComType
{
	COMTYPE_COMPORT,
	COMTYPE_TCPCLIENT,
	COMTYPE_TCPSERVER,
	COMTYPE_MAX,
};


Nexus::TReceiveCallback LeftAsyncReceiverReceived(void* a_pUserParam, CData *a_pData, IMetaData *a_pMetaData)
{
    CAsyncReceiver* l_pRight = (CAsyncReceiver*)a_pUserParam;
    a_pData->PrintHex();
    
    l_pRight->Send(a_pData, NULL, NULL);

    return Nexus::TReceiveCallback_DoNothing;
}
Nexus::TReceiveCallback RightAsyncReceiverReceived(void* a_pUserParam, CData *a_pData, IMetaData *a_pMetaData)
{
    CAsyncReceiver* l_pLeft = (CAsyncReceiver*)a_pUserParam;
    a_pData->PrintHex();

    l_pLeft->Send(a_pData, NULL, NULL);

    return Nexus::TReceiveCallback_DoNothing;
}

void LeftCommStateChanged(bool a_bConnectionState)
{
    if (a_bConnectionState)
        printf("Left Connected!\n");
    else
        printf("Left Disconnected!\n");
}
void RightCommStateChanged(bool a_bConnectionState)
{
    if (a_bConnectionState)
        printf("Right Connected!\n");
    else
        printf("Right Disconnected!\n");
}

EComType GetSelection(std::string name)
{
    int selection = -1;

    while (!((selection > 0) && (selection <= COMTYPE_MAX)))
    {
        printf("Select %s comm type:\n", name.c_str());
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
    EComType leftComtype;
    EComType rightComtype;
    
    CUart l_oLeftUart;
    CServerSocket l_oLeftServer;
    CClientSocket l_oLeftClient;
    CUart l_oRightUart;
    CServerSocket l_oRightServer;
    CClientSocket l_oRightClient;

    ICommBase* l_oLeftComm;
    ICommBase* l_oRightComm;

    CAsyncReceiver l_oLeftAsyncRec(LeftAsyncReceiverReceived, NULL, NULL);
    CAsyncReceiver l_oRightAsyncRec(RightAsyncReceiverReceived, NULL, &l_oLeftAsyncRec);
    l_oLeftAsyncRec.SetUserParam(&l_oRightAsyncRec);
    CAutoConnect l_oLeftAutoConnect(LeftCommStateChanged);
    CAutoConnect l_oRightAutoConnect(RightCommStateChanged);
    CConsoleComm l_oConsole;
    CDebug l_oDebug(&l_oConsole, NULL);


	// Ask the user what to do
    leftComtype = GetSelection("left side");
    rightComtype = GetSelection("right side");

	// Do
    switch (leftComtype)
    {
    case COMTYPE_COMPORT:
        l_oLeftUart.SetBaudRate(19200);
#ifdef WIN32
        l_oLeftUart.SetPortName("\\\\.\\COM1");
#else
        l_oLeftUart.SetPortName("/dev/ttyUSB0");
#endif
        l_oLeftComm = &l_oLeftUart;

        break;
    case COMTYPE_TCPCLIENT:
        l_oLeftClient.SetConnectionParameters(CLIENT_IP, CLIENT_PORT);
        l_oLeftComm = &l_oLeftClient;
        break;
    case COMTYPE_TCPSERVER:
        l_oLeftServer.Initialize(SERVER_PORT);
        l_oLeftComm = &l_oLeftServer;
        break;
    case COMTYPE_MAX:
        exit(0);
        break;
    default:
        exit(-1);
        break;
    }

    switch (rightComtype)
    {
    case COMTYPE_COMPORT:
        l_oRightUart.SetBaudRate(19200);
#ifdef WIN32
        l_oRightUart.SetPortName("\\\\.\\COM1");
#else
        l_oRightUart.SetPortName("/dev/ttyUSB0");
#endif
        l_oRightComm = &l_oRightUart;

        break;
    case COMTYPE_TCPCLIENT:
        l_oRightClient.SetConnectionParameters(CLIENT_IP, CLIENT_PORT);
        l_oRightComm = &l_oRightClient;
        break;
    case COMTYPE_TCPSERVER:
        l_oRightServer.Initialize(SERVER_PORT);
        l_oRightComm = &l_oRightServer;
        break;
    case COMTYPE_MAX:
        exit(0);
        break;
    default:
        exit(-1);
        break;
    }

	// Prepare to communication chain
    l_oLeftAutoConnect.SetUnderlyingComm(l_oLeftComm);
    l_oLeftAutoConnect.SetDebug(&l_oDebug);
    l_oLeftAsyncRec.SetUnderlyingComm(&l_oLeftAutoConnect);
    l_oLeftComm->SetDebug(&l_oDebug);
    l_oRightAutoConnect.SetUnderlyingComm(l_oRightComm);
    l_oRightAutoConnect.SetDebug(&l_oDebug);
    l_oRightAsyncRec.SetUnderlyingComm(&l_oRightAutoConnect);
    l_oRightComm->SetDebug(&l_oDebug);

	// And connect
    if (l_oLeftComm->Connect() == E_NEXUS_OK)
    {
        l_oLeftAutoConnect.Connect();
        l_oLeftAsyncRec.Connect();

        printf("Left Connected\n");
    }
    if (l_oRightComm->Connect() == E_NEXUS_OK)
    {
        l_oRightAutoConnect.Connect();
        l_oRightAsyncRec.Connect();

        printf("Right Connected\n");
    }

    printf("Press any key to exit...");

	getchar();
	getchar();

	printf("Ciao!");

    return 0;
}
