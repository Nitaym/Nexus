#ifndef WIN32
#include <unistd.h>
#endif

#include "Comm/Physical/CClientSocket.h"
#include "Comm/Physical/CServerSocket.h"
#include "Comm/Physical/CUart.h"

#include "Comm/Physical/CConsoleComm.h"

#include "General/Utils.h"
#include <sstream>

using namespace Nexus;

int main(int argc, char* argv[])
{
//	CUart l_oUart;

	CClientSocket l_oClient;
//	l_oClient.SetConnectionParameters("tesyst.ddns.net", 7654);
	l_oClient.SetConnectionParameters("127.0.0.1", 8000);

	CConsoleComm l_oConsole;
	l_oConsole.WriteType(false, true);
	CDebug l_oDebug(&l_oConsole, NULL);

	ICommBase* l_oComm = &l_oClient;
	l_oComm->SetDebug(&l_oDebug);

	// Prepare an HTTP get packet (useful for testing TCP connections
	stringstream ss;
	ss << "GET /index.html HTTP/1.1" << endl;
	ss << "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)" << endl;
	ss << "Host: localhost" << endl;
	ss << "Accept-Language: en-us" << endl;
	ss << "Accept-Encoding: gzip, deflate" << endl;
	ss << "Connection: Keep-Alive\r\n\r\n";
	CData data(ss.str().c_str());


	printf("Connecting...  ");
	if (l_oComm->Connect() == E_NEXUS_OK)
	{
		printf("Success\n");

		printf("Writing data...");
		if (l_oComm->Send(&data) == E_NEXUS_OK)
		{
			printf("Success\n");

			printf("Reading...  ");
			if (l_oComm->Receive(&data) == E_NEXUS_OK)
			{
				printf("Success\n");

				printf("Received:\n");
				data.Print();
			}
			else
			{
				printf("Failed\n");
			}
		}
		else
		{
			printf("Failed\n");
		}
	}
	else
	{
		printf("Failed\n");
	}

	getchar();

	printf("Ciao!\n");

	return 0;
}
