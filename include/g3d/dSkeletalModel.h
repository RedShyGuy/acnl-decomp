#pragma once

#include "decomp.h"
#include "g3d/dModel.h"

namespace g3d {
// RTTI N3g3d13SkeletalModelE @ 0x008D0CC4
// vtable 0x00903A04 (vptr 0x00903A0C), offset_to_top 0, 4 entries
class SkeletalModel : public ::g3d::Model
{
public:
    SkeletalModel(); // ctor candidate(s) 0x004EE3C4 (unverified)
    virtual void vf_0x00(); // 0x004EE40C slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004EE3E4 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x08(); // 0x004EE3A8 slot 0x08 | virtual slot, introduced by g3d::TransformNode
};
} // namespace g3d
