#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx24FrustumProjectionUpdaterE
// vtable 0x0090288C (vptr 0x00902894), offset_to_top 0, 7 entries
// A perspective projection of a rectangle at the near plane (member names are ours).
class FrustumProjectionUpdater : public ::nw::gfx::CameraProjectionUpdater
{
public:
    // (inline)
    FrustumProjectionUpdater(os::IAllocator* pAllocator, bool isResourceOwner, res::ResFrustumProjectionUpdaterData* pResource)
        : CameraProjectionUpdater(pAllocator, isResourceOwner), m_pResource(pResource)
    {
    }
    virtual ~FrustumProjectionUpdater();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C4B8 slot 0x08
    virtual void Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection); // 0x004A38B4 slot 0x0C | nintendogs:bytes
    virtual res::ResCameraProjectionUpdater GetResource();
    virtual const res::ResCameraProjectionUpdater GetResource() const { return res::ResCameraProjectionUpdater(m_pResource); } // 0x0073C4B0 slot 0x14
    virtual u32 GetKind() const { return 2; } // 0x0073C4C4 slot 0x18
    static FrustumProjectionUpdater* Create(os::IAllocator* pAllocator, res::ResFrustumProjectionUpdater resource); // 0x004A384C | nintendogs:callseq-callee [tier A]

    static const TypeInfo s_TypeInfo;

    res::ResFrustumProjectionUpdaterData* m_pResource; // 0x1C
};
ASSERT_SIZE(FrustumProjectionUpdater, 0x20);
} // namespace gfx
} // namespace nw
