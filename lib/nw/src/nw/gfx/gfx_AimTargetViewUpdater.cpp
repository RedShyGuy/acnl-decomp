#include <cmath>
#include <new>

#include "nn/math/math_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX33.h"
#include "nw/gfx/gfx_AimTargetViewUpdater.h"
#include "nw/math/math_Api.h"

namespace nw {
namespace gfx {
// 0x0097F3C8
const TypeInfo nw::gfx::AimTargetViewUpdater::s_TypeInfo(&CameraViewUpdater::s_TypeInfo);

// sets s_TypeInfo
// 0x0079F684 (static initializer)

// 0x004A0A48 slot 0x10 (name is ours)
nw::gfx::res::ResCameraViewUpdater nw::gfx::AimTargetViewUpdater::GetResource()
{
    return res::ResCameraViewUpdater(m_pResource);
}

// 0x004A0A50 | nintendogs:callseq-callee [tier A]
nw::gfx::AimTargetViewUpdater* nw::gfx::AimTargetViewUpdater::Create(os::IAllocator* pAllocator, res::ResAimTargetViewUpdater resource)
{
    void* pMemory = pAllocator->Alloc(sizeof(AimTargetViewUpdater), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) AimTargetViewUpdater(pAllocator, false, resource.ptr());
}

// 0x004A0A9C | nintendogs:bytes
void nw::gfx::AimTargetViewUpdater::Update(nn::math::MTX34* pView, const nn::math::MTX34& worldMatrix, const nn::math::VEC3& cameraPosition)
{
    const u32 flags = m_pResource->flags;
    if (flags & (res::ResAimTargetViewUpdaterData::FLAG_INHERIT_ROTATION | res::ResAimTargetViewUpdaterData::FLAG_TARGET_RELATIVE)) {
        const nn::math::VEC3& targetPosition = m_pResource->targetPosition;
        const f32 dx = cameraPosition.x - targetPosition.x;
        const f32 dy = cameraPosition.y - targetPosition.y;
        const f32 dz = cameraPosition.z - targetPosition.z;
        // not straight up or down: the twist turns an upward vector, which may then turn with the parent
        if (!(dx == 0.0f && dz == 0.0f)) {
            nn::math::MTX33 rotation;
            nn::math::ARMv6::MTX34ToMTX33Asm(&rotation, &worldMatrix);

            // the line of sight and a horizontal vector across it
            const f32 inverse = 1.0f / std::sqrt(dx * dx + dy * dy + dz * dz);
            const f32 forwardX = dx * inverse;
            const f32 forwardY = dy * inverse;
            const f32 forwardZ = dz * inverse;
            const f32 sideInverse = 1.0f / std::sqrt(dz * dz + 0.0f * 0.0f + -dx * -dx);
            const f32 sideX = dz * sideInverse;
            const f32 sideY = 0.0f * sideInverse;
            const f32 sideZ = -dx * sideInverse;
            const f32 upX = forwardY * sideZ - forwardZ * sideY;
            const f32 upY = forwardZ * sideX - forwardX * sideZ;
            const f32 upZ = forwardX * sideY - forwardY * sideX;

            f32 sinTwist, cosTwist;
            nn::math::SinCosFIdx(&sinTwist, &cosTwist, m_pResource->twist * math::RAD_TO_FIDX);
            nn::math::VEC3 upward;
            upward.x = upX * cosTwist - sinTwist * sideX;
            upward.y = upY * cosTwist - sinTwist * sideY;
            upward.z = upZ * cosTwist - sinTwist * sideZ;

            nn::math::VEC3 target = m_pResource->targetPosition;
            if (flags & res::ResAimTargetViewUpdaterData::FLAG_INHERIT_ROTATION) {
                nn::math::ARMv6::VEC3TransformAsm(&target, &rotation, &target);
                nn::math::ARMv6::VEC3TransformAsm(&upward, &rotation, &upward);
            }
            if (flags & res::ResAimTargetViewUpdaterData::FLAG_TARGET_RELATIVE) {
                target.x += cameraPosition.x;
                target.y += cameraPosition.y;
                target.z += cameraPosition.z;
            }
            nn::math::ARMv6::MTX34LookAtC_FAST(pView, &cameraPosition, &upward, &target);
            return;
        }
    }
    math::MTX34LookAtFIdx(pView, &cameraPosition, m_pResource->twist * math::RAD_TO_FIDX, &m_pResource->targetPosition);
}

// 0x004A0DA0 slot 0x00 (name is ours)
// 0x004A0D6C (deleting dtor)
nw::gfx::AimTargetViewUpdater::~AimTargetViewUpdater()
{
    if (m_IsResourceOwner && m_pResource != nullptr) {
        GetAllocator()->Free(m_pResource);
    }
}

} // namespace gfx
} // namespace nw
