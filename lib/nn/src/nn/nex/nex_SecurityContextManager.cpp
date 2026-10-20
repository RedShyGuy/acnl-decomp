#include "nn/nex/nex_SecurityContextManager.h"
#include "nn/nex/nex_Core.h"

namespace nn {
namespace nex {
// 0x0039DE20 (name is ours)
nn::nex::SecurityContextManager* nn::nex::SecurityContextManager::GetInstance()
{
    Core* pCore = Core::GetInstance();
    if (pCore == nullptr) {
        return nullptr;
    }
    return pCore->m_pSecurityContextManager;
}

// 0x0039DE34 | fefates:bytes [tier B]
u32 nn::nex::SecurityContextManager::StaticGetCurrentCID()
{
    Core* pCore = Core::GetInstance();
    const SecurityContextManager* pManager;
    if (pCore == nullptr || (pManager = pCore->m_pSecurityContextManager) == nullptr) {
        return 0;
    }
    if (pManager->m_Contexts.GetRef().GetSize() == 0) {
        return 0;
    }
    return pManager->m_Contexts.GetRef().front().m_ConnectionId;
}

// 0x0039DE7C | fefates:bytes [tier B]
u32 nn::nex::SecurityContextManager::StaticGetCurrentPID()
{
    Core* pCore = Core::GetInstance();
    const SecurityContextManager* pManager;
    if (pCore == nullptr || (pManager = pCore->m_pSecurityContextManager) == nullptr) {
        return 0;
    }
    if (pManager->m_Contexts.GetRef().GetSize() == 0) {
        return 0;
    }
    return pManager->m_Contexts.GetRef().front().m_PrincipalId;
}

// 0x0039DEC4 | mk7dlp:bytes [tier A]
void nn::nex::SecurityContextManager::Pop()
{
    qList<SecurityContext>& contexts = m_Contexts.GetRef();
    contexts.erase(contexts.GetFirst());
}

// 0x0039DF20 | mk7dlp:callseq [tier A]
void nn::nex::SecurityContextManager::Push(unsigned int principalId, unsigned int connectionId, unsigned int address)
{
    m_Contexts.GetRef().push_front(SecurityContext(principalId, connectionId, address));
}

// 0x0039E014
nn::nex::SecurityContextManager::SecurityContextManager() : m_Contexts(qList<SecurityContext>())
{
}

// 0x0039E1E4
// 0x0039E1B0 (deleting dtor)
nn::nex::SecurityContextManager::~SecurityContextManager()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072D0B0 | fefates:bytes [tier B]
u32 nn::nex::SecurityContextManager::GetCurrentAddress() const
{
    if (m_Contexts.GetRef().GetSize() == 0) {
        return 0;
    }
    return m_Contexts.GetRef().front().m_Address;
}

// 0x007D613C
// 0x007D610C (deleting dtor)
template nn::nex::ThreadVariable<nn::nex::qList<nn::nex::SecurityContext> >::~ThreadVariable();
// 0x007D5F6C (name is ours)
template void nn::nex::ThreadVariable<nn::nex::qList<nn::nex::SecurityContext> >::ClearAll();
// 0x007D5EB0 (name is ours)
template void nn::nex::ThreadVariable<nn::nex::qList<nn::nex::SecurityContext> >::ClearCurrentThread();
// 0x007D5E10 (name is ours)
template void nn::nex::ThreadVariable<nn::nex::qList<nn::nex::SecurityContext> >::ClearThread(unsigned int);
// 0x007D5FD8 | mk7dlp:callgraph [tier A]
template nn::nex::qList<nn::nex::SecurityContext>& nn::nex::ThreadVariable<nn::nex::qList<nn::nex::SecurityContext> >::GetRef();
// 0x00827B34 | fefates:callgraph [tier C]
template const nn::nex::qList<nn::nex::SecurityContext>& nn::nex::ThreadVariable<nn::nex::qList<nn::nex::SecurityContext> >::GetRef() const;

// 0x003CF518
template <>
DECOMP_NOINLINE qList<SecurityContext>::qList(const qList<SecurityContext>& other) : base_type(base_type::NoInit())
{
    // (the first buffer has the nodes of a buffer that grows, not one node)
    AddBufferInline(false);
    m_pHead = m_pNextAvail;
    m_pNextAvail++;
    m_pHead->m_pNext = m_pHead;
    m_pHead->m_pPrev = m_pHead;
    Node* pPos = m_pHead->m_pNext;
    for (Node* pNode = other.GetFirst(); !other.IsEnd(pNode); pNode = pNode->m_pNext) {
        insert(pPos, pNode->m_Value);
    }
}

// 0x003CF6A8 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE qList<SecurityContext>::~qList()
{
    // nothing to do: the list is freed by std::list (inline here)
}

} // namespace nex
} // namespace nn

namespace std {
// 0x0082E7CC | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE nn::nex::qList<nn::nex::SecurityContext>& map<u32, nn::nex::qList<nn::nex::SecurityContext>, less<u32>, nn::nex::MemAllocator<pair<const u32, nn::nex::qList<nn::nex::SecurityContext> > > >::operator[](const u32& key)
{
    return IndexInline(key);
}
} // namespace std

namespace std {
// 0x0082F864 (name is ours; std::list::operator=)
template <>
DECOMP_NOINLINE list<nn::nex::SecurityContext, nn::nex::MemAllocator<nn::nex::SecurityContext> >& list<nn::nex::SecurityContext, nn::nex::MemAllocator<nn::nex::SecurityContext> >::operator=(const list<nn::nex::SecurityContext, nn::nex::MemAllocator<nn::nex::SecurityContext> >& other)
{
    return AssignInline(other);
}
} // namespace std
