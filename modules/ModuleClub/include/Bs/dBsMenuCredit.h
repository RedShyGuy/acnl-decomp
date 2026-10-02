#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x13E80 in ModuleClub.cro, offset_to_top 0, 25 entries
// vtable +0x13EEC in ModuleClub.cro, offset_to_top -40, 3 entries
class BsMenuCredit : public ::MenuBase, public ::state::Mode<BsMenuCredit>
{
public:
    BsMenuCredit(); // ctor address unknown
    virtual ~BsMenuCredit(); // ModuleClub.cro +0x004650 slot 0x00
    virtual void Initialize(); // ModuleClub.cro +0x003CD4 slot 0x0C
    virtual void Finalize(); // ModuleClub.cro +0x00438C slot 0x18
    virtual void Calc(); // ModuleClub.cro +0x003FC0 slot 0x24
    virtual void Draw(); // ModuleClub.cro +0x003C6C slot 0x30
    virtual void FUN_006ab3d8(); // ModuleClub.cro +0x010C64 slot 0x40
    virtual void OnClose(); // ModuleClub.cro +0x010C70 slot 0x44
    virtual void FUN_006ab3f4(); // ModuleClub.cro +0x010C7C slot 0x48
    virtual void FUN_006ab450(); // ModuleClub.cro +0x010C94 slot 0x4C
    virtual void FUN_006ab3e0(); // ModuleClub.cro +0x010C68 slot 0x50
    virtual void FUN_006ab440(); // ModuleClub.cro +0x010C8C slot 0x54
    virtual void FUN_006ab3fc(); // ModuleClub.cro +0x010C84 slot 0x58
    virtual void FUN_006ab3ec(); // ModuleClub.cro +0x010C74 slot 0x5C
    virtual void FUN_00767b30(); // ModuleClub.cro +0x0110EC slot 0x60
};
