#pragma once

// nn::os::Event - a kernel event (the class name is from the binary, e.g.
// nn::uds::CTR::Initialize(nn::os::Event*, ...)). Everything it does is in EventBase.

#include "decomp.h"
#include "nn/os/os_EventBase.h"

namespace nn {
namespace os {

class Event : public EventBase
{
};

} // namespace os
} // namespace nn
