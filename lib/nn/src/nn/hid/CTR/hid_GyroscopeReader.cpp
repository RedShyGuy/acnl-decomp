#include "nn/hid/CTR/hid_GyroscopeReader.h"

namespace nn {
namespace hid {
namespace CTR {
// 0x00352808 | nintendogs:bytes-fuzzy [tier B]
void nn::hid::CTR::GyroscopeReader::ReadLatest(nn::hid::CTR::GyroscopeStatus*)
{
}

// 0x00352934 | nintendogs:bytes [tier B]
void nn::hid::CTR::GyroscopeReader::CalculateDirection()
{
}

// 0x00352B00 | nintendogs:bytes [tier B]
void nn::hid::CTR::GyroscopeReader::CalculateGyroscopeAxisStatus(float*, int*, float*, int, float, int*)
{
}

// 0x003537EC | mk7dlp:bytes [tier B]
nn::hid::CTR::GyroscopeReader::~GyroscopeReader()
{
}

} // namespace CTR
} // namespace hid
} // namespace nn
