#pragma once

#include "decomp.h"

namespace pead {
//
// pia's copy of sead::ListNode: a node of a doubly linked list (ListImpl). The class and function names are from the reference
// symbols, the member names are ours.
class ListNode
{
public:
    ListNode() : mPrev(nullptr), mNext(nullptr) {}

    // links node in front of this node
    void insertFront_(ListNode* node); // 0x0053D914 (name after sead)
    // takes the node out of its list
    void erase_(); // 0x0053D930 (name after sead)
    // links node behind this node (inline: pead has no copy of it; name after sead)
    void insertBack_(ListNode* node)
    {
        ListNode* next = mNext;
        mNext = node;
        node->mPrev = this;
        node->mNext = next;
        if (next != nullptr) {
            next->mPrev = node;
        }
    }

    ListNode* mPrev; // 0x0
    ListNode* mNext; // 0x4
};
ASSERT_SIZE(ListNode, 0x8);
} // namespace pead
