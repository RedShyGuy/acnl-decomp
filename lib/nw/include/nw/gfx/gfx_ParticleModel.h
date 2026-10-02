#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Model.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13ParticleModelE @ 0x008D05F8
// vtable 0x009024FC (vptr 0x00902504), offset_to_top 0, 11 entries
class ParticleModel : public ::nw::gfx::Model
{
public:
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    ParticleModel(); // ctor candidate(s) 0x0049419C (unverified)
    virtual ~ParticleModel(); // 0x004947E4 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004947D8 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00738A24 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x00494148 slot 0x10 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x0049368C slot 0x24 | slot vf_0x24 of nw::gfx::SceneNode
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleModel, const nw::gfx::ParticleModel::Description&); // 0x00493AE0 | nintendogs:bytes [tier A]
    void GetMemorySizeForInitialize(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleModel, const nw::gfx::ParticleModel::Description&); // 0x00493BD0 | nintendogs:callgraph [tier A]
    void GetDeviceMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleModel, const nw::gfx::ParticleModel::Description&); // 0x004940AC | nintendogs:bytes [tier A]
    void Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::ParticleModel::Description&, nw::os::IAllocator*, nw::os::IAllocator*); // 0x0049419C | nintendogs:callseq [tier A]
};
} // namespace gfx
} // namespace nw
