#pragma once

#include "decomp.h"
#include "Sound/dSoundFurnitureMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N17SoundFurnitureMgr18SingletonDisposer_E @ 0x008CDB08
// vtable 0x008FBAA0 (vptr 0x008FBAA8), offset_to_top 0, 2 entries
class SoundFurnitureMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x002D20D4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x002D2090 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
