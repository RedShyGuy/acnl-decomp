#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/gfx_AmbientLight.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x0048DB5C, 0x0048E028 (unverified)
nw::gfx::AmbientLight::AmbientLight()
{
}

// 0x0048E240 slot 0x00 | slot vf_0x00 of nw::gfx::SceneNode
nw::gfx::AmbientLight::~AmbientLight()
{
}

// 0x0048E234 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::AmbientLight::vf_0x04()
{
}

// 0x00737890 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::AmbientLight::GetRuntimeTypeInfo() const
{
}

// 0x0048DFD4 slot 0x10 | nintendogs:bytes
void nw::gfx::AmbientLight::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x0048DB08 slot 0x24 | nintendogs:callseq
void nw::gfx::AmbientLight::Initialize(nw::os::IAllocator*)
{
}

// 0x00737888 slot 0x2C | virtual slot, introduced by nw::gfx::Light
void nw::gfx::AmbientLight::vf_0x2C()
{
}

// 0x00737880 slot 0x30 | virtual slot, introduced by nw::gfx::Light
void nw::gfx::AmbientLight::vf_0x30()
{
}

// 0x0048DC34 | nintendogs:bytes [tier A]
void nw::gfx::AmbientLight::CreateOriginalValue(nw::os::IAllocator*)
{
}

// 0x0048DD1C | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::AmbientLight::CreateResAmbientLight(nw::os::IAllocator*, const char*)
{
}

// 0x0048DEDC | nintendogs:bytes [tier A]
void nw::gfx::AmbientLight::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResAmbientLight, nw::gfx::AmbientLight::Description)
{
}

} // namespace gfx
} // namespace nw
