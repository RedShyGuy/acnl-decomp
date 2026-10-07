#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session21SessionSearchCriteriaE @ 0x008D00B0
//
// The criteria of a search for sessions (the networks derive it). Only the layout the users of
// the base need is known (from the constructor of local::LocalSessionSearchCriteria); the names
// are ours.
class SessionSearchCriteria : public ::nn::pia::common::RootObject
{
public:
    // (inline; the default number of results is 20)
    SessionSearchCriteria() : m_Unknown0x4(0), m_ResultOffset(0), m_ResultNumMax(20) {}
    virtual ~SessionSearchCriteria() {} // slot 0x00
    // slot 0x04 (deleting dtor)

    u8 m_Unknown0x4;      // 0x4
    u32 m_ResultOffset;   // 0x8, the matches skipped before the first result
    u32 m_ResultNumMax;   // 0xC, as many as the session info list takes (Session::BrowseSessionAsync)
};
ASSERT_OFFSET(SessionSearchCriteria, m_ResultNumMax, 0xC);
} // namespace session
} // namespace pia
} // namespace nn
