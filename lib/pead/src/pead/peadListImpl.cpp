#include "pead/peadListImpl.h"

namespace pead {
// 0x0053D810 (name after sead)
void pead::ListImpl::sort(s32 offset, s32 (*cmp)(const void* a, const void* b))
{
    if (mCount <= 1) {
        return;
    }
    ListNode* node = mStartEnd.mNext->mNext;
    while (node != &mStartEnd) {
        ListNode* prev = node->mPrev;
        ListNode* next = node->mNext;
        const void* obj = reinterpret_cast<u8*>(node) - offset;
        if (cmp(reinterpret_cast<u8*>(prev) - offset, obj) > 0) {
            // behind the last node that is not greater
            ListNode* it = prev->mPrev;
            while (it != &mStartEnd && cmp(reinterpret_cast<u8*>(it) - offset, obj) > 0) {
                it = it->mPrev;
            }
            node->erase_();
            it->insertBack_(node);
        }
        node = next;
    }
}

// 0x0053D8DC | nintendogs:callseq-callee [tier A]
void pead::ListImpl::clear()
{
    ListNode* node = mStartEnd.mNext;
    while (node != &mStartEnd) {
        ListNode* next = node->mNext;
        node->mNext = nullptr;
        node->mPrev = nullptr;
        node = next;
    }
    mStartEnd.mNext = &mStartEnd;
    mCount = 0;
    mStartEnd.mPrev = &mStartEnd;
}
} // namespace pead
