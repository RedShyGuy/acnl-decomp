#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx22OrthoProjectionUpdaterE
// vtable 0x009027DC (vptr 0x009027E4), offset_to_top 0, 7 entries
// An orthographic projection of a rectangle (member names are ours).
class OrthoProjectionUpdater : public ::nw::gfx::CameraProjectionUpdater
{
public:
    // (inline)
    OrthoProjectionUpdater(os::IAllocator* pAllocator, bool isResourceOwner, res::ResOrthoProjectionUpdaterData* pResource)
        : CameraProjectionUpdater(pAllocator, isResourceOwner), m_pResource(pResource)
    {
    }
    virtual ~OrthoProjectionUpdater();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073B9DC slot 0x08
    virtual void Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection); // 0x004A0F8C slot 0x0C | nintendogs:bytes
    virtual res::ResCameraProjectionUpdater GetResource();
    virtual const res::ResCameraProjectionUpdater GetResource() const { return res::ResCameraProjectionUpdater(m_pResource); } // 0x0073B9D4 slot 0x14
    virtual u32 GetKind() const { return 1; } // 0x0073B9E8 slot 0x18
    // an updater with a resource of its own (the defaults)
    static OrthoProjectionUpdater* Create(os::IAllocator* pAllocator); // 0x004A0E08 | nintendogs:bytes [tier A]
    static OrthoProjectionUpdater* Create(os::IAllocator* pAllocator, res::ResOrthoProjectionUpdater resource); // 0x004A0F24 | nintendogs:callseq-callee [tier A]

    static const TypeInfo s_TypeInfo;

    res::ResOrthoProjectionUpdaterData* m_pResource; // 0x1C
};
ASSERT_SIZE(OrthoProjectionUpdater, 0x20);
} // namespace gfx
} // namespace nw
