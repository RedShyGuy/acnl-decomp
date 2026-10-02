#pragma once

#include "decomp.h"
#include "g3d/dTransformNode.h"

namespace g3d {
// RTTI N3g3d6CameraE @ 0x008D0D78
// vtable 0x00903BDC (vptr 0x00903BE4), offset_to_top 0, 4 entries
class Camera : public ::g3d::TransformNode
{
public:
    Camera(); // ctor candidate(s) 0x004F2820 (unverified)
    virtual void vf_0x00(); // 0x004F2874 slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004F2850 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x08(); // 0x004F27C4 slot 0x08 | virtual slot, introduced by g3d::TransformNode
};
} // namespace g3d
