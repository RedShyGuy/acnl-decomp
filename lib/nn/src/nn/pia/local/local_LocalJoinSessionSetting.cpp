#include "nn/pia/local/local_LocalJoinSessionSetting.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalSessionInfo.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x0041E968
nn::Result nn::pia::local::LocalJoinSessionSetting::SetApplicationData(const void* pData, u32 size)
{
    if (!common::IsValidPointer(pData) || size > APPLICATION_DATA_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    std::memcpy(m_ApplicationData, pData, size);
    m_ApplicationDataSize = size;
    return nn::Result();
}

// 0x0041E9B0
nn::pia::local::LocalJoinSessionSetting::LocalJoinSessionSetting() : m_PassphraseSize(0), m_ApplicationDataSize(0)
{
    std::memset(m_ApplicationData, 0, sizeof(m_ApplicationData));
}

// 0x0041EA08
// 0x0041E9F4 (deleting dtor)
nn::pia::local::LocalJoinSessionSetting::~LocalJoinSessionSetting()
{
    // empty (in the original too)
}

// 0x0073166C
u32 nn::pia::local::LocalJoinSessionSetting::GetSessionId() const
{
    if (m_pSessionInfo == nullptr) {
        return 0;
    }
    return static_cast<const LocalSessionInfo*>(m_pSessionInfo)->GetSessionId();
}

} // namespace local
} // namespace pia
} // namespace nn
