#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

namespace ssys {
namespace st {
// a doubly linked list of ListNodes (first and last node; all function names are ours)
// RTTI N4ssys2st4ListE @ 0x008D253C
// vtable 0x009073DC (vptr 0x009073E4), offset_to_top 0, 2 entries
class List
{
public:
    DECOMP_EMIT_COPY DECOMP_NOINLINE List() : m_pFirst(NULL), m_pLast(NULL) {} // 0x0012651C
    virtual ~List(); // 0x0056BF90 slot 0x00

    ListNode* GetFirst() const { return m_pFirst; }
    ListNode* GetLast() const { return m_pLast; }

    // appends an unlinked node
    DECOMP_EMIT_COPY void PushBack(ListNode* node) // 0x0013F860
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
        m_pLast->m_pNext = node;
        node->m_pPrev = m_pLast;
        m_pLast = node;
        node->m_pNext = NULL;
    }
    void PushFront(ListNode* node);               // 0x0056BF38
    void Insert(ListNode* before, ListNode* node); // 0x0056BEC0

    DECOMP_EMIT_COPY DECOMP_NOINLINE void Erase(ListNode* node) // 0x0013F72C
    {
        ListNode* prev = node->m_pPrev;
        ListNode* next = node->m_pNext;
        if (prev == NULL && next == NULL) {
            // the only node (or not in the list)
            if (m_pFirst == node && m_pLast == node) {
                m_pFirst = NULL;
                m_pLast = NULL;
            }
            return;
        }
        if (prev != NULL) {
            prev->m_pNext = next;
        } else if (m_pFirst == node) {
            m_pFirst = next;
        }
        if (next != NULL) {
            next->m_pPrev = prev;
        } else if (m_pLast == node) {
            m_pLast = prev;
        }
        node->m_pPrev = NULL;
        node->m_pNext = NULL;
    }

    // whether node is in the list
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool Contains(const ListNode* node) const // 0x00139764
    {
        if (node == NULL) {
            return false;
        }
        for (ListNode* it = m_pFirst; it != NULL; it = it->m_pNext) {
            if (it == node) {
                return true;
            }
        }
        return false;
    }

    // unlinks all nodes
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Clear() // 0x00133B34
    {
        ListNode* node = m_pFirst;
        while (node != NULL) {
            ListNode* next = node->m_pNext;
            node->m_pNext = NULL;
            node->m_pPrev = NULL;
            node = next;
        }
        m_pFirst = NULL;
        m_pLast = NULL;
    }

    ListNode* m_pFirst;  // 0x4 (names after libgarden)
    ListNode* m_pLast;  // 0x8
};
ASSERT_SIZE(List, 0xC);
} // namespace st
} // namespace ssys
