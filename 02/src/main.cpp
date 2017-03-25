/**
 * @author Felix Stegmaier
 */

#include "Printf.h"
#include "debug.h"

int main() {
  char buffer[256] = {0};
  int ret = Printf(buffer, buffer+256,
                  "verbatim text %d, %u, %c, %s, %x, %b, %%",
                   -4316, 2958, '&', "some string", 0xdeadbeef, 0xdeadbeef)
                   ? 0 : 1;
  DEBUG(buffer);
  return ret;
}
