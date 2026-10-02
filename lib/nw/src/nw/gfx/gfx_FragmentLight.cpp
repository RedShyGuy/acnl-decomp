#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/gfx_FragmentLight.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x00492AD8, 0x004932A0 (unverified)
nw::gfx::FragmentLight::FragmentLight()
{
}

// 0x00493374 slot 0x00 | nintendogs:bytes
nw::gfx::FragmentLight::~FragmentLight()
{
}

// 0x00493368 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::FragmentLight::vf_0x04()
{
}

// 0x00738A18 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::FragmentLight::GetRuntimeTypeInfo() const
{
}

// 0x0049324C slot 0x10 | nintendogs:bytes
void nw::gfx::FragmentLight::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x00492A84 slot 0x24 | nintendogs:callseq
void nw::gfx::FragmentLight::Initialize(nw::os::IAllocator*)
{
}

// 0x00492BA0 slot 0x28 | nintendogs:bytes
void nw::gfx::FragmentLight::UpdateDirection()
{
}

// 0x00738A10 slot 0x2C | virtual slot, introduced by nw::gfx::Light
void nw::gfx::FragmentLight::vf_0x2C()
{
}

// 0x00738A04 slot 0x30 | virtual slot, introduced by nw::gfx::Light
void nw::gfx::FragmentLight::vf_0x30()
{
}

// 0x00492C78 | nintendogs:bytes [tier A]
void nw::gfx::FragmentLight::CreateOriginalValue(nw::os::IAllocator*)
{
}

// 0x00492E04 | nintendogs:bytes [tier A]
void nw::gfx::FragmentLight::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResFragmentLight, nw::gfx::FragmentLight::Description)
{
}

// 0x00492EFC | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::FragmentLight::CreateResFragmentLight(nw::os::IAllocator*, const char*)
{
}

// 0x0049319C | nintendogs:bytes [tier A]
void nw::gfx::FragmentLight::DestroyResFragmentLight(nw::os::IAllocator*, nw::gfx::res::ResFragmentLightData*)
{
}

// 0x004932A0 | nintendogs:bytes [tier A]
void nw::gfx::FragmentLight::Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::FragmentLight::Description&, nw::os::IAllocator*)
{
}

} // namespace gfx
} // namespace nw
