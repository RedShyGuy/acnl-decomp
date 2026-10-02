#pragma once

#include "decomp.h"
#include "nn/pia/session/session_JoinSessionSetting.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local23LocalJoinSessionSettingE @ 0x008CFC58
// vtable 0x00900FF4 (vptr 0x00900FFC), offset_to_top 0, 8 entries
class LocalJoinSessionSetting : public ::nn::pia::session::JoinSessionSetting
{
public:
    LocalJoinSessionSetting(); // ctor candidate(s) 0x0041E9B0 (unverified)
    virtual void vf_0x00(); // 0x0041EA08 slot 0x00 | virtual slot, introduced by nn::pia::session::JoinSessionSetting
    virtual void vf_0x04(); // 0x0041E9F4 slot 0x04 | virtual slot, introduced by nn::pia::session::JoinSessionSetting
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0073166C slot 0x1C | virtual slot, introduced by nn::pia::local::LocalJoinSessionSetting
};
} // namespace local
} // namespace pia
} // namespace nn
