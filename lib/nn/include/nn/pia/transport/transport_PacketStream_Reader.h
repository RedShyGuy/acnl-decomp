#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_PacketStream.h"

class nn::pia::transport::PacketStream::Reader
{
public:
    void PullAll(); // 0x0044DB78 | fefates:bytes [tier B]
    void PullOne(); // 0x0044DBA8 | fefates:bytes [tier B]
};
