#include "nn/os/os_WaitableCounter.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
// 0x0097F018
nn::Handle WaitableCounter::s_ArbitrationObject;

// 0x0011DEC8 | nintendogs:bytes [tier A]
void nn::os::WaitableCounter::Initialize()
{
    if (s_ArbitrationObject.IsValid()) {
        return;
    }
    nn::Handle arbiter;
    if (nn::svc::CreateAddressArbiter(&arbiter).IsSuccess()) {
        s_ArbitrationObject = arbiter;
    }
}

} // namespace os
} // namespace nn
