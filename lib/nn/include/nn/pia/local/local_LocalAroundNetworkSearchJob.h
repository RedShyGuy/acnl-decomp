#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local27LocalAroundNetworkSearchJobE
// vtable 0x00901214 (vptr 0x0090121C), offset_to_top 0, 6 entries
//
// The search of the networks around while connected: the host commands it (start / stop until
// all clients answered), every station scans in the background job at intervals and sends what it
// found to the others until they answered; found networks expire. The member names are ours.
class LocalAroundNetworkSearchJob : public ::nn::pia::common::StepSequenceJob
{
public:
    // the time between two sends of a message that is not answered yet
    static const u32 RESEND_INTERVAL_MSEC = 80;
    // the lifetimes of the found networks are counted down in these steps
    static const u32 LIFE_TIME_INTERVAL_MSEC = 100;
    // the random part of the interval between two scans (0, 1 or 2 times this)
    static const u32 SEARCH_INTERVAL_STEP_MSEC = 15;

    LocalAroundNetworkSearchJob(); // 0x00422B58
    virtual ~LocalAroundNetworkSearchJob(); // 0x00422C08 slot 0x00
    // 0x00422BC4 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316C0 slot 0x14

    void LifeTimeProcess(); // 0x0042151C | fefates:bytes [tier B]
    common::ExecuteResult SendAroundNetworkStatus(); // 0x004215F8 | fefates:bytes [tier B]
    common::ExecuteResult WaitAroundNetworkSearch(); // 0x0042164C | fefates:bytes [tier B]
    common::ExecuteResult ReceiveAroundNetworkInfo(); // 0x0042194C | fefates:bytes [tier B]
    common::ExecuteResult StartAroundNetworkSearch(); // 0x00421C04 | fefates:bytes [tier B]
    common::ExecuteResult WaitHostMigrationCompleted(); // 0x00421D88 | fefates:bytes [tier B]
    common::ExecuteResult WaitAroundNetworkSearchActivated(); // 0x00421ED0 | fefates:bytes [tier B]
    common::ExecuteResult SendStopAroundNetworkSearchMessage(); // 0x004220B8 | fefates:bytes [tier B]
    common::ExecuteResult SendStartAroundNetworkSearchMessage(); // 0x0042210C | fefates:bytes [tier B]
    common::ExecuteResult WaitSendAroundNetworkStatusCompleted(); // 0x00422160 | fefates:bytes [tier B]
    common::ExecuteResult WaitSendStopAroundNetworkSearchMessageCompleted(); // 0x00422504 | fefates:bytes [tier B]
    common::ExecuteResult WaitSendStartAroundNetworkSearchMessageCompleted(); // 0x00422750 | fefates:bytes [tier B]
    void Cleanup(); // 0x00422A64 (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext); // 0x00422AD8 | fefates:bytes [tier B]

    // the common transitions of the steps (inline; names are ours)
    inline common::ExecuteResult StopSearch();
    inline common::ExecuteResult EndSearch(bool isDuringHostMigration);

    common::CallContext* m_pCallContext;           // 0x40, of LocalAroundNetworkSearchManager
    common::CallContext* m_pBackgroundCallContext; // 0x44, of the background job
    common::Time m_SendTime;                       // 0x48, of the last message
    common::Time m_SearchStartTime;                // 0x50, of the last scan
    common::Time m_LifeTimeTime;                   // 0x58, of the last LifeTimeProcess
    u32 m_SearchIntervalMsec;                      // 0x60
};
ASSERT_OFFSET(LocalAroundNetworkSearchJob, m_SendTime, 0x48);
ASSERT_SIZE(LocalAroundNetworkSearchJob, 0x68);
} // namespace local
} // namespace pia
} // namespace nn
