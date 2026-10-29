#include "pead/peadListNode.h"

namespace pead {
// 0x0053D914 (name after sead)
void pead::ListNode::insertFront_(ListNode* node)
{
    ListNode* prev = mPrev;
    mPrev = node;
    node->mNext = this;
    node->mPrev = prev;
    if (prev != nullptr) {
        prev->mNext = node;
    }
}

// 0x0053D930 (name after sead)
void pead::ListNode::erase_()
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
} // namespace pead
