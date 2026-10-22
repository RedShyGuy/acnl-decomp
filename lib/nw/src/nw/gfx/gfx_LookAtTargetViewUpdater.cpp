#include <cstring>
#include <new>

#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX33.h"
#include "nw/gfx/gfx_LookAtTargetViewUpdater.h"

namespace nw {
namespace gfx {
// 0x009760D8
const TypeInfo nw::gfx::LookAtTargetViewUpdater::s_TypeInfo(&CameraViewUpdater::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FCD8 (static initializer)

// 0x004A1D28 slot 0x10 (name is ours)
nw::gfx::res::ResCameraViewUpdater nw::gfx::LookAtTargetViewUpdater::GetResource()
{
    return res::ResCameraViewUpdater(m_pResource);
}

// 0x004A1D30 | nintendogs:bytes [tier A]
nw::gfx::LookAtTargetViewUpdater* nw::gfx::LookAtTargetViewUpdater::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(LookAtTargetViewUpdater), 4);

    void* pResourceMemory = pAllocator->Alloc(sizeof(res::ResLookAtTargetViewUpdaterData), 4);
    std::memset(pResourceMemory, 0, sizeof(res::ResLookAtTargetViewUpdaterData));
    res::ResLookAtTargetViewUpdaterData* pResource = new (pResourceMemory) res::ResLookAtTargetViewUpdaterData();
    pResource->typeInfo = res::ResLookAtTargetViewUpdaterData::TYPE;
    pResource->targetPosition = s_DefaultTargetPosition;
    pResource->flags = 0;
    pResource->upwardVector = s_DefaultUpwardVector;

    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) LookAtTargetViewUpdater(pAllocator, true, pResource);
}

// 0x004A1E08 | nintendogs:callseq-callee [tier A]
nw::gfx::LookAtTargetViewUpdater* nw::gfx::LookAtTargetViewUpdater::Create(os::IAllocator* pAllocator, res::ResLookAtTargetViewUpdater resource)
{
    void* pMemory = pAllocator->Alloc(sizeof(LookAtTargetViewUpdater), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) LookAtTargetViewUpdater(pAllocator, false, resource.ptr());
}

// 0x004A1E54 | nintendogs:bytes
void nw::gfx::LookAtTargetViewUpdater::Update(nn::math::MTX34* pView, const nn::math::MTX34& worldMatrix, const nn::math::VEC3& cameraPosition)
{
    const u32 flags = m_pResource->flags;
    nn::math::VEC3 target = m_pResource->targetPosition;
    nn::math::VEC3 upward = m_pResource->upwardVector;
    if (flags & (res::ResLookAtTargetViewUpdaterData::FLAG_INHERIT_TARGET_ROTATION | res::ResLookAtTargetViewUpdaterData::FLAG_INHERIT_UPWARD_ROTATION |
                 res::ResLookAtTargetViewUpdaterData::FLAG_TARGET_RELATIVE)) {
        nn::math::MTX33 rotation;
        nn::math::ARMv6::MTX34ToMTX33Asm(&rotation, &worldMatrix);
        if (flags & res::ResLookAtTargetViewUpdaterData::FLAG_INHERIT_TARGET_ROTATION) {
            nn::math::ARMv6::VEC3TransformAsm(&target, &rotation, &target);
        }
        if (flags & res::ResLookAtTargetViewUpdaterData::FLAG_INHERIT_UPWARD_ROTATION) {
            nn::math::ARMv6::VEC3TransformAsm(&upward, &rotation, &upward);
        }
        if (flags & res::ResLookAtTargetViewUpdaterData::FLAG_TARGET_RELATIVE) {
            target.x += cameraPosition.x;
            target.y += cameraPosition.y;
            target.z += cameraPosition.z;
        }
    }
    nn::math::ARMv6::MTX34LookAtC_FAST(pView, &cameraPosition, &upward, &target);
}

// 0x004A204C slot 0x00 (name is ours)
// 0x004A2018 (deleting dtor)
nw::gfx::LookAtTargetViewUpdater::~LookAtTargetViewUpdater()
{
    if (m_IsResourceOwner && m_pResource != nullptr) {
        GetAllocator()->Free(m_pResource);
    }
}

} // namespace gfx
} // namespace nw
