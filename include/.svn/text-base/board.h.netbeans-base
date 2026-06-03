#ifndef _BOARD_H
#define _BOARD_H

#include <delays.h>

#define FCY         16000000LL
#define FOSC        FCY*4
#define INSTR_FREQ  FOSC/4

#define __delay_ms(ms)      Delay10KTCYx((((INSTR_FREQ/10000)*ms)/1000))
#define __delay_us(us)      Delay10TCYx((((INSTR_FREQ/10)*us)/1000))

#endif

// EOF

