#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0xC8D0 in ModuleStation.cro, offset_to_top 0, 25 entries
// vtable +0xC93C in ModuleStation.cro, offset_to_top -40, 3 entries
class BsTripList : public ::MenuBase, public ::state::Mode<BsTripList>
{
public:
    BsTripList(); // ctor address unknown
    virtual ~BsTripList(); // ModuleStation.cro +0x006BAC slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleStation.cro +0x001FD0 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleStation.cro +0x002638 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleStation.cro +0x002414 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleStation.cro +0x001F90 slot 0x30
    virtual bool Unk0(); // ModuleStation.cro +0x00A498 slot 0x3C
    virtual void FUN_006ab3d8(); // ModuleStation.cro +0x009550 slot 0x40
    virtual void OnClose(); // ModuleStation.cro +0x00955C slot 0x44
    virtual void FUN_006ab3f4(bool value); // ModuleStation.cro +0x009568 slot 0x48
    virtual bool FUN_006ab450(); // ModuleStation.cro +0x009580 slot 0x4C
    virtual void FUN_006ab3e0(bool value); // ModuleStation.cro +0x009554 slot 0x50
    virtual bool FUN_006ab440(); // ModuleStation.cro +0x009578 slot 0x54
    virtual void FUN_006ab3fc(bool value); // ModuleStation.cro +0x009570 slot 0x58
    virtual bool FUN_006ab3ec(); // ModuleStation.cro +0x009560 slot 0x5C
    virtual bool FUN_00767b30(); // ModuleStation.cro +0x00A4EC slot 0x60
};
