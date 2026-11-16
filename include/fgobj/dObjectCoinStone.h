#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// A rock that is hit (PLACE_TYPE_HIT_COIN_STONE): it shakes once to and fro in a direction (the class name is from the
// RTTI, the rest is ours; the Proc builds it, see 0x0059D888).
// RTTI N5fgobj15ObjectCoinStoneE @ 0x008D29E8
// vtable 0x00908CF4 (vptr 0x00908CFC), offset_to_top 0, 10 entries
class ObjectCoinStone : public ::fgobj::ObjectExe
{
public:
    virtual ~ObjectCoinStone(); // 0x005959F8 slot 0x00
    // 0x005959C8 (deleting dtor)
    s32 Execute() override; // 0x00595930 slot 0x20

    u16 m_Direction = 0;    // 0x124 the direction of the shake (a 16-bit angle)
    s16 m_Phase = 0;        // 0x126 half a sine wave in steps of 0x1000
};
ASSERT_SIZE(ObjectCoinStone, 0x128);
} // namespace fgobj
