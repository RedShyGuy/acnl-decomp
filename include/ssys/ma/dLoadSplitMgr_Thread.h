#pragma once

#include "decomp.h"
#include "sead/seadThread.h"
#include "ssys/ma/dLoadSplitMgr.h"

// the thread that works through the queue of LoadSplitMgr when it gets a message
// RTTI N4ssys2ma12LoadSplitMgr6ThreadE @ 0x008D241C
// vtable 0x009071D0 (vptr 0x009071D8), offset_to_top 0, 18 entries
// vtable 0x00907220 (vptr 0x00907228), offset_to_top -24, 1 entries
class ssys::ma::LoadSplitMgr::Thread : public ::sead::Thread
{
public:
    // (inline in CreateThread)
    Thread(const sead::SafeStringBase<char>& name, sead::Heap* heap, s32 priority, sead::MessageQueue::BlockType blockType,
           sead::MessageQueue::Element quitMessage, s32 stackSize, s32 messageQueueSize)
        : sead::Thread(name, heap, priority, blockType, quitMessage, stackSize, messageQueueSize)
    {
    }
    virtual ~Thread(); // 0x00567D08 slot 0x00
    void calc_(sead::MessageQueue::Element message) override; // 0x00567BB4 slot 0x40 (name after sead::Thread)
};
