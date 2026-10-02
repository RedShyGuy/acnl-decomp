#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 15BsWeatherSakura @ 0x008CC000
// vtable 0x008F17F0 (vptr 0x008F17F8), offset_to_top 0, 22 entries
class BsWeatherSakura : public ::UtlBase<Base>
{
public:
    BsWeatherSakura(); // ctor candidate(s) 0x00297790 (unverified)
    virtual ~BsWeatherSakura(); // 0x002979BC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00297988 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00296A48 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00296A1C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00296688 slot 0x40 | virtual slot, introduced by BsWeatherSakura
    virtual void vf_0x44(); // 0x00296624 slot 0x44 | virtual slot, introduced by BsWeatherSakura
    virtual void vf_0x48(); // 0x00295F84 slot 0x48 | virtual slot, introduced by BsWeatherSakura
    virtual void vf_0x4C(); // 0x00296A14 slot 0x4C | virtual slot, introduced by BsWeatherSakura
    virtual void vf_0x50(); // 0x002969DC slot 0x50 | virtual slot, introduced by BsWeatherSakura
    virtual void vf_0x54(); // 0x002969B0 slot 0x54 | virtual slot, introduced by BsWeatherSakura
};
