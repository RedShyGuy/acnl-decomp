#pragma once

// Enumerations of nn::os that are passed to system calls.
// The type names are from the binary (nn::svc::CreateEvent(nn::Handle*, nn::os::ResetType),
// nn::svc::ArbitrateAddress(..., nn::os::ArbitrationType, ...)). The enumerator names are
// descriptive, the values are the kernel's (3dbrew "SVC").
//
// Only "types.h" may be included here: include/forward.h includes this header.

#include "types.h"

namespace nn {
namespace os {

enum ResetType {
    RESET_TYPE_ONESHOT = 0,
    RESET_TYPE_STICKY = 1,
    RESET_TYPE_PULSE = 2,
};

enum ArbitrationType {
    ARBITRATION_TYPE_SIGNAL = 0,
    ARBITRATION_TYPE_WAIT_IF_LESS_THAN = 1,
    ARBITRATION_TYPE_DECREMENT_AND_WAIT_IF_LESS_THAN = 2,
    ARBITRATION_TYPE_WAIT_IF_LESS_THAN_TIMEOUT = 3,
    ARBITRATION_TYPE_DECREMENT_AND_WAIT_IF_LESS_THAN_TIMEOUT = 4,
};

} // namespace os
} // namespace nn
