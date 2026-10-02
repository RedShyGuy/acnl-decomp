#pragma once

#include "decomp.h"
#include "sead/seadSoundHandle.h"

// RTTI 18SoundHandleBindSeq @ 0x008CC8D4
// vtable 0x008F4AFC (vptr 0x008F4B04), offset_to_top 0, 2 entries
class SoundHandleBindSeq : public ::sead::SoundHandle
{
public:
    SoundHandleBindSeq(); // ctor candidate(s) 0x002E837C (unverified)
    virtual void vf_0x00(); // 0x002E83C0 slot 0x00 | virtual slot, introduced by SoundHandleBindSeq
    virtual void vf_0x04(); // 0x002E8394 slot 0x04 | virtual slot, introduced by SoundHandleBindSeq
};
