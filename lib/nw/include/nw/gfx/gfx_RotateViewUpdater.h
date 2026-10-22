#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx17RotateViewUpdaterE @ 0x008D06B8
// vtable 0x0090274C (vptr 0x00902754), offset_to_top 0, 7 entries
// A view turned by angles (member names are ours).
class RotateViewUpdater : public ::nw::gfx::CameraViewUpdater
{
public:
    // (inline)
    RotateViewUpdater(os::IAllocator* pAllocator, bool isResourceOwner, res::ResRotateViewUpdaterData* pResource)
        : CameraViewUpdater(pAllocator), m_IsResourceOwner(isResourceOwner), m_pResource(pResource)
    {
    }
    virtual ~RotateViewUpdater();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073A88C slot 0x08
    virtual void Update(nn::math::MTX34* pView, const nn::math::MTX34& worldMatrix, const nn::math::VEC3& cameraPosition); // 0x0049E184 slot 0x0C | nintendogs:bytes
    virtual res::ResCameraViewUpdater GetResource();
    virtual const res::ResCameraViewUpdater GetResource() const { return res::ResCameraViewUpdater(m_pResource); } // 0x0073A884 slot 0x14
    virtual u32 GetKind() const { return 2; } // 0x0073A898 slot 0x18
    static RotateViewUpdater* Create(os::IAllocator* pAllocator, res::ResRotateViewUpdater resource); // 0x0049E138 | nintendogs:callseq-callee [tier A]

    static const TypeInfo s_TypeInfo;

    bool m_IsResourceOwner;                       // 0x8 the resource is freed with the updater
    res::ResRotateViewUpdaterData* m_pResource; // 0xC
};
ASSERT_SIZE(RotateViewUpdater, 0x10);
} // namespace gfx
} // namespace nw
