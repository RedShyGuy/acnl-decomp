#pragma once

#include "decomp.h"
#include "Other/dTourData.h"
#include "sead/seadIDisposer.h"

// RTTI N8TourData18SingletonDisposer_E @ 0x008D4048
// vtable 0x0090C104 (vptr 0x0090C10C), offset_to_top 0, 2 entries
class TourData::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x006C4634 (unverified)
    virtual ~SingletonDisposer_(); // 0x006C492C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006C48C0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
