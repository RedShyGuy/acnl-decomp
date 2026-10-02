#pragma once

#include "decomp.h"
#include "Other/dTourResult.h"
#include "sead/seadIDisposer.h"

// RTTI N10TourResult18SingletonDisposer_E @ 0x008CD858
// vtable 0x008FAACC (vptr 0x008FAAD4), offset_to_top 0, 2 entries
class TourResult::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x001B7C44 (unverified)
    virtual ~SingletonDisposer_(); // 0x001B7E14 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001B7DD0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
