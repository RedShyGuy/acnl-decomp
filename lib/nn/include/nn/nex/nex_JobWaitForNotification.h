#pragma once

#include "decomp.h"
#include "nn/nex/nex_Chrono.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_NotificationEvent.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class CallContext;
class MyNotificationEventHandler;
class NotificationEventFilterInterface;
class NotificationEventManager;

// Waits (with a notification event manager of its own, registered globally) for a notification
// event of a type; the call completes with the event or after the timeout. The names of the members
// and of the functions are ours.
// RTTI N2nn3nex22JobWaitForNotificationE @ 0x008CEC24
// vtable 0x008FE114 (vptr 0x008FE11C), offset_to_top 0, 14 entries
class JobWaitForNotification : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    // (inline in WaitForNotification)
    JobWaitForNotification() : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_IsRegistered(false) {}
    virtual ~JobWaitForNotification(); // 0x0039D4FC slot 0x00
    // 0x0039D44C slot 0x04 (deleting dtor)
    // ppFilter: the filter of the handler (else it filters by param1); pEvent: the event when the call
    // completes
    virtual nn::nex::qResult Initialize(unsigned int callContextId, unsigned int type, unsigned int subType, unsigned int timeout, unsigned int param1,
                                        nn::nex::NotificationEventFilterInterface** ppFilter, nn::nex::NotificationEvent* pEvent); // 0x0039CF1C slot 0x34

    void CompleteJob(const nn::nex::qResult& result); // 0x0039D0FC
    void WaitForEvent(); // 0x0039D190

    // the call of pContext completes with the event of the type (type * 1000 + subType)
    static bool WaitForNotification(nn::nex::CallContext* pContext, unsigned int type, unsigned int subType, unsigned int timeout,
                                    nn::nex::NotificationEventFilterInterface** ppFilter, nn::nex::NotificationEvent* pEvent); // 0x0039D270
    static bool WaitForNotification(nn::nex::CallContext* pContext, unsigned int type, unsigned int subType, unsigned int timeout, unsigned int param1,
                                    nn::nex::NotificationEventFilterInterface** ppFilter, nn::nex::NotificationEvent* pEvent); // 0x0039D29C

    u32 m_CallContextId;                     // 0x68
    MyNotificationEventHandler* m_pHandler;  // 0x6C
    NotificationEventManager* m_pManager;    // 0x70
    Chrono m_Chrono;                         // 0x78
    bool m_IsRegistered;                     // 0xA0
    NotificationEvent* m_pOutEvent;          // 0xA4
    NotificationEvent m_Event;               // 0xA8 (the result of the handler)
    u32 m_Timeout;                           // 0xCC (ms)
};
ASSERT_OFFSET(JobWaitForNotification, m_Chrono, 0x78);
ASSERT_OFFSET(JobWaitForNotification, m_Event, 0xA8);
ASSERT_SIZE(JobWaitForNotification, 0xD0);
} // namespace nex
} // namespace nn
