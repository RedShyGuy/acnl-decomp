#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_SocketTransport.h"

// RTTI N2nn3nex15SocketTransport12TransportJobE @ 0x008CE4E4
// vtable 0x008FCF24 (vptr 0x008FCF2C), offset_to_top 0, 12 entries
class nn::nex::SocketTransport::TransportJob : public ::nn::nex::Job
{
public:
    TransportJob(); // ctor address unknown
    virtual ~TransportJob(); // 0x0037B8B0 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0037B884 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void Execute(); // 0x0037BEF4 slot 0x0C | slot vf_0x0C of nn::nex::Job
};
