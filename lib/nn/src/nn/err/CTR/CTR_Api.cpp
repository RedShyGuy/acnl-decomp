// err_Api.cpp (static initializer __sti___11_err_Api_cpp at 0x00785708)
#include "nn/err/CTR/CTR_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/err/CTR/err_FatalErr.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_LightEvent.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace err {
namespace CTR {
namespace {
// the revision of the info (the names are ours)
const u8 REVISION_HIGH = 0;
const u16 REVISION_LOW = 0xF22C;

const char PORT_NAME[] = "err:f";
// (ARMCC loads both handles and the name from one .rodata block at 0x0089E930)
const nn::Handle CURRENT_PROCESS(PSEUDO_HANDLE_CURRENT_PROCESS);
const nn::Handle INVALID_HANDLE;

// ConnectToPort while all sessions of the port are in use: temporary, would block, os, 52
// (name is ours)
const bit32 RESULT_PORT_SESSIONS_FULL = 0xD0401834;
const s64 RETRY_INTERVAL_MSEC = 5;

// the levels of results that are no errors (signed): info and status
const s32 LEVEL_INFO = 1;
const s32 LEVEL_STATUS = -7;
} // namespace

// the globals of this file (names are ours)
// 0x00975EF0
nn::Handle s_Session(INVALID_HANDLE);
// 0x00AE1AB8
FatalErrInfo s_Info;
// 0x00AE1B38
nn::os::CriticalSection s_Lock((nn::os::CriticalSection::InitializeTag()));

namespace {
// 0x00129EB4 | fefates:bytes [tier B]
// sends the info to err:f; the program stops here unless the card was removed or the error is
// only logged
DECOMP_NOIPA void Throw(FatalErrInfo& info)
{
    for (;;) {
        s_Lock.Enter();
        nn::Result result;
        if (!s_Session.IsValid()) {
            result = nn::svc::ConnectToPort(&s_Session, PORT_NAME);
        }
        if (result.IsSuccess()) {
            nn::svc::GetProcessId(&info.processId, CURRENT_PROCESS);
            FatalErr(s_Session).Throw(info);
            if (s_Session.IsValid()) {
                nn::svc::CloseHandle(s_Session);
                s_Session = INVALID_HANDLE;
            }
            break;
        }
        // with the card removed the error must get through: wait for a free session
        if (result != RESULT_PORT_SESSIONS_FULL || info.type != NN_ERR_FATAL_ERR_TYPE_CARD_REMOVED) {
            break;
        }
        nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(RETRY_INTERVAL_MSEC));
        s_Lock.Exit();
    }
    s_Lock.Exit();
    if (info.type != NN_ERR_FATAL_ERR_TYPE_CARD_REMOVED && info.type != NN_ERR_FATAL_ERR_TYPE_LOGGED) {
        // waits forever
        nn::os::LightEvent event;
        event.Initialize(true);
        event.Wait();
    }
}

inline void SetInfo(u8 type, nn::Result result, uptr address)
{
    s_Info.type = type;
    s_Info.revisionHigh = REVISION_HIGH;
    s_Info.revisionLow = REVISION_LOW;
    s_Info.pcAddress = address;
    s_Info.resultCode = result.GetPrintableBits();
}
} // namespace

// 0x00129DC4 (name is ours)
void ThrowFatalErrLogged(nn::Result result, uptr address)
{
    SetInfo(NN_ERR_FATAL_ERR_TYPE_LOGGED, result, address);
    Throw(s_Info);
    // (ARMCC branches to a copy of nndbgPanic at 0x00129CE4)
    nndbgPanic();
}

// 0x00129E04 | nintendogs:bytes-fuzzy [tier A]
void ThrowFatalErr(nn::Result result, nnerrFatalErrType type, uptr address)
{
    SetInfo(static_cast<u8>(type), result, address);
    Throw(s_Info);
}

// 0x00129E3C | nintendogs:bytes [tier A]
void ThrowFatalErr(nn::Result result, uptr address)
{
    s32 level = static_cast<s32>(result.GetPrintableBits()) >> 27;
    if (level == LEVEL_INFO || level == LEVEL_STATUS) {
        return;
    }
    SetInfo(NN_ERR_FATAL_ERR_TYPE_GENERIC, result, address);
    Throw(s_Info);
}

// 0x00129E84 | nintendogs:bytes [tier A]
void ThrowFatalErrAll(nn::Result result, uptr address)
{
    SetInfo(NN_ERR_FATAL_ERR_TYPE_GENERIC, result, address);
    Throw(s_Info);
}

} // namespace CTR
} // namespace err
} // namespace nn
