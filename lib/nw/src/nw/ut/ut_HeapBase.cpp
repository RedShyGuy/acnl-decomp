#include <cstring>

#include "nw/ut/ut_HeapBase.h"

namespace nw {
namespace ut {
namespace {
// the heaps that are not inside another heap (built by the static initializer)
// 0x00AF63C0 (name is ours)
HeapBase::HeapList s_RootList;

// the list the heap belongs to: the child list of the heap that contains it, or the root list
// (inline; name is ours)
HeapBase::HeapList* FindListContainHeap(HeapBase* pHeap)
{
    HeapBase::HeapList* pList = &s_RootList;
    HeapBase* pContainHeap = HeapBase::FindContainHeap(&s_RootList, pHeap);
    if (pContainHeap != nullptr) {
        pList = &pContainHeap->m_ChildList;
    }
    return pList;
}
} // namespace

// the debug fill values (OPTION_DEBUG_FILL), by FillType (in .data: they can be changed, the
// setter is not in the game)
// 0x009828B0 (name is ours)
u32 g_HeapFillValues[HeapBase::FILL_TYPE_COUNT] = {0xC3C3C3C3, 0xF3F3F3F3, 0xD3D3D3D3};

// constructs s_RootList
// 0x007931F4 (static initializer)

// 0x0048BA90 slot 0x00
// 0x0048BA80 (deleting dtor)
nw::ut::HeapBase::~HeapBase()
{
    // empty (in the original too)
}

// 0x0013EE80 | nintendogs:bytes [tier A]
void nw::ut::HeapBase::Initialize(u32 signature, void* pHeapStart, void* pHeapEnd, u16 option)
{
    m_Option = static_cast<u8>(option);
    m_Signature = signature;
    m_pHeapStart = pHeapStart;
    m_pHeapEnd = pHeapEnd;
    if ((m_Option & OPTION_DEBUG_FILL) != 0) {
        std::memset(pHeapStart, g_HeapFillValues[FILL_TYPE_NOT_USED], static_cast<u8*>(pHeapEnd) - static_cast<u8*>(pHeapStart));
    }
    HeapList* pList = FindListContainHeap(this);
    pList->PushBack(this);
}

// 0x00140E78 | nintendogs:bytes [tier A]
void nw::ut::HeapBase::FillFreeMemory(void* pAddress, u32 size)
{
    if ((m_Option & OPTION_DEBUG_FILL) != 0) {
        std::memset(pAddress, g_HeapFillValues[FILL_TYPE_FREE], size);
    }
}

// 0x00140EA4 | nintendogs:bytes [tier A]
void nw::ut::HeapBase::FillAllocMemory(void* pAddress, u32 size)
{
    if ((m_Option & OPTION_CLEAR_ALLOC_MEMORY) != 0) {
        std::memset(pAddress, 0, size);
    } else if ((m_Option & OPTION_DEBUG_FILL) != 0) {
        std::memset(pAddress, g_HeapFillValues[FILL_TYPE_ALLOC], size);
    }
}

// 0x0014387C | nintendogs:bytes [tier A]
DECOMP_NOINLINE nw::ut::HeapBase* nw::ut::HeapBase::FindContainHeap(nw::ut::LinkList<nw::ut::HeapBase, (long)4>* pList, const void* pMemory)
{
    for (HeapList::iterator it = pList->begin(); it != pList->end(); ++it) {
        HeapBase* pHeap = HeapList::GetPointerFromNode(it.GetNode());
        if (pHeap->m_pHeapStart <= pMemory && pMemory < pHeap->m_pHeapEnd) {
            HeapBase* pChild = FindContainHeap(&pHeap->m_ChildList, pMemory);
            if (pChild != nullptr) {
                return pChild;
            }
            return pHeap;
        }
    }
    return nullptr;
}

// 0x001438D4 | nintendogs:bytes [tier A]
void nw::ut::HeapBase::Finalize()
{
    HeapList* pList = FindListContainHeap(this);
    pList->Erase(this);
    m_Signature = 0;
}

} // namespace ut
} // namespace nw
