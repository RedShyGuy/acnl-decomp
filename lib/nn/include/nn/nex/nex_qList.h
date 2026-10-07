#pragma once

#include "decomp.h"
#include "nn/nex/nex_MemoryManager.h"
#include <new>

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::qList<nn::nex::URLProbe>  typeinfo 0x008CF550
//
// The list of the RW STL with a node pool (like qMap): the buffers of nodes, the free nodes and the
// circular list behind the head node. ARMCC inlines its functions. The member names are ours.
template <typename T0>
class qList
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

    // (one node is the head of the circular list)
    qList() : m_pBufferList(nullptr), m_pFreeList(nullptr), m_pNextAvail(nullptr), m_pLast(nullptr), m_pHead(nullptr), m_Size(0)
    {
        AddBuffer(1);
        m_pHead = m_pNextAvail;
        m_pNextAvail++;
        m_pHead->m_pNext = m_pHead;
        m_pHead->m_pPrev = m_pHead;
    }
    ~qList()
    {
        if (m_pHead != nullptr) {
            clear();
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
    void push_back(const T0& value)
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
    // (out of line in the original, one instance per type; not decompiled yet)
    qList& operator=(const qList& other);
    T0& front() { return GetFirst()->m_Value; }

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
    void clear()
    {
        Node* pEnd = m_pHead;
        for (Node* pNode = m_pHead->m_pNext; pNode != pEnd;) {
            pNode = erase(pNode);
        }
    }

    // a buffer of num nodes (the RW _C_add_buffer; 0: 32 nodes or 1 + 1/2 + 1/8 of the last
    // buffer, at least 32 more)
    void AddBuffer(u32 num)
    {
        if (num == 0) {
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
        Buffer* pBuffer = static_cast<Buffer*>(MemoryManager::Allocate(sizeof(Buffer)));
        pBuffer->m_pNodes = static_cast<Node*>(MemoryManager::Allocate(num * sizeof(Node)));
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
                AddBuffer(0);
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
} // namespace nex
} // namespace nn
