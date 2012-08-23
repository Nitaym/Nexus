/***************************************************************************
* Description:
*	General definitions of useful types and macros
****************************************************************************/

#pragma once

//General Infra definitions

#include "Types.h"
#include "Macros.h"

namespace Nexus
{

// Some more definitions
#define REPORT_ERROR(str)			printf("Error in file %s line %d: %s\n", __FILE__, __LINE__, str)
#define REPORT_ERRORN(str, err)		printf("Error %d in file %s	line %d: %s\n", err, __FILE__, __LINE__, str)
#define REPORT_MSG(str)				printf(str)
#define REPORT_DEBUG				printf

}