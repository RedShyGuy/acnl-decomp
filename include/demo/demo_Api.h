#pragma once

#include "decomp.h"

namespace demo {
void CheckOrder(demo::OrderID, bool); // 0x00316DA4 | libgarden [tier A]
void MakeOrderPlayerThink(unsigned char); // 0x0052C6C8 | libgarden [tier A]
void RevokeNetgameLockOrder(); // 0x0052CC00 | libgarden [tier A]
void MakeOrderNetgameLock(); // 0x0052CD74 | libgarden [tier A]
void MakeOrderIslandForceLeaveThink(); // 0x0052D020 | libgarden [tier A]
} // namespace demo
