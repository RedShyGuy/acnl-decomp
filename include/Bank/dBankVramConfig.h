#pragma once

#include "decomp.h"

// The VRAM heaps of the slots of a bank (only a template argument; the class name is from the RTTI of the banks, the
// parameter names are ours: the users create the two VramHeaps of each slot from these values, see bank-progress).
template <u32 VRAM0_SIZE, u32 VRAM0_PARAM, u32 VRAM1_SIZE, u32 VRAM1_PARAM, bool FLAG>
struct BankVramConfig
{
};
