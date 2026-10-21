#pragma once

#include "decomp.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_HighResolutionClock.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// A job of a protocol broker for an event of an end point (it holds a reference to the end point).
// The names of the members are ours.
// RTTI N2nn3nex23JobProcessProtocolEventE @ 0x008CECF4
// vtable 0x008FE570 (vptr 0x008FE578), offset_to_top 0, 13 entries
class JobProcessProtocolEvent : public ::nn::nex::Job
{
public:
    // (inline in the constructor of JobProcessProtocolMessage)
    JobProcessProtocolEvent(ProtocolRequestBrokerInterface* pBroker, EndPoint* pEndPoint)
        : Job(static_cast<JobType>(2), DebugString()), m_pBroker(pBroker), m_pEndPoint(pEndPoint), m_Unknown0x50(0), m_Unknown0x54(0)
    {
        if (!ProtocolRequestBrokerInterface::s_IsEndPointNotReferenced) {
            pEndPoint->Open();
            m_StartTime = HighResolutionClock::GetHighResolutionTime();
        }
    }
    virtual ~JobProcessProtocolEvent(); // 0x003B113C slot 0x00
    // 0x003B1100 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void vf_0x30(); // 0x003B10FC slot 0x30

    ProtocolRequestBrokerInterface* m_pBroker; // 0x3C
    EndPoint* m_pEndPoint;                     // 0x40
    Time m_StartTime;                          // 0x48 (high resolution)
    u32 m_Unknown0x50;                         // 0x50
    u32 m_Unknown0x54;                         // 0x54
};
ASSERT_OFFSET(JobProcessProtocolEvent, m_StartTime, 0x48);
ASSERT_SIZE(JobProcessProtocolEvent, 0x58);
} // namespace nex
} // namespace nn
