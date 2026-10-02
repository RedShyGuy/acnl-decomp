#pragma once

#include "decomp.h"

namespace netgame {
class PacketQueue
{
public:
    void AddPacketData(void const*, unsigned int); // 0x0062881C | libgarden [tier A]
    void BeginPacket(); // 0x00628A4C | libgarden [tier A]
    void EndPacket(unsigned char, netgame::PlayerNo); // 0x00628AF4 | libgarden [tier A]
};
} // namespace netgame
