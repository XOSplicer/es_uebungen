#include "PoolAllocator.h"

void* BaseHeap::Allocate(size_t /*sizeInBytes*/) {
  return nullptr;
}

void BaseHeap::Deallocate(void*) {

}
/* Returns remaining # of available bytes */
size_t BaseHeap::Available() const {
  return 0;
}
