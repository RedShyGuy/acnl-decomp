#pragma once

#include "decomp.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20PRUDPMessageSelectorE @ 0x008CE8D0
// vtable 0x008FD884 (vptr 0x008FD88C), offset_to_top 0, 9 entries
class PRUDPMessageSelector : public ::nn::nex::PRUDPMessageInterface, public ::nn::nex::RootObject
{
public:
    PRUDPMessageSelector(); // ctor address unknown
    virtual ~PRUDPMessageSelector(); // 0x003977F4 slot 0x00 | fefates:bytes
    // 0x003977C4 slot 0x04 | slot vf_0x04 of nn::nex::PRUDPMessageSelector (deleting dtor)
    virtual void CalcSignature(nn::nex::PacketOut*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&); // 0x00397554 slot 0x08 | fefates:bytes
    virtual void CalcSignature(nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&); // 0x00397520 slot 0x0C | fefates:bytes
    virtual void DecideSignatureMethod(const nn::nex::Packet*, nn::nex::Stream::Type, bool); // 0x003975BC slot 0x10 | fefates:bytes
    virtual void CalcExpectedSignature(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&); // 0x00397588 slot 0x14 | fefates:bytes
    virtual void Unpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*); // 0x0039760C slot 0x18 | fefates:bytes
    virtual void FastUnpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*); // 0x00397478 slot 0x1C | fefates:bytes
    virtual void Pack(nn::nex::PacketOut*, nn::nex::ByteStream*); // 0x003975D4 slot 0x20 | fefates:bytes
};
} // namespace nex
} // namespace nn
