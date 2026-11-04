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
    /* 0x0000 */ s8 playerIndex; // ctor sets to 0xFF (-1: none)
    /* 0x0001 */ u8 pad_0x1;
    /* 0x0002 */ SvRoom museumRoom;
    /* 0x0304 */ SvMyDesign exhibitPoster;
    /* 0x0B74 */ char16 exhibitTitle[0x11];
    /* 0x0B96 */ u16 pad_0xB96;
};
ASSERT_SIZE(SvMuseumExhibit, 0xB98);

// the museum (one struct in the original: GetExhibit gets its address)
struct SvMuseum {
    // the exhibition room (the first one for an index of 4 or more)
    SvMuseumExhibit* GetExhibit(u32 index); // 0x006BCED8 (name is ours)

    /* 0x0000 */ SvDate donationDates[0x112];
    /* 0x0448 */ u8 donations[0x112];
    /* 0x055A */ u8 pad_0x55A[2];
    /* 0x055C */ SvMuseumExhibit exhibits[4];
};
ASSERT_SIZE(SvMuseum, 0x33BC);

#pragma pack(pop)
