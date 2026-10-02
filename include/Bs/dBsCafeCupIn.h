#pragma once

#include "decomp.h"
#include "Bs/dBsGeneralObj.h"

// RTTI 11BsCafeCupIn @ 0x008CB240
// vtable 0x008EC7B8 (vptr 0x008EC7C0), offset_to_top 0, 25 entries
class BsCafeCupIn : public ::BsGeneralObj
{
public:
    BsCafeCupIn(); // ctor candidate(s) 0x001C28A0 (unverified)
    virtual ~BsCafeCupIn(); // 0x001C2918 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001C28C0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x48(); // 0x001C268C slot 0x48 | virtual slot, introduced by BsGeneralObj
    virtual void vf_0x4C(); // 0x001C283C slot 0x4C | virtual slot, introduced by BsGeneralObj
};
