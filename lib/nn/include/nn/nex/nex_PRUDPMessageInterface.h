#pragma once

#include "decomp.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
class ByteStream;
class Key;
class Packet;
class PacketIn;
class PacketOut;
class TransportSignatureGenerator;
struct SignatureBytes;

// The format of PRUDP messages (V0, V1, the selector of both, relayed messages): the header with the
// signature, the payload and the checksum. The names of the functions without a reference name and
// of the parameters are ours.
// RTTI N2nn3nex21PRUDPMessageInterfaceE @ 0x008CEB30
class PRUDPMessageInterface
{
public:
    // the data of the signature (the names are ours, after what CalcSignatureHelper of V0 signs)
    enum SignatureMethod : u32
    {
        SIGNATURE_METHOD_ADDRESS = 1,              // the address and port of the peer
        SIGNATURE_METHOD_PAYLOAD = 2,              // the payload
        SIGNATURE_METHOD_KEY_SEQUENCE_PAYLOAD = 3, // the key, the sequence id, the fragment id and the payload
        SIGNATURE_METHOD_HEADER = 7,               // the fields of the header and the payload size
        SIGNATURE_METHOD_UNSIGNED = 9,             // (a disconnect without the signature, V0)
    };

    // (inline)
    PRUDPMessageInterface() {}
    virtual ~PRUDPMessageInterface() {}
    // the signature of a packet to send (false if it cannot be made)
    virtual bool CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature) = 0; // slot 0x08
    virtual void CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature) = 0; // slot 0x0C
    virtual nn::nex::PRUDPMessageInterface::SignatureMethod DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing) = 0; // slot 0x10
    // the signature a received packet must have
    virtual int CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature) = 0; // slot 0x14
    // the packet at the position of the stream (the size of the message in *pSize)
    virtual bool Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize) = 0; // slot 0x18
    // only the header (to find the end point)
    virtual bool FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize) = 0; // slot 0x1C
    virtual bool Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream) = 0; // slot 0x20
};
} // namespace nex
} // namespace nn
