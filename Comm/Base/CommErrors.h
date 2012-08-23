/************************************************************************
* Comm/Base/CommErrors.h
* Description:
*	Defines common error values for all infra communication modules
*************************************************************************/

#pragma once

namespace Nexus
{

// Communication infra error values
enum TCommErr
{
	E_NEXUS_OK						=	0,
	E_NEXUS_OPEN_FAIL				=  -1,
	E_NEXUS_NOT_CONNECTED			=  -2,
	E_NEXUS_ALREADY_CONNECTED		=  -3,
	E_NEXUS_INVALID_PACKET			=  -4,
	E_NEXUS_READ_ERROR				=  -5,
	E_NEXUS_TIMEOUT					=  -6,
	E_NEXUS_INDEX_OUT_OF_RANGE		=  -7,
	E_NEXUS_NOTHING_TO_READ			=  -8,
	E_NEXUS_NOT_INITIALIZED			=  -9,
	E_NEXUS_NO_UNDERLYING_COMM		= -10,
	E_NEXUS_INVALID					= -11,
	E_NEXUS_BUSY					= -12,
	E_NEXUS_NO_ENTRY				= -13,
	E_NEXUS_PERMISSION				= -14,
	E_NEXUS_WRITE_ERROR				= -15,
	E_NEXUS_INVALID_BAUDRATE		= -16,
	E_NEXUS_SET_BAUDRATE_FAILED		= -17,
	E_NEXUS_PIPE					= -18,
	E_NEXUS_MODE					= -19,
	E_NEXUS_INVALID_PARAMS			= -20,
	E_NEXUS_SET_REGISTER_VAL_SIZE	= -21,
	E_NEXUS_SEM_FAILED				= -22,
	E_NEXUS_NOT_SUPPORTED			= -23,
	E_NEXUS_ALLOCATION_ERROR		= -24,
	E_NEXUS_FAIL					= -25,
	E_NEXUS_NO_CLIENTS				= -26,
	E_NEXUS_GPIO_UNEXPORT_ERR		= -27,
	E_NEXUS_GPIO_REQUEST_ERR		= -28,
	E_NEXUS_GPIO_SETVAL_ERR			= -29,
	E_NEXUS_INVALID_GPIO_NUM		= -30,
	E_NEXUS_INVALID_DIRECTION		= -31,
	E_NEXUS_MORE_DATA_AVAILABLE		= -32,
};

}