#pragma once

#include "decomp.h"
#include "nn/nex/nex_LiteBuffer.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// The message format of the relay (stream type 11): the magic 0xF5 0xD0, the relay header
// (Packet::RelayHeader, two words masked), an MD5 signature over the key, the first 28 bytes and
// the payload, then the payload. A ping carries 12 more bytes before the payload. The names of the
// members and constants are ours.
// RTTI N2nn3nex12RelayMessageE @ 0x008CE174
// vtable 0x008FC634 (vptr 0x008FC63C), offset_to_top 0, 9 entries
class RelayMessage : public ::nn::nex::PRUDPMessageInterface, public ::nn::nex::RootObject
{
public:
    static const u8 MAGIC_0 = 0xF5;
    static const u8 MAGIC_1 = 0xD0; // (also the message version of the packet)
    // the size of the header with the signature, and of its signed part
    static const u32 HEADER_SIZE = 44;
    static const u32 SIGNED_HEADER_SIZE = 28;
    static const u32 SIGNATURE_SIZE = 16;
    // the size of the ping data before the payload
    static const u32 PING_SIZE = 12;
    // the port of a relay packet (stream type 11, port 15)
    static const u8 PORT = 0xBF;
    // the relay types
    static const u32 TYPE_DATA = 0;
    static const u32 TYPE_PING = 1;
    // the masks of the header words 0x10 and 0x14 on the wire
    static const u32 MASK_0x10 = 0xDB319579;
    static const u32 MASK_0x14 = 0xBEE17D2F;
    // the time stamps may differ by this many seconds
    static const u32 TIME_STAMP_TOLERANCE = 150;

    // (inline)
    RelayMessage() : m_BaseTime(0), m_BaseTimeStamp(0), m_Unknown0x34(0) {}
    virtual ~RelayMessage(); // 0x0035FA94 slot 0x00
    // 0x0035FA70 slot 0x04 (deleting dtor)
    virtual bool CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x0035F090 slot 0x08 | virtual slot, introduced by nn::nex::RelayMessage
    virtual void CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x0035F08C slot 0x0C | virtual slot, introduced by nn::nex::RelayMessage
    virtual nn::nex::PRUDPMessageInterface::SignatureMethod DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing); // 0x0035F138 slot 0x10 | slot vf_0x10 of nn::nex::RelayMessage
    virtual int CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x0035F130 slot 0x14 | slot vf_0x14 of nn::nex::RelayMessage
    virtual bool Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize); // 0x0035F718 slot 0x18 | fefates:bytes
    virtual bool FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize); // 0x0035EE38 slot 0x1C | slot vf_0x1C of nn::nex::RelayMessage
    virtual bool Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream); // 0x0035F3B4 slot 0x20 | fefates:bytes
    bool UnpackPing(nn::nex::PacketIn* pPacket); // 0x0035EE40 | fefates:bytes [tier B]
    void CalcSignature(nn::nex::SignatureBytes* pSignature, nn::nex::RelayType type, const unsigned char* pKey, unsigned int keySize, const unsigned char* pHeader, const unsigned char* pPayload, unsigned int payloadSize); // 0x0035EF80 | fefates:bytes [tier B]
    bool CheckTimeStamp(nn::nex::PacketIn* pPacket); // 0x0035F098
    bool UnpackWithoutValidation(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize, bool copyPayload); // 0x0035F140 | fefates:bytes [tier B]
    bool Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream, const unsigned char* pKey, unsigned int keySize); // 0x0035F434 | fefates:bytes [tier B]
    bool PackPing(nn::nex::PacketOut* pPacket); // 0x0035F7F8 | fefates:bytes [tier B]

    // (inline; the name is ours) the seconds of the relay clock
    u32 GetTimeStamp() const
    {
        return static_cast<s64>(SystemClock::s_LastTime.m_Value - m_BaseTime.m_Value) / 1000 + m_BaseTimeStamp;
    }

    LiteBuffer<16> m_Key;      // 0x04 (of the signatures)
    u8 m_Unknown0x1C[0xC];     // 0x1C
    Time m_BaseTime;           // 0x28 (the system time of m_BaseTimeStamp)
    u32 m_BaseTimeStamp;       // 0x30
    u32 m_Unknown0x34;         // 0x34
};
ASSERT_OFFSET(RelayMessage, m_BaseTime, 0x28);
ASSERT_SIZE(RelayMessage, 0x38);
} // namespace nex
} // namespace nn
