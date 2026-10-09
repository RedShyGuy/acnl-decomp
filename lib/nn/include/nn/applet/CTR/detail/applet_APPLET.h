#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/applet/CTR/applet_Types.h"
#include "nn/ptm/CTR/ptm_Types.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
// The commands of the APT service (3dbrew "NS and APT Services"); all static, on s_Session. The
// function names are from the symbols or after the 3dbrew command names, the parameter names are ours.
class APPLET
{
public:
    static nn::Result GetLockHandle(nn::Handle* pLock, unsigned attribute, unsigned* pAttribute, unsigned* pState); // 0x00120518 | nintendogs:bytes [tier A]
    static nn::Result Initialize(u32 appId, u32 attribute, nn::Handle* pNotificationEvent, nn::Handle* pParameterEvent); // 0x001204C8 | tier B
    static nn::Result Enable(u32 attribute); // 0x001205EC | tier B
    static nn::Result GetAppletManInfo(nn::applet::CTR::AppletPos pos, nn::applet::CTR::AppletPos* pPos, u32* pRequestedAppId, u32* pHomeMenuAppId, u32* pActiveAppId); // 0x004811C8 | tier B
    static nn::Result IsRegistered(u32 appId, bool* pIsRegistered); // 0x00137318 | tier B
    static nn::Result InquireNotification(u32 appId, u8* pNotification); // 0x0012BC8C | tier B
    static nn::Result SendParameter(u32 sourceAppId, u32 destinationAppId, u32 commandId, const u8* pParameter, u32 size, nn::Handle handle); // 0x00131960 | tier B
    static nn::Result ReceiveParameter(u32* pSenderAppId, u32 appId, u32* pCommand, u8* pBuffer, u32 bufferSize, s32* pSize, nn::Handle* pHandle); // 0x00131B38 | tier B
    static nn::Result GlanceParameter(u32* pSenderAppId, u32 appId, u32* pCommand, u8* pBuffer, u32 bufferSize, s32* pSize, nn::Handle* pHandle); // 0x00131A14 | tier B
    static nn::Result CancelParameter(bool checkSender, u32 senderAppId, bool checkReceiver, u32 receiverAppId, bool* pIsCanceled); // 0x001319B8 | tier B
    static nn::Result PrepareToStartLibraryApplet(u32 appId); // 0x0048144C | tier B
    static nn::Result PrepareToStartSystemApplet(u32 appId); // 0x00481414 | tier B
    static nn::Result StartLibraryApplet(u32 appId, const u8* pParameter, u32 size, nn::Handle handle); // 0x00481288 | tier B
    static nn::Result StartSystemApplet(u32 appId, const u8* pParameter, u32 size, nn::Handle handle); // 0x00481238 | tier B
    static nn::Result PrepareToCloseApplication(bool cancelPreload); // 0x00131C28 | tier B
    static nn::Result CloseApplication(const u8* pParameter, u32 size, nn::Handle handle); // 0x00131AE4 | tier B
    static nn::Result PrepareToJumpToHomeMenu(); // 0x004813A4 | nintendogs:callgraph [tier A]
    static nn::Result JumpToHomeMenu(const u8* pParameter, u32 size, nn::Handle handle); // 0x00481174 | tier B
    static nn::Result PrepareToDoApplicationJump(u8 flags, u64 titleId, u8 mediaType); // 0x00131C64 | tier B
    static nn::Result DoApplicationJump(const u8* pParameter, u32 size, const u8* pHmac, u32 hmacSize); // 0x00131BC4 | tier B
    static nn::Result CancelLibraryApplet(bool isExiting); // 0x0013735C | tier B
    static nn::Result ReplySleepQuery(u32 appId, nn::applet::CTR::QueryReply reply); // 0x00131AA0 | tier B
    static nn::Result ReplySleepNotificationComplete(u32 appId); // 0x00131CB0 | tier B
    static nn::Result SendCaptureBufferInfo(const u8* pInfo, u32 size); // 0x0048131C | tier B
    static nn::Result SleepSystem(u64 time); // 0x0012BC1C | tier B
    static nn::Result NotifyToWait(u32 appId); // 0x0012BC54 | tier B
    static nn::Result Wrap(void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize); // 0x00481484 | tier B
    static nn::Result Unwrap(void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize); // 0x0048154C | tier B
    static nn::Result Wrap1(void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize); // 0x004814E8 | tier B
    static nn::Result Unwrap1(void* pOutput, const void* pInput, u32 outputSize, u32 inputSize, u32 nonceOffset, u32 nonceSize); // 0x004815B0 | tier B
    static nn::Result AppletUtility(u32 utilityId, const u8* pInput, u32 inputSize, u8* pOutput, u32 outputSize, s32* pResult); // 0x001318E4 | tier B
    static nn::Result GetAppletProgramInfo(u32 appId, u32 flags, u16* pVersion); // 0x004812D8 | tier B
    static nn::Result SetApplicationCpuTimeLimit(u32 fixed, u32 percent); // 0x001205B4 | tier B
    static nn::Result SetScreenCapturePostPermission(u8 permission); // 0x0011E794 | tier B
    static nn::Result GetTargetPlatform(nn::ptm::CTR::TargetPlatform* pPlatform); // 0x00120574 | fefates:bytes [tier B]
    static nn::Result GetApplicationRunningMode(nn::applet::CTR::ApplicationRunningMode* pMode); // 0x004813D4 | fefates:bytes [tier B]
    static nn::Result IsStandardMemoryLayout(bool* pIsStandard); // 0x00481368 (name after 3dbrew)
};

// the session to the service (APT:A, APT:S or APT:U), open between LockAndConnect and
// DisconnectAndUnlock (0x0097E818)
extern nn::Handle s_Session;
} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
