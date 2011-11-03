/***********************************
* Description:
*	General Macros
************************************/

#pragma once

#define ASSIGNED(f) (f != 0)

// Fetch high or low byte from word
enum {HIGH_BYTE, LOW_BYTE };

// Fetch high word, mid word or low word from long
enum { HIGH_WORD, MID_WORD, LOW_WORD };

// Fetch byte from addresses at offset
#define get_byte(addr, i)	(*( ((byte*)(addr)) + (i) ))
#define get_char(addr, i)   (*( ((char*)(addr)) + (i) ))

#define addr_get_word(addr, i)	(*( ((word*)(addr)) + (i) ))

#define high_byte(addr)		get_byte(addr, HIGH_BYTE)
#define low_byte(addr)		get_byte(addr, LOW_BYTE)

#define low_word(addr)		addr_get_word(addr, 1)
#define high_word(addr)		addr_get_word(addr, 0)

// Cyclically increase x with base c 
#define cyclic_inc(x, c)	if ((++x)>=(c)) (x)=0

// Cyclically decrease x with base c
#define cyclic_dec(x, c)	if ((x--)<=0) (x)=((c)-1)

// Shift x s bits
#define shift(x, s)			( ((s)>0) ? (x) << (s) : (x) >> (s) )

// Signals bit b
#define setbit(x, b)		((X) |= (1<<(b)))

// Clears bit b
#define clearbit(x, b)		((x) &= ~(1<<(b)))

// Sets of clears bit according to the bit state
// Fill here

// Tests bit b
#define testbit(x, b)		(((x) & (1 << (b)) ) != 0)

// Compare bit of 2 parameters
#define compbit(x,y,bit)	(testbit(x,bit) == testbit(y, bit))


#ifndef NOMINMAX
#ifndef MAX
#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)	(((a) < (b) ? (a) : (b))
#endif
#endif // NOMINMAX




// More utility macros
#define SAFE_FREE(a)			if(a != NULL) {free(a); a = NULL;}
#define SAFE_DELETE(x)			{if ((x) != NULL) delete (x); (x)=NULL;}
#define SAFE_DELETE_ARRAY(x)	{if ((x) != NULL) delete[] (x); (x)=NULL;}

