#ifndef _CBUFFER_H
#define _CBUFFER_H

#include <Generic.h>

#define LARGE_BUFFER_SIZE   270
#define SMALL_BUFFER_SIZE    30

#define BUFFER_OK           0x00
#define BUFFER_FULL         0x0F
#define BUFFER_EMPTY        0x0E

// Circular buffer struct

typedef struct {
    UINT16 size;
    UINT16 rdp; // READ pointer
    UINT16 wrp; // WRITE pointer
    unsigned char data[LARGE_BUFFER_SIZE]; // vector of elements
} CBuffer_large;

/*typedef struct {
    UINT16 size;
    UINT16 rdp; // READ pointer
    UINT16 wrp; // WRITE pointer
    unsigned char data[SMALL_BUFFER_SIZE]; // vector of elements
} CBuffer_small;*/

void cbuffer_large_init(CBuffer_large *cb);
//void cbuffer_small_init(CBuffer_small *cb);

#define cbuffer_isfull(cb)      ((((*cb).wrp + 1) % (*cb).size) == (*cb).rdp)
#define cbuffer_isempty(cb)     ((*cb).wrp == (*cb).rdp)

BYTE cbuffer_large_write(CBuffer_large *cb, unsigned char ch);
//BYTE cbuffer_small_write(CBuffer_small *cb, unsigned char ch);
BYTE cbuffer_large_read(CBuffer_large *cb, unsigned char *ch);
//BYTE cbuffer_small_read(CBuffer_small *cb, unsigned char *ch);

UINT cbuffer_large_getframe_length(CBuffer_large *cb, UINT *flen, UINT *foffset);
//UINT cbuffer_small_getframe_length(CBuffer_small *cb, UINT *flen, UINT *foffset);

unsigned char *cbuffer_large_dumpdata(CBuffer_large *cb);
//unsigned char *cbuffer_small_dumpdata(CBuffer_large *cb);

#endif

// EOF
