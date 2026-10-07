#pragma once

#include "decomp.h"
#include "nn/pia/common/common_ListBase.h"
#include <new>
#include <stddef.h>

namespace nn {
namespace pia {
namespace common {
// A list of objects in a fixed buffer of nodes: the list itself (the ListBase base) holds the
// used nodes, m_FreeList the unused ones. The template name and its base are from the RTTI;
// the layout is from transport::StationIdTable, all member and function names are ours.
//
// Instantiations found in the binary (RTTI):
//   nn::pia::common::ObjList<nn::pia::inet::NatDetecter::SendNatCheckMessage>  typeinfo 0x008CFEDC
//   nn::pia::common::ObjList<nn::pia::inet::NatProbe>  typeinfo 0x008CFF00
//   nn::pia::common::ObjList<nn::pia::inet::NatProbeRequest>  typeinfo 0x008CFEE8
//   nn::pia::common::ObjList<nn::pia::inet::NatTraversalTime>  typeinfo 0x008CFEF4
template <typename T>
class ObjList : public ListBase
{
public:
    // an object with its list node in front
    struct Node : public ListNode
    {
        T m_Value; // 0x8
    };

    ObjList() : m_pBuffer(nullptr), m_Capacity(0) { m_FreeList.SetOffset(0); }

    // the nodes of buffer (num of them) become the free nodes
    void Initialize(Node* buffer, u32 num)
    {
        if (num == 0 || buffer == nullptr) {
            return;
        }
        m_pBuffer = buffer;
        m_Capacity = num;
        for (u32 i = 0; i < num; i++) {
            ::new (static_cast<ListNode*>(&m_pBuffer[i])) ListNode();
            m_FreeList.PushBack(&m_pBuffer[i]);
        }
    }

    // all used nodes back to the free list
    void Clear()
    {
        if (m_pBuffer == nullptr) {
            return;
        }
        ClearNodes();
    }
    void ClearNodes()
    {
        for (ListNode* node = m_StartEnd.m_pNext; node != &m_StartEnd;) {
            ListNode* next = node->m_pNext;
            node->m_pPrev = nullptr;
            node->m_pNext = nullptr;
            m_FreeList.PushFront(static_cast<Node*>(node));
            node = next;
        }
        Init();
    }

    // a value initialized object from the free list at the end, null without free nodes
    T* PushBackNew()
    {
        if (m_FreeList.GetCount() == 0) {
            return nullptr;
        }
        Node* node = m_FreeList.PopBack();
        ::new (&node->m_Value) T();
        InsertBeforeNode(&m_StartEnd, node);
        return &node->m_Value;
    }

    // the same at the front
    T* PushFrontNew()
    {
        if (m_FreeList.GetCount() == 0) {
            return nullptr;
        }
        Node* node = m_FreeList.PopBack();
        ::new (&node->m_Value) T();
        InsertAfterNode(&m_StartEnd, node);
        return &node->m_Value;
    }

    // the object back to the free list
    void Erase(T* obj)
    {
        Node* node = ToNode(obj);
        EraseNode(node);
        m_FreeList.PushFront(node);
    }

    // the first object, null if there is none
    T* Front() const
    {
        Node* node = static_cast<Node*>(GetFrontNode());
        return node != nullptr ? &node->m_Value : nullptr;
    }
    // the last object, null if there is none
    T* Back() const
    {
        Node* node = static_cast<Node*>(GetBackNode());
        return node != nullptr ? &node->m_Value : nullptr;
    }
    // whether obj is one of the used objects
    bool Contains(const T* obj) const
    {
        for (Node* node = Begin(); node != End(); node = Advance(node)) {
            if (&node->m_Value == obj) {
                return true;
            }
        }
        return false;
    }

    static Node* ToNode(T* obj) { return reinterpret_cast<Node*>(reinterpret_cast<u8*>(obj) - offsetof(Node, m_Value)); }

    // iteration over the used nodes
    Node* Begin() const { return static_cast<Node*>(m_StartEnd.m_pNext); }
    Node* End() const { return static_cast<Node*>(const_cast<ListNode*>(&m_StartEnd)); }
    static Node* Advance(Node* node) { return static_cast<Node*>(node->m_pNext); }

    u32 GetCapacity() const { return m_Capacity; }
    s32 GetFreeCount() const { return m_FreeList.GetCount(); }

    OffsetList<Node> m_FreeList; // 0x10
    Node* m_pBuffer;             // 0x24
    u32 m_Capacity;              // 0x28
};
} // namespace common
} // namespace pia
} // namespace nn
