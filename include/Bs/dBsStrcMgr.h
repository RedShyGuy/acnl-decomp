#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 9BsStrcMgr @ 0x008CD754
// vtable 0x008FA4D0 (vptr 0x008FA4D8), offset_to_top 0, 22 entries
class BsStrcMgr : public ::UtlBase<Base>
{
public:
    BsStrcMgr(); // ctor candidate(s) 0x006E0E74 (unverified)
    virtual ~BsStrcMgr(); // 0x006E12B8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006E12A8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x006E0620 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x006DF8E0 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x006DC4A0 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x006DB884 slot 0x44
    bool EndInitialize() override; // 0x006DB3B0 slot 0x48
    bool StartFinalize() override; // 0x006DD610 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x006DD45C slot 0x50
    bool EndFinalize() override; // 0x006DCD90 slot 0x54
};
