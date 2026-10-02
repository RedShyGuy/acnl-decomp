#pragma once

#include "decomp.h"
#include "sead/seadUlcdDoubleCmdGameFrameworkCtrNw4c.h"

// RTTI 13UlcdFramework @ 0x008CBB10
// vtable 0x008EF870 (vptr 0x008EF878), offset_to_top 0, 38 entries
class UlcdFramework : public ::sead::UlcdDoubleCmdGameFrameworkCtrNw4c
{
public:
    UlcdFramework(); // ctor candidate(s) 0x0011D148 (unverified)
    virtual void vf_0x00(); // 0x00718F28 slot 0x00 | virtual slot, introduced by sead::Framework
    virtual void vf_0x04(); // 0x00718EDC slot 0x04 | virtual slot, introduced by sead::Framework
    virtual ~UlcdFramework(); // 0x0024F380 slot 0x08 | mk7dlp:bytes
    // 0x0024F354 slot 0x0C | slot vf_0x0C of sead::Framework (deleting dtor)
    virtual void vf_0x28(); // 0x0024F1D4 slot 0x28 | virtual slot, introduced by sead::Framework
    virtual void waitStartDisplayLoop_(); // 0x00543044 slot 0x58 | slot vf_0x58 of sead::GameFramework
    virtual void procFrame_(); // 0x0054DAEC slot 0x64 | slot vf_0x64 of sead::GameFrameworkCtrNw4c
    virtual void procDraw_(); // 0x0024F238 slot 0x68 | slot vf_0x68 of sead::GameFrameworkCtrNw4c
    virtual void swapBuffer_(); // 0x0024EFF4 slot 0x78 | slot vf_0x78 of sead::GameFrameworkCtrNw4c
    virtual void waitForVBlank_(); // 0x0024F18C slot 0x7C | slot vf_0x7C of sead::GameFrameworkCtrNw4c
    virtual void clearFrameBuffers_(int); // 0x0024F1C0 slot 0x80 | slot vf_0x80 of sead::GameFrameworkCtrNw4c
};
