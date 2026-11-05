#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0xF360 in ModuleTrain.cro, offset_to_top 0, 25 entries
// vtable +0xF3CC in ModuleTrain.cro, offset_to_top -40, 3 entries
class BsMenuAppraiser : public ::MenuBase, public ::state::Mode<BsMenuAppraiser>
{
public:
    BsMenuAppraiser(); // ctor address unknown
    virtual ~BsMenuAppraiser(); // ModuleTrain.cro +0x0089CC slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleTrain.cro +0x0063D8 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleTrain.cro +0x0088F4 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleTrain.cro +0x008854 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleTrain.cro +0x0063C0 slot 0x30
    virtual bool Unk0(); // ModuleTrain.cro +0x00C044 slot 0x3C
    virtual void FUN_006ab3d8(); // ModuleTrain.cro +0x00C008 slot 0x40
    virtual void OnClose(); // ModuleTrain.cro +0x00C014 slot 0x44
    virtual void FUN_006ab3f4(bool value); // ModuleTrain.cro +0x00C020 slot 0x48
    virtual bool FUN_006ab450(); // ModuleTrain.cro +0x00C038 slot 0x4C
    virtual void FUN_006ab3e0(bool value); // ModuleTrain.cro +0x00C00C slot 0x50
    virtual bool FUN_006ab440(); // ModuleTrain.cro +0x00C030 slot 0x54
    virtual void FUN_006ab3fc(bool value); // ModuleTrain.cro +0x00C028 slot 0x58
    virtual bool FUN_006ab3ec(); // ModuleTrain.cro +0x00C018 slot 0x5C
    virtual bool FUN_00767b30(); // ModuleTrain.cro +0x00C098 slot 0x60
};
