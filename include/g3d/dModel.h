#pragma once

#include "decomp.h"
#include "g3d/dTransformNode.h"

namespace g3d {
// RTTI N3g3d5ModelE @ 0x008D0D6C
// vtable 0x00903BC4 (vptr 0x00903BCC), offset_to_top 0, 4 entries
class Model : public ::g3d::TransformNode
{
public:
    Model(); // ctor candidate(s) 0x004F1B04 (unverified)
    virtual void vf_0x00(); // 0x004F1B34 slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004F1B24 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x0C(); // 0x004F18F8 slot 0x0C | virtual slot, introduced by g3d::TransformNode
};
} // namespace g3d
