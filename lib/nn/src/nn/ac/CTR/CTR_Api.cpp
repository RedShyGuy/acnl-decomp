// ac_Api.cpp (the name is ours; static initializer at 0x007842E0: s_Lock)
#include "nn/ac/CTR/CTR_Api.h"
#include <string.h>
#include "nn/ac/CTR/detail/ac_Ac.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/ndm/CTR/detail/ndm_Interface.h"
#include "nn/ndm/ndm_Api.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Event.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace ac {
namespace CTR {
namespace {
// results (module 39; the names are ours)
const bit32 RESULT_ALREADY_INITIALIZED = 0xD8A09FF9;         // permanent, invalid state, 1017
const bit32 RESULT_ALREADY_INITIALIZED_NOTHING = 0x00209FF9; // success, nothing happened, 1017
const bit32 RESULT_NOT_INITIALIZED_NOTHING = 0x00209FF8;     // success, nothing happened, 1016
const bit32 RESULT_INVALID_POINTER = 0xE0409FFF;             // usage, would block, 1023
const bit32 RESULT_EVENT_FAILED = 0xF9609FF3;                // usage, internal, 1011
const bit32 RESULT_EULA_NOT_AGREED = 0xC9209F84;             // status, canceled, 900
// GetConnectResult: the connection stays (exclusive mode is kept)
const bit32 RESULT_CONNECTION_KEPT = 0xE1209C46;             // usage, canceled, 70

const char SERVICE_NAME[] = "ac:u";
// what SetClientVersion tells ac:u
const u32 CLIENT_VERSION = 0x0B0500C8;

// the network is held exclusively for the infrastructure connection
// 0x00975AC4
bool s_IsNdmExclusive;
// 0x00975AC8
s32 s_InitializeCount;
// the session of ac:i (nothing in ACNL opens it; name is ours)
// 0x0097E7E0
nn::Handle s_SessionInternal;
// 0x00AE0A40
nn::os::CriticalSection s_Lock((nn::os::CriticalSection::InitializeTag()));

// (inline everywhere)
inline bool IsInitialized()
{
    s_Lock.Enter();
    bool isInitialized = s_InitializeCount != 0;
    s_Lock.Exit();
    return isInitialized;
}

// takes the network exclusively for an infrastructure connection (inline)
inline nn::Result NdmEnterExclusive()
{
    s_Lock.Enter();
    nn::Result result;
    if (!s_IsNdmExclusive) {
        result = nn::ndm::Initialize();
        if (result.IsSuccess()) {
            result = nn::ndm::CTR::detail::Interface::EnterExclusiveStateEntry(nn::ndm::CTR::EXCLUSIVE_MODE_INFRASTRUCTURE);
            if (result.IsSuccess()) {
                s_IsNdmExclusive = true;
            } else {
                nn::ndm::Finalize();
            }
        }
    }
    s_Lock.Exit();
    return result;
}

// 0x00345884 | fefates:bytes [tier B]
nn::Result NdmLeaveExclusive()
{
    s_Lock.Enter();
    nn::Result result;
    if (s_IsNdmExclusive) {
        result = nn::ndm::CTR::detail::Interface::LeaveExclusiveStateEntry();
        s_IsNdmExclusive = false;
        nn::Result finalizeResult = nn::ndm::Finalize();
        if (finalizeResult.IsFailure()) {
            result = finalizeResult;
        }
    }
    s_Lock.Exit();
    return result;
}

// (inline in Close)
inline nn::Result CloseAsync(nn::os::Event* pEvent)
{
    if (!pEvent->GetHandle().IsValid()) {
        return RESULT_INVALID_POINTER;
    }
    return nn::ac::CTR::detail::Ac::CloseAsync(pEvent->GetHandle());
}
} // namespace

// 0x003454A4 | fefates:bytes [tier B]
nn::Result Initialize()
{
    s_Lock.Enter();
    if (IsInitializedInternal()) {
        s_Lock.Exit();
        return RESULT_ALREADY_INITIALIZED;
    }
    if (IsInitialized()) {
        s_InitializeCount++;
        s_Lock.Exit();
        return RESULT_ALREADY_INITIALIZED_NOTHING;
    }
    nn::Result result = nn::srv::Initialize();
    if (result.IsFailure()) {
        s_Lock.Exit();
        return result;
    }
    result = nn::srv::GetServiceHandle(&detail::s_Session, SERVICE_NAME, strlen(SERVICE_NAME), 0);
    if (result.IsFailure()) {
        s_Lock.Exit();
        return result;
    }
    s_InitializeCount++;
    detail::Ac::SetClientVersion(CLIENT_VERSION);
    s_Lock.Exit();
    return nn::Result();
}

// 0x003455BC | nintendogs:bytes [tier A]
bool IsConnected()
{
    if (!IsInitialized() && !IsInitializedInternal()) {
        return false;
    }
    bool isConnected = false;
    detail::Ac::IsConnected(0, &isConnected);
    return isConnected;
}

// 0x0034561C (name is ours, after the command)
nn::Result GetLastErrorCode(unsigned int* pCode)
{
    if (pCode == 0) {
        return RESULT_INVALID_POINTER;
    }
    return detail::Ac::GetLastErrorCode(pCode);
}

// 0x00345634 | fefates:bytes [tier B]
nn::Result ConnectWithoutEula(nnacConfig& config)
{
    nn::os::Event event;
    if (event.TryInitialize(nn::os::RESET_TYPE_ONESHOT).IsFailure()) {
        return RESULT_EVENT_FAILED;
    }
    nn::Result result = ConnectAsyncWithoutEula(config, &event);
    if (result.IsSuccess()) {
        event.Wait();
    }
    event.Close();
    if (result.IsFailure()) {
        NdmLeaveExclusive();
        return result;
    }
    result = detail::Ac::GetConnectResult();
    if (result != nn::Result(RESULT_CONNECTION_KEPT)) {
        NdmLeaveExclusive();
    }
    return result;
}

// 0x00345718 (name is ours, after the command)
nn::Result CreateDefaultConfig(nnacConfig* pConfig)
{
    if (pConfig == 0) {
        return RESULT_INVALID_POINTER;
    }
    return detail::Ac::CreateDefaultConfig(pConfig);
}

// 0x00345730 | nintendogs:callgraph [tier A]
bool IsInitializedInternal()
{
    s_Lock.Enter();
    bool isInitialized = s_SessionInternal.IsValid();
    s_Lock.Exit();
    return isInitialized;
}

// 0x0034576C (name is ours, after the command)
nn::Result GetLastDetailErrorCode(unsigned int* pCode)
{
    if (pCode == 0) {
        return RESULT_INVALID_POINTER;
    }
    return detail::Ac::GetLastDetailErrorCode(pCode);
}

// 0x00345784 | fefates:bytes [tier B]
nn::Result ConnectAsyncWithoutEula(nnacConfig& config, nn::os::Event* pEvent)
{
    InfraPriority priority;
    nn::Result result = detail::Ac::GetInfraPriority(config, &priority);
    if (result.IsFailure()) {
        return result;
    }
    if (priority != INFRA_PRIORITY_0) {
        detail::Ac::AddDenyApType(config, &config, AP_TYPE_0x40);
    } else {
        result = NdmEnterExclusive();
        if (result.IsFailure()) {
            return result;
        }
        detail::Ac::SetRequestEulaVersion(config, &config, 0, 0);
    }
    return detail::Ac::ConnectAsync(config, pEvent->GetHandle());
}

// 0x00345864 | fefates:bytes [tier B]
nn::Result RegisterDisconnectEvent(nn::os::Event* pEvent)
{
    if (pEvent == 0 || !pEvent->GetHandle().IsValid()) {
        return RESULT_INVALID_POINTER;
    }
    return detail::Ac::RegisterDisconnectEvent(pEvent->GetHandle());
}

// 0x003458DC | fefates:bytes [tier B]
nn::Result Close()
{
    nn::os::Event event;
    if (event.TryInitialize(nn::os::RESET_TYPE_ONESHOT).IsFailure()) {
        return RESULT_EVENT_FAILED;
    }
    nn::Result result = CloseAsync(&event);
    if (result.IsSuccess()) {
        event.Wait();
    }
    event.Close();
    if (result.IsFailure()) {
        return result;
    }
    return detail::Ac::GetCloseResult();
}

// 0x00345DD4 | fefates:bytes [tier B]
nn::Result Connect(nnacConfig& config)
{
    nn::cfg::CTR::Initialize();
    bool isAgreed = nn::cfg::CTR::IsAgreedEula();
    nn::cfg::CTR::Finalize();
    if (!isAgreed) {
        return RESULT_EULA_NOT_AGREED;
    }
    return ConnectWithoutEula(config);
}

// 0x00345E0C | fefates:bytes [tier B]
nn::Result Finalize()
{
    s_Lock.Enter();
    if (!IsInitialized()) {
        s_Lock.Exit();
        return RESULT_NOT_INITIALIZED_NOTHING;
    }
    NdmLeaveExclusive();
    if (--s_InitializeCount == 0) {
        nn::Result result = nn::svc::CloseHandle(detail::s_Session);
        if (result.IsFailure()) {
            s_Lock.Exit();
            return result;
        }
    }
    s_Lock.Exit();
    return nn::Result();
}

} // namespace CTR
} // namespace ac
} // namespace nn
