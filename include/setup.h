#ifndef _SETUP_H
#define _SETUP_H

/**
 * @file setup.h
 * @brief Board initialization functions for PIC_V4.
 */

/**
 * @brief Configure I/O pins, timers, and board-specific hardware defaults.
 */
void ucsetup(void);

/**
 * @brief Initialize persistent memory structures and EEPROM-derived calibration parameters.
 */
void memsetup(void);

#endif

// EOF
