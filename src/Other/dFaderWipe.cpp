#include "ssys/co/dFaderBase.h"
#include "Other/dFaderWipe.h"

// 0x006E5598
FaderWipe::FaderWipe(u8 arg, s32 status) : ssys::co::FaderBase(arg, status)
{
    // (only the base is constructed)
}

// 0x006E55C0 slot 0x00
// 0x006E55B0 (deleting dtor)
FaderWipe::~FaderWipe()
{
}

// 0x006E5568 slot 0x08 | virtual slot, introduced by ssys::co::FaderBase
void FaderWipe::SetStatus(s32)
{
}

// 0x006E5498 slot 0x10 | virtual slot, introduced by ssys::co::FaderBase
bool FaderWipe::FadeIn()
{
}

// 0x006E5500 slot 0x14 | virtual slot, introduced by ssys::co::FaderBase
bool FaderWipe::FadeOut()
{
}

// 0x006E538C slot 0x18 | virtual slot, introduced by ssys::co::FaderBase
bool FaderWipe::Calc()
{
}

// 0x006E5494 slot 0x1C | virtual slot, introduced by ssys::co::FaderBase
void FaderWipe::Draw()
{
}

