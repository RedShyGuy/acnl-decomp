#include "nn/pia/local/local_UdsJoinSessionSetting.h"
#include "nn/pia/common/common_Result.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x0041D55C
nn::Result nn::pia::local::UdsJoinSessionSetting::SetPassphrase(const void* pPassphrase, u32 size)
{
    if (!common::IsValidPointer(pPassphrase) || size < PASSPHRASE_SIZE_MIN || size > PASSPHRASE_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    std::memcpy(m_Passphrase, pPassphrase, size);
    m_PassphraseSize = size;
    return nn::Result();
}

// 0x0041EA04
// 0x0041D5A4 (deleting dtor)
nn::pia::local::UdsJoinSessionSetting::~UdsJoinSessionSetting()
{
    // empty (in the original too)
}

// 0x00731500
const char* nn::pia::local::UdsJoinSessionSetting::GetPassphrase() const
{
    return m_Passphrase;
}

} // namespace local
} // namespace pia
} // namespace nn
