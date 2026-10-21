#include "nn/dbg/dbg_Api.h"
#include "nn/svc/svc_Api.h"

namespace {
// a function that nndbgPanic calls once before the break; nothing in this program sets it (the
// arguments are all 0; the names are ours)
typedef void (*PanicHandler)(u32, u32, u32, u32, u32, u32);
} // namespace

// 0x00975EEC (name is ours)
PanicHandler s_PanicHandler;

// 0x0013590C
// (ARMCC also made a copy at 0x00129CE4 that nn::err::CTR::ThrowFatalErrLogged branches to)
extern "C" void nndbgPanic()
{
    PanicHandler handler = s_PanicHandler;
    if (handler != nullptr) {
        s_PanicHandler = nullptr;
        handler(0, 0, 0, 0, 0, 0);
    }
    nn::svc::Break(0, nullptr, 0);
}
