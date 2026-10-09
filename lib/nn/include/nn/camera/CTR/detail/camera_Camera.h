#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/camera/CTR/camera_Types.h"
#include "nn/y2r/CTR/y2r_Types.h"

namespace nn {
namespace camera {
namespace CTR {
namespace detail {
// the commands of cam:u (the class name and most names are from the reference symbols, the others
// and all parameters after 3dbrew "Camera Services"); they use s_Session
class Camera
{
public:
    static nn::Result StartCapture(nn::camera::CTR::Port port); // 0x00482204
    static nn::Result StopCapture(nn::camera::CTR::Port port); // 0x0013B730
    static nn::Result IsBusy(bool* isBusy, nn::camera::CTR::Port port); // 0x004826D8
    static nn::Result ClearBuffer(nn::camera::CTR::Port port); // 0x0013B6F4
    static nn::Result GetVsyncInterruptEvent(nn::Handle* event, nn::camera::CTR::Port port); // 0x004824DC
    static nn::Result GetBufferErrorInterruptEvent(nn::Handle* event, nn::camera::CTR::Port port); // 0x004825E8
    // the image goes to the buffer of the process; the event signals the end
    static nn::Result SetReceiving(nn::Handle* event, nn::Handle process, void* buffer, nn::camera::CTR::Port port, size_t imageSize,
                                   s16 transferUnit); // 0x00482154
    static nn::Result SetTransferLines(nn::camera::CTR::Port port, s16 lines, s16 width, s16 height); // 0x004823DC
    static nn::Result GetMaxLines(s16* lines, s16 width, s16 height); // 0x00482028
    static nn::Result SetTransferBytes(nn::camera::CTR::Port port, u32 bytes, s16 width, s16 height); // 0x00482388
    static nn::Result GetTransferBytes(u32* bytes, nn::camera::CTR::Port port); // 0x00482300
    static nn::Result GetMaxBytes(u32* bytes, s16 width, s16 height); // 0x00481FD4
    static nn::Result SetTrimming(nn::camera::CTR::Port port, bool isTrimming); // 0x0048207C
    static nn::Result SetTrimmingParamsCenter(nn::camera::CTR::Port port, s16 trimWidth, s16 trimHeight, s16 cameraWidth,
                                              s16 cameraHeight); // 0x00482528
    static nn::Result Activate(nn::camera::CTR::CameraSelect select); // 0x00137400
    static nn::Result SetSharpness(nn::camera::CTR::CameraSelect select, s8 sharpness); // 0x004821BC
    static nn::Result SetAutoExposure(nn::camera::CTR::CameraSelect select, bool isAutoExposure); // 0x00482288
    static nn::Result SetAutoWhiteBalance(nn::camera::CTR::CameraSelect select, bool isAutoWhiteBalance); // 0x00482434
    static nn::Result SetSize(nn::camera::CTR::CameraSelect select, nn::camera::CTR::Size size, nn::camera::CTR::Context context); // 0x00482724
    static nn::Result SetFrameRate(nn::camera::CTR::CameraSelect select, nn::camera::CTR::FrameRate frameRate); // 0x004820C4
    static nn::Result SetPhotoMode(nn::camera::CTR::CameraSelect select, nn::camera::CTR::PhotoMode photoMode); // 0x0048210C
    static nn::Result SetAutoExposureWindow(nn::camera::CTR::CameraSelect select, s16 startX, s16 startY, s16 width, s16 height); // 0x0048247C
    static nn::Result SetAutoWhiteBalanceWindow(nn::camera::CTR::CameraSelect select, s16 startX, s16 startY, s16 width,
                                                s16 height); // 0x00482588
    static nn::Result SetNoiseFilter(nn::camera::CTR::CameraSelect select, bool isNoiseFilter); // 0x00482240
    static nn::Result GetStereoCameraCalibrationData(nn::camera::CTR::StereoCameraCalibrationData* data); // 0x00482634 | nintendogs:bytes [tier B]
    static nn::Result GetSuitableY2rStandardCoefficient(nn::y2r::CTR::StandardCoefficient* coefficient); // 0x0048269C | nintendogs:bytes [tier B]
    static nn::Result PlayShutterSound(nn::camera::CTR::ShutterSoundType type); // 0x0048234C
    static nn::Result DriverInitialize(); // 0x004822D0 | nintendogs:bytes [tier B]
    static nn::Result DriverFinalize(); // 0x001373D0 | nintendogs:bytes [tier B]
    static nn::Result GetActivatedCamera(nn::camera::CTR::CameraSelect* select); // 0x00131E20 | nintendogs:bytes [tier A]
    static nn::Result GetSleepCamera(nn::camera::CTR::CameraSelect* select); // 0x00131DA8 | nintendogs:bytes [tier A]
    static nn::Result SetSleepCamera(nn::camera::CTR::CameraSelect select); // 0x00131DE4
};
} // namespace detail
} // namespace CTR
} // namespace camera
} // namespace nn
