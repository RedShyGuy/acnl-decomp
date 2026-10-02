#pragma once

#include "decomp.h"
#include "g3d/dFragmentLight.h"

namespace g3d {
// RTTI N3g3d24DirectionalFragmentLightE @ 0x008D0D3C
// vtable 0x00903B34 (vptr 0x00903B3C), offset_to_top 0, 7 entries
class DirectionalFragmentLight : public ::g3d::FragmentLight
{
public:
    DirectionalFragmentLight(); // ctor address unknown
    virtual void vf_0x00(); // 0x004F0F28 slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004F0F04 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x14(); // 0x004F0EA0 slot 0x14 | virtual slot, introduced by g3d::FragmentLight
};
} // namespace g3d
