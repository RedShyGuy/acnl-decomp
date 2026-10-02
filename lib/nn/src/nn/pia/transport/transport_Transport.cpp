#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
// 0x007370E8 slot 0x00 | virtual slot, introduced by nn::pia::transport::Transport
void nn::pia::transport::Transport::vf_0x00()
{
}

// 0x0045FA5C | fefates:bytes-fuzzy [tier B]
void nn::pia::transport::Transport::initialize(const nn::pia::transport::Transport::Setting&)
{
}

// 0x0045FEE0 | fefates:bytes [tier B]
void nn::pia::transport::Transport::GetInputStream()
{
}

// 0x0045FF08 | fefates:bytes [tier B]
void nn::pia::transport::Transport::DestroyInstance()
{
}

// 0x00460000 | fefates:bytes [tier B]
void nn::pia::transport::Transport::GetOutputStream()
{
}

// 0x00460060 | fefates:bytes [tier B]
void nn::pia::transport::Transport::OutputStreamUpdateEvent()
{
}

// 0x00460254 | fefates:bytes [tier B]
void nn::pia::transport::Transport::Cleanup()
{
}

// 0x00460330 | fefates:bytes [tier B]
void nn::pia::transport::Transport::Startup(const nn::pia::common::StationAddress*, const nn::pia::common::CryptoSetting*)
{
}

// 0x004605AC | fefates:bytes [tier B]
void nn::pia::transport::Transport::finalize()
{
}

// 0x004606B0 | fefates:bytes [tier B]
nn::pia::transport::Transport::Transport()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
