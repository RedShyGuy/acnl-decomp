#include "nn/pia/local/local_LocalMessage.h"
#include "nn/pia/local/local_LocalDestroyNetworkMessage.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x00420BEC (unverified)
nn::pia::local::LocalDestroyNetworkMessage::LocalDestroyNetworkMessage()
{
}

// 0x00420C20 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalDestroyNetworkMessage::vf_0x00()
{
}

// 0x00420C1C slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalDestroyNetworkMessage::vf_0x04()
{
}

// 0x00420BA0 slot 0x08 | slot vf_0x08 of nn::pia::local::LocalMessage
void nn::pia::local::LocalDestroyNetworkMessage::UpdateMessageHeader()
{
}

// 0x00420B60 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalMessage
void nn::pia::local::LocalDestroyNetworkMessage::ParseMessageHeader()
{
}

} // namespace local
} // namespace pia
} // namespace nn
