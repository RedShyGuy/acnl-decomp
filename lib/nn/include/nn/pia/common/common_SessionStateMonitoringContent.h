#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_MonitoringData.h"

namespace nn {
namespace pia {
namespace common {
// The monitoring data of the state of a session (block type 1, sent with the session end). The
// layout is the one of Serialize (the fields in order, each aligned); most members are not named
// yet. The class and Cleanup are from the fefates symbols, the rest is ours.
class SessionStateMonitoringContent : public MonitoringContent
{
public:
    // all fields 0xFF, the header set
    void Initialize(); // 0x004286D4
    // the fields of the last session invalid again
    void Cleanup(); // 0x0042871C | fefates:callseq-callee [tier C]
    DECOMP_NOINLINE static unsigned int GetSerializedSize(); // 0x00428710
    nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00732A28

    u32 m_DispatchCount;               // 0x010
    u32 m_Unknown0x14;                 // 0x014
    u32 m_Unknown0x18;                 // 0x018
    u32 m_Unknown0x1C;                 // 0x01C
    // the packets of the transport (TransportAnalyzer::SetMonitoringData): [0] all of them
    // (protocol id 0), then the first 15 protocols
    u32 m_SendProtocolIds[16];             // 0x020
    u32 m_SendPacketNums[16];             // 0x060
    u64 m_SendSizes[16];             // 0x0A0
    u32 m_ReceiveProtocolIds[16];            // 0x120
    u32 m_ReceivePacketNums[16];            // 0x160
    u64 m_ReceiveSizes[16];            // 0x1A0
    // the stations (TransportAnalyzer::SetMonitoringData): the MD5 hash of the principal id, the
    // round trip time and the lost packets in 0.01 %
    u32 m_StationPrincipalIdHashes[23];            // 0x220
    u8 m_StationIndices[23];             // 0x27C
    u16 m_StationRtts[23];            // 0x294
    u16 m_StationPacketLosses[23];            // 0x2C2
    // the wait of the stream threads when idle (1..254, else 255; transport::ThreadStreamManager)
    u8 m_SendThreadWaitMSec;                 // 0x2F0
    u8 m_ReceiveThreadWaitMSec;                 // 0x2F1
    // as in SessionBeginMonitoringContent (transport::Transport::SetMonitoringNetworkRtt)
    u16 m_MinRtt;                // 0x2F2
    u32 m_MinRttPrincipalIdHash;                // 0x2F4
    u16 m_MaxRtt;                // 0x2F8
    u32 m_MaxRttPrincipalIdHash;                // 0x2FC
    // received packets with a wrong signature (transport::ReceiveThreadStream)
    u32 m_SignatureErrorNum;                // 0x300
    // the sends of transport::ReliableProtocol that found the window full
    u32 m_ReliableBufferFullNum;                // 0x304
    // a large reliable transfer to the stations 0..3: its size and its duration in ms
    // (transport::ReliableSlidingWindow)
    u32 m_ReliableTransferSize[4];     // 0x308
    u32 m_ReliableTransferMSec[4];     // 0x318
    u32 m_Unknown0x328;                // 0x328
    u32 m_Unknown0x32C;                // 0x32C
    u8 m_Unknown0x330;                 // 0x330
    u32 m_Unknown0x334;                // 0x334
    u8 m_Unknown0x338;                 // 0x338
    u8 m_Unknown0x339;                 // 0x339
    u32 m_Unknown0x33C;                // 0x33C
    u32 m_Unknown0x340;                // 0x340
    u16 m_Unknown0x344;                // 0x344
    u16 m_Unknown0x346;                // 0x346
    u32 m_Unknown0x348;                // 0x348
    u8 m_Unknown0x34C;                 // 0x34C
    u8 m_Unknown0x34D;                 // 0x34D
    u8 m_Unknown0x34E;                 // 0x34E
    u8 m_Unknown0x34F;                 // 0x34F
    u8 m_Unknown0x350;                 // 0x350
    u8 m_Unknown0x351;                 // 0x351
    u32 m_Unknown0x354;                // 0x354
    u8 m_Unknown0x358;                 // 0x358
    u32 m_Unknown0x35C;                // 0x35C
    u8 m_Unknown0x360;                 // 0x360
    u8 m_Unknown0x361;                 // 0x361
    u32 m_Unknown0x364;                // 0x364
    u32 m_Unknown0x368;                // 0x368
    u8 m_Unknown0x36C[4];              // 0x36C
    u8 m_Unknown0x370[5];              // 0x370
    u32 m_Unknown0x378;                // 0x378
    u16 m_Unknown0x37C;                // 0x37C
    u8 m_Unknown0x37E;                 // 0x37E
    u32 m_Unknown0x380;                // 0x380
    u32 m_Unknown0x384;                // 0x384
    // the most resends of a reliable message (0xFFFF: none yet)
    u16 m_ReliableResendCountMax;      // 0x388
    u8 m_Unknown0x38A;                 // 0x38A
    u16 m_Unknown0x38C;                // 0x38C
    u16 m_Unknown0x38E;                // 0x38E
    u32 m_Unknown0x390;                // 0x390
    u16 m_Unknown0x394;                // 0x394
    u16 m_Unknown0x396;                // 0x396
    u8 m_Unknown0x398;                 // 0x398
    u32 m_Unknown0x39C;                // 0x39C
    u32 m_Unknown0x3A0;                // 0x3A0
    u32 m_Unknown0x3A4;                // 0x3A4
    u32 m_Unknown0x3A8;                // 0x3A8
    u64 m_Unknown0x3B0;                // 0x3B0
    u64 m_Unknown0x3B8;                // 0x3B8
    u8 m_Unknown0x3C0;                 // 0x3C0
    u8 m_Unknown0x3C1;                 // 0x3C1
    u8 m_Unknown0x3C2;                 // 0x3C2
    u8 m_Unknown0x3C3;                 // 0x3C3
    u32 m_Unknown0x3C4;                // 0x3C4
    u8 m_Unknown0x3C8;                 // 0x3C8
    u8 m_Unknown0x3C9;                 // 0x3C9
    u8 m_Unknown0x3CA;                 // 0x3CA
    u8 m_Unknown0x3CB;                 // 0x3CB
    u8 m_Unknown0x3CC;                 // 0x3CC
    u8 m_Unknown0x3CD;                 // 0x3CD
    u32 m_Unknown0x3D0;                // 0x3D0
    u8 m_Unknown0x3D4;                 // 0x3D4
    u8 m_Unknown0x3D5;                 // 0x3D5
    u8 m_Unknown0x3D6;                 // 0x3D6
    u8 m_Unknown0x3D7;                 // 0x3D7
    u8 m_Unknown0x3D8;                 // 0x3D8
    u8 m_Unknown0x3D9;                 // 0x3D9
    u16 m_Unknown0x3DA;                // 0x3DA
    u16 m_Unknown0x3DC;                // 0x3DC
    u8 m_Unknown0x3DE[12];             // 0x3DE
    u32 m_Terminator;                  // 0x3EC
};
ASSERT_SIZE(SessionStateMonitoringContent, 0x3F0);

// the monitoring data of the current session (name is ours)
extern SessionStateMonitoringContent g_SessionStateMonitoringContent;
} // namespace common
} // namespace pia
} // namespace nn
