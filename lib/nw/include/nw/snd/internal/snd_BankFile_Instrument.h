#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::Instrument
{
public:
    // the key region of a key (NULL: none)
    const KeyRegion* GetKeyRegion(u32 key) const; // 0x00742C84 | nintendogs:bytes [tier A]

    BinaryReference keyRegionTable; // 0x0
};
