#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
// The links of an object in an intrusive doubly linked list (LinkList). The class names are from
// the reference symbols, the member names are ours.
class LinkListNode
{
public:
    // (inline)
    LinkListNode() : m_pNext(nullptr), m_pPrev(nullptr) {}

    LinkListNode* m_pNext; // 0x0
    LinkListNode* m_pPrev; // 0x4
};
ASSERT_SIZE(LinkListNode, 0x8);

namespace internal {
// The untyped part of LinkList: a ring of nodes around m_Node, which is the end of the list.
class LinkListImpl
{
public:
    // a position in the list (the node it points to)
    class iterator
    {
    public:
        explicit iterator(LinkListNode* pNode) : m_pNode(pNode) {}
        LinkListNode* GetNode() const { return m_pNode; }
        iterator& operator++()
        {
            m_pNode = m_pNode->m_pNext;
            return *this;
        }
        bool operator==(const iterator& other) const { return m_pNode == other.m_pNode; }
        bool operator!=(const iterator& other) const { return m_pNode != other.m_pNode; }

        LinkListNode* m_pNode;
    };

    // (inline)
    LinkListImpl() : m_Count(0)
    {
        m_Node.m_pNext = &m_Node;
        m_Node.m_pPrev = &m_Node;
    }

    iterator begin() { return iterator(m_Node.m_pNext); }
    iterator end() { return iterator(&m_Node); }
    s32 size() const { return m_Count; }
    bool empty() const { return m_Count == 0; }

    // the node goes in front of the position
    iterator insert(iterator it, LinkListNode* pNode); // 0x0013EEFC | libgarden [tier A]
    // the node is taken out of the list; the position behind it
    iterator erase(LinkListNode* pNode); // 0x00140F40 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    // (libgarden calls it erase(LinkListNode*) as well; the bytes names 0x00140F40 so, this one takes
    // the node as a position and erases up to the next one; name is ours)
    iterator erase(iterator it); // 0x00137580 (name is ours)
    // the nodes from first up to last are taken out of the list; last
    iterator erase(iterator first, iterator last); // 0x00140EE8 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]

    s32 m_Count;         // 0x0
    LinkListNode m_Node; // 0x4 (the end of the list)
};
ASSERT_SIZE(LinkListImpl, 0xC);
} // namespace internal
} // namespace ut
} // namespace nw
