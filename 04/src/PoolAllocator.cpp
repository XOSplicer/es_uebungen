#include "PoolAllocator.h"

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
  if (!deallocate_ptr) return;
  BlockInfo* first_of_chunk = find_first_of_chunk(deallocate_ptr);
  mark_chunk_as_free(first_of_chunk);

}

BlockInfo* BaseHeap::find_first_of_chunk(void* at_ptr) {
  DEBUG("Finding start of chunk for " << at_ptr);
  if (!at_ptr
      || at_ptr < m_map[0].start
      || at_ptr >= m_map[m_block_count-1].start + m_block_size) {
    return nullptr;
  }
  /* TODO make sure we are at an legit position in the block raster*/
  //at_ptr = at_ptr - (at_ptr % m_block_size);
  //DEBUG("after adjustment: " << at_ptr);
  /* scan for corresponding block of at_ptr */
  BlockInfo* at_block = nullptr;
  for(BlockInfo* block = m_map; ; block++) {
    if (block->start == at_ptr) {
      at_block = block;
      break;
    }
  }
  if  (!at_block) {
    DEBUG("Could not find corresponding block");
    return nullptr;
  }
  DEBUG("Found corresponding block at:");
  at_block->debug();
  BlockInfo* first_block_of_chunk = nullptr;
  for(BlockInfo* block = at_block; ; block--) {
    if (block->first) {
      first_block_of_chunk = block;
      break;
    }
  }
  if (!first_block_of_chunk) {
    DEBUG("Could not find frist block of chunk");
    return nullptr;
  }
  DEBUG("Found frist block of chunk at:");
  first_block_of_chunk->debug();
  return first_block_of_chunk;
}

void BaseHeap::mark_chunk_as_free(BlockInfo* first_block) {
  if(!first_block) {
    return;
  }
  DEBUG("Freeing blocks starting with:");
  first_block->debug();
  for(BlockInfo* block = first_block;
    (block->start) < (first_block->chunk_end); block++) {
      block->taken = false;
      /* handle last block, so we dont overshoot */
      if (block == &m_map[m_block_count]) {
        break;
      }
    }
}

void BaseHeap::merge_free_chunks(BlockInfo* new_free) {
  if (!new_free) {
    return;
  }
}
