#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"

namespace sead {
class Projection;

// RTTI N4sead27CameraProjectionUpdaterNw4cE @ 0x008D1F78
// vtable 0x00906840 (vptr 0x00906848), offset_to_top 0, 7 entries
// The projection updater of an nw::gfx camera that takes the matrices of a sead projection (its
// resource is a bare header; member names and create are ours).
class CameraProjectionUpdaterNw4c : public ::nw::gfx::CameraProjectionUpdater
{
public:
    CameraProjectionUpdaterNw4c(nw::os::IAllocator* allocator, Projection* projection)
        : CameraProjectionUpdater(allocator, false), mProjection(projection)
    {
        mResource.typeInfo = 0x80000000;
    }
    // the updater on the memory of allocator (NULL if there is none)
    static CameraProjectionUpdaterNw4c* create(nw::os::IAllocator* allocator, Projection* projection); // 0x0054DA38

    virtual ~CameraProjectionUpdaterNw4c(); // 0x0054DAE8 slot 0x00
    // 0x0054DAE4 slot 0x04 (deleting dtor)
    virtual const nw::gfx::TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0074E3DC slot 0x08
    // the device projection matrix of the projection (its upper rows for the textures)
    virtual void Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection); // 0x0054DAA8 slot 0x0C
    virtual nw::gfx::res::ResCameraProjectionUpdater GetResource(); // 0x0054DA30 slot 0x10
    virtual const nw::gfx::res::ResCameraProjectionUpdater GetResource() const // 0x0074E3D4 slot 0x14
    {
        return nw::gfx::res::ResCameraProjectionUpdater(&mResource);
    }
    // the kind of the projection (0 perspective, 1 orthographic; others as 0)
    virtual u32 GetKind() const; // 0x0074E3E8 slot 0x18

    static const nw::gfx::TypeInfo s_TypeInfo;

    Projection* mProjection;                                   // 0x1C
    nw::gfx::res::ResCameraProjectionUpdaterData mResource;    // 0x20
};
ASSERT_SIZE(CameraProjectionUpdaterNw4c, 0x2C);
} // namespace sead
