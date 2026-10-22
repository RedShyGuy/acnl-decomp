#include <new>

#include "nn/math/math_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX33.h"
#include "nw/gfx/gfx_RotateViewUpdater.h"
#include "nw/math/math_Api.h"

namespace nw {
namespace gfx {
// 0x0097F494
const TypeInfo nw::gfx::RotateViewUpdater::s_TypeInfo(&CameraViewUpdater::s_TypeInfo);

// sets s_TypeInfo
// 0x0079E8C0 (static initializer)

// 0x0049E130 slot 0x10 (name is ours)
nw::gfx::res::ResCameraViewUpdater nw::gfx::RotateViewUpdater::GetResource()
{
    return res::ResCameraViewUpdater(m_pResource);
}

// 0x0049E138 | nintendogs:callseq-callee [tier A]
nw::gfx::RotateViewUpdater* nw::gfx::RotateViewUpdater::Create(os::IAllocator* pAllocator, res::ResRotateViewUpdater resource)
{
    void* pMemory = pAllocator->Alloc(sizeof(RotateViewUpdater), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) RotateViewUpdater(pAllocator, false, resource.ptr());
}

// 0x0049E184 | nintendogs:bytes
void nw::gfx::RotateViewUpdater::Update(nn::math::MTX34* pView, const nn::math::MTX34& worldMatrix, const nn::math::VEC3& cameraPosition)
{
    if (!(m_pResource->flags & res::ResRotateViewUpdaterData::FLAG_INHERIT_ROTATION)) {
        // the inverse of the rotation (around Z, then X, then Y) at the camera position
        const nn::math::VEC3& rotate = m_pResource->viewRotate;
        f32 sinX, cosX, sinY, cosY, sinZ, cosZ;
        nn::math::SinCosFIdx(&sinX, &cosX, rotate.x * math::RAD_TO_FIDX);
        nn::math::SinCosFIdx(&sinY, &cosY, rotate.y * math::RAD_TO_FIDX);
        nn::math::SinCosFIdx(&sinZ, &cosZ, rotate.z * math::RAD_TO_FIDX);
        const f32 sinXsinY = sinX * sinY;
        const f32 sinXcosY = sinX * cosY;

        pView->m[0][0] = sinXsinY * sinZ + cosY * cosZ;
        pView->m[0][1] = cosX * sinZ;
        pView->m[0][2] = sinXcosY * sinZ - sinY * cosZ;
        pView->m[0][3] = -(cameraPosition.x * pView->m[0][0] + cameraPosition.y * pView->m[0][1]) - cameraPosition.z * pView->m[0][2];
        pView->m[1][0] = sinXsinY * cosZ - cosY * sinZ;
        pView->m[1][1] = cosX * cosZ;
        pView->m[1][2] = sinXcosY * cosZ + sinY * sinZ;
        pView->m[1][3] = -(cameraPosition.x * pView->m[1][0] + cameraPosition.y * pView->m[1][1]) - cameraPosition.z * pView->m[1][2];
        pView->m[2][0] = cosX * sinY;
        pView->m[2][1] = -sinX;
        pView->m[2][2] = cosX * cosY;
        pView->m[2][3] = -(cameraPosition.x * pView->m[2][0] + cameraPosition.y * pView->m[2][1]) - cameraPosition.z * pView->m[2][2];
        return;
    }

    // relative to the parent: the upward vector and the line of sight turned, then a look-at view
    nn::math::MTX33 parentRotation;
    nn::math::ARMv6::MTX34ToMTX33Asm(&parentRotation, &worldMatrix);
    nn::math::VEC3 upward(0.0f, 1.0f, 0.0f);
    nn::math::VEC3 target(0.0f, 0.0f, -1.0f);

    nn::math::MTX34 rotateX, rotateY, rotateZ, rotation;
    math::MTX34RotateXYZRad(&rotateX, m_pResource->viewRotate.x, 0.0f, 0.0f);
    math::MTX34RotateXYZRad(&rotateY, 0.0f, m_pResource->viewRotate.y, 0.0f);
    math::MTX34RotateXYZRad(&rotateZ, 0.0f, 0.0f, m_pResource->viewRotate.z);
    nn::math::ARMv6::MTX34MultAsm(&rotation, &rotateY, &rotateX);
    nn::math::ARMv6::MTX34MultAsm(&rotation, &rotation, &rotateZ);

    nn::math::ARMv6::VEC3TransformAsm(&upward, &rotation, &upward);
    nn::math::ARMv6::VEC3TransformAsm(&upward, &parentRotation, &upward);
    nn::math::ARMv6::VEC3TransformAsm(&target, &rotation, &target);
    nn::math::ARMv6::VEC3TransformAsm(&target, &parentRotation, &target);
    target.x += cameraPosition.x;
    target.y += cameraPosition.y;
    target.z += cameraPosition.z;
    nn::math::ARMv6::MTX34LookAtC_FAST(pView, &cameraPosition, &upward, &target);
}

// 0x0049E998 slot 0x00 (name is ours)
// 0x0049E964 (deleting dtor)
nw::gfx::RotateViewUpdater::~RotateViewUpdater()
{
    if (m_IsResourceOwner && m_pResource != nullptr) {
        GetAllocator()->Free(m_pResource);
    }
}

} // namespace gfx
} // namespace nw
