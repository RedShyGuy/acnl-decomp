#pragma once

#include "decomp.h"

namespace netgame {
class LockMgr
{
public:
    void HandleRequestResult(netgame::PlayerNo, bool); // 0x0061AFA8 | libgarden [tier A]
};
} // namespace netgame
