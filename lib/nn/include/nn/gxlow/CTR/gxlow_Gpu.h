#pragma once

#include "decomp.h"

namespace nn {
namespace gxlow {
namespace CTR {
class Gpu
{
public:
    void WriteHWRegs(unsigned, const unsigned char*, unsigned); // 0x001314B0 | nintendogs:bytes [tier A]
    void AcquireRight(nn::Handle, bool); // 0x001314F8 | nintendogs:bytes [tier A]
    void ReleaseRight(); // 0x0013153C | nintendogs:bytes [tier A]
    void SetLcdForceBlack(bool); // 0x00131564 | nintendogs:bytes [tier A]
    void WriteHWRegsWithMask(unsigned, const unsigned char*, const unsigned char*, unsigned); // 0x001315C4 | nintendogs:bytes [tier A]
    void ReadHWRegs(unsigned, unsigned char*, unsigned); // 0x00137218 | nintendogs:bytes [tier A]
    void RegisterInterruptRelayQueue(nn::Handle, unsigned, nn::Handle*, int*); // 0x0013726C | nintendogs:bytes [tier A]
    void FlushDataCache(nn::Handle, unsigned, unsigned); // 0x0013B6B8 | nintendogs:bytes [tier A]
    void TriggerCmdReqQueue(); // 0x0013EE58 | nintendogs:bytes [tier A]
    void StoreDataCache(nn::Handle, unsigned int, unsigned int); // 0x00144C58 | fefates:bytes [tier B]
    void ImportDisplayCaptureInfo(nn::gxlow::CTR::DisplayCaptureInfo*); // 0x0047F874 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace gxlow
} // namespace nn
