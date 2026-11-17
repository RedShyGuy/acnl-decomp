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
    virtual oml::framework::Result Calc(); // 0x00296A48 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x00296A1C slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x00296688 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x00296624 slot 0x44
    bool EndInitialize() override; // 0x00295F84 slot 0x48
    bool StartFinalize() override; // 0x00296A14 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x002969DC slot 0x50
    bool EndFinalize() override; // 0x002969B0 slot 0x54
};
