#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 17BsCharRoomViewMgr @ 0x008CC468
// vtable 0x008F3438 (vptr 0x008F3440), offset_to_top 0, 22 entries
class BsCharRoomViewMgr : public ::UtlBase<Base>
{
public:
    BsCharRoomViewMgr(); // ctor candidate(s) 0x002C6FAC (unverified)
    virtual ~BsCharRoomViewMgr(); // 0x002C70D8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C70C8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x002C6A84 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002C6898 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x002C5C34 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x002C58EC slot 0x44
    bool EndInitialize() override; // 0x002C56C4 slot 0x48
    bool StartFinalize() override; // 0x002C645C slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x002C6288 slot 0x50
    bool EndFinalize() override; // 0x002C60AC slot 0x54
};
