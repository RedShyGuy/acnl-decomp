// ndm_Api.cpp (the name is ours; static initializer at 0x0079A2F0: s_Lock)
#include "nn/ndm/ndm_Api.h"
#include <string.h>
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/ndm/CTR/detail/ndm_Interface.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace ndm {
namespace {
// results (module 26; the names are ours)
const bit32 RESULT_OUT_OF_RANGE = 0xD8E06BED;   // permanent, invalid argument, 1005
const bit32 RESULT_NOT_INITIALIZED = 0xC8A06BF8; // status, invalid state, 1016

const char SERVICE_NAME[] = "ndm:u";

// what an application leaves to the system: BOSS and NIM
const bit32 SUSPENDED_DAEMONS_DEFAULT = nn::ndm::CTR::DAEMON_MASK_BOSS | nn::ndm::CTR::DAEMON_MASK_NIM;

// 0x00975F58
s32 s_InitializeCount;
// 0x00AE1F38
nn::os::CriticalSection s_Lock((nn::os::CriticalSection::InitializeTag()));

inline void PanicIfFailed(nn::Result result)
{
    if (result.IsFailure()) {
        nndbgPanic();
    }
}
} // namespace

// 0x0011E2B8 | nintendogs:bytes [tier A]
void SetupDaemonsDefault()
{
    if (nn::applet::CTR::IsInitialized() && nn::applet::CTR::GetAppletType() == 0) {
        PanicIfFailed(Initialize());
        nn::ndm::CTR::detail::Interface::OverrideDefaultDaemonsEntry(nn::ndm::CTR::DAEMON_MASK_ALL);
        SuspendDaemons(SUSPENDED_DAEMONS_DEFAULT);
    }
}

// 0x0011FF34 | nintendogs:bytes [tier A]
nn::Result Initialize()
{
    s_Lock.Enter();
    if (s_InitializeCount == 0) {
        nn::srv::Initialize();
        nn::Result result = nn::srv::GetServiceHandle(&nn::ndm::CTR::detail::s_Session, SERVICE_NAME, strlen(SERVICE_NAME), 0);
        if (result.IsFailure()) {
            s_Lock.Exit();
            return result;
        }
    }
    s_InitializeCount++;
    s_Lock.Exit();
    return nn::Result();
}

// 0x00124720 | nintendogs:callgraph [tier A]
nn::Result SuspendDaemons(bit32 mask)
{
    return nn::ndm::CTR::detail::Interface::SuspendDaemons(mask);
}

// 0x0014370C | nintendogs:callgraph [tier A]
nn::Result Resume(nn::ndm::CTR::DaemonName name)
{
    if (name >= nn::ndm::CTR::DAEMON_NAME_MAX) {
        return RESULT_OUT_OF_RANGE;
    }
    return nn::ndm::CTR::detail::Interface::ResumeDaemons(1 << name);
}

// 0x00354ACC | nintendogs:bytes [tier B]
nn::Result QueryExclusiveMode(nn::ndm::CTR::ExclusiveMode& mode)
{
    int value;
    nn::Result result = nn::ndm::CTR::detail::Interface::QueryExclusiveMode(&value);
    if (result.IsFailure()) {
        return result;
    }
    mode = static_cast<nn::ndm::CTR::ExclusiveMode>(value);
    return nn::Result();
}

// 0x00354BB4 | nintendogs:callgraph [tier A]
nn::Result Suspend(nn::ndm::CTR::DaemonName name)
{
    if (name >= nn::ndm::CTR::DAEMON_NAME_MAX) {
        return RESULT_OUT_OF_RANGE;
    }
    return nn::ndm::CTR::detail::Interface::SuspendDaemons(1 << name);
}

// 0x00354BD4 | nintendogs:bytes [tier A]
nn::Result Finalize()
{
    s_Lock.Enter();
    if (s_InitializeCount == 0) {
        s_Lock.Exit();
        return RESULT_NOT_INITIALIZED;
    }
    if (s_InitializeCount == 1) {
        nn::Result result = nn::svc::CloseHandle(nn::ndm::CTR::detail::s_Session);
        if (result.IsFailure()) {
            s_Lock.Exit();
            return result;
        }
        nn::ndm::CTR::detail::s_Session = nn::Handle();
    }
    s_InitializeCount--;
    s_Lock.Exit();
    return nn::Result();
}

} // namespace ndm
} // namespace nn
