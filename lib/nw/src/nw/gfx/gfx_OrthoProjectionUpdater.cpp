#include <cstring>
#include <new>

#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_OrthoProjectionUpdater.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the matrix that projects textures for an orthographic projection (name is ours)
inline void MakeOrthoTextureProjection(nn::math::MTX34* pOut, f32 l, f32 r, f32 b, f32 t, const nn::math::VEC2& scale,
                                       const nn::math::VEC2& translate)
{
    const f32 width = 1.0f / (r - l);
    const f32 height = 1.0f / (t - b);
    pOut->m[0][0] = 2.0f * width * scale.x;
    pOut->m[0][1] = 0.0f;
    pOut->m[0][2] = 0.0f;
    pOut->m[0][3] = -(r + l) * width * scale.x + translate.x;
    pOut->m[1][0] = 0.0f;
    pOut->m[1][1] = 2.0f * height * scale.y;
    pOut->m[1][2] = 0.0f;
    pOut->m[1][3] = -(t + b) * height * scale.y + translate.y;
    pOut->m[2][0] = 0.0f;
    pOut->m[2][1] = 0.0f;
    pOut->m[2][2] = 0.0f;
    pOut->m[2][3] = 1.0f;
}
} // namespace

// 0x009760E0
const TypeInfo nw::gfx::OrthoProjectionUpdater::s_TypeInfo(&CameraProjectionUpdater::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FB10 (static initializer)

// 0x004A0E00 slot 0x10 (name is ours)
nw::gfx::res::ResCameraProjectionUpdater nw::gfx::OrthoProjectionUpdater::GetResource()
{
    return res::ResCameraProjectionUpdater(m_pResource);
}

// 0x004A0E08 | nintendogs:bytes [tier A]
nw::gfx::OrthoProjectionUpdater* nw::gfx::OrthoProjectionUpdater::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(OrthoProjectionUpdater), 4);

    void* pResourceMemory = pAllocator->Alloc(sizeof(res::ResOrthoProjectionUpdaterData), 4);
    std::memset(pResourceMemory, 0, sizeof(res::ResOrthoProjectionUpdaterData));
    res::ResOrthoProjectionUpdaterData* pResource = new (pResourceMemory) res::ResOrthoProjectionUpdaterData();
    pResource->typeInfo = res::ResOrthoProjectionUpdaterData::TYPE;
    pResource->near = s_DefaultNear;
    pResource->far = s_DefaultFar;
    pResource->aspectRatio = s_DefaultAspectRatio;
    pResource->center = s_DefaultCenter;
    pResource->height = s_DefaultHeight;

    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) OrthoProjectionUpdater(pAllocator, true, pResource);
}

// 0x004A0F24 | nintendogs:callseq-callee [tier A]
nw::gfx::OrthoProjectionUpdater* nw::gfx::OrthoProjectionUpdater::Create(os::IAllocator* pAllocator, res::ResOrthoProjectionUpdater resource)
{
    void* pMemory = pAllocator->Alloc(sizeof(OrthoProjectionUpdater), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) OrthoProjectionUpdater(pAllocator, false, resource.ptr());
}

// 0x004A0F8C | nintendogs:bytes
void nw::gfx::OrthoProjectionUpdater::Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection)
{
    const res::ResOrthoProjectionUpdaterData* pResource = m_pResource;
    const f32 halfWidth = pResource->height * pResource->aspectRatio * 0.5f;
    const f32 halfHeight = pResource->height * 0.5f;
    const f32 left = pResource->center.x - halfWidth;
    const f32 right = pResource->center.x + halfWidth;
    const f32 bottom = pResource->center.y - halfHeight;
    const f32 top = pResource->center.y + halfHeight;
    nn::math::ARMv6::MTX44OrthoC_FAST(pProjection, left, right, bottom, top, pResource->near, pResource->far);
    nn::math::ARMv6::MTX44PivotC_FAST(pProjection, m_Pivot);
    MakeOrthoTextureProjection(pTextureProjection, left, right, bottom, top, m_TextureScale, m_TextureTranslate);
}

// 0x004A11BC slot 0x00 (name is ours)
// 0x004A1188 (deleting dtor)
nw::gfx::OrthoProjectionUpdater::~OrthoProjectionUpdater()
{
    if (m_IsResourceOwner && m_pResource != nullptr) {
        GetAllocator()->Free(m_pResource);
    }
}

} // namespace gfx
} // namespace nw
