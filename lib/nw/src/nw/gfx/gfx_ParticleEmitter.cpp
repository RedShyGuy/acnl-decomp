#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/gfx_ParticleEmitter.h"

namespace nw {
namespace gfx {
// ctor address unknown
nw::gfx::ParticleEmitter::ParticleEmitter()
{
}

// 0x0049BBFC slot 0x00 | nintendogs:bytes
nw::gfx::ParticleEmitter::~ParticleEmitter()
{
}

// 0x0049BBF8 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::ParticleEmitter::vf_0x04()
{
}

// 0x00738BCC slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::ParticleEmitter::GetRuntimeTypeInfo() const
{
}

// 0x0049B6FC slot 0x10 | nintendogs:bytes
void nw::gfx::ParticleEmitter::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x0049B750 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::ParticleEmitter::Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::ParticleEmitter::Description&, nw::os::IAllocator*)
{
}

} // namespace gfx
} // namespace nw
