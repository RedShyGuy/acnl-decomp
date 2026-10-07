#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalJoinSessionSetting.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21UdsJoinSessionSettingE @ 0x008CFC1C
// vtable 0x00900F30 (vptr 0x00900F38), offset_to_top 0, 8 entries
//
// The setting of joining a session of the uds network: the passphrase. The constructor is inline
// in the application (the passphrase is not set there); the member name is ours.
class UdsJoinSessionSetting : public ::nn::pia::local::LocalJoinSessionSetting
{
public:
    static const u32 PASSPHRASE_SIZE_MIN = 8;
    static const u32 PASSPHRASE_SIZE_MAX = 255;

    UdsJoinSessionSetting() {}
    // (a nop that falls into the destructor of LocalJoinSessionSetting)
    virtual ~UdsJoinSessionSetting(); // 0x0041EA04 slot 0x00
    // 0x0041D5A4 slot 0x04 (deleting dtor)
    virtual const char* GetPassphrase() const; // 0x00731500 slot 0x14
    virtual nn::Result SetPassphrase(const void* pPassphrase, u32 size); // 0x0041D55C slot 0x18

    char m_Passphrase[PASSPHRASE_SIZE_MAX]; // 0x30
};
ASSERT_SIZE(UdsJoinSessionSetting, 0x130);
} // namespace local
} // namespace pia
} // namespace nn
