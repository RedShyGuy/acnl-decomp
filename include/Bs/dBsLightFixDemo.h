#pragma once

#include "decomp.h"
#include "Bs/dBsLightFixParam.h"

// RTTI 14BsLightFixDemo @ 0x008CBBA4
// vtable 0x008EFD28 (vptr 0x008EFD30), offset_to_top 0, 21 entries
class BsLightFixDemo : public ::BsLightFixParam
{
public:
    BsLightFixDemo(); // ctor candidate(s) 0x007EEA40 (unverified)
    virtual ~BsLightFixDemo(); // 0x00257258 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00257234 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00256D10 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00257214 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00257080 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x48(); // 0x00257204 slot 0x48 | virtual slot, introduced by BsLightBase
};
