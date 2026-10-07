#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// A node of an intrusive doubly linked list. The class name is from the signatures of ListBase,
// the members are ours. It derives RootObject like the other pia classes: that is why the node
// in ListBase starts at 4 (two RootObject subobjects cannot share an address).
class ListNode : public RootObject
{
public:
    ListNode() : m_pPrev(nullptr), m_pNext(nullptr) {}

    ListNode* m_pPrev; // 0x0
    ListNode* m_pNext; // 0x4
};
ASSERT_SIZE(ListNode, 0x8);

// RTTI N2nn3pia6common8ListBaseE @ 0x008CFF18
//
// A ring of ListNodes around m_StartEnd (prev = back, next = front). Layout from Init; the member
// names are ours.
class ListBase : public RootObject
{
public:
    ListBase() { Init(); }

    ListNode* PopBackNode(); // 0x0042955C | fefates:bytes [tier B]
    ListNode* PopFrontNode(); // 0x0042959C | fefates:bytes [tier B]
    void InsertAfterNode(nn::pia::common::ListNode* pos, nn::pia::common::ListNode* node); // 0x004295F0 | fefates:bytes [tier B]
    void InsertBeforeNode(nn::pia::common::ListNode* pos, nn::pia::common::ListNode* node); // 0x004296B4 | fefates:bytes [tier B]
    void Init(); // 0x00429740 | fefates:bytes [tier B]
    void EraseNode(nn::pia::common::ListNode* node); // 0x00429758 | fefates:bytes [tier B]
    bool IsIncludeNode(const nn::pia::common::ListNode* node) const; // 0x007333D8 | fefates:bytes [tier B]

    s32 GetCount() const { return m_Count; }
    ListNode* GetBackNode() const { return m_Count != 0 ? m_StartEnd.m_pPrev : nullptr; }
    ListNode* GetFrontNode() const { return m_Count != 0 ? m_StartEnd.m_pNext : nullptr; }

    ListNode m_StartEnd; // 0x4
    s32 m_Count;         // 0xC
};
ASSERT_SIZE(ListBase, 0x10);

// A list of objects that hold their ListNode at m_Offset (the inline functions of the Scheduler
// lists; the template name and its members are ours)
template <typename T>
class OffsetList : public ListBase
{
public:
    OffsetList() : m_Offset(-1) {}

    void SetOffset(s32 offset) { m_Offset = offset; }

    ListNode* ToNode(T* obj) const
    {
        return reinterpret_cast<ListNode*>(reinterpret_cast<u8*>(obj) + m_Offset);
    }
    T* ToObj(ListNode* node) const
    {
        return reinterpret_cast<T*>(reinterpret_cast<u8*>(node) - m_Offset);
    }

    // the first / last object, null if empty
    T* Front() const
    {
        ListNode* node = GetFrontNode();
        return node != nullptr ? ToObj(node) : nullptr;
    }
    T* Back() const
    {
        ListNode* node = GetBackNode();
        return node != nullptr ? ToObj(node) : nullptr;
    }
    // iteration up to End() (the start / end node as an object)
    T* Begin() const { return ToObj(m_StartEnd.m_pNext); }
    T* End() const { return ToObj(const_cast<ListNode*>(&m_StartEnd)); }
    T* Advance(T* obj) const { return ToObj(ToNode(obj)->m_pNext); }

    // the object after obj, null at the end
    T* Next(T* obj) const
    {
        ListNode* node = ToNode(obj)->m_pNext;
        return node != &m_StartEnd ? ToObj(node) : nullptr;
    }

    void PushFront(T* obj) { InsertAfterNode(&m_StartEnd, ToNode(obj)); }
    void PushBack(T* obj) { InsertBeforeNode(&m_StartEnd, ToNode(obj)); }
    void InsertBefore(T* pos, T* obj) { InsertBeforeNode(ToNode(pos), ToNode(obj)); }
    T* PopFront()
    {
        ListNode* node = PopFrontNode();
        return node != nullptr ? ToObj(node) : nullptr;
    }
    T* PopBack()
    {
        ListNode* node = PopBackNode();
        return node != nullptr ? ToObj(node) : nullptr;
    }
    bool IsInclude(T* obj) const { return IsIncludeNode(ToNode(obj)); }
    void Erase(T* obj) { EraseNode(ToNode(obj)); }

    s32 m_Offset; // 0x10, offset of the ListNode in T
};
} // namespace common
} // namespace pia
} // namespace nn
