#pragma once

#include "decomp.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// The relay information that the server returns for a NAT session (JobStartNATSession): the relay
// server, the time of the server and how the relay is used. Only the functions that pass it to the
// NAT traversal engine are left in the binary; the base class is assumed from the name (fefates),
// the member names are ours.
class JobResolveRelayInfo : public ::nn::nex::StepSequenceJob
{
public:
    // how the address of the relay server is given
    enum AddressType
    {
        ADDRESS_TYPE_HOST_NAME = 0,      // m_Address
        ADDRESS_TYPE_RELAY_ADDRESS = 1,  // the relay address of the root transport
    };

    // the relay server goes to the NAT traversal engine; false if it is unknown
    DECOMP_NOINLINE bool SetRelayAddress(); // 0x00393800 | fefates:bytes-fuzzy [tier B]
    void ResolvedRelayInfo(); // 0x00393950 | fefates:bytes-fuzzy [tier B]
    // the relay is not used
    DECOMP_NOINLINE static void FailResolvedRelayInfo(); // 0x00393B34 | fefates:callgraph [tier C]

    u8 m_Unknown0x68[0x18];  // 0x68
    DateTime m_ServerTime;   // 0x80
    String m_Address;        // 0x88
    u16 m_Port;              // 0x90 (0: the default port)
    u32 m_AddressType;       // 0x94 (AddressType in the low byte)
    s32 m_Result;            // 0x98 (in the low byte: 1 relay as needed, 2 relay unless the NAT maps per
                             // destination, 3 relay always; else failed)
    s32 m_RelayId;           // 0x9C (of the relay stream)
};
ASSERT_OFFSET(JobResolveRelayInfo, m_ServerTime, 0x80);
ASSERT_OFFSET(JobResolveRelayInfo, m_AddressType, 0x94);
ASSERT_OFFSET(JobResolveRelayInfo, m_RelayId, 0x9C);
} // namespace nex
} // namespace nn
