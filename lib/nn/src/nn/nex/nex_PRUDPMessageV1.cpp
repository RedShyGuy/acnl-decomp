#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_PRUDPMessageV1.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003739B8 (unverified)
nn::nex::PRUDPMessageV1::PRUDPMessageV1()
{
}

// 0x00373AD0 slot 0x00 | virtual slot, introduced by nn::nex::PRUDPMessageV1
void nn::nex::PRUDPMessageV1::vf_0x00()
{
}

// 0x00373A94 slot 0x04 | fefates:bytes
nn::nex::PRUDPMessageV1::~PRUDPMessageV1()
{
}

// 0x00372B58 slot 0x08 | fefates:bytes
void nn::nex::PRUDPMessageV1::CalcSignature(nn::nex::PacketOut*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x00372B34 slot 0x0C | fefates:bytes
void nn::nex::PRUDPMessageV1::CalcSignature(nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::Stream::Type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x00373414 slot 0x10 | slot vf_0x10 of nn::nex::PRUDPMessageV1
void nn::nex::PRUDPMessageV1::DecideSignatureMethod(const nn::nex::Packet*, nn::nex::Stream::Type, bool)
{
}

// 0x003733D4 slot 0x14 | fefates:bytes
void nn::nex::PRUDPMessageV1::CalcExpectedSignature(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x003737B4 slot 0x18 | fefates:callseq
void nn::nex::PRUDPMessageV1::vf_0x18()
{
}

// 0x00372710 slot 0x1C | fefates:bytes
void nn::nex::PRUDPMessageV1::FastUnpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*)
{
}

// 0x00373480 slot 0x20 | fefates:callseq
void nn::nex::PRUDPMessageV1::vf_0x20()
{
}

// 0x003727CC | fefates:bytes [tier B]
void nn::nex::PRUDPMessageV1::HeaderParse(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int&, unsigned int&)
{
}

// 0x003728AC | fefates:bytes [tier B]
void nn::nex::PRUDPMessageV1::OptionUnpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int)
{
}

// 0x00372C08 | fefates:bytes [tier B]
void nn::nex::PRUDPMessageV1::OptionBuilder(nn::nex::PacketOut*)
{
}

} // namespace nex
} // namespace nn
