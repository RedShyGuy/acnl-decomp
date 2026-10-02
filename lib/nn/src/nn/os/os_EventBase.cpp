#include "nn/os/os_InterruptEvent.h"
#include "nn/os/os_EventBase.h"

namespace nn {
namespace os {
// EventBase::EventBase() is inline in the header

// 0x0034C9A0 | fefates:bytes [tier B]
nn::Result nn::os::EventBase::TryInitialize(nn::os::ResetType resetType)
{
    const bit32 SUMMARY_OUT_OF_RESOURCE = 3;
    nn::Result result = TryInitializeImpl(resetType);
    if (result.GetSummary() != SUMMARY_OUT_OF_RESOURCE && result.IsFailure()) {
        CTR::detail::HandleInternalError(result);
    }
    return result;
}

} // namespace os
} // namespace nn
