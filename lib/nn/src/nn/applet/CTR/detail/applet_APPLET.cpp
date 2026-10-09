// The APT commands (the original file is unknown; its static initializer at 0x007887B8 clears
// s_Session)
#include "nn/applet/CTR/detail/applet_APPLET.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "NS and APT Services")
const bit32 COMMAND_GET_LOCK_HANDLE = 0x00010040;
const bit32 COMMAND_INITIALIZE = 0x00020080;
const bit32 COMMAND_ENABLE = 0x00030040;
const bit32 COMMAND_GET_APPLET_MAN_INFO = 0x00050040;
const bit32 COMMAND_IS_REGISTERED = 0x00090040;
const bit32 COMMAND_INQUIRE_NOTIFICATION = 0x000B0040;
const bit32 COMMAND_SEND_PARAMETER = 0x000C0104;
const bit32 COMMAND_RECEIVE_PARAMETER = 0x000D0080;
const bit32 COMMAND_GLANCE_PARAMETER = 0x000E0080;
const bit32 COMMAND_CANCEL_PARAMETER = 0x000F0100;
const bit32 COMMAND_PREPARE_TO_START_LIBRARY_APPLET = 0x00180040;
const bit32 COMMAND_PREPARE_TO_START_SYSTEM_APPLET = 0x00190040;
const bit32 COMMAND_START_LIBRARY_APPLET = 0x001E0084;
const bit32 COMMAND_START_SYSTEM_APPLET = 0x001F0084;
const bit32 COMMAND_PREPARE_TO_CLOSE_APPLICATION = 0x00220040;
const bit32 COMMAND_CLOSE_APPLICATION = 0x00270044;
const bit32 COMMAND_PREPARE_TO_JUMP_TO_HOME_MENU = 0x002B0000;
const bit32 COMMAND_JUMP_TO_HOME_MENU = 0x002C0044;
const bit32 COMMAND_PREPARE_TO_DO_APPLICATION_JUMP = 0x00310100;
const bit32 COMMAND_DO_APPLICATION_JUMP = 0x00320084;
const bit32 COMMAND_CANCEL_LIBRARY_APPLET = 0x003B0040;
const bit32 COMMAND_REPLY_SLEEP_QUERY = 0x003E0080;
const bit32 COMMAND_REPLY_SLEEP_NOTIFICATION_COMPLETE = 0x003F0040;
const bit32 COMMAND_SEND_CAPTURE_BUFFER_INFO = 0x00400042;
const bit32 COMMAND_SLEEP_SYSTEM = 0x00420080;
const bit32 COMMAND_NOTIFY_TO_WAIT = 0x00430040;
const bit32 COMMAND_WRAP = 0x00460104;
const bit32 COMMAND_UNWRAP = 0x00470104;
const bit32 COMMAND_APPLET_UTILITY = 0x004B00C2;
const bit32 COMMAND_GET_APPLET_PROGRAM_INFO = 0x004D0080;
const bit32 COMMAND_SET_APPLICATION_CPU_TIME_LIMIT = 0x004F0080;
const bit32 COMMAND_WRAP1 = 0x00520104;
const bit32 COMMAND_UNWRAP1 = 0x00530104;
const bit32 COMMAND_SET_SCREEN_CAPTURE_POST_PERMISSION = 0x00550040;
const bit32 COMMAND_GET_TARGET_PLATFORM = 0x01010000;
const bit32 COMMAND_GET_APPLICATION_RUNNING_MODE = 0x01030000;
const bit32 COMMAND_IS_STANDARD_MEMORY_LAYOUT = 0x01040000;

// the translation descriptors (3dbrew "IPC")
const bit32 DESCRIPTOR_COPY_HANDLE = 0;

inline bit32 StaticBufferDescriptor(size_t size, s32 index)
{
    return (size << 14) | (index << 10) | 2;
}

inline bit32 ReadBufferDescriptor(size_t size)
{
    return (size << 4) | 0xA;
}

inline bit32 WriteBufferDescriptor(size_t size)
{
    return (size << 4) | 0xC;
}

inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline nn::Result Send(bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// sends a request whose reply goes into a buffer of the caller (the result of the system call)
inline nn::Result SendWithReceiveBuffer(void* buffer, size_t size, bit32* staticBuffers)
{
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(size, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    return result;
}

// Wrap, Unwrap, Wrap1 and Unwrap1 only differ in the command
inline nn::Result SendWrap(bit32 header, void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    command[1] = outputSize;
    command[2] = inputSize;
    command[3] = nonceOffset;
    command[4] = nonceSize;
    command[6] = reinterpret_cast<uptr>(pInput);
    command[8] = reinterpret_cast<uptr>(pOutput);
    command[7] = WriteBufferDescriptor(outputSize);
    command[5] = ReadBufferDescriptor(inputSize);
    return Send(command);
}
} // namespace

// 0x0097E818
nn::Handle s_Session;

// 0x00120518 | nintendogs:bytes [tier A]
nn::Result nn::applet::CTR::detail::APPLET::GetLockHandle(nn::Handle* pLock, unsigned attribute, unsigned* pAttribute, unsigned* pState)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_LOCK_HANDLE;
    command[1] = attribute;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pAttribute = command[2];
    *pState = command[3];
    *pLock = nn::Handle(command[5]);
    return nn::Result(command[1]);
}

// 0x001204C8 | tier B
nn::Result nn::applet::CTR::detail::APPLET::Initialize(u32 appId, u32 attribute, nn::Handle* pNotificationEvent, nn::Handle* pParameterEvent)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE;
    command[1] = appId;
    command[2] = attribute;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pNotificationEvent = nn::Handle(command[3]);
    *pParameterEvent = nn::Handle(command[4]);
    return nn::Result(command[1]);
}

// 0x001205EC | tier B
nn::Result nn::applet::CTR::detail::APPLET::Enable(u32 attribute)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ENABLE;
    command[1] = attribute;
    return Send(command);
}

// 0x004811C8 | tier B
nn::Result nn::applet::CTR::detail::APPLET::GetAppletManInfo(nn::applet::CTR::AppletPos pos, nn::applet::CTR::AppletPos* pPos, u32* pRequestedAppId, u32* pHomeMenuAppId, u32* pActiveAppId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_APPLET_MAN_INFO;
    SetByte(&command[1], pos);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pPos = static_cast<nn::applet::CTR::AppletPos>(*reinterpret_cast<u8*>(&command[2]));
    *pRequestedAppId = command[3];
    *pHomeMenuAppId = command[4];
    *pActiveAppId = command[5];
    return nn::Result(command[1]);
}

// 0x00137318 | tier B
nn::Result nn::applet::CTR::detail::APPLET::IsRegistered(u32 appId, bool* pIsRegistered)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IS_REGISTERED;
    command[1] = appId;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pIsRegistered = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0012BC8C | tier B
nn::Result nn::applet::CTR::detail::APPLET::InquireNotification(u32 appId, u8* pNotification)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INQUIRE_NOTIFICATION;
    command[1] = appId;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pNotification = *reinterpret_cast<u8*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00131960 | tier B
nn::Result nn::applet::CTR::detail::APPLET::SendParameter(u32 sourceAppId, u32 destinationAppId, u32 commandId, const u8* pParameter, u32 size, nn::Handle handle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_PARAMETER;
    command[1] = sourceAppId;
    command[2] = destinationAppId;
    command[3] = commandId;
    command[4] = size;
    command[5] = DESCRIPTOR_COPY_HANDLE;
    command[6] = handle.GetPrintableBits();
    command[7] = StaticBufferDescriptor(size, 0);
    command[8] = reinterpret_cast<uptr>(pParameter);
    return Send(command);
}

// 0x00131B38 | tier B
nn::Result nn::applet::CTR::detail::APPLET::ReceiveParameter(u32* pSenderAppId, u32 appId, u32* pCommand, u8* pBuffer, u32 bufferSize, s32* pSize, nn::Handle* pHandle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RECEIVE_PARAMETER;
    command[1] = appId;
    command[2] = bufferSize;
    nn::Result result = SendWithReceiveBuffer(pBuffer, bufferSize, nn::os::detail::GetIpcStaticBuffers());
    if (result.IsFailure()) {
        return result;
    }
    *pSenderAppId = command[2];
    *pCommand = command[3];
    *pSize = command[4];
    *pHandle = nn::Handle(command[6]);
    return nn::Result(command[1]);
}

// 0x00131A14 | tier B
nn::Result nn::applet::CTR::detail::APPLET::GlanceParameter(u32* pSenderAppId, u32 appId, u32* pCommand, u8* pBuffer, u32 bufferSize, s32* pSize, nn::Handle* pHandle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GLANCE_PARAMETER;
    command[1] = appId;
    command[2] = bufferSize;
    nn::Result result = SendWithReceiveBuffer(pBuffer, bufferSize, nn::os::detail::GetIpcStaticBuffers());
    if (result.IsFailure()) {
        return result;
    }
    *pSenderAppId = command[2];
    *pCommand = command[3];
    *pSize = command[4];
    *pHandle = nn::Handle(command[6]);
    return nn::Result(command[1]);
}

// 0x001319B8 | tier B
nn::Result nn::applet::CTR::detail::APPLET::CancelParameter(bool checkSender, u32 senderAppId, bool checkReceiver, u32 receiverAppId, bool* pIsCanceled)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CANCEL_PARAMETER;
    SetByte(&command[1], checkSender);
    command[2] = senderAppId;
    SetByte(&command[3], checkReceiver);
    command[4] = receiverAppId;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pIsCanceled = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0048144C | tier B
nn::Result nn::applet::CTR::detail::APPLET::PrepareToStartLibraryApplet(u32 appId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_PREPARE_TO_START_LIBRARY_APPLET;
    command[1] = appId;
    return Send(command);
}

// 0x00481414 | tier B
nn::Result nn::applet::CTR::detail::APPLET::PrepareToStartSystemApplet(u32 appId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_PREPARE_TO_START_SYSTEM_APPLET;
    command[1] = appId;
    return Send(command);
}

// 0x00481288 | tier B
nn::Result nn::applet::CTR::detail::APPLET::StartLibraryApplet(u32 appId, const u8* pParameter, u32 size, nn::Handle handle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_START_LIBRARY_APPLET;
    command[4] = handle.GetPrintableBits();
    command[1] = appId;
    command[2] = size;
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[5] = StaticBufferDescriptor(size, 0);
    command[6] = reinterpret_cast<uptr>(pParameter);
    return Send(command);
}

// 0x00481238 | tier B
nn::Result nn::applet::CTR::detail::APPLET::StartSystemApplet(u32 appId, const u8* pParameter, u32 size, nn::Handle handle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_START_SYSTEM_APPLET;
    command[4] = handle.GetPrintableBits();
    command[1] = appId;
    command[2] = size;
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[5] = StaticBufferDescriptor(size, 0);
    command[6] = reinterpret_cast<uptr>(pParameter);
    return Send(command);
}

// 0x00131C28 | tier B
nn::Result nn::applet::CTR::detail::APPLET::PrepareToCloseApplication(bool cancelPreload)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_PREPARE_TO_CLOSE_APPLICATION;
    SetByte(&command[1], cancelPreload);
    return Send(command);
}

// 0x00131AE4 | tier B
nn::Result nn::applet::CTR::detail::APPLET::CloseApplication(const u8* pParameter, u32 size, nn::Handle handle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CLOSE_APPLICATION;
    command[3] = handle.GetPrintableBits();
    command[1] = size;
    command[2] = DESCRIPTOR_COPY_HANDLE;
    command[5] = reinterpret_cast<uptr>(pParameter);
    command[4] = StaticBufferDescriptor(size, 0);
    return Send(command);
}

// 0x004813A4 | nintendogs:callgraph [tier A]
nn::Result nn::applet::CTR::detail::APPLET::PrepareToJumpToHomeMenu()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_PREPARE_TO_JUMP_TO_HOME_MENU;
    return Send(command);
}

// 0x00481174 | tier B
nn::Result nn::applet::CTR::detail::APPLET::JumpToHomeMenu(const u8* pParameter, u32 size, nn::Handle handle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_JUMP_TO_HOME_MENU;
    command[3] = handle.GetPrintableBits();
    command[1] = size;
    command[2] = DESCRIPTOR_COPY_HANDLE;
    command[5] = reinterpret_cast<uptr>(pParameter);
    command[4] = StaticBufferDescriptor(size, 0);
    return Send(command);
}

// 0x00131C64 | tier B
nn::Result nn::applet::CTR::detail::APPLET::PrepareToDoApplicationJump(u8 flags, u64 titleId, u8 mediaType)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_PREPARE_TO_DO_APPLICATION_JUMP;
    SetByte(&command[1], flags);
    *reinterpret_cast<u64*>(&command[2]) = titleId;
    SetByte(&command[4], mediaType);
    return Send(command);
}

// 0x00131BC4 | tier B
nn::Result nn::applet::CTR::detail::APPLET::DoApplicationJump(const u8* pParameter, u32 size, const u8* pHmac, u32 hmacSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DO_APPLICATION_JUMP;
    command[1] = size;
    command[2] = hmacSize;
    command[4] = reinterpret_cast<uptr>(pParameter);
    command[3] = StaticBufferDescriptor(size, 0);
    command[5] = StaticBufferDescriptor(hmacSize, 2);
    command[6] = reinterpret_cast<uptr>(pHmac);
    return Send(command);
}

// 0x0013735C | tier B
nn::Result nn::applet::CTR::detail::APPLET::CancelLibraryApplet(bool isExiting)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CANCEL_LIBRARY_APPLET;
    SetByte(&command[1], isExiting);
    return Send(command);
}

// 0x00131AA0 | tier B
nn::Result nn::applet::CTR::detail::APPLET::ReplySleepQuery(u32 appId, nn::applet::CTR::QueryReply reply)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_REPLY_SLEEP_QUERY;
    command[1] = appId;
    SetByte(&command[2], reply);
    return Send(command);
}

// 0x00131CB0 | tier B
nn::Result nn::applet::CTR::detail::APPLET::ReplySleepNotificationComplete(u32 appId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_REPLY_SLEEP_NOTIFICATION_COMPLETE;
    command[1] = appId;
    return Send(command);
}

// 0x0048131C | tier B
nn::Result nn::applet::CTR::detail::APPLET::SendCaptureBufferInfo(const u8* pInfo, u32 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_CAPTURE_BUFFER_INFO;
    command[1] = size;
    command[3] = reinterpret_cast<uptr>(pInfo);
    command[2] = StaticBufferDescriptor(size, 0);
    return Send(command);
}

// 0x0012BC1C | tier B
nn::Result nn::applet::CTR::detail::APPLET::SleepSystem(u64 time)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SLEEP_SYSTEM;
    *reinterpret_cast<u64*>(&command[1]) = time;
    return Send(command);
}

// 0x0012BC54 | tier B
nn::Result nn::applet::CTR::detail::APPLET::NotifyToWait(u32 appId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_NOTIFY_TO_WAIT;
    command[1] = appId;
    return Send(command);
}

// 0x00481484 | tier B
nn::Result nn::applet::CTR::detail::APPLET::Wrap(void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize)
{
    return SendWrap(COMMAND_WRAP, pOutput, pInput, outputSize, inputSize, nonceOffset, nonceSize);
}

// 0x0048154C | tier B
nn::Result nn::applet::CTR::detail::APPLET::Unwrap(void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize)
{
    return SendWrap(COMMAND_UNWRAP, pOutput, pInput, outputSize, inputSize, nonceOffset, nonceSize);
}

// 0x004814E8 | tier B
nn::Result nn::applet::CTR::detail::APPLET::Wrap1(void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize)
{
    return SendWrap(COMMAND_WRAP1, pOutput, pInput, outputSize, inputSize, nonceOffset, nonceSize);
}

// 0x004815B0 | tier B
nn::Result nn::applet::CTR::detail::APPLET::Unwrap1(void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize)
{
    return SendWrap(COMMAND_UNWRAP1, pOutput, pInput, outputSize, inputSize, nonceOffset, nonceSize);
}

// 0x001318E4 | tier B
nn::Result nn::applet::CTR::detail::APPLET::AppletUtility(u32 utilityId, const u8* pInput, u32 inputSize, u8* pOutput, u32 outputSize, s32* pResult)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_APPLET_UTILITY;
    command[1] = utilityId;
    command[2] = inputSize;
    command[3] = outputSize;
    command[4] = StaticBufferDescriptor(inputSize, 1);
    command[5] = reinterpret_cast<uptr>(pInput);
    nn::Result result = SendWithReceiveBuffer(pOutput, outputSize, nn::os::detail::GetIpcStaticBuffers());
    if (result.IsFailure()) {
        return result;
    }
    *pResult = command[2];
    return nn::Result(command[1]);
}

// 0x004812D8 | tier B
nn::Result nn::applet::CTR::detail::APPLET::GetAppletProgramInfo(u32 appId, u32 flags, u16* pVersion)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_APPLET_PROGRAM_INFO;
    command[1] = appId;
    command[2] = flags;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pVersion = *reinterpret_cast<u16*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x001205B4 | tier B
nn::Result nn::applet::CTR::detail::APPLET::SetApplicationCpuTimeLimit(u32 fixed, u32 percent)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_APPLICATION_CPU_TIME_LIMIT;
    command[1] = fixed;
    command[2] = percent;
    return Send(command);
}

// 0x0011E794 | tier B
nn::Result nn::applet::CTR::detail::APPLET::SetScreenCapturePostPermission(u8 permission)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SCREEN_CAPTURE_POST_PERMISSION;
    SetByte(&command[1], permission);
    return Send(command);
}

// 0x00120574 | fefates:bytes [tier B]
nn::Result nn::applet::CTR::detail::APPLET::GetTargetPlatform(nn::ptm::CTR::TargetPlatform* pPlatform)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TARGET_PLATFORM;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pPlatform = static_cast<nn::ptm::CTR::TargetPlatform>(*reinterpret_cast<u8*>(&command[2]));
    return nn::Result(command[1]);
}

// 0x004813D4 | fefates:bytes [tier B]
nn::Result nn::applet::CTR::detail::APPLET::GetApplicationRunningMode(nn::applet::CTR::ApplicationRunningMode* pMode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_APPLICATION_RUNNING_MODE;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pMode = static_cast<nn::applet::CTR::ApplicationRunningMode>(*reinterpret_cast<u8*>(&command[2]));
    return nn::Result(command[1]);
}

// 0x00481368 (name after 3dbrew)
nn::Result nn::applet::CTR::detail::APPLET::IsStandardMemoryLayout(bool* pIsStandard)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IS_STANDARD_MEMORY_LAYOUT;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pIsStandard = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
