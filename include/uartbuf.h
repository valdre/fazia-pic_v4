#ifndef _UARTBUF_H_
#define _UARTBUF_H_

#include "cbuffer.h"
#include "frame.h"

/**
 * @file uartbuf.h
 * @brief Buffer management for UART RX/TX frame handling.
 */

#define SLAVE_RX       0
#define SLAVE_TX       1
#define SLAVE_HPTX     2

/**
 * @brief Initialize UART buffers and internal channel state.
 */
void uartbuf_init(void);

/**
 * @brief Retrieve a direct pointer to the internal UART buffer data.
 * @param channel Buffer channel identifier.
 * @return Pointer to the internal data array.
 */
unsigned char *uartbuf_dump(UINT channel);

/**
 * @brief Flush the specified UART buffer channel.
 * @param channel Buffer channel identifier.
 */
void uartbuf_flush(UINT channel);

/**
 * @brief Copy a frame from the UART receive buffer into a destination buffer.
 * @param channel Buffer channel identifier.
 * @param f Destination frame buffer.
 * @param flen Length of the frame to copy.
 * @param foffset Offset into the internal buffer.
 */
void uartbuf_getframe(UINT channel, unsigned char *f, UINT flen, UINT foffset);

/**
 * @brief Enqueue a frame for UART transmission.
 * @param channel Buffer channel identifier.
 * @param f Source frame buffer.
 */
void uartbuf_putframe(UINT channel, unsigned char *f);

#endif

// EOF
