#pragma once

#include "decomp.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N16SoundDataLoadMgr18SingletonDisposer_E @ 0x008CDAD8
// vtable 0x008FBA5C (vptr 0x008FBA64), offset_to_top 0, 2 entries
class SoundDataLoadMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x002BD010
    // 0x002BCFAC (deleting dtor)
};
