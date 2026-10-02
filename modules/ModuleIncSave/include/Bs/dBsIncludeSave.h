#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "script/dIFlowRecept.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// vtable +0x1E094 in ModuleIncSave.cro, offset_to_top 0, 20 entries
// vtable +0x1E0EC in ModuleIncSave.cro, offset_to_top -20, 3 entries
// vtable +0x1E100 in ModuleIncSave.cro, offset_to_top -56, 63 entries
// vtable +0x1E204 in ModuleIncSave.cro, offset_to_top -180, 14 entries
class BsIncludeSave : public ::Base, public ::state::Mode<BsIncludeSave>, public ::script::ITalkRecept, public ::script::IFlowRecept
{
public:
    BsIncludeSave(); // ctor address unknown
    virtual ~BsIncludeSave(); // ModuleIncSave.cro +0x005284 slot 0x00
    virtual void Initialize(); // ModuleIncSave.cro +0x004B70 slot 0x0C
    virtual void Finalize(); // ModuleIncSave.cro +0x004EE8 slot 0x18
    virtual void Calc(); // ModuleIncSave.cro +0x004E58 slot 0x24
    virtual void Draw(); // ModuleIncSave.cro +0x004B68 slot 0x30
    virtual void Unk0(); // ModuleIncSave.cro +0x017CE0 slot 0x3C
};
