#pragma once

#include "decomp.h"
#include "pead/peadListImpl.h"

namespace pead {
// pia's copy of sead::TListNode: a ListNode with the object it stands for and the list it is in (layout from
// the constructor of the main thread 0x0053C328; the member names are ours).
template <typename T>
class TList;

template <typename T>
class TListNode : public ListNode
{
public:
    TListNode() : ListNode(), mData(), mList(nullptr) {}
    explicit TListNode(T data) : ListNode(), mData(data), mList(nullptr) {}

    T mData;         // 0x8
    TList<T>* mList; // 0xC, the list the node is in
};

// a list of TListNodes (names are ours)
template <typename T>
class TList : public ListImpl
{
public:
    void pushBack(TListNode<T>* node)
    {
        if (node->mList != nullptr) {
            node->mList->erase(node);
        }
        node->mList = this;
        mStartEnd.insertFront_(node);
        mCount++;
    }
    void erase(TListNode<T>* node)
    {
        node->mList = nullptr;
        node->erase_();
        mCount--;
    }

    TListNode<T>* front() const { return static_cast<TListNode<T>*>(mStartEnd.mNext); }
    const ListNode* end() const { return &mStartEnd; }
    static TListNode<T>* next(TListNode<T>* node) { return static_cast<TListNode<T>*>(node->mNext); }
};
} // namespace pead
