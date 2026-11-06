#pragma once

#include "decomp.h"
#include "Sound/dSoundMuInfo.h"
#include "sead/seadIDisposer.h"

// RTTI N11SoundMuInfo18SingletonDisposer_E @ 0x008CD8DC
// vtable 0x008FAE2C (vptr 0x008FAE34), offset_to_top 0, 2 entries
class SoundMuInfo::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x001DA100 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001DA0BC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
