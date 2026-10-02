#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResFragmentLightingTable.h"
#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/gfx_Material.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004B04EC (unverified)
nw::gfx::Material::Material()
{
}

// 0x004B0658 slot 0x00 | nintendogs:bytes
nw::gfx::Material::~Material()
{
}

// 0x004B05A8 slot 0x04 | virtual slot, introduced by nw::gfx::Material
void nw::gfx::Material::vf_0x04()
{
}

// 0x0073C918 slot 0x08 | virtual slot, introduced by nw::gfx::Material
void nw::gfx::Material::vf_0x08()
{
}

// 0x004AEECC | nintendogs:callgraph [tier A]
void nw::gfx::Material::CreateBuffers(nw::os::IAllocator*)
{
}

// 0x004AF064 | nintendogs:callseq [tier A]
void nw::gfx::Material::CopyResMaterial(nw::os::IAllocator*, unsigned)
{
}

// 0x004AF5C4 | nintendogs:bytes [tier A]
void nw::gfx::Material::DestroyResMaterial(nw::os::IAllocator*, nw::gfx::res::ResMaterial)
{
}

// 0x004AF818 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::Material::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResMaterial, int, unsigned)
{
}

// 0x004AFC78 | nintendogs:callgraph [tier A]
void nw::gfx::Material::DestroyResFragmentShader(nw::os::IAllocator*, nw::gfx::res::ResFragmentShader)
{
}

// 0x004AFF70 | nintendogs:bytes [tier A]
void nw::gfx::Material::CopyResLightingLookupTable(nw::os::IAllocator*, nw::gfx::res::ResLightingLookupTable)
{
}

// 0x004B00B4 | nintendogs:bytes [tier A]
void nw::gfx::Material::CopyResFragmentLightingTable(nw::os::IAllocator*, nw::gfx::res::ResFragmentLightingTable)
{
}

// 0x004B0388 | nintendogs:bytes [tier A]
void nw::gfx::Material::Create(nw::gfx::res::ResMaterial, int, nw::gfx::Model*, nw::os::IAllocator*)
{
}

// 0x0073C888 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::Material::CanUseBuffer(unsigned) const
{
}

} // namespace gfx
} // namespace nw
