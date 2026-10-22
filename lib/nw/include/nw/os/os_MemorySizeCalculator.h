#pragma once

#include "decomp.h"

namespace nw {
namespace os {
// Adds up the sizes of the blocks a later setup allocates, each aligned (the class name is from the
// reference symbols, members ours).
class MemorySizeCalculator
{
public:
    // (inline) an empty calculator for blocks of the alignment
    explicit MemorySizeCalculator(u32 alignment) : m_Size(0), m_Alignment(alignment), m_MaxAlignment(alignment) {}

    void Add(u32 size)
    {
        m_Size = ((m_Size + m_Alignment - 1) & ~(m_Alignment - 1)) + size;
    }
    // (inline) the blocks of the other calculator as one block (with room to align it; name is ours)
    void Add(const MemorySizeCalculator& other) { Add(other.m_Size + other.m_MaxAlignment - m_Alignment); }
    // (inline) the blocks count times (name is ours)
    MemorySizeCalculator& operator*=(int count)
    {
        m_Size = ((m_Size + m_Alignment - 1) & ~(m_Alignment - 1)) * count;
        return *this;
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
