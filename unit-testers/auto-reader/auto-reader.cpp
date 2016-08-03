#ifndef WIN32
#include <unistd.h>
#endif

#include "Comm/Protocols/CAsyncReceiver.h"
#include "Comm/Protocols/CAutoConnect.h"
#include "Comm/Physical/CUart.h"
#include "Comm/Physical/CConsoleComm.h"

using namespace Nexus;

Nexus::TReceiveCallback AsyncReceiverReceived(void* a_pParam, Nexus::CData *a_pData, Nexus::IMetaData *a_pMetaData)
{
    a_pData->PrintHex();

    return Nexus::TReceiveCallback_DoNothing;
}

void UartConnectionStateChanged(bool a_bConnectionState)
{
    if (a_bConnectionState)
        printf("Uart Connected!\n");
    else
        printf("Uart Disconnected!\n");
}

int main(int argc, char* argv[])
{
    CUart l_oUart;
    CAsyncReceiver l_oAsyncRec(AsyncReceiverReceived, NULL, NULL);
    CAutoConnect l_oAutoConnect(UartConnectionStateChanged);
    CConsoleComm l_oConsole;
    CDebug l_oDebug(&l_oConsole, NULL);

    l_oAutoConnect.SetUnderlyingComm(&l_oUart);
    l_oAsyncRec.SetUnderlyingComm(&l_oUart);


    l_oUart.SetBaudRate(460800);
#ifdef WIN32
    l_oUart.SetPortName("\\\\.\\COM12");
#else
    l_oUart.SetPortName("/dev/ttyUSB0");
#endif

    l_oUart.SetDebug(&l_oDebug);
    l_oAutoConnect.SetDebug(&l_oDebug);

    if (!(l_oUart.Connect() == E_NEXUS_OK))
        return -1;

    l_oAutoConnect.Connect();
    l_oAsyncRec.Connect();

  
    
    Sleep(600000);

    printf("Ciao!");

    return 0;
}
