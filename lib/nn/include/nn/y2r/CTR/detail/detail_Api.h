#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/y2r/CTR/y2r_Types.h"

namespace nn {
namespace os {
class EventBase;
} // namespace os

namespace y2r {
namespace CTR {
namespace detail {
// The library functions: the IPC commands, any failure stops the program (nndbgPanic).
// Names without a tier come from 3dbrew's command names (ours).
void StopConversion(); // 0x00136D64 | nintendogs:callseq [tier A]
bool IsBusyConversion(); // 0x00136D80 | nintendogs:callseq [tier A]
void SetRotation(nn::y2r::CTR::Rotation rotation); // 0x00469F64 | nintendogs:callseq [tier A]
// buffer: in device memory or WRAM
void SetReceiving(uptr buffer, size_t size, s16 transferUnit, s16 transferGap); // 0x00469F80
void SetInputLines(s16 lines); // 0x0046A038 | nintendogs:callseq [tier A]
void SetSendingYuv(uptr buffer, size_t size, s16 transferUnit, s16 transferGap); // 0x0046A054
// connects to the service (name), once; false if it does not exist or the driver failed
bool InitializeBase(nn::Handle* session, const char* name); // 0x0046A10C | nintendogs:bytes [tier A]
void SetInputFormat(nn::y2r::CTR::InputFormat format); // 0x0046A1B4
void SetOutputFormat(nn::y2r::CTR::OutputFormat format); // 0x0046A1D0 | nintendogs:callseq [tier A]
// RESULT_CONVERSION_BUSY is returned, any other failure is fatal
nn::Result StartConversion(); // 0x0046A1EC | nintendogs:bytes [tier A]
void SetBlockAlignment(nn::y2r::CTR::BlockAlignment alignment); // 0x0046A214
void SetInputLineWidth(s16 width); // 0x0046A230
// bytes of 8 output lines (at most 0x6000)
size_t GetOutputBlockSize(s16 lineWidth, nn::y2r::CTR::OutputFormat format); // 0x0046A24C
size_t GetOutputImageSize(s16 width, s16 height, nn::y2r::CTR::OutputFormat format); // 0x0046A298
void GetTransferEndEvent(nn::os::EventBase* event); // 0x0046A2DC
void SetPackageParameter(const nn::y2r::CTR::PackageParameter& parameter); // 0x0046A31C
size_t GetOutputFormatBytes(nn::y2r::CTR::OutputFormat format); // 0x0046A338 | nintendogs:bytes [tier B]
void SetStandardCoefficient(nn::y2r::CTR::StandardCoefficient coefficient); // 0x0046A370
void SetTransferEndInterrupt(bool enable); // 0x0046A38C

// names are ours
extern nn::Handle s_Session;    // 0x00975FD4, "y2r:u"
extern bool s_IsInitialized;    // 0x00975FD1

// StartConversion: a conversion is still running (level status, module 20, description 1)
const bit32 RESULT_CONVERSION_BUSY = 0xC9405001;
} // namespace detail
} // namespace CTR
} // namespace y2r
} // namespace nn
