#pragma once

#include "decomp.h"
#include "Bs/dBsLightAmbientFix.h"

// RTTI 21BsLightAmbientFixDemo @ 0x008CCC5C
// vtable 0x008F5F34 (vptr 0x008F5F3C), offset_to_top 0, 20 entries
class BsLightAmbientFixDemo : public ::BsLightAmbientFix
{
public:
    BsLightAmbientFixDemo(); // ctor candidate(s) 0x007F221C (unverified)
    virtual ~BsLightAmbientFixDemo(); // 0x00325900 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x003258DC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00325474 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x003258BC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00325740 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x48(); // 0x003258AC slot 0x48 | virtual slot, introduced by BsLightBase
};
