#include "nn/pia/session/session_SyncClock.h"
#include "nn/pia/common/common_TimeSpan.h"

namespace nn {
namespace pia {
namespace session {
// 0x0044D2D0 | fefates:bytes [tier B]
void nn::pia::session::SyncClock::SetBaseTime(const nn::pia::common::Time& time)
{
    m_BaseTime = time;
    m_IsBaseTimeSet = true;
}

// 0x0044D2EC | fefates:bytes [tier B]
void nn::pia::session::SyncClock::ClearBaseTime()
{
    m_IsBaseTimeSet = false;
    m_BaseTime = common::Time();
}

// 0x0044D304 | fefates:bytes [tier B]
nn::pia::session::SyncClock::SyncClock() : m_IsBaseTimeSet(false)
{
}

// 0x00734B00 | fefates:bytes
s64 nn::pia::session::SyncClock::GetTime() const
{
    if (!m_IsBaseTimeSet) {
        return -1;
    }
    common::Time now;
    now.SetNow();
    return (now - m_BaseTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
}

} // namespace session
} // namespace pia
} // namespace nn
