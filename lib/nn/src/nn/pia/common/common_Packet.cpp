#include "nn/pia/common/common_Packet.h"

namespace nn {
namespace pia {
namespace common {
// 0x00429000 | fefates:bytes [tier B]
void nn::pia::common::Packet::AssignPayload(unsigned int)
{
}

// 0x0042902C | fefates:bytes [tier B]
void nn::pia::common::Packet::Reset()
{
}

// 0x00429098 | fefates:bytes [tier B]
void nn::pia::common::Packet::Decrypt(const nn::pia::common::Crypto::Setting&)
{
}

// 0x0042919C | fefates:bytes [tier B]
void nn::pia::common::Packet::Encrypt(const nn::pia::common::Crypto::Setting&)
{
}

// 0x004292D0 | fefates:bytes [tier B]
nn::pia::common::Packet::Packet()
{
}

// 0x00429358 | fefates:bytes [tier B]
nn::pia::common::Packet::~Packet()
{
}

// 0x00733374 | fefates:bytes [tier B]
void nn::pia::common::Packet::GetPacketNumInNetwork() const
{
}

// 0x0073338C | fefates:bytes [tier B]
void nn::pia::common::Packet::IsValid() const
{
}

} // namespace common
} // namespace pia
} // namespace nn
