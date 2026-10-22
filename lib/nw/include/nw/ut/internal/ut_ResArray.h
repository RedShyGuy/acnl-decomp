#pragma once

#include "decomp.h"
#include "nw/ut/ut_Offset.h"

namespace nw {
namespace ut {
namespace internal {
// How an element of a ResArray is made from its offset: a resource class is constructed from the
// object (the class names are from the reference symbols, the members are ours).
struct ResArrayClassTraits
{
    // (inline)
    template <typename T>
    static T Get(const Offset& offset)
    {
        return T(offset.ToPtr());
    }
};

// A range of self-relative offsets in a binary resource, seen as an array of resource classes
// (passed by value: begin and end).
template <typename T, typename TTraits>
class ResArray
{
public:
    // (inline)
    ResArray(const Offset* pBegin, int count) : m_pBegin(pBegin), m_pEnd(pBegin + count) {}
    // (inline) the number of elements
    u32 size() const
    {
        return static_cast<u32>(reinterpret_cast<uintptr_t>(m_pEnd) - reinterpret_cast<uintptr_t>(m_pBegin)) / sizeof(Offset);
    }
    // (inline) the element at index
    T operator[](int index) const { return TTraits::template Get<T>(m_pBegin[index]); }

    const Offset* m_pBegin; // 0x0
    const Offset* m_pEnd;   // 0x4
};
} // namespace internal
} // namespace ut
} // namespace nw
