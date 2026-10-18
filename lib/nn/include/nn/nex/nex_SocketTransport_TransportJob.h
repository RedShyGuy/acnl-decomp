#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_SocketTransport.h"

// The regular job of a socket transport (a round of DoWork). The member names are ours.
// RTTI N2nn3nex15SocketTransport12TransportJobE @ 0x008CE4E4
// vtable 0x008FCF24 (vptr 0x008FCF2C), offset_to_top 0, 12 entries
class nn::nex::SocketTransport::TransportJob : public ::nn::nex::Job
{
public:
    // (inline in StartThreads)
    explicit TransportJob(SocketTransport* pTransport) : Job(JOB_TYPE_DEFAULT, DebugString(String(L"SocketTransport::TransportJob"))), m_pTransport(pTransport) {}
    virtual ~TransportJob(); // 0x0037B8B0 slot 0x00
    // 0x0037B884 slot 0x04 (deleting dtor)
    virtual void Execute(); // 0x0037BEF4 slot 0x0C

    SocketTransport* m_pTransport; // 0x3C
};
ASSERT_SIZE(nn::nex::SocketTransport::TransportJob, 0x40);
