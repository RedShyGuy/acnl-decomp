#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_ProtocolId.h"

namespace nn {
namespace pia {
namespace transport {
// Packet counters per protocol and in total (PacketAnalyzer counts into it, Print writes it with
// the AnalysisPrinter). The layout is from ClearExceptName and PacketAnalyzer::
// UpdatePacketAnalysisData; the member names, ProtocolData and AddProtocol are ours.
class PacketAnalysisData
{
public:
    static const u32 PROTOCOL_NUM_MAX = 32;
    static const u32 NAME_LENGTH = 32;

    // the counters of one protocol: the packets and bytes since the last ClearCounters and in
    // total
    struct ProtocolData
    {
        ProtocolData(); // 0x00457C08 (name is ours)

        ProtocolId m_ProtocolId; // 0x00
        u32 m_PacketNum;         // 0x04
        u32 m_Size;              // 0x08
        u32 m_TotalPacketNum;    // 0x0C
        u64 m_TotalSize;         // 0x10
    };

    void ClearCounters(); // 0x00457B54 | fefates:bytes [tier B]
    void ClearExceptName(); // 0x00457B94 | fefates:bytes [tier B]
    // printAll: also the protocols of pia itself
    void Print(bool printAll) const; // 0x00735C0C | fefates:bytes [tier B]
    // the index of the protocol, -1 if it has no counters
    s32 GetIndex(nn::pia::transport::ProtocolId protocolId) const; // 0x00736014 | fefates:bytes [tier B]
    // counters for a new protocol, -1 if there is no room
    s32 AddProtocol(nn::pia::transport::ProtocolId protocolId); // 0x00457B28 (name is ours)

    ProtocolData m_ProtocolData[PROTOCOL_NUM_MAX]; // 0x000
    u32 m_ProtocolNum;                             // 0x300
    // the time since the start of the analysis (set by PacketAnalyzer::GetPacketAnalysisData)
    s32 m_ElapsedMSec;                             // 0x304
    char m_Name[NAME_LENGTH];                      // 0x308
    u32 m_PacketNum;                               // 0x328
    u32 m_Size;                                    // 0x32C
    u32 m_TotalPacketNum;                          // 0x330
    u64 m_TotalSize;                               // 0x338
};
ASSERT_OFFSET(PacketAnalysisData, m_ProtocolNum, 0x300);
ASSERT_OFFSET(PacketAnalysisData, m_PacketNum, 0x328);
ASSERT_SIZE(PacketAnalysisData, 0x340);
} // namespace transport
} // namespace pia
} // namespace nn
