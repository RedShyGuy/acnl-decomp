#include "nn/cec/CTR/cec_CecControl.h"
#include "nn/cec/CTR/CTR_Api.h"
#include "nn/cec/CTR/cec_CecControlSys.h"
#include "nn/cec/CTR/detail/cec_Result.h"
#include "nn/cec/CTR/detail/detail_Api.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/ndm/ndm_Api.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace cec {
namespace CTR {
namespace {
// the commands of cecd's Start and Stop (3dbrew "CECD:Start"; names are ours)
const u32 CEC_COMMAND_STOP = 11;
const u32 CEC_COMMAND_STOP_FORCE = 12;
const u32 CEC_COMMAND_START = 14;
// the state of cecd after a stop (3dbrew "CECD:GetCecdState"; name is ours)
const u32 CECD_STATE_IDLE = 1;
// how long StopScanning waits for cecd (60 times a second, then it gives up)
const s64 STOP_WAIT_NSEC = 1000000000;
const s32 STOP_WAIT_COUNT = 60;
// the time ndm needs to suspend the daemon
const s64 SUSPEND_WAIT_MSEC = 16;
} // namespace

using namespace nn::cec::CTR::detail;

// the state of the library (names are ours)
// 0x00975BB4
bool s_IsInitialized = false;
// 0x00975BB5
bool s_IsNdmInitialized = false;
// the cec daemon of ndm is suspended
// 0x00975BB6
bool s_IsDaemonSuspended = false;
// 0x00975BB8
bool s_IsDebugMode = false;
// 0x00AE1A88
nn::os::CriticalSection s_ControlLock((nn::os::CriticalSection::InitializeTag()));

// 0x001407C0 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::CecControl::StartScanning(bool start)
{
    nn::Result result;
    if (!s_IsInitialized && !CecControlSys::IsInitializedSys()) {
        return nn::Result(RESULT_BUSY);
    }
    nn::os::CriticalSection::ScopedLock lock(s_ControlLock);
    if (start) {
        if (!(CecControlSys::IsInitializedSys() || s_IsDebugMode)) {
            return nn::Result(RESULT_NOT_AUTHORIZED);
        }
        result = detail::Start(CEC_COMMAND_START);
    }
    if (!s_IsNdmInitialized) {
        return nn::Result(RESULT_BUSY);
    }
    if (s_IsDaemonSuspended) {
        result = nn::ndm::Resume(nn::ndm::CTR::DAEMON_NAME_CEC);
        if (result.IsFailure()) {
            return result;
        }
        s_IsDaemonSuspended = false;
    }
    return result;
}

// 0x0034D348 | nintendogs:callseq [tier C]
nn::Result nn::cec::CTR::CecControl::Initialize(nn::fnd::IAllocator& allocator)
{
    SetAllocFunc(allocator);
    return Initialize();
}

// 0x0034D358 | fefates:bytes [tier B]
nn::Result nn::cec::CTR::CecControl::Initialize()
{
    nn::Result result;
    if (s_IsInitialized) {
        return result;
    }
    result = detail::InitializeCecControl();
    if (result.IsFailure()) {
        return result;
    }
    detail::WaitForSessionValid();
    result = nn::ndm::Initialize();
    if (result.IsFailure()) {
        return result;
    }
    s_IsNdmInitialized = true;
    nn::cfg::CTR::Initialize();
    s_IsDebugMode = nn::cfg::CTR::IsDebugMode();
    nn::cfg::CTR::Finalize();
    s_IsInitialized = true;
    return result;
}

// 0x0034D3B8 | nintendogs:callgraph [tier A]
nn::Result nn::cec::CTR::CecControl::StopScanning(bool stop, bool noWait)
{
    if (!s_IsInitialized && !CecControlSys::IsInitializedSys()) {
        return nn::Result(RESULT_BUSY);
    }
    nn::os::CriticalSection::ScopedLock lock(s_ControlLock);
    if (!s_IsNdmInitialized) {
        return nn::Result(RESULT_BUSY);
    }
    if (!s_IsDaemonSuspended) {
        nn::Result result = nn::ndm::Suspend(nn::ndm::CTR::DAEMON_NAME_CEC);
        if (result.IsFailure()) {
            return result;
        }
        nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(SUSPEND_WAIT_MSEC));
        s_IsDaemonSuspended = true;
    }
    if (noWait) {
        if (stop) {
            return detail::Stop(CEC_COMMAND_STOP_FORCE);
        }
        return detail::Stop(CEC_COMMAND_STOP);
    }

    // wait until cecd has stopped
    nn::Handle event;
    nn::Handle handle;
    if (detail::GetChangeStateEventHandle(&handle).IsSuccess()) {
        event = handle;
    }
    nn::Result result;
    if (stop) {
        result = detail::Stop(CEC_COMMAND_STOP_FORCE);
    } else {
        result = detail::Stop(CEC_COMMAND_STOP);
    }
    for (s32 i = 0;; i++) {
        nn::Result waitResult = nn::svc::WaitSynchronization1(event, STOP_WAIT_NSEC);
        if (waitResult.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(waitResult);
        }
        if (waitResult.GetDescription() != DESCRIPTION_TIMEOUT) {
            break;
        }
        u32 state;
        detail::GetCecdState(&state);
        if (state == CECD_STATE_IDLE) {
            break;
        }
        if (i > STOP_WAIT_COUNT) {
            result = nn::Result(RESULT_BUSY);
            break;
        }
    }
    if (event.IsValid()) {
        nn::svc::CloseHandle(event);
    }
    return result;
}

// 0x0034D568 (name is ours)
bit32 MakeCecTitleId(bit32 uniqueId, u8 variation)
{
    return ((uniqueId & ~0xF00000) << 8) | variation;
}

// 0x0034D574 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::CecControl::Suspend()
{
    nn::Result result;
    if (!s_IsNdmInitialized) {
        return nn::Result(RESULT_BUSY);
    }
    if (s_IsDaemonSuspended) {
        return result;
    }
    nn::os::CriticalSection::ScopedLock lock(s_ControlLock);
    result = nn::ndm::Suspend(nn::ndm::CTR::DAEMON_NAME_CEC);
    if (result.IsFailure()) {
        return result;
    }
    nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(SUSPEND_WAIT_MSEC));
    s_IsDaemonSuspended = true;
    return result;
}

// 0x0034D604 | nintendogs:callseq [tier A]
nn::Result nn::cec::CTR::CecControl::Finalize()
{
    nn::Result result;
    if (!s_IsInitialized) {
        return result;
    }
    if (s_IsNdmInitialized) {
        nn::Result ndmResult = nn::ndm::Finalize();
        if (ndmResult.IsFailure()) {
            return ndmResult;
        }
        s_IsDaemonSuspended = false;
        s_IsNdmInitialized = false;
    }
    result = detail::FinalizeCecControl();
    if (result.IsFailure()) {
        return result;
    }
    FinalizeAllocFunc();
    s_IsInitialized = false;
    return result;
}

} // namespace CTR
} // namespace cec
} // namespace nn
