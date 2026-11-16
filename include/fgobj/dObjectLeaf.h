#pragma once

#include "decomp.h"
#include "fgobj/dObjectFall.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace fgobj {
// A leaf that sails down to its unit, swaying in x (the class name is from the RTTI, the rest is ours).
// RTTI N5fgobj10ObjectLeafE @ 0x008D2940
// vtable 0x00908AC4 (vptr 0x00908ACC), offset_to_top 0, 10 entries
class ObjectLeaf : public ::fgobj::ObjectFall
{
public:
    // a leaf of item at the unit x, z that starts at offset above it (direction: m_Direction; fallDirection: StartFall)
    static ObjectLeaf* Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* offset, u8 direction, u8 fallDirection); // 0x00592238

    virtual ~ObjectLeaf(); // 0x00592534 slot 0x00
    // 0x00592504 (deleting dtor)
    s32 Execute() override; // 0x00592410 slot 0x20

    u8 m_Unk0x143;                  // 0x143
    nn::math::VEC3 m_PathPosition;  // 0x144 the position without the sway
    u8 m_Frame;                     // 0x150 counts down from LEAF_FRAME_NUM
};
ASSERT_SIZE(ObjectLeaf, 0x154);
} // namespace fgobj
