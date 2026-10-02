#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/applet/CTR/applet_Types.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/ptm/CTR/ptm_Types.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
void InitializeConnect(unsigned int, unsigned int, int); // 0x0011E5C0 | fefates:bytes [tier B]
void Enable(bool); // 0x0011E7D0 | nintendogs:bytes [tier A]
void Disconnect(); // 0x00120264 | nintendogs:bytes [tier A]
void SetInitialParamSize(int); // 0x001202C0 | nintendogs:callgraph [tier A]
void SetInitialParamValid(); // 0x001202D0 | nintendogs:callgraph [tier A]
void GetInitialParamBuffer(); // 0x001202E4 | nintendogs:callgraph [tier A]
void SetInitialWakeupState(nn::applet::CTR::WakeupState); // 0x001202F0 | nintendogs:callgraph [tier A]
void InitializeClientThread(int, nn::Handle, nn::Handle); // 0x00120300 | nintendogs:callseq [tier A]
void SetInitialParamSenderId(unsigned); // 0x001204B8 | nintendogs:callgraph [tier A]
// LockAndConnect, the APPLET command, DisconnectAndUnlock
nn::Result GetTargetPlatform(nn::ptm::CTR::TargetPlatform* platform); // 0x0011E59C | tier C
nn::Result GetApplicationRunningMode(nn::applet::CTR::ApplicationRunningMode* mode); // 0x00480DBC (name is ours)
void Connect(); // 0x00120624 | fefates:bytes [tier B]
void LockTransition(unsigned, bool); // 0x001250DC | nintendogs:bytes [tier A]
void UnlockTransition(unsigned); // 0x00125110 | nintendogs:bytes [tier A]
void RestartApplication(const void*, unsigned int); // 0x0012513C | fefates:bytes [tier B]
void RestoreVramSysArea(); // 0x00125188 | nintendogs:bytes [tier A]
void SetReceiveCallback(bool(*)(unsigned), unsigned); // 0x001251B0 | nintendogs:callgraph [tier A]
void SleepIfShellClosed(); // 0x001251DC | nintendogs:bytes [tier A]
void Receive(unsigned int*, unsigned int*, unsigned char*, unsigned int, int*, nn::Handle*, nn::fnd::TimeSpan); // 0x0012523C | fefates:bytes [tier B]
void IsActive(); // 0x00125500 | nintendogs:callgraph [tier A]
void TryReceive(unsigned*, unsigned*, unsigned char*, unsigned, int*, nn::Handle*, bool); // 0x0012ADC0 | nintendogs:bytes-fuzzy [tier A]
void CallUtility(unsigned, const unsigned char*, unsigned, unsigned char*, unsigned, int*); // 0x0012AEF4 | nintendogs:bytes [tier A]
void AssignDspRight(bool); // 0x0012AFD4 | nintendogs:bytes [tier A]
void AssignGpuRight(bool); // 0x0012B024 | nintendogs:bytes [tier A]
void LockAndConnect(); // 0x0012B09C | nintendogs:callgraph [tier A]
void CancelParameter(bool, unsigned, bool, unsigned); // 0x0012B0D0 | nintendogs:bytes [tier A]
void WaitForStarting(unsigned*, unsigned char*, unsigned, int*, nn::Handle*, nn::fnd::TimeSpan); // 0x0012B124 | nintendogs:bytes-fuzzy [tier A]
void DoApplicationJump(const unsigned char*, unsigned int, const unsigned char*, unsigned int); // 0x0012B58C | fefates:bytes [tier B]
void NotifyToWait(); // 0x0012B6D8 | nintendogs:bytes [tier A]
void DisconnectAndUnlock(); // 0x0012B6FC | nintendogs:bytes [tier A]
void CloseApplication(const unsigned char*, unsigned int, nn::Handle); // 0x0012B76C | fefates:bytes [tier B]
void CloseApplicationCore(const unsigned char*, unsigned int, nn::Handle); // 0x0012B7A8 | fefates:bytes [tier B]
void PrepareToJumpApplication(nn::applet::CTR::AppJumpType, unsigned long long, nn::fs::MediaType); // 0x0012B908 | fefates:bytes [tier B]
void ReplySleepQueryToManager(nn::applet::CTR::QueryReply); // 0x0012B9A4 | nintendogs:bytes [tier A]
void ReplySleepNotificationCompleteToManager(); // 0x0012BA04 | nintendogs:bytes [tier A]
void Send(unsigned, unsigned, const unsigned char*, unsigned, nn::Handle, nn::fnd::TimeSpan); // 0x0012BA28 | nintendogs:callseq-callee [tier A]
void Glance(unsigned*, unsigned*, unsigned char*, unsigned, int*, nn::Handle*); // 0x0012BCD0 | nintendogs:bytes [tier A]
void SetPortName(const char*); // 0x00131694 | fefates:bytes [tier B]
void FinalizeClientThread(); // 0x001316C4 | fefates:bytes [tier B]
void CancelLibraryAppletIfRegistered(bool, nn::applet::CTR::WakeupState*); // 0x00131770 | nintendogs:bytes [tier A]
void CaptureScreen(unsigned); // 0x0047FEA0 | nintendogs:callseq [tier A]
void GetDisplayInfo(nn::applet::CTR::AppletDisplayInfo*); // 0x0047FFD4 | nintendogs:bytes [tier A]
void ConvertL16ToB16(unsigned*, const unsigned*, unsigned, const nn::applet::CTR::detail::OffsetTable&); // 0x00480198 | nintendogs:bytes [tier A]
void ConvertL24ToB24(unsigned*, const unsigned*, unsigned, const nn::applet::CTR::detail::OffsetTable&); // 0x00480208 | nintendogs:bytes [tier A]
void WaitForRegister(unsigned, nn::fnd::TimeSpan); // 0x004802E8 | nintendogs:callseq-callee [tier A]
void GetAppletManInfo(nn::applet::CTR::AppletPos, nn::applet::CTR::AppletPos*, unsigned*, unsigned*, unsigned*); // 0x0048046C | nintendogs:bytes [tier A]
void StartLibraryApplet(unsigned, const unsigned char*, unsigned, nn::Handle); // 0x00480710 | nintendogs:callgraph [tier A]
void WaitToCaptureScreen(unsigned, nn::Handle*); // 0x0048089C | nintendogs:callseq-callee [tier A]
void CalcCaptureBufferInfo(nn::applet::CTR::CaptureBufferInfo*); // 0x00480998 | nintendogs:callseq-callee [tier A]
void SendCaptureBufferInfo(const unsigned char*, unsigned); // 0x00480ACC | nintendogs:callseq [tier A]
void PrepareToJumpToHomeMenu(); // 0x00480AF8 | nintendogs:bytes [tier A]
void CaptureDisplayBuffer(unsigned, const nn::applet::CTR::AppletDisplayInfo*, const nn::applet::CTR::CaptureBufferInfo*); // 0x00480B84 | nintendogs:bytes [tier A]
void CaptureDisplayBufferCore(unsigned, const nn::applet::CTR::AppletDisplayInfo*, bool, bool); // 0x00480C18 | nintendogs:callgraph [tier A]
void AttachTransferMemoryHandle(nn::os::TransferMemoryBlock*, nn::Handle, unsigned, unsigned); // 0x00480DE0 | nintendogs:bytes [tier A]
void PrepareToStartSystemApplet(unsigned); // 0x00480DF4 | nintendogs:bytes [tier A]
void PrepareToStartLibraryApplet(unsigned); // 0x00480F00 | nintendogs:bytes [tier A]
void CaptureScreenForSystemApplet(unsigned); // 0x00480FEC | nintendogs:callseq [tier A]
} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
