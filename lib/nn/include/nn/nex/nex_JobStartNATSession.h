#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class JobResolveRelayInfo;

// Starts a NAT session: waits for the call of the session, resolves the address of the relay
// server of the relay information and passes the information to the NAT traversal engine.
// armlink kept only the steps (no constructor, vtable or RTTI); the class name and StepFinish /
// CompleteJob are from the fefates symbols, the names of the members and the other steps are ours.
class JobStartNATSession : public ::nn::nex::StepSequenceJob
{
public:
    // waits for the end of the call of the session
    void WaitForSession(); // 0x0038BEF8 (name is ours)
    // the relay server of the relay information is resolved (if it is given by name)
    void ResolveRelayServer(); // 0x0038BA58 (name is ours)
    void WaitForRelayServer(); // 0x0038BDE8 (name is ours)
    void StepFinish(); // 0x0038B990 | fefates:bytes [tier B]
    void CompleteJob(const nn::nex::qResult& result); // 0x0038B9E4 | fefates:bytes [tier B]

    u8 m_Unknown0x68[0x8];                 // 0x68
    CallContext m_SessionContext;          // 0x70 (the call of the session)
    u32 m_CallContextId;                   // 0xD0 (the call of the caller)
    u32 m_Unknown0xD4;                     // 0xD4
    CallContext m_RelayInfoContext;        // 0xD8 (the call of the relay information)
    JobResolveRelayInfo* m_pRelayInfo;     // 0x138
    u32 m_Unknown0x13C;                    // 0x13C
    CallContext m_NameResolveContext;      // 0x140 (the resolution of the relay server)
};
ASSERT_OFFSET(JobStartNATSession, m_SessionContext, 0x70);
ASSERT_OFFSET(JobStartNATSession, m_CallContextId, 0xD0);
ASSERT_OFFSET(JobStartNATSession, m_RelayInfoContext, 0xD8);
ASSERT_OFFSET(JobStartNATSession, m_pRelayInfo, 0x138);
ASSERT_OFFSET(JobStartNATSession, m_NameResolveContext, 0x140);
ASSERT_SIZE(JobStartNATSession, 0x1A0);
} // namespace nex
} // namespace nn
