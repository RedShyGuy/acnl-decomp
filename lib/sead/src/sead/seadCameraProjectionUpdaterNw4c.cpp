#include "nn/math/math_ARMv6.h"
#include "sead/seadCameraProjectionUpdaterNw4c.h"
#include "sead/seadProjection.h"

#include <new>

namespace sead {
// (set by a static initializer with two type informations of classes that are not in the binary)
// 0x00976314
const nw::gfx::TypeInfo CameraProjectionUpdaterNw4c::s_TypeInfo(&nw::gfx::CameraProjectionUpdater::s_TypeInfo);

// 0x0054DA30 slot 0x10
nw::gfx::res::ResCameraProjectionUpdater sead::CameraProjectionUpdaterNw4c::GetResource()
{
    return nw::gfx::res::ResCameraProjectionUpdater(&mResource);
}

// 0x0054DA38 (name is ours)
sead::CameraProjectionUpdaterNw4c* sead::CameraProjectionUpdaterNw4c::create(nw::os::IAllocator* allocator,
                                                                           Projection* projection)
{
    void* memory = allocator->Alloc(sizeof(CameraProjectionUpdaterNw4c), 4);
    if (memory == nullptr) {
        return nullptr;
    }
    return new (memory) CameraProjectionUpdaterNw4c(allocator, projection);
}

// 0x0054DAA8 slot 0x0C
void sead::CameraProjectionUpdaterNw4c::Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection)
{
    // (copies into the given matrices)
    nn::math::ARMv6::MTX44CopyAsm(pProjection, &mProjection->getDeviceProjectionMatrix());
    nn::math::ARMv6::MTX34CopyAsm(pTextureProjection,
                                  reinterpret_cast<const nn::math::MTX34*>(&mProjection->getDeviceProjectionMatrix()));
}

// 0x0054DAE8 slot 0x00
// 0x0054DAE4 (deleting dtor)
sead::CameraProjectionUpdaterNw4c::~CameraProjectionUpdaterNw4c()
{
    // empty (in the original too)
}

// 0x0074E3E8 slot 0x18
u32 sead::CameraProjectionUpdaterNw4c::GetKind() const
{
    s32 type = mProjection->getProjectionType();
    if (type == 0 || type == 1) {
        return type;
    }
    return 0;
}
} // namespace sead
