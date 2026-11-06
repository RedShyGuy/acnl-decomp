#pragma once

#include "decomp.h"
#include "Sound/dSoundSeaMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N11SoundSeaMgr18SingletonDisposer_E @ 0x008CD8E8
// vtable 0x008FAE3C (vptr 0x008FAE44), offset_to_top 0, 2 entries
class SoundSeaMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x001DA8D0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001DA86C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
