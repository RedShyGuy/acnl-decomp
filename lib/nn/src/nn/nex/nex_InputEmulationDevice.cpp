#include "nn/nex/nex_EmulationDevice.h"
#include "nn/nex/nex_InputEmulationDevice.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::InputEmulationDevice::InputEmulationDevice()
{
}

// 0x003967D4 slot 0x00 | virtual slot, introduced by nn::nex::InputEmulationDevice
void nn::nex::InputEmulationDevice::vf_0x00()
{
}

// 0x003967BC slot 0x04 | virtual slot, introduced by nn::nex::InputEmulationDevice
void nn::nex::InputEmulationDevice::vf_0x04()
{
}

// 0x00377650 slot 0x08 | virtual slot, introduced by nn::nex::InputEmulationDevice
void nn::nex::InputEmulationDevice::vf_0x08()
{
}

} // namespace nex
} // namespace nn
