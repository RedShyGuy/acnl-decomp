#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
class RttCalculator;

// RTTI N2nn3pia9transport11RttProtocolE @ 0x008D0158
// vtable 0x00901D48 (vptr 0x00901D50), offset_to_top 0, 9 entries
//
// Measures the round trip time to every station: a request with the send time goes out when a
// station joins and when the last answer is too old, the station answers with the same time.
// Layout from the constructor; the member names, the data types and SendRequest are ours.
class RttProtocol : public ::nn::pia::transport::Protocol
{
public:
    // the type name is from the signature of send; the members are ours (big endian in the
    // message)
    struct Data
    {
        u32 m_Type;     // 0x0, DATA_TYPE_*
        u32 m_Reserved; // 0x4
        u64 m_Time;     // 0x8, the ticks of the request
    };

    static const u32 DATA_TYPE_REQUEST = 0;
    static const u32 DATA_TYPE_RESPONSE = 1;

    RttProtocol(); // 0x0044DA6C | fefates:bytes [tier B]
    virtual ~RttProtocol(); // 0x0044DAA8 slot 0x00
    // 0x0044DA98 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00734B78 slot 0x08
    virtual u16 GetProtocolType() const; // 0x00734B70 slot 0x0C
    virtual nn::Result Startup(nn::pia::StationIndex stationIndex); // 0x0044D72C slot 0x10
    virtual void Cleanup(); // 0x0044D6D0 slot 0x14 | fefates:bytes
    virtual nn::Result Dispatch(); // 0x0044D778 slot 0x18 | fefates:callseq
    virtual nn::Result UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event); // 0x0044D4A4 slot 0x1C

    // a calculator for each of stationNum stations
    nn::Result Initialize(unsigned int stationNum); // 0x0044D3FC (name is ours)
    void Finalize(); // 0x0044DA2C | fefates:bytes [tier B]

    // the median of the round trip times to the station (the latest num), -1 without one
    s32 GetRtt(nn::pia::StationIndex stationIndex) const; // 0x00734B7C | fefates:bytes [tier B]
    s32 GetRtt(nn::pia::StationIndex stationIndex, unsigned int num) const; // 0x00734BA4 | fefates:bytes [tier B]

    nn::Result send(nn::pia::StationIndex stationIndex, const nn::pia::transport::RttProtocol::Data& data); // 0x0044D5C0 | fefates:bytes [tier B]
    // a request to the station (inline)
    void SendRequest(StationIndex stationIndex);

    StationIndex m_LocalStationIndex; // 0x14
    RttCalculator* m_pCalculators;    // 0x18
    u32 m_StationNum;                 // 0x1C
};
ASSERT_SIZE(RttProtocol, 0x20);
} // namespace transport
} // namespace pia
} // namespace nn
