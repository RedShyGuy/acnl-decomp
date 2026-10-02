#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_RelayMessage.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::RelayMessage::RelayMessage()
{
}

// 0x0035FA94 slot 0x00 | virtual slot, introduced by nn::nex::RelayMessage
void nn::nex::RelayMessage::vf_0x00()
{
}

// 0x0035FA70 slot 0x04 | virtual slot, introduced by nn::nex::RelayMessage
void nn::nex::RelayMessage::vf_0x04()
{
}

// 0x0035F090 slot 0x08 | virtual slot, introduced by nn::nex::RelayMessage
void nn::nex::RelayMessage::vf_0x08()
{
}

// 0x0035F08C slot 0x0C | virtual slot, introduced by nn::nex::RelayMessage
void nn::nex::RelayMessage::vf_0x0C()
{
}

// 0x0035F138 slot 0x10 | slot vf_0x10 of nn::nex::RelayMessage
void nn::nex::RelayMessage::DecideSignatureMethod(const nn::nex::Packet*, nn::nex::Stream::Type, bool)
{
}

// 0x0035F130 slot 0x14 | slot vf_0x14 of nn::nex::RelayMessage
void nn::nex::RelayMessage::CalcExpectedSignature(const nn::nex::Packet*, nn::nex::PRUDPMessageInterface::SignatureMethod, const nn::nex::Key*, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes&)
{
}

// 0x0035F718 slot 0x18 | fefates:bytes
void nn::nex::RelayMessage::Unpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*)
{
}

// 0x0035EE38 slot 0x1C | slot vf_0x1C of nn::nex::RelayMessage
void nn::nex::RelayMessage::FastUnpack(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*)
{
}

// 0x0035F3B4 slot 0x20 | fefates:bytes
void nn::nex::RelayMessage::Pack(nn::nex::PacketOut*, nn::nex::ByteStream*)
{
}

// 0x0035EE40 | fefates:bytes [tier B]
void nn::nex::RelayMessage::UnpackPing(nn::nex::PacketIn*)
{
}

// 0x0035EF80 | fefates:bytes [tier B]
void nn::nex::RelayMessage::CalcSignature(nn::nex::SignatureBytes*, nn::nex::RelayType, const unsigned char*, unsigned int, const unsigned char*, const unsigned char*, unsigned int)
{
}

// 0x0035F140 | fefates:bytes [tier B]
void nn::nex::RelayMessage::UnpackWithoutValidation(nn::nex::ByteStream*, nn::nex::PacketIn*, unsigned int*, bool)
{
}

// 0x0035F434 | fefates:bytes [tier B]
void nn::nex::RelayMessage::Pack(nn::nex::PacketOut*, nn::nex::ByteStream*, const unsigned char*, unsigned int)
{
}

// 0x0035F7F8 | fefates:bytes [tier B]
void nn::nex::RelayMessage::PackPing(nn::nex::PacketOut*)
{
}

} // namespace nex
} // namespace nn
