#pragma once

#include "decomp.h"

// RTTI 6SvProc @ 0x008CD314
// vtable 0x008F8CF0 (vptr 0x008F8CF8), offset_to_top 0, 2 entries
class SvProc
{
public:
    SvProc(); // ctor candidate(s) 0x007855E8 (unverified)
    virtual void vf_0x00(); // 0x005CE220 slot 0x00 | virtual slot, introduced by SvProc
    virtual void vf_0x04(); // 0x005CE21C slot 0x04 | virtual slot, introduced by SvProc

    // a flag of the save state (+0xFAD of the object at 0x0094F3D8); g3d::Draw draws nothing while it is set (name
    // is ours)
    static bool IsUnknownFlag0xFADSet(); // 0x005CEE38
};
