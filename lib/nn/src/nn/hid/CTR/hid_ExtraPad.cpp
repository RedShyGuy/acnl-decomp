#include "nn/hid/CTR/hid_ExtraPad.h"
#include "nn/ir/CTR/CTR_Api.h"

namespace nn {
namespace hid {
namespace CTR {
// 0x00354680 | nintendogs:bytes [tier A]
bool nn::hid::CTR::ExtraPad::IsSampling()
{
    return nn::ir::CTR::CepdGetStatus() == nn::ir::CTR::CEPD_STATUS_SAMPLING;
}

} // namespace CTR
} // namespace hid
} // namespace nn
