#pragma once

#include "decomp.h"
#include "Sound/dSoundMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N8SoundMgr18SingletonDisposer_E @ 0x008D403C
// vtable 0x0090C0F4 (vptr 0x0090C0FC), offset_to_top 0, 2 entries
class SoundMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x006B3C0C
    // 0x006B3BBC (deleting dtor)
};
