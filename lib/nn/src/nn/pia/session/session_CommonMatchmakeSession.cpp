#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// usage, internal, 42: the defaults of the slots the networks do not have (name is ours)
const bit32 RESULT_NOT_IMPLEMENTED = 0xE161482A;
} // namespace

// 0x00440FC4
nn::pia::session::CommonMatchmakeSession::CommonMatchmakeSession() : m_Unknown0x4(false), m_Unknown0x6(0)
{
}

// 0x0044102C
// 0x00441028 (deleting dtor)
nn::pia::session::CommonMatchmakeSession::~CommonMatchmakeSession()
{
    // empty (in the original too)
}

// 0x00440F9C (name is ours)
void nn::pia::session::CommonMatchmakeSession::Cleanup()
{
    // (ARMCC writes the 32 bytes with word stores, GCC calls memset)
    memset(m_SignatureSetting.m_KeyBuffer, 0, sizeof(m_SignatureSetting.m_KeyBuffer));
}

// 0x00734078
u16 nn::pia::session::CommonMatchmakeSession::vf_0x68() const
{
    return m_Unknown0x6;
}

// 0x00440EF8
nn::Result nn::pia::session::CommonMatchmakeSession::UpdateSessionSettingAsync(nn::pia::common::CallContext*, u32)
{
    return RESULT_NOT_IMPLEMENTED;
}

// 0x00440F18
bool nn::pia::session::CommonMatchmakeSession::IsUpdateSessionSettingCompleted()
{
    return false;
}

// 0x00440F30 (name is ours)
nn::pia::common::SignatureSetting* nn::pia::session::CommonMatchmakeSession::GetSignatureSetting()
{
    return &m_SignatureSetting;
}

// 0x00440F38 (name is ours)
nn::Result nn::pia::session::CommonMatchmakeSession::SetSignatureSetting(nn::pia::common::SignatureSetting::Mode mode, const void* pKey, u32 keySize)
{
    if (pKey == nullptr || keySize != KEY_SIZE) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_SignatureSetting.Set(mode, m_SignatureSetting.m_KeyBuffer, KEY_SIZE);
    for (u32 i = 0; i < KEY_SIZE; i++) {
        m_SignatureSetting.m_KeyBuffer[i] = static_cast<const u8*>(pKey)[i];
    }
    return nn::Result();
}

// 0x00734068
bool nn::pia::session::CommonMatchmakeSession::vf_0x88() const
{
    return m_Unknown0x3C;
}

// 0x00440EF0
void nn::pia::session::CommonMatchmakeSession::vf_0x8C(bool value)
{
    m_Unknown0x3C = value;
}

// 0x00734070
u32 nn::pia::session::CommonMatchmakeSession::vf_0x90() const
{
    return m_Unknown0x38;
}

// 0x00440F20
void nn::pia::session::CommonMatchmakeSession::vf_0x94(u32 value)
{
    m_Unknown0x38 = value;
}

// 0x00440F0C
nn::Result nn::pia::session::CommonMatchmakeSession::UpdateProgressScore(u32, u8)
{
    return RESULT_NOT_IMPLEMENTED;
}

// 0x00440F28
bool nn::pia::session::CommonMatchmakeSession::IsProgressScoreUpdatable(u8)
{
    return false;
}

// 0x00440F04
bool nn::pia::session::CommonMatchmakeSession::vf_0xA0() const
{
    return m_Unknown0x4;
}

} // namespace session
} // namespace pia
} // namespace nn
