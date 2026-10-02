#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x20C34 in ModuleVillage.cro, offset_to_top 0, 25 entries
// vtable +0x20CA0 in ModuleVillage.cro, offset_to_top -40, 3 entries
class BsMenuTicketExchange : public ::MenuBase, public ::state::Mode<BsMenuTicketExchange>
{
public:
    BsMenuTicketExchange(); // ctor address unknown
    virtual ~BsMenuTicketExchange(); // ModuleVillage.cro +0x003E24 slot 0x00
    virtual void Initialize(); // ModuleVillage.cro +0x0037D8 slot 0x0C
    virtual void Finalize(); // ModuleVillage.cro +0x003B0C slot 0x18
    virtual void Calc(); // ModuleVillage.cro +0x0039F0 slot 0x24
    virtual void Draw(); // ModuleVillage.cro +0x003768 slot 0x30
    virtual void Unk0(); // ModuleVillage.cro +0x01BBC4 slot 0x3C
    virtual void FUN_006ab3d8(); // ModuleVillage.cro +0x019524 slot 0x40
    virtual void OnClose(); // ModuleVillage.cro +0x019530 slot 0x44
    virtual void FUN_006ab3f4(); // ModuleVillage.cro +0x01953C slot 0x48
    virtual void FUN_006ab450(); // ModuleVillage.cro +0x019554 slot 0x4C
    virtual void FUN_006ab3e0(); // ModuleVillage.cro +0x019528 slot 0x50
    virtual void FUN_006ab440(); // ModuleVillage.cro +0x01954C slot 0x54
    virtual void FUN_006ab3fc(); // ModuleVillage.cro +0x019544 slot 0x58
    virtual void FUN_006ab3ec(); // ModuleVillage.cro +0x019534 slot 0x5C
    virtual void FUN_00767b30(); // ModuleVillage.cro +0x01BBFC slot 0x60
};
