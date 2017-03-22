/**
 * @author Felix Stegmaier
 */

#include "Printf.h"

/**
 * Minimal printf implementation
 * Support of
 *  (0) Varbatim text
 *  (1) %d for signed int
 *  (2) %u for unsigned int
 *  (3) %c for a single char
 *  (4) %s for a null terminated string
 *  (5) %x for hexadecimal TODO signed ? raw ?
 *  (6) %b for binary TODO signed ? raw ?
 *  (7) %% for char '%'
 *
 * @param  dst     pointer to the buffer to write the formated string to
 * @param  end     pointer to the first byte after the buffer
 * @param  fmt     format string to be used
 * @param  VARARGS format value arguments
 * @return         pointer to the first writable char (after '\0')
 *                 nullptr if any argument is not valid
 */
char* Printf(char* dst, const void* end, const char* fmt, ...) {
  if (!dst
      || !end
      || !fmt
      || end < dst) {
    return nullptr;
    }
  return nullptr;
}
