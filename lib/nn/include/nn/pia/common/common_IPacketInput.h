#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
class Packet;

// RTTI N2nn3pia6common12IPacketInputE @ 0x008CFE28
//
// Interface of the streams packets are read from (inet::SocketInputStream,
// local::LocalInputStream). It has no code of its own: the three slots of the implementations
// are this-adjusting thunks. Read is named after the implementations.
class IPacketInput
{
public:
    virtual ~IPacketInput() {}                                  // slot 0x00, 0x04 (deleting)
    virtual nn::Result Read(nn::pia::common::Packet* pPacket) = 0; // slot 0x08
};
} // namespace common
} // namespace pia
} // namespace nn
