#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"

class SvFgName;
namespace math {
class Angle16;
} // namespace math

namespace fgobj {
// A stump that is dug out: after a delay a hole appears at its unit, then it flies off in a direction and fades out
// (the class name is from the RTTI, the rest is ours).
// RTTI N5fgobj11ObjectStumpE @ 0x008D2994
// vtable 0x00908BC4 (vptr 0x00908BCC), offset_to_top 0, 10 entries
class ObjectStump : public ::fgobj::ObjectMove
{
public:
    // the stump of item at the unit x, z that flies off in the direction of angle
    static ObjectStump* Create(const SvFgName& item, const s32* x, const s32* z, const math::Angle16* angle); // 0x00593328

    virtual ~ObjectStump(); // 0x0059347C slot 0x00
    // 0x0059344C (deleting dtor)
    s32 Execute() override; // 0x00593438 slot 0x20

    // the delay with the hole, then the flight; true at the end
    DECOMP_NOINLINE bool Update(); // 0x00593178

    u8 m_Delay;     // 0x140 the frames until the hole appears
    u8 m_FlightFrame; // 0x141 the frames of the flight that are left (the last ones fade out)
};
ASSERT_SIZE(ObjectStump, 0x144);
} // namespace fgobj
