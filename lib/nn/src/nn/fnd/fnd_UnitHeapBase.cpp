#include "nn/fnd/fnd_UnitHeapBase.h"

namespace nn {
namespace fnd {

// 0x00352428 slot 0x00
// 0x003523FC slot 0x04 (deleting dtor)
nn::fnd::UnitHeapBase::~UnitHeapBase()
{
    Finalize();
}

// 0x003523E0 slot 0x08 | mk7dlp:bytes
void nn::fnd::UnitHeapBase::FreeV(void* p)
{
    FreeUnit(p);
}

// 0x001369A4 | nintendogs:bytes [tier A]
void nn::fnd::UnitHeapBase::Initialize(size_t unitSize, uptr address, size_t size, s32 alignment, bit32 option)
{
    mOption = option;
    mUnitSize = (unitSize + alignment - 1) / alignment * alignment;
    mHeapStart = (address + alignment - 1) / alignment * alignment;
    mHeapSize = (address + size - mHeapStart) / mUnitSize * mUnitSize;
    mUsedCount = 0;
    mAlignment = alignment;
    // the list runs from the first unit to the last
    void* head = 0;
    for (uptr unit = mHeapStart + mHeapSize - mUnitSize; mHeapStart <= unit; unit -= mUnitSize) {
        *reinterpret_cast<void**>(unit) = head;
        head = reinterpret_cast<void*>(unit);
    }
    mFreeList = head;
}

} // namespace fnd
} // namespace nn
