#include "uartbuf.h"
#include "frame.h"
#include "Generic.h"
#include "functions.h"

// #include <usart.h>
#include <string.h>     // for strlen

#ifdef DEBUG_FRAME
#include <string.h>
#include <stdio.h>
#endif

#pragma udata large_udata
CBuffer_large _Uart[2];
CBuffer_large *Uart = _Uart;
#pragma udata

/**
 * @brief Initialize both UART receive and transmit circular buffers.
 */
void uartbuf_init(void) {

   cbuffer_large_init(&Uart[0]);
   cbuffer_large_init(&Uart[1]);
}

/**
 * @brief Return a pointer to the raw buffer data for a UART channel.
 * @param channel UART channel identifier
 * @return unsigned char status or result code.
 */
unsigned char * uartbuf_dump(uint16_t channel) {
   if ((channel == SLAVE_RX) || (channel == SLAVE_TX))
      return (cbuffer_large_dumpdata(&Uart[channel]));
   return NULL;
}

/**
 * @brief Transmit all buffered data from the TX buffer via UART.
 * @param channel UART channel identifier
 */
void uartbuf_flush(uint16_t channel) {

   unsigned char ch;

   if (channel == SLAVE_TX) 
   {

      while (!cbuffer_isempty(&Uart[channel])) {
         cbuffer_large_read(&Uart[channel], &ch);
         #ifdef DEBUG
         printf("(0x%03X) - ", ch);
         #else
         while (BusyUSART());
         putcUSART(ch);
         #endif
      }
   }
}

/**
 * @brief Extract a complete frame from the RX buffer.
 * @param channel UART channel
 * @param f Output frame buffer
 * @param f Output frame buffer
 * @param f Output frame buffer
 */
void uartbuf_getframe(uint16_t channel, unsigned char *f, uint16_t flen, uint16_t foffset) {
   uint16_t i;
   unsigned char ch;
   if ((channel == SLAVE_TX) || (channel == SLAVE_RX)) {
      i = 0;
      while (i < (foffset - 1)) {
         cbuffer_large_read(&Uart[channel], &ch);
         i++;
      }
      i = 0;
      while (i < flen) {
         cbuffer_large_read(&Uart[channel], &ch);
         f[i++] = ch;
      }
      f[flen] = '\0';
   }
}

/**
 * @brief Append a frame to the TX buffer for transmission.
 * @param channel UART channel
 * @param f Frame buffer to transmit
 */
void uartbuf_putframe(uint16_t channel, unsigned char *f) {
   uint16_t i;
   uint16_t fsize = strlen((char *)f);
   if ((channel == SLAVE_TX) || (channel == SLAVE_RX)) {
      for (i=0; i<fsize; i++) {
         cbuffer_large_write(&Uart[channel], f[i]);
      }
   }
   #ifdef DEBUG_FRAME
   printf("\r\n\nPutFrame(");
   if (channel == SLAVE_TX)
      printf("Slave TX");
   else if (channel == SLAVE_RX)
      printf("Slave RX");
   else if (channel == SLAVE_HPTX)
      printf("Slave #HP# TX");
   else if (channel == SLAVE_HPRX)
      printf("Slave #HP# RX");
   printf(") for FEC: \n\r");
   printf("\tKW = 0x%X \t", frame_getkw(f));
   printf("IDB = 0x%03X \t", frame_getidb(f));
   printf("IDS = 0x%02X \t", frame_getids(f));
   printf("CMD = 0x%X \t", frame_getcmd(f));
   printf("DATA = %s \t", frame_getdata(f));
   printf("CRC = 0x%X \t", frame_getcrc(f));
   printf("fsize = %d\n\r", f->size);
   #endif
}

// EOF

