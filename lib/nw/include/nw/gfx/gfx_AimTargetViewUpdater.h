#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx20AimTargetViewUpdaterE
// vtable 0x00902794 (vptr 0x0090279C), offset_to_top 0, 7 entries
// A view that looks at a target, turned around the line of sight (member names are ours).
class AimTargetViewUpdater : public ::nw::gfx::CameraViewUpdater
{
public:
    // (inline)
    AimTargetViewUpdater(os::IAllocator* pAllocator, bool isResourceOwner, res::ResAimTargetViewUpdaterData* pResource)
        : CameraViewUpdater(pAllocator), m_IsResourceOwner(isResourceOwner), m_pResource(pResource)
    {
    }
    virtual ~AimTargetViewUpdater();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073B0C0 slot 0x08
    virtual void Update(nn::math::MTX34* pView, const nn::math::MTX34& worldMatrix, const nn::math::VEC3& cameraPosition); // 0x004A0A9C slot 0x0C | nintendogs:bytes
    virtual res::ResCameraViewUpdater GetResource();
    virtual const res::ResCameraViewUpdater GetResource() const { return res::ResCameraViewUpdater(m_pResource); } // 0x0073B0B8 slot 0x14
    virtual u32 GetKind() const { return 0; } // 0x0073B0CC slot 0x18
    static AimTargetViewUpdater* Create(os::IAllocator* pAllocator, res::ResAimTargetViewUpdater resource); // 0x004A0A50 | nintendogs:callseq-callee [tier A]

    static const TypeInfo s_TypeInfo;

    bool m_IsResourceOwner;                       // 0x8 the resource is freed with the updater
    res::ResAimTargetViewUpdaterData* m_pResource; // 0xC
};
ASSERT_SIZE(AimTargetViewUpdater, 0x10);
} // namespace gfx
} // namespace nw
