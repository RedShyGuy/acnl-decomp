#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "script/dIFlowRecept.h"
#include "script/dITalkRecept.h"

// vtable +0x20E08 in ModuleVillage.cro, offset_to_top 0, 22 entries
// vtable +0x20E68 in ModuleVillage.cro, offset_to_top -20, 63 entries
// vtable +0x20F6C in ModuleVillage.cro, offset_to_top -144, 14 entries
class BsTitle : public ::Base, public ::script::ITalkRecept, public ::script::IFlowRecept
{
public:
    class PNameRecept;
    BsTitle(); // ctor address unknown
    virtual ~BsTitle(); // ModuleVillage.cro +0x00D360 slot 0x00
    virtual void Initialize(); // ModuleVillage.cro +0x018228 slot 0x0C
    virtual void Finalize(); // ModuleVillage.cro +0x018334 slot 0x18
    virtual void Calc(); // ModuleVillage.cro +0x018324 slot 0x24
    virtual void Draw(); // ModuleVillage.cro +0x01818C slot 0x30
};
