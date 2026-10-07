#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session20CreateSessionSettingE @ 0x008D005C
class CreateSessionSetting : public ::nn::pia::common::RootObject
{
public:
    // (inline)
    CreateSessionSetting() : m_Unknown0x4(0), m_Unknown0x6(0) {}
    virtual ~CreateSessionSetting() {} // slot 0x00
    // slot 0x04 (deleting dtor)

    // (the layout is from inet::NexCreateSessionSetting and local::LocalCreateSessionSetting; the
    // names are ours)
    u16 m_Unknown0x4; // 0x4
    u16 m_Unknown0x6; // 0x6
};
} // namespace session
} // namespace pia
} // namespace nn
