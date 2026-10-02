#pragma once

#include "decomp.h"
#include "sead/seadGameFrameworkCtrNw4c.h"

namespace sead {
// RTTI N4sead29DoubleCmdGameFrameworkCtrNw4cE @ 0x008D1F84
// vtable 0x00906864 (vptr 0x0090686C), offset_to_top 0, 36 entries
class DoubleCmdGameFrameworkCtrNw4c : public ::sead::GameFrameworkCtrNw4c
{
public:
    DoubleCmdGameFrameworkCtrNw4c(); // ctor candidate(s) 0x00120A64 (unverified)
    virtual void vf_0x00(); // 0x0074E45C slot 0x00 | virtual slot, introduced by sead::Framework
    virtual void vf_0x04(); // 0x0074E410 slot 0x04 | virtual slot, introduced by sead::Framework
    virtual ~DoubleCmdGameFrameworkCtrNw4c(); // 0x0054DF60 slot 0x08 | slot vf_0x08 of sead::Framework
    // 0x0054DF1C slot 0x0C | slot vf_0x0C of sead::Framework (deleting dtor)
    virtual void mainLoop_(); // 0x0054DEE4 slot 0x60 | slot vf_0x60 of sead::GameFrameworkCtrNw4c
    virtual void procFrame_(); // 0x0054DAF0 slot 0x64 | slot vf_0x64 of sead::GameFrameworkCtrNw4c
    virtual void presentTop_(); // 0x0054DC6C slot 0x70 | slot vf_0x70 of sead::GameFrameworkCtrNw4c
    virtual void presentBtm_(); // 0x0054DC34 slot 0x74 | slot vf_0x74 of sead::GameFrameworkCtrNw4c
    virtual void swapBuffer_(); // 0x0054DCA4 slot 0x78 | nintendogs:callseq
    virtual void waitForVBlank_(); // 0x0054DD50 slot 0x7C | slot vf_0x7C of sead::GameFrameworkCtrNw4c
    virtual void vf_0x88(); // 0x0054DEC8 slot 0x88 | virtual slot, introduced by sead::GameFrameworkCtrNw4c
    virtual void vf_0x8C(); // 0x0054DECC slot 0x8C | virtual slot, introduced by sead::DoubleCmdGameFrameworkCtrNw4c
};
} // namespace sead
