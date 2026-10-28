#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Transform.h"
#include "nn/math/math_Vector3.h"
#include "nw/ut/ut_Offset.h"
#include "nw/ut/ut_ResDicPatricia.h"

namespace nw {
namespace gfx {
namespace res {
// A bone of a skeleton (3dbrew "CGFX": the skeleton entries; class names from the reference
// symbols, member names ours; the fields between are filled in where code uses them).
struct ResBoneData
{
    enum Flag
    {
        FLAG_IS_IDENTITY = 1 << 0,
        FLAG_IS_TRANSLATE_ZERO = 1 << 1,
        FLAG_IS_ROTATE_ZERO = 1 << 2,
        FLAG_IS_SCALE_ONE = 1 << 3,
        FLAG_IS_UNIFORM_SCALE = 1 << 4,
        // the scale of the parent does not shrink the bone (WorldMatrixUpdater::CalculateWorldMayaSsc)
        FLAG_SEGMENT_SCALE_COMPENSATE = 1 << 5,
        // the bone has a skinning matrix (the world matrix times the inverse bind pose; name is ours)
        FLAG_HAS_SKINNING_MATRIX = 1 << 9,
        // (set together with FLAG_HAS_SKINNING_MATRIX on the bones whose skinning matrix is used)
        FLAG_UNKNOWN_0x40 = 1 << 6,
    };

    ut::Offset name;          // 0x00
    u32 flags;                // 0x04
    s32 jointId;              // 0x08
    s32 parentId;             // 0x0C
    ut::Offset parent;        // 0x10
    u8 unknown0x14[0xC];      // 0x14
    nn::math::Transform3 transform; // 0x20 scale, rotate (angles in radians), translate
    nn::math::MTX34 localMatrix;        // 0x44
    nn::math::MTX34 worldMatrix;        // 0x74
    nn::math::MTX34 inverseBaseMatrix;  // 0xA4 the inverse of the bind pose
    u32 billboardMode;                  // 0xD4 ResBone::BillboardMode (low byte)
};
ASSERT_OFFSET(ResBoneData, transform, 0x20);
ASSERT_OFFSET(ResBoneData, billboardMode, 0xD4);

// the wrapper of a bone (passed by value like all resources)
class ResBone
{
public:
    // how the bone faces the camera (names are ours, after BillboardUpdater::Update)
    enum BillboardMode : u8
    {
        BILLBOARD_MODE_OFF = 0,
        BILLBOARD_MODE_WORLD = 1,             // z along the z axis of the camera
        BILLBOARD_MODE_WORLD_VIEWPOINT = 2,   // z towards the camera position
        BILLBOARD_MODE_SCREEN = 3,            // parallel to the screen, the up vector kept in view space
        BILLBOARD_MODE_SCREEN_VIEWPOINT = 4,  // towards the camera in view space
        BILLBOARD_MODE_Y_AXIAL = 5,           // turns about its y axis only
        BILLBOARD_MODE_Y_AXIAL_VIEWPOINT = 6,
    };

    // (inline)
    explicit ResBone(const void* pData = nullptr) : m_pData(static_cast<const ResBoneData*>(pData)) {}
    const ResBoneData* ptr() const { return m_pData; }

private:
    const ResBoneData* m_pData; // 0x0
};
// A skeleton: its bones by name and how the world matrices are calculated (only the fields known so
// far; names are ours).
struct ResSkeletonData
{
    // how a bone inherits the scale of its parent (the three WorldMatrixUpdater::CalculateWorld*)
    enum ScalingRule : u8
    {
        SCALING_RULE_STANDARD = 0, // CalculateWorldBasic
        SCALING_RULE_MAYA = 1,     // CalculateWorldMayaSsc
        SCALING_RULE_SOFTIMAGE = 2, // CalculateWorldXsi
    };
    enum Flag
    {
        FLAG_IS_MODEL_COORDINATE = 1 << 0,
    };

    u32 m_Unknown0x0[6];   // 0x00
    s32 boneCount;         // 0x18
    ut::Offset boneDic;    // 0x1C ResDicPatricia of the ResBoneData
    u32 m_Unknown0x20;     // 0x20
    u32 scalingRule;       // 0x24 ScalingRule (low byte)
    u32 flags;             // 0x28 Flag
};
ASSERT_OFFSET(ResSkeletonData, flags, 0x28);

// the wrapper of a skeleton (the class name is from the reference symbols)
class ResSkeleton
{
public:
    // (inline)
    explicit ResSkeleton(const void* pData = nullptr) : m_pData(static_cast<const ResSkeletonData*>(pData)) {}
    const ResSkeletonData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }
    s32 GetBoneCount() const { return m_pData->boneCount; }
    ResBone GetBone(int index) const
    {
        ut::ResDicPatricia dic = {m_pData->boneDic.To<ut::ResDicPatricia::Data>()};
        return ResBone(dic.GetDataByIndex(index));
    }
    // the local transform of the bone at index (the game reads it; name is ours)
    DECOMP_EMIT_COPY const nn::math::Transform3& GetBoneTransform(int index) const // 0x0073C924 (name is ours)
    {
        return GetBone(index).ptr()->transform;
    }

    const ResSkeletonData* m_pData; // 0x0
};
ASSERT_SIZE(ResSkeleton, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
