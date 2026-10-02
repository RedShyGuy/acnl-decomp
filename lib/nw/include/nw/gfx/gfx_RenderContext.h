#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResPrimitive.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13RenderContextE @ 0x008D0610
// vtable 0x00902544 (vptr 0x0090254C), offset_to_top 0, 2 entries
class RenderContext : public ::nw::gfx::GfxObject
{
public:
    class MaterialHash;
    class Builder;
    RenderContext(); // ctor candidate(s) 0x00496E6C (unverified)
    virtual ~RenderContext(); // 0x00497454 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x00497448 slot 0x04 | virtual slot, introduced by nw::gfx::RenderContext
    void ResetState(int, int); // 0x00495AFC | nintendogs:bytes [tier B]
    void ResetState(); // 0x00495BBC | nintendogs:bytes [tier B]
    void ActivateFog(); // 0x00495C10 | nintendogs:bytes [tier A]
    void ActivateContext(nw::gfx::IMaterialActivator*); // 0x00495E6C | nintendogs:callseq [tier A]
    void RenderPrimitive(nw::gfx::res::ResPrimitive); // 0x00495F6C | nintendogs:bytes [tier B]
    void SetCameraMatrix(nw::gfx::Camera*, bool); // 0x0049605C | nintendogs:bytes [tier A]
    void ActivateFragmentLight(int, const nw::gfx::FragmentLight*); // 0x00496140 | nintendogs:callgraph [tier A]
    void SetModelMatrixForModel(nw::gfx::Model*); // 0x00496858 | nintendogs:bytes-fuzzy [tier A]
    void ActivateHemiSphereLight(); // 0x00496944 | nintendogs:bytes [tier B]
    void ActivateVertexAttribute(nw::gfx::res::ResMesh); // 0x00496AE4 | nintendogs:bytes [tier A]
    void ActivateSceneEnvironment(); // 0x00496B8C | nintendogs:bytes [tier A]
    void TransformToViewCoordinate(nn::math::VEC4*, const nn::math::MTX34*, const nn::math::VEC4*); // 0x00496CB4 | nintendogs:bytes [tier A]
    void SetModelMatrixForSkeletalModel(nw::gfx::SkeletalModel*); // 0x00496D30 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace gfx
} // namespace nw
