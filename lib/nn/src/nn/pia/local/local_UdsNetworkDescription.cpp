#include "nn/pia/local/local_LocalNetworkDescription.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"

namespace nn {
namespace pia {
namespace local {
// ctor address unknown
nn::pia::local::UdsNetworkDescription::UdsNetworkDescription()
{
}

// 0x00731530 slot 0x00 | fefates:bytes
void nn::pia::local::UdsNetworkDescription::GetCurrentParticipants() const
{
}

// 0x0073151C slot 0x04 | fefates:bytes
void nn::pia::local::UdsNetworkDescription::GetMaxParticipants() const
{
}

// 0x00731598 slot 0x08 | fefates:bytes
void nn::pia::local::UdsNetworkDescription::IsOpened() const
{
}

// 0x00731544 slot 0x0C | fefates:bytes
void nn::pia::local::UdsNetworkDescription::GetLocalCommunicationId() const
{
}

// 0x00731584 slot 0x10 | fefates:bytes
void nn::pia::local::UdsNetworkDescription::GetSubId() const
{
}

// 0x00731508 slot 0x14 | virtual slot, introduced by nn::pia::local::UdsNetworkDescription
void nn::pia::local::UdsNetworkDescription::vf_0x14()
{
}

// 0x0073155C slot 0x18 | virtual slot, introduced by nn::pia::local::UdsNetworkDescription
void nn::pia::local::UdsNetworkDescription::vf_0x18()
{
}

// 0x0041D5B4 slot 0x1C | virtual slot, introduced by nn::pia::local::UdsNetworkDescription
void nn::pia::local::UdsNetworkDescription::vf_0x1C()
{
}

} // namespace local
} // namespace pia
} // namespace nn
