#pragma once

#include "decomp.h"
#include "g3d/dTransformNode.h"
#include "nw/gfx/res/gfx_ResFog.h"

namespace nw {
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

namespace g3d {
// a fog of the scene with a resource of its own (names are ours)
// RTTI N3g3d3FogE @ 0x008D0D54
// vtable 0x00903B94 (vptr 0x00903B9C), offset_to_top 0, 4 entries
class Fog : public ::g3d::TransformNode
{
public:
    Fog(); // 0x004F12D8
    virtual ~Fog(); // 0x004F1310 slot 0x00
    // 0x004F1300 (deleting dtor)
    void Update() override; // 0x004F12D4 slot 0x08

    // a linear white fog from 100 to 1000 (density 10) in the memory of allocator (NULL: the one of the engine); always
    // true
    bool Initialize(nw::os::IAllocator* allocator); // 0x004F1218
    DECOMP_EMIT_COPY nw::gfx::res::ResFog& GetResFog() { return m_ResFog; } // 0x004F12CC

    nw::gfx::res::ResFog m_ResFog; // 0x8 the resource of the fog
};
ASSERT_SIZE(Fog, 0xC);
} // namespace g3d
