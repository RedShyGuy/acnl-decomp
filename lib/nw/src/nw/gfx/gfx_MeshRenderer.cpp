#include "nw/gfx/res/gfx_ResPrimitiveSet.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_MeshRenderer.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x0048E9F0 (unverified)
nw::gfx::MeshRenderer::MeshRenderer()
{
}

// 0x0048EA34 slot 0x00 | virtual slot, introduced by nw::gfx::MeshRenderer
void nw::gfx::MeshRenderer::vf_0x00()
{
}

// 0x0048EA30 slot 0x04 | virtual slot, introduced by nw::gfx::MeshRenderer
void nw::gfx::MeshRenderer::vf_0x04()
{
}

// 0x0048E58C | nintendogs:callseq [tier A]
void nw::gfx::MeshRenderer::RenderMesh(nw::gfx::res::ResMesh, nw::gfx::Model*)
{
}

// 0x0048E66C | nintendogs:bytes [tier B]
void nw::gfx::MeshRenderer::SetMatrixPalette(nw::gfx::SkeletalModel*, nw::gfx::res::ResPrimitiveSet, int)
{
}

// 0x0048E83C | nintendogs:callseq [tier A]
void nw::gfx::MeshRenderer::RenderSeparateDataShape(nw::gfx::Model*, nw::gfx::res::ResSeparateDataShape, int)
{
}

// 0x0048E9F0 | nintendogs:bytes [tier A]
void nw::gfx::MeshRenderer::Create(nw::os::IAllocator*)
{
}

} // namespace gfx
} // namespace nw
