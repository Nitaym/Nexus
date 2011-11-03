/*********************************************************************
* Comm/Base/CData.cpp
* Description:
* Defines a single packet. Managing its memory management of a dynamic length
**********************************************************************/

#include <string.h> // strlen
#include <stdio.h> // printf
#include <iostream> // cout
#include <string>
#include "CData.h"
#include "CommErrors.h"

using namespace std;

#define dprintf printf // prints directly to stdout, has no logger.

/*************************************************************
*	CData()
* 
*	Description:
*		Constructor of an empty packet
*
*	Arguments:
*		None
*
*	Return value:
*		None
*
**************************************************************/
CData::CData()
{
	m_oData.clear();
	SetMaxSize(4096);
}


/*****************************************************************
*	CData::CData(const char *a_strText, DWORD a_dwMaxDataSize)
*
*	Description:
*		Constructor of a packet of a given null-ending string
*
*	Arguments:
*		const char *a_strText
*			The given text string
*		DWORD a_dwMaxDataSize
*			Max capacity for data (default 256)
*
*	Return Value:
*		None
*		
*****************************************************************/
CData::CData(const char *a_strText, DWORD a_dwMaxDataSize)
{
	SetMaxSize(a_dwMaxDataSize);

	if (a_strText)
		SetData((BYTE*)a_strText, strlen(a_strText));
}

/*****************************************************************
*	CData::CData(BYTE *a_pBuffer, DWORD a_dwLength, DWORD a_dwMaxDataSize)
*
*	Description:
*		Constructor from an array of bytes
*
*	Arguments:
*		BYTE *a_pBuffer
*			The given array of bytes
*		DWORD a_dwLength
*			The size of the array
*		DWORD a_dwMaxDataSize
*			Max size of data a packet can contain
*
*	Return Value:
*		None
*		
*****************************************************************/
CData::CData(BYTE *a_pBuffer, DWORD a_dwLength, DWORD a_dwMaxDataSize)
{
	SetMaxSize(a_dwMaxDataSize);

	if (a_pBuffer && a_dwLength > 0)
		SetData(a_pBuffer, a_dwLength);
}


/*****************************************************************
*	CData::~CData()
*
*	Description:
*		Destructor - Frees memory
*
*	Arguments:
*		Nonte
*
*	Return Value:
*		None
*		
*****************************************************************/
CData::~CData()
{
	FreeData();
}

/*****************************************************************
*	CData* CData::Clone() const
*
*	Description:
*		Returns a new allocated instance with the same values
*		WARNING: Remember to delete this when not needed anymore!
*
*	Arguments:
*		None
*
*	Return Value:
*		The newly allocated CData instance
*		
*****************************************************************/
CData* CData::Clone() const
{
	CData* retData = new CData();
	retData->SetMaxSize(this->GetMaxSize());

	retData->SetData((byte*)&(m_oData[0]), m_oData.size());
	return retData;
}

/*****************************************************************
*	bool CData::SetData(BYTE* a_pbyNewData, DWORD a_dwNewSize)
*
*	Description:
*		Allocates memory and copies a given data at a given size
*		Notice that it deletes all previous information and frees previous allocated memory
*
*	Arguments:
*		BYTE *a_pbyNewData
*			New data array address
*		DWORD a_dwNewSize
*			Size of the new data array
*
*	Return Value:
*		True if the operation completed. False if couldn't allocate memory
*		
*****************************************************************/
bool CData::SetData(BYTE* a_pbyNewData, DWORD a_dwNewSize)
{
	if (a_dwNewSize > GetMaxSize())
	{
		dprintf(
			"CData::SetData: New data size (%d) is greater than the max size (%d)\n", 
			(int)a_dwNewSize, 
			(int)GetMaxSize());

		return false;
	}

	FreeData();

	// Insert the buffer the vector way
	m_oData.insert(m_oData.end(), a_pbyNewData, a_pbyNewData + a_dwNewSize);

	return true;
}

/*****************************************************************
*	void CData::FreeData()
*
*	Description:
*		Frees the allocated memory for data
*		Clears the vector elements and resets it size to 0.
*
*	Arguments:
*		None
*
*	Return Value:
*		void
*		
*****************************************************************/
void CData::FreeData()
{
	m_oData.clear();
}


/*****************************************************************
*	int CData::GetByte(DWORD a_dwIndex, OUT BYTE* a_pbyByte)
*
*	Description:
*		Returns one byte of a requested index
*
*	Arguments:
*		DWORD a_dwIndex
*			The index of the requested byte
*		OUT BYTE* a_pbyByte
*			The returned byte
*
*	Return Value:
*		TCommErr that states the error value
*		
*****************************************************************/
int CData::GetByte(DWORD a_dwIndex, OUT BYTE* a_pbyByte) const
{
	if (a_dwIndex <= m_oData.size())
	{
		*a_pbyByte = m_oData[a_dwIndex];
		return E_OK;
	}
	
	dprintf("CData::GetByte: Index out of range\n");
	return E_INDEX_OUT_OF_RANGE;
}


/*****************************************************************
*	DWORD CData::GetSize() const
*
*	Description:
*		Returns the size of the buffer, 0 if not allocated of empty
*
*	Arguments:
*		None
*
*	Return Value:
*		DWORD - The size of the buffer
*		
*****************************************************************/
DWORD CData::GetSize() const
{
	return m_oData.size();
}


/*****************************************************************
*	int CData::GetData(OUT BYTE *a_pUserBuffer, DWORD a_dwOffset, DWORD a_dwLength) const
*
*	Description:
*		Copy data to a user's supplied buffer
*
*	Arguments:
*		OUT BYTE* a_pUserBuffer
*			A user supplied buffer
*		DWORD a_dwOffset
*			Offset of inner buffer to start copying the data from
*		DWORD a_dwLength
*			Number of bytes to copy
*
*	Return Value:
*		TCommErr that states the error value
*		
*****************************************************************/
int CData::GetData(OUT BYTE *a_pUserBuffer, DWORD a_dwOffset, DWORD a_dwLength) const
{
	if (a_dwOffset + a_dwLength < m_oData.size())
	{
		return E_INDEX_OUT_OF_RANGE;
	}

	memcpy(a_pUserBuffer, & m_oData[a_dwOffset], a_dwLength);
	return E_OK;
}


/*****************************************************************
*	void CData::SetString(const string &a_strData)
*
*	Description:
*		Sets a string into the buffer
*
*	Arguments:
*		const string &a_strData
*			The string to be set onto the buffer
*
*	Return Value:
*		None
*		
*****************************************************************/
void CData::SetString(const string &a_strData)
{
	SetData((byte*)(a_strData.c_str()), a_strData.length());
}


/*****************************************************************
*	string& CData::GetString()
*
*	Description:
*		Returns a copy of the internal buffer as string
*
*	Arguments:
*		None
*
*	Return Value:
*		The data buffer as a string
*		
*****************************************************************/
string& CData::GetString()
{
	m_sStringData.assign((char*)&(m_oData[0]), m_oData.size());
	return (m_sStringData);
}


/*****************************************************************
*	void CData::Print() const
*
*	Description:
*		Prints the data content
*
*	Arguments:
*		None
*
*	Return Value:
*		None
*		
*****************************************************************/
void CData::Print() const
{
	if (m_oData.size() == 0)
	{
		dprintf("NULL Data\n");
	}
	else
	{
		for (int i = 0; i < m_oData.size(); i++)
		{
			cout << m_oData[i];
		}
		cout << endl;
	}
}


/*****************************************************************
*	void CData::PrintHex() const
*
*	Description:
*		Prints the data as a hex string
*
*	Arguments:
*		None
*
*	Return Value:
*		None
*		
*****************************************************************/
void CData::PrintHex() const
{
	DWORD i, dwSize = m_oData.size();

	dprintf("[%d]", (int)dwSize);
	for (i = 0; i < dwSize; i++)
		dprintf(" %02X", m_oData[i]);

	dprintf("\n");
}