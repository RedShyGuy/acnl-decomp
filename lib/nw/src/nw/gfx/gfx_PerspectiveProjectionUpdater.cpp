#include <cstring>
#include <new>

#include "nn/math/math_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_PerspectiveProjectionUpdater.h"
#include "nw/math/math_Api.h"

namespace nw {
namespace gfx {
namespace {
// (inline) a perspective projection by the field of view (name is ours)
inline void MakePerspective(nn::math::MTX44* pOut, f32 fovy, f32 aspect, f32 near, f32 far)
{
    f32 sinValue, cosValue;
    nn::math::SinCosFIdx(&sinValue, &cosValue, fovy * 0.5f * math::RAD_TO_FIDX);
    const f32 cotangent = cosValue / sinValue;
    const f32 depth = 1.0f / (far - near);
    pOut->m[0][1] = 0.0f;
    pOut->m[0][2] = 0.0f;
    pOut->m[0][3] = 0.0f;
    pOut->m[1][0] = 0.0f;
    pOut->m[1][2] = 0.0f;
    pOut->m[1][3] = 0.0f;
    pOut->m[2][0] = 0.0f;
    pOut->m[2][1] = 0.0f;
    pOut->m[3][0] = 0.0f;
    pOut->m[3][1] = 0.0f;
    pOut->m[3][2] = -1.0f;
    pOut->m[3][3] = 0.0f;
    pOut->m[0][0] = cotangent / aspect;
    pOut->m[1][1] = cotangent;
    pOut->m[2][2] = far * depth;
    pOut->m[2][3] = far * near * depth;
}

// (inline) the matrix that projects textures for a perspective projection (name is ours)
inline void MakePerspectiveTextureProjection(nn::math::MTX34* pOut, f32 fovy, f32 aspect, const nn::math::VEC2& scale,
                                             const nn::math::VEC2& translate)
{
    f32 sinValue, cosValue;
    nn::math::SinCosFIdx(&sinValue, &cosValue, fovy * 0.5f * math::RAD_TO_FIDX);
    const f32 cotangent = 1.0f / (sinValue / cosValue);
    pOut->m[0][0] = cotangent / aspect * scale.x;
    pOut->m[0][1] = 0.0f;
    pOut->m[0][2] = -translate.x;
    pOut->m[0][3] = 0.0f;
    pOut->m[1][0] = 0.0f;
    pOut->m[1][1] = cotangent * scale.y;
    pOut->m[1][2] = -translate.y;
    pOut->m[1][3] = 0.0f;
    pOut->m[2][0] = 0.0f;
    pOut->m[2][1] = 0.0f;
    pOut->m[2][2] = -1.0f;
    pOut->m[2][3] = 0.0f;
}
} // namespace

// 0x0097F490
const TypeInfo nw::gfx::PerspectiveProjectionUpdater::s_TypeInfo(&CameraProjectionUpdater::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FEA8 (static initializer)

// 0x004A439C slot 0x10 (name is ours)
nw::gfx::res::ResCameraProjectionUpdater nw::gfx::PerspectiveProjectionUpdater::GetResource()
{
    return res::ResCameraProjectionUpdater(m_pResource);
}

// 0x004A43A4 | nintendogs:bytes [tier A]
nw::gfx::PerspectiveProjectionUpdater* nw::gfx::PerspectiveProjectionUpdater::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(PerspectiveProjectionUpdater), 4);

    void* pResourceMemory = pAllocator->Alloc(sizeof(res::ResPerspectiveProjectionUpdaterData), 4);
    std::memset(pResourceMemory, 0, sizeof(res::ResPerspectiveProjectionUpdaterData));
    res::ResPerspectiveProjectionUpdaterData* pResource = new (pResourceMemory) res::ResPerspectiveProjectionUpdaterData();
    pResource->typeInfo = res::ResPerspectiveProjectionUpdaterData::TYPE;
    pResource->near = s_DefaultNear;
    pResource->far = s_DefaultFar;
    pResource->fovy = s_DefaultFovy;
    pResource->aspectRatio = s_DefaultAspectRatio;

    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) PerspectiveProjectionUpdater(pAllocator, true, pResource);
}

// 0x004A44A0 | nintendogs:callseq-callee [tier A]
nw::gfx::PerspectiveProjectionUpdater* nw::gfx::PerspectiveProjectionUpdater::Create(os::IAllocator* pAllocator,
                                                                                   res::ResPerspectiveProjectionUpdater resource)
{
    void* pMemory = pAllocator->Alloc(sizeof(PerspectiveProjectionUpdater), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) PerspectiveProjectionUpdater(pAllocator, false, resource.ptr());
}

// 0x004A4508 | nintendogs:bytes
void nw::gfx::PerspectiveProjectionUpdater::Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection)
{
    const res::ResPerspectiveProjectionUpdaterData* pResource = m_pResource;
    MakePerspective(pProjection, pResource->fovy, pResource->aspectRatio, pResource->near, pResource->far);
    nn::math::ARMv6::MTX44PivotC_FAST(pProjection, m_Pivot);
    MakePerspectiveTextureProjection(pTextureProjection, pResource->fovy, pResource->aspectRatio, m_TextureScale, m_TextureTranslate);
}

// 0x004A4728 slot 0x00 (name is ours)
// 0x004A46F4 (deleting dtor)
nw::gfx::PerspectiveProjectionUpdater::~PerspectiveProjectionUpdater()
{
    if (m_IsResourceOwner && m_pResource != nullptr) {
        GetAllocator()->Free(m_pResource);
    }
}

} // namespace gfx
} // namespace nw
