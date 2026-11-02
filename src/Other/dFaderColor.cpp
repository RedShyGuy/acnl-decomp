#include "ssys/co/dFaderBase.h"
#include "Other/dFaderColor.h"

// 0x001AA9F8 (not decompiled yet: the rest of the body)
FaderColor::FaderColor(u8 arg, const u8*, u32, s32 status) : ssys::co::FaderBase(arg, status)
{
}

// 0x00567008 slot 0x00
// 0x001AAA48 (deleting dtor)
FaderColor::~FaderColor()
{
    // empty (in the original too: falls into the destructor of FaderBase)
}

// 0x001AA9C8 slot 0x08 | virtual slot, introduced by ssys::co::FaderBase
void FaderColor::SetStatus(s32)
{
}

// 0x001AA840 slot 0x10 | virtual slot, introduced by ssys::co::FaderBase
bool FaderColor::FadeIn()
{
}

// 0x001AA908 slot 0x14 | virtual slot, introduced by ssys::co::FaderBase
bool FaderColor::FadeOut()
{
}

// 0x001AA734 slot 0x18 | virtual slot, introduced by ssys::co::FaderBase
bool FaderColor::Calc()
{
}

// 0x001AA83C slot 0x1C | virtual slot, introduced by ssys::co::FaderBase
void FaderColor::Draw()
{
}

