#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// The job of NATTraversalEngine::ProcessPendingURLs for a station whose NAT maps the ports per
// destination: the relay interface checks the public port, then the probe request with the next
// port goes to the peers and the job waits for the probes. The caller's call completes with the
// result. The names of the members and of the steps are ours.
// RTTI N2nn3nex16JobCheckAndRelayE @ 0x008CE590
// vtable 0x008FD0F0 (vptr 0x008FD0F8), offset_to_top 0, 13 entries
class JobCheckAndRelay : public ::nn::nex::StepSequenceJob
{
public:
    // (inline in NATTraversalEngine::ProcessPendingURLs) the call of pContext starts
    JobCheckAndRelay(CallContext* pContext, const qList<StationURL>& urls, const StationURL& url)
        : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_Deadline(0), m_URLs(urls), m_URL(url)
    {
        pContext->InitiateCall();
        m_CallContextId = pContext->m_Id;
        SetStepInline(&JobCheckAndRelay::CheckPublicPort, "");
    }
    virtual ~JobCheckAndRelay(); // 0x0037FAD4 slot 0x00
    // 0x0037FA80 slot 0x04 (deleting dtor)

    // the call fails
    void Fail(); // 0x0037F2D4
    // waits until the relay interface knows the public port
    void WaitForPublicPort(); // 0x0037F350
    // waits for the probes of the URLs
    void WaitForProbes(); // 0x0037F4C4
    // the relay interface checks the public port
    void CheckPublicPort(); // 0x0037F768
    // the probe request with the next port of the NAT goes to the peers
    void SendProbeRequest(); // 0x0037F8A8

    u32 m_CallContextId;      // 0x068 (the call of the caller)
    CallContext m_Context;    // 0x070 (of the check of the public port)
    Time m_Deadline;          // 0x0D0
    qList<StationURL> m_URLs; // 0x0D8 (the URLs of the peers)
    StationURL m_URL;         // 0x0F0 (the URL of the console for the peers)
};
ASSERT_OFFSET(JobCheckAndRelay, m_Context, 0x70);
ASSERT_OFFSET(JobCheckAndRelay, m_URLs, 0xD8);
ASSERT_SIZE(JobCheckAndRelay, 0x158);
} // namespace nex
} // namespace nn
