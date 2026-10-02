#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class PacketOld
{
public:
    void SetSignatureSize(unsigned int); // 0x00429A94 | fefates:bytes [tier B]
    void IsValidSignatureSize(unsigned int); // 0x00429AB8 | fefates:bytes [tier B]
    void SetDefaultPayloadSize(unsigned int); // 0x00429AD4 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
