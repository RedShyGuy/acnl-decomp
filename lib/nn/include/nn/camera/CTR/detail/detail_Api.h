#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/camera/CTR/camera_Types.h"
#include "nn/y2r/CTR/y2r_Types.h"

namespace nn {
namespace camera {
namespace CTR {
// the end of the camera library (the counterpart of the inline Initialize; name is ours)
void Finalize(); // 0x0012DE08

namespace detail {
// the cam:u session (name is ours)
extern nn::Handle s_Session; // 0x00975BB0

// the session of the service; with isCheckOnly it is closed again (names are ours)
nn::Result InitializeBase(nn::Handle* session, const char* name, bool isCheckOnly); // 0x00481998
DECOMP_NOINLINE void FinalizeBase(nn::Handle* session); // 0x00131CE8

// the application gets the cameras back / gives them up (the HOME menu; applet)
void ArriveApplication(); // 0x0012BD6C | fefates:bytes [tier B]
void LeaveApplication(); // 0x00131D38 | fefates:bytes [tier B]

// The commands of Camera; the ones that return nothing stop the program on an error (nndbgPanic).
// The ones with a result try twice; errors of the I2C module are tried again. (Names after the
// commands where the reference symbols have none or the wrong one.)
nn::Result Activate(nn::camera::CTR::CameraSelect select); // 0x0013743C
void StartCapture(nn::camera::CTR::Port port); // 0x0048197C
void StopCapture(nn::camera::CTR::Port port); // 0x001373B4
bool IsBusy(nn::camera::CTR::Port port); // 0x00482774 | nintendogs:bytes [tier B]
void ClearBuffer(nn::camera::CTR::Port port); // 0x00137398
// a new event, the old one in *event is closed
void GetVsyncInterruptEvent(nn::Handle* event, nn::camera::CTR::Port port); // 0x00481DF8
void GetBufferErrorInterruptEvent(nn::Handle* event, nn::camera::CTR::Port port); // 0x00481EFC
// the buffer must be device memory or WRAM
void SetReceiving(nn::Handle* event, void* buffer, nn::camera::CTR::Port port, size_t imageSize, s16 transferUnit); // 0x00481818
void SetTransferLines(nn::camera::CTR::Port port, s16 lines, s16 width, s16 height); // 0x00481CB8
s16 GetMaxLines(s16 width, s16 height); // 0x004816D0 | nintendogs:bytes [tier B]
void SetTransferBytes(nn::camera::CTR::Port port, u32 bytes, s16 width, s16 height); // 0x00481C9C
u32 GetTransferBytes(nn::camera::CTR::Port port); // 0x00481C24
u32 GetMaxBytes(s16 width, s16 height); // 0x004816AC
void SetTrimming(nn::camera::CTR::Port port, bool isTrimming); // 0x004816F4
// (symbols.json: SetTrimmingParams)
void SetTrimmingParamsCenter(nn::camera::CTR::Port port, s16 trimWidth, s16 trimHeight, s16 cameraWidth, s16 cameraHeight); // 0x00481E38
nn::Result SetSharpness(nn::camera::CTR::CameraSelect select, s8 sharpness); // 0x004818F8 | nintendogs:bytes [tier C]
nn::Result SetAutoExposure(nn::camera::CTR::CameraSelect select, bool isAutoExposure); // 0x00481BA0 | nintendogs:bytes [tier C]
nn::Result SetAutoWhiteBalance(nn::camera::CTR::CameraSelect select, bool isAutoWhiteBalance); // 0x00481CD4
// (symbols.json: FlipImage)
nn::Result SetSize(nn::camera::CTR::CameraSelect select, nn::camera::CTR::Size size, nn::camera::CTR::Context context); // 0x00482794
nn::Result SetFrameRate(nn::camera::CTR::CameraSelect select, nn::camera::CTR::FrameRate frameRate); // 0x00481710 | nintendogs:bytes [tier C]
nn::Result SetPhotoMode(nn::camera::CTR::CameraSelect select, nn::camera::CTR::PhotoMode photoMode); // 0x00481794 | nintendogs:bytes [tier C]
nn::Result SetAutoExposureWindow(nn::camera::CTR::CameraSelect select, s16 startX, s16 startY, s16 width, s16 height); // 0x00481D58
nn::Result SetAutoWhiteBalanceWindow(nn::camera::CTR::CameraSelect select, s16 startX, s16 startY, s16 width, s16 height); // 0x00481E5C
nn::Result SetNoiseFilter(nn::camera::CTR::CameraSelect select, bool isNoiseFilter); // 0x00481B1C
void GetStereoCameraCalibrationData(nn::camera::CTR::StereoCameraCalibrationData* data); // 0x00481F3C
// (symbols.json: SetPackageParameterWithContext)
nn::Result GetSuitableY2rStandardCoefficient(nn::y2r::CTR::StandardCoefficient* coefficient); // 0x00481F58
// (tries twice, without the I2C rule)
nn::Result PlayShutterSound(nn::camera::CTR::ShutterSoundType type); // 0x00481C44 | nintendogs:bytes [tier B]
} // namespace detail
} // namespace CTR
} // namespace camera
} // namespace nn
