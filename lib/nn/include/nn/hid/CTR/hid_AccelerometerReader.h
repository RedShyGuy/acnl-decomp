#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
class AccelerometerReader
{
public:
    AccelerometerReader(); // TODO: default ctor added so derived stubs compile - may not exist
    void ReadLatest(nn::hid::CTR::AccelerometerStatus*); // 0x00353C94 | nintendogs:bytes [tier B]
    void ConvertToAcceleration(nn::hid::CTR::AccelerationFloat*, int, const nn::hid::CTR::AccelerometerStatus*); // 0x00353D58 | nintendogs:bytes [tier B]
    void Read(nn::hid::CTR::AccelerometerStatus*, int*, int); // 0x00353E7C | nintendogs:bytes [tier B]
    void Transform(nn::hid::CTR::AccelerometerStatus*); // 0x003540A4 | nintendogs:bytes [tier B]
    AccelerometerReader(nn::hid::CTR::Accelerometer&); // 0x00354220 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace CTR
} // namespace hid
} // namespace nn
