#pragma once

#include "decomp.h"
#include "ssys/co/dFaderBase.h"

// RTTI 10FaderColor @ 0x008CB0DC
// vtable 0x008EC174 (vptr 0x008EC17C), offset_to_top 0, 8 entries
class FaderColor : public ::ssys::co::FaderBase
{
public:
    FaderColor(u8 arg, const u8* color, u32 _3, s32 status); // 0x001AA9F8
    virtual ~FaderColor(); // 0x00567008 slot 0x00
    void SetStatus(s32 status) override; // 0x001AA9C8 slot 0x08
    bool FadeIn() override; // 0x001AA840 slot 0x10
    bool FadeOut() override; // 0x001AA908 slot 0x14
    bool Calc() override; // 0x001AA734 slot 0x18
    void Draw() override; // 0x001AA83C slot 0x1C
};
