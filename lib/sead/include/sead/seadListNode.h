#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead8ListNodeE @ 0x008D2200
//
// A node of a doubly linked list (ListImpl). The class and function names are from the reference
// symbols, the member names are ours.
class ListNode
{
public:
    ListNode() : mPrev(nullptr), mNext(nullptr) {}

    // links node in front of this node
    void insertFront_(ListNode* node); // 0x001393A4 | nintendogs:callseq-callee [tier A]
    // takes the node out of its list
    void erase_(); // 0x0013C908 | nintendogs:callseq-callee [tier A]
    // links node behind this node
    void insertBack_(ListNode* node); // 0x00561170 | nintendogs:bytes [tier B]

    ListNode* mPrev; // 0x0
    ListNode* mNext; // 0x4
};
ASSERT_SIZE(ListNode, 0x8);
} // namespace sead
