#pragma once

#include "decomp.h"
#include "nn/pia/session/session_JoinSessionSetting.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet21NexJoinSessionSettingE @ 0x008CF984
// vtable 0x009003E0 (vptr 0x009003E8), offset_to_top 0, 9 entries
class NexJoinSessionSetting : public ::nn::pia::session::JoinSessionSetting
{
public:
    NexJoinSessionSetting(); // ctor candidate(s) 0x004015C8 (unverified)
    virtual void vf_0x00(); // 0x00439AA8 slot 0x00 | virtual slot, introduced by nn::pia::session::JoinSessionSetting
    virtual void vf_0x04(); // 0x0040160C slot 0x04 | virtual slot, introduced by nn::pia::session::JoinSessionSetting
    virtual void vf_0x10(); // 0x0072F1D0 slot 0x10 | virtual slot, introduced by nn::pia::session::JoinSessionSetting
    virtual void vf_0x14(); // 0x004015B8 slot 0x14 | virtual slot, introduced by nn::pia::inet::NexJoinSessionSetting
    virtual void vf_0x18(); // 0x0072F178 slot 0x18 | virtual slot, introduced by nn::pia::inet::NexJoinSessionSetting
    virtual void vf_0x1C(); // 0x004015C0 slot 0x1C | virtual slot, introduced by nn::pia::inet::NexJoinSessionSetting
    virtual void vf_0x20(); // 0x0072F1A4 slot 0x20 | virtual slot, introduced by nn::pia::inet::NexJoinSessionSetting
};
} // namespace inet
} // namespace pia
} // namespace nn
