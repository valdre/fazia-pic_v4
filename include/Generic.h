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

typedef unsigned char       uint8_t;               // 8-bit
typedef unsigned short int  WORD;               // 16-bit
typedef unsigned long       DWORD;              // 32-bit

typedef unsigned int        uint16_t;
typedef unsigned char       UINT8;              // other name for 8-bit integer
typedef unsigned short      UINT16;             // other name for 16-bit integer
typedef unsigned long       uint32_t;             // other name for 32-bit integer


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
    WORD    b0:     1;
    WORD    b1:     1;
    WORD    b2:     1;
    WORD    b3:     1;
    WORD    b4:     1;
    WORD    b5:     1;
    WORD    b6:     1;
    WORD    b7:     1;
    WORD    b8:     1;
    WORD    b9:     1;
    WORD    b10:    1;
    WORD    b11:    1;
    WORD    b12:    1;
    WORD    b13:    1;
    WORD    b14:    1;
    WORD    b15:    1;
}WORD_BITS;

typedef union _BYTE_VAL
{
    BYTE_BITS bits;
    uint8_t Val;
} BYTE_VAL;


typedef union _WORD_VAL
{
    WORD Val;
    WORD_BITS   bits;
    struct
    {
        uint8_t LB;
        uint8_t HB;
    } byte;
    struct
    {
        BYTE_VAL    low;
        BYTE_VAL    high;
    }byteUnion;

    uint8_t v[2];
} WORD_VAL;

typedef union _DWORD_VAL
{
    DWORD Val;
    struct
    {
        uint8_t LB;
        uint8_t HB;
        uint8_t UB;
        uint8_t MB;
    } byte;
    struct
    {
        WORD LW;
        WORD HW;
    } word;
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
    WORD w[2];
} DWORD_VAL;

#define LSB(a)          ((a).v[0])
#define MSB(a)          ((a).v[1])

#define LOWER_LSB(a)    ((a).v[0])
#define LOWER_MSB(a)    ((a).v[1])
#define UPPER_LSB(a)    ((a).v[2])
#define UPPER_MSB(a)    ((a).v[3])

#endif

// EOF
