#include "nn/fnd/detail/detail_Api.h"

namespace nn {
namespace fnd {
namespace detail {
// 0x001245F8 | nintendogs:bytes [tier A]
void CreateHeap(nn::fnd::detail::ExpHeapImpl*, void*, unsigned, unsigned short)
{
}

// 0x00129FD0 | nintendogs:bytes [tier A]
void NNSi_FndInitHeapHead(nn::fnd::detail::ExpHeapImpl*, unsigned, void*, void*, unsigned short)
{
}

// 0x00130CBC | nintendogs:bytes [tier A]
void AllocFromHeap(nn::fnd::detail::ExpHeapImpl*, unsigned, int)
{
}

// 0x00130E04 | nintendogs:bytes [tier A]
void AppendListObject(nn::fnd::detail::NNSFndList*, void*)
{
}

// 0x00130E6C | nintendogs:callgraph [tier A]
void SetGroupIDForHeap(nn::fnd::detail::ExpHeapImpl*, unsigned short)
{
}

// 0x00130E7C | nintendogs:bytes [tier A]
void SetAllocModeForHeap(nn::fnd::detail::ExpHeapImpl*, unsigned short)
{
}

// 0x00130E9C | nintendogs:callgraph [tier A]
void UseMarginOfAlignmentForHeap(nn::fnd::detail::ExpHeapImpl*, bool)
{
}

// 0x00130EAC | nintendogs:bytes [tier A]
void InitList(nn::fnd::detail::NNSFndList*, unsigned short)
{
}

// 0x00136A44 | fefates:bytes [tier B]
void AllocUsedBlockFromFreeBlock(nn::fnd::detail::NNSiFndExpHeapHead*, nn::fnd::detail::NNSiFndExpHeapMBlockHead*, void*, unsigned int, unsigned short)
{
}

// 0x0013B454 | fefates:bytes [tier B]
void FindContainHeap(nn::fnd::detail::NNSFndList*, const void*)
{
}

// 0x0013B4AC | nintendogs:bytes [tier A]
void GetNextListObject(const nn::fnd::detail::NNSFndList*, const void*)
{
}

// 0x0013ECA4 | nintendogs:bytes-fuzzy [tier A]
void FreeToHeap(nn::fnd::detail::ExpHeapImpl*, void*)
{
}

// 0x0013EDE0 | nintendogs:callgraph [tier A]
void DestroyHeap(nn::fnd::detail::ExpHeapImpl*)
{
}

// 0x0013EDE4 | nintendogs:bytes [tier A]
void NNSi_FndFinalizeHeap(nn::fnd::detail::ExpHeapImpl*)
{
}

// 0x00140A28 | nintendogs:bytes [tier A]
void RemoveListObject(nn::fnd::detail::NNSFndList*, void*)
{
}

} // namespace detail
} // namespace fnd
} // namespace nn
