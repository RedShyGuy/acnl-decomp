#pragma once

#include "decomp.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12RelayMessageE @ 0x008CE174
// vtable 0x008FC634 (vptr 0x008FC63C), offset_to_top 0, 9 entries
class RelayMessage : public ::nn::nex::PRUDPMessageInterface, public ::nn::nex::RootObject
{
public:
    RelayMessage(); // ctor address unknown
    virtual void vf_0x00(); // 0x0035FA94 slot 0x00 | virtual slot, introduced by nn::nex::RelayMessage
    virtual void vf_0x04(); // 0x0035FA70 slot 0x04 | virtual slot, introduced by nn::nex::RelayMessage
    virtual void vf_0x08(); // 0x0035F090 slot 0x08 | virtual slot, introduced by nn::nex::RelayMessage
    virtual void vf_0x0C(); // 0x0035F08C slot 0x0C | virtual slot, introduced by nn::nex::RelayMessage
    virtual void DecideSignatureMethod(const nn::nex::Packet*, nn::nex::Stream::Type, bool); // 0x0035F138 slot 0x10 | slot vf_0x10 of nn::nex::RelayMessage
    virtual void CalcExpectedSignature(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&); // 0x0035F130 slot 0x14 | slot vf_0x14 of nn::nex::RelayMessage
    virtual void Unpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*); // 0x0035F718 slot 0x18 | fefates:bytes
    virtual void FastUnpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*); // 0x0035EE38 slot 0x1C | slot vf_0x1C of nn::nex::RelayMessage
    virtual void Pack(nn::nex::PacketOut*, nn::nex::ByteStream*); // 0x0035F3B4 slot 0x20 | fefates:bytes
    void UnpackPing(nn::nex::PacketIn*); // 0x0035EE40 | fefates:bytes [tier B]
    void CalcSignature(nn::nex::SignatureBytes*, nn::nex::RelayType, const unsigned char*, unsigned int, const unsigned char*, const unsigned char*, unsigned int); // 0x0035EF80 | fefates:bytes [tier B]
    void UnpackWithoutValidation(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*, bool); // 0x0035F140 | fefates:bytes [tier B]
    void Pack(nn::nex::PacketOut*, nn::nex::ByteStream*, const unsigned char*, unsigned int); // 0x0035F434 | fefates:bytes [tier B]
    void PackPing(nn::nex::PacketOut*); // 0x0035F7F8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
