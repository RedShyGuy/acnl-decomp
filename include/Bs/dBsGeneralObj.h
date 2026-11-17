#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 12BsGeneralObj @ 0x008CB514
// vtable 0x008EDFD8 (vptr 0x008EDFE0), offset_to_top 0, 25 entries
class BsGeneralObj : public ::UtlBase<Base>
{
public:
    BsGeneralObj(); // ctor candidate(s) 0x001F9938 (unverified)
    virtual ~BsGeneralObj(); // 0x001F9AD0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001F9A8C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x001F9434 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x001F9410 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x001F9350 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x001F92E8 slot 0x44
    bool EndInitialize() override; // 0x001F91DC slot 0x48
    bool StartFinalize() override; // 0x001F93FC slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x001F93F4 slot 0x50
    bool EndFinalize() override; // 0x001F93BC slot 0x54
    virtual void vf_0x58(); // 0x001F96E4 slot 0x58 | virtual slot, introduced by BsGeneralObj
    virtual void vf_0x5C(); // 0x001F9628 slot 0x5C | virtual slot, introduced by BsGeneralObj
    virtual void vf_0x60(); // 0x001F957C slot 0x60 | virtual slot, introduced by BsGeneralObj
};
