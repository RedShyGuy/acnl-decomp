#include "nn/pia/local/local_LocalMessage.h"
#include "nn/pia/local/local_LocalUpdateSessionMessage.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x00420B28 (unverified)
nn::pia::local::LocalUpdateSessionMessage::LocalUpdateSessionMessage()
{
}

// 0x00420B5C slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalUpdateSessionMessage::vf_0x00()
{
}

// 0x00420B58 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalUpdateSessionMessage::vf_0x04()
{
}

// 0x00420AAC slot 0x08 | fefates:callseq
void nn::pia::local::LocalUpdateSessionMessage::UpdateMessageHeader()
{
}

// 0x00420A50 slot 0x0C | fefates:bytes
void nn::pia::local::LocalUpdateSessionMessage::ParseMessageHeader()
{
}

} // namespace local
} // namespace pia
} // namespace nn
