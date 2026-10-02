#pragma once

// Museum exhibition rooms.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvHouse.h"
#include "Sv/dSvMyDesign.h"

#pragma pack(push, 1)

struct SvMuseumExhibit {
    /* 0x0000 */ u8 playerIndex; // ctor sets to 0xFF
    /* 0x0001 */ u8 pad_0x1;
    /* 0x0002 */ SvRoom museumRoom;
    /* 0x0304 */ SvMyDesign exhibitPoster;
    /* 0x0B74 */ char16 exhibitTitle[0x11];
    /* 0x0B96 */ u16 pad_0xB96;
};
ASSERT_SIZE(SvMuseumExhibit, 0xB98);

#pragma pack(pop)
