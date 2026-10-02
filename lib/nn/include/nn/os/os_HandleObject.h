#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/svc/svc_Api.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace os {
// RTTI N2nn2os12HandleObjectE @ 0x008CDDDC
class HandleObject : public ::nn::util::ADLFireWall::NonCopyable<nn::os::HandleObject>
{
public:
    HandleObject() {}
    // inline: every derived destructor ends with this (e.g. Thread::~Thread)
    ~HandleObject() { Close(); }

    nn::Handle GetHandle() const { return mHandle; }

    // closes the handle if there is one (name is ours)
    void Close()
    {
        if (mHandle.IsValid()) {
            nn::svc::CloseHandle(mHandle);
            mHandle = nn::Handle();
        }
    }

protected:
    nn::Handle mHandle;
};
} // namespace os
} // namespace nn
