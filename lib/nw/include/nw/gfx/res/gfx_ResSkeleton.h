#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "nw/ut/ut_Offset.h"

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
    };

    ut::Offset name;          // 0x00
    u32 flags;                // 0x04
    s32 jointId;              // 0x08
    s32 parentId;             // 0x0C
    ut::Offset parent;        // 0x10
    u8 unknown0x14[0xC];      // 0x14
    nn::math::VEC3_ scale;    // 0x20
    nn::math::VEC3_ rotate;   // 0x2C angles in radians
    nn::math::VEC3_ translate; // 0x38
};
ASSERT_OFFSET(ResBoneData, scale, 0x20);
ASSERT_OFFSET(ResBoneData, translate, 0x38);

// the wrapper of a bone (passed by value like all resources)
class ResBone
{
public:
    // how the bone faces the camera (the values are filled in with BillboardUpdater)
    enum BillboardMode : u8;

    // (inline)
    explicit ResBone(const void* pData = nullptr) : m_pData(static_cast<const ResBoneData*>(pData)) {}
    const ResBoneData* ptr() const { return m_pData; }

private:
    const ResBoneData* m_pData; // 0x0
};
} // namespace res
} // namespace gfx
} // namespace nw
