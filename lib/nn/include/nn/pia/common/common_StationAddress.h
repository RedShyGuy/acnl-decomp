#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common14StationAddressE @ 0x008CFE38
// vtable 0x0090152C (vptr 0x00901534), offset_to_top 0, 1 entries
class StationAddress : public ::nn::pia::common::RootObject
{
public:
    virtual void vf_0x00(); // 0x00731978 slot 0x00 | virtual slot, introduced by nn::pia::common::StationAddress
    void Deserialize(const unsigned char*); // 0x00426DB0 | fefates:bytes [tier B]
    void SetInetAddress(const nn::pia::common::InetAddress&); // 0x00426E0C | fefates:bytes [tier B]
    void Clear(); // 0x00426E20 | fefates:bytes [tier B]
    void Compare(const nn::pia::common::StationAddress&, const nn::pia::common::StationAddress&); // 0x00426E3C | fefates:bytes [tier B]
    StationAddress(const nn::pia::common::StationAddress&); // 0x00426EC4 | fefates:bytes [tier B]
    StationAddress(); // 0x00426EF0 | fefates:bytes [tier B]
    void operator=(const nn::pia::common::StationAddress&); // 0x00426F40 | fefates:bytes [tier B]
    void IsValid() const; // 0x0073197C | fefates:bytes [tier B]
    void Serialize(unsigned char*, unsigned int*, unsigned int) const; // 0x007319A4 | fefates:bytes [tier B]
    void operator==(const nn::pia::common::StationAddress&) const; // 0x00731A1C | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
