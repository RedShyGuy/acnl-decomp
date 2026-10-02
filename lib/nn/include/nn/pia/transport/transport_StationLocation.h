#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport15StationLocationE @ 0x008D01AC
// vtable 0x00901DEC (vptr 0x00901DF4), offset_to_top 0, 6 entries
class StationLocation : public ::nn::pia::common::RootObject
{
public:
    virtual ~StationLocation(); // 0x004513B8 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x0045138C slot 0x04 | virtual slot, introduced by nn::pia::transport::StationLocation
    virtual void GetSerializedSize() const; // 0x007352F0 slot 0x08 | fefates:bytes
    virtual void Serialize(unsigned char*, unsigned int*, unsigned int) const; // 0x00735308 slot 0x0C | fefates:bytes
    virtual void Deserialize(const unsigned char*); // 0x00451138 slot 0x10 | fefates:bytes
    virtual void Trace(unsigned long long) const; // 0x00735304 slot 0x14 | slot vf_0x14 of nn::pia::transport::StationLocation
    void SetStationLocation(const nn::pia::transport::StationLocation&); // 0x0045125C | fefates:bytes [tier B]
    StationLocation(const nn::pia::transport::StationLocation&); // 0x004512C8 | fefates:bytes [tier B]
    StationLocation(); // 0x00451344 | fefates:bytes [tier B]
    void operator=(const nn::pia::transport::StationLocation&); // 0x004513E0 | fefates:bytes [tier B]
    void operator==(const nn::pia::transport::StationLocation&) const; // 0x00735450 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
