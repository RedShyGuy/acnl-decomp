#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformNode.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx15ParticleEmitterE @ 0x008D0658
// vtable 0x00902654 (vptr 0x0090265C), offset_to_top 0, 11 entries
class ParticleEmitter : public ::nw::gfx::TransformNode
{
public:
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    ParticleEmitter(); // ctor address unknown
    virtual ~ParticleEmitter(); // 0x0049BBFC slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x0049BBF8 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00738BCC slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x0049B6FC slot 0x10 | nintendogs:bytes
    void Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::ParticleEmitter::Description&, nw::os::IAllocator*); // 0x0049B750 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace gfx
} // namespace nw
