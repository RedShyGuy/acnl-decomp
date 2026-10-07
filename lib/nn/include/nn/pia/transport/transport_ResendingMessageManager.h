#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_ProtocolId.h"

namespace nn {
namespace pia {
namespace common {
class StationAddress;
}
namespace transport {
class PacketHandler;

// RTTI N2nn3pia9transport23ResendingMessageManagerE @ 0x008D026C
// vtable 0x00901FAC (vptr 0x00901FB4), offset_to_top 0, 1 entries
//
// Messages that are sent again every 500 ms until they are acknowledged (StopResending with the
// ack id appended to them) or time out. Layout from CreateInstance and Initialize; the member names
// are ours. The destructor is not virtual (the vtable only has Trace).
class ResendingMessageManager : public ::nn::pia::common::RootObject
{
public:
    // the largest message (with the ack id)
    static const u32 MESSAGE_SIZE_MAX = 0x596;

    // (inline in CreateInstance)
    ResendingMessageManager()
        : m_pSendTimes(nullptr), m_pTimeouts(nullptr), m_pAckIds(nullptr), m_pMessages(nullptr), m_pMessageSizes(nullptr),
          m_pStationIndices(nullptr), m_pStationAddresses(nullptr), m_pProtocolIds(nullptr), m_NextAckId(0), m_MessageNumMax(0)
    {
    }
    virtual void Trace(u64 flag) const; // 0x0073676C slot 0x00

    static nn::Result CreateInstance(); // 0x0045CA08 | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x0045CBEC | fefates:bytes [tier B]
    nn::Result Initialize(unsigned int messageNumMax); // 0x0045C6B0 | fefates:bytes [tier B]
    void Finalize(); // 0x0045CE04 | fefates:bytes [tier B]
    nn::Result Startup(nn::pia::transport::PacketHandler* pPacketHandler); // 0x0045CC44 | fefates:bytes [tier B]
    void Cleanup(); // 0x0045CC30 | fefates:bytes [tier B]
    nn::Result Dispatch(); // 0x0045CC68 | fefates:bytes [tier B]

    // the message to the station (by its index, else by its address) until timeout (ticks, 0: no
    // timeout); pAckId: the ack id appended to it
    nn::Result SetSendMessage(unsigned int* pAckId, const unsigned char* pData, unsigned int size, nn::pia::StationIndex stationIndex, const nn::pia::common::StationAddress& address, nn::pia::transport::ProtocolId protocolId, long long timeout); // 0x0045CA80 | fefates:bytes [tier B]
    bool StopResending(unsigned int ackId); // 0x0045C988 | fefates:bytes [tier B]
    bool CheckNowResending(unsigned int ackId) const; // 0x007366C0 | fefates:bytes [tier B]
    // the ack id at the end of a received message (0 if it is too short)
    u32 ExtractAckIdFromMessage(const unsigned char* pData, unsigned int size) const; // 0x00736750 | fefates:bytes [tier B]

    static ResendingMessageManager* s_pInstance;

    s64* m_pSendTimes;                         // 0x004, the next time the message is sent
    s64* m_pTimeouts;                          // 0x008
    u32* m_pAckIds;                            // 0x00C, 0: a free entry
    u8** m_pMessages;                          // 0x010
    u32* m_pMessageSizes;                      // 0x014
    StationIndex* m_pStationIndices;           // 0x018
    common::StationAddress* m_pStationAddresses; // 0x01C
    ProtocolId* m_pProtocolIds;                // 0x020
    PacketHandler* m_pPacketHandler;           // 0x024
    u32 m_NextAckId;                           // 0x028
    u8 m_Unknown0x2C[0x5C4 - 0x2C];            // 0x02C (not used here)
    u32 m_MessageNumMax;                       // 0x5C4
};
ASSERT_OFFSET(ResendingMessageManager, m_NextAckId, 0x28);
ASSERT_SIZE(ResendingMessageManager, 0x5C8);
} // namespace transport
} // namespace pia
} // namespace nn
