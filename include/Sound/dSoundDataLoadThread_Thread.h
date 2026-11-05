#pragma once

#include "decomp.h"
#include "Sound/dSoundDataLoadThread.h"
#include "sead/seadThread.h"

// RTTI N19SoundDataLoadThread6ThreadE @ 0x008CDB50
// vtable 0x008FBB10 (vptr 0x008FBB18), offset_to_top 0, 18 entries
// vtable 0x008FBB60 (vptr 0x008FBB68), offset_to_top -24, 1 entries
class SoundDataLoadThread::Thread : public ::sead::Thread
{
public:
    Thread(const sead::SafeStringBase<char>& name, sead::Heap* heap, s32 priority)
        : sead::Thread(name, heap, priority, sead::MessageQueue::BLOCK_TYPE_BLOCK, 0x7FFFFFFF, 0x1000, 32)
    {
    }
    virtual ~Thread();
    // loads the requests of the list until it is empty
    virtual void calc_(sead::MessageQueue::Element message) override; // 0x002FEE84 slot 0x40
};
