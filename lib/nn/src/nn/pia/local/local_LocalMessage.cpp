#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x00414A74 (unverified)
nn::pia::local::LocalMessage::LocalMessage()
{
}

// 0x00414AB0 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalMessage::vf_0x00()
{
}

// 0x00414AAC slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalMessage::vf_0x04()
{
}

// 0x00414980 slot 0x08 | fefates:bytes
void nn::pia::local::LocalMessage::UpdateMessageHeader()
{
}

// 0x00414940 slot 0x0C | fefates:callseq-callee
void nn::pia::local::LocalMessage::ParseMessageHeader()
{
}

// 0x004149C0 | fefates:bytes [tier B]
void nn::pia::local::LocalMessage::SetData(const void*, int, unsigned short)
{
}

// 0x00414A2C | fefates:bytes [tier B]
void nn::pia::local::LocalMessage::SetData(const void*, unsigned short)
{
}

// 0x0072FBB0 | fefates:bytes [tier B]
void nn::pia::local::LocalMessage::GetData(void*, int, unsigned short) const
{
}

} // namespace local
} // namespace pia
} // namespace nn
