#pragma once

#include "decomp.h"
#include "sead/seadThread.h"

namespace sead {
// RTTI N4sead10MainThreadE @ 0x008D13C0
// vtable 0x00904EC4 (vptr 0x00904ECC), offset_to_top 0, 18 entries
// vtable 0x00904F14 (vptr 0x00904F1C), offset_to_top -24, 1 entries
//
// The thread that runs main: it is not started, quit or waited for by sead.
class MainThread : public ::sead::Thread
{
public:
    MainThread(Heap* heap, nn::os::Thread* threadInner, u32 id) : Thread(heap, threadInner, id) {}
    virtual ~MainThread(); // 0x0053F224 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0053F1F8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void destroy(); // 0x0053F1F0 slot 0x08
    virtual void quit(bool isJam); // 0x0053F1E8 slot 0x1C
    virtual void waitDone(); // 0x0053F1F4 slot 0x20
    virtual void quitAndDestroySingleThread(bool isJam); // 0x0053F1E4 slot 0x24
    virtual void setPriority(s32 priority); // 0x0053F1E0 slot 0x2C
    virtual void calc_(MessageQueue::Element message); // 0x0053F1EC slot 0x40
};
} // namespace sead
