#pragma once

#include "decomp.h"
#include "nn/Result.h"

// the interrupts of the GPU module (3dbrew "GSP Shared Memory"; a C enum, the name is from the
// symbols, the values after 3dbrew)
enum nngxlowInterrupt : u8
{
    NN_GXLOW_INTERRUPT_PSC0 = 0,
    NN_GXLOW_INTERRUPT_PSC1 = 1,
    NN_GXLOW_INTERRUPT_VBLANK_TOP = 2,
    NN_GXLOW_INTERRUPT_VBLANK_BOTTOM = 3,
    NN_GXLOW_INTERRUPT_PPF = 4,
    NN_GXLOW_INTERRUPT_P3D = 5,
    NN_GXLOW_INTERRUPT_DMA = 6,
    NN_GXLOW_INTERRUPT_MAX = 7,
};

namespace nn {
namespace gxlow {
namespace CTR {
// the framebuffers of one screen (3dbrew "GSPGPU:ImportDisplayCaptureInfo"; names are ours)
struct DisplayCaptureInfoScreen
{
    void* leftFramebuffer;   // 0x0
    void* rightFramebuffer;  // 0x4, the top screen only
    s32 format;              // 0x8, -1 for none
    u32 stride;              // 0xC
};

struct DisplayCaptureInfo
{
    DisplayCaptureInfoScreen top;    // 0x00
    DisplayCaptureInfoScreen bottom; // 0x10
};
ASSERT_SIZE(DisplayCaptureInfo, 0x20);

// a framebuffer of a screen in the shared memory (3dbrew "GSP Shared Memory"; names are ours)
struct FramebufferInfo
{
    u32 activeFramebuffer; // 0x00
    void* leftAddress;     // 0x04
    void* rightAddress;    // 0x08
    u32 stride;            // 0x0C
    u32 format;            // 0x10
    u32 select;            // 0x14
    u32 attribute;         // 0x18
};
ASSERT_SIZE(FramebufferInfo, 0x1C);

// the two framebuffer infos of a screen; the module shows the one at index when isUpdated
struct FramebufferInfoQueue
{
    volatile s32 header;        // byte 0 index, byte 1 isUpdated
    FramebufferInfo entries[2]; // 0x04
};

void SetAppletMode(); // 0x001200F8 | nintendogs:callgraph [tier C]
void Initialize(); // 0x0012A9E0 | mk7dlp:callseq [tier A]
void StartLcdDisplay(); // 0x001247D8 | nintendogs:bytes [tier A]
void StopLcdDisplay(); // 0x001311A4 | fefates:bytes [tier B]
nn::Result AcquireGpuRight(); // 0x0012AB24 | nintendogs:bytes [tier A]
nn::Result ReleaseGpuRight(); // 0x0012AB8C | nintendogs:bytes [tier A]
nn::Result SaveVramSysArea(); // 0x0047F7B0 (name is ours, after the command)
nn::Result RestoreVramSysArea(); // 0x0012ABE0 | nintendogs:callgraph [tier A]
nn::Result WriteHWRegs(unsigned regAddress, const void* data, unsigned size); // 0x0012AAE8 (name is ours, after GSPGPU_WriteHWRegs)
nn::Result WriteHWRegsWithMask(unsigned regAddress, const void* data, const void* mask, unsigned size); // 0x0012AC0C | nintendogs:bytes [tier A]
// the old handler
void (*RegisterInterruptHandler(void (*handler)(), nngxlowInterrupt interrupt))(); // 0x0012AC58 | nintendogs:bytes [tier A]

// the requests of the command queue
void RequestDma(void* pDst, const void* pSrc, unsigned size, bool isFlushed, bool isOnlyToVram); // 0x00136D9C | nintendogs:bytes [tier A]
nn::Result SetMemoryFill(void* start0, void* end0, unsigned value0, unsigned control0, void* start1, void* end1, unsigned value1, unsigned control1); // 0x00136E7C | nintendogs:bytes [tier A]
nn::Result SetCommandlist(void* buffer, unsigned size, bool isFlushed, bool updatesGasAdditiveBlend); // 0x00136F24 | nintendogs:bytes [tier A]
nn::Result SetTextureCopy(void* src, void* dst, unsigned size, unsigned short srcLineWidth, unsigned short srcGap, unsigned short dstLineWidth, unsigned short dstGap, unsigned flags); // 0x00136FF0 | nintendogs:bytes [tier A]
nn::Result SetDisplayTransfer(void* src, unsigned short srcWidth, unsigned short srcHeight, void* dst, unsigned short dstWidth, unsigned short dstHeight, unsigned flags); // 0x001371AC | nintendogs:bytes [tier A]

nn::Result StoreDataCache(unsigned address, unsigned size); // 0x0014372C (name is ours, after the command)
uptr GetPhysicalAddr(unsigned int address); // 0x00140C54 | fefates:bytes [tier B]
bool IsSwapPending(int screen); // 0x0047F700 | fefates:bytes [tier B]
void SetBufferSwap(int screen, int activeFramebuffer, void* left, void* right, unsigned int stride, unsigned int format, unsigned int select); // 0x0047F71C | fefates:bytes [tier B]
void ImportDisplayCaptureInfo(nn::gxlow::CTR::DisplayCaptureInfo* pInfo); // 0x0047F7D8 | nintendogs:bytes [tier A]

// the functions behind the C functions of the same names (names are ours)
s64 GetSystemTick(); // 0x00128428 (name is ours)
void YieldThread(); // 0x0012AB14 (name is ours)
bool IsFirstInitialization(); // 0x0012AC40 (name is ours)
u32 GetNumSpeculativeRequests(); // 0x0012ACA4 (name is ours)
void Lock(); // 0x00131628 (name is ours)
void Unlock(); // 0x00131638 (name is ours)
} // namespace CTR
} // namespace gxlow
} // namespace nn

// the C interface (names from the symbols; each one falls into the function above)
extern "C" {
s64 nngxlowGetSystemTick(); // 0x00128424
void nngxlowYieldThread(); // 0x0012AB10
nn::Result GSPGPU_WriteHWRegs(unsigned regAddress, const void* data, unsigned size); // 0x0012AAE4
nn::Result nngxlowWriteHWRegsWithMask(unsigned regAddress, const void* data, const void* mask, unsigned size); // 0x0012AC08
bool nngxlowIsFirstInitialization(); // 0x0012AC3C
void (*nngxlowRegisterInterruptHandler(void (*handler)(), nngxlowInterrupt interrupt))(); // 0x0012AC54
u32 nngxlowGetNumSpeculativeRequests(); // 0x0012ACA0
void nngxlowLock(); // 0x00131624
void nngxlowUnlock(); // 0x00131634
nn::Result nngxlowFlushDataCache(unsigned address, unsigned size); // 0x00136EEC
nn::Result GSPGPU_FlushDataCache(unsigned address, unsigned size); // 0x00136EF0
uptr nngxlowGetPhysicalAddr(unsigned int address); // 0x00140C50
uptr nngxGetPhysicalAddr(unsigned int address); // 0x00143B68
// (gx library, not decompiled yet; declared for gr)
void nngxSplitDrawCmdlist(); // 0x001280E0
void nngxAddMemoryFillCommand(unsigned int address0, unsigned int size0, unsigned int value0, unsigned int width0,
                              unsigned int address1, unsigned int size1, unsigned int value1, unsigned int width1); // 0x007B175C
bool nngxlowIsSwapPending(int screen); // 0x0047F6FC
void nngxlowSetBufferSwap(int screen, int activeFramebuffer, void* left, void* right, unsigned int stride, unsigned int format, unsigned int select); // 0x007B2AE4
}
