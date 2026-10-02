#pragma once

#include "decomp.h"

class SvPlayerInventory
{
public:
    struct ItemAppearance { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void GetItem(unsigned int, SvPlayerInventory::ItemAppearance*) const; // 0x007250DC | libgarden [tier A]
};
