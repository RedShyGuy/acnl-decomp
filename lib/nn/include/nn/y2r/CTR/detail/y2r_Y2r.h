#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/y2r/CTR/y2r_Types.h"

namespace nn {
namespace y2r {
namespace CTR {
namespace detail {

// IPC commands of "y2r:u" (session detail::s_Session). The class name and the first five
// method names are from the reference symbols; the setters are named after 3dbrew
// ("Y2R Services"). 0x0046A628 is GetTransferEndEvent (symbols.json calls it
// nn::cec::CTR::detail::Cec::GetCecInfoEventHandle, but it sends command 0xF on the y2r session).
class Y2r
{
public:
    static nn::Result DriverFinalize(); // 0x0013B524 | nintendogs:bytes [tier A]
    static nn::Result StopConversion(); // 0x0013B554 | nintendogs:bytes [tier A]
    static nn::Result IsBusyConversion(bool* isBusy); // 0x0013B584 | nintendogs:bytes [tier A]
    static nn::Result StartConversion(); // 0x0046A550 | nintendogs:bytes [tier A]
    static nn::Result DriverInitialize(); // 0x0046A580 | nintendogs:callgraph [tier A]

    static nn::Result SetInputFormat(nn::y2r::CTR::InputFormat format); // 0x0046A4D8 (name after 3dbrew "Y2R Services")
    static nn::Result SetOutputFormat(nn::y2r::CTR::OutputFormat format); // 0x0046A514 (name after 3dbrew "Y2R Services")
    static nn::Result SetRotation(nn::y2r::CTR::Rotation rotation); // 0x0046A3A8 (name after 3dbrew "Y2R Services")
    static nn::Result SetBlockAlignment(nn::y2r::CTR::BlockAlignment alignment); // 0x0046A5B0 (name after 3dbrew "Y2R Services")
    static nn::Result SetTransferEndInterrupt(bool enable); // 0x0046A6E8 (name after 3dbrew "Y2R Services")
    static nn::Result GetTransferEndEvent(nn::Handle* event); // 0x0046A628 (name after 3dbrew "Y2R Services")
    // DMA source / destination: buffer of process (transferUnit bytes per transfer, gap between)
    static nn::Result SetSendingYuv(nn::Handle process, uptr buffer, size_t size, s16 transferUnit, s16 transferGap); // 0x0046A47C (name after 3dbrew "Y2R Services")
    static nn::Result SetReceiving(nn::Handle process, uptr buffer, size_t size, s16 transferUnit, s16 transferGap); // 0x0046A3E4 (name after 3dbrew "Y2R Services")
    static nn::Result SetInputLineWidth(s16 width); // 0x0046A5EC (name after 3dbrew "Y2R Services")
    static nn::Result SetInputLines(s16 lines); // 0x0046A440 (name after 3dbrew "Y2R Services")
    static nn::Result SetStandardCoefficient(nn::y2r::CTR::StandardCoefficient coefficient); // 0x0046A6AC (name after 3dbrew "Y2R Services")
    static nn::Result SetPackageParameter(const nn::y2r::CTR::PackageParameter& parameter); // 0x0046A664 (name after 3dbrew "Y2R Services")
};

} // namespace detail
} // namespace CTR
} // namespace y2r
} // namespace nn
