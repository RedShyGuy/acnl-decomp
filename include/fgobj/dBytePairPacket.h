#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"

namespace fgobj {
// two bytes that a console sends to the host (packet type 0x48), which keeps the last three pairs in its town data
// (Func52B188); the console code that sends it is not known (it is reached through a veneer, maybe from a module; all
// names are ours)
struct BytePairPacket {
    // (inline; a copy for the packet handler)
    DECOMP_EMIT_COPY BytePairPacket() : a(0), b(0) {} // 0x0059591C

    void Set(u8 a, u8 b); // 0x00595910
    void Send() const; // 0x005958FC
    void Process() const; // 0x005958D4

    static const u8 PACKET_TYPE = 0x48;

    /* 0x0 */ u8 a;
    /* 0x1 */ u8 b;
};
ASSERT_SIZE(BytePairPacket, 0x2);
} // namespace fgobj
