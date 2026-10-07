#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The map of the RW STL (a red-black tree with a node pool) with nex::MemAllocator; qMap<K, V>
// derives from it. Only what pia uses is here: the layout is from the constructor of
// MatchmakeParam, the out-of-line tree functions are declared per instance where they are used.
// The member names are ours.
template <typename T0, typename T1>
class qMap
{
public:
    struct Node
    {
        u32 m_Color;    // 0x00
        Node* m_pParent; // 0x04 (the root in the header node)
        Node* m_pLeft;   // 0x08 (the first node in the header node)
        Node* m_pRight;  // 0x0C (the last node in the header node)
        T0 m_Key;        // 0x10
        T1 m_Value;
    };
    struct Iterator
    {
        Iterator(Node* pNode) : m_pNode(pNode) {}
        Iterator(const Iterator& rhs) : m_pNode(rhs.m_pNode) {}
        Node* m_pNode;
    };

    Iterator Begin() const { return Iterator(m_pHeader->m_pLeft); }
    Iterator End() const { return Iterator(m_pHeader); }
    // the nodes from first to last are removed (the erase of the RW tree; out of line)
    Iterator Erase(Iterator first, Iterator last);
    void Clear() { Erase(Begin(), End()); }

    void* m_pBufferList; // 0x00
    void* m_pFreeList;   // 0x04
    void* m_pNextAvail;  // 0x08
    void* m_pLast;       // 0x0C
    Node* m_pHeader;     // 0x10
    u32 m_Size;          // 0x14
    u8 m_Unknown0x18;    // 0x18
    u8 m_Unknown0x19;    // 0x19
};
} // namespace nex
} // namespace nn
