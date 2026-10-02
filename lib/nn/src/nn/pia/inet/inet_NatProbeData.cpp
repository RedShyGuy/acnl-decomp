#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/inet/inet_NatProbeData.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E4550 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatProbeData
void nn::pia::inet::NatProbeData::vf_0x00()
{
}

// 0x003E454C slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbeData
void nn::pia::inet::NatProbeData::vf_0x04()
{
}

// 0x003E44B8 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeData::Deserialize(const unsigned char*, unsigned int)
{
}

// 0x003E4528 | fefates:bytes [tier B]
nn::pia::inet::NatProbeData::NatProbeData()
{
}

// 0x0072EF60 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeData::Serialize(unsigned char*, unsigned int*, unsigned int) const
{
}

} // namespace inet
} // namespace pia
} // namespace nn
