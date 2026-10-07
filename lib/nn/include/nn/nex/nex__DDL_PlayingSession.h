#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class Gathering;
class MatchmakeSession;
class String;
// RTTI N2nn3nex19_DDL_PlayingSessionE @ 0x008CE82C
// vtable 0x008FD740 (vptr 0x008FD748), offset_to_top 0, 2 entries
//
// A session a principal plays (the list of pia::inet::NexMatchmakeSession: 20 bytes per entry).
// The member names are ours.
class _DDL_PlayingSession : public ::nn::nex::RootObject
{
public:
    _DDL_PlayingSession(); // ctor address unknown
    virtual void vf_0x00(); // 0x0039535C slot 0x00 | virtual slot, introduced by nn::nex::_DDL_PlayingSession
    virtual void vf_0x04(); // 0x00395324 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_PlayingSession

    // the gathering if it is a matchmake session, else null (inline; armlink placed it at the
    // end of the code; name is ours)
    MatchmakeSession* GetMatchmakeSession() const; // 0x0072B208

    u8 m_Unknown0x4;                                  // 0x04
    u32 m_PrincipalId;                                // 0x08
    AnyObjectHolder<Gathering, String> m_Gathering;   // 0x0C
};
ASSERT_OFFSET(_DDL_PlayingSession, m_Gathering, 0xC);
ASSERT_SIZE(_DDL_PlayingSession, 0x14);
} // namespace nex
} // namespace nn
