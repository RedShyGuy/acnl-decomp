#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"
#include "nn/math/math_Vector3.h"

namespace fgobj {
// An item that is thrown onto its unit: it grows to its size on the way, bounces once and lands (the class name is from
// the RTTI, the rest is ours; the Proc builds it, see 0x0059BB68).
// RTTI N5fgobj11ObjectThrowE @ 0x008D29AC
// vtable 0x00908C24 (vptr 0x00908C2C), offset_to_top 0, 10 entries
class ObjectThrow : public ::fgobj::ObjectMove
{
public:
    virtual ~ObjectThrow(); // 0x00594C20 slot 0x00
    // 0x00594BF0 (deleting dtor)
    // Update; outdoors without other players the kinds of thrown items set s_Unk950530 / s_Unk950531
    s32 Execute() override; // 0x00594B58 slot 0x20

    // the flight from from to the position of the object (the target), starting at scale
    void StartThrow(const nn::math::VEC3* from, f32 scale); // 0x00594AA4
    // the flight, the bounce and the shadow; true when it has landed
    bool Update(); // 0x00594900

    f32 m_Scale;        // 0x140 grows to 1
    s16 m_Frame;        // 0x144 the frames until the target (then of the bounce)
    bool m_IsBounced;   // 0x146
};
ASSERT_SIZE(ObjectThrow, 0x148);
} // namespace fgobj
