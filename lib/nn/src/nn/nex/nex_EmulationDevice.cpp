#include "nn/nex/nex_EmulationDevice.h"

namespace nn {
namespace nex {
// 0x00377650 slot 0x08 (name is ours)
void nn::nex::EmulationDevice::Enable()
{
    m_IsEnabled = true;
}

} // namespace nex
} // namespace nn
