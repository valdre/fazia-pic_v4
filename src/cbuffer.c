#include "cbuffer.h"
#include "frame.h"

#include <stdlib.h>
#include <string.h>     // for memset

void cbuffer_large_init(CBuffer_large *cb) {

    cb->size = LARGE_BUFFER_SIZE;
    memset(cb->data, '\0', cb->size);
    cb->rdp = cb->wrp = 0;
}

/*void cbuffer_small_init(CBuffer_small *cb) {

    cb->size = SMALL_BUFFER_SIZE;
    memset(cb->data, '\0', cb->size);
    cb->rdp = cb->wrp = 0;
}*/

BYTE cbuffer_large_write(CBuffer_large *cb, unsigned char ch) 
{
    BYTE retval = BUFFER_OK;

    cb->data[cb->wrp] = ch;
    cb->wrp = (cb->wrp + 1) % cb->size;
    if (cb->wrp == cb->rdp) {
        cb->rdp = (cb->rdp + 1) % cb->size; // full, overwrite
        retval = BUFFER_FULL;
    }
    return (retval);
}

/*BYTE cbuffer_small_write(CBuffer_small *cb, unsigned char ch) 
{
    BYTE retval = BUFFER_OK;

    cb->data[cb->wrp] = ch;
    cb->wrp = (cb->wrp + 1) % cb->size;
    if (cb->wrp == cb->rdp) {
        cb->rdp = (cb->rdp + 1) % cb->size; // full, overwrite
        retval = BUFFER_FULL;
    }
    return (retval);
}*/

BYTE cbuffer_large_read(CBuffer_large *cb, unsigned char *ch) {

    if (cbuffer_isempty(cb))
        return BUFFER_EMPTY;

    (*ch) = cb->data[cb->rdp];
    cb->rdp = (cb->rdp + 1) % cb->size;

    return BUFFER_OK;
}

/*BYTE cbuffer_small_read(CBuffer_small *cb, unsigned char *ch) {

    if (cbuffer_isempty(cb))
        return BUFFER_EMPTY;

    (*ch) = cb->data[cb->rdp];
    cb->rdp = (cb->rdp + 1) % cb->size;

    return BUFFER_OK;
}*/

UINT cbuffer_large_getframe_length(CBuffer_large *cb, UINT *flen, UINT *foffset) 
{
    UINT j,len, offset;
    UINT16 lbound,hbound;
    unsigned char ch;
    BYTE ncrc = 0;
    BOOL frame_start = FALSE;
    BOOL frame_end = FALSE;
    BOOL loop_end = FALSE;

    if (cbuffer_isempty(cb))
        return ERR_FRAME_EMPTY;

    len = 0;
    offset = 0;
    lbound=cb->rdp;
    hbound=cb->wrp;
    j=lbound;

    while (!loop_end && (j != hbound)) 
    {
        ch = cb->data[j];
        j = (j + 1) % cb->size;

        offset++;

        if (is_startkw(ch)) 
        {
            *foffset = offset;
            ncrc = 0;
            len=1;
            frame_start = TRUE;
            frame_end = FALSE;
        } 
        else 
        {
            if (frame_start)
            {
                len++;
                
                if (!frame_end)
                {
                    if (is_endkw(ch))
                        frame_end = TRUE;
                }
                else
                {
                    ncrc++;
                    if (ncrc == 2)
                        loop_end = TRUE;
                }
            }
        }
    }

    if (loop_end) 
    { // valid frame detected on buffer
        *flen = len;
        return (ERR_FRAME_NONE);
    } 
    else
        return (ERR_FRAME_INCOMPLETE);
}

/*UINT cbuffer_small_getframe_length(CBuffer_small *cb, UINT *flen, UINT *foffset) 
{
    UINT j, len, offset;
    UINT16 lbound = cb->rdp; // buffer lowest bound
    UINT16 hbound = cb->wrp; // buffer highest bound
    UINT16 size = cb->size; // buffer size
    unsigned char ch;
    BYTE ncrc = 0;
    BOOL frame_start = FALSE;
    BOOL frame_end = FALSE;
    BOOL loop_end = FALSE;

    if (cbuffer_isempty(cb))
        return ERR_FRAME_EMPTY;

    j = lbound;
    len = 0;
    offset = 0;

    while (!loop_end && (j != hbound)) {

        ch = cb->data[j];
        j = (j + 1) % size;

        offset++;

        if (is_startkw(ch)) {

            *foffset = offset;
            len = ncrc = 0;
            len++;
            frame_start = TRUE;
            frame_end = FALSE;

            continue;
        }

        if ((ch >= 0x20) && frame_start && !frame_end) {

            len++;
            continue;
        }

        if (is_endkw(ch) && frame_start) {

            len++;
            frame_end = TRUE;
            continue;
        }

        if (frame_start && frame_end) { // two CRC bytes to read...

            len++;
            ncrc++;

            if (ncrc == 2)
                loop_end = TRUE;

            continue;
        }
    }

    if (loop_end) { // valid frame detected on buffer
        *flen = len;
        return (ERR_FRAME_NONE);
    } else
        return (ERR_FRAME_INCOMPLETE);
}*/

unsigned char *cbuffer_large_dumpdata(CBuffer_large *cb) {

    return (cb->data);
}

/*unsigned char *cbuffer_small_dumpdata(CBuffer_small *cb) {

    return (cb->data);
}*/

// EOF


