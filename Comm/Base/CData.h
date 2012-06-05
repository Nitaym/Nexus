/******************************************************************
*	Comm/Base/CData.h
*	Description:
*		Defines a single packet. Managing its memory management of a dynamic length.
*******************************************************************/

#pragma once

#include <string>
#include "General/General.h"
#include <vector>

enum EEndianity
{
	ENDIANITY_NORMAL,
	ENDIANITY_SWITCH,
};

using namespace std;

class CData
{
private:
	bool m_bSwitchEndianity;
	DWORD m_dwMaxDataSize;	// Max size of the data
	vector<byte> m_oData;	// Data vector
	string m_sStringData;	// This string is for the GetString function. It is not really needed.
							// But we like to be able to do GetString() without worrying about 
							// freeing the string afterwards

	void SwapEndianity(unsigned short *a_shNumber)
	{
		*a_shNumber = 
			(*a_shNumber>>8) | 
			(*a_shNumber<<8);
	}
	void SwapEndianity(unsigned int *a_iNumber)
	{
		*a_iNumber = 
			(*a_iNumber >> 24) | 
			((*a_iNumber << 8) & 0x00FF0000) |
			((*a_iNumber >> 8) & 0x0000FF00) |
			(*a_iNumber << 24);
	}

public:
	// Constructor of an empty packet
	CData();

	// Constructor from given null-ending string
	CData(const char *a_strText, DWORD a_dwMaxDataSize=256);

	// Constructor from an array of bytes
	CData(BYTE *a_pBuffer, DWORD a_dwLength, DWORD a_dwMaxDataSize=256);

	// Destructor - Free memory
	~CData();

	// Clone - returned new allocated instance with the same values. Warning: remember to delete this
	// when not needed anymore!
	CData* Clone() const;

	// CopyFrom - Copies the data from another CData to this one
	void CopyFrom(CData* a_oData);

	// SetData - Allocate memory and copy a given data at a given a a_dwNewSize
	bool SetData(BYTE *a_pbyNewData, DWORD a_dwNewSize);

	// Append - Appends a buffer in the end of the data
	bool Append(BYTE *a_pUserBuffer, int a_dwSize);
	bool Append(byte a_byByte);
	bool Append(unsigned short a_wShort);
	bool Append(int a_iInt);
	bool Append(CData *a_pData);

	// Insert - Inserts bytes in the middle of the CData
	bool Insert(BYTE *a_iBuffer, int a_iIndex, int a_iCount);
	bool Insert(CData *a_pData, int a_iIndex);
	bool Insert(byte a_iByte, int a_iIndex);

	// FreeData - free the allocated memory for data
	void FreeData();

	// GetByte - returns one byte of the requested index
	int GetByte(DWORD a_dwIndex, OUT BYTE* a_pbyByte);
	int GetWord(DWORD a_dwIndex, OUT unsigned short *a_pwWord);
	int GetDword(DWORD a_dwIndex, OUT DWORD* a_pdwDWord);
	
	// GetSize - returns the size of the buffer, 0 if not allocated or empty
	DWORD GetSize() const;

	// GetMaxSize - returns the max data size. Default value is 256
	DWORD GetMaxSize() const {return m_dwMaxDataSize;}

	// SetMaxSize - sets the max data size.
	void SetMaxSize(DWORD a_dwMaxDataSize) {m_dwMaxDataSize = a_dwMaxDataSize;}

	// GetData - copy data to a user supplied buffer
	int GetData(OUT BYTE *a_pUserBuffer, DWORD a_dwOffset, DWORD a_dwLength) const;

	// ForceEndianity - Set the endianity of the CData object no matter of the architecture
	void ForceEndianity(EEndianity a_eEndianity);

	// Remove - Removes a range of bytes from the data
	bool Remove(DWORD a_dwIndex, DWORD a_dwCount);

	// SetString - copy user supplied string to an internal string
	void SetString(const string &a_strData);

	// GetString - returns a copy of the internal string
	string& GetString();

	// Find - Returns the index of a byte in the data
	int Find(DWORD a_dwStartIndex, byte a_byValue);

	// Print - prints data content
	void Print() const;

	// PrintHex - prints the data as Hex
	void PrintHex() const;

	// Dump - Dumps the data to a file
	void Dump(std::string a_sFilename);

	byte& operator[](int i) { return m_oData[i]; }
};