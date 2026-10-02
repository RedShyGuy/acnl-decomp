#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResPrimitiveSet.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx12MeshRendererE @ 0x008D0598
// vtable 0x0090240C (vptr 0x00902414), offset_to_top 0, 2 entries
class MeshRenderer : public ::nw::gfx::GfxObject
{
public:
    MeshRenderer(); // ctor candidate(s) 0x0048E9F0 (unverified)
    virtual void vf_0x00(); // 0x0048EA34 slot 0x00 | virtual slot, introduced by nw::gfx::MeshRenderer
    virtual void vf_0x04(); // 0x0048EA30 slot 0x04 | virtual slot, introduced by nw::gfx::MeshRenderer
    void RenderMesh(nw::gfx::res::ResMesh, nw::gfx::Model*); // 0x0048E58C | nintendogs:callseq [tier A]
    void SetMatrixPalette(nw::gfx::SkeletalModel*, nw::gfx::res::ResPrimitiveSet, int); // 0x0048E66C | nintendogs:bytes [tier B]
    void RenderSeparateDataShape(nw::gfx::Model*, nw::gfx::res::ResSeparateDataShape, int); // 0x0048E83C | nintendogs:callseq [tier A]
    void Create(nw::os::IAllocator*); // 0x0048E9F0 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
