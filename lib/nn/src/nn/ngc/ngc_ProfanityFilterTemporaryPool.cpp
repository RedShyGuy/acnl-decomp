#include "nn/ngc/ngc_ProfanityFilterTemporaryPool.h"
#include <string.h>

namespace nn {
namespace ngc {
// 0x003DFD50 | fefates:bytes [tier B]
void nn::ngc::ProfanityFilterTemporaryPool::Initialize(uptr buffer)
{
    m_UsedMap = reinterpret_cast<u8*>(buffer);
    m_Units = reinterpret_cast<u8*>(buffer + USED_MAP_SIZE);
    m_NextIndex = 0;
    memset(reinterpret_cast<void*>(buffer), 0, USED_MAP_SIZE);
}

// 0x003DFD70 | fefates:bytes [tier B]
void nn::ngc::ProfanityFilterTemporaryPool::Free(void* buffer, u32 unitCount)
{
    u32 index = (reinterpret_cast<uptr>(buffer) - reinterpret_cast<uptr>(m_Units)) / UNIT_SIZE;
    for (u32 i = index; i < index + unitCount; i++) {
        m_UsedMap[i] = 0;
    }
}

namespace {
// unitCount free units in a row from the units begin to end (name is ours)
DECOMP_ALWAYS_INLINE void* AllocateIn(ProfanityFilterTemporaryPool* pool, u32 begin, u32 end, u32 unitCount)
{
    u8* usedMap = pool->m_UsedMap;
    for (u32 index = begin; index < end; index++) {
        u32 count = 0;
        while (index + count < end && usedMap[index + count] == 0) {
            count++;
            if (count == unitCount) {
                for (u32 i = index; i < index + unitCount; i++) {
                    usedMap[i] = 1;
                }
                pool->m_NextIndex = index + unitCount;
                return pool->m_Units + index * ProfanityFilterTemporaryPool::UNIT_SIZE;
            }
        }
        index += count;
    }
    return NULL;
}
} // namespace

// 0x003DFDA4 | fefates:bytes [tier B]
void* nn::ngc::ProfanityFilterTemporaryPool::Allocate(u32 unitCount)
{
    void* buffer = AllocateIn(this, m_NextIndex, UNIT_COUNT, unitCount);
    if (buffer == NULL) {
        buffer = AllocateIn(this, 0, m_NextIndex, unitCount);
    }
    return buffer;
}

// 0x003DFEE4 | fefates:callgraph [tier C]
nn::ngc::ProfanityFilterTemporaryPool::ProfanityFilterTemporaryPool() : m_UsedMap(NULL), m_Units(NULL), m_NextIndex(0)
{
}

} // namespace ngc
} // namespace nn
