#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResCamera.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx17CameraViewUpdaterE @ 0x008D06A0
// The part of a camera that makes its view matrix from its resource (member and virtual function
// names are ours).
class CameraViewUpdater : public ::nw::gfx::GfxObject
{
public:
    // (inline)
    explicit CameraViewUpdater(os::IAllocator* pAllocator) : GfxObject(pAllocator) {}
    virtual ~CameraViewUpdater() {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const = 0; // slot 0x08
    // the view matrix of a camera at the position with the world matrix of its parent
    virtual void Update(nn::math::MTX34* pView, const nn::math::MTX34& worldMatrix, const nn::math::VEC3& cameraPosition) = 0; // slot 0x0C
    virtual res::ResCameraViewUpdater GetResource() = 0; // slot 0x10
    virtual const res::ResCameraViewUpdater GetResource() const = 0; // slot 0x14
    // the kind of updater: 0 aim target, 1 look-at target, 2 rotate
    virtual u32 GetKind() const = 0; // slot 0x18

    static const TypeInfo s_TypeInfo;
    // the default target and upward vector of a look-at camera (names are ours)
    static const nn::math::VEC3 s_DefaultTargetPosition;
    static const nn::math::VEC3 s_DefaultUpwardVector;
};
} // namespace gfx
} // namespace nw
