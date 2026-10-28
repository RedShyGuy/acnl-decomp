#include "sead/seadListNode.h"

namespace sead {
// 0x001393A4 | nintendogs:callseq-callee [tier A]
void sead::ListNode::insertFront_(ListNode* node)
{
    ListNode* prev = mPrev;
    mPrev = node;
    node->mNext = this;
    node->mPrev = prev;
    if (prev != nullptr) {
        prev->mNext = node;
    }
}

// 0x0013C908 | nintendogs:callseq-callee [tier A]
void sead::ListNode::erase_()
{
    if (mPrev != nullptr) {
        mPrev->mNext = mNext;
    }
    if (mNext != nullptr) {
        mNext->mPrev = mPrev;
    }
    mNext = nullptr;
    mPrev = nullptr;
}

// 0x00561170 | nintendogs:bytes [tier B]
void sead::ListNode::insertBack_(ListNode* node)
{
    ListNode* next = mNext;
    mNext = node;
    node->mPrev = this;
    node->mNext = next;
    if (next != nullptr) {
        next->mPrev = node;
    }
}

} // namespace sead
