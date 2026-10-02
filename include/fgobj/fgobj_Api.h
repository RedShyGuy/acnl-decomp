#pragma once

#include "decomp.h"

namespace fgobj {
void GetFgLut(); // 0x00595874 | libgarden [tier A]
void FindPlaceableUnitAroundPlayer(long&, long&, unsigned char&, fgobj::FindUnitForPlayerCheck, bool); // 0x005990AC | libgarden [tier A]
void GetUnitLockHandle(unsigned char, unsigned char, stage::Name); // 0x005A11BC | libgarden [tier A]
void UnlockUnit(unsigned char, unsigned char, stage::Name, PlayerNumber); // 0x005A1278 | libgarden [tier A]
void LockUnit(fgobj::PlaceType, unsigned char, unsigned char, stage::Name, bool); // 0x005A13C4 | libgarden [tier A]
} // namespace fgobj
