#pragma once

#include "decomp.h"
#include "nn/nex/nex_LogicalClockTmpl.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
class Buffer;
class InetAddress;
class Packet;
class PacketEncDec;
class PRUDPMessageInterface;
class RootTransport;
class Time;

// A stream of single packets (no connection): the packets are signed and encoded like the ones of
// the PRUDP streams. The names of the members and of the functions without a reference name are
// ours.
// RTTI N2nn3nex15HighLevelStreamE @ 0x008CE4A8
// vtable 0x008FCEA0 (vptr 0x008FCEA8), offset_to_top 0, 7 entries
class HighLevelStream : public ::nn::nex::Stream
{
public:
    // the port of a stream that does not listen
    static const u8 NO_PORT = 0xFF;

    // pTransport: the transport of the stream (null: the one of the Network)
    HighLevelStream(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport); // 0x00378308
    virtual ~HighLevelStream(); // 0x0037846C slot 0x00
    // 0x003783E8 slot 0x04 (deleting dtor)
    virtual bool ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket); // 0x00377D88 slot 0x08
    virtual void DoWork(); // 0x003781D0 slot 0x0C
    // the payload of a received packet
    virtual void ProcessPayload(const nn::nex::InetAddress* pAddress, unsigned char sourceType, unsigned char sourcePort, nn::nex::Buffer* pPayload,
                                const nn::nex::Time& time, unsigned short port); // 0x00377D84 slot 0x14
    // the payload goes to the port of the stream type at the address
    virtual nn::nex::qResult Send(const nn::nex::InetAddress* pAddress, unsigned char destinationType, unsigned char destinationPort, nn::nex::Buffer* pPayload,
                                  unsigned char unknown, unsigned short port); // 0x00377EEC slot 0x18
    // the stream listens on the port of the transport; false if it listens already
    bool Listen(unsigned char port); // 0x003781D4

    u8 m_MessageVersion;                         // 0x05
    bool m_IsSigned;                             // 0x06
    bool m_IsHeaderSigned;                       // 0x07 (the signature method of the header, else the selected one)
    RootTransport* m_pTransport;                 // 0x08
    u8 m_Port;                                   // 0x0C (NO_PORT: none)
    PRUDPMessageInterface* m_pMessageInterface;  // 0x10 (the message selector of the Network)
    PacketEncDec* m_pEncDec;                     // 0x14 (with an encryption key)
    u8 m_SessionId;                              // 0x18
    LogicalClockTmpl<u16, s16> m_SequenceId;     // 0x1C
};
ASSERT_OFFSET(HighLevelStream, m_SequenceId, 0x1C);
ASSERT_SIZE(HighLevelStream, 0x24);
} // namespace nex
} // namespace nn
