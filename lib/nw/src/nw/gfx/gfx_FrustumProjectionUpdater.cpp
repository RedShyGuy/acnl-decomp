#include <new>

#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_FrustumProjectionUpdater.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the matrix that projects textures for a frustum projection (name is ours)
inline void MakeFrustumTextureProjection(nn::math::MTX34* pOut, f32 l, f32 r, f32 b, f32 t, f32 n, const nn::math::VEC2& scale,
                                         const nn::math::VEC2& translate)
{
    const f32 width = 1.0f / (r - l);
    const f32 height = 1.0f / (t - b);
    const f32 near2 = n * 2.0f;
    pOut->m[0][0] = near2 * width * scale.x;
    pOut->m[0][1] = 0.0f;
    pOut->m[0][2] = (r + l) * width * scale.x - translate.x;
    pOut->m[0][3] = 0.0f;
    pOut->m[1][0] = 0.0f;
    pOut->m[1][1] = near2 * height * scale.y;
    pOut->m[1][2] = (t + b) * height * scale.y - translate.y;
    pOut->m[1][3] = 0.0f;
    pOut->m[2][0] = 0.0f;
    pOut->m[2][1] = 0.0f;
    pOut->m[2][2] = -1.0f;
    pOut->m[2][3] = 0.0f;
}
} // namespace

// 0x0097F464
const TypeInfo nw::gfx::FrustumProjectionUpdater::s_TypeInfo(&CameraProjectionUpdater::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FD20 (static initializer)

// 0x004A3844 slot 0x10 (name is ours)
nw::gfx::res::ResCameraProjectionUpdater nw::gfx::FrustumProjectionUpdater::GetResource()
{
    return res::ResCameraProjectionUpdater(m_pResource);
}

// 0x004A384C | nintendogs:callseq-callee [tier A]
nw::gfx::FrustumProjectionUpdater* nw::gfx::FrustumProjectionUpdater::Create(os::IAllocator* pAllocator, res::ResFrustumProjectionUpdater resource)
{
    void* pMemory = pAllocator->Alloc(sizeof(FrustumProjectionUpdater), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) FrustumProjectionUpdater(pAllocator, false, resource.ptr());
}

// 0x004A38B4 | nintendogs:bytes
void nw::gfx::FrustumProjectionUpdater::Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection)
{
    const res::ResFrustumProjectionUpdaterData* pResource = m_pResource;
    const f32 halfWidth = pResource->height * pResource->aspectRatio * 0.5f;
    const f32 halfHeight = pResource->height * 0.5f;
    const f32 left = pResource->center.x - halfWidth;
    const f32 right = pResource->center.x + halfWidth;
    const f32 bottom = pResource->center.y - halfHeight;
    const f32 top = pResource->center.y + halfHeight;
    nn::math::ARMv6::MTX44FrustumC_FAST(pProjection, left, right, bottom, top, pResource->near, pResource->far);
    nn::math::ARMv6::MTX44PivotC_FAST(pProjection, m_Pivot);
    MakeFrustumTextureProjection(pTextureProjection, left, right, bottom, top, pResource->near, m_TextureScale, m_TextureTranslate);
}

// 0x004A3AD8 slot 0x00 (name is ours)
// 0x004A3AA4 (deleting dtor)
nw::gfx::FrustumProjectionUpdater::~FrustumProjectionUpdater()
{
    if (m_IsResourceOwner && m_pResource != nullptr) {
        GetAllocator()->Free(m_pResource);
    }
}

} // namespace gfx
} // namespace nw
