#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/gfx_ParticleShape.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x00495860 (unverified)
nw::gfx::ParticleShape::ParticleShape()
{
}

// 0x00495A88 slot 0x00 | nintendogs:bytes
nw::gfx::ParticleShape::~ParticleShape()
{
}

// 0x00495A14 slot 0x04 | virtual slot, introduced by nw::gfx::ParticleShape
void nw::gfx::ParticleShape::vf_0x04()
{
}

// 0x00738A30 slot 0x08 | virtual slot, introduced by nw::gfx::ParticleShape
void nw::gfx::ParticleShape::vf_0x08()
{
}

// 0x00494EC8 | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::AddVertexParam(int, unsigned, int, float*, unsigned char**)
{
}

// 0x00494FB8 | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::AddVertexStream(int, unsigned, int, int, unsigned char**)
{
}

// 0x004950C4 | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::AddVertexParamSize(unsigned, int, int)
{
}

// 0x004950E8 | nintendogs:callseq [tier A]
void nw::gfx::ParticleShape::CreateCommandCache(nw::gfx::ParticleSet*)
{
}

// 0x00495768 | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::AddVertexStreamSize(unsigned, int, int, int)
{
}

// 0x004957A4 | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, int)
{
}

// 0x004957EC | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::GetDeviceMemorySizeInternal(nw::os::MemorySizeCalculator*, int)
{
}

// 0x00495860 | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::Create(nw::gfx::res::ResSceneObject, int, nw::os::IAllocator*, nw::os::IAllocator*)
{
}

} // namespace gfx
} // namespace nw
