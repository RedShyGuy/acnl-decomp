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
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0022A420 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0022A3F4 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00229C18 slot 0x40 | virtual slot, introduced by BsWeatherRain
    virtual void vf_0x44(); // 0x00229BB4 slot 0x44 | virtual slot, introduced by BsWeatherRain
    virtual void vf_0x48(); // 0x00229568 slot 0x48 | virtual slot, introduced by BsWeatherRain
    virtual void vf_0x4C(); // 0x0022A29C slot 0x4C | virtual slot, introduced by BsWeatherRain
    virtual void vf_0x50(); // 0x0022A264 slot 0x50 | virtual slot, introduced by BsWeatherRain
    virtual void vf_0x54(); // 0x00229FA8 slot 0x54 | virtual slot, introduced by BsWeatherRain
};
