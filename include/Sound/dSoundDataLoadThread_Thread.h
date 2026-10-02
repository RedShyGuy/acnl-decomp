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
    Thread(); // ctor address unknown
    virtual ~Thread(); // 0x002FF098 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x002FF088 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x40(); // 0x002FEE84 slot 0x40 | virtual slot, introduced by sead::Thread
};
