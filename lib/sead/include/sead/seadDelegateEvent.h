#pragma once

#include "decomp.h"
#include "sead/seadDelegate1.h"
#include "sead/seadIDelegate1.h"
#include "sead/seadIDisposer.h"
#include "sead/seadTListNode.h"

namespace sead {
// Instantiations found in the binary:
//   sead::DelegateEvent<char const*>  typeinfo 0x008D162C  vtable 0x009054C0
//   sead::DelegateEvent<sead::TaskBase*>  typeinfo 0x008D1640  vtable 0x009054E0
//   sead::DelegateEvent<sead::TaskBase*>::Slot  typeinfo 0x008D1634  vtable 0x009054D0
//   sead::DelegateEvent<void*>  typeinfo 0x008D1648  vtable 0x009054F0
//
// An event that calls the delegates of its connected slots. Only the destructors are in the binary:
// they disconnect the slots. Member names are ours.
template <typename T0>
class DelegateEvent
{
public:
    class Slot : public IDisposer
    {
    public:
        // a slot that calls method of obj (inline)
        template <typename T>
        Slot(T* obj, void (T::*method)(T0)) : IDisposer(), mNode(this), mDelegate(nullptr), mIsConnected(false)
        {
            mDelegate = new (mDelegateHolder) Delegate1<T, T0>(obj, method);
        }
        virtual ~Slot() { release(); }

        // takes the slot out of the event
        void release()
        {
            if (mIsConnected) {
                if (mNode.mList != nullptr) {
                    mNode.mList->erase(&mNode);
                }
                mIsConnected = false;
            }
        }

        TListNode<Slot*> mNode;     // 0x10
        IDelegate1<T0>* mDelegate;  // 0x20, the delegate (in the holder)
        u32 mDelegateHolder[5];     // 0x24
        bool mIsConnected;          // 0x38
    };

    virtual ~DelegateEvent()
    {
        ListNode* node = mSlotList.mStartEnd.mNext;
        while (node != &mSlotList.mStartEnd) {
            Slot* slot = static_cast<TListNode<Slot*>*>(node)->mData;
            node = node->mNext;
            slot->release();
        }
    }

    // adds slot (taken out of the event it was in)
    void connect(Slot* slot)
    {
        slot->release();
        mSlotList.pushBack(&slot->mNode);
        slot->mIsConnected = true;
    }

    // calls the delegates of the connected slots with arg
    void fire(T0 arg)
    {
        ListNode* node = mSlotList.mStartEnd.mNext;
        while (node != &mSlotList.mStartEnd) {
            ListNode* next = node->mNext;
            Slot* slot = static_cast<TListNode<Slot*>*>(node)->mData;
            if (slot->mDelegate != nullptr) {
                slot->mDelegate->invoke(arg);
            }
            node = next;
        }
    }

    TList<Slot*> mSlotList; // 0x04
};
} // namespace sead
