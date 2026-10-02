#include "nw/gfx/gfx_SceneNode.h"
#include "nw/gfx/gfx_ParticleSet.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x0048D57C (unverified)
nw::gfx::ParticleSet::ParticleSet()
{
}

// 0x0048D65C slot 0x00 | nintendogs:bytes
nw::gfx::ParticleSet::~ParticleSet()
{
}

// 0x0048D658 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::ParticleSet::vf_0x04()
{
}

// 0x00737874 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::ParticleSet::GetRuntimeTypeInfo() const
{
}

// 0x0048C914 slot 0x10 | nintendogs:bytes
void nw::gfx::ParticleSet::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x0048C5E0 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::ParticleSet::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleSet, const nw::gfx::ParticleSet::Description&)
{
}

// 0x0048C968 | nintendogs:callseq [tier A]
void nw::gfx::ParticleSet::Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::ParticleSet::Description&, nw::os::IAllocator*, nw::os::IAllocator*, nw::gfx::ParticleShape*)
{
}

// 0x0048D57C | nintendogs:callseq-callee [tier A]
nw::gfx::ParticleSet::ParticleSet(nw::os::IAllocator*, nw::gfx::res::ResParticleSet, const nw::gfx::ParticleSet::Description&)
{
}

// 0x0049EC90 | nintendogs:bytes [tier A]
void nw::gfx::ParticleSet::GetDeviceMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleSet)
{
}

} // namespace gfx
} // namespace nw
