#include "uartbuf.h"
#include "frame.h"
#include "Generic.h"

#include <usart.h>
#include <string.h>     // for strlen

#ifdef DEBUG_FRAME
#include <string.h>
#include <stdio.h>
#endif

#pragma udata large_udata
volatile CBuffer_large _Uart[2];
volatile CBuffer_large *Uart = _Uart;
//volatile CBuffer_small _hpUart[2];
//volatile CBuffer_small *hpUart = _hpUart;
#pragma udata

void uartbuf_init(void) {

   cbuffer_large_init(&Uart[0]);
   cbuffer_large_init(&Uart[1]);
   //cbuffer_small_init(&hpUart[0]);
   //cbuffer_small_init(&hpUart[1]);
}

unsigned char *uartbuf_dump(UINT channel) {
   if ((channel == SLAVE_RX) || (channel == SLAVE_TX))
      return (cbuffer_large_dumpdata(&Uart[channel]));
   /*if (channel == SLAVE_HPTX)
      return (cbuffer_small_dumpdata(&hpUart[channel - 2]));*/
}

void uartbuf_flush(UINT channel) {

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
   /*else 
       if (channel == SLAVE_HPTX) 
       {

      while (!cbuffer_isempty(&hpUart[channel - 2])) {
         cbuffer_small_read(&hpUart[channel - 2], &ch);
#ifdef DEBUG
         printf("(0x%03X) - ", ch);
#else
         while (BusyUSART());
         putcUSART(ch);
#endif
      }
   }*/
}

void uartbuf_getframe(UINT channel, unsigned char *f, UINT flen, UINT foffset) {

   UINT i;
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

   } /*else if (channel == SLAVE_HPTX) {

      i = 0;
      while (i < (foffset - 1)) {
         cbuffer_small_read(&hpUart[channel - 2], &ch);
         i++;
      }

      i = 0;
      while (i < flen) {
         cbuffer_small_read(&hpUart[channel - 2], &ch);
         f[i++] = ch;
      }

      f[flen] = '\0';
   }*/
}

void uartbuf_putframe(UINT channel, unsigned char *f) {

   UINT i;
   UINT fsize = strlen((char *)f);

   if ((channel == SLAVE_TX) || (channel == SLAVE_RX)) {

      for (i=0; i<fsize; i++)
         cbuffer_large_write(&Uart[channel], f[i]);

   } /*else if (channel == SLAVE_HPTX) {

      for (i=0; i<fsize; i++)
         cbuffer_small_write(&hpUart[channel - 2], f[i]);
   }*/
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

