#include "pead/peadEvent.h"
#include "nn/os/os_Tick.h"

namespace pead {
// 0x0053B8B4
void pead::Event::Clear()
{
    mEvent.ClearSignal();
}

// 0x0053B8D4
bool pead::Event::Wait(s64 ticks)
{
    nn::Result result =
        nn::svc::WaitSynchronization1(mEvent.GetHandle(), nn::os::Tick(ticks).ToTimeSpan().GetNanoSeconds());
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    return result.GetDescription() != DESCRIPTION_TIMEOUT;
}

// 0x0053B944
void pead::Event::Wait()
{
    mEvent.Wait();
}

// 0x0053B96C
void pead::Event::Signal()
{
    mEvent.Signal();
}

// 0x0053B98C
pead::Event::Event(bool manualReset) : IDisposer(), mEvent()
{
    mEvent.Initialize(manualReset ? nn::os::RESET_TYPE_STICKY : nn::os::RESET_TYPE_ONESHOT);
}

// 0x0053BA28 slot 0x00 | slot vf_0x00 of pead::IDisposer
// 0x0053B9E4 (deleting dtor)
pead::Event::~Event()
{
    // (only the members are destroyed: nn::os::Event closes its handle)
}
} // namespace pead
