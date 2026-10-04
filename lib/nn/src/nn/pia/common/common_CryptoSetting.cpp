#include "nn/pia/common/common_CryptoSetting.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
// 0x00426D74 | fefates:bytes [tier B]
nn::pia::common::CryptoSetting::CryptoSetting() : m_Mode(Crypto::MODE_NONE)
{
    memset(m_Key, 0, sizeof(m_Key));
}

} // namespace common
} // namespace pia
} // namespace nn
