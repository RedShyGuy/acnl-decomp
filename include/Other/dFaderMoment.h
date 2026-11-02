#pragma once

#include "decomp.h"
#include "ssys/co/dFaderBase.h"

// RTTI 11FaderMoment @ 0x008CB2CC
// vtable 0x008ECB50 (vptr 0x008ECB58), offset_to_top 0, 8 entries
class FaderMoment : public ::ssys::co::FaderBase
{
public:
    FaderMoment(u8 arg, s32 status); // 0x001C884C
    virtual ~FaderMoment(); // 0x001C8874 slot 0x00
    void SetStatus(s32 status) override; // 0x00566FAC slot 0x08
    bool FadeIn() override; // 0x00566F48 slot 0x10
    bool FadeOut() override; // 0x00566F7C slot 0x14
    bool Calc() override; // 0x001C878C slot 0x18
    void Draw() override; // 0x001C8848 slot 0x1C
};
