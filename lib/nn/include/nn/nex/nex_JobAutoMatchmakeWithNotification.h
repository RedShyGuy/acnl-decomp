#pragma once

#include "decomp.h"
#include "nn/nex/nex_AutoMatchmakeParam.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_MatchmakeSession.h"
#include "nn/nex/nex_NotificationEvent.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
class MatchmakeExtensionClient;
class NotificationEventFilterInterface;

// An automatic matchmaking (MatchmakeExtension method 45) whose gathering comes with a notification
// (type 125): the gathering id of the notification, then its matchmake session. armlink kept only
// the steps (no constructor, vtable or RTTI); the class name and the names of the members and the
// steps are ours.
class JobAutoMatchmakeWithNotification : public ::nn::nex::StepSequenceJob
{
public:
    void CallAutoMatchmake(); // 0x00380BB8
    void WaitForAutoMatchmake(); // 0x00380D0C
    void ProcessNotification(); // 0x003809C4
    void CallGetMatchmakeSession(); // 0x00380E08
    void ProcessMatchmakeSession(); // 0x00380F28
    // (symbols.json names it nn::nex::CTRHttpConnection::Initialize)
    void CompleteJob(const nn::nex::qResult& result); // 0x00380920

    u32 m_CallContextId;                          // 0x68 (the call of the caller)
    MatchmakeExtensionClient* m_pClient;          // 0x6C
    AutoMatchmakeParam m_Param;                   // 0x70
    MatchmakeSession* m_pSession;                 // 0x188 (the result)
    u32 m_Unknown0x18C;                           // 0x18C
    ProtocolCallContext m_ProtocolCallContext;    // 0x190
    CallContext m_NotificationContext;            // 0x208
    u8 m_Unknown0x268[0x18];                      // 0x268
    NotificationEventFilterInterface* m_pFilter;  // 0x280
    NotificationEvent m_Event;                    // 0x284
    u64 m_GatheringId;                            // 0x2A8 (of the automatic matchmaking)
    u32 m_NotifiedGatheringId;                    // 0x2B0 (of the notification)
    MatchmakeSession m_Session;                   // 0x2B8
};
ASSERT_OFFSET(JobAutoMatchmakeWithNotification, m_pSession, 0x188);
ASSERT_OFFSET(JobAutoMatchmakeWithNotification, m_ProtocolCallContext, 0x190);
ASSERT_OFFSET(JobAutoMatchmakeWithNotification, m_NotificationContext, 0x208);
ASSERT_OFFSET(JobAutoMatchmakeWithNotification, m_pFilter, 0x280);
ASSERT_OFFSET(JobAutoMatchmakeWithNotification, m_GatheringId, 0x2A8);
ASSERT_OFFSET(JobAutoMatchmakeWithNotification, m_Session, 0x2B8);
} // namespace nex
} // namespace nn
