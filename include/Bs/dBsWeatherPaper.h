#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 14BsWeatherPaper @ 0x008CBC68
// vtable 0x008F01A8 (vptr 0x008F01B0), offset_to_top 0, 22 entries
class BsWeatherPaper : public ::UtlBase<Base>
{
public:
    BsWeatherPaper(); // ctor candidate(s) 0x00267510 (unverified)
    virtual ~BsWeatherPaper(); // 0x00267714 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002676E0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x00266CC8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x00266C9C slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x002669A8 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x00266944 slot 0x44
    bool EndInitialize() override; // 0x0026634C slot 0x48
    bool StartFinalize() override; // 0x00266C94 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x00266C74 slot 0x50
    bool EndFinalize() override; // 0x00266C48 slot 0x54
};
