#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_HeapBase.h"

namespace nn {
namespace fnd {
namespace detail {
struct ExpHeapImpl;
}

// RTTI N2nn3fnd11ExpHeapBaseE @ 0x008CDE40
// vtable 0x008FC030 (vptr 0x008FC038), offset_to_top 0, 7 entries
//
// A heap of blocks of any size. The block management (nn::fnd::detail::ExpHeapImpl at +0x18) is not
// worked out yet; the members are ours.
class ExpHeapBase : public ::nn::fnd::HeapBase
{
public:
    // how a block is searched (values ours, 0 is used)
    enum AllocationMode : u8 {
        ALLOCATION_MODE_FIRST_FIT = 0,
    };

    // inline (e.g. in the static initializer of the socket heap)
    ExpHeapBase() : mImplHead(0), mAllocationCount(0) {}
    virtual ~ExpHeapBase(); // 0x003523A4 slot 0x00, 0x00352360 slot 0x04 (deleting)
    virtual void FreeV(void* p); // 0x00352340 slot 0x08
    virtual void* GetHeapStart() const { return reinterpret_cast<void*>(mHeapStart); } // 0x00729710 slot 0x0C (name is ours)
    virtual size_t GetHeapSize() const { return mHeapEnd - mHeapStart; } // 0x00729700 slot 0x10 (name is ours)
    virtual void PrintState() {} // 0x0072971C slot 0x14 (name is ours)
    virtual bool HasAddress(const void* p) const { return mHeapStart <= reinterpret_cast<uptr>(p) && reinterpret_cast<uptr>(p) < mHeapEnd; } // 0x007296E0 slot 0x18 | fefates:bytes

    void Initialize(uptr address, size_t size, bit32 option); // 0x0011FF0C | nintendogs:bytes [tier A]
    void Invalidate(); // 0x001368EC | nintendogs:bytes [tier A]
    void* Allocate(size_t size, s32 alignment, u8 groupId, AllocationMode mode, bool useMargin); // 0x00136918 | nintendogs:bytes [tier A]
    void Finalize(); // 0x0013697C | nintendogs:bytes [tier A]
    void Free(void* p); // 0x0013EC84 | nintendogs:callgraph [tier A]

protected:
    // nn::fnd::detail::ExpHeapImpl (0x18 to 0x53), kept as words
    uptr mImplHead;             // 0x18
    uptr mImpl1C[5];            // 0x1C
    uptr mHeapStart;            // 0x30
    uptr mHeapEnd;              // 0x34
    uptr mImpl38[7];            // 0x38
    s32 mAllocationCount;       // 0x54
};
ASSERT_SIZE(ExpHeapBase, 0x58);
} // namespace fnd
} // namespace nn
