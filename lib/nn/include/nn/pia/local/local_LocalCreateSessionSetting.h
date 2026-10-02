#pragma once

#include "decomp.h"
#include "nn/pia/session/session_CreateSessionSetting.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalCreateSessionSettingE @ 0x008CFCA0
// vtable 0x009010EC (vptr 0x009010F4), offset_to_top 0, 5 entries
class LocalCreateSessionSetting : public ::nn::pia::session::CreateSessionSetting
{
public:
    LocalCreateSessionSetting(); // ctor candidate(s) 0x004202B8 (unverified)
    virtual void vf_0x00(); // 0x00420300 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalCreateSessionSetting
    virtual void vf_0x04(); // 0x004202F8 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalCreateSessionSetting
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x007316A0 slot 0x10 | virtual slot, introduced by nn::pia::local::LocalCreateSessionSetting
};
} // namespace local
} // namespace pia
} // namespace nn
