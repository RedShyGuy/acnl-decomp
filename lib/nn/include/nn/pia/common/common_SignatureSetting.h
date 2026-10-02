#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common16SignatureSettingE @ 0x008CFE8C
// vtable 0x009015A0 (vptr 0x009015A8), offset_to_top 0, 1 entries
class SignatureSetting
{
public:
    struct Mode { u32 _unknown; }; // TODO: real type unknown (placeholder)
    SignatureSetting(); // ctor candidate(s) 0x00427F28, 0x00439D28, 0x0079F0D0 (unverified)
    virtual void vf_0x00(); // 0x00731AF8 slot 0x00 | virtual slot, introduced by nn::pia::common::SignatureSetting
    void Set(nn::pia::common::SignatureSetting::Mode, const void*, unsigned int); // 0x00427EB0 | fefates:bytes-fuzzy [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
