#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_InterruptEvent.h"
#include "nn/os/os_Types.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
// RTTI N2nn2os9EventBaseE @ 0x008CDE1C
class EventBase : public ::nn::os::InterruptEvent
{
public:
    EventBase() {}
    // out of resource failures are returned, any other failure is fatal
    nn::Result TryInitialize(nn::os::ResetType resetType); // 0x0034C9A0 | fefates:bytes [tier B]

    // inline parts (names are ours); TryInitialize starts with TryInitializeImpl
    nn::Result TryInitializeImpl(nn::os::ResetType resetType)
    {
        if (mHandle.IsValid()) {
            return nn::Result(RESULT_ALREADY_INITIALIZED);
        }
        nn::Handle handle;
        nn::Result result = nn::svc::CreateEvent(&handle, resetType);
        if (result.IsFailure()) {
            return result;
        }
        mHandle = handle;
        return nn::Result();
    }

    void Initialize(nn::os::ResetType resetType)
    {
        nn::Result result = TryInitializeImpl(resetType);
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
    }

    // takes over handle (closes the one it had; the name is ours)
    void AttachHandle(nn::Handle handle)
    {
        Close();
        mHandle = handle;
    }

    void Signal()
    {
        nn::Result result = nn::svc::SignalEvent(mHandle);
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
    }

    // waits without a timeout (inline, e.g. in gxlow's interrupt thread; the name is ours)
    void Wait()
    {
        nn::Result result = nn::svc::WaitSynchronization1(mHandle, -1);
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
    }

    // resets the event (inline; the name is ours)
    void ClearSignal()
    {
        nn::Result result = nn::svc::ClearEvent(mHandle);
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
    }

    // the event already has a handle (level 28, summary invalid state, module os, description 59)
    static const bit32 RESULT_ALREADY_INITIALIZED = 0xE0A0183B;
};
} // namespace os
} // namespace nn
