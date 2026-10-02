#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 14BsIndoorDoorSE @ 0x008CBB80
// vtable 0x008EFC14 (vptr 0x008EFC1C), offset_to_top 0, 22 entries
class BsIndoorDoorSE : public ::UtlBase<Base>
{
public:
    BsIndoorDoorSE(); // ctor candidate(s) 0x002565D8 (unverified)
    virtual ~BsIndoorDoorSE(); // 0x00256678 slot 0x00 | mk7dlp:bytes
    // 0x00256654 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00256464 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x40(); // 0x00256330 slot 0x40 | virtual slot, introduced by BsIndoorDoorSE
    virtual void vf_0x44(); // 0x00256328 slot 0x44 | virtual slot, introduced by BsIndoorDoorSE
    virtual void vf_0x48(); // 0x00256314 slot 0x48 | virtual slot, introduced by BsIndoorDoorSE
    virtual void vf_0x4C(); // 0x00256394 slot 0x4C | virtual slot, introduced by BsIndoorDoorSE
    virtual void vf_0x50(); // 0x0025638C slot 0x50 | virtual slot, introduced by BsIndoorDoorSE
    virtual void vf_0x54(); // 0x00256384 slot 0x54 | virtual slot, introduced by BsIndoorDoorSE
};
