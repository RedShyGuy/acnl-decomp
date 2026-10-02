#pragma once

#include "decomp.h"
#include "g3d/dBaseLight.h"

namespace g3d {
// RTTI N3g3d12AmbientLightE @ 0x008D0C94
// vtable 0x00903974 (vptr 0x0090397C), offset_to_top 0, 5 entries
class AmbientLight : public ::g3d::BaseLight
{
public:
    AmbientLight(); // ctor candidate(s) 0x004ECF44 (unverified)
    virtual void vf_0x00(); // 0x004EEB98 slot 0x00 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x04(); // 0x004ECF78 slot 0x04 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x08(); // 0x004EEB3C slot 0x08 | virtual slot, introduced by g3d::TransformNode
    virtual void vf_0x10(); // 0x004ECEA8 slot 0x10 | virtual slot, introduced by g3d::AmbientLight
};
} // namespace g3d
