#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::KeyRegion
{
public:
    // the velocity region of a velocity (NULL: none)
    const VelocityRegion* GetVelocityRegion(u32 velocity) const; // 0x00742F4C | nintendogs:bytes [tier A]

    BinaryReference velocityRegionTable; // 0x0
};
