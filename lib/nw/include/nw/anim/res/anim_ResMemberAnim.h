#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResAnimCurve.h"
#include "nw/ut/ut_Offset.h"

namespace nw {
namespace anim {
namespace res {
// the type of the animated value (name is ours)
enum PrimitiveType
{
    PRIMITIVE_FLOAT = 0,
    PRIMITIVE_INT = 1,
    PRIMITIVE_BOOL = 2,
    PRIMITIVE_VECTOR2 = 3,
    PRIMITIVE_VECTOR3 = 4,
    PRIMITIVE_TRANSFORM = 5,
    PRIMITIVE_RGBA_COLOR = 6,
    PRIMITIVE_TEXTURE = 7,
};

// The animation of one member: the curves of its components, each one a constant or an Offset to
// a curve (names are ours). Per component a flag says the value is constant and, for the vectors and
// colors, another one that the component is not animated.
struct ResMemberAnimData
{
    enum Flag
    {
        FLAG_CONSTANT = 1 << 0,       // float, int, bool, texture
        FLAG_CONSTANT_VALUE = 1 << 1, // the value of a constant bool
    };

    u32 flags;          // 0x0
    ut::Offset path;    // 0x4 the path of the animated member
    u32 primitiveType;  // 0x8 PrimitiveType
    union               // 0xC the curve of the first component
    {
        f32 constantValue;
        s32 constantIntValue;
        ut::Offset curve;
    };
};
ASSERT_SIZE(ResMemberAnimData, 0x10);

// a component of a vector or color animation
union ResAnimComponent
{
    f32 constantValue;
    ut::Offset curve;
};

struct ResVec2AnimData
{
    enum Flag
    {
        FLAG_CONSTANT_X = 1 << 0,
        FLAG_CONSTANT_Y = 1 << 1,
        FLAG_NOT_EXIST_X = 1 << 2,
        FLAG_NOT_EXIST_Y = 1 << 3,
    };

    u32 flags;                   // 0x0
    ut::Offset path;             // 0x4
    u32 primitiveType;           // 0x8
    ResAnimComponent components[2]; // 0xC
};

struct ResVec3AnimData
{
    enum Flag
    {
        FLAG_CONSTANT_X = 1 << 0,
        FLAG_NOT_EXIST_X = 1 << 3,
    };

    u32 flags;                   // 0x0
    ut::Offset path;             // 0x4
    u32 primitiveType;           // 0x8
    ResAnimComponent components[3]; // 0xC
};

struct ResRgbaColorAnimData
{
    enum Flag
    {
        FLAG_CONSTANT_R = 1 << 0,
        FLAG_NOT_EXIST_R = 1 << 4,
    };

    u32 flags;                   // 0x0
    ut::Offset path;             // 0x4
    u32 primitiveType;           // 0x8
    ResAnimComponent components[4]; // 0xC
};

struct ResTextureAnimData
{
    u32 flags;               // 0x0
    ut::Offset path;         // 0x4
    u32 primitiveType;       // 0x8
    ResAnimComponent index;  // 0xC the index of the texture (a float curve)
    s32 textureCount;        // 0x10
    ut::Offset textures;     // 0x14 an array of Offsets to the texture references
};

// the binary resource of a member animation (the wrapper of its data)
class ResMemberAnim
{
public:
    // (inline)
    explicit ResMemberAnim(const void* pData = nullptr) : m_pData(static_cast<const ResMemberAnimData*>(pData)) {}
    const ResMemberAnimData* ptr() const { return m_pData; }
    u32 GetPrimitiveType() const { return m_pData->primitiveType; }

    // the size of the value the animation writes
    int GetPrimitiveSize() const;
    // copies the cached value of the animated components to the target
    void ApplyCacheForType(void* pTarget, const void* pCache) const;
    // writes the value at the frame to the target; returns result with the bits of the written
    // components
    u32 EvaluateResultForType(void* pTarget, u32 result, f32 frame, const void* pOriginalValue) const;

protected:
    const ResMemberAnimData* m_pData; // 0x0
};
} // namespace res
} // namespace anim
} // namespace nw
