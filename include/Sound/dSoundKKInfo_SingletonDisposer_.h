#pragma once

#include "decomp.h"
#include "Sound/dSoundKKInfo.h"
#include "sead/seadIDisposer.h"

// RTTI N11SoundKKInfo18SingletonDisposer_E @ 0x008CD8C4
// vtable 0x008FAE0C (vptr 0x008FAE14), offset_to_top 0, 2 entries
class SoundKKInfo::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x001D9AD4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001D9A90 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
