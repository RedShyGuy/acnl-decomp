#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 13BsWeatherSnow @ 0x008CB994
// vtable 0x008EF1AC (vptr 0x008EF1B4), offset_to_top 0, 22 entries
class BsWeatherSnow : public ::UtlBase<Base>
{
public:
    BsWeatherSnow(); // ctor candidate(s) 0x0022BB5C (unverified)
    virtual ~BsWeatherSnow(); // 0x0022BD5C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0022BD28 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x0022B6B0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x0022B684 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x0022AF5C slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x0022AEF8 slot 0x44
    bool EndInitialize() override; // 0x0022A844 slot 0x48
    bool StartFinalize() override; // 0x0022B52C slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x0022B4F4 slot 0x50
    bool EndFinalize() override; // 0x0022B2D8 slot 0x54
};
