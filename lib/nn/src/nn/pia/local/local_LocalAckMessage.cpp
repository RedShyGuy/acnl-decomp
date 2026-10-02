#include "nn/pia/local/local_LocalMessage.h"
#include "nn/pia/local/local_LocalAckMessage.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x004169F4 (unverified)
nn::pia::local::LocalAckMessage::LocalAckMessage()
{
}

// 0x00416A28 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalAckMessage::vf_0x00()
{
}

// 0x00416A24 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalAckMessage::vf_0x04()
{
}

// 0x004169A0 slot 0x08 | fefates:bytes
void nn::pia::local::LocalAckMessage::UpdateMessageHeader()
{
}

// 0x00416954 slot 0x0C | fefates:bytes
void nn::pia::local::LocalAckMessage::ParseMessageHeader()
{
}

} // namespace local
} // namespace pia
} // namespace nn
