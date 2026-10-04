#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common16SignatureSettingE @ 0x008CFE8C
// vtable 0x009015A0 (vptr 0x009015A8), offset_to_top 0, 1 entries
//
// How the packets are signed: not at all or with HMAC-MD5 and a key. The member names and the
// mode names are ours.
class SignatureSetting
{
public:
    enum Mode : u8
    {
        MODE_NONE = 0,
        MODE_HMAC_MD5 = 1,
    };

    // Set, and the off state if the arguments are wrong
    SignatureSetting(Mode mode, const void* pKey, unsigned int keySize); // 0x00427F28
    virtual void Trace(u64 flag) const; // 0x00731AF8 slot 0x00 (name after StepSequenceJob::Trace)

    DECOMP_NOINLINE nn::Result Set(nn::pia::common::SignatureSetting::Mode mode, const void* pKey, unsigned int keySize); // 0x00427EB0 | fefates:bytes-fuzzy [tier B]

    Mode m_Mode;            // 0x04
    const void* m_pKey;     // 0x08
    unsigned int m_KeySize; // 0x0C
};
ASSERT_SIZE(SignatureSetting, 0x10);
} // namespace common
} // namespace pia
} // namespace nn
