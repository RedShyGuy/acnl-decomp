#pragma once

#include "decomp.h"
#include "Sound/dSoundHistoryMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N15SoundHistoryMgr18SingletonDisposer_E
// vtable 0x008FBA30 (vptr 0x008FBA38), offset_to_top 0, 2 entries
class SoundHistoryMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x002A3910
    // 0x002A38CC (deleting dtor)
};
