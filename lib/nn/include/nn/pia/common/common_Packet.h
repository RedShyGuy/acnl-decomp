#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class Packet
{
public:
    void AssignPayload(unsigned int); // 0x00429000 | fefates:bytes [tier B]
    void Reset(); // 0x0042902C | fefates:bytes [tier B]
    void Decrypt(const nn::pia::common::Crypto::Setting&); // 0x00429098 | fefates:bytes [tier B]
    void Encrypt(const nn::pia::common::Crypto::Setting&); // 0x0042919C | fefates:bytes [tier B]
    Packet(); // 0x004292D0 | fefates:bytes [tier B]
    ~Packet(); // 0x00429358 | fefates:bytes [tier B]
    void GetPacketNumInNetwork() const; // 0x00733374 | fefates:bytes [tier B]
    void IsValid() const; // 0x0073338C | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
