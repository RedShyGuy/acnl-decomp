#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalJoinSessionSetting.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21UdsJoinSessionSettingE @ 0x008CFC1C
// vtable 0x00900F30 (vptr 0x00900F38), offset_to_top 0, 8 entries
class UdsJoinSessionSetting : public ::nn::pia::local::LocalJoinSessionSetting
{
public:
    UdsJoinSessionSetting(); // ctor candidate(s) 0x005123F8 (unverified)
    virtual void vf_0x00(); // 0x0041EA04 slot 0x00 | virtual slot, introduced by nn::pia::session::JoinSessionSetting
    virtual void vf_0x04(); // 0x0041D5A4 slot 0x04 | virtual slot, introduced by nn::pia::session::JoinSessionSetting
    virtual void vf_0x14(); // 0x00731500 slot 0x14 | virtual slot, introduced by nn::pia::local::LocalJoinSessionSetting
    virtual void vf_0x18(); // 0x0041D55C slot 0x18 | virtual slot, introduced by nn::pia::local::LocalJoinSessionSetting
};
} // namespace local
} // namespace pia
} // namespace nn
