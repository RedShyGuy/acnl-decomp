#pragma once

#include "decomp.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_RoutingTable.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_UserContext.h"

namespace nn {
namespace nex {
class Buffer;
class PacketOut;
class RootTransport;

// The stream of the relay messages (port 15 of the transport with the relay enabled): relayed data
// goes to the transport, the pings to the callback. The names of the members and of the functions
// without a reference name are ours.
// RTTI N2nn3nex11RelayStreamE @ 0x008CE074
// vtable 0x008FC420 (vptr 0x008FC428), offset_to_top 0, 5 entries
class RelayStream : public ::nn::nex::Stream
{
public:
    // the relay types (low 4 bits of the relay header flags)
    enum RelayType : u8
    {
        RELAY_TYPE_DATA = 0,
        RELAY_TYPE_PING = 1
    };
    // receives a ping: the source, the port, the time stamp, the ping payload and the data
    typedef void (*PingCallback)(UserContext* pContext, const InetAddress* pSource, unsigned short port, unsigned int timeStamp, u8* pPing, Buffer* pData);

    // the type of the stream
    static const u32 TYPE = 11;

    // (inline in the constructor of SocketTransport)
    RelayStream()
        : Stream(static_cast<Type>(TYPE)), m_pTransport(nullptr), m_RoutingTable(GetSettings(static_cast<Type>(1))->m_MaxSilenceTime * 3), m_Unknown0x40(-1),
          m_Unknown0x44(-1), m_pPingCallback(nullptr)
    {
        m_PingContext.m_Value = 0;
    }
    virtual ~RelayStream(); // 0x0035B9A4 slot 0x00
    // 0x0035B8B4 (deleting dtor)
    virtual bool ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket); // 0x0035B774 slot 0x08 | fefates:bytes
    virtual void DoWork(); // 0x0036037C slot 0x0C

    // a packet of the relay type with the payload (from the pools if the payload is)
    static nn::nex::PacketOut* CreatePacket(nn::nex::Buffer* pPayload, unsigned char relayType); // 0x0035B694 (name is ours)

    RootTransport* m_pTransport;   // 0x08
    u32 m_Unknown0x0C;             // 0x0C
    RoutingTable m_RoutingTable;   // 0x10
    s32 m_Unknown0x40;             // 0x40 (sent in the relay header)
    s32 m_Unknown0x44;             // 0x44 (sent in the relay header)
    PingCallback m_pPingCallback;  // 0x48
    UserContext m_PingContext;     // 0x4C
};
ASSERT_OFFSET(RelayStream, m_RoutingTable, 0x10);
ASSERT_SIZE(RelayStream, 0x50);
} // namespace nex
} // namespace nn
