#pragma once

#include "decomp.h"
#include "nn/nex/nex_NetworkEmulator.h"
#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_PacketQueue.h"
#include "nn/nex/nex_Socket.h"
#include "nn/nex/nex_qChain.h"

namespace nn {
namespace nex {
class PRUDPMessageInterface;
class PacketIn;
class RootTransport;
class TransportBuffer;
class TransportStreamManager;

// A socket of a transport: the sent and received packets go through the network emulation if its
// devices are enabled, and the packets of a queue are bundled into one buffer. The names of the
// members and of the functions without a reference name are ours.
// RTTI N2nn3nex13QueuingSocketE @ 0x008CE260
// vtable 0x008FC8D4 (vptr 0x008FC8DC), offset_to_top 0, 9 entries
class QueuingSocket : public ::nn::nex::Socket
{
public:
    typedef qChain<Packet*, ChainPolicyHistoryPacket<Packet*> > HistoryChain;

    QueuingSocket(u32 context, nn::nex::RootTransport* pTransport, u32 maxPacketSize, nn::nex::TransportBuffer* pTransportBuffer); // 0x0036A8F0 | fefates:callgraph [tier C]
    virtual ~QueuingSocket(); // 0x0036AA1C slot 0x00 | fefates:callseq
    // 0x0036A9EC slot 0x04 (deleting dtor)
    virtual bool Bind(nn::nex::InetAddress* pAddress, bool unknown); // 0x0036A7BC slot 0x08
    // (no waiting receive)
    virtual s32 Receive(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x0036A840 slot 0x0C
    // from the input emulation if it is enabled
    virtual s32 ReceiveNonBlocking(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x00369624 slot 0x10
    // into the output emulation if it is enabled
    virtual s32 Send(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x0036A848 slot 0x14
    // true if there may be data to receive
    virtual bool CanReceive(); // 0x0072A994 slot 0x18
    // the send and the receive of the socket itself
    virtual s32 SendDirect(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x00369594 slot 0x1C
    virtual s32 ReceiveDirect(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x00369544 slot 0x20

    // the next packet of the buffer (the duplicates of the bundles that the streams already have are
    // skipped first); null if none (*pIsOutOfMemory is set if no packet could be made)
    nn::nex::PacketIn* ReadPacket(nn::nex::TransportStreamManager* pStreams, nn::nex::InetAddress* pAddress, nn::nex::Buffer* pBuffer, bool* pIsOutOfMemory); // 0x0036968C
    // the received data goes into the input emulation
    void ReceiveToEmulator(u32 bufferSize); // 0x00369A90
    // the packets of the queue (to the address of the first one) in one buffer of at most maxSize
    // bytes, with the packets of the history that fit; null if the first one is not due
    nn::nex::Buffer* CreateBundle(nn::nex::PacketQueue* pQueue, HistoryChain* pHistory, u32 maxSize); // 0x00369C78
    // the packets of the history of the id that are due are sent again (put in front of the queue)
    static void RetransmitFromHistoryPacketQueue(nn::nex::PacketQueue* pQueue, HistoryChain* pHistory, unsigned short id); // 0x0036A628 | fefates:bytes [tier B]

    u32 m_UseCount;                         // 0x44 (the listens of the port)
    NetworkEmulator m_OutputEmulator;       // 0x48
    NetworkEmulator m_InputEmulator;        // 0xA0
    u32 m_Unknown0xF8;                      // 0xF8
    PRUDPMessageInterface* m_pMessage;      // 0xFC (the message selector of the network)
    PacketQueue m_HistoryQueue;             // 0x100 (the packets of a bundle for the history)
    TransportBuffer* m_pTransportBuffer;    // 0x118
    bool m_UsesPacketBuffers;               // 0x11C
};
ASSERT_OFFSET(QueuingSocket, m_OutputEmulator, 0x48);
ASSERT_OFFSET(QueuingSocket, m_InputEmulator, 0xA0);
ASSERT_OFFSET(QueuingSocket, m_HistoryQueue, 0x100);
ASSERT_SIZE(QueuingSocket, 0x120);
} // namespace nex
} // namespace nn
