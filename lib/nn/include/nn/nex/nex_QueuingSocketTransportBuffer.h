#pragma once

#include "decomp.h"
#include "nn/nex/nex_QueuingSocket.h"
#include "nn/nex/nex_TransportBufferSet.h"

namespace nn {
namespace nex {
class Condition;

// A queuing socket whose sends and receives go through buffer sets that a transport buffer thread
// sends and fills. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex28QueuingSocketTransportBufferE @ 0x008CF0B4
// vtable 0x008FF098 (vptr 0x008FF0A0), offset_to_top 0, 9 entries
class QueuingSocketTransportBuffer : public ::nn::nex::QueuingSocket
{
public:
    // (inline in SocketTransport)
    QueuingSocketTransportBuffer(u32 context, nn::nex::RootTransport* pTransport, u32 maxPacketSize, nn::nex::TransportBuffer* pTransportBuffer, u32 bufferCount,
                                 u32 receiveCount, u32 sendCount);
    virtual ~QueuingSocketTransportBuffer(); // 0x003BE368 slot 0x00
    // 0x003BE2E8 slot 0x04 (deleting dtor)
    // true if the input emulation is enabled or a received buffer is waiting
    virtual bool CanReceive(); // 0x0072DE44 slot 0x18
    // the buffer goes into the send set
    virtual s32 SendDirect(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x003BDF00 slot 0x1C
    // the first buffer of the receive set
    virtual s32 ReceiveDirect(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x003BDDFC slot 0x20

    // the buffers of the send set are sent (at most m_SendCount); false if one failed
    bool SendBuffers(); // 0x003BDFDC
    // received data goes into the receive set (at most m_ReceiveCount buffers); false if none came
    bool ReceiveBuffers(bool isNonBlocking); // 0x003BE178

    TransportBufferSet m_ReceiveSet; // 0x120
    Buffer* m_pReceiveBuffer;        // 0x19C
    TransportBufferSet m_SendSet;    // 0x1A0
    u32 m_ReceiveCount;              // 0x21C
    u32 m_SendCount;                 // 0x220
    Condition* m_pCondition;         // 0x224 (signalled after a send was queued)
};
ASSERT_OFFSET(QueuingSocketTransportBuffer, m_ReceiveSet, 0x120);
ASSERT_OFFSET(QueuingSocketTransportBuffer, m_SendSet, 0x1A0);
ASSERT_SIZE(QueuingSocketTransportBuffer, 0x228);
} // namespace nex
} // namespace nn
