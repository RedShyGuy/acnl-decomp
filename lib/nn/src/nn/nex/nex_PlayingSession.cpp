#include "nn/nex/nex_PlayingSession.h"

namespace nn {
namespace nex {
// 0x00373B30 slot 0x00
// 0x00373AF8 (deleting dtor)
nn::nex::PlayingSession::~PlayingSession()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CF42C (name is ours)
template <>
DECOMP_NOINLINE qList<PlayingSession>::qList() : m_pBufferList(nullptr), m_pFreeList(nullptr), m_pNextAvail(nullptr), m_pLast(nullptr), m_pHead(nullptr), m_Size(0)
{
    InitializeInline();
}

// 0x0082F478 (name is ours; std::list::~list, the destructor of the qList calls it)
template <>
DECOMP_NOINLINE qList<PlayingSession>::~qList()
{
    // (DestroyInline with the loop of clear)
    if (m_pHead != nullptr) {
        ClearInline();
        m_pHead->m_pNext = m_pFreeList;
        m_pFreeList = m_pHead;
    }
    while (m_pBufferList != nullptr) {
        Buffer* pBuffer = m_pBufferList;
        m_pBufferList = pBuffer->m_pNext;
        MemoryManager::Free(pBuffer->m_pNodes);
        MemoryManager::Free(pBuffer);
    }
    m_pFreeList = nullptr;
    m_pNextAvail = nullptr;
    m_pLast = nullptr;
}

// 0x0082F404 (name is ours)
template <>
DECOMP_NOINLINE void qList<PlayingSession>::clear()
{
    ClearInline();
}

} // namespace nex
} // namespace nn
