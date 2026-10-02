#pragma once

#include "decomp.h"

namespace netgame {
// RTTI N7netgame15MachineBitTableE @ 0x008D3F68
class MachineBitTable
{
public:
    MachineBitTable(); // ctor address unknown
    void Add(netgame::PlayerNo); // 0x0062557C | libgarden [tier A]
    void Remove(netgame::PlayerNo); // 0x006255FC | libgarden [tier A]
    void Reset(); // 0x00625740 | libgarden [tier A]
};
} // namespace netgame
