#pragma once

// The timeout of the waiting loops of the applet library (Receive, Send, WaitForRegister): the
// start tick is only read for a real timeout, WAIT_NONE gives up at once and WAIT_INFINITE never.
// Inline in the original; the names are ours.

#include "nn/applet/CTR/CTR_Api.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_Tick.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
inline s64 GetTimeoutStart(const nn::fnd::TimeSpan& timeout)
{
    if (timeout.GetNanoSeconds() == WAIT_NONE.GetNanoSeconds() ||
        timeout.GetNanoSeconds() == WAIT_INFINITE.GetNanoSeconds()) {
        return 0;
    }
    return nn::svc::GetSystemTick();
}

inline bool IsTimedOut(const nn::fnd::TimeSpan& timeout, s64 startTick)
{
    if (timeout.GetNanoSeconds() == WAIT_NONE.GetNanoSeconds()) {
        return true;
    }
    if (timeout.GetNanoSeconds() == WAIT_INFINITE.GetNanoSeconds()) {
        return false;
    }
    return timeout.GetNanoSeconds() < nn::os::Tick(nn::svc::GetSystemTick() - startTick).ToTimeSpan().GetNanoSeconds();
}
} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
