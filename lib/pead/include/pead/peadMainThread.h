#pragma once

#include "decomp.h"
#include "pead/peadThread.h"

namespace pead {
// RTTI N4pead10MainThreadE @ 0x008D1144
// vtable 0x009049B0 (vptr 0x009049B8), offset_to_top 0, 17 entries
//
// The Thread object of the thread that runs main (pia's copy of sead::MainThread): its system thread is not
// pead's, so the slots that control it do nothing.
class MainThread : public ::pead::Thread
{
public:
    MainThread(Heap* heap, nn::os::Thread* threadInner, u32 id) : Thread(heap, threadInner, id) {}
    virtual ~MainThread(); // 0x00538014 slot 0x00 | nintendogs:bytes
    // 0x00537FEC slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    virtual void destroy(); // 0x00537FE4 slot 0x08
    virtual void quit(bool isJam); // 0x00537FDC slot 0x18
    virtual void waitDone(); // 0x00537FE8 slot 0x1C
    virtual void quitAndDestroySingleThread(bool isJam); // 0x00537FD8 slot 0x20
    virtual void setPriority(s32 priority); // 0x00537FD4 slot 0x28
    virtual void calc_(MessageQueue::Element message); // 0x00537FE0 slot 0x3C
};
} // namespace pead
