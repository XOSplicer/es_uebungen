/**
 * @author Felix Stegmaier
 */

#include "Printf.h"
#include "debug.h"

int main() {
  char buffer[256] = {62};
  Printf(buffer, buffer+256, "hello %u asd");
  DEBUG(buffer);
  return 0;
}
