#include "nw/gfx/gfx_BillboardUpdater.h"

#include <cmath>
#include <new>

#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/gfx/gfx_CalculatedTransform.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the vector scaled to length 1 (name is ours)
inline void Normalize(nn::math::VEC3* pV)
{
    const f32 scale = 1.0f / std::sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
    pV->x *= scale;
    pV->y *= scale;
    pV->z *= scale;
}

// (inline) the axes as the columns of the rotation (name is ours)
inline void SetAxes(nn::math::MTX34* pMatrix, const nn::math::VEC3& x, const nn::math::VEC3& y, const nn::math::VEC3& z)
{
    pMatrix->m[0][0] = x.x;
    pMatrix->m[0][1] = y.x;
    pMatrix->m[0][2] = z.x;
    pMatrix->m[1][0] = x.y;
    pMatrix->m[1][1] = y.y;
    pMatrix->m[1][2] = z.y;
    pMatrix->m[2][0] = x.z;
    pMatrix->m[2][1] = y.z;
    pMatrix->m[2][2] = z.z;
}
} // namespace

// 0x0049BF78 slot 0x00 (name is ours)
// 0x0049BF74 (deleting dtor)
nw::gfx::BillboardUpdater::~BillboardUpdater()
{
}

// 0x0049BF3C | nintendogs:callgraph [tier A]
nw::gfx::BillboardUpdater* nw::gfx::BillboardUpdater::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(BillboardUpdater), 4);
    return pMemory != nullptr ? new (pMemory) BillboardUpdater(pAllocator) : nullptr;
}

// 0x00739568 | nintendogs:bytes [tier B]
void nw::gfx::BillboardUpdater::CalculateLocalMatrix(nn::math::MTX34* pMatrix, const CalculatedTransform& transform, nn::math::VEC3 direction,
                                                     bool isZFixed) const
{
    Normalize(&direction);
    if (!transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_IDENTITY)) {
        const nn::math::MTX34& m = transform.m_TransformMatrix;
        nn::math::VEC3 y(m.m[0][1], m.m[1][1], m.m[2][1]);
        nn::math::VEC3SafeNormalize(&y, &y, nn::math::VEC3(1.0f, 0.0f, 0.0f));
        nn::math::VEC3 x;
        nn::math::VEC3Cross(&x, &y, &direction);
        nn::math::VEC3SafeNormalize(&x, &x, nn::math::VEC3(m.m[0][2], m.m[1][2], m.m[2][2]));
        if (isZFixed) {
            nn::math::VEC3Cross(&y, &direction, &x);
        } else {
            nn::math::VEC3Cross(&direction, &x, &y);
        }
        SetAxes(pMatrix, x, y, direction);
        pMatrix->m[0][3] = m.m[0][3];
        pMatrix->m[1][3] = m.m[1][3];
        pMatrix->m[2][3] = m.m[2][3];
    } else {
        nn::math::VEC3 y(0.0f, 1.0f, 0.0f);
        nn::math::VEC3 x;
        nn::math::VEC3SafeNormalize(nn::math::VEC3Cross(&x, &y, &direction), &x, nn::math::VEC3(1.0f, 0.0f, 0.0f));
        if (isZFixed) {
            nn::math::VEC3Cross(&y, &direction, &x);
        } else {
            nn::math::VEC3Cross(&direction, &x, &y);
        }
        SetAxes(pMatrix, x, y, direction);
        pMatrix->m[0][3] = 0.0f;
        pMatrix->m[1][3] = 0.0f;
        pMatrix->m[2][3] = 0.0f;
    }
}

// 0x00739780 | nintendogs:bytes [tier B]
void nw::gfx::BillboardUpdater::CalculateScreenLocalMatrix(nn::math::MTX34* pMatrix, const CalculatedTransform& transform,
                                                           const nn::math::MTX34& inverseViewMatrix, nn::math::VEC3 up,
                                                           nn::math::VEC3& direction) const
{
    const nn::math::MTX34& m = transform.m_TransformMatrix;
    if (!transform.IsEnabledFlags(CalculatedTransform::FLAG_IS_IDENTITY)) {
        Normalize(&up);
        nn::math::VEC3 x;
        Normalize(nn::math::VEC3Cross(&x, &up, &direction));
        nn::math::VEC3Cross(&up, &direction, &x);
        SetAxes(pMatrix, x, up, direction);
        pMatrix->m[0][3] = m.m[0][3];
        pMatrix->m[1][3] = m.m[1][3];
        pMatrix->m[2][3] = m.m[2][3];
    } else {
        nn::math::VEC3 y(0.0f, 1.0f, 0.0f);
        nn::math::VEC3 x;
        Normalize(nn::math::VEC3Cross(&x, &y, &direction));
        nn::math::VEC3Cross(&y, &direction, &x);
        SetAxes(pMatrix, x, y, direction);
        pMatrix->m[0][3] = 0.0f;
        pMatrix->m[1][3] = 0.0f;
        pMatrix->m[2][3] = 0.0f;
    }
    nn::math::ARMv6::MTX33MultAsm(pMatrix, &inverseViewMatrix, pMatrix);
    pMatrix->m[0][3] = m.m[0][3];
    pMatrix->m[1][3] = m.m[1][3];
    pMatrix->m[2][3] = m.m[2][3];
}

// 0x007399A8 | nintendogs:bytes-fuzzy [tier B]
void nw::gfx::BillboardUpdater::Update(nn::math::MTX34* pMatrix, const nn::math::MTX34& viewMatrix, const nn::math::MTX34& inverseViewMatrix,
                                       const nn::math::VEC3& cameraPosition, const CalculatedTransform& worldTransform,
                                       const CalculatedTransform& localTransform, res::ResBone::BillboardMode mode) const
{
    const nn::math::MTX34& world = worldTransform.m_TransformMatrix;
    const nn::math::MTX34& local = localTransform.m_TransformMatrix;
    switch (mode) {
    case res::ResBone::BILLBOARD_MODE_WORLD:
        CalculateLocalMatrix(pMatrix, worldTransform,
                             nn::math::VEC3(inverseViewMatrix.m[0][2], inverseViewMatrix.m[1][2], inverseViewMatrix.m[2][2]), true);
        break;
    case res::ResBone::BILLBOARD_MODE_WORLD_VIEWPOINT:
        CalculateLocalMatrix(pMatrix, worldTransform,
                             nn::math::VEC3(cameraPosition.x - world.m[0][3], cameraPosition.y - world.m[1][3], cameraPosition.z - world.m[2][3]),
                             true);
        break;
    case res::ResBone::BILLBOARD_MODE_SCREEN: {
        nn::math::VEC3 direction(0.0f, 0.0f, 1.0f);
        CalculateScreenLocalMatrix(pMatrix, worldTransform, inverseViewMatrix, nn::math::VEC3(local.m[0][1], local.m[1][1], local.m[2][1]),
                                   direction);
        break;
    }
    case res::ResBone::BILLBOARD_MODE_SCREEN_VIEWPOINT: {
        // the direction to the camera in view space
        const f32 x = world.m[0][3];
        const f32 y = world.m[1][3];
        const f32 z = world.m[2][3];
        nn::math::VEC3 direction(-(viewMatrix.m[0][1] * y + viewMatrix.m[0][2] * z + (viewMatrix.m[0][3] + viewMatrix.m[0][0] * x)),
                                 -(viewMatrix.m[1][1] * y + viewMatrix.m[1][2] * z + (viewMatrix.m[1][3] + viewMatrix.m[1][0] * x)),
                                 -(viewMatrix.m[2][1] * y + viewMatrix.m[2][2] * z + (viewMatrix.m[2][3] + viewMatrix.m[2][0] * x)));
        nn::math::VEC3SafeNormalize(&direction, &direction, nn::math::VEC3(0.0f, 0.0f, 1.0f));
        CalculateScreenLocalMatrix(pMatrix, worldTransform, inverseViewMatrix, nn::math::VEC3(local.m[0][1], local.m[1][1], local.m[2][1]),
                                   direction);
        break;
    }
    case res::ResBone::BILLBOARD_MODE_Y_AXIAL:
        CalculateLocalMatrix(pMatrix, worldTransform,
                             nn::math::VEC3(inverseViewMatrix.m[0][2], inverseViewMatrix.m[1][2], inverseViewMatrix.m[2][2]), false);
        break;
    case res::ResBone::BILLBOARD_MODE_Y_AXIAL_VIEWPOINT:
        CalculateLocalMatrix(pMatrix, worldTransform,
                             nn::math::VEC3(cameraPosition.x - world.m[0][3], cameraPosition.y - world.m[1][3], cameraPosition.z - world.m[2][3]),
                             false);
        break;
    default:
        break;
    }
}

} // namespace gfx
} // namespace nw
