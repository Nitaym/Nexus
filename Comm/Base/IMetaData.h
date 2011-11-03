/***************************************************************************
* Comm/Base/IMetaData.h
* Description:
*	Defines common interface for all communication metadata information
***************************************************************************/

#pragma once

class IMetaData
{
public:
	// Clone - Returned new allocated instance with the same values (Remember to free the cloned copy)
	virtual IMetaData* Clone() = 0;

	// Copy - Copies known fields from source to this instance (by dynamic casting). 
	virtual void CopyFrom(IMetaData *a_pSource) = 0;
};