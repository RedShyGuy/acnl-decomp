#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_PRUDPMessageV0.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::PRUDPMessageV0::PRUDPMessageV0()
{
}

// 0x0037270C slot 0x00 | virtual slot, introduced by nn::nex::PRUDPMessageV0
void nn::nex::PRUDPMessageV0::vf_0x00()
{
}

// 0x003726F4 slot 0x04 | slot vf_0x04 of nn::nex::PRUDPMessageV0
nn::nex::PRUDPMessageV0::~PRUDPMessageV0()
{
}

// 0x00371AF8 slot 0x08 | fefates:bytes
void nn::nex::PRUDPMessageV0::CalcSignature(nn::nex::PacketOut*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x00371AD0 slot 0x0C | fefates:bytes
void nn::nex::PRUDPMessageV0::CalcSignature(nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x00371EC8 slot 0x10 | slot vf_0x10 of nn::nex::PRUDPMessageV0
void nn::nex::PRUDPMessageV0::DecideSignatureMethod(const nn::nex::Packet*, nn::nex::Stream::Type, bool)
{
}

// 0x00371E88 slot 0x14 | fefates:bytes
void nn::nex::PRUDPMessageV0::CalcExpectedSignature(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x003723B8 slot 0x18 | fefates:callseq
void nn::nex::PRUDPMessageV0::vf_0x18()
{
}

// 0x0037190C slot 0x1C | fefates:bytes
void nn::nex::PRUDPMessageV0::FastUnpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*)
{
}

// 0x00371F64 slot 0x20 | fefates:callseq
void nn::nex::PRUDPMessageV0::vf_0x20()
{
}

// 0x00371B24 | fefates:bytes-fuzzy [tier B]
void nn::nex::PRUDPMessageV0::CalcSignatureHelper(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, nn::nex::SignatureBytes&)
{
}

} // namespace nex
} // namespace nn
