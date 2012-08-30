#pragma once

// Base
#include "Comm/Base/CData.h"
#include "Comm/Base/CDebug.h"
#include "Comm/Base/CommErrors.h"
#include "Comm/Base/ICommBase.h"
#include "Comm/Base/IMetaData.h"

// Physical
#include "Comm/PhysicalCClientSocket.h"
#include "Comm/PhysicalCLoggerComm.h"
#include "Comm/PhysicalCLoopbackComm.h"
#include "Comm/PhysicalCUart.h"

// Protocols
#include "Comm/Protocols/CallbackValues.h"
#include "Comm/Protocols/CAsyncReceiver.h"
#include "Comm/Protocols/CAsyncSender.h"
#include "Comm/Protocols/ICommLayer.h"
#include "Comm/Protocols/TComm.h"

// General
#include "General/General.h"
#include "General/Macros.h"
#include "General/types.h"
#include "General/Utils.h"

