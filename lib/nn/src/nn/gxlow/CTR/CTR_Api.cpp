// gxlow_Management.cpp (static initializer __sti___20_gxlow_Management_cpp at 0x0079B5C4: s_Lock,
// s_Lock2 and the handles) with the request functions of the command queue
#include "nn/gxlow/CTR/CTR_Api.h"
#include <new>
#include <string.h>
#include "nn/gxlow/CTR/detail/detail_Api.h"
#include "nn/gxlow/CTR/gxlow_Gpu.h"
#include "nn/gxlow/CTR/gxlow_InterruptReceiver.h"
#include "nn/os/CTR/CTR_Api.h"
#include "nn/os/CTR/MPCore/MPCore_Api.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/pl/CTR/CTR_Api.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace gxlow {
namespace CTR {
namespace {
// results (module 10; the names are ours)
const bit32 RESULT_NOT_INITIALIZED = 0x00202BF8; // success, nothing happened, 1016

// the GPU memory as the process sees it and its physical address
const uptr VRAM_ADDRESS = 0x1F000000;
const size_t VRAM_SIZE = 0x00600000;
const uptr VRAM_PHYSICAL_ADDRESS = 0x18000000;

// the shared font: mapped either at 0x18000000.. or ..0x30000000 and above
const uptr SHARED_FONT_HIGH_ADDRESS = 0x30000000;
const uptr SHARED_FONT_HIGH_OFFSET = 0x10000000;
const uptr SHARED_FONT_LOW_OFFSET = 0x0C000000;

// DisplayCaptureInfoScreen::format: the pixel format, stereo, and the bits of an active screen
const u32 CAPTURE_FORMAT_MASK = 0x07;
const u32 CAPTURE_FORMAT_STEREO = 0x20;
const u32 CAPTURE_FORMAT_ACTIVE_MASK = 0x70;
const s32 CAPTURE_FORMAT_NONE = -1;

const s32 SCREEN_COUNT = 2;

// 0x008B3F50
const nn::Handle CURRENT_PROCESS(nn::PSEUDO_HANDLE_CURRENT_PROCESS);
// 0x008B3F54
const nn::Handle INVALID_HANDLE;
// (0x008B3F58 and 0x008B3F5C are copies of CURRENT_PROCESS in other files of the original)

// the template of the requests (bytes 1 and 3 are 1)
// 0x008C23D0
const detail::CmdReq DEFAULT_CMD_REQ = {0, 1, 0, 1, {0, 0, 0, 0, 0, 0, 0}};

// 0x00AE95D4
nn::os::CriticalSection s_Lock((nn::os::CriticalSection::InitializeTag()));
// (not used in nn::gxlow)
// 0x00AE95E0
nn::os::CriticalSection s_Lock2((nn::os::CriticalSection::InitializeTag()));

// the memory of the InterruptReceiver (constructed in Initialize)
// 0x0094CC40
u64 s_InterruptReceiverStorage[sizeof(nn::gxlow::CTR::InterruptReceiver) / sizeof(u64)];

inline nn::Result Enqueue(const detail::CmdReq& request)
{
    return detail::GetInterruptReceiver()->m_CmdReqQueue.TryEnqueue(&request);
}

inline bit32 MakeSize(u16 width, u16 height)
{
    return width | (height << 16);
}
} // namespace

namespace detail {
// The globals of gxlow_Management.cpp (external: nothing in ACNL writes some of them, but the
// original reads them every time)
// what CmdReq::flags of the requests is (except cache flushes)
// 0x0097FA58
u8 s_CmdReqFlags = 1;
// 0x0097FA5C
const detail::CmdReq* s_pDefaultCmdReq = &DEFAULT_CMD_REQ;

// 0x0097E8C0
bool s_IsInitialized;
// 0x0097E8C1
bool s_IsAppletMode;
// 0x0097E8C2
bool s_IsFatalErrMode;
// 0x0097E8C3
bool s_IsSrvInitialized;
// 0x0097E8C4
const char* s_GpuServiceName = "gsp::Gpu";
// (not used in nn::gxlow)
// 0x0097E8C8
const char* s_LcdServiceName = "gsp::Lcd";
// 0x0097E8CC
u32 s_NumSpeculativeRequests = 3;
// 0x0097E8D0
nn::Handle s_Session;
// 0x0097E8D4
nn::gxlow::CTR::Gpu s_Gpu;
// (not used in nn::gxlow)
// 0x0097E8D8
nn::Handle s_LcdSession;
// 0x0097E8DC
nn::gxlow::CTR::InterruptReceiver* s_pInterruptReceiver;

// 0x0012ACB4 | nintendogs:callgraph [tier A]
bool IsAppletMode()
{
    return s_IsAppletMode;
}

// 0x00131194 | nintendogs:callgraph [tier A]
bool IsInitialized()
{
    return s_IsInitialized;
}

// 0x001372BC | nintendogs:callgraph [tier C]
bool IsFatalErrMode()
{
    return s_IsFatalErrMode;
}

// 0x001372CC | nintendogs:callgraph [tier A]
nn::gxlow::CTR::InterruptReceiver* GetInterruptReceiver()
{
    return s_pInterruptReceiver;
}

// 0x001372DC | nintendogs:callgraph [tier A]
nn::gxlow::CTR::Gpu* GetGpuIpc()
{
    return &s_Gpu;
}
} // namespace detail

using namespace detail;

// 0x001200F8 | fefates:callgraph [tier C]
void SetAppletMode()
{
    s_IsAppletMode = true;
}

// 0x001247D8 | nintendogs:bytes [tier A]
void StartLcdDisplay()
{
    if (s_IsInitialized) {
        s_Gpu.SetLcdForceBlack(false);
    }
}

// 0x00128428 (name is ours)
s64 GetSystemTick()
{
    return nn::svc::GetSystemTick();
}

// 0x0012A9E0 | mk7dlp:callseq [tier A]
void Initialize()
{
    s_Lock.Enter();
    if (s_IsInitialized) {
        s_Lock.Exit();
        return;
    }
    s_IsInitialized = true;
    if (!s_IsSrvInitialized) {
        nn::srv::Initialize();
        s_IsSrvInitialized = true;
    }
    nn::srv::GetServiceHandle(&s_Session, s_GpuServiceName, strlen(s_GpuServiceName), 0);
    s_Gpu.m_Session = s_Session;
    s_Gpu.AcquireRight(CURRENT_PROCESS, s_IsFatalErrMode);
    s_pInterruptReceiver = new (s_InterruptReceiverStorage) InterruptReceiver;
    s_pInterruptReceiver->Initialize();
    s_Lock.Exit();
}

// 0x0012AAE8 (name is ours, after GSPGPU_WriteHWRegs)
nn::Result WriteHWRegs(unsigned regAddress, const void* data, unsigned size)
{
    return detail::GetGpuIpc()->WriteHWRegs(regAddress, static_cast<const u8*>(data), size);
}

// 0x0012AB14 (name is ours)
void YieldThread()
{
    s_pInterruptReceiver->WaitAnyHandlerDone();
}

// 0x0012AB24 | nintendogs:bytes [tier A]
nn::Result AcquireGpuRight()
{
    if (!detail::IsInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    nn::Result result = detail::GetGpuIpc()->AcquireRight(CURRENT_PROCESS, false);
    if (result.IsSuccess() && detail::IsAppletMode()) {
        detail::GetInterruptReceiver()->m_InterruptQueue.SuppressPdcEvents(false);
    }
    return result;
}

// 0x0012AB8C | nintendogs:bytes [tier A]
nn::Result ReleaseGpuRight()
{
    if (!detail::IsInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    nn::Result result = detail::GetGpuIpc()->ReleaseRight();
    if (result.IsSuccess() && detail::IsAppletMode()) {
        detail::GetInterruptReceiver()->m_InterruptQueue.SuppressPdcEvents(true);
    }
    return result;
}

// 0x0012ABE0 | nintendogs:callgraph [tier A]
nn::Result RestoreVramSysArea()
{
    if (!detail::IsInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    return detail::GetGpuIpc()->RestoreVramSysArea();
}

// 0x0012AC0C | nintendogs:bytes [tier A]
nn::Result WriteHWRegsWithMask(unsigned regAddress, const void* data, const void* mask, unsigned size)
{
    return detail::GetGpuIpc()->WriteHWRegsWithMask(regAddress, static_cast<const u8*>(data), static_cast<const u8*>(mask), size);
}

// 0x0012AC40 (name is ours)
bool IsFirstInitialization()
{
    return s_pInterruptReceiver->m_IsFirstInitialization;
}

// 0x0012AC58 | nintendogs:bytes [tier A]
void (*RegisterInterruptHandler(void (*handler)(), nngxlowInterrupt interrupt))()
{
    InterruptReceiver* receiver = s_pInterruptReceiver;
    if (interrupt >= NN_GXLOW_INTERRUPT_MAX) {
        return 0;
    }
    receiver->m_Lock.Enter();
    InterruptReceiver::Handler old = receiver->m_Handlers[interrupt];
    receiver->m_Handlers[interrupt] = handler;
    receiver->m_Lock.Exit();
    return old;
}

// 0x0012ACA4 (name is ours)
u32 GetNumSpeculativeRequests()
{
    return s_NumSpeculativeRequests;
}

// 0x001311A4 | fefates:bytes [tier B]
void StopLcdDisplay()
{
    if (s_IsInitialized) {
        s_Gpu.SetLcdForceBlack(true);
        return;
    }
    // without the library: a session just for this
    s_Lock.Enter();
    nn::srv::GetServiceHandle(&s_Session, s_GpuServiceName, strlen(s_GpuServiceName), 0);
    s_Gpu.m_Session = s_Session;
    s_Gpu.SetLcdForceBlack(true);
    nn::svc::CloseHandle(s_Session);
    s_Gpu.m_Session = INVALID_HANDLE;
    s_Lock.Exit();
}

// 0x00131628 (name is ours)
void Lock()
{
    s_Lock.Enter();
}

// 0x00131638 (name is ours)
void Unlock()
{
    s_Lock.Exit();
}

// 0x00136D9C | nintendogs:bytes [tier A]
void RequestDma(void* pDst, const void* pSrc, unsigned size, bool isFlushed, bool isOnlyToVram)
{
    if (isOnlyToVram && !(reinterpret_cast<uptr>(pDst) - VRAM_ADDRESS < VRAM_SIZE)) {
        return;
    }
    detail::CmdReq request = *s_pDefaultCmdReq;
    request.id = detail::CMD_REQ_REQUEST_DMA;
    request.flags = s_CmdReqFlags;
    request.params[0] = reinterpret_cast<uptr>(pSrc);
    request.params[1] = reinterpret_cast<uptr>(pDst);
    request.params[2] = size;
    request.params[6] = isFlushed;
    Enqueue(request);
}

// 0x00136E7C | nintendogs:bytes [tier A]
nn::Result SetMemoryFill(void* start0, void* end0, unsigned value0, unsigned control0, void* start1, void* end1, unsigned value1, unsigned control1)
{
    detail::CmdReq request = *s_pDefaultCmdReq;
    request.id = detail::CMD_REQ_MEMORY_FILL;
    request.flags = s_CmdReqFlags;
    request.params[0] = reinterpret_cast<uptr>(start0);
    request.params[1] = value0;
    request.params[2] = reinterpret_cast<uptr>(end0);
    request.params[3] = reinterpret_cast<uptr>(start1);
    request.params[4] = value1;
    request.params[5] = reinterpret_cast<uptr>(end1);
    reinterpret_cast<u16*>(&request.params[6])[0] = control0;
    reinterpret_cast<u16*>(&request.params[6])[1] = control1;
    return Enqueue(request);
}

// 0x00136F24 | nintendogs:bytes [tier A]
nn::Result SetCommandlist(void* buffer, unsigned size, bool isFlushed, bool updatesGasAdditiveBlend)
{
    nn::Result result;
    if (isFlushed) {
        detail::CmdReq request = *s_pDefaultCmdReq;
        request.id = detail::CMD_REQ_FLUSH_CACHE_REGIONS;
        request.flags = 0;
        request.params[0] = reinterpret_cast<uptr>(buffer);
        request.params[1] = size;
        result = Enqueue(request);
    }
    if (result.IsFailure()) {
        return result;
    }
    detail::CmdReq request = *s_pDefaultCmdReq;
    request.id = detail::CMD_REQ_SET_COMMAND_LIST;
    request.flags = s_CmdReqFlags;
    request.params[0] = reinterpret_cast<uptr>(buffer);
    request.params[1] = size;
    request.params[2] = updatesGasAdditiveBlend;
    request.params[6] = 0;
    return Enqueue(request);
}

// 0x00136FF0 | nintendogs:bytes [tier A]
nn::Result SetTextureCopy(void* src, void* dst, unsigned size, unsigned short srcLineWidth, unsigned short srcGap, unsigned short dstLineWidth,
                          unsigned short dstGap, unsigned flags)
{
    detail::CmdReq request = *s_pDefaultCmdReq;
    request.id = detail::CMD_REQ_TEXTURE_COPY;
    request.flags = s_CmdReqFlags;
    request.params[0] = reinterpret_cast<uptr>(src);
    request.params[1] = reinterpret_cast<uptr>(dst);
    request.params[2] = size;
    request.params[3] = MakeSize(srcLineWidth, srcGap);
    request.params[4] = MakeSize(dstLineWidth, dstGap);
    request.params[5] = flags;
    return Enqueue(request);
}

// 0x001371AC | nintendogs:bytes [tier A]
nn::Result SetDisplayTransfer(void* src, unsigned short srcWidth, unsigned short srcHeight, void* dst, unsigned short dstWidth, unsigned short dstHeight,
                              unsigned flags)
{
    detail::CmdReq request = *s_pDefaultCmdReq;
    request.id = detail::CMD_REQ_DISPLAY_TRANSFER;
    request.flags = s_CmdReqFlags;
    request.params[0] = reinterpret_cast<uptr>(src);
    request.params[1] = reinterpret_cast<uptr>(dst);
    request.params[2] = MakeSize(srcWidth, srcHeight);
    request.params[3] = MakeSize(dstWidth, dstHeight);
    request.params[4] = flags;
    return Enqueue(request);
}

// 0x00140C54 | fefates:bytes [tier B]
uptr GetPhysicalAddr(unsigned int address)
{
    uptr physical = nn::os::CTR::MPCore::ConvertAddressForDevice(address, 1);
    if (physical != 0) {
        return physical;
    }
    if (address - VRAM_ADDRESS <= VRAM_SIZE) {
        return address - VRAM_ADDRESS + VRAM_PHYSICAL_ADDRESS;
    }
    if (nn::os::CTR::IsWramEnabled()) {
        physical = nn::os::CTR::MPCore::ConvertAddressForWram(address, 1);
        if (physical != 0) {
            return physical;
        }
    }
    uptr font = reinterpret_cast<uptr>(nn::pl::CTR::GetSharedFontAddress());
    uptr fontEnd = font + nn::pl::CTR::GetSharedFontSize();
    if (font != 0 && font <= address && address < fontEnd) {
        if (font >= SHARED_FONT_HIGH_ADDRESS) {
            return address - SHARED_FONT_HIGH_OFFSET;
        }
        return address + SHARED_FONT_LOW_OFFSET;
    }
    return 0;
}

// 0x0014372C (name is ours, after the command)
nn::Result StoreDataCache(unsigned address, unsigned size)
{
    return detail::GetGpuIpc()->StoreDataCache(CURRENT_PROCESS, address, size);
}

// 0x0047F700 | fefates:bytes [tier B]
bool IsSwapPending(int screen)
{
    FramebufferInfoQueue* info = detail::GetInterruptReceiver()->m_pFramebufferInfo[screen];
    return reinterpret_cast<volatile bool*>(&info->header)[1];
}

// 0x0047F71C | fefates:bytes [tier B]
void SetBufferSwap(int screen, int activeFramebuffer, void* left, void* right, unsigned int stride, unsigned int format, unsigned int select)
{
    InterruptReceiver* receiver = detail::GetInterruptReceiver();
    if (static_cast<u32>(screen) >= SCREEN_COUNT) {
        return;
    }
    FramebufferInfoQueue* info = receiver->m_pFramebufferInfo[screen];
    u8 index = 1 - *reinterpret_cast<volatile u8*>(&info->header);
    FramebufferInfo& entry = info->entries[index];
    entry.activeFramebuffer = activeFramebuffer;
    entry.attribute = 0;
    entry.leftAddress = left;
    entry.rightAddress = right;
    entry.stride = stride;
    entry.format = format;
    entry.select = select;
    nn::os::detail::DataSynchronizationBarrier();
    // the new entry, marked as updated
    s32 header;
    do {
        header = nn::os::detail::LoadExclusive(&receiver->m_pFramebufferInfo[screen]->header);
        header = (header & ~0xFFFF) | index | (1 << 8);
    } while (nn::os::detail::StoreExclusive(&receiver->m_pFramebufferInfo[screen]->header, header));
}

// 0x0047F7B0 (name is ours, after the command)
nn::Result SaveVramSysArea()
{
    if (!detail::IsInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    return detail::GetGpuIpc()->SaveVramSysArea();
}

// 0x0047F7D8 | nintendogs:bytes [tier A]
void ImportDisplayCaptureInfo(nn::gxlow::CTR::DisplayCaptureInfo* pInfo)
{
    detail::GetGpuIpc()->ImportDisplayCaptureInfo(pInfo);
    s32 format = pInfo->top.format;
    if ((format & CAPTURE_FORMAT_ACTIVE_MASK) == 0) {
        pInfo->top.format = CAPTURE_FORMAT_NONE;
    } else {
        // without stereo both eyes show the left framebuffer
        if ((format & CAPTURE_FORMAT_STEREO) == 0) {
            pInfo->top.rightFramebuffer = pInfo->top.leftFramebuffer;
        }
        pInfo->top.format = format & CAPTURE_FORMAT_MASK;
    }
    pInfo->bottom.format &= CAPTURE_FORMAT_MASK;
    if (pInfo->top.leftFramebuffer == 0) {
        pInfo->top.format = CAPTURE_FORMAT_NONE;
    }
    if (pInfo->top.rightFramebuffer == 0) {
        pInfo->top.format = CAPTURE_FORMAT_NONE;
    }
    if (pInfo->bottom.leftFramebuffer == 0) {
        pInfo->bottom.format = CAPTURE_FORMAT_NONE;
    }
    pInfo->bottom.rightFramebuffer = pInfo->bottom.leftFramebuffer;
}

} // namespace CTR
} // namespace gxlow
} // namespace nn

// 0x00128424
s64 nngxlowGetSystemTick()
{
    return nn::gxlow::CTR::GetSystemTick();
}

// 0x0012AAE4
nn::Result GSPGPU_WriteHWRegs(unsigned regAddress, const void* data, unsigned size)
{
    return nn::gxlow::CTR::WriteHWRegs(regAddress, data, size);
}

// 0x0012AB10
void nngxlowYieldThread()
{
    nn::gxlow::CTR::YieldThread();
}

// 0x0012AC08
nn::Result nngxlowWriteHWRegsWithMask(unsigned regAddress, const void* data, const void* mask, unsigned size)
{
    return nn::gxlow::CTR::WriteHWRegsWithMask(regAddress, data, mask, size);
}

// 0x0012AC3C
bool nngxlowIsFirstInitialization()
{
    return nn::gxlow::CTR::IsFirstInitialization();
}

// 0x0012AC54
void (*nngxlowRegisterInterruptHandler(void (*handler)(), nngxlowInterrupt interrupt))()
{
    return nn::gxlow::CTR::RegisterInterruptHandler(handler, interrupt);
}

// 0x0012ACA0
u32 nngxlowGetNumSpeculativeRequests()
{
    return nn::gxlow::CTR::GetNumSpeculativeRequests();
}

// 0x00131624
void nngxlowLock()
{
    nn::gxlow::CTR::Lock();
}

// 0x00131634
void nngxlowUnlock()
{
    nn::gxlow::CTR::Unlock();
}

// 0x00136EEC
nn::Result nngxlowFlushDataCache(unsigned address, unsigned size)
{
    return GSPGPU_FlushDataCache(address, size);
}

// 0x00136EF0
nn::Result GSPGPU_FlushDataCache(unsigned address, unsigned size)
{
    return nn::gxlow::CTR::detail::GetGpuIpc()->FlushDataCache(nn::gxlow::CTR::CURRENT_PROCESS, address, size);
}

// 0x00140C50
uptr nngxlowGetPhysicalAddr(unsigned int address)
{
    return nn::gxlow::CTR::GetPhysicalAddr(address);
}

// 0x00143B68
uptr nngxGetPhysicalAddr(unsigned int address)
{
    return nn::gxlow::CTR::GetPhysicalAddr(address);
}

// 0x0047F6FC
bool nngxlowIsSwapPending(int screen)
{
    return nn::gxlow::CTR::IsSwapPending(screen);
}

// 0x007B2AE4
void nngxlowSetBufferSwap(int screen, int activeFramebuffer, void* left, void* right, unsigned int stride, unsigned int format, unsigned int select)
{
    nn::gxlow::CTR::SetBufferSwap(screen, activeFramebuffer, left, right, stride, format, select);
}
