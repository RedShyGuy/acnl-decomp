#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"
#include "nn/math/math_Transform.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/res/gfx_ResCamera.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
class AnimGroup;

// RTTI N2nw3gfx6CameraE @ 0x008D07F0
// vtable 0x00902A50 (vptr 0x00902A58), offset_to_top 0, 11 entries
// A camera: a transform node with a view and a projection updater, which make its matrices from its
// world position (member names are ours).
class Camera : public ::nw::gfx::TransformNode
{
public:
    class DynamicBuilder;
    // (the same fields as the one of TransformNode; name from the reference symbols)
    struct Description : public TransformNode::Description
    {
    };

    Camera(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description,
           GfxPtr<CameraViewUpdater> viewUpdater, GfxPtr<CameraProjectionUpdater> projectionUpdater, f32 wScale,
           bool isDynamic); // 0x004AE5B0 | nintendogs:bytes [tier A]
    virtual ~Camera();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C838 slot 0x08 | nintendogs:bytes
    virtual void Accept(ISceneVisitor* pVisitor); // 0x004AE144 slot 0x10 | nintendogs:bytes
    // the original values and the animation group
    virtual u32 Initialize(os::IAllocator* pAllocator); // 0x004ACF98 slot 0x24 | nintendogs:bytes

    // a copy of the resource and its updaters, the values that animations restore
    u32 StoreOriginal(os::IAllocator* pAllocator); // 0x004ACFE0 | nintendogs:bytes [tier A]
    u32 CreateAnimGroup(os::IAllocator* pAllocator); // 0x004AD684 | nintendogs:bytes [tier A]
    // a resource copied by StoreOriginal (or made by the DynamicBuilder) freed with its updaters
    static void DestroyResCamera(os::IAllocator* pAllocator, res::ResCamera resCamera); // 0x004AD944 | nintendogs:callgraph [tier A]
    // the view and projection matrices and their inverses
    void UpdateCameraMatrix(); // 0x004ADA80 | nintendogs:bytes [tier A]
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResCamera resCamera, Description description); // 0x004ADEEC | nintendogs:bytes [tier A]
    static Camera* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator); // 0x004AE198 | nintendogs:callseq [tier A]

    // (inline)
    DECOMP_NOINLINE f32 GetFar() const // 0x0073C844 | nintendogs:bytes [tier A]
    {
        return static_cast<const CameraProjectionUpdater*>(m_ProjectionUpdater.Get())->GetResource().ptr()->far;
    }
    DECOMP_NOINLINE f32 GetNear() const // 0x0073C860 | nintendogs:bytes [tier A]
    {
        return static_cast<const CameraProjectionUpdater*>(m_ProjectionUpdater.Get())->GetResource().ptr()->near;
    }
    // the upward vector of a look-at view (name is ours)
    void SetUpwardVector(const nn::math::VEC3& upwardVector); // 0x004AD908 (name is ours)
    // the target of an aim or a look-at view (name is ours)
    void SetTargetPosition(const nn::math::VEC3& targetPosition); // 0x004AD9F4 (name is ours)

    static const TypeInfo s_TypeInfo;

    nn::math::MTX34 m_ViewMatrix;                       // 0x148
    nn::math::MTX34 m_InverseViewMatrix;                // 0x178
    nn::math::MTX44 m_ProjectionMatrix;                 // 0x1A8
    nn::math::MTX44 m_InverseProjectionMatrix;          // 0x1E8
    nn::math::MTX34 m_TextureProjectionMatrix;          // 0x228
    GfxPtr<CameraViewUpdater> m_ViewUpdater;            // 0x258
    GfxPtr<CameraProjectionUpdater> m_ProjectionUpdater; // 0x260
    AnimGroup* m_pAnimGroup;                            // 0x268
    res::ResCameraData* m_pOriginalResource;            // 0x26C the copy of StoreOriginal
    nn::math::Transform3 m_OriginalTransform;           // 0x270
    f32 m_WScale;                                       // 0x294
    bool m_IsDynamic;                                   // 0x298 the resource belongs to the camera (DynamicBuilder)
};
ASSERT_OFFSET(Camera, m_ViewMatrix, 0x148);
ASSERT_SIZE(Camera, 0x29C);
} // namespace gfx
} // namespace nw
