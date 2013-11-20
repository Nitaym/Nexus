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
#include <iostream>
#include <fstream>

using namespace std;
using namespace Nexus;

#define dprintf printf // prints directly to stdout, has no logger.

static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


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
*	CData* CData::Compare(CData* comparedData) const
*
*	Description:
*		Compares this CData with comparedData
*
*	Arguments:
*		None
*
*	Return Value:
*		true if the data is the same in comparedData and this 
*       CData, false if not
*
*****************************************************************/
bool Nexus::CData::Compare(CData* comparedData)
{
    if (this->GetSize() != comparedData->GetSize())
        return false;

    for (unsigned int i = 0; i < this->GetSize(); i++)
    {
        if (m_oData[i] != (*comparedData)[i])
            return false;
    }

    return true;
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
			(int)(a_dwSize + GetSize()),
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
		CData::SwapEndianity(&a_wShort);
	}
	return Append((byte*)&a_wShort, 2);
}

bool CData::Append(int a_iInt)
{
	if (m_bSwitchEndianity)
	{
		CData::SwapEndianity((unsigned int*)&a_iInt);
	}
	return Append((byte*)&a_iInt, 4);
}

bool CData::Append(CData *a_pData, int a_iIndex, int a_iCount)
{
    // The (int) is here to solve a warning
    if (a_iCount > (int)GetMaxSize())
    {
        dprintf(
            "CData::Append: New data size (%d) is greater than the max size (%d)\n",
            (int)(GetSize() + a_iCount),
            (int)GetMaxSize());

        return false;
    }

    // An empty CData is valid
    if (a_iCount == 0)
        return true;

    byte* l_abyBuffer = new byte[a_iCount];
    a_pData->GetData(l_abyBuffer, a_iIndex, a_iCount);

    // Insert the buffer the vector way
    return Append(l_abyBuffer, a_iCount);
}

bool CData::Append(CData *a_pData, int a_iCount)
{
    return Append(a_pData, 0, a_iCount);
}

bool CData::Append(CData *a_pData)
{
	return Append(a_pData, a_pData->GetSize());
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

bool CData::Insert(CData *a_pData, int a_iIndex, int a_iCount)
{
	byte *buffer = new byte[a_iCount];

	a_pData->GetData(buffer, 0, a_iCount);

	bool l_bResult = Insert(buffer, a_iIndex, a_iCount);

	SAFE_DELETE_ARRAY(buffer);
	return l_bResult;
}

bool CData::Insert(CData *a_pData, int a_iIndex)
{
	return Insert(a_pData, a_iIndex, a_pData->GetSize());
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
int CData::GetByte(DWORD a_dwIndex, NX_OUT BYTE* a_pbyByte)
{
	if (a_dwIndex < m_oData.size())
	{
		*a_pbyByte = m_oData[a_dwIndex];
		return E_NEXUS_OK;
	}

	dprintf("CData::GetByte: Index out of range\n");
	return E_NEXUS_INDEX_OUT_OF_RANGE;
}

int CData::GetWord(DWORD a_dwIndex, NX_OUT unsigned short *a_pwWord)
{
	if (m_bSwitchEndianity)
	{
		if (GetData((byte*)a_pwWord, a_dwIndex, 2) == E_NEXUS_OK)
		{
			CData::SwapEndianity(a_pwWord);
			return E_NEXUS_OK;
		}
	}

	return GetData((byte*)a_pwWord, a_dwIndex, 2);
}

int CData::GetDword(DWORD a_dwIndex, NX_OUT DWORD* a_pdwDWord)
{
	if (m_bSwitchEndianity)
	{
		if (GetData((byte*)a_pdwDWord, a_dwIndex, 4) == E_NEXUS_OK)
		{
			CData::SwapEndianity((unsigned int*)a_pdwDWord);
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
int CData::GetData(NX_OUT BYTE *a_pUserBuffer, DWORD a_dwOffset, DWORD a_dwLength) const
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
*	void CData::GetEndianity()
*
*	Description:
*       GetEndianity - Returns the endianity of the CData object
*
*	Arguments:
*
*	Return Value:
*       The Endianity
*
*****************************************************************/
EEndianity CData::GetEndianity()
{
    if (m_bSwitchEndianity)
        return ENDIANITY_SWITCH;

    return ENDIANITY_NORMAL;
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
    // Set the string into the data                          (and add null termination)
	SetData((byte*)(a_strData.c_str()), a_strData.length() + 1);
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
    return GetString(0, GetSize());
}

/*****************************************************************
*	string& CData::GetString(int a_iStartIndex, int a_iCount)
*
*	Description:
*		Returns a copy of the internal buffer as string
*
*	Arguments:
*       a_iStartIndex - Where to start copying the string from
*		a_iCount - How many characaters to copy
*
*	Return Value:
*		The data buffer as a string
*
*****************************************************************/
string& CData::GetString(int a_iStartIndex, int a_iCount)
{
    m_sStringData.assign((char*)&(m_oData[a_iStartIndex]), a_iCount);
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
*		The index of the byte stream
*
*****************************************************************/
int CData::Find(DWORD a_dwStartIndex, byte a_byValue)
{
	unsigned int l_iIndex = a_dwStartIndex;

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
*	int CData::Find(DWORD a_dwStartIndex, byte* a_byValue)
*
*	Description:
*		Returns the index of a byte stream in the data
*
*	Arguments:
*		None
*
*	Return Value:
*		The index of the byte stream
*
*****************************************************************/
int CData::Find(DWORD a_dwStartIndex, byte* a_byValue, int a_iLength)
{
    std::string l_sStringData(m_oData.begin(), m_oData.end());
    std::string l_sWhatToFind(a_byValue, a_byValue + a_iLength);

    std::size_t l_iPosition = l_sStringData.find(l_sWhatToFind);

    if (l_iPosition < std::string::npos)
        return l_iPosition;
    else
        return -1;
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

bool CData::Dump(std::string a_sFilename)
{
    return Dump(a_sFilename, "wb");
}

bool CData::Dump(std::string a_sFilename, std::string a_sOpenFlags)
{
    // TODO: Check the result of fopen!! And why don't we use C++ here???
    FILE * l_pFile;
    l_pFile = fopen(a_sFilename.c_str(), a_sOpenFlags.c_str());
    if (l_pFile != NULL)
    {
        fwrite(&(m_oData)[0], 1, this->GetSize(), l_pFile);
        fclose(l_pFile);

        return true;
    }

    return false;
}

bool CData::FromFile(std::string a_sFilename)
{
    std::ifstream::pos_type l_iSize;
    char* l_pMemblock;
    bool result;

    ifstream l_oFile (a_sFilename, ios::in|ios::binary|ios::ate);

    if (l_oFile.is_open())
    {
        l_iSize = l_oFile.tellg();
        l_pMemblock = new char [(unsigned int)l_iSize];
        l_oFile.seekg (0, ios::beg);
        l_oFile.read (l_pMemblock, l_iSize);
        l_oFile.close();

        result = SetData((BYTE*)l_pMemblock, (unsigned int)l_iSize);

        delete[] l_pMemblock;

        return result;
    }

    return false;
}


std::string Nexus::CData::DumpBase64()
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    int bytesRemaining = this->GetSize();
    int currentByte = 0;

    while (bytesRemaining--) {
        char_array_3[i++] = this->m_oData[currentByte++];
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}