#include "nn/pia/transport/transport_ProtocolMessageWriter.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045A2E8 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::SetPayload(const void*, unsigned int, unsigned int)
{
}

// 0x0045A2FC | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::AddMessageBuffer(nn::pia::common::Packet*, void*, unsigned int, bool, bool)
{
}

// 0x0045A330 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::Reset(const nn::pia::transport::ProtocolId&, unsigned int, bool, bool)
{
}

// 0x0045A35C | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageWriter::Commit()
{
}

// 0x0045A51C | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageWriter::ProtocolMessageWriter()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
