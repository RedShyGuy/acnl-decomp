#pragma once

#include "decomp.h"
#include "pead/peadIDelegate2.h"
#include "pead/peadSafeStringBase.h"
#include "pead/peadThread.h"

namespace pead {
// RTTI N4pead14DelegateThreadE @ 0x008D1178
// vtable 0x00904A0C (vptr 0x00904A14), offset_to_top 0, 17 entries
class DelegateThread : public ::pead::Thread
{
public:
    // a thread that calls the delegate for each message (pia's common::BackgroundScheduler passes
    // its priority, 1, 0x7FFFFFFF, 0x1000, 32 for the last five); the parameter names are ours
    DelegateThread(const SafeStringBase<char>& name, IDelegate2<Thread*, int>* delegate, Heap* heap, int priority, int blockType, int quitMessage, int stackSize, int messageQueueSize); // 0x00538788
    virtual ~DelegateThread(); // 0x0053C570 slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x005387C4 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    virtual void vf_0x3C(); // 0x0053876C slot 0x3C | virtual slot, introduced by pead::Thread

    IDelegate2<Thread*, int>* mDelegate; // 0x88 (name is ours)
};
ASSERT_SIZE(DelegateThread, 0x8C);
} // namespace pead
