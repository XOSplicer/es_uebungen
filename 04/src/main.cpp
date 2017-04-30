/**
 * @author Felix Stegmaier
 */

#include "PoolAllocator.h"
#include "debug.h"

int main() {
  /* code */
  DEBUG("main");
  CREATE(test, 1024, sizeof(int));
  DEBUG("available: " << test.Available());
  return 0;
}
