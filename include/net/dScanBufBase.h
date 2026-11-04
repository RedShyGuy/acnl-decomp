#pragma once

#include "decomp.h"

namespace net {
// RTTI N3net11ScanBufBaseE @ 0x008D0FC0
//
// A session that a search found (Impl::ScanLocal) or that a friend announced (Impl::GetSessionNotifications):
// its owner, session id, parameter, the number of its participants and, in the buffer of the derived class,
// its application data or text (names are ours).
class ScanBufBase
{
public:
    ScanBufBase() {}
    virtual ~ScanBufBase() {}
    // the buffer for the application data of the session and its size
    virtual void* GetBuffer() = 0;  // slot 0x08
    virtual u32 GetBufferSize() = 0; // slot 0x0C

    u32 m_OwnerPrincipalId; // 0x04 (of a session of a friend)
    u32 m_SessionId;        // 0x08
    u16 m_Param;            // 0x0C (of a session of a friend)
    u8 m_ParticipantNum; // 0x0E
};
} // namespace net
