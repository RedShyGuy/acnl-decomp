#pragma once

#include "decomp.h"
#include "g3d/dBaseLight.h"

namespace g3d {
// RTTI N3g3d13FragmentLightE @ 0x008D0CB8
// vtable 0x009039E0 (vptr 0x009039E8), offset_to_top 0, 7 entries
class FragmentLight : public ::g3d::BaseLight
{
public:
    FragmentLight(); // ctor candidate(s) 0x004EDA84 (unverified)
    virtual void vf_0x00(); // 0x004EDADC slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004EDAB8 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x08(); // 0x004EDA80 slot 0x08 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x0C(); // 0x004EDA64 slot 0x0C | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x10(); // 0x004ED620 slot 0x10 | virtual slot, introduced by g3d::FragmentLight
    virtual void vf_0x14(); // 0x004ED8F4 slot 0x14 | virtual slot, introduced by g3d::FragmentLight
    virtual void vf_0x18(); // 0x004ED834 slot 0x18 | virtual slot, introduced by g3d::FragmentLight
};
} // namespace g3d
