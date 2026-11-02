#include "ssys/ma/dHeapAllocator.h"
#include "Other/dHeapUtil.h"
#include "sead/RuntimeTypeInfo/seadInterface.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFrameHeap.h"

namespace ssys {
namespace ma {
// 0x00567DC8 (name is ours)
u32 ssys::ma::HeapAllocator::Adjust()
{
    if (m_pHeap == NULL) {
        return 0;
    }
    return HeapUtil::AdjustHeap(sead::DynamicCast<sead::ExpHeap>(m_pHeap));
}

// 0x00567E50 (name is ours)
bool ssys::ma::HeapAllocator::CreateFrameHeap(u32 size, sead::Heap* parent, const sead::SafeStringBase<char>& name,
                                              sead::Heap::HeapDirection direction)
{
    if (m_pHeap != NULL) {
        m_pHeap->destroy();
        m_pHeap = NULL;
    }
    sead::FrameHeap* heap = sead::FrameHeap::create(size, name, parent, direction, false);
    if (heap == NULL) {
        return false;
    }
    m_pHeap = heap;
    return true;
}

// 0x00567EB0
ssys::ma::HeapAllocator::HeapAllocator()
{
    // (only the base is constructed)
}

// 0x00317FD4 slot 0x00
// 0x00567EC8 (deleting dtor)
ssys::ma::HeapAllocator::~HeapAllocator()
{
    if (m_pHeap != NULL) {
        m_pHeap->destroy();
        m_pHeap = NULL;
    }
}

} // namespace ma
} // namespace ssys
