#pragma once

#include "decomp.h"
#include "g3d/dBaseLight.h"

namespace g3d {
// RTTI N3g3d15HemiSphereLightE @ 0x008D0CF8
// vtable 0x00903A6C (vptr 0x00903A74), offset_to_top 0, 5 entries
class HemiSphereLight : public ::g3d::BaseLight
{
public:
    HemiSphereLight(); // ctor candidate(s) 0x004EF260 (unverified)
    virtual void vf_0x00(); // 0x004EF2A4 slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004EF294 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x08(); // 0x004EF25C slot 0x08 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x10(); // 0x004EF178 slot 0x10 | virtual slot, introduced by g3d::HemiSphereLight
};
} // namespace g3d
