#pragma once

#include "decomp.h"
#include "Other/dICameraUpdater.h"

// RTTI 18TotakekeLiveCamera @ 0x008CC91C
// vtable 0x008F4BE8 (vptr 0x008F4BF0), offset_to_top 0, 1 entries
class TotakekeLiveCamera : public ::ICameraUpdater
{
public:
    TotakekeLiveCamera(); // ctor candidate(s) 0x002ECBB8, 0x002ECC2C (unverified)
    virtual void vf_0x00(); // 0x002ECA50 slot 0x00 | virtual slot, introduced by TotakekeLiveCamera
};
