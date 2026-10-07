#pragma once

#include "decomp.h"
#include "nn/pia/session/session_SessionSearchCriteria.h"

namespace nn {
namespace pia {
namespace local {
class LocalNetworkDescription;

// RTTI N2nn3pia5local26LocalSessionSearchCriteriaE @ 0x008CFCF4
// vtable 0x009011F4 (vptr 0x009011FC), offset_to_top 0, 3 entries
//
// The criteria of a search for sessions of the local network; the constructor is inline in the
// application. The member names and the name of the unnamed function are ours.
class LocalSessionSearchCriteria : public ::nn::pia::session::SessionSearchCriteria
{
public:
    static const u16 PARTICIPANTS_ANY = 0xFFFF;

    LocalSessionSearchCriteria()
        : m_MaxParticipantsMax(PARTICIPANTS_ANY), m_MaxParticipantsMin(PARTICIPANTS_ANY), m_IsOpenedOnly(true), m_IsVacantOnly(false),
          m_LocalCommunicationId(0), m_SubId(0)
    {
    }
    virtual ~LocalSessionSearchCriteria(); // 0x00421518 slot 0x00
    // 0x00421514 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316B4 slot 0x08

    // the network matches the criteria
    bool IsMatch(const nn::pia::local::LocalNetworkDescription* pDescription) const; // 0x004213F4

    u16 m_MaxParticipantsMax;     // 0x10, of the network
    u16 m_MaxParticipantsMin;     // 0x12
    bool m_IsOpenedOnly;          // 0x14
    bool m_IsVacantOnly;          // 0x15
    u32 m_LocalCommunicationId;   // 0x18
    u8 m_SubId;                   // 0x1C
};
ASSERT_SIZE(LocalSessionSearchCriteria, 0x20);
} // namespace local
} // namespace pia
} // namespace nn
