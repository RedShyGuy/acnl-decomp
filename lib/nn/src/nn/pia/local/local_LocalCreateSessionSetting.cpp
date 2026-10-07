#include "nn/pia/local/local_LocalCreateSessionSetting.h"
#include "nn/pia/common/common_Result.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x00420270
nn::Result nn::pia::local::LocalCreateSessionSetting::SetApplicationData(const void* pData, u32 size)
{
    if (!common::IsValidPointer(pData) || size > APPLICATION_DATA_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    std::memcpy(m_ApplicationData, pData, size);
    m_ApplicationDataSize = size;
    return nn::Result();
}

// 0x004202B8
nn::pia::local::LocalCreateSessionSetting::LocalCreateSessionSetting() : m_ApplicationDataSize(0)
{
    std::memset(m_ApplicationData, 0, sizeof(m_ApplicationData));
}

// 0x00420300
// 0x004202F8 (deleting dtor)
nn::pia::local::LocalCreateSessionSetting::~LocalCreateSessionSetting()
{
    // empty (in the original too)
}

// 0x007316A0
void nn::pia::local::LocalCreateSessionSetting::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
