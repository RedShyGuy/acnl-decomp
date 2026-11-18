#pragma once

#include "decomp.h"

// The VRAM heaps of the slots of a bank (only a template argument; the class name is from the RTTI of the banks, the
// parameter names are ours: the users create the two VramHeaps of each slot from these values, see bank-progress).
template <u32 VRAM0_SIZE_, u32 VRAM0_PARAM_, u32 VRAM1_SIZE_, u32 VRAM1_PARAM_, bool FLAG>
struct BankVramConfig
{
    // (the values; CreateHeaps reads them: VRAM heaps of VRAM0_SIZE / VRAM1_SIZE bytes with VRAM0_PARAM / VRAM1_PARAM
    // blocks)
    static const u32 VRAM0_SIZE = VRAM0_SIZE_;
    static const u32 VRAM0_PARAM = VRAM0_PARAM_;
    static const u32 VRAM1_SIZE = VRAM1_SIZE_;
    static const u32 VRAM1_PARAM = VRAM1_PARAM_;
};
