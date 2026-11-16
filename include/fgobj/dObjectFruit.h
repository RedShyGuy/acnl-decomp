#pragma once

#include "decomp.h"
#include "fgobj/dObjectFall.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace fgobj {
// An item that falls out of a tree (fruit and others), bounces once and lands (the class name is from the RTTI, the
// rest is ours).
// RTTI N5fgobj11ObjectFruitE @ 0x008D2964
// vtable 0x00908B04 (vptr 0x00908B0C), offset_to_top 0, 10 entries
class ObjectFruit : public ::fgobj::ObjectFall
{
public:
    // an item at the unit x, z that falls from offset above it (angle in degrees turns it about z; direction:
    // m_Direction; isMirrored: the model is mirrored in x)
    static ObjectFruit* Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* offset, f32 angle, u8 direction, bool isMirrored); // 0x00592664

    virtual ~ObjectFruit(); // 0x00592A44 slot 0x00
    // 0x00592A14 (deleting dtor)
    s32 Execute() override; // 0x00592860 slot 0x20

    bool m_IsFallSoundRequested;    // 0x143 the sound of the fall in the next frame
};
ASSERT_SIZE(ObjectFruit, 0x144);
} // namespace fgobj
