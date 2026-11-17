#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 6BsFlag @ 0x008CD2D8
// vtable 0x008F8BC8 (vptr 0x008F8BD0), offset_to_top 0, 22 entries
class BsFlag : public ::UtlBase<Base>
{
public:
    BsFlag(); // ctor address unknown
    virtual ~BsFlag(); // 0x005BFEC0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x005BFE68 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x005BFBEC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x005BFBD8 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x005BF89C slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x005BF7C8 slot 0x44
    bool EndInitialize() override; // 0x005BF508 slot 0x48
    bool StartFinalize() override; // 0x005BF9FC slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x005BF9F4 slot 0x50
    bool EndFinalize() override; // 0x005BF9B0 slot 0x54
};
