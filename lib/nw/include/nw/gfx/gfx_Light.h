#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformNode.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx5LightE @ 0x008D07D8
// vtable 0x009029E0 (vptr 0x009029E8), offset_to_top 0, 13 entries
class Light : public ::nw::gfx::TransformNode
{
public:
    Light(); // ctor address unknown
    virtual ~Light(); // 0x004AAA38 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004AAA2C slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x0073C820 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x004AA9D8 slot 0x10 | nintendogs:bytes
    virtual void vf_0x2C(); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x30(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    void CreateAnimGroup(nw::os::IAllocator*); // 0x004AA760 | nintendogs:bytes-fuzzy [tier A]
    void DestroyOriginalValue(); // 0x004AA9B0 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
