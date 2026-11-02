#pragma once

#include "decomp.h"
#include "ssys/co/dFaderBase.h"

// RTTI 9FaderWipe @ 0x008CD790
// vtable 0x008FA680 (vptr 0x008FA688), offset_to_top 0, 8 entries
class FaderWipe : public ::ssys::co::FaderBase
{
public:
    FaderWipe(u8 arg, s32 status); // 0x006E5598
    virtual ~FaderWipe(); // 0x006E55C0 slot 0x00
    void SetStatus(s32 status) override; // 0x006E5568 slot 0x08
    bool FadeIn() override; // 0x006E5498 slot 0x10
    bool FadeOut() override; // 0x006E5500 slot 0x14
    bool Calc() override; // 0x006E538C slot 0x18
    void Draw() override; // 0x006E5494 slot 0x1C
};
