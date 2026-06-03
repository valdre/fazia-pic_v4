#ifndef _UTILS_H
#define _UTILS_H

/**
 * @file utils.h
 * @brief Generic utility functions for PIC_V4.
 */

/**
 * @brief Convert a hexadecimal ASCII string to an unsigned integer.
 * @param ptr Null-terminated ASCII hexadecimal string.
 * @return Parsed unsigned integer value.
 */
unsigned int htoi(const char *ptr);

#endif

// EOF
