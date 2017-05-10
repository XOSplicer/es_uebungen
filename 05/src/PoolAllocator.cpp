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
  BlockInfo* first_of_chunk = find_first_fit(sizeInBytes);
  split_chunk(first_of_chunk, blocks_to_allocate);
  mark_chunk_as(first_of_chunk, true);
  return first_of_chunk->start;
}

BlockInfo* BaseHeap::find_first_fit(size_t bytes) {
  DEBUG("Searching for chunk with at least " << bytes << " bytes");
  if (0 == bytes) {
    return nullptr;
  }
  for (BlockInfo* block = m_map; block!=&m_map[m_block_count-1]; block++) {
    if (block->first
        && ((block->chunk_end) - (block->start) >= static_cast<int>(bytes))) {
      DEBUG("Found chunk starting at:");
      block->debug();
      return block;
    }
  }
  DEBUG("No fitting block found");
  return nullptr;
}

void BaseHeap::split_chunk(BlockInfo* first_block, size_t blocks_in_first) {
  if (!first_block
      || !blocks_in_first
      || !(first_block->first)
      || !(first_block->chunk_end)
      || (first_block + blocks_in_first) >= &m_map[m_block_count-1]) {
    return;
  }
  BlockInfo* next_chunk = first_block + blocks_in_first;
  next_chunk->first = true;
  next_chunk->chunk_end = first_block->chunk_end;
  first_block->chunk_end = next_chunk->start;
}

void BaseHeap::Deallocate(void* deallocate_ptr) {
  DEBUG("Deallocating of " << deallocate_ptr);
  if (!deallocate_ptr) return;
  BlockInfo* first_of_chunk = find_first_of_chunk(deallocate_ptr);
  mark_chunk_as(first_of_chunk, false);
  merge_free_chunks(first_of_chunk);
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

void BaseHeap::mark_chunk_as(BlockInfo* first_block, bool taken) {
  if(!first_block
    || !(first_block->first)
    || !(first_block->chunk_end)) {
    return;
  }
  DEBUG("Marking blocks as taken=" << taken << " starting with:");
  first_block->debug();
  for(BlockInfo* block = first_block;
    (block->start) < (first_block->chunk_end); block++) {
      block->taken = taken;
      /* handle last block, so we dont overshoot */
      if (block == &m_map[m_block_count-1]) {
        break;
      }
    }
}

void BaseHeap::merge_free_chunks(BlockInfo* middle_first) {
  merge_free_chunks_right(middle_first);
  merge_free_chunks_left(middle_first);
}

void BaseHeap::merge_free_chunks_right(BlockInfo* middle_first) {
  if (!middle_first
      || !(middle_first->first)
      || !(middle_first->chunk_end)) {
    return;
  }
  DEBUG("Merging right free starting at:");
  middle_first->debug();
  /* last chunk can not be merged on the right */
  if ((middle_first->chunk_end) > m_map[m_block_count-1].start) {
    DEBUG("Cant merge free right, last chunk");
    return;
  }
  BlockInfo* first_of_next_chunk = find_first_of_chunk(middle_first->chunk_end);
  if (!first_of_next_chunk) {
    DEBUG("Cant merge free right, no right chunk found");
    return;
  }
  if (first_of_next_chunk->taken) {
    DEBUG("Cant merge right, chunk not free");
    return;
  }
  DEBUG("Merging free right with:");
  first_of_next_chunk->debug();
  /* perform the actual merge */
  middle_first->chunk_end = first_of_next_chunk->chunk_end;
  first_of_next_chunk->first = false;
  first_of_next_chunk->chunk_end = nullptr;
}

void BaseHeap::merge_free_chunks_left(BlockInfo* middle_first) {
  if (!middle_first
      || !(middle_first->first)
      || !(middle_first->chunk_end)) {
    return;
  }
  DEBUG("Merging right free starting at:");
  middle_first->debug();
  /* first chunk can not be merged on the left */
  if (middle_first == m_map) {
    DEBUG("Cant merge left free, first chunk");
    return;
  }
  BlockInfo* first_of_prev_chunk = find_first_of_chunk((middle_first->start) - m_block_size);
  if (!first_of_prev_chunk) {
    DEBUG("Cant merge left, no left chunk found");
    return;
  }
  if (first_of_prev_chunk->taken) {
    DEBUG("Cant merge left, chunk not free");
    return;
  }
  DEBUG("Merging free left with:");
  first_of_prev_chunk->debug();
  /* perform the actual merge */
  first_of_prev_chunk->chunk_end = middle_first->chunk_end;
  middle_first->first = false;
  middle_first->chunk_end = nullptr;
}
