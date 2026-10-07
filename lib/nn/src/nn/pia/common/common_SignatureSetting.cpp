#include "nn/pia/common/common_SignatureSetting.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace common {
// constructed by the static initializer at 0x0079F0D0 (symbols.json calls it a SignatureSetting
// constructor)
// 0x00AF5B70
const SignatureSetting g_DefaultSignatureSetting;

// 0x00427EB0 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::common::SignatureSetting::Set(nn::pia::common::SignatureSetting::Mode mode, const void* pKey, unsigned int keySize)
{
    switch (mode) {
    case MODE_NONE:
        m_Mode = mode;
        m_pKey = nullptr;
        m_KeySize = 0;
        return nn::Result();
    case MODE_HMAC_MD5:
        if (!IsValidPointer(pKey) || keySize == 0) {
            return RESULT_INVALID_ARGUMENT;
        }
        m_Mode = mode;
        m_pKey = pKey;
        m_KeySize = keySize;
        return nn::Result();
    default:
        return RESULT_INVALID_ARGUMENT;
    }
}

// 0x00427F28 (name after C++)
nn::pia::common::SignatureSetting::SignatureSetting(Mode mode, const void* pKey, unsigned int keySize)
{
    if (Set(mode, pKey, keySize).IsFailure()) {
        m_Mode = MODE_NONE;
        m_pKey = nullptr;
        m_KeySize = 0;
    }
}

// 0x00731AF8 (name after StepSequenceJob::Trace)
void nn::pia::common::SignatureSetting::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace common
} // namespace pia
} // namespace nn
