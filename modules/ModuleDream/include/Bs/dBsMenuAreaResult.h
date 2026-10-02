#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x13714 in ModuleDream.cro, offset_to_top 0, 25 entries
// vtable +0x13780 in ModuleDream.cro, offset_to_top -40, 3 entries
class BsMenuAreaResult : public ::MenuBase, public ::state::Mode<BsMenuAreaResult>
{
public:
    BsMenuAreaResult(); // ctor address unknown
    virtual ~BsMenuAreaResult(); // ModuleDream.cro +0x004E40 slot 0x00
    virtual void Initialize(); // ModuleDream.cro +0x00758C slot 0x0C
    virtual void Finalize(); // ModuleDream.cro +0x0079D0 slot 0x18
    virtual void Calc(); // ModuleDream.cro +0x00789C slot 0x24
    virtual void Draw(); // ModuleDream.cro +0x007550 slot 0x30
    virtual void Unk0(); // ModuleDream.cro +0x00A110 slot 0x3C
    virtual void FUN_006ab3d8(); // ModuleDream.cro +0x00A058 slot 0x40
    virtual void OnClose(); // ModuleDream.cro +0x00A064 slot 0x44
    virtual void FUN_006ab3f4(); // ModuleDream.cro +0x00A070 slot 0x48
    virtual void FUN_006ab450(); // ModuleDream.cro +0x00A088 slot 0x4C
    virtual void FUN_006ab3e0(); // ModuleDream.cro +0x00A05C slot 0x50
    virtual void FUN_006ab440(); // ModuleDream.cro +0x00A080 slot 0x54
    virtual void FUN_006ab3fc(); // ModuleDream.cro +0x00A078 slot 0x58
    virtual void FUN_006ab3ec(); // ModuleDream.cro +0x00A068 slot 0x5C
    virtual void FUN_00767b30(); // ModuleDream.cro +0x00A244 slot 0x60
};
