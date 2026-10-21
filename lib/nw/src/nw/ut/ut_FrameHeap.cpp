#include <new>

#include "nw/ut/ut_FrameHeap.h"

namespace nw {
namespace ut {
namespace {
// (names are ours)
inline uptr RoundUp(uptr value, u32 alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}
inline uptr RoundDown(uptr value, u32 alignment)
{
    return value & ~(alignment - 1);
}
} // namespace

// (the lock calls of the heap functions are empty in the release build; armlink replaced them by
// nops)

// 0x0013B76C | nintendogs:bytes [tier A]
nw::ut::FrameHeap* nw::ut::FrameHeap::Create(void* pStartAddress, u32 size, u16 option)
{
    uptr end = RoundDown(reinterpret_cast<uptr>(pStartAddress) + size, 4);
    uptr start = RoundUp(reinterpret_cast<uptr>(pStartAddress), 4);
    if (static_cast<s32>(start - end) > 0 || end - start < sizeof(FrameHeap)) {
        return nullptr;
    }
    FrameHeap* pHeap = ::new (reinterpret_cast<void*>(start)) FrameHeap();
    pHeap->Initialize(SIGNATURE, reinterpret_cast<void*>(start + sizeof(FrameHeap)), reinterpret_cast<void*>(end), option);
    pHeap->m_pHeadAllocator = pHeap->m_pHeapStart;
    pHeap->m_pTailAllocator = pHeap->m_pHeapEnd;
    pHeap->m_pState = nullptr;
    return pHeap;
}

// 0x00140F6C | nintendogs:callgraph [tier A]
void* nw::ut::FrameHeap::Destroy()
{
    Finalize();
    return this;
}

// (inline)
void* nw::ut::FrameHeap::AllocFromHead(u32 size, s32 alignment)
{
    void* pMemory = reinterpret_cast<void*>(RoundUp(reinterpret_cast<uptr>(m_pHeadAllocator), alignment));
    void* pEnd = static_cast<u8*>(pMemory) + size;
    if (m_pTailAllocator < pEnd) {
        return nullptr;
    }
    FillAllocMemory(m_pHeadAllocator, static_cast<u8*>(pEnd) - static_cast<u8*>(m_pHeadAllocator));
    m_pHeadAllocator = pEnd;
    return pMemory;
}

// (inline)
void* nw::ut::FrameHeap::AllocFromTail(u32 size, s32 alignment)
{
    void* pMemory = reinterpret_cast<void*>(RoundDown(reinterpret_cast<uptr>(m_pTailAllocator) - size, alignment));
    if (m_pHeadAllocator > pMemory) {
        return nullptr;
    }
    FillAllocMemory(pMemory, static_cast<u8*>(m_pTailAllocator) - static_cast<u8*>(pMemory));
    m_pTailAllocator = pMemory;
    return pMemory;
}

// 0x0013F050 | nintendogs:bytes [tier A]
void* nw::ut::FrameHeap::Alloc(u32 size, s32 alignment)
{
    if (size == 0) {
        size = 1;
    }
    size = RoundUp(size, 4);
    void* pMemory;
    if (alignment >= 0) {
        pMemory = AllocFromHead(size, alignment);
    } else {
        pMemory = AllocFromTail(size, -alignment);
    }
    return pMemory;
}

// 0x00140DE8 | nintendogs:bytes [tier A]
void nw::ut::FrameHeap::Free(s32 mode)
{
    if ((mode & FREE_HEAD) != 0) {
        FillFreeMemory(m_pHeapStart, static_cast<u8*>(m_pHeadAllocator) - static_cast<u8*>(m_pHeapStart));
        m_pState = nullptr;
        m_pHeadAllocator = m_pHeapStart;
    }
    if ((mode & FREE_TAIL) != 0) {
        FillFreeMemory(m_pTailAllocator, static_cast<u8*>(m_pHeapEnd) - static_cast<u8*>(m_pTailAllocator));
        for (HeapState* pState = m_pState; pState != nullptr; pState = pState->pNext) {
            pState->pTailAllocator = m_pHeapEnd;
        }
        m_pTailAllocator = m_pHeapEnd;
    }
}

// 0x0013EFC4 | nintendogs:bytes [tier A]
bool nw::ut::FrameHeap::RecordState(u32 tagName)
{
    void* pOldHead = m_pHeadAllocator;
    void* pMemory = AllocFromHead(sizeof(HeapState), 4);
    if (pMemory == nullptr) {
        return false;
    }
    HeapState* pState = ::new (pMemory) HeapState(tagName, pOldHead);
    pState->pTailAllocator = m_pTailAllocator;
    pState->pNext = m_pState;
    m_pState = pState;
    return true;
}

// 0x0013EF20 | nintendogs:bytes [tier A]
bool nw::ut::FrameHeap::FreeByState(u32 tagName)
{
    HeapState* pState = m_pState;
    if (tagName != 0) {
        for (; pState != nullptr; pState = pState->pNext) {
            if (pState->tagName == tagName) {
                break;
            }
        }
    }
    if (pState == nullptr) {
        return false;
    }
    void* pOldHead = m_pHeadAllocator;
    void* pOldTail = m_pTailAllocator;
    m_pHeadAllocator = pState->pHeadAllocator;
    m_pTailAllocator = pState->pTailAllocator;
    m_pState = pState->pNext;
    FillFreeMemory(m_pHeadAllocator, static_cast<u8*>(pOldHead) - static_cast<u8*>(m_pHeadAllocator));
    FillFreeMemory(pOldTail, static_cast<u8*>(m_pTailAllocator) - static_cast<u8*>(pOldTail));
    return true;
}

// 0x0048BB54 slot 0x00
// 0x0048BB44 (deleting dtor)
nw::ut::FrameHeap::~FrameHeap()
{
}

} // namespace ut
} // namespace nw
