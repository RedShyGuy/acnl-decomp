#include "nw/gfx/res/gfx_ResFog.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/gfx_Fog.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004A4BE4, 0x004A52EC (unverified)
nw::gfx::Fog::Fog()
{
}

// 0x004A5650 slot 0x00 | nintendogs:bytes-fuzzy
nw::gfx::Fog::~Fog()
{
}

// 0x004A5644 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::Fog::vf_0x04()
{
}

// 0x0073C6AC slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::Fog::GetRuntimeTypeInfo() const
{
}

// 0x004A5298 slot 0x10 | nintendogs:bytes
void nw::gfx::Fog::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x004A4788 slot 0x24 | nintendogs:bytes
void nw::gfx::Fog::Initialize(nw::os::IAllocator*)
{
}

// 0x004A4880 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::Fog::CreateResFog(nw::os::IAllocator*, const char*)
{
}

// 0x004A4B1C | nintendogs:bytes [tier A]
void nw::gfx::Fog::DestroyResFog(nw::os::IAllocator*, nw::gfx::res::ResFogData*)
{
}

// 0x004A4CB0 | nintendogs:bytes [tier A]
void nw::gfx::Fog::CreateAnimGroup(nw::os::IAllocator*)
{
}

// 0x004A4E4C | nintendogs:callgraph [tier A]
void nw::gfx::Fog::SetupFogSampler(nw::gfx::res::ResImageLookupTable, nw::gfx::res::ResFogUpdater, const nn::math::MTX44&)
{
}

// 0x004A50E4 | nintendogs:bytes [tier A]
void nw::gfx::Fog::CreateOriginalValue(nw::os::IAllocator*)
{
}

// 0x004A5188 | nintendogs:bytes [tier A]
void nw::gfx::Fog::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResFog, nw::gfx::Fog::Description)
{
}

// 0x004A550C | nintendogs:bytes [tier A]
void nw::gfx::Fog::Update(const nw::gfx::Camera*)
{
}

} // namespace gfx
} // namespace nw
