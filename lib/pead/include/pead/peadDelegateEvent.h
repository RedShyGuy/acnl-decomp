#pragma once

#include "decomp.h"
#include "pead/peadIDelegate1.h"
#include "pead/peadIDisposer.h"
#include "pead/peadTListNode.h"

namespace pead {
// Instantiations found in the binary:
//   pead::DelegateEvent<pead::PrintConfig::PrintEventArg const&>  typeinfo 0x008D1170  vtable 0x009049FC
//
// pia's copy of sead::DelegateEvent: an event that calls the delegates of its connected slots (PrintConfig's
// output event). Member names are ours.
template <typename T0>
class DelegateEvent
{
public:
    class Slot : public IDisposer
    {
    public:
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

    virtual ~DelegateEvent() // <PrintConfig::PrintEventArg const&>: see peadPrintConfig.cpp
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
} // namespace pead
