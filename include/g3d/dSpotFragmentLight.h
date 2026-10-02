#pragma once

#include "decomp.h"
#include "g3d/dFragmentLight.h"

namespace g3d {
// RTTI N3g3d17SpotFragmentLightE @ 0x008D0D18
// vtable 0x00903AC0 (vptr 0x00903AC8), offset_to_top 0, 7 entries
class SpotFragmentLight : public ::g3d::FragmentLight
{
public:
    SpotFragmentLight(); // ctor address unknown
    virtual void vf_0x00(); // 0x004F0174 slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004F0150 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x14(); // 0x004F00B0 slot 0x14 | virtual slot, introduced by g3d::FragmentLight
};
} // namespace g3d
