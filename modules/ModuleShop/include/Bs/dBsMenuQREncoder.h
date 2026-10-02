#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x26AB0 in ModuleShop.cro, offset_to_top 0, 25 entries
// vtable +0x26B1C in ModuleShop.cro, offset_to_top -40, 3 entries
class BsMenuQREncoder : public ::MenuBase, public ::state::Mode<BsMenuQREncoder>
{
public:
    BsMenuQREncoder(); // ctor address unknown
    virtual ~BsMenuQREncoder(); // ModuleShop.cro +0x0086AC slot 0x00
    virtual void Initialize(); // ModuleShop.cro +0x007F54 slot 0x0C
    virtual void Finalize(); // ModuleShop.cro +0x00830C slot 0x18
    virtual void Calc(); // ModuleShop.cro +0x00821C slot 0x24
    virtual void Draw(); // ModuleShop.cro +0x007F24 slot 0x30
    virtual void Unk0(); // ModuleShop.cro +0x0230A0 slot 0x3C
    virtual void FUN_006ab3d8(); // ModuleShop.cro +0x0228E0 slot 0x40
    virtual void OnClose(); // ModuleShop.cro +0x0228EC slot 0x44
    virtual void FUN_006ab3f4(); // ModuleShop.cro +0x0228F8 slot 0x48
    virtual void FUN_006ab450(); // ModuleShop.cro +0x022910 slot 0x4C
    virtual void FUN_006ab3e0(); // ModuleShop.cro +0x0228E4 slot 0x50
    virtual void FUN_006ab440(); // ModuleShop.cro +0x022908 slot 0x54
    virtual void FUN_006ab3fc(); // ModuleShop.cro +0x022900 slot 0x58
    virtual void FUN_006ab3ec(); // ModuleShop.cro +0x0228F0 slot 0x5C
    virtual void FUN_00767b30(); // ModuleShop.cro +0x0230F4 slot 0x60
};
