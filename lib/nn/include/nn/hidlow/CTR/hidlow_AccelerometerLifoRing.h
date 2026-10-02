#pragma once

#include "decomp.h"

namespace nn {
namespace hidlow {
namespace CTR {
class AccelerometerLifoRing
{
public:
    void ReadData(nn::hid::CTR::AccelerometerStatus*, int, int*, long long*, int*); // 0x004848A4 | nintendogs:bytes [tier B]
};
} // namespace CTR
} // namespace hidlow
} // namespace nn
