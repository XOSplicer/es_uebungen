#include "PoolAllocator.h"

void* BaseHeap::Allocate(size_t sizeInBytes) {
  size_t blocks_to_allocate = CEIL_DIV(sizeInBytes, m_block_size);
  DEBUG("Allocating " << sizeInBytes << " bytes, "
          << blocks_to_allocate << " blocks");
  if (0 == sizeInBytes) {
    return nullptr;
  }
  for (size_t block = 0; block < m_block_count; block++) {
    /*scan for large enough free block, first fit */
    if ((m_map[block].first)
        && (static_cast<size_t>((m_map[block].chunk_end - m_map[block].start)) >= sizeInBytes)) {
      DEBUG("Found chunk at:");
      m_map[block].debug();
      /* mark following as taken */
      for (size_t taken_block = block; taken_block < blocks_to_allocate; taken_block++) {
        m_map[taken_block].taken = true;
      }
      /* set next block as first of next free chunk */
      if (!(m_map[block + blocks_to_allocate].first)) {
        m_map[block + blocks_to_allocate].first = true;
        m_map[block + blocks_to_allocate].chunk_end = nullptr;
        for (size_t next_first_block = block + blocks_to_allocate + 1; next_first_block < m_block_count; next_first_block++) {
          if (m_map[next_first_block].first) {
            m_map[block + blocks_to_allocate].chunk_end
              = m_map[next_first_block].start;
              DEBUG("Next chunk at:")
              m_map[next_first_block].debug();
              break; /* inner loop */
          }
        }
      }
      return m_map[block].start;
    }
  }
  return nullptr;
}

void BaseHeap::Deallocate(void* deallocate_ptr) {
  DEBUG("Deallocating at " << deallocate_ptr);
  for (size_t block = 0; block < m_block_count; block++) {
    /* scan for corresponding block */
    if (deallocate_ptr == m_map[block].start) {
      size_t first_in_chunk = block;
      /* find start of allocated chunk */
      while (!(m_map[first_in_chunk].first)) {
        first_in_chunk--;
      }
      m_map[first_in_chunk].taken = false;
      /* set to blocks in chunk to free */
      //TODO handle last
      for (size_t free_block = first_in_chunk+1; !(m_map[free_block].first); free_block++) {
        m_map[free_block].taken = false;
      }

      /* TODO merge with possible before and after free chunk */

      return;
    }
  }
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
