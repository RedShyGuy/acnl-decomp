#include "nn/hid/CTR/hid_AccelerometerReader.h"

namespace nn {
namespace hid {
namespace CTR {
// TODO: default ctor added so derived stubs compile - may not exist
nn::hid::CTR::AccelerometerReader::AccelerometerReader()
{
}

// 0x00353C94 | nintendogs:bytes [tier B]
void nn::hid::CTR::AccelerometerReader::ReadLatest(nn::hid::CTR::AccelerometerStatus*)
{
}

// 0x00353D58 | nintendogs:bytes [tier B]
void nn::hid::CTR::AccelerometerReader::ConvertToAcceleration(nn::hid::CTR::AccelerationFloat*, int, const nn::hid::CTR::AccelerometerStatus*)
{
}

// 0x00353E7C | nintendogs:bytes [tier B]
void nn::hid::CTR::AccelerometerReader::Read(nn::hid::CTR::AccelerometerStatus*, int*, int)
{
}

// 0x003540A4 | nintendogs:bytes [tier B]
void nn::hid::CTR::AccelerometerReader::Transform(nn::hid::CTR::AccelerometerStatus*)
{
}

// 0x00354220 | nintendogs:bytes-fuzzy [tier B]
nn::hid::CTR::AccelerometerReader::AccelerometerReader(nn::hid::CTR::Accelerometer&)
{
}

} // namespace CTR
} // namespace hid
} // namespace nn
