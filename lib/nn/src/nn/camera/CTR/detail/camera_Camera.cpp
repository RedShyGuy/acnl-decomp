#include "nn/camera/CTR/detail/camera_Camera.h"
#include <string.h>
#include "nn/camera/CTR/detail/detail_Api.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace camera {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "Camera Services")
const bit32 COMMAND_START_CAPTURE = 0x00010040;
const bit32 COMMAND_STOP_CAPTURE = 0x00020040;
const bit32 COMMAND_IS_BUSY = 0x00030040;
const bit32 COMMAND_CLEAR_BUFFER = 0x00040040;
const bit32 COMMAND_GET_VSYNC_INTERRUPT_EVENT = 0x00050040;
const bit32 COMMAND_GET_BUFFER_ERROR_INTERRUPT_EVENT = 0x00060040;
const bit32 COMMAND_SET_RECEIVING = 0x00070102;
const bit32 COMMAND_SET_TRANSFER_LINES = 0x00090100;
const bit32 COMMAND_GET_MAX_LINES = 0x000A0080;
const bit32 COMMAND_SET_TRANSFER_BYTES = 0x000B0100;
const bit32 COMMAND_GET_TRANSFER_BYTES = 0x000C0040;
const bit32 COMMAND_GET_MAX_BYTES = 0x000D0080;
const bit32 COMMAND_SET_TRIMMING = 0x000E0080;
const bit32 COMMAND_SET_TRIMMING_PARAMS_CENTER = 0x00120140;
const bit32 COMMAND_ACTIVATE = 0x00130040;
const bit32 COMMAND_SET_SHARPNESS = 0x00180080;
const bit32 COMMAND_SET_AUTO_EXPOSURE = 0x00190080;
const bit32 COMMAND_SET_AUTO_WHITE_BALANCE = 0x001B0080;
const bit32 COMMAND_SET_SIZE = 0x001F00C0;
const bit32 COMMAND_SET_FRAME_RATE = 0x00200080;
const bit32 COMMAND_SET_PHOTO_MODE = 0x00210080;
const bit32 COMMAND_SET_AUTO_EXPOSURE_WINDOW = 0x00260140;
const bit32 COMMAND_SET_AUTO_WHITE_BALANCE_WINDOW = 0x00270140;
const bit32 COMMAND_SET_NOISE_FILTER = 0x00280080;
const bit32 COMMAND_GET_STEREO_CAMERA_CALIBRATION_DATA = 0x002B0000;
const bit32 COMMAND_GET_SUITABLE_Y2R_STANDARD_COEFFICIENT = 0x00360000;
const bit32 COMMAND_PLAY_SHUTTER_SOUND = 0x00380040;
const bit32 COMMAND_DRIVER_INITIALIZE = 0x00390000;
const bit32 COMMAND_DRIVER_FINALIZE = 0x003A0000;
const bit32 COMMAND_GET_ACTIVATED_CAMERA = 0x003B0000;
const bit32 COMMAND_GET_SLEEP_CAMERA = 0x003C0000;
const bit32 COMMAND_SET_SLEEP_CAMERA = 0x003D0040;

// the arguments go into the low byte / half word of their word
inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline void SetHalf(bit32* word, s16 value)
{
    *reinterpret_cast<s16*>(word) = value;
}

inline nn::Result Send(bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}
} // namespace

// 0x00482204
nn::Result nn::camera::CTR::detail::Camera::StartCapture(nn::camera::CTR::Port port)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_START_CAPTURE;
    SetByte(&command[1], port);
    return Send(command);
}

// 0x0013B730
nn::Result nn::camera::CTR::detail::Camera::StopCapture(nn::camera::CTR::Port port)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_STOP_CAPTURE;
    SetByte(&command[1], port);
    return Send(command);
}

// 0x004826D8
nn::Result nn::camera::CTR::detail::Camera::IsBusy(bool* isBusy, nn::camera::CTR::Port port)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IS_BUSY;
    SetByte(&command[1], port);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *isBusy = *reinterpret_cast<const bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0013B6F4
nn::Result nn::camera::CTR::detail::Camera::ClearBuffer(nn::camera::CTR::Port port)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CLEAR_BUFFER;
    SetByte(&command[1], port);
    return Send(command);
}

// 0x004824DC
nn::Result nn::camera::CTR::detail::Camera::GetVsyncInterruptEvent(nn::Handle* event, nn::camera::CTR::Port port)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_VSYNC_INTERRUPT_EVENT;
    SetByte(&command[1], port);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    // (command[2] is the descriptor of the handle)
    *event = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x004825E8
nn::Result nn::camera::CTR::detail::Camera::GetBufferErrorInterruptEvent(nn::Handle* event, nn::camera::CTR::Port port)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_BUFFER_ERROR_INTERRUPT_EVENT;
    SetByte(&command[1], port);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *event = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x00482154
nn::Result nn::camera::CTR::detail::Camera::SetReceiving(nn::Handle* event, nn::Handle process, void* buffer, nn::camera::CTR::Port port,
                                                        size_t imageSize, s16 transferUnit)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_RECEIVING;
    command[1] = reinterpret_cast<uptr>(buffer);
    SetByte(&command[2], port);
    command[3] = imageSize;
    SetHalf(&command[4], transferUnit);
    // the handle of the process to copy into
    command[5] = 0;
    command[6] = process.GetPrintableBits();
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *event = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x004823DC
nn::Result nn::camera::CTR::detail::Camera::SetTransferLines(nn::camera::CTR::Port port, s16 lines, s16 width, s16 height)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_TRANSFER_LINES;
    SetByte(&command[1], port);
    SetHalf(&command[2], lines);
    SetHalf(&command[3], width);
    SetHalf(&command[4], height);
    return Send(command);
}

// 0x00482028
nn::Result nn::camera::CTR::detail::Camera::GetMaxLines(s16* lines, s16 width, s16 height)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_MAX_LINES;
    SetHalf(&command[1], width);
    SetHalf(&command[2], height);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *lines = *reinterpret_cast<const s16*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00482388
nn::Result nn::camera::CTR::detail::Camera::SetTransferBytes(nn::camera::CTR::Port port, u32 bytes, s16 width, s16 height)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_TRANSFER_BYTES;
    SetByte(&command[1], port);
    command[2] = bytes;
    SetHalf(&command[3], width);
    SetHalf(&command[4], height);
    return Send(command);
}

// 0x00482300
nn::Result nn::camera::CTR::detail::Camera::GetTransferBytes(u32* bytes, nn::camera::CTR::Port port)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TRANSFER_BYTES;
    SetByte(&command[1], port);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *bytes = command[2];
    return nn::Result(command[1]);
}

// 0x00481FD4
nn::Result nn::camera::CTR::detail::Camera::GetMaxBytes(u32* bytes, s16 width, s16 height)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_MAX_BYTES;
    SetHalf(&command[1], width);
    SetHalf(&command[2], height);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *bytes = command[2];
    return nn::Result(command[1]);
}

// 0x0048207C
nn::Result nn::camera::CTR::detail::Camera::SetTrimming(nn::camera::CTR::Port port, bool isTrimming)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_TRIMMING;
    SetByte(&command[1], port);
    SetByte(&command[2], isTrimming);
    return Send(command);
}

// 0x00482528
nn::Result nn::camera::CTR::detail::Camera::SetTrimmingParamsCenter(nn::camera::CTR::Port port, s16 trimWidth, s16 trimHeight, s16 cameraWidth,
                                                                   s16 cameraHeight)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_TRIMMING_PARAMS_CENTER;
    SetByte(&command[1], port);
    SetHalf(&command[2], trimWidth);
    SetHalf(&command[3], trimHeight);
    SetHalf(&command[4], cameraWidth);
    SetHalf(&command[5], cameraHeight);
    return Send(command);
}

// 0x00137400
nn::Result nn::camera::CTR::detail::Camera::Activate(nn::camera::CTR::CameraSelect select)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ACTIVATE;
    SetByte(&command[1], select);
    return Send(command);
}

// 0x004821BC
nn::Result nn::camera::CTR::detail::Camera::SetSharpness(nn::camera::CTR::CameraSelect select, s8 sharpness)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SHARPNESS;
    SetByte(&command[1], select);
    SetByte(&command[2], sharpness);
    return Send(command);
}

// 0x00482288
nn::Result nn::camera::CTR::detail::Camera::SetAutoExposure(nn::camera::CTR::CameraSelect select, bool isAutoExposure)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_AUTO_EXPOSURE;
    SetByte(&command[1], select);
    SetByte(&command[2], isAutoExposure);
    return Send(command);
}

// 0x00482434
nn::Result nn::camera::CTR::detail::Camera::SetAutoWhiteBalance(nn::camera::CTR::CameraSelect select, bool isAutoWhiteBalance)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_AUTO_WHITE_BALANCE;
    SetByte(&command[1], select);
    SetByte(&command[2], isAutoWhiteBalance);
    return Send(command);
}

// 0x00482724
nn::Result nn::camera::CTR::detail::Camera::SetSize(nn::camera::CTR::CameraSelect select, nn::camera::CTR::Size size, nn::camera::CTR::Context context)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SIZE;
    SetByte(&command[1], select);
    SetByte(&command[2], size);
    SetByte(&command[3], context);
    return Send(command);
}

// 0x004820C4
nn::Result nn::camera::CTR::detail::Camera::SetFrameRate(nn::camera::CTR::CameraSelect select, nn::camera::CTR::FrameRate frameRate)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_FRAME_RATE;
    SetByte(&command[1], select);
    SetByte(&command[2], frameRate);
    return Send(command);
}

// 0x0048210C
nn::Result nn::camera::CTR::detail::Camera::SetPhotoMode(nn::camera::CTR::CameraSelect select, nn::camera::CTR::PhotoMode photoMode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_PHOTO_MODE;
    SetByte(&command[1], select);
    SetByte(&command[2], photoMode);
    return Send(command);
}

// 0x0048247C
nn::Result nn::camera::CTR::detail::Camera::SetAutoExposureWindow(nn::camera::CTR::CameraSelect select, s16 startX, s16 startY, s16 width,
                                                                 s16 height)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_AUTO_EXPOSURE_WINDOW;
    SetByte(&command[1], select);
    SetHalf(&command[2], startX);
    SetHalf(&command[3], startY);
    SetHalf(&command[4], width);
    SetHalf(&command[5], height);
    return Send(command);
}

// 0x00482588
nn::Result nn::camera::CTR::detail::Camera::SetAutoWhiteBalanceWindow(nn::camera::CTR::CameraSelect select, s16 startX, s16 startY,
                                                                     s16 width, s16 height)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_AUTO_WHITE_BALANCE_WINDOW;
    SetByte(&command[1], select);
    SetHalf(&command[2], startX);
    SetHalf(&command[3], startY);
    SetHalf(&command[4], width);
    SetHalf(&command[5], height);
    return Send(command);
}

// 0x00482240
nn::Result nn::camera::CTR::detail::Camera::SetNoiseFilter(nn::camera::CTR::CameraSelect select, bool isNoiseFilter)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_NOISE_FILTER;
    SetByte(&command[1], select);
    SetByte(&command[2], isNoiseFilter);
    return Send(command);
}

// 0x00482634 | nintendogs:bytes [tier B]
nn::Result nn::camera::CTR::detail::Camera::GetStereoCameraCalibrationData(nn::camera::CTR::StereoCameraCalibrationData* data)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_STEREO_CAMERA_CALIBRATION_DATA;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *data = *reinterpret_cast<const StereoCameraCalibrationData*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0048269C | nintendogs:bytes [tier B]
nn::Result nn::camera::CTR::detail::Camera::GetSuitableY2rStandardCoefficient(nn::y2r::CTR::StandardCoefficient* coefficient)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_SUITABLE_Y2R_STANDARD_COEFFICIENT;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *coefficient = *reinterpret_cast<const nn::y2r::CTR::StandardCoefficient*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0048234C
nn::Result nn::camera::CTR::detail::Camera::PlayShutterSound(nn::camera::CTR::ShutterSoundType type)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_PLAY_SHUTTER_SOUND;
    SetByte(&command[1], type);
    return Send(command);
}

// 0x004822D0 | nintendogs:bytes [tier B]
nn::Result nn::camera::CTR::detail::Camera::DriverInitialize()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DRIVER_INITIALIZE;
    return Send(command);
}

// 0x001373D0 | nintendogs:bytes [tier B]
nn::Result nn::camera::CTR::detail::Camera::DriverFinalize()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DRIVER_FINALIZE;
    return Send(command);
}

// 0x00131E20 | nintendogs:bytes [tier A]
nn::Result nn::camera::CTR::detail::Camera::GetActivatedCamera(nn::camera::CTR::CameraSelect* select)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_ACTIVATED_CAMERA;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *select = *reinterpret_cast<const CameraSelect*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00131DA8 | nintendogs:bytes [tier A]
nn::Result nn::camera::CTR::detail::Camera::GetSleepCamera(nn::camera::CTR::CameraSelect* select)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_SLEEP_CAMERA;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *select = *reinterpret_cast<const CameraSelect*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00131DE4
nn::Result nn::camera::CTR::detail::Camera::SetSleepCamera(nn::camera::CTR::CameraSelect select)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SLEEP_CAMERA;
    SetByte(&command[1], select);
    return Send(command);
}

} // namespace detail
} // namespace CTR
} // namespace camera
} // namespace nn
