#include "cbuffer.h"
#include "frame.h"

#include <stdlib.h>
#include <string.h>     // for memset

/**
 * @brief Initialize a large circular buffer for frame storage.
 * @param cb Circular buffer structure to initialize
 */
void cbuffer_large_init(CBuffer_large *cb) {
    cb->size = LARGE_BUFFER_SIZE;
    memset(cb->data, '\0', cb->size);
    cb->rdp = cb->wrp = 0;
}

/**
 * @brief Write a byte to the circular buffer with wrap-around.
 * @param cb Circular buffer
 * @param ch Byte to write
 * @return uint8_t status or result code.
 */
uint8_t cbuffer_large_write(CBuffer_large *cb, unsigned char ch) {
    uint8_t retval = BUFFER_OK;

    cb->data[cb->wrp] = ch;
    cb->wrp = (cb->wrp + 1) % cb->size;
    if (cb->wrp == cb->rdp) {
        cb->rdp = (cb->rdp + 1) % cb->size; // full, overwrite
        retval = BUFFER_FULL;
    }
    return (retval);
}

/**
 * @brief Remove and return the next byte from the circular buffer.
 * @param cb Circular buffer
 * @param ch Pointer to output byte
 * @return uint8_t status or result code.
 */
uint8_t cbuffer_large_read(CBuffer_large *cb, unsigned char *ch) {

    if (cbuffer_isempty(cb))
        return BUFFER_EMPTY;

    (*ch) = cb->data[cb->rdp];
    cb->rdp = (cb->rdp + 1) % cb->size;

    return BUFFER_OK;
}

/**
 * @brief Scan the buffer to find and measure a complete frame.
 * @param cb Circular buffer
 * @param flen Frame length output
 * @param foffset Frame offset output
 * @return uint16_t status or result code.
 */
uint16_t cbuffer_large_getframe_length(CBuffer_large *cb, uint16_t *flen, uint16_t *foffset) 
{
    uint16_t j,len, offset;
    uint16_t lbound,hbound;
    unsigned char ch;
    uint8_t ncrc = 0;
    bool frame_start = FALSE;
    bool frame_end = FALSE;
    bool loop_end = FALSE;

    if (cbuffer_isempty(cb)) {
        return ERR_FRAME_EMPTY;
    }

    len = 0;
    offset = 0;
    lbound=cb->rdp;
    hbound=cb->wrp;
    j=lbound;

    while (!loop_end && (j != hbound)) {
        ch = cb->data[j];
        j = (j + 1) % cb->size;
        offset++;
        if (is_startkw(ch)) {
            *foffset = offset;
            ncrc = 0;
            len=1;
            frame_start = TRUE;
            frame_end = FALSE;
        } else {
            if (frame_start) {
                len++;
                if (!frame_end) {
                    if (is_endkw(ch))
                        frame_end = TRUE;
                } else {
                    ncrc++;
                    if (ncrc == 2) {
                        loop_end = TRUE;
                    }
                }
            }
        }
    }
    if (loop_end) { // valid frame detected on buffer
        *flen = len;
        return (ERR_FRAME_NONE);
    } else {
        return (ERR_FRAME_INCOMPLETE);
    }
}

/**
 * @brief Operate on a circular buffer used for UART frame storage.
 * @param cb Input or output parameter.
 * @return unsigned char status or result code.
 */
unsigned char *cbuffer_large_dumpdata(CBuffer_large *cb) {
    return (cb -> data);
}
// EOF


