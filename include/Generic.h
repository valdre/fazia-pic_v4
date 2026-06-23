#include <stdint.h>

#ifndef _GENERIC_H_
#define _GENERIC_H_

/*  UnComment the following macro to use AND_OR mask based setting, Comment it to use AND based macros */
//#define USE_AND_OR

#define bitget(var, bitno)  (var & 1<<bitno)?1:0
#define bitset(var, bitno)  (var |= 1<<bitno)
#define bitclr(var, bitno)  (var &= ~(1<<bitno))

#define PUBLIC
#define PROTECTED
#define PRIVATE                 static

typedef enum _BOOL { FALSE = 0, TRUE } bool;

typedef struct
{
    uint8_t    b0:     1;
    uint8_t    b1:     1;
    uint8_t    b2:     1;
    uint8_t    b3:     1;
    uint8_t    b4:     1;
    uint8_t    b5:     1;
    uint8_t    b6:     1;
    uint8_t    b7:     1;

}BYTE_BITS;
typedef struct
{
    unsigned int    b0:     1;
    unsigned int    b1:     1;
    unsigned int    b2:     1;
    unsigned int    b3:     1;
    unsigned int    b4:     1;
    unsigned int    b5:     1;
    unsigned int    b6:     1;
    unsigned int    b7:     1;
    unsigned int    b8:     1;
    unsigned int    b9:     1;
    unsigned int    b10:    1;
    unsigned int    b11:    1;
    unsigned int    b12:    1;
    unsigned int    b13:    1;
    unsigned int    b14:    1;
    unsigned int    b15:    1;
}WORD_BITS;

typedef union _BYTE_VAL
{
    BYTE_BITS bits;
    uint8_t Val;
} BYTE_VAL;


typedef union _WORD_VAL
{
    uint16_t Val;
    WORD_BITS   bits;
    struct
    {
        uint8_t LB;
        uint8_t HB;
    } uint8_t;
    struct
    {
        BYTE_VAL    low;
        BYTE_VAL    high;
    }byteUnion;

    uint8_t v[2];
} WORD_VAL;

typedef union _DWORD_VAL
{
    uint32_t Val;
    struct
    {
        uint8_t LB;
        uint8_t HB;
        uint8_t UB;
        uint8_t MB;
    } uint8_t;
    struct
    {
        uint16_t LW;
        uint16_t HW;
    } uint32_t;
    struct
    {
        WORD_VAL    low;
        WORD_VAL    high;
    }wordUnion;
    struct
    {
        BYTE_VAL    lowLSB;
        BYTE_VAL    lowMSB;
        BYTE_VAL    highLSB;
        BYTE_VAL    highMSB;
    }byteUnion;
    uint8_t v[4];
    uint16_t w[2];
} DWORD_VAL;

#define LSB(a)          ((a).v[0])
#define MSB(a)          ((a).v[1])

#define LOWER_LSB(a)    ((a).v[0])
#define LOWER_MSB(a)    ((a).v[1])
#define UPPER_LSB(a)    ((a).v[2])
#define UPPER_MSB(a)    ((a).v[3])

#endif

// EOF
