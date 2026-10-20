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
DECOMP_NOINLINE qList<PlayingSession>::qList()
{
    // nothing to do: std::list makes the head node
}

// 0x003CF4AC (name is ours; it calls the destructor of std::list)
template qList<PlayingSession>::~qList();

} // namespace nex
} // namespace nn

namespace std {
// 0x0082F478 (name is ours; std::list::~list, the destructor of the qList calls it)
template <>
DECOMP_NOINLINE list<nn::nex::PlayingSession, nn::nex::MemAllocator<nn::nex::PlayingSession> >::~list()
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
        nn::nex::MemoryManager::Free(pBuffer->m_pNodes);
        nn::nex::MemoryManager::Free(pBuffer);
    }
    m_pFreeList = nullptr;
    m_pNextAvail = nullptr;
    m_pLast = nullptr;
}

// 0x0082F404 (name is ours)
template <>
DECOMP_NOINLINE void list<nn::nex::PlayingSession, nn::nex::MemAllocator<nn::nex::PlayingSession> >::clear()
{
    ClearInline();
}
} // namespace std
