#include <p18cxxx.h>
#include <stdio.h>
#include <usart.h>
#include <timers.h>
#include "uartbuf.h"
#include "setup.h"
#include "frame.h"
#include "board.h"
#include "functions.h"


extern ram UINT32 time_scheduling;

// UART ISR vars
static unsigned char ch;

// TIMER0 ISR vars
extern ram BOOL both_fpga_ok;
extern ram BOOL check;

extern ram UINT max;

extern CBuffer_large _Uart[2];
extern CBuffer_large *Uart;
//extern CBuffer_small _hpUart[2];
//extern CBuffer_small *hpUart;

#pragma code isr=0x08
#pragma interrupt isr

void isr(void) 
{   
    UINT test;
    
    test = (UINT)FSR1L+(UINT)(FSR1H<<8);
    if (test>max)
        max = test;
    
    if (PIR1bits.TMR2IF) 
    {
        time_scheduling++;
        PIR1bits.TMR2IF = 0;
    }
    
    if (PIR1bits.RCIF) 
    {
        test = (UINT)FSR1L+(UINT)(FSR1H<<8);
        if (test>max)
            max = test;
        
        while (DataRdyUSART())
        {
            if (RCSTAbits.OERR || RCSTAbits.FERR)
            {
                RCSTAbits.CREN = 0; // Clearing CREN clears any Overrun (OERR) errors
                Nop();
                RCSTAbits.CREN = 1; // Re-enable continuous USART receive
                RCSTAbits.SPEN = 0;
                RCSTAbits.SPEN = 1;
            }

            ch = ReadUSART();

            Uart[SLAVE_RX].data[Uart[SLAVE_RX].wrp] = ch;
            Uart[SLAVE_RX].wrp = (Uart[SLAVE_RX].wrp + 1) % Uart[SLAVE_RX].size;

            if (Uart[SLAVE_RX].wrp == Uart[SLAVE_RX].rdp)
                Uart[SLAVE_RX].rdp = (Uart[SLAVE_RX].rdp + 1) % Uart[SLAVE_RX].size; // full, overwrite
        }

        PIR1bits.RCIF = 0; // reset interrupt
    }
}

#pragma code

// EOF


