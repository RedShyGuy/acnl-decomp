#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_PRUDPMessageSelector.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::PRUDPMessageSelector::PRUDPMessageSelector()
{
}

// 0x003977F4 slot 0x00 | fefates:bytes
nn::nex::PRUDPMessageSelector::~PRUDPMessageSelector()
{
}

// 0x00397554 slot 0x08 | fefates:bytes
void nn::nex::PRUDPMessageSelector::CalcSignature(nn::nex::PacketOut*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x00397520 slot 0x0C | fefates:bytes
void nn::nex::PRUDPMessageSelector::CalcSignature(nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x003975BC slot 0x10 | fefates:bytes
void nn::nex::PRUDPMessageSelector::DecideSignatureMethod(const nn::nex::Packet*, nn::nex::Stream::Type, bool)
{
}

// 0x00397588 slot 0x14 | fefates:bytes
void nn::nex::PRUDPMessageSelector::CalcExpectedSignature(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x0039760C slot 0x18 | fefates:bytes
void nn::nex::PRUDPMessageSelector::Unpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*)
{
}

// 0x00397478 slot 0x1C | fefates:bytes
void nn::nex::PRUDPMessageSelector::FastUnpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*)
{
}

// 0x003975D4 slot 0x20 | fefates:bytes
void nn::nex::PRUDPMessageSelector::Pack(nn::nex::PacketOut*, nn::nex::ByteStream*)
{
}

} // namespace nex
} // namespace nn
