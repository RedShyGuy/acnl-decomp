#pragma once

#include "decomp.h"
#include "Other/dVisitorLocalInfo.h"
#include "sead/seadIDisposer.h"

// RTTI N16VisitorLocalInfo18SingletonDisposer_E @ 0x008CDAE4
// vtable 0x008FBA6C (vptr 0x008FBA74), offset_to_top 0, 2 entries
class VisitorLocalInfo::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x002C2790 (unverified)
    virtual ~SingletonDisposer_(); // 0x002C289C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x002C2858 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
