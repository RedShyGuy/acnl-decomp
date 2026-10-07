#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace transport {
// A received message as the protocols parse it: the payload and where it comes from, filled from
// a ProtocolMessageReader or from the reliable stream (StationProtocol::Dispatch). The type name is
// from the fefates symbols (the parameter of the Parse* functions); the members are ours.
struct ReceivedMessageAccessor
{
    const u8* m_pData;                       // 0x00, the payload
    u32 m_Size;                              // 0x04
    StationIndex m_SourceStationIndex;       // 0x08
    common::StationAddress m_SourceAddress;  // 0x0C
    u32 m_SourceStationKey;                  // 0x1C (relayed messages)
    u8 m_ConnectionId;                       // 0x20, byte 5 of the packet
};
ASSERT_OFFSET(ReceivedMessageAccessor, m_SourceAddress, 0x0C);
ASSERT_OFFSET(ReceivedMessageAccessor, m_ConnectionId, 0x20);
} // namespace transport
} // namespace pia
} // namespace nn
