/**
 * @author Felix Stegmaier
 */

#include "Printf.h"
#include "debug.h"

int main() {
  char buffer[256] = {0};
  int ret = Printf(buffer, buffer+256, "hello %% %s asd", "yop") ? 0 : 1;
  DEBUG(buffer);
  return ret;
}
