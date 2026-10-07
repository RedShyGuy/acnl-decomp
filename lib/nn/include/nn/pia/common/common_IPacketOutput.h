#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
class Packet;

// RTTI N2nn3pia6common13IPacketOutputE @ 0x008CFE30
//
// Interface of the streams packets are written to (inet::SocketOutputStream,
// local::LocalOutputStream). It has no code of its own: the six slots of the implementations
// are this-adjusting thunks. Write and OnStationConnectionEvent are named after the implementations.
class IPacketOutput
{
public:
    virtual ~IPacketOutput() {}                                         // slot 0x00, 0x04 (deleting)
    // the stations changed (transport::Transport::OutputStreamUpdateEvent)
    virtual nn::Result OnStationConnectionEvent() = 0;                  // slot 0x08
    virtual nn::Result Write(const nn::pia::common::Packet& packet) = 0; // slot 0x0C
    // (StationPacketHandler::Initialize; names are ours)
    virtual u32 GetDestinationNumMax() = 0;                             // slot 0x10
    virtual bool IsBroadcast() = 0;                                     // slot 0x14
};
} // namespace common
} // namespace pia
} // namespace nn
