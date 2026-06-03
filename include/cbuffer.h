#ifndef _CBUFFER_H
#define _CBUFFER_H

#include <Generic.h>

/**
 * @file cbuffer.h
 * @brief Circular buffer utility definitions for UART frame buffering.
 */

#define LARGE_BUFFER_SIZE   270
#define SMALL_BUFFER_SIZE    30

#define BUFFER_OK           0x00
#define BUFFER_FULL         0x0F
#define BUFFER_EMPTY        0x0E

/**
 * @brief Large circular buffer representation.
 */
typedef struct {
    UINT16 size;
    UINT16 rdp; // READ pointer
    UINT16 wrp; // WRITE pointer
    unsigned char data[LARGE_BUFFER_SIZE]; // vector of elements
} CBuffer_large;

/**
 * @brief Initialize a large circular buffer.
 * @param cb Pointer to the buffer structure.
 */
void cbuffer_large_init(CBuffer_large *cb);

#define cbuffer_isfull(cb)      ((((*cb).wrp + 1) % (*cb).size) == (*cb).rdp)
#define cbuffer_isempty(cb)     ((*cb).wrp == (*cb).rdp)

/**
 * @brief Append a byte to the circular buffer.
 * @param cb Buffer structure.
 * @param ch Byte to append.
 * @return BUFFER_OK if the byte was stored, BUFFER_FULL if the buffer is full.
 */
BYTE cbuffer_large_write(CBuffer_large *cb, unsigned char ch);

/**
 * @brief Read a byte from the circular buffer.
 * @param cb Buffer structure.
 * @param ch Output pointer for the read byte.
 * @return BUFFER_OK if a byte was available, BUFFER_EMPTY otherwise.
 */
BYTE cbuffer_large_read(CBuffer_large *cb, unsigned char *ch);

/**
 * @brief Detect the size and offset of a complete frame stored in the circular buffer.
 * @param cb Buffer structure.
 * @param flen Output frame length.
 * @param foffset Output buffer offset at which the frame starts.
 * @return Error code describing frame validity.
 */
UINT cbuffer_large_getframe_length(CBuffer_large *cb, UINT *flen, UINT *foffset);

/**
 * @brief Access the internal raw data pointer of a large circular buffer.
 * @param cb Buffer structure.
 * @return Pointer to the internal data array.
 */
unsigned char *cbuffer_large_dumpdata(CBuffer_large *cb);

#endif

// EOF
