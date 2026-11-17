#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 19BsMyDesignEditModel @ 0x008CC9AC
// vtable 0x008F4EEC (vptr 0x008F4EF4), offset_to_top 0, 22 entries
class BsMyDesignEditModel : public ::UtlBase<Base>
{
public:
    BsMyDesignEditModel(); // ctor candidate(s) 0x002F5B94 (unverified)
    virtual ~BsMyDesignEditModel(); // 0x002F5DF8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002F5CF0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x002F56D4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002F5428 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x002F4DA4 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x002F4764 slot 0x44
    bool EndInitialize() override; // 0x002F3AC4 slot 0x48
    bool StartFinalize() override; // 0x002F5064 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x002F505C slot 0x50
    bool EndFinalize() override; // 0x002F4F18 slot 0x54
};
