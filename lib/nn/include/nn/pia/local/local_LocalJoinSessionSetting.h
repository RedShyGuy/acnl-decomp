#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/session/session_JoinSessionSetting.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local23LocalJoinSessionSettingE @ 0x008CFC58
// vtable 0x00900FF4 (vptr 0x00900FFC), offset_to_top 0, 8 entries
//
// The setting of joining a session of the local network: the passphrase of the network
// (UdsJoinSessionSetting keeps it) and application data. The member and slot names are ours.
class LocalJoinSessionSetting : public ::nn::pia::session::JoinSessionSetting
{
public:
    static const u32 APPLICATION_DATA_SIZE_MAX = 32;

    LocalJoinSessionSetting(); // 0x0041E9B0
    // (the destructor of UdsJoinSessionSetting is a nop that falls into it)
    virtual ~LocalJoinSessionSetting(); // 0x0041EA08 slot 0x00
    // 0x0041E9F4 slot 0x04 (deleting dtor)
    virtual const char* GetPassphrase() const = 0; // slot 0x14
    virtual nn::Result SetPassphrase(const void* pPassphrase, u32 size) = 0; // slot 0x18
    // the session id of the session to join, 0 without one
    virtual u32 GetSessionId() const; // 0x0073166C slot 0x1C

    nn::Result SetApplicationData(const void* pData, u32 size); // 0x0041E968

    u32 m_PassphraseSize;                            // 0x08
    u8 m_ApplicationData[APPLICATION_DATA_SIZE_MAX]; // 0x0C
    u32 m_ApplicationDataSize;                      // 0x2C
};
ASSERT_SIZE(LocalJoinSessionSetting, 0x30);
} // namespace local
} // namespace pia
} // namespace nn
