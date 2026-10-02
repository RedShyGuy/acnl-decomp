#pragma once

#include "decomp.h"
#include "Bank/dBank.h"

// RTTI 14PlayerFaceBank @ 0x008CBD4C
// vtable 0x008F061C (vptr 0x008F0624), offset_to_top 0, 3 entries
class PlayerFaceBank : public ::Bank<40543u, 1u, true, BankVramConfig<0u, 0u, 0u, 0u, false> >
{
public:
    PlayerFaceBank(); // ctor candidate(s) 0x00271410 (unverified)
    virtual void vf_0x00(); // 0x002714F0 slot 0x00 | virtual slot, introduced by Bank<40543u, 1u, true, BankVramConfig<0u, 0u, 0u, 0u, false> >
    virtual void vf_0x04(); // 0x00271458 slot 0x04 | virtual slot, introduced by Bank<40543u, 1u, true, BankVramConfig<0u, 0u, 0u, 0u, false> >
};
