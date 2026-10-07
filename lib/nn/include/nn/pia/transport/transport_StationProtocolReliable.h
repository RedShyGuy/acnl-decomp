#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace common {
class StationAddress;
}
namespace transport {
class ProtocolEvent;

// RTTI N2nn3pia9transport23StationProtocolReliableE @ 0x008D0278
// vtable 0x00901FB8 (vptr 0x00901FC0), offset_to_top 0, 9 entries
//
// The reliable stream of the station messages: the ReliableSlidingWindow of each station is
// started when it joins; StationProtocol reads the messages (Receive). The member names are ours.
class StationProtocolReliable : public ::nn::pia::transport::Protocol
{
public:
    StationProtocolReliable(); // 0x0045D388 | fefates:bytes [tier B]
    virtual ~StationProtocolReliable(); // 0x0045D454 slot 0x00
    // 0x0045D444 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00736778 slot 0x08
    virtual u16 GetProtocolType() const; // 0x00736770 slot 0x0C
    virtual nn::Result Startup(nn::pia::StationIndex localStationIndex); // 0x0045D2DC slot 0x10 | fefates:bytes
    virtual void Cleanup(); // 0x0045D1A0 slot 0x14
    virtual nn::Result Dispatch(); // 0x0045D2F4 slot 0x18 | fefates:bytes
    virtual nn::Result UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event); // 0x0045D0E8 slot 0x1C | fefates:bytes

    // the next message of the station
    nn::Result Receive(nn::pia::StationIndex stationIndex, unsigned int bufferSize, unsigned char* pBuffer, unsigned int* pSize, nn::pia::common::StationAddress* pAddress); // 0x0045D1AC | fefates:bytes [tier B]
    // the received messages into the windows of their stations
    nn::Result receiveProc(); // 0x0045CF6C | fefates:callseq [tier C]

    StationIndex m_LocalStationIndex; // 0x14
};
ASSERT_SIZE(StationProtocolReliable, 0x18);
} // namespace transport
} // namespace pia
} // namespace nn
