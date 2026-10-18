#pragma once

#include "decomp.h"
#include "nn/nex/nex_PseudoSingleton.h"

namespace nn {
namespace nex {
// The connectivity of the network (the call of the connectivity check and the buffers it
// reserved). The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex19ConnectivityManagerE @ 0x008CE7A8
// vtable 0x008FD660 (vptr 0x008FD668), offset_to_top 0, 2 entries
class ConnectivityManager : public ::nn::nex::PseudoSingleton
{
public:
    static const u32 KIND = 11;

    ConnectivityManager(); // (inline in the constructor of Network)
    virtual ~ConnectivityManager(); // 0x00392CAC slot 0x00
    // 0x00392C7C slot 0x04 (deleting dtor)
    // the address of the relay is the public address of the URLs with an RVCID
    static void SetRelayPublicAddress(); // 0x00392B78 | fefates:callgraph [tier C]

    u8 m_Unknown0x11;       // 0x11
    u8 m_Unknown0x12;       // 0x12
    u16 m_PortNumber;       // 0x14
    u16 m_Unknown0x16;      // 0x16
    u32 m_Unknown0x18;      // 0x18
    u32 m_CallContextId;    // 0x1C (the call that is cancelled with the manager)
    u32 m_Unknown0x20;      // 0x20
    u32 m_Unknown0x24;      // 0x24
    bool m_HasPacketBuffer; // 0x28 (one user of the packet buffer manager)
};
ASSERT_SIZE(ConnectivityManager, 0x2C);
} // namespace nex
} // namespace nn
