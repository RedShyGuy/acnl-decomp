#pragma once

#include "decomp.h"
#include "nn/nex/nex_MemAllocator.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_RootObject.h"
#include <new>

namespace std {
// The list of the RW STL with a node pool (like the trees of the maps): the buffers of nodes, the
// free nodes and the circular list behind the head node. ARMCC inlines its functions; some instances
// are out of line (they specialize the functions with the bodies of the *Inline functions). The
// class and function names are from the binary, the member names are ours.
template <typename T0, typename Alloc = nn::nex::MemAllocator<T0> >
class list
{
public:
    // a node of the circular list (the layout is from pia::inet::NexFacade; the names are ours)
    struct Node
    {
        Node* m_pNext; // 0x0
        Node* m_pPrev; // 0x4
        T0 m_Value;    // 0x8
    };
    // a buffer of nodes (MemoryManager)
    struct Buffer
    {
        Buffer* m_pNext; // 0x0
        u32 m_Size;      // 0x4
        Node* m_pNodes;  // 0x8
    };
    // (the members only: the copies of qList build the list themselves)
    struct NoInit
    {
    };

    // (one node is the head of the circular list)
    list() : m_pBufferList(nullptr), m_pFreeList(nullptr), m_pNextAvail(nullptr), m_pLast(nullptr), m_pHead(nullptr), m_Size(0) { InitializeInline(); }
    explicit list(NoInit) : m_pBufferList(nullptr), m_pFreeList(nullptr), m_pNextAvail(nullptr), m_pLast(nullptr), m_pHead(nullptr), m_Size(0) {}
    void InitializeInline()
    {
        AddBuffer(true);
        m_pHead = m_pNextAvail;
        m_pNextAvail++;
        m_pHead->m_pNext = m_pHead;
        m_pHead->m_pPrev = m_pHead;
    }
    // (out of line in some instances: they specialize the destructor with the body of DestroyInline)
    ~list() { DestroyInline(); }
    void DestroyInline()
    {
        if (m_pHead != nullptr) {
            clear();
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
    // (out of line in some instances: they specialize push_back with the body of PushBackInline)
    void push_back(const T0& value) { PushBackInline(value); }
    void PushBackInline(const T0& value)
    {
        Node* pHead = m_pHead;
        Node* pNode = AllocNode();
        ::new (static_cast<void*>(&pNode->m_Value)) T0(value);
        pNode->m_pNext = pHead;
        pNode->m_pPrev = pHead->m_pPrev;
        pHead->m_pPrev->m_pNext = pNode;
        pHead->m_pPrev = pNode;
        m_Size++;
    }
    // the value goes in front of the node
    void insert(Node* pPos, const T0& value)
    {
        Node* pNode = AllocNode();
        ::new (static_cast<void*>(&pNode->m_Value)) T0(value);
        pNode->m_pNext = pPos;
        pNode->m_pPrev = pPos->m_pPrev;
        pPos->m_pPrev->m_pNext = pNode;
        pPos->m_pPrev = pNode;
        m_Size++;
    }
    void push_front(const T0& value)
    {
        Node* pFirst = m_pHead->m_pNext;
        Node* pNode = AllocNode();
        ::new (static_cast<void*>(&pNode->m_Value)) T0(value);
        pNode->m_pNext = pFirst;
        pNode->m_pPrev = pFirst->m_pPrev;
        pFirst->m_pPrev->m_pNext = pNode;
        pFirst->m_pPrev = pNode;
        m_Size++;
    }
    // (out of line in the original, one instance per type) the elements are assigned, the rest is
    // erased or appended
    list& operator=(const list& other) { return AssignInline(other); }
    DECOMP_ALWAYS_INLINE list& AssignInline(const list& other)
    {
        if (this != &other) {
            Node* pNode = m_pHead->m_pNext;
            Node* pOther = other.m_pHead->m_pNext;
            for (; pNode != m_pHead && pOther != other.m_pHead; pNode = pNode->m_pNext, pOther = pOther->m_pNext) {
                pNode->m_Value = pOther->m_Value;
            }
            if (pOther == other.m_pHead) {
                while (pNode != m_pHead) {
                    pNode = erase(pNode);
                }
            } else {
                for (; pOther != other.m_pHead; pOther = pOther->m_pNext) {
                    push_back(pOther->m_Value);
                }
            }
        }
        return *this;
    }
    T0& front() { return GetFirst()->m_Value; }
    const T0& front() const { return GetFirst()->m_Value; }
    T0& back() { return m_pHead->m_pPrev->m_Value; }
    const T0& back() const { return m_pHead->m_pPrev->m_Value; }
    void pop_back() { erase(m_pHead->m_pPrev); }

    // the node is destroyed and goes to the free nodes; the next one
    Node* erase(Node* pNode)
    {
        Node* pNext = pNode;
        if (pNode != m_pHead) {
            pNext = pNode->m_pNext;
            pNode->m_pPrev->m_pNext = pNext;
            pNext->m_pPrev = pNode->m_pPrev;
            m_Size--;
            pNode->m_Value.~T0();
            pNode->m_pNext = m_pFreeList;
            m_pFreeList = pNode;
        }
        return pNext;
    }
    // the elements equal to the value are erased (out of line in the original, one instance per type)
    void remove(const T0& value)
    {
        Node* pHead = m_pHead;
        for (Node* pNode = pHead->m_pNext; pNode != pHead;) {
            Node* pNext = pNode->m_pNext;
            if (pNode->m_Value == value) {
                erase(pNode);
            }
            pNode = pNext;
        }
    }
    // (out of line in some instances: they specialize clear with the body of ClearInline)
    void clear() { ClearInline(); }
    void ClearInline()
    {
        Node* pEnd = m_pHead;
        for (Node* pNode = m_pHead->m_pNext; pNode != pEnd;) {
            pNode = erase(pNode);
        }
    }

    // a buffer of nodes (the RW _C_add_buffer; the first one has one node, else 32 nodes or 1 + 1/2 +
    // 1/8 of the last buffer, at least 32 more; out of line in some instances: they specialize it
    // with the body of AddBufferInline)
    void AddBuffer(bool isFirst) { AddBufferInline(isFirst); }
    void AddBufferInline(bool isFirst)
    {
        u32 num = 1;
        if (!isFirst) {
            if (m_pBufferList == nullptr) {
                num = 32;
            } else {
                u32 size = m_pBufferList->m_Size;
                num = size + 32;
                u32 grown = size + (size >> 1) + (size >> 3);
                if (num <= grown) {
                    num = grown;
                }
            }
        }
        Buffer* pBuffer = static_cast<Buffer*>(nn::nex::MemoryManager::Allocate(sizeof(Buffer)));
        pBuffer->m_pNodes = static_cast<Node*>(nn::nex::MemoryManager::Allocate(num * sizeof(Node)));
        pBuffer->m_pNext = m_pBufferList;
        pBuffer->m_Size = num;
        m_pBufferList = pBuffer;
        m_pNextAvail = pBuffer->m_pNodes;
        m_pLast = pBuffer->m_pNodes + num;
    }
    // a free node, else the next one of the buffer
    Node* AllocNode()
    {
        Node* pNode = m_pFreeList;
        if (pNode != nullptr) {
            m_pFreeList = pNode->m_pNext;
        } else {
            if (m_pNextAvail == m_pLast) {
                AddBuffer(false);
            }
            pNode = m_pNextAvail;
            m_pNextAvail++;
        }
        return pNode;
    }

    u32 GetSize() const { return m_Size; }
    // the first node (the head node itself if the list is empty)
    Node* GetFirst() const { return m_pHead->m_pNext; }
    bool IsEnd(const Node* pNode) const { return pNode == m_pHead; }

    Buffer* m_pBufferList; // 0x00
    Node* m_pFreeList;     // 0x04
    Node* m_pNextAvail;    // 0x08
    Node* m_pLast;         // 0x0C
    Node* m_pHead;         // 0x10
    u32 m_Size;            // 0x14
};
} // namespace std

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::qList<nn::nex::URLProbe>  typeinfo 0x008CF550
//
// The std::list of nex (with MemAllocator). Its constructor and destructor are its own: inline, with
// copies out of line per type (the destructor then calls the one of std::list).
// (nex allocates the lists of the any adapters with new: RootObject)
template <typename T0>
class qList : public std::list<T0, MemAllocator<T0> >, public RootObject
{
public:
    typedef std::list<T0, MemAllocator<T0> > base_type;

    qList() {}
    ~qList() {}
    // (out of line in the original, one instance per type)
    qList(const qList& other);
    qList& operator=(const qList& other)
    {
        base_type::operator=(other);
        return *this;
    }
};

// (out of line in the original; nex_qList.cpp)
template <>
DECOMP_NOINLINE qList<u32>::qList();
} // namespace nex
} // namespace nn

namespace std {
// (out of line in the original; nex_qList.cpp)
template <>
DECOMP_NOINLINE list<u32, nn::nex::MemAllocator<u32> >::~list();
template <>
DECOMP_NOINLINE void list<u32, nn::nex::MemAllocator<u32> >::AddBuffer(bool isFirst);
template <>
DECOMP_NOINLINE void list<u32, nn::nex::MemAllocator<u32> >::clear();
template <>
DECOMP_NOINLINE list<u32, nn::nex::MemAllocator<u32> >& list<u32, nn::nex::MemAllocator<u32> >::operator=(const list<u32, nn::nex::MemAllocator<u32> >& other);
} // namespace std
