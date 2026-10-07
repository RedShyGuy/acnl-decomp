#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace session {
class ISessionInfo;
// RTTI N2nn3pia7session18JoinSessionSettingE @ 0x008D002C
// vtable 0x009019B0 (vptr 0x009019B8), offset_to_top 0, 5 entries
//
// The setting of joining a session: the id of the session (NexJoinSessionSetting adds the rest).
// The member and function names are ours.
class JoinSessionSetting : public ::nn::pia::common::RootObject
{
public:
    JoinSessionSetting(); // 0x00439A94
    virtual ~JoinSessionSetting(); // 0x00439AAC slot 0x00
    // 0x00439AA4 slot 0x04 (deleting dtor)
    // the session to join (names are ours)
    virtual ISessionInfo* GetSessionInfo() const; // 0x00733910 slot 0x08
    virtual void SetSessionInfo(ISessionInfo* pSessionInfo); // 0x00439A8C slot 0x0C
    virtual void Trace(u64 flag) const; // 0x00733918 slot 0x10

    ISessionInfo* m_pSessionInfo; // 0x4
};
ASSERT_SIZE(JoinSessionSetting, 0x8);
} // namespace session
} // namespace pia
} // namespace nn
