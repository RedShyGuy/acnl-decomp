#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/err/CTR/CTR_Api.h"

namespace nn {
namespace os {
namespace CTR {
namespace detail {
namespace {

// the system's shared configuration page: ENVINFO, bit 0 set on retail units (3dbrew
// "Configuration Memory")
const uptr CONFIG_MEMORY_ENVINFO = 0x1FF80014;

// Result levels as signed 5 bit numbers
const s32 LEVEL_INFO = 1;
const s32 LEVEL_STATUS = -7;
const s32 LEVEL_FATAL = -1;

} // namespace

// 0x00975F70
bool s_InternalErrorHandlingMode;

// 0x0011D534 | nintendogs:callgraph [tier A]
void SetInternalErrorHandlingMode(bool mode)
{
    s_InternalErrorHandlingMode = mode;
}

// 0x001308F0
// On retail units (or with the mode set) everything but info / status results is a fatal error;
// on development units only fatal results are reported, then the program stops (for the debugger).
void HandleInternalError(nn::Result result)
{
    s32 level = static_cast<s32>(result.GetPrintableBits()) >> 27;
    bool isRetailUnit = *reinterpret_cast<volatile u8*>(CONFIG_MEMORY_ENVINFO) & 1;
    uptr caller = reinterpret_cast<uptr>(__builtin_return_address(0));
    if (isRetailUnit || s_InternalErrorHandlingMode) {
        if (level == LEVEL_STATUS || level == LEVEL_INFO) {
            return;
        }
        nn::err::CTR::ThrowFatalErr(result, NN_ERR_FATAL_ERR_TYPE_GENERIC, caller);
        return;
    }
    if (level == LEVEL_FATAL) {
        nn::err::CTR::ThrowFatalErr(result, NN_ERR_FATAL_ERR_TYPE_GENERIC, caller);
    }
    nndbgPanic();
}

} // namespace detail
} // namespace CTR
} // namespace os
} // namespace nn
