#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// Who made the current call (the principal and connection id and the address of the station); the
// SecurityContextManager keeps a stack of them per thread. The names of the members are ours.
// RTTI N2nn3nex15SecurityContextE @ 0x008CE4D8
// vtable 0x008FCF14 (vptr 0x008FCF1C), offset_to_top 0, 2 entries
class SecurityContext : public ::nn::nex::RootObject
{
public:
    // (inline)
    SecurityContext(u32 principalId, u32 connectionId, u32 address) : m_PrincipalId(principalId), m_ConnectionId(connectionId), m_Address(address) {}
    virtual ~SecurityContext(); // 0x0037AFD0 slot 0x00
    // 0x0037AFB8 slot 0x04 (deleting dtor)

    u32 m_PrincipalId;  // 0x04
    u32 m_ConnectionId; // 0x08
    u32 m_Address;      // 0x0C
};
ASSERT_SIZE(SecurityContext, 0x10);
} // namespace nex
} // namespace nn
