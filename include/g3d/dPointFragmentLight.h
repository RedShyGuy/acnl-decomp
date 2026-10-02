#pragma once

#include "decomp.h"
#include "g3d/dFragmentLight.h"

namespace g3d {
// RTTI N3g3d18PointFragmentLightE @ 0x008D0D30
// vtable 0x00903B10 (vptr 0x00903B18), offset_to_top 0, 7 entries
class PointFragmentLight : public ::g3d::FragmentLight
{
public:
    PointFragmentLight(); // ctor address unknown
    virtual void vf_0x00(); // 0x004F0978 slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004F0954 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x14(); // 0x004F0904 slot 0x14 | virtual slot, introduced by g3d::FragmentLight
};
} // namespace g3d
