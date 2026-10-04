#pragma once

#include "decomp.h"
#include "pead/hostio/peadReflexible.h"
#include "pead/peadIDisposer.h"
#include "pead/peadINamable.h"
#include "sead/seadMessageQueue.h"

namespace pead {
// RTTI N4pead6ThreadE @ 0x008D128C
// vtable 0x00904CE8 (vptr 0x00904CF0), offset_to_top 0, 17 entries
class Thread : public ::pead::IDisposer, public ::pead::INamable, public ::pead::hostio::Reflexible
{
public:
    Thread(); // ctor address unknown
    virtual ~Thread(); // 0x0053C574 slot 0x00 | nintendogs:callgraph
    // 0x0053C560 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0053C2D0 slot 0x08 | virtual slot, introduced by pead::Thread
    virtual void sendMessage(int, sead::MessageQueue::BlockType); // 0x005383C4 slot 0x0C | nintendogs:bytes
    virtual void vf_0x10(); // 0x0053BFA0 slot 0x10 | virtual slot, introduced by pead::Thread
    virtual void start(); // 0x0053C1C0 slot 0x14 | nintendogs:bytes
    virtual void quit(bool isJam); // 0x0053C0F0 slot 0x18 (name is ours) | virtual slot, introduced by pead::Thread
    virtual void waitDone(); // 0x0053C2DC slot 0x1C (name is ours) | virtual slot, introduced by pead::Thread
    virtual void vf_0x20(); // 0x0053C0B8 slot 0x20 | virtual slot, introduced by pead::Thread
    virtual void quitAndWaitDoneSingleThread(bool); // 0x0053C0C4 slot 0x24 | nintendogs:bytes
    virtual void setPriority(int); // 0x0053BFBC slot 0x28 | nintendogs:bytes
    virtual void vf_0x2C(); // 0x00749534 slot 0x2C | virtual slot, introduced by pead::Thread
    virtual void vf_0x30(); // 0x0074953C slot 0x30 | virtual slot, introduced by pead::Thread
    virtual void vf_0x34(); // 0x00749544 slot 0x34 | virtual slot, introduced by pead::Thread
    virtual void vf_0x38(); // 0x0074954C slot 0x38 | virtual slot, introduced by pead::Thread
    virtual void vf_0x3C(); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x40(); // 0x007495BC slot 0x40 | virtual slot, introduced by pead::Thread

    // 0x10, members of INamable / hostio::Reflexible and of the thread, not decompiled yet
    // (DelegateThread adds its delegate at 0x88: constructor 0x00538788)
    u8 mThreadData[0x78];
};
ASSERT_SIZE(Thread, 0x88);
} // namespace pead
