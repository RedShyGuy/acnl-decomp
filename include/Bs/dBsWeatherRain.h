#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 13BsWeatherRain @ 0x008CB988
// vtable 0x008EF14C (vptr 0x008EF154), offset_to_top 0, 22 entries
class BsWeatherRain : public ::UtlBase<Base>
{
public:
    BsWeatherRain(); // ctor candidate(s) 0x0022A5A8 (unverified)
    virtual ~BsWeatherRain(); // 0x0022A7DC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0022A7A8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x0022A420 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x0022A3F4 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x00229C18 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x00229BB4 slot 0x44
    bool EndInitialize() override; // 0x00229568 slot 0x48
    bool StartFinalize() override; // 0x0022A29C slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x0022A264 slot 0x50
    bool EndFinalize() override; // 0x00229FA8 slot 0x54
};
