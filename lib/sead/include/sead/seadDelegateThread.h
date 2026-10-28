#pragma once

#include "decomp.h"
#include "sead/seadIDelegate2.h"
#include "sead/seadThread.h"

namespace sead {
// RTTI N4sead14DelegateThreadE @ 0x008D16E0
// vtable 0x00905748 (vptr 0x00905750), offset_to_top 0, 18 entries
// vtable 0x00905798 (vptr 0x009057A0), offset_to_top -24, 1 entries
//
// A thread that calls the delegate for each message (the parameter and member names are ours).
class DelegateThread : public ::sead::Thread
{
public:
    DelegateThread(const SafeStringBase<char>& name, IDelegate2<Thread*, MessageQueue::Element>* delegate, Heap* heap,
                   s32 priority, MessageQueue::BlockType blockType, MessageQueue::Element quitMessage, s32 stackSize,
                   s32 messageQueueSize); // 0x00544838 (name is ours)
    virtual ~DelegateThread(); // 0x0054488C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054487C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void calc_(MessageQueue::Element message); // 0x0054481C slot 0x40

    IDelegate2<Thread*, MessageQueue::Element>* mDelegate; // 0x90
};
ASSERT_SIZE(DelegateThread, 0x94);
} // namespace sead
