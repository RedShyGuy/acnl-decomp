#pragma once

#include "decomp.h"
#include "Other/dBlackWait.h"
#include "sead/seadIDisposer.h"

// RTTI N9BlackWait18SingletonDisposer_E @ 0x008D40A8
// vtable 0x0090C20C (vptr 0x0090C214), offset_to_top 0, 2 entries
class BlackWait::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0011F2C4 (unverified)
    virtual ~SingletonDisposer_(); // 0x006CF884 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006CF82C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
