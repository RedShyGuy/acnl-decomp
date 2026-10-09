#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_ProfanityFilterTemporaryPool.h"
#include <new>

namespace nn {
namespace ngc {
// The links of a node of UnitList; the list itself is the sentinel node (name is ours).
struct UnitListNodeBase
{
    UnitListNodeBase* m_Prev;   // 0x0
    UnitListNodeBase* m_Next;   // 0x4
};

// A ring list whose nodes come from a ProfanityFilterTemporaryPool (whole units). The names of
// the members other than Iterator / ConstIterator / PushBackNew are ours.
template <typename T>
class UnitList : public UnitListNodeBase
{
public:
    struct Node : public UnitListNodeBase
    {
        T m_Value;  // 0x8
    };

    class Iterator
    {
    public:
        Iterator() : m_Node(NULL) {}
        explicit Iterator(UnitListNodeBase* node) : m_Node(node) {}
        Iterator(const Iterator& other) : m_Node(other.m_Node) {}

        Iterator& operator=(const Iterator& other)
        {
            m_Node = other.m_Node;
            return *this;
        }
        T& operator*() const { return static_cast<Node*>(m_Node)->m_Value; }
        T* operator->() const { return &static_cast<Node*>(m_Node)->m_Value; }
        Iterator& operator++()
        {
            m_Node = m_Node->m_Next;
            return *this;
        }
        Iterator& operator--()
        {
            m_Node = m_Node->m_Prev;
            return *this;
        }
        bool operator==(const Iterator& other) const { return m_Node == other.m_Node; }
        bool operator!=(const Iterator& other) const { return m_Node != other.m_Node; }
        bool IsNull() const { return m_Node == NULL; }

        UnitListNodeBase* m_Node;
    };

    class ConstIterator
    {
    public:
        ConstIterator() : m_Node(NULL) {}
        explicit ConstIterator(const UnitListNodeBase* node) : m_Node(node) {}
        ConstIterator(const ConstIterator& other) : m_Node(other.m_Node) {}
        ConstIterator(const Iterator& other) : m_Node(other.m_Node) {}

        ConstIterator& operator=(const ConstIterator& other)
        {
            m_Node = other.m_Node;
            return *this;
        }
        const T& operator*() const { return static_cast<const Node*>(m_Node)->m_Value; }
        const T* operator->() const { return &static_cast<const Node*>(m_Node)->m_Value; }
        ConstIterator& operator++()
        {
            m_Node = m_Node->m_Next;
            return *this;
        }
        bool operator==(const ConstIterator& other) const { return m_Node == other.m_Node; }
        bool operator!=(const ConstIterator& other) const { return m_Node != other.m_Node; }
        bool IsNull() const { return m_Node == NULL; }

        const UnitListNodeBase* m_Node;
    };

    UnitList() : m_Pool(NULL)
    {
        m_Prev = this;
        m_Next = this;
    }
    ~UnitList() { Clear(); }

    static u32 GetNodeUnitCount()
    {
        return (sizeof(Node) + ProfanityFilterTemporaryPool::UNIT_SIZE - 1) / ProfanityFilterTemporaryPool::UNIT_SIZE;
    }

    void SetPool(ProfanityFilterTemporaryPool* pool) { m_Pool = pool; }

    Iterator Begin() { return Iterator(m_Next); }
    Iterator End() { return Iterator(this); }
    ConstIterator Begin() const { return ConstIterator(m_Next); }
    ConstIterator End() const { return ConstIterator(this); }
    bool IsEmpty() const { return m_Next == this; }

    // a new node (default constructed) in front of position; null without memory
    Iterator Insert(Iterator position)
    {
        Node* node = static_cast<Node*>(m_Pool->Allocate(GetNodeUnitCount()));
        if (node == NULL) {
            return Iterator();
        }
        new (&node->m_Value) T;
        UnitListNodeBase* next = position.m_Node;
        node->m_Prev = next->m_Prev;
        node->m_Next = next;
        next->m_Prev = node;
        node->m_Prev->m_Next = node;
        return Iterator(node);
    }

    Iterator PushBackNew() { return Insert(End()); }

    // -> the node behind
    Iterator Erase(Iterator position)
    {
        UnitListNodeBase* node = position.m_Node;
        UnitListNodeBase* next = node->m_Next;
        node->m_Prev->m_Next = node->m_Next;
        node->m_Next->m_Prev = node->m_Prev;
        static_cast<Node*>(node)->m_Value.~T();
        m_Pool->Free(node, GetNodeUnitCount());
        return Iterator(next);
    }

    // gives the nodes back to the pool (a list without pool has none)
    void Clear()
    {
        if (m_Pool != NULL) {
            UnitListNodeBase* node = m_Next;
            while (node != this) {
                UnitListNodeBase* next = node->m_Next;
                static_cast<Node*>(node)->m_Value.~T();
                m_Pool->Free(node, GetNodeUnitCount());
                node = next;
            }
            m_Pool = NULL;
            m_Prev = this;
            m_Next = this;
        }
    }

    // makes this a copy of other (nodes from the pool of other)
    bool CopyFrom(const UnitList& other)
    {
        Clear();
        m_Pool = other.m_Pool;
        for (ConstIterator it = other.Begin(); it != other.End(); ++it) {
            Iterator node = PushBackNew();
            if (node.IsNull()) {
                return false;
            }
            *node = *it;
        }
        return true;
    }

    // takes the nodes of other, which is empty afterwards
    void MoveFrom(UnitList& other)
    {
        Clear();
        m_Next = other.m_Next;
        other.m_Next->m_Prev = this;
        m_Prev = other.m_Prev;
        other.m_Prev->m_Next = this;
        other.m_Next = &other;
        other.m_Prev = &other;
        m_Pool = other.m_Pool;
    }

    ProfanityFilterTemporaryPool* m_Pool;   // 0x8
};
} // namespace ngc
} // namespace nn
