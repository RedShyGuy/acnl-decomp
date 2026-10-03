#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_HeapBase.h"

namespace nn {
namespace fnd {
// RTTI N2nn3fnd12UnitHeapBaseE @ 0x008CDE4C
// vtable 0x008FC054 (vptr 0x008FC05C), offset_to_top 0, 7 entries
//
// A heap of units of one size: the free units form a list through their first word.
// Member names are ours.
class UnitHeapBase : public ::nn::fnd::HeapBase
{
public:
    UnitHeapBase() : mFreeList(0) {}
    virtual ~UnitHeapBase();
    virtual void FreeV(void* p);
    // inline (out-of-line copies for the vtable; the names of slots 0x0C to 0x14 are ours)
    virtual void* GetHeapStart() const { return reinterpret_cast<void*>(mHeapStart); } // 0x00729750 slot 0x0C
    virtual size_t GetHeapSize() const { return mHeapSize; } // 0x00729748 slot 0x10
    virtual void PrintState() {} // 0x00729758 slot 0x14
    virtual bool HasAddress(const void* p) const { return mHeapStart <= reinterpret_cast<uptr>(p) && reinterpret_cast<uptr>(p) < mHeapStart + mHeapSize; } // 0x00729720 slot 0x18

    // the units are unitSize rounded up to alignment, from address rounded up to alignment
    void Initialize(size_t unitSize, uptr address, size_t size, s32 alignment, bit32 option); // 0x001369A4 | nintendogs:bytes [tier A]

protected:
    // inline (in the destructors; name is ours)
    void Finalize()
    {
        if (mFreeList != 0) {
            mFreeList = 0;
        }
    }

    // takes a unit from the free list, 0 if there is none (inline; name is ours)
    void* AllocateUnit()
    {
        void* unit = mFreeList;
        if (unit != 0) {
            mFreeList = *static_cast<void**>(unit);
            mUsedCount++;
            if (mOption & OPTION_ZERO_CLEAR) {
                FillMemory32(reinterpret_cast<uptr>(unit), reinterpret_cast<uptr>(unit) + mUnitSize, 0);
            }
        }
        return unit;
    }

    // puts a unit back (inline; name is ours)
    void FreeUnit(void* unit)
    {
        *static_cast<void**>(unit) = mFreeList;
        mFreeList = unit;
        mUsedCount--;
    }

    size_t mUnitSize;   // 0x18
    uptr mHeapStart;    // 0x1C
    size_t mHeapSize;   // 0x20
    void* mFreeList;    // 0x24
    s32 mAlignment;     // 0x28
    s32 mUsedCount;     // 0x2C
};
ASSERT_SIZE(UnitHeapBase, 0x30);
} // namespace fnd
} // namespace nn
