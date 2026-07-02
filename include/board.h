#ifndef _BOARD_H
#define _BOARD_H

// #include <delays.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ      64000000UL
#endif

#define BOARD_FOSC       (_XTAL_FREQ)
#define BOARD_INSTR_FREQ (BOARD_FOSC / 4UL)
#define FCY              (BOARD_INSTR_FREQ)

#ifndef __delay_ms
#define __delay_ms(ms)   _delay((unsigned long)(ms) * (BOARD_INSTR_FREQ / 1000UL))
#endif

#ifndef __delay_us
#define __delay_us(us)   _delay((unsigned long)(us) * (BOARD_INSTR_FREQ / 1000000UL))
#endif

#endif

// EOF

