#pragma once

#include "decomp.h"

// Game functions and objects that BsPlayerMgr uses (names after the address while the purpose is unknown).

// (every 512 frames, BsPlayerMgr::Calc)
void Func5C78E8(); // 0x005C78E8
// (each frame with the object 0x00AAFD58, BsPlayerMgr::Calc)
void Func5C99E4(void* object); // 0x005C99E4
// clears two floats of the object 0x00AAFF90 (BsPlayerMgr::StartInitialize)
void Func5C72A8(); // 0x005C72A8
// the four bytes netgame::g_UnknownA83000 (set to 0xFF on the island stages)
u8* Func5C1A20(); // 0x005C1A20
// (after the items of the tour are given)
void Func583528(); // 0x00583528

// (type unknown)
extern u8 g_UnknownAAFD58[];
