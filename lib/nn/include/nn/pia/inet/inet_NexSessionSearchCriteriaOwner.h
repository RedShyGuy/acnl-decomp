#pragma once

#include "decomp.h"
#include "nn/pia/session/session_SessionSearchCriteria.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet29NexSessionSearchCriteriaOwnerE @ 0x008CFA5C
// vtable 0x00900728 (vptr 0x00900730), offset_to_top 0, 3 entries
//
// The search for the sessions of an owner (the application builds it inline). The member name is
// ours.
class NexSessionSearchCriteriaOwner : public ::nn::pia::session::SessionSearchCriteria
{
public:
    // (inline)
    NexSessionSearchCriteriaOwner() : m_OwnerPrincipalId(0) {}
    virtual ~NexSessionSearchCriteriaOwner(); // 0x00411154 slot 0x00
    // 0x00411150 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00734060 slot 0x08

    u32 m_OwnerPrincipalId; // 0x10
};
ASSERT_SIZE(NexSessionSearchCriteriaOwner, 0x14);
} // namespace inet
} // namespace pia
} // namespace nn
