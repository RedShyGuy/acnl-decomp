#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Crypto.h"

namespace nn {
namespace pia {
namespace common {
// The encryption the application sets for a session: the mode and a 16 byte key. Layout from the
// constructor; the member names are ours.
class CryptoSetting
{
public:
    static const size_t KEY_SIZE = 16;

    CryptoSetting(); // 0x00426D74 | fefates:bytes [tier B]

    Crypto::Mode m_Mode;   // 0x00
    u8 m_Key[KEY_SIZE];    // 0x01
};
ASSERT_SIZE(CryptoSetting, 0x11);
} // namespace common
} // namespace pia
} // namespace nn
