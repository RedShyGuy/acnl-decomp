#pragma once

#include "decomp.h"

namespace nn {
namespace hidlow {
namespace CTR {
class PadLifoRing
{
public:
    void ReadData(nn::hid::CTR::PadStatus*, int, int*, long long*, int*); // 0x00484414 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace hidlow
} // namespace nn
