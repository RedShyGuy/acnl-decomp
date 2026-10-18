#pragma once

#include "decomp.h"
#include "nn/nex/nex_Protocol.h"

namespace nn {
namespace nex {
class Message;

// (only the function the jobs use; the class name is from the reference symbols)
class ProtocolRequestBroker
{
public:
    // the protocol key of a message to the protocol of the type (it falls into AddProtocolKey)
    // (the reference name has an unsigned char protocol id; the callers pass the halfword of m_ProtocolId)
    static void InitMessage(nn::nex::Message* pMessage, unsigned short protocolId, nn::nex::Protocol::_Type type); // 0x003D5A4C | fefates:callgraph [tier C]
};
} // namespace nex
} // namespace nn
