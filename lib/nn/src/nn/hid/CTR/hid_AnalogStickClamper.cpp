#include "nn/hid/CTR/hid_AnalogStickClamper.h"

namespace nn {
namespace hid {
namespace CTR {
// 0x003538B8 | nintendogs:bytes [tier A]
void nn::hid::CTR::AnalogStickClamper::ClampValueOfClamp()
{
}

// 0x00353904 | nintendogs:bytes [tier B]
void nn::hid::CTR::AnalogStickClamper::NormalizeStickWithScale(float*, float*, short, short)
{
}

// 0x00353B9C | nintendogs:bytes [tier A]
void nn::hid::CTR::AnalogStickClamper::ClampCore(short*, short*, int, int)
{
}

// 0x00353C30 | nintendogs:bytes [tier A]
nn::hid::CTR::AnalogStickClamper::AnalogStickClamper()
{
}

} // namespace CTR
} // namespace hid
} // namespace nn
