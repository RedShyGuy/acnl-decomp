#pragma once

#include "decomp.h"

namespace nn {
namespace hidlow {
namespace CTR {
class GyroscopeLowLifoRing
{
public:
    void ReadData(nn::hid::CTR::GyroscopeLowStatus*, int, int*, long long*, int*); // 0x00484730 | nintendogs:bytes [tier B]
};
} // namespace CTR
} // namespace hidlow
} // namespace nn
