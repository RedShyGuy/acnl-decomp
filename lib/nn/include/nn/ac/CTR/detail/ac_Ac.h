#pragma once

#include "decomp.h"

namespace nn {
namespace ac {
namespace CTR {
namespace detail {
class Ac
{
public:
    void ConnectAsync(const nnacConfig&, nn::Handle); // 0x00345A34 | fefates:bytes [tier B]
    void AddDenyApType(const nnacConfig&, nnacConfig*, nn::ac::CTR::ApType); // 0x00345A8C | fefates:bytes [tier B]
    void GetInfraPriority(const nnacConfig&, nn::ac::CTR::InfraPriority*); // 0x00345B64 | fefates:bytes [tier B]
    void GetLastErrorCode(unsigned int*); // 0x00345BB4 | fefates:bytes [tier B]
    void CreateDefaultConfig(nnacConfig*); // 0x00345C30 | fefates:bytes [tier B]
    void GetConnectingLocation(unsigned char*); // 0x00345C84 | fefates:bytes [tier B]
    void SetRequestEulaVersion(const nnacConfig&, nnacConfig*, unsigned char, unsigned char); // 0x00345CE0 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace CTR
} // namespace ac
} // namespace nn
