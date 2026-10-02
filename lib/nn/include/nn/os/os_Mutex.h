#pragma once

// nn::os::Mutex - a kernel mutex. Only what code in ACNL uses so far (DefaultAutoStackManager),
// all inline; the names of the methods are ours.

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_WaitObject.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {

class Mutex : public WaitObject
{
public:
    void Initialize(bool initialLocked)
    {
        nn::Result result;
        if (mHandle.IsValid()) {
            result = nn::Result(RESULT_ALREADY_INITIALIZED);
        } else {
            nn::Handle handle;
            result = nn::svc::CreateMutex(&handle, initialLocked);
            if (result.IsSuccess()) {
                mHandle = handle;
                result = nn::Result();
            }
        }
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
    }

    void Lock()
    {
        nn::Result result = nn::svc::WaitSynchronization1(mHandle, -1);
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
    }

    void Finalize() { Close(); }

    // the mutex already has a handle (level 28, summary invalid state, module os, description 59)
    static const bit32 RESULT_ALREADY_INITIALIZED = 0xE0A0183B;
};

} // namespace os
} // namespace nn
