#ifndef _UARTBUF_H_
#define _UARTBUF_H_

#include "cbuffer.h"
#include "frame.h"

#define SLAVE_RX       0
#define SLAVE_TX       1
#define SLAVE_HPTX     2

void uartbuf_init(void);

unsigned char *uartbuf_dump(UINT channel);

void uartbuf_flush(UINT channel);

void uartbuf_getframe(UINT channel, unsigned char *f, UINT flen, UINT foffset);
void uartbuf_putframe(UINT channel, unsigned char *f);

#endif

// EOF



