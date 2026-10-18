#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// 0x003CF898 (name is ours)
template <>
DECOMP_NOINLINE qList<u32>::qList() : m_pBufferList(nullptr), m_pFreeList(nullptr), m_pNextAvail(nullptr), m_pLast(nullptr), m_pHead(nullptr), m_Size(0)
{
    InitializeInline();
}

// 0x008305FC (name is ours)
template <>
DECOMP_NOINLINE qList<u32>::~qList()
{
    DestroyInline();
}

// 0x0083051C (name is ours)
template <>
DECOMP_NOINLINE void qList<u32>::AddBuffer(bool isFirst)
{
    AddBufferInline(isFirst);
}

// 0x00830594 (name is ours)
template <>
DECOMP_NOINLINE void qList<u32>::clear()
{
    ClearInline();
}

// 0x00830678 (name is ours)
template <>
DECOMP_NOINLINE qList<u32>& qList<u32>::operator=(const qList<u32>& other)
{
    return AssignInline(other);
}

} // namespace nex
} // namespace nn
