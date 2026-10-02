#include "nn/pia/local/local_LocalStreamBase.h"
#include "nn/pia/common/common_IPacketOutput.h"
#include "nn/pia/local/local_LocalOutputStream.h"

namespace nn {
namespace pia {
namespace local {
// 0x00416CC8 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalStreamBase
void nn::pia::local::LocalOutputStream::vf_0x00()
{
}

// 0x00416CB8 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalStreamBase
void nn::pia::local::LocalOutputStream::vf_0x04()
{
}

// 0x007302A8 slot 0x08 | virtual slot, introduced by nn::pia::local::LocalStreamBase
void nn::pia::local::LocalOutputStream::vf_0x08()
{
}

// 0x00416BE4 slot 0x0C | virtual slot, introduced by nn::pia::local::LocalOutputStream
void nn::pia::local::LocalOutputStream::vf_0x0C()
{
}

// 0x00416BF4 slot 0x10 | fefates:bytes
void nn::pia::local::LocalOutputStream::Write(const nn::pia::common::Packet&)
{
}

// 0x007302A0 slot 0x14 | virtual slot, introduced by nn::pia::local::LocalOutputStream
void nn::pia::local::LocalOutputStream::vf_0x14()
{
}

// 0x00730298 slot 0x18 | virtual slot, introduced by nn::pia::local::LocalOutputStream
void nn::pia::local::LocalOutputStream::vf_0x18()
{
}

// 0x00416C98 | fefates:bytes [tier B]
nn::pia::local::LocalOutputStream::LocalOutputStream()
{
}

} // namespace local
} // namespace pia
} // namespace nn
