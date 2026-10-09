#include "nn/camera/CTR/detail/detail_Api.h"
#include <string.h>
#include "nn/camera/CTR/detail/camera_Camera.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/os/CTR/CTR_Api.h"
#include "nn/os/os_Api.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace camera {
namespace CTR {
namespace detail {
namespace {
// results (module cam, 20; the names are ours)
const bit32 RESULT_STATUS_CHANGED = 0xC9405001;      // status, status changed, 1: the camera is asleep
const bit32 RESULT_FATAL = 0xF9605002;               // fatal, internal, 2
const bit32 RESULT_FATAL_1022 = 0xF96053FE;          // fatal, internal, 1022
const bit32 RESULT_ALREADY_INITIALIZED = 0xD82053F9; // permanent, nothing happened, 1017
const bit32 RESULT_DRIVER_UNAVAILABLE = 0xD82053F8;  // permanent, nothing happened, 1016
const bit32 RESULT_SESSIONS_FULL = 0xC8A053F9;       // status, invalid state, 1017
// GetServiceHandle while all sessions of the service are in use (os)
const bit32 RESULT_PORT_SESSIONS_FULL = 0xD0401834;

// the errors of the camera's I2C bus are tried again
const bit32 MODULE_I2C = 11;
const s32 RETRY_COUNT = 2;

// (ARMCC loads it from .rodata at 0x0089E90C)
const nn::Handle CURRENT_PROCESS(PSEUDO_HANDLE_CURRENT_PROCESS);

// the retry of the commands with a result (a macro in the original, it seems)
template <typename Call>
inline nn::Result CallWithRetry(Call call)
{
    for (s32 i = 0; i < RETRY_COUNT; i++) {
        nn::Result result = call();
        if (result.IsSuccess()) {
            return nn::Result();
        }
        if (result == RESULT_STATUS_CHANGED) {
            return result;
        }
        if (result.GetModule() == MODULE_I2C) {
            continue;
        }
        if (result == RESULT_FATAL_1022) {
            break;
        }
        if (result.IsFailure()) {
            nndbgPanic();
        }
    }
    return RESULT_FATAL;
}

inline void PanicIfFailed(nn::Result result)
{
    if (result.IsFailure()) {
        nndbgPanic();
    }
}

inline void CloseSession(nn::Handle* session)
{
    PanicIfFailed(nn::svc::CloseHandle(*session));
}

// replaces the event in *event (the old one is closed)
inline void ReplaceEvent(nn::Handle* event, nn::Handle newEvent)
{
    if (event->IsValid()) {
        nn::svc::CloseHandle(*event);
        *event = nn::Handle();
    }
    *event = newEvent;
}

inline bool IsInRange(uptr address, uptr begin, size_t size)
{
    return begin <= address && address < begin + size;
}
} // namespace

// the globals (names are ours)
// 0x00975BB0
nn::Handle s_Session;
// the cameras that were active when the application left
// 0x0097E820
CameraSelect s_SleepCamera;
// 0x0097E821
bool s_IsInitialized;

// 0x00481998
nn::Result InitializeBase(nn::Handle* session, const char* name, bool isCheckOnly)
{
    if (s_IsInitialized) {
        return RESULT_ALREADY_INITIALIZED;
    }
    PanicIfFailed(nn::srv::Initialize());
    nn::Result result = nn::srv::GetServiceHandle(session, name, strlen(name), 1);
    if (result.IsFailure()) {
        if (result == RESULT_PORT_SESSIONS_FULL) {
            return RESULT_SESSIONS_FULL;
        }
        nndbgPanic();
    }
    if (isCheckOnly) {
        CloseSession(session);
        return nn::Result();
    }
    for (s32 i = 0; i < RETRY_COUNT; i++) {
        result = Camera::DriverInitialize();
        if (result.IsSuccess()) {
            s_IsInitialized = true;
            return nn::Result();
        }
        if (result == RESULT_STATUS_CHANGED) {
            CloseSession(session);
            return result;
        }
        if (result.GetModule() != MODULE_I2C || result == RESULT_DRIVER_UNAVAILABLE) {
            break;
        }
    }
    CloseSession(session);
    return RESULT_FATAL;
}

// 0x00131CE8
void FinalizeBase(nn::Handle* session)
{
    if (s_IsInitialized) {
        Camera::DriverFinalize();
        CloseSession(session);
        s_IsInitialized = false;
    }
}

// 0x0012BD6C | fefates:bytes [tier B]
void ArriveApplication()
{
    if (s_IsInitialized) {
        if (Camera::Activate(s_SleepCamera) == RESULT_STATUS_CHANGED) {
            Camera::SetSleepCamera(s_SleepCamera);
        }
    }
}

// 0x00131D38 | fefates:bytes [tier B]
void LeaveApplication()
{
    if (!s_IsInitialized) {
        return;
    }
    CameraSelect activated;
    if (Camera::GetActivatedCamera(&activated).IsFailure()) {
        return;
    }
    CameraSelect sleeping;
    if (Camera::GetSleepCamera(&sleeping).IsFailure()) {
        return;
    }
    s_SleepCamera = static_cast<CameraSelect>(activated | sleeping);
    if (Camera::Activate(SELECT_NONE) == RESULT_STATUS_CHANGED) {
        Camera::SetSleepCamera(SELECT_NONE);
    }
}

// 0x0013743C
nn::Result Activate(nn::camera::CTR::CameraSelect select)
{
    return CallWithRetry([&] { return Camera::Activate(select); });
}

// 0x0048197C
void StartCapture(nn::camera::CTR::Port port)
{
    PanicIfFailed(Camera::StartCapture(port));
}

// 0x001373B4
void StopCapture(nn::camera::CTR::Port port)
{
    PanicIfFailed(Camera::StopCapture(port));
}

// 0x00482774 | nintendogs:bytes [tier B]
bool IsBusy(nn::camera::CTR::Port port)
{
    bool isBusy;
    PanicIfFailed(Camera::IsBusy(&isBusy, port));
    return isBusy;
}

// 0x00137398
void ClearBuffer(nn::camera::CTR::Port port)
{
    PanicIfFailed(Camera::ClearBuffer(port));
}

// 0x00481DF8
void GetVsyncInterruptEvent(nn::Handle* event, nn::camera::CTR::Port port)
{
    nn::Handle newEvent;
    PanicIfFailed(Camera::GetVsyncInterruptEvent(&newEvent, port));
    ReplaceEvent(event, newEvent);
}

// 0x00481EFC
void GetBufferErrorInterruptEvent(nn::Handle* event, nn::camera::CTR::Port port)
{
    nn::Handle newEvent;
    PanicIfFailed(Camera::GetBufferErrorInterruptEvent(&newEvent, port));
    ReplaceEvent(event, newEvent);
}

// 0x00481818
void SetReceiving(nn::Handle* event, void* buffer, nn::camera::CTR::Port port, size_t imageSize, s16 transferUnit)
{
    uptr address = reinterpret_cast<uptr>(buffer);
    if (!IsInRange(address, nn::os::GetDeviceMemoryAddress(), nn::os::GetDeviceMemorySize())) {
        if (!nn::os::CTR::IsWramEnabled() || !IsInRange(address, nn::os::CTR::GetWramAddress(), nn::os::CTR::GetWramSize())) {
            nndbgPanic();
        }
    }
    nn::Handle newEvent;
    PanicIfFailed(Camera::SetReceiving(&newEvent, CURRENT_PROCESS, buffer, port, imageSize, transferUnit));
    ReplaceEvent(event, newEvent);
}

// 0x00481CB8
void SetTransferLines(nn::camera::CTR::Port port, s16 lines, s16 width, s16 height)
{
    PanicIfFailed(Camera::SetTransferLines(port, lines, width, height));
}

// 0x004816D0 | nintendogs:bytes [tier B]
s16 GetMaxLines(s16 width, s16 height)
{
    s16 lines;
    PanicIfFailed(Camera::GetMaxLines(&lines, width, height));
    return lines;
}

// 0x00481C9C
void SetTransferBytes(nn::camera::CTR::Port port, u32 bytes, s16 width, s16 height)
{
    PanicIfFailed(Camera::SetTransferBytes(port, bytes, width, height));
}

// 0x00481C24
u32 GetTransferBytes(nn::camera::CTR::Port port)
{
    u32 bytes;
    PanicIfFailed(Camera::GetTransferBytes(&bytes, port));
    return bytes;
}

// 0x004816AC
u32 GetMaxBytes(s16 width, s16 height)
{
    u32 bytes;
    PanicIfFailed(Camera::GetMaxBytes(&bytes, width, height));
    return bytes;
}

// 0x004816F4
void SetTrimming(nn::camera::CTR::Port port, bool isTrimming)
{
    PanicIfFailed(Camera::SetTrimming(port, isTrimming));
}

// 0x00481E38
void SetTrimmingParamsCenter(nn::camera::CTR::Port port, s16 trimWidth, s16 trimHeight, s16 cameraWidth, s16 cameraHeight)
{
    PanicIfFailed(Camera::SetTrimmingParamsCenter(port, trimWidth, trimHeight, cameraWidth, cameraHeight));
}

// 0x004818F8 | nintendogs:bytes [tier C]
nn::Result SetSharpness(nn::camera::CTR::CameraSelect select, s8 sharpness)
{
    return CallWithRetry([&] { return Camera::SetSharpness(select, sharpness); });
}

// 0x00481BA0 | nintendogs:bytes [tier C]
nn::Result SetAutoExposure(nn::camera::CTR::CameraSelect select, bool isAutoExposure)
{
    return CallWithRetry([&] { return Camera::SetAutoExposure(select, isAutoExposure); });
}

// 0x00481CD4
nn::Result SetAutoWhiteBalance(nn::camera::CTR::CameraSelect select, bool isAutoWhiteBalance)
{
    return CallWithRetry([&] { return Camera::SetAutoWhiteBalance(select, isAutoWhiteBalance); });
}

// 0x00482794
nn::Result SetSize(nn::camera::CTR::CameraSelect select, nn::camera::CTR::Size size, nn::camera::CTR::Context context)
{
    return CallWithRetry([&] { return Camera::SetSize(select, size, context); });
}

// 0x00481710 | nintendogs:bytes [tier C]
nn::Result SetFrameRate(nn::camera::CTR::CameraSelect select, nn::camera::CTR::FrameRate frameRate)
{
    return CallWithRetry([&] { return Camera::SetFrameRate(select, frameRate); });
}

// 0x00481794 | nintendogs:bytes [tier C]
nn::Result SetPhotoMode(nn::camera::CTR::CameraSelect select, nn::camera::CTR::PhotoMode photoMode)
{
    return CallWithRetry([&] { return Camera::SetPhotoMode(select, photoMode); });
}

// 0x00481D58
nn::Result SetAutoExposureWindow(nn::camera::CTR::CameraSelect select, s16 startX, s16 startY, s16 width, s16 height)
{
    return CallWithRetry([&] { return Camera::SetAutoExposureWindow(select, startX, startY, width, height); });
}

// 0x00481E5C
nn::Result SetAutoWhiteBalanceWindow(nn::camera::CTR::CameraSelect select, s16 startX, s16 startY, s16 width, s16 height)
{
    return CallWithRetry([&] { return Camera::SetAutoWhiteBalanceWindow(select, startX, startY, width, height); });
}

// 0x00481B1C
nn::Result SetNoiseFilter(nn::camera::CTR::CameraSelect select, bool isNoiseFilter)
{
    return CallWithRetry([&] { return Camera::SetNoiseFilter(select, isNoiseFilter); });
}

// 0x00481F3C
void GetStereoCameraCalibrationData(nn::camera::CTR::StereoCameraCalibrationData* data)
{
    PanicIfFailed(Camera::GetStereoCameraCalibrationData(data));
}

// 0x00481F58
nn::Result GetSuitableY2rStandardCoefficient(nn::y2r::CTR::StandardCoefficient* coefficient)
{
    return CallWithRetry([&] { return Camera::GetSuitableY2rStandardCoefficient(coefficient); });
}

// 0x00481C44 | nintendogs:bytes [tier B]
nn::Result PlayShutterSound(nn::camera::CTR::ShutterSoundType type)
{
    for (s32 i = 0; i < RETRY_COUNT; i++) {
        nn::Result result = Camera::PlayShutterSound(type);
        if (result.IsSuccess()) {
            return nn::Result();
        }
        if (result == RESULT_STATUS_CHANGED) {
            return result;
        }
    }
    return RESULT_FATAL;
}

} // namespace detail

// the library was initialized (the inline Initialize sets it; name is ours)
// 0x009581EC
bool s_IsInitialized;

// 0x0012DE08
void Finalize()
{
    if (s_IsInitialized) {
        detail::FinalizeBase(&detail::s_Session);
    }
    s_IsInitialized = false;
}

} // namespace CTR
} // namespace camera
} // namespace nn
