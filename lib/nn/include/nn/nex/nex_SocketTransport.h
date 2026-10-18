#pragma once

#include "decomp.h"
#include "nn/nex/nex_InterfaceInfo.h"
#include "nn/nex/nex_PacketQueue.h"
#include "nn/nex/nex_ProfilingUnit.h"
#include "nn/nex/nex_QueuingSocket.h"
#include "nn/nex/nex_Relay.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Router.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qSortedVector.h"

namespace nn {
namespace nex {
class TransportBuffer;
class TransportBufferThreadInterface;

// A transport over the sockets of the socket driver (one queuing socket per port). The names of the
// members and of the functions without a reference name are ours.
// RTTI N2nn3nex15SocketTransportE @ 0x008CE4F0
// vtable 0x008FCF5C (vptr 0x008FCF64), offset_to_top 0, 17 entries
class SocketTransport : public ::nn::nex::RootTransport
{
public:
    class TransportJob;

    // the interfaces of the console (their array is made with new[])
    struct InterfaceInfoArray
    {
        // (inline)
        InterfaceInfoArray() : m_Count(0), m_pInfos(nullptr) {}
        ~InterfaceInfoArray()
        {
            m_Count = 0;
            if (m_pInfos != nullptr) {
                delete[] m_pInfos;
                m_pInfos = nullptr;
            }
        }

        u32 m_Count;               // 0x00
        InterfaceInfo* m_pInfos;   // 0x04
    };

    // the bytes of the headers of a relayed and of a routed packet
    static const u32 RELAY_HEADER_SIZE = 44;
    static const u32 ROUTING_HEADER_SIZE = 24;
    // the addresses of a send to port 0
    static const u32 MAX_SEND_MULTI_ADDRESSES = 15;
    // the time between the retransmissions of the bundling history (milliseconds)
    static const s32 RETRANSMIT_INTERVAL = 15;

    SocketTransport(); // 0x0037D124
    virtual ~SocketTransport(); // 0x0037D534 slot 0x00
    // 0x0037D500 slot 0x04 (deleting dtor)
    virtual nn::nex::qResult Start(); // 0x0037B374 slot 0x08
    virtual bool StartListen(unsigned short port, unsigned short* pPort, bool isNew, unsigned int context, bool usesPacketBuffers); // 0x0037B85C slot 0x0C | fefates:bytes
    virtual bool StartListenUnknown(unsigned short port, unsigned short* pPort, bool isNew, unsigned int context, bool usesPacketBuffers); // 0x0037BB6C slot 0x10
    virtual bool StopListen(unsigned short port); // 0x0037B3A0 slot 0x14
    virtual bool StopListenAll(); // 0x0037B7D8 slot 0x18
    virtual u32 GetPortCount(); // 0x0037C498 slot 0x1C
    virtual bool SendPacket(unsigned short port, unsigned char streamType, unsigned char sourcePort, unsigned char destinationPort, nn::nex::PacketOut* pPacket,
                            bool unknown); // 0x0037CE24 slot 0x20
    virtual bool Receive(unsigned short port, nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress); // 0x0037CF98 slot 0x24 | mk7dlp:bytes
    virtual Router* GetRouter(); // 0x0037CFDC slot 0x28
    virtual Relay* GetRelay(); // 0x0037CFD4 slot 0x2C
    virtual void* vf_0x30(); // 0x0072B6D0 slot 0x30
    virtual void* vf_0x34(); // 0x0072B6D8 slot 0x34
    virtual nn::nex::qResult SendRaw(nn::nex::StationURL* pURL, nn::nex::Buffer* pBuffer); // 0x0037CC38 slot 0x38
    virtual void DropPoolPackets(); // 0x0037CA5C slot 0x3C
    virtual void RequestStop(); // 0x0037C718 slot 0x40

    // a socket on the port (port 0: a free port; *pPort: the port, 0 if it failed)
    bool CreateSocket(unsigned short port, unsigned short* pPort, unsigned int context, bool usesPacketBuffers, bool unknown); // 0x0037AFD4
    bool StartListenImpl(unsigned short port, unsigned short* pPort, bool isNew, unsigned int context, bool usesPacketBuffers, bool unknown); // 0x0037B8C8 | fefates:callgraph
    // the job of the transport and the transport buffer thread are started
    void StartThreads(); // 0x0037BB94 (symbols.json names it the constructor of ObjectThread<TransportBufferThread, void*>)
    // a round of the transport: receive, the streams, send
    void DoWork(); // 0x0037BEFC
    // the packets of the bundle queue are bundled and sent (and the history is retransmitted)
    void SendQueuedPackets(); // 0x0037C090
    // the sockets are read into the received packets
    void ReceivePackets(); // 0x0037C4E8
    // the packets of the buffer go to the received packets; false if a data handler took the buffer
    bool FillPacketQueueFromBuffer(nn::nex::QueuingSocket* pSocket, nn::nex::Buffer* pBuffer, const nn::nex::InetAddress* pAddress); // 0x0037C724 | mk7dlp:callgraph [tier A]
    // the buffer goes to the addresses of a send to port 0 that are routed or relayed; false if one
    // of them is neither
    bool SendMulti(nn::nex::Buffer* pBuffer, unsigned short port); // 0x0037C890
    QueuingSocket* GetSocket(unsigned short port); // 0x0037CFE4 | mk7dlp:callgraph [tier A]

    TransportBufferThreadInterface* m_pBufferThread;     // 0x138
    TransportJob* m_pJob;                                 // 0x13C
    qSortedVector<u16, QueuingSocket*> m_Sockets;         // 0x140
    bool m_IsStopping;                                    // 0x150
    InterfaceInfoArray m_Interfaces;                      // 0x154
    PacketQueue m_ReceivedPackets;                        // 0x15C
    PacketQueue* m_pBundleQueue;                          // 0x174 (a ProtectedPacketQueue if nex is thread safe)
    QueuingSocket::HistoryChain m_History;                // 0x178
    u32 m_Unknown0x18C;                                   // 0x18C
    ProfilingUnit m_ProfilingUnit;                        // 0x190
    u32 m_Context;                                        // 0x200 (the pseudo singleton context of the creator)
    Router m_Router;                                      // 0x204
    Relay m_Relay;                                        // 0x214
    Time m_NextRetransmitTime;                            // 0x248
    TransportBuffer* m_pTransportBuffer;                  // 0x250
    u32 m_SentCount;                                      // 0x254
    u32 m_ReceivedCount;                                  // 0x258
};
ASSERT_OFFSET(SocketTransport, m_Sockets, 0x140);
ASSERT_OFFSET(SocketTransport, m_ReceivedPackets, 0x15C);
ASSERT_OFFSET(SocketTransport, m_History, 0x178);
ASSERT_OFFSET(SocketTransport, m_ProfilingUnit, 0x190);
ASSERT_OFFSET(SocketTransport, m_Router, 0x204);
ASSERT_OFFSET(SocketTransport, m_Relay, 0x214);
ASSERT_OFFSET(SocketTransport, m_NextRetransmitTime, 0x248);
ASSERT_SIZE(SocketTransport, 0x260);
} // namespace nex
} // namespace nn
