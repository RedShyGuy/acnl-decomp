#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_MonitoringData.h"

namespace nn {
namespace pia {
namespace common {
// The monitoring data of a session as it begins (block type 0): settings and counters of the
// modules. The layout is the one of Serialize (the fields in order, each aligned); most members
// are not named yet. The class, Serialize, GetSerializedSize and Cleanup are from the fefates
// symbols, the rest is ours.
class SessionBeginMonitoringContent : public MonitoringContent
{
public:
    // all fields 0xFF, the header and the constant fields set (name is ours)
    void Initialize(); // 0x0042843C
    // the fields of the last session invalid again
    void Cleanup(); // 0x004284C4 | fefates:callseq-callee [tier C]
    DECOMP_NOINLINE static unsigned int GetSerializedSize(); // 0x004284B8 | fefates:callgraph [tier C]
    nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00731E44 | fefates:callgraph [tier C]

    u32 m_Version;                     // 0x010
    u32 m_Unknown0x14;                 // 0x014
    u32 m_Unknown0x18;                 // 0x018
    u8 m_Unknown0x1C;                  // 0x01C
    u8 m_Unknown0x1D;                  // 0x01D
    u8 m_CountryCode;                  // 0x01E
    u8 m_RegionCode;                   // 0x01F
    u32 m_CommonHeapSize;              // 0x020
    u32 m_Unknown0x24;                 // 0x024
    u32 m_Unknown0x28;                 // 0x028
    u8 m_Unknown0x2C;                  // 0x02C
    u32 m_Unknown0x30;                 // 0x030
    u32 m_Unknown0x34;                 // 0x034
    u8 m_Unknown0x38;                  // 0x038
    u8 m_Unknown0x39;                  // 0x039
    u32 m_Unknown0x3C;                 // 0x03C
    u32 m_Unknown0x40;                 // 0x040
    u8 m_Unknown0x44;                  // 0x044
    u8 m_Unknown0x45;                  // 0x045
    u8 m_Unknown0x46;                  // 0x046
    u8 m_Unknown0x47;                  // 0x047
    u16 m_Unknown0x48;                 // 0x048
    u16 m_Unknown0x4A;                 // 0x04A
    u16 m_Unknown0x4C;                 // 0x04C
    u32 m_Unknown0x50;                 // 0x050
    u32 m_Unknown0x54;                 // 0x054
    u32 m_Unknown0x58;                 // 0x058
    u32 m_Unknown0x5C;                 // 0x05C
    u8 m_Unknown0x60;                  // 0x060
    u32 m_Unknown0x64;                 // 0x064
    u32 m_Unknown0x68;                 // 0x068
    u32 m_Unknown0x6C;                 // 0x06C
    u8 m_Unknown0x70;                  // 0x070
    u8 m_Unknown0x71;                  // 0x071
    u16 m_Unknown0x72;                 // 0x072
    u32 m_Unknown0x74;                 // 0x074
    u32 m_Unknown0x78;                 // 0x078
    u32 m_Unknown0x7C;                 // 0x07C
    u32 m_Unknown0x80;                 // 0x080
    u32 m_Unknown0x84;                 // 0x084
    u16 m_Unknown0x88;                 // 0x088
    u32 m_Unknown0x8C;                 // 0x08C
    u16 m_Unknown0x90;                 // 0x090
    u32 m_Unknown0x94;                 // 0x094
    u16 m_Unknown0x98;                 // 0x098
    u16 m_Unknown0x9A;                 // 0x09A
    u16 m_Unknown0x9C;                 // 0x09C
    u32 m_Unknown0xA0;                 // 0x0A0
    u8 m_Unknown0xA4;                  // 0x0A4
    u8 m_Unknown0xA5;                  // 0x0A5
    u8 m_Unknown0xA6;                  // 0x0A6
    u32 m_Unknown0xA8;                 // 0x0A8
    u32 m_Unknown0xAC;                 // 0x0AC
    u8 m_Unknown0xB0;                  // 0x0B0
    u32 m_Unknown0xB4[23];             // 0x0B4
    u8 m_Unknown0x110;                 // 0x110
    u32 m_Unknown0x114[12];            // 0x114
    u16 m_Unknown0x144;                // 0x144
    u8 m_Unknown0x146;                 // 0x146
    u8 m_Unknown0x147;                 // 0x147
    u32 m_Unknown0x148;                // 0x148
    u16 m_Unknown0x14C;                // 0x14C
    u16 m_Unknown0x14E;                // 0x14E
    u16 m_Unknown0x150;                // 0x150
    u16 m_Unknown0x152;                // 0x152
    u16 m_Unknown0x154;                // 0x154
    u16 m_Unknown0x156;                // 0x156
    u16 m_Unknown0x158;                // 0x158
    u16 m_Unknown0x15A;                // 0x15A
    u8 m_Unknown0x15C;                 // 0x15C
    u16 m_Unknown0x15E;                // 0x15E
    u16 m_Unknown0x160;                // 0x160
    u16 m_Unknown0x162;                // 0x162
    u8 m_Unknown0x164;                 // 0x164
    u32 m_Unknown0x168;                // 0x168
    u8 m_Unknown0x16C;                 // 0x16C
    u8 m_Unknown0x16D;                 // 0x16D
    u8 m_Unknown0x16E;                 // 0x16E
    u8 m_Unknown0x16F;                 // 0x16F
    u32 m_Unknown0x170;                // 0x170
    u32 m_Unknown0x174;                // 0x174
    u32 m_Unknown0x178;                // 0x178
    u8 m_Unknown0x17C;                 // 0x17C
    u8 m_Unknown0x17D;                 // 0x17D
    u32 m_Unknown0x180;                // 0x180
    u32 m_Unknown0x184;                // 0x184
    u32 m_Unknown0x188[6];             // 0x188
    u16 m_Unknown0x1A0;                // 0x1A0
    u32 m_Unknown0x1A4;                // 0x1A4
    u8 m_Unknown0x1A8;                 // 0x1A8
    u8 m_Unknown0x1A9;                 // 0x1A9
    u8 m_Unknown0x1AA;                 // 0x1AA
    u8 m_Unknown0x1AB;                 // 0x1AB
    u8 m_Unknown0x1AC[6];              // 0x1AC
    u32 m_Unknown0x1B4[6];             // 0x1B4
    u32 m_Unknown0x1CC[6];             // 0x1CC
    u8 m_Unknown0x1E4;                 // 0x1E4
    u8 m_Unknown0x1E5;                 // 0x1E5
    u8 m_Unknown0x1E6;                 // 0x1E6
    u32 m_Unknown0x1E8;                // 0x1E8
    u8 m_Unknown0x1EC;                 // 0x1EC
    u8 m_Unknown0x1ED;                 // 0x1ED
    u8 m_Unknown0x1EE;                 // 0x1EE
    u8 m_Unknown0x1EF;                 // 0x1EF
    u8 m_Unknown0x1F0[6];              // 0x1F0
    u32 m_Unknown0x1F8[6];             // 0x1F8
    u32 m_Unknown0x210[6];             // 0x210
    u8 m_Unknown0x228;                 // 0x228
    u8 m_Unknown0x229;                 // 0x229
    u8 m_Unknown0x22A;                 // 0x22A
    u8 m_Unknown0x22B;                 // 0x22B
    u8 m_Unknown0x22C;                 // 0x22C
    u8 m_Unknown0x22D;                 // 0x22D
    u32 m_Unknown0x230;                // 0x230
    u32 m_Unknown0x234;                // 0x234
    u32 m_Unknown0x238;                // 0x238
    u8 m_Unknown0x23C;                 // 0x23C
    u8 m_Unknown0x23D;                 // 0x23D
    u32 m_Unknown0x240;                // 0x240
    u32 m_Unknown0x244;                // 0x244
    u32 m_Unknown0x248;                // 0x248
    u8 m_Unknown0x24C;                 // 0x24C
    u8 m_Unknown0x24D;                 // 0x24D
    u8 m_Unknown0x24E;                 // 0x24E
    u8 m_Unknown0x24F;                 // 0x24F
    u8 m_Unknown0x250;                 // 0x250
    u8 m_Unknown0x251;                 // 0x251
    u8 m_Unknown0x252;                 // 0x252
    u8 m_Unknown0x253;                 // 0x253
    u32 m_Unknown0x254;                // 0x254
    u32 m_Unknown0x258;                // 0x258
    u8 m_Unknown0x25C;                 // 0x25C
    u8 m_Unknown0x25D;                 // 0x25D
    u8 m_Unknown0x25E;                 // 0x25E
    u8 m_Unknown0x25F;                 // 0x25F
    u8 m_Unknown0x260;                 // 0x260
    u8 m_Unknown0x261;                 // 0x261
    u16 m_Unknown0x262;                // 0x262
    u16 m_Unknown0x264;                // 0x264
    u32 m_Unknown0x268;                // 0x268
    u32 m_Unknown0x26C;                // 0x26C
    u16 m_Unknown0x270;                // 0x270
    u16 m_Unknown0x272;                // 0x272
    u32 m_Unknown0x274[23];            // 0x274
    u32 m_Unknown0x2D0[23];            // 0x2D0
    u32 m_Unknown0x32C[23];            // 0x32C
    u32 m_Unknown0x388[23];            // 0x388
    u32 m_Unknown0x3E4[23];            // 0x3E4
    u8 m_Unknown0x440[23];             // 0x440
    u8 m_Unknown0x457[23];             // 0x457
    u8 m_Unknown0x46E[23];             // 0x46E
    u8 m_Unknown0x485[23];             // 0x485
    u8 m_Unknown0x49C;                 // 0x49C
    u8 m_Unknown0x49D;                 // 0x49D
    u16 m_Unknown0x49E;                // 0x49E
    u16 m_Unknown0x4A0;                // 0x4A0
    u16 m_Unknown0x4A2;                // 0x4A2
    u8 m_Unknown0x4A4[8];              // 0x4A4
    u32 m_Terminator;                  // 0x4AC
};
ASSERT_SIZE(SessionBeginMonitoringContent, 0x4B0);

// the monitoring data of the current session (written by all modules; name is ours)
extern SessionBeginMonitoringContent g_SessionBeginMonitoringContent;
} // namespace common
} // namespace pia
} // namespace nn
