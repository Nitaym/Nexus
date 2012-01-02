/************************************************************************
* Comm/Base/CommErrors.h
* Description:
*	Defines common error values for all infra communication modules
*************************************************************************/

#pragma once

// Communication infra error values
enum TCommErr
{
	E_OK					=	0,
	E_OPEN_FAIL				=  -1,
	E_NOT_CONNECTED			=  -2,
	E_ALREADY_CONNECTED		=  -3,
	E_INVALID_PACKET		=  -4,
	E_READ_ERROR			=  -5,
	E_TIMEOUT				=  -6,
	E_INDEX_OUT_OF_RANGE	=  -7,
	E_NOTHING_TO_READ		=  -8,
	E_NOT_INITIALIZED		=  -9,
	E_NO_UNDERLYING_COMM	= -10,
	E_INVALID				= -11,
	E_BUSY					= -12,
	E_NO_ENTRY				= -13,
	E_PERMISSION			= -14,
	E_WRITE_ERROR			= -15,
	E_INVALID_BAUDRATE		= -16,
	E_SET_BAUDRATE_FAILED	= -17,
	E_PIPE					= -18,
	E_MODE					= -19,
	E_INVALID_PARAMS		= -20,
	E_SET_REGISTER_VAL_SIZE	= -21,
	E_SEM_FAILED			= -22,
	E_NOT_SUPPORTED			= -23,
	E_ALLOCATION_ERROR		= -24,
//	E_FAIL					= -25,
	E_NO_CLIENTS			= -26,
	E_GPIO_UNEXPORT_ERR		= -27,
	E_GPIO_REQUEST_ERR		= -28,
	E_GPIO_SETVAL_ERR		= -29,
	E_INVALID_GPIO_NUM		= -30,
	E_INVALID_DIRECTION		= -31,
	E_MORE_DATA_AVAILABLE   = -32,
};