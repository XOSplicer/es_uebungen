#include "PoolAllocator.h"

void* BaseHeap::Allocate(size_t /*sizeInBytes*/) {
  return nullptr;
}

void BaseHeap::Deallocate(void*) {

}
/* Returns remaining # of available bytes */
size_t BaseHeap::Available() const {
  size_t free_blocks = 0;
  for (size_t block = 0; block < m_block_count; block++) {
    if (!m_map[block].taken) {
      free_blocks++;
    }
  }
  return free_blocks * m_block_size;
}
