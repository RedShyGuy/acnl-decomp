#pragma once

#include "decomp.h"
#include "sead/seadAccelerometerAddon.h"

namespace sead {
// RTTI N4sead21CtrAccelerometerAddonE @ 0x008D1E84
// vtable 0x00906568 (vptr 0x00906570), offset_to_top 0, 5 entries
class CtrAccelerometerAddon : public ::sead::AccelerometerAddon
{
public:
    CtrAccelerometerAddon(); // ctor candidate(s) 0x0054B400 (unverified)
    virtual void vf_0x00(); // 0x0074D6D8 slot 0x00 | virtual slot, introduced by sead::CtrAccelerometerAddon
    virtual void vf_0x04(); // 0x0074D68C slot 0x04 | virtual slot, introduced by sead::CtrAccelerometerAddon
    virtual void vf_0x08(); // 0x0054B450 slot 0x08 | virtual slot, introduced by sead::CtrAccelerometerAddon
    virtual void vf_0x0C(); // 0x0054B44C slot 0x0C | virtual slot, introduced by sead::CtrAccelerometerAddon
    virtual void vf_0x10(); // 0x0054B31C slot 0x10 | virtual slot, introduced by sead::CtrAccelerometerAddon
    CtrAccelerometerAddon(sead::Controller*); // 0x0054B400 | nintendogs:bytes [tier A]
};
} // namespace sead
