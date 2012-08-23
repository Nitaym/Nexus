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
using namespace Nexus;

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
	m_bSwitchEndianity = false;
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
	m_bSwitchEndianity = false;

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
	m_bSwitchEndianity = false;

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
*	void CData::CopyFrom(CData* a_oData)
*
*	Description:
*		CopyFrom - Copies the data from another CData to this one
*
*	Arguments:
*
*	Return Value:
*		
*****************************************************************/
void CData::CopyFrom(CData* a_oData)
{
	int l_iSize = a_oData->GetSize();
	byte *l_abBuffer = new byte[l_iSize];

	a_oData->GetData(l_abBuffer, 0, l_iSize);
	SetData(l_abBuffer, l_iSize);
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

	// This obviously clears all the data
	FreeData();

	// Insert the buffer the vector way
	m_oData.insert(m_oData.end(), a_pbyNewData, a_pbyNewData + a_dwNewSize);

	return true;
}


/*****************************************************************
*	bool CData::Append(BYTE *a_pUserBuffer, int a_dwSize);
*
*	Description:
*		Appends a buffer in the end of the data
*
*	Arguments:
*
*	Return Value:
*		None
*		
*****************************************************************/
bool CData::Append(BYTE *a_pUserBuffer, int a_dwSize)
{
	if (a_dwSize + GetSize() > GetMaxSize())
	{
		dprintf(
			"CData::Append: New data size (%d) is greater than the max size (%d)\n", 
			(int)a_dwSize + GetSize(), 
			(int)GetMaxSize());

		return false;
	}

	// Insert the buffer the vector way
	m_oData.insert(m_oData.end(), a_pUserBuffer, a_pUserBuffer + a_dwSize);

	return true;
}


bool CData::Append(byte a_byByte)
{
	return Append((byte*)&a_byByte, 1);
}

bool CData::Append(unsigned short a_wShort)
{
	if (m_bSwitchEndianity)
	{
		SwapEndianity(&a_wShort);
	}
	return Append((byte*)&a_wShort, 2);
}

bool CData::Append(int a_iInt)
{
	if (m_bSwitchEndianity)
	{
		SwapEndianity((unsigned int*)&a_iInt);
	}
	return Append((byte*)&a_iInt, 4);
}

bool CData::Append(CData *a_pData)
{
	if (a_pData->GetSize() + GetSize() > GetMaxSize())
	{
		dprintf(
			"CData::Append: New data size (%d) is greater than the max size (%d)\n", 
			(int)a_pData->GetSize() + GetSize(), 
			(int)GetMaxSize());

		return false;
	}

	// An empty CData is valid
	if (a_pData->GetSize() == 0)
		return true;

	byte* l_abyBuffer = new byte[a_pData->GetSize()];
	a_pData->GetData(l_abyBuffer, 0, a_pData->GetSize());

	// Insert the buffer the vector way
	return Append(l_abyBuffer, a_pData->GetSize());
}


/*****************************************************************
*	bool CData::Insert(byte* a_iBuffer, int a_iIndex, int a_iCount)
*
*	Description:
*		Inserts bytes in the middle of the CData
*
*	Arguments:
*
*	Return Value:
*		None
*		
*****************************************************************/
bool CData::Insert(BYTE* a_iBuffer, int a_iIndex, int a_iCount)
{
	if (this->GetSize() < (DWORD)a_iIndex)
	{
		dprintf(
			"CData::Insert: Index (%d) must be smaller or equal to the CData size (%d)\n", 
			(int)a_iIndex,
			(int)this->GetSize());

		return false;
	}

	m_oData.insert(m_oData.begin() + a_iIndex, a_iBuffer, a_iBuffer + a_iCount);

	return true;
}

bool CData::Insert(CData *a_pData, int a_iIndex)
{
	byte *buffer = new byte[a_pData->GetSize()];

	a_pData->GetData(buffer, 0, a_pData->GetSize());

	bool l_bResult = Insert(buffer, a_iIndex, a_pData->GetSize());

	SAFE_DELETE_ARRAY(buffer);
	return l_bResult;
}

bool CData::Insert(byte a_iByte, int a_iIndex)
{
	byte l_abBytes[1];
	l_abBytes[0] = a_iByte;

	return this->Insert((byte*)&l_abBytes, a_iIndex, 1);
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
int CData::GetByte(DWORD a_dwIndex, OUT BYTE* a_pbyByte)
{
	if (a_dwIndex < m_oData.size())
	{
		*a_pbyByte = m_oData[a_dwIndex];
		return E_NEXUS_OK;
	}
	
	dprintf("CData::GetByte: Index out of range\n");
	return E_NEXUS_INDEX_OUT_OF_RANGE;
}

int CData::GetWord(DWORD a_dwIndex, OUT unsigned short *a_pwWord)
{
	if (m_bSwitchEndianity)
	{
		if (GetData((byte*)a_pwWord, a_dwIndex, 2) == E_NEXUS_OK)
		{
			SwapEndianity(a_pwWord);
			return E_NEXUS_OK;
		}
	}

	return GetData((byte*)a_pwWord, a_dwIndex, 2);
}

int CData::GetDword(DWORD a_dwIndex, OUT DWORD* a_pdwDWord)
{
	if (m_bSwitchEndianity)
	{
		if (GetData((byte*)a_pdwDWord, a_dwIndex, 4) == E_NEXUS_OK)
		{
			SwapEndianity((unsigned int*)a_pdwDWord);
			return E_NEXUS_OK;
		}
	}

	return GetData((byte*)a_pdwDWord, a_dwIndex, 4);
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
	if ((a_dwOffset + a_dwLength > m_oData.size()) || a_dwLength == 0)
	{
		return E_NEXUS_INDEX_OUT_OF_RANGE;
	}

	memcpy(a_pUserBuffer, & m_oData[a_dwOffset], a_dwLength);
	return E_NEXUS_OK;
}



/*****************************************************************
*	void CData::ForceEndianity(EEndianity a_eEndianity)
*
*	Description:
*		ForceEndianity - Set the endianity of the CData object no matter of the architecture
*
*	Arguments:
*
*	Return Value:
*		
*****************************************************************/
void CData::ForceEndianity(EEndianity a_eEndianity)
{
	m_bSwitchEndianity = (a_eEndianity == ENDIANITY_SWITCH);
}

/*****************************************************************
*	bool CData::Remove(DWORD a_dwIndex, DWORD a_dwCount)
*
*	Description:
*		Remove - Removes a range of bytes from the data
*
*	Arguments:
*
*	Return Value:
*		
*****************************************************************/
bool CData::Remove(DWORD a_dwIndex, DWORD a_dwCount)
{
	if (a_dwIndex >= GetSize())
	{
		dprintf(
			"CData::Remove: Index (%d) is greater than the data size (%d)\n", 
			(int)a_dwIndex, 
			(int)GetSize());

		return false;
	}
	if (a_dwIndex + a_dwCount > GetSize())
	{
		dprintf(
			"CData::Remove: Count (%d) exceeds the data size (%d)\n", 
			(int)a_dwCount, 
			(int)GetSize());

		return false;
	}

	// Remove the needed range
	m_oData.erase(m_oData.begin() + a_dwIndex, m_oData.begin() + a_dwIndex + a_dwCount);

	return true;
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
*	int CData::Find(DWORD a_dwStartIndex, byte a_byValue)
*
*	Description:
*		Returns the index of a byte in the data
*
*	Arguments:
*		None
*
*	Return Value:
*		None
*		
*****************************************************************/
int CData::Find(DWORD a_dwStartIndex, byte a_byValue)
{
	int l_iIndex = a_dwStartIndex;

	if (a_dwStartIndex > GetSize() - 1)
	{
		dprintf(
		"CData::Find: Start Index (%d) is greater than the data size (%d)\n", 
		(int)a_dwStartIndex, 
		(int)GetSize());

		return -1;
	}

	
	while (m_oData[l_iIndex] != a_byValue)
	{
		if (l_iIndex == GetSize() - 1)
			return -1;

		l_iIndex++;
	}

	return l_iIndex;
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
		for (DWORD i = 0; i < m_oData.size(); i++)
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

void CData::Dump(std::string a_sFilename)
{
	FILE * l_pFile;
	l_pFile = fopen(a_sFilename.c_str(), "wb");
	
	fwrite(&(m_oData)[0], 1, this->GetSize(), l_pFile);
	fclose(l_pFile);
}

