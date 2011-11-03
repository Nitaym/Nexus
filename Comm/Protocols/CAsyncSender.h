/*******************************************************************************************
* Comm/Protocols/CAsyncReceiver.h
* Description:
*	Incoming packet buffer - Manages an incoming packets queue, receives packet 
*	automatically by its own thread
*******************************************************************************************/

#pragma once

#define NOMINMAX

#include <queue>
#include "Comm/Protocols/ICommLayer.h"
#include "General/Utils.h"
#include "General/General.h"
#include "CallbackValues.h"

using namespace std; // for the queue

// Struct for the queue elements
struct TPacket
{
	CData *a_pData;
	IMetaData *a_pMetaData;
	DWORD a_dwTimeout;
};

typedef TSendCallback (*typeAsyncSenderCallback)(CData *a_pData, IMetaData *a_pMetaData);

class CAsync