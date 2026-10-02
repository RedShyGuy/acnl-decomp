#pragma once

// Custom designs ("My Design" patterns).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvPersonalId.h"

#pragma pack(push, 1)

struct SvMyDesign {
    /* 0x0000 */ char16 title[0x15];
    /* 0x002A */ SvPersonalId creatorData;
    /* 0x0058 */ u8 palette[0xF];
    /* 0x0067 */ u8 unk_0x67; // research name: UnusedChecksum / changing seems to have no effect / Default: 1, Set at 0x1B4F74 EUR 1.5
    /* 0x0068 */ u8 tenConstant; // seems to always be 0x0A
    /* 0x0069 */ u8 patternType;
    /* 0x006A */ u16 pad_0x6A; // Zero Padding; Always 0x0000
    /* 0x006C */ u8 patternData1[0x200]; // mandatory
    /* 0x026C */ u8 patternData2[0x200]; // optional
    /* 0x046C */ u8 patternData3[0x200]; // optional
    /* 0x066C */ u8 patternData4[0x200]; // optional
    /* 0x086C */ u32 pad_0x86C; // Zero Padding; Optional*/
};
ASSERT_SIZE(SvMyDesign, 0x870);

#pragma pack(pop)
