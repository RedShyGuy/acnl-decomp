#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 13BsStrcCatalog @ 0x008CB97C
// vtable 0x008EF0EC (vptr 0x008EF0F4), offset_to_top 0, 22 entries
class BsStrcCatalog : public ::UtlBase<Base>
{
public:
    BsStrcCatalog(); // ctor candidate(s) 0x00229204 (unverified)
    virtual ~BsStrcCatalog(); // 0x002293A8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00229398 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x0022904C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002278FC slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x00227764 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x00227564 slot 0x44
    bool EndInitialize() override; // 0x002270E0 slot 0x48
    bool StartFinalize() override; // 0x002278E4 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x002278D4 slot 0x50
    bool EndFinalize() override; // 0x002277FC slot 0x54
};
