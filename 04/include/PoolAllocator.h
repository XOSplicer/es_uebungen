/**
 * @author Felix Stegmaier
 */

#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include "util.h"
#include "debug.h"

class IHeap {
  public:
    virtual void* Allocate(size_t sizeInBytes) = 0;
    virtual void Deallocate(void*) = 0;
    /* Returns remaining # of available bytes */
    virtual size_t Available() const = 0;
};

#define CREATE(varName, blockCount, blockSize )\
  PoolAllocator<blockCount, blockSize> varName;

struct BlockInfo {
  bool taken;
  bool first;
  uint8_t* start;
};

class BaseHeap
  : public IHeap {
  public:
    BaseHeap(uint8_t* memory,  BlockInfo* blockmap, size_t block_count, size_t block_size)
    : m_mem(memory),
      m_map(blockmap),
      m_block_count(block_count),
      m_block_size(block_size) {
        DEBUG("New BaseHeap");
        for (size_t block = 0; block < block_count; block++) {
          m_map[block].taken = false;
          m_map[block].first = false;
          m_map[block].start = memory + (block * block_size);
          DEBUG("Memory Block " << block << " at " << (void*) m_map[block].start);
        }
        m_map[0].first = true;
      }

    void* Allocate(size_t sizeInBytes);
    void Deallocate(void*);
    /* Returns remaining # of available bytes */
    size_t Available() const;

  protected:

  private:
    /* bytewise memory, will by only given out as blocks of blocksize */
    uint8_t* m_mem;
    /* bitmap of blocks, true=alocated, false=free */
    BlockInfo* m_map;
    size_t m_block_count;
    size_t m_block_size;

};

template<size_t BLOCKCOUNT, size_t BLOCKSIZE>
class PoolAllocator
  : public BaseHeap {
  public:
    PoolAllocator()
    : BaseHeap(m_mem, m_blockmap, BLOCKCOUNT, BLOCKSIZE) {
      DEBUG("New PoolAllocator <" << BLOCKCOUNT << ", " << BLOCKSIZE << ">");
    }

  private:
    uint8_t m_mem[BLOCKCOUNT * BLOCKSIZE] = {0};
    BlockInfo m_blockmap[BLOCKCOUNT];
};


#endif
