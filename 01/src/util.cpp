/**
 * @author Felix Stegmaier
 */

#include "util.h"

/**
 * simple strlen substitute
 * @param  str the string, \0 terminated
 * @return     the length, - for NULL
 */
int util::strlen(const char* str) {
  if(!str) {
    return -1;
  }
  /* dont stop, may segfault or run forever */
  for(const char* char_ptr = str;  ; ++char_ptr) {
    if ('\0' == *char_ptr) {
      return char_ptr - str;
    }
  }
}
