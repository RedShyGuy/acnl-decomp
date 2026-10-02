#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformNode.h"

namespace g3d {
// RTTI N3g3d8FuncNodeE @ 0x008D0D98
// vtable 0x00903C2C (vptr 0x00903C34), offset_to_top 0, 11 entries
class FuncNode : public ::nw::gfx::TransformNode
{
public:
    FuncNode(); // ctor candidate(s) 0x004F1370 (unverified)
    virtual ~FuncNode(); // 0x004F2DF4 slot 0x00 | slot vf_0x00 of nw::gfx::SceneNode
    virtual void vf_0x04(); // 0x004F2DE8 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00747500 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
};
} // namespace g3d
