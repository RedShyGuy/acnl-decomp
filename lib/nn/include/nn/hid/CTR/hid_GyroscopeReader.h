#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
class GyroscopeReader
{
public:
    void ReadLatest(nn::hid::CTR::GyroscopeStatus*); // 0x00352808 | nintendogs:bytes-fuzzy [tier B]
    void CalculateDirection(); // 0x00352934 | nintendogs:bytes [tier B]
    void CalculateGyroscopeAxisStatus(float*, int*, float*, int, float, int*); // 0x00352B00 | nintendogs:bytes [tier B]
    ~GyroscopeReader(); // 0x003537EC | mk7dlp:bytes [tier B]
};
} // namespace CTR
} // namespace hid
} // namespace nn
