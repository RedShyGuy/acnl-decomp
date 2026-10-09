// applet_Wrapper.cpp (file name from the static initializer 0x00797D1C: s_CaptureBufferInfo,
// s_SleepAcceptedCallbackLock): starting other applets, the HOME menu and the screen capture
#include "nn/applet/CTR/detail/applet_APPLET.h"
#include "nn/applet/CTR/detail/applet_Timeout.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/camera/CTR/detail/detail_Api.h"
#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/nfp/CTR/CTR_Api.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Thread.h"
#include "nn/os/os_TransferMemoryBlock.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
namespace {
// results (module 51 applet; the names are ours)
const bit32 RESULT_NOT_ALLOWED = 0xC8A0CC04;     // status, invalid state, 4: not an application / system applet
const bit32 RESULT_ALREADY_PREPARED = 0xC8A0CFFC; // status, invalid state, 1020
const bit32 RESULT_NO_PARAMETER = 0xC8A0CFEF;    // status, invalid state, 1007
const bit32 RESULT_BUSY_1 = 0xC8A0CFF0;
const bit32 RESULT_BUSY_2 = 0xE0A0CC08;
const bit32 RESULT_BUSY_3 = 0xC8A0CC02;

// the internet browser applet (3dbrew "NS and APT Services", AppID)
const u32 APP_ID_INTERNET_BROWSER = 0x114;
// GetAppletProgramInfo flags (3dbrew "APT:GetAppletProgramInfo")
const u32 PROGRAM_INFO_FLAGS = 17;

// the commands of parameters (3dbrew "NS and APT Services", Command)
const u32 COMMAND_REQUEST = 2;
const u32 COMMAND_RESPONSE = 3;
const u32 COMMAND_REQUEST_FOR_SYS_APPLET = 16;

// the permissions of the capture buffer of the next applet (read / write)
const u32 CAPTURE_PERMISSION = 3;

// the screens (in pixels; the capture has 240 lines, rounded to 256 in the buffer)
const u32 TOP_WIDTH = 400;
const u32 BOTTOM_WIDTH = 320;
const u32 SCREEN_HEIGHT = 240;
const u32 TILE_SIZE = 8;

const s64 RETRY_WAIT_MSEC = 10;

inline bool IsBusy(nn::Result result)
{
    return result == RESULT_BUSY_1 || result == RESULT_BUSY_2 || result == RESULT_BUSY_3;
}

inline void WaitToRetry()
{
    nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(RETRY_WAIT_MSEC));
}

// no sleep while another applet starts; afterwards as before
inline bool DisableSleepForTransition()
{
    bool isSleepEnabled = IsEnableSleep();
    if (isSleepEnabled) {
        DisableSleep(true);
    }
    return isSleepEnabled;
}

inline void RestoreSleep(bool isSleepEnabled)
{
    if (isSleepEnabled) {
        if (!IsEnableSleep()) {
            EnableSleep(true);
        }
    } else if (IsEnableSleep()) {
        DisableSleep(true);
    }
}

// the bytes of a pixel of a framebuffer format (RGBA8, RGB8, RGB565, RGB5A1, RGBA4)
inline u32 GetBytesPerPixel(u32 format)
{
    switch (format) {
    case 0:
        return 4;
    case 1:
        return 3;
    case 2:
    case 3:
    case 4:
        return 2;
    }
    return 0;
}

// the capture has no alpha
inline u32 GetCaptureBytesPerPixel(u32 format)
{
    u32 bytes = GetBytesPerPixel(format);
    return bytes > 3 ? 3 : bytes;
}

typedef void (*ConvertFunction)(unsigned*, const unsigned*, unsigned, const OffsetTable&);
} // namespace

// the layout of the captures in the buffer of the next applet
// 0x00AE1A5C
nn::applet::CTR::CaptureBufferInfo s_CaptureBufferInfo;
// 0x00AE1A7C
nn::os::CriticalSection s_SleepAcceptedCallbackLock((nn::os::CriticalSection::InitializeTag()));

// 0x0047FA2C (name is ours)
bool IsInternetBrowserAvailable()
{
    u16 version;
    LockAndConnect();
    nn::Result result = APPLET::GetAppletProgramInfo(APP_ID_INTERNET_BROWSER, PROGRAM_INFO_FLAGS, &version);
    DisconnectAndUnlock();
    if (result.IsFailure()) {
        return false;
    }
    bool isStandard;
    LockAndConnect();
    result = APPLET::IsStandardMemoryLayout(&isStandard);
    DisconnectAndUnlock();
    if (result.IsFailure() || !isStandard) {
        return false;
    }
    return true;
}

// captures both screens into the transfer memory the library applet sends back
// 0x0047FEA0 | nintendogs:callseq [tier A]
nn::Result CaptureScreen(unsigned appId)
{
    nn::applet::CTR::AppletDisplayInfo displayInfo;
    GetDisplayInfo(&displayInfo);
    WaitForRegister(appId, WAIT_INFINITE);
    s_CaptureBufferInfo.screens[0].format = displayInfo.screens[0].format;
    s_CaptureBufferInfo.screens[1].format = displayInfo.screens[1].format;
    s_CaptureBufferInfo.is3D = displayInfo.screens[0].leftAddress != displayInfo.screens[0].rightAddress;
    CalcCaptureBufferInfo(&s_CaptureBufferInfo);
    nn::os::TransferMemoryBlock block;
    Send(appId, COMMAND_REQUEST, reinterpret_cast<const unsigned char*>(&s_CaptureBufferInfo), sizeof(s_CaptureBufferInfo),
         INVALID_HANDLE, WAIT_INFINITE);
    nn::Handle handle;
    WaitToCaptureScreen(appId, &handle);
    AttachTransferMemoryHandle(&block, handle, s_CaptureBufferInfo.size, CAPTURE_PERMISSION);
    CaptureDisplayBuffer(block.GetAddress(), &displayInfo, &s_CaptureBufferInfo);
    block.Finalize();
    SendCaptureBufferInfo(reinterpret_cast<const unsigned char*>(&s_CaptureBufferInfo), sizeof(s_CaptureBufferInfo));
    return nn::Result();
}

// 0x0047FFD4 | nintendogs:bytes [tier A]
void GetDisplayInfo(nn::applet::CTR::AppletDisplayInfo* pInfo)
{
    if (pInfo == 0) {
        return;
    }
    nn::gxlow::CTR::DisplayCaptureInfo info;
    nn::gxlow::CTR::ImportDisplayCaptureInfo(&info);
    pInfo->screens[0].leftAddress = reinterpret_cast<uptr>(info.top.leftFramebuffer);
    pInfo->screens[0].rightAddress = reinterpret_cast<uptr>(info.top.rightFramebuffer);
    pInfo->screens[1].leftAddress = reinterpret_cast<uptr>(info.bottom.leftFramebuffer);
    pInfo->screens[1].rightAddress = reinterpret_cast<uptr>(info.bottom.rightFramebuffer);
    pInfo->screens[0].format = info.top.format;
    pInfo->screens[1].format = info.bottom.format;
    pInfo->screens[0].stride = info.top.stride;
    pInfo->screens[1].stride = info.bottom.stride;
}

// leaves the application for the HOME menu (the program waits in WaitForStarting)
// 0x00480038 | tier C
nn::Result JumpToHomeMenu(const unsigned char* pParameter, unsigned int size, nn::Handle handle)
{
    nn::applet::CTR::AppletPos pos;
    u32 requestedAppId;
    u32 homeMenuAppId;
    u32 activeAppId;
    LockAndConnect();
    nn::err::CTR::ThrowFatalErrIfFailure(
        APPLET::GetAppletManInfo(POS_NONE, &pos, &requestedAppId, &homeMenuAppId, &activeAppId));
    DisconnectAndUnlock();
    u32 appId = homeMenuAppId;
    if (IsApplication()) {
        for (;;) {
            bool isRegistered;
            LockAndConnect();
            nn::err::CTR::ThrowFatalErrIfFailure(APPLET::IsRegistered(appId, &isRegistered));
            DisconnectAndUnlock();
            if (isRegistered) {
                break;
            }
            WaitToRetry();
        }
        s_IsVramSaved = true;
        nn::gxlow::CTR::SaveVramSysArea();
        CaptureScreenForSystemApplet(appId);
        nn::nfp::CTR::Finalize();
        FinalizeModule98();
    }
    AssignDspRight(false);
    AssignGpuRight(false);
    nn::camera::CTR::detail::LeaveApplication();
    LockAndConnect();
    nn::Result result = APPLET::JumpToHomeMenu(pParameter, size, handle);
    nn::err::CTR::ThrowFatalErrIfFailure(result);
    DisconnectAndUnlock();
    SetInactive();
    return result;
}

// an 8x8 tile of a 16 bit framebuffer (lines left to right) in the block order of the GPU
// 0x00480198 | nintendogs:bytes [tier A]
void ConvertL16ToB16(unsigned* pOutput, const unsigned* pInput, unsigned stride, const nn::applet::CTR::detail::OffsetTable& table)
{
    for (s32 i = 4; i > 0; i--) {
        // two 2x2 blocks below each other
        const unsigned* pLine = pInput + table.offsets[i];
        *pOutput++ = pLine[0];
        *pOutput++ = pLine[stride];
        *pOutput++ = pLine[1];
        *pOutput++ = pLine[stride + 1];
        pLine += stride * 2;
        *pOutput++ = pLine[0];
        *pOutput++ = pLine[stride];
        *pOutput++ = pLine[1];
        *pOutput++ = pLine[stride + 1];
    }
}

// the same for 24 bit pixels (two lines of four pixels in six words)
// 0x00480208 | nintendogs:bytes [tier A]
void ConvertL24ToB24(unsigned* pOutput, const unsigned* pInput, unsigned stride, const nn::applet::CTR::detail::OffsetTable& table)
{
    for (s32 i = 4; i > 0; i--) {
        // two blocks of 2x2 pixels below each other
        const unsigned* pLine = pInput + table.offsets[i];
        const unsigned* pNext = pLine + stride;
        *pOutput++ = pLine[0];
        *pOutput++ = (pLine[1] & 0xFFFF) | (pNext[0] << 16);
        *pOutput++ = (pNext[0] >> 16) | (pNext[1] << 16);
        *pOutput++ = (pLine[1] >> 16) | (pLine[2] << 16);
        *pOutput++ = (pLine[2] >> 16) | (pNext[1] & 0xFFFF0000);
        *pOutput++ = pNext[2];
        pLine += stride * 2;
        pNext = pLine + stride;
        *pOutput++ = pLine[0];
        *pOutput++ = (pLine[1] & 0xFFFF) | (pNext[0] << 16);
        *pOutput++ = (pNext[0] >> 16) | (pNext[1] << 16);
        *pOutput++ = (pLine[1] >> 16) | (pLine[2] << 16);
        *pOutput++ = (pLine[2] >> 16) | (pNext[1] & 0xFFFF0000);
        *pOutput++ = pNext[2];
    }
}

// true when the applet is registered before the timeout
// 0x004802E8 | nintendogs:callseq-callee [tier A]
bool WaitForRegister(unsigned appId, nn::fnd::TimeSpan timeout)
{
    s64 startTick = GetTimeoutStart(timeout);
    for (;;) {
        bool isRegistered;
        LockAndConnect();
        nn::err::CTR::ThrowFatalErrIfFailure(APPLET::IsRegistered(appId, &isRegistered));
        DisconnectAndUnlock();
        if (isRegistered) {
            return true;
        }
        if (IsTimedOut(timeout, startTick)) {
            return false;
        }
        WaitToRetry();
    }
}

// 0x0048046C | nintendogs:bytes [tier A]
void GetAppletManInfo(nn::applet::CTR::AppletPos pos, nn::applet::CTR::AppletPos* pPos, unsigned* pRequestedAppId, unsigned* pHomeMenuAppId, unsigned* pActiveAppId)
{
    nn::applet::CTR::AppletPos activePos;
    u32 requestedAppId;
    u32 homeMenuAppId;
    u32 activeAppId;
    LockAndConnect();
    nn::err::CTR::ThrowFatalErrIfFailure(
        APPLET::GetAppletManInfo(pos, &activePos, &requestedAppId, &homeMenuAppId, &activeAppId));
    DisconnectAndUnlock();
    if (pPos) {
        *pPos = activePos;
    }
    if (pRequestedAppId) {
        *pRequestedAppId = requestedAppId;
    }
    if (pHomeMenuAppId) {
        *pHomeMenuAppId = homeMenuAppId;
    }
    if (pActiveAppId) {
        *pActiveAppId = activeAppId;
    }
}

// 0x004804F0 | tier C (confirmed by the code)
nn::Result StartSystemApplet(unsigned int appId, const unsigned char* pParameter, unsigned int size, nn::Handle handle)
{
    if (!IsApplication() && !IsSystemApplet()) {
        return nn::Result(RESULT_NOT_ALLOWED);
    }
    if (IsSystemApplet()) {
        AssignDspRight(false);
        AssignGpuRight(false);
    } else if (IsApplication()) {
        s_IsVramSaved = true;
        nn::gxlow::CTR::SaveVramSysArea();
        nn::nfp::CTR::Finalize();
        FinalizeModule98();
        AssignDspRight(false);
        AssignGpuRight(false);
        nn::camera::CTR::detail::LeaveApplication();
    }
    bool isSleepEnabled = DisableSleepForTransition();
    nn::Result result;
    for (;;) {
        LockAndConnect();
        result = APPLET::StartSystemApplet(appId, pParameter, size, handle);
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            break;
        }
        WaitToRetry();
    }
    RestoreSleep(isSleepEnabled);
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    SetInactive();
    if (IsApplication()) {
        result = CaptureScreenForSystemApplet(appId);
    }
    if (IsSystemApplet() && !IsHomeMenuResident()) {
        nn::svc::ExitProcess();
    }
    return result;
}

// 0x00480710 | nintendogs:callgraph [tier A]
nn::Result StartLibraryApplet(unsigned appId, const unsigned char* pParameter, unsigned size, nn::Handle handle)
{
    if (!IsApplication() && !IsSystemApplet()) {
        return nn::Result(RESULT_NOT_ALLOWED);
    }
    if (IsApplication()) {
        s_IsVramSaved = true;
        nn::gxlow::CTR::SaveVramSysArea();
        nn::nfp::CTR::Finalize();
        FinalizeModule98();
    }
    CaptureScreen(appId);
    AssignGpuRight(false);
    bool isSleepEnabled = DisableSleepForTransition();
    nn::Result result;
    for (;;) {
        LockAndConnect();
        result = APPLET::StartLibraryApplet(appId, pParameter, size, handle);
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            break;
        }
        WaitToRetry();
    }
    RestoreSleep(isSleepEnabled);
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    SetInactive();
    return result;
}

// waits for the answer of the applet to the capture request (with the transfer memory)
// 0x0048089C | nintendogs:callseq-callee [tier A]
nn::Result WaitToCaptureScreen(unsigned appId, nn::Handle* pHandle)
{
    for (;;) {
        bool isDone = true;
        nn::Handle handle;
        unsigned senderAppId;
        unsigned command;
        int size;
        nn::Result result = Receive(&senderAppId, &command, 0, 0, &size, &handle, WAIT_INFINITE);
        if (result.GetDescription() != (RESULT_NO_PARAMETER & 0x3FF)) {
            if (result.IsSuccess() && s_Callbacks.parameter) {
                isDone = s_Callbacks.parameter(s_CallbackArguments[CALLBACK_PARAMETER], senderAppId, command, 0, 0,
                                               size, handle);
            }
            if (pHandle) {
                *pHandle = handle;
            }
        }
        if (!isDone) {
            continue;
        }
        if (result.IsFailure() || (command == COMMAND_RESPONSE && senderAppId == appId)) {
            return result;
        }
    }
}

// the offsets and the size of the captures (the bottom screen first, then the top screen, the
// right eye after the left one in 3D)
// 0x00480998 | nintendogs:callseq-callee [tier A]
void CalcCaptureBufferInfo(nn::applet::CTR::CaptureBufferInfo* pInfo)
{
    u32 offset = GetCaptureBytesPerPixel(pInfo->screens[1].format) * BOTTOM_WIDTH * 256;
    pInfo->screens[1].rightOffset = 0;
    pInfo->screens[1].leftOffset = 0;
    pInfo->screens[0].rightOffset = offset;
    pInfo->screens[0].leftOffset = offset;
    offset += GetCaptureBytesPerPixel(pInfo->screens[0].format) * TOP_WIDTH * 256;
    if (pInfo->is3D) {
        pInfo->screens[0].rightOffset = offset;
        offset += GetCaptureBytesPerPixel(pInfo->screens[0].format) * TOP_WIDTH * 256;
    }
    pInfo->size = offset + GetCaptureBytesPerPixel(pInfo->screens[0].format) * (512 - TOP_WIDTH) * 256;
}

// 0x00480ACC | nintendogs:callseq [tier A]
nn::Result SendCaptureBufferInfo(const unsigned char* pInfo, unsigned size)
{
    LockAndConnect();
    nn::Result result = APPLET::SendCaptureBufferInfo(pInfo, size);
    DisconnectAndUnlock();
    return result;
}

// 0x00480AF8 | nintendogs:bytes [tier A]
nn::Result PrepareToJumpToHomeMenu()
{
    SetTransitionType(TRANSITION_JUMP_TO_HOME_MENU);
    for (;;) {
        LockAndConnect();
        nn::Result result = APPLET::PrepareToJumpToHomeMenu();
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            return result;
        }
        WaitToRetry();
    }
}

// 0x00480B84 | nintendogs:bytes [tier A]
void CaptureDisplayBuffer(unsigned address, const nn::applet::CTR::AppletDisplayInfo* pDisplayInfo, const nn::applet::CTR::CaptureBufferInfo* pBufferInfo)
{
    if (pDisplayInfo == 0 || pBufferInfo == 0) {
        return;
    }
    if (pBufferInfo->is3D) {
        CaptureDisplayBufferCore(address + pBufferInfo->screens[1].leftOffset, pDisplayInfo, true, false);
        CaptureDisplayBufferCore(address + pBufferInfo->screens[0].leftOffset, pDisplayInfo, false, false);
        CaptureDisplayBufferCore(address + pBufferInfo->screens[0].rightOffset, pDisplayInfo, false, true);
    } else {
        CaptureDisplayBufferCore(address + pBufferInfo->screens[1].leftOffset, pDisplayInfo, true, false);
        CaptureDisplayBufferCore(address + pBufferInfo->screens[0].leftOffset, pDisplayInfo, false, false);
    }
}

// copies a screen into the capture buffer, tile by tile (240 lines rounded up to 256)
// 0x00480C18 | nintendogs:callgraph [tier A]
void CaptureDisplayBufferCore(unsigned address, const nn::applet::CTR::AppletDisplayInfo* pDisplayInfo, bool isBottom, bool isRight)
{
    u32 width = isBottom ? BOTTOM_WIDTH : TOP_WIDTH;
    const nn::applet::CTR::AppletDisplayInfo::Screen& screen = pDisplayInfo->screens[isBottom];
    const unsigned* pSource = reinterpret_cast<const unsigned*>(isRight ? screen.rightAddress : screen.leftAddress);
    u32 format = screen.format;
    u32 bitsPerPixel = format == 0 ? 24 : GetBytesPerPixel(format) * 8;
    u32 strideWords = screen.stride / 4;
    u32 tileWords = bitsPerPixel * TILE_SIZE / 4;
    u32 gapWords = bitsPerPixel * 16 / 4;

    OffsetTable table;
    ConvertFunction convert;
    switch (format) {
    case 0:
    case 1:
        table.offsets[1] = 3 + strideWords * 4;
        table.offsets[2] = strideWords * 4;
        table.offsets[0] = 0;
        table.offsets[3] = 3;
        table.offsets[4] = 0;
        convert = ConvertL24ToB24;
        break;
    case 2:
    case 3:
    case 4:
        table.offsets[1] = 2 + strideWords * 4;
        table.offsets[2] = strideWords * 4;
        table.offsets[0] = 0;
        table.offsets[3] = 2;
        table.offsets[4] = 0;
        convert = ConvertL16ToB16;
        break;
    default:
        return;
    }
    if (address == 0) {
        return;
    }
    unsigned* pOutput = reinterpret_cast<unsigned*>(address);
    for (u32 x = 0; x < width; x += TILE_SIZE) {
        const unsigned* pInput = pSource;
        for (u32 y = 0; y < SCREEN_HEIGHT; y += TILE_SIZE) {
            convert(pOutput, pInput, strideWords, table);
            pInput += bitsPerPixel / 4;
            pOutput += tileWords;
        }
        pSource += strideWords * TILE_SIZE;
        pOutput += gapWords;
    }
}

// 0x00480DE0 | nintendogs:bytes [tier A]
nn::Result AttachTransferMemoryHandle(nn::os::TransferMemoryBlock* pBlock, nn::Handle handle, unsigned size, unsigned permission)
{
    return pBlock->AttachAndMap(handle, size, permission, CAPTURE_PERMISSION);
}

// 0x00480DF4 | nintendogs:bytes [tier A]
nn::Result PrepareToStartSystemApplet(unsigned appId)
{
    CancelLibraryAppletIfRegistered(false, 0);
    SetTransitionType(TRANSITION_START_SYSTEM_APPLET);
    bool isSleepEnabled = DisableSleepForTransition();
    nn::Result result;
    for (;;) {
        LockAndConnect();
        result = APPLET::PrepareToStartSystemApplet(appId);
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            break;
        }
        WaitToRetry();
    }
    RestoreSleep(isSleepEnabled);
    if (IsApplication() && result == RESULT_ALREADY_PREPARED) {
        result = nn::Result();
    }
    return result;
}

// 0x00480F00 | nintendogs:bytes [tier A]
nn::Result PrepareToStartLibraryApplet(unsigned appId)
{
    SetTransitionType(TRANSITION_START_LIBRARY_APPLET);
    bool isSleepEnabled = DisableSleepForTransition();
    nn::Result result;
    for (;;) {
        LockAndConnect();
        result = APPLET::PrepareToStartLibraryApplet(appId);
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            break;
        }
        WaitToRetry();
    }
    RestoreSleep(isSleepEnabled);
    if (result == RESULT_ALREADY_PREPARED) {
        result = nn::Result();
    }
    return result;
}

// the capture for the HOME menu / a system applet (it maps the buffer itself)
// 0x00480FEC | nintendogs:callseq [tier A]
nn::Result CaptureScreenForSystemApplet(unsigned appId)
{
    nn::applet::CTR::AppletDisplayInfo displayInfo;
    GetDisplayInfo(&displayInfo);
    WaitForRegister(appId, WAIT_INFINITE);
    s_CaptureBufferInfo.screens[0].format = displayInfo.screens[0].format;
    s_CaptureBufferInfo.screens[1].format = displayInfo.screens[1].format;
    s_CaptureBufferInfo.is3D = displayInfo.screens[0].leftAddress != displayInfo.screens[0].rightAddress;
    CalcCaptureBufferInfo(&s_CaptureBufferInfo);
    Send(appId, COMMAND_REQUEST_FOR_SYS_APPLET, reinterpret_cast<const unsigned char*>(&s_CaptureBufferInfo),
         sizeof(s_CaptureBufferInfo), INVALID_HANDLE, WAIT_INFINITE);
    WaitToCaptureScreen(appId, 0);
    SendCaptureBufferInfo(reinterpret_cast<const unsigned char*>(&s_CaptureBufferInfo), sizeof(s_CaptureBufferInfo));
    return nn::Result();
}

// 0x004810B8 (name after the command)
nn::Result SetScreenCapturePostPermission(u8 permission)
{
    LockAndConnect();
    nn::Result result = APPLET::SetScreenCapturePostPermission(permission);
    DisconnectAndUnlock();
    return result;
}

// encrypts with the console key: the output has a 16 byte MAC more
// 0x004810DC | tier C
nn::Result Wrap(void* pOutput, const void* pInput, unsigned size, int nonceOffset, unsigned nonceSize)
{
    LockAndConnect();
    nn::Result result = APPLET::Wrap(pOutput, pInput, size + 16, size, nonceOffset, nonceSize);
    DisconnectAndUnlock();
    return result;
}

// 0x00481128 (name after the command)
nn::Result Wrap1(void* pOutput, const void* pInput, unsigned size, int nonceOffset, unsigned nonceSize)
{
    LockAndConnect();
    nn::Result result = APPLET::Wrap1(pOutput, pInput, size + 16, size, nonceOffset, nonceSize);
    DisconnectAndUnlock();
    return result;
}

// 0x00481614 | tier C
nn::Result Unwrap(void* pOutput, const void* pInput, unsigned size, int nonceOffset, unsigned nonceSize)
{
    LockAndConnect();
    nn::Result result = APPLET::Unwrap(pOutput, pInput, size - 16, size, nonceOffset, nonceSize);
    DisconnectAndUnlock();
    return result;
}

// 0x00481660 (name after the command)
nn::Result Unwrap1(void* pOutput, const void* pInput, unsigned size, int nonceOffset, unsigned nonceSize)
{
    LockAndConnect();
    nn::Result result = APPLET::Unwrap1(pOutput, pInput, size - 16, size, nonceOffset, nonceSize);
    DisconnectAndUnlock();
    return result;
}

// 0x003E2270 (name is ours)
nn::Result FinalizeModule98()
{
    return nn::Result(0xC8A18A00); // status, invalid state, module 98, 512: not supported
}

} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
