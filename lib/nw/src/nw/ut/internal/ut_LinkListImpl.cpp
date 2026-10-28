#include "nw/ut/internal/ut_LinkListImpl.h"

namespace nw {
namespace ut {
namespace internal {
// 0x00137580 | fefates:bytes
nw::ut::internal::LinkListImpl::iterator nw::ut::internal::LinkListImpl::erase(nw::ut::internal::LinkListImpl::iterator it)
{
    iterator next = it;
    ++next;
    return erase(it, next);
}

// 0x0013EEFC | libgarden [tier A]
nw::ut::internal::LinkListImpl::iterator nw::ut::internal::LinkListImpl::insert(nw::ut::internal::LinkListImpl::iterator it, nw::ut::LinkListNode* pNode)
{
    LinkListNode* pNext = it.m_pNode;
    LinkListNode* pPrev = pNext->m_pPrev;
    pNode->m_pNext = pNext;
    pNode->m_pPrev = pPrev;
    pNext->m_pPrev = pNode;
    pPrev->m_pNext = pNode;
    m_Count++;
    return iterator(pNode);
}

// 0x00140EE8 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
nw::ut::internal::LinkListImpl::iterator nw::ut::internal::LinkListImpl::erase(nw::ut::internal::LinkListImpl::iterator first, nw::ut::internal::LinkListImpl::iterator last)
{
    LinkListNode* pNode = first.m_pNode;
    while (pNode != last.m_pNode) {
        LinkListNode* pNext = pNode->m_pNext;
        LinkListNode* pPrev = pNode->m_pPrev;
        pNext->m_pPrev = pPrev;
        pPrev->m_pNext = pNext;
        m_Count--;
        pNode->m_pNext = nullptr;
        pNode->m_pPrev = nullptr;
        pNode = pNext;
    }
    return last;
}

// 0x00140F40 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
nw::ut::internal::LinkListImpl::iterator nw::ut::internal::LinkListImpl::erase(nw::ut::LinkListNode* pNode)
{
    LinkListNode* pNext = pNode->m_pNext;
    LinkListNode* pPrev = pNode->m_pPrev;
    pNext->m_pPrev = pPrev;
    pPrev->m_pNext = pNext;
    m_Count--;
    pNode->m_pNext = nullptr;
    pNode->m_pPrev = nullptr;
    return iterator(pNext);
}

} // namespace internal
} // namespace ut
} // namespace nw
