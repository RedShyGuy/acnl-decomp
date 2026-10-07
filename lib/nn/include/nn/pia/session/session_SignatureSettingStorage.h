#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session23SignatureSettingStorageE @ 0x008D00E0
// vtable 0x00901BE8 (vptr 0x00901BF0), offset_to_top 0, 4 entries
//
// The signature setting of the session (Mesh::Startup passes it on). The network factory makes
// it (slot 0x3C); the member names and the function names are ours.
class SignatureSettingStorage : public ::nn::pia::common::RootObject
{
public:
    // (inline in NexNetworkFactory)
    SignatureSettingStorage() : m_pSetting(nullptr), m_IsSet(false) {}
    virtual ~SignatureSettingStorage(); // 0x00442B04 slot 0x00
    // 0x00442B00 slot 0x04 (deleting dtor)
    // here only marks it as set
    virtual void SetSetting(const void* pSetting); // 0x00442AF4 slot 0x08
    // null if it is not set
    virtual const void* GetSetting() const; // 0x00734158 slot 0x0C

    const void* m_pSetting; // 0x4
    bool m_IsSet;           // 0x8
};
ASSERT_SIZE(SignatureSettingStorage, 0xC);
} // namespace session
} // namespace pia
} // namespace nn
