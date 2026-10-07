#include "nn/pia/session/session_SignatureSettingStorage.h"

namespace nn {
namespace pia {
namespace session {
// 0x00442AF4 slot 0x08 (name is ours)
void nn::pia::session::SignatureSettingStorage::SetSetting(const void*)
{
    m_IsSet = true;
}

// 0x00442B04 slot 0x00
// 0x00442B00 (deleting dtor)
nn::pia::session::SignatureSettingStorage::~SignatureSettingStorage()
{
    // empty (in the original too)
}

// 0x00734158 slot 0x0C (name is ours)
const void* nn::pia::session::SignatureSettingStorage::GetSetting() const
{
    return m_IsSet ? m_pSetting : nullptr;
}

} // namespace session
} // namespace pia
} // namespace nn
