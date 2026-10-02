#pragma once

// Town wide progress flags.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"

#pragma pack(push, 1)

struct SvTownFlags {
    /* 0x0000.0 */ u8 unk_0x0_0 : 1;
    /* 0x0000.1 */ u8 earlyBirdOrdinance : 1;
    /* 0x0000.2 */ u8 nightOwlOrdinance : 1;
    /* 0x0000.3 */ u8 bellBoomOrdinance : 1;
    /* 0x0000.4 */ u8 beautifulOrdinance : 1;
    /* 0x0000.5 */ u8 unk_0x0_5 : 1;
    /* 0x0000.6 */ u8 unk_0x0_6 : 1;
    /* 0x0000.7 */ u8 unk_0x0_7 : 1;
    /* 0x0001.0 */ u8 unk_0x1_0 : 1;
    /* 0x0001.1 */ u8 unk_0x1_1 : 1;
    /* 0x0001.2 */ u8 unk_0x1_2 : 1; // gulliver woken up?
    /* 0x0001.3 */ u8 unk_0x1_3 : 1; // second player created?
    /* 0x0001.4 */ u8 unk_0x1_4 : 1;
    /* 0x0001.5 */ u8 unk_0x1_5 : 1;
    /* 0x0001.6 */ u8 unk_0x1_6 : 1;
    /* 0x0001.7 */ u8 unk_0x1_7 : 1;
    /* 0x0002.0 */ u8 unk_0x2_0 : 1;
    /* 0x0002.1 */ u8 unk_0x2_1 : 1;
    /* 0x0002.2 */ u8 unk_0x2_2 : 1;
    /* 0x0002.3 */ u8 unk_0x2_3 : 1;
    /* 0x0002.4 */ u8 unk_0x2_4 : 1;
    /* 0x0002.5 */ u8 unk_0x2_5 : 1;
    /* 0x0002.6 */ u8 unk_0x2_6 : 1;
    /* 0x0002.7 */ u8 unk_0x2_7 : 1;
    /* 0x0003.0 */ u8 unk_0x3_0 : 1;
    /* 0x0003.1 */ u8 unk_0x3_1 : 1;
    /* 0x0003.2 */ u8 unk_0x3_2 : 1;
    /* 0x0003.3 */ u8 unk_0x3_3 : 1;
    /* 0x0003.4 */ u8 unk_0x3_4 : 1;
    /* 0x0003.5 */ u8 unk_0x3_5 : 1;
    /* 0x0003.6 */ u8 unk_0x3_6 : 1;
    /* 0x0003.7 */ u8 unk_0x3_7 : 1;
    /* 0x0004.0 */ u8 unk_0x4_0 : 1;
    /* 0x0004.1 */ u8 unk_0x4_1 : 1;
    /* 0x0004.2 */ u8 unk_0x4_2 : 1;
    /* 0x0004.3 */ u8 unk_0x4_3 : 1;
    /* 0x0004.4 */ u8 unk_0x4_4 : 1;
    /* 0x0004.5 */ u8 unk_0x4_5 : 1;
    /* 0x0004.6 */ u8 unk_0x4_6 : 1; // ctor sets this bit
    /* 0x0004.7 */ u8 unk_0x4_7 : 1;
    /* 0x0005.0 */ u8 unk_0x5_0 : 1;
    /* 0x0005.1 */ u8 unk_0x5_1 : 1;
    /* 0x0005.2 */ u8 unk_0x5_2 : 1;
    /* 0x0005.3 */ u8 unk_0x5_3 : 1;
    /* 0x0005.4 */ u8 unk_0x5_4 : 1;
    /* 0x0005.5 */ u8 unk_0x5_5 : 1;
    /* 0x0005.6 */ u8 unk_0x5_6 : 1;
    /* 0x0005.7 */ u8 unk_0x5_7 : 1;
    /* 0x0006.0 */ u8 unk_0x6_0 : 1;
    /* 0x0006.1 */ u8 unk_0x6_1 : 1;
    /* 0x0006.2 */ u8 unk_0x6_2 : 1;
    /* 0x0006.3 */ u8 unk_0x6_3 : 1;
    /* 0x0006.4 */ u8 unk_0x6_4 : 1;
    /* 0x0006.5 */ u8 unk_0x6_5 : 1;
    /* 0x0006.6 */ u8 unk_0x6_6 : 1;
    /* 0x0006.7 */ u8 qrMachineUnlocked : 1;
    /* 0x0007.0 */ u8 unk_0x7_0 : 1;
    /* 0x0007.1 */ u8 unk_0x7_1 : 1;
    /* 0x0007.2 */ u8 unk_0x7_2 : 1;
    /* 0x0007.3 */ u8 unk_0x7_3 : 1;
    /* 0x0007.4 */ u8 unk_0x7_4 : 1;
    /* 0x0007.5 */ u8 unk_0x7_5 : 1;
    /* 0x0007.6 */ u8 unk_0x7_6 : 1;
    /* 0x0007.7 */ u8 unk_0x7_7 : 1;
    /* 0x0008.0 */ u8 unk_0x8_0 : 1;
    /* 0x0008.1 */ u8 unk_0x8_1 : 1;
    /* 0x0008.2 */ u8 unk_0x8_2 : 1;
    /* 0x0008.3 */ u8 unk_0x8_3 : 1;
    /* 0x0008.4 */ u8 unk_0x8_4 : 1;
    /* 0x0008.5 */ u8 unk_0x8_5 : 1;
    /* 0x0008.6 */ u8 unk_0x8_6 : 1;
    /* 0x0008.7 */ u8 unk_0x8_7 : 1; // gets set when you talked to isabelle after she wants to build the dream suite
    /* 0x0009.0 */ u8 unk_0x9_0 : 1; // ctor sets this bit
    /* 0x0009.1 */ u8 unk_0x9_1 : 1; // ctor sets this bit
    /* 0x0009.2 */ u8 unk_0x9_2 : 1; // ctor sets this bit
    /* 0x0009.3 */ u8 unk_0x9_3 : 1;
    /* 0x0009.4 */ u8 unk_0x9_4 : 1;
    /* 0x0009.5 */ u8 unk_0x9_5 : 1;
    /* 0x0009.6 */ u8 unk_0x9_6 : 1;
    /* 0x0009.7 */ u8 unk_0x9_7 : 1;
    /* 0x000A.0 */ u8 unk_0xA_0 : 1;
    /* 0x000A.1 */ u8 unk_0xA_1 : 1;
    /* 0x000A.2 */ u8 unk_0xA_2 : 1;
    /* 0x000A.3 */ u8 unk_0xA_3 : 1;
    /* 0x000A.4 */ u8 unk_0xA_4 : 1;
    /* 0x000A.5 */ u8 unk_0xA_5 : 1;
    /* 0x000A.6 */ u8 unk_0xA_6 : 1; // someone new moved in, isabelle mentions it, then its set to 0 again
    /* 0x000A.7 */ u8 openPublicWorkProject : 1;
    /* 0x000B.0 */ u8 unk_0xB_0 : 1;
    /* 0x000B.1 */ u8 unk_0xB_1 : 1;
    /* 0x000B.2 */ u8 unk_0xB_2 : 1;
    /* 0x000B.3 */ u8 unk_0xB_3 : 1;
    /* 0x000B.4 */ u8 unk_0xB_4 : 1;
    /* 0x000B.5 */ u8 unk_0xB_5 : 1;
    /* 0x000B.6 */ u8 unk_0xB_6 : 1;
    /* 0x000B.7 */ u8 unk_0xB_7 : 1;
    /* 0x000C.0 */ u8 unk_0xC_0 : 1;
    /* 0x000C.1 */ u8 unk_0xC_1 : 1;
    /* 0x000C.2 */ u8 unk_0xC_2 : 1;
    /* 0x000C.3 */ u8 unk_0xC_3 : 1;
    /* 0x000C.4 */ u8 unk_0xC_4 : 1;
    /* 0x000C.5 */ u8 unk_0xC_5 : 1;
    /* 0x000C.6 */ u8 unk_0xC_6 : 1;
    /* 0x000C.7 */ u8 unk_0xC_7 : 1;
    /* 0x000D.0 */ u8 hhsStandUnlocked : 1; // lyles stand in tom nooks shop
    /* 0x000D.1 */ u8 unk_0xD_1 : 1;
    /* 0x000D.2 */ u8 unk_0xD_2 : 1;
    /* 0x000D.3 */ u8 unk_0xD_3 : 1;
    /* 0x000D.4 */ u8 unk_0xD_4 : 1;
    /* 0x000D.5 */ u8 unk_0xD_5 : 1;
    /* 0x000D.6 */ u8 unk_0xD_6 : 1; // found magic lamp?
    /* 0x000D.7 */ u8 unk_0xD_7 : 1;
    /* 0x000E.0 */ u8 unk_0xE_0 : 1;
    /* 0x000E.1 */ u8 unk_0xE_1 : 1;
    /* 0x000E.2 */ u8 unk_0xE_2 : 1;
    /* 0x000E.3 */ u8 unk_0xE_3 : 1;
    /* 0x000E.4 */ u8 unk_0xE_4 : 1;
    /* 0x000E.5 */ u8 unk_0xE_5 : 1;
    /* 0x000E.6 */ u8 unk_0xE_6 : 1;
    /* 0x000E.7 */ u8 unk_0xE_7 : 1; // this might be related to the last census menu entry
    /* 0x000F.0 */ u8 unk_0xF_0 : 1; // this might be related to the last census menu entry
    /* 0x000F.1 */ u8 unk_0xF_1 : 1;
    /* 0x000F.2 */ u8 unk_0xF_2 : 1;
    /* 0x000F.3 */ u8 unk_0xF_3 : 1;
    /* 0x000F.4 */ u8 unk_0xF_4 : 1; // its snowing/raining?
    /* 0x000F.5 */ u8 unk_0xF_5 : 1;
    /* 0x000F.6 */ u8 unk_0xF_6 : 1;
    /* 0x000F.7 */ u8 unk_0xF_7 : 1;
    /* 0x0010.0 */ u8 unk_0x10_0 : 1;
    /* 0x0010.1 */ u8 unk_0x10_1 : 1;
    /* 0x0010.2 */ u8 unk_0x10_2 : 1;
    /* 0x0010.3 */ u8 unk_0x10_3 : 1;
    /* 0x0010.4 */ u8 unk_0x10_4 : 1;
    /* 0x0010.5 */ u8 unk_0x10_5 : 1;
    /* 0x0010.6 */ u8 unk_0x10_6 : 1;
};
ASSERT_SIZE(SvTownFlags, 0x11);

#pragma pack(pop)
