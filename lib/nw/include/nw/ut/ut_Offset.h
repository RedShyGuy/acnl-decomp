#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
// A pointer in a binary resource, stored as the distance from the field itself (0: none). Copying
// one to another place recomputes the distance (names are ours).
class Offset
{
public:
    // (inline)
    const void* ToPtr() const { return m_Offset != 0 ? reinterpret_cast<const u8*>(this) + m_Offset : nullptr; }
    void* ToPtr() { return m_Offset != 0 ? reinterpret_cast<u8*>(this) + m_Offset : nullptr; }
    template <typename T>
    const T* To() const
    {
        return static_cast<const T*>(ToPtr());
    }
    template <typename T>
    T* To()
    {
        return static_cast<T*>(ToPtr());
    }
    void Set(const void* ptr)
    {
        m_Offset = ptr != nullptr ? reinterpret_cast<const u8*>(ptr) - reinterpret_cast<const u8*>(this) : 0;
    }
    Offset& operator=(const Offset& other)
    {
        Set(other.ToPtr());
        return *this;
    }

    s32 m_Offset; // 0x0
};
ASSERT_SIZE(Offset, 0x4);
} // namespace ut
} // namespace nw
