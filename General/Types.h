/*****************************************
* Description:
*  General Types
*****************************************/

#pragma once

#ifndef LOCAL
#define LOCAL
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef INTERRUPT
#define INTERRUPT
#endif

#ifndef EXTERN
#define EXTERN extern "C"
#endif

#ifndef CALLBACK
#define CALLBACK
#endif

#ifndef NULL
#define NULL	0
#endif

#define NULL_FUNCTION	(void (*)(void))) 0

#define MAX_BYTE	0xFF
#define MAX_CHAR	0x7F
#define MAX_WORD	0xFFFF
#define MAX_SHORT	0x7FFF
#define MAX_LONG	0xFFFFFFFFl
#define MAX_SLONG	0x7FFFFFFFl

#ifndef FALSE
#define FALSE	(0)
#endif

#ifndef TRUE
#define TRUE	(!FALSE)
#endif

#define False	FALSE
#define True	TRUE

typedef unsigned char	byte;
// typedef unsigned short	word;
typedef unsigned long	dword;

#ifndef __cplusplus
typedef unsigned char	bool;
#endif


#ifdef WIN32
typedef dword	DWORD;
typedef	byte	BYTE;


#else
typedef bool	boolean;
typedef bool	Bool;

typedef char	CHAR;
typedef	byte	BYTE;
typedef short	SHORT;
typedef word	WORD;
typedef long	LONG;
typedef dword	DWORD;
typedef bool	BOOL;
typedef float	FLOAT;

typedef byte			UInt8;
typedef signed char		SInt8;
typedef unsigned short	UInt16;
typedef short			SInt16;
typedef unsigned long	UInt32;
typedef long			SInt32;
typedef unsigned int	UInt;

typedef UInt8			UINT8;
typedef SInt8			INT8;
typedef UInt16			UINT16;
typedef SInt16			INT16;
typedef UInt32			UINT32;
typedef SInt32			INT32;

typedef UInt32			LWORD;
typedef unsigned short	USHORT;

typedef int				INT;
typedef unsigned int	UINT;

typedef byte				UByte;	// uby
typedef CHAR				SByte;  // sby
typedef unsigned long long	UInt64; // uddw
typedef signed long long	SInt64; // sddw
typedef float				Float32; // sf
typedef double				Float64; // sdf

typedef SHORT			*LPSHORT;
typedef FLOAT			*PFLOAT;
typedef BOOL			*LPBOOL;
typedef BYTE			*LPBYTE;
typedef const BYTE		*LPCBYTE;
typedef CHAR			*LPCHAR;
typedef int				*LPINT;
typedef WORD			*LPWORD;
typedef long			*LPLONG;
typedef DWORD			*LPDWORD;
typedef void			*LPVOID;
typedef CONST void		*LPCVOID;
typedef unsigned int	*PUINT;
typedef CHAR *			LPSTR;
typedef const CHAR *	LPCSTR;
typedef void			VOID;

typedef bool			BOOLEAN;
typedef unsigned long	ULONG;

typedef void*			HANDLE;
typedef void*			LPARAM;

#endif

typedef void (*LPVOIDFUNC)(void);
typedef void (*LPVOIDLPVOIDFUNC)(void *);


namespace Nexus
{

// Unions - Little Endian
// ----------------------
// Word decomposition into bytes
typedef struct {
	byte byLsb;
	byte byMsb;
} TWordBytes;

typedef union {
	unsigned short wValue;
	TWordBytes stBytes;
} UWord;

// DWORD decomposition into words
typedef struct {
	UWord wLsw;
	UWord wMsw;
} TDwordWords;

typedef union {
	DWORD dwValue;
	TDwordWords stWords;
} UDword;

}
