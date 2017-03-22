/**
 * @author Felix Stegmaier
 */

#include "Printf.h"
#include "debug.h"

int main() {
  char buffer[256] = {0};
  Printf(buffer, buffer+256, "hello");
  DEBUG(buffer);
  return 0;
}
