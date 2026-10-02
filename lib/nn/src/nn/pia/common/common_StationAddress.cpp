#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_StationAddress.h"

namespace nn {
namespace pia {
namespace common {
// 0x00731978 slot 0x00 | virtual slot, introduced by nn::pia::common::StationAddress
void nn::pia::common::StationAddress::vf_0x00()
{
}

// 0x00426DB0 | fefates:bytes [tier B]
void nn::pia::common::StationAddress::Deserialize(const unsigned char*)
{
}

// 0x00426E0C | fefates:bytes [tier B]
void nn::pia::common::StationAddress::SetInetAddress(const nn::pia::common::InetAddress&)
{
}

// 0x00426E20 | fefates:bytes [tier B]
void nn::pia::common::StationAddress::Clear()
{
}

// 0x00426E3C | fefates:bytes [tier B]
void nn::pia::common::StationAddress::Compare(const nn::pia::common::StationAddress&, const nn::pia::common::StationAddress&)
{
}

// 0x00426EC4 | fefates:bytes [tier B]
nn::pia::common::StationAddress::StationAddress(const nn::pia::common::StationAddress&)
{
}

// 0x00426EF0 | fefates:bytes [tier B]
nn::pia::common::StationAddress::StationAddress()
{
}

// 0x00426F40 | fefates:bytes [tier B]
void nn::pia::common::StationAddress::operator=(const nn::pia::common::StationAddress&)
{
}

// 0x0073197C | fefates:bytes [tier B]
void nn::pia::common::StationAddress::IsValid() const
{
}

// 0x007319A4 | fefates:bytes [tier B]
void nn::pia::common::StationAddress::Serialize(unsigned char*, unsigned int*, unsigned int) const
{
}

// 0x00731A1C | fefates:bytes [tier B]
void nn::pia::common::StationAddress::operator==(const nn::pia::common::StationAddress&) const
{
}

} // namespace common
} // namespace pia
} // namespace nn
