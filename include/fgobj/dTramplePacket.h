#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"
#include "Sv/dSvFgName.h"

namespace fgobj {
// a flower that was trampled at a unit, also on the other consoles (packet type 0x47; layout and the names of the type,
// Process and Send after libgarden fgobj/trample.hpp)
struct TramplePacket {
    // (inline; copies for the packet handler)
    DECOMP_EMIT_COPY TramplePacket() : stage(stage::NAME_NONE), x(0), z(0) {} // 0x00594FEC
    DECOMP_EMIT_COPY ~TramplePacket() {} // 0x0059500C

    // the flower at the unit is removed when it is still replace (not on the host while the unit is locked); on the
    // current stage the unit is made new with the effect of the trampled flower
    void Process(); // 0x00594EE0
    void Send() const
    {
        netgame::EnqueuePacket(PACKET_TYPE, netgame::PLAYER_NO_TARGET_ALL, this, sizeof(*this));
    }

    static const u8 PACKET_TYPE = 0x47;

    /* 0x0 */ SvFgName replace;
    /* 0x4 */ stage::Name stage;
    /* 0x5 */ u8 x;
    /* 0x6 */ u8 z;
    /* 0x7 */ u8 padding;
};
ASSERT_SIZE(TramplePacket, 0x8);

// the player moves over the unit x, z of the field: a flower there may be trampled (12.5 %; only when changedUnit and
// isUnknown2 are set, not with isUnknown1 alone; meaning of the flags after the caller: the player entered the unit /
// two comparisons of its speed; name after the packet)
void Trample(s32 x, s32 z, bool changedUnit, bool isUnknown1, bool isUnknown2); // 0x00597F04
} // namespace fgobj
