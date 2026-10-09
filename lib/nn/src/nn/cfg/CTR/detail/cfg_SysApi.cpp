// the functions of cfg:s and cfg:i (file name is ours: their globals and code lie apart from cfg:u)
#include "nn/cfg/CTR/detail/detail_Api.h"
#include "nn/os/os_CriticalSection.h"

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {
namespace {
// the ports (ARMCC loads the empty handle and the name of each from one .rodata block)
const char PORT_NAME_SYS[] = "cfg:s";
const char PORT_NAME_INIT[] = "cfg:i";
const nn::Handle INVALID_HANDLE;

const bit32 RESULT_NOT_FOUND = 0xD92103FB; // permanent, canceled, 1019: no session of the port
} // namespace

// the globals of cfg:i and cfg:s (names are ours)
// 0x0097FA48
s32 s_InitInitializeCount;
// 0x0097FA4C
bool s_IsSysInitialized;
// 0x0097FA50
s32 s_SysInitializeCount;
// 0x00982EB4
nn::Handle s_InitSession;
// 0x00982EB8
nn::Handle s_SysSession;

// 0x001242E0 | fefates:bytes [tier B]
void FinalizeSys()
{
    nn::os::CriticalSection& lock = *GetCriticalSectionForInitializeFinalize();
    lock.Enter();
    if (s_SysInitializeCount > 0) {
        s_SysInitializeCount--;
    }
    if (s_SysInitializeCount == 0 && s_IsSysInitialized) {
        s_IsSysInitialized = false;
        if (FinalizeBase(&s_SysSession).IsSuccess()) {
            s_Session = INVALID_HANDLE;
        }
    }
    lock.Exit();
}

// 0x0012435C | fefates:bytes [tier B]
void FinalizeInit()
{
    nn::os::CriticalSection& lock = *GetCriticalSectionForInitializeFinalize();
    lock.Enter();
    if (s_InitInitializeCount > 0) {
        s_InitInitializeCount--;
    }
    if (s_InitInitializeCount == 0) {
        if (FinalizeBase(&s_InitSession).IsSuccess()) {
            s_Session = INVALID_HANDLE;
            s_SysSession = INVALID_HANDLE;
        }
    }
    lock.Exit();
}

// 0x001243D0 | nintendogs:callgraph [tier A]
nn::Result InitializeSys()
{
    nn::os::CriticalSection& lock = *GetCriticalSectionForInitializeFinalize();
    lock.Enter();
    if (s_SysInitializeCount == 0) {
        nn::Result result = InitializeBase(&s_SysSession, PORT_NAME_SYS);
        if (result.IsSuccess()) {
            s_IsSysInitialized = true;
            s_Session = s_SysSession;
        } else if (result == RESULT_NOT_FOUND) {
            lock.Exit();
            return result;
        }
    }
    s_SysInitializeCount++;
    lock.Exit();
    return nn::Result();
}

// 0x00124478 | nintendogs:callgraph [tier A]
nn::Result InitializeInit()
{
    nn::os::CriticalSection& lock = *GetCriticalSectionForInitializeFinalize();
    lock.Enter();
    if (s_InitInitializeCount == 0) {
        nn::Result result = InitializeBase(&s_InitSession, PORT_NAME_INIT);
        if (result.IsSuccess()) {
            // cfg:i has the commands of cfg:s, too
            s_SysSession = s_InitSession;
            s_Session = s_InitSession;
        } else if (result == RESULT_NOT_FOUND) {
            lock.Exit();
            return result;
        }
    }
    s_InitInitializeCount++;
    lock.Exit();
    return nn::Result();
}

} // namespace detail
} // namespace CTR
} // namespace cfg
} // namespace nn
