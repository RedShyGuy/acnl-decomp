#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_ProtocolId.h"

namespace nn {
namespace pia {
namespace transport {
class RttProtocol;
class StationProtocol;
class StationProtocolReliable;

// The protocols of the stations themselves: the station protocol (port 0), its reliable part
// (port 1) and the round trip time protocol, created in the ProtocolManager of the Transport.
// Layout from CreateInstance and Initialize; the member names are ours.
class StationProtocolManager : public common::RootObject
{
public:
    static nn::Result CreateInstance(); // 0x0045BF54 | fefates:callgraph [tier C]
    static void DestroyInstance(); // 0x0045BFE4 | fefates:callgraph [tier C]

    // stationNum: the stations of the round trip time protocol (parameter name is ours)
    nn::Result Initialize(unsigned int stationNum); // 0x0045BDEC | fefates:callseq [tier C]
    void Finalize(); // 0x0045C044 | fefates:bytes [tier B]

    RttProtocol* GetRttProtocol(); // 0x0045BFC8 | fefates:bytes [tier B]
    StationProtocol* GetStationProtocol(); // 0x0045C00C | fefates:bytes [tier B]
    StationProtocolReliable* GetStationProtocolReliable(); // 0x0045C028 | fefates:bytes [tier B]

    static StationProtocolManager* s_pInstance;

    ProtocolId m_StationProtocolId;         // 0x0
    ProtocolId m_StationProtocolReliableId; // 0x4
    ProtocolId m_RttProtocolId;             // 0x8
};
ASSERT_SIZE(StationProtocolManager, 0xC);
} // namespace transport
} // namespace pia
} // namespace nn
