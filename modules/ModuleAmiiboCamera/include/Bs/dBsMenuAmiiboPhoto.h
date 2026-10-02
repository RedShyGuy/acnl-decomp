#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// vtable +0x1541C in ModuleAmiiboCamera.cro, offset_to_top 0, 27 entries
// vtable +0x15490 in ModuleAmiiboCamera.cro, offset_to_top -40, 63 entries
// vtable +0x15594 in ModuleAmiiboCamera.cro, offset_to_top -164, 3 entries
class BsMenuAmiiboPhoto : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuAmiiboPhoto>
{
public:
    BsMenuAmiiboPhoto(); // ctor address unknown
    virtual ~BsMenuAmiiboPhoto(); // ModuleAmiiboCamera.cro +0x008240 slot 0x00
    virtual void Initialize(); // ModuleAmiiboCamera.cro +0x00B7F4 slot 0x0C
    virtual void Finalize(); // ModuleAmiiboCamera.cro +0x00BE24 slot 0x18
    virtual void Calc(); // ModuleAmiiboCamera.cro +0x00BBDC slot 0x24
    virtual void Draw(); // ModuleAmiiboCamera.cro +0x00B6FC slot 0x30
    virtual void FUN_006ab3d8(); // ModuleAmiiboCamera.cro +0x011300 slot 0x40
    virtual void OnClose(); // ModuleAmiiboCamera.cro +0x01130C slot 0x44
    virtual void FUN_006ab3f4(); // ModuleAmiiboCamera.cro +0x011318 slot 0x48
    virtual void FUN_006ab450(); // ModuleAmiiboCamera.cro +0x011330 slot 0x4C
    virtual void FUN_006ab3e0(); // ModuleAmiiboCamera.cro +0x011304 slot 0x50
    virtual void FUN_006ab440(); // ModuleAmiiboCamera.cro +0x011328 slot 0x54
    virtual void FUN_006ab3fc(); // ModuleAmiiboCamera.cro +0x011320 slot 0x58
    virtual void FUN_006ab3ec(); // ModuleAmiiboCamera.cro +0x011310 slot 0x5C
    virtual void FUN_00767b30(); // ModuleAmiiboCamera.cro +0x01262C slot 0x60
};
