#pragma once

#include "decomp.h"
#include "nn/pia/session/session_JoinSessionSetting.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet21NexJoinSessionSettingE
// vtable 0x009003E0 (vptr 0x009003E8)
//
// The join setting of the inet network: the session by its id (or by the session info of the
// base) and two strings. The layout is from the constructor; the member and slot names are ours.
class NexJoinSessionSetting : public ::nn::pia::session::JoinSessionSetting
{
public:
    NexJoinSessionSetting(); // 0x004015C8
    virtual ~NexJoinSessionSetting(); // 0x00439AA8 slot 0x00
    // 0x0040160C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F1D0 slot 0x10
    virtual void SetSessionId(u32 sessionId); // 0x004015B8 slot 0x14
    // the set id, else the one of the session info
    virtual u32 GetSessionId() const; // 0x0072F178 slot 0x18
    virtual void vf_0x1C(u16 value); // 0x004015C0 slot 0x1C
    // the set value without a session info, else the one of the session info
    virtual u16 vf_0x20() const; // 0x0072F1A4 slot 0x20

    wchar_t m_Unknown0x8[33];  // 0x08
    wchar_t m_Unknown0x4A[17]; // 0x4A
    u32 m_SessionId;           // 0x6C
    u16 m_Unknown0x70;         // 0x70
};
ASSERT_OFFSET(NexJoinSessionSetting, m_SessionId, 0x6C);
ASSERT_SIZE(NexJoinSessionSetting, 0x74);
} // namespace inet
} // namespace pia
} // namespace nn
