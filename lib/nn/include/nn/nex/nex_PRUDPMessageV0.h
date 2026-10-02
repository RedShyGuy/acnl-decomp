#pragma once

#include "decomp.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14PRUDPMessageV0E @ 0x008CE378
// vtable 0x008FCC00 (vptr 0x008FCC08), offset_to_top 0, 9 entries
class PRUDPMessageV0 : public ::nn::nex::PRUDPMessageInterface, public ::nn::nex::RootObject
{
public:
    PRUDPMessageV0(); // ctor address unknown
    virtual void vf_0x00(); // 0x0037270C slot 0x00 | virtual slot, introduced by nn::nex::PRUDPMessageV0
    virtual ~PRUDPMessageV0(); // 0x003726F4 slot 0x04 | slot vf_0x04 of nn::nex::PRUDPMessageV0
    virtual void CalcSignature(nn::nex::PacketOut*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&); // 0x00371AF8 slot 0x08 | fefates:bytes
    virtual void CalcSignature(nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&); // 0x00371AD0 slot 0x0C | fefates:bytes
    virtual void DecideSignatureMethod(const nn::nex::Packet*, nn::nex::Stream::Type, bool); // 0x00371EC8 slot 0x10 | slot vf_0x10 of nn::nex::PRUDPMessageV0
    virtual void CalcExpectedSignature(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&); // 0x00371E88 slot 0x14 | fefates:bytes
    virtual void vf_0x18(); // 0x003723B8 slot 0x18 | fefates:callseq
    virtual void FastUnpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*); // 0x0037190C slot 0x1C | fefates:bytes
    virtual void vf_0x20(); // 0x00371F64 slot 0x20 | fefates:callseq
    void CalcSignatureHelper(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, nn::nex::SignatureBytes&); // 0x00371B24 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
