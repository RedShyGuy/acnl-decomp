#pragma once

#include "decomp.h"
#include "g3d/dTransformNode.h"
#include "nw/gfx/res/gfx_ResCamera.h"
#include "sead/seadPerspectiveProjection.h"

namespace nw {
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

namespace sead {
class CameraProjectionUpdaterNw4c;
} // namespace sead

namespace g3d {
// a camera of the scene: looks at a target, with a perspective projection of sead (names are ours)
// RTTI N3g3d6CameraE @ 0x008D0D78
// vtable 0x00903BDC (vptr 0x00903BE4), offset_to_top 0, 4 entries
class Camera : public ::g3d::TransformNode
{
public:
    // the projection of Initialize
    struct PerspectiveParam
    {
        f32 near;        // 0x0
        f32 far;         // 0x4
        f32 fovyDegree;  // 0x8
        f32 aspect;      // 0xC
    };

    Camera(); // 0x004F2820
    virtual ~Camera(); // 0x004F2874 slot 0x00
    // 0x004F2850 (deleting dtor)
    // the world matrix and the camera matrix
    void Update() override; // 0x004F27C4 slot 0x08

    // the camera with a look-at view at target (upward y) and the projection in the memory of allocator (NULL: the
    // one of the engine); false if it could not be made
    bool Initialize(const nn::math::VEC3& target, const PerspectiveParam& param, nw::os::IAllocator* allocator); // 0x004F2558
    // the target and the upward vector: y turned about x, then y, then x again
    void SetTarget(const nn::math::VEC3& target, s16 upRotateX, s16 upRotateY, s16 upRotateX2); // 0x004F24CC

    nw::gfx::res::ResLookAtTargetViewUpdaterData* m_pViewData;     // 0x08 the resource of the view updater
    sead::CameraProjectionUpdaterNw4c* m_pProjectionUpdater;       // 0x0C
    sead::PerspectiveProjection m_Projection;                      // 0x10
};
} // namespace g3d
