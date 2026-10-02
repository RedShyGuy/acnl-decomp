#include "nn/pia/transport/transport_PacketStream.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0044DAAC | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Initialize(unsigned int, unsigned int)
{
}

// 0x0044DD14 | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Cleanup()
{
}

// 0x0044DD38 | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Startup()
{
}

// 0x0044DDAC | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Finalize()
{
}

// 0x0044DE40 | fefates:bytes [tier B]
nn::pia::transport::PacketStream::PacketStream()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
