#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// 0x003CF898 (name is ours)
template <>
DECOMP_NOINLINE qList<u32>::qList()
{
    // nothing to do: std::list makes the head node
}

} // namespace nex
} // namespace nn

namespace std {
// 0x008305FC (name is ours)
template <>
DECOMP_NOINLINE list<u32, nn::nex::MemAllocator<u32> >::~list()
{
    DestroyInline();
}

// 0x0083051C (name is ours)
template <>
DECOMP_NOINLINE void list<u32, nn::nex::MemAllocator<u32> >::AddBuffer(bool isFirst)
{
    AddBufferInline(isFirst);
}

// 0x00830594 (name is ours)
template <>
DECOMP_NOINLINE void list<u32, nn::nex::MemAllocator<u32> >::clear()
{
    ClearInline();
}

// 0x00830678 (name is ours)
template <>
DECOMP_NOINLINE list<u32, nn::nex::MemAllocator<u32> >& list<u32, nn::nex::MemAllocator<u32> >::operator=(const list<u32, nn::nex::MemAllocator<u32> >& other)
{
    return AssignInline(other);
}
} // namespace std
