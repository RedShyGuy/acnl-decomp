#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"
#include "nn/math/math_Vector2.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResCamera.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx23CameraProjectionUpdaterE @ 0x008D0730
// The part of a camera that makes its projection matrix (and the matrix that projects textures) from
// its resource (member and virtual function names are ours).
class CameraProjectionUpdater : public ::nw::gfx::GfxObject
{
public:
    // (inline)
    CameraProjectionUpdater(os::IAllocator* pAllocator, bool isResourceOwner)
        : GfxObject(pAllocator), m_Pivot(nn::math::PIVOT_DIRECTION_0), m_IsResourceOwner(isResourceOwner), m_TextureScale(0.5f, 0.5f),
          m_TextureTranslate(0.5f, 0.5f)
    {
    }
    virtual ~CameraProjectionUpdater() {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const = 0; // slot 0x08
    // the projection matrix and the matrix that projects textures
    virtual void Update(nn::math::MTX44* pProjection, nn::math::MTX34* pTextureProjection) = 0; // slot 0x0C
    virtual res::ResCameraProjectionUpdater GetResource() = 0; // slot 0x10
    virtual const res::ResCameraProjectionUpdater GetResource() const = 0; // slot 0x14
    // the kind of updater: 0 perspective, 1 orthographic, 2 frustum
    virtual u32 GetKind() const = 0; // slot 0x18

    static const TypeInfo s_TypeInfo;
    // the default center of an orthographic camera (name is ours)
    static const nn::math::VEC2 s_DefaultCenter;
    // the defaults of a new resource (names are ours)
    static const f32 s_DefaultNear;
    static const f32 s_DefaultFar;
    static const f32 s_DefaultFovy;
    static const f32 s_DefaultAspectRatio;
    static const f32 s_DefaultHeight;

    nn::math::PivotDirection m_Pivot;   // 0x08 the turn of the screen (0 and 4: none)
    bool m_IsResourceOwner;             // 0x09 the resource is freed with the updater
    nn::math::VEC2 m_TextureScale;      // 0x0C
    nn::math::VEC2 m_TextureTranslate;  // 0x14
};
ASSERT_SIZE(CameraProjectionUpdater, 0x1C);

} // namespace gfx
} // namespace nw
