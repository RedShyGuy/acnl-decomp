#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/inet/inet_NatProbe.h"

namespace nn {
namespace pia {
namespace inet {
// ctor candidate(s) 0x003E45F8, 0x003E4C14, 0x00412E70 (unverified)
nn::pia::inet::NatProbe::NatProbe()
{
}

// 0x00412F98 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatProbe
void nn::pia::inet::NatProbe::vf_0x00()
{
}

// 0x00412F78 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbe
void nn::pia::inet::NatProbe::vf_0x04()
{
}

// 0x0072FACC slot 0x08 | virtual slot, introduced by nn::pia::inet::NatProbe
void nn::pia::inet::NatProbe::vf_0x08()
{
}

// 0x00412C90 | fefates:bytes [tier B]
void nn::pia::inet::NatProbe::SprayTargetPort()
{
}

// 0x00412CCC | fefates:bytes [tier B]
void nn::pia::inet::NatProbe::UpdateTargetPort(const nn::pia::transport::StationLocation&)
{
}

// 0x00412CF0 | fefates:bytes [tier B]
void nn::pia::inet::NatProbe::GetPortSprayCount()
{
}

// 0x00412E38 | fefates:bytes [tier B]
void nn::pia::inet::NatProbe::UpdateRtt(const nn::pia::common::Time&, const nn::pia::common::Time&)
{
}

// 0x00412E70 | fefates:bytes [tier B]
nn::pia::inet::NatProbe::NatProbe(const nn::pia::transport::StationLocation&, const nn::pia::common::Time&, const nn::pia::common::TimeSpan&, unsigned char, unsigned char, unsigned char)
{
}

// 0x0072FA74 | fefates:bytes [tier B]
void nn::pia::inet::NatProbe::UpdateIsNeeded(nn::pia::common::Time) const
{
}

} // namespace inet
} // namespace pia
} // namespace nn
