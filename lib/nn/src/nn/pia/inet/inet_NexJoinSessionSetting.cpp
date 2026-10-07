#include "nn/pia/inet/inet_NexJoinSessionSetting.h"
#include <cstring>
#include "nn/pia/inet/inet_NexSessionInfo.h"

namespace nn {
namespace pia {
namespace inet {
// 0x004015B8
void nn::pia::inet::NexJoinSessionSetting::SetSessionId(u32 sessionId)
{
    m_SessionId = sessionId;
}

// 0x004015C0
void nn::pia::inet::NexJoinSessionSetting::vf_0x1C(u16 value)
{
    m_Unknown0x70 = value;
}

// 0x004015C8
nn::pia::inet::NexJoinSessionSetting::NexJoinSessionSetting() : m_SessionId(0), m_Unknown0x70(0)
{
    memset(m_Unknown0x8, 0, sizeof(m_Unknown0x8));
    memset(m_Unknown0x4A, 0, sizeof(m_Unknown0x4A));
}

// 0x00439AA8
// 0x0040160C (deleting dtor)
nn::pia::inet::NexJoinSessionSetting::~NexJoinSessionSetting()
{
    // empty (in the original too)
}

// 0x0072F178
u32 nn::pia::inet::NexJoinSessionSetting::GetSessionId() const
{
    if (m_SessionId != 0) {
        return m_SessionId;
    }
    if (m_pSessionInfo != nullptr) {
        return static_cast<const NexSessionInfo*>(m_pSessionInfo)->vf_0x0C();
    }
    return 0;
}

// 0x0072F1A4
u16 nn::pia::inet::NexJoinSessionSetting::vf_0x20() const
{
    if (m_pSessionInfo == nullptr) {
        return m_Unknown0x70;
    }
    return static_cast<const NexSessionInfo*>(m_pSessionInfo)->vf_0x18();
}

// 0x0072F1D0
void nn::pia::inet::NexJoinSessionSetting::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
