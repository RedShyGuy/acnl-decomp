#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx23LookAtTargetViewUpdaterE @ 0x008D073C
// vtable 0x0090283C (vptr 0x00902844), offset_to_top 0, 7 entries
// A view that looks at a target with an upward vector (member names are ours).
class LookAtTargetViewUpdater : public ::nw::gfx::CameraViewUpdater
{
public:
    // (inline)
    LookAtTargetViewUpdater(os::IAllocator* pAllocator, bool isResourceOwner, res::ResLookAtTargetViewUpdaterData* pResource)
        : CameraViewUpdater(pAllocator), m_IsResourceOwner(isResourceOwner), m_pResource(pResource)
    {
    }
    virtual ~LookAtTargetViewUpdater();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C484 slot 0x08
    virtual void Update(nn::math::MTX34* pView, const nn::math::MTX34& worldMatrix, const nn::math::VEC3& cameraPosition); // 0x004A1E54 slot 0x0C | nintendogs:bytes
    virtual res::ResCameraViewUpdater GetResource();
    virtual const res::ResCameraViewUpdater GetResource() const { return res::ResCameraViewUpdater(m_pResource); } // 0x0073C47C slot 0x14
    virtual u32 GetKind() const { return 1; } // 0x0073C490 slot 0x18
    // an updater with a resource of its own (the default target and upward vector)
    static LookAtTargetViewUpdater* Create(os::IAllocator* pAllocator); // 0x004A1D30 | nintendogs:bytes [tier A]
    static LookAtTargetViewUpdater* Create(os::IAllocator* pAllocator, res::ResLookAtTargetViewUpdater resource); // 0x004A1E08 | nintendogs:callseq-callee [tier A]

    static const TypeInfo s_TypeInfo;

    bool m_IsResourceOwner;                       // 0x8 the resource is freed with the updater
    res::ResLookAtTargetViewUpdaterData* m_pResource; // 0xC
};
ASSERT_SIZE(LookAtTargetViewUpdater, 0x10);
} // namespace gfx
} // namespace nw
