#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Model.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13SkeletalModelE @ 0x008D0628
// vtable 0x00902564 (vptr 0x0090256C), offset_to_top 0, 11 entries
class SkeletalModel : public ::nw::gfx::Model
{
public:
    class Builder;
    SkeletalModel(); // ctor candidate(s) 0x00497C5C (unverified)
    virtual ~SkeletalModel(); // 0x00498360 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x00498354 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00738B74 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x00497C08 slot 0x10 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x00497808 slot 0x24 | nintendogs:bytes
    void CreateSkeletalAnimGroup(nw::os::IAllocator*); // 0x0049785C | nintendogs:bytes-fuzzy [tier A]
    void SetupAnimGroup(nw::gfx::AnimGroup*, bool) const; // 0x00738A3C | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
