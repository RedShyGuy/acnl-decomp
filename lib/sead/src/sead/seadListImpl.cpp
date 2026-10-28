#include "sead/seadListImpl.h"

namespace sead {
// 0x0012CD84 | nintendogs:bytes [tier A]
sead::ListNode* sead::ListImpl::popFront()
{
    if (mCount <= 0) {
        return nullptr;
    }
    ListNode* node = mStartEnd.mNext;
    node->erase_();
    mCount--;
    return node;
}

// 0x0074F944 | nintendogs:bytes [tier A]
s32 sead::ListImpl::indexOf(const ListNode* node) const
{
    s32 index = 0;
    for (const ListNode* it = mStartEnd.mNext; it != &mStartEnd; it = it->mNext) {
        if (it == node) {
            return index;
        }
        index++;
    }
    return -1;
}

// 0x00561138 (name after pead::ListImpl)
void sead::ListImpl::clear()
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

// 0x0056106C (name is ours)
void sead::ListImpl::sort(s32 offset, s32 (*cmp)(const void* a, const void* b))
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

} // namespace sead
