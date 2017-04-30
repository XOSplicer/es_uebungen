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
  uint8_t* chunk_end; /* start of next taken chunk of blocks,
                        only valid if first is set */
  void debug() {
    DEBUG("BlockInfo[taken=" << taken
            << ", first=" << first
            << ", start=" << (void*) start
            << ", chunk_end=" << (void*) chunk_end <<"]");
  }
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
          m_map[block].chunk_end = nullptr;
          //m_map[block].debug();
        }
        m_map[0].first = true;
        m_map[0].chunk_end = memory + block_count * block_size;

        DEBUG("first chunk:");
        m_map[0].debug();
        DEBUG("last block:");
        m_map[block_count-1].debug();
      }

    void* Allocate(size_t sizeInBytes);
    void Deallocate(void*);
    /* Returns remaining # of available bytes */
    size_t Available() const;

  protected:

  private:
    /* bytewise memory, will by only given out as blocks of blocksize */
    uint8_t* const m_mem;
    /* bitmap of blocks, true=alocated, false=free */
    BlockInfo* const m_map;
    const size_t m_block_count;
    const size_t m_block_size;

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
