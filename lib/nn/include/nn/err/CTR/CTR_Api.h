#pragma once

#include "decomp.h"

// how ThrowFatalErr reports (C enum, name from the binary; the values after 3dbrew "ERR:Throw")
enum nnerrFatalErrType : s32 {
    NN_ERR_FATAL_ERR_TYPE_GENERIC = 0,
    NN_ERR_FATAL_ERR_TYPE_CORRUPTED = 1,
    NN_ERR_FATAL_ERR_TYPE_CARD_REMOVED = 2,
    NN_ERR_FATAL_ERR_TYPE_EXCEPTION = 3,
    NN_ERR_FATAL_ERR_TYPE_RESULT_FAILURE = 4,
    NN_ERR_FATAL_ERR_TYPE_LOGGED = 5,
};

namespace nn {
namespace err {
namespace CTR {
// the fatal error screen of err:f; the program stops (except for a removed card or a logged error)
void ThrowFatalErr(nn::Result result, nnerrFatalErrType type, uptr address); // 0x00129E04 | nintendogs:bytes-fuzzy [tier A]
// not for results of the levels info and status
void ThrowFatalErr(nn::Result result, uptr address); // 0x00129E3C | nintendogs:bytes [tier A]
void ThrowFatalErrAll(nn::Result result, uptr address); // 0x00129E84 | nintendogs:bytes [tier A]
// logs the error with err:f, then nndbgPanic (name is ours)
void ThrowFatalErrLogged(nn::Result result, uptr address); // 0x00129DC4

// the address of this code: the callers pass it to ThrowFatalErr* ("mov rX, pc" in the original)
// (name is ours)
inline uptr GetCurrentAddress()
{
    uptr address;
    __asm__ __volatile__("mov %0, pc" : "=r"(address));
    return address;
}

// stops with a fatal error when result is a failure (name is ours)
inline void ThrowFatalErrIfFailure(nn::Result result)
{
    uptr address = GetCurrentAddress();
    if (result.IsFailure()) {
        ThrowFatalErr(result, address);
    }
}

// stops with a fatal error screen when result is a failure (name is ours)
inline void ThrowFatalErrAllIfFailure(nn::Result result)
{
    uptr address = GetCurrentAddress();
    if (result.IsFailure()) {
        ThrowFatalErrAll(result, address);
    }
}
} // namespace CTR
} // namespace err
} // namespace nn
