#include "nn/y2r/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/os/CTR/CTR_Api.h"
#include "nn/os/os_Api.h"
#include "nn/os/os_EventBase.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"
#include "nn/y2r/CTR/detail/y2r_Y2r.h"

#include <string.h>

namespace nn {
namespace y2r {
namespace CTR {
namespace detail {
namespace {

// srv: the service does not exist (or may not be used)
const bit32 RESULT_SERVICE_NOT_FOUND = 0xD0401834;
const size_t MAX_OUTPUT_BLOCK_SIZE = 0x6000;
const s32 OUTPUT_BLOCK_LINES = 8;

void CheckResult(nn::Result result)
{
    if (result.IsFailure()) {
        nndbgPanic();
    }
}

// DMA works from device memory and WRAM only (inlined in the original, also with GCC)
inline __attribute__((always_inline)) void CheckBuffer(uptr buffer)
{
    if (nn::os::GetDeviceMemoryAddress() <= buffer &&
        buffer < nn::os::GetDeviceMemoryAddress() + nn::os::GetDeviceMemorySize()) {
        return;
    }
    if (nn::os::CTR::IsWramEnabled() && nn::os::CTR::GetWramAddress() <= buffer &&
        buffer < nn::os::CTR::GetWramAddress() + nn::os::CTR::GetWramSize()) {
        return;
    }
    nndbgPanic();
}

} // namespace

// 0x00975FD4
nn::Handle s_Session;
// 0x00975FD1
bool s_IsInitialized;

// 0x00136D64 | nintendogs:callseq [tier A]
void StopConversion()
{
    CheckResult(Y2r::StopConversion());
}

// 0x00136D80 | nintendogs:callseq [tier A]
bool IsBusyConversion()
{
    bool isBusy;
    CheckResult(Y2r::IsBusyConversion(&isBusy));
    return isBusy;
}

// 0x00469F64 | nintendogs:callseq [tier A]
void SetRotation(nn::y2r::CTR::Rotation rotation)
{
    CheckResult(Y2r::SetRotation(rotation));
}

// 0x00469F80
void SetReceiving(uptr buffer, size_t size, s16 transferUnit, s16 transferGap)
{
    CheckBuffer(buffer);
    CheckResult(Y2r::SetReceiving(nn::Handle(nn::PSEUDO_HANDLE_CURRENT_PROCESS), buffer, size,
                                                  transferUnit, transferGap));
}

// 0x0046A038 | nintendogs:callseq [tier A]
void SetInputLines(s16 lines)
{
    CheckResult(Y2r::SetInputLines(lines));
}

// 0x0046A054
void SetSendingYuv(uptr buffer, size_t size, s16 transferUnit, s16 transferGap)
{
    CheckBuffer(buffer);
    CheckResult(Y2r::SetSendingYuv(nn::Handle(nn::PSEUDO_HANDLE_CURRENT_PROCESS), buffer, size,
                                                   transferUnit, transferGap));
}

// 0x0046A10C | nintendogs:bytes [tier A]
bool InitializeBase(nn::Handle* session, const char* name)
{
    if (s_IsInitialized) {
        return true;
    }
    CheckResult(nn::srv::Initialize());
    nn::Result result = nn::srv::GetServiceHandle(session, name, strlen(name), 1);
    if (result.IsFailure()) {
        if (result == nn::Result(RESULT_SERVICE_NOT_FOUND)) {
            return false;
        }
        nndbgPanic();
    }
    if (Y2r::DriverInitialize().IsFailure()) {
        CheckResult(nn::svc::CloseHandle(*session));
        return false;
    }
    s_IsInitialized = true;
    return true;
}

// 0x0046A1B4
void SetInputFormat(nn::y2r::CTR::InputFormat format)
{
    CheckResult(Y2r::SetInputFormat(format));
}

// 0x0046A1D0 | nintendogs:callseq [tier A]
void SetOutputFormat(nn::y2r::CTR::OutputFormat format)
{
    CheckResult(Y2r::SetOutputFormat(format));
}

// 0x0046A1EC | nintendogs:bytes [tier A]
nn::Result StartConversion()
{
    nn::Result result = Y2r::StartConversion();
    if (result == nn::Result(RESULT_CONVERSION_BUSY)) {
        return result;
    }
    CheckResult(result);
    return nn::Result();
}

// 0x0046A214
void SetBlockAlignment(nn::y2r::CTR::BlockAlignment alignment)
{
    CheckResult(Y2r::SetBlockAlignment(alignment));
}

// 0x0046A230
void SetInputLineWidth(s16 width)
{
    CheckResult(Y2r::SetInputLineWidth(width));
}

// 0x0046A24C
size_t GetOutputBlockSize(s16 lineWidth, nn::y2r::CTR::OutputFormat format)
{
    size_t size = lineWidth * GetOutputFormatBytes(format) * OUTPUT_BLOCK_LINES;
    if (size > MAX_OUTPUT_BLOCK_SIZE) {
        nndbgPanic();
    }
    return size;
}

// 0x0046A298
size_t GetOutputImageSize(s16 width, s16 height, nn::y2r::CTR::OutputFormat format)
{
    return width * height * GetOutputFormatBytes(format);
}

// 0x0046A2DC
void GetTransferEndEvent(nn::os::EventBase* event)
{
    nn::Handle handle;
    CheckResult(Y2r::GetTransferEndEvent(&handle));
    event->AttachHandle(handle);
}

// 0x0046A31C
void SetPackageParameter(const nn::y2r::CTR::PackageParameter& parameter)
{
    CheckResult(Y2r::SetPackageParameter(parameter));
}

// 0x0046A338 | nintendogs:bytes [tier B]
size_t GetOutputFormatBytes(nn::y2r::CTR::OutputFormat format)
{
    switch (format) {
    case nn::y2r::CTR::OUTPUT_RGB_32:
        return 4;
    case nn::y2r::CTR::OUTPUT_RGB_24:
        return 3;
    case nn::y2r::CTR::OUTPUT_RGB_16_555:
    case nn::y2r::CTR::OUTPUT_RGB_16_565:
        return 2;
    default:
        nndbgPanic();
        return 0;
    }
}

// 0x0046A370
void SetStandardCoefficient(nn::y2r::CTR::StandardCoefficient coefficient)
{
    CheckResult(Y2r::SetStandardCoefficient(coefficient));
}

// 0x0046A38C
void SetTransferEndInterrupt(bool enable)
{
    CheckResult(Y2r::SetTransferEndInterrupt(enable));
}

} // namespace detail
} // namespace CTR
} // namespace y2r
} // namespace nn
