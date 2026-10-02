#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace transport {
// 0x004513B8 slot 0x00 | fefates:bytes
nn::pia::transport::StationLocation::~StationLocation()
{
}

// 0x0045138C slot 0x04 | virtual slot, introduced by nn::pia::transport::StationLocation
void nn::pia::transport::StationLocation::vf_0x04()
{
}

// 0x007352F0 slot 0x08 | fefates:bytes
void nn::pia::transport::StationLocation::GetSerializedSize() const
{
}

// 0x00735308 slot 0x0C | fefates:bytes
void nn::pia::transport::StationLocation::Serialize(unsigned char*, unsigned int*, unsigned int) const
{
}

// 0x00451138 slot 0x10 | fefates:bytes
void nn::pia::transport::StationLocation::Deserialize(const unsigned char*)
{
}

// 0x00735304 slot 0x14 | slot vf_0x14 of nn::pia::transport::StationLocation
void nn::pia::transport::StationLocation::Trace(unsigned long long) const
{
}

// 0x0045125C | fefates:bytes [tier B]
void nn::pia::transport::StationLocation::SetStationLocation(const nn::pia::transport::StationLocation&)
{
}

// 0x004512C8 | fefates:bytes [tier B]
nn::pia::transport::StationLocation::StationLocation(const nn::pia::transport::StationLocation&)
{
}

// 0x00451344 | fefates:bytes [tier B]
nn::pia::transport::StationLocation::StationLocation()
{
}

// 0x004513E0 | fefates:bytes [tier B]
void nn::pia::transport::StationLocation::operator=(const nn::pia::transport::StationLocation&)
{
}

// 0x00735450 | fefates:bytes [tier B]
void nn::pia::transport::StationLocation::operator==(const nn::pia::transport::StationLocation&) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
