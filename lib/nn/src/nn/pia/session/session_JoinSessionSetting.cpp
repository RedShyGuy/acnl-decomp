#include "nn/pia/session/session_JoinSessionSetting.h"

namespace nn {
namespace pia {
namespace session {
// 0x00439A8C slot 0x0C (name is ours)
void nn::pia::session::JoinSessionSetting::SetSessionInfo(ISessionInfo* pSessionInfo)
{
    m_pSessionInfo = pSessionInfo;
}

// 0x00439A94
nn::pia::session::JoinSessionSetting::JoinSessionSetting()
{
    // empty (in the original too)
}

// 0x00439AAC slot 0x00
// 0x00439AA4 (deleting dtor)
nn::pia::session::JoinSessionSetting::~JoinSessionSetting()
{
    // empty (in the original too)
}

// 0x00733910 slot 0x08 (name is ours)
nn::pia::session::ISessionInfo* nn::pia::session::JoinSessionSetting::GetSessionInfo() const
{
    return m_pSessionInfo;
}

// 0x00733918 slot 0x10
void nn::pia::session::JoinSessionSetting::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
