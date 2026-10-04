#include "nn/pia/local/local_LocalStreamBase.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/local/local_LocalInputStream.h"

namespace nn {
namespace pia {
namespace local {
// 0x00416A40 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalStreamBase
void nn::pia::local::LocalInputStream::vf_0x00()
{
}

// 0x00416B88 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalStreamBase
void nn::pia::local::LocalInputStream::vf_0x04()
{
}

// 0x00730248 slot 0x08 | virtual slot, introduced by nn::pia::local::LocalStreamBase
void nn::pia::local::LocalInputStream::vf_0x08()
{
}

// 0x00416A50 slot 0x0C | fefates:bytes-fuzzy
nn::Result nn::pia::local::LocalInputStream::Read(nn::pia::common::Packet*)
{
}

// 0x00416B68 | fefates:bytes [tier B]
nn::pia::local::LocalInputStream::LocalInputStream()
{
}

} // namespace local
} // namespace pia
} // namespace nn
