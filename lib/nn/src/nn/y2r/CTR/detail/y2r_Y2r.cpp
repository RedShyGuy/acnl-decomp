#include "nn/y2r/CTR/detail/y2r_Y2r.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"
#include "nn/y2r/CTR/detail/detail_Api.h"

namespace nn {
namespace y2r {
namespace CTR {
namespace detail {
namespace {

// command headers (id << 16 | parameter counts), 3dbrew "Y2R Services"
const bit32 COMMAND_START_CONVERSION = 0x00260000;
const bit32 COMMAND_STOP_CONVERSION = 0x00270000;
const bit32 COMMAND_IS_BUSY_CONVERSION = 0x00280000;
const bit32 COMMAND_DRIVER_INITIALIZE = 0x002B0000;
const bit32 COMMAND_DRIVER_FINALIZE = 0x002C0000;

// a command without parameters: the result of the request, else the service's result
nn::Result Call(bit32 header)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsSuccess()) {
        result = nn::Result(command[1]);
    }
    return result;
}

// command headers of the setters
const bit32 COMMAND_SET_INPUT_FORMAT = 0x00010040;
const bit32 COMMAND_SET_OUTPUT_FORMAT = 0x00030040;
const bit32 COMMAND_SET_ROTATION = 0x00050040;
const bit32 COMMAND_SET_BLOCK_ALIGNMENT = 0x00070040;
const bit32 COMMAND_SET_TRANSFER_END_INTERRUPT = 0x000D0040;
const bit32 COMMAND_GET_TRANSFER_END_EVENT = 0x000F0000;
const bit32 COMMAND_SET_SENDING_YUV = 0x00130102;
const bit32 COMMAND_SET_RECEIVING = 0x00180102;
const bit32 COMMAND_SET_INPUT_LINE_WIDTH = 0x001A0040;
const bit32 COMMAND_SET_INPUT_LINES = 0x001C0040;
const bit32 COMMAND_SET_STANDARD_COEFFICIENT = 0x00200040;
const bit32 COMMAND_SET_PACKAGE_PARAMETER = 0x002901C0;

// translate descriptor: copy one handle
const bit32 IPC_COPY_HANDLE = 0;

nn::Result Send(bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(nn::y2r::CTR::detail::s_Session);
    if (result.IsSuccess()) {
        result = nn::Result(command[1]);
    }
    return result;
}

// commands with one byte / one halfword parameter
nn::Result CallU8(bit32 header, u8 value)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    *reinterpret_cast<u8*>(&command[1]) = value;
    return Send(command);
}

nn::Result CallS16(bit32 header, s16 value)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    *reinterpret_cast<s16*>(&command[1]) = value;
    return Send(command);
}

nn::Result CallBuffer(bit32 header, nn::Handle process, uptr buffer, size_t size, s16 transferUnit, s16 transferGap)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    command[1] = buffer;
    command[2] = size;
    *reinterpret_cast<s16*>(&command[3]) = transferUnit;
    *reinterpret_cast<s16*>(&command[4]) = transferGap;
    command[6] = process.GetPrintableBits();
    command[5] = IPC_COPY_HANDLE;
    return Send(command);
}

} // namespace

// 0x0013B524 | nintendogs:bytes [tier A]
nn::Result nn::y2r::CTR::detail::Y2r::DriverFinalize()
{
    return Call(COMMAND_DRIVER_FINALIZE);
}

// 0x0013B554 | nintendogs:bytes [tier A]
nn::Result nn::y2r::CTR::detail::Y2r::StopConversion()
{
    return Call(COMMAND_STOP_CONVERSION);
}

// 0x0013B584 | nintendogs:bytes [tier A]
nn::Result nn::y2r::CTR::detail::Y2r::IsBusyConversion(bool* isBusy)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IS_BUSY_CONVERSION;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *isBusy = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0046A550 | nintendogs:bytes [tier A]
nn::Result nn::y2r::CTR::detail::Y2r::StartConversion()
{
    return Call(COMMAND_START_CONVERSION);
}

// 0x0046A580 | nintendogs:callgraph [tier A]
nn::Result nn::y2r::CTR::detail::Y2r::DriverInitialize()
{
    return Call(COMMAND_DRIVER_INITIALIZE);
}

// 0x0046A4D8 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetInputFormat(nn::y2r::CTR::InputFormat format)
{
    return CallU8(COMMAND_SET_INPUT_FORMAT, format);
}

// 0x0046A514 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetOutputFormat(nn::y2r::CTR::OutputFormat format)
{
    return CallU8(COMMAND_SET_OUTPUT_FORMAT, format);
}

// 0x0046A3A8 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetRotation(nn::y2r::CTR::Rotation rotation)
{
    return CallU8(COMMAND_SET_ROTATION, rotation);
}

// 0x0046A5B0 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetBlockAlignment(nn::y2r::CTR::BlockAlignment alignment)
{
    return CallU8(COMMAND_SET_BLOCK_ALIGNMENT, alignment);
}

// 0x0046A6E8 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetTransferEndInterrupt(bool enable)
{
    return CallU8(COMMAND_SET_TRANSFER_END_INTERRUPT, enable);
}

// 0x0046A628 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::GetTransferEndEvent(nn::Handle* event)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TRANSFER_END_EVENT;
    nn::Result result = nn::svc::SendSyncRequest(nn::y2r::CTR::detail::s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *event = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x0046A47C (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetSendingYuv(nn::Handle process, uptr buffer, size_t size, s16 transferUnit, s16 transferGap)
{
    return CallBuffer(COMMAND_SET_SENDING_YUV, process, buffer, size, transferUnit, transferGap);
}

// 0x0046A3E4 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetReceiving(nn::Handle process, uptr buffer, size_t size, s16 transferUnit, s16 transferGap)
{
    return CallBuffer(COMMAND_SET_RECEIVING, process, buffer, size, transferUnit, transferGap);
}

// 0x0046A5EC (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetInputLineWidth(s16 width)
{
    return CallS16(COMMAND_SET_INPUT_LINE_WIDTH, width);
}

// 0x0046A440 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetInputLines(s16 lines)
{
    return CallS16(COMMAND_SET_INPUT_LINES, lines);
}

// 0x0046A6AC (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetStandardCoefficient(nn::y2r::CTR::StandardCoefficient coefficient)
{
    return CallU8(COMMAND_SET_STANDARD_COEFFICIENT, coefficient);
}

// 0x0046A664 (name after 3dbrew "Y2R Services")
nn::Result nn::y2r::CTR::detail::Y2r::SetPackageParameter(const nn::y2r::CTR::PackageParameter& parameter)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_PACKAGE_PARAMETER;
    *reinterpret_cast<nn::y2r::CTR::PackageParameter*>(&command[1]) = parameter;
    return Send(command);
}

} // namespace detail
} // namespace CTR
} // namespace y2r
} // namespace nn
