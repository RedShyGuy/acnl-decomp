#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_RandomNumberGenerator.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// The PRUDP message format V1: the magic 0xEA 0xD0, a 12 byte header (Packet::HeaderV1), the 16
// byte packet signature (an HMAC of the stream's checksum algorithm), the options and the payload.
// The names of the members and constants are ours.
// RTTI N2nn3nex14PRUDPMessageV1E @ 0x008CE398
// vtable 0x008FCC2C (vptr 0x008FCC34), offset_to_top 0, 9 entries
class PRUDPMessageV1 : public ::nn::nex::PRUDPMessageInterface, public ::nn::nex::RootObject
{
public:
    static const u8 MAGIC_0 = 0xEA;
    static const u8 MAGIC_1 = 0xD0;
    static const u8 VERSION = 1;
    static const u32 SIGNATURE_SIZE = 16;
    static const u32 MAX_OPTIONS_SIZE = 52;
    static const u32 MAX_STREAM_TYPE = 11;
    // the options (id, size, value)
    static const u8 OPTION_SUPPORTED_FUNCTIONS = 0; // u32 (Packet +0x8C)
    static const u8 OPTION_CONNECTION_SIGNATURE = 1; // 16 bytes (Packet::m_Signature)
    static const u8 OPTION_FRAGMENT_ID = 2;          // u8 (Packet +0x64)
    static const u8 OPTION_INITIAL_SEQUENCE_ID = 3;  // u16 (Packet +0x4C)
    static const u8 OPTION_MAX_SUBSTREAM_ID = 4;     // u8 (Packet +0x67)
    static const u8 OPTION_END = 0xFF;
    // the fill bytes of a signature without data / without an algorithm
    static const u8 SIGNATURE_FILL_NONE = 0xAB;
    static const u8 SIGNATURE_FILL_NO_ALGORITHM = 0xEF;

    PRUDPMessageV1(); // 0x003739B8 (name is ours)
    virtual ~PRUDPMessageV1(); // 0x00373AD0 slot 0x00
    // 0x00373A94 slot 0x04 (deleting dtor)
    virtual bool CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00372B58 slot 0x08 | fefates:bytes
    virtual void CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00372B34 slot 0x0C | fefates:bytes
    virtual nn::nex::PRUDPMessageInterface::SignatureMethod DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing); // 0x00373414 slot 0x10
    virtual int CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x003733D4 slot 0x14 | fefates:bytes
    virtual bool Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize); // 0x003737B4 slot 0x18 | fefates:callseq
    virtual bool FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize); // 0x00372710 slot 0x1C | fefates:bytes
    virtual bool Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream); // 0x00373480 slot 0x20 | fefates:callseq
    // the header after the magic (the sizes of the options and of the payload)
    bool HeaderParse(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int& optionsSize, unsigned int& payloadSize); // 0x003727CC | fefates:bytes [tier B]
    bool OptionUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int size); // 0x003728AC | fefates:bytes [tier B]
    bool OptionBuilder(nn::nex::PacketOut* pPacket); // 0x00372C08 | fefates:bytes [tier B]
    void CalcSignatureHelper(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00372F98 | fefates:callgraph [tier C]

    RandomNumberGenerator m_Random; // 0x004 (of the random signatures)
    CriticalSection m_Lock;         // 0x9D0
};
ASSERT_OFFSET(PRUDPMessageV1, m_Lock, 0x9D0);
ASSERT_SIZE(PRUDPMessageV1, 0x9F8);
} // namespace nex
} // namespace nn
