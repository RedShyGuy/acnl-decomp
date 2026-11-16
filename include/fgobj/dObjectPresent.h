#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace fgobj {
// A present of a balloon that falls from where the balloon was hit onto the ground (then onto its unit) or into the water
// (SE_OBJ_BALLOON_HIT_*; the class name is from the RTTI, the rest is ours).
// RTTI N5fgobj13ObjectPresentE @ 0x008D29DC
// vtable 0x00908CC4 (vptr 0x00908CCC), offset_to_top 0, 10 entries
class ObjectPresent : public ::fgobj::ObjectMove
{
public:
    // m_State
    enum State : u8
    {
        STATE_FALL = 0,                 // onto the ground, then a bounce onto the unit
        STATE_BOUNCE = 1,
        STATE_FALL_TO_NEIGHBOUR = 2,    // the bounce leaves the unit
        STATE_BOUNCE_TO_NEIGHBOUR = 3,
        STATE_FALL_INTO_WATER = 4,
    };

    // the present item for the unit x, z falling from position
    static ObjectPresent* Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* position, u8 state); // 0x005953D4

    virtual ~ObjectPresent(); // 0x00595710 slot 0x00
    // 0x005956E0 (deleting dtor)
    s32 Execute() override; // 0x00595530 slot 0x20

    // the splash effect 582 on the surface of the water with a free effect handle of the Proc
    void StartSplashEffect(); // 0x005952C4

    f32 m_FloorHeight;  // 0x140 the height it falls to (the ground, then the unit)
    s16 m_BounceFrames; // 0x144 the frames of the bounce (8, from high above 24)
    u8 m_State;         // 0x146 State
};
ASSERT_SIZE(ObjectPresent, 0x148);
} // namespace fgobj
