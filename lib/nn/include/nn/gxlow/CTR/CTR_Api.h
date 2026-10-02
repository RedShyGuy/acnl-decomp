#pragma once

#include "decomp.h"

namespace nn {
namespace gxlow {
namespace CTR {
void StartLcdDisplay(); // 0x001247D8 | nintendogs:bytes [tier A]
void Initialize(); // 0x0012A9DC | mk7dlp:callseq [tier A]
void AcquireGpuRight(); // 0x0012AB24 | nintendogs:bytes [tier A]
void ReleaseGpuRight(); // 0x0012AB8C | nintendogs:bytes [tier A]
void RestoreVramSysArea(); // 0x0012ABE0 | nintendogs:callgraph [tier A]
void WriteHWRegsWithMask(unsigned, const void*, const void*, unsigned); // 0x0012AC0C | nintendogs:bytes [tier A]
void RegisterInterruptHandler(void(*)(), nngxlowInterrupt); // 0x0012AC58 | nintendogs:bytes [tier A]
void StopLcdDisplay(); // 0x001311A4 | fefates:bytes [tier B]
void RequestDma(void*, const void*, unsigned, bool, bool); // 0x00136D9C | nintendogs:bytes [tier A]
void SetMemoryFill(void*, void*, unsigned, unsigned, void*, void*, unsigned, unsigned); // 0x00136E7C | nintendogs:bytes [tier A]
void SetCommandlist(void*, unsigned, bool, bool); // 0x00136F24 | nintendogs:bytes [tier A]
void SetTextureCopy(void*, void*, unsigned, unsigned short, unsigned short, unsigned short, unsigned short, unsigned); // 0x00136FF0 | nintendogs:bytes [tier A]
void SetDisplayTransfer(void*, unsigned short, unsigned short, void*, unsigned short, unsigned short, unsigned); // 0x001371AC | nintendogs:bytes [tier A]
void GetPhysicalAddr(unsigned int); // 0x00140C54 | fefates:bytes [tier B]
void IsSwapPending(int); // 0x0047F700 | fefates:bytes [tier B]
void SetBufferSwap(int, int, void*, void*, unsigned int, unsigned int, unsigned int); // 0x0047F71C | fefates:bytes [tier B]
void ImportDisplayCaptureInfo(nn::gxlow::CTR::DisplayCaptureInfo*); // 0x0047F7D8 | nintendogs:bytes [tier A]
} // namespace CTR
} // namespace gxlow
} // namespace nn
