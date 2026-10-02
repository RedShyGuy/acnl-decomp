#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 20BsMenuMyDesignSelect @ 0x008CCAEC
// vtable 0x008F54F4 (vptr 0x008F54FC), offset_to_top 0, 25 entries
// vtable 0x008F5560 (vptr 0x008F5568), offset_to_top -40, 63 entries
// vtable 0x008F5664 (vptr 0x008F566C), offset_to_top -164, 3 entries
class BsMenuMyDesignSelect : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuMyDesignSelect>
{
public:
    BsMenuMyDesignSelect(); // ctor address unknown
    virtual ~BsMenuMyDesignSelect(); // 0x0031B8EC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0031B8D4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00319E10 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0031AD0C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0031AB58 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00319DB8 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x00319158 slot 0x44 | slot vf_0x44 of MenuBase
};
