#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace fgobj {
// An item that is thrown into its unit in 15 frames while it grows to its size (the class name is from the RTTI, the rest
// is ours).
// RTTI N5fgobj15ObjectThrowHoleE @ 0x008D2A18
// vtable 0x00908DB4 (vptr 0x00908DBC), offset_to_top 0, 10 entries
class ObjectThrowHole : public ::fgobj::ObjectMove
{
public:
    // the item for the unit x, z thrown from from, starting at scale
    static ObjectThrowHole* Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* from, f32 scale); // 0x00596B0C

    virtual ~ObjectThrowHole(); // 0x00596D54 slot 0x00
    // 0x00596D24 (deleting dtor)
    s32 Execute() override; // 0x00596C58 slot 0x20

    f32 m_Scale;    // 0x140 grows to 1
    u8 m_Frame;     // 0x144 the frames of the flight that are left
};
ASSERT_SIZE(ObjectThrowHole, 0x148);
} // namespace fgobj
