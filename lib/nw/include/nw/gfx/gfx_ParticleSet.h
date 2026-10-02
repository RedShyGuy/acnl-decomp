#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneNode.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx11ParticleSetE @ 0x008D0574
// vtable 0x009023A0 (vptr 0x009023A8), offset_to_top 0, 10 entries
class ParticleSet : public ::nw::gfx::SceneNode
{
public:
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    ParticleSet(); // ctor candidate(s) 0x0048D57C (unverified)
    virtual ~ParticleSet(); // 0x0048D65C slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x0048D658 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00737874 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x0048C914 slot 0x10 | nintendogs:bytes
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleSet, const nw::gfx::ParticleSet::Description&); // 0x0048C5E0 | nintendogs:bytes-fuzzy [tier A]
    void Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::ParticleSet::Description&, nw::os::IAllocator*, nw::os::IAllocator*, nw::gfx::ParticleShape*); // 0x0048C968 | nintendogs:callseq [tier A]
    ParticleSet(nw::os::IAllocator*, nw::gfx::res::ResParticleSet, const nw::gfx::ParticleSet::Description&); // 0x0048D57C | nintendogs:callseq-callee [tier A]
    void GetDeviceMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleSet); // 0x0049EC90 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
