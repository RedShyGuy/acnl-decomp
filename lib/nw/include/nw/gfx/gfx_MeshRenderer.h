#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResPrimitiveSet.h"
#include "nw/gfx/res/gfx_ResShape.h"

namespace nw {
namespace gfx {
class Model;
class RenderContext;
class SkeletalModel;

// RTTI N2nw3gfx12MeshRendererE @ 0x008D0598
// vtable 0x0090240C (vptr 0x00902414), offset_to_top 0, 2 entries
// Draws the meshes of models with a render context (the class name and the functions with sources are
// from the reference symbols, the rest is ours).
class MeshRenderer : public GfxObject
{
public:
    // (inline in Create)
    explicit MeshRenderer(os::IAllocator* pAllocator) : GfxObject(pAllocator), m_pRenderContext(nullptr) {}
    virtual ~MeshRenderer();

    // the material, the vertex attributes and the shape of the mesh, then the attributes off again
    void RenderMesh(res::ResMesh mesh, Model* pModel); // 0x0048E58C | nintendogs:callseq [tier A]
    // the matrices of the bones of the palette as uniforms (from c25)
    void SetMatrixPalette(SkeletalModel* pModel, res::ResPrimitiveSet primitiveSet, int count); // 0x0048E66C | nintendogs:bytes [tier B]
    // the primitive of each primitive set of the shape drawn, with the matrices of the model
    void RenderSeparateDataShape(Model* pModel, res::ResSeparateDataShape shape, int primitiveIndex); // 0x0048E83C | nintendogs:callseq [tier A]
    // a renderer (NULL: no memory)
    static MeshRenderer* Create(os::IAllocator* pAllocator); // 0x0048E9F0 | nintendogs:bytes [tier A]

    // the parts of RenderMesh (out of line for the game; names are ours)
    void ActivateMaterial(res::ResMesh mesh, Model* pModel); // 0x00495E3C (name is ours)
    void ActivateVertexAttribute(res::ResMesh mesh); // 0x00496ADC (name is ours)
    void RenderShape(res::ResMesh mesh, Model* pModel); // 0x0048E7FC (name is ours)
    void DeactivateVertexAttribute(res::ResMesh mesh); // 0x0048E638 (name is ours)

    void SetRenderContext(RenderContext* pRenderContext) { m_pRenderContext = pRenderContext; }

    RenderContext* m_pRenderContext; // 0x8
};
ASSERT_SIZE(MeshRenderer, 0xC);
} // namespace gfx
} // namespace nw
