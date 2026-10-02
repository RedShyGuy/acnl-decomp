#pragma once

#include "decomp.h"
#include "Other/dTourList.h"
#include "sead/seadIDisposer.h"

// RTTI N8TourList18SingletonDisposer_E @ 0x008D4054
// vtable 0x0090C114 (vptr 0x0090C11C), offset_to_top 0, 2 entries
class TourList::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x006C5D34 (unverified)
    virtual ~SingletonDisposer_(); // 0x006C5DE4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006C5DA0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
