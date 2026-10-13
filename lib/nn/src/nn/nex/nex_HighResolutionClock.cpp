#include "nn/nex/nex_HighResolutionClock.h"
#include "nn/os/os_Tick.h"

namespace nn {
namespace nex {
// 0x0039333C | fefates:bytes [tier B]
u64 nn::nex::HighResolutionClock::GetHighResolutionTime()
{
    return static_cast<u64>(nnosTickGetSystemCurrent()) * 1000000 / nn::os::Tick::TICKS_PER_SECOND;
}

} // namespace nex
} // namespace nn
