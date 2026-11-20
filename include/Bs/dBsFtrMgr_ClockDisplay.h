#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"

class BsFtrMgr;

// The time that the clocks among the furniture show (BsFtrMgr +0x868; code at 0x0058E3C4..0x0058E50C; no RTTI, names are
// ours): the current date and the one before, the tens and ones of the hour, minute and second with their values of
// the frame before, and the blinking colon (255 shown, 0 hidden; 30 frames).
struct FtrClockDisplay
{
    // a digit and its value of the frame before
    struct Digit
    {
        s32 value;    // 0x0
        s32 previous; // 0x4
    };
    static const s32 BLINK_FRAMES = 30;

    FtrClockDisplay(); // 0x0058E4E0
    // the digits of the current time
    void UpdateDigits(); // 0x0058E3C4
    void UpdateBlink(); // 0x0058E4AC

    nn::fnd::DateTimeParameters m_Date;         // 0x00
    nn::fnd::DateTimeParameters m_PreviousDate; // 0x0C
    Digit m_HourTens;                           // 0x18
    Digit m_HourOnes;                           // 0x20
    Digit m_MinuteTens;                         // 0x28
    Digit m_MinuteOnes;                         // 0x30
    Digit m_SecondTens;                         // 0x38
    Digit m_SecondOnes;                         // 0x40
    s32 m_BlinkTimer;                           // 0x48
    s32 m_ColonAlpha;                           // 0x4C
};
ASSERT_SIZE(FtrClockDisplay, 0x50);
