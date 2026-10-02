#include "nn/os/CTR/CTR_Api.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/os/os_ThreadLocalRegion.h"

#include <stdlib.h>

// ARM C++ runtime: the default unexpected handler (Thumb code at 0x001007A4, reached through the
// ARM veneer below; the name is ours), and the exception buffer setup that is only linked in
// when used (weak; ACNL's literal is 0)
// 0x00100798
extern "C" void nnosDefaultUnexpectedHandler();
extern "C" void* __ARM_exceptions_buffer_init() __attribute__((weak));

namespace nn {
namespace os {
namespace CTR {
namespace {

// what the ARM C++ runtime keeps per thread (member names are ours)
struct CppExceptionEnvironment {
    void* caughtExceptions;             // 0x00
    void (*unexpectedHandler)();        // 0x04
    void (*terminateHandler)();         // 0x08
    bool unknown0C;                     // 0x0C
    u32 unknown10;                      // 0x10
    u32 unknown14;                      // 0x14
    u32 unknown18;                      // 0x18
    void* exceptionsBuffer;             // 0x1C
};

// in the thread local region: +0x5C points to the environment at +0x60
const s32 TLR_CPP_EXCEPTION_POINTER = 0x5C / 4;
const s32 TLR_CPP_EXCEPTION_ENVIRONMENT = 0x60 / 4;

const u8 RUNNING_MODE_EXT_A = 2;
const u8 RUNNING_MODE_EXT_B = 4;

// 0x00123DC4
void DefaultTerminateHandler()
{
    abort();
}

bool IsTargetPlatformSnake()
{
    nn::ptm::CTR::TargetPlatform platform;
    if (nn::applet::CTR::detail::GetTargetPlatform(&platform).IsFailure()) {
        nndbgPanic();
    }
    return platform != nn::ptm::CTR::TARGET_PLATFORM_CTR;
}

} // namespace

// 0x0097F00C (names are ours)
uptr s_WramAddress;
// 0x0097F010
size_t s_WramSize;

// 0x0011D4C8 | fefates:bytes [tier B]
bool IsRunOnSnake()
{
    // 0x00975F68 (guard at 0x00975F6C)
    static const bool s_IsRunOnSnake = IsTargetPlatformSnake();
    return s_IsRunOnSnake;
}

// 0x00123D64 | nintendogs:bytes [tier A]
void SetupThreadCppExceptionEnvironment()
{
    uptr* tlr = detail::GetThreadLocalRegion();
    CppExceptionEnvironment* env = reinterpret_cast<CppExceptionEnvironment*>(tlr + TLR_CPP_EXCEPTION_ENVIRONMENT);
    tlr[TLR_CPP_EXCEPTION_POINTER] = reinterpret_cast<uptr>(env);
    env->unexpectedHandler = nnosDefaultUnexpectedHandler;
    env->terminateHandler = DefaultTerminateHandler;
    env->caughtExceptions = 0;
    env->unknown0C = false;
    env->unknown10 = 0;
    env->unknown14 = 0;
    env->unknown18 = 0;
    env->exceptionsBuffer = __ARM_exceptions_buffer_init ? __ARM_exceptions_buffer_init() : 0;
}

// 0x0034C750 | fefates:bytes [tier B]
bool IsRunningAsExtApplication()
{
    if (!nn::applet::CTR::IsInitialized()) {
        nndbgPanic();
    }
    nn::applet::CTR::ApplicationRunningMode mode;
    if (nn::applet::CTR::detail::GetApplicationRunningMode(&mode).IsSuccess() &&
        (mode == RUNNING_MODE_EXT_A || mode == RUNNING_MODE_EXT_B)) {
        return true;
    }
    return false;
}

// 0x00144B70 | tier C
uptr GetWramAddress()
{
    return s_WramAddress;
}

// 0x00144B60 | tier C
size_t GetWramSize()
{
    return s_WramSize;
}

// 0x0014316C
bool IsWramEnabled()
{
    return s_WramSize != 0;
}

} // namespace CTR
} // namespace os
} // namespace nn
