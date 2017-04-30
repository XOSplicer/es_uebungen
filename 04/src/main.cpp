/**
 * @author Felix Stegmaier
 */

#include "PoolAllocator.h"
#include "debug.h"

int main() {
  /* code */
  DEBUG("main");
  CREATE(test, 1024, sizeof(int));
  return 0;
}
