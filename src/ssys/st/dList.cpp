#include "ssys/st/dList.h"

namespace ssys {
namespace st {
// links node in front of before (an empty list gets before itself, as in the original)
// 0x0056BEC0 (name is ours)
void ssys::st::List::Insert(ListNode* before, ListNode* node)
{
    if (m_pFirst == NULL) {
        m_pFirst = before;
        m_pLast = before;
        before->m_pPrev = NULL;
        m_pFirst->m_pNext = NULL;
        return;
    }
    if (m_pFirst == before) {
        if (node->m_pNext != NULL || node->m_pPrev != NULL) {
            return;
        }
        before->m_pPrev = node;
        node->m_pNext = m_pFirst;
        m_pFirst = node;
        node->m_pPrev = NULL;
        return;
    }
    node->m_pPrev = before->m_pPrev;
    node->m_pNext = before;
    before->m_pPrev = node;
    if (node->m_pPrev != NULL) {
        node->m_pPrev->m_pNext = node;
    }
}

// puts an unlinked node in front
// 0x0056BF38 (name is ours)
void ssys::st::List::PushFront(ListNode* node)
{
    if (node->m_pNext != NULL || node->m_pPrev != NULL) {
        return;
    }
    if (m_pFirst == NULL) {
        m_pFirst = node;
        m_pLast = node;
        node->m_pPrev = NULL;
        m_pFirst->m_pNext = NULL;
        return;
    }
    m_pFirst->m_pPrev = node;
    node->m_pNext = m_pFirst;
    m_pFirst = node;
    node->m_pPrev = NULL;
}

// 0x0056BF90 slot 0x00
// 0x0056BF8C (deleting dtor)
ssys::st::List::~List()
{
    // empty (in the original too)
}

} // namespace st
} // namespace ssys
