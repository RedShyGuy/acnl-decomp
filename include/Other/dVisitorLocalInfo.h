#pragma once

#include "decomp.h"

struct SvTownFlags;

// Data of a town that is visited (only the parts the flows use are laid out; the class name is from the RTTI, the rest
// is ours): each part stands in for the data of the own town while its flag is set.
class VisitorLocalInfo
{
public:
    class SingletonDisposer_;

    // (game code, not decompiled yet; called when the player is back in the own town)
    void Func2C2B28(); // 0x002C2B28
    // (game code, not decompiled yet; called after loads)
    void Func2C2AA8(); // 0x002C2AA8
    void Func2C2ADC(); // 0x002C2ADC
    void Func2C2B04(); // 0x002C2B04
    void Func2C2B10(); // 0x002C2B10
    void Func2C2B34(); // 0x002C2B34
    void Func2C2CDC(); // 0x002C2CDC

    // the 51 flags the flow events 64/65 and the branch 146 use
    struct Flags
    {
        u8 bit0 : 1;
        u8 bit1 : 1;
        u8 bit2 : 1;
        u8 bit3 : 1;
        u8 bit4 : 1;
        u8 bit5 : 1;
        u8 bit6 : 1;
        u8 bit7 : 1;
        u8 bit8 : 1;
        u8 bit9 : 1;
        u8 bit10 : 1;
        u8 bit11 : 1;
        u8 bit12 : 1;
        u8 bit13 : 1;
        u8 bit14 : 1;
        u8 bit15 : 1;
        u8 bit16 : 1;
        u8 bit17 : 1;
        u8 bit18 : 1;
        u8 bit19 : 1;
        u8 bit20 : 1;
        u8 bit21 : 1;
        u8 bit22 : 1;
        u8 bit23 : 1;
        u8 bit24 : 1;
        u8 bit25 : 1;
        u8 bit26 : 1;
        u8 bit27 : 1;
        u8 bit28 : 1;
        u8 bit29 : 1;
        u8 bit30 : 1;
        u8 bit31 : 1;
        u8 bit32 : 1;
        u8 bit33 : 1;
        u8 bit34 : 1;
        u8 bit35 : 1;
        u8 bit36 : 1;
        u8 bit37 : 1;
        u8 bit38 : 1;
        u8 bit39 : 1;
        u8 bit40 : 1;
        u8 bit41 : 1;
        u8 bit42 : 1;
        u8 bit43 : 1;
        u8 bit44 : 1;
        u8 bit45 : 1;
        u8 bit46 : 1;
        u8 bit47 : 1;
        u8 bit48 : 1;
        u8 bit49 : 1;
        u8 bit50 : 1;
    };

    SvTownFlags* GetTownFlags(); // 0x002C2764
    u8* GetByte41(); // 0x002C27F4
    Flags* GetFlags(); // 0x002C28D0
    u8* GetByte42(); // 0x002C293C

    u8 _unknown0x0[0x30];   // 0x00
    u8 m_HasTownFlags;      // 0x30
    u8 m_HasFlags;          // 0x31
    u8 m_HasByte41;         // 0x32
    u8 m_HasByte42;         // 0x33
    u8 _unknown0x34[4];     // 0x34

    static VisitorLocalInfo* s_pInstance;
};
ASSERT_SIZE(VisitorLocalInfo, 0x38);
