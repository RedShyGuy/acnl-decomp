#pragma once

#include "decomp.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace fnd {
// Instantiations found in the binary:
//   nn::fnd::IntrusiveLinkedList<nn::fnd::HeapBase, void>::Item  typeinfo 0x008CDEC4
//   nn::fnd::IntrusiveLinkedList<nn::srv::NotificationHandler, void>::Item  typeinfo 0x008CDEDC
//
// A circular doubly linked list of objects that derive from Item; the list holds the first item
// (whose prev is the last one). The class and Item are from RTTI; the members and functions are
// ours, all inline (seen in nn::srv).
template <typename T, typename Tag>
class IntrusiveLinkedList
{
public:
    class Item : private ::nn::util::ADLFireWall::NonCopyable<Item>
    {
    public:
        Item() : mPrev(0), mNext(0) {}

    private:
        friend class IntrusiveLinkedList;

        Item* mPrev;    // 0x0
        Item* mNext;    // 0x4
    };

    IntrusiveLinkedList() : mFirst(0) {}

    T* GetFront() const { return static_cast<T*>(mFirst); }
    T* GetBack() const { return mFirst != 0 ? static_cast<T*>(mFirst->mPrev) : 0; }

    // the item after p, 0 after the last one
    T* GetNext(T* p) const
    {
        if (p == GetBack()) {
            return 0;
        }
        return static_cast<T*>(static_cast<Item*>(p)->mNext);
    }

    void PushBack(T* p)
    {
        Item* item = static_cast<Item*>(p);
        if (mFirst == 0) {
            item->mNext = item;
            item->mPrev = item;
            mFirst = item;
        } else {
            item->mNext = mFirst;
            mFirst->mPrev->mNext = item;
            item->mPrev = mFirst->mPrev;
            mFirst->mPrev = item;
        }
    }

private:
    Item* mFirst;   // 0x0
};
} // namespace fnd
} // namespace nn
