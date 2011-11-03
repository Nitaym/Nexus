/******************************************************************
* Comm/Protocols/CallbackValues.h
* Description:
*	Defines user callback return values for receive and transmit
******************************************************************/

#pragma once

// User receive callback return values
enum TReceiveCallback
{
	TReceiveCallback_PacketPutInQueue = 0,
	TReceiveCallback_DoNothing,
};

// Communication infra error values
enum TSendCallback
{
	TSendCallback_FreePacket = 0,
};