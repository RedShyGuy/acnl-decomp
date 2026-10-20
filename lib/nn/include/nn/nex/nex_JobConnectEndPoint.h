#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_SystemComponent.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class Buffer;
class ConnectionManager;
class EndPoint;
class UserContext;

// The job of ConnectionManager::Connect: the connection techniques (direct, then through the
// routing station) one after the other; per technique the URLs one after the other, each with a
// NAT traversal if needed, the resolution of the address and the connection of an end point. The
// names of the members and of the steps without a reference name are ours.
// RTTI N2nn3nex18JobConnectEndPointE @ 0x008CE70C
// vtable 0x008FD4B0 (vptr 0x008FD4B8), offset_to_top 0, 14 entries
class JobConnectEndPoint : public ::nn::nex::StepSequenceJob
{
public:
    // unknown: the peer starts the NAT traversal (the probes are only waited for and the result is
    // not reported)
    JobConnectEndPoint(nn::nex::Job::JobType type, nn::nex::ConnectionManager* pManager, unsigned int callContextId, nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData,
                       const nn::nex::qList<nn::nex::StationURL>& urls, nn::nex::EndPoint** ppEndPoint, unsigned int timeout, bool unknown); // 0x003896CC
    virtual ~JobConnectEndPoint(); // 0x00389A60 slot 0x00 | fefates:bytes
    // 0x00389A30 slot 0x04 (deleting dtor)
    virtual void TestSuspendedJobState(); // 0x00388F8C slot 0x10 | fefates:bytes
    virtual void CancelJob(); // 0x00389644 slot 0x24 | fefates:bytes
    virtual void vf_0x28(); // 0x00388E20 slot 0x28
    virtual void CheckExceptions(); // 0x003884C8 slot 0x30 | fefates:bytes
    virtual void Trace(unsigned long long flags); // 0x00389530 slot 0x34 | fefates:bytes

    // the steps (names after the reference ones where there is one)
    // the connection of the end point to the current URL
    void Connect(); // 0x00387FF8
    // the next URL (or the next technique)
    void TryNextURL(); // 0x0038828C
    void FailJob(); // 0x00388548
    // the techniques start with the first URL (direct)
    void TryConnectDirect(); // 0x003885F0
    // the address of the current URL is resolved
    void ResolveURL(); // 0x00388694
    // the current URL again if the NAT traversal knows another port of it, else the next one
    void RetryURL(); // 0x00388768
    void CancelledJob(); // 0x00388870
    void SucceededJob(); // 0x003888FC
    void StartNATTraversal(); // 0x00388A08
    void WaitForNATTraversal(); // 0x00388C24
    void TryConnectViaRouting(); // 0x00388E2C | fefates:bytes [tier B]
    void WaitForURLResolution(); // 0x00388E90 | fefates:bytes [tier B]
    // true if the NAT traversal saw another port of the current URL (it gets that port)
    bool ApplyUpdatedPort(); // 0x00388FB8
    // the connection result: disconnects a failed end point
    void ProcessConnectResult(); // 0x003890E0
    void ProcessDisconnectResult(); // 0x003891FC
    void ReportNATTraversalResult(bool isSucceeded); // 0x0038934C | fefates:bytes-fuzzy [tier B]
    void SelectConnectionTechnique(); // 0x00389450 | fefates:bytes [tier B]
    // the URLs are sorted and filtered by the transport adapter
    void SortURLs(); // 0x00389548

    // the completions of EndPoint::_Connect and _Disconnect (the context is the job)
    static void OnConnectCompleted(nn::nex::EndPoint* pEndPoint, nn::nex::qResult result, const nn::nex::UserContext* pContext); // 0x0033F754
    static void OnDisconnectCompleted(nn::nex::EndPoint* pEndPoint, nn::nex::qResult result, const nn::nex::UserContext* pContext); // 0x0033F820

    // (inline) the current URL gets its port back and the next one is tried
    void NextURL()
    {
        if (m_OriginalPort != 0) {
            m_pCurrentURL->m_Value.SetPortNumberInline(m_OriginalPort);
            m_OriginalPort = 0;
        }
        m_pCurrentURL = m_pCurrentURL->m_pNext;
        SetStepInline(&JobConnectEndPoint::TryNextURL, "");
    }

    SystemComponent::Use m_Use;               // 0x068 (of the connection manager)
    EndPoint* m_pEndPoint;                    // 0x078
    qResult m_Result;                         // 0x07C (of the connection)
    ConnectionManager* m_pManager;            // 0x088
    u32 m_Unknown0x8C;                        // 0x08C
    u32 m_Unknown0x90;                        // 0x090
    u32 m_Unknown0x94;                        // 0x094
    u32 m_Timeout;                            // 0x098 (ms; -1: none)
    qList<Step> m_Techniques;                 // 0x09C
    u32 m_Unknown0xB4;                        // 0x0B4
    CallContext m_Context;                    // 0x0B8
    CallContext m_ResolveContext;             // 0x118
    u32 m_CallContextId;                      // 0x178 (the call of the caller)
    Buffer* m_pConnectData;                   // 0x17C
    Buffer* m_pReplyData;                     // 0x180
    EndPoint** m_ppEndPoint;                  // 0x184
    Time m_ProbeDeadline;                     // 0x188
    qList<StationURL> m_URLs;                 // 0x190
    bool m_IsRouting;                         // 0x1A8
    u16 m_OriginalPort;                       // 0x1AA (0: the port of the current URL was not changed)
    qList<StationURL>::Node* m_pCurrentURL;   // 0x1AC
    bool m_IsPassive;                         // 0x1B0
    bool m_IsCancelled;                       // 0x1B1
    Job* m_pSubJob;                           // 0x1B4 (cancelled with the job; nothing sets it here)
    u32 m_Rtt;                                // 0x1B8 (of the probe of the NAT traversal)
    bool m_IsFault8;                          // 0x1BC (the end point signaled fault error 8)
};
ASSERT_OFFSET(JobConnectEndPoint, m_pEndPoint, 0x78);
ASSERT_OFFSET(JobConnectEndPoint, m_Techniques, 0x9C);
ASSERT_OFFSET(JobConnectEndPoint, m_ResolveContext, 0x118);
ASSERT_OFFSET(JobConnectEndPoint, m_URLs, 0x190);
ASSERT_OFFSET(JobConnectEndPoint, m_pCurrentURL, 0x1AC);
ASSERT_SIZE(JobConnectEndPoint, 0x1C0);
} // namespace nex
} // namespace nn

namespace std {
// (out of line in the original)
template <>
DECOMP_NOINLINE void list<nn::nex::StepSequenceJob::Step, nn::nex::MemAllocator<nn::nex::StepSequenceJob::Step> >::push_back(const nn::nex::StepSequenceJob::Step& value);
} // namespace std
