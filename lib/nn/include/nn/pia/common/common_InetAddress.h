#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class InetAddress
{
public:
    void Deserialize(const unsigned char*); // 0x00426BD4 | fefates:bytes [tier B]
    void GetAddressString(nn::pia::common::String*) const; // 0x0073182C | fefates:bytes [tier B]
    void GetKey() const; // 0x00731888 | fefates:bytes [tier B]
    void IsValid() const; // 0x007318A4 | fefates:bytes [tier B]
    void IsPrivate() const; // 0x007318C4 | fefates:bytes [tier B]
    void Serialize(unsigned char*, unsigned int*, unsigned int) const; // 0x00731914 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
