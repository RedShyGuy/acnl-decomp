#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx28PerspectiveProjectionUpdaterE @ 0x008D0790
// vtable 0x00902904 (vptr 0x0090290C), offset_to_top 0, 7 entries
// A perspective projection by the field of view (member names are ours).
class PerspectiveProjectionUpdater : public ::nw::gfx::CameraProjectionUpdater
{
public:
    // (inline)
    PerspectiveProjectionUpdater(os::IAllocator* pAllocator, bool isResourceOwner, res::ResPerspectiveProjectionUpdaterData* pResource)
        : CameraProjectionUpdater(pAllocator, isResourceOwner), m_pResource(pResource)
    {
    }
    virtual ~PerspectiveProjectionUpdater();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C540 slot 0x08
    virtual void Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection); // 0x004A4508 slot 0x0C | nintendogs:bytes
    virtual res::ResCameraProjectionUpdater GetResource();
    virtual const res::ResCameraProjectionUpdater GetResource() const { return res::ResCameraProjectionUpdater(m_pResource); } // 0x0073C538 slot 0x14
    virtual u32 GetKind() const { return 0; } // 0x0073C54C slot 0x18
    // an updater with a resource of its own (the defaults)
    static PerspectiveProjectionUpdater* Create(os::IAllocator* pAllocator); // 0x004A43A4 | nintendogs:bytes [tier A]
    static PerspectiveProjectionUpdater* Create(os::IAllocator* pAllocator, res::ResPerspectiveProjectionUpdater resource); // 0x004A44A0 | nintendogs:callseq-callee [tier A]

    static const TypeInfo s_TypeInfo;

    res::ResPerspectiveProjectionUpdaterData* m_pResource; // 0x1C
};
ASSERT_SIZE(PerspectiveProjectionUpdater, 0x20);
} // namespace gfx
} // namespace nw
