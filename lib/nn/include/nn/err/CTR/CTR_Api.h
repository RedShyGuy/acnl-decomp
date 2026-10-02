#pragma once

#include "decomp.h"

// how ThrowFatalErr reports (C enum, name from the binary); the values are not named yet,
// nn::os::CTR::detail::HandleInternalError passes 0
enum nnerrFatalErrType : s32 {
};

namespace nn {
namespace err {
namespace CTR {
void ThrowFatalErr(nn::Result, nnerrFatalErrType, unsigned); // 0x00129E04 | nintendogs:bytes-fuzzy [tier A]
void ThrowFatalErr(nn::Result, unsigned); // 0x00129E3C | nintendogs:bytes [tier A]
void ThrowFatalErrAll(nn::Result result, uptr address); // 0x00129E84 | nintendogs:bytes [tier A]

// the address of this code: the callers pass it to ThrowFatalErr* ("mov rX, pc" in the original)
// (name is ours)
inline uptr GetCurrentAddress()
{
    uptr address;
    __asm__ __volatile__("mov %0, pc" : "=r"(address));
    return address;
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
