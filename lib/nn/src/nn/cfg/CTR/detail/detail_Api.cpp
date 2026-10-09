#include "nn/cfg/CTR/detail/detail_Api.h"
#include <new>
#include <string.h>
#include "nn/cfg/CTR/detail/cfg_IpcUser.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Thread.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {
namespace {
// the ports (ARMCC loads the empty handle and the name of each from one .rodata block)
const char PORT_NAME_USER[] = "cfg:u";
const nn::Handle INVALID_HANDLE;

// results (module cfg, 64; the names are ours)
const bit32 RESULT_NOT_INITIALIZED = 0xD8A103F7;     // permanent, invalid state, 1015
const bit32 RESULT_ALREADY_INITIALIZED = 0xD8A103F9; // permanent, invalid state, 1017
const bit32 RESULT_NOT_FOUND = 0xD92103FB;           // permanent, canceled, 1019: no session of the port
const bit32 RESULT_NO_PORT = 0xD90103EA;             // permanent, wrong argument, 1002

// the lock of GetCriticalSectionForInitializeFinalize: 0 none, 1 being made, else the address
const s32 LOCK_STATE_NONE = 0;
const s32 LOCK_STATE_MAKING = 1;
const s64 LOCK_WAIT_MSEC = 1;
} // namespace

// the globals of cfg:u (ARMCC addresses them from 0x0097E830; names are ours)
// 0x0097E830
bool s_IsInitialized;
// 0x0097E834
volatile s32 s_InitializeFinalizeLock;
// 0x0097E838
s32 s_InitializeCount;
// 0x0097E83C
nn::Handle s_Session;
// the memory of the lock
// 0x00AE9554
u32 s_InitializeFinalizeLockStorage[sizeof(nn::os::CriticalSection) / sizeof(u32)];

// 0x00129C44 | fefates:bytes [tier B]
nn::os::CriticalSection* GetCriticalSectionForInitializeFinalize()
{
    if (nn::os::detail::AtomicCompareAndSwap(&s_InitializeFinalizeLock, LOCK_STATE_NONE, LOCK_STATE_MAKING) == LOCK_STATE_NONE) {
        nn::os::CriticalSection* lock =
            new (s_InitializeFinalizeLockStorage) nn::os::CriticalSection(nn::os::CriticalSection::InitializeTag());
        nn::os::detail::AtomicStore(&s_InitializeFinalizeLock, reinterpret_cast<s32>(lock));
    } else {
        // another thread is making it
        while (static_cast<u32>(s_InitializeFinalizeLock) <= LOCK_STATE_MAKING) {
            nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(LOCK_WAIT_MSEC));
        }
    }
    return reinterpret_cast<nn::os::CriticalSection*>(s_InitializeFinalizeLock);
}

// 0x00129BA4 | nintendogs:bytes [tier A]
nn::Result FinalizeBase(nn::Handle* session)
{
    if (!session->IsValid()) {
        return RESULT_NOT_INITIALIZED;
    }
    nn::Result result = nn::svc::CloseHandle(*session);
    if (result.IsFailure()) {
        nndbgPanic();
    }
    *session = INVALID_HANDLE;
    return result;
}

// 0x00129BE8 | nintendogs:bytes [tier A]
nn::Result InitializeBase(nn::Handle* session, const char* name)
{
    if (nn::srv::Initialize().IsFailure()) {
        nndbgPanic();
    }
    if (session->IsValid()) {
        return RESULT_ALREADY_INITIALIZED;
    }
    nn::Result result = nn::srv::GetServiceHandle(session, name, strlen(name), 0);
    if (result.IsFailure()) {
        return RESULT_NOT_FOUND;
    }
    return result;
}

// 0x0011FCF0 | nintendogs:callgraph [tier A]
nn::Result Initialize()
{
    nn::os::CriticalSection& lock = *GetCriticalSectionForInitializeFinalize();
    lock.Enter();
    if (s_InitializeCount == 0) {
        nn::Result result = InitializeBase(&s_Session, PORT_NAME_USER);
        if (result.IsSuccess()) {
            s_IsInitialized = true;
        } else if (result == RESULT_NOT_FOUND) {
            lock.Exit();
            return result;
        }
    }
    s_InitializeCount++;
    lock.Exit();
    return nn::Result();
}

// 0x001367E4 | fefates:bytes [tier B]
void Finalize()
{
    nn::os::CriticalSection& lock = *GetCriticalSectionForInitializeFinalize();
    lock.Enter();
    if (s_InitializeCount > 0) {
        s_InitializeCount--;
    }
    if (s_InitializeCount == 0 && s_IsInitialized) {
        s_IsInitialized = false;
        if (s_Session.IsValid()) {
            FinalizeBase(&s_Session);
        }
    }
    lock.Exit();
}

// 0x0011FDCC | nintendogs:callgraph [tier A]
void FinalizeProperPort(nn::cfg::CTR::detail::_IPCPortType type)
{
    switch (type) {
    case IPC_PORT_TYPE_USER:
        Finalize();
        break;
    case IPC_PORT_TYPE_SYS:
        FinalizeSys();
        break;
    case IPC_PORT_TYPE_INIT:
        FinalizeInit();
        break;
    default:
        break;
    }
}

// 0x0011FE84 | nintendogs:bytes [tier A]
nn::Result InitializeProperPort(nn::cfg::CTR::detail::_IPCPortType* type)
{
    if (Initialize().IsSuccess()) {
        *type = IPC_PORT_TYPE_USER;
        return nn::Result();
    }
    if (InitializeSys().IsSuccess()) {
        *type = IPC_PORT_TYPE_SYS;
        return nn::Result();
    }
    if (InitializeInit().IsSuccess()) {
        *type = IPC_PORT_TYPE_INIT;
        return nn::Result();
    }
    return RESULT_NO_PORT;
}

// 0x0011FEF0 | nintendogs:bytes [tier A]
nn::cfg::CTR::CfgRegionCode GetRegion()
{
    nn::cfg::CTR::CfgRegionCode region = CFG_REGION_JPN;
    IpcUser::GetRegion(&region);
    return region;
}

// 0x00124524 | nintendogs:callgraph [tier A]
// (a nop that falls into IpcUser::GetConfig)
nn::Result GetConfig(void* buffer, size_t size, u32 blockId)
{
    return IpcUser::GetConfig(buffer, size, blockId);
}

// 0x00350F48 | nintendogs:callgraph [tier A]
nn::Result GetTransferableId(u32 unknown, u64* id)
{
    return IpcUser::GetTransferableId(unknown, id);
}

// 0x00350F90
nn::Result GetRegionCanadaUSA(bool* isCanadaOrUsa)
{
    return IpcUser::GetRegionCanadaUSA(isCanadaOrUsa);
}

// 0x00350FD0
nn::Result GetCountryCodeString(char* string, u16 countryCodeId)
{
    return IpcUser::GetCountryCodeString(string, countryCodeId);
}

} // namespace detail
} // namespace CTR
} // namespace cfg
} // namespace nn
