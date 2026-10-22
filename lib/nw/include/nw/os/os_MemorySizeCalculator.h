#pragma once

#include "decomp.h"

namespace nw {
namespace os {
// Adds up the sizes of the blocks a later setup allocates, each aligned (the class name is from the
// reference symbols, members ours).
class MemorySizeCalculator
{
public:
    // (inline)
    void Add(u32 size)
    {
        m_Size = ((m_Size + m_Alignment - 1) & ~(m_Alignment - 1)) + size;
    }
    u32 GetSize() const { return m_Size; }
    u32 GetAlignment() const { return m_Alignment; }
    // the alignment of the next blocks (the largest one is remembered)
    void SetAlignment(u32 alignment)
    {
        m_Alignment = alignment;
        if (m_MaxAlignment < alignment) {
            m_MaxAlignment = alignment;
        }
    }

    u32 m_Size;         // 0x0
    u32 m_Alignment;    // 0x4
    u32 m_MaxAlignment; // 0x8
};
ASSERT_SIZE(MemorySizeCalculator, 0xC);
} // namespace os
} // namespace nw
