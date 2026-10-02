#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_PacketHandler.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00734CE0 slot 0x00 | slot vf_0x00 of nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::checkDerivedRuntimeTypeInfo(const pead::RuntimeTypeInfo::Interface*) const
{
}

// 0x00734BEC slot 0x04 | virtual slot, introduced by nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::vf_0x04()
{
}

// 0x0044ED8C slot 0x08 | fefates:bytes
nn::pia::transport::PacketHandler::~PacketHandler()
{
}

// 0x0044EAEC slot 0x10 | slot vf_0x10 of nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::AssignByStationIndex(const nn::pia::transport::ProtocolId&, nn::pia::StationIndex, unsigned int, bool)
{
}

// 0x0044EB0C slot 0x14 | slot vf_0x14 of nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::AssignByStationBitmap(const nn::pia::transport::ProtocolId&, unsigned int, unsigned int, bool)
{
}

// 0x0044EB14 slot 0x18 | fefates:bytes
void nn::pia::transport::PacketHandler::AssignByStationAddress(const nn::pia::transport::ProtocolId&, const nn::pia::common::StationAddress&, unsigned int, bool)
{
}

// 0x0044EABC slot 0x1C | virtual slot, introduced by nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::vf_0x1C()
{
}

// 0x0044E16C slot 0x20 | fefates:bytes
void nn::pia::transport::PacketHandler::AssignPacket(nn::pia::StationIndex, unsigned int, const nn::pia::common::StationAddress&, bool)
{
}

// 0x0044E2AC slot 0x24 | slot vf_0x24 of nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::RelayMessage(const nn::pia::transport::ProtocolMessageReader&)
{
}

// 0x0044E4E4 slot 0x28 | fefates:bytes
void nn::pia::transport::PacketHandler::BeginIteration()
{
}

// 0x00734BD0 slot 0x2C | fefates:bytes
void nn::pia::transport::PacketHandler::IsEndIteration() const
{
}

// 0x0044E2B0 slot 0x30 | fefates:bytes-fuzzy
void nn::pia::transport::PacketHandler::NextIteration()
{
}

// 0x0044DE94 slot 0x34 | slot vf_0x34 of nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::CheckPacket(const nn::pia::common::Packet&)
{
}

// 0x0044E1D8 slot 0x38 | slot vf_0x38 of nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::CheckMessage(const nn::pia::transport::ProtocolMessageReader&)
{
}

// 0x0044E1E0 slot 0x3C | slot vf_0x3C of nn::pia::transport::PacketHandler
void nn::pia::transport::PacketHandler::CheckReceive(const nn::pia::transport::ProtocolMessageReader&)
{
}

// 0x0044DED0 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::CleanupCore()
{
}

// 0x0044DF50 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::GetIterator(const nn::pia::transport::ProtocolId&)
{
}

// 0x0044DF88 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::GetIterator(unsigned short)
{
}

// 0x0044E21C | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::FinalizeCore()
{
}

// 0x0044E518 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::InitializeCore(unsigned int, bool, unsigned int)
{
}

// 0x0044E6A0 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::EndDispatchCore()
{
}

// 0x0044E814 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::BeginDispatchCore()
{
}

// 0x0044EAC4 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::AssignPacketPayload(nn::pia::common::Packet*, unsigned int)
{
}

// 0x0044EAF4 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::ReserveMessageWriter()
{
}

// 0x0044ECAC | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::ClearPacketAnalysisData()
{
}

// 0x0044ECE8 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::Commit()
{
}

// 0x0044ED18 | fefates:bytes [tier B]
nn::pia::transport::PacketHandler::PacketHandler()
{
}

// 0x00734C44 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::GetPacketAnalysisData(nn::pia::transport::PacketAnalysisData*, nn::pia::transport::PacketAnalysisData*) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
