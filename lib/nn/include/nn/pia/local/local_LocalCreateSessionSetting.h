#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"
#include "nn/pia/session/session_CreateSessionSetting.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalCreateSessionSettingE @ 0x008CFCA0
// vtable 0x009010EC (vptr 0x009010F4), offset_to_top 0, 5 entries
//
// The setting of a new session of the local network (UdsCreateSessionSetting adds the setting of
// the network). The member names and the slot names are ours.
class LocalCreateSessionSetting : public ::nn::pia::session::CreateSessionSetting
{
public:
    static const u32 APPLICATION_DATA_SIZE_MAX = 32;

    LocalCreateSessionSetting(); // 0x004202B8
    // (the destructor of UdsCreateSessionSetting is a nop that falls into it)
    virtual ~LocalCreateSessionSetting(); // 0x00420300 slot 0x00
    // 0x004202F8 slot 0x04 (deleting dtor)
    virtual nn::pia::local::LocalCreateNetworkSetting* GetCreateNetworkSetting() = 0; // slot 0x08
    virtual void SetCreateNetworkSetting(const nn::pia::local::LocalCreateNetworkSetting& setting) = 0; // slot 0x0C
    virtual void Trace(u64 flag) const; // 0x007316A0 slot 0x10

    nn::Result SetApplicationData(const void* pData, u32 size); // 0x00420270

    u8 m_ApplicationData[APPLICATION_DATA_SIZE_MAX]; // 0x08
    u32 m_ApplicationDataSize;                      // 0x28
};
ASSERT_SIZE(LocalCreateSessionSetting, 0x2C);
} // namespace local
} // namespace pia
} // namespace nn
