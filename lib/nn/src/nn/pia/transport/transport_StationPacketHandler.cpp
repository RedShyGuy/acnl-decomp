#include "nn/pia/transport/transport_PacketStream.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_StationPacketHandler.h"

namespace nn {
namespace pia {
namespace transport {
// 0x007360FC slot 0x00 | slot vf_0x00 of nn::pia::transport::PacketHandler
void nn::pia::transport::StationPacketHandler::checkDerivedRuntimeTypeInfo(const pead::RuntimeTypeInfo::Interface*) const
{
}

// 0x007360B0 slot 0x04 | virtual slot, introduced by nn::pia::transport::PacketHandler
void nn::pia::transport::StationPacketHandler::vf_0x04()
{
}

// 0x0045A16C slot 0x08 | slot vf_0x08 of nn::pia::transport::PacketHandler
nn::pia::transport::StationPacketHandler::~StationPacketHandler()
{
}

// 0x00459974 slot 0x10 | fefates:bytes
void nn::pia::transport::StationPacketHandler::AssignByStationIndex(const nn::pia::transport::ProtocolId&, nn::pia::StationIndex, unsigned int, bool)
{
}

// 0x00459A10 slot 0x14 | fefates:bytes
void nn::pia::transport::StationPacketHandler::AssignByStationBitmap(const nn::pia::transport::ProtocolId&, unsigned int, unsigned int, bool)
{
}

// 0x004595B4 slot 0x1C | virtual slot, introduced by nn::pia::transport::PacketHandler
void nn::pia::transport::StationPacketHandler::vf_0x1C()
{
}

// 0x00458D94 slot 0x20 | fefates:bytes
void nn::pia::transport::StationPacketHandler::AssignPacket(nn::pia::StationIndex, unsigned int, const nn::pia::common::StationAddress&, bool)
{
}

// 0x004590CC slot 0x24 | fefates:bytes
void nn::pia::transport::StationPacketHandler::RelayMessage(const nn::pia::transport::ProtocolMessageReader&)
{
}

// 0x00458CCC slot 0x34 | fefates:callseq
void nn::pia::transport::StationPacketHandler::CheckPacket(const nn::pia::common::Packet&)
{
}

// 0x00458FE0 slot 0x38 | fefates:bytes
void nn::pia::transport::StationPacketHandler::CheckMessage(const nn::pia::transport::ProtocolMessageReader&)
{
}

// 0x00459050 slot 0x3C | fefates:bytes
void nn::pia::transport::StationPacketHandler::CheckReceive(const nn::pia::transport::ProtocolMessageReader&)
{
}

// 0x0044E60C | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::EndDispatch(const nn::pia::common::Time&)
{
}

// 0x00458A60 | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::Initialize(unsigned int, bool, unsigned int)
{
}

// 0x00458A9C | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::AssignMulti(const nn::pia::transport::ProtocolId&, unsigned int, unsigned int, bool, nn::pia::StationIndex, unsigned int, bool)
{
}

// 0x00458E3C | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::AssignSingle(const nn::pia::transport::ProtocolId&, nn::pia::StationIndex, unsigned int, bool)
{
}

// 0x00459194 | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::BeginDispatch(const nn::pia::common::Time&)
{
}

// 0x004592CC | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::UpdateMyStation()
{
}

// 0x00459328 | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::AssignAllMessage(unsigned int, unsigned int, unsigned int, bool)
{
}

// 0x0045946C | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::AssignHostMessage(unsigned int, unsigned int, bool)
{
}

// 0x0045976C | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::AssignDirectMessage(unsigned int, unsigned int, unsigned int, bool)
{
}

// 0x00459A7C | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::AssignRelayNodeMessage(unsigned int, unsigned int, unsigned int, bool)
{
}

// 0x00459C48 | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::AssignRelayStationMessage(nn::pia::StationIndex, unsigned int, unsigned int, unsigned int, bool)
{
}

// 0x00459DD4 | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::Startup(nn::pia::transport::PacketStream::Writer*, nn::pia::transport::PacketStream::Reader*, const nn::pia::transport::RelayRouteManager*, const nn::pia::common::StationAddress*, const nn::pia::common::CryptoSetting*)
{
}

// 0x00459EA0 | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::AssignAll(const nn::pia::transport::ProtocolId&, unsigned int, bool)
{
}

// 0x0045A0FC | fefates:bytes [tier B]
nn::pia::transport::StationPacketHandler::StationPacketHandler()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
