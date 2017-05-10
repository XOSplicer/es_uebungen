/**
 * @author Felix Stegmaier
 */

#include "PoolAllocator.h"
#include "debug.h"

int main() {
  /* code */
  DEBUG("main");

  DEBUG("test1")
  CREATE(test1, 1024, sizeof(int));
  DEBUG("available: " << test1.Available() << "\n");
  int* al1 = static_cast<int*>(test1.Allocate(32*sizeof(int)));
  DEBUG("allocated at" << (void*)al1);
  DEBUG("available: " << test1.Available() << "\n");
  int* al2 = static_cast<int*>(test1.Allocate(64*sizeof(int)));
  DEBUG("allocated at" << (void*)al2);
  DEBUG("available: " << test1.Available() << "\n");
  test1.Deallocate(al1);
  DEBUG("available: " << test1.Available() << "\n");
  test1.Deallocate(al2);
  DEBUG("available: " << test1.Available() << "\n");

  DEBUG("test2");
  CREATE(test2, 551, sizeof(long));
  DEBUG("available: " << test2.Available());
  char* al3 = static_cast<char*>(test2.Allocate(52*sizeof(int)));
  DEBUG("available: " << test2.Available());
  test2.Deallocate(al3);
  DEBUG("available: " << test2.Available());

  return 0;
}
