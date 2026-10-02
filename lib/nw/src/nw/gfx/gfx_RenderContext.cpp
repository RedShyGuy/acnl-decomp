#include "nw/gfx/res/gfx_ResPrimitive.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_RenderContext.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x00496E6C (unverified)
nw::gfx::RenderContext::RenderContext()
{
}

// 0x00497454 slot 0x00 | nintendogs:bytes
nw::gfx::RenderContext::~RenderContext()
{
}

// 0x00497448 slot 0x04 | virtual slot, introduced by nw::gfx::RenderContext
void nw::gfx::RenderContext::vf_0x04()
{
}

// 0x00495AFC | nintendogs:bytes [tier B]
void nw::gfx::RenderContext::ResetState(int, int)
{
}

// 0x00495BBC | nintendogs:bytes [tier B]
void nw::gfx::RenderContext::ResetState()
{
}

// 0x00495C10 | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::ActivateFog()
{
}

// 0x00495E6C | nintendogs:callseq [tier A]
void nw::gfx::RenderContext::ActivateContext(nw::gfx::IMaterialActivator*)
{
}

// 0x00495F6C | nintendogs:bytes [tier B]
void nw::gfx::RenderContext::RenderPrimitive(nw::gfx::res::ResPrimitive)
{
}

// 0x0049605C | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::SetCameraMatrix(nw::gfx::Camera*, bool)
{
}

// 0x00496140 | nintendogs:callgraph [tier A]
void nw::gfx::RenderContext::ActivateFragmentLight(int, const nw::gfx::FragmentLight*)
{
}

// 0x00496858 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::RenderContext::SetModelMatrixForModel(nw::gfx::Model*)
{
}

// 0x00496944 | nintendogs:bytes [tier B]
void nw::gfx::RenderContext::ActivateHemiSphereLight()
{
}

// 0x00496AE4 | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::ActivateVertexAttribute(nw::gfx::res::ResMesh)
{
}

// 0x00496B8C | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::ActivateSceneEnvironment()
{
}

// 0x00496CB4 | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::TransformToViewCoordinate(nn::math::VEC4*, const nn::math::MTX34*, const nn::math::VEC4*)
{
}

// 0x00496D30 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::RenderContext::SetModelMatrixForSkeletalModel(nw::gfx::SkeletalModel*)
{
}

} // namespace gfx
} // namespace nw
