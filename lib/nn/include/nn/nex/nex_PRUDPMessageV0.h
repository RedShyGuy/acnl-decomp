#pragma once

#include "decomp.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// The PRUDP message format V0: ports, type/flags, session id, packet signature, sequence id, the
// connection signature (SYN/CONNECT) or the fragment id (DATA), the optional payload size, the
// payload (compressed and encrypted with the algorithms of the stream type) and a checksum byte.
// The names of the constants are ours.
// RTTI N2nn3nex14PRUDPMessageV0E @ 0x008CE378
// vtable 0x008FCC00 (vptr 0x008FCC08), offset_to_top 0, 9 entries
class PRUDPMessageV0 : public ::nn::nex::PRUDPMessageInterface, public ::nn::nex::RootObject
{
public:
    // the signature without a checksum algorithm / without data
    static const u32 SIGNATURE_NO_ALGORITHM = 0x9999;
    static const u32 SIGNATURE_NO_DATA = 0x12345678;
    // the size of the signatures in the header
    static const u32 SIGNATURE_SIZE = 4;
    // the stream types above this are invalid
    static const u32 MAX_STREAM_TYPE = 11;
    // the flag of a payload size in the header
    static const u16 FLAG_HAS_SIZE = 0x80;

    // (inline)
    PRUDPMessageV0() {}
    virtual ~PRUDPMessageV0(); // 0x0037270C slot 0x00
    // 0x003726F4 slot 0x04 (deleting dtor)
    virtual bool CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00371AF8 slot 0x08 | fefates:bytes
    virtual void CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00371AD0 slot 0x0C | fefates:bytes
    virtual nn::nex::PRUDPMessageInterface::SignatureMethod DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing); // 0x00371EC8 slot 0x10
    virtual int CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00371E88 slot 0x14 | fefates:bytes
    virtual bool Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize); // 0x003723B8 slot 0x18 | fefates:callseq
    virtual bool FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize); // 0x0037190C slot 0x1C | fefates:bytes
    virtual bool Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream); // 0x00371F64 slot 0x20 | fefates:callseq
    void CalcSignatureHelper(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, nn::nex::SignatureBytes& signature); // 0x00371B24 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
