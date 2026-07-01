#ifndef _BOARD_H
#define _BOARD_H

// #include <delays.h>

#define FCY             16000000LL
#define BOARD_FOSC      (FCY * 4)
#define BOARD_INSTR_FREQ (BOARD_FOSC / 4)

#ifndef __delay_ms
#define __delay_ms(ms)      _delay((unsigned long)((ms) * (_XTAL_FREQ / 4000.0)))
#endif

#ifndef __delay_us
#define __delay_us(us)      _delay((unsigned long)((us) * (_XTAL_FREQ / 4000000.0)))
#endif

#endif

// EOF

