#pragma once

#include "decomp.h"
#include "nn/pia/common/common_SignatureSetting.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
// A SignatureSetting with its own key buffer (N bytes) that the key points to. Its vtable has only
// the Trace of SignatureSetting. The member name is ours.
//
// Instantiations found in the binary:
//   nn::pia::common::SignatureSettingWithKeyBuffer<32u>  typeinfo 0x008CFEB8  vtable 0x009015E4
template <u32 N>
class SignatureSettingWithKeyBuffer : public SignatureSetting
{
public:
    // (inline in session::CommonMatchmakeSession)
    SignatureSettingWithKeyBuffer() : SignatureSetting(MODE_HMAC_MD5, m_KeyBuffer, N) { memset(m_KeyBuffer, 0, sizeof(m_KeyBuffer)); }

    u8 m_KeyBuffer[N]; // 0x10
};
} // namespace common
} // namespace pia
} // namespace nn
