#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallProtocolMethodOperation.h"
#include "nn/nex/nex_JobProcessProtocolEvent.h"

namespace nn {
namespace nex {
class Buffer;

// The job of a protocol broker for a message of an end point: the broker processes it (the job
// waits as long as the operation says). The names of the members are ours.
// RTTI N2nn3nex25JobProcessProtocolMessageE @ 0x008CEEA0
// vtable 0x008FE9E4 (vptr 0x008FE9EC), offset_to_top 0, 13 entries
class JobProcessProtocolMessage : public ::nn::nex::JobProcessProtocolEvent
{
public:
    JobProcessProtocolMessage(nn::nex::ProtocolRequestBrokerInterface* pBroker, nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pData); // 0x003B6FBC | fefates:callgraph [tier C]
    virtual ~JobProcessProtocolMessage(); // 0x003B712C slot 0x00
    // 0x003B708C slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Execute(); // 0x003B6F70 slot 0x0C | fefates:bytes
    virtual void vf_0x30(); // 0x003B6F6C slot 0x30

    CallProtocolMethodOperation m_Operation; // 0x58 (its message is the data)
};
ASSERT_SIZE(JobProcessProtocolMessage, 0x88);
} // namespace nex
} // namespace nn
