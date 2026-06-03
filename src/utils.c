#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Convert a hexadecimal ASCII string to a 16-bit unsigned integer.
 * @param ptr Hexadecimal ASCII string
 * @return unsigned int status or result code.
 */
unsigned int htoi(const char *ptr) {

   unsigned int value = 0;
   char ch = *ptr;

   while (ch == ' ' || ch == '\t')
      ch = *(++ptr);

   for (;;) {

      if (ch >= '0' && ch <= '9')
         value = (value << 4) + (ch - '0');
      else if (ch >= 'A' && ch <= 'F')
         value = (value << 4) + (ch - 'A' + 10);
      else if (ch >= 'a' && ch <= 'f')
         value = (value << 4) + (ch - 'a' + 10);
      else
         return value;
      ch = *(++ptr);
   }
}

// EOF
