#pragma once

#include "decomp.h"
#include "nn/svc/svc_Api.h"

namespace sead {
// A point in time in system ticks (inline everywhere in sead; the out-of-line copies of the binary are
// pead::TickTime::TickTime and nn::pia::common::Time::SetNow, the same code). Member names are ours.
class TickTime
{
public:
    TickTime() : mTick(nn::svc::GetSystemTick()) {}
    void setNow() { mTick = nn::svc::GetSystemTick(); }

    u64 mTick; // 0x0
};
} // namespace sead
