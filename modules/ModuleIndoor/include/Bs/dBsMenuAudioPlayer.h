#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x668E0 in ModuleIndoor.cro, offset_to_top 0, 25 entries
// vtable +0x6694C in ModuleIndoor.cro, offset_to_top -40, 3 entries
class BsMenuAudioPlayer : public ::MenuBase, public ::state::Mode<BsMenuAudioPlayer>
{
public:
    BsMenuAudioPlayer(); // ctor address unknown
    virtual ~BsMenuAudioPlayer(); // ModuleIndoor.cro +0x030B40 slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x02706C slot 0x0C
    virtual void Finalize(); // ModuleIndoor.cro +0x0278E8 slot 0x18
    virtual void Calc(); // ModuleIndoor.cro +0x02777C slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x027008 slot 0x30
    virtual void FUN_006ab3d8(); // ModuleIndoor.cro +0x055C68 slot 0x40
    virtual void OnClose(); // ModuleIndoor.cro +0x055C74 slot 0x44
    virtual void FUN_006ab3f4(); // ModuleIndoor.cro +0x055C80 slot 0x48
    virtual void FUN_006ab450(); // ModuleIndoor.cro +0x055C98 slot 0x4C
    virtual void FUN_006ab3e0(); // ModuleIndoor.cro +0x055C6C slot 0x50
    virtual void FUN_006ab440(); // ModuleIndoor.cro +0x055C90 slot 0x54
    virtual void FUN_006ab3fc(); // ModuleIndoor.cro +0x055C88 slot 0x58
    virtual void FUN_006ab3ec(); // ModuleIndoor.cro +0x055C78 slot 0x5C
    virtual void FUN_00767b30(); // ModuleIndoor.cro +0x05AFBC slot 0x60
};
