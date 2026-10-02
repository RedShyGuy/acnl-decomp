#include "nn/pia/session/session_SyncClock.h"

namespace nn {
namespace pia {
namespace session {
// 0x0044D2D0 | fefates:bytes [tier B]
void nn::pia::session::SyncClock::SetBaseTime(const nn::pia::common::Time&)
{
}

// 0x0044D2EC | fefates:bytes [tier B]
void nn::pia::session::SyncClock::ClearBaseTime()
{
}

// 0x0044D304 | fefates:bytes [tier B]
nn::pia::session::SyncClock::SyncClock()
{
}

} // namespace session
} // namespace pia
} // namespace nn
