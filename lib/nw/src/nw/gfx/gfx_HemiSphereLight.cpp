#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/gfx_HemiSphereLight.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x0049A9E0, 0x0049AEB4 (unverified)
nw::gfx::HemiSphereLight::HemiSphereLight()
{
}

// 0x0049B0CC slot 0x00 | slot vf_0x00 of nw::gfx::SceneNode
nw::gfx::HemiSphereLight::~HemiSphereLight()
{
}

// 0x0049B0C0 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::HemiSphereLight::vf_0x04()
{
}

// 0x00738BC0 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::HemiSphereLight::GetRuntimeTypeInfo() const
{
}

// 0x0049AE60 slot 0x10 | nintendogs:bytes
void nw::gfx::HemiSphereLight::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x0049A98C slot 0x24 | nintendogs:callseq
void nw::gfx::HemiSphereLight::Initialize(nw::os::IAllocator*)
{
}

// 0x00738BB8 slot 0x2C | virtual slot, introduced by nw::gfx::Light
void nw::gfx::HemiSphereLight::vf_0x2C()
{
}

// 0x00738BB0 slot 0x30 | virtual slot, introduced by nw::gfx::Light
void nw::gfx::HemiSphereLight::vf_0x30()
{
}

// 0x0049AAB8 | nintendogs:bytes [tier A]
void nw::gfx::HemiSphereLight::CreateOriginalValue(nw::os::IAllocator*)
{
}

// 0x0049ABA8 | nintendogs:bytes [tier A]
void nw::gfx::HemiSphereLight::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResHemiSphereLight, nw::gfx::HemiSphereLight::Description)
{
}

// 0x0049ACA0 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::HemiSphereLight::CreateResHemiSphereLight(nw::os::IAllocator*, const char*)
{
}

} // namespace gfx
} // namespace nw
