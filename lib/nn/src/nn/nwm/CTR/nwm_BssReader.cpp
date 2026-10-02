#include "nn/nwm/CTR/nwm_BssReader.h"

namespace nn {
namespace nwm {
namespace CTR {
// ctor address unknown
nn::nwm::CTR::BssReader::BssReader()
{
}

// 0x0072EE00 | fefates:bytes [tier B]
void nn::nwm::CTR::BssReader::GetVendorIe(const unsigned char*, unsigned char) const
{
}

} // namespace CTR
} // namespace nwm
} // namespace nn
