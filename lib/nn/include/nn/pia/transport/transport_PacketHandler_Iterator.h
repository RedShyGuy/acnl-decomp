#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_PacketHandler.h"

class nn::pia::transport::PacketHandler::Iterator
{
public:
    void GetMessageReader() const; // 0x00734D3C | fefates:bytes [tier B]
};
