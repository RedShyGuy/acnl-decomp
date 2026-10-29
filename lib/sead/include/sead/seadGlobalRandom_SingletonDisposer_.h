#pragma once

#include "decomp.h"
#include "sead/seadGlobalRandom.h"
#include "sead/seadIDisposer.h"

// RTTI N4sead12GlobalRandom18SingletonDisposer_E @ 0x008D1594
// vtable 0x0090521C (vptr 0x00905224), offset_to_top 0, 2 entries
//
// Forgets the GlobalRandom when its heap is destroyed.
class sead::GlobalRandom::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x00540ECC slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00540E88 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
