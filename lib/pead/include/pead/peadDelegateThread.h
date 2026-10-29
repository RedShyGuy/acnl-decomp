#pragma once

#include "decomp.h"
#include "pead/peadIDelegate2.h"
#include "pead/peadSafeStringBase.h"
#include "pead/peadThread.h"

namespace pead {
// RTTI N4pead14DelegateThreadE @ 0x008D1178
// vtable 0x00904A0C (vptr 0x00904A14), offset_to_top 0, 17 entries
//
// A thread that calls the delegate for each message (pia's copy of sead::DelegateThread; pia's
// common::BackgroundScheduler passes its priority, BLOCK_TYPE_NO_BLOCK, 0x7FFFFFFF, 0x1000, 32).
class DelegateThread : public ::pead::Thread
{
public:
    DelegateThread(const SafeStringBase<char>& name, IDelegate2<Thread*, MessageQueue::Element>* delegate, Heap* heap,
                   s32 priority, MessageQueue::BlockType blockType, MessageQueue::Element quitMessage, s32 stackSize,
                   s32 messageQueueSize); // 0x00538788 (name after sead)
    // (the original falls into the destructor of Thread with a nop)
    virtual ~DelegateThread(); // 0x0053C570 slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x005387C4 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    virtual void calc_(MessageQueue::Element message); // 0x0053876C slot 0x3C

    IDelegate2<Thread*, MessageQueue::Element>* mDelegate; // 0x88
};
ASSERT_SIZE(DelegateThread, 0x8C);
} // namespace pead
