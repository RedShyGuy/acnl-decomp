#include "ssys/co/dFaderBase.h"
#include "Other/dFaderMoment.h"

// 0x001C884C
FaderMoment::FaderMoment(u8 arg, s32 status) : ssys::co::FaderBase(arg, status)
{
    // (only the base is constructed)
}

// 0x001C8874 slot 0x00
// 0x001C8864 (deleting dtor)
FaderMoment::~FaderMoment()
{
}

// 0x00566FAC slot 0x08 (falls into the one of FaderBase)
void FaderMoment::SetStatus(s32 status)
{
    ssys::co::FaderBase::SetStatus(status);
}

// 0x00566F48 slot 0x10 (falls into the one of FaderBase)
bool FaderMoment::FadeIn()
{
    return ssys::co::FaderBase::FadeIn();
}

// 0x00566F7C slot 0x14 (falls into the one of FaderBase)
bool FaderMoment::FadeOut()
{
    return ssys::co::FaderBase::FadeOut();
}

// 0x001C878C slot 0x18 | virtual slot, introduced by ssys::co::FaderBase
bool FaderMoment::Calc()
{
}

// 0x001C8848 slot 0x1C | virtual slot, introduced by ssys::co::FaderBase
void FaderMoment::Draw()
{
}

