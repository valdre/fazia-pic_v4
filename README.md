# libs to update be cause t dose not exist anymore (probably all in xc.h)

- #include <timers.h>     // to delete
  Macro :
  - TIMER_INT_ON
  - TIMER_INT_OFF
  - T1_16BIT_RW
  - T2_PS_1_16
  - T2_POST_1_16
  Fonction :
  - OpenTimer1();
  - WriteTimer1
  - ReadTimer1
  - OpenTimer2
- #include <delays.h>     // to delete
- #include <spi.h>        // to delete
- #include <usart.h>      // to delete
  - BusyUSART();
  - putcUSART(char);
- #include <p18cxxx.h>      // to delete
  en gros lui c'est elif define ... pour au final inclure #include <p18f46k20.h> qui du coup j'imagine n'est plus dispo non plus.
  Macro :
  aucune.
  Fonctions :
  aucune.

#include <delays.h>     ->      delete (included in xc.h)
#include <p18cxxx.h>    ->      #include <xc.h>
#include <timers.h>     ->      delete (included in xc.h)
#include <delays.h>     ->      delete (included in xc.h)
#include <spi.h>        ->      delete (made by hand)
#include <usart.h>      ->      delete (made by hand)
#include <stddef.h>     ->      stay
#include <stdio.h>      ->      stay
#include <stdlib.h>     ->      stay
#include <string.h>     ->      stay

#include "cbuffer.h"
#include "frame.h"
#include "utils.h"
#include "functions.h"
#include "uartbuf.h"
#include "setup.h"
#include "board.h"
#include "Generic.h"

fonction a remplacer
Delay10TCYx(n)          -> fait attendre n * 10 us
    __delay_us(n * 10);
Delay100TCYx(n)         -> fait attendre n * 10 ms
    __delay_ms(n * 10);
ReadUSART();            -> lis ce qu'il y à sur l'uart
    config à la main à voir
OpenSPI(spi_fsoc, mode, SMPMID) ->  a config à la main
OpenTimer1();
OpenTimer2();
OpenTimer3();
ReadTimer1();
ReadTimer2();
ReadTimer3();
WriteTimer1();
WriteTimer2();
WriteTimer3();

replace rom by simple const     ->      done
replace WORD	->	uint16_t
replace DWORD	->	uint32_t
replace BYTE 		->	uint8_t

Ajout de prototype pour les fonctions à utilisé que je vais devoir recoder plus tard.

```c
// Emulation des Delays C18 vers les fonctions natives XC8 
// XC8 a besoin que _XTAL_FREQ soit dÃ©fini (ex: #define _XTAL_FREQ 16000000)
#define Delay10TCYx(x)   __delay_us((x) * 2)  // Ajustement temporaire
#define Delay100TCYx(x)  __delay_us((x) * 20)
#define Delay10KTCYx(x)  __delay_ms((x) * 2)

// Emulation des anciennes fonctions SPI (Stubs temporaires) ---
#define SPI_FOSC_16 0
#define MODE_00     0
#define MODE_10     0
#define SMPEND      0
#define SMPMID      0
#define _XTAL_FREQ  99999 // TODO c'est une valeur au pif la faut changer à la vrai valeur
void OpenSPI(char m, char edge, char smp);
void CloseSPI(void);
void putcSPI(unsigned char data);
uint8_t getcSPI(void);

// Emulation des anciennes fonctions USART (Stubs temporaires) ---
char DataRdyUSART(void);
char ReadUSART(void);
void putcUSART(char data);
char BusyUSART(void);

#endif
```

# OLD File structure

- include/
  - board.h
  - cbuffer.h
  - frame.h
  - functions.h
  - Generic.h
  - isr.h
  - setup.h
  - uartbuf.h
  - utils.h
- src/
  - cbuffer.c
  - frame.c
  - functions.c
  - isr.c
  - main.c
  - setup.c
  - uartbuf.c
  - utils.c
  - myfunc/
    - ads8332.c
    - analog.c
    - dac5568.c
    - display.c
    - maths.c
    - spi.c
    - Tensor.c
    - wr_eeprom.c


# Erreur à fix que je comprend pas encore :
src/main.c:222:18: error: cannot assign to variable 'time_lc_prec' with const-qualified type 'const uint32_t' (aka 'const unsigned long')
  222 |     time_lc_prec = 0;
      |     ~~~~~~~~~~~~ ^
src/main.c:122:16: note: variable 'time_lc_prec' declared const here
  122 | const uint32_t time_lc_prec;
      | ~~~~~~~~~~~~~~~^~~~~~~~~~~~

