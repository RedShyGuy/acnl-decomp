#pragma once

#include "decomp.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N4sead8AudioMgr18SingletonDisposer_E @ 0x008D2170
// vtable 0x00906CA0 (vptr 0x00906CA8), offset_to_top 0, 2 entries
class sead::AudioMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x00560A98 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00560A40 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
