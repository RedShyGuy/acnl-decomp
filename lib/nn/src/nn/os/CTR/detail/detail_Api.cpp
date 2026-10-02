#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/err/CTR/CTR_Api.h"

namespace nn {
namespace os {
namespace CTR {
namespace detail {
namespace {

// the system's shared configuration page: UNITINFO, bit 0 set on development units (3dbrew)
const uptr CONFIG_UNITINFO = 0x1FF80014;

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
// On development units (or with the mode set) everything but info / status results is a fatal
// error; on retail units only fatal results are reported, then the program stops.
void HandleInternalError(nn::Result result)
{
    s32 level = static_cast<s32>(result.GetPrintableBits()) >> 27;
    bool isDevelopmentUnit = *reinterpret_cast<volatile u8*>(CONFIG_UNITINFO) & 1;
    uptr caller = reinterpret_cast<uptr>(__builtin_return_address(0));
    if (isDevelopmentUnit || s_InternalErrorHandlingMode) {
        if (level == LEVEL_STATUS || level == LEVEL_INFO) {
            return;
        }
        nn::err::CTR::ThrowFatalErr(result, static_cast<nnerrFatalErrType>(0), caller);
        return;
    }
    if (level == LEVEL_FATAL) {
        nn::err::CTR::ThrowFatalErr(result, static_cast<nnerrFatalErrType>(0), caller);
    }
    nndbgPanic();
}

} // namespace detail
} // namespace CTR
} // namespace os
} // namespace nn
