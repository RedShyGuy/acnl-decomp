#pragma once

#include "decomp.h"
#include "pead/peadListNode.h"

namespace pead {
// pia's copy of sead::ListImpl: a doubly linked list of ListNodes around an anchor node (the first node is mStartEnd.mNext, the
// last mStartEnd.mPrev). The class and function names are from the reference symbols, the member
// names are ours.
class ListImpl
{
public:
    ListImpl() : mCount(0)
    {
        mStartEnd.mNext = &mStartEnd;
        mStartEnd.mPrev = &mStartEnd;
    }

    // unlinks all nodes (name after pead::ListImpl)
    void clear(); // 0x0053D8DC | nintendogs:callseq-callee [tier A]
    // orders the objects (whose nodes are offset bytes into them) by cmp; stable (name is ours)
    void sort(s32 offset, s32 (*cmp)(const void* a, const void* b)); // 0x0053D810 (name after sead)

    ListNode mStartEnd; // 0x0
    s32 mCount;         // 0x8
};
ASSERT_SIZE(ListImpl, 0xC);

// A ListImpl of objects of type T whose ListNode is mOffset bytes into the object (names are ours).
template <typename T>
class OffsetList : public ListImpl
{
public:
    OffsetList() : mOffset(-1) {}

    void initOffset(s32 offset) { mOffset = offset; }

    ListNode* objToListNode(T* obj) const { return reinterpret_cast<ListNode*>(reinterpret_cast<u8*>(obj) + mOffset); }
    T* listNodeToObj(ListNode* node) const { return reinterpret_cast<T*>(reinterpret_cast<u8*>(node) - mOffset); }

    void pushBack(T* obj)
    {
        mStartEnd.insertFront_(objToListNode(obj));
        mCount++;
    }
    // links obj in front of basis
    void insertBefore(T* basis, T* obj)
    {
        objToListNode(basis)->insertFront_(objToListNode(obj));
        mCount++;
    }
    void erase(T* obj)
    {
        objToListNode(obj)->erase_();
        mCount--;
    }

    // the first object; endObj() if the list is empty
    T* front() const { return listNodeToObj(mStartEnd.mNext); }
    // the object behind the end (the anchor)
    T* endObj() const { return listNodeToObj(const_cast<ListNode*>(&mStartEnd)); }
    T* next(T* obj) const { return listNodeToObj(objToListNode(obj)->mNext); }
    T* prev(T* obj) const { return listNodeToObj(objToListNode(obj)->mPrev); }
    // the first / last object, NULL if the list is empty
    T* frontOrNull() const { return mCount <= 0 ? nullptr : listNodeToObj(mStartEnd.mNext); }
    T* backOrNull() const { return mCount <= 0 ? nullptr : listNodeToObj(mStartEnd.mPrev); }
    void sort(s32 (*cmp)(const void* a, const void* b)) { ListImpl::sort(mOffset, cmp); }

    s32 mOffset; // 0xC
};
} // namespace pead
