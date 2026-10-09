#include "nn/fnd/fnd_HeapBase.h"
#include "nn/dbg/dbg_Api.h"

namespace nn {
namespace fnd {

// 0x00136C28 | nintendogs:bytes [tier A]
void nn::fnd::HeapBase::FillMemory32(uptr begin, uptr end, bit32 value)
{
    for (; begin != end; begin += sizeof(bit32)) {
        *reinterpret_cast<bit32*>(begin) = value;
    }
}

// 0x0013B4C4 | nintendogs:bytes [tier A]
nn::fnd::HeapBase::~HeapBase()
{
    // the heap must be out of the tree
    if (mParent != 0) {
        nndbgPanic();
    }
    if (!mChildren.IsEmpty()) {
        nndbgPanic();
    }
}

} // namespace fnd
} // namespace nn
