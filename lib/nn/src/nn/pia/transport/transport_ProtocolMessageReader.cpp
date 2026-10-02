#include "nn/pia/transport/transport_ProtocolMessageReader.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045A1A8 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageReader::Clear()
{
}

// 0x0045A1CC | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageReader::Attach(const nn::pia::common::Packet&, unsigned int)
{
}

// 0x0045A298 | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageReader::ProtocolMessageReader()
{
}

// 0x0045A2B8 | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageReader::~ProtocolMessageReader()
{
}

// 0x007361B0 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageReader::GetDestination() const
{
}

// 0x007361C4 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageReader::GetReservedData() const
{
}

// 0x007361D8 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageReader::GetProtocolIdPort() const
{
}

// 0x007361F0 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageReader::GetSourceStationKey() const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
