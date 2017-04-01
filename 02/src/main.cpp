/**
 * @author Felix Stegmaier
 */

#include "Printf.h"
#include "debug.h"

int main() {
  char buffer[256] = {0};
  char small_buffer[32];
  int ret = Printf(buffer, buffer+256,
                  "verbatim text %d, %u, %c, %s, %x, %b, %%",
                   -4316, 2958, '&', "some string", 0xdeadbeef, 0xdeadbeef)
                   ? 0 : 1;
  DEBUG(buffer);
  ret |= Printf(small_buffer, small_buffer+32,
                "%%d%%%d",
                -123)
                ? 0 : 1;
  DEBUG(small_buffer);
  /* should fail, ret swapped */
  ret |= Printf(small_buffer, small_buffer+32,
                "%s",
                "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
                ? 1 : 0;
  DEBUG(small_buffer);
  return ret;
}
