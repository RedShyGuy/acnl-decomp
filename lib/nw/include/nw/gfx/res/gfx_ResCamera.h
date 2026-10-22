#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"

namespace nw {
namespace gfx {
namespace res {
// The resources of the view and projection updaters of a camera (CGFX "CCAM"; the layouts are not on
// 3dbrew: only the fields known so far, names are ours). Each starts with its type.

// a view updater: how the view matrix is made
struct ResCameraViewUpdaterData
{
    u32 typeInfo; // 0x0
    u32 flags;    // 0x4
};
ASSERT_SIZE(ResCameraViewUpdaterData, 0x8);

// looks at a target with a twist around the line of sight
struct ResAimTargetViewUpdaterData : public ResCameraViewUpdaterData
{
    enum Type
    {
        TYPE = 0x80000000,
    };
    enum Flag
    {
        FLAG_INHERIT_ROTATION = 0x1,  // the target and the twist turn with the parent
        FLAG_TARGET_RELATIVE = 0x2,   // the target is relative to the camera position
    };

    nn::math::VEC3 targetPosition; // 0x08
    f32 twist;                     // 0x14 radians
};
ASSERT_SIZE(ResAimTargetViewUpdaterData, 0x18);

// looks at a target with an upward vector
struct ResLookAtTargetViewUpdaterData : public ResCameraViewUpdaterData
{
    enum Type
    {
        TYPE = 0x40000000,
    };
    enum Flag
    {
        FLAG_INHERIT_TARGET_ROTATION = 0x1,  // the target turns with the parent
        FLAG_INHERIT_UPWARD_ROTATION = 0x2,  // the upward vector turns with the parent
        FLAG_TARGET_RELATIVE = 0x4,          // the target is relative to the camera position
    };

    nn::math::VEC3 targetPosition; // 0x08
    nn::math::VEC3 upwardVector;   // 0x14
};
ASSERT_SIZE(ResLookAtTargetViewUpdaterData, 0x20);

// turned by angles
struct ResRotateViewUpdaterData : public ResCameraViewUpdaterData
{
    enum Type
    {
        TYPE = 0x20000000,
    };
    enum Flag
    {
        FLAG_INHERIT_ROTATION = 0x1,  // the rotation is relative to the parent
    };

    nn::math::VEC3 viewRotate; // 0x08 radians around X, Y, Z
};
ASSERT_SIZE(ResRotateViewUpdaterData, 0x14);

// a projection updater: how the projection matrix is made
struct ResCameraProjectionUpdaterData
{
    u32 typeInfo; // 0x0
    f32 near;     // 0x4
    f32 far;      // 0x8
};
ASSERT_SIZE(ResCameraProjectionUpdaterData, 0xC);

struct ResPerspectiveProjectionUpdaterData : public ResCameraProjectionUpdaterData
{
    enum Type
    {
        TYPE = 0x20000000,
    };

    f32 aspectRatio; // 0x0C
    f32 fovy;        // 0x10 radians
};
ASSERT_SIZE(ResPerspectiveProjectionUpdaterData, 0x14);

// the view volume of an orthographic or a frustum projection: a rectangle at the near plane
struct ResRectProjectionUpdaterData : public ResCameraProjectionUpdaterData
{
    f32 aspectRatio;        // 0x0C width / height
    f32 height;             // 0x10
    nn::math::VEC2 center;  // 0x14
};
ASSERT_SIZE(ResRectProjectionUpdaterData, 0x1C);

struct ResOrthoProjectionUpdaterData : public ResRectProjectionUpdaterData
{
    enum Type
    {
        TYPE = 0x40000000,
    };
};

struct ResFrustumProjectionUpdaterData : public ResRectProjectionUpdaterData
{
    enum Type
    {
        TYPE = 0x80000000,
    };
};

// A camera (CGFX "CCAM"): a transform node with its view and projection updaters.
struct ResCameraData : public ResTransformNodeData
{
    // the kinds of updaters (names are ours)
    enum ViewType : u8
    {
        VIEW_TYPE_AIM = 0,
        VIEW_TYPE_LOOK_AT = 1,
        VIEW_TYPE_ROTATE = 2,
    };
    enum ProjectionType : u8
    {
        PROJECTION_TYPE_PERSPECTIVE = 0,
        PROJECTION_TYPE_FRUSTUM = 1,
        PROJECTION_TYPE_ORTHO = 2,
    };
    enum Type
    {
        TYPE = 0x4000000A,
    };

    u32 viewType;                  // 0xB4 ViewType
    u32 projectionType;            // 0xB8 ProjectionType
    ut::Offset viewUpdater;        // 0xBC ResCameraViewUpdaterData
    ut::Offset projectionUpdater;  // 0xC0 ResCameraProjectionUpdaterData
    f32 wScale;                    // 0xC4
};
ASSERT_SIZE(ResCameraData, 0xC8);

// the binary resources (the class names are from the reference symbols, except the two bases: ours)
#define NW_GFX_RES_UPDATER_CLASS(name)                                                                      \
    class name                                                                                                     \
    {                                                                                                              \
    public:                                                                                                        \
        explicit name(const void* pData = nullptr) : m_pData(static_cast<const name##Data*>(pData)) {}            \
        const name##Data* ptr() const { return m_pData; }                                                          \
        name##Data* ptr() { return const_cast<name##Data*>(m_pData); }                                             \
        bool IsValid() const { return m_pData != nullptr; }                                                        \
                                                                                                                   \
        const name##Data* m_pData;                                                                                 \
    };                                                                                                             \
    ASSERT_SIZE(name, 0x4)

NW_GFX_RES_UPDATER_CLASS(ResCameraViewUpdater);
NW_GFX_RES_UPDATER_CLASS(ResAimTargetViewUpdater);
NW_GFX_RES_UPDATER_CLASS(ResLookAtTargetViewUpdater);
NW_GFX_RES_UPDATER_CLASS(ResRotateViewUpdater);
NW_GFX_RES_UPDATER_CLASS(ResCameraProjectionUpdater);
NW_GFX_RES_UPDATER_CLASS(ResPerspectiveProjectionUpdater);
NW_GFX_RES_UPDATER_CLASS(ResOrthoProjectionUpdater);
NW_GFX_RES_UPDATER_CLASS(ResFrustumProjectionUpdater);
NW_GFX_RES_UPDATER_CLASS(ResCamera);
#undef NW_GFX_RES_UPDATER_CLASS
} // namespace res
} // namespace gfx
} // namespace nw
