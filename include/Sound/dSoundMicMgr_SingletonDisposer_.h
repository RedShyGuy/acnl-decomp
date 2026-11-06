#pragma once

#include "decomp.h"
#include "Sound/dSoundMicMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N11SoundMicMgr18SingletonDisposer_E @ 0x008CD8D0
// vtable 0x008FAE1C (vptr 0x008FAE24), offset_to_top 0, 2 entries
class SoundMicMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x001D9CB8 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001D9C68 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
