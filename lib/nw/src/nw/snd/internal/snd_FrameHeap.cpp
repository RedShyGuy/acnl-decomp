#include "nw/snd/internal/snd_FrameHeap.h"

#include <new>

#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the mode of ut::FrameHeap::Free for the head and the tail
const s32 FREE_ALL = 3;

// (inline) the last section freed: the callbacks of its blocks from the last one (unless one
// callback was called for all of them)
inline void FreeLastSection(FrameHeap* pHeap, bool isCallbackDone)
{
    FrameHeap::Section* pSection = reinterpret_cast<FrameHeap::Section*>(pHeap->m_SectionList.m_Node.m_pPrev);
    if (isCallbackDone) {
        pSection->m_IsUseCallback = false;
    }
    if (pSection->m_IsUseCallback) {
        ut::LinkListNode* pNode = pSection->m_BlockList.end().GetNode();
        while (pNode != pSection->m_BlockList.begin().GetNode()) {
            pNode = pNode->m_pPrev;
            FrameHeap::Block* pBlock = reinterpret_cast<FrameHeap::Block*>(pNode);
            if (pBlock->m_Callback != nullptr) {
                pBlock->m_Callback(pBlock->m_pBuffer, pBlock->m_Size, pBlock->m_pCallbackArg);
            }
        }
    }
    pSection->m_BlockList.erase(pSection->m_BlockList.begin(), pSection->m_BlockList.end());
    pHeap->m_SectionList.erase(&pSection->m_Link);
}
} // namespace

// 0x001327AC | nintendogs:bytes [tier A]
int nw::snd::internal::FrameHeap::SaveState()
{
    if (!m_pFrameHeap->RecordState(m_SectionList.size())) {
        return -1;
    }
    if (!NewSection()) {
        m_pFrameHeap->FreeByState(0);
        return -1;
    }
    return m_SectionList.size() - 1;
}

// 0x00138540 | nintendogs:bytes [tier A]
bool nw::snd::internal::FrameHeap::Create(void* pStartAddress, u32 size)
{
    if (m_pFrameHeap != nullptr) {
        ClearSection();
        m_pFrameHeap->Free(FREE_ALL);
        m_pFrameHeap->Destroy();
        m_pFrameHeap = nullptr;
    }
    u8* pStart = ut::RoundUp(static_cast<u8*>(pStartAddress), 4);
    u8* pEnd = static_cast<u8*>(pStartAddress) + size;
    if (pStart > pEnd) {
        return false;
    }
    m_pFrameHeap = ut::FrameHeap::Create(pStart, pEnd - pStart, 0);
    if (m_pFrameHeap == nullptr) {
        return false;
    }
    return NewSection();
}

// 0x001385C4 | nintendogs:bytes [tier A]
nw::snd::internal::FrameHeap::FrameHeap() : m_pFrameHeap(nullptr)
{
}

// 0x0013F3FC | nintendogs:bytes-fuzzy [tier B]
void nw::snd::internal::FrameHeap::LoadState(int level)
{
    if (level == 0) {
        Clear();
        return;
    }
    const bool isCallbackDone = ProcessCallback(level);
    while (m_SectionList.size() > level) {
        FreeLastSection(this, isCallbackDone);
    }
    m_pFrameHeap->FreeByState(level);
    m_pFrameHeap->RecordState(m_SectionList.size());
    NewSection();
}

// 0x0013F4D8 | nintendogs:bytes [tier A]
bool nw::snd::internal::FrameHeap::NewSection()
{
    void* pMemory = m_pFrameHeap->Alloc(sizeof(Section), 4);
    if (pMemory == nullptr) {
        return false;
    }
    Section* pSection = new (pMemory) Section;
    pSection->m_BlockList.erase(pSection->m_BlockList.begin(), pSection->m_BlockList.end());
    m_SectionList.insert(m_SectionList.end(), &pSection->m_Link);
    return true;
}

// 0x00141964 | nintendogs:bytes [tier A]
void nw::snd::internal::FrameHeap::ClearSection()
{
    const bool isCallbackDone = ProcessCallback(0);
    while (!m_SectionList.empty()) {
        FreeLastSection(this, isCallbackDone);
    }
}

// 0x001419FC | nintendogs:callgraph [tier A]
bool nw::snd::internal::FrameHeap::ProcessCallback(int level)
{
    bool isDone = false;
    u8* pStart = nullptr;
    uptr end = 0;
    FreeCallback callback = nullptr;
    void* pCallbackArg = nullptr;
    bool isSameCallback = true;
    int index = m_SectionList.size();
    ut::LinkListNode* pNode = m_SectionList.end().GetNode();
    if (pNode == m_SectionList.begin().GetNode()) {
        return false;
    }
    // the sections from the last one down to the level
    do {
        pNode = pNode->m_pPrev;
        --index;
        Section* pSection = reinterpret_cast<Section*>(pNode);
        if (end == 0 && !pSection->m_BlockList.empty()) {
            Block* pLast = reinterpret_cast<Block*>(pSection->m_BlockList.m_Node.m_pPrev);
            callback = pLast->m_Callback;
            pCallbackArg = pLast->m_pCallbackArg;
            end = reinterpret_cast<uptr>(pLast->m_pBuffer) + pLast->m_Size;
        }
        for (ut::LinkListNode* pBlockNode = pSection->m_BlockList.begin().GetNode(); pBlockNode != pSection->m_BlockList.end().GetNode();
             pBlockNode = pBlockNode->m_pNext) {
            const Block* pBlock = reinterpret_cast<const Block*>(pBlockNode);
            if (callback != pBlock->m_Callback || pCallbackArg != pBlock->m_pCallbackArg) {
                isSameCallback = false;
                break;
            }
        }
        if (index == level) {
            pStart = reinterpret_cast<u8*>(pSection);
            break;
        }
    } while (pNode != m_SectionList.begin().GetNode());

    if (end != 0 && isSameCallback) {
        if (callback != nullptr) {
            callback(pStart, end - reinterpret_cast<uptr>(pStart), pCallbackArg);
        }
        isDone = true;
    }
    return isDone;
}

// 0x00141AFC | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::FrameHeap::Clear()
{
    ClearSection();
    m_pFrameHeap->Free(FREE_ALL);
    NewSection();
}

// 0x00141B20 | nintendogs:bytes [tier A]
void nw::snd::internal::FrameHeap::Destroy()
{
    if (m_pFrameHeap == nullptr) {
        return;
    }
    ClearSection();
    m_pFrameHeap->Free(FREE_ALL);
    m_pFrameHeap->Destroy();
    m_pFrameHeap = nullptr;
}

// 0x004D4B3C | nintendogs:bytes [tier A]
void* nw::snd::internal::FrameHeap::Alloc(u32 size, FreeCallback callback, void* pCallbackArg)
{
    void* pMemory = m_pFrameHeap->Alloc(ut::RoundUp(size, 32) + BLOCK_HEADER_SIZE, 32);
    if (pMemory == nullptr) {
        return nullptr;
    }
    Block* pBlock = static_cast<Block*>(pMemory);
    pBlock->m_Link.m_pNext = nullptr;
    pBlock->m_Link.m_pPrev = nullptr;
    pBlock->m_pBuffer = static_cast<u8*>(pMemory) + BLOCK_HEADER_SIZE;
    pBlock->m_Size = size;
    pBlock->m_Callback = callback;
    pBlock->m_pCallbackArg = pCallbackArg;
    Section* pSection = reinterpret_cast<Section*>(m_SectionList.m_Node.m_pPrev);
    pSection->m_BlockList.insert(pSection->m_BlockList.end(), &pBlock->m_Link);
    return pBlock->m_pBuffer;
}

// 0x004D4BAC | nintendogs:bytes [tier A]
nw::snd::internal::FrameHeap::~FrameHeap()
{
    Destroy();
}

} // namespace internal
} // namespace snd
} // namespace nw
