#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_ParticleModel.h"

namespace nw {
namespace gfx {
// 0x0097F488
const TypeInfo nw::gfx::ParticleModel::s_TypeInfo(&Model::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C1E8 (static initializer)

// ctor candidate(s) 0x0049419C (unverified)
nw::gfx::ParticleModel::ParticleModel()
{
}

// 0x004947E4 slot 0x00 | nintendogs:bytes
nw::gfx::ParticleModel::~ParticleModel()
{
}

// 0x004947D8 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::ParticleModel::vf_0x04()
{
}

// 0x00494148 slot 0x10 | nintendogs:bytes
void nw::gfx::ParticleModel::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x0049368C slot 0x24 | slot vf_0x24 of nw::gfx::SceneNode
u32 nw::gfx::ParticleModel::Initialize(nw::os::IAllocator*)
{
}

// 0x00493AE0 | nintendogs:bytes [tier A]
void nw::gfx::ParticleModel::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleModel, const nw::gfx::ParticleModel::Description&)
{
}

// 0x00493BD0 | nintendogs:callgraph [tier A]
void nw::gfx::ParticleModel::GetMemorySizeForInitialize(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleModel, const nw::gfx::ParticleModel::Description&)
{
}

// 0x004940AC | nintendogs:bytes [tier A]
void nw::gfx::ParticleModel::GetDeviceMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResParticleModel, const nw::gfx::ParticleModel::Description&)
{
}

// 0x0049419C | nintendogs:callseq [tier A]
void nw::gfx::ParticleModel::Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::ParticleModel::Description&, nw::os::IAllocator*, nw::os::IAllocator*)
{
}

} // namespace gfx
} // namespace nw
