#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 18BsMenuMyDesignKeep @ 0x008CC7A0
// vtable 0x008F434C (vptr 0x008F4354), offset_to_top 0, 25 entries
// vtable 0x008F43B8 (vptr 0x008F43C0), offset_to_top -40, 63 entries
// vtable 0x008F44BC (vptr 0x008F44C4), offset_to_top -164, 3 entries
class BsMenuMyDesignKeep : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuMyDesignKeep>
{
public:
    BsMenuMyDesignKeep(); // ctor address unknown
    virtual ~BsMenuMyDesignKeep(); // 0x002DCED8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002DCEC8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002DC520 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002DCB40 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002DC950 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002DC4CC slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x002DB2D8 slot 0x44 | slot vf_0x44 of MenuBase
};
