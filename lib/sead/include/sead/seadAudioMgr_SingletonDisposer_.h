#pragma once

#include "decomp.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N4sead8AudioMgr18SingletonDisposer_E @ 0x008D2170
// vtable 0x00906CA0 (vptr 0x00906CA8), offset_to_top 0, 2 entries
//
// Forgets the AudioMgr when its heap is destroyed.
class sead::AudioMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x00560A98 slot 0x00
    // 0x00560A40 slot 0x04 (deleting dtor)
};
