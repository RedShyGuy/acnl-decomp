#include "nw/gfx/gfx_MeshRenderer.h"

#include <new>

#include "nn/gx/detail/gx_Context.h"
#include "nn/nstd/nstd_String.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_RenderContext.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"

namespace nw {
namespace gfx {
namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_VSH_FLOATUNIFORM_INDEX = 0x2C0;
const u32 REG_VSH_FLOATUNIFORM_DATA = 0x2C1;

// a float uniform index in 32-bit mode
const u32 FLOAT_UNIFORM_F32 = 0x80000000;
// the float uniforms of the gfx shaders (names are ours)
const u32 UNIFORM_POSITION_OFFSET = 6;  // c6
const u32 UNIFORM_MATRIX_PALETTE = 25;  // c25 on, three per bone (or the world matrix)
} // namespace

// 0x0048E58C | nintendogs:callseq [tier A]
void nw::gfx::MeshRenderer::RenderMesh(res::ResMesh mesh, Model* pModel)
{
    ActivateMaterial(mesh, pModel);
    ActivateVertexAttribute(mesh);
    RenderShape(mesh, pModel);
    DeactivateVertexAttribute(mesh);
}

// 0x0048E638 (name is ours)
void nw::gfx::MeshRenderer::DeactivateVertexAttribute(res::ResMesh mesh)
{
    const u32 size = mesh.ptr()->deactivateCommandSize;
    nnnstdMemCpy(nn::gx::detail::s_pCommandBufferCurrent, mesh.ptr()->pDeactivateCommand, size);
    nn::gx::detail::s_pCommandBufferCurrent =
        reinterpret_cast<u32*>(reinterpret_cast<u8*>(nn::gx::detail::s_pCommandBufferCurrent) + (size & ~3));
}

// 0x0048E66C | nintendogs:bytes [tier B]
void nw::gfx::MeshRenderer::SetMatrixPalette(SkeletalModel* pModel, res::ResPrimitiveSet primitiveSet, int count)
{
    // the bool uniforms of the skinning
    ShaderProgram* pProgram = m_pRenderContext->GetShaderProgram();
    const bool isRigid = primitiveSet.ptr()->skinningMode != res::ResPrimitiveSetData::SKINNING_MODE_SMOOTH;
    u32 bools = pProgram->m_VertexBoolUniformCommand[0];
    if (!isRigid) {
        bools |= ShaderProgram::BOOL_SMOOTH_SKINNING;
    } else {
        bools &= ~ShaderProgram::BOOL_SMOOTH_SKINNING;
    }
    pProgram->m_VertexBoolUniformCommand[0] = bools;
    if (isRigid) {
        bools |= ShaderProgram::BOOL_RIGID_SKINNING;
    } else {
        bools &= ~ShaderProgram::BOOL_RIGID_SKINNING;
    }
    pProgram->m_VertexBoolUniformCommand[0] = bools;

    // the matrix of each bone: its skinning matrix for smooth skinning, else its world matrix
    Skeleton* pSkeleton = pModel->GetSkeleton();
    const res::ResSkeletonData* pResSkeleton = reinterpret_cast<const res::ResSkeletonData*>(pSkeleton->GetResSceneObject().ptr());
    const Skeleton::MatrixArray* pWorldMatrices = pSkeleton->GetWorldMatrices();
    const Skeleton::MatrixArray* pSkinningMatrices = pSkeleton->GetSkinningMatrices();
    for (int i = 0; i < count; ++i) {
        const int boneIndex = primitiveSet.ptr()->boneIndices.To<s32>()[i];
        ut::ResDicPatricia bones = {pResSkeleton->boneDic.To<ut::ResDicPatricia::Data>()};
        const res::ResBoneData* pBone = static_cast<const res::ResBoneData*>(bones.GetDataByIndex(boneIndex));
        const bool hasSkinningMatrix = (pBone->flags & res::ResBoneData::FLAG_HAS_SKINNING_MATRIX) != 0;
        const Skeleton::MatrixArray* pMatrices = (hasSkinningMatrix && !isRigid) ? pSkinningMatrices : pWorldMatrices;
        const f32* pMatrix = reinterpret_cast<const f32*>(&(*pMatrices)[boneIndex]);
        if (i == 0) {
            u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
            pCommand[0] = FLOAT_UNIFORM_F32 | (UNIFORM_MATRIX_PALETTE + i * 3);
            pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX);
            internal::NWCopyMtx34WithHeader(reinterpret_cast<f32*>(pCommand + 2), pMatrix,
                                            Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, count * 12 - 1));
            nn::gx::detail::s_pCommandBufferCurrent += 15;
        } else {
            internal::NWCopyMtx34Reverse(reinterpret_cast<f32*>(nn::gx::detail::s_pCommandBufferCurrent), pMatrix);
            nn::gx::detail::s_pCommandBufferCurrent += 12;
        }
    }
    // the padding of the command
    nn::gx::detail::s_pCommandBufferCurrent += 1;
}

// 0x0048E7FC (name is ours)
void nw::gfx::MeshRenderer::RenderShape(res::ResMesh mesh, Model* pModel)
{
    RenderSeparateDataShape(pModel, res::ResSeparateDataShape(pModel->GetResModel().GetShape(mesh.ptr()->shapeIndex)),
                            mesh.ptr()->primitiveIndex);
}

// 0x0048E83C | nintendogs:callseq [tier A]
void nw::gfx::MeshRenderer::RenderSeparateDataShape(Model* pModel, res::ResSeparateDataShape shape, int primitiveIndex)
{
    if (shape.ptr() == nullptr) {
        return;
    }
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = FLOAT_UNIFORM_F32 | UNIFORM_POSITION_OFFSET;
    pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX, 0xF, 4, true);
    internal::NWCopyVec3Reverse(reinterpret_cast<f32*>(pCommand + 2), &shape.ptr()->positionOffset.x);
    nn::gx::detail::s_pCommandBufferCurrent += 6;

    SkeletalModel* pSkeletalModel = DynamicCast<SkeletalModel>(pModel);
    if (m_pRenderContext->GetModel() != pModel) {
        if (pSkeletalModel != nullptr) {
            m_pRenderContext->SetModelMatrixForSkeletalModel(pSkeletalModel);
        } else {
            m_pRenderContext->SetModelMatrixForModel(pModel);
        }
    }

    const ut::Offset* pBegin = shape.ptr()->primitiveSets.array.To<ut::Offset>();
    const ut::Offset* pEnd = pBegin + shape.ptr()->primitiveSets.count;
    for (const ut::Offset* pIt = pBegin; pIt != pEnd; ++pIt) {
        const res::ResPrimitiveSetData* pPrimitiveSet = pIt->To<res::ResPrimitiveSetData>();
        if (pSkeletalModel != nullptr && pPrimitiveSet->boneIndexCount != 0) {
            SetMatrixPalette(pSkeletalModel, res::ResPrimitiveSet(pPrimitiveSet), pPrimitiveSet->boneIndexCount);
        } else {
            // no skinning: the world matrix of the model
            m_pRenderContext->GetShaderProgram()->m_VertexBoolUniformCommand[0] &=
                ~(ShaderProgram::BOOL_SMOOTH_SKINNING | ShaderProgram::BOOL_RIGID_SKINNING);
            u32* pMatrixCommand = nn::gx::detail::s_pCommandBufferCurrent;
            pMatrixCommand[0] = FLOAT_UNIFORM_F32 | UNIFORM_MATRIX_PALETTE;
            pMatrixCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX);
            internal::NWCopyMtx34WithHeader(reinterpret_cast<f32*>(pMatrixCommand + 2), reinterpret_cast<const f32*>(&pModel->m_WorldMatrix),
                                            Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 11));
            nn::gx::detail::s_pCommandBufferCurrent += 16;
        }
        m_pRenderContext->RenderPrimitive(res::ResPrimitive(pPrimitiveSet->primitives.GetData(primitiveIndex)));
    }
}

// 0x0048E9F0 | nintendogs:bytes [tier A]
nw::gfx::MeshRenderer* nw::gfx::MeshRenderer::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(MeshRenderer), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) MeshRenderer(pAllocator);
}

// 0x0048EA34 slot 0x00
// 0x0048EA30 (deleting dtor)
nw::gfx::MeshRenderer::~MeshRenderer()
{
}

// 0x00495E3C (name is ours)
void nw::gfx::MeshRenderer::ActivateMaterial(res::ResMesh mesh, Model* pModel)
{
    m_pRenderContext->SetMaterial(pModel->GetMaterial(mesh.ptr()->materialIndex));
    m_pRenderContext->ActivateContext(nullptr);
}

// 0x00496ADC (name is ours)
void nw::gfx::MeshRenderer::ActivateVertexAttribute(res::ResMesh mesh)
{
    m_pRenderContext->ActivateVertexAttribute(mesh);
}

} // namespace gfx
} // namespace nw
