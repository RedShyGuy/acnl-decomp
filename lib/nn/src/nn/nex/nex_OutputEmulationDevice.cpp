#include "nn/nex/nex_EmulationDevice.h"
#include "nn/nex/nex_OutputEmulationDevice.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::OutputEmulationDevice::OutputEmulationDevice()
{
}

// 0x0039A6E0 slot 0x00 | virtual slot, introduced by nn::nex::OutputEmulationDevice
void nn::nex::OutputEmulationDevice::vf_0x00()
{
}

// 0x0039A6C8 slot 0x04 | virtual slot, introduced by nn::nex::OutputEmulationDevice
void nn::nex::OutputEmulationDevice::vf_0x04()
{
}

// 0x00377650 slot 0x08 | virtual slot, introduced by nn::nex::InputEmulationDevice
void nn::nex::OutputEmulationDevice::vf_0x08()
{
}

} // namespace nex
} // namespace nn
