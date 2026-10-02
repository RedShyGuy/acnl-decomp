#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_PacketStream.h"

class nn::pia::transport::PacketStream::Accessor
{
public:
    void Get(int); // 0x0044DD70 | fefates:bytes [tier B]
};
