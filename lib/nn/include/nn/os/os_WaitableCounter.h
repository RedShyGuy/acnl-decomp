#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/os/os_Types.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
class WaitableCounter
{
public:
    // creates the address arbiter once
    static void Initialize(); // 0x0011DEC8 | nintendogs:bytes [tier A]

    // the process wide address arbiter that all counter based waits use
    // (SimpleLock, LightEvent, ...); name is ours
    static nn::Handle s_ArbitrationObject; // 0x0097F018
};

namespace detail {

// sleeps while *counter < value (all counter based waits; the names are ours)
inline void ArbitrateWaitIfLessThan(volatile s32* counter, s32 value)
{
    nn::svc::ArbitrateAddress(WaitableCounter::s_ArbitrationObject, reinterpret_cast<uptr>(counter),
                              ARBITRATION_TYPE_WAIT_IF_LESS_THAN, value, 0);
}

// wakes count threads sleeping on counter (-1: all)
inline void ArbitrateSignal(volatile s32* counter, s32 count)
{
    nn::svc::ArbitrateAddress(WaitableCounter::s_ArbitrationObject, reinterpret_cast<uptr>(counter),
                              ARBITRATION_TYPE_SIGNAL, count, 0);
}

} // namespace detail
} // namespace os
} // namespace nn
