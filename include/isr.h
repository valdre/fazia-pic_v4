#ifndef _UARTINT_H_
#define _UARTINT_H_

/**
 * @file isr.h
 * @brief Interrupt service routine declarations.
 */

/**
 * @brief Timer0 interrupt service routine.
 * @warning This declaration corresponds to the interrupt handler installed in the vector table.
 */
void Timer0_isr(void);

#endif

// EOF
